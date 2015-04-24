/* 
 * This source code is "Not a Contribution" under Apache license
 *
 * Sierra Wireless RIL
 *
 * Based on reference-ril by The Android Open Source Project
 * and U300 RIL by ST-Ericsson.
 * Modified by Sierra Wireless, Inc.
 *
 * Copyright (C) 2011 Sierra Wireless, Inc.
 * Copyright (C) ST-Ericsson AB 2008-2009
 * Copyright 2006, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Based on reference-ril by The Android Open Source Project.
 *
 * Heavily modified for ST-Ericsson U300 modems.
 * Author: Christian Bejram <christian.bejram@stericsson.com>
 */

#include <telephony/ril.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <alloca.h>
#include <getopt.h>
#include <sys/socket.h>
#include <cutils/sockets.h>
#include <termios.h>
#include <cutils/properties.h>
#include <signal.h>
#include <sys/wait.h>

#include "at_channel.h"
#include "at_tok.h"
#include "at_misc.h"

#include "SWIWWANCMAPI.h"
#include "swiril_main.h"
#include "swiril_config.h"
#include "swiril_gps.h"
#include "swiril_network.h"
#include "swiril_sim.h"
#include "swiril_requestdatahandler.h"
#include "swiril_cache.h"
#include "swims_ossdkuproto.h"
#include "swiril_oem.h"
#include "swiril_misc.h"
#include "swiril_sim_qmi.h"
#include "swiril_misc_qmi.h"
#include "swiril_network_qmi.h"

/* For QMI */
#include "swiril_main_qmi.h"
#include "swiril_sms_qmi.h"
#include "swi_osapi.h"
#include "qmerrno.h"

#define LOG_TAG "RIL"
#include "swiril_log.h"

#define RIL_VERSION_STRING  "Sierra Ril V2.1.0.7 QMI"

#define MAX_AT_RESPONSE 0x1000

#define timespec_cmp(a, b, op)         \
        ((a).tv_sec == (b).tv_sec    \
        ? (a).tv_nsec op (b).tv_nsec \
        : (a).tv_sec op (b).tv_sec)

#define IMEISV_MC77XX_STRING  "IMEI SV: "
#define IMEISV_MC83XX_STRING  "SVN: "
#define IMEI_SV_STRING_LENGTH 20

/* RIL solicited command start from 1 */
#define RIL_SOL_NUMBER_MAX  (RIL_REQUEST_REPORT_STK_SERVICE_IS_RUNNING + 1)

/* AT Port Closing timeout (sec). The AT port is opened once at the start
 * of a transaction and remains open until this timeout elapses. The value 
 * is passed to the AT handler when at_open() is called during queueRunner
 * initialization. Units are Seconds
 */
#define AT_PORT_CLOSE_TIMEOUT 12

/*** Declarations ***/
static void onRequest(int request, void *data, size_t datalen,
                      RIL_Token t);


static int isRadioOn();
static void signalCloseQueues(void);
extern const char *requestToString(int request);

/*** Static Variables ***/
static const RIL_RadioFunctions s_callbacks = {
    RIL_VERSION,
    onRequest,
    currentState,
    onSupports,
    onCancel,
    getVersion
};

static RIL_RadioState sState = RADIO_STATE_UNAVAILABLE;

static pthread_mutex_t s_state_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t s_screen_state_mutex = PTHREAD_MUTEX_INITIALIZER;
int s_screenState = 1;

static RIL_RequestHandlerFunc s_rilsolreqhandler[RIL_SOL_NUMBER_MAX];
static RIL_RequestHandlerFunc s_rilsolreqhandlercdma[RIL_SOL_NUMBER_MAX];

typedef struct RILRequest {
    int request;
    void *data;
    size_t datalen;
    RIL_Token token;
    struct RILRequest *next;
} RILRequest;

typedef struct RILEvent {
    void (*eventCallback) (void *param);
    void *param;
    struct timespec abstime;
    struct RILEvent *next;
    struct RILEvent *prev;
} RILEvent;

typedef struct RequestQueue {
    pthread_mutex_t queueMutex;
    pthread_cond_t cond;
    RILRequest *requestList;
    RILEvent *eventList;
    char enabled;
    char closed;
} RequestQueue;

static RequestQueue s_requestQueue = {
    .queueMutex = PTHREAD_MUTEX_INITIALIZER,
    .cond = PTHREAD_COND_INITIALIZER,
    .requestList = NULL,
    .eventList = NULL,
    .enabled = 1,
    .closed = 1
};

static RequestQueue s_requestQueuePrio = {
    .queueMutex = PTHREAD_MUTEX_INITIALIZER,
    .cond = PTHREAD_COND_INITIALIZER,
    .requestList = NULL,
    .eventList = NULL,
    .enabled = 0,
    .closed = 1
};

static RequestQueue *s_requestQueues[] = {
    &s_requestQueue,
    &s_requestQueuePrio
};

static const struct timeval TIMEVAL_0 = { 0, 0 };
static const struct timeval TIMEVAL_RADIO_POWER_POLL = { 2, 0 };
static const struct timeval TIMEVAL_GET_SMS_LIST = { 15, 0 };

/* Need to keep track of whether the modem supports Circuit Switched (CS)
   or is only Packet Switched (PS)
 */
static int s_modem_ps_only = 0;

/**
 *
 * Determine which technology is using by module
 *
 * @return
 *     enum SWI_RunningTech of technology
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     this function will also return false if the current or realtime 
 *     technology is not known, because the modem is not registered yet.  
 *     Thus, the default is GSM until the modem is registered.
 *
 */
static SWI_RunningTech getRunningTechnology(void)
{
    SWI_FW_INFO_TYPE technology;
    int runningtech = SWI_RUNNING_TECH_UNKNOWN;
    
    if (getFirmwareInfoTechQMI(&technology)) {
        switch(technology) {
            case SWI_FW_INFO_TYPE_GOBI_CDMA:
                LOGV("getRunningTechnology: CDMA\n");
                runningtech = SWI_RUNNING_TECH_CDMA;
                break;
                
            case SWI_FW_INFO_TYPE_GOBI_UMTS:
            case SWI_FW_INFO_TYPE_SWI_UMTS_ONLY:
                LOGV("getRunningTechnology: GSM/UMTS\n");
                runningtech = SWI_RUNNING_TECH_GSM;
                break;
                
            /* SWI_TBD the current decision is using RIL handler 
             * matching with the module running technology for MC7750 
             * liked modules, althoug it might cause mismatch in case of 
             * active handoff between LTE and eHRPD */
            case SWI_FW_INFO_TYPE_SWI_DUAL_MODE:
                if (isDualModeRunningLTE()) {
                    runningtech = SWI_RUNNING_TECH_GSM;
                }
                else {
                    runningtech = SWI_RUNNING_TECH_CDMA;
                }
                break;
                
            default:
                LOGE("getRunningTechnology: unexpected handler technology: %d!!!", runningtech);
                runningtech = SWI_RUNNING_TECH_UNKNOWN;
                break;
        }
    }
    
    return runningtech;
}

/**
 *
 * Determine whether modem is set for PS only
 *
 * @return
 *     TRUE: modem is PS only
 *     FALSE: modem is configured for PS and CS.
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 *
 */
int is_modem_ps_only(void)
{
    return s_modem_ps_only;
}


/**
 * Enqueue a RILEvent to the request queue. isPrio specifies in what queue
 * the request will end up.
 *
 * 0 = the "normal" queue, 1 = prio queue and 2 = both. If only one queue
 * is present, then the event will be inserted into that queue.
 */
void enqueueRILEvent(int isPrio, void (*callback) (void *param), 
                     void *param, const struct timeval *relativeTime)
{
    struct timeval tv;
    char done = 0;
    RequestQueue *q = NULL;

    RILEvent *e = malloc(sizeof(RILEvent));
    memset(e, 0, sizeof(RILEvent));

    e->eventCallback = callback;
    e->param = param;

    if (relativeTime == NULL) {
        relativeTime = alloca(sizeof(struct timeval));
        memset((struct timeval *) relativeTime, 0, sizeof(struct timeval));
    }
    
    gettimeofday(&tv, NULL);

    e->abstime.tv_sec = tv.tv_sec + relativeTime->tv_sec;
    e->abstime.tv_nsec = (tv.tv_usec + relativeTime->tv_usec) * 1000;

    if (e->abstime.tv_nsec > 1000000000) {
        e->abstime.tv_sec++;
        e->abstime.tv_nsec -= 1000000000;
    }

    if (!s_requestQueuePrio.enabled || 
        (isPrio == RIL_EVENT_QUEUE_NORMAL || isPrio == RIL_EVENT_QUEUE_ALL)) {
        q = &s_requestQueue;
    } else if (isPrio == RIL_EVENT_QUEUE_PRIO) {
        q = &s_requestQueuePrio;
    }

again:
    pthread_mutex_lock(&q->queueMutex);

    if (q->eventList == NULL) {
        q->eventList = e;
    } else {
        if (timespec_cmp(q->eventList->abstime, e->abstime, > )) {
            e->next = q->eventList;
            q->eventList->prev = e;
            q->eventList = e;
        } else {
            RILEvent *tmp = q->eventList;
            do {
                if (timespec_cmp(tmp->abstime, e->abstime, > )) {
                    tmp->prev->next = e;
                    e->prev = tmp->prev;
                    tmp->prev = e;
                    e->next = tmp;
                    break;
                } else if (tmp->next == NULL) {
                    tmp->next = e;
                    e->prev = tmp;
                    break;
                }
                tmp = tmp->next;
            } while (tmp);
        }
    }
    
    pthread_cond_broadcast(&q->cond);
    pthread_mutex_unlock(&q->queueMutex);

    if (s_requestQueuePrio.enabled && isPrio == RIL_EVENT_QUEUE_ALL && !done) {
        RILEvent *e2 = malloc(sizeof(RILEvent));
        memcpy(e2, e, sizeof(RILEvent));
        e = e2;
        done = 1;
        q = &s_requestQueuePrio;

        goto again;
    }

    return;
}

/**
 *
 * Turn on/off AT unsolicited notifications controlled by RIL ScreenState command.
 *
 * @param on_off
 *     if TRUE, screen is on and notifications should be enabled.
 *
 * @return
 *     zero     : success
 *     non-zero : failure
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 *
 */
static int setATUnsolNotifState(int on_off)
{
/* SWI_TBD in transition to QMI API */
#if 0
    int err=0;

    if (on_off) {
        /* Screen is on - enable all unsolicited notifications */

        /* Subscribe to network registration events. 
         *  n = 2 - Enable network registration and location information 
         *          unsolicited result code +CREG: <stat>[,<lac>,<ci>] 
         */
        err = at_send_command("AT+CREG=2", NULL);
        if (err < 0)
            return err;

        /* Configure Packet Domain Network Registration Status events
         *    2 = Enable network registration and location information
         *        unsolicited result code
         */
        err = at_send_command("AT+CGREG=2", NULL);
        if (err < 0)
            return err;

        /* Subscribe to Packet Domain Event Reporting.
         *  mode = 1 - Discard unsolicited result codes when ME-TE link is reserved
         *             (e.g. in on-line data mode); otherwise forward them directly
         *             to the TE.
         *   bfr = 0 - MT buffer of unsolicited result codes defined within this
         *             command is cleared when <mode> 1 is entered.
         */
        err = at_send_command("AT+CGEREP=2,0", NULL);
        if (err < 0)
            return err;

        /* Configure Mobile Equipment Event Reporting.
         *  mode = 1 - discard unsolicited result codes when TA-TE link is reserved
         *             (e.g. in on-line data mode); otherwise forward them directly
         *             to the TE.
         *  keyp = 0 - no keypad event reporting.
         *  disp = 0 - no display event reporting.
         *   ind = 1 - indicator event reporting using result code +CIEV: <ind>,<value>.
         *             <ind> indicates the indicator order number (as specified for
         *             +CIND) and <value> is the new value of indicator. Only those
         *             indicator events, which are not caused by +CIND shall be
         *             indicated by the TA to the TE.
         *   bfr = 0 - TA buffer of unsolicited result codes defined within this
         *             command is cleared when <mode> 1...3 is entered.
         */
        err = at_send_command("AT+CMER=1,0,0,1,0", NULL);
        if (err < 0)
            return err;

    } else {
        /* Screen is off - disable all unsolicited notifications. */

        /* SWI_TBD
           According to the comment in ril.h, it is only cell updates that
           should be disabled, in which case +CREG and +CGREG should be set
           to 1 instead of 0.  This would also correspond better with the
           location updates command. Need to think about this some more.
         */
        err = at_send_command("AT+CREG=0", NULL);
        if (err < 0)
            return err;
        err = at_send_command("AT+CGREG=0", NULL);
        if (err < 0)
            return err;
        err = at_send_command("AT+CGEREP=0,0", NULL);
        if (err < 0)
            return err;
        err = at_send_command("AT+CMER=0,0,0,0,0", NULL);
        if (err < 0)
            return err;
    }
#endif
    /* everything was okay, as indicated by a return value of zero */
    return 0;
}

/** Do post-AT+CFUN=1 initialization. */
static void onRadioPowerOn()
{
    resetpollSIMRetry();
    enqueueRILEvent(RIL_EVENT_QUEUE_PRIO, pollSIMState, NULL, NULL);
}

/** Do post- SIM ready initialization. */
static void onSIMReady()
{
    int err = 0;

    /* Enable automatic time zone update via NITZ */
    err = at_send_command("AT+CTZU=1", NULL);
    if (err < 0)
        LOGE("%s AT+CTZU failed", __func__);

#if 0
    ATResponse *atresponse = NULL;
    int err = 0;

    /* If the screen is currently on, then turn on all AT unsolicited notifications
       related to the ScreenState command. We don't need locking here, since the
       return value fits into a single word.

       IMPORTANT: This relies on the initial value of the screen to be on, so that
       these get enabled during system initialization.
     */
    if (getScreenState()) {
        setATUnsolNotifState(1);
    }

    /* SWI_TBD
       Should TZ reporting be turned off when the screen is off.  If it is, then
       a fake NITZ event should be generated when the screen is turned back on.
     */
    /* Configure time zone change event reporting
     *    1 = Enable time zone change event reporting.
     */
    at_send_command("AT+CTZR=1", NULL);
#endif
}

/**
 * RIL_REQUEST_GET_IMSI
*/
static void requestGetIMSI(void *data, size_t datalen, RIL_Token t)
{
    ATResponse *atresponse = NULL;
    int err;

    err = at_send_command_numeric("AT+CIMI", &atresponse);

    if (err < 0 || atresponse->success == 0) {
        RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    } else {
        /* Return the successful response and cache the value for next time */
        swiril_cache_RIL_onRequestComplete(t, RIL_E_SUCCESS, 
                                           atresponse->p_intermediates->line,
                                           sizeof(char *),
                                           RIL_REQUEST_GET_IMSI);
    }
    at_response_free(atresponse);
    return;
}

/**
 * Get IMEISV
*/
int getIMEISV(char **datap)
{
    ATResponse *atresponse = NULL;
    char *bufp = NULL;
    int  err;
    char *imeiSVString = NULL;
    SWI_FW_INFO_TYPE technology;

    if(IsSierraDevice()) {
        imeiSVString = IMEISV_MC77XX_STRING;
    } else {
        imeiSVString = IMEISV_MC83XX_STRING;
    }

    /* ATI response after IMEI SV will go to onUnsolicited() */
    err = at_send_command_singleline("ATI", imeiSVString, &atresponse);
    if (err < 0 || atresponse->success == 0) 
    {
        at_response_free(atresponse);
        return -1;
    }

    bufp = strstr(atresponse->p_intermediates->line, imeiSVString);
    if (bufp != NULL)
    {
        asprintf(datap, "%s", bufp + strlen(imeiSVString));
    }
    else
    {
        datap = NULL;
    }
    at_response_free(atresponse);

    return err;
}

/* RIL_REQUEST_DEVICE_IDENTITY
 *
 * Request the device ESN / MEID / IMEI / IMEISV.
 *
 */
static void requestDeviceIdentity(void *data, size_t datalen, RIL_Token t)
{
    ATResponse *atresponse = NULL;
    char* response[4];
    int err;
    int i;

    /* IMEI */ 
    err = at_send_command_numeric("AT+CGSN", &atresponse);

    if (err < 0 || atresponse->success == 0) {
        goto error;
    } else {
        asprintf(&response[0], "%s", atresponse->p_intermediates->line);
    }

    /* IMEISV */
    err = getIMEISV(&response[1]);

    if (err < 0) {
        goto error;
    }

    /* CDMA not supported */
    response[2] = NULL;
    response[3] = NULL;

    /* Return the successful response and cache the value for next time */
    swiril_cache_RIL_onRequestComplete(t, RIL_E_SUCCESS,
                                       &response,
                                       sizeof(response),
                                       RIL_REQUEST_DEVICE_IDENTITY);

finally:
    for (i=0; i<4; i++)
    {
        if (response[i] != NULL)
        {
            free(response[i]);
        }
    }
    at_response_free(atresponse);
    return;

error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    goto finally;
}

/* Deprecated */
/**
 * RIL_REQUEST_GET_IMEI
 *
 * Get the device IMEI, including check digit.
*/
static void requestGetIMEI(void *data, size_t datalen, RIL_Token t)
{
    ATResponse *atresponse = NULL;
    int err;

    err = at_send_command_numeric("AT+CGSN", &atresponse);

    if (err < 0 || atresponse->success == 0) {
        RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    } else {
        /* Return the successful response and cache the value for next time */
        swiril_cache_RIL_onRequestComplete(t, RIL_E_SUCCESS,
                                           atresponse->p_intermediates->line,
                                           sizeof(char *),
                                           RIL_REQUEST_GET_IMEI);
    }
    at_response_free(atresponse);
    return;
}

/* Deprecated */
/**
 * RIL_REQUEST_GET_IMEISV
 *
 * Get the device IMEISV, which should be two decimal digits.
*/
static void requestGetIMEISV(void *data, size_t datalen, RIL_Token t)
{
    int err;
    char *response = NULL;

    err = getIMEISV(&response);

    if (err < 0) 
    {
        RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE,
                              NULL,
                              0);
    }
    else
    {
        /* Return the successful response and cache the value for next time.
           Note that the response could be a NULL string.
         */
        swiril_cache_RIL_onRequestComplete(t, RIL_E_SUCCESS,
                                           response,
                                           sizeof(char *),
                                           RIL_REQUEST_GET_IMEISV);
    }
    if (response != NULL)
        free(response);
}

/**
 * RIL_REQUEST_RADIO_POWER
 *
 * Toggle radio on and off (for "airplane" mode).
*/
static void requestRadioPower(void *data, size_t datalen, RIL_Token t)
{
    int onOff;
    int err;
    ATResponse *atresponse = NULL;

    assert(datalen >= sizeof(int *));
    onOff = ((int *) data)[0];

    if (onOff == 0 && sState != RADIO_STATE_OFF) {
        err = at_send_command("AT+CFUN=0", &atresponse);
        if (err < 0 || atresponse->success == 0)
            goto error;
        setRadioState(RADIO_STATE_OFF);
    } else if (onOff > 0 && sState == RADIO_STATE_OFF) {
        err = at_send_command("AT+CFUN=1", &atresponse);
        if (err < 0 || atresponse->success == 0) {
            goto error;
        }
        setRadioState(RADIO_STATE_SIM_NOT_READY);
    } else {
        LOGE("Erroneous input to requestRadioPower(): onOff=%i, sState=%i", onOff, sState);
        goto error;
    }
    
    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
finally:
    at_response_free(atresponse);
    return;
error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    goto finally;
}

/**
 * Will LOCK THE MUTEX! MAKE SURE TO RELEASE IT!
 */
void getScreenStateLock(void)
{
    /* Just make sure we're not changing anything with regards to screen state. */
    pthread_mutex_lock(&s_screen_state_mutex);
}

int getScreenState(void)
{
    return s_screenState;
}

void releaseScreenStateLock(void)
{
    pthread_mutex_unlock(&s_screen_state_mutex);
}

/**
 * RIL_REQUEST_BASEBAND_VERSION
 *
 * Return string value indicating baseband version, eg
 * response from AT+CGMR.
*/
static void requestBasebandVersion(void *data, size_t datalen, RIL_Token t)
{
    int err;
    ATResponse *atresponse = NULL;
    char *line;

    /* The AT command output is a single line with no initial prefix string,
       so use the empty string as the response prefix.
     */
    err = at_send_command_singleline("AT+CGMR", "", &atresponse);

    if (err < 0 || 
        atresponse->success == 0 || 
        atresponse->p_intermediates == NULL) {
        goto error;
    }

    line = atresponse->p_intermediates->line;

    /* Return the successful response and cache the value for next time */
    swiril_cache_RIL_onRequestComplete(t, RIL_E_SUCCESS, line, sizeof(char *),
                                       RIL_REQUEST_BASEBAND_VERSION);

finally:
    at_response_free(atresponse);
    return;

error:
    LOGE("Error in requestBasebandVersion()");
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    goto finally;
}

static char isPrioRequest(int request)
{
    unsigned int i;
    for (i = 0; i < sizeof(prioRequests) / sizeof(int); i++)
        if (request == prioRequests[i])
            return 1;
    return 0;
}

static void processRequest(int request, void *data, size_t datalen, RIL_Token t)
{
    LOGE("processRequest: %s", requestToString(request));

    /* Ignore all requests except RIL_REQUEST_GET_SIM_STATUS
     * when RADIO_STATE_UNAVAILABLE.
     */
    if (sState == RADIO_STATE_UNAVAILABLE
        && request != RIL_REQUEST_GET_SIM_STATUS) {
        RIL_onRequestComplete(t, RIL_E_RADIO_NOT_AVAILABLE, NULL, 0);
        return;
    }

    /* Ignore all non-power requests when RADIO_STATE_OFF
     * (except RIL_REQUEST_GET_SIM_STATUS and a few more).
     */
    if ((sState == RADIO_STATE_OFF || sState == RADIO_STATE_SIM_NOT_READY)
        && !(request == RIL_REQUEST_RADIO_POWER || 
             /*not allow to get SIM status if SIM not ready
               because if we are in SIM ready delay stage,  
               get SIM status will set SIM status to ready */
             /*request == RIL_REQUEST_GET_SIM_STATUS ||*/
             request == RIL_REQUEST_GET_IMEISV ||
             request == RIL_REQUEST_GET_IMEI ||
             request == RIL_REQUEST_BASEBAND_VERSION || 
             request == RIL_REQUEST_OEM_HOOK_STRINGS ||
             request == RIL_REQUEST_DEVICE_IDENTITY)) {
        RIL_onRequestComplete(t, RIL_E_RADIO_NOT_AVAILABLE, NULL, 0);
        return;
    }
    
    /* 
     * These commands won't accept RADIO_NOT_AVAILABLE, so we just return
     * GENERIC_FAILURE if we're not in SIM_STATE_READY or NV_STATE_READY.
     */
    if ( (sState != RADIO_STATE_SIM_READY && sState != RADIO_STATE_NV_READY && sState != RADIO_STATE_RUIM_READY)
        && (request == RIL_REQUEST_WRITE_SMS_TO_SIM ||
            request == RIL_REQUEST_DELETE_SMS_ON_SIM ||
            request == RIL_REQUEST_SCREEN_STATE)) {
        RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
        return;
    }

    /* Don't allow radio operations when sim is absent or locked! */
    if (sState == RADIO_STATE_SIM_LOCKED_OR_ABSENT
        && !(request == RIL_REQUEST_ENTER_SIM_PIN ||
             request == RIL_REQUEST_ENTER_SIM_PUK ||
             request == RIL_REQUEST_ENTER_SIM_PIN2 ||
             request == RIL_REQUEST_ENTER_SIM_PUK2 ||
             request == RIL_REQUEST_ENTER_NETWORK_DEPERSONALIZATION ||
             request == RIL_REQUEST_GET_SIM_STATUS ||
             request == RIL_REQUEST_RADIO_POWER ||
             request == RIL_REQUEST_GET_IMEISV ||
             request == RIL_REQUEST_GET_IMEI ||
             request == RIL_REQUEST_BASEBAND_VERSION ||
             request == RIL_REQUEST_QUERY_FACILITY_LOCK ||
             request == RIL_REQUEST_SET_FACILITY_LOCK ||
             request == RIL_REQUEST_OEM_HOOK_STRINGS ||
             request == RIL_REQUEST_DEVICE_IDENTITY)) {
        RIL_onRequestComplete(t, RIL_E_RADIO_NOT_AVAILABLE, NULL, 0);
        return;
    }

    /* If the command could affect network state, clear the network cache data
     */
    if (request == RIL_REQUEST_RADIO_POWER ||
        request == RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC ||
        request == RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL ||
        request == RIL_REQUEST_SET_BAND_MODE ||
        request == RIL_REQUEST_SET_PREFERRED_NETWORK_TYPE) {

        swiril_cache_clearnetworkdata();
    }

    /* If the RIL command supports caching, check the cache first */
    if (request == RIL_REQUEST_REGISTRATION_STATE ||
        request == RIL_REQUEST_GPRS_REGISTRATION_STATE ||
        request == RIL_REQUEST_SIGNAL_STRENGTH ||
        request == RIL_REQUEST_OPERATOR ||
        request == RIL_REQUEST_QUERY_NETWORK_SELECTION_MODE ||
        request == RIL_REQUEST_GET_IMSI ||
        request == RIL_REQUEST_DEVICE_IDENTITY ||
        request == RIL_REQUEST_GET_IMEI ||
        request == RIL_REQUEST_GET_IMEISV ||
        request == RIL_REQUEST_BASEBAND_VERSION) {

        void *responsep;
        int responselen;
        
        swiril_cache_get(request, &responsep, &responselen);
        if (responsep != NULL) {
            RIL_onRequestComplete(t, RIL_E_SUCCESS, responsep, responselen);
            return;
        }
    }

    /* check handler struct first, call handler if existing */
    if ((request > 0) && (request < RIL_SOL_NUMBER_MAX)) { 
        if (getRunningTechnology() != SWI_RUNNING_TECH_UNKNOWN) {
            if ((getRunningTechnology() == SWI_RUNNING_TECH_GSM) && (s_rilsolreqhandler[request] != NULL)) {
                s_rilsolreqhandler[request](data, datalen, t);
                return;
            }
            else if (getRunningTechnology() == SWI_RUNNING_TECH_CDMA) {
                if (s_rilsolreqhandlercdma[request] != NULL) {
                    s_rilsolreqhandlercdma[request](data, datalen, t);
                    return;
                }
                /* fall through for RUIM SIM related of AT handle */
            }
        }
        else {
            /* initialize again */
            LOGE("processRequest: QMI handler unknown, try initialize again\n");
            initFirmwareInfoQMI();
            RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
            return;
        }
    }
    
    switch (request) {
        /* SIM Handling Requests */
        case RIL_REQUEST_SIM_IO:
            requestSIM_IO(data, datalen, t);
            break;
        case RIL_REQUEST_GET_SIM_STATUS:
            requestGetSimStatus(data, datalen, t);
            break;
        case RIL_REQUEST_QUERY_FACILITY_LOCK:
            requestQueryFacilityLock(data, datalen, t);
            break;
        case RIL_REQUEST_SET_FACILITY_LOCK:
            requestSetFacilityLock(data, datalen, t);
            break;

        /* Network Selection */
        case RIL_REQUEST_ENTER_NETWORK_DEPERSONALIZATION:
            requestEnterNetworkDepersonalization(data, datalen, t);
            break;

        default:
            LOGW("Unsupported request logged: %s",
                 requestToString(request));
            RIL_onRequestComplete(t, RIL_E_REQUEST_NOT_SUPPORTED, NULL, 0);
            break;
    }
}

/*** Callback methods from the RIL library to us ***/

/**
 * Call from RIL to us to make a RIL_REQUEST.
 *
 * Must be completed with a call to RIL_onRequestComplete().
 */
static void onRequest(int request, void *data, size_t datalen, RIL_Token t)
{
    RILRequest *r;
    char done = 0;
    RequestQueue *q = &s_requestQueue;

    if (s_requestQueuePrio.enabled && isPrioRequest(request))
        q = &s_requestQueuePrio;

    r = malloc(sizeof(RILRequest));  
    memset(r, 0, sizeof(RILRequest));

    /* Formulate a RILRequest and put it in the queue. */
    r->request = request;
    r->data = dupRequestData(request, data, datalen);
    r->datalen = datalen;
    r->token = t;

    pthread_mutex_lock(&q->queueMutex);

    /* Queue empty, just throw r on top. */
    if (q->requestList == NULL) {
        q->requestList = r;
    } else {
        RILRequest *l = q->requestList;
        while (l->next != NULL)
            l = l->next;

        l->next = r;
    }

    pthread_cond_broadcast(&q->cond);
    pthread_mutex_unlock(&q->queueMutex);
}

/**
 * Synchronous call from the RIL to us to return current radio state.
 * RADIO_STATE_UNAVAILABLE should be the initial state.
 */
RIL_RadioState currentState()
{
    return sState;
}

/**
 * Call from RIL to us to find out whether a specific request code
 * is supported by this implementation.
 *
 * Return 1 for "supported" and 0 for "unsupported".
 *
 * Currently just stubbed with the default value of one. This is currently
 * not used by android, and therefore not implemented here. We return
 * RIL_E_REQUEST_NOT_SUPPORTED when we encounter unsupported requests.
 */
int onSupports(int requestCode)
{
    LOGI("onSupports() called!");

    return 1;
}

/** 
 * onCancel() is currently stubbed, because android doesn't use it and
 * our implementation will depend on how a cancellation is handled in 
 * the upper layers.
 */
void onCancel(RIL_Token t)
{
    LOGI("onCancel() called!");
}

const char *getVersion(void)
{
    return RIL_VERSION_STRING;
}


static void pollRadioPower(void *params)
{
    LOGD("Enter pollRadioPower");
    
    /* Doesn't really poll anything */
    setRadioState(RADIO_STATE_NV_READY);
}

void setRadioState(RIL_RadioState newState)
{
    RIL_RadioState oldState;

    pthread_mutex_lock(&s_state_mutex);

    oldState = sState;

    if (sState != newState) {
        sState = newState;
    }

    pthread_mutex_unlock(&s_state_mutex);

    /* Do these outside of the mutex. */
    if ((sState != oldState) || 
        (sState == RADIO_STATE_SIM_LOCKED_OR_ABSENT) ||
        (sState == RADIO_STATE_RUIM_LOCKED_OR_ABSENT)) {
        RIL_onUnsolicitedResponse(RIL_UNSOL_RESPONSE_RADIO_STATE_CHANGED,
                                  NULL, 0);

        /**
         * Get the unread SMS list from the device and notify application side,
         * if any unread SMSs are present
         */
        if (sState == RADIO_STATE_SIM_READY) {
            enqueueRILEvent(RIL_EVENT_QUEUE_PRIO, onSIMReady, NULL, NULL);
            enqueueRILEvent(RIL_EVENT_QUEUE_PRIO, getSMSList, NULL,
                                                 &TIMEVAL_GET_SMS_LIST);
        } else if ((sState == RADIO_STATE_RUIM_READY) ||
                   (sState == RADIO_STATE_NV_READY)) {
            enqueueRILEvent(RIL_EVENT_QUEUE_PRIO, getSMSList, NULL,
                                                 &TIMEVAL_GET_SMS_LIST);
        } else if ((sState == RADIO_STATE_SIM_NOT_READY) ||
                   (sState == RADIO_STATE_RUIM_NOT_READY)) {
            enqueueRILEvent(RIL_EVENT_QUEUE_NORMAL, onRadioPowerOn, NULL, NULL);
        } else if (sState == RADIO_STATE_NV_NOT_READY) {
            /* add a 2 seconds delay to set RADIO_STATE_NV_READY */
            enqueueRILEvent(RIL_EVENT_QUEUE_NORMAL, pollRadioPower,
                            NULL, &TIMEVAL_RADIO_POWER_POLL);
        }
    }
}

/** Returns 1 if on, 0 if off, and -1 on error. */
static int isRadioOn()
{
    ATResponse *atresponse = NULL;
    int err;
    char *line;
    int ret;

    err = at_send_command_singleline("AT+CFUN?", "+CFUN:", &atresponse);
    if (err < 0 || atresponse->success == 0) {
        /* Assume radio is off. */
        goto error;
    }

    line = atresponse->p_intermediates->line;

    err = at_tok_start(&line);
    if (err < 0)
        goto error;

    err = at_tok_nextint(&line, &ret);
    if (err < 0)
        goto error;

    switch (ret) {
        case 1:         /* Full functionality (switched on) */
            ret = 1;
            break;

        default:
            ret = 0;
    }

    at_response_free(atresponse);
    return ret;

error:
    at_response_free(atresponse);
    return -1;
}

/** 
 * Common initialization for all AT channels.
 *
 * Note that most of the AT unsolicited result code reporting is only turned
 * on once the SIM is ready, in onSIMReady().
 */
static char initializeCommon(void)
{
    int err = 0;

    if (at_handshake() < 0) {
        LOG_FATAL("Handshake failed!");
        goto error;
    }

    /* Configure/set
     *   command echo (E), result code suppression (Q), DCE response format (V)
     *
     *  E0 = DCE does not echo characters during command state and online
     *       command state
     *  Q0 = DCE transmits result codes
     *  V1 = Display verbose result codes
     */
    err = at_send_command("ATE0Q0V1", NULL);
    if (err < 0)
        goto error;

    /* Enable +CME ERROR: <err> result code and use numeric <err> values. */
    err = at_send_command("AT+CMEE=1", NULL);
    if (err < 0)
        goto error;

    return 0;
error:
    return 1;
}

/**
 * Initialize everything that can be configured while we're still in
 * AT+CFUN=0.
 *
 * Note that most of the AT unsolicited result code reporting is only turned
 * on once the SIM is ready, in onSIMReady().
 */
static char initializeChannel(void)
{
    int err;

    LOGI("initializeChannel()");

    /* The radio should initially be off.
     * Simulate a RIL RadioPower off command
     */
    setRadioState(RADIO_STATE_OFF);

    /* Set phone functionality.
     *    0 = minimum functionality.
     */
    err = at_send_command("AT+CFUN=0", NULL);
    if (err < 0)
        goto error;

    /* If the AT+CFUN=0 does not work for some reason, but doesn't fail, and
     * the radio is still on, then start the power on process.
     *
     * Assume radio is off if isRadioOn() returns an error.
     */
    if (isRadioOn() > 0) {
        setRadioState(RADIO_STATE_SIM_NOT_READY);
    }

    return 0;

error:
    return 1;
}

/**
 * Initialize everything that can be configured while we're still in
 * AT+CFUN=0.
 *
 * Since there is no priority AT channel, there is nothing to initialize
 * here, but keep the function as a placeholder for now.
 */
static char initializePrioChannel()
{
    LOGI("initializePrioChannel()");
    return 0;
}

/**
 * Called by atchannel when an unsolicited line appears.
 * This is called on atchannel's reader thread. AT commands may
 * not be issued here.
 */
static void onUnsolicited(const char *s, const char *sms_pdu)
{
    /* Do nothing. This is a stub function */
}

static void signalCloseQueues(void)
{
    unsigned int i;
    for (i = 0; i < (sizeof(s_requestQueues) / sizeof(RequestQueue *)); i++) {
        RequestQueue *q = s_requestQueues[i];
        pthread_mutex_lock(&q->queueMutex);
        q->closed = 1;
        pthread_cond_signal(&q->cond);
        pthread_mutex_unlock(&q->queueMutex);
    }
}

/**
 *
 * Deregistration function, called when the AT handler package 
 * encounters a problem. This function used to be responsible 
 * for device detection, but for QMI devices, that responsibility
 * has been handed over to the QMI callback: "SetDeviceStateChangeCbk()"
 *
 * The reader loop (reader thread) portion of the AT package can 
 * still close down the interface if it detects a other type of 
 * error condition. Refer to "readline()" in at_channel.c for cases
 * where a NULL pointer is returned for details of specific error
 * conditions which can cause this callback to be invoked.
 *
 * @param 
 *    none
 * @return
 *    none
 * @note
 *    none
 */
static void onATReaderClosed(void)
{
    LOGI("AT channel closed\n");

    /* We don't know what's happened with the device, so clear all the data */
    swiril_cache_clearalldata();

    /* Don't inform the upper layers yet that the AT port has closed, until we
       get a better idea of why this happened.  However, do any other cleanup
       that should be done.
     */
    signalCloseQueues();
}

/**
 * 
 * Function registered to be called when device is no longer 
 * detected. The QMI callback - SetDeviceStateChangeCbk() - 
 * invokes this entry whenever the device's status, present or
 * disconnected, changes. 
 * 
 * @param [in] device_state
 *    indicates the device's current state: 
 *        DEVICE_STATE_DISCONNECTED, or
 *        DEVICE_STATE_READY
 * @return
 *    none
 * @note
 *    none
 *
 */
static void onDeviceStateChg( eDevState device_state )
{
    if( device_state == DEVICE_STATE_DISCONNECTED ) {
        LOGI("%s: Device has deregistered\n", __func__);

        /* The device has gone away so clear all the data */
        swiril_cache_clearalldata();

        /*
         * Inform the queueRunner queues that they are now closed and need
         * to pass control back to the outer loop
         */
        signalCloseQueues();
     }
     else {
        LOGI("%s: Device is ready\n", __func__);
     }
}

/* Called on command thread. */
static void onATTimeout()
{
    LOGI("AT channel timeout; restarting..\n");
    /* Last resort, throw escape on the line, close the channel
       and hope for the best. */
    at_send_escape();

    setRadioState(RADIO_STATE_UNAVAILABLE);
    signalCloseQueues();

    /* Hopefully QMI interface is still working, so try resetting modem. */
    LOGI("Try resetting the modem..\n");
    setPowerOff();
}

static void usage(char *s)
{
    fprintf(stderr, "usage: %s [-z] [-p <tcp port>] [-d /dev/tty_device] [-x /dev/tty_device] [-i <network interface>] [-o <atport open delay ms>]\n", s);
    exit(-1);
}

/** 
 * RIL request queue arguments
 */
struct queueArgs {
    int port;                   /**< port number */
    char * loophost;            /**< local host IP address */
    const char *device_path;    /**< device path */
    char isPrio;                /**< specifies which queue will be used */
    char hasPrio;               /**< has priority channel */
    char isdevicesocket;        /**< device socket flag */
    char isautodetect;          /**< auto detect modem AT port flag */
    int  atopendelay;           /**< Delay before using AT port after opening */
};

/**
 *
 * Initialize the file descriptor and execute the requests from queue
 *
 * @param[in] param 
 *     Pointer to the queueArgs structure
 *
 * @return
 *     Pointer to a void type
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 */
static void *queueRunner(void *param)
{
    int gpsfd, fd;
    int ret;
    struct queueArgs *queueArgs = (struct queueArgs *) param;
    struct RequestQueue *q = NULL;
    char * tty_name = NULL;
    char * tty_save_name = NULL;

    LOGI("queueRunner starting!");

    for (;;) {
        /* QMI port check */
        if (qmiGetConnectedDeviceID() < 0) {
            /* disconnect and re-connect again */
            LOGI("QMI port reconnecting\n");
            qmiDeviceDisconnect();
            qmiDeviceConnect();
        }
        fd = -1;
        while (fd < 0) {
            if (queueArgs->port > 0) {
                if (queueArgs->loophost) {
                    fd = socket_network_client(queueArgs->loophost, queueArgs->port, SOCK_STREAM);
                } else {
                    fd = socket_loopback_client(queueArgs->port, SOCK_STREAM);
                }

            } else if (queueArgs->isdevicesocket) {
                if (!strcmp(queueArgs->device_path, "/dev/socket/qemud")) {
                    /* Qemu-specific control socket */
                    fd = socket_local_client( "qemud",
                                              ANDROID_SOCKET_NAMESPACE_RESERVED,
                                              SOCK_STREAM );
                    if (fd >= 0 ) {
                        char  answer[2];

                        if ( write(fd, "gsm", 3) != 3 ||
                             read(fd, answer, 2) != 2 ||
                             memcmp(answer, "OK", 2) != 0)
                        {
                            close(fd);
                            fd = -1;
                        }
                    }
                }
                else
                    fd = socket_local_client( queueArgs->device_path,
                                              ANDROID_SOCKET_NAMESPACE_FILESYSTEM,
                                              SOCK_STREAM );

            } else if ((queueArgs->device_path != NULL) || (queueArgs->isautodetect != 0)) {
                if (queueArgs->isautodetect != 0) {
                    /* use modem scan for AT command port */
                    tty_name = swims_ossdkgetatifname();
                    if (tty_name != NULL) {
                        tty_name = strdup(tty_name);
                        tty_save_name = strdup(tty_name);
                        /* open the port */
                        if (tty_name != NULL) {
                            fd = open(tty_name, O_RDWR);
                        }
                    }
                }
                else {
                    if (queueArgs->device_path != NULL) {
                        fd = open(queueArgs->device_path, O_RDWR);
                        /* Make a copy of the name we used 
                         * to open the AT port
                         */
                        tty_save_name = strdup( queueArgs->device_path );
                    }
                }

                /* If the port was successfully opened and the 
                 * path to the device was either a serial port
                 * or a USB serial device that was detected by
                 * the SWIMS package...
                 */
                if ( fd >= 0 && 
                     ((queueArgs->device_path != NULL && 
                     !memcmp(queueArgs->device_path, "/dev/ttyS", 9)) ||
                     (tty_name != NULL && 
                     strstr(tty_name, "/dev/ttyUSB") != NULL))) {

                    LOGI("%s: AT port detected on %s and validated", 
                         __func__, tty_save_name);

                    /* We now know the port is available and 
                     * can be opened. Close it for now and 
                     * save a copy of the port name for later
                     * use
                     */
                    close( fd );
                }
            }

            /* If the AT Cmd port couldn't be opened */
            if (fd < 0) {
                if (queueArgs->isautodetect != 0) {
                    if (tty_name != NULL) {
                        LOGE("FAILED to open AT channel %s (%s), retrying in 2 sec.", 
                            tty_name, strerror(errno));
                    }
                    else {
                        LOGE("FAILED to auto-detect AT channel, retrying in 2 sec.");
                    }

                    /* wait 2 seconds for modem scan */
                    sleep(2);
                }
                else {
                    if (queueArgs->device_path != NULL) {
                        LOGE("FAILED to open AT channel %s (%s), retrying in 10 sec.", 
                            queueArgs->device_path, strerror(errno));
                    }
                    else {
                        LOGE("FAILED to open AT channel, retrying in 10 sec.");
                    }
                    /* wait 10 seconds when modem scan is not enabled */
                    sleep(10);
                }
            }

            if (tty_name != NULL) {
                free(tty_name);
                tty_name = NULL;
            }
        }

        /* Before opening the AT channel, make sure all the cache values
           are cleared.
         */
        swiril_cache_clearalldata();

        /* reset s_pendingSIMReady to false */
        ResetPendingSIMReady();

        /* reset s_ratCDMA to RIL_RAT_UNKNOWN*/
        setRatCDMA(RIL_RAT_UNKNOWN);

        /* Start the AT Command reader thread */
        ret = at_open(tty_save_name, onUnsolicited, 
                      queueArgs->atopendelay, AT_PORT_CLOSE_TIMEOUT);

        if (ret < 0) {
            LOGE("AT error %d on at_open\n", ret);
            at_close();
            continue;
        }

        at_set_on_reader_closed(onATReaderClosed);
        at_set_on_timeout(onATTimeout);
        
        q = &s_requestQueue;

        if(initializeCommon()) {
            LOGE("FAILED to initialize channel!");
            at_close();
            continue;
        }

        /* Install a pointer to the device state change callback */
        SetDeviceStateChangeCbk( onDeviceStateChg );

        if (queueArgs->isPrio == 0) {
            q->closed = 0;
            if (getRunningTechnology() == SWI_RUNNING_TECH_CDMA)  {
                initializeChannelCDMA();
            }
            else {
                initializeChannelUMTS();
            }

            /* Stop the DHCP client if already running */
            checkDHCPnStopService();

            at_make_default_channel();
        } else {
            q = &s_requestQueuePrio;
            q->closed = 0;
            at_set_timeout_msec(1000 * 30); 
        }

        /* If there is no priority queue, then do the priority channel init on
         * the regular channel, otherwise only do it for the priority channel.
         */
        if (queueArgs->hasPrio == 0 || queueArgs->isPrio)
            if (initializePrioChannel()) {
                LOGE("FAILED to initialize channel!");
                at_close();
                continue;
            }

        LOGE("Looping the requestQueue!");
        for (;;) {
            RILRequest *r;
            RILEvent *e;
            struct timeval tv;
            struct timespec ts;

            memset(&ts, 0, sizeof(ts));

            pthread_mutex_lock(&q->queueMutex);

            if (q->closed != 0) {
                LOGW("Device error, attempting to recover..");
                pthread_mutex_unlock(&q->queueMutex);
                break;
            }

            while (q->closed == 0 && q->requestList == NULL && 
                   q->eventList == NULL) {
                pthread_cond_wait(&q->cond,
                                  &q->queueMutex);

            }

            /* eventList is prioritized, smallest abstime first. */
            if (q->closed == 0 && q->requestList == NULL && q->eventList) {
                int err = 0;
                err = pthread_cond_timedwait(&q->cond, 
                                             &q->queueMutex, 
                                             &q->eventList->abstime);
                if (err && err != ETIMEDOUT)
                    LOGE("timedwait returned unexpected error: %s",
                         strerror(err));
            }

            if (q->closed != 0) {
                pthread_mutex_unlock(&q->queueMutex);
                continue; /* Catch the closed bit at the top of the loop. */
            }

            e = NULL;
            r = NULL;

            gettimeofday(&tv, NULL);

            /* Saves a macro, uses some stack and clock cycles.
               TODO Might want to change this. */
            ts.tv_sec = tv.tv_sec;
            ts.tv_nsec = tv.tv_usec * 1000;

            if (q->eventList != NULL &&
                timespec_cmp(q->eventList->abstime, ts, < )) {
                e = q->eventList;
                q->eventList = e->next;
            }

            if (q->requestList != NULL) {
                r = q->requestList;
                q->requestList = r->next;
            }

            pthread_mutex_unlock(&q->queueMutex);

            if (e) {
                e->eventCallback(e->param);
                free(e);
            }

            if (r) {
               processRequest(r->request, r->data, r->datalen, r->token);
               freeRequestData(r->request, r->data, r->datalen);
               free(r);
            }
        }

        at_close();
    
        /* Free the tty_save_name now */
        if (tty_save_name != NULL) {
            free(tty_save_name);
        }

        /* Inform the application that the modem has gone away */
        setRadioState(RADIO_STATE_UNAVAILABLE);

        /* Wait a bit to give the resetting AT port a chance to come back */
        sleep(2);
        /* clear Device Node and Key*/
        clearDeviceNodeAndKey();
        
        LOGI("Re-opening after close");
    }
    return NULL;
}

pthread_t s_tid_queueRunner;
pthread_t s_tid_queueRunnerPrio;

void dummyFunction(void *args)
{
    LOGE("dummyFunction: %p", args);
}

/**
 *
 * Initialize the RIL command handler function pointer
 *
 * @return
 *     None
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 */
void initRILFunctionHandler(void)
{
    int i;

    /* initialize all handler pointer to NULL */
    for (i=0; i<RIL_SOL_NUMBER_MAX; i++) {
        s_rilsolreqhandler[i] = NULL;
        s_rilsolreqhandlercdma[i] = NULL;
    }
    
    /* only support QMI API handlers for now */
    initRILFunctionHandlerQMI (&s_rilsolreqhandler[0],
                               &s_rilsolreqhandlercdma[0]);
}

/**
 *
 * Install a signal handler for one signal
 *
 * @param[in] signo
 *          signal number
 * @param[in] functionp
 *          signal handler function of type
 *          void (*functionp)(int, siginfo_t *, void *)
 *
 * @return
 *     none
 *
 * @note
 *     none
 */
void RIL_SignalInstall( unsigned int signo,
                        void (*functionp)(int, siginfo_t *, void *) )
{
    struct sigaction sa;

    if (functionp == NULL)
        LOGI("signal handler install failed: NULL signal handler");

    sa.sa_sigaction = functionp;
    sigemptyset(&sa.sa_mask);

    sa.sa_flags = SA_NODEFER | SA_SIGINFO;

    if (sigaction(signo, &sa, NULL) < 0)
        LOGI("signal handler install failed %d", errno);
}

/**
 *
 * Signal handler for SIGTERM
 *
 * @param[in] signo
 *          signal number
 * @param[in] siginfop
 *          pointer to signal info
 * @param[in] contextp
 *          pointer to interrupted user context
 *
 * @return
 *     none
 *
 * @note
 *     none
 */
void RIL_SIGTERMhandler(int signo, siginfo_t *siginfop, void *contextp)
{
    /* Disconnect from SLQS SDK */
    qmiDeviceDisconnect();

    /* simple exit */
    exit(EXIT_SUCCESS);
}

/**
 * Callback for restarting the SDK process
 *
 * @param[in] param
 *     void pointer passed into the callback. In this case NULL.
 *
 * @return
 *     none
 *
 * @note
 *     none
 */
void restartSDK(void *param)
{
    qmiSLQSRestart();

    /**
     * Get the SMS list from the device once the UIM/NV is ready; if UIM/NV not
     * ready now, the SMS list would be fetched on receiving either of
     * RADIO_STATE_SIM_READY, RADIO_STATE_RUIM_READY or RADIO_STATE_NV_READY in
     * setRadioState()
     */
    if ((sState == RADIO_STATE_SIM_READY) ||
        (sState == RADIO_STATE_RUIM_READY) ||
        (sState == RADIO_STATE_NV_READY)) {
        getSMSList(NULL);
    }
}

/**
 * Signal handler for SIGCHLD
 *
 * @param[in] signo
 *     signal number
 * @param[in] siginfop
 *     pointer to signal info
 * @param[in] contextp
 *     pointer to interrupted user context
 *
 * @return
 *     none
 *
 * @note
 *     none
 */
void RIL_SIGCHLDhandler(int signo, siginfo_t *siginfop, void *contextp)
{
    int status;
    pid_t pid;

    /* Allow the SDK process to terminate, only if there is a pid match */
    swi_osapigetprocessID( &pid );
    if( pid == siginfop->si_pid ) {
        wait(&status);
        enqueueRILEvent(RIL_EVENT_QUEUE_PRIO, restartSDK, NULL, NULL);
    }
}

/**
 * Install a signal handler for software termination signal from kill
 *
 * @param
 *     none
 *
 * @return
 *     none
 *
 * @note
 *     none
 */
void RIL_SignalInit()
{
    LOGI( "RIL_Signalinit\n" );
    RIL_SignalInstall(SIGTERM, RIL_SIGTERMhandler);
    RIL_SignalInstall(SIGCHLD, RIL_SIGCHLDhandler);
}

/**
 *
 * Initialize the RIL functions
 *
 * @param[in] env 
 *     Pointer to the RIL environment 
 * @param argc 
 *     argument count
 * @param[in] argv 
 *     Pointer to the argument strings 
 *
 * @return
 *     pointer to the RIL radio functions
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 */
const RIL_RadioFunctions *RIL_Init(const struct RIL_Env *env, int argc,
                                   char **argv)
{
    int ret;
    int opt;
    int port = -1;
    char *loophost = NULL;
    const char *device_path = NULL;
    const char *priodevice_path = NULL;
    const char *handler_method = NULL;
    int opendelay=AT_OPEN_DELAY_MS;
    char isdevicesocket = 0;
    struct queueArgs *queueArgs;
    struct queueArgs *prioQueueArgs;
    pthread_attr_t attr;
    char autodetect = 0;

    s_rilenv = env;

    LOGI("Entering RIL_Init..");

    while (-1 != (opt = getopt(argc, argv, "z:i:p:o:d:s:x:a"))) {
        switch (opt) {
            case 'z':
                loophost = optarg;
                LOGI("Using loopback host %s..", loophost);
                break;

            case 'i':
                ril_iface = optarg;
                LOGI("Using network interface %s as primary data channel.",
                     ril_iface);
                break;

            case 'p':
                port = atoi(optarg);
                if (port == 0) {
                    usage(argv[0]);
                    return NULL;
                }
                LOGI("Opening loopback port %d\n", port);
                break;

            case 'o':
                opendelay = atoi(optarg);
                if( opendelay > 750 ) {
                    opendelay = 750;
                }
                else if( opendelay < 0 ) {
                   opendelay = 0;
                }
                LOGI("AT port opening delay=%d\n", opendelay);
                break;

            case 'd':
                device_path = optarg;
                LOGI("Opening tty device %s\n", device_path);
                break;

            case 'a':
                /* '-a' specified for automatic AT port scan */
                autodetect = 1;
                /* initialize and start modem scan */
                swims_ossdkscaninit();
                LOGI("Opening tty device automatically\n");
                break;

            case 'x':
                priodevice_path = optarg;
                LOGI("Opening priority tty device %s\n", priodevice_path);
                break;

            case 's':
                device_path = optarg;
                isdevicesocket=1;
                LOGI("Opening socket %s\n", device_path);
                break;
                
            default:
                LOGE("%c is not a valid calling argument", optopt );
                usage(argv[0]);
                return NULL;
        }
    }

    if (ril_iface == NULL) {
        LOGI("Network interface was not supplied, falling back on usb0!");
        ril_iface = strdup("usb0\0");
    }

    if (port < 0 && device_path == NULL && autodetect == 0) {
        usage(argv[0]);
        return NULL;
    }

    /* Initialize the RIL deamon signal handling */
    RIL_SignalInit();

    /* Initialize the RIL command handler function pointer */
    initRILFunctionHandler();
    /* connect sdk */
    qmiDeviceConnect();

    queueArgs = malloc(sizeof(struct queueArgs));
    memset(queueArgs, 0, sizeof(struct queueArgs));

    queueArgs->device_path = device_path;
    queueArgs->port = port;
    queueArgs->loophost = loophost;
    queueArgs->isdevicesocket = isdevicesocket;
    queueArgs->atopendelay = opendelay;
    if (autodetect != 0) {
        /* set flag of modem scan */
        queueArgs->isautodetect = 1;
    }
    else {
        queueArgs->isautodetect = 0;
    }
    
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    if (priodevice_path != NULL) {
        prioQueueArgs = malloc(sizeof(struct queueArgs));
        memset(prioQueueArgs, 0, sizeof(struct queueArgs));
        prioQueueArgs->device_path = priodevice_path;
        prioQueueArgs->isPrio = 1;
        prioQueueArgs->hasPrio = 1;
        prioQueueArgs->atopendelay = opendelay;
        queueArgs->hasPrio = 1;

        s_requestQueuePrio.enabled = 1;

        pthread_create(&s_tid_queueRunnerPrio, &attr, queueRunner, prioQueueArgs);
    }

    pthread_create(&s_tid_queueRunner, &attr, queueRunner, queueArgs);

    /* Create the swiril_gps thread */
    RILGPS_Init(GPS_RILTYPE_QMI);

    return &s_callbacks;
}
