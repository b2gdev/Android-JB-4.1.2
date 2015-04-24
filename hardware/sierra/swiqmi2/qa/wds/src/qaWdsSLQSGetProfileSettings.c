/*
 * \ingroup wds
 *
 * \file qaWdsSlqsGetProfileSettings.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        QMI_WDS_GET_PROFILE_SETTINGS message.
 *
 * Copyright: © 2011-2012 Sierra Wireless, Inc. all rights reserved
 *
 */
/* include files */

#include "aa/aaglobal.h"
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "amudefs.h"

#include "qaQmiBasic.h"
#include "qaWdsSLQSGetProfileSettings.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the SLQSGetProfile profileType field to the QMI message
 * SDU
 *
 * \param  pBuf   - Pointer to storage into which the packed data will be
 *                  placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
local enum eQCWWANError BuildTlvProfileType(BYTE *pBuf, BYTE *pParam )
{
    GetProfileSettingIn *pReq = (GetProfileSettingIn *)pParam;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Add Profile Type */
    eRCode = PutByte(pBuf, pReq->ProfileType);

    /* Add Profile Id */
    if ( eQCWWAN_ERR_NONE == eRCode )
        eRCode = PutByte(pBuf, pReq->ProfileID);

    return eRCode;
}

/*
 * This function packs the SLQSGetProfile parameters
 * to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \param  pReq        [IN]  - Structure containing details of the profile.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 * \sa     qaGobiApiWds.h for remaining parameter descriptions.
 *
 */
enum eQCWWANError PkQmiWdsSlqsGetProfileSettings(
    WORD                *pMlength,
    BYTE                *pParamField,
    GetProfileSettingIn *pReq )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_PROFILE_IDENTIFIER, &BuildTlvProfileType },
        { eTLV_TYPE_INVALID,       NULL }  /* Important. Sentinel.
                                            * Signifies last item in map.
                                            */
    };
    enum eQCWWANError eRCode;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pReq,
                      map,
                      eQMI_WDS_GET_PROFILE,
                      pMlength );
    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the profile name from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvProfName( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP  *pProfile = &(pProf->curProfile.SlqsProfile3GPP);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if( NULL == pProfile->pProfilename ||
        NULL == pProfile->pProfilenameSize )
        return eRCode;

    /* Extract Profile Name */
    eRCode = qmQmiExtractString( pTlvData,
                                 pProfile->pProfilename,
                                 (ULONG)*(pProfile->pProfilenameSize) );

    return eRCode;
}

/*
 * This function unpacks the PDPType from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvPDPType( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP  *pProfile = &(pProf->curProfile.SlqsProfile3GPP);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pPDPtype )
        return eRCode;

    /* Extract PDP Type */
    eRCode = GetByte(pTlvData, pProfile->pPDPtype );

    return eRCode;
}

/*
 * This function unpacks the PDP Header Compression Type from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvPDPHeadComprType( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP  *pProfile = &(pProf->curProfile.SlqsProfile3GPP);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pPdpHdrCompType )
        return eRCode;

    /* Extract PDP Header Compression Type */
    eRCode = GetByte(pTlvData, pProfile->pPdpHdrCompType );

    return eRCode;
}

/*
 * This function unpacks the PDP Data Compression Type from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvPDPDataComprType( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP  *pProfile = &(pProf->curProfile.SlqsProfile3GPP);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pPdpDataCompType )
        return eRCode;

    /* Extract PDP Data Compression Type */
    eRCode = GetByte(pTlvData, pProfile->pPdpDataCompType );

    return eRCode;
}

/*
 * This function unpacks the APN name from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvAPNName( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP  *pProfile = &(pProf->curProfile.SlqsProfile3GPP);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pAPNName ||
         NULL == pProfile->pAPNnameSize )
        return eRCode;

    /* Extract APN Name */
    eRCode = qmQmiExtractString( pTlvData,
                                 pProfile->pAPNName,
                                 (ULONG)*(pProfile->pAPNnameSize) );

    return eRCode;
}

/*
 * This function unpacks the Primary DNS Address Preference from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvPriDNSIPv4Addr( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP  *pProfile = &(pProf->curProfile.SlqsProfile3GPP);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pPriDNSIPv4AddPref )
        return eRCode;

    /* get primary DNS IPv4 Address Preference */
    eRCode = GetLong( pTlvData, pProfile->pPriDNSIPv4AddPref );

    return eRCode;
}

/*
 * This function unpacks the Secondary DNS Address Preference from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvSecDNSIPv4Addr( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP  *pProfile = &(pProf->curProfile.SlqsProfile3GPP);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pSecDNSIPv4AddPref )
        return eRCode;

    /* Extract secondary DNS IPv4 Address Preference */
    eRCode = GetLong( pTlvData, pProfile->pSecDNSIPv4AddPref );

    return eRCode;
}

/*
 * This function unpacks the UMTS requested QoS from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvUMTSReqQoS( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP  *pProfile = &(pProf->curProfile.SlqsProfile3GPP);
    struct UMTSQoS      *pUMTSQoS = pProfile->pUMTSReqQoS;
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pUMTSQoS )
        return eRCode;

    /* Extract Traffic class */
    eRCode = GetByte( pTlvData, &pUMTSQoS->trafficClass );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract Maximum uplink bit rate */
    eRCode = GetLong( pTlvData, &pUMTSQoS->maxUplinkBitrate );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract maximum downlink bit rate */
    eRCode = GetLong( pTlvData, &pUMTSQoS->maxDownlinkBitrate );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract guaranteed uplink bitrate */
    eRCode = GetLong( pTlvData, &pUMTSQoS->grntUplinkBitrate );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract guaranteed downlink bitrate */
    eRCode = GetLong( pTlvData, &pUMTSQoS->grntDownlinkBitrate );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract QoS delivery order */
    eRCode = GetByte( pTlvData, &pUMTSQoS->qosDeliveryOrder );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract maximum SDU size */
    eRCode = GetLong( pTlvData, &pUMTSQoS->maxSDUSize );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract SDU Error ratio */
    eRCode = GetByte( pTlvData, &pUMTSQoS->sduErrorRatio );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract residual bit error ratio */
    eRCode = GetByte( pTlvData, &pUMTSQoS->resBerRatio );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract delivery erroneous SDUs */
    eRCode = GetByte( pTlvData, &pUMTSQoS->deliveryErrSDU );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract transfer delay */
    eRCode = GetLong( pTlvData, &pUMTSQoS->transferDelay );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract traffic handling priority */
    eRCode = GetLong( pTlvData, &pUMTSQoS->trafficPriority );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    return eRCode;
}

/*
 * This function unpacks the UMTS minimum QoS from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvUMTSMinQoS( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP  *pProfile = &(pProf->curProfile.SlqsProfile3GPP);
    struct UMTSQoS      *pUMTSQoS = pProfile->pUMTSMinQoS;
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pUMTSQoS )
        return eRCode;

    /* Extract Traffic class */
    eRCode = GetByte( pTlvData, &pUMTSQoS->trafficClass );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract Maximum uplink bit rate */
    eRCode = GetLong( pTlvData, &pUMTSQoS->maxUplinkBitrate );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract maximum downlink bit rate */
    eRCode = GetLong( pTlvData, &pUMTSQoS->maxDownlinkBitrate );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract guaranteed uplink bitrate */
    eRCode = GetLong( pTlvData, &pUMTSQoS->grntUplinkBitrate );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract guaranteed downlink bitrate */
    eRCode = GetLong( pTlvData, &pUMTSQoS->grntDownlinkBitrate );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract QoS delivery order */
    eRCode = GetByte( pTlvData, &pUMTSQoS->qosDeliveryOrder );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract maximum SDU size */
    eRCode = GetLong( pTlvData, &pUMTSQoS->maxSDUSize );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract SDU Error ratio */
    eRCode = GetByte( pTlvData, &pUMTSQoS->sduErrorRatio );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract residual bit error ratio */
    eRCode = GetByte( pTlvData, &pUMTSQoS->resBerRatio );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract delivery erroneous SDUs */
    eRCode = GetByte( pTlvData, &pUMTSQoS->deliveryErrSDU );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract transfer delay */
    eRCode = GetLong( pTlvData, &pUMTSQoS->transferDelay );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract traffic handling priority */
    eRCode = GetLong( pTlvData, &pUMTSQoS->trafficPriority );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    return eRCode;
}

/*
 * This function unpacks the GPRS requested QoS from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvGPRSReqQoS( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP      *pProfile = &(pProf->curProfile.SlqsProfile3GPP);
    struct GPRSRequestedQoS *pGPRSQoS = pProfile->pGPRSRequestedQos;
    enum   eQCWWANError     eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pGPRSQoS )
        return eRCode;

    /* Extract precedence class */
    eRCode = GetLong( pTlvData, &pGPRSQoS->precedenceClass );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract delay class */
    eRCode = GetLong( pTlvData, &pGPRSQoS->delayClass );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract reliability class */
    eRCode = GetLong( pTlvData, &pGPRSQoS->reliabilityClass );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract peak throughput class */
    eRCode = GetLong( pTlvData, &pGPRSQoS->peakThroughputClass );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract mean throughput class */
    eRCode = GetLong( pTlvData, &pGPRSQoS->meanThroughputClass );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    return eRCode;
}

/*
 * This function unpacks the GPRS minimum QoS from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvGPRSMinQoS( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP      *pProfile = &(pProf->curProfile.SlqsProfile3GPP);
    struct GPRSRequestedQoS *pGPRSQoS = pProfile->pGPRSMinimumQoS;
    enum   eQCWWANError     eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pGPRSQoS )
        return eRCode;

    /* Extract precedence class */
    eRCode = GetLong( pTlvData, &pGPRSQoS->precedenceClass );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract delay class */
    eRCode = GetLong( pTlvData, &pGPRSQoS->delayClass );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract reliability class */
    eRCode = GetLong( pTlvData, &pGPRSQoS->reliabilityClass );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract peak throughput class */
    eRCode = GetLong( pTlvData, &pGPRSQoS->peakThroughputClass );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract mean throughput class */
    eRCode = GetLong( pTlvData, &pGPRSQoS->meanThroughputClass );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    return eRCode;
}

/*
 * This function unpacks the user name from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvUsername( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP  *pProfile = &(pProf->curProfile.SlqsProfile3GPP);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pUsername ||
         NULL == pProfile->pUsernameSize )
        return eRCode;

    /* Extract Username */
    eRCode = qmQmiExtractString( pTlvData,
                                 pProfile->pUsername,
                                 (ULONG)*(pProfile->pUsernameSize) );

    return eRCode;
}

/*
 * This function unpacks the password from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvPasswd( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP  *pProfile = &(pProf->curProfile.SlqsProfile3GPP);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pPassword ||
         NULL == pProfile->pPasswordSize )
        return eRCode;

    /* Extract password */
    eRCode = qmQmiExtractString( pTlvData,
                                 pProfile->pPassword,
                                 (ULONG)*(pProfile->pPasswordSize) );

    return eRCode;
}

/*
 * This function unpacks the authentication preference from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvAuthPref( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP  *pProfile = &(pProf->curProfile.SlqsProfile3GPP);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pAuthenticationPref )
        return eRCode;

    /* Extract authentication Preference */
    eRCode = GetByte( pTlvData, pProfile->pAuthenticationPref );

    return eRCode;
}

/*
 * This function unpacks the IPv4 Address preference from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvIPv4AddrPref( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP  *pProfile = &(pProf->curProfile.SlqsProfile3GPP);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pIPv4AddrPref )
        return eRCode;

    /* Extract IPv4 Address Preference */
    eRCode = GetLong( pTlvData, pProfile->pIPv4AddrPref );

    return eRCode;
}

/*
 * This function unpacks the PCSCF address using PCO Flag from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvPCSCFAddUsingPCO( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP  *pProfile = &(pProf->curProfile.SlqsProfile3GPP);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pPcscfAddrUsingPCO )
        return eRCode;

    /* Extract PCSCF address using PCO Flag */
    eRCode = GetByte( pTlvData, pProfile->pPcscfAddrUsingPCO );

    return eRCode;
}

/*
 * This function unpacks the PDP Access Control Flag from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvPDPAccessCtrl( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP  *pProfile = &(pProf->curProfile.SlqsProfile3GPP);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pPdpAccessConFlag )
        return eRCode;

    /* Extract PDP access control Flag */
    eRCode = GetByte( pTlvData, pProfile->pPdpAccessConFlag );

    return eRCode;
}

/*
 * This function unpacks the PCSF address using DHCP from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvPCSCFAddUsinDHCP( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP  *pProfile = &(pProf->curProfile.SlqsProfile3GPP);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pPcscfAddrUsingDhcp )
        return eRCode;

    /* Extract PCSCF Address using DHCP */
    eRCode = GetByte( pTlvData, pProfile->pPcscfAddrUsingDhcp );

    return eRCode;
}

/*
 * This function unpacks the IM CN Flag from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvIMCNFlag( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP  *pProfile = &(pProf->curProfile.SlqsProfile3GPP);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pImCnFlag )
        return eRCode;

    /* Extract IM CN Flag */
    eRCode = GetByte( pTlvData, pProfile->pImCnFlag );

    return eRCode;
}

/*
 * This function unpacks the TFT ID1 parameters from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvTFTID1Param( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP  *pProfile     = &(pProf->curProfile.SlqsProfile3GPP);
    struct TFTIDParams  *pTFTIDParams = pProfile->pTFTID1Params;
    enum   eQCWWANError eRCode        = eQCWWAN_ERR_NONE;
    WORD                lTempW;
    ULONG               lTempL;
    BYTE                idx           = 0;

    if ( NULL == pTFTIDParams )
        return eRCode;

    /* Extract Filter ID */
    eRCode = GetByte( pTlvData, &pTFTIDParams->filterId );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract eval ID */
    eRCode = GetByte( pTlvData, &pTFTIDParams->eValid );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract IP version */
    eRCode = GetByte( pTlvData, &pTFTIDParams->ipVersion );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* If IP version is IPV6 fetch all 16 bytes */
    if( IPv6 == pTFTIDParams->ipVersion )
    {
        for( idx = 0; idx < 8; idx++ )
        {
            eRCode = GetWordBe( pTlvData, &lTempW );
            if( eQCWWAN_ERR_NONE != eRCode )
                return eRCode;
            pTFTIDParams->pSourceIP[idx] = lTempW;
        }
    }
    else
    {
        /* If IP version is IPV4 fetch first 4 bytes */
        eRCode = GetLong( pTlvData, &lTempL );
        if( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;
        *(pTFTIDParams->pSourceIP) = lTempL;

        /* Unpack remaining buffer - not returned to user */
        for( idx = 0; idx < 6; idx++ )
        {
            eRCode = GetWordBe( pTlvData, &lTempW );
            if( eQCWWAN_ERR_NONE != eRCode )
                return eRCode;
        }
    }

    /* Extract Next Header */
    eRCode = GetByte( pTlvData, &pTFTIDParams->nextHeader );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract Destination port range - start value */
    eRCode = GetWord( pTlvData, &pTFTIDParams->destPortRangeStart );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract Destination port range - end value */
    eRCode = GetWord( pTlvData, &pTFTIDParams->destPortRangeEnd );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract Source port range - start value */
    eRCode = GetWord( pTlvData, &pTFTIDParams->srcPortRangeStart );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract Source port range - end value */
    eRCode = GetWord( pTlvData, &pTFTIDParams->srcPortRangeEnd );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract IPSEC Security Parameter Index */
    eRCode = GetLong( pTlvData, &pTFTIDParams->IPSECSPI );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract TOS Mask */
    eRCode = GetWord( pTlvData, &pTFTIDParams->tosMask );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract Flow Label */
    eRCode = GetLong( pTlvData, &pTFTIDParams->flowLabel );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    return eRCode;
}

/*
 * This function unpacks the TFT ID2 parameters from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvTFTID2Param( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP  *pProfile     = &(pProf->curProfile.SlqsProfile3GPP);
    struct TFTIDParams  *pTFTIDParams = pProfile->pTFTID2Params;
    enum   eQCWWANError eRCode        = eQCWWAN_ERR_NONE;
    WORD                lTempW;
    ULONG               lTempL;
    BYTE                idx           = 0;

    if ( NULL == pTFTIDParams )
        return eRCode;

    /* Extract Filter ID */
    eRCode = GetByte( pTlvData, &pTFTIDParams->filterId );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract eval ID */
    eRCode = GetByte( pTlvData, &pTFTIDParams->eValid );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract IP version */
    eRCode = GetByte( pTlvData, &pTFTIDParams->ipVersion );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* If IP version is IPV6 fetch all 16 bytes */
    if( IPv6 == pTFTIDParams->ipVersion )
    {
        for( idx = 0; idx < 8; idx++ )
        {
            eRCode = GetWordBe( pTlvData, &lTempW );
            if( eQCWWAN_ERR_NONE != eRCode )
                return eRCode;
            pTFTIDParams->pSourceIP[idx] = lTempW;
        }
    }
    else
    {
        /* If IP version is IPV4 fetch first 4 bytes */
        eRCode = GetLong( pTlvData, &lTempL );
        if( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;
        *(pTFTIDParams->pSourceIP) = lTempL;

        /* Unpack remaining buffer - Not returned to user*/
        for( idx = 0; idx < 6; idx++ )
        {
            eRCode = GetWordBe( pTlvData, &lTempW );
            if( eQCWWAN_ERR_NONE != eRCode )
                return eRCode;
        }
    }

    /* Extract Next Header */
    eRCode = GetByte( pTlvData, &pTFTIDParams->nextHeader );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract Destination port range - start value */
    eRCode = GetWord( pTlvData, &pTFTIDParams->destPortRangeStart );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract Destination port range - end value */
    eRCode = GetWord( pTlvData, &pTFTIDParams->destPortRangeEnd );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract Source port range - start value */
    eRCode = GetWord( pTlvData, &pTFTIDParams->srcPortRangeStart );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract Source port range - end value */
    eRCode = GetWord( pTlvData, &pTFTIDParams->srcPortRangeEnd );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract IPSEC Security Parameter Index */
    eRCode = GetLong( pTlvData, &pTFTIDParams->IPSECSPI );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract TOS Mask */
    eRCode = GetWord( pTlvData, &pTFTIDParams->tosMask );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract Flow Label */
    eRCode = GetLong( pTlvData, &pTFTIDParams->flowLabel );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    return eRCode;
}

/*
 * This function unpacks the PDP context number from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvPDPCtx( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP  *pProfile = &(pProf->curProfile.SlqsProfile3GPP);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pPdpContext )
        return eRCode;

    /* Extract PDP Context Number */
    eRCode = GetByte( pTlvData, pProfile->pPdpContext );

    return eRCode;
}

/*
 * This function unpacks the PDP context secondary flag from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvPDPCtxSecFlag( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP  *pProfile = &(pProf->curProfile.SlqsProfile3GPP);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pSecondaryFlag )
        return eRCode;

    /* Extract PDP Context Secondary Flag */
    eRCode = GetByte( pTlvData, pProfile->pSecondaryFlag );

    return eRCode;
}

/*
 * This function unpacks the PDP context primary ID from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvPDPCtxPriID( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP  *pProfile = &(pProf->curProfile.SlqsProfile3GPP);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pPrimaryID )
        return eRCode;

    /* Extract PDP Context Primary ID */
    eRCode = GetByte( pTlvData, pProfile->pPrimaryID );

    return eRCode;
}

/*
 * This function unpacks the IPv6 Address preference from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvIPv6AddrPref( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP  *pProfile = &(pProf->curProfile.SlqsProfile3GPP);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;
    WORD                lTemp;
    BYTE                idx       = 0;

    if ( NULL == pProfile->pIPv6AddPref )
        return eRCode;

    /* Extract IPv6 Address preference */
    for( idx = 0; idx < 8; idx++ )
    {
        eRCode = GetWordBe( pTlvData, &lTemp );
        if( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;
        pProfile->pIPv6AddPref[idx] = lTemp;
    }
    return eRCode;
}

/*
 * This function unpacks the UMTS Request QoS with Signalling Indication flag
 * from the QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvUMTSReqQoSSigInd( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP      *pProfile    = &(pProf->curProfile.SlqsProfile3GPP);
    struct UMTSReqQoSSigInd *pUMTSQoSSig = pProfile->pUMTSReqQoSSigInd;
    struct UMTSQoS          *pUMTSQoS    = &(pUMTSQoSSig->UMTSReqQoS);
    enum   eQCWWANError     eRCode       = eQCWWAN_ERR_NONE;

    if ( NULL == pUMTSQoSSig )
        return eRCode;

    /* Extract Traffic class */
    eRCode = GetByte( pTlvData, &pUMTSQoS->trafficClass );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract Maximum uplink bit rate */
    eRCode = GetLong( pTlvData, &pUMTSQoS->maxUplinkBitrate );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract maximum downlink bit rate */
    eRCode = GetLong( pTlvData, &pUMTSQoS->maxDownlinkBitrate );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract guaranteed uplink bitrate */
    eRCode = GetLong( pTlvData, &pUMTSQoS->grntUplinkBitrate );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract guaranteed downlink bitrate */
    eRCode = GetLong( pTlvData, &pUMTSQoS->grntDownlinkBitrate );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract QoS delivery order */
    eRCode = GetByte( pTlvData, &pUMTSQoS->qosDeliveryOrder );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract maximum SDU size */
    eRCode = GetLong( pTlvData, &pUMTSQoS->maxSDUSize );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract SDU Error ratio */
    eRCode = GetByte( pTlvData, &pUMTSQoS->sduErrorRatio );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract residual bit error ratio */
    eRCode = GetByte( pTlvData, &pUMTSQoS->resBerRatio );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract delivery erroneous SDUs */
    eRCode = GetByte( pTlvData, &pUMTSQoS->deliveryErrSDU );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract transfer delay */
    eRCode = GetLong( pTlvData, &pUMTSQoS->transferDelay );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract traffic handling priority */
    eRCode = GetLong( pTlvData, &pUMTSQoS->trafficPriority );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract signalling indication flag */
    eRCode = GetByte( pTlvData, &pUMTSQoSSig->SigInd );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    return eRCode;
}

/*
 * This function unpacks the UMTS minimum QoS with Signalling Indication flag
 * from the QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvUMTSMinQoSSigInd( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP      *pProfile    = &(pProf->curProfile.SlqsProfile3GPP);
    struct UMTSReqQoSSigInd *pUMTSQoSSig = pProfile->pUMTSMinQosSigInd;
    struct UMTSQoS          *pUMTSQoS    = &(pUMTSQoSSig->UMTSReqQoS);
    enum   eQCWWANError     eRCode       = eQCWWAN_ERR_NONE;

    if ( NULL == pUMTSQoSSig )
        return eRCode;

    /* Extract Traffic class */
    eRCode = GetByte( pTlvData, &pUMTSQoS->trafficClass );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract Maximum uplink bit rate */
    eRCode = GetLong( pTlvData, &pUMTSQoS->maxUplinkBitrate );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract maximum downlink bit rate */
    eRCode = GetLong( pTlvData, &pUMTSQoS->maxDownlinkBitrate );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract guaranteed uplink bitrate */
    eRCode = GetLong( pTlvData, &pUMTSQoS->grntUplinkBitrate );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract guaranteed downlink bitrate */
    eRCode = GetLong( pTlvData, &pUMTSQoS->grntDownlinkBitrate );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract QoS delivery order */
    eRCode = GetByte( pTlvData, &pUMTSQoS->qosDeliveryOrder );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract maximum SDU size */
    eRCode = GetLong( pTlvData, &pUMTSQoS->maxSDUSize );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract SDU Error ratio */
    eRCode = GetByte( pTlvData, &pUMTSQoS->sduErrorRatio );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract residual bit error ratio */
    eRCode = GetByte( pTlvData, &pUMTSQoS->resBerRatio );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract delivery erroneous SDUs */
    eRCode = GetByte( pTlvData, &pUMTSQoS->deliveryErrSDU );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract transfer delay */
    eRCode = GetLong( pTlvData, &pUMTSQoS->transferDelay );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract traffic handling priority */
    eRCode = GetLong( pTlvData, &pUMTSQoS->trafficPriority );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract signalling indication flag */
    eRCode = GetByte( pTlvData, &pUMTSQoSSig->SigInd );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    return eRCode;
}

/*
 * This function unpacks the primary DNS IPv6 Address preference from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvPriDNSIPv6Addr( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP  *pProfile = &(pProf->curProfile.SlqsProfile3GPP);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;
    WORD                lTemp;
    BYTE                idx       = 0;

    if ( NULL == pProfile->pPriDNSIPv6addpref )
        return eRCode;

    /* Extract primary DNS IPv6 Address preference */
    for( idx = 0; idx < 8; idx++ )
    {
        eRCode = GetWordBe( pTlvData, &lTemp );
        if( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;
        pProfile->pPriDNSIPv6addpref[idx] = lTemp;
    }
    return eRCode;
}

/*
 * This function unpacks the secondary DNS IPv6 Address preference from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvSecDNSIPv6Addr( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP  *pProfile = &(pProf->curProfile.SlqsProfile3GPP);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;
    WORD                lTemp;
    BYTE                idx       = 0;

    if ( NULL == pProfile->pSecDNSIPv6addpref )
        return eRCode;

    /* Extract secondary DNS IPv6 Address preference */
    for( idx = 0; idx < 8; idx++ )
    {
        eRCode = GetWordBe( pTlvData, &lTemp );
        if( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;
        pProfile->pSecDNSIPv6addpref[idx] = lTemp;
    }
    return eRCode;
}

/*
 * This function unpacks the address allocation preference from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvAddrAllocPref( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP  *pProfile = &(pProf->curProfile.SlqsProfile3GPP);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pAddrAllocPref )
        return eRCode;

    /* Extract address allocation preference */
    eRCode = GetByte( pTlvData, pProfile->pAddrAllocPref );

    return eRCode;
}

/*
 * This function unpacks the 3GPP LTE QoS parameters from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlv3GPPLTEQoS( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP  *pProfile    = &(pProf->curProfile.SlqsProfile3GPP);
    struct QosClassID   *pQosClassID = pProfile->pQosClassID;
    enum   eQCWWANError eRCode       = eQCWWAN_ERR_NONE;

    if ( NULL == pQosClassID )
        return eRCode;

    /* Extract QoS Class Identifier */
    eRCode = GetByte( pTlvData, &pQosClassID->QCI );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract guaranteed downlink bit rate */
    eRCode = GetLong( pTlvData, &pQosClassID->gDlBitRate );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract maximum downlink bit rate */
    eRCode = GetLong( pTlvData, &pQosClassID->maxDlBitRate );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract guaranteed uplink bit rate */
    eRCode = GetLong( pTlvData, &pQosClassID->gUlBitRate );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Extract maximum uplink bit rate */
    eRCode = GetLong( pTlvData, &pQosClassID->maxUlBitRate );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    return eRCode;
}

/*
 * This function unpacks the APN disabled flag from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvAPNDisabled( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP  *pProfile = &(pProf->curProfile.SlqsProfile3GPP);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pAPNDisabledFlag )
        return eRCode;

    /* Extract APN disabled flag */
    eRCode = GetByte( pTlvData, pProfile->pAPNDisabledFlag );

    return eRCode;
}

/*
 * This function unpacks the PDN inactivity timeout from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvInactivTimeout( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP  *pProfile = &(pProf->curProfile.SlqsProfile3GPP);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pPDNInactivTimeout )
        return eRCode;

    /* Extract PDN Inactivity timeout */
    eRCode = GetLong( pTlvData, pProfile->pPDNInactivTimeout );

    return eRCode;
}

/*
 * This function unpacks the APN class from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvAPNClass( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP  *pProfile = &(pProf->curProfile.SlqsProfile3GPP);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pAPNClass )
        return eRCode;

    /* Extract APN class */
    eRCode = GetByte( pTlvData, pProfile->pAPNClass );

    return eRCode;
}

/*
 * This function unpacks the Negotiate DNS Server preference value from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvNegoDNSServPref( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP2 *pProfile = &(pProf->curProfile.SlqsProfile3GPP2);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pNegoDnsSrvrPref )
        return eRCode;

    /* Extract Negotiate DNS server preference */
    eRCode = GetByte( pTlvData, pProfile->pNegoDnsSrvrPref );

    return eRCode;
}

/*
 * This function unpacks the PPP Session close timer for DO value from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvPPPSesCloseTimD0( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP2 *pProfile = &(pProf->curProfile.SlqsProfile3GPP2);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pPppSessCloseTimerDO )
        return eRCode;

    /* Extract PPP Session close timer for DO */
    eRCode = GetLong( pTlvData, pProfile->pPppSessCloseTimerDO );

    return eRCode;
}

/*
 * This function unpacks the PPP Session close timer for 1X value from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvPPPSesCloseTim1X( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP2 *pProfile = &(pProf->curProfile.SlqsProfile3GPP2);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pPppSessCloseTimer1x )
        return eRCode;

    /* Extract PPP Session close timer for 1X */
    eRCode = GetLong( pTlvData, pProfile->pPppSessCloseTimer1x );

    return eRCode;
}

/*
 * This function unpacks the Allow Linger value from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvAllowLinger( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP2 *pProfile = &(pProf->curProfile.SlqsProfile3GPP2);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pAllowLinger )
        return eRCode;

    /* Extract Allow Linger */
    eRCode = GetByte( pTlvData, pProfile->pAllowLinger );

    return eRCode;
}

/*
 * This function unpacks the LCP ACK timeout value from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvLCPAckTimeout( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP2 *pProfile = &(pProf->curProfile.SlqsProfile3GPP2);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pLcpAckTimeout )
        return eRCode;

    /* Extract LCP ACK Timeout */
    eRCode = GetWord( pTlvData, pProfile->pLcpAckTimeout );

    return eRCode;
}

/*
 * This function unpacks the IPCP ACK timeout value from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvIPCPAckTimeout( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP2 *pProfile = &(pProf->curProfile.SlqsProfile3GPP2);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pIpcpAckTimeout )
        return eRCode;

    /* Extract IPCP ACK Timeout */
    eRCode = GetWord( pTlvData, pProfile->pIpcpAckTimeout );

    return eRCode;
}

/*
 * This function unpacks the AUTH timeout value from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvAuthTimeout( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP2 *pProfile = &(pProf->curProfile.SlqsProfile3GPP2);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pAuthTimeout )
        return eRCode;

    /* Extract AUTH Timeout */
    eRCode = GetWord( pTlvData, pProfile->pAuthTimeout );

    return eRCode;
}

/*
 * This function unpacks the LCP configuration request retry count value
 * from the QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvLCPCReqRetCount( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP2 *pProfile = &(pProf->curProfile.SlqsProfile3GPP2);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pLcpCreqRetryCount )
        return eRCode;

    /* Extract LCP config request retry count */
    eRCode = GetByte( pTlvData, pProfile->pLcpCreqRetryCount );

    return eRCode;
}

/*
 * This function unpacks the IPCP configuration request retry count value
 * from the QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvIPCPCReqRetCount( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP2 *pProfile = &(pProf->curProfile.SlqsProfile3GPP2);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pIpcpCreqRetryCount )
        return eRCode;

    /* Extract IPCP Config Request Retry count */
    eRCode = GetByte( pTlvData, pProfile->pIpcpCreqRetryCount );

    return eRCode;
}

/*
 * This function unpacks the authentication retry count value
 * from the QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvAuthRetryCount( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP2 *pProfile = &(pProf->curProfile.SlqsProfile3GPP2);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pAuthRetryCount )
        return eRCode;

    /* Extract authentication Request Retry count */
    eRCode = GetByte( pTlvData, pProfile->pAuthRetryCount );

    return eRCode;
}

/*
 * This function unpacks the authentication protocol value
 * from the QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvAuthProtocol( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP2 *pProfile = &(pProf->curProfile.SlqsProfile3GPP2);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pAuthProtocol )
        return eRCode;

    /* Extract authentication protocol */
    eRCode = GetByte( pTlvData, pProfile->pAuthProtocol );

    return eRCode;
}

/*
 * This function unpacks the user ID from the QMI response message
 * to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvUserID( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP2 *pProfile  = &(pProf->curProfile.SlqsProfile3GPP2);
    enum   eQCWWANError eRCode     = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pUserId ||
         NULL == pProfile->pUserIdSize )
        return eRCode;

    /* Extract User ID */
    eRCode = qmQmiExtractString( pTlvData,
                                 pProfile->pUserId,
                                 (ULONG)*(pProfile->pUserIdSize) );

    return eRCode;
}

/*
 * This function unpacks the Authentication password from the QMI response
 * message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvAuthPass( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP2 *pProfile = &(pProf->curProfile.SlqsProfile3GPP2);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pAuthPassword ||
         NULL == pProfile->pAuthPasswordSize )
        return eRCode;

    /* Extract authentication password */
    eRCode = qmQmiExtractString( pTlvData,
                                 pProfile->pAuthPassword,
                                 (ULONG)*(pProfile->pAuthPasswordSize) );

    return eRCode;
}


/*
 * This function unpacks the Authentication password from the QMI response
 * message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvDataRate( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP2 *pProfile = &(pProf->curProfile.SlqsProfile3GPP2);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pDataRate )
        return eRCode;

    /* Extract data rate */
    eRCode = GetByte( pTlvData, pProfile->pDataRate );

    return eRCode;
}

/*
 * This function unpacks the application type from the QMI response
 * message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvAppType( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP2 *pProfile = &(pProf->curProfile.SlqsProfile3GPP2);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pAppType )
        return eRCode;

    /* Extract application type */
    eRCode = GetLong( pTlvData, pProfile->pAppType );

    return eRCode;
}

/*
 * This function unpacks the data mode from the QMI response
 * message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvDataMode( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP2 *pProfile = &(pProf->curProfile.SlqsProfile3GPP2);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pDataMode )
        return eRCode;

    /* Extract data mode */
    eRCode = GetByte( pTlvData, pProfile->pDataMode );

    return eRCode;
}

/*
 * This function unpacks the application priority from the QMI response
 * message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvAPPPrio( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP2 *pProfile = &(pProf->curProfile.SlqsProfile3GPP2);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pAppPriority )
        return eRCode;

    /* Extract application priority */
    eRCode = GetByte( pTlvData, pProfile->pAppPriority );

    return eRCode;
}

/*
 * This function unpacks the APN String from the QMI response
 * message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvAPNString( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP2 *pProfile = &(pProf->curProfile.SlqsProfile3GPP2);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pApnString ||
         NULL == pProfile->pApnStringSize )
        return eRCode;

    /* Extract APN string */
    eRCode = qmQmiExtractString( pTlvData,
                                 pProfile->pApnString,
                                 (ULONG)*(pProfile->pApnStringSize) );

    return eRCode;
}

/*
 * This function unpacks the PDN type from the QMI response
 * message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvPDNType( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP2 *pProfile = &(pProf->curProfile.SlqsProfile3GPP2);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pPdnType )
        return eRCode;

    /* Extract PDN Type */
    eRCode = GetByte( pTlvData, pProfile->pPdnType );

    return eRCode;
}

/*
 * This function unpacks IS PCSCF address needed value from the QMI response
 * message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvISPCSCFAddrNeed( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP2 *pProfile = &(pProf->curProfile.SlqsProfile3GPP2);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pIsPcscfAddressNedded )
        return eRCode;

    /* Extract IS PCSCF Address Needed */
    eRCode = GetByte( pTlvData, pProfile->pIsPcscfAddressNedded );

    return eRCode;
}

/*
 * This function unpacks IPv4 primary DNS Address from the QMI response
 * message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvPriV4DNSAddr( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP2 *pProfile = &(pProf->curProfile.SlqsProfile3GPP2);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pPrimaryV4DnsAddress )
        return eRCode;

    /* Extract IPv4 Primary DNS address */
    eRCode = GetLong( pTlvData, pProfile->pPrimaryV4DnsAddress );

    return eRCode;
}

/*
 * This function unpacks IPv4 secondary DNS Address from the QMI response
 * message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvSecV4DNSAddr( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP2 *pProfile = &(pProf->curProfile.SlqsProfile3GPP2);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pSecondaryV4DnsAddress )
        return eRCode;

    /* Extract IPv4 Secondary DNS address */
    eRCode = GetLong( pTlvData, pProfile->pSecondaryV4DnsAddress );

    return eRCode;
}

/*
 * This function unpacks IPv6 primary DNS Address from the QMI response
 * message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvPriV6DNSAddr( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP2 *pProfile = &(pProf->curProfile.SlqsProfile3GPP2);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;
    WORD                lTemp;
    BYTE                idx       = 0;

    if ( NULL == pProfile->pPriV6DnsAddress )
        return eRCode;

    /* Extract IPv6 primary DNS address */
    for( idx = 0; idx < 8 ; idx++ )
    {
        eRCode = GetWordBe( pTlvData, &lTemp );
        if( eQCWWAN_ERR_NONE != eRCode )
           return eRCode;
        pProfile->pPriV6DnsAddress[idx] = lTemp;
    }
    return eRCode;
}

/*
 * This function unpacks IPv6 secondary DNS Address from the QMI response
 * message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvSecV6DNSAddr( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP2 *pProfile = &(pProf->curProfile.SlqsProfile3GPP2);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;
    WORD                lTemp;
    BYTE                idx       = 0;

    if ( NULL == pProfile->pSecV6DnsAddress )
        return eRCode;

    /* Extract IPv6 secondary DNS address */
    for( idx = 0; idx < 8 ; idx++ )
    {
        eRCode = GetWordBe( pTlvData, &lTemp );
        if( eQCWWAN_ERR_NONE != eRCode )
           return eRCode;
        pProfile->pSecV6DnsAddress[idx] = lTemp;
    }

    return eRCode;
}

/*
 * This function unpacks RAT type from the QMI response
 * message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvRatType( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP2 *pProfile = &(pProf->curProfile.SlqsProfile3GPP2);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pRATType )
        return eRCode;

    /* Extract RAT type */
    eRCode = GetByte( pTlvData, pProfile->pRATType );

    return eRCode;
}

/*
 * This function unpacks APN enabled from the QMI response
 * message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvAPNEnabled3GPP2( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP2 *pProfile = &(pProf->curProfile.SlqsProfile3GPP2);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pAPNEnabled3GPP2 )
        return eRCode;

    /* Extract APN Enabled */
    eRCode = GetByte( pTlvData, pProfile->pAPNEnabled3GPP2 );

    return eRCode;
}

/*
 * This function unpacks PDN Inactivity Timeout from the QMI response
 * message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvPDNInactTim3GPP2( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP2 *pProfile = &(pProf->curProfile.SlqsProfile3GPP2);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pPDNInactivTimeout3GPP2 )
        return eRCode;

    /* Extract PDN Inactivity Timeout */
    eRCode = GetLong( pTlvData, pProfile->pPDNInactivTimeout3GPP2 );

    return eRCode;
}

/*
 * This function unpacks APN class from the QMI response
 * message to a user provided response structure
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
local enum eQCWWANError UnpackTlvAPNClass3GPP2( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    struct Profile3GPP2 *pProfile = &(pProf->curProfile.SlqsProfile3GPP2);
    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProfile->pAPNClass3GPP2 )
        return eRCode;

    /* Extract APN Class */
    eRCode = GetByte( pTlvData, pProfile->pAPNClass3GPP2 );

    return eRCode;
}

/*
 * This function unpacks the extended error code from the
 * QMI response message to a user provided response structure.
 *
 * \param pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]    - Pointer to structure containing storage
 *                            to return data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 */
local enum eQCWWANError UnpackTlvGetExtErrorCode( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSlqsGetProfileSettingsResp *lResp =
        (struct QmiWdsSlqsGetProfileSettingsResp *)pResp;
    GetProfileSettingOut *pProf = lResp->pProfileSettings;

    enum   eQCWWANError eRCode    = eQCWWAN_ERR_NONE;

    if ( NULL == pProf->pExtErrCode )
        return eRCode;

    /* Extract Extended Error Code */
    eRCode = GetWord( pTlvData, pProf->pExtErrCode );

    return eRCode;
}

/*
 * This function unpacks the SLQSGetProfile response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp     [IN]  - Pointer to packed response from the modem.
 *
 * \param  pProfileType [IN]  - Profile type of the profile to be fetched.
 *
 * \param  pApiResp     [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UpkQmiWdsSlqsGetProfileSettings(
    BYTE                                    *pMdmResp,
    BYTE                                    *pProfileType,
    struct QmiWdsSlqsGetProfileSettingsResp *pApiResp)
{
    enum   eQCWWANError eRCode = eQCWWAN_ERR_QMI_INVALID_PROFILE_TYPE;

    /* Profile type is mandatory parameter */
    if( PROFILE_3GPP != *pProfileType &&
        PROFILE_3GPP2 != *pProfileType )
    {
        return eRCode;
    }

    if ( PROFILE_3GPP == *(pProfileType) )
    {
        static struct qmTlvUnpackerItem map[] =
        {
            { eTLV_RESULT_CODE,                &qmUnpackTlvResultCode },
            { eTLV_GET_PROFILE_NAME,           &UnpackTlvProfName },
            { eTLV_GET_PDP_TYPE,               &UnpackTlvPDPType },
            { eTLV_GET_PDP_HEADER_COMPR_TYPE,  &UnpackTlvPDPHeadComprType },
            { eTLV_GET_PDP_DATA_COMPR_TYPE,    &UnpackTlvPDPDataComprType },
            { eTLV_GET_APN_NAME,               &UnpackTlvAPNName },
            { eTLV_GET_PRI_DNS_IPV4_ADDR_PREF, &UnpackTlvPriDNSIPv4Addr },
            { eTLV_GET_SEC_DNS_IPV4_ADDR_PREF, &UnpackTlvSecDNSIPv4Addr },
            { eTLV_GET_UMTS_REQ_QOS,           &UnpackTlvUMTSReqQoS },
            { eTLV_GET_UMTS_MIN_QOS,           &UnpackTlvUMTSMinQoS },
            { eTLV_GET_GPRS_REQ_QOS,           &UnpackTlvGPRSReqQoS },
            { eTLV_GET_GPRS_MIN_QOS,           &UnpackTlvGPRSMinQoS },
            { eTLV_GET_USERNAME,               &UnpackTlvUsername },
            { eTLV_GET_PASSWORD,               &UnpackTlvPasswd },
            { eTLV_GET_AUTHENTICATION_PREF,    &UnpackTlvAuthPref },
            { eTLV_GET_IPV4_ADDR_PREF,         &UnpackTlvIPv4AddrPref },
            { eTLV_GET_PCSCF_ADDR_USI_PCO,     &UnpackTlvPCSCFAddUsingPCO },
            { eTLV_GET_PDP_ACCESS_CONT_FLAG,   &UnpackTlvPDPAccessCtrl },
            { eTLV_GET_PCSCF_ADDR_USI_DHCP,    &UnpackTlvPCSCFAddUsinDHCP },
            { eTLV_GET_IM_CN_FLAG,             &UnpackTlvIMCNFlag },
            { eTLV_GET_TFT_ID1_PARAM,          &UnpackTlvTFTID1Param },
            { eTLV_GET_TFT_ID2_PARAM,          &UnpackTlvTFTID2Param },
            { eTLV_GET_PDP_CONTEXT,            &UnpackTlvPDPCtx },
            { eTLV_GET_PDP_CONTEXT_SEC_FLAG,   &UnpackTlvPDPCtxSecFlag },
            { eTLV_GET_PDP_CONTEXT_PRI_ID,     &UnpackTlvPDPCtxPriID },
            { eTLV_GET_IPV6_ADDRESS_PREF,      &UnpackTlvIPv6AddrPref },
            { eTLV_GET_UMTS_REQ_QOS_SIG_IND,   &UnpackTlvUMTSReqQoSSigInd },
            { eTLV_GET_UMTS_MIN_QOS_SIG_IND,   &UnpackTlvUMTSMinQoSSigInd },
            { eTLV_GET_PRI_DNS_IPV6_ADDR_PREF, &UnpackTlvPriDNSIPv6Addr },
            { eTLV_GET_SEC_DNS_IPV6_ADDR_PREF, &UnpackTlvSecDNSIPv6Addr },
            { eTLV_GET_ADDR_ALLOC_PREF,        &UnpackTlvAddrAllocPref },
            { eTLV_GET_3GPP_LTE_QOS_PARAMS,    &UnpackTlv3GPPLTEQoS },
            { eTLV_GET_APN_DISABLED_FLAG,      &UnpackTlvAPNDisabled },
            { eTLV_GET_PDN_INACTIVITY_TIMEOUT, &UnpackTlvInactivTimeout },
            { eTLV_GET_APN_CLASS,              &UnpackTlvAPNClass },
            { eTLV_GET_PROFILE_EXTENDED_ERROR, &UnpackTlvGetExtErrorCode },
            { eTLV_TYPE_INVALID,               NULL } /* Important. Sentinel.
                                                       * Signifies last item
                                                       * in map
                                                       */
       };
        eRCode = qmunpackresp( pMdmResp,
                              (BYTE *)pApiResp,
                              map,
                              eQMI_WDS_GET_PROFILE );
    }
    else
    {
        static struct qmTlvUnpackerItem map[] =
        {
            { eTLV_RESULT_CODE,                 &qmUnpackTlvResultCode },
            { eTLV_GET_NEGO_DNS_SERVER_PREF,    &UnpackTlvNegoDNSServPref },
            { eTLV_GET_PPP_SESS_CLOSE_TIMER_DO, &UnpackTlvPPPSesCloseTimD0 },
            { eTLV_GET_PPP_SESS_CLOSE_TIMER_1X, &UnpackTlvPPPSesCloseTim1X },
            { eTLV_GET_ALLOW_LINGER,            &UnpackTlvAllowLinger },
            { eTLV_GET_LCP_ACK_TIMEOUT,         &UnpackTlvLCPAckTimeout },
            { eTLV_GET_IPCP_ACK_TIMEOUT,        &UnpackTlvIPCPAckTimeout },
            { eTLV_GET_AUTH_TIMEOUT,            &UnpackTlvAuthTimeout },
            { eTLV_GET_LCP_CREQ_RET_COUNT,      &UnpackTlvLCPCReqRetCount },
            { eTLV_GET_IPCP_CREQ_RET_COUNT,     &UnpackTlvIPCPCReqRetCount },
            { eTLV_GET_AUTH_RET_COUNT,          &UnpackTlvAuthRetryCount },
            { eTLV_GET_AUTH_PROTOCOl,           &UnpackTlvAuthProtocol },
            { eTLV_GET_USER_ID,                 &UnpackTlvUserID },
            { eTLV_GET_AUTH_PASS,               &UnpackTlvAuthPass },
            { eTLV_GET_DATA_RATE,               &UnpackTlvDataRate },
            { eTLV_GET_APP_TYPE,                &UnpackTlvAppType },
            { eTLV_GET_DATA_MODE,               &UnpackTlvDataMode },
            { eTLV_GET_APP_PRIOR,               &UnpackTlvAPPPrio },
            { eTLV_GET_APN_STRING,              &UnpackTlvAPNString },
            { eTLV_GET_PDN_TYPE,                &UnpackTlvPDNType },
            { eTLV_GET_IS_PCSCF_ADDR_NEEDED,    &UnpackTlvISPCSCFAddrNeed },
            { eTLV_GET_PRIMARY_V4_DNS_ADDR,     &UnpackTlvPriV4DNSAddr },
            { eTLV_GET_SECONDARY_V4_DNS_ADDR,   &UnpackTlvSecV4DNSAddr },
            { eTLV_GET_PRIMARY_V6_DNS_ADDR,     &UnpackTlvPriV6DNSAddr },
            { eTLV_GET_SECONDARY_V6_DNS_ADDR,   &UnpackTlvSecV6DNSAddr },
            { eTLV_GET_RAT_TYPE,                &UnpackTlvRatType },
            { eTLV_GET_APN_ENABLED_3GPP2,       &UnpackTlvAPNEnabled3GPP2 },
            { eTLV_GET_PDN_INACTIV_TIMER_3GPP2, &UnpackTlvPDNInactTim3GPP2 },
            { eTLV_GET_APN_CLASS_3GPP2,         &UnpackTlvAPNClass3GPP2 },
            { eTLV_GET_PROFILE_EXTENDED_ERROR,  &UnpackTlvGetExtErrorCode },
            { eTLV_TYPE_INVALID,                NULL } /* Important. Sentinel.
                                                        * Signifies last item
                                                        * in map
                                                        */
       };
        eRCode = qmunpackresp( pMdmResp,
                              (BYTE *)pApiResp,
                              map,
                              eQMI_WDS_GET_PROFILE );
    }
    return eRCode;
}
