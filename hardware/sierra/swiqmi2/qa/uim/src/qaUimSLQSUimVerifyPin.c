/*
 * \ingroup uim
 *
 * \file    qaUimSLQSUimVerifyPin.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_UIM_VERIFY_PIN message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaUimSLQSUimVerifyPin.h"
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
enum eQCWWANError BuildTlvVPSessionInfo( BYTE *pBuf, BYTE *pParam )
{
    UIMSessionInformation *pReq = &(( UIMVerifyPinReq *)pParam)->sessionInfo;

    if ( NULL == pReq )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Insert Session Information */
    return PackTlvCommonSessionInfo( pBuf, pReq );
}

/*
 * This function packs the Verify PIN parameter to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvVerifyPin( BYTE *pBuf, BYTE *pParam )
{
    verifyUIMPIN *pReq = &(( UIMVerifyPinReq *)pParam)->verifyPIN;
    enum eQCWWANError  eRCode;
    WORD               lCount;

    if ( NULL == pReq )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Insert Verify PIN Info */
    eRCode = PutByte( pBuf, pReq->pinID );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = PutByte( pBuf, pReq->pinLen );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < pReq->pinLen ; lCount++ )
    {
        eRCode = PutByte( pBuf, pReq->pinVal[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function packs the Encrypted PIN1 parameter to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvEncryptedPin1( BYTE *pBuf, BYTE *pParam )
{
    encryptedPIN1 *pReq = (( UIMVerifyPinReq *)pParam)->pEncryptedPIN1;
    enum eQCWWANError  eRCode;
    WORD               lCount;

    if ( NULL == pReq )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Encrypted PIN1 Info */
    eRCode = PutByte( pBuf, pReq->pin1Len );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < pReq->pin1Len ; lCount++ )
    {
        eRCode = PutByte( pBuf, pReq->pin1Val[lCount] );
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
enum eQCWWANError BuildTlvVPKeyRefID( BYTE *pBuf, BYTE *pParam )
{
    UIMVerifyPinReq *pReq = ( UIMVerifyPinReq *)pParam;

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
enum eQCWWANError BuildTlvVPTokenInd( BYTE *pBuf, BYTE *pParam )
{
    UIMVerifyPinReq *pReq = ( UIMVerifyPinReq *)pParam;

    if ( NULL == pReq->pIndicationToken )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Indication Token */
    return PutLong( pBuf, *pReq->pIndicationToken );
}

/*
 * This function packs the SLQS VerifyPin request parameters to the QMI message SDU.
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaUimSLQSUimVerifyPin.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiUimSlqsVerifyPin(
    WORD            *pMlength,
    BYTE            *pParamField,
    UIMVerifyPinReq *pUIMVerifyPinReq )
{
    /* There is no mandatory TLV and hence the map is empty */
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_VP_SESSION_INFO,   &BuildTlvVPSessionInfo },
        { eTLV_VERIFY_PIN,        &BuildTlvVerifyPin },
        { eTLV_VP_ENCRYPTED_PIN1, &BuildTlvEncryptedPin1 },
        { eTLV_VP_KEY_REF_ID,     &BuildTlvVPKeyRefID },
        { eTLV_VP_TOKEN_IND,      &BuildTlvVPTokenInd },
        { eTLV_TYPE_INVALID,      NULL } /* Important. Sentinel.
                                          * Signifies last item in map
                                          */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pUIMVerifyPinReq,
                      map,
                      eQMI_UIM_VERIFY_PIN,
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
enum eQCWWANError UnpackTlvVPRetriesRemaining(
    BYTE *pTlvData,
    BYTE *pResp )
{
    remainingRetries *lResp = ((struct QmiUimSLQSVerifyPinResp *)
                           pResp)->pUIMVerifyPinResp->pRemainingRetries;

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
enum eQCWWANError UnpackTlvVPEncryptedPIN1(
    BYTE *pTlvData,
    BYTE *pResp )
{
    encryptedPIN1 *lResp = ((struct QmiUimSLQSVerifyPinResp *)
                                   pResp)->pUIMVerifyPinResp->pEncryptedPIN1;

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
enum eQCWWANError UnpackTlvVPIndToken(
    BYTE *pTlvData,
    BYTE *pResp )
{
    UIMPinResp *lResp = ((struct QmiUimSLQSVerifyPinResp *)
                                        pResp)->pUIMVerifyPinResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pIndicationToken )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Copy the Indication Token into the List */
    return GetLong( pTlvData, lResp->pIndicationToken );
}

/*
 * This function unpacks the SLQS VerifyPin response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiUimSlqsVerifyPin(
    BYTE                           *pMdmResp,
    struct QmiUimSLQSVerifyPinResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,            &qmUnpackTlvResultCode },
        { eTLV_VP_RETRIES_REM,         &UnpackTlvVPRetriesRemaining },
        { eTLV_VP_ENCRYPTED_PIN1_RESP, &UnpackTlvVPEncryptedPIN1 },
        { eTLV_VP_RESP_IND_RESP,       &UnpackTlvVPIndToken },
        { eTLV_TYPE_INVALID,           NULL } /* Important. Sentinel.
                                               * Signifies last item in map.
                                               */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_UIM_VERIFY_PIN );
    return eRCode;
}
