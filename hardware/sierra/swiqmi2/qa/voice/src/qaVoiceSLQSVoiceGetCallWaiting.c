/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceGetCallWaiting.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          eQMI_VOICE_GET_CALL_WAITING message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaVoiceSLQSVoiceGetCallWaiting.h"
#include "qaGobiApiVoiceCommon.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the Call Waiting Service Class values to the
 * QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvCWaitServClass( BYTE *pBuf, BYTE *pParam )
{
    struct QmiVoiceSLQSGetCallWaitingReq *pReq =
                     (struct QmiVoiceSLQSGetCallWaitingReq *)pParam;

    if ( NULL == pReq->pSvcClass )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Service Class */
    return PutByte( pBuf, *pReq->pSvcClass );
}

/*
 * This function packs the SLQS Get Call Waiting request parameters to
 * the QMI message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaVoiceSLQSVoiceGetCallWaiting.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiVoiceSlqsGetCallWaiting(
    WORD *pMlength,
    BYTE *pParamField,
    BYTE *pSvcClass )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_CWAIT_S_CLASS_REQ, &BuildTlvCWaitServClass },
        { eTLV_TYPE_INVALID,      NULL } /* Important. Sentinel.
                                          * Signifies last item in map
                                          */
    };

    struct QmiVoiceSLQSGetCallWaitingReq req;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiVoiceSLQSGetCallWaitingReq));
    req.pSvcClass = pSvcClass;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_VOICE_GET_CALL_WAITING,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the Call Waiting Service Class from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCWaitServiceClass(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceGetCallWaitInfo *lResp = ((struct QmiVoiceSLQSGetCallWaitingResp *)
                                               pResp)->pVoiceGetCallWaitInfo;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pSvcClass )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pSvcClass );
}

/*
 * This function unpacks the Call Waiting Failure Cause from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCWaitFailureCause(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceGetCallWaitInfo *lResp = ((struct QmiVoiceSLQSGetCallWaitingResp *)
                                               pResp)->pVoiceGetCallWaitInfo;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pFailCause )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetWord( pTlvData, lResp->pFailCause );
}

/*
 * This function unpacks the Call Waiting Alpha Identifier Info from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCWaitAlphaId(
    BYTE *pTlvData,
    BYTE *pResp )
{
    alphaIDInfo *lResp = ((struct QmiVoiceSLQSGetCallWaitingResp *)
                                   pResp)->pVoiceGetCallWaitInfo->pAlphaIDInfo;

    return UnpackTlvCommonAlphaIdentifier( pTlvData, lResp );
}

/*
 * This function unpacks the Call Waiting Result Type from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCWaitResultType(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceGetCallWaitInfo *lResp = ((struct QmiVoiceSLQSGetCallWaitingResp *)
                                                 pResp)->pVoiceGetCallWaitInfo;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pCCResType )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pCCResType );
}

/*
 * This function unpacks the Call Waiting Call Identifier from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCWaitCallId(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceGetCallWaitInfo *lResp = ((struct QmiVoiceSLQSGetCallWaitingResp *)
                                                 pResp)->pVoiceGetCallWaitInfo;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pCallID )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pCallID );
}

/*
 * This function unpacks the Call Waiting Call Control Supplementary Service
 * Type from the QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCWaitccSupsType(
    BYTE *pTlvData,
    BYTE *pResp )
{
    ccSUPSType *lResp = ((struct QmiVoiceSLQSGetCallWaitingResp *)
                                pResp)->pVoiceGetCallWaitInfo->pCCSUPSType;

    return UnpackTlvCommonccSupsType( pTlvData, lResp );
}

/*
 * This function unpacks the SLQS Get Call Waiting response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiVoiceSlqsGetCallWaiting(
    BYTE                                  *pMdmResp,
    struct QmiVoiceSLQSGetCallWaitingResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,         &qmUnpackTlvResultCode },
        { eTLV_CWAIT_S_CLASS_RESP,  &UnpackTlvCWaitServiceClass },
        { eTLV_CWAIT_FAILURE_CAUSE, &UnpackTlvCWaitFailureCause },
        { eTLV_CWAIT_ALPHA_ID,      &UnpackTlvCWaitAlphaId },
        { eTLV_CWAIT_RESULT_TYPE,   &UnpackTlvCWaitResultType },
        { eTLV_CWAIT_CALL_ID,       &UnpackTlvCWaitCallId },
        { eTLV_CWAIT_CC_SUP_S_TYPE, &UnpackTlvCWaitccSupsType },
        { eTLV_TYPE_INVALID,       NULL } /* Important. Sentinel.
                                           * Signifies last item in map.
                                           */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_VOICE_GET_CALL_WAITING );
    return eRCode;
}
