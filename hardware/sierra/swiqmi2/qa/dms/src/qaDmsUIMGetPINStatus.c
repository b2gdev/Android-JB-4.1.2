/*
 * \ingroup dms
 *
 * \file qaDmsUIMGetPINStatus.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        QMI_DMS_UIM_GET_PIN_STATUS message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaQmiBasic.h"

#include "qaDmsUIMGetPINStatus.h"

#define SIM_PIN_ONE 1
#define SIM_PIN_TWO 2

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the UIMGetPINStatus parameters to the QMI message SDU
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
enum eQCWWANError PkQmiDmsUIMGetPINStatus(
    WORD  *pMlength,
    BYTE  *pParamField )
{
    /* There is no mandatory TLV and hence the map is empty */
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };

    enum eQCWWANError eRCode;

    eRCode = qmbuild( pParamField,
                      NULL,
                      map,
                      eQMI_DMS_UIM_GET_PIN_STATUS,
                      pMlength );

    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the retries left from the UIM_GET_PIN_STATUS
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
enum eQCWWANError UnpackTlvPIN1Status( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiDmsUIMGetPINStatusResp *lResp =
        (struct QmiDmsUIMGetPINStatusResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    BYTE lByte = 0;

    if ( lResp->id != SIM_PIN_ONE )
        return eRCode;

    if ( !lResp->pStatus || !lResp->pVerifyRetriesLeft
        || !lResp->pUnblockRetriesLeft )
        return eRCode;

    eRCode = GetByte( pTlvData, &lByte );
    if ( eRCode == eQCWWAN_ERR_NONE )
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
 * This function unpacks the retries left from the UIM_GET_PIN_STATUS
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
enum eQCWWANError UnpackTlvPIN2Status( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiDmsUIMGetPINStatusResp *lResp =
    (struct QmiDmsUIMGetPINStatusResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    BYTE lByte = 0;

    if ( lResp->id != SIM_PIN_TWO )
        return eRCode;

    if ( !lResp->pStatus || !lResp->pVerifyRetriesLeft
         || !lResp->pUnblockRetriesLeft )
        return eRCode;

    eRCode = GetByte( pTlvData, &lByte );
    if ( eRCode == eQCWWAN_ERR_NONE )
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
 * This function unpacks the UIM_GET_PIN_STATUS response
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
enum eQCWWANError UpkQmiDmsUIMGetPINStatus(
    BYTE                             *pMdmResp,
    struct QmiDmsUIMGetPINStatusResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_PIN_1_STATUS, &UnpackTlvPIN1Status },
        { eTLV_PIN_2_STATUS, &UnpackTlvPIN2Status },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_DMS_UIM_GET_PIN_STATUS );

    return eRCode;
}

