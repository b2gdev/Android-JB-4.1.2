/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceGetCOLP.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_VOICE_GET_COLP message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaVoiceSLQSVoiceGetCOLP.h"
#include "qaGobiApiVoiceCommon.h"


/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the SLQS Get COLP request parameters to the
 * QMI message SDU.
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaVoiceSLQSGetCOLP.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiVoiceSlqsGetCOLP(
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
                      eQMI_VOICE_GET_COLP,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the COLP Response from the QMI response message
 * to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCOLPResponse(
    BYTE *pTlvData,
    BYTE *pResp )
{
    COLPResp *lResp = ((struct QmiVoiceSLQSGetCOLPResp *)
                                  pResp)->pVoiceGetCOLPResp->pCOLPResp;

    enum eQCWWANError eRCode;

    /* Check For Invalid Parameter */
    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Copy the COLP Response values into the list */
    eRCode = GetByte( pTlvData, &lResp->ActiveStatus );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetByte( pTlvData, &lResp->ProvisionStatus );
}

/*
 * This function unpacks the COLP Failure Cause from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCOLPFailureCause(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceGetCOLPResp *lResp = ((struct QmiVoiceSLQSGetCOLPResp *)
                                pResp)->pVoiceGetCOLPResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pFailCause )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetWord( pTlvData, lResp->pFailCause );
}

/*
 * This function unpacks the COLP Alpha Identifier Info from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCOLPAlphaIdentifier(
    BYTE *pTlvData,
    BYTE *pResp )
{
    alphaIDInfo *lResp = ((struct QmiVoiceSLQSGetCOLPResp *)
                                    pResp)->pVoiceGetCOLPResp->pAlphaIDInfo;

    return UnpackTlvCommonAlphaIdentifier( pTlvData, lResp );
}

/*
 * This function unpacks the COLP Result Type from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvControlResultType(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceGetCOLPResp *lResp = ((struct QmiVoiceSLQSGetCOLPResp *)
                                          pResp)->pVoiceGetCOLPResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pCCResType )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pCCResType );
}

/*
 * This function unpacks the COLP Call Identifier
 * from the QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCOLPCallId(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceGetCOLPResp *lResp = ((struct QmiVoiceSLQSGetCOLPResp *)
                                          pResp)->pVoiceGetCOLPResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pCallID )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pCallID );
}

/*
 * This function unpacks the COLP Call Control Supplementary Service Type
 * from the QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCallControlSuppSerType(
    BYTE *pTlvData,
    BYTE *pResp )
{
    ccSUPSType *lResp = ((struct QmiVoiceSLQSGetCOLPResp *)
                                        pResp)->pVoiceGetCOLPResp->pCCSUPSType;

    return UnpackTlvCommonccSupsType( pTlvData, lResp );
}

/*
 * This function unpacks the SLQS Get COLP response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiVoiceSlqsGetCOLP(
    BYTE                           *pMdmResp,
    struct QmiVoiceSLQSGetCOLPResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,                &qmUnpackTlvResultCode },
        { eTLV_COLP_RESPONSE,              &UnpackTlvCOLPResponse},
        { eTLV_COLP_FAILURE_CAUSE,         &UnpackTlvCOLPFailureCause},
        { eTLV_COLP_ALPHA_IDENTIFIER,      &UnpackTlvCOLPAlphaIdentifier},
        { eTLV_CALL_CONTROL_RESULT_TYPE,   &UnpackTlvControlResultType},
        { eTLV_COLP_CALL_ID,               &UnpackTlvCOLPCallId},
        { eTLV_CALL_CONTROL_SUPP_SER_TYPE, &UnpackTlvCallControlSuppSerType},
        { eTLV_TYPE_INVALID,               NULL } /* Important. Sentinel.
                                                   * Signifies last item in map.
                                                   */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_VOICE_GET_COLP );
    return eRCode;
}
