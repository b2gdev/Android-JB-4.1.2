/*
 * \ingroup sms
 *
 * \file qaSmsModifyTag.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        QMI_WMS_MODIFY_TAG message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaSmsModifyTag.h"


/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the ModifySMSStatus WMS Message Tag field
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
enum eQCWWANError BuildTlvSmsMessageTag( BYTE *pBuf, BYTE *pParam )
{
    struct QmiSmsModifyTagReq *pReq =
                     (struct QmiSmsModifyTagReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Add TLV data */
    eRCode = PutByte( pBuf, pReq->storageType );
    if ( eRCode == eQCWWAN_ERR_NONE )
    {
        eRCode = PutLong( pBuf, pReq->messageIndex );
    if ( eRCode == eQCWWAN_ERR_NONE )
         eRCode = PutByte( pBuf, pReq->messageTag );
    }
    return eRCode;
}

/*
 * This function packs the ModifySMSStatus Message Mode field
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
enum eQCWWANError BuildTlvSmsMessageMode( BYTE *pBuf, BYTE *pParam )
{
    struct QmiSmsModifyTagReq *pReq =
                     (struct QmiSmsModifyTagReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pReq->pMode )
        return eRCode;

    /* Add TLV data */
    eRCode = PutByte( pBuf, *(pReq->pMode) );

    return eRCode;
}

/*
 * This function packs the ModifySMSStatus parameters to the QMI message SDU
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
enum eQCWWANError PkQmiSmsModifyTag(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG storageType,
    ULONG messageIndex,
    ULONG messageTag,
    BYTE  *pMode )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SMS_MESSAGE_TAG,  &BuildTlvSmsMessageTag },
        { eTLV_SMS_MESSAGE_MODE, &BuildTlvSmsMessageMode },
        { eTLV_TYPE_INVALID,     NULL } /* Important. Sentinel.
                                        * Signifies last item in map.
                                        */
    };

    struct QmiSmsModifyTagReq req;
    enum   eQCWWANError       eRCode;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiSmsModifyTagReq));
    req.storageType  = storageType ;
    req.messageIndex = messageIndex ;
    req.messageTag   = messageTag;
    req.pMode        = pMode;

    eRCode = qmbuild ( pParamField,
                       (BYTE *)&req,
                       map,
                       eQMI_WMS_MODIFY_TAG,
                       pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the MODIFY_TAG response
 * message to a user-provided response structure.
 *
 * \param  MdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp  [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError UpkQmiSmsModifyTag(
    BYTE                       *pMdmResp,
    struct QmiSmsModifyTagResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };
    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_WMS_MODIFY_TAG );
    return eRCode;
}
