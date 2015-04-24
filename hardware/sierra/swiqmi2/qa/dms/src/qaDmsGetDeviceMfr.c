/*
 * \ingroup dms
 *
 * \file qaDmsGetDeviceMfr.c
 *
 * \brief  Contains Packing and UnPacking routines for the 
 *  QMI_DMS_GET_DEVICE_MFR message.
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

#include "qaDmsGetDeviceMfr.h"

/*****************************************************************************
 * Local Variables
 ******************************************************************************/

/*****************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the Get Device Manufacturer parameters
 * to the QMI message SDU
 *
 * \param       pParamField [OUT] - Pointer to storage into which the packed
 *                                  data will be placed by this function.
 *
 * \param       pMlength    [OUT] - Total length of built message.
 *
 * \return      eQCWWAN_ERR_NONE   - Increment successful
 * \return      eQCWWAN_ERR_MEMORY - Increment beyond allowed size attempted
 *
 */
enum eQCWWANError PkQmiDmsGetDeviceMfr ( 
    WORD    *pMlength,
    BYTE    *pParamField )
{
    /* There is no mandatory TLV and hence the map is empty */ 
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
                       eQMI_DMS_GET_MANUFACTURER,
                       pMlength );

    return eRCode;
}


/*****************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the Get Model ID from the
 * QMI response message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, upon success
 */
enum eQCWWANError UnpackTlvGetDeviceMfr (
    BYTE* pTlvData,
    BYTE *pResp )
{ 
    struct QmiDmsGetDeviceMfrResp *lResp =
              (struct QmiDmsGetDeviceMfrResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( lResp->stringSize == 0 )
        return eRCode;

    /* Extract the String Parameter.
     * The size of the string is stored to check for buffer size in API
     */
    eRCode = qmQmiExtractString( pTlvData,
                                 lResp->pString,
                                 lResp->stringSize );
    return eRCode;
}


/*
 * This function unpacks the Get Device Mfr response message to a
 * user-provided response structure.
 *
 * \param    pMdmResp - Pointer to packed response from the modem.
 *
 * \param    pApiResp - Pointer to storage to unpack into.
 *
 * \return   eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UpkQmiDmsGetDeviceMfr(
    BYTE    *pMdmResp,
    struct  QmiDmsGetDeviceMfrResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,             &qmUnpackTlvResultCode },
        { eTLV_DEVICE_MANUFACTURER ,    &UnpackTlvGetDeviceMfr },
        { eTLV_TYPE_INVALID,            NULL }  /* Important. Sentinel. 
                                                 * Signifies last item in map.
                                                 */
    };

    eRCode = qmunpackresp( pMdmResp,
                          (BYTE*)pApiResp,
                          map,
                          eQMI_DMS_GET_MANUFACTURER );
    return eRCode;
}
