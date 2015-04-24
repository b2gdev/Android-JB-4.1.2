/*
 * \ingroup pds
 *
 * \file    qaPdsGetAGPSConfig.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_PDS_GET_AGPS_CONFIG message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaPdsGetAGPSConfig.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the GetAGPSConfig parameters to the QMI message SDU
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError PkQmiPdsGetAGPSConfig(
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

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      NULL,
                      map,
                      eQMI_PDS_GET_AGPS_CONFIG,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the GetAGPSConfig location server from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.

 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise

 *
 */
enum eQCWWANError UnpackTlvLocationServer( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiPdsGetAGPSConfigResp *lResp =
        (struct QmiPdsGetAGPSConfigResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !lResp->pServerAddress || !lResp->pServerPort )
         return eRCode;

    /* Validate TLV parameter length field */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                           eTLV_AGPS_LOCATION_SERVER_LENGTH );

    if ( eQCWWAN_ERR_NONE == eRCode )
    {
        eRCode = GetLong( pTlvData, lResp->pServerAddress );
        if ( eQCWWAN_ERR_NONE == eRCode)
            eRCode = GetLong( pTlvData, lResp->pServerPort );
    } 
    return eRCode;
}

/*
 * This function unpacks the GetAGPSConfig location server URL from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.

 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise

 *
 */
enum eQCWWANError UnpackTlvLocationServerUrl( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiPdsGetAGPSConfigResp *lResp =
        (struct QmiPdsGetAGPSConfigResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE lByte = 0;

    if ( !lResp->pServerURL || !lResp->pServerURLLength )
         return eRCode;

    /* obtain the URL length */
    eRCode = GetByte( pTlvData, &lByte );
    if( *(lResp->pServerURLLength) < lByte )
        return eQCWWAN_ERR_BUFFER_SZ;

    /* update the URL length for the user */
    *(lResp->pServerURLLength) = lByte;

    while( lByte-- )    
    {
        if( eQCWWAN_ERR_NONE != eRCode )
            break;
        eRCode = GetByte( pTlvData, lResp->pServerURL++ );
    }
    return eRCode;
}

/*
 * This function unpacks the GetAGPSConfig response message to a user-provided
 * response structure.
 *
 * \param  pMdmResp     [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp     [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiPdsGetAGPSConfig(
    BYTE                           *pMdmResp,
    struct QmiPdsGetAGPSConfigResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,                  &qmUnpackTlvResultCode },
        { eTLV_GET_AGPS_LOCATION_SERVER,     &UnpackTlvLocationServer },
        { eTLV_GET_AGPS_LOCATION_SERVER_URL, &UnpackTlvLocationServerUrl },
        { eTLV_TYPE_INVALID,                 NULL }/* Important. Sentinel.
                                                    * Signifies last item in map
                                                    */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_PDS_GET_AGPS_CONFIG );
    return eRCode;
}
