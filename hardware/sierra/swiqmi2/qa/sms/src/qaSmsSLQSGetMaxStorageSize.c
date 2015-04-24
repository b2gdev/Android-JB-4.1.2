/*
 * \ingroup sms
 *
 * \file    qaSmsSLQSGetMaxStorageSize.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          eQMI_WMS_GET_STORE_MAX_SIZE message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaSmsSLQSGetMaxStorageSize.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * Packs the GetMaxStorageSize Storage Type field to the QMI message SDU
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
local enum eQCWWANError BuildTlvReqMemoryStorage( BYTE *pBuf, BYTE *pParam )
{
    smsMaxStorageSizeReq *lReq = (smsMaxStorageSizeReq *)pParam;

    /* Add TLV data */
    return PutByte( pBuf, lReq->storageType );
}

/*
 * This function packs the GetMaxStorageSize Message Mode field
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
local enum eQCWWANError BuildTlvReqMessageMode( BYTE *pBuf, BYTE *pParam )
{
    smsMaxStorageSizeReq *lReq = (smsMaxStorageSizeReq *)pParam;

    if (NULL == lReq->pMessageMode)
    {
       return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(lReq->pMessageMode) );
}

/*
 * This function packs the SLQS Get Maximum Storage Size request parameters to
 * the QMI message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed data
 *                              will be placed by this function.
 *
 * \param  pMaxStorageSizeReq[IN]
 *         - Pointer to Structure containing values to be packed
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError PkQmiSmsSLQSGetMaxStorageSize(
    WORD *pMlength,
    BYTE *pParamField,
    smsMaxStorageSizeReq *pMaxStorageSizeReq )
{
    /* There are mandatory TLV and hence the map contains all those */
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_MEMORY_STORE, &BuildTlvReqMemoryStorage },
        { eTLV_MESSAGE_MODE, &BuildTlvReqMessageMode },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map
                                     */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode =  qmbuild( pParamField,
                       (BYTE *)pMaxStorageSizeReq,
                       map,
                       eQMI_WMS_GET_MSG_LIST_MAX,
                       pMlength );
    return eRCode;
}

/*****************************************************************************
 * Response handling
 *****************************************************************************/
/*
 * Unpacks the memory store size from the QMI response message to a user
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
enum eQCWWANError UnpackTlvMaxStorageSize( BYTE *pTlvData, BYTE *pResp )
{
    smsMaxStorageSizeResp *lResp =
        ( ( struct QmiSmsSLQSGetMaxStorageSizeResp * )pResp )->pMaxStorageSizeResp;

    /* Extract the parameters into the structure */
    return GetLong( pTlvData, &lResp->maxStorageSize );
}

/*
 * Unpacks the memory available from the QMI response message to a user
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
enum eQCWWANError UnpackTlvStorageFreeSlot( BYTE *pTlvData, BYTE *pResp )
{
    smsMaxStorageSizeResp *lResp =
        ( ( struct QmiSmsSLQSGetMaxStorageSizeResp * )pResp )->pMaxStorageSizeResp;

    /* Extract the parameters into the structure */
    return GetLong( pTlvData, &lResp->freeSlots );
}

/*
 * This function unpacks the SLQS Get Maximum Storage Size response to
 * a user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiSmsSLQSGetmaxStorageSize(
    BYTE                                *pMdmResp,
    struct QmiSmsSLQSGetMaxStorageSizeResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,      &qmUnpackTlvResultCode },
        { eTLV_STORE_MAX_SIZE,   &UnpackTlvMaxStorageSize },
        { eTLV_STORE_FREE_SLOT,  &UnpackTlvStorageFreeSlot },
        { eTLV_TYPE_INVALID,     NULL } /* Important. Sentinel.
                                         * Signifies last item in map.
                                         */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_WMS_GET_MSG_LIST_MAX );
    return eRCode;
}
