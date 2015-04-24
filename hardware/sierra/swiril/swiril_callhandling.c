/**
 *
 * @ingroup swiril
 *
 * @file 
 * Provides voice related QMI based Sierra functions
 *
 * @author
 * Copyright: © 2012 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */

#include <stdio.h>
#include <string.h>
#include <telephony/ril.h>
#include <assert.h>
#include <stdbool.h>
#include "swiril_main.h"
#include "swiril_callhandling.h"
#include "swiril_misc.h"

#define LOG_TAG "RIL"
#include "swiril_log.h"

/* For QMI */
#include "qmerrno.h"
#include "SWIWWANCMAPI.h"

/* QMI related CLIR defines for SLQS */
#define CLIR_NOT_SET     0x00
#define CLIR_SUPPRESSION 0x01
#define CLIR_INVOCATION  0x02

#define CLIR_INACTIVE 0x00
#define CLIR_ACTIVE   0x01

#define CLIR_NOT_PROVISIONED      0x00
#define CLIR_PROVISIONED_PERM     0x01
#define CLIR_TEMP_RESTRICTED      0x02
#define CLIR_PRESENTATION_ALLOWED 0x03

/* 3GPP related CLIR defines for RIL */
#define GPP_CLIR_NOCHANGE    0x00
#define GPP_CLIR_INVOCATION  0x01
#define GPP_CLIR_SUPPRESSION 0x02

#define GPP_CLIR_NOT_PROVISIONED      0x00
#define GPP_CLIR_PROVISIONED_PERM     0x01
#define GPP_CLIR_UNKNOWN              0x02
#define GPP_CLIR_TEMP_RESTRICTED      0x03
#define GPP_CLIR_PRESENTATION_ALLOWED 0x04

BYTE storedCLIRVal = CLIR_NOT_SET;

enum eQMI_CALL_STATES {
    CALL_STATE_ORIGINATION      = 0x01,
    CALL_STATE_INCOMING         = 0x02,
    CALL_STATE_CONVERSATION     = 0x03,
    CALL_STATE_CC_IN_PROGRESS   = 0x04,
    CALL_STATE_ALERTING         = 0x05,
    CALL_STATE_HOLD             = 0x06,
    CALL_STATE_WAITING          = 0x07,
    CALL_STATE_DISCONNECTING    = 0x08,
    CALL_STATE_END              = 0x09,
    CALL_STATE_SETUP            = 0x0A,
    CALL_STATE_UNAVAILABLE      = 0xFF,
};

enum eQMI_CALL_DIRECTION {
    CALL_DIRECTION_MO = 0x01,
    CALL_DIRECTION_MT = 0x02,
};

enum eQMI_CALL_MODE {
    CALL_MODE_CDMA       = 0x01,
    CALL_MODE_GSM        = 0x02,
    CALL_MODE_UMTS       = 0x03,
    CALL_MODE_LTE        = 0x04,
    CALL_MODE_TDS        = 0x05,
    CALL_MODE_UNAVALABLE = 0xFF,
};

enum eQMI_VOICE_PRIVACY {
    VOICE_PRIVACY_STANDARD    = 0x01,
    VOICE_PRIVACY_ENHANCED    = 0x02,
    VOICE_PRIVACY_UNAVAILABLE = 0xFF,
};

enum eQMI_SUPS_TYPE {
    SUPS_TYPE_RELEASE_HELD_OR_WAITING               = 0x01,
    SUPS_TYPE_RELEASE_ACTIVE_ACCEPT_HELD_OR_WAITING = 0x02,
    SUPS_TYPE_HOLD_ACTIVE_ACCEPT_WAITING_OR_HELD    = 0x03,
    SUPS_TYPE_HOLD_ALL_EXCEPT_SPECIFIED_CALL        = 0x04,
    SUPS_TYPE_MAKE_CONFERENCE_CALL                  = 0x05,
    SUPS_TYPE_EXPLICIT_CALL_TRANSFER                = 0x06,
    SUPS_TYPE_CCBS_ACTIVATION                       = 0x07,
    SUPS_TYPE_END_ALL_CALLS                         = 0x08,
    SUPS_TYPE_RELEASE_SPECIFIED_CALL                = 0x09,
};

/* Last call fail cause */
static int s_lastCallFailCause = CALL_FAIL_ERROR_UNSPECIFIED;


/**
 * Converts the call state information from QMI to RIL
 *
 * @param [in] state
 *     QMI call state information
 * @param [out] p_state
 *     converted RIL call state information
 *
 * @return
 *     true, if the call information is retrieved; false, otherwise
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 */
bool qmiCallStateToRILState(
    enum eQMI_CALL_STATES state,
    RIL_CallState *p_state)
{
    bool stateStatus = true;

    switch (state) {
        /**
         * Disconnect and End are grouped with active call, as there is no 
         * corresponding RIL state. RIL would disconnect the next time
         * the call state changes (call ID would not be present)
         */
        case CALL_STATE_CONVERSATION:
        case CALL_STATE_END:
        case CALL_STATE_DISCONNECTING:
            *p_state = RIL_CALL_ACTIVE;
            break;

        case CALL_STATE_HOLD:
            *p_state = RIL_CALL_HOLDING;
            break;

        case CALL_STATE_ORIGINATION:
        case CALL_STATE_SETUP:
        case CALL_STATE_CC_IN_PROGRESS:
            *p_state = RIL_CALL_DIALING;
            break;

        case CALL_STATE_ALERTING:
            *p_state = RIL_CALL_ALERTING;
            break;

        case CALL_STATE_INCOMING:
            *p_state = RIL_CALL_INCOMING;
            break;

        case CALL_STATE_WAITING:
            *p_state = RIL_CALL_WAITING;
            break;

        case CALL_STATE_UNAVAILABLE:
        default:
            stateStatus = false;
            break;
    }
    return stateStatus;
}

/**
 * Converts the call information from SLQS to the RIL_Call type
 *
 * @param [in] pAllCallInfo
 *     call information received from SLQS
 * @param [out] pCalls
 *     call information which needs to be filled to be sent to application layer
 *
 * @return
 *     true, if the call information is retrieved; false, otherwise
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 */
bool getCallInformation (voiceGetAllCallInfo *pAllCallInfo, RIL_Call *pCalls)
{
    int countCalls = 0, countNumbers = 0, countNames = 0, length = 0;
    bool callInfoStatus = true;
    int i = 0, j = 0, k = 0;

    LOGD("%s: Entered", __func__);

    if (pAllCallInfo->pArrCallInfo != NULL) {
        countCalls = pAllCallInfo->pArrCallInfo->numInstances;
    } else {
        callInfoStatus = false;
    }

    LOGD("%s: countCalls : %d", __func__, countCalls);

    for (i = 0; i < countCalls; i++) {
        getAllCallInformation *pCurCallInfo =
                    &pAllCallInfo->pArrCallInfo->getAllCallInfo[i];
        RIL_Call *pCurCall = pCalls + i;

        pCurCall->index = pCurCallInfo->Callinfo.callID;

        /* Get the call information, if error return to the request routine */
        callInfoStatus = qmiCallStateToRILState(
                            pCurCallInfo->Callinfo.callState, &pCurCall->state);
        if (!callInfoStatus)
            break;

        /* Is call mobile terminted */
        pCurCall->isMT = 
            (pCurCallInfo->Callinfo.direction == CALL_DIRECTION_MO)?0:1;

        /* Is Multiparty enabled */
        pCurCall->isMpty = pCurCallInfo->isEmpty;

        /* Alternate Line Service Indicator */
        pCurCall->als = pCurCallInfo->ALS;

        /* Is the current call a voice call */
        pCurCall->isVoice = (pCurCallInfo->Callinfo.callType == 0)?1:0;

        /* Is CDMA voice privacy enabled */
        if (*(pAllCallInfo->pVoicePrivacy) != VOICE_PRIVACY_UNAVAILABLE) {
            pCurCall->isVoicePrivacy = 1;
        }

        /* Type of address */
        pCurCall->toa = pCurCallInfo->Callinfo.callType;

        /* Update the number and number presentation */
        countNumbers = pAllCallInfo->pArrRemotePartyNum->numInstances;
        for (j = 0; j < countNumbers; j++) {
            getAllCallRmtPtyNum *pCurNumInfo =
                    &pAllCallInfo->pArrRemotePartyNum->RmtPtyNum[j];   

            /* Copy the number information if the CallID matches */
            if (pCurNumInfo->callID == pCurCallInfo->Callinfo.callID){
                length = pCurNumInfo->RemotePartyNum.numLen;
                pCurNumInfo->RemotePartyNum.remPartyNumber[length] = 0;
                pCurCall->number = 
                        (char *)pCurNumInfo->RemotePartyNum.remPartyNumber;
                pCurCall->numberPresentation = 
                        pCurNumInfo->RemotePartyNum.presentationInd;
            }
        }

        /* Update the name and name presentation */
        countNames = pAllCallInfo->pArrRemotePartyName->numInstances;
        for (k = 0; k < countNames; k++) {
            getAllCallRmtPtyName *pCurNameInfo =
                    &pAllCallInfo->pArrRemotePartyName->GetAllCallRmtPtyName[k];

            /* Copy the name information if the CallID matches */
            if (pCurNameInfo->callID == pCurCallInfo->Callinfo.callID){
                length = pCurNameInfo->RemotePartyName.nameLen;
                pCurNameInfo->RemotePartyName.callerName[length] = 0;
                pCurCall->name =
                        (char *) pCurNameInfo->RemotePartyName.callerName;
                pCurCall->namePresentation =
                         pCurNameInfo->RemotePartyName.namePI;
            }
        }
    }
    return callInfoStatus;
}

/**
 * RIL_REQUEST_GET_CURRENT_CALLS
 * Requests current call list
 *
 * @param [in] data
 *     "data" is NULL
 * @param datalen
 *     length of the data parameter received
 * @param t
 *     token passed to the RIL command request
 *
 * @return
 *     none
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 */
void requestGetCurrentCalls(void *data, size_t datalen, RIL_Token t)
{
    int countCalls, i;
    RIL_Call *calls;
    RIL_Call **response = NULL;
    ULONG nRet = eQCWWAN_ERR_NONE;
    /* Parameters required for getting the voice information */
    voiceGetAllCallInfo allCallInfo;
    arrCallInfo         callInformation;
    arrRemotePartyNum   remotePartyNum;
    arrRemotePartyName  remotePartyName;
    BYTE                voicePrivacy;

    LOGD("%s: Entered", __func__);

    /** 
     * Need to assign memory only for the parameters which needs to be
     * fetched
     */
    memset( &allCallInfo, 0, sizeof(allCallInfo));
    allCallInfo.pArrCallInfo        = &callInformation;
    allCallInfo.pArrRemotePartyNum  = &remotePartyNum;
    allCallInfo.pArrRemotePartyName = &remotePartyName;
    allCallInfo.pVoicePrivacy       = &voicePrivacy;

    nRet = SLQSVoiceGetAllCallInfo (&allCallInfo);

    if ( nRet != eQCWWAN_ERR_NONE ) {
        LOGD("%s: SLQSVoiceGetAllCallInfo() failed %lu", __func__, nRet);
        goto error;
    }

    countCalls = allCallInfo.pArrCallInfo->numInstances;
    if ( countCalls == 0 ) {
        LOGD("%s: No Active/Waiting/Held Call", __func__);
        goto finally;
    }


    /* There's an array of pointers and then an array of structures. */
    response = (RIL_Call **) alloca(countCalls * sizeof(RIL_Call *));
    calls = (RIL_Call *) alloca(countCalls * sizeof(RIL_Call));
    memset(calls, 0, countCalls * sizeof(RIL_Call));

    /* Init the pointer array. */
    for (i = 0; i < countCalls; i++) {
        response[i] = &(calls[i]);
    }

    if(!getCallInformation(&allCallInfo, calls))
        goto error;

finally:
    RIL_onRequestComplete(t, RIL_E_SUCCESS, response,
                          countCalls * sizeof(RIL_Call *));
    return;

error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
}

/**
 * Enqueued function to send the call state change unsolicited response
 *
 * @param [in] params
 *     NULL in this case
 *
 * @return
 *     none
 */
static void onCallStateChange (void *params)
{
    RIL_onUnsolicitedResponse(RIL_UNSOL_RESPONSE_CALL_STATE_CHANGED,
                              NULL,
                              0);
}

/**
 * It Displays call information of all call in progress
 *
 * @param [in] pCallBackInfo
 *     All call state information
 *
 * @return
 *     none
 */
void VoiceCallInfoCallback( voiceSetAllCallStatusCbkInfo *pCallBackInfo )
{
    int i = 0;
    LOGD("%s: Enqueue the state change indication", __func__);

    if (pCallBackInfo->pArrCallEndReason != NULL) {
        arrCallEndReason *pCallEndReason = pCallBackInfo->pArrCallEndReason;
        i = pCallEndReason->numInstances - 1; 
        s_lastCallFailCause = pCallEndReason->callEndReason[i];
    }

    /**
     * Enqueue the function to send the usolicited response for the call state
     * change. We do not require to send anything else as the GET_CURRENT_CALLS
     * would be invoked subsequently to get the call information
     * TBD - Need to decide whether we need to send the RING indication, as of
     * now seems to be redundant as the CALL_STATE_CHANGED has to be sent anyway
     */ 
    enqueueRILEvent(RIL_EVENT_QUEUE_PRIO, onCallStateChange, NULL, NULL);
}

/**
 * Register voice notification call back function
 *
 * @param 
 *     none
 *
 * @return
 *     none
 */
void registerVoiceCallbackQMI(void)
{
    LOGD("%s:: entered\n", __func__);
    ULONG nRet = eQCWWAN_ERR_NONE;

    /* register callback function */
    nRet = SLQSVoiceSetAllCallStatusCallBack (&VoiceCallInfoCallback);
    if ( nRet != eQCWWAN_ERR_NONE ) {
        LOGE("%s:: SLQSVoiceSetAllCallStatusCallBack failed, nRet: %lu\n",
             __func__, nRet );
    }
}

/**
 * RIL_REQUEST_LAST_CALL_FAIL_CAUSE
 * Requests the failure cause code for the most recently terminated call
 *
 * @param [in] data
 *     "data" is NULL
 * @param datalen
 *     length of the data parameter received
 * @param t
 *     token passed to the RIL command request
 *
 * @return
 *     none
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     See also: RIL_REQUEST_LAST_PDP_FAIL_CAUSE
 */
void requestLastCallFailCause(void *data, size_t datalen, RIL_Token t)
{
    RIL_onRequestComplete(t, RIL_E_SUCCESS, &s_lastCallFailCause,
                              sizeof(int));
}

/**
 * Store the CLIR value set using RIL_REQUEST_SET_CLIR
 *
 * @param [in] clirVal
 *     CLIR value to be set
 *
 * @return
 *     none
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 */
void setCLIRVal(int clirVal)
{    
    switch (clirVal) {
        case CLIR_INVOCATION:
            storedCLIRVal = GPP_CLIR_INVOCATION;
        case CLIR_SUPPRESSION:
            storedCLIRVal = GPP_CLIR_SUPPRESSION;
        default:
            storedCLIRVal = GPP_CLIR_NOCHANGE;
    }
}

/**
 * Retreive the CLIR value set using RIL_REQUEST_SET_CLIR
 *
 * @param
 *     none
 *
 * @return
 *     none
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 */
static BYTE getStoredCLIRVal()
{
    return storedCLIRVal;
}

/**
 * Convert the CLIR activation status from QMI representation to RIL
 * representation
 *
 * @param [in] clirActiveStatus
 *     activation status in QMI format
 *
 * @return
 *     activation status in 3GPP format
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 */
BYTE convertCLIRActiveStatus(BYTE clirActiveStatus)
{
    BYTE clirActStatus;

    switch (clirActiveStatus) {
        case CLIR_INACTIVE:
            clirActStatus = GPP_CLIR_SUPPRESSION;
            break;
        case CLIR_ACTIVE:
            clirActStatus = GPP_CLIR_INVOCATION;
            break;
        default:
            clirActStatus = GPP_CLIR_NOCHANGE;
            break;
    }
    return clirActStatus;
}

/**
 * Convert the CLIR provision status from QMI representation to RIL
 * representation
 *
 * @param [in] clirProvisionStatus
 *     provision status in QMI format
 *
 * @return
 *     provision status in 3GPP format
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 */
BYTE convertCLIRProvStatus(BYTE clirProvisionStatus)
{
    BYTE clirProvStatus;

    switch (clirProvisionStatus) {
        case CLIR_NOT_PROVISIONED:
            clirProvStatus = GPP_CLIR_NOT_PROVISIONED;
            break;
        case CLIR_PROVISIONED_PERM:
            clirProvStatus = GPP_CLIR_PROVISIONED_PERM;
            break;
        case CLIR_TEMP_RESTRICTED:
            clirProvStatus = GPP_CLIR_TEMP_RESTRICTED;
            break;
        case CLIR_PRESENTATION_ALLOWED:
            clirProvStatus = GPP_CLIR_PRESENTATION_ALLOWED;
            break;
        default:
            clirProvStatus = GPP_CLIR_UNKNOWN;
            break;
    }
    return clirProvStatus;
}

/**
 * RIL_REQUEST_DIAL
 * Initiate voice call
 *
 * @param [in] data
 *     data is const RIL_Dial *
 * @param datalen
 *     length of the data parameter received
 * @param t
 *     token passed to the RIL command request
 *
 * @return
 *     none
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 */
void requestDial(void *data, size_t datalen, RIL_Token t)
{
    RIL_Dial *dial;    
    voiceCallRequestParams  CallRequestParams;
    voiceCallResponseParams CallResponseParams;
    BYTE clirVal = 0;
    ULONG nRet = eQCWWAN_ERR_NONE;

    dial = (RIL_Dial *) data;
    memset( &CallRequestParams, 0, sizeof(CallRequestParams));
    memset( &CallResponseParams, 0, sizeof(CallResponseParams));

    /* Copy the destination address */
    strcpy((char *)CallRequestParams.callNumber, dial->address);

    /* Use the CLIR value which is set using the RIL_REQUEST_SET_CLIR request */
    clirVal = getStoredCLIRVal();
    if (clirVal != CLIR_NOT_SET) {
        CallRequestParams.pCLIRType = &clirVal;
    }

    /* Set the CLIR value, do not set the value if not provided */
    switch (dial->clir) {
        /* Invocation */
        case 1:
            clirVal = CLIR_INVOCATION;
            CallRequestParams.pCLIRType = &clirVal;
            break;
        /* Suppression */
        case 2:
            clirVal = CLIR_SUPPRESSION;
            CallRequestParams.pCLIRType = &clirVal;
            break;
        /* Subscribtion default */
        case 0:
        default:
            break;
    }

    nRet = SLQSVoiceDialCall( &CallRequestParams, &CallResponseParams );

    if ( nRet != eQCWWAN_ERR_NONE ) {
        LOGD("%s: SLQSVoiceDialCall() failed %lu\n", __func__, nRet);
        goto error;
    }

    /* Success or failure is ignored by the upper layer here,
       it will call GET_CURRENT_CALLS and determine success that way. */
    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
    return;

error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
}

/**
 * Get the call ID for the incoming call state
 *
 * @param callState
 *     The call state for which the call ID inforamtion need to be fetched
 *
 * @return
 *      call ID of the alerting call
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 */
BYTE getCallID (RIL_CallState callState)
{
    BYTE callID = 0;
    RIL_CallState state;
    int countCalls = 0, i = 0;
    bool status;
    ULONG nRet = eQCWWAN_ERR_NONE;
    /* Parameters required for getting the voice information */
    voiceGetAllCallInfo allCallInfo;
    arrCallInfo         callInformation;

    LOGD("%s: Entered", __func__);

    /** 
     * Need to assign memory only for the parameters which needs to be
     * fetched
     */
    memset( &allCallInfo, 0, sizeof(allCallInfo));
    allCallInfo.pArrCallInfo        = &callInformation;

    nRet = SLQSVoiceGetAllCallInfo (&allCallInfo);

    countCalls = allCallInfo.pArrCallInfo->numInstances;

    for (i = 0; i < countCalls; i++) {
        getAllCallInformation *pCurCallInfo =
                    &allCallInfo.pArrCallInfo->getAllCallInfo[i];
        status = qmiCallStateToRILState( pCurCallInfo->Callinfo.callState,
                                         &state);

        if ((status) && (state == callState)) {
            callID = pCurCallInfo->Callinfo.callID;
            break;
        }
    }
    return callID;
}

/**
 * RIL_REQUEST_ANSWER
 * Answer incoming call
 *
 * @param [in] data
 *     "data" is NULL
 * @param datalen
 *     length of the data parameter received
 * @param t
 *     token passed to the RIL command request
 *
 * @return
 *     none
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     Will not be called for WAITING calls.
 *     RIL_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE will be used in this 
 *     case instead
 */
void requestAnswer(void *data, size_t datalen, RIL_Token t)
{
    BYTE callID;
    voiceAnswerCall answerCallParams;
    ULONG nRet = eQCWWAN_ERR_NONE;

    callID = getCallID(RIL_CALL_INCOMING);
    answerCallParams.pCallId = &callID;

    nRet = SLQSVoiceAnswerCall( &answerCallParams );

    if ( nRet != eQCWWAN_ERR_NONE ) {
        LOGD("%s: SLQSVoiceAnswerCall() failed %lu\n", __func__, nRet);
        goto error;
    }

    /* Success or failure is ignored by the upper layer here,
       it will call GET_CURRENT_CALLS and determine success that way. */
    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
    return;

error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
}

/**
 * RIL_REQUEST_HANGUP
 * Hang up a specific line
 *
 * @param [in] data
 *     data is an "int *"
 *     (int *)data)[0] contains Connection index
 * @param datalen
 *     length of the data parameter received
 * @param t
 *     token passed to the RIL command request
 *
 * @return
 *     none
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 */
void requestHangup(void *data, size_t datalen, RIL_Token t)
{
    BYTE cid;
    ULONG nRet = eQCWWAN_ERR_NONE;

    cid = ((int *) data)[0];

    nRet = SLQSVoiceEndCall( &cid );

    if ( nRet != eQCWWAN_ERR_NONE ) {
        LOGD("%s: SLQSVoiceEndCall() failed %lu\n", __func__, nRet);
        goto error;
    }

    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
    return;

error:
    /* Success or failure is ignored by the upper layer here,
       it will call GET_CURRENT_CALLS and determine success that way. */
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
}

/**
 * RIL_REQUEST_HANGUP_WAITING_OR_BACKGROUND
 * Hang up waiting or held
 *
 * @param [in] data
 *     "data" is NULL
 * @param datalen
 *     length of the data parameter received
 * @param t
 *     token passed to the RIL command request
 *
 * @return
 *     none
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 */
void requestHangupWaitingOrBackground(void *data, size_t datalen,
                                      RIL_Token t)
{
    ULONG nRet = eQCWWAN_ERR_NONE;
    voiceManageCallsReq  manageCallsReq;
    voiceManageCallsResp manageCallsResp;

    memset( &manageCallsReq, 0, sizeof(manageCallsReq));

    /**
     * Release the held call, and make the backgroud call active; if there are
     * not any background calls no active calls would be present after this
     */ 
    manageCallsReq.SUPSType = SUPS_TYPE_RELEASE_HELD_OR_WAITING;
    nRet = SLQSVoiceManageCalls (&manageCallsReq, &manageCallsResp);

    if ( nRet != eQCWWAN_ERR_NONE ) {
        LOGD("%s: SLQSVoiceManageCalls() failed %lu\n", __func__, nRet);
        goto error;
    }

    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
    return;

error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);}

/**
 * RIL_REQUEST_HANGUP_FOREGROUND_RESUME_BACKGROUND
 * Hang up waiting or held
 *
 * @param [in] data
 *     "data" is NULL
 * @param datalen
 *     length of the data parameter received
 * @param t
 *     token passed to the RIL command request
 *
 * @return
 *     none
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 */
void requestHangupForegroundResumeBackground(void *data, size_t datalen,
                                             RIL_Token t)
{
    ULONG nRet = eQCWWAN_ERR_NONE;
    voiceManageCallsReq  manageCallsReq;
    voiceManageCallsResp manageCallsResp;

    memset( &manageCallsReq, 0, sizeof(manageCallsReq));

    /**
     * Release the held call, and make the backgroud call active; if there are
     * not any background calls no active calls would be present after this
     */ 
    manageCallsReq.SUPSType = SUPS_TYPE_RELEASE_ACTIVE_ACCEPT_HELD_OR_WAITING;
    nRet = SLQSVoiceManageCalls (&manageCallsReq, &manageCallsResp);

    if ( nRet != eQCWWAN_ERR_NONE ) {
        LOGD("%s: SLQSVoiceManageCalls() failed %lu\n", __func__, nRet);
        goto error;
    }

    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
    return;

error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
}

/**
 * RIL_REQUEST_UDUB
 * Send UDUB (user determined used busy) to ringing or waiting call answer
 * (RIL_BasicRequest r).
 *
 * @param [in] data
 *     data is NULL
 * @param datalen
 *     length of the data parameter received
 * @param t
 *     token passed to the RIL command request
 *
 * @return
 *     none
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 */
void requestUDUB(void *data, size_t datalen, RIL_Token t)
{
    ULONG nRet = eQCWWAN_ERR_NONE;
    voiceManageCallsReq  manageCallsReq;
    voiceManageCallsResp manageCallsResp;

    memset( &manageCallsReq, 0, sizeof(manageCallsReq));

    /**
     * Release the held call, and make the backgroud call active; if there are
     * not any background calls no active calls would be present after this
     */ 
    manageCallsReq.SUPSType = SUPS_TYPE_END_ALL_CALLS;
    nRet = SLQSVoiceManageCalls (&manageCallsReq, &manageCallsResp);

    if ( nRet != eQCWWAN_ERR_NONE ) {
        LOGD("%s: SLQSVoiceManageCalls() failed %lu\n", __func__, nRet);
        goto error;
    }

    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
    return;

error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
}

/**
 * RIL_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE
 * Switch waiting or holding call and active call
 *
 * @param [in] data
 *     "data" is NULL
 * @param datalen
 *     length of the data parameter received
 * @param t
 *     token passed to the RIL command request
 *
 * @return
 *     none
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 */
void requestSwitchWaitingOrHoldingAndActive(void *data, size_t datalen,
                                            RIL_Token t)
{
    ULONG nRet = eQCWWAN_ERR_NONE;
    voiceManageCallsReq  manageCallsReq;
    voiceManageCallsResp manageCallsResp;

    memset( &manageCallsReq, 0, sizeof(manageCallsReq));

    /**
     * Release the held call, and make the backgroud call active; if there are
     * not any background calls no active calls would be present after this
     */ 
    manageCallsReq.SUPSType = SUPS_TYPE_HOLD_ACTIVE_ACCEPT_WAITING_OR_HELD;
    nRet = SLQSVoiceManageCalls (&manageCallsReq, &manageCallsResp);

    if ( nRet != eQCWWAN_ERR_NONE ) {
        LOGD("%s: SLQSVoiceManageCalls() failed %lu\n", __func__, nRet);
        goto error;
    }

    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
    return;

error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
}

/**
 * RIL_REQUEST_CONFERENCE
 * Conference holding and active (like AT+CHLD=3)
 *
 * @param data
 *     "data" is NULL
 * @param datalen
 *     length of the data parameter received
 * @param t
 *     token passed to the RIL command request
 *
 * @return
 *     none
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 */
void requestConference(void *data, size_t datalen, RIL_Token t)
{
    ULONG nRet = eQCWWAN_ERR_NONE;
    voiceManageCallsReq  manageCallsReq;
    voiceManageCallsResp manageCallsResp;

    memset( &manageCallsReq, 0, sizeof(manageCallsReq));

    /**
     * Release the held call, and make the backgroud call active; if there are
     * not any background calls no active calls would be present after this
     */ 
    manageCallsReq.SUPSType = SUPS_TYPE_MAKE_CONFERENCE_CALL;
    nRet = SLQSVoiceManageCalls (&manageCallsReq, &manageCallsResp);

    if ( nRet != eQCWWAN_ERR_NONE ) {
        LOGD("%s: SLQSVoiceManageCalls() failed %lu\n", __func__, nRet);
        goto error;
    }

    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
    return;

error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
}

/**
 * RIL_REQUEST_SEPARATE_CONNECTION
 * Separate a party from a multiparty call placing the multiparty call
 * (less the specified party) on hold and leaving the specified party 
 * as the only other member of the current (active) call
 *
 * @param [in] data
 *     data is an "int *"
 *     (int *)data)[0] contains Connection index (value of 'x' in CHLD above)
 * @param datalen
 *     length of the data parameter received
 * @param t
 *     token passed to the RIL command request
 *
 * @return
 *     none
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     See TS 22.084 1.3.8.2 (iii)
 *     TS 22.030 6.5.5 "Entering "2X followed by send"
 */
void requestSeparateConnection(void *data, size_t datalen, RIL_Token t)
{
    ULONG nRet = eQCWWAN_ERR_NONE;
    BYTE party = (BYTE)((int *) data)[0];
    voiceManageCallsReq  manageCallsReq;
    voiceManageCallsResp manageCallsResp;

    /* Make sure that party is a single digit. */
    if (party < 1 || party > 9)
        goto error;

    memset( &manageCallsReq, 0, sizeof(manageCallsReq));

    /**
     * Release the held call, and make the backgroud call active; if there are
     * not any background calls no active calls would be present after this
     */ 
    manageCallsReq.SUPSType = SUPS_TYPE_HOLD_ALL_EXCEPT_SPECIFIED_CALL;
    manageCallsReq.pCallID  = &party;
    nRet = SLQSVoiceManageCalls (&manageCallsReq, &manageCallsResp);

    if ( nRet != eQCWWAN_ERR_NONE ) {
        LOGD("%s: SLQSVoiceManageCalls() failed %lu\n", __func__, nRet);
        goto error;
    }

    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
    return;

error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
}

/**
 * RIL_REQUEST_EXPLICIT_CALL_TRANSFER
 * Connects the two calls and disconnects the subscriber from both calls
 *
 * @param [in] data
 *     "data" is NULL
 * @param datalen
 *     length of the data parameter received
 * @param t
 *     token passed to the RIL command request
 *
 * @return
 *     none
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 */
void requestExplicitCallTransfer(void *data, size_t datalen, RIL_Token t)
{
    ULONG nRet = eQCWWAN_ERR_NONE;
    voiceManageCallsReq  manageCallsReq;
    voiceManageCallsResp manageCallsResp;

    memset( &manageCallsReq, 0, sizeof(manageCallsReq));

    /**
     * Release the held call, and make the backgroud call active; if there are
     * not any background calls no active calls would be present after this
     */ 
    manageCallsReq.SUPSType = SUPS_TYPE_EXPLICIT_CALL_TRANSFER;
    nRet = SLQSVoiceManageCalls (&manageCallsReq, &manageCallsResp);

    if ( nRet != eQCWWAN_ERR_NONE ) {
        LOGD("%s: SLQSVoiceManageCalls() failed %lu\n", __func__, nRet);
        goto error;
    }

    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
    return;

error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
}

