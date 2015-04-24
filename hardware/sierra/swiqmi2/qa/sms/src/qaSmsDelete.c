/*
 * \ingroup sms
 *
 * \file    qaSmsDelete.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_WMS_DELETE message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */
#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaSmsDelete.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the DeleteSMS Storage Type field
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
enum eQCWWANError BuildTlvStorageType( BYTE *pBuf, BYTE *pParam )
{
    struct QmiSmsDeleteReq *pReq =
                     (struct QmiSmsDeleteReq *)pParam;

    /* Add TLV data */
    return PutByte( pBuf, pReq->storageType );
}

/*
 * This function packs the DeleteSMS Message Index field
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
enum eQCWWANError BuildTlvMessageIndex( BYTE *pBuf, BYTE *pParam )
{
    struct QmiSmsDeleteReq *pReq =
                 (struct QmiSmsDeleteReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pReq->pMessageIndex )
        return eRCode;

    /* Add TLV data */
    return PutLong( pBuf, *(pReq->pMessageIndex) );
}

/*
 * This function packs the DeleteSMS Message Tag field
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
enum eQCWWANError BuildTlvMessageTag( BYTE *pBuf, BYTE *pParam )
{
    struct QmiSmsDeleteReq *pReq =
                 (struct QmiSmsDeleteReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pReq->pMessageTag )
        return eRCode;

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pMessageTag) );
}

/*
 * This function packs the DeleteSMS Message Mode field
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
enum eQCWWANError BuildTlvMessageMod( BYTE *pBuf, BYTE *pParam )
{
    struct QmiSmsDeleteReq *pReq =
                 (struct QmiSmsDeleteReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pReq->pMode )
        return eRCode;

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pMode) );
}

/*
 * This function packs the DeleteSMS parameters to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                                    data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \param  storageType
 *         - SMS message storage type
 *             - 0 - UIM
 *             - 1 - NV
 *
 * \param  pMessageIndex[IN]
 *         - (Optional) message index
 *
 * \param  pMessageTag[IN]
 *         - (Optional) message tag
 *             - 0 - Read
 *             - 1 - Not read
 *             - 2 - Mobile originated and sent
 *             - 3 - Mobile originated but not yet sent
 *
 * \param  pMessageMode[IN]
 *         - 0x00 - CDMA
 *           0x01 - GW
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError PkQmiSmsDelete(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG storageType,
    ULONG *pMessageIndex,
    ULONG *pMessageTag,
    BYTE  *pMode )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_STORAGE_TYPE,  &BuildTlvStorageType },
        { eTLV_MESSAGE_INDEX, &BuildTlvMessageIndex },
        { eTLV_MESSAGE_TAG,   &BuildTlvMessageTag },
        { eTLV_MESSAGE_MOD,   &BuildTlvMessageMod },
        { eTLV_TYPE_INVALID,  NULL } /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    struct QmiSmsDeleteReq req;
    enum   eQCWWANError    eRCode;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiSmsDeleteReq));
    req.storageType   = storageType;
    req.pMessageIndex = pMessageIndex;
    req.pMessageTag   = pMessageTag;
    req.pMode         = pMode;

    eRCode = qmbuild ( pParamField,
                       (BYTE *)&req,
                       map,
                       eQMI_WMS_DELETE,
                       pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the DeleteSMS response
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
enum eQCWWANError UpkQmiSmsDelete(
    BYTE                    *pMdmResp,
    struct QmiSmsDeleteResp *pApiResp )
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
                           eQMI_WMS_DELETE );
    return eRCode;
}

