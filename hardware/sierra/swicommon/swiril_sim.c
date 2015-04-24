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
** Heavily modified for ST-Ericsson U300 modems.
** Author: Christian Bejram <christian.bejram@stericsson.com>
*/

#include <telephony/ril.h>
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <cutils/properties.h>

#include "at_channel.h"
#include "at_tok.h"
#include "fcp_parser.h"
#include "at_misc.h"
#include "swiril_main.h"
#include "swiril_sim.h"
#include "swiril_misc.h"
   
#define LOG_TAG "RIL"
#include "swiril_log.h"

typedef enum {
    SIM_ABSENT = 0,
    SIM_NOT_READY = 1,
    SIM_READY = 2,    /* SIM_READY means radio state = RADIO_STATE_SIM_READY */
    SIM_PIN = 3,
    SIM_PUK = 4,
    SIM_NETWORK_PERSONALIZATION = 5,
    SIM_FAILURE = 6
} SIM_Status; 

#define SIM_POLL_RETRY 10 
static const struct timeval TIMEVAL_SIMPOLL = { 1, 0 };
static const struct timeval TIMEVAL_SIMRESET = { 60, 0 };

typedef enum {
    SIM_PIN_TYPE,
    SIM_PUK_TYPE,
    SIM_PIN2_TYPE,
    SIM_PUK2_TYPE
} SIM_PinPukType; 

typedef struct {
    int simPIN;
    int simPUK;
    int simPIN2;
    int simPUK2;
}PINRetryCount_s;

extern void setPowerOff(void);

/**
 * Get the corresponsing PIN or PUK type from the incoming request
 *
 * @param [in] request
 *      the following are the requests which are supported
 *          RIL_REQUEST_ENTER_SIM_PIN
 *          RIL_REQUEST_ENTER_SIM_PUK
 *          RIL_REQUEST_ENTER_SIM_PIN2
 *          RIL_REQUEST_ENTER_SIM_PUK2
 *
 * @return
 *     enumeration SIM_PinPukType is returned
 *
 * @note
 *     none
 */
static SIM_PinPukType getSIMPinPukType( int request )
{
    SIM_PinPukType simPinPukType = SIM_PIN_TYPE;

    switch(request) {
        case RIL_REQUEST_ENTER_SIM_PIN:
            simPinPukType = SIM_PIN_TYPE;
            break;
        case RIL_REQUEST_ENTER_SIM_PUK:
            simPinPukType = SIM_PUK_TYPE;
            break;
        case RIL_REQUEST_ENTER_SIM_PIN2:
            simPinPukType = SIM_PIN2_TYPE;
            break;
        case RIL_REQUEST_ENTER_SIM_PUK2:
            simPinPukType = SIM_PUK2_TYPE;
            break;
    }
    return simPinPukType;
}

/**
 * Get the PIN retry count for the associated PIN PUK type requested
 *
 * @param [in] simPinPukType
 *      the associated PIN retry count 
 *
 * @return
 *     no of retries left for the PIN or PUK type requested, -1 for unknown
 *
 * @note
 *     none
 */
static int getPINRetryCount(SIM_PinPukType simPinPukType)
{
    ATResponse *atresponse = NULL;
    int err;
    char *cpincLine;
    int num_retries = -1;
    PINRetryCount_s retryCount;

    /* Set to default values : retry count is defaulted to -1[unknown] */
    retryCount.simPIN  = -1;
    retryCount.simPUK  = -1;
    retryCount.simPIN2 = -1;
    retryCount.simPUK2 = -1;

    err = at_send_command_singleline("AT+CPINC?", "+CPINC:", &atresponse);
    if ( err < 0 || atresponse->success ==0 ) {
        goto done;
    }

    cpincLine = atresponse->p_intermediates->line;
    err = at_tok_start(&cpincLine);
    if (err < 0) {
        goto done;
    }
    err = at_tok_nextint(&cpincLine, &retryCount.simPIN);
    if (err < 0) {
        goto done;
    }
    err = at_tok_nextint(&cpincLine, &retryCount.simPIN2);
    if (err < 0) {
        goto done;
    }
    err = at_tok_nextint(&cpincLine, &retryCount.simPUK);
    if (err < 0) {
        goto done;
    }
    at_tok_nextint(&cpincLine, &retryCount.simPUK2);

done:
    at_response_free(atresponse);
    switch(simPinPukType) {
        case SIM_PIN_TYPE:
            num_retries = retryCount.simPIN;
            break;
        case SIM_PIN2_TYPE:
            num_retries = retryCount.simPIN2;
            break;
        case SIM_PUK_TYPE:
            num_retries = retryCount.simPUK;
            break;
        case SIM_PUK2_TYPE:
            num_retries = retryCount.simPUK2;
            break;
    }
    return num_retries;
}

/**
 *
 * Query the SIM status
 *
 * @param [out] substate
 *     personalization substat
 *
 * @return
 *     One of enum SIM_Status
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     Returns one of SIM_*. Returns SIM_NOT_READY on error.
 *
 */
static SIM_Status getSIMStatus(RIL_PersoSubstate *substate)
{
    ATResponse *atresponse = NULL;
    int err;
    int ret = SIM_READY;
    char *cpinLine;
    char *cpinResult;

    *substate = RIL_PERSOSUBSTATE_UNKNOWN;
    if (currentState() == RADIO_STATE_OFF ||
        currentState() == RADIO_STATE_UNAVAILABLE) {
        ret = SIM_NOT_READY;
        goto done;
    }

    err = at_send_command_singleline("AT+CPIN?", "+CPIN:", &atresponse);

    if (err != 0) {
        ret = SIM_NOT_READY;
        goto done;
    }

    switch (at_get_cme_error(atresponse)) {
        case CME_SUCCESS:
            break;

        case CME_SIM_NOT_INSERTED:
            ret = SIM_ABSENT;
            goto done;

        case CME_SIM_FAILURE:
            ret = SIM_FAILURE;
            goto done;

        default:
            ret = SIM_NOT_READY;
            goto done;
    }

    /* CPIN? has succeeded, now look at the result. */

    cpinLine = atresponse->p_intermediates->line;
    err = at_tok_start(&cpinLine);

    if (err < 0) {
        ret = SIM_NOT_READY;
        goto done;
    }

    err = at_tok_nextstr(&cpinLine, &cpinResult);

    if (err < 0) {
        ret = SIM_NOT_READY;
        goto done;
    }

    if (0 == strcmp(cpinResult, "SIM PIN")) {
        ret = SIM_PIN;
        goto done;
    } else if (0 == strcmp(cpinResult, "SIM PUK")) {
        ret = SIM_PUK;
        goto done;
    } else if (0 == strcmp(cpinResult, "PH-NET PIN")) {
        *substate = RIL_PERSOSUBSTATE_SIM_NETWORK;
        return SIM_NETWORK_PERSONALIZATION;
    } else if (0 == strcmp(cpinResult, "PH-NETSUB PIN")) {
        *substate = RIL_PERSOSUBSTATE_SIM_NETWORK_SUBSET;
        return SIM_NETWORK_PERSONALIZATION;
    } else if (0 == strcmp(cpinResult, "PH-SP PIN")) {
        *substate = RIL_PERSOSUBSTATE_SIM_SERVICE_PROVIDER;
        return SIM_NETWORK_PERSONALIZATION;
    } else if (0 == strcmp(cpinResult, "PH-CORP PIN")) {
        *substate = RIL_PERSOSUBSTATE_SIM_CORPORATE;
        return SIM_NETWORK_PERSONALIZATION;
    } else if (0 != strcmp(cpinResult, "READY")) {
        /* We're treating unsupported lock types as "sim absent". */
        ret = SIM_ABSENT;
        goto done;
    }

done:
    at_response_free(atresponse);
    return ret;
}

/**
 * Get the current card status.
 *
 * This must be freed using freeCardStatus.
 * @return: On success returns RIL_E_SUCCESS.
 */
#ifndef SWI_RIL_VERSION_6
static int getCardStatus(RIL_CardStatus **pp_card_status) {
    static RIL_AppStatus app_status_array[] = {
        /* SIM_ABSENT = 0 */
        { RIL_APPTYPE_UNKNOWN, RIL_APPSTATE_UNKNOWN, RIL_PERSOSUBSTATE_UNKNOWN,
          NULL, NULL, 0, RIL_PINSTATE_UNKNOWN, RIL_PINSTATE_UNKNOWN },
        /* SIM_NOT_READY = 1 */
        { RIL_APPTYPE_SIM, RIL_APPSTATE_DETECTED, RIL_PERSOSUBSTATE_UNKNOWN,
          NULL, NULL, 0, RIL_PINSTATE_UNKNOWN, RIL_PINSTATE_UNKNOWN },
        /* SIM_READY = 2 */
        { RIL_APPTYPE_SIM, RIL_APPSTATE_READY, RIL_PERSOSUBSTATE_READY,
          NULL, NULL, 0, RIL_PINSTATE_UNKNOWN, RIL_PINSTATE_UNKNOWN },
        /* SIM_PIN = 3 */
        { RIL_APPTYPE_SIM, RIL_APPSTATE_PIN, RIL_PERSOSUBSTATE_UNKNOWN,
          NULL, NULL, 0, RIL_PINSTATE_ENABLED_NOT_VERIFIED, RIL_PINSTATE_UNKNOWN },
        /* SIM_PUK = 4 */
        { RIL_APPTYPE_SIM, RIL_APPSTATE_PUK, RIL_PERSOSUBSTATE_UNKNOWN,
          NULL, NULL, 0, RIL_PINSTATE_ENABLED_BLOCKED, RIL_PINSTATE_UNKNOWN },
        /* SIM_NETWORK_PERSONALIZATION = 5 */
        { RIL_APPTYPE_SIM, RIL_APPSTATE_SUBSCRIPTION_PERSO, RIL_PERSOSUBSTATE_SIM_NETWORK,
          NULL, NULL, 0, RIL_PINSTATE_ENABLED_NOT_VERIFIED, RIL_PINSTATE_UNKNOWN }
    };
    RIL_CardState card_state;
    int num_apps;
    RIL_PersoSubstate substate;

    SIM_Status sim_status = getSIMStatus(&substate);
    if ((sim_status == SIM_ABSENT) || (sim_status == SIM_FAILURE)) {
        card_state = RIL_CARDSTATE_ABSENT;
        num_apps = 0;
    } else {
        card_state = RIL_CARDSTATE_PRESENT;
        num_apps = 1;
    }

    /* Allocate and initialize base card status. */
    RIL_CardStatus *p_card_status = malloc(sizeof(RIL_CardStatus));
    p_card_status->card_state = card_state;
    p_card_status->universal_pin_state = RIL_PINSTATE_UNKNOWN;
    p_card_status->gsm_umts_subscription_app_index = RIL_CARD_MAX_APPS;
    p_card_status->cdma_subscription_app_index = RIL_CARD_MAX_APPS;
    p_card_status->num_applications = num_apps;

    /* Initialize application status. */
    int i;
    for (i = 0; i < RIL_CARD_MAX_APPS; i++) {
        p_card_status->applications[i] = app_status_array[SIM_ABSENT];
    }

    /* Pickup the appropriate application status
       that reflects sim_status for gsm. */
    if (num_apps != 0) {
        /* Only support one app, gsm. */
        p_card_status->num_applications = 1;
        p_card_status->gsm_umts_subscription_app_index = 0;

        /* Get the correct app status. */
        p_card_status->applications[0] = app_status_array[sim_status];
        /* update the substate of personalization */
        if (p_card_status->applications[0].app_state == RIL_APPSTATE_SUBSCRIPTION_PERSO) {
            p_card_status->applications[0].perso_substate = substate;
        }
        /* MC7750-like module uses CDMA as default */
        if (isDualModeModule()) {
            if (!isDualModeRunningLTE()) {
                LOGD("getCardStatus: CSIM");
                p_card_status->applications[0].app_type = RIL_APPTYPE_CSIM;
                p_card_status->cdma_subscription_app_index = 0;
            }
        }
    }

    *pp_card_status = p_card_status;
    return RIL_E_SUCCESS;
}
#else
static int getCardStatus(RIL_CardStatus_v6 **pp_card_status) {
    static RIL_AppStatus app_status_array[] = {
        /* SIM_ABSENT = 0 */
        { RIL_APPTYPE_UNKNOWN, RIL_APPSTATE_UNKNOWN, RIL_PERSOSUBSTATE_UNKNOWN,
          NULL, NULL, 0, RIL_PINSTATE_UNKNOWN, RIL_PINSTATE_UNKNOWN },
        /* SIM_NOT_READY = 1 */
        { RIL_APPTYPE_SIM, RIL_APPSTATE_DETECTED, RIL_PERSOSUBSTATE_UNKNOWN,
          NULL, NULL, 0, RIL_PINSTATE_UNKNOWN, RIL_PINSTATE_UNKNOWN },
        /* SIM_READY = 2 */
        { RIL_APPTYPE_SIM, RIL_APPSTATE_READY, RIL_PERSOSUBSTATE_READY,
          NULL, NULL, 0, RIL_PINSTATE_UNKNOWN, RIL_PINSTATE_UNKNOWN },
        /* SIM_PIN = 3 */
        { RIL_APPTYPE_SIM, RIL_APPSTATE_PIN, RIL_PERSOSUBSTATE_UNKNOWN,
          NULL, NULL, 0, RIL_PINSTATE_ENABLED_NOT_VERIFIED, RIL_PINSTATE_UNKNOWN },
        /* SIM_PUK = 4 */
        { RIL_APPTYPE_SIM, RIL_APPSTATE_PUK, RIL_PERSOSUBSTATE_UNKNOWN,
          NULL, NULL, 0, RIL_PINSTATE_ENABLED_BLOCKED, RIL_PINSTATE_UNKNOWN },
        /* SIM_NETWORK_PERSONALIZATION = 5 */
        { RIL_APPTYPE_SIM, RIL_APPSTATE_SUBSCRIPTION_PERSO, RIL_PERSOSUBSTATE_SIM_NETWORK,
          NULL, NULL, 0, RIL_PINSTATE_ENABLED_NOT_VERIFIED, RIL_PINSTATE_UNKNOWN }
    };
    RIL_CardState card_state;
    int num_apps;
    RIL_PersoSubstate substate;

    SIM_Status sim_status = getSIMStatus(&substate);
    if ((sim_status == SIM_ABSENT) || (sim_status == SIM_FAILURE)) {
        card_state = RIL_CARDSTATE_ABSENT;
        num_apps = 0;
    } else {
        card_state = RIL_CARDSTATE_PRESENT;
        num_apps = 1;
    }

    /* Allocate and initialize base card status. */
    RIL_CardStatus_v6 *p_card_status = malloc(sizeof(RIL_CardStatus_v6));
    p_card_status->card_state = card_state;
    p_card_status->universal_pin_state = RIL_PINSTATE_UNKNOWN;
    p_card_status->gsm_umts_subscription_app_index = RIL_CARD_MAX_APPS;
    p_card_status->cdma_subscription_app_index = RIL_CARD_MAX_APPS;
    p_card_status->ims_subscription_app_index = RIL_CARD_MAX_APPS;
    p_card_status->num_applications = num_apps;

    /* Initialize application status. */
    int i;
    for (i = 0; i < RIL_CARD_MAX_APPS; i++) {
        p_card_status->applications[i] = app_status_array[SIM_ABSENT];
    }

    /* Pickup the appropriate application status
       that reflects sim_status for gsm. */
    if (num_apps != 0) {
        /* Only support one app, gsm. */
        p_card_status->num_applications = 1;
        p_card_status->gsm_umts_subscription_app_index = 0;

        /* Get the correct app status. */
        p_card_status->applications[0] = app_status_array[sim_status];
        /* update the substate of personalization */
        if (p_card_status->applications[0].app_state == RIL_APPSTATE_SUBSCRIPTION_PERSO) {
            p_card_status->applications[0].perso_substate = substate;
        }
        /* MC7750-like module uses CDMA as default */
        if (isDualModeModule()) {
            if (!isDualModeRunningLTE()) {
                LOGD("getCardStatus: CSIM");
                p_card_status->applications[0].app_type = RIL_APPTYPE_CSIM;
                p_card_status->cdma_subscription_app_index = 0;
            }
        }
    }

    *pp_card_status = p_card_status;
    return RIL_E_SUCCESS;
}
#endif
/**
 * Free the card status returned by getCardStatus.
 */
#ifndef SWI_RIL_VERSION_6
static void freeCardStatus(RIL_CardStatus *p_card_status) {
#else
static void freeCardStatus(RIL_CardStatus_v6 *p_card_status) {
#endif
    free(p_card_status);
}

static int poll_retry_count = SIM_POLL_RETRY;

void resetpollSIMRetry(void)
{
	poll_retry_count = SIM_POLL_RETRY;
}


/**
 * SIM ready means any commands that access the SIM will work, including:
 *  AT+CPIN, AT+CSMS, AT+CNMI, AT+CRSM
 *  (all SMS-related commands).
 */
void pollSIMState(void *param)
{
    RIL_PersoSubstate substate;
    char propValue[PROPERTY_VALUE_MAX];
    int intpropvalue=0;
    struct timeval timeval_simpoll = { 0, 0 };
   
    if (((int) param) != 1 &&
        currentState() != RADIO_STATE_SIM_NOT_READY &&
        currentState() != RADIO_STATE_SIM_LOCKED_OR_ABSENT &&
        currentState() != RADIO_STATE_RUIM_NOT_READY &&
        currentState() != RADIO_STATE_RUIM_LOCKED_OR_ABSENT) {
        /* No longer valid to poll. */
        return;
    }

    switch (getSIMStatus(&substate)) {
        case SIM_ABSENT:      
        case SIM_FAILURE:
        	 if (property_get("persist.sierra.sim_poll_delay", propValue, NULL) > 0)
        	 {
        		 intpropvalue = atoi(propValue);
        	     LOGI("%s: poll pindelay=%i retry = %i", __func__, intpropvalue,poll_retry_count);
        	     timeval_simpoll.tv_sec = intpropvalue;
        	     if(intpropvalue > 0 && poll_retry_count-- > 0 )
        	    	 enqueueRILEvent(RIL_EVENT_QUEUE_PRIO, pollSIMState, NULL,&timeval_simpoll);
        	     else
        	    	 setRadioState(RADIO_STATE_SIM_LOCKED_OR_ABSENT);
        	 }
        	 else
        		 setRadioState(RADIO_STATE_SIM_LOCKED_OR_ABSENT);

        	break;
            
        case SIM_PIN:
        case SIM_PUK:
        case SIM_NETWORK_PERSONALIZATION:

        default:
            /* MC7750-like module uses CDMA as default */
            if (isDualModeModule() && (!isDualModeRunningLTE())) {
                LOGD("%s: set RADIO_STATE_SIM_LOCKED_OR_ABSENT", __func__);
                setRadioState(RADIO_STATE_SIM_LOCKED_OR_ABSENT);
            }
            else {
                setRadioState(RADIO_STATE_SIM_LOCKED_OR_ABSENT);
            }
            return;
    
        case SIM_NOT_READY:
            enqueueRILEvent(RIL_EVENT_QUEUE_PRIO, pollSIMState, NULL,
                            &TIMEVAL_SIMPOLL);
            return;
    
        case SIM_READY:
            if ( ! useDelayedSIMReady() ) {
                /* MC7750-like module uses CDMA as default */
                if (isDualModeModule() && (!isDualModeRunningLTE())) {
                    LOGD("%s: set RADIO_STATE_SIM_READY", __func__);
                    setRadioState(RADIO_STATE_SIM_READY);
                }
                else {
                    setRadioState(RADIO_STATE_SIM_READY);
                }
            }
            return;
    }
}

/** 
 * RIL_REQUEST_GET_SIM_STATUS
 *
 * Requests status of the SIM interface and the SIM card.
 * 
 * Valid errors:
 *  Must never fail.
 */
void requestGetSimStatus(void *data, size_t datalen, RIL_Token t)
{
#ifndef SWI_RIL_VERSION_6
    RIL_CardStatus* p_card_status = NULL;
#else
    RIL_CardStatus_v6* p_card_status = NULL;
#endif

    if (getCardStatus(&p_card_status) != RIL_E_SUCCESS)
        goto error;
    
    RIL_onRequestComplete(t, RIL_E_SUCCESS, (char*)p_card_status, sizeof(*p_card_status));

finally:
    if (p_card_status != NULL) {
        freeCardStatus(p_card_status);
    }
    return;

error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    goto finally;
}

/**
 * convertSimIoFcp
 *
 * Convert the incoming SIM response to the "GET RESPONSE"
 * command into a 2G FCP image, if applicable. The upper
 * layers of the Android stack, notably in a file called
 * IccFileHandler.java, expect the SIM File Header info
 * to be packed by a 2G SIM. If the incoming data appears
 * to be from a 3G SIM, this function converts it to 2G
 * format before proceeding. 
 */
int convertSimIoFcp(RIL_SIM_IO_Response *sr, char **cvt)
{
    int err;
    size_t pos;
    size_t fcplen;
    struct ts_51011_921_resp resp;
    void *cvt_buf = NULL;

    if (!sr->simResponse || !cvt) {
        err = -EINVAL;
        goto error;
    }

    fcplen = strlen(sr->simResponse);
    if ((fcplen == 0) || (fcplen & 1)) {
        err = -EINVAL;
        goto error;
    }

    err = fcp_to_ts_51011(sr->simResponse, fcplen, &resp);
    if (err < 0) {
        LOGE("%s: fcp parser returned %d", __func__, err );
        goto error;
    }

    cvt_buf = malloc(sizeof(resp) * 2 + 1);
    if (!cvt_buf) {
        err = -ENOMEM;
        goto error;
    }

    err = binaryToString((unsigned char*)(&resp),
                   sizeof(resp), cvt_buf);
    if (err < 0)
        goto error;

    /* cvt_buf ownership is moved to the caller */
    *cvt = cvt_buf;
    cvt_buf = NULL;

finally:
    return err;

error:
    free(cvt_buf);
    goto finally;
}

/**
 * RIL_REQUEST_SIM_IO
 *
 * Request SIM I/O operation.
 * This is similar to the TS 27.007 "restricted SIM" operation
 * where it assumes all of the EF selection will be done by the
 * callee.
 */
void requestSIM_IO(void *data, size_t datalen, RIL_Token t)
{
    ATResponse *atresponse = NULL;
    RIL_SIM_IO_Response sr;
    int len, cvt_done, err;
    char *cmd = NULL;
    RIL_SIM_IO *ioargs;
    char *line;

    cvt_done = 0;
    memset(&sr, 0, sizeof(sr));

    ioargs = (RIL_SIM_IO *) data;

    /* FIXME Handle pin2. */

    /* If the command is SIM_GET_RESPONSE increase the #
     * of requested bytes to ensure we get the whole
     * header 
     */
    if(ioargs->command == SIM_GET_RESPONSE) {
        ioargs->p3 = SIM_HEADER_REQ_SIZE;
    }

    if (ioargs->data == NULL) {
        asprintf(&cmd, "AT+CRSM=%d,%d,%d,%d,%d",
                 ioargs->command, ioargs->fileid,
                 ioargs->p1, ioargs->p2, ioargs->p3);
    } else {
        len = asprintf(&cmd, "AT+CRSM=%d,%d,%d,%d,%d,\"%s\"",
                 ioargs->command, ioargs->fileid,
                 ioargs->p1, ioargs->p2, ioargs->p3, ioargs->data);
        if( len < 0 ) {
            err = len;
            goto error;
        }
        /* Convert the data to uppercase hex, if applicable */
        hexToUpper(cmd, len);
    }

    /* SWI_TBD: try timing out after 30s instead of the usual 3min */
    err = at_send_command_singleline_with_timeout(cmd, "+CRSM:", &atresponse, 30000);

    if (err < 0 || atresponse->success == 0) {
        goto error;
    }

    line = atresponse->p_intermediates->line;

    err = at_tok_start(&line);
    if (err < 0)
        goto error;

    err = at_tok_nextint(&line, &(sr.sw1));
    if (err < 0)
        goto error;

    err = at_tok_nextint(&line, &(sr.sw2));
    if (err < 0)
        goto error;

    if (at_tok_hasmore(&line)) {
        err = at_tok_nextstr(&line, &(sr.simResponse));
        if (err < 0)
            goto error;
    }

    /*
     * Now that the modem has responded, re-check the command and 
     * check the first byte of the response to see if it is an FCP 
     * Template. If so we parse it here, otherwise it's probably a 
     * 2G header so just let the java layer handle it
     */
    if (ioargs->command == SIM_GET_RESPONSE && is3GSim( sr.simResponse ) ) {
        LOGD("FCP Template  detected");
        err = convertSimIoFcp(&sr, &sr.simResponse);
        if (err < 0)
            goto error;
        cvt_done = 1; /* sr.simResponse needs to be freed */
    }
    LOGD("%s - successfully completed", __func__ );
    RIL_onRequestComplete(t, RIL_E_SUCCESS, &sr, sizeof(sr));

finally:
    at_response_free(atresponse);
    free(cmd);
    if( cvt_done ) {
        free(sr.simResponse);
    }
    return;

error:
    LOGE("%s - error detected: %d", __func__, err );
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    goto finally;
}

/**
 * Enter SIM PIN, might be PIN, PIN2, PUK, PUK2, etc.
 *
 * Data can hold pointers to one or two strings, depending on what we
 * want to enter. (PUK requires new PIN, etc.).
 *
 */
void requestEnterSimPin(void *data, size_t datalen, RIL_Token t, int request)
{
    ATResponse *atresponse = NULL;
    int err;
    int cme_err;
    char *cmd = NULL;
    const char **strings = (const char **) data;
    int num_retries = -1;
    PINRetryCount_s retryCount;

    if (datalen == sizeof(char *)) {
        asprintf(&cmd, "AT+CPIN=\"%s\"", strings[0]);
    } else if ((datalen == 2 * sizeof(char *)) ||
               (datalen == 3 * sizeof(char *))) {
        /* SWI_TDB Modem can't access AID(Application Identifier) now, so ignore it */
        if(strings[1] == NULL)
            asprintf(&cmd, "AT+CPIN=\"%s\"", strings[0]);
        else
            asprintf(&cmd, "AT+CPIN=\"%s\",\"%s\"", strings[0], strings[1]);
    } 
    else {
        goto error;
    }

    err = at_send_command(cmd, &atresponse);
    free(cmd);

    cme_err = at_get_cme_error(atresponse);

    if (cme_err != CME_SUCCESS && (err < 0 || atresponse->success == 0)) {
        if (cme_err == CME_INCORRECT_PASSWORD || 
            cme_err == CME_SIM_PIN_REQUIRED) {

            /* Set the number of retries based on the request */
            num_retries = getPINRetryCount( getSIMPinPukType(request) );

            RIL_onRequestComplete( t,
                                   RIL_E_PASSWORD_INCORRECT,
                                   &num_retries,
                                   sizeof(int *) );
        } else
            goto error;

    } else {
        /* Got OK, so return success and start SIM polling process so that the
           RADIO state can be updated once the SIM is ready. While not actually
           required, wait a short time before starting the polling process,
           since the SIM won't be ready right away anyways, and thus we avoid
           unnecessary polling.
         */      
        resetpollSIMRetry(); 
        enqueueRILEvent(RIL_EVENT_QUEUE_PRIO, pollSIMState, NULL, &TIMEVAL_SIMPOLL);

        /* Set the number of retries based on the request */
        num_retries = getPINRetryCount( getSIMPinPukType(request) );

        RIL_onRequestComplete(t, RIL_E_SUCCESS, &num_retries, sizeof(int *));
    }

finally:
    at_response_free(atresponse);
    return;
error:
    /* Set the number of retries based on the request */
    num_retries = getPINRetryCount( getSIMPinPukType(request) );

    RIL_onRequestComplete( t,
                           RIL_E_GENERIC_FAILURE,
                           &num_retries,
                           sizeof(int *) );
    goto finally;
}

void requestChangePassword(void *data, size_t datalen, RIL_Token t,
                           char *facility)
{
    int err = 0;
    char *oldPassword = NULL;
    char *newPassword = NULL;
    char *cmd = NULL;
    ATResponse *atresponse = NULL;
    int num_retries = -1;
    PINRetryCount_s retryCount;

    if( datalen > 3 * sizeof(char *) || 
        datalen < 2 * sizeof(char *) || 
        strlen(facility) != 2 )
        goto error;

    oldPassword = ((char **) data)[0];
    newPassword = ((char **) data)[1];

    asprintf(&cmd, "AT+CPWD=\"%s\",\"%s\",\"%s\"", facility, oldPassword,
             newPassword);

    err = at_send_command(cmd, &atresponse);
    free(cmd);
    if (err < 0 || atresponse->success == 0)
        goto error;

    /* Set the number of retries if the SIM PIN or SIM PIN2 is the facility */
    if( !strcmp(facility, "SC") ) {
        num_retries = getPINRetryCount( SIM_PIN_TYPE );
    }
    else if( !strcmp(facility, "P2") ) {
        num_retries = getPINRetryCount( SIM_PIN2_TYPE );
    }

    RIL_onRequestComplete(t, RIL_E_SUCCESS, &num_retries, sizeof(int *));

finally:
    at_response_free(atresponse);
    return;

error:
    /* Set the number of retries if the SIM PIN or SIM PIN2 is the facility */
    if( !strcmp(facility, "SC") ) {
        num_retries = getPINRetryCount( SIM_PIN_TYPE );
    }
    else if( !strcmp(facility, "P2") ) {
        num_retries = getPINRetryCount( SIM_PIN2_TYPE );
    }

    if (atresponse != NULL && 
        at_get_cme_error(atresponse) == CME_INCORRECT_PASSWORD) {
        RIL_onRequestComplete( t,
                               RIL_E_PASSWORD_INCORRECT,
                               &num_retries,
                               sizeof(int *) );
    } else {
        RIL_onRequestComplete( t,
                               RIL_E_GENERIC_FAILURE,
                               &num_retries,
                               sizeof(int *) );
    }
    goto finally;
}

/**
 * RIL_REQUEST_SET_FACILITY_LOCK
 *
 * Enable/disable one facility lock.
 */
void requestSetFacilityLock(void *data, size_t datalen, RIL_Token t)
{
    int err;
    ATResponse *atresponse = NULL;
    char *cmd = NULL;
    char *facility_string = NULL;
    int facility_mode = -1;
    char *facility_mode_str = NULL;
    char *facility_password = NULL;
    char *facility_class = NULL;
    int num_retries = -1;

    assert(datalen >= (4 * sizeof(char **)));

    facility_string = ((char **) data)[0];
    facility_mode_str = ((char **) data)[1];
    facility_password = ((char **) data)[2];
    facility_class = ((char **) data)[3];

    assert(*facility_mode_str == '0' || *facility_mode_str == '1');
    facility_mode = atoi(facility_mode_str);

    asprintf(&cmd, "AT+CLCK=\"%s\",%d,\"%s\",%s", facility_string,
             facility_mode, facility_password, facility_class);
    err = at_send_command(cmd, &atresponse);
    free(cmd);
    if (err < 0 || atresponse->success == 0) {
        goto error;
    }

    /* Get the retry count only if the SIM PIN is the facility */
    if( !strcmp(facility_string, "SC") ) {
        /* Try QMI API if applicable first, and if that fails, use AT command */
        num_retries = getPINRetryCountQMI();
        if (num_retries < 0)
            num_retries = getPINRetryCount( SIM_PIN_TYPE );
    }
    
    RIL_onRequestComplete(t, RIL_E_SUCCESS, &num_retries, sizeof(int *));
    at_response_free(atresponse);
    return;

error:
    at_response_free(atresponse);

    /* Get the retry count only if the SIM PIN is the facility */
    if( !strcmp(facility_string, "SC") ) {
        /* Try QMI API if applicable first, and if that fails, use AT command */
        num_retries = getPINRetryCountQMI();
        if (num_retries < 0)
            num_retries = getPINRetryCount( SIM_PIN_TYPE );
    }
    RIL_onRequestComplete( t,
                           RIL_E_GENERIC_FAILURE,
                           &num_retries,
                           sizeof(int *) );

    /* If number of retry is 0, send out RIL_UNSOL_RESPONSE_SIM_STATUS_CHANGED 
        which will trigger up layer to send GET_SIM_STATUS request */
    if(num_retries == 0){
        LOGD("%s:: retry number %d, send RIL_UNSOL_RESPONSE_SIM_STATUS_CHANGED ",
            __func__, num_retries );
        RIL_onUnsolicitedResponse(RIL_UNSOL_RESPONSE_SIM_STATUS_CHANGED,
                      NULL, 0);
    }

}

/**
 * RIL_REQUEST_QUERY_FACILITY_LOCK
 *
 * Query the status of a facility lock state.
 */
void requestQueryFacilityLock(void *data, size_t datalen, RIL_Token t)
{
    int err, rat, response;
    ATResponse *atresponse = NULL;
    char *cmd = NULL;
    char *line = NULL;
    char *facility_string = NULL;
    char *facility_password = NULL;
    char *facility_class = NULL;

    assert(datalen >= (3 * sizeof(char **)));

    facility_string = ((char **) data)[0];
    facility_password = ((char **) data)[1];
    facility_class = ((char **) data)[2];

    asprintf(&cmd, "AT+CLCK=\"%s\",2,\"%s\",%s", facility_string,
             facility_password, facility_class);
    err = at_send_command_singleline(cmd, "+CLCK:", &atresponse);
    free(cmd);
    if (err < 0 || atresponse->success == 0) {
        goto error;
    }

    line = atresponse->p_intermediates->line;

    err = at_tok_start(&line);

    if (err < 0)
        goto error;

    err = at_tok_nextint(&line, &response);

    if (err < 0)
        goto error;

    RIL_onRequestComplete(t, RIL_E_SUCCESS, &response, sizeof(int));

finally:
    at_response_free(atresponse);
    return;

error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    goto finally;
}
