/*
 * \ingroup dms
 *
 * \file qaDmsUIMSetCkProtection.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        QMI_DMS_UIM_SET_CK_PROTECTION message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaQmiBasic.h"

#include "qaDmsUIMSetCkProtection.h"


/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the UIMSetCkProtection facility field
 * to the QMI message SDU
 *
 * \param  pBuf   - Pointer to storage into which the packed
 *                 data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvSetFacility( BYTE *pBuf, BYTE *pParam )
{
    struct QmiDmsUIMSetCkProtectionReq *pReq =
                     (struct QmiDmsUIMSetCkProtectionReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pReq->pValue )
        return eRCode;

    /* Add TLV data */
    eRCode = PutByte( pBuf, pReq->id );
    if ( eRCode == eQCWWAN_ERR_NONE )
    {
        eRCode = PutByte( pBuf, pReq->status );
        if ( eRCode == eQCWWAN_ERR_NONE )
        {
            /* Assuming Length of pValue shall not exceed BYTE storage */
            eRCode = PutByte( pBuf, slstrlen(pReq->pValue));
            while ( (*(pReq->pValue) != EOS) &&
                    (eRCode == eQCWWAN_ERR_NONE) ) 
            {
                eRCode = PutByte( pBuf, *(pReq->pValue)++ );
            }
        }
    }
    return eRCode;
}

/*
 * This function packs the UIMSetCkProtection parameters to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 * \sa     qaGobiApiDms.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiDmsUIMSetCkProtection(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG id,
    ULONG status,
    CHAR  *pValue )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SET_FACILITY, &BuildTlvSetFacility },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };

    struct QmiDmsUIMSetCkProtectionReq req;

    enum eQCWWANError eRCode;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiDmsUIMSetCkProtectionReq));
    req.id     = id;
    req.status = status;
    req.pValue = pValue;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_DMS_UIM_SET_CK_PROT,
                      pMlength );

    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the retries left from the UIM_SET_CK_PROTECTION
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
enum eQCWWANError UnpackTlvVerifyRetriesLeft(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiDmsUIMSetCkProtectionResp *lResp =
        (struct QmiDmsUIMSetCkProtectionResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    BYTE lByte = 0;

    if ( !lResp->pVerifyRetriesLeft )
        return eRCode;

    eRCode = GetByte( pTlvData, &lByte );
    if ( eRCode == eQCWWAN_ERR_NONE )
        *(lResp->pVerifyRetriesLeft) = lByte;

    return eRCode;
}

/*
 * This function unpacks the UIM_SET_CK_PROTECTION response
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
enum eQCWWANError UpkQmiDmsUIMSetCkProtection(
    BYTE                                *pMdmResp,
    struct QmiDmsUIMSetCkProtectionResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,         &qmUnpackTlvResultCode },
        { eTLV_VERIFY_RETRIES_LEFT, &UnpackTlvVerifyRetriesLeft },
        { eTLV_TYPE_INVALID,        NULL } /* Important. Sentinel.
                                            * Signifies last item in map.
                                            */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_DMS_UIM_SET_CK_PROT );
    return eRCode;
}

