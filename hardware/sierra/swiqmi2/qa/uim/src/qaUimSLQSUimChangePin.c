/*
 * \ingroup uim
 *
 * \file    qaUimSLQSUimChangePin.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_UIM_CHANGE_PIN message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaUimSLQSUimChangePin.h"
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
enum eQCWWANError BuildTlvCPSessionInfo( BYTE *pBuf, BYTE *pParam )
{
    UIMSessionInformation *pReq = &(( UIMChangePinReq *)pParam)->sessionInfo;

    if ( NULL == pReq )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Insert Session Information */
    return PackTlvCommonSessionInfo( pBuf, pReq );
}

/*
 * This function packs the Change PIN parameter to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvChangePin( BYTE *pBuf, BYTE *pParam )
{
    changeUIMPIN *pReq = &(( UIMChangePinReq *)pParam)->changePIN;
    enum eQCWWANError  eRCode;
    WORD               lCount;

    if ( NULL == pReq )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Insert Change PIN Info */
    eRCode = PutByte( pBuf, pReq->pinID );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }


    eRCode = PutByte( pBuf, pReq->oldPINLen );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < pReq->oldPINLen ; lCount++ )
    {
        eRCode = PutByte( pBuf, pReq->oldPINVal[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
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
enum eQCWWANError BuildTlvCPKeyRefID( BYTE *pBuf, BYTE *pParam )
{
    UIMChangePinReq *pReq = ( UIMChangePinReq *)pParam;

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
enum eQCWWANError BuildTlvCPTokenInd( BYTE *pBuf, BYTE *pParam )
{
    UIMChangePinReq *pReq = ( UIMChangePinReq *)pParam;

    if ( NULL == pReq->pIndicationToken )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Indication Token */
    return PutLong( pBuf, *pReq->pIndicationToken );
}

/*
 * This function packs the SLQS ChangePin request parameters to the QMI message SDU.
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaUimSLQSUimChangePin.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiUimSlqsChangePin(
    WORD            *pMlength,
    BYTE            *pParamField,
    UIMChangePinReq *pUIMChangePinReq )
{
    /* There is no mandatory TLV and hence the map is empty */
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_CP_SESSION_INFO, &BuildTlvCPSessionInfo },
        { eTLV_CHANGE_PIN,      &BuildTlvChangePin },
        { eTLV_CP_KEY_REF_ID,   &BuildTlvCPKeyRefID },
        { eTLV_CP_TOKEN_IND,    &BuildTlvCPTokenInd },
        { eTLV_TYPE_INVALID,    NULL } /* Important. Sentinel.
                                        * Signifies last item in map
                                        */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pUIMChangePinReq,
                      map,
                      eQMI_UIM_CHANGE_PIN,
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
enum eQCWWANError UnpackTlvCPRetriesRemaining(
    BYTE *pTlvData,
    BYTE *pResp )
{
    remainingRetries *lResp = ((struct QmiUimSLQSChangePinResp *)
                           pResp)->pUIMChangePinResp->pRemainingRetries;

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
enum eQCWWANError UnpackTlvCPEncryptedPIN1(
    BYTE *pTlvData,
    BYTE *pResp )
{
    encryptedPIN1 *lResp = ((struct QmiUimSLQSChangePinResp *)
                                   pResp)->pUIMChangePinResp->pEncryptedPIN1;

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
enum eQCWWANError UnpackTlvCPIndToken(
    BYTE *pTlvData,
    BYTE *pResp )
{
    UIMPinResp *lResp = ((struct QmiUimSLQSChangePinResp *)
                                        pResp)->pUIMChangePinResp;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pIndicationToken )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Copy the Indication Token into the List */
    return GetLong( pTlvData, lResp->pIndicationToken );
}

/*
 * This function unpacks the SLQS ChangePin response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiUimSlqsChangePin(
    BYTE                           *pMdmResp,
    struct QmiUimSLQSChangePinResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,       &qmUnpackTlvResultCode },
        { eTLV_CP_RETRIES_REM,    &UnpackTlvCPRetriesRemaining },
        { eTLV_CP_ENCRYPTED_PIN1, &UnpackTlvCPEncryptedPIN1 },
        { eTLV_CP_RESP_IND_RESP,  &UnpackTlvCPIndToken },
        { eTLV_TYPE_INVALID,      NULL } /* Important. Sentinel.
                                          * Signifies last item in map.
                                          */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_UIM_CHANGE_PIN );
    return eRCode;
}
