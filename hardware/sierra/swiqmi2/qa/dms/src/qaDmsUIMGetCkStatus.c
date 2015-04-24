/*
 * \ingroup dms
 *
 * \file qaDmsUIMGetCkStatus.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        QMI_DMS_UIM_GET_CK_STATUS message.
 *
 * Copyright: © 2010 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaQmiBasic.h"
#include "qaDmsUIMGetCkStatus.h"


/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the UIMGetCkStatus facility field
 * to the QMI message SDU
 *
 * \param  pBuf   - Pointer to storage into which the packed
 *                 data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvFacility( BYTE *pBuf, BYTE *pParam )
{
    struct QmiDmsUIMGetCkStatusReq *pReq =
                     (struct QmiDmsUIMGetCkStatusReq *)pParam;

    /* Add TLV data */
    return PutByte(pBuf, (BYTE)pReq->id);
}

/*
 * This function packs the UIMGetCkStatus parameters to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                                    data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError PkQmiDmsUIMGetCkStatus(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG id )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_FACILITY,     &BuildTlvFacility },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };

    struct QmiDmsUIMGetCkStatusReq req;

    enum eQCWWANError eRCode;

    slmemset((char *)&req, 0, sizeof(struct QmiDmsUIMGetCkStatusReq));
    req.id = id;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_DMS_UIM_GET_CK_STATUS,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the retries left from the UIM_GET_CK_STATUS
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
enum eQCWWANError UnpackTlvCKStatus( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiDmsUIMGetCkStatusResp *lResp =
        (struct QmiDmsUIMGetCkStatusResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    BYTE lByte = 0;

    if ( !lResp->pStatus || !lResp->pVerifyRetriesLeft
         || !lResp->pUnblockRetriesLeft )
        return eRCode;

    eRCode = GetByte( pTlvData, &lByte );
    if( eRCode == eQCWWAN_ERR_NONE )
    {
        *(lResp->pStatus) = lByte;
        eRCode = GetByte( pTlvData, &lByte );
        if ( eRCode == eQCWWAN_ERR_NONE )
        {
            *(lResp->pVerifyRetriesLeft) = lByte;
            eRCode = GetByte( pTlvData, &lByte ); 
            if ( eRCode == eQCWWAN_ERR_NONE )
                *(lResp->pUnblockRetriesLeft) = lByte;
        }
    }
    return eRCode;
}

/*
 * This function unpacks the UIM_GET_CK_STATUS response
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
enum eQCWWANError UpkQmiDmsUIMGetCkStatus(
    BYTE                            *pMdmResp,
    struct QmiDmsUIMGetCkStatusResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_CK_STATUS,    &UnpackTlvCKStatus },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_DMS_UIM_GET_CK_STATUS );

    return eRCode;
}

