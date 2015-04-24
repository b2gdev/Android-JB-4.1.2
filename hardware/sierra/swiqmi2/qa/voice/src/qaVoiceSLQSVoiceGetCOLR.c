/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceGetCOLR.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_VOICE_GET_COLR message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaVoiceSLQSVoiceGetCOLR.h"
#include "qaGobiApiVoiceCommon.h"


/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the SLQS Get COLR request parameters to the
 * QMI message SDU.
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaVoiceSLQSGetCOLR.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiVoiceSlqsGetCOLR(
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
                      eQMI_VOICE_GET_COLR,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the COLR Response from the QMI response message
 * to a user provided response structure
 *
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCOLRResponse(
    BYTE *pTlvData,
    BYTE *pResp )
{
    COLRResp *lResp = ((struct QmiVoiceSLQSGetCOLRResp *)
                                  pResp)->pVoiceGetCOLRResp->pCOLRResp;
    enum eQCWWANError eRCode;

    /* Check For Invalid Parameter */
    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Copy the COLR Response values into the list */
    eRCode = GetByte( pTlvData, &lResp->ActiveStatus );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetByte( pTlvData, &lResp->ProvisionStatus );
}

/*
 * This function unpacks the COLR Failure Cause from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCOLRFailureCause(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceGetCOLRResp *lResp = ((struct QmiVoiceSLQSGetCOLRResp *)
                                          pResp)->pVoiceGetCOLRResp;

     /* Check For Invalid Parameter */
    if ( NULL == lResp->pFailCause )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetWord( pTlvData, lResp->pFailCause );
}

/*
 * This function unpacks the COLR Alpha Identifier Info from the
 * QMI response message to a user provided response structure
 *
 * \param      pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCOLRAlphaIdentifier(
    BYTE *pTlvData,
    BYTE *pResp )
{
    alphaIDInfo *lResp = ((struct QmiVoiceSLQSGetCOLRResp *)
                                    pResp)->pVoiceGetCOLRResp->pAlphaIDInfo;

    return UnpackTlvCommonAlphaIdentifier( pTlvData, lResp );
}

/*
 * This function unpacks the COLR Result Type from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCOLRControlResultType(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceGetCOLRResp *lResp = ((struct QmiVoiceSLQSGetCOLRResp *)
                                          pResp)->pVoiceGetCOLRResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pCCResType )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pCCResType );
}

/*
 * This function unpacks the COLR Call Identifier
 * from the QMI response message to a user provided response structure
 *
 * \param      pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param      pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCOLRCallId(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceGetCOLRResp *lResp = ((struct QmiVoiceSLQSGetCOLRResp *)
                                          pResp)->pVoiceGetCOLRResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pCallID )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pCallID );
}

/*
 * This function unpacks the COLR Call Control Supplementary Service Type
 * from the QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCOLRCCSuppSerType(
    BYTE *pTlvData,
    BYTE *pResp )
{
    ccSUPSType *lResp = ((struct QmiVoiceSLQSGetCOLRResp *)
                                        pResp)->pVoiceGetCOLRResp->pCCSUPSType;

    return UnpackTlvCommonccSupsType( pTlvData, lResp );
}

/*
 * This function unpacks the SLQS Get COLR response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiVoiceSlqsGetCOLR(
    BYTE                           *pMdmResp,
    struct QmiVoiceSLQSGetCOLRResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,              &qmUnpackTlvResultCode },
        { eTLV_COLR_RESPONSE,            &UnpackTlvCOLRResponse},
        { eTLV_COLR_FAILURE_CAUSE,       &UnpackTlvCOLRFailureCause},
        { eTLV_COLR_ALPHA_IDENTIFIER,    &UnpackTlvCOLRAlphaIdentifier},
        { eTLV_COLR_CONTROL_RESULT_TYPE, &UnpackTlvCOLRControlResultType},
        { eTLV_COLR_CALL_ID,             &UnpackTlvCOLRCallId},
        { eTLV_COLR_CC_SUPP_SER_TYPE,    &UnpackTlvCOLRCCSuppSerType},
        { eTLV_TYPE_INVALID,             NULL } /* Important. Sentinel.
                                                 * Signifies last item in map.
                                                 */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_VOICE_GET_COLR );
    return eRCode;
}
