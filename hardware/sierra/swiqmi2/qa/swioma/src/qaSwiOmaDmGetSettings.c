/*
 * \ingroup swioma
 *
 * \file    qaSwiOmaDmGetSettings.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_SWIOMA_DM_GET_SETTINGS message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaSwiOmaDmGetSettings.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the SLQSOMADMGetSettings parameters to the QMI message
 * SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError PkQmiSwiOmaDmGetSettings(
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
                      eQMI_SWIOMA_GET_SETTINGS,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the SLQSOMADMGetSettings OMA-DM Enabled from the
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
enum eQCWWANError UnpackTlvOmaDmEnabled( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiSwiOmaDmGetSettingsResp *tResp =
        (struct QmiSwiOmaDmGetSettingsResp *)pResp;

    SLQSOMADMSettings *lResp = tResp->pSLQSOMADMSettings;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !lResp->pOMADMEnabled )
         return eRCode;

    /* Validate TLV parameter length field */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                            eTLV_OMA_DM_ENABLED_LENGTH );

    if ( eQCWWAN_ERR_NONE == eRCode )
        eRCode = GetLong( pTlvData, lResp->pOMADMEnabled );

    return eRCode;
}

/*
 * This function unpacks the SLQSOMADMGetSettings FOTA Download from the
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
enum eQCWWANError UnpackTlvFOTADownload( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiSwiOmaDmGetSettingsResp *tResp =
        (struct QmiSwiOmaDmGetSettingsResp *)pResp;

    SLQSOMADMSettings *lResp = tResp->pSLQSOMADMSettings;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !lResp->pFOTAdownload )
         return eRCode;

    /* Validate TLV parameter length field */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                     eTLV_FOTA_AUTOMATIC_DOWNLOAD_LENGTH );

    if ( eQCWWAN_ERR_NONE == eRCode )
    {
        eRCode = GetByte( pTlvData, lResp->pFOTAdownload );
    }

    return eRCode;
}

/*
 * This function unpacks the SLQSOMADMGetSettings FOTA Update from the
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
enum eQCWWANError UnpackTlvFOTAUpdate( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiSwiOmaDmGetSettingsResp *tResp =
        (struct QmiSwiOmaDmGetSettingsResp *)pResp;

    SLQSOMADMSettings *lResp = tResp->pSLQSOMADMSettings;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !lResp->pFOTAUpdate )
         return eRCode;

    /* Validate TLV parameter length field */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                       eTLV_FOTA_AUTOMATIC_UPDATE_LENGTH );

    if ( eQCWWAN_ERR_NONE == eRCode )
    {
        eRCode = GetByte( pTlvData, lResp->pFOTAUpdate );
    }

    return eRCode;
}

/*
 * This function unpacks the SLQSOMADMGetSettings OMA Automatic UI Alert
 * response from the QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvOMAAutoSDM( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiSwiOmaDmGetSettingsResp *tResp =
        (struct QmiSwiOmaDmGetSettingsResp *)pResp;

    SLQSOMADMSettings *lResp = tResp->pSLQSOMADMSettings;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !lResp->pAutosdm )
         return eRCode;

    /* Validate TLV parameter length field */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                            eTLV_OMA_AUTOSDM_LENGTH );

    if ( eQCWWAN_ERR_NONE == eRCode )
    {
        eRCode = GetByte( pTlvData, lResp->pAutosdm );
    }

    return eRCode;
}

/*
 * This function unpacks the SLQSOMADMGetSettings response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp     [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp     [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiSwiOmaDmGetSettings(
    BYTE                              *pMdmResp,
    struct QmiSwiOmaDmGetSettingsResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,                 &qmUnpackTlvResultCode },
        { eTLV_GET_OMA_DM_ENABLED,          &UnpackTlvOmaDmEnabled },
        { eTLV_GET_FOTA_AUTOMATIC_DOWNLOAD, &UnpackTlvFOTADownload },
        { eTLV_GET_FOTA_AUTOMATIC_UPDATE,   &UnpackTlvFOTAUpdate },
        { eTLV_GET_OMA_AUTOSDM,             &UnpackTlvOMAAutoSDM },
        { eTLV_TYPE_INVALID,                NULL } /* Important. Sentinel.
                                                    * Signifies last item in map.
                                                    */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_SWIOMA_GET_SETTINGS );
    return eRCode;
}
