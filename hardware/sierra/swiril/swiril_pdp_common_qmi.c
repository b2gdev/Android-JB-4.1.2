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

#include <stdio.h>
#include <telephony/ril.h>
#include <cutils/properties.h>

#define LOG_TAG "RIL"
#include "swiril_log.h"

#include "swiril_misc.h"
#include "swiril_main.h"
#include "swiril_misc_qmi.h"
#include "swiril_pdp_qmi.h"
#include "swiril_network_qmi.h"
#include "swiril_main_qmi.h"

/* For QMI */
#include "qmerrno.h"
#include "sludefs.h"
#include "SWIWWANCMAPI.h"

/** 
 * QMI network reject cause lookup tables. Based on table
 * 2.3.2 (UMTS part) of QCOM document 80-VF219-10B,
 *          GobiConnectionMgmtAPI.pdf
 *
 * To use the following Cause Code tables, take the code
 * received from the SDK, perform a range check to determine
 * which table to index, reduce the sdk code by the offset
 * value (Defined constant in front of each table) and use
 * the result as an index into the table. The value at that
 * index is the associated Android RIL error code
 *
 */

/*
 * General Call End Reasons
 *
 *      There are 11 codes in this range based at 
 *      1 and ending at 11. The offset must be 
 *      applied in order to access the table entries
 *      properly
 * 
 * NOTE: The commented text is the Description field
 *       from the above-mentioned Qualcom document
 *       for the General codes
 */
static const int qmi_general_cause_codes[] =
{
    PDP_FAIL_ERROR_UNSPECIFIED,        /* 1 - Reason unspecified */
    PDP_FAIL_INSUFFICIENT_RESOURCES,   /* 2 - Client ended the call */
    PDP_FAIL_INSUFFICIENT_RESOURCES,   /* 3 - Device has no service */
    PDP_FAIL_ERROR_UNSPECIFIED,        /* 4 - Call has ended abnormally */
    PDP_FAIL_ERROR_UNSPECIFIED,        /* 5 - Received release from base station; no reason given */
    PDP_FAIL_ERROR_UNSPECIFIED,        /* 6 - Access attempt already in progress */
    PDP_FAIL_ERROR_UNSPECIFIED,        /* 7 - Access attempt failure for reason other than the above */
    PDP_FAIL_ERROR_UNSPECIFIED,        /* 8 - Call rejected because of redirection or handoff */
    PDP_FAIL_ERROR_UNSPECIFIED,        /* 9 - Call failed because close is in progress */
    PDP_FAIL_USER_AUTHENTICATION,      /* 10 - Authentication failed */
    PDP_FAIL_ERROR_UNSPECIFIED         /* 11 - Client rejected the incoming call */
};
#define QMI_MAX_GENERAL_CAUSES (sizeof(qmi_general_cause_codes)/sizeof(qmi_general_cause_codes[0]))

/* For range checking */
#define QMI_GENERAL_CAUSE_OFFSET 1
#define QMI_GENERAL_CAUSE_START QMI_GENERAL_CAUSE_OFFSET
#define QMI_GENERAL_CAUSE_END (QMI_GENERAL_CAUSE_START + (QMI_MAX_GENERAL_CAUSES-1))

/*
 * CDMA Call End Reasons
 *
 *      There are 16 codes in this range based at 
 *      500 and ending at 515. The code can be used 
 *      to index this table directly 
 * 
 * NOTE: The commented text is the Description field
 *       from the above-mentioned Qualcom document
 *       for the General codes
 */

/* For table range checking */
static const int qmi_cdma_cause_codes[] =
{
    CALL_FAIL_CDMA_LOCKED_UNTIL_POWER_CYCLE, /* 500 - Device is CDMA-locked until power cycle */
    CALL_FAIL_CDMA_INTERCEPT,       /* 501 - Received intercept from base station; origination only */
    CALL_FAIL_CDMA_REORDER,         /* 502 - Received reorder from base station; origination only */
    CALL_FAIL_CDMA_SO_REJECT,       /* 503 - Received release from base station; service option reject */
    CALL_FAIL_BUSY,                 /* 504 - Received incoming call from base station */
    CALL_FAIL_CDMA_ACCESS_FAILURE,  /* 505 - Received alert stop from base station; incoming only */
    CALL_FAIL_CDMA_DROP,            /* 506 - Received end activation; OTASP call only */
    CALL_FAIL_ERROR_UNSPECIFIED,    /* 507 - Max access probes transmitted */
    CALL_FAIL_ERROR_UNSPECIFIED,    /* 508 - Concurrent service is not supported by base station */
    CALL_FAIL_ERROR_UNSPECIFIED,    /* 509 - No response received from base station */
    CALL_FAIL_CDMA_ACCESS_FAILURE,  /* 510 - Call rejected by base station */
    CALL_FAIL_CDMA_ACCESS_FAILURE,  /* 511 - Concurrent services requested were not compatible */
    CALL_FAIL_CDMA_ACCESS_FAILURE,  /* 512 - Call manager (CM) subsystem already in TC */
    CALL_FAIL_CDMA_ACCESS_FAILURE,  /* 513 - CM subsystem ending a GPRS call in favor of a user call */
    CALL_FAIL_CDMA_ACCESS_FAILURE,  /* 514 - CM subsystem ending a SMS call in favor of a user call */
    CALL_FAIL_CDMA_ACCESS_FAILURE   /* 515 - Device has no CDMA service */
};
#define QMI_MAX_CDMA_CAUSES (sizeof(qmi_cdma_cause_codes)/sizeof(qmi_cdma_cause_codes[0]))

/* For range checking */
#define QMI_CDMA_CAUSE_OFFSET 500
#define QMI_CDMA_CAUSE_START QMI_CDMA_CAUSE_OFFSET
#define QMI_CDMA_CAUSE_END (QMI_CDMA_CAUSE_START + (QMI_MAX_CDMA_CAUSES-1))

/*
 * UMTS cause codes.
 *
 *      There are 37 codes in this range based at 
 *      1000 and ending at 1036. Users of this table
 *      must be mindful of the UMTS offset of 1000
 * 
 * NOTE: The commented text is the Description field
 *       from the above-mentioned Qualcom document
 *       for the UMTS codes
 */
static const int qmi_umts_cause_codes[] =
{
    PDP_FAIL_PROTOCOL_ERRORS,               /* 1000 - Call origination request failed */
    PDP_FAIL_ACTIVATION_REJECT_UNSPECIFIED, /* 1001 - Client rejected the incoming call */
    PDP_FAIL_ERROR_UNSPECIFIED,             /* 1002 - Device has no UMTS service */
    PDP_FAIL_ERROR_UNSPECIFIED,             /* 1003 - Network ended the call */
    PDP_FAIL_PROTOCOL_ERRORS,               /* 1004 - LLC or SNDCP failure */
    PDP_FAIL_INSUFFICIENT_RESOURCES,        /* 1005 - Insufficient resources */
    PDP_FAIL_SERVICE_OPTION_OUT_OF_ORDER,   /* 1006 - Service option temporarily out of order */
    PDP_FAIL_NSAPI_IN_USE,                  /* 1007 - NSAPI already used */
    PDP_FAIL_ERROR_UNSPECIFIED,             /* 1008 - Regular PDP context deactivation */
    PDP_FAIL_ERROR_UNSPECIFIED,             /* 1009 - Network failure */
    PDP_FAIL_ACTIVATION_REJECT_UNSPECIFIED, /* 1010 - Reactivation requested */
    PDP_FAIL_ERROR_UNSPECIFIED,             /* 1011 - Protocol error, unspecified */
    PDP_FAIL_OPERATOR_BARRED,               /* 1012 - Operator-determined barring (exclusion) */
    PDP_FAIL_MISSING_UKNOWN_APN,            /* 1013 - Unknown or missing APN */
    PDP_FAIL_UNKNOWN_PDP_ADDRESS_TYPE,      /* 1014 - Unknown PDP address or PDP type */
    PDP_FAIL_ACTIVATION_REJECT_GGSN,        /* 1015 - Activation requested by GGSN */
    PDP_FAIL_ACTIVATION_REJECT_UNSPECIFIED, /* 1016 - Activation rejected, unspecified */
    PDP_FAIL_SERVICE_OPTION_NOT_SUPPORTED,  /* 1017 - Service option not supported */
    PDP_FAIL_SERVICE_OPTION_NOT_SUBSCRIBED, /* 1018 - Requested service option not subscribed */
    PDP_FAIL_INSUFFICIENT_RESOURCES,        /* 1019 - Quality of service not accepted */
    PDP_FAIL_PROTOCOL_ERRORS,               /* 1020 - Semantic error in TFT operation */
    PDP_FAIL_PROTOCOL_ERRORS,               /* 1021 - Syntactical error in packet filter(s) */
    PDP_FAIL_UNKNOWN_PDP_ADDRESS_TYPE,      /* 1022 - Unknown PDP context */
    PDP_FAIL_PROTOCOL_ERRORS,               /* 1023 - Semantic error(s) in packet filter(s) */
    PDP_FAIL_PROTOCOL_ERRORS,               /* 1024 - Syntactical error in packet filter(s) */
    PDP_FAIL_PROTOCOL_ERRORS,               /* 1025 - PDP context without TFT already activated */
    PDP_FAIL_PROTOCOL_ERRORS,               /* 1026 - Invalid transaction ID value */
    PDP_FAIL_PROTOCOL_ERRORS,               /* 1027 - Semantically incorrect message */
    PDP_FAIL_PROTOCOL_ERRORS,               /* 1028 - Invalid mandatory information */
    PDP_FAIL_PROTOCOL_ERRORS,               /* 1029 - Message type nonexistent or not implemented */
    PDP_FAIL_PROTOCOL_ERRORS,               /* 1030 - Message not compatible with state */
    PDP_FAIL_PROTOCOL_ERRORS,               /* 1031 - Information element nonexistent or not implemented */
    PDP_FAIL_PROTOCOL_ERRORS,               /* 1032 - Conditional information error */
    PDP_FAIL_PROTOCOL_ERRORS,               /* 1033 - Message not compatible with protocol state */
    PDP_FAIL_PROTOCOL_ERRORS,               /* 1034 - APN restriction value incompatible w/active PDP context */
    PDP_FAIL_INSUFFICIENT_RESOURCES,        /* 1035 - No GPRS context present */
    PDP_FAIL_INSUFFICIENT_RESOURCES         /* 1036 - Requested feature not supported */
};
#define QMI_MAX_UMTS_CAUSES (sizeof(qmi_umts_cause_codes)/sizeof(qmi_umts_cause_codes[0]))

/* For range checking */
#define QMI_UMTS_CAUSE_OFFSET 1000
#define QMI_UMTS_CAUSE_START QMI_UMTS_CAUSE_OFFSET
#define QMI_UMTS_CAUSE_END (QMI_UMTS_CAUSE_START + (QMI_MAX_UMTS_CAUSES-1))

/*
 * CDMA 1xEV-DO cause codes.
 *
 *      There are 8 codes in this range based at 
 *      1500 and ending at 1507. Users of this table
 *      must be mindful of the 1xEV-DO offset
 * 
 * NOTE: The commented text is the Description field
 *       from the above-mentioned Qualcom document
 *       for the UMTS codes
 */
#ifndef SWI_RIL_VERSION_6
static const int qmi_evdo_cause_codes[] =
{
    CALL_FAIL_BUSY,                 /* 1500 - Abort connection setup: deny code=general or network busy */
    PDP_FAIL_REGISTRATION_FAIL,     /* 1501 - Abort connection setup: deny code=billing or auth failure */
    CALL_FAIL_NORMAL,               /* 1502 - Change EV-DO system due to redirection or PRL not preferred */
    CALL_FAIL_NORMAL,               /* 1503 - Exit EV-DO due to redirection or PRL not preferred */
    PDP_FAIL_REGISTRATION_FAIL,     /* 1504 - No EV-DO session */
    CALL_FAIL_NORMAL,               /* 1505 - Call mgr is ending a EV-DO call orig'n in favor of a GPRS call */
    CALL_FAIL_BUSY,                 /* 1506 - Connection setup timeout */
    CALL_FAIL_NORMAL                /* 1507 - Call manager released EV-DO call */
};
#else
static const int qmi_evdo_cause_codes[] =
{
    CALL_FAIL_BUSY,                 /* 1500 - Abort connection setup: deny code=general or network busy */
    PDP_FAIL_DATA_REGISTRATION_FAIL,/* 1501 - Abort connection setup: deny code=billing or auth failure */
    CALL_FAIL_NORMAL,               /* 1502 - Change EV-DO system due to redirection or PRL not preferred */
    CALL_FAIL_NORMAL,               /* 1503 - Exit EV-DO due to redirection or PRL not preferred */
    PDP_FAIL_DATA_REGISTRATION_FAIL,/* 1504 - No EV-DO session */
    CALL_FAIL_NORMAL,               /* 1505 - Call mgr is ending a EV-DO call orig'n in favor of a GPRS call */
    CALL_FAIL_BUSY,                 /* 1506 - Connection setup timeout */
    CALL_FAIL_NORMAL                /* 1507 - Call manager released EV-DO call */
};
#endif

/* structure declaration for function data bearer poll function */
typedef struct{
    int loopcount;
    ULONG dataBearer;
    ULONG ratMask;
    ULONG soMask;
}STRUCT_dataBearerPollParams;
#define POLL_DATA_BEARER_MAX      10000

#define QMI_MAX_EVDO_CAUSES (sizeof(qmi_evdo_cause_codes)/sizeof(qmi_evdo_cause_codes[0]))

/* For range checking */
#define QMI_EVDO_CAUSE_OFFSET 1500
#define QMI_EVDO_CAUSE_START QMI_EVDO_CAUSE_OFFSET
#define QMI_EVDO_CAUSE_END (QMI_EVDO_CAUSE_START + (QMI_MAX_EVDO_CAUSES-1))

/* Last pdp fail cause */
static int s_lastPdpFailCauseQMI = PDP_FAIL_ERROR_UNSPECIFIED;
static const struct timeval TIMEVAL_DATA_BEARER_POLL = { 5, 0 };

pthread_mutex_t s_datasession_terminated_mutex = PTHREAD_MUTEX_INITIALIZER;
static bool s_datasession_client_terminated = false;
/**
 *
 * Check if there is an activae data session
 *
 * @return
 *      true  - there is an activae data session
 *      false - otherwise
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     None
 */
bool isDataSessionActive(void)
{
    ULONG  nRet;
    ULONG  State = 0;
    bool bRet = false;

    /* Get data session state */
    nRet = GetSessionState( &State );

    if (nRet != eQCWWAN_ERR_NONE) {
        /* It's important, retry once */
        sleep(1);
        nRet = GetSessionState( &State );
    }

    if (nRet == eQCWWAN_ERR_NONE) {
        LOGI("%s session state: %lu\n", __func__, State );
        if (State == PACKET_STATE_CONNECTED) {
            bRet = true;
        }
    }
    else {
        LOGE("%s: get session state Error: %lu\n", __func__, nRet );
    }
    
    return bRet;
}

/**
 *
 * Check and set the data session deactivated state by client/framework
 *
 * @param new client state
 *      data session deactivated by client state (1 or 0)
 * @return old client state
 *      1 - deactivated by client
 *      0 - default, not deactivated by client
 *
 * @note
 *     None
 */

bool isDataSessionCloseByClient(bool state)
{
    bool ret = false;

    /* Protect the data session terminated state */
    pthread_mutex_lock(&s_datasession_terminated_mutex);
    ret = s_datasession_client_terminated;
    s_datasession_client_terminated = state;
    LOGD("isDataSessionCloseByClient : return state=%d , new state = %d \n", ret, s_datasession_client_terminated );
    pthread_mutex_unlock(&s_datasession_terminated_mutex);

    return ret;
}

/**
 *
 * Get the RIL error code of last data call failure
 *
 * @return
 *      RIL error code
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     None
 */
int getLastPdpFailCauseQMI(void)
{
    return s_lastPdpFailCauseQMI;
}

/**
 *
 * Set the RIL error code of last data call failure
 *
 * @param errCode 
 *     RIL error code
 * @return
 *      None
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     Update the global copy of reject cause.
 */
void setLastPdpFailCauseQMI(int errCode)
{
    s_lastPdpFailCauseQMI = errCode;
}

/**
 * RIL_UNSOL_PDP_CONTEXT_LIST_CHANGED
 *
 * Indicate a PDP context state has been deactivated.
 *
 * See also: RIL_REQUEST_PDP_CONTEXT_LIST
 */
void onPDPContextDisconnectedQMI(void *param)
{
    setRatCDMA(RIL_RAT_UNKNOWN);
    requestOrSendPDPContextListQMI(NULL);
}

/**
 * RIL_REQUEST_PDP_CONTEXT_LIST
 *
 * Queries the status of PDP contexts, returning for each
 * its CID, whether or not it is active, and its PDP type,
 * APN, and PDP adddress.
 */
void requestPDPContextListQMI(void *data, size_t datalen, RIL_Token t)
{
    requestOrSendPDPContextListQMI(&t);
}

/**
 *
 * Convert the last PDP activation error code into a RIL error code
 * and store it in the global variable
 *
 * @return
 *      0 - successfully retrieved error code
 *     -1 - otherwise
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     Update the global copy of reject cause.
 */
int retrievePDPRejectCauseQMI(ULONG qmicode)
{
    int ret;
    static const int *tablep;
    int offset;
    
    /* set last reject cause to PDP_FAIL_ERROR_UNSPECIFIED here in case of any failure later */
    s_lastPdpFailCauseQMI = PDP_FAIL_ERROR_UNSPECIFIED;

    /* Determine which range the code is from and use the appropriate table */
    if( qmicode >= QMI_GENERAL_CAUSE_START && qmicode <= QMI_GENERAL_CAUSE_END ) {
        tablep = qmi_general_cause_codes;
        offset = QMI_GENERAL_CAUSE_OFFSET;
    }
    else if( qmicode >= QMI_CDMA_CAUSE_START && qmicode <= QMI_CDMA_CAUSE_END ) {
        tablep = qmi_cdma_cause_codes;
        offset = QMI_CDMA_CAUSE_OFFSET;
    }
    else if( qmicode >= QMI_UMTS_CAUSE_START && qmicode <= QMI_UMTS_CAUSE_END ) {
        tablep = qmi_umts_cause_codes;
        offset = QMI_UMTS_CAUSE_OFFSET;
    }
    else if( qmicode >= QMI_EVDO_CAUSE_START && qmicode <= QMI_EVDO_CAUSE_END ) {
        tablep = qmi_evdo_cause_codes;
        offset = QMI_EVDO_CAUSE_OFFSET;
    }
    else {
        LOGE("No table for QMI code: %d\n", (int) qmicode );
        return -1;
    }

    /* Set the RIL code */
    s_lastPdpFailCauseQMI = tablep[qmicode-offset];
    return 0;
}

/**
 *
 * Poll data bearer when there is an active data session
 *
 * @param[in] params 
 *     Pointer to the loop counter
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     None
 */
void pollDataBearer(void *params)
{
    STRUCT_dataBearerPollParams *poll_params = NULL;
    QmiWDSDataBearers dataBearerInfo;
    ULONG  nRet;

    if (params == NULL) {
        LOGE("%s params is NULL", __func__);
        free(params);
        return;
    }

    poll_params = (STRUCT_dataBearerPollParams *) params;

    if (!isDataSessionActive() ||
        (poll_params->loopcount > POLL_DATA_BEARER_MAX)) {
        /* no data session anymore or exceed max count */
        free(params);
        return;
    }
    
    poll_params->loopcount++;

    nRet = SLQSGetDataBearerTechnology( &dataBearerInfo );
    if (nRet == eQCWWAN_ERR_NONE) {
        QmiWDSDataBearerTechnology *pResp =
                 dataBearerInfo.pCurDataBearerTechnology;

        if ((poll_params->dataBearer != pResp->currentNetwork) ||
            (poll_params->ratMask != pResp->ratMask) ||
            (poll_params->soMask != pResp->soMask)) {
            poll_params->dataBearer = pResp->currentNetwork;
            poll_params->ratMask = pResp->ratMask;
            poll_params->soMask = pResp->soMask;
            LOGD("%s Databearer %x\n", __func__, pResp->currentNetwork );
            LOGD("ratMask %lu\n", pResp->ratMask );
            LOGD("soMask %lu\n", pResp->soMask );
        }
    }
    else
        LOGE("%s: SLQSGetDataBearerTechnology() failed %lu\n", __func__, nRet );
        
    enqueueRILEvent(RIL_EVENT_QUEUE_NORMAL, pollDataBearer,
                    poll_params, &TIMEVAL_DATA_BEARER_POLL);
}

static void sessionStateCallback(ULONG state, ULONG sessionEndReason)
{
    STRUCT_dataBearerPollParams *poll_params = NULL;

    /* Delay sending disconnect notification by a few seconds. This is useful
     * in some cases, such as network-initiated OMADM sessions, but it is
     * probably a good idea in general, to add a small delay before trying
     * to restart the data session.
     */
    static const struct timeval TIMEVAL_DISCONNECT = { 2, 0 };
    
    LOGD("sessionStateCallback: state=%lu\n", state );
    LOGD("sessionStateCallback: sessionEndReason=%lu\n", sessionEndReason );

    /* if the session becomes disconnected, send an UNSOL notification.
     * RIL_UNSOL_DATA_CALL_LIST_CHANGED is not expected to be
     * issued because of an RIL_REQUEST_DEACTIVATE_DATA_CALL.*/
    if ( (state == PACKET_STATE_DISCONNECTED)
         /* For the MC8355, sometimes the modem decides to stop the data session
            when it has been idle for a while. The exact cause is still TBD. We
         need to send the notification in this case, so that the data session will be restarted.
         Ice Cream Sandwich has the requirement to not send
         the notification for RIL initiated disconnects.
         So, only send unsolicited response to ril when data session deactivate BY MODULE*/
         && (!isDataSessionCloseByClient(false))
         ) {
        /* queue up a callback to run in the main rild context */    
        enqueueRILEvent(RIL_EVENT_QUEUE_NORMAL, onPDPContextDisconnectedQMI,
                        NULL, &TIMEVAL_DISCONNECT);
    }
#if 0
SWI_TBD:
The pollDataBearer() function needs to be fixed, because the fields of dataBearerInfo
are pointers, and have not been initialized.  However, this function may not be needed
any more, since the data bearer callback works now, so this code could be removed.
Keep it for now, until we are sure that it is no longer needed.

    else if ( state == PACKET_STATE_CONNECTED) {
        if (getPropertyRadioNotifiction()) {
            /* queue up for polling */
            poll_params = malloc(sizeof(STRUCT_dataBearerPollParams));
    
            poll_params->loopcount = 0;
            poll_params->dataBearer = QMI_CURRENT_DATA_BEARER_CURRENT_NETWORK_UNKNOWN;
            poll_params->ratMask = QMI_CURRENT_DATA_BEARER_RAT_MASK_CDMA_DONOT_CARE;
            poll_params->soMask = QMI_CURRENT_DATA_BEARER_SO_MASK_CDMA_DONOT_CARE;
            enqueueRILEvent(RIL_EVENT_QUEUE_NORMAL, pollDataBearer,
                            poll_params, NULL);
        }
    }
#endif
}

/**
 *
 * Dormancy state callback handler
 *
 * @param[in] params 
 *     Pointer to NULL
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     None
 */
void checkEHRPD(void *params)
{
    /* property radio notification not set and found valid data bearer, stop polling */
    if (getEHRPDFromDataBearer() && !getPropertyRadioNotifiction()) {
        registerDormancyStatusCallbackQMI(false);
    }
}

void registerSessionStateCallbackQMI(void)
{
    ULONG nRet;

    nRet = SetSessionStateCallback(sessionStateCallback);

    if ( nRet != eQCWWAN_ERR_NONE ) {
        LOGE("registerSessionStateCallback: failed to enable sessionStateCallback callback %lu\n", nRet );
    }
}

static void dormancyStatusCallback(ULONG dormancyStatus)
{
    LOGD("------------------------------- dormancyStatusCallback: dormancyStatus=%lx\n", dormancyStatus );
    if ( dormancyStatus == QMI_TRAFFIC_CHANNEL_ACTIVE) {
        /* can't run in callback, queue up */
        enqueueRILEvent(RIL_EVENT_QUEUE_NORMAL, checkEHRPD,
                        NULL, NULL);
    }   
}

void registerDormancyStatusCallbackQMI(bool enable)
{
    ULONG nRet;

    if (enable) {
        nRet = SetDormancyStatusCallback(dormancyStatusCallback);
    
        if ( nRet != eQCWWAN_ERR_NONE ) {
            LOGE("registerSessionStateCallback: failed to enable dormancyStatusCallback callback %lu\n", nRet );
        }
    }
    else {
        nRet = SetDormancyStatusCallback(NULL);
    
        if ( nRet != eQCWWAN_ERR_NONE ) {
            LOGE("registerSessionStateCallback: failed to disable dormancyStatusCallback callback %lu\n", nRet );
        }
    }
}

static void setByteTotalsCB(
    ULONGLONG txTotalBytes,
    ULONGLONG rxTotalBytes )
{
    static ULONGLONG txTotalBytesLast = 0;
    static ULONGLONG rxTotalBytesLast = 0;
    
    if ((txTotalBytesLast != txTotalBytes) || (rxTotalBytesLast != rxTotalBytes)) {
        txTotalBytesLast = txTotalBytes;
        rxTotalBytesLast = rxTotalBytes;
        LOGD("Transmitted Bytes: %llu\n", txTotalBytes );
        LOGD("Received Bytes:    %llu\n", rxTotalBytes );
    }
}

void registerByteTotalsCallbackQMI(void)
{
    ULONG nRet;

    nRet = SetByteTotalsCallback(&setByteTotalsCB, 5);

    if ( nRet != eQCWWAN_ERR_NONE ) {
        LOGE("registerByteTotalsCallbackQMI: failed to enable callback %lu\n", nRet );
    }
}
