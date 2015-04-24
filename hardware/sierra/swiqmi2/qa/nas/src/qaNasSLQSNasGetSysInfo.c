/*
 * \ingroup nas
 *
 * \file    qaNasSLQSNasGetSysInfo.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_VOICE_GET_SYS_INFO message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaNasSLQSNasGetSysInfo.h"
#include "qaGobiApiNasCommon.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the GetSysInfo parameters to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError PkQmiNasSlqsGetSysInfo(
    WORD *pMlength,
    BYTE *pParamField )
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
                      eQMI_NAS_GET_SYS_INFO,
                      pMlength);
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the CDMA Service Status Information from the QMI
 * response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvCDMASSInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    SrvStatusInfo *lResp = ((struct QmiNasSLQSGetSysInfoResp *)
                                    pResp)->pSysInfoResp->pCDMASrvStatusInfo;

    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the CDMA Service Status Info */
    return UnpackTlvCommonSrvStatusInfo( pTlvData, lResp );
}

/*
 * This function unpacks the HDR Service Status Information from the QMI
 * response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvHDRSSInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    SrvStatusInfo *lResp = ((struct QmiNasSLQSGetSysInfoResp *)
                                    pResp)->pSysInfoResp->pHDRSrvStatusInfo;

    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the HDR Service Status Info */
    return UnpackTlvCommonSrvStatusInfo( pTlvData, lResp );
}

/*
 * This function unpacks the GSM Service Status Information from the QMI
 * response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvGSMSSInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    GSMSrvStatusInfo *lResp = ((struct QmiNasSLQSGetSysInfoResp *)
                                    pResp)->pSysInfoResp->pGSMSrvStatusInfo;

    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the GSM Service Status Info */
    return UnpackTlvCommonGSMSrvStatusInfo( pTlvData, lResp );
}

/*
 * This function unpacks the WCDMA Service Status Information from the QMI
 * response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvWCDMASSInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    GSMSrvStatusInfo *lResp = ((struct QmiNasSLQSGetSysInfoResp *)
                                    pResp)->pSysInfoResp->pWCDMASrvStatusInfo;

    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the WCDMA Service Status Info */
    return UnpackTlvCommonGSMSrvStatusInfo( pTlvData, lResp );
}

/*
 * This function unpacks the LTE Service Status Information from the QMI
 * response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvLTESSInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    GSMSrvStatusInfo *lResp = ((struct QmiNasSLQSGetSysInfoResp *)
                                    pResp)->pSysInfoResp->pLTESrvStatusInfo;

    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the LTE Service Status Info */
    return UnpackTlvCommonGSMSrvStatusInfo( pTlvData, lResp );
}

/*
 * This function unpacks the CDMA System Information from the QMI response
 * message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvCDMASysInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    CDMASysInfo *lResp = ((struct QmiNasSLQSGetSysInfoResp *)
                                    pResp)->pSysInfoResp->pCDMASysInfo;

    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the CDMA System Info */
    return UnpackTlvCommonCDMASysInfo( pTlvData, lResp );
}

/*
 * This function unpacks the HDR System Information from the QMI response
 * message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvHDRSysInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    HDRSysInfo *lResp = ((struct QmiNasSLQSGetSysInfoResp *)
                                    pResp)->pSysInfoResp->pHDRSysInfo;

    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the HDR System Info */
    return UnpackTlvCommonHDRSysInfo( pTlvData, lResp );
}

/*
 * This function unpacks the GSM System Information from the QMI response
 * message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvGSMSysInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    GSMSysInfo *lResp = ((struct QmiNasSLQSGetSysInfoResp *)
                                    pResp)->pSysInfoResp->pGSMSysInfo;

    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the GSM System Info */
    return UnpackTlvCommonGSMSysInfo( pTlvData, lResp );
}

/*
 * This function unpacks the WCDMA System Information from the QMI response
 * message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvWCDMASysInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    WCDMASysInfo *lResp = ((struct QmiNasSLQSGetSysInfoResp *)
                                    pResp)->pSysInfoResp->pWCDMASysInfo;

    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the WCDMA System Info */
    return UnpackTlvCommonWCDMASysInfo( pTlvData, lResp );
}

/*
 * This function unpacks the LTE System Information from the QMI response
 * message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvLTESysInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    LTESysInfo *lResp = ((struct QmiNasSLQSGetSysInfoResp *)
                                    pResp)->pSysInfoResp->pLTESysInfo;

    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the LTE System Info */
    return UnpackTlvCommonLTESysInfo( pTlvData, lResp );
}

/*
 * This function unpacks the Additional CDMA System Information from the QMI
 * response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvAddCDMASysInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    AddCDMASysInfo *lResp = ((struct QmiNasSLQSGetSysInfoResp *)
                                    pResp)->pSysInfoResp->pAddCDMASysInfo;

    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the Additional CDMA System Info */
    return UnpackTlvCommonAddCDMASysInfo( pTlvData, lResp );
}

/*
 * This function unpacks the Additional HDR System Information from the QMI
 * response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvAddHDRSysInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    nasGetSysInfoResp *lResp = ((struct QmiNasSLQSGetSysInfoResp *)
                                                    pResp)->pSysInfoResp;

    if ( NULL == lResp->pAddHDRSysInfo )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the Additional HDR System Info */
    return GetWord( pTlvData, lResp->pAddHDRSysInfo );
}

/*
 * This function unpacks the Additional GSM System Information from the QMI
 * response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvAddGSMSysInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    AddSysInfo *lResp = ((struct QmiNasSLQSGetSysInfoResp *)
                                    pResp)->pSysInfoResp->pAddGSMSysInfo;

    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the Additional GSM System Info */
    return UnpackTlvCommonAddSysInfo( pTlvData, lResp );
}

/*
 * This function unpacks the Additional WCDMA System Information from the QMI
 * response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvAddWCDMASysInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    AddSysInfo *lResp = ((struct QmiNasSLQSGetSysInfoResp *)
                                    pResp)->pSysInfoResp->pAddWCDMASysInfo;

    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the Additional WCDMA System Info */
    return UnpackTlvCommonAddSysInfo( pTlvData, lResp );
}

/*
 * This function unpacks the Additional LTE System Information from the QMI
 * response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvAddLTESysInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    nasGetSysInfoResp *lResp = ((struct QmiNasSLQSGetSysInfoResp *)
                                                  pResp)->pSysInfoResp;

    if ( NULL == lResp->pAddLTESysInfo )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the Additional LTE System Info */
    return GetWord( pTlvData, lResp->pAddLTESysInfo );
}

/*
 * This function unpacks the GSM Call Barring System Information from the QMI
 * response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvGSMCallBarringSysInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    CallBarringSysInfo *lResp = ((struct QmiNasSLQSGetSysInfoResp *)
                                  pResp)->pSysInfoResp->pGSMCallBarringSysInfo;

    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the GSM Call Barring System Info */
    return UnpackTlvCommonCallBarringSysInfo( pTlvData, lResp );
}

/*
 * This function unpacks the WCDMA Call Barring System Information from the QMI
 * response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvWCDMACallBarringSysInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    CallBarringSysInfo *lResp = ((struct QmiNasSLQSGetSysInfoResp *)
                                pResp)->pSysInfoResp->pWCDMACallBarringSysInfo;

    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the WCDMA Call Barring System Info */
    return UnpackTlvCommonCallBarringSysInfo( pTlvData, lResp );
}

/*
 * This function unpacks the LTE Voice Support System Information from the QMI
 * response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvLTEVoiceSupportSysInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    nasGetSysInfoResp *lResp = ((struct QmiNasSLQSGetSysInfoResp *)
                                                  pResp)->pSysInfoResp;

    if ( NULL == lResp->pLTEVoiceSupportSysInfo )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the LTE Voice Support System Info */
    return GetByte( pTlvData, lResp->pLTEVoiceSupportSysInfo );
}

/*
 * This function unpacks the GSM Cipher Domain System Information from the QMI
 * response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvGSMCipherDomainSysInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    nasGetSysInfoResp *lResp = ((struct QmiNasSLQSGetSysInfoResp *)
                                                  pResp)->pSysInfoResp;

    if ( NULL == lResp->pGSMCipherDomainSysInfo )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the GSM Cipher Domain System Info */
    return GetByte( pTlvData, lResp->pGSMCipherDomainSysInfo );
}

/*
 * This function unpacks the WCDMA Cipher Domain System Information from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvWCDMACipherDomainSysInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    nasGetSysInfoResp *lResp = ((struct QmiNasSLQSGetSysInfoResp *)
                                                  pResp)->pSysInfoResp;

    if ( NULL == lResp->pWCDMACipherDomainSysInfo )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the Additional LTE System Info */
    return GetByte( pTlvData, lResp->pWCDMACipherDomainSysInfo );
}

/*
 * This function unpacks the SLQSGetSysInfo response message to to a
 * user-provided response structure.
 *
 * \param     pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp  [OUT]  - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiNasSlqsGetSysInfo(
    BYTE                            *pMdmResp,
    struct QmiNasSLQSGetSysInfoResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,        &qmUnpackTlvResultCode },
        { eTLV_CDMA_SS_INFO,       &UnpackTlvCDMASSInfo },
        { eTLV_HDR_SS_INFO,        &UnpackTlvHDRSSInfo },
        { eTLV_GSM_SS_INFO,        &UnpackTlvGSMSSInfo },
        { eTLV_WCDMA_SS_INFO,      &UnpackTlvWCDMASSInfo },
        { eTLV_LTE_SS_INFO,        &UnpackTlvLTESSInfo },
        { eTLV_CDMA_SYS_INFO,      &UnpackTlvCDMASysInfo },
        { eTLV_HDR_SYS_INFO,       &UnpackTlvHDRSysInfo },
        { eTLV_GSM_SYS_INFO,       &UnpackTlvGSMSysInfo },
        { eTLV_WCDMA_SYS_INFO,     &UnpackTlvWCDMASysInfo },
        { eTLV_LTE_SYS_INFO,       &UnpackTlvLTESysInfo },
        { eTLV_ADD_CDMA_SYS_INFO,  &UnpackTlvAddCDMASysInfo },
        { eTLV_ADD_HDR_SYS_INFO,   &UnpackTlvAddHDRSysInfo },
        { eTLV_ADD_GSM_SYS_INFO,   &UnpackTlvAddGSMSysInfo },
        { eTLV_ADD_WCDMA_SYS_INFO, &UnpackTlvAddWCDMASysInfo },
        { eTLV_ADD_LTE_SYS_INFO,   &UnpackTlvAddLTESysInfo },
        { eTLV_GSM_CB_SYS_INFO,    &UnpackTlvGSMCallBarringSysInfo },
        { eTLV_WCDMA_CB_SYS_INFO,  &UnpackTlvWCDMACallBarringSysInfo },
        { eTLV_LTE_VS_SYS_INFO,    &UnpackTlvLTEVoiceSupportSysInfo },
        { eTLV_GSM_CD_SYS_INFO,    &UnpackTlvGSMCipherDomainSysInfo },
        { eTLV_WCDMA_CD_SYS_INFO,  &UnpackTlvWCDMACipherDomainSysInfo },
        { eTLV_TYPE_INVALID,       NULL }  /* Important. Sentinel.
                                            * Signifies last item in map.
                                            */
    };
    eRCode = qmunpackresp( pMdmResp,
                           (BYTE*)pApiResp,
                           map,
                           eQMI_NAS_GET_SYS_INFO );
    return eRCode;
}
