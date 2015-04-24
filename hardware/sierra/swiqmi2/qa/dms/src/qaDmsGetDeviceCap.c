/*
 * \ingroup : dms 
 *
 * \file    : qaDmsGetDeviceCap.c
 *
 * \brief   : Contains Packing and UnPacking routines for the
 *            eQMI_DMS_GET_DEVICE_CAP message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaQmiBasic.h"

#include "qaDmsGetDeviceCap.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the Get Device Capabilities parameters
 * to the QMI message SDU
 *
 * \param       pParamField [OUT] - Pointer to storage into which the packed
 *                                    data will be placed by this function.
 *
 * \param       pMlength    [OUT] - Total length of built message.
 *
 * \return      eQCWWAN_ERR_NONE, on success
 * \return      eQCWWAN_ERR_NO_MEMORY, Access beyond allowed size attempted
 *
 */
enum eQCWWANError PkQmiDmsGetDeviceCap(
    WORD *pMlength,
    BYTE *pParamField )
{
    /* There is no mandatory TLV and hence the map is empty */
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    enum eQCWWANError eRCode;

    eRCode = qmbuild( pParamField,
                      NULL,
                      map,
                      eQMI_DMS_GET_CAPS,
                      pMlength);
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the DeviceCapabilities from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return      eQCWWAN_ERR_NONE, on success
 * \return      eQCWWAN_ERR_NO_MEMORY, Access beyond allowed size attempted
 */
enum eQCWWANError UnpackTlvDeviceCap( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiDmsGetDeviceCapResp *lResp =
              ( struct QmiDmsGetDeviceCapResp *)pResp;
    BYTE lArrayIndex = 0;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    BYTE lByte = 0;
    if ( !lResp->pMaxTXChannelRate      || 
         !lResp->pMaxRXChannelRate      || 
         !lResp->pDataServiceCapability || 
         !lResp->pSimCapability         ||
         !lResp->pRadioIfacesSize       || 
         !lResp->pRadioIfaces )
        
         return eRCode;

    eRCode = GetLong( pTlvData, lResp->pMaxTXChannelRate );
    if ( eQCWWAN_ERR_NONE == eRCode )
    {
        eRCode = GetLong( pTlvData, lResp->pMaxRXChannelRate );
        if ( eQCWWAN_ERR_NONE == eRCode )
        {
            eRCode = GetByte( pTlvData, &lByte );
            if ( eQCWWAN_ERR_NONE == eRCode )
            {
                *(lResp->pDataServiceCapability) = lByte;
                eRCode = GetByte( pTlvData, &lByte );
                if ( eQCWWAN_ERR_NONE == eRCode )
                {
                     *(lResp->pSimCapability) = lByte;
                     eRCode = GetByte( pTlvData, &lByte );
                     if ( eQCWWAN_ERR_NONE == eRCode )
                     {
                         if ( lByte > *(lResp->pRadioIfacesSize) )
                             eRCode = eQCWWAN_ERR_BUFFER_SZ;
                         else
                         {
                             *(lResp->pRadioIfacesSize) = lByte;
                             while ( lByte-- && (eQCWWAN_ERR_NONE == eRCode))
                             {
                                 /* Copy the Device Capability Responses into
                                  *  the array in the current occurance
                                  */
                                 eRCode = GetByte( pTlvData,
                                           &lResp->pRadioIfaces[lArrayIndex] );

                                 lArrayIndex++;
                             }
                         }
                     }
                }
            }
        }
    }
    return eRCode;
}
/******************************************************************************
 * Response handling
 *****************************************************************************/
/*
 * This function unpacks the eQMI_DMS_GET_DEVICECAP_RESP response
 * message to a user-provided response structure.
 *
 * \param  pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp  [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE      - Increment successful
 * \return eQCWWAN_ERR_NO_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError UpkQmiDmsGetDeviceCap(
    BYTE                           *pMdmResp,
    struct QmiDmsGetDeviceCapResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_DEVICE_CAP,   &UnpackTlvDeviceCap},
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_DMS_GET_CAPS);
    return eRCode;
}
