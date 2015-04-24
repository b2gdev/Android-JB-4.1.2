/*
 * \ingroup uim
 *
 * \file    qaUimSLQSUimSetPinProtection.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_UIM_SET_PIN_PROTECTION message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaUimSLQSUimSetPinProtection.h"
#include "qaGobiApiUimCommon.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the Session Information parameter to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvSetPPSessionInfo( BYTE *pBuf, BYTE *pParam )
{
    UIMSessionInformation *pReq = &(( UIMSetPinProtectionReq *)pParam)->sessionInfo;

    if ( NULL == pReq )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Insert Session Information */
    return PackTlvCommonSessionInfo( pBuf, pReq );
}

/*
 * This function packs the Pin Protection parameter to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvSetPinProtection( BYTE *pBuf, BYTE *pParam )
{
    setPINProtection *pReq = &(( UIMSetPinProtectionReq *)pParam)->pinProtection;
    enum eQCWWANError  eRCode;
    WORD               lCount;

    if ( NULL == pReq )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Insert Pin Protection Info */
    eRCode = PutByte( pBuf, pReq->pinID );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = PutByte( pBuf, pReq->pinOperation );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = PutByte( pBuf, pReq->pinLength );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < pReq->pinLength ; lCount++ )
    {
        eRCode = PutByte( pBuf, pReq->pinValue[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function packs the Key Reference ID parameter to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvSetPPKeyRefID( BYTE *pBuf, BYTE *pParam )
{
    UIMSetPinProtectionReq *pReq = ( UIMSetPinProtectionReq *)pParam;

    if ( NULL == pReq->pKeyReferenceID )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Key Reference ID */
    return PutByte( pBuf, *pReq->pKeyReferenceID );
}

/*
 * This function packs the Indication Token parameter to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvSetPPRespInd( BYTE *pBuf, BYTE *pParam )
{
    UIMSetPinProtectionReq *pReq = ( UIMSetPinProtectionReq *)pParam;

    if ( NULL == pReq->pIndicationToken )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Indication Token */
    return PutLong( pBuf, *pReq->pIndicationToken );
}

/*
 * This function packs the SLQS SetPinProtection request parameters to the QMI message SDU.
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaUimSLQSUimSetPinProtection.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiUimSlqsSetPinProtection(
    WORD                   *pMlength,
    BYTE                   *pParamField,
    UIMSetPinProtectionReq *pUIMSetPinProtectionReq )
{
    /* There is no mandatory TLV and hence the map is empty */
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SET_PP_SESSION_INFO, &BuildTlvSetPPSessionInfo },
        { eTLV_SET_PIN_PROTECTION,  &BuildTlvSetPinProtection },
        { eTLV_SET_PP_KEY_REF_ID,   &BuildTlvSetPPKeyRefID },
        { eTLV_SET_PP_RESP_IND_REQ, &BuildTlvSetPPRespInd },
        { eTLV_TYPE_INVALID,        NULL } /* Important. Sentinel.
                                            * Signifies last item in map
                                            */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pUIMSetPinProtectionReq,
                      map,
                      eQMI_UIM_SET_PIN_PROTECTION,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the Retries Remaining Values from the QMI response
 * message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvSetPPRetriesRemaining(
    BYTE *pTlvData,
    BYTE *pResp )
{
    remainingRetries *lResp = ((struct QmiUimSLQSSetPinProtectionResp *)
                           pResp)->pUIMSetPinProtectionResp->pRemainingRetries;

    /* Check For Invalid Parameter */
    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Copy the Retries Remaining Values into the List */
    return UnpackTlvCommonRetriesRem( pTlvData, lResp );
}

/*
 * This function unpacks the Encrypted PIN1 Values from the QMI response
 * message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvSetPPEncryptedPIN1(
    BYTE *pTlvData,
    BYTE *pResp )
{
    encryptedPIN1 *lResp = ((struct QmiUimSLQSSetPinProtectionResp *)
                                   pResp)->pUIMSetPinProtectionResp->pEncryptedPIN1;

    /* Check For Invalid Parameter */
    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Copy the Encrypted PIN1 Values into the List */
    return UnpackTlvCommonEncryptedPin1( pTlvData, lResp );
}

/*
 * This function unpacks the Indication Token from the QMI response
 * message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvSetPPIndToken(
    BYTE *pTlvData,
    BYTE *pResp )
{
    UIMPinResp *lResp = ((struct QmiUimSLQSSetPinProtectionResp *)
                                        pResp)->pUIMSetPinProtectionResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pIndicationToken )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Copy the Indication Token into the List */
    return GetLong( pTlvData, lResp->pIndicationToken );
}

/*
 * This function unpacks the SLQS SetPinProtection response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiUimSlqsSetPinProtection(
    BYTE                                  *pMdmResp,
    struct QmiUimSLQSSetPinProtectionResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,           &qmUnpackTlvResultCode },
        { eTLV_SET_PP_RETRIES_REM,    &UnpackTlvSetPPRetriesRemaining },
        { eTLV_SET_PP_ENCRYPTED_PIN1, &UnpackTlvSetPPEncryptedPIN1 },
        { eTLV_SET_PP_RESP_IND_RESP,  &UnpackTlvSetPPIndToken },
        { eTLV_TYPE_INVALID,          NULL } /* Important. Sentinel.
                                              * Signifies last item in map.
                                              */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_UIM_SET_PIN_PROTECTION );
    return eRCode;
}
