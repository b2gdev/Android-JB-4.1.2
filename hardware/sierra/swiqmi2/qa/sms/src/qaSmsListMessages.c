/*
 * \ingroup : sms
 *
 * \file    : qaSmsListMessages.c
 *
 * \brief   : Contains Packing and UnPacking routines for the
 *            eQMI_SMS_LIST_MESSAGE message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaSmsListMessages.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * Packs the GetSMSList Storage Type field to the QMI message SDU
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
enum eQCWWANError BuildTlvReqMemoryStorage( BYTE *pBuf, BYTE *pParam )
{
    struct QmiSmsListMessagesReq *pReq =
                     (struct QmiSmsListMessagesReq *)pParam;

    /* Add TLV data */
    return PutByte( pBuf, pReq->storageType );
}

/*
 * Packs the GetSMSList MessageTag field to the QMI message SDU
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
enum eQCWWANError BuildTlvReqTag( BYTE *pBuf, BYTE *pParam )
{
    struct QmiSmsListMessagesReq *pReq =
                 (struct QmiSmsListMessagesReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pReq->pRequestedTag )
        return eRCode;

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pRequestedTag) );
}

/*
 * This function packs the GetSMSList Message Mode field
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
enum eQCWWANError BuildTlvSmsListMessageMode( BYTE *pBuf, BYTE *pParam )
{
    struct QmiSmsListMessagesReq *pReq =
                     (struct QmiSmsListMessagesReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pReq->pMode )
        return eRCode;

    /* Add TLV data */
    eRCode = PutByte( pBuf, *(pReq->pMode) );

    return eRCode;
}

/*
 * Packs the GetSMSList parameters to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 *  \param  storageType
 *          - SMS message storage type
 *              - 0 - UIM
 *              - 1 - NV
 *
 *  \param  pRequestedTag[IN]
 *          - (Optional) Message tag
 *              - 0 - Read
 *              - 1 - Not read
 *              - 2 - Mobile originated and sent
 *              - 3 - Mobile originated but not yet sent
 *
 *  \param  pMode[IN]
 *          - 0x00 - CDMA
 *            0x01 - GW
 *
 * \return eQCWWAN_ERR_NONE, on success
 * \return eQCWWAN_ERR_NO_MEMORY, Access beyond allowed size attempted
 *
 */
enum eQCWWANError PkQmiSmsListMessages(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG storageType,
    ULONG *pRequestedTag,
    BYTE  *pMode )
{
    /* There are mandatory TLV and hence the map contains all those */
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_REQ_MEMORY_STORAGE,    &BuildTlvReqMemoryStorage },
        { eTLV_REQ_TAG,               &BuildTlvReqTag },
        { eTLV_SMS_LIST_MESSAGE_MODE, &BuildTlvSmsListMessageMode },
        { eTLV_TYPE_INVALID,          NULL }  /* Important. Sentinel.
                                               * Signifies last item in map.
                                               */
    };

    struct QmiSmsListMessagesReq req;

    enum eQCWWANError eRCode;

    slmemset( (CHAR *)&req, 0, sizeof(struct QmiSmsListMessagesReq) );
    req.storageType   = storageType;
    req.pRequestedTag = pRequestedTag;
    req.pMode         = pMode;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_WMS_GET_MSG_LIST,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * Unpacks the GetSMSList from the QMI response message to a user
 * provided response structure
 *
 * \param       pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]    - Pointer to structure containing storage
 *                                  to return data for this TLV.
 *
 * \return      eQCWWAN_ERR_NONE, on success
 * \return      eQCWWAN_ERR_NO_MEMORY, Access beyond allowed size attempted
 */
enum eQCWWANError UnpackTlvMessagesList( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiSmsListMessagesResp *lResp =
              ( struct QmiSmsListMessagesResp *)pResp;

    ULONG lMessageList = 0;
    BYTE  lArrayIndex = 0, lByte = 0;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( lResp->pMessageListSize == NULL ||
         lResp->pMessageList     == NULL )
         return eRCode;

    /* Extract the parameters into the structure */
    eRCode  = GetLong( pTlvData, &lMessageList );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Check for the Buffer Undersize Error */
    if ( lMessageList > *(lResp->pMessageListSize) )
    {
        *(lResp->pMessageListSize) = lMessageList;
        return eQCWWAN_ERR_BUFFER_SZ;
    }

    *(lResp->pMessageListSize) = lMessageList;

    /* Fill the Message List */
    while ( lMessageList-- && (eQCWWAN_ERR_NONE == eRCode) )
    {
        /* Copy the Message List Responses into the array
         * in the current occurance
         */
        eRCode = GetLong( pTlvData,
                          &(lResp->pMessageList[lArrayIndex].messageIndex) );

        if ( eQCWWAN_ERR_NONE == eRCode )
        {
            eRCode = GetByte( pTlvData, &lByte );
            if ( eQCWWAN_ERR_NONE == eRCode )
                lResp->pMessageList[lArrayIndex].messageTag = lByte;

            lArrayIndex++;
        }
    }
    return eRCode;
}

/******************************************************************************
 * Response handling
 *****************************************************************************/
/*
 * This function unpacks the eQMI_SMS_LIST_MESSAGE_RESP response
 * message to a user-provided response structure.
 *
 * \param  pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp  [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE      - Increment successful
 * \return eQCWWAN_ERR_NO_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError UpkQmiSmsListMessages(
    BYTE                          *pMdmResp,
    struct QmiSmsListMessagesResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,   &qmUnpackTlvResultCode },
        { eTLV_MESSAGES_LIST, &UnpackTlvMessagesList },
        { eTLV_TYPE_INVALID,  NULL } /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };
    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_WMS_GET_MSG_LIST );
    return eRCode;
}
