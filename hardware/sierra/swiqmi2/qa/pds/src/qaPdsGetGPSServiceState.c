/*
 * \ingroup pds
 *
 * \file qaPdsGetGPSServiceState.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        QMI_PDS_GET_SERVICE_STATE message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaPdsGetGPSServiceState.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the GetServiceState parameters to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError PkQmiPdsGetServiceState(
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
                      eQMI_PDS_GET_STATE,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the GetServiceState GPS service state and
 * Tracking session state from the QMI response message to a user
 * provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvServiceState( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiPdsGetServiceStateResp *lResp =
        (struct QmiPdsGetServiceStateResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( lResp->pEnabledStatus  == NULL ||
         lResp->pTrackingStatus == NULL )
         return eRCode;

    /* Validate TLV parameter length field */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                           eTLV_GET_GPS_SERVICE_STATE_LENGTH );
    if (eRCode != eQCWWAN_ERR_NONE)
        return eRCode;

    BYTE lTemp;
    /* pEnabledStatus is a ULONG * but its size as per document is 1 byte */
    eRCode = GetByte( pTlvData, &lTemp );
    if ( eQCWWAN_ERR_NONE == eRCode )
    {
        *(lResp->pEnabledStatus) = lTemp;
        /* pTrackingStatus is a ULONG * but its size as per document is 1 byte */
        eRCode = GetByte( pTlvData, &lTemp );
        if ( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;
        *(lResp->pTrackingStatus) = lTemp;
    }
    return eRCode;
}

/*
 * This function unpacks the GetServiceState response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiPdsGetServiceState(
    BYTE                             *pMdmResp,
    struct QmiPdsGetServiceStateResp *pApiResp )
{
    ULONG eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,           &qmUnpackTlvResultCode },
        { eTLV_GET_GPS_SERVICE_STATE, &UnpackTlvServiceState },
        { eTLV_TYPE_INVALID,          NULL } /* Important. Sentinel.
                                              * Signifies last item in map.
                                              */
    };
    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_PDS_GET_STATE );
    return eRCode;
}
