/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceGetCLIP.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_VOICE_GET_CLIP message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaVoiceSLQSVoiceGetCLIP.h"
#include "qaGobiApiVoiceCommon.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the SLQS Get CLIP Info request parameters to the
 * QMI message SDU.
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaVoiceSLQSVoiceGetCLIP.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiVoiceSlqsGetCLIP(
    WORD *pMlength,
    BYTE *pParamField )
{
    /* There is no mandatory TLV and hence the map is empty */
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map
                                     */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      NULL,
                      map,
                      eQMI_VOICE_GET_CLIP,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the CLIP Response from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCLIPResponse(
    BYTE *pTlvData,
    BYTE *pResp )
{
    CLIPResp *lResp = ((struct QmiVoiceSLQSGetCLIPResp *)
                                     pResp)->pVoiceGetCLIPResp->pCLIPResp;
    enum eQCWWANError eRCode;

    /* Check For Invalid Parameter */
    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Copy the CLIP Response values into the list */
    eRCode = GetByte( pTlvData, &lResp->ActiveStatus );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->ProvisionStatus );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return eRCode;
}

/*
 * This function unpacks the CLIP Failure Cause from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCLIPFailureCause(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceGetCLIPResp *lResp = ((struct QmiVoiceSLQSGetCLIPResp *)
                                     pResp)->pVoiceGetCLIPResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pFailCause )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetWord( pTlvData, lResp->pFailCause );
}

/*
 * This function unpacks the CLIP Alpha Identifier Info from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCLIPAlphaId(
    BYTE *pTlvData,
    BYTE *pResp )
{
    alphaIDInfo *lResp = ((struct QmiVoiceSLQSGetCLIPResp *)
                                     pResp)->pVoiceGetCLIPResp->pAlphaIDInfo;

    return UnpackTlvCommonAlphaIdentifier( pTlvData, lResp );
}

/*
 * This function unpacks the CLIP Result Type from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCLIPResultType(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceGetCLIPResp *lResp = ((struct QmiVoiceSLQSGetCLIPResp *)
                                     pResp)->pVoiceGetCLIPResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pCCResType )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pCCResType );
}

/*
 * This function unpacks the CLIP Call Identifier from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCLIPCallId(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceGetCLIPResp *lResp = ((struct QmiVoiceSLQSGetCLIPResp *)
                                     pResp)->pVoiceGetCLIPResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pCallID )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pCallID );
}

/*
 * This function unpacks the CLIP Call Control Supplementary Service Type
 * from the QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCLIPccSupsType(
    BYTE *pTlvData,
    BYTE *pResp )
{
    ccSUPSType *lResp = ((struct QmiVoiceSLQSGetCLIPResp *)
                                pResp)->pVoiceGetCLIPResp->pCCSUPSType;

    return UnpackTlvCommonccSupsType( pTlvData, lResp );
}

/*
 * This function unpacks the SLQS Get CLIP response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiVoiceSlqsGetCLIP(
    BYTE                           *pMdmResp,
    struct QmiVoiceSLQSGetCLIPResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,        &qmUnpackTlvResultCode },
        { eTLV_CLIP_RESPONSE,      &UnpackTlvCLIPResponse },
        { eTLV_CLIP_FAILURE_CAUSE, &UnpackTlvCLIPFailureCause },
        { eTLV_CLIP_ALPHA_ID,      &UnpackTlvCLIPAlphaId },
        { eTLV_CLIP_RESULT_TYPE,   &UnpackTlvCLIPResultType },
        { eTLV_CLIP_CALL_ID,       &UnpackTlvCLIPCallId },
        { eTLV_CLIP_CC_SUP_S_TYPE, &UnpackTlvCLIPccSupsType },
        { eTLV_TYPE_INVALID,       NULL } /* Important. Sentinel.
                                           * Signifies last item in map.
                                           */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_VOICE_GET_CLIP );
    return eRCode;
}
