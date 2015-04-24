/*
 * \ingroup cbk
 *
 * \file    qaCbkVoiceSLQSVoiceAllCallStatusInd.c
 *
 * \brief   Contains UnPacking routines for the QMI_VOICE_USSD_IND message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCbkVoiceSLQSVoiceAllCallStatusInd.h"
#include "qaGobiApiVoiceCommon.h"

/* Locals */
static arrRemotePartyNum  RemoteInformationList;
static arrRemotePartyName RemotePartyNameList;
static arrAlertingType    AlertingTypeList;
static arrSvcOption       SvcOptionList;
static arrCallEndReason   CallEndReasonList;
static arrAlphaID         AlphaIDList;
static arrConnectPartyNum ConnectPartyNumList;
static arrDiagInfo        DiagInfoList;
static arrCalledPartyNum  CalledPartyNumList;
static arrRedirPartyNum   RedirPartyNumList;
static arrAlertingPattern AlertingPatternList;

/* Functions */

/*
 * This function unpacks the Call Information TLV parameters from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvCallInformation(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceSetAllCallStatusCbkInfo *lResponse =
        ((voiceSetAllCallStatusCbkInfo *)pResp);

    arrCallInfo *lResp = &lResponse->arrCallInfomation;
    return UnpackTlvCommonArrCallInfo( pTlvData, lResp );
}

/*
 * This function unpacks the Remote Party Number TLV parameters from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvRemPartyNum(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceSetAllCallStatusCbkInfo *lResponse =
        ((voiceSetAllCallStatusCbkInfo *)pResp);

    /* Allocate memory to the pointer */
    lResponse->pArrRemotePartyNum = &RemoteInformationList;
    arrRemotePartyNum *lResp = lResponse->pArrRemotePartyNum;

    return UnpackTlvCommonArrRemPartyNum( pTlvData, lResp );
}

/*
 * This function unpacks the Remote Party name TLV parameters from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvRemPartyName(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceSetAllCallStatusCbkInfo *lResponse =
        ((voiceSetAllCallStatusCbkInfo *)pResp);

    /* Allocate memory to the pointer */
    lResponse->pArrRemotePartyName = &RemotePartyNameList;
    arrRemotePartyName *lResp = lResponse->pArrRemotePartyName;

    return UnpackTlvCommonArrRemPartyName( pTlvData, lResp );
}

/*
 * This function unpacks the AlertType TLV parameters from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvAlertType(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceSetAllCallStatusCbkInfo *lResponse =
        ((voiceSetAllCallStatusCbkInfo *)pResp);

    /* Allocate memory to the pointer */
    lResponse->pArrAlertingType = &AlertingTypeList;

    arrAlertingType *lResp = lResponse->pArrAlertingType;
    return UnpackTlvCommonArrAlertType( pTlvData, lResp );
}

/*
 * This function unpacks the Service Option TLV parameters from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvServiceOpt(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceSetAllCallStatusCbkInfo *lResponse =
        ((voiceSetAllCallStatusCbkInfo *)pResp);

    /* Allocate memory to the pointer */
    lResponse->pArrSvcOption = &SvcOptionList;

    arrSvcOption *lResp = lResponse->pArrSvcOption;
    return UnpackTlvCommonArrServiceOpt( pTlvData, lResp );
}

/*
 * This function unpacks the Call End Reason TLV parameters from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvVoiceCallEndReason(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceSetAllCallStatusCbkInfo *lResponse =
        ((voiceSetAllCallStatusCbkInfo *)pResp);

    /* Allocate memory to the pointer */
    lResponse->pArrCallEndReason = &CallEndReasonList;

    arrCallEndReason *lResp = lResponse->pArrCallEndReason;
    return UnpackTlvCommonArrCallEndReason( pTlvData, lResp );
}

/*
 * This function unpacks the Alpha Identifier TLV parameters from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvAlphaIden(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceSetAllCallStatusCbkInfo *lResponse =
        ((voiceSetAllCallStatusCbkInfo *)pResp);

    /* Allocate memory to the pointer */
    lResponse->pArrAlphaID = &AlphaIDList;

    arrAlphaID *lResp = lResponse->pArrAlphaID;
    return UnpackTlvCommonArrAlphaID( pTlvData, lResp );
}

/*
 * This function unpacks the Connected Party Numer TLV parameters from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvConnPartyNum(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceSetAllCallStatusCbkInfo *lResponse =
        ((voiceSetAllCallStatusCbkInfo *)pResp);

    /* Allocate memory to the pointer */
    lResponse->pArrConnectPartyNum = &ConnectPartyNumList;

    arrConnectPartyNum *lResp = lResponse->pArrConnectPartyNum;
    return UnpackTlvCommonArrConnPartyNum( pTlvData, lResp );
}

/*
 * This function unpacks the Diagnostic Information TLV parameters from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvDiagnosInform(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceSetAllCallStatusCbkInfo *lResponse =
        ((voiceSetAllCallStatusCbkInfo *)pResp);

    /* Allocate memory to the pointer */
    lResponse->pArrDiagInfo = &DiagInfoList;

    arrDiagInfo *lResp = lResponse->pArrDiagInfo;
    return UnpackTlvCommonArrDiagInfo( pTlvData, lResp );
}

/*
 * This function unpacks the Called Party Number TLV parameters from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvCalledPartyNum(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceSetAllCallStatusCbkInfo *lResponse =
        ((voiceSetAllCallStatusCbkInfo *)pResp);

    /* Allocate memory to the pointer */
    lResponse->pArrCalledPartyNum = &CalledPartyNumList;

    arrCalledPartyNum *lResp = lResponse->pArrCalledPartyNum;
    return UnpackTlvCommonArrCalledPartyNum( pTlvData, lResp );
}

/*
 * This function unpacks the Redirect Party Number TLV parameters from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvRedirPartyNum(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceSetAllCallStatusCbkInfo *lResponse =
        ((voiceSetAllCallStatusCbkInfo *)pResp);

    /* Allocate memory to the pointer */
    lResponse->pArrRedirPartyNum = &RedirPartyNumList;

    arrRedirPartyNum *lResp = lResponse->pArrRedirPartyNum;
    return UnpackTlvCommonArrRedPartyNum( pTlvData, lResp );
}

/*
 * This function unpacks the Alerting Pattern TLV parameters from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvAlertPattern(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceSetAllCallStatusCbkInfo *lResponse =
        ((voiceSetAllCallStatusCbkInfo *)pResp);

    /* Allocate memory to the pointer */
    lResponse->pArrAlertingPattern = &AlertingPatternList;

    arrAlertingPattern *lResp = lResponse->pArrAlertingPattern;
    return UnpackTlvCommonArrAlertPattern( pTlvData, lResp );
}

/*
 * This function unpacks the Event Report Indication message to a
 * user-provided response structure.
 *
 * \param   pMdmResp - Pointer to packed response from the modem.
 *
 * \param   pApiResp - Pointer to storage to unpack into.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UpkQmiCbkVoiceAllCallStatusInd(
    BYTE                         *pMdmResp,
    voiceSetAllCallStatusCbkInfo *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_ARR_CALL_INFORM,      &UnpackCbkTlvCallInformation },
        { eTLV_ARR_REM_PARTY_NUM,    &UnpackCbkTlvRemPartyNum  },
        { eTLV_ARR_REM_PARTY_NAME,   &UnpackCbkTlvRemPartyName },
        { eTLV_ARR_AlERT_TYPE,       &UnpackCbkTlvAlertType },
        { eTLV_ARR_SERVICE_OPT,      &UnpackCbkTlvServiceOpt },
        { eTLV_ARR_CALL_END_REASON,  &UnpackCbkTlvVoiceCallEndReason },
        { eTLV_ARR_ALPHA_IDEN,       &UnpackCbkTlvAlphaIden },
        { eTLV_ARR_CONN_PARTY_NUM,   &UnpackCbkTlvConnPartyNum },
        { eTLV_ARR_DIAGNOS_INFORM,   &UnpackCbkTlvDiagnosInform },
        { eTLV_ARR_CALLED_PARTY_NUM, &UnpackCbkTlvCalledPartyNum },
        { eTLV_ARR_REDIR_PARTY_NUM,  &UnpackCbkTlvRedirPartyNum },
        { eTLV_ARR_ALERT_PATTERN,    &UnpackCbkTlvAlertPattern },
        { eTLV_TYPE_INVALID,         NULL } /* Important. Sentinel.
                                             * Signifies last item in map.
                                             */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_VOICE_ALL_CALL_STATUS_IND );
    return eRCode;
}
