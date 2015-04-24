/*
 * \ingroup dms
 *
 * \file qaDmsGetTime.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        QMI_DMS_GET_TIME message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaQmiBasic.h"

#include "qaDmsGetTime.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the GetNetworkTime parameters to the QMI message SDU
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
enum eQCWWANError PkQmiDmsGetTime(
    WORD *pMlength,
    BYTE *pParamField )
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
                      eQMI_DMS_GET_TIME,
                      pMlength );

    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the GetNetworkTime Time from the QMI response
 * message to a user provided response structure
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
enum eQCWWANError UnpackTlvDeviceTime( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiDmsGetTimeResp *lResp =
        (struct QmiDmsGetTimeResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !lResp->pDeviceTime )
        return eRCode;

    eRCode = GetLongLong( pTlvData, lResp->pDeviceTime );

    return eRCode;
}

/*
 * This function unpacks the GetNetworkTime response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError UpkQmiDmsGetTime(
    BYTE                     *pMdmResp,
    struct QmiDmsGetTimeResp *pApiResp )
{
    ULONG eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_DEVICE_TIME,  &UnpackTlvDeviceTime },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_DMS_GET_TIME );

    return eRCode;
}

