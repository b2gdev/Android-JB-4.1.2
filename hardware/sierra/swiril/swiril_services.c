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
#include <assert.h>
#include <telephony/ril.h>
#include "swiril_main.h"

#define LOG_TAG "RIL"
#include "swiril_log.h"

/* For QMI */
#include "qmerrno.h"
#include "SWIWWANCMAPI.h"

BYTE setCLIRVal(int clirVal);
BYTE convertCLIRActiveStatus(BYTE clirActiveStatus);
BYTE convertCLIRProvStatus(BYTE clirProvisionStatus);

#define QMI_CALL_SUPS_ACTIVATE   0x01
#define QMI_CALL_SUPS_DEACTIVATE 0x02
#define QMI_CALL_SUPS_REGISTER   0x03
#define QMI_CALL_SUPS_ERASURE    0x04

#define GPP_CALL_SUPS_DEACTIVATE 0x00
#define GPP_CALL_SUPS_ACTIVATE   0x01
#define GPP_CALL_SUPS_REGISTER   0x03
#define GPP_CALL_SUPS_ERASURE    0x04

#define CALL_WAITING_REASON  0x0F

/* QMI call forwarding reasons */
#define QMI_FWDREASON_UNCONDITIONAL  0x01
#define QMI_FWDREASON_MOBILEBUSY     0x02
#define QMI_FWDREASON_NOREPLY        0x03
#define QMI_FWDREASON_UNREACHABLE    0x04
#define QMI_FWDREASON_ALLFORWARDING  0x05
#define QMI_FWDREASON_ALLCONDITIONAL 0x06

/* 3GPP call forwarding reasons */
#define GPP_FWDREASON_UNCONDITIONAL  0x00
#define GPP_FWDREASON_MOBILEBUSY     0x01
#define GPP_FWDREASON_NOREPLY        0x02
#define GPP_FWDREASON_UNREACHABLE    0x03
#define GPP_FWDREASON_ALLFORWARDING  0x04
#define GPP_FWDREASON_ALLCONDITIONAL 0x05

/**
 * Convert 3GPP call-forwarding modes to QMI ones
 *
 * @param [in] gppCallForwReason
 *     3GPP call forwarding modes
 *
 * @return
 *     QMI call forwarding modes
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 */
BYTE convertCFModeFrom3GPPtoQMI (BYTE gppCallForwMode)
{
    BYTE QmiCallForwMode;
    switch (gppCallForwMode) {
        case GPP_CALL_SUPS_ACTIVATE:
            QmiCallForwMode = QMI_CALL_SUPS_ACTIVATE;
            break;
        case GPP_CALL_SUPS_DEACTIVATE:
            QmiCallForwMode = QMI_CALL_SUPS_DEACTIVATE;
            break;
        case GPP_CALL_SUPS_REGISTER:
            QmiCallForwMode = QMI_CALL_SUPS_REGISTER;
            break;
        case GPP_CALL_SUPS_ERASURE:
            QmiCallForwMode = QMI_CALL_SUPS_ERASURE;
            break;
        default :
            QmiCallForwMode = QMI_CALL_SUPS_ACTIVATE;
            break;
    }
    return QmiCallForwMode;
}

/**
 * Convert 3GPP call-forwarding reasons to QMI ones
 *
 * @param [in] gppCallForwReason
 *     3GPP call forwarding reasons
 *
 * @return
 *     QMI call forwarding reasons
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 */
BYTE convertCFReasonFrom3GPPtoQMI (BYTE gppCallForwReason)
{
    BYTE QmiCallForwReason;
    switch (gppCallForwReason) {
        case GPP_FWDREASON_UNCONDITIONAL:
            QmiCallForwReason = QMI_FWDREASON_UNCONDITIONAL;
            break;
        case GPP_FWDREASON_MOBILEBUSY:
            QmiCallForwReason = QMI_FWDREASON_MOBILEBUSY;
            break;
        case GPP_FWDREASON_NOREPLY:
            QmiCallForwReason = QMI_FWDREASON_NOREPLY;
            break;
        case GPP_FWDREASON_UNREACHABLE:
            QmiCallForwReason = QMI_FWDREASON_UNREACHABLE;
            break;
        case GPP_FWDREASON_ALLFORWARDING:
            QmiCallForwReason = QMI_FWDREASON_ALLFORWARDING;
            break;
        case GPP_FWDREASON_ALLCONDITIONAL:
            QmiCallForwReason = QMI_FWDREASON_ALLCONDITIONAL;
            break;
        default :
            QmiCallForwReason = QMI_FWDREASON_UNCONDITIONAL;
            break;
    }
    return QmiCallForwReason;
}

/**
 * RIL_REQUEST_QUERY_CLIP
 * Queries the status of the CLIP supplementary service.
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
void requestQueryClip(void *data, size_t datalen, RIL_Token t)
{
    CLIPResp clipResp;
    voiceGetCLIPResp resp;
    int response = 2;

    memset( &resp, 0, sizeof(resp) );
    resp.pCLIPResp    = &clipResp;

    ULONG nRet = SLQSVoiceGetCLIP( &resp );
    if (eQCWWAN_ERR_NONE != nRet) {
        LOGE("%s:: SLQSVoiceGetCLIP failed, nRet: %lu\n",__func__, nRet );
        goto error;
    }

    if (NULL != resp.pCLIPResp) {
        response = resp.pCLIPResp->ProvisionStatus;
    }

    RIL_onRequestComplete(t, RIL_E_SUCCESS, &response, sizeof(int));
    return;

error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
}

/**
 * RIL_REQUEST_GET_CLIR
 * Gets current CLIR status.
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
void requestGetCLIR(void *data, size_t datalen, RIL_Token t)
{
    CLIRResp clirResp;
    voiceGetCLIRResp resp;
    int response[2] = {0,2};
    ULONG nRet;

    memset( &resp, 0, sizeof(resp) );
    resp.pCLIRResp    = &clirResp;

    nRet = SLQSVoiceGetCLIR( &resp );
    if (eQCWWAN_ERR_NONE != nRet) {
        LOGE("%s:: SLQSVoiceGetCLIR failed, nRet: %lu\n",__func__, nRet );
        goto error;
    }

    if (NULL != resp.pCLIRResp) {
        response[0] = convertCLIRActiveStatus(resp.pCLIRResp->ActiveStatus);
        response[1] = convertCLIRProvStatus(resp.pCLIRResp->ProvisionStatus);
    }

    RIL_onRequestComplete(t, RIL_E_SUCCESS, response, sizeof(response));
    return;

error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
}

/**
 * RIL_REQUEST_SET_CLIR
 * Sets the CLIR status.
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
 *     CLIR activation is not supported because the 3GPP specification does not
 *     allow CLIR activation; we remember the CLIR type and use the same in
 *     RIL_REQUEST_DIAL
 */
void requestSetCLIR(void *data, size_t datalen, RIL_Token t)
{
    setCLIRVal(((int *) data)[0]);
    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
}

/**
 * RIL_REQUEST_QUERY_CALL_FORWARD_STATUS
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
void requestQueryCallForwardStatus(void *data, size_t datalen, RIL_Token t)
{
    const RIL_CallForwardInfo *pCallForwardInfo = (RIL_CallForwardInfo *) data;
    int n;
    int i = 0;
    ULONG nRet;
    RIL_CallForwardInfo *rilResponse;
    RIL_CallForwardInfo **rilResponseArray;
    /* SLQS structures */
    voiceGetCallFWReq  getCallFWReq;
    voiceGetCallFWResp getCallFWResp;
    getCallFWInfo GetCallFWInfo, *pGetCallFWInfo;

    /* Initialize request and response structures */
    memset (&getCallFWReq, 0, sizeof(getCallFWReq));
    memset (&getCallFWResp, 0, sizeof(getCallFWResp));
    getCallFWReq.Reason =
            convertCFReasonFrom3GPPtoQMI (pCallForwardInfo->reason);
    getCallFWResp.pGetCallFWInfo = &GetCallFWInfo;

    nRet = SLQSVoiceGetCallForwardingStatus (&getCallFWReq, &getCallFWResp);
    if (eQCWWAN_ERR_NONE != nRet) {
        LOGE("%s:: SLQSVoiceGetCallForwardingStatus failed, nRet: %lu\n",
                __func__, nRet );
        goto error;
    }

    /* Get the number of occurances */
    pGetCallFWInfo = getCallFWResp.pGetCallFWInfo;
    n = pGetCallFWInfo->numInstances;

    /* Initialize the RIL response structures */
    rilResponse = alloca(n * sizeof(RIL_CallForwardInfo));
    rilResponseArray = alloca(n * sizeof(RIL_CallForwardInfo *));
    memset(rilResponse, 0, sizeof(RIL_CallForwardInfo) * n);

    for (i = 0; i < n; i++) {
        rilResponseArray[i] = &(rilResponse[i]);
    }

    /* Get the Call Fwd infor from SLQS to RIL response structure */
    for (i = 0; i < n; i++) {
        BYTE numberIndex = pGetCallFWInfo->CallFWInfo[i].numLen;
        rilResponse[i].status = pGetCallFWInfo->CallFWInfo[i].SvcStatus;
        rilResponse[i].serviceClass = pGetCallFWInfo->CallFWInfo[i].SvcClass;
        pGetCallFWInfo->CallFWInfo[i].number[numberIndex] = 0;
        rilResponse[i].number = (char *)pGetCallFWInfo->CallFWInfo[i].number;
        rilResponse[i].toa = 145;
        rilResponse[i].timeSeconds = pGetCallFWInfo->CallFWInfo[i].noReplyTimer;
    }

    RIL_onRequestComplete(t, RIL_E_SUCCESS, rilResponseArray,
                          n * sizeof(RIL_CallForwardInfo *));
    return;

error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
}

/**
 * RIL_REQUEST_SET_CALL_FORWARD
 * Configure call forward rule.
 *
 * @param [in] data
 *     "data" is const RIL_CallForwardInfo *
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
void requestSetCallForward(void *data, size_t datalen, RIL_Token t)
{
    BYTE SvcClass;
    BYTE noReplyTimer;
    voiceSetSUPSServiceReq  req;
    voiceSetSUPSServiceResp resp;
    ULONG nRet;
    const RIL_CallForwardInfo *callForwardInfo = (RIL_CallForwardInfo *) data;

    memset (&req, 0, sizeof(req));
    memset (&resp, 0, sizeof(resp));

    req.voiceSvc = convertCFModeFrom3GPPtoQMI (callForwardInfo->status);
    req.reason = convertCFReasonFrom3GPPtoQMI (callForwardInfo->reason);

    if (callForwardInfo->number != NULL) {
        req.pCallForwardingNumber = (BYTE *)callForwardInfo->number;
        SvcClass = callForwardInfo->serviceClass;
        noReplyTimer = callForwardInfo->timeSeconds;
        req.pTimerVal = &noReplyTimer;
        req.pServiceClass = &SvcClass;
    }

    nRet = SLQSVoiceSetSUPSService( &req , &resp );
    if (eQCWWAN_ERR_NONE != nRet) {
        LOGE("%s:: SLQSVoiceSetSUPSService failed, nRet: %lu\n",
             __func__, nRet );
        goto error;
    }

    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
    return;

error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
}

/**
 * RIL_REQUEST_QUERY_CALL_WAITING
 * Query current call waiting state.
 *
 * @param [in] data
 *     "data" is const int *
 *     ((const int *)data)[0] is the TS 27.007 service class to query.
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
void requestQueryCallWaiting(void *data, size_t datalen, RIL_Token t)
{
    int response[2] = { 0, 0 };
    const int class = ((int *) data)[0];
    ULONG nRet;
    int status = 0;
    voiceGetCallWaitInfo getCallWaitInfo;
    BYTE svcClass = 0;
    ccSUPSType CCSUPSType;

    /* Initialize the request/response structures */
    memset (&CCSUPSType, 0, sizeof(CCSUPSType));
    memset (&getCallWaitInfo, 0, sizeof(getCallWaitInfo));

    svcClass = class;
    getCallWaitInfo.pSvcClass = &svcClass;
    getCallWaitInfo.pCCSUPSType = &CCSUPSType;

    nRet = SLQSVoiceGetCallWaiting (&getCallWaitInfo);
    if (eQCWWAN_ERR_NONE != nRet) {
        LOGE("%s::SLQSVoiceGetCallWaiting failed, nRet: %lu\n",__func__, nRet );
        goto error;
    }

    /* Get the status - active or inactive */
    if (getCallWaitInfo.pCCSUPSType->svcType == 0x01) {
        status = 1;
    } else if (getCallWaitInfo.pCCSUPSType->svcType == 0x02) {
        status = 0;
    }

    /* Fill the service class information in the response structure */
    if (status == 1 && svcClass > 0 && svcClass <= 128) {
        response[1] |= svcClass;
    }

    if (response[1] > 0) {
        response[0] = 1;
    }

    RIL_onRequestComplete(t, RIL_E_SUCCESS, response, sizeof(int) * 2);
    return;

error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
}

/**
 * RIL_REQUEST_SET_CALL_WAITING
 * Configure current call waiting state.
 *
 * @param [in] data
 *     "data" is const int *
 *     ((const int *)data)[0] is 0 for "disabled" and 1 for "enabled"
 *     ((const int *)data)[1] is the TS 27.007 service class bit vector of
 *     services to modify
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
void requestSetCallWaiting(void *data, size_t datalen, RIL_Token t)
{
    voiceSetSUPSServiceReq  req;
    voiceSetSUPSServiceResp resp;
    BYTE svcClass = 0;
    ULONG nRet;
    const int mode = ((int *) data)[0];
    const int class = ((int *) data)[1];

    memset( &req, 0, sizeof(req) );
    memset( &resp, 0, sizeof(resp) );

    if( (mode<0) || (mode>1) ) {
        goto error;
    }

    if (mode == 0) {
        req.voiceSvc = QMI_CALL_SUPS_DEACTIVATE;
    } else {
        req.voiceSvc = QMI_CALL_SUPS_ACTIVATE;
    }

    req.reason = CALL_WAITING_REASON;
    svcClass = class;
    req.pServiceClass = &svcClass;

    nRet = SLQSVoiceSetSUPSService( &req , &resp );
    if (eQCWWAN_ERR_NONE != nRet) {
        LOGE("%s:: SLQSVoiceSetSUPSService failed, nRet: %lu\n",
             __func__, nRet );
        goto error;
    }

    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
    return;

error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
}

