/*
 * \ingroup dms
 *
 * \file qaDmsGetOperatingMode.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        QMI_DMS_GET_OPERATING_MODE message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaQmiBasic.h"

#include "qaDmsGetOperatingMode.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the GetPower parameters to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError PkQmiDmsGetOperatingMode(
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
                      eQMI_DMS_GET_OPERATING_MODE,
                      pMlength );

    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the GetOperatingMode OperatingMode from the QMI
 * response message to a user provided response structure
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
enum eQCWWANError UnpackTlvOperatingMode( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiDmsGetOperatingModeResp *lResp =
        (struct QmiDmsGetOperatingModeResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    BYTE lByte = 0;

    if ( !lResp->pOperatingMode )
        return eRCode;

    eRCode = GetByte( pTlvData, &lByte );
    if ( eRCode == eQCWWAN_ERR_NONE )
        *(lResp->pOperatingMode) = lByte;

    return eRCode;
}

/*
 * This function unpacks the GetOperatingMode OfflineReason from the QMI
 * response message to a user provided response structure
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
enum eQCWWANError UnpackTlvOfflineReason( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiDmsGetOperatingModeResp *lResp =
        (struct QmiDmsGetOperatingModeResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    WORD lWord;

    if ( !lResp->pOfflineReason )
        return eRCode;

    eRCode = GetWord( pTlvData, &lWord );
    if ( eRCode == eQCWWAN_ERR_NONE )
        *(lResp->pOfflineReason) = lWord;

    return eRCode;
}

/*
 * This function unpacks the GetOperatingMode HardwareControlledMode from the
 * QMI response message to a user provided response structure
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
enum eQCWWANError UnpackTlvHardwareControlledMode( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiDmsGetOperatingModeResp *lResp =
        (struct QmiDmsGetOperatingModeResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    BYTE lByte = 0; 

    if ( !lResp->pHardwareControlledMode )
        return eRCode;

    eRCode = GetByte( pTlvData, &lByte );
    if ( eRCode == eQCWWAN_ERR_NONE )
        *(lResp->pHardwareControlledMode) = lByte;

    return eRCode;
}

/*
 * This function unpacks the GetOperatingMode response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError UpkQmiDmsGetOperatingMode(
    BYTE                              *pMdmResp,
    struct QmiDmsGetOperatingModeResp *pApiResp )
{
    ULONG eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,         &qmUnpackTlvResultCode },
        { eTLV_GET_OPERATING_MODE,  &UnpackTlvOperatingMode },
        { eTLV_GET_OFFLINE_REASON,  &UnpackTlvOfflineReason },
        { eTLV_GET_HW_CONTROL_MODE, &UnpackTlvHardwareControlledMode },
        { eTLV_TYPE_INVALID,        NULL } /* Important. Sentinel.
                                            * Signifies last item in map.
                                            */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_DMS_GET_OPERATING_MODE );
    return eRCode;
}
