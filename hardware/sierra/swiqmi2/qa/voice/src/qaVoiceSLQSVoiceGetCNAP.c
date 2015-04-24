/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceGetCNAP.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_VOICE_GET_CNAP message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaVoiceSLQSVoiceGetCNAP.h"
#include "qaGobiApiVoiceCommon.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the SLQS Get CNAP request parameters to the
 * QMI message SDU.
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaVoiceSLQSGetCNAP.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiVoiceSlqsGetCNAP(
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
                      eQMI_VOICE_GET_CNAP,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the CNAP Response from the QMI response message
 * to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCNAPResponse(
    BYTE *pTlvData,
    BYTE *pResp )
{
    CNAPResp *lResp = ((struct QmiVoiceSLQSGetCNAPResp *)
                                  pResp)->pVoiceGetCNAPResp->pCNAPResp;

    enum eQCWWANError eRCode;

    /* Check For Invalid Parameter */
    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Copy the CNAP Response values into the list */
    eRCode = GetByte( pTlvData, &lResp->ActiveStatus );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetByte( pTlvData, &lResp->ProvisionStatus );
}

/*
 * This function unpacks the CNAP Failure Cause from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCNAPFailureCause(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceGetCNAPResp *lResp = ((struct QmiVoiceSLQSGetCNAPResp *)
                                          pResp)->pVoiceGetCNAPResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pFailCause )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetWord( pTlvData, lResp->pFailCause );
}

/*
 * This function unpacks the CNAP Alpha Identifier Info from the
 * QMI response message to a user provided response structure
 *
 * \param      pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param      pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCNAPAlphaIdentifier(
    BYTE *pTlvData,
    BYTE *pResp )
{
    alphaIDInfo *lResp = ((struct QmiVoiceSLQSGetCNAPResp *)
                                    pResp)->pVoiceGetCNAPResp->pAlphaIDInfo;

    return UnpackTlvCommonAlphaIdentifier( pTlvData, lResp );
}

/*
 * This function unpacks the CNAP Result Type from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCNAPControlResultType(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceGetCNAPResp *lResp = ((struct QmiVoiceSLQSGetCNAPResp *)
                                          pResp)->pVoiceGetCNAPResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pCCResType )
    {
       return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pCCResType );
}

/*
 * This function unpacks the CNAP Call Identifier
 * from the QMI response message to a user provided response structure
 *
 * \param     pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param      pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCNAPCallId(
    BYTE *pTlvData,
    BYTE *pResp )
{
    voiceGetCNAPResp *lResp = ((struct QmiVoiceSLQSGetCNAPResp *)
                                          pResp)->pVoiceGetCNAPResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pCallID )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pCallID );
}

/*
 * This function unpacks the CNAP Call Control Supplementary Service Type
 * from the QMI response message to a user provided response structure
 *
 * \param      pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param      pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCNAPCCSuppSerType(
    BYTE *pTlvData,
    BYTE *pResp )
{
    ccSUPSType *lResp = ((struct QmiVoiceSLQSGetCNAPResp *)
                                        pResp)->pVoiceGetCNAPResp->pCCSUPSType;

    return UnpackTlvCommonccSupsType( pTlvData, lResp );
}

/*
 * This function unpacks the SLQS Get CNAP response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiVoiceSlqsGetCNAP(
    BYTE                           *pMdmResp,
    struct QmiVoiceSLQSGetCNAPResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,              &qmUnpackTlvResultCode },
        { eTLV_CNAP_RESPONSE,            &UnpackTlvCNAPResponse},
        { eTLV_CNAP_FAILURE_CAUSE,       &UnpackTlvCNAPFailureCause},
        { eTLV_CNAP_ALPHA_IDENTIFIER,    &UnpackTlvCNAPAlphaIdentifier},
        { eTLV_CNAP_CONTROL_RESULT_TYPE, &UnpackTlvCNAPControlResultType},
        { eTLV_CNAP_CALL_ID,             &UnpackTlvCNAPCallId},
        { eTLV_CNAP_CC_SUPP_SER_TYPE,    &UnpackTlvCNAPCCSuppSerType},
        { eTLV_TYPE_INVALID,             NULL } /* Important. Sentinel.
                                                 * Signifies last item in map.
                                                 */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_VOICE_GET_CNAP );
    return eRCode;
}
