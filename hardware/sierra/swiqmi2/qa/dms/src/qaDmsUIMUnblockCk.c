/*
 * \ingroup  dms
 *
 * \file qaDmsUIMUnblockCk.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        QMI_DMS_UIM_UNBLOCK_CK message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaQmiBasic.h"

#include "qaDmsUIMUnblockCk.h"


/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the UIMUnblockCk facility field
 * to the QMI message SDU
 *
 * \param  pBuf   - Pointer to storage into which the packed
 *                  data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlvFacilityInformation( BYTE *pBuf, BYTE *pParam )
{
    struct QmiDmsUIMUnblockCkReq *pReq =
                     (struct QmiDmsUIMUnblockCkReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pReq->pValue )
        return eRCode;

    /* Add TLV data */
    eRCode = PutByte( pBuf, pReq->id );
    if ( eRCode == eQCWWAN_ERR_NONE )
    {
        /* Assuming Length of pValue shall not exceed BYTE storage */
        eRCode = PutByte( pBuf, slstrlen(pReq->pValue) );
        while ( (*(pReq->pValue) != EOS) &&
                (eRCode == eQCWWAN_ERR_NONE) )
        {
            eRCode = PutByte(pBuf, *(pReq->pValue)++);
        }
    }

    return eRCode;
}

/*
 * This function packs the UIMUnblockCk parameters to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                                    data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 * \sa     qaGobiApiDms.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiDmsUIMUnblockCk(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG id,
    CHAR  *pValue )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_FACILITY_INFO, &BuildTlvFacilityInformation },
        { eTLV_TYPE_INVALID,  NULL } /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    struct QmiDmsUIMUnblockCkReq req;

    enum eQCWWANError eRCode;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiDmsUIMUnblockCkReq));
    req.id     = id;
    req.pValue = pValue;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_DMS_UIM_UNBLOCK_CK,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the retries left from the UIM_UNBLOCK_CK
 * response message to a user provided response structure.
 *
 * \param  pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]    - Pointer to structure containing storage
 *                             to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError UnpackTlvUnblockRetriesLeft( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiDmsUIMUnblockCkResp *lResp =
        (struct QmiDmsUIMUnblockCkResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    BYTE lByte = 0;

    if ( !lResp->pUnblockRetriesLeft )
        return eRCode;

    eRCode = GetByte( pTlvData, &lByte );
    if ( eRCode == eQCWWAN_ERR_NONE )
        *(lResp->pUnblockRetriesLeft) = lByte;

    return eRCode;
}

/*
 * This function unpacks the UIM_UNBLOCK_CK response
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
enum eQCWWANError UpkQmiDmsUIMUnblockCk(
    BYTE                          *pMdmResp,
    struct QmiDmsUIMUnblockCkResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,          &qmUnpackTlvResultCode },
        { eTLV_UNBLOCK_RETRIES_LEFT, &UnpackTlvUnblockRetriesLeft },
        { eTLV_TYPE_INVALID,         NULL } /* Important. Sentinel.
                                             * Signifies last item in map.
                                             */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_DMS_UIM_UNBLOCK_CK );
    return eRCode;
}

