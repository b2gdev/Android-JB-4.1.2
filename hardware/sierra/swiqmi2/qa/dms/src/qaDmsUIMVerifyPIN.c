/*
 * \ingroup dms
 *
 * \file qaDmsUIMVerifyPIN.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        QMI_DMS_UIM_VERIFY_PIN message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaQmiBasic.h"

#include "qaDmsUIMVerifyPIN.h"


/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the UIMVerifyPIN PinInformation field
 * to the QMI message SDU
 *
 * \param  pBuf   - Pointer to storage into which the packed
 *                  data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE - on success
 *
 */
enum eQCWWANError BuildTlvPinInformation( BYTE *pBuf, BYTE *pParam )
{
    struct QmiDmsUIMVerifyPINReq *pReq =
                     (struct QmiDmsUIMVerifyPINReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pReq->pValue )
        return eRCode;

    /* Add TLV data */
    eRCode = PutByte( pBuf, pReq->id );

    if ( eRCode == eQCWWAN_ERR_NONE )
    { 
        /* Assuming Size shall not go beyond byte storage */
        eRCode = PutByte( pBuf, slstrlen(pReq->pValue) );
        while ( (*(pReq->pValue) != EOS ) &&
                ( eRCode == eQCWWAN_ERR_NONE ))
        {
            eRCode = PutByte( pBuf, *(pReq->pValue)++ );
        }
    }
    return eRCode; 
}

/*
 * This function packs the UIMverifyPIN parameters to the QMI message SDU
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
enum eQCWWANError PkQmiDmsUIMVerifyPIN(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG id,
    CHAR  *pValue)
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_PIN_INFORMATION, &BuildTlvPinInformation },
        { eTLV_TYPE_INVALID,    NULL }  /* Important. Sentinel.
                                         * Signifies last item in map.
                                         */
    };

    struct QmiDmsUIMVerifyPINReq req;

    enum eQCWWANError eRCode;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiDmsUIMVerifyPINReq));
    req.id     = id;
    req.pValue = pValue;

    eRCode = qmbuild ( pParamField,
                       (BYTE *)&req,
                       map,
                       eQMI_DMS_UIM_PIN_VERIFY,
                       pMlength );

    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the retries left from the UIM_VERIFY_PIN
 * response message to a user provided response structure.
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError UnpackTlvLeftRetries( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiDmsUIMVerifyPINResp *lResp =
        (struct QmiDmsUIMVerifyPINResp *)pResp;

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
 * This function unpacks the UIM_VERIFY_PIN response
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
enum eQCWWANError UpkQmiDmsUIMVerifyPIN(
    BYTE                          *pMdmResp,
    struct QmiDmsUIMVerifyPINResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_LEFT_RETRIES, &UnpackTlvLeftRetries },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_DMS_UIM_PIN_VERIFY );

    return eRCode;
}

