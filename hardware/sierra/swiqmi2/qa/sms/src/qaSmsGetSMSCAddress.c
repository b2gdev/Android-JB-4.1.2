/*
 * \ingroup wms
 *
 * \file    qaSmsGetSMSCAddress.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_WMS_GET_SMSC_ADDRESS message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaSmsGetSMSCAddress.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the GetSMSCAddressSMS parameters to the QMI message SDU
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
enum eQCWWANError PkQmiSmsGetSMSCAddress(
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
    eRCode = qmbuild ( pParamField,
                       NULL,
                       map,
                       eQMI_WMS_GET_SMSC_ADDR,
                       pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the GetSMSCAddress raw message data from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError UnpackTlvSMSCAddress( BYTE *pTlvData, BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    struct QmiSmsGetSMSCAddressResp *lResp =
                 (struct QmiSmsGetSMSCAddressResp *)pResp;

    BYTE count = 0, lByte = 0;

    if ( !lResp->pSMSCAddress || !lResp->pSMSCType ) 
        return eRCode;

    for ( count = 0; count < QMI_SMSC_TYPE_LEN; count++ ) 
    {
        eRCode = GetByte( pTlvData, &lByte );
        if ( eRCode != eQCWWAN_ERR_NONE )
            return eRCode;
        else
            *(lResp->pSMSCType)++ = lByte;
    }
    *(lResp->pSMSCType)++ = EOS;

    /* Using Count for SMSC Address length */
    eRCode = GetByte( pTlvData, &count );
    if ( count > lResp->addressSize )
        eRCode = eQCWWAN_ERR_BUFFER_SZ;

    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    while ( count-- && (eRCode == eQCWWAN_ERR_NONE) )
    {
        eRCode = GetByte( pTlvData, &lByte );
        if ( eRCode == eQCWWAN_ERR_NONE )
            *(lResp->pSMSCAddress)++ = lByte;
    }
    *(lResp->pSMSCAddress)++ = EOS;

    return eRCode;
}

/*
 * This function unpacks the GET_SMSC_ADDRESS response
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
enum eQCWWANError UpkQmiSmsGetSMSCAddress(
    BYTE                            *pMdmResp,
    struct QmiSmsGetSMSCAddressResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_SMSC_ADDRESS, &UnpackTlvSMSCAddress }, 
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };
    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_WMS_GET_SMSC_ADDR );
    return eRCode;
}

