/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceGetCallBarring.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          eQMI_VOICE_GET_CALL_BARRING message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaVoiceSLQSVoiceGetCallBarring.h"
#include "qaGobiApiVoiceCommon.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the Call Barring Reason to the
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
enum eQCWWANError BuildTlvCallBarReason( BYTE *pBuf, BYTE *pParam )
{
	voiceGetCallBarringReq *pReq =(voiceGetCallBarringReq *)pParam;

    if ( NULL == pReq )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Reason */
    return PutByte( pBuf, pReq->reason );
}

/*
 * This function packs the Call Barring Service Class values to the
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
enum eQCWWANError BuildTlvCBarServClass( BYTE *pBuf, BYTE *pParam )
{
	voiceGetCallBarringReq *pReq =(voiceGetCallBarringReq *)pParam;

    if ( NULL == pReq->pSvcClass )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Service Class */
    return PutByte( pBuf, *pReq->pSvcClass );
}

/*
 * This function packs the SLQS Get Call Barring request parameters to
 * the QMI message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaVoiceSLQSVoiceGetCallBarring.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiVoiceSlqsGetCallBarring(
    WORD                   *pMlength,
    BYTE                   *pParamField,
    voiceGetCallBarringReq *pVoiceGetCallBarringReq )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_CBAR_REASON,      &BuildTlvCallBarReason },
        { eTLV_CBAR_S_CLASS_REQ, &BuildTlvCBarServClass },
        { eTLV_TYPE_INVALID,     NULL } /* Important. Sentinel.
                                         * Signifies last item in map
                                         */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pVoiceGetCallBarringReq,
                      map,
                      eQMI_VOICE_GET_CALL_BARRING,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the Call Barring Service Class from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCBarServiceClass(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceGetCallBarringResp *lResp = ((struct QmiVoiceSLQSGetCallBarringResp *)
                                               pResp)->pVoiceGetCallBarringResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pSvcClass )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pSvcClass );
}

/*
 * This function unpacks the Call Barring Failure Cause from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCBarFailureCause(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceGetCallBarringResp *lResp = ((struct QmiVoiceSLQSGetCallBarringResp *)
                                               pResp)->pVoiceGetCallBarringResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pFailCause )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetWord( pTlvData, lResp->pFailCause );
}

/*
 * This function unpacks the Call Barring Alpha Identifier Info from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCBarAlphaId(
    BYTE *pTlvData,
    BYTE *pResp )
{
    alphaIDInfo *lResp = ((struct QmiVoiceSLQSGetCallBarringResp *)
                                   pResp)->pVoiceGetCallBarringResp->pAlphaIDInfo;

    return UnpackTlvCommonAlphaIdentifier( pTlvData, lResp );
}

/*
 * This function unpacks the Call Barring Result Type from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCBarResultType(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceGetCallBarringResp *lResp = ((struct QmiVoiceSLQSGetCallBarringResp *)
                                                 pResp)->pVoiceGetCallBarringResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pCCResType )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pCCResType );
}

/*
 * This function unpacks the Call Barring Call Identifier from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCBarCallId(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceGetCallBarringResp *lResp = ((struct QmiVoiceSLQSGetCallBarringResp *)
                                                 pResp)->pVoiceGetCallBarringResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pCallID  )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pCallID );
}

/*
 * This function unpacks the Call Barring Call Control Supplementary Service
 * Type from the QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCBarccSupsType(
    BYTE *pTlvData,
    BYTE *pResp )
{
    ccSUPSType *lResp = ((struct QmiVoiceSLQSGetCallBarringResp *)
                                pResp)->pVoiceGetCallBarringResp->pCCSUPSType;

    return UnpackTlvCommonccSupsType( pTlvData, lResp );
}

/*
 * This function unpacks the SLQS Get Call Barring response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiVoiceSlqsGetCallBarring(
    BYTE                                  *pMdmResp,
    struct QmiVoiceSLQSGetCallBarringResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,        &qmUnpackTlvResultCode },
        { eTLV_CBAR_S_CLASS_RESP,  &UnpackTlvCBarServiceClass },
        { eTLV_CBAR_FAILURE_CAUSE, &UnpackTlvCBarFailureCause },
        { eTLV_CBAR_ALPHA_ID,      &UnpackTlvCBarAlphaId },
        { eTLV_CBAR_RESULT_TYPE,   &UnpackTlvCBarResultType },
        { eTLV_CBAR_CALL_ID,       &UnpackTlvCBarCallId },
        { eTLV_CBAR_CC_SUP_S_TYPE, &UnpackTlvCBarccSupsType },
        { eTLV_TYPE_INVALID,       NULL } /* Important. Sentinel.
                                           * Signifies last item in map.
                                           */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_VOICE_GET_CALL_BARRING );
    return eRCode;
}
