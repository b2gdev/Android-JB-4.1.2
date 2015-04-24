/*
 * \ingroup dms
 *
 * \file qaDmsUIMChangePIN.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        QMI_DMS_UIM_CHANGE_PIN message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaQmiBasic.h"

#include "qaDmsUIMChangePIN.h"


/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the UIMChangePIN PinChange Information field
 * to the QMI message SDU
 *
 * \param   pBuf   - Pointer to storage into which the packed
 *                   data will be placed by this function.
 *
 * \param   pParam - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE   - on success
 *
 */
enum eQCWWANError BuildTlvPinChangeInformation( BYTE *pBuf, BYTE *pParam )
{
    struct QmiDmsUIMChangePINReq *pReq =
                     (struct QmiDmsUIMChangePINReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pReq->pOldValue || !pReq->pNewValue )
        return eRCode;

    /* Add TLV data */
    eRCode = PutByte( pBuf, pReq->id );
 
    if ( eRCode == eQCWWAN_ERR_NONE )
    {
        /* Assuming Size shall not go beyond byte storage */
        eRCode = PutByte( pBuf, slstrlen(pReq->pOldValue) );
        while( (*(pReq->pOldValue) != EOS ) &&
               ( eRCode == eQCWWAN_ERR_NONE ))
        {
            eRCode = PutByte( pBuf, *(pReq->pOldValue)++ );
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
 * This function packs the UIMChangePIN parameters to the QMI message SDU
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
enum eQCWWANError PkQmiDmsUIMChangePIN(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG id,
    CHAR  *pOldValue,
    CHAR  *pNewValue )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_PIN_CHANGE_INFORMATION, &BuildTlvPinChangeInformation },
        { eTLV_TYPE_INVALID,            NULL } /* Important. Sentinel.
                                                * Signifies last item in map.
                                                */
    };

    struct QmiDmsUIMChangePINReq req;

    enum eQCWWANError eRCode;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiDmsUIMChangePINReq));
    req.id         = id;
    req.pOldValue  = pOldValue;
    req.pNewValue  = pNewValue;

    eRCode = qmbuild ( pParamField,
                       (BYTE *)&req,
                       map,
                       eQMI_DMS_UIM_PIN_CHANGE,
                       pMlength );

    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the retries left from the UIM_CHANGE_PIN
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
enum eQCWWANError UnpackTlvRemainingRetries( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiDmsUIMChangePINResp *lResp =
        (struct QmiDmsUIMChangePINResp *)pResp;

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
 * This function unpacks the UIM_CHANGE_PIN response
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
enum eQCWWANError UpkQmiDmsUIMChangePIN(
    BYTE                          *pMdmResp,
    struct QmiDmsUIMChangePINResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,       &qmUnpackTlvResultCode },
        { eTLV_REMAINING_RETRIES, &UnpackTlvRemainingRetries },
        { eTLV_TYPE_INVALID,      NULL } /* Important. Sentinel.
                                          * Signifies last item in map.
                                          */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_DMS_UIM_PIN_CHANGE );
    return eRCode;
}

