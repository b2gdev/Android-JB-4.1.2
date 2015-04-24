/*
 *
 * \ingroup : dms 
 *
 * \file    : qaDmsGetDeviceSerialNumbers.c
 *
 * \brief   : Contains Packing and UnPacking routines for the 
 *            QMI_DMS_GET_DEVICE_SERIAL_NUMBERS message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaQmiBasic.h"

#include "qaDmsGetDeviceSerialNumbers.h"


/*****************************************************************************
 * Request handling
 ******************************************************************************/

/*
 *
 * This function packs the Serial Numbers parameters to the QMI message SDU
 *
 * \param     pParamField [OUT] - Pointer to storage into which the packed
 *                                    data will be placed by this function.
 *
 * \param     pMlength    [OUT] - Total length of built message.
 *
 * \return    eQCWWAN_ERR_NONE    - Increment successful
 * \return    eQCWWAN_ERR_MEMORY   - Increment beyond allowed size attempted
 *
 */
enum eQCWWANError PkQmiDmsGetDeviceSerialNumbers( 
    WORD    *pMlength,
    BYTE    *pParamField )
{
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
                      eQMI_DMS_GET_IDS,
                      pMlength);

    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/

/*
 *
 * This function unpacks the ESN from the DEVICE_SERIAL_NUMBERS response message  
 * to a user provided response structure.
 *
 * \param     pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param     pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return    eQCWWAN_ERR_NONE
 *
 */
enum eQCWWANError UnpackTlvESN (BYTE* pTlvData, BYTE *pResp)
{
    struct QmiDmsGetDeviceSerialNumbersResp *lResp =
        (struct QmiDmsGetDeviceSerialNumbersResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( lResp->esnSize == 0 )
        return eQCWWAN_ERR_NONE;

    /* Extract the String Parameter */
    /* The size of the string is stored to check for buffer size in API */
    eRCode = qmQmiExtractString( pTlvData,
                                 lResp->pESNString,
                                 lResp->esnSize );
    return eRCode;

}

/*
 *
 * This function unpacks the IMEI from the DEVICE_SERIAL_NUMBERS  
 * response message to a user provided response structure.
 *
 * \param     pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param     pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return    eQCWWAN_ERR_NONE
 *
 */
enum eQCWWANError UnpackTlvIMEI (BYTE* pTlvData, BYTE *pResp)
{
    struct QmiDmsGetDeviceSerialNumbersResp *lResp =
        (struct QmiDmsGetDeviceSerialNumbersResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( lResp->imeiSize == 0 )
        return eQCWWAN_ERR_NONE;

    /* Extract the String Parameter */
    /* The size of the string is stored to check for buffer size in API */
    eRCode = qmQmiExtractString( pTlvData,
                                 lResp->pIMEIString,
                                 lResp->imeiSize );
    return eRCode;
}

/*
 *
 * This function unpacks the MEID from the DEVICE_SERIAL_NUMBERS  
 * response message to a user provided response structure.
 *
 * \param     pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param     pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return    eQCWWAN_ERR_NONE
 *
 */
enum eQCWWANError UnpackTlvMEID (BYTE* pTlvData, BYTE *pResp)
{
    struct QmiDmsGetDeviceSerialNumbersResp *lResp =
        (struct QmiDmsGetDeviceSerialNumbersResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( lResp->meidSize == 0 )
        return eQCWWAN_ERR_NONE;

    /* Extract the String Parameter */
    /* The size of the string is stored to check for buffer size in API */
    eRCode = qmQmiExtractString( pTlvData,
                                 lResp->pMEIDString,
                                 lResp->meidSize );
    return eRCode;
}

/*
 *
 * This function unpacks the GET_DEVICE_SERIAL_NUMBERS response 
 * message to a user-provided response structure.
 *
 * \param     pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 *
 */

enum eQCWWANError UpkQmiDmsGetDeviceSerialNumbers(
    BYTE    *pMdmResp,
    struct  QmiDmsGetDeviceSerialNumbersResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE       ,&qmUnpackTlvResultCode },
        { eTLV_DEVICE_ESN        ,&UnpackTlvESN },
        { eTLV_DEVICE_IMEI       ,&UnpackTlvIMEI },
        { eTLV_DEVICE_MEID       ,&UnpackTlvMEID },
        { eTLV_TYPE_INVALID      ,NULL }  /* Important. Sentinel. 
                                           * Signifies last item in map.
                                           */
    };

    eRCode =  qmunpackresp( pMdmResp,
                           (BYTE*)pApiResp,
                           map,
                           eQMI_DMS_GET_IDS);

    return eRCode;
}

