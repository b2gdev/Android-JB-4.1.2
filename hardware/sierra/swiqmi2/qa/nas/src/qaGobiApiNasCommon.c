/*
 * \ingroup nas
 *
 * \file    qaGobiApiNasCommon.c
 *
 * \brief   Nas Service Common Pack and Unpack function definitions.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaGobiApiNasCommon.h"

/*
 * A common function to API and callback which unpacks the array of Service
 * Status Info from the QMI message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure of SrvStatusInfo.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCommonSrvStatusInfo (
    BYTE          *pTlvData,
    SrvStatusInfo *pResp )
{
    enum eQCWWANError eRCode;

    eRCode = GetByte( pTlvData, &pResp->srvStatus );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetByte( pTlvData, &pResp->isPrefDataPath );
}

/*
 * A common function to API and callback which unpacks the array of GSM Service
 * Status Info from the QMI message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure of GSMSrvStatusInfo.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCommonGSMSrvStatusInfo (
    BYTE             *pTlvData,
    GSMSrvStatusInfo *pResp )
{
    enum eQCWWANError eRCode;

    eRCode = GetByte( pTlvData, &pResp->srvStatus );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->trueSrvStatus );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetByte( pTlvData, &pResp->isPrefDataPath );
}

/*
 * A common function to API and callback which unpacks the System Info from the
 * QMI message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure of sysInfoCommon.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvSysInfoCommon (
    BYTE          *pTlvData,
    sysInfoCommon *pResp )
{
    enum eQCWWANError eRCode;

    eRCode = GetByte( pTlvData, &pResp->srvDomainValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->srvDomain );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->srvCapabilityValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->srvCapability );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->roamStatusValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->roamStatus );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->isSysForbiddenValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetByte( pTlvData, &pResp->isSysForbidden );
}

/*
 * A common function to API and callback which unpacks the array of CDMA
 * System Info from the QMI message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure of CDMASysInfo.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCommonCDMASysInfo (
    BYTE        *pTlvData,
    CDMASysInfo *pResp )
{
    enum eQCWWANError eRCode;
    WORD              lC;

    eRCode = UnpackTlvSysInfoCommon( pTlvData, &pResp->sysInfoCDMA );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->isSysPrlMatchValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->isSysPrlMatch );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->pRevInUseValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->pRevInUse );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->bsPRevValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->bsPRev );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->ccsSupportedValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->ccsSupported );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->cdmaSysIdValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &pResp->systemID );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &pResp->networkID );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->bsInfoValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &pResp->baseId );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetLong( pTlvData, &pResp->baseLat );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetLong( pTlvData, &pResp->baseLong );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->packetZoneValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &pResp->packetZone );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->networkIdValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lC = 0 ; lC < PLMN_LENGTH ; lC++ )
    {
        eRCode = GetByte( pTlvData, &pResp->MCC[lC] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    for ( lC = 0 ; lC < PLMN_LENGTH ; lC++ )
    {
        eRCode = GetByte( pTlvData, &pResp->MNC[lC] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * A common function to API and callback which unpacks the HDR System Info from
 * the QMI message to a user provided response structure.
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure of HDRSysInfo.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCommonHDRSysInfo (
    BYTE       *pTlvData,
    HDRSysInfo *pResp )
{
    enum eQCWWANError eRCode;
    WORD              lC;

    eRCode = UnpackTlvSysInfoCommon( pTlvData, &pResp->sysInfoHDR );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->isSysPrlMatchValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->isSysPrlMatch );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->hdrPersonalityValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->hdrPersonality );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->hdrActiveProtValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->hdrActiveProt );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->is856SysIdValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lC = 0 ; lC < SLQS_SYSTEM_ID_SIZE ; lC++ )
    {
        eRCode = GetByte( pTlvData, &pResp->is856SysId[lC] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * A common function to API and callback which unpacks the GSM System Info from
 * the QMI message to a user provided response structure.
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure of GSMSysInfo.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCommonGSMSysInfo (
    BYTE       *pTlvData,
    GSMSysInfo *pResp )
{
    enum eQCWWANError eRCode;
    WORD              lC;

    eRCode = UnpackTlvSysInfoCommon( pTlvData, &pResp->sysInfoGSM );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->lacValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &pResp->lac );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->cellIdValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetLong( pTlvData, &pResp->cellId );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->regRejectInfoValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->rejectSrvDomain );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->rejCause );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->networkIdValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lC = 0 ; lC < PLMN_LENGTH ; lC++ )
    {
        eRCode = GetByte( pTlvData, &pResp->MCC[lC] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    for ( lC = 0 ; lC < PLMN_LENGTH ; lC++ )
    {
        eRCode = GetByte( pTlvData, &pResp->MNC[lC] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    eRCode = GetByte( pTlvData, &pResp->egprsSuppValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->egprsSupp );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->dtmSuppValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetByte( pTlvData, &pResp->dtmSupp );
}

/*
 * A common function to API and callback which unpacks the WCDMA System Info
 * from the QMI message to a user provided response structure.
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure of WCDMASysInfo.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCommonWCDMASysInfo (
    BYTE         *pTlvData,
    WCDMASysInfo *pResp )
{
    enum eQCWWANError eRCode;
    WORD              lC;

    eRCode = UnpackTlvSysInfoCommon( pTlvData, &pResp->sysInfoWCDMA );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->lacValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &pResp->lac );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->cellIdValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetLong( pTlvData, &pResp->cellId );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->regRejectInfoValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->rejectSrvDomain );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->rejCause );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->networkIdValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lC = 0 ; lC < PLMN_LENGTH ; lC++ )
    {
        eRCode = GetByte( pTlvData, &pResp->MCC[lC] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    for ( lC = 0 ; lC < PLMN_LENGTH ; lC++ )
    {
        eRCode = GetByte( pTlvData, &pResp->MNC[lC] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    eRCode = GetByte( pTlvData, &pResp->hsCallStatusValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->hsCallStatus );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->hsIndValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->hsInd );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->pscValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetWord( pTlvData, &pResp->psc );
}

/*
 * A common function to API and callback which unpacks the LTE System Info
 * from the QMI message to a user provided response structure.
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure of LTESysInfo.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCommonLTESysInfo (
    BYTE       *pTlvData,
    LTESysInfo *pResp )
{
    enum eQCWWANError eRCode;
    WORD              lC;

    eRCode = UnpackTlvSysInfoCommon( pTlvData, &pResp->sysInfoLTE );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->lacValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &pResp->lac );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->cellIdValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetLong( pTlvData, &pResp->cellId );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->regRejectInfoValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->rejectSrvDomain );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->rejCause );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->networkIdValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lC = 0 ; lC < PLMN_LENGTH ; lC++ )
    {
        eRCode = GetByte( pTlvData, &pResp->MCC[lC] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    for ( lC = 0 ; lC < PLMN_LENGTH ; lC++ )
    {
        eRCode = GetByte( pTlvData, &pResp->MNC[lC] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    eRCode = GetByte( pTlvData, &pResp->tacValid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetWord( pTlvData, &pResp->tac );
}

/*
 * A common function to API and callback which unpacks the additional CDMA
 * System Info from the QMI message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure of AddCDMASysInfo.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCommonAddCDMASysInfo (
    BYTE           *pTlvData,
    AddCDMASysInfo *pResp )
{
    enum eQCWWANError eRCode;

    eRCode = GetWord( pTlvData, &pResp->geoSysIdx );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetWord( pTlvData, &pResp->regPrd );
}

/*
 * A common function to API and callback which unpacks the additional System
 * Info from the QMI message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure of AddSysInfo.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCommonAddSysInfo (
    BYTE       *pTlvData,
    AddSysInfo *pResp )
{
    enum eQCWWANError eRCode;

    eRCode = GetWord( pTlvData, &pResp->geoSysIdx );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetLong( pTlvData, &pResp->cellBroadcastCap );
}

/*
 * A common function to API and callback which unpacks the Call Barring System
 * Info from the QMI message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]    - Pointer to structure of CallBarringSysInfo.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCommonCallBarringSysInfo (
    BYTE               *pTlvData,
    CallBarringSysInfo *pResp )
{
    enum eQCWWANError eRCode;

    eRCode = GetLong( pTlvData, &pResp->csBarStatus );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetLong( pTlvData, &pResp->psBarStatus );
}
