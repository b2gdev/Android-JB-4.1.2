/*************
 *
 * Filename: swi_osapi.c
 *
 * Purpose:  Wrap OS functions for API-Side (application processes)
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 * The SDK process creates UNIX IPC sockets. To view them, type:
 *      netstat -a --protocol=unix|grep SWI
 *
 **************/

#define SWI_SOCKET_REAP_MS 500        /* socket reap time after exit */
#define SWI_PROCESS_STARTUP_MS 1000   /* process startup time */
#define SWI_RETRY_MS 100              /* milliseconds */
#define SWI_MESSAGE_BYTES 5000
#define SWI_STACK_SIZE    (1024UL * 1024UL)    /* stack size for threads */

/* include files */
#include "swi_osapi.h"        /* packages user include file  */

#ifndef ANDROID_LOGGING
/* All platforms except Android */
#include <syslog.h>

local void swi_osapierrordefault(const char *, int);    /* fwd reference */

static void (*swi_osapierrorfnp)(const char *, int) = swi_osapierrordefault;
#else
/* Android logging */
#define LOG_TAG "RILQAPI"
/* RILSTART */
/* #include <utils/Log.h> */
#include "swiril_log.h"

static struct swi_ossdkprocessInfo sdkprocessInfo;
/* RILSTOP */

local void swi_osapilogerror(const char *errorp, int llerrno);

static void (*swi_osapierrorfnp)(const char *, int) = swi_osapilogerror;
#endif

/*************
 *
 * Name: swi_osapierrorhandlerset
 *
 * Purpose: Change handler for API-side OS error.
 *
 * Params:  handlerp - function called as:
 *
 *              (*handlerp)(const char *errorp, int llerrno)
 *
 *          where:
 *              errorp - high-level explanation of failure
 *              llerrno - low-level error number (from errno)
 *
 * Return:  No value
 *
 * Abort:   none
 *
 * Notes:   The default handler writes to stderr
 *
 **************/

void swi_osapierrorhandlerset(void (*handlerp)(const char *, int))
{
    swi_osapierrorfnp = handlerp;
}


/*************
 *
 * Name: swi_osapilogerror
 *
 * Purpose:  Write to log, show low-level error
 *
 * Params:   errorp  - string describing problem
 *           llerrno - low-level error number (from errno)
 *
 * Return:   No value
 *
 * Abort:    none
 *
 * Notes:    Don't overuse or repeatedly call this function for the same
 *           error; i.e., for every retry
 *           Needed by the test application.
 *
 **************/
local void swi_osapilogerror(const char *errorp, int llerrno)
{
#ifdef ANDROID_LOGGING
    /* Android logging */
    if (llerrno != 0)
        LOGE("%5i : %s", llerrno, errorp);
    else
        LOGI("%s", errorp);
#else
    /* All other platforms */
    if (llerrno != 0)
    {
        errno = llerrno;
        syslog(LOG_USER | LOG_ERR, "%s: %m", errorp);
    }
    else
        syslog(LOG_USER | LOG_ERR, "%s", errorp);
#endif
}

/*************
 *
 * Name: swi_osapilog
 *
 * Purpose:  Write to the system log /var/log/user.log
 *
 * Params:   errorp  - string describing problem
 *
 * Return:   No value
 *
 * Abort:    none
 *
 * Notes:    Don't overuse or repeatedly call this function for the same error;
 *           i.e., for every retry
 *           Needed by the test application.
 *
 **************/
void swi_osapilog(const char *errorp)
{
    swi_osapilogerror(errorp, 0);
}

/*************
 *
 * Name: swi_osapiaborterror
 *
 * Purpose:  Abort the process, write to log, show low-level error
 *
 * Params:   reasonp - string describing problem
 *           llerrno - low-level error number (from errno)
 *
 * Return:   No value
 *
 * Abort:    none
 *
 * Notes:    Needed by the test application.
 *
 **************/

void swi_osapiaborterror(const char *errorp, int llerrno)
{
    swi_osapilogerror(errorp, llerrno);

    abort();
}

/*************
 *
 * Name: swi_osapiabort
 *
 * Purpose:  Abort the process, write to log /var/log/user.log
 *
 * Params:   reasonp - string describing problem
 *
 * Return:   No value
 *
 * Abort:    none
 *
 * Notes:    Needed by the test application.
 *
 **************/

void swi_osapiabort(const char *errorp)
{
    swi_osapiaborterror(errorp, 0);
}




/*************
 *
 * Name: swi_osapierrordefault
 *
 * Purpose: Default reporter of an API-side error.
 *
 * Params:  errorp - high-level explanation of failure
 *
 *          llerrno - low-level error number (from errno)
 *
 * Return:  No value
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/

local void swi_osapierrordefault(const char *errorp, int llerrno)
{
    char msg[128];

    snprintf(msg, sizeof(msg), "SWI API: %s", errorp);

    if (llerrno != 0)
    {
        errno = llerrno;
        perror(msg);
    }
    else
        fprintf(stderr, "%s\n", msg);
}

/*************
 *
 * Name: swi_osapierror
 *
 * Purpose: Report an API-side error.
 *
 * Params:  errorp - high-level explanation of failure
 *          llerrno - low-level error number (from errno)
 *
 * Return:  No value
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/

local void swi_osapierror(const char *errorp, int llerrno)
{
    if (swi_osapierrorfnp != NULL)
        (*swi_osapierrorfnp)(errorp, llerrno);
}

/*************
 *
 * Name: swi_osapidelay
 *
 * Purpose: Block for a specified time.
 *
 * Params:  timeout - timeout in ms
 *
 * Return:  No value
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/

local void swi_osapidelay(swi_uint32 timeout)
{
    struct timespec timev;
    struct timespec timer;
    int rv;

    timer.tv_sec = timeout / 1000UL;
    timer.tv_nsec = (timeout * 1000000UL) % 1000000000UL;

    do
    {
        timev = timer;        /* remaining time */
        rv = nanosleep(&timev, &timer);

    } while (rv < 0 && errno == EINTR);
}


/*************
 *
 * Name: swi_osapisdkrunning
 *
 * Purpose: Determine if SDK process is already running
 *
 * Params:  runverifytime  - time to verify that process is running
 *                           and hasn't just ended
 *          stopverifytime - time to verify that process is stopped and
 *                           hasn't just been started
 *
 * Return:  TRUE if already running
 *
 * Abort:   no memory
 *
 * Notes:   Errs in the direction of saying process is NOT running as
 *          a newly-created SDK process also checks to see if it is a
 *          duplicate.
 *
 **************/
local swi_bool swi_osapisdkrunning(swi_uint32 runverifytime,
                                   swi_uint32 stopverifytime)
{
    struct sockaddr_un run_sa;
    int rv;
    swi_uint32 delay;
    int sock;
    swi_bool isrunning;

    run_sa.sun_family = AF_UNIX;
    memset(run_sa.sun_path, 0, sizeof(run_sa.sun_path)); /* must be zeroed */
    snprintf(&run_sa.sun_path[1], sizeof(run_sa.sun_path) - 1,
        "SWI_SDK_SOCKET_MUTEX");  /* use abstract namespace */

    sock = socket(PF_LOCAL, SOCK_DGRAM, 0);    /* must be SOCK_DGRAM */
    if (sock < 0)
    {
        swi_osapierror("SDK run check (s) failed", errno);
        return FALSE;
    }

    delay = 0;
    for ( ; ; )
    {
        if (connect(sock, (struct sockaddr *)&run_sa,
                            sizeof(run_sa)) < 0) /* probably not running */
        {
            if (errno == EINTR)
            {
                /* interrupted - repeat */
            }
            else if (delay >= stopverifytime)    /* if not running */
            {
                isrunning = FALSE;
                break;
            }
            else
            {
                swi_osapidelay(SWI_RETRY_MS);
                delay += SWI_RETRY_MS;
            }
        }
        else        /* probably running */
        {
            run_sa.sun_family = AF_UNSPEC;
            connect(sock, (struct sockaddr *)&run_sa,
                    sizeof(run_sa));    /* disassociate */
            run_sa.sun_family = AF_UNIX;

            if (delay >= runverifytime) /* bind timout is socket reap time */
            {
                isrunning = TRUE;
                break;
            }

            swi_osapidelay(SWI_RETRY_MS);
            delay += SWI_RETRY_MS;
        }
    }

    do
    {
        rv = close(sock);
    } while (rv < 0 && errno == EINTR);

    return isrunning;
}

/* RILSTART */
/*************
 *
 * Name: swi_osapisdkrunning
 *
 * Purpose: Determine if SDK process is already running
 *
 * Params:  None
 *
 * Return:  TRUE if already running
 *
 * Abort:   None
 *
 * Notes:   None
 *
 **************/
swi_bool swi_osapiverifySDKstatus( void )
{
    return swi_osapisdkrunning(SWI_SOCKET_REAP_MS, 0);
}

/*************
 *
 * Name: swi_osapisetprocessID
 *
 * Purpose: Set the process ID of the SDK process
 *
 * Params:  pid - process ID of the SDK
 *
 * Return:  None
 *
 **************/
static void swi_osapisetprocessID( pid_t pid )
{
    sdkprocessInfo.pid = pid;
}

/*************
 *
 * Name: swi_osapigetprocessID
 *
 * Purpose: Get the process ID of the SDK process
 *
 * Params:  [out]pid - process ID of the SDK
 *
 * Return:  None
 *
 **************/
swi_bool swi_osapigetprocessID( pid_t *pid )
{
    *pid = sdkprocessInfo.pid;
    return TRUE;
}
/* RILSTOP */

/*************
 *
 * Name: swi_osapisdkprocesscreate
 *
 * Purpose: Create the SDK process, if it hasn't been created already,
 *          otherwise do nothing.
 *
 * Params:  pathp     - pointer to path name of SDK executable
 *
 * Return:  TRUE if process created or was already running. FALSE if
 *          fatal Linux error occurred.
 *
 * Abort:   none
 *
 * Notes:   This function should not be called more than once by a single
 *          process. It blocks for awhile when the SDK process is already
 *          running to make sure it wasn't just killed and to see it
 *          started up.
 *
 **************/

swi_bool swi_osapisdkprocesscreate(const char *pathp)
{
    char *argp[2];
    char *envp[1];
    pid_t pid;

    argp[0] = (char *)pathp;
    argp[1] = NULL;
    envp[0] = NULL;

    /* RILSTART */
    /* Changes for debug use, not for back-porting */
    swi_osapilog("starting swi_osapisdkprocesscreate");
    /* RILSTOP */

    if (swi_osapisdkrunning(SWI_SOCKET_REAP_MS, 0)) /* if already running */
    {
        return TRUE;
    }

    /* start SDK process */
    pid = fork();

    if (pid < 0)
    {
        swi_osapierror("process creation failed", errno);
        return FALSE;
    }
    else if (pid == 0)    /* in child process */
    {
        execve(pathp, argp, envp);        /* launch executable */
        swi_osapierror("SDK launch failed", errno);
        setsid();                         /* prevent zombification */
        exit(1);                          /* terminate child */
    }

    /* still in parent process */

    if (!swi_osapisdkrunning(SWI_PROCESS_STARTUP_MS,
                            SWI_PROCESS_STARTUP_MS))
    /* if not now running, giving time to open sockets */
    {
        swi_osapierror("SDK process startup failed", 0);
        return FALSE;
    }

    /* RILSTART */
    swi_osapisetprocessID(pid);
    swi_osapilog("SDK Pid set");
    /* RILSTOP */

    return TRUE;
}

/*************
 *
 * Name: swi_osapiipcformlocalsocketname
 *
 * Purpose: Form local socket name
 *
 * Params:  str         - Point to buffer that save socket name.
 *          size        - Size of the buffer pointer by str
 *          ipcindex    - IPC channel No.
 *          ipcnamep    - Pointer to channel name.
 *
 * Return:  None.
 *
 * Abort:   None.
 *
 * Notes:   The implementation of this function must match the implementation
 *          of function swi_ossdkipcformremotesocketname().
 *
 **************/
local void swi_osapiipcformlocalsocketname(char *str,
                                      size_t size,
                                      size_t ipcindex,
                                      swi_uint8 * ipcnamep)
{
    memset(str, 0, size); /* must be zeroed */

    /* use abstract namespace */
    snprintf(str+1, size - 1, "SWI_API_%s_SOCKET_%u", ipcnamep, ipcindex);
}

/*************
 *
 * Name: swi_osapiipcformremotesocketname
 *
 * Purpose: Form remote socket name
 *
 * Params:  str         - Point to buffer that save socket name.
 *          size        - Size of the buffer pointer by str
 *          ipcindex    - IPC channel No.
 *          ipcnamep    - Pointer to channel name.
 *
 * Return:  None.
 *
 * Abort:   None.
 *
 * Notes:   The implementation of this function must match the implementation
 *          of function swi_ossdkipcformlocalsocketname().
 *
 **************/
local void swi_osapiipcformremotesocketname(char *str,
                                      size_t size,
                                      size_t ipcindex,
                                      swi_uint8 * ipcnamep)
{
    memset(str, 0, size); /* must be zeroed */

    /* use abstract namespace */
    snprintf(str+1, size - 1, "SWI_SDK_%s_SOCKET_%u", ipcnamep, ipcindex);
}

/*************
 *
 * Name: swi_osapiipcformlocalsocketaddr
 *
 * Purpose: Form local socket address
 *
 * Params:  sap         - Pointer to structure that save socket address.
 *          ipcindex    - IPC channel No.
 *          ipcnamep    - Pointer to channel name.
 *
 * Return:  None.
 *
 * Abort:   None.
 *
 **************/
local void swi_osapiipcformlocalsocketaddr(struct sockaddr_un *sap,
                                         size_t ipcindex,
                                         swi_uint8 * ipcnamep)
{
    sap->sun_family = AF_UNIX;
    swi_osapiipcformlocalsocketname(sap->sun_path, sizeof(sap->sun_path),
            ipcindex, ipcnamep);
}

/*************
 *
 * Name: swi_osapiipcformremotesocketaddr
 *
 * Purpose: Form remote socket address
 *
 * Params:  sap         - Pointer to structure that save socket address.
 *          ipcindex    - IPC channel No.
 *          ipcnamep    - Pointer to channel name.
 *
 * Return:  None.
 *
 * Abort:   None.
 *
 **************/
local void swi_osapiipcformremotesocketaddr(struct sockaddr_un *sap,
                                         size_t ipcindex,
                                         swi_uint8 * ipcnamep)
{
    sap->sun_family = AF_UNIX;
    swi_osapiipcformremotesocketname(sap->sun_path, sizeof(sap->sun_path),
            ipcindex, ipcnamep);
}

/*************
 *
 * Name: swi_osapiipcsetdest
 *
 * Purpose: Set destination socket
 *
 * Params:  ip          - Pointer to structure that saves destination socket
 *                        address to be set.
 *          ipcindex    - IPC channel No. to set.
 *          ipcnamep    - Pointer to channel name.
 *
 * Return:  None.
 *
 * Abort:   None.
 *
 **************/
void swi_osapiipcsetdest(struct swi_osipcapi *ip,
                         swi_size_t ipcindex,
                         swi_uint8 * ipcnamep)
{
    swi_osapiipcformremotesocketaddr(&ip->osipcdest, ipcindex, ipcnamep);
}


/*************
 *
 * Name: swi_osapiipcopen
 *
 * Purpose: Initialize an inter-process communication channel on the API side
 *          for communication with the SDK process
 *
 * Params:  ip       - pointer to IPC structure
 *          ipcindex - channel number
 *          ipcnamep - pointer to channel name
 *
 * Return:  TRUE if OK, false if channel already exists or fatal Linux error.
 *
 * Abort:   No permission
 *
 * Notes:   swi_osapisdkprocesscreate must have been called and returned first
 *          Writes error message using error handler
 *
 **************/

swi_bool swi_osapiipcopen(  struct swi_osipcapi *ip,
                            size_t ipcindex,
                            swi_uint8 *ipcnamep)
{
    struct sockaddr_un loc_sa;
    int socketsize;
    int delay;

    /* build our local address */
    swi_osapiipcformlocalsocketaddr(&loc_sa, ipcindex, ipcnamep);

    /* build remote address in IPC structure */
    swi_osapiipcsetdest(ip, ipcindex, ipcnamep);

    ip->osipcsocket = socket(PF_LOCAL, SOCK_DGRAM, 0);

    if (ip->osipcsocket < 0)
    {
        swi_osapierror("IPC open (s) failed", errno);
        return FALSE;
    }


    delay = 0;
    for ( ; ; )
    {
        /* bind local address */
        if (bind(ip->osipcsocket, (struct sockaddr *)&loc_sa,
                                            sizeof(loc_sa)) >= 0)
            break;

        if (delay >= SWI_SOCKET_REAP_MS ||
                            errno != EADDRINUSE)
        {
            swi_osapierror("IPC open (b) failed", errno);
            return FALSE;
        }

        /* linux takes a while to reap sockets so retry after SWI_RETRY_MS */
        swi_osapidelay(SWI_RETRY_MS);
        delay += SWI_RETRY_MS;
    }

    socketsize = SWI_MESSAGE_BYTES;
    if (setsockopt(ip->osipcsocket, SOL_SOCKET, SO_SNDBUF, &socketsize,
                                sizeof(socketsize)) < 0)
    {
        swi_osapierror("IPC open (o) failed", errno);
        return FALSE;
    }

    return TRUE;
}

/*************
 *
 * Name: swi_osapiipcclose
 *
 * Purpose: Close socket
 *
 * Params:  picp        - Pointer to structure that saves destination socket
 *                        address to be closed.
 *
 * Return:  TRUE if socket is closed succefully; otherwise return FALSE;
 *
 * Abort:   None.
 *
 **************/
swi_bool swi_osapiipcclose(struct swi_osipcapi *picp)
{
    /* Close socket */
    return (close(picp->osipcsocket) == 0);
}

/*************
 *
 * Name: swi_osapiipcwrite
 *
 * Purpose: Write a message to an IPC channel
 *
 * Params:  ip      - pointer to IPC structure
 *          bufp    - pointer to buffer to write
 *          length  - size of buffer to write (in bytes)
 *
 * Return:  TRUE if OK, false if SDK process doesn't exist or hasn't opened
 *          its IPC channel yet.
 *
 * Abort:   none
 *
 * Notes:   swi_osipcapiopen must have been called first
 *          Writes error message using error handler
 *          Retries the first time to give the SDK process time to start
 *
 **************/
swi_bool swi_osapiipcwrite(struct swi_osipcapi *ip,
                           const void *bufp,
                           size_t length)
{
    int delay;

    delay = 0;
    for ( ; ; )
    {
        if (sendto(ip->osipcsocket, bufp, length, MSG_EOR | MSG_NOSIGNAL,
                        (struct sockaddr *)&ip->osipcdest,
                        sizeof(ip->osipcdest)) >= 0)
            break;

        if (errno == EINTR)
        {
            /* signal - just retry */
        }
        else if (errno != ECONNREFUSED)
        {
            swi_osapierror("IPC write failed", errno);
            return FALSE;
        }
        else if (delay > 0)        /* if tried once before */
        {
            return FALSE;
        }
        else
        {
            swi_osapidelay(SWI_RETRY_MS);
            delay += SWI_RETRY_MS;
        }
    }

    return TRUE;
}

/*************
 *
 * Name: swi_osapiipcread
 *
 * Purpose: Read a message from an IPC channel
 *
 * Params:  ip      - pointer to IPC structure
 *          bufp    - pointer to buffer in which to read
 *          lengthp - pointer to size of read buffer
 *          timeout - timeout in ms (0 to wait forever)
 *
 * Return:  TRUE if OK, false if serious error.
 *
 * Abort:   none
 *
 * Notes:   The number of bytes read is returned in lengthp, which is zero
 *          if the timeout occurred before a message arrived
 *
 *          There is no way to determine if the SDK process is running
 *          with this function. Just waits if not.
 *
 *          swi_osipcapiopen must have been called first
 *
 *          Writes error message using error handler
 *
 **************/
swi_bool swi_osapiipcread(struct swi_osipcapi *ip, void *bufp, size_t *lengthp, swi_uint32 timeout)
{
    ssize_t rv;
    struct timeval times;
    struct timeval *timesp;
    fd_set fdmask;

    if (timeout != 0)
    {
        timesp = &times;
        times.tv_sec = timeout / 1000UL;
        times.tv_usec = (timeout * 1000UL) % 1000000UL;
    }
    else
        timesp = NULL;

    /* wait for message */

    FD_ZERO(&fdmask);
    FD_SET(ip->osipcsocket, &fdmask); /* set file descriptor mask bit */
    do {
        rv = select(ip->osipcsocket + 1, &fdmask, NULL, &fdmask, timesp);
    } while (rv < 0 && errno == EINTR);

    if (rv < 0)
    {
        swi_osapierror("IPC read (s) failed", errno);
        return FALSE;
    }
    else if (rv == 0 && timeout > 0)    /* timeout */
    {
        *lengthp = 0;
        return TRUE;
    }
    /* else rv is 1 */

    /* read message */
    do
    {
        rv = recv(ip->osipcsocket, bufp, *lengthp, MSG_NOSIGNAL);
    } while (rv < 0 && errno == EINTR);

    if (rv < 0)
    {
        *lengthp = 0;
        swi_osapierror("IPC read (r) failed", errno);
        return FALSE;
    }

    *lengthp = (size_t)rv;
    return TRUE;
}

/*************
 *
 * Name: swi_osapithreadcreate
 *
 * Purpose: Create a thread in the current process
 *
 * Params:  fnp   - pointer to function that comprises the thread
 *
 * Return:  No value
 *
 * Abort:   Out of memory; can't create.
 *
 * Notes:   Needed by the test application.
 *
 **************/
void swi_osapithreadcreate(void (*fnp)(void))
{
    pthread_attr_t attr;
    pthread_t tid;

    if ( pthread_attr_init(&attr) < 0)
        swi_osapiaborterror("thread create (i) failed", errno);

    /* PowerPC's PTHREAD_STACK_MIN defaults to 131072 (128K),
     * double our minimum.
     */
#ifdef PPC
    if ( pthread_attr_setstacksize (&attr, 2*SWI_STACK_SIZE) !=0)
#else
    if ( pthread_attr_setstacksize (&attr, SWI_STACK_SIZE) !=0)
#endif
        swi_osapiaborterror("thread create (s) failed", errno);

    if ( pthread_attr_setdetachstate(&attr,
                        PTHREAD_CREATE_DETACHED) < 0)
        swi_osapiaborterror("thread create (d) failed", errno);

#ifndef ANDROID_NO_STACKDUMP
    /* Not directly related to stack dump, but this feature is
     * not available on Android platforms either
     */
    if ( pthread_attr_setinheritsched(&attr,
                        PTHREAD_INHERIT_SCHED) < 0)
        swi_osapiaborterror("thread create (v) failed", errno);
#endif

    if (pthread_create(&tid, &attr, (void *(*)(void *))fnp, NULL) < 0)
        swi_osapiaborterror("thread create (c) failed", errno);
}

/*************
 *
 * Name:    swi_osapi_stat
 *
 * Purpose: wrapper for unix stat system call
 *
 * Params:  path(IN)   - path to stat
 *
 * Return:  return value of stat system call
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
global int swi_osapi_stat(
    const char *path )
{
    struct stat buf;
    int rv;

    rv = stat(path, &buf);
    if(rv)
    {
        perror(__func__);
    }
    return rv;
}

/*************
 *
 * Name:    swi_osapi_readdir
 *
 * Purpose: wrapper for unix readdir system call
 *
 * Params:  (IN/OUT)    pswidir - swi directory structure
 *
 * Return:  success: TRUE
 *          failure: FALSE
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
global swi_bool swi_osapi_readdir(
    struct swi_osdirapi *pswidir )
{
    pswidir->pentry = readdir( pswidir->pdir );
    if( NULL == pswidir->pentry ) /* end of stream */
    {
        return FALSE;
    }
    return TRUE;
}

/*************
 *
 * Name:    swi_osapi_opendir
 *
 * Purpose: wrapper for unix opendir system call
 *
 * Params:  (IN)    path    - path
 *          (OUT)   pswidir - swi directory structure
 *
 * Return:  success: TRUE
 *          failure: FALSE
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
global swi_bool swi_osapi_opendir(
    const swi_char   *path,
    struct swi_osdirapi *pswidir )
{
    pswidir->pdir = opendir(path);

    /* process all files at the specified path */
    if( pswidir->pdir == NULL )
    {
        return FALSE;
    }

    return TRUE;
}

/*************
 *
 * Name:    swi_osapi_closedir
 *
 * Purpose: wrapper for unix opendir system call
 *
 * Params:  (IN)    path    - path
 *          (OUT)   pswidir - swi directory structure
 *
 * Return:  success: TRUE
 *          failure: FALSE
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
global swi_bool swi_osapi_closedir(
    struct swi_osdirapi *pswidir )
{
    if( NULL != pswidir )
    {
        if( 0 != closedir(pswidir->pdir) )
        {
            return FALSE;
        }
    }

    return TRUE;
}

/*************
 *
 * Name:    swi_osapi_system
 *
 * Purpose: wrapper for system library call
 *
 * Params:  (IN)    str     - system call string
 *
 * Return:  system call return code
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
global swi_int32 swi_osapi_system(
    const char *str )
{
    return system(str);
}

/*************
 *
 * Name:    swi_osapi_mutexattr_settype
 *
 * Purpose: Wrapper function to set attribute type of mutex.
 *
 * Params:  attr - Pointer to mutex attribute
 *
 * Return:  const pointer to mutex attribute
 *
 * Abort:   On error conditions.
 *
 * Notes:   None
 *
 **************/
local const pthread_mutexattr_t* swi_osapi_mutexattr_settype(
    pthread_mutexattr_t *attr )
{
    int lerrno = 0;
    int type = PTHREAD_MUTEX_ERRORCHECK;
    lerrno = pthread_mutexattr_settype( attr, type );
    if ( lerrno != 0 )
    {
        /* Abort on error conditions */
        swi_osapiaborterror("mutex attribute set type failed", lerrno);
    }
    return attr;
}

/*************
 *
 * Name:    swi_osapi_mutexinit
 *
 * Purpose: Initialize the mutex structure before it is used
 *
 * Params:  mp - Pointer to mutex structure
 *
 * Return:  No value
 *
 * Abort:   On error conditions
 *
 * Notes:   Must be called prior to using the mutex structure.
 *
 **************/
void swi_osapi_mutexinit(
    struct swi_osmutex *mp )
{
    int lerrno;
    lerrno = pthread_mutex_init(&mp->osmutex,
                               swi_osapi_mutexattr_settype(&mp->osmutex_attr));
    if ( lerrno != 0 )
    {
        /* Abort on error conditions */
        swi_osapiaborterror("mutex initialize failed", lerrno);
    }
}

/*************
 *
 * Name:    swi_osapi_mutexlock
 *
 * Purpose: Obtain exclusive access to the mutex, waiting if necessary
 *
 * Params:  mp - Pointer to mutex structure
 *
 * Return:  No value
 *
 * Abort:   On error conditions
 *
 * Notes:   The mutex structure must have been initialized via swi_osapi_mutexinit
 *
 **************/
void swi_osapi_mutexlock(
    struct swi_osmutex *mp )
{
    int lerrno;
    lerrno = pthread_mutex_lock( &mp->osmutex );
    if ( lerrno != 0 )
    {
        /* Abort on error conditions */
        swi_osapiaborterror("mutex lock failed", lerrno);
    }
}

/*************
 *
 * Name:    swi_osapi_mutexunlock
 *
 * Purpose: Release exclusive access to the mutex
 *
 * Params:  mp - Pointer to mutex structure
 *
 * Return:  No value
 *
 * Abort:   On error conditions
 *
 * Notes:   The mutex structure must have been initialized
 *          via swi_osapi_mutexinit
 *
 **************/
void swi_osapi_mutexunlock(
    struct swi_osmutex *mp )
{
    int lerrno;
    lerrno = pthread_mutex_unlock( &mp->osmutex );
    if ( lerrno != 0 )
    {
        /* Abort on error conditions */
        swi_osapiaborterror("mutex unlock failed", lerrno);
    }
}

/*************
 *
 * Name:    swi_osapi_mutexdestroy
 *
 * Purpose: Destroy the mutex
 *
 * Params:  mp - Pointer to mutex
 *
 * Return:  No value
 *
 * Abort:   On error conditions.
 *
 * Notes:   The mutex structure must have been initialized
 *          via swi_osapi_mutexinit
 *
 **************/
void swi_osapi_mutexdestroy(
    pthread_mutex_t *mp )
{
    int lerrno;
    lerrno = pthread_mutex_destroy( mp );
    if ( lerrno != 0 )
    {
        /* Abort on error conditions */
        swi_osapiaborterror("mutex destroy failed", lerrno);
    }
}

