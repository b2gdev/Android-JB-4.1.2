/*
 * \ingroup dms
 *
 * \file qaDmsGetDeviceRevisionID.c
 *
 * \brief  Contains Packing and UnPacking routines for the 
 *   QMI_DMS_GET_DEVICE_REVISION_ID message.
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

#include "qaDmsGetDeviceRevisionID.h"

/*****************************************************************************
 * Local Variables
 ******************************************************************************/

/*****************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the Get Revision ID parameters
 * to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Increment beyond allowed size attempted
 *
 * \sa     qaGobiApiDms.h for remaining parameter descriptions.
 *
 */
enum eQCWWANError PkQmiDmsGetDeviceRevID (
    USHORT   *pParamLength,
    BYTE     *pParamField )
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
                      eQMI_DMS_GET_REV_ID,
                      pParamLength);
    return eRCode;
}


/*****************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the Get Revision ID from the
 * QMI response message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success
 */
enum eQCWWANError UnpackTlvGetDeviceRevisionID(
    BYTE    *pTlvData,
    BYTE    *pResp )
{ 
    struct QmiDmsGetDeviceRevIDResp *lResp =
              (struct QmiDmsGetDeviceRevIDResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( lResp->amssSize == 0 )
        return eQCWWAN_ERR_NONE;

    /* Extract the String Parameter */
    /* The size of the string is stored to check for buffer size in API */
    eRCode = qmQmiExtractString( pTlvData,
                                 lResp->pAMSSString,
                                 lResp->amssSize );
    return eRCode;
}


/*
 * This function unpacks the Get Boot Code ID from the
 * QMI response message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success
 */
enum eQCWWANError UnpackTlvGetDeviceBootCoderevision(
    BYTE    *pTlvData,
    BYTE    *pResp )
{ 
    struct QmiDmsGetDeviceRevIDResp *lResp =
              (struct QmiDmsGetDeviceRevIDResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( lResp->bootSize == 0 )
        return eQCWWAN_ERR_NONE;

    /* Extract the String Parameter */
    /* The size of the string is stored to check for buffer size in API */
    eRCode = qmQmiExtractString( pTlvData,
                                 lResp->pBootString,
                                 lResp->bootSize );
    return eRCode;
}

/*
 * This function unpacks the Get PRI Revision from the
 * QMI response message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvGetDevicePRIRevision(
    BYTE    *pTlvData,
    BYTE    *pResp )
{ 
    struct QmiDmsGetDeviceRevIDResp *lResp =
              (struct QmiDmsGetDeviceRevIDResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( lResp->priSize == 0 )
        return eQCWWAN_ERR_NONE;

    /* Extract the String Parameter */
    /* The size of the string is stored to check for buffer size in API */
    eRCode = qmQmiExtractString( pTlvData,
                                 lResp->pPRIString,
                                 lResp->priSize );
    return eRCode;
}

/*
 * This function unpacks the Get Revision ID response message to to a
 * user-provided response structure.
 *
 * \param    pMdmResp - Pointer to packed response from the modem.
 *
 * \param    pApiResp - Pointer to storage to unpack into.
 *
 * \return   eQCWWAN_ERR_NONE, on success 
 */
enum eQCWWANError UpkQmiDmsGetDeviceRevID(
    BYTE    *pMdmResp,
    struct  QmiDmsGetDeviceRevIDResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,             &qmUnpackTlvResultCode },
        { eTLV_DEVICE_REVISION_ID,      &UnpackTlvGetDeviceRevisionID },
        { eTLV_DEVICE_BOOT_CODE,        &UnpackTlvGetDeviceBootCoderevision },
        { eTLV_DEVICE_PRI,              &UnpackTlvGetDevicePRIRevision },
        { eTLV_TYPE_INVALID,            NULL }  /* Important. Sentinel.
                                                 * Signifies last item in map.
                                                 */
    };

    eRCode = qmunpackresp( pMdmResp,
                          (BYTE*)pApiResp,
                          map,
                          eQMI_DMS_GET_REV_ID );
    return eRCode;
}
