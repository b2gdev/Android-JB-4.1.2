/*
 * \ingroup sms
 *
 * \file qaSmsRawRead.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        QMI_WMS_RAW_READ message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */
#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaSmsRawRead.h"


/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the GetSMS Memory Storage Identification field
 * to the QMI message SDU
 *
 * \param pBuf   - Pointer to storage into which the packed
 *                 data will be placed by this function.
 *
 * \param pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError BuildTlvMemoryStorageID( BYTE *pBuf, BYTE *pParam )
{
    struct QmiSmsRawReadReq *pReq =
                     (struct QmiSmsRawReadReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Add TLV data */
    eRCode = PutByte( pBuf, pReq->storageType );
    if ( eRCode == eQCWWAN_ERR_NONE )
        eRCode = PutLong( pBuf, pReq->messageIndex );

    return eRCode;
}

/*
 * This function packs the GetSMS Message Mode field
 * to the QMI message SDU
 *
 * \param pBuf   - Pointer to storage into which the packed
 *                 data will be placed by this function.
 *
 * \param pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError BuildTlvMsgMode( BYTE *pBuf, BYTE *pParam )
{
    struct QmiSmsRawReadReq *pReq =
                     (struct QmiSmsRawReadReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pReq->pMode )
        return eRCode;

    /* Add TLV data */
    eRCode = PutByte( pBuf, *(pReq->pMode) );

    return eRCode;
}

/*
 * This function packs the RawReadSMS parameters to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                                    data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 * \sa     qaGobiApiSms.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiSmsRawRead(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG storageType,
    ULONG messageIndex,
    BYTE  *pMode )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_MEMORY_STORAGE_ID, &BuildTlvMemoryStorageID },
        { eTLV_MSG_MODE,          &BuildTlvMsgMode },
        { eTLV_TYPE_INVALID,      NULL } /* Important. Sentinel.
                                          * Signifies last item in map.
                                          */
    };

    struct QmiSmsRawReadReq req;
    enum   eQCWWANError     eRCode;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiSmsRawReadReq));
    req.storageType  = storageType ;
    req.messageIndex = messageIndex ;
    req.pMode        = pMode;

    eRCode = qmbuild ( pParamField,
                       (BYTE *)&req,
                       map,
                       eQMI_WMS_RAW_READ,
                       pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the GetSMS raw message data from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError UnpackTlvRawMessageData( BYTE *pTlvData, BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    struct QmiSmsRawReadResp *lResp =
                 (struct QmiSmsRawReadResp *)pResp;

    WORD lmessageSize = 0;

    BYTE lByte = 0;
    if ( !lResp->pMessageTag || !lResp->pMessageFormat ||
         !lResp->pMessage    || !lResp->pMessageSize)
        return eRCode;

    eRCode = GetByte( pTlvData, &lByte );

    if ( eRCode == eQCWWAN_ERR_NONE )
    {
        *(lResp->pMessageTag) = lByte;
        eRCode = GetByte( pTlvData, &lByte );
        if ( eRCode == eQCWWAN_ERR_NONE )
        {
            *(lResp->pMessageFormat) = lByte;
            eRCode = GetWord( pTlvData, &lmessageSize );
            if ( eRCode == eQCWWAN_ERR_NONE )
            {
                if ( lmessageSize > *lResp->pMessageSize )
                {
                    eRCode = eQCWWAN_ERR_BUFFER_SZ;
                }
                else
                {
                    *(lResp->pMessageSize) = lmessageSize;
                    /* Fill the ERI File content till lmessageSize */
                    while ( lmessageSize-- )
                    {
                         eRCode = GetByte( pTlvData, (lResp->pMessage)++ );
                         if ( eRCode != eQCWWAN_ERR_NONE )
                            return eRCode;
                    }
                }
            }
        }
    }
    return eRCode;
}

/*
 * This function unpacks the RAW_READ response
 * message to a user-provided response structure.
 *
 * \param  pMdmResp  [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp  [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError UpkQmiSmsRawRead(
    BYTE                     *pMdmResp,
    struct QmiSmsRawReadResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,      &qmUnpackTlvResultCode },
        { eTLV_RAW_MESSAGE_DATA, &UnpackTlvRawMessageData },
        { eTLV_TYPE_INVALID,     NULL } /* Important. Sentinel.
                                         * Signifies last item in map.
                                         */
    };
    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_WMS_RAW_READ );
    return eRCode;
}

