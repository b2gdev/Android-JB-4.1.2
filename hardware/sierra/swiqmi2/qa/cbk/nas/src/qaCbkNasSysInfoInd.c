/*
 * \ingroup nas
 *
 * \file    qaCbkNasSysInfoInd.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          eQMI_NAS_SYS_INFO_IND message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCbkNasSysInfoInd.h"
#include "qaGobiApiNasCommon.h"

/* Locals */
static SrvStatusInfo      cdmassinfo;
static SrvStatusInfo      hdrssinfo;
static GSMSrvStatusInfo   gsmssinfo;
static GSMSrvStatusInfo   wcdmassinfo;
static GSMSrvStatusInfo   ltessinfo;
static CDMASysInfo        cdmasysinfo;
static HDRSysInfo         hdrsysinfo;
static GSMSysInfo         gsmsysinfo;
static WCDMASysInfo       wcdmasysinfo;
static LTESysInfo         ltesysinfo;
static AddCDMASysInfo     addcdmasysinfo;
static WORD               addhdrsysinfo;
static AddSysInfo         addgsmsysinfo;
static AddSysInfo         addwcdmasysinfo;
static WORD               addltesysinfo;
static CallBarringSysInfo gsmcbsysinfo;
static CallBarringSysInfo wcdmacbsysinfo;
static BYTE               ltevssi;
static BYTE               gsmcdsi;
static BYTE               wcdmacdsi;
static BYTE               sysinfonochnge;

/* Functions */
/*
 * This function unpacks the CDMA Service Status Information from the QMI
 * indication message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvIndCDMASSInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    nasSysInfo *lResp = ( nasSysInfo *)pResp;

    /* Allocate memory to the pointer */
    lResp->pCDMASrvStatusInfo = &cdmassinfo;

    /* Extract the CDMA Service Status Info */
    return UnpackTlvCommonSrvStatusInfo( pTlvData, lResp->pCDMASrvStatusInfo );
}

/*
 * This function unpacks the HDR Service Status Information from the QMI
 * indication message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvIndHDRSSInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    nasSysInfo *lResp = ( nasSysInfo *)pResp;

    /* Allocate memory to pointer */
    lResp->pHDRSrvStatusInfo = &hdrssinfo;

    /* Extract the HDR Service Status Info */
    return UnpackTlvCommonSrvStatusInfo( pTlvData, lResp->pHDRSrvStatusInfo );
}

/*
 * This function unpacks the GSM Service Status Information from the QMI
 * indication message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvIndGSMSSInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    nasSysInfo *lResp = ( nasSysInfo *)pResp;

    /* Allocate memory to pointer */
    lResp->pGSMSrvStatusInfo = &gsmssinfo;

    /* Extract the GSM Service Status Info */
    return UnpackTlvCommonGSMSrvStatusInfo( pTlvData,
                                            lResp->pGSMSrvStatusInfo );
}

/*
 * This function unpacks the WCDMA Service Status Information from the QMI
 * indication message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvIndWCDMASSInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    nasSysInfo *lResp = ( nasSysInfo *)pResp;

    /* Allocate memory to pointer */
    lResp->pWCDMASrvStatusInfo = &wcdmassinfo;

    /* Extract the WCDMA Service Status Info */
    return UnpackTlvCommonGSMSrvStatusInfo( pTlvData,
                                            lResp->pWCDMASrvStatusInfo );
}

/*
 * This function unpacks the LTE Service Status Information from the QMI
 * indication message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvIndLTESSInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    nasSysInfo *lResp = ( nasSysInfo *)pResp;

    /* Allocate memory to pointer */
    lResp->pLTESrvStatusInfo = &ltessinfo;

    /* Extract the LTE Service Status Info */
    return UnpackTlvCommonGSMSrvStatusInfo( pTlvData,
                                            lResp->pLTESrvStatusInfo );
}

/*
 * This function unpacks the CDMA System Information from the QMI indication
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
enum eQCWWANError UnpackTlvIndCDMASysInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    nasSysInfo *lResp = ( nasSysInfo *)pResp;

    /* Allocate memory to pointer */
    lResp->pCDMASysInfo = &cdmasysinfo;

    /* Extract the CDMA System Info */
    return UnpackTlvCommonCDMASysInfo( pTlvData, lResp->pCDMASysInfo );
}

/*
 * This function unpacks the HDR System Information from the QMI indication
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
enum eQCWWANError UnpackTlvIndHDRSysInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    nasSysInfo *lResp = ( nasSysInfo *)pResp;

    /* Allocate memory to pointer */
    lResp->pHDRSysInfo = &hdrsysinfo;

    /* Extract the HDR System Info */
    return UnpackTlvCommonHDRSysInfo( pTlvData, lResp->pHDRSysInfo );
}

/*
 * This function unpacks the GSM System Information from the QMI indication
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
enum eQCWWANError UnpackTlvIndGSMSysInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    nasSysInfo *lResp = ( nasSysInfo *)pResp;

    /* Allocate memory to pointer */
    lResp->pGSMSysInfo = &gsmsysinfo;

    /* Extract the GSM System Info */
    return UnpackTlvCommonGSMSysInfo( pTlvData, lResp->pGSMSysInfo );
}

/*
 * This function unpacks the WCDMA System Information from the QMI indication
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
enum eQCWWANError UnpackTlvIndWCDMASysInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    nasSysInfo *lResp = ( nasSysInfo *)pResp;

    /* Allocate memory to pointer */
    lResp->pWCDMASysInfo = &wcdmasysinfo;

    /* Extract the WCDMA System Info */
    return UnpackTlvCommonWCDMASysInfo( pTlvData, lResp->pWCDMASysInfo );
}

/*
 * This function unpacks the LTE System Information from the QMI indication
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
enum eQCWWANError UnpackTlvIndLTESysInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    nasSysInfo *lResp = ( nasSysInfo *)pResp;

    /* Allocate memory to pointer */
    lResp->pLTESysInfo = &ltesysinfo;

    /* Extract the LTE System Info */
    return UnpackTlvCommonLTESysInfo( pTlvData, lResp->pLTESysInfo );
}

/*
 * This function unpacks the Additional CDMA System Information from the QMI
 * indication message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvIndAddCDMASysInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    nasSysInfo *lResp = ( nasSysInfo *)pResp;

    /* Allocate memory to pointer */
    lResp->pAddCDMASysInfo = &addcdmasysinfo;

    /* Extract the Additional CDMA System Info */
    return UnpackTlvCommonAddCDMASysInfo( pTlvData, lResp->pAddCDMASysInfo );
}

/*
 * This function unpacks the Additional HDR System Information from the QMI
 * indication message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvIndAddHDRSysInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    nasSysInfo *lResp = ( nasSysInfo *)pResp;

    /* Allocate memory to pointer */
    lResp->pAddHDRSysInfo = &addhdrsysinfo;

    /* Extract the Additional HDR System Info */
    return GetWord( pTlvData, lResp->pAddHDRSysInfo );
}

/*
 * This function unpacks the Additional GSM System Information from the QMI
 * indication message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvIndAddGSMSysInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    nasSysInfo *lResp = ( nasSysInfo *)pResp;

    /* Allocate memory to pointer */
    lResp->pAddGSMSysInfo = &addgsmsysinfo;

    /* Extract the Additional GSM System Info */
    return UnpackTlvCommonAddSysInfo( pTlvData, lResp->pAddGSMSysInfo );
}

/*
 * This function unpacks the Additional WCDMA System Information from the QMI
 * indication message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvIndAddWCDMASysInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    nasSysInfo *lResp = ( nasSysInfo *)pResp;

    /* Allocate memory to pointer */
    lResp->pAddWCDMASysInfo = &addwcdmasysinfo;

    /* Extract the Additional WCDMA System Info */
    return UnpackTlvCommonAddSysInfo( pTlvData, lResp->pAddWCDMASysInfo );
}

/*
 * This function unpacks the Additional LTE System Information from the QMI
 * indication message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvIndAddLTESysInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    nasSysInfo *lResp = ( nasSysInfo *)pResp;

    /* Allocate memory to pointer */
    lResp->pAddLTESysInfo = &addltesysinfo;

    /* Extract the Additional LTE System Info */
    return GetWord( pTlvData, lResp->pAddLTESysInfo );
}

/*
 * This function unpacks the GSM Call Barring System Information from the QMI
 * indication message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvIndGSMCallBarringSysInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    nasSysInfo *lResp = ( nasSysInfo *)pResp;

    /* Allocate memory to pointer */
    lResp->pGSMCallBarringSysInfo = &gsmcbsysinfo;

    /* Extract the GSM Call Barring System Info */
    return UnpackTlvCommonCallBarringSysInfo( pTlvData,
                                              lResp->pGSMCallBarringSysInfo );
}

/*
 * This function unpacks the WCDMA Call Barring System Information from the QMI
 * indication message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvIndWCDMACallBarringSysInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    nasSysInfo *lResp = ( nasSysInfo *)pResp;

    /* Allocate memory to pointer */
    lResp->pWCDMACallBarringSysInfo = &wcdmacbsysinfo;

    /* Extract the WCDMA Call Barring System Info */
    return UnpackTlvCommonCallBarringSysInfo(pTlvData,
                                             lResp->pWCDMACallBarringSysInfo );
}

/*
 * This function unpacks the LTE Voice Support System Information from the QMI
 * indication message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvIndLTEVoiceSupportSysInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    nasSysInfo *lResp = ( nasSysInfo *)pResp;

    /* Allocate memory to pointer */
    lResp->pLTEVoiceSupportSysInfo = &ltevssi;

    /* Extract the LTE Voice Support System Info */
    return GetByte( pTlvData, lResp->pLTEVoiceSupportSysInfo );
}

/*
 * This function unpacks the GSM Cipher Domain System Information from the QMI
 * indication message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvIndGSMCipherDomainSysInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    nasSysInfo *lResp = ( nasSysInfo *)pResp;

    /* Allocate memory to pointer */
    lResp->pGSMCipherDomainSysInfo = &gsmcdsi;

    /* Extract the GSM Cipher Domain System Info */
    return GetByte( pTlvData, lResp->pGSMCipherDomainSysInfo );
}

/*
 * This function unpacks the WCDMA Cipher Domain System Information from the
 * QMI indication message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvIndWCDMACipherDomainSysInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    nasSysInfo *lResp = ( nasSysInfo *)pResp;

    /* Allocate memory to pointer */
    lResp->pWCDMACipherDomainSysInfo = &wcdmacdsi;

    /* Extract the WCDMA Cipher Domain System Info */
    return GetByte( pTlvData, lResp->pWCDMACipherDomainSysInfo );
}

/*
 * This function unpacks the System Information No Change parameter from the
 * QMI indication message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvIndSysInfoNoChange(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    nasSysInfo *lResp = ( nasSysInfo *)pResp;

    /* Allocate memory to pointer */
    lResp->pSysInfoNoChange = &sysinfonochnge;

    /* Extract the System Information No Change Info */
    return GetByte( pTlvData, lResp->pSysInfoNoChange );
}

/*
 * This function unpacks the System Information indication message to to a
 * user-provided response structure.
 *
 * \param     pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp  [OUT]  - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiCbkNasSysInfoInd(
    BYTE       *pMdmResp,
    nasSysInfo *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_IND_CDMA_SS_INFO,       &UnpackTlvIndCDMASSInfo },
        { eTLV_IND_HDR_SS_INFO,        &UnpackTlvIndHDRSSInfo },
        { eTLV_IND_GSM_SS_INFO,        &UnpackTlvIndGSMSSInfo },
        { eTLV_IND_WCDMA_SS_INFO,      &UnpackTlvIndWCDMASSInfo },
        { eTLV_IND_LTE_SS_INFO,        &UnpackTlvIndLTESSInfo },
        { eTLV_IND_CDMA_SYS_INFO,      &UnpackTlvIndCDMASysInfo },
        { eTLV_IND_HDR_SYS_INFO,       &UnpackTlvIndHDRSysInfo },
        { eTLV_IND_GSM_SYS_INFO,       &UnpackTlvIndGSMSysInfo },
        { eTLV_IND_WCDMA_SYS_INFO,     &UnpackTlvIndWCDMASysInfo },
        { eTLV_IND_LTE_SYS_INFO,       &UnpackTlvIndLTESysInfo },
        { eTLV_IND_ADD_CDMA_SYS_INFO,  &UnpackTlvIndAddCDMASysInfo },
        { eTLV_IND_ADD_HDR_SYS_INFO,   &UnpackTlvIndAddHDRSysInfo },
        { eTLV_IND_ADD_GSM_SYS_INFO,   &UnpackTlvIndAddGSMSysInfo },
        { eTLV_IND_ADD_WCDMA_SYS_INFO, &UnpackTlvIndAddWCDMASysInfo },
        { eTLV_IND_ADD_LTE_SYS_INFO,   &UnpackTlvIndAddLTESysInfo },
        { eTLV_IND_GSM_CB_SYS_INFO,    &UnpackTlvIndGSMCallBarringSysInfo },
        { eTLV_IND_WCDMA_CB_SYS_INFO,  &UnpackTlvIndWCDMACallBarringSysInfo },
        { eTLV_IND_LTE_VS_SYS_INFO,    &UnpackTlvIndLTEVoiceSupportSysInfo },
        { eTLV_IND_GSM_CD_SYS_INFO,    &UnpackTlvIndGSMCipherDomainSysInfo },
        { eTLV_IND_WCDMA_CD_SYS_INFO,  &UnpackTlvIndWCDMACipherDomainSysInfo },
        { eTLV_IND_SYS_INFO_NO_CHANGE, &UnpackTlvIndSysInfoNoChange },
        { eTLV_TYPE_INVALID,           NULL }  /* Important. Sentinel.
                                                * Signifies last item in map.
                                                */
    };
    eRCode = qmunpackresp( pMdmResp,
                           (BYTE*)pApiResp,
                           map,
                           eQMI_NAS_SYS_INFO_IND );
    return eRCode;
}
