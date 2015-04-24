/*
** This source code is "Not a Contribution" under Apache license
**
** Sierra Wireless RIL
**
** Based on reference-ril by The Android Open Source Project
** and U300 RIL by ST-Ericsson.
** Modified by Sierra Wireless, Inc.
**
** Copyright (C) 2011 Sierra Wireless, Inc.
** Copyright (C) ST-Ericsson AB 2008-2009
** Copyright 2006, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
**
** Based on reference-ril by The Android Open Source Project.
**
** Modified for ST-Ericsson U300 modems.
** Author: Christian Bejram <christian.bejram@stericsson.com>
*/

#include <telephony/ril.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <termios.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include <poll.h>

#include "at_channel.h"
#include "at_tok.h"
#include "at_misc.h"
#include "swiril_main.h"

#define LOG_TAG "AT"
#include "swiril_log.h"

#ifdef HAVE_ANDROID_OS
/* For IOCTL's */
#include <linux/omap_csmi.h>
#endif /*HAVE_ANDROID_OS*/

#ifdef HAVE_ANDROID_OS
#define USE_NP 1
#endif /* HAVE_ANDROID_OS */


#define NUM_ELEMS(x) (sizeof(x)/sizeof(x[0]))

#define MAX_AT_RESPONSE (8 * 1024)
#define HANDSHAKE_RETRY_COUNT 8
#define HANDSHAKE_TIMEOUT_MSEC 250
#define DEFAULT_AT_TIMEOUT_MSEC (3 * 60 * 1000)

#define AT_ONE_SECOND 1
/* Basic time interval between successive calls 
 * to the AT port closing timer tick handler 
 */
static const struct timeval TIMER_TICK_INTERVAL = {AT_ONE_SECOND, 0};

/* forward declaration */
void at_port_teardown( void );

struct atcontext {
    pthread_t tid_reader;
    int fd;                  /* fd of the AT channel. */
    char *at_tty_namep;
    int readerCmdFds[2];
    int isInitialized;
    ATUnsolHandler unsolHandler;

    /* For input buffering. */
    char ATBuffer[MAX_AT_RESPONSE+1];
    char *ATBufferCur;

    int readCount;

    /* Flag for the exclusive use of the readerLoop. Set whenever the
     * 'o' command has been received, cleared when the readerLoop has
     * unblocked the thread that issued the command (usually queueRunner).
     */
    bool at_is_open;  

    /* Delay to use whenever AT command port is opened. 
     * This value is initialized by calling at_open() 
     */
    int opendelay;

    /* Active timer setting for port closing delay. If this timer
     * is nonzero the port is open. The value is decremented
     * each time the tick function is called. When it reaches 
     * zero the port is closed unconditionally
     */
    int at_port_close_timer;

    /* Timeout setting. This value is initialized when at_open()
     * is called and remains in force until the next time the
     * function is called. It is loaded into at_port_close_timer
     * each time a call is made to open the AT port 
     */
    int at_port_close_timeout;

    /*
     * For current pending command, these are protected by commandmutex.
     *
     * The mutex and cond struct is memset in the getAtChannel() function,
     * so no initializer should be needed.
     */
    pthread_mutex_t requestmutex;
    pthread_mutex_t commandmutex;
    pthread_cond_t requestcond;
    pthread_cond_t commandcond;

    pthread_mutex_t portctlmutex;

    ATCommandType type;
    const char *responsePrefix;
    const char *smsPDU;
    ATResponse *response;

    void (*onTimeout)(void);
    void (*onReaderClosed)(void);
    int readerClosed;
    int noreaderThread;

    int timeoutMsec;

    struct termios ios;
};

static struct atcontext *s_defaultAtContext = NULL;

static pthread_key_t key;
static pthread_once_t key_once = PTHREAD_ONCE_INIT;

static int writeCtrlZ (const char *s);
static int writeline (const char *s);
static void onReaderClosed();

static void make_key()
{
    (void) pthread_key_create(&key, NULL);
}

/**
 * Set the atcontext pointer. Useful for sub-threads that needs to hold
 * the same state information.
 *
 * The caller IS responsible for freeing any memory already allocated
 * for any previous atcontexts.
 */
static void setAtContext(struct atcontext *ac)
{
    (void) pthread_once(&key_once, make_key);
    (void) pthread_setspecific(key, ac);
}

static int initializeAtContext()
{
    struct atcontext *ac;

    /* SWI_TBD -- according to Linux man page, pthread_once() always return 0 */
    if (pthread_once(&key_once, make_key)) {
        LOGE("Pthread_once failed!");
        return -1;
    }

    if ((ac = pthread_getspecific(key)) == NULL) {
        LOGI("%s: initializing at context", __func__);
        ac = malloc(sizeof(struct atcontext));

        memset(ac, 0, sizeof(struct atcontext));

        ac->noreaderThread = 1;
        ac->fd = -1;
        ac->readerCmdFds[0] = -1;
        ac->readerCmdFds[1] = -1;
        ac->ATBufferCur = ac->ATBuffer;

        /* Initially AT command port to modem is closed */
        ac->at_is_open = false;

        if (pipe(ac->readerCmdFds)) {
            LOGE("%s: Failed to create pipe: %s", __func__,strerror(errno));
            return -1;
        }

        pthread_mutex_init(&ac->commandmutex, NULL);
        pthread_mutex_init(&ac->requestmutex, NULL);
        pthread_mutex_init(&ac->portctlmutex, NULL);
        pthread_cond_init(&ac->requestcond, NULL);
        pthread_cond_init(&ac->commandcond, NULL);

        ac->timeoutMsec = DEFAULT_AT_TIMEOUT_MSEC;

        if (pthread_setspecific(key, ac)) {
            LOGE("pthread_setspecific failed!");
            return -1;
        }
    }

    LOGI("Initialized new AT Context!");

    return 0;
}

static struct atcontext *getAtContext()
{
    struct atcontext *ac = NULL;

    (void) pthread_once(&key_once, make_key);

    if ((ac = pthread_getspecific(key)) == NULL) {
        if (s_defaultAtContext)
            ac = s_defaultAtContext;
        else {
            LOGE("WARNING! getAtContext() called from external thread with "
                 "no defaultAtContext set!! This IS a bug! "
                 "A crash is probably nearby!");
        }
    } 

    return ac;
}

/**
 * This function will make the current AT thread the default channel,
 * meaning that calls from a thread that is not a queuerunner will
 * be executed in this context.
 */
void at_make_default_channel(void)
{
    struct atcontext *ac = getAtContext();

    if (ac->isInitialized)
        s_defaultAtContext = ac;
}

#if AT_DEBUG
void  AT_DUMP(const char*  prefix, const char*  buff, int  len)
{
    if (len < 0)
        len = strlen(buff);
    LOGD("%.*s", len, buff);
}
#endif

#ifndef USE_NP
static void setTimespecRelative(struct timespec *p_ts, long long msec)
{
    struct timeval tv;

    gettimeofday(&tv, (struct timezone *) NULL);

    /* What's really funny about this is that I know
       pthread_cond_timedwait just turns around and makes this
       a relative time again. */
    p_ts->tv_sec = tv.tv_sec + (msec / 1000);
    p_ts->tv_nsec = (tv.tv_usec + (msec % 1000) * 1000L ) * 1000L;

    /* Check for wrap-around of the nsec count. SWI_TBD use a defined constant */
    if (p_ts->tv_nsec >= 1000000000L) {
        LOGW("before time wrap %li %li\n", (long)p_ts->tv_sec, p_ts->tv_nsec);
        p_ts->tv_nsec %= 1000000000L;
        p_ts->tv_sec++;
        LOGW(" after time wrap %li %010li\n", (long)p_ts->tv_sec, p_ts->tv_nsec);
    }
}
#endif /*USE_NP*/

static void sleepMsec(long long msec)
{
    struct timespec ts;
    int err;

    ts.tv_sec = (msec / 1000);
    ts.tv_nsec = (msec % 1000) * 1000 * 1000;

    do {
        err = nanosleep (&ts, &ts);
    } while (err < 0 && errno == EINTR);
}



/** Add an intermediate response to sp_response. */
static void addIntermediate(const char *line)
{
    ATLine *p_new;
    struct atcontext *ac = getAtContext();

    p_new = (ATLine  *) malloc(sizeof(ATLine));

    p_new->line = strdup(line);

    /* Note: This adds to the head of the list, so the list will
       be in reverse order of lines received. the order is flipped
       again before passing on to the command issuer. */
    p_new->p_next = ac->response->p_intermediates;
    ac->response->p_intermediates = p_new;
}


/**
 * Returns 1 if line is a final response indicating error.
 * See 27.007 annex B.
 * WARNING: NO CARRIER and others are sometimes unsolicited.
 */
static const char * s_finalResponsesError[] = {
    "ERROR",
    "+CMS ERROR:",
    "+CME ERROR:",
    "NO CARRIER",      /* Sometimes! */
    "NO ANSWER",
    "NO DIALTONE",

    /* SWI_TBD:
       *CNTI does not follow the normal convention for +CME ERROR.
       Should probably submit a FW bug for this.
     */
    "*CNTI: +CME ERROR:"  
};
static int isFinalResponseError(const char *line)
{
    size_t i;

    for (i = 0 ; i < NUM_ELEMS(s_finalResponsesError) ; i++) {
        if (strStartsWith(line, s_finalResponsesError[i])) {
            return 1;
        }
    }

    return 0;
}

/**
 * Returns 1 if line is a final response indicating success.
 * See 27.007 annex B.
 * WARNING: NO CARRIER and others are sometimes unsolicited.
 */
static const char * s_finalResponsesSuccess[] = {
    "OK",
    "CONNECT"       /* Some stacks start up data on another channel. */
};
static int isFinalResponseSuccess(const char *line)
{
    size_t i;

    for (i = 0 ; i < NUM_ELEMS(s_finalResponsesSuccess) ; i++) {
        if (strStartsWith(line, s_finalResponsesSuccess[i])) {
            return 1;
        }
    }

    return 0;
}

/**
 * Returns 1 if line is a final response, either  error or success.
 * See 27.007 annex B.
 * WARNING: NO CARRIER and others are sometimes unsolicited.
 */
static int isFinalResponse(const char *line)
{
    return isFinalResponseSuccess(line) || isFinalResponseError(line);
}


/**
 * Returns 1 if line is the first line in (what will be) a two-line
 * SMS unsolicited response.
 */
static const char * s_smsUnsoliciteds[] = {
    "+CMT:",
    "+CDS:",
    "+CBM:"
};
static int isSMSUnsolicited(const char *line)
{
    size_t i;

    for (i = 0 ; i < NUM_ELEMS(s_smsUnsoliciteds) ; i++) {
        if (strStartsWith(line, s_smsUnsoliciteds[i])) {
            return 1;
        }
    }

    return 0;
}


/** 
 * handleFinalResponse - signal the command-initiator that
 * the final part of the AT response has been received. 
 * The effect of this is to unblock the command side which
 * runs in the queueRunner thread's context. 
 *          Assumes s_commandmutex is held
 */
static void handleFinalResponse(const char *line)
{
    struct atcontext *ac = getAtContext();

    ac->response->finalResponse = strdup(line);

    pthread_cond_signal(&ac->commandcond);
}

static void handleUnsolicited(const char *line)
{
    struct atcontext *ac = getAtContext();

    if (ac->unsolHandler != NULL) {
        ac->unsolHandler(line, NULL);
    }
}

static void processLine(const char *line)
{
    struct atcontext *ac = getAtContext();
    pthread_mutex_lock(&ac->commandmutex);

    if (ac->response == NULL) {
        /* No command pending. */
        handleUnsolicited(line);
    } else if (isFinalResponseSuccess(line)) {
        ac->response->success = 1;
        handleFinalResponse(line);
    } else if (isFinalResponseError(line)) {
        ac->response->success = 0;
        handleFinalResponse(line);
    } else if (ac->smsPDU != NULL && 0 == strcmp(line, "> ")) {
        /* See eg. TS 27.005 4.3.
           Commands like AT+CMGS have a "> " prompt. */
        writeCtrlZ(ac->smsPDU);
        ac->smsPDU = NULL;
    } else switch (ac->type) {
        case NO_RESULT:
            handleUnsolicited(line);
            break;
        case NUMERIC:
            if (ac->response->p_intermediates == NULL
                && isdigit(line[0])
            ) {
                addIntermediate(line);
            } else {
                /* Either we already have an intermediate response or
                   the line doesn't begin with a digit. */
                handleUnsolicited(line);
            }
            break;
        case SINGLELINE:
            if (ac->response->p_intermediates == NULL
                && strStartsWith (line, ac->responsePrefix)
            ) {
                addIntermediate(line);
            } else {
                /* We already have an intermediate response. */
                handleUnsolicited(line);
            }
            break;
        case MULTILINE:
            if (strStartsWith (line, ac->responsePrefix)) {
                addIntermediate(line);
            } else {
                handleUnsolicited(line);
            }
            break;

        /* This assumes that once a line with the initial response prefix
           is received, that no AT async output can appear until the complete
           response has been received.  This assumption is valid for the way
           Sierra commands are implemented on QCOM.  Sierra specific commands
           are currently the only commands known to use this non-standard
           output format.
        */
        case MULTIVALUE:
            /* Try to find the first response line, which contains the prefix */
            if (ac->response->p_intermediates == NULL) {
                if (strStartsWith (line, ac->responsePrefix)) {
                    addIntermediate(line);
                } else {
                    /* Haven't found the first line yet */
                    handleUnsolicited(line);
                }

            } else {
                /* First line already found; all remaining intermediate lines
                   are assumed to be part of the response
                 */
                addIntermediate(line);
            }
            break;

        default: /* This should never be reached */
            LOGE("Unsupported AT command type %d\n", ac->type);
            handleUnsolicited(line);
        break;
    }

    pthread_mutex_unlock(&ac->commandmutex);
}


/**
 * Returns a pointer to the end of the next line,
 * special-cases the "> " SMS prompt.
 *
 * returns NULL if there is no complete line.
 */
static char * findNextEOL(char *cur)
{
    if (cur[0] == '>' && cur[1] == ' ' && cur[2] == '\0') {
        /* SMS prompt character...not \r terminated */
        return cur+2;
    }

    // Find next newline
    while (*cur != '\0' && *cur != '\r' && *cur != '\n') cur++;

    return *cur == '\0' ? NULL : cur;
}


/**
 * Reads a line from the AT channel, returns NULL on timeout.
 * Assumes it has exclusive read access to the FD.
 *
 * This line is valid only until the next call to readline.
 *
 * This function exists because as of writing, android libc does not
 * have buffered stdio.
 */

static const char *readline()
{
    ssize_t count;

    char *p_read = NULL;
    char *p_eol = NULL;
    char *ret;
    char buf[10];

    struct atcontext *ac = getAtContext();

    LOGV("%s started",__func__);

    /* This is a little odd. I use *s_ATBufferCur == 0 to mean
     * "buffer consumed completely". If it points to a character,
     * then the buffer continues until a \0.
     */
    if (*ac->ATBufferCur == '\0') {
        /* Empty buffer. */
        ac->ATBufferCur = ac->ATBuffer;
        *ac->ATBufferCur = '\0';
        p_read = ac->ATBuffer;
    } else {   /* *s_ATBufferCur != '\0' */
        /* There's data in the buffer from the last read. */

        /* skip over leading newlines */
        while (*ac->ATBufferCur == '\r' || *ac->ATBufferCur == '\n')
            ac->ATBufferCur++;

        p_eol = findNextEOL(ac->ATBufferCur);

        if (p_eol == NULL) {
            /* A partial line. Move it up and prepare to read more. */
            size_t len;

            len = strlen(ac->ATBufferCur);

            memmove(ac->ATBuffer, ac->ATBufferCur, len + 1);
            p_read = ac->ATBuffer + len;
            ac->ATBufferCur = ac->ATBuffer;
        }
        /* Otherwise, (p_eol != NULL) there is a complete line 
           that will be returned from the while () loop below. */
    }

    while (p_eol == NULL) {
        int err;
        struct pollfd pfds[2];

        if (0 == MAX_AT_RESPONSE - (p_read - ac->ATBuffer)) {
            LOGE("ERROR: Input line exceeded buffer\n");
            /* Ditch buffer and start over again. */
            ac->ATBufferCur = ac->ATBuffer;
            *ac->ATBufferCur = '\0';
            p_read = ac->ATBuffer;
        }

        /* Block here until we are told the AT port is open */
        while( !ac->at_is_open ) {
            pfds[0].fd = ac->readerCmdFds[0];
            pfds[0].events = POLLIN;

            /* Block until an event occurs */
            err = poll(pfds, 1, -1);

            if(pfds[0].revents == POLLIN) {
                read(pfds[0].fd, &buf, 1);
                LOGD("%s received cmd(a): %c", __func__, buf[0]);
                /* Remember we received the 'o' command */
                if( buf[0] == 'o' ) {
                    ac->at_is_open = true;
                }
            }
        }

        /* At this point, the AT port must be open,
         * prepare the parameters to poll the serial 
         * port and the command pipe
         */
        pfds[1].fd = ac->fd;
        pfds[1].events = POLLIN | POLLERR;

        pfds[0].fd = ac->readerCmdFds[0];
        pfds[0].events = POLLIN;

        /* Block until an event occurs on either handle */
        err = poll(pfds, 2, -1);

        if (err < 0) {
            LOGE("poll: error: %s", strerror(errno));
            return NULL;
        }

        if (pfds[0].revents & POLLIN) {
            /* The port remains open until closed by timer 
             * expiry in function at_close_timer(). A 'c' 
             * command means the port was closed and this 
             * function should return to the upper poll()
             * and await an 'o' command. 'o' commands are
             * acceptable any time and have no effect other
             * than to kick this function back to the top
             * of the while loop
             */
            read(pfds[0].fd, &buf, 1);
            LOGD("%s received cmd(b): %c", __func__, buf[0]);
            if( buf[0] != 'c' ) {
                continue;
            }
            /* This means the command-issuer will close the AT port 
             * and this function should return to the top of the while
             * loop to await the next command
             */
            ac->at_is_open = false;
            continue;
        }

        /* Trap all AT port poll errors here */
        if (!(pfds[1].revents & POLLIN) && pfds[1].revents) {
            LOGE("%s, poll() reports err=%d and revents=0x%x", 
                 __func__, err, pfds[1].revents );

            /* Go back to the top and await another 'o' command */
            continue;
        }

        do {
            count = read(ac->fd, p_read,
                            MAX_AT_RESPONSE - (p_read - ac->ATBuffer));
        } while (count < 0 && errno == EINTR);

        if (count > 0) {
            AT_DUMP( "<< ", p_read, count );
            ac->readCount += count;

            p_read[count] = '\0';

            /* Skip over leading newlines. */
            while (*ac->ATBufferCur == '\r' || *ac->ATBufferCur == '\n')
                ac->ATBufferCur++;

            p_eol = findNextEOL(ac->ATBufferCur);
            p_read += count;
        } else if (count <= 0) {
            /* Read error encountered or EOF reached. */
            if(count == 0) {
                LOGD("atchannel: EOF reached.");
            } else {
                LOGD("atchannel: read error %s", strerror(errno));
            }
            return NULL;
        }
    }

    /* A full line in the buffer. Place a \0 over the \r and return. */

    ret = ac->ATBufferCur;
    *p_eol = '\0';
    ac->ATBufferCur = p_eol + 1;     /* This will always be <= p_read,    
                                        and there will be a \0 at *p_read. */

    LOGI("AT(%d)< %s\n", ac->fd, ret);
    return ret;
}


static void onReaderClosed()
{
    struct atcontext *ac = getAtContext();
    if (ac->onReaderClosed != NULL && ac->readerClosed == 0) {

        pthread_mutex_lock(&ac->commandmutex);

        ac->readerClosed = 1;


        pthread_cond_signal(&ac->commandcond);

        pthread_mutex_unlock(&ac->commandmutex);

        ac->onReaderClosed();
    }
}


/**
 * 
 * Thread for reading traffic from the AT command port. This thread
 * blocks on the read and when AT traffic originates from the modem
 * the thread unblocks, reads the traffic and processes the response.
 *
 * @param [in] arg
 *    Used to set the context for this thread
 *
 * @return 
 *    NULL, always
 *
 * @note:
 *    The thread must also synchronize with calls to at_send_command...
 *    The serial port for AT traffic is closed unless there is a need
 *    to actively send a command. The calling thread opens the serial 
 *    port and then unblocks this thread until the response is handled
 *
 */
static void *readerLoop(void *arg)
{
    struct atcontext *ac;

    LOGI("%s started",__func__);

    setAtContext((struct atcontext *) arg);
    ac = getAtContext();

    /* Now there is a reader thread */
    ac->noreaderThread = 0;

    for (;;) {
        const char * line;


        line = readline();

        if (line == NULL) {
            break;
        }

        if(isSMSUnsolicited(line)) {
            char *line1;
            const char *line2;

            /* The scope of string returned by 'readline()' is valid only
             * until next call to 'readline()' hence making a copy of line
             * before calling readline again
             */
            line1 = strdup(line);
            line2 = readline();

            if (line2 == NULL) {
                break;
            }

            if (ac->unsolHandler != NULL) {
                ac->unsolHandler (line1, line2);
            }
            free(line1);
        } else {
            processLine(line);
        }
    }

    onReaderClosed();

    /* Flag there is no more reader thread */
    pthread_mutex_lock( &ac->portctlmutex );
    ac->noreaderThread = 1;
    pthread_mutex_unlock( &ac->portctlmutex );

    return NULL;
}

/**
 * Sends string s to the radio with a \r appended.
 * Returns AT_ERROR_* on error, 0 on success.
 *
 * This function exists because as of writing, android libc does not
 * have buffered stdio.
 */
static int writeline (const char *s)
{
    size_t cur = 0;
    size_t len = strlen(s);
    ssize_t written;

    struct atcontext *ac = getAtContext();

    if (ac->fd < 0 || ac->readerClosed > 0) {
        return AT_ERROR_CHANNEL_CLOSED;
    }

    LOGI("AT(%d)> %s\n", ac->fd, s);

    AT_DUMP( ">> ", s, strlen(s) );

    /* The main string. */
    while (cur < len) {
        do {
            written = write (ac->fd, s + cur, len - cur);
        } while (written < 0 && errno == EINTR);

        if (written < 0) {
            return AT_ERROR_GENERIC;
        }

        cur += written;
    }

    /* The \r  */

    do {
        written = write (ac->fd, "\r" , 1);
    } while ((written < 0 && errno == EINTR) || (written == 0));

    if (written < 0) {
        return AT_ERROR_GENERIC;
    }

    return 0;
}


static int writeCtrlZ (const char *s)
{
    size_t cur = 0;
    size_t len = strlen(s);
    ssize_t written;

    struct atcontext *ac = getAtContext();

    if (ac->fd < 0 || ac->readerClosed > 0) {
        return AT_ERROR_CHANNEL_CLOSED;
    }

    LOGD("AT> %s^Z\n", s);

    AT_DUMP( ">* ", s, strlen(s) );

    /* The main string. */
    while (cur < len) {
        do {
            written = write (ac->fd, s + cur, len - cur);
        } while (written < 0 && errno == EINTR);

        if (written < 0) {
            return AT_ERROR_GENERIC;
        }

        cur += written;
    }

    /* the ^Z  */
    do {
        written = write (ac->fd, "\032" , 1);
    } while ((written < 0 && errno == EINTR) || (written == 0));

    if (written < 0) {
        return AT_ERROR_GENERIC;
    }

    return 0;
}

static void clearPendingCommand()
{
    struct atcontext *ac = getAtContext();

    if (ac->response != NULL) {
        at_response_free(ac->response);
    }

    ac->response = NULL;
    ac->responsePrefix = NULL;
    ac->smsPDU = NULL;
}


/**
 * This entry opens the AT handler, first initializing some values
 * in the control block and then starting the AT reader loop thread
 *
 * @param [in] ttynamep
 *    Pointer to a string representing the TTY to use for AT commands
 * @param [in] unsolhandlerp
 *    Pointer to a function for handling incoming unsolicited AT command
 *    traffic from the modem
 * @param [in] atopendelay
 *    Delay value (ms) to use whenever the AT command port is opened
 *    in preparation for initiating an AT command. Changes to this 
 *    value require that at_close() be called followed by a subsequent
 *    call to at_open()
 * @param [in] closetime
 *    The delay to use for the AT port closing timer in seconds.
 *    Converted to ticks inside this function prior to use.
 * @return
 *     0 - Function executed without errors
 *    -1 - Function had errors. See logs for details
 *
 * @note
 *    This function is not concerned with opening the AT command port
 *    to the modem. That happens within this file whenever there are 
 *    AT commands to send to the modem. This function initializes 
 *    the internal variables required for the AT mechanism to work.
 *
 */
int at_open(char *ttynamep, ATUnsolHandler h, int atopendelay, int closetime)
{
    int ret;
    pthread_t tid;
    pthread_attr_t attr;

    struct atcontext *ac;

    LOGI("%s started", __func__);
    
    if (initializeAtContext()) {
        LOGE("%s: InitializeAtContext failed!",__func__);
        return -1;
    }
    
    ac = getAtContext();

    /* Save the name pointer. 
     * NOTE: Needs to be freed if 
     * the thread ever ends
     */
    ac->at_tty_namep = ttynamep;

    ac->isInitialized = 1;
    ac->unsolHandler = h;
    ac->readerClosed = 0;

    ac->responsePrefix = NULL;
    ac->smsPDU = NULL;
    ac->response = NULL;

    ac->opendelay = atopendelay;

    /* User-specified timeout for holding the AT port open */
    ac->at_port_close_timeout = closetime;

    /* Initialize the timer to not running */
    ac->at_port_close_timer = 0;

    pthread_attr_init (&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    /* Only create the thread if it's not already running */
    pthread_mutex_lock( &ac->portctlmutex );
    if( ac->noreaderThread ) {
        ret = pthread_create(&ac->tid_reader, &attr, readerLoop, ac);
        if (ret < 0) {
            pthread_mutex_unlock( &ac->portctlmutex );
            perror ("pthread_create");
            return -1;
        }
    }
    pthread_mutex_unlock( &ac->portctlmutex );

    /* Clear out the termio settings structure */
    memset( (void *) &ac->ios, 0, sizeof(ac->ios) );

    return 0;
}

/**
 * Close the AT command mechanism. Not to be confused with 
 * closing the AT Command port to the modem. That is handled
 * elsewhere (at_port_teardown). This function is called when
 * there is no longer a need to send and receive AT commands
 *
 * @param
 *    none
 * @return
 *    none
 * @note
 *    This entry is only called from within the queueRunner() loop
 *    and is called whenever a condition occurs where the AT package
 *    should be closed.
 */
void at_close(void)
{
    struct atcontext *ac = getAtContext();

    LOGI("%s started", __func__);

    /* Make sure the AT port is closed, and readerloop has been notified.
       Note that it is safe to call this function more than once, without
       any intervening AT port open or readerloop 'o' command.
       */
    at_port_teardown();

    /* Critical section - set the reader closed flag */
    pthread_mutex_lock(&ac->commandmutex);

    ac->at_tty_namep=NULL;
    ac->readerClosed = 1;

    pthread_cond_signal(&ac->commandcond);

    pthread_mutex_unlock(&ac->commandmutex);
}

static ATResponse * at_response_new()
{
    return (ATResponse *) calloc(1, sizeof(ATResponse));
}

void at_response_free(ATResponse *p_response)
{
    ATLine *p_line;

    if (p_response == NULL) return;

    p_line = p_response->p_intermediates;

    while (p_line != NULL) {
        ATLine *p_toFree;

        p_toFree = p_line;
        p_line = p_line->p_next;

        free(p_toFree->line);
        free(p_toFree);
    }

    free (p_response->finalResponse);
    free (p_response);
}

/**
 * The line reader places the intermediate responses in reverse order,
 * here we flip them back.
 */
static void reverseIntermediates(ATResponse *p_response)
{
    ATLine *pcur,*pnext;

    pcur = p_response->p_intermediates;
    p_response->p_intermediates = NULL;

    while (pcur != NULL) {
        pnext = pcur->p_next;
        pcur->p_next = p_response->p_intermediates;
        p_response->p_intermediates = pcur;
        pcur = pnext;
    }
}

/**
 *
 * AT function to decrement an active timer and close the AT
 * port when the timer reaches zero. Runs in the context of
 * queueRunner() as an enqueued function which guarantees no
 * race condition exists between the setting of this timer
 * and its periodic decrementing
 *
 * @param [in] paramp
 *     - Optional void pointer to accept incoming parameters
 *       from the caller who enqueued this function. Not 
 *       used in this function
 *
 * @return
 *       None
 * 
 * @note
 *       Runs in the context of the queueRunner() thread
 *
 */
static void at_close_timer( void * paramp )
{
    struct atcontext *ac = getAtContext();
    
    LOGV("%s started",__func__);

    /* If the timer is running */
    if( ac->at_port_close_timer )
    {
        LOGV("Timer running: %d ticks remain", ac->at_port_close_timer);
        /* Decrement it */
        if( !(--ac->at_port_close_timer) )
        {
            /* Timer just expired */
            at_port_teardown();
        }
        else
        {
			/* re-arm this timeout function until timer expires */
			enqueueRILEvent( RIL_EVENT_QUEUE_NORMAL, at_close_timer, 
                             NULL, &TIMER_TICK_INTERVAL);
        }
    }
}

/**
 * 
 * Prepare the AT Command port, opening it and adjusting the
 * tty settings for our use. If the port is already open 
 * don't do anything except re-arm the port closure timeout
 *
 * @param 
 *    none
 * @return
 *     0 - Successfully opened the port
 *    -1 - An error occurred opening the port
 * @note
 *     Called from the context of the command thread always
 *
 */
static int at_port_setup( void )
{
    int fd;
    struct atcontext *ac = getAtContext();

    LOGI("%s started",__func__);

    /* If the port name wasn't defined...*/
    if(!ac->at_tty_namep) {
        LOGE("%s: No AT tty specified", __func__);
        return -1;
    }

    /* Only open the port if the timer is not running. 
     * If it is running then the port must have already
     * been opened 
     */
    if( !ac->at_port_close_timer ) {
    
        /* otherwise, open the port */
        fd = open(ac->at_tty_namep, O_RDWR);

        /* Disable echo on serial ports. */
        tcgetattr(fd, &ac->ios);
        cfmakeraw(&ac->ios);
        cfsetospeed(&ac->ios, B115200);
        cfsetispeed(&ac->ios, B115200);
        ac->ios.c_cflag |= CREAD | CLOCAL;
        tcflush(fd, TCIOFLUSH);
        tcsetattr(fd, TCSANOW, &ac->ios);
    
        /* Save the file descriptor */
        ac->fd = fd;

        /* Insert a short delay to give the modem a 
         * chance to catch up with the host. Some 
         * modems require more time to prepare their
         * serial interfaces than the host and this
         * short delay gives them time to prepare
         */
        if( ac->opendelay )
            sleepMsec(ac->opendelay);

    }

    return 0;
}

/**
 * 
 * Close the AT port and do any local clean up that is required.
 * The AT port opens when commands need to be sent and is closed
 * when the response has been received. Therefore, closing the 
 * AT port does not mean the reader thread needs to terminate.
 *
 * @param 
 *    none
 * @return
 *    none
 * @note
 *     Always called from the context of the command (RIL) thread
 *
 */
void at_port_teardown( void )
{
    struct atcontext *ac = getAtContext();
    
    LOGI("%s started",__func__);
    
    /* Close the port if an FD is defined */
    if (ac->fd >= 0) {
        if (close(ac->fd) != 0)
            LOGE("%s: FAILED to close fd %d!", __func__,ac->fd);
    }
    ac->fd = -1;

    /* Kick readerloop. */
    write(ac->readerCmdFds[1], "c", 1);

    return;
}

/**
 * Internal send_command implementation.
 * Doesn't lock or call the timeout callback.
 * Must be called with commandmutex already locked
 *
 * All send command calls ultimately pass through 
 * this one
 *
 * timeoutMsec == 0 means infinite timeout.
 */

static int at_send_command_full_nolock (const char *command, ATCommandType type,
                    const char *responsePrefix, const char *smspdu,
                    long long timeoutMsec, ATResponse **pp_outResponse)
{
    int err = 0;
#ifndef USE_NP
    struct timespec ts;
#endif /* USE_NP */

    struct atcontext *ac = getAtContext();

    /* FIXME This is to prevent future problems due 
     * to calls from other threads; should be revised 
     */
    while (pthread_mutex_trylock(&ac->requestmutex) == EBUSY) {
        pthread_cond_wait(&ac->requestcond, &ac->commandmutex);
    }

    if(ac->response != NULL) {
        err = AT_ERROR_COMMAND_PENDING;
        LOGE("%s, (a) error: %d", __func__, err);
        goto error;
    }

    /* Open the AT Command port here and adjust the control block */
    at_port_setup();

     /* Tell readline() the port is open */
    write(ac->readerCmdFds[1], "o", 1);

    err = writeline (command);

    if (err < 0) {
        LOGE("%s, (b) error: %d", __func__, err);
        goto error;
    }

    ac->type = type;
    ac->responsePrefix = responsePrefix;
    ac->smsPDU = smspdu;
    ac->response = at_response_new();

    /* Block while the readerloop prepares the response from the modem */
    while (ac->response->finalResponse == NULL && ac->readerClosed == 0) {
        if (timeoutMsec != 0) {
#ifdef USE_NP
            err = pthread_cond_timeout_np(&ac->commandcond, &ac->commandmutex, timeoutMsec);
#else
            setTimespecRelative(&ts, timeoutMsec);
            err = pthread_cond_timedwait(&ac->commandcond, &ac->commandmutex, &ts);
#endif /*USE_NP*/
        } else {
            err = pthread_cond_wait(&ac->commandcond, &ac->commandmutex);
        }

        if (err == ETIMEDOUT) {
            err = AT_ERROR_TIMEOUT;
            LOGI("%s, (c) timed out, retrying: %d", __func__, err);
            goto error;
        }
    }

    if (pp_outResponse == NULL) {
        at_response_free(ac->response);
    } else {
        /* Line reader stores intermediate responses in reverse order. */
        reverseIntermediates(ac->response);
        *pp_outResponse = ac->response;
    }

    ac->response = NULL;

    if(ac->readerClosed > 0) {
        err = AT_ERROR_CHANNEL_CLOSED;
        LOGE("%s, (d) error: %d", __func__, err);
        goto error;
    }

    err = 0;
error:
    clearPendingCommand();

    /* SWI_TBD 
       If we never start the timer function, the timer will never
       expire, and the port will always remain open.  Probably 
       should be configurable in some way, but this is good enough 
       for now.
     */
#if 0
    /* Now that the command is done, enqueue the 
     * timer function but only if it's not already
     * running
     */
    if( !(ac->at_port_close_timer) ) {
    
        /* Enqueue a timeout function to operate
         * the timer and close the AT port when
         * it reaches 0 
         */
        enqueueRILEvent( RIL_EVENT_QUEUE_NORMAL, at_close_timer, 
                         NULL, &TIMER_TICK_INTERVAL );
    }
#endif
  
    /* Always [re] arm the timer tick with the user-specified timeout value */
    ac->at_port_close_timer = ac->at_port_close_timeout;

    /* Unblock any threads which may be blocking
     * in the while loop at the start of this 
     * function
     */
    pthread_cond_broadcast(&ac->requestcond);
    pthread_mutex_unlock(&ac->requestmutex);

    return err;
}

/**
 * Internal send_command implementation.
 *
 * timeoutMsec == 0 means infinite timeout.
 */
static int at_send_command_full (const char *command, ATCommandType type,
                    const char *responsePrefix, const char *smspdu,
                    long long timeoutMsec, ATResponse **pp_outResponse)
{
    int err;

    struct atcontext *ac = getAtContext();

    LOGE("--- %s", command);

    if (0 != pthread_equal(ac->tid_reader, pthread_self())) {
        /* Cannot be called from reader thread. */
        return AT_ERROR_INVALID_THREAD;
    }

    pthread_mutex_lock(&ac->commandmutex);

    err = at_send_command_full_nolock(command, type,
                    responsePrefix, smspdu,
                    timeoutMsec, pp_outResponse);

    pthread_mutex_unlock(&ac->commandmutex);

    if (err == AT_ERROR_TIMEOUT && ac->onTimeout != NULL) {
        ac->onTimeout();
    }

    return err;
}

/* Only call this from onTimeout, since we're not locking or anything. */
void at_send_escape (void)
{
    struct atcontext *ac = getAtContext();
    int written;

    do {
        written = write (ac->fd, "\033" , 1);
    } while ((written < 0 && errno == EINTR) || (written == 0));
}

/**
 * Issue a single normal AT command with no intermediate response expected.
 *
 * "command" should not include \r.
 * pp_outResponse can be NULL.
 *
 * if non-NULL, the resulting ATResponse * must be eventually freed with
 * at_response_free.
 */
int at_send_command (const char *command, ATResponse **pp_outResponse)
{
    int err;

    struct atcontext *ac = getAtContext();

    err = at_send_command_full (command, NO_RESULT, NULL,
                                    NULL, ac->timeoutMsec, pp_outResponse);

    return err;
}


int at_send_command_singleline (const char *command,
                                const char *responsePrefix,
                                 ATResponse **pp_outResponse)
{
    int err;

    struct atcontext *ac = getAtContext();

    err = at_send_command_full (command, SINGLELINE, responsePrefix,
                                    NULL, ac->timeoutMsec, pp_outResponse);

    if (err == 0 && pp_outResponse != NULL
        && (*pp_outResponse)->success > 0
        && (*pp_outResponse)->p_intermediates == NULL
    ) {
        /* Successful command must have an intermediate response. */
        at_response_free(*pp_outResponse);
        *pp_outResponse = NULL;
        return AT_ERROR_INVALID_RESPONSE;
    }

    return err;
}


int at_send_command_singleline_with_timeout (const char *command,
                                             const char *responsePrefix,
                                             ATResponse **pp_outResponse,
                                             int timeoutMsec)
{
    int err;

    err = at_send_command_full (command, SINGLELINE, responsePrefix,
                                    NULL, timeoutMsec, pp_outResponse);

    if (err == 0 && pp_outResponse != NULL
        && (*pp_outResponse)->success > 0
        && (*pp_outResponse)->p_intermediates == NULL
    ) {
        /* Successful command must have an intermediate response. */
        at_response_free(*pp_outResponse);
        *pp_outResponse = NULL;
        return AT_ERROR_INVALID_RESPONSE;
    }

    return err;
}


int at_send_command_numeric (const char *command,
                                 ATResponse **pp_outResponse)
{
    int err;

    struct atcontext *ac = getAtContext();

    err = at_send_command_full (command, NUMERIC, NULL,
                                    NULL, ac->timeoutMsec, pp_outResponse);

    if (err == 0 && pp_outResponse != NULL
        && (*pp_outResponse)->success > 0
        && (*pp_outResponse)->p_intermediates == NULL
    ) {
        /* Successful command must have an intermediate response. */
        at_response_free(*pp_outResponse);
        *pp_outResponse = NULL;
        return AT_ERROR_INVALID_RESPONSE;
    }

    return err;
}


int at_send_command_sms (const char *command,
                                const char *pdu,
                                const char *responsePrefix,
                                 ATResponse **pp_outResponse)
{
    int err;

    struct atcontext *ac = getAtContext();

    err = at_send_command_full (command, SINGLELINE, responsePrefix,
                                    pdu, ac->timeoutMsec, pp_outResponse);

    if (err == 0 && pp_outResponse != NULL
        && (*pp_outResponse)->success > 0
        && (*pp_outResponse)->p_intermediates == NULL
    ) {
        /* Successful command must have an intermediate response. */
        at_response_free(*pp_outResponse);
        *pp_outResponse = NULL;
        return AT_ERROR_INVALID_RESPONSE;
    }

    return err;
}


int at_send_command_multiline (const char *command,
                                const char *responsePrefix,
                                 ATResponse **pp_outResponse)
{
    int err;

    struct atcontext *ac = getAtContext();

    err = at_send_command_full (command, MULTILINE, responsePrefix,
                                    NULL, ac->timeoutMsec, pp_outResponse);

    return err;
}

int at_send_command_multivalue (const char *command,
                                const char *responsePrefix,
                                ATResponse **pp_outResponse)
{
    int err;

    struct atcontext *ac = getAtContext();

    err = at_send_command_full (command, MULTIVALUE, responsePrefix,
                                    NULL, ac->timeoutMsec, pp_outResponse);

    return err;
}


/**
 * Set the default timeout. Let it be reasonably high, some commands
 * take their time. Default is 10 minutes.
 */
void at_set_timeout_msec(int timeout)
{
    struct atcontext *ac = getAtContext();

    ac->timeoutMsec = timeout;
}

/** This callback is invoked on the command thread. */
void at_set_on_timeout(void (*onTimeout)(void))
{
    struct atcontext *ac = getAtContext();

    ac->onTimeout = onTimeout;
}


/*
 * This callback is invoked on the reader thread (like ATUnsolHandler), when the
 * input stream closes before you call at_close (not when you call at_close()).
 * You should still call at_close(). It may also be invoked immediately from the
 * current thread if the read channel is already closed.
 */
void at_set_on_reader_closed(void (*onClose)(void))
{
    struct atcontext *ac = getAtContext();

    ac->onReaderClosed = onClose;
}


/**
 * Periodically issue an AT command and wait for a response.
 * Used to ensure channel has start up and is active.
 */
int at_handshake()
{
    int i;
    int err = 0;

    struct atcontext *ac = getAtContext();

    if (0 != pthread_equal(ac->tid_reader, pthread_self())) {
        /* Cannot be called from reader thread. */
        return AT_ERROR_INVALID_THREAD;
    }

    pthread_mutex_lock(&ac->commandmutex);

    for (i = 0 ; i < HANDSHAKE_RETRY_COUNT ; i++) {
        /* Some stacks start with verbose off. */
        err = at_send_command_full_nolock ("ATE0Q0V1", NO_RESULT,
                    NULL, NULL, HANDSHAKE_TIMEOUT_MSEC, NULL);

        if (err == 0) {
            break;
        }
        else {
            LOGE("%s: got error %d from at_send_cmd_full_nolock()", 
                 __func__, err);
        }
    }

    if (err == 0) {
        /* Pause for a bit to let the input buffer drain any unmatched OK's
           (they will appear as extraneous unsolicited responses). */
        LOGD("pausing..");
        sleepMsec(HANDSHAKE_TIMEOUT_MSEC);
    }

    pthread_mutex_unlock(&ac->commandmutex);

    return err;
}

/**
 * Returns error code from response.
 * Assumes AT+CMEE=1 (numeric) mode.
 */
AT_CME_Error at_get_cme_error(const ATResponse *p_response)
{
    int ret;
    int err;
    char *p_cur;

    if (p_response->success > 0) {
        return CME_SUCCESS;
    }

    p_cur = p_response->finalResponse;

    /* SWI_TBD:
       *CNTI does not follow the normal convention for +CME ERROR.  Also had to
       change s_finalResponsesError. Should probably submit a FW bug for this.
     */
    if ( (p_cur != NULL) && strStartsWith(p_cur, "*CNTI:") ) {
        /* skip over the *CNTI: prefix */
        err = at_tok_start(&p_cur);
        if (err < 0) {
            return CME_ERROR_NON_CME;
        }

        /* skip over the space between the two prefixes */
        p_cur++;
    }

    if ((p_cur == NULL) || !strStartsWith(p_cur, "+CME ERROR:")) {
        return CME_ERROR_NON_CME;
    }

    /* Looks like a real +CME ERROR line, so parse it */
    err = at_tok_start(&p_cur);

    if (err < 0) {
        return CME_ERROR_NON_CME;
    }

    err = at_tok_nextint(&p_cur, &ret);

    if (err < 0) {
        return CME_ERROR_NON_CME;
    }

    return (AT_CME_Error) ret;
}

