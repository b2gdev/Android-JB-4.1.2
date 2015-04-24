/*
 * \ingroup sms
 *
 * \file    qaSmsRawSend.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_WMS_RAW_SEND message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */
#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaSmsRawSend.h"


/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the SendSMS Message Data field
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
enum eQCWWANError BuildTlvMessageData( BYTE *pBuf, BYTE *pParam )
{
    struct QmiSmsRawSendReq *pReq =
                     (struct QmiSmsRawSendReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Add TLV data */
    eRCode = PutByte( pBuf, pReq->messageFormat );
    if ( eRCode == eQCWWAN_ERR_NONE )
        eRCode = PutWord( pBuf, pReq->messageSize );

    while ( pReq->messageSize && (eRCode == eQCWWAN_ERR_NONE) )
    {
        eRCode = PutByte( pBuf, *(pReq->pMessage)++ );
        pReq->messageSize--;
    }

    return eRCode;
}




/*
 * This function packs the Link timer field
 * to the QMI message SDU
 *
 * \param pBuf   - Pointer to link timer which the packed by this function.
 *
 * \param pParam - Pointer to structure containing link timer for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError BuildTlvLinkTimer( BYTE *pBuf, BYTE *pParam )
{
    struct QmiSmsRawSendReq *pReq =
                     (struct QmiSmsRawSendReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    /* Add TLV data */
    if(pReq->linktimer)
        eRCode = PutByte( pBuf, pReq->linktimer );
    return eRCode;
}

/*
 * This function packs the SendSMS parameters to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 * \sa     qaGobiApiSms.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiSmsRawSend(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG messageFormat,
    ULONG messageSize,
    BYTE linktimer,
    BYTE  *pMessage )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_MESSAGE_DATA, &BuildTlvMessageData },
        { eTLV_LINK_TIMER  , &BuildTlvLinkTimer },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };

    struct QmiSmsRawSendReq req;

    enum eQCWWANError eRCode;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiSmsRawSendReq));
    req.messageFormat = messageFormat;
    req.messageSize   = messageSize;
    req.linktimer     = linktimer;
    req.pMessage      = pMessage;

    eRCode = qmbuild ( pParamField,
                       (BYTE *)&req,
                       map,
                       eQMI_WMS_RAW_SEND,
                       pMlength );

    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the SendSMS Message ID from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    - Pointer to structure containing storage
 *                    to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError UnpackTlvMessageID( BYTE *pTlvData, BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    struct QmiSmsRawSendResp *lResp =
                 (struct QmiSmsRawSendResp *)pResp;
    WORD lWord = 0;

    if ( !lResp->pMessageID)
        return eRCode;

    eRCode = GetWord( pTlvData, &lWord );

    if ( eRCode == eQCWWAN_ERR_NONE )
        *(lResp->pMessageID) = lWord;
    return eRCode;
}

/*
 * This function unpacks the SendSMS Cause Code from the
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
enum eQCWWANError UnpackTlvCauseCode( BYTE *pTlvData, BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    struct QmiSmsRawSendResp *lResp =
                 (struct QmiSmsRawSendResp *)pResp;

    WORD lWord = 0;

    if ( !lResp->pMessageFailureCode )
        return eRCode;

    eRCode = GetWord( pTlvData, &lWord );

    if ( eRCode == eQCWWAN_ERR_NONE )
        *(lResp->pMessageFailureCode) = lWord;

    return eRCode;
}

/*
 * This function unpacks the RAW_SEND response
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
enum eQCWWANError UpkQmiSmsRawSend(
    BYTE                     *pMdmResp,
    struct QmiSmsRawSendResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_MESSAGE_ID,   &UnpackTlvMessageID },
        { eTLV_CAUSE_CODE,   &UnpackTlvCauseCode },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };
    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_WMS_RAW_SEND );
    return eRCode;
}

