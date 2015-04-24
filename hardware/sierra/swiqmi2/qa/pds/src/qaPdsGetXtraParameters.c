/*
 * \ingroup pds
 *
 * \file    qaPdsGetXtraParameters.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_PDS_GET_XTRA_PARAMETERS message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaPdsGetXtraParameters.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * Packs the GetXtraParameters parameters to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError PkQmiPdsGetXtraParameters(
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
                      eQMI_PDS_GET_XTRA_PARAMS,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * Unpacks the GetXtraParameters enabled and interval fields from the
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
enum eQCWWANError UnpackTlvXtraDbAutoDwnld( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiPdsGetXtraParametersResp *lResp =
        (struct QmiPdsGetXtraParametersResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE              lTemp;

    if ( !lResp->pbEnabled ||
         !lResp->pInterval )
       return eRCode;

    /* Validate TLV parameter length field */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                           eTLV_GET_XTRA_DB_AUTODWNLD_LEN );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Auto Download state is ULONG * but its size as per document is 1 byte */
    eRCode = GetByte( pTlvData, &lTemp );
    if ( eQCWWAN_ERR_NONE == eRCode )
    {
        *(lResp->pbEnabled) = lTemp;
        /* Interval between Fix requests */
        eRCode = GetWord( pTlvData, lResp->pInterval );
    }

    return eRCode;
}

/*
 * Unpacks the GetXtraParameters preference fields from the
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
enum eQCWWANError UnpackTlvXtraDbNetworkConfig( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiPdsGetXtraParametersResp *lResp =
        (struct QmiPdsGetXtraParametersResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE              lTemp;

    if ( !lResp->pPreference )
       return eRCode;

    /* Validate TLV parameter length field */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                          eTLV_GET_XTRA_DB_NETWORK_CONFIG_LEN );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Network Preference is ULONG * but its size as per document is 1 byte */
    eRCode = GetByte( pTlvData, &lTemp );
    if ( eQCWWAN_ERR_NONE == eRCode )
        *(lResp->pPreference) = lTemp;

    return eRCode;
}

/*
 * This function unpacks the GetXtraParameters preference fields
 * from the QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvXtraDbValidity( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiPdsGetXtraParametersResp *lResp =
        (struct QmiPdsGetXtraParametersResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !lResp->pGPSWeek ||
         !lResp->pGPSWeekOffset ||
         !lResp->pDuration )
         return eRCode;

    /* Validate TLV parameter length field */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                          eTLV_GET_XTRA_DB_VALIDITY_LEN );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    eRCode = GetWord( pTlvData, lResp->pGPSWeek );
    if ( eQCWWAN_ERR_NONE == eRCode )
    {
        eRCode = GetWord( pTlvData, lResp->pGPSWeekOffset );
        if ( eQCWWAN_ERR_NONE == eRCode )
            eRCode = GetWord( pTlvData, lResp->pDuration );
    }

    return eRCode;
}

/*
 * Unpacks the GetXtraParameters response message to user-provided
 * response structure.
 *
 * \param  pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiPdsGetXtraParameters(
    BYTE                               *pMdmResp,
    struct QmiPdsGetXtraParametersResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,                &qmUnpackTlvResultCode },
        { eTLV_GET_XTRA_DB_AUTODWNLD,      &UnpackTlvXtraDbAutoDwnld },
        { eTLV_GET_XTRA_DB_NETWORK_CONFIG, &UnpackTlvXtraDbNetworkConfig },
        { eTLV_GET_XTRA_DB_VALIDITY,       &UnpackTlvXtraDbValidity },
        { eTLV_TYPE_INVALID,               NULL } /* Important. Sentinel.
                                                   * Signifies last item in map.
                                                   */
    };
    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_PDS_GET_XTRA_PARAMS );
    return eRCode;
}
