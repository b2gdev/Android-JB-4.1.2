/*
 * \ingroup uim
 *
 * \file    qaGobiApiUimCommon.c
 *
 * \brief   UIM Service Common Pack and Unpack function definitions.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaGobiApiUimCommon.h"

/*
 * This function packs the Session Information parameter to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pReq   [IN]  - Pointer to structure of UIMSessionInformation.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError PackTlvCommonSessionInfo(
    BYTE                  *pBuf,
    UIMSessionInformation *pReq )
{
    enum eQCWWANError eRCode;
    WORD              lCount;

    eRCode = PutByte( pBuf, pReq->sessionType );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = PutByte( pBuf, pReq->aidLength );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < pReq->aidLength ; lCount++ )
    {
        eRCode = PutByte( pBuf, pReq->aid[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * A common function to API and callback which unpacks the Retries Remaining
 * from the QMI message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure of alphaIDInfo.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCommonRetriesRem(
    BYTE             *pTlvData,
    remainingRetries *pResp )
{
    enum eQCWWANError eRCode;

    eRCode = GetByte( pTlvData, &pResp->verifyLeft );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetByte( pTlvData, &pResp->unblockLeft );
}

/*
 * A common function to API and callback which unpacks the Encrypted PIN1
 * from the QMI message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure of alphaIDInfo.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCommonEncryptedPin1(
    BYTE          *pTlvData,
    encryptedPIN1 *pResp )
{
    enum eQCWWANError eRCode;
    BYTE              lcount;

    eRCode = GetByte( pTlvData, &pResp->pin1Len );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lcount=0; lcount < pResp->pin1Len ; lcount++ )
    {
        eRCode = GetByte( pTlvData, &pResp->pin1Val[lcount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}
