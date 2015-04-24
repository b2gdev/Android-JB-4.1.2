/*
 * \ingroup sms
 *
 * \file qaSmsRawWrite.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        QMI_WMS_RAW_WRITE message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */
#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaSmsRawWrite.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the SaveSMS Raw Message Data field
 * to the QMI message SDU
 *
 * \param  pBuf   - Pointer to storage into which the packed
 *                 data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError BuildTlvSmsData( BYTE *pBuf, BYTE *pParam )
{
    struct QmiSmsRawWriteReq *pReq =
                     (struct QmiSmsRawWriteReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pReq->pMessage )
        return eRCode;

    /* Add TLV data */
    eRCode = PutByte( pBuf, pReq->storageType );
    if ( eRCode == eQCWWAN_ERR_NONE )
    {
        eRCode = PutByte( pBuf, pReq->messageFormat );
        if ( eRCode == eQCWWAN_ERR_NONE )
        {
            eRCode = PutWord( pBuf, pReq->messageSize );
            while ( (pReq->messageSize)-- && (eRCode == eQCWWAN_ERR_NONE) )
                eRCode = PutByte( pBuf, *(pReq->pMessage)++ );
        }
    }
    return eRCode;
}

/*
 * This function packs the SaveSMS parameters to the QMI message SDU
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
enum eQCWWANError PkQmiSmsRawWrite(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG storageType,
    ULONG messageFormat,
    ULONG messageSize,
    BYTE  *pMessage )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_RAW_MSG_DATA, &BuildTlvSmsData },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };

    struct QmiSmsRawWriteReq req;
    enum   eQCWWANError      eRCode;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiSmsRawWriteReq));
    req.storageType   = storageType ;
    req.messageFormat = messageFormat;
    req.messageSize   = messageSize;
    req.pMessage      = pMessage;

    eRCode = qmbuild ( pParamField,
                       (BYTE *)&req,
                       map,
                       eQMI_WMS_RAW_WRITE,
                       pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the SaeSMS message index from the
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
enum eQCWWANError UnpackTlvMessageIndex( BYTE *pTlvData, BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    struct QmiSmsRawWriteResp *lResp =
                 (struct QmiSmsRawWriteResp *)pResp;

    if ( !lResp->pMessageIndex )
        return eRCode;

    /* Validate TLV parameter length field */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                           eTLV_MSG_INDEX_LENGTH );
 
    if ( eRCode == eQCWWAN_ERR_NONE )
        eRCode = GetLong( pTlvData, (lResp->pMessageIndex) );

    return eRCode;
}

/*
 * This function unpacks the RAW_WRITE response
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
enum eQCWWANError UpkQmiSmsRawWrite(
    BYTE                      *pMdmResp,
    struct QmiSmsRawWriteResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_MSG_INDEX,    &UnpackTlvMessageIndex }, 
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };
    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_WMS_RAW_WRITE );
    return eRCode;
}

