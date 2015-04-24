/*
 * \ingroup dms
 *
 * \file qaDmsUIMUnblockPIN.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        QMI_DMS_UIM_UNBLOCK_PIN message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaQmiBasic.h"

#include "qaDmsUIMUnblockPIN.h"


/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the UIMUnblockPIN PinUnblock Information field
 * to the QMI message SDU
 *
 * \param pBuf   - Pointer to storage into which the packed
 *                  data will be placed by this function.
 *
 * \param pParam - Pointer to structure containing data for this TLV.
 *
 */
enum eQCWWANError BuildTlvPinUnblockInformation( BYTE *pBuf, BYTE *pParam )
{
    struct QmiDmsUIMUnblockPINReq *pReq =
                     (struct QmiDmsUIMUnblockPINReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pReq->pPUKValue || !pReq->pNewValue)
        return eRCode;

    /* Add TLV data */
    eRCode = PutByte( pBuf, pReq->id );
    if ( eRCode == eQCWWAN_ERR_NONE )
    {
        /* Assuming Size shall not go beyond byte storage */
        eRCode = PutByte( pBuf, slstrlen(pReq->pPUKValue) );
        while ( (*(pReq->pPUKValue) != EOS ) &&
                ( eRCode == eQCWWAN_ERR_NONE ) )
        {
            eRCode = PutByte( pBuf, *(pReq->pPUKValue)++ );
        }
        if ( eRCode == eQCWWAN_ERR_NONE )
        {
            eRCode = PutByte( pBuf, slstrlen(pReq->pNewValue) );
            while ( (*(pReq->pNewValue) != EOS ) &&
                    ( eRCode == eQCWWAN_ERR_NONE ) )
            {
                eRCode = PutByte( pBuf, *(pReq->pNewValue)++ );
            }
        }
    }

    return eRCode;
}

/*
 * This function packs the UIMUnblockPIN parameters to the QMI message SDU
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
enum eQCWWANError PkQmiDmsUIMUnblockPIN(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG id,
    CHAR  *pPUKValue,
    CHAR  *pNewValue )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_PIN_UNBLOCK_INFORMATION, &BuildTlvPinUnblockInformation },
        { eTLV_TYPE_INVALID,            NULL } /* Important. Sentinel.
                                                * Signifies last item in map.
                                                */
    };

    struct QmiDmsUIMUnblockPINReq req;

    enum eQCWWANError eRCode;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiDmsUIMUnblockPINReq));
    req.id         = id;
    req.pPUKValue  = pPUKValue;
    req.pNewValue  = pNewValue;

    eRCode = qmbuild ( pParamField,
                       (BYTE *)&req,
                       map,
                       eQMI_DMS_UIM_PIN_UNBLOCK,
                       pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the retries left from the UIM_UNBLOCK_PIN
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
enum eQCWWANError UnpackTlvRetriesRemaining( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiDmsUIMUnblockPINResp *lResp =
        (struct QmiDmsUIMUnblockPINResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    BYTE lByte = 0;

    if ( !lResp->pVerifyRetriesLeft || !lResp->pUnblockRetriesLeft )
        return eRCode;

    eRCode = GetByte( pTlvData, &lByte );
    if ( eRCode == eQCWWAN_ERR_NONE )
    {
        *(lResp->pVerifyRetriesLeft) = lByte;

        eRCode = GetByte( pTlvData, &lByte );
        if ( eRCode == eQCWWAN_ERR_NONE )
            *(lResp->pUnblockRetriesLeft) = lByte;
    }

    return eRCode;
}

/*
 * This function unpacks the UIM_UNBLOCK_PIN response
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
enum eQCWWANError UpkQmiDmsUIMUnblockPIN(
    BYTE                           *pMdmResp,
    struct QmiDmsUIMUnblockPINResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,       &qmUnpackTlvResultCode },
        { eTLV_RETRIES_REMAINING, &UnpackTlvRetriesRemaining },
        { eTLV_TYPE_INVALID,      NULL } /* Important. Sentinel.
                                          * Signifies last item in map.
                                          */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_DMS_UIM_PIN_UNBLOCK);

    return eRCode;
}

