/*
 *
 * \ingroup : dms 
 *
 * \file    : qaDmsGetDeviceHardwareRev.c
 *
 * \brief   : Contains Packing and UnPacking routines for the 
 *            QMI_DMS_GET_DEVICE_HARDWARE_REV message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaQmiBasic.h"

#include "qaDmsGetDeviceHardwareRev.h"


/*****************************************************************************
 * Request handling
 ******************************************************************************/

/*
 *
 * This function packs the Hardware Revision parameters to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE     - Increment successful
 * \return eQCWWAN_ERR_MEMORY   - Increment beyond allowed size attempted
 *
 */
enum eQCWWANError PkQmiDmsGetDeviceHardwareRev( 
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

    eRCode = qmbuild ( pParamField,
                       NULL,
                       map,
                       eQMI_DMS_GET_HARDWARE_REV,
                       pMlength );

    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/

/*
 *
 * This function unpacks the hardware revision from the DEVICE_HARDWARE_REV
 * response message to a user provided response structure.
 *
 * \param      pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param      pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return     eQCWWAN_ERR_NONE
 *
 */
enum eQCWWANError UnpackTlvDeviceHardwareRevision(BYTE* pTlvData, BYTE *pResp)
{
    struct QmiDmsGetDeviceHardwareRevResp *lResp =
        (struct QmiDmsGetDeviceHardwareRevResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( lResp->stringSize == 0 )
        return eQCWWAN_ERR_NONE;

    /* Extract the String Parameter */
    /* The size of the string is stored to check for buffer size in API */
    eRCode = qmQmiExtractString ( pTlvData,
                                  lResp->pString,
                                  lResp->stringSize );
    return eRCode;
}

/*
 *
 * This function unpacks the GET_DEVICE_HARDWARE_REV response 
 * message to a user-provided response structure.
 *
 * \param       MdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param       pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return      eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UpkQmiDmsGetDeviceHardwareRev(
    BYTE    *pMdmResp,
    struct  QmiDmsGetDeviceHardwareRevResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE       ,&qmUnpackTlvResultCode },
        { eTLV_HARQWARE_REV      ,&UnpackTlvDeviceHardwareRevision },
        { eTLV_TYPE_INVALID      ,NULL }  /* Important. Sentinel. 
                                           * Signifies last item in map.
                                           */
    };

    eRCode = \
        qmunpackresp( pMdmResp,
                      (BYTE*)pApiResp,
                      map,
                      eQMI_DMS_GET_HARDWARE_REV);

    return eRCode;
}

