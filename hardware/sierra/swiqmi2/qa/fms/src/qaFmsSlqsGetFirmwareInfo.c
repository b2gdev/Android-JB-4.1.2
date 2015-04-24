/*
 * \ingroup fms 
 *
 * \file    qaFmsSlqsGetFirmwareInfo.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_FMS_SLQS_GET_FIRMWARE_INFO message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaGobiApiFms.h"
#include "qaFmsSlqsGetFirmwareInfo.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the SLQSGetFirmwareInfo parameters to the QMI message SDU
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError PkQmiFmsSlqsGetFirmwareInfo(
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
                      eQMI_DMS_GET_CWE_SPKGS_INFO,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the SLQSGetFirmwareInfoS Model Id from the
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
enum eQCWWANError UnpackTlvModelId( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiFmsSlqsGetFirmwareInfoResp *lResp =
        (struct QmiFmsSlqsGetFirmwareInfoResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !lResp->pinfo )
         return eRCode;

    slmemset( lResp->pinfo->modelid_str, 0, SLQSFWINFO_MODELID_SZ );
    eRCode = qmQmiExtractString( pTlvData,
                                 lResp->pinfo->modelid_str,
                                 SLQSFWINFO_MODELID_SZ );
 
    return eRCode;
}

/*
 * This function unpacks the SLQSGetFirmwareInfo Boot Version from the
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
enum eQCWWANError UnpackTlvBootVersion( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiFmsSlqsGetFirmwareInfoResp *lResp =
        (struct QmiFmsSlqsGetFirmwareInfoResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !lResp->pinfo )
         return eRCode;

    slmemset( lResp->pinfo->bootversion_str, 0, SLQSFWINFO_BOOTVERSION_SZ );
    eRCode = qmQmiExtractString( pTlvData,
                                 lResp->pinfo->bootversion_str,
                                 SLQSFWINFO_BOOTVERSION_SZ );

    return eRCode;
}

/*
 * This function unpacks the SLQSGetFirmwareInfo Application Version from
 * the QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvApplicationVersion( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiFmsSlqsGetFirmwareInfoResp *lResp =
        (struct QmiFmsSlqsGetFirmwareInfoResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !lResp->pinfo )
         return eRCode;

    slmemset( lResp->pinfo->appversion_str, 0, SLQSFWINFO_APPVERSION_SZ);
    eRCode = qmQmiExtractString( pTlvData,
                                 lResp->pinfo->appversion_str,
                                 SLQSFWINFO_APPVERSION_SZ );

    return eRCode;
}

/*
 * This function unpacks the SLQSGetFirmwareInfo SKU from the
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
enum eQCWWANError UnpackTlvSKU( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiFmsSlqsGetFirmwareInfoResp *lResp =
        (struct QmiFmsSlqsGetFirmwareInfoResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !lResp->pinfo )
         return eRCode;

    slmemset( lResp->pinfo->sku_str, 0, SLQSFWINFO_SKU_SZ);
    eRCode = qmQmiExtractString( pTlvData,
                                 lResp->pinfo->sku_str,
                                 SLQSFWINFO_SKU_SZ );

    return eRCode;
}

/*
 * This function unpacks the SLQSGetFirmwareInfo Package from the
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
enum eQCWWANError UnpackTlvPackage( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiFmsSlqsGetFirmwareInfoResp *lResp =
        (struct QmiFmsSlqsGetFirmwareInfoResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !lResp->pinfo )
         return eRCode;

    slmemset( lResp->pinfo->packageid_str, 0, SLQSFWINFO_PACKAGEID_SZ );
    eRCode = qmQmiExtractString( pTlvData,
                                 lResp->pinfo->packageid_str,
                                 SLQSFWINFO_PACKAGEID_SZ );

    return eRCode;
}

/*
 * This function unpacks the SLQSGetFirmwareInfo Carrier from the
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
enum eQCWWANError UnpackTlvCarrier( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiFmsSlqsGetFirmwareInfoResp *lResp =
        (struct QmiFmsSlqsGetFirmwareInfoResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !lResp->pinfo )
         return eRCode;

    slmemset( lResp->pinfo->carrier_str, 0, SLQSFWINFO_CARRIER_SZ );
    eRCode = qmQmiExtractString( pTlvData,
                                 lResp->pinfo->carrier_str,
                                 SLQSFWINFO_CARRIER_SZ );

    return eRCode;
}

/*
 * This function unpacks the SLQSGetFirmwareInfo PRI from the
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
enum eQCWWANError UnpackTlvPRI( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiFmsSlqsGetFirmwareInfoResp *lResp =
        (struct QmiFmsSlqsGetFirmwareInfoResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !lResp->pinfo )
         return eRCode;

    slmemset( lResp->pinfo->priversion_str, 0, SLQSFWINFO_PRIVERSION_SZ );
    eRCode = qmQmiExtractString( pTlvData,
                                 lResp->pinfo->priversion_str,
                                 SLQSFWINFO_PRIVERSION_SZ );

    return eRCode;
}

/*
 * This function unpacks the SLQSGetFirmwareInfo response message to
 * a user-provided response structure.
 *
 * \param  pMdmResp     [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp     [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiFmsSlqsGetFirmwareInfo(
    BYTE                                 *pMdmResp,
    struct QmiFmsSlqsGetFirmwareInfoResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,         &qmUnpackTlvResultCode },
        { eTLV_MODEL_ID,            &UnpackTlvModelId },
        { eTLV_BOOT_VERSION,        &UnpackTlvBootVersion },
        { eTLV_APPLICATION_VERSION, &UnpackTlvApplicationVersion },
        { eTLV_SKU,                 &UnpackTlvSKU },
        { eTLV_PACKAGE,             &UnpackTlvPackage },
        { eTLV_CARRIER,             &UnpackTlvCarrier },
        { eTLV_PRI,                 &UnpackTlvPRI },
        { eTLV_TYPE_INVALID,        NULL } /* Important. Sentinel.
                                            * Signifies last item in map.
                                            */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_DMS_GET_CWE_SPKGS_INFO );
    return eRCode;
}
