/*
 * \ingroup uim
 *
 * \file    qaUimSLQSUimUnblockPin.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_UIM_UNBLOCK_PIN message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaUimSLQSUimUnblockPin.h"
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
enum eQCWWANError BuildTlvUPSessionInfo( BYTE *pBuf, BYTE *pParam )
{
    UIMSessionInformation *pReq = &(( UIMUnblockPinReq *)pParam)->sessionInfo;

    if ( NULL == pReq )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Insert Session Information */
    return PackTlvCommonSessionInfo( pBuf, pReq );
}

/*
 * This function packs the Unblock PIN parameter to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvUnblockPin( BYTE *pBuf, BYTE *pParam )
{
    unblockUIMPIN *pReq = &(( UIMUnblockPinReq *)pParam)->unblockPIN;
    enum eQCWWANError  eRCode;
    WORD               lCount;

    if ( NULL == pReq )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Insert Unblock PIN Info */
    eRCode = PutByte( pBuf, pReq->pinID );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }


    eRCode = PutByte( pBuf, pReq->pukLen );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < pReq->pukLen ; lCount++ )
    {
        eRCode = PutByte( pBuf, pReq->pukVal[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    eRCode = PutByte( pBuf, pReq->newPINLen );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < pReq->newPINLen ; lCount++ )
    {
        eRCode = PutByte( pBuf, pReq->newPINVal[lCount] );
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
enum eQCWWANError BuildTlvUPKeyRefID( BYTE *pBuf, BYTE *pParam )
{
    UIMUnblockPinReq *pReq = ( UIMUnblockPinReq *)pParam;

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
enum eQCWWANError BuildTlvUPTokenInd( BYTE *pBuf, BYTE *pParam )
{
    UIMUnblockPinReq *pReq = ( UIMUnblockPinReq *)pParam;

    if ( NULL == pReq->pIndicationToken )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Indication Token */
    return PutLong( pBuf, *pReq->pIndicationToken );
}

/*
 * This function packs the SLQS UnblockPin request parameters to the QMI message SDU.
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaUimSLQSUimUnblockPin.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiUimSlqsUnblockPin(
    WORD             *pMlength,
    BYTE             *pParamField,
    UIMUnblockPinReq *pUIMUnblockPinReq )
{
    /* There is no mandatory TLV and hence the map is empty */
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_UP_SESSION_INFO, &BuildTlvUPSessionInfo },
        { eTLV_UNBLOCK_PIN,     &BuildTlvUnblockPin },
        { eTLV_UP_KEY_REF_ID,   &BuildTlvUPKeyRefID },
        { eTLV_UP_TOKEN_IND,    &BuildTlvUPTokenInd },
        { eTLV_TYPE_INVALID,    NULL } /* Important. Sentinel.
                                        * Signifies last item in map
                                        */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pUIMUnblockPinReq,
                      map,
                      eQMI_UIM_UNBLOCK_PIN,
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
enum eQCWWANError UnpackTlvUPRetriesRemaining(
    BYTE *pTlvData,
    BYTE *pResp )
{
    remainingRetries *lResp = ((struct QmiUimSLQSUnblockPinResp *)
                           pResp)->pUIMUnblockPinResp->pRemainingRetries;

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
enum eQCWWANError UnpackTlvUPEncryptedPIN1(
    BYTE *pTlvData,
    BYTE *pResp )
{
    encryptedPIN1 *lResp = ((struct QmiUimSLQSUnblockPinResp *)
                                   pResp)->pUIMUnblockPinResp->pEncryptedPIN1;

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
enum eQCWWANError UnpackTlvUPIndToken(
    BYTE *pTlvData,
    BYTE *pResp )
{
    UIMPinResp *lResp = ((struct QmiUimSLQSUnblockPinResp *)
                                        pResp)->pUIMUnblockPinResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pIndicationToken )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Copy the Indication Token into the List */
    return GetLong( pTlvData, lResp->pIndicationToken );
}

/*
 * This function unpacks the SLQS UnblockPin response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiUimSlqsUnblockPin(
    BYTE                           *pMdmResp,
    struct QmiUimSLQSUnblockPinResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,       &qmUnpackTlvResultCode },
        { eTLV_UP_RETRIES_REM,    &UnpackTlvUPRetriesRemaining },
        { eTLV_UP_ENCRYPTED_PIN1, &UnpackTlvUPEncryptedPIN1 },
        { eTLV_UP_RESP_IND_RESP,  &UnpackTlvUPIndToken },
        { eTLV_TYPE_INVALID,      NULL } /* Important. Sentinel.
                                          * Signifies last item in map.
                                          */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_UIM_UNBLOCK_PIN );
    return eRCode;
}
