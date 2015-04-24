/**
 * \ingroup swioma
 *
 * \file    qaGobiApiSwiOmadms.h
 *
 * \brief   SWI Open Mobile Alliance Device Management Service API function
 *          prototypes
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __GOBI_API_SWIOMADMS_H__
#define __GOBI_API_SWIOMADMS_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  Starts an OMA-DM session.
 *
 *  \param  sessionType[IN]
 *          - Session type
 *              - 0x01 - FOTA, to check availability of FW Update
 *              - 0x02 - DM, to check availability of DM Update
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7750\n
 *          Timeout: 20 seconds
 */
ULONG SLQSOMADMStartSession(
    ULONG sessionType );

/**
 *  Cancels an ongoing OMA-DM session.
 *
 *  \param  session[IN]
 *          - Session
 *              - 0x01 - FOTA, to check availability of FW Update
 *              - 0x02 - DM, to check availability of DM Update
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7750\n
 *          Timeout: 20 seconds
 */
ULONG SLQSOMADMCancelSession(
    ULONG session );

/**
 *  Returns information related to the current (or previous if no session is
 *  active) OMA-DM session.
 *
 *  \param  SessionType[IN/OUT]
 *          - Session type
 *              - 0x00 - FOTA
 *
 *  \param  pSessionState[OUT]
 *          - Session state
 *              - 0x00 - OMA-DM Session Complete
 *              - 0x01 - OMA-DM Session Failed
 *              - 0x02 - Downloading Firmware Session Complete
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7750\n
 *          Timeout: 20 seconds
 */
ULONG SLQSOMADMGetSessionInfo(
    ULONG *pSessionType,
    ULONG *pSessionState );

/**
 *  Sends the specified OMA-DM selection for the current network initiated
 *  session.
 *
 *  \param  selection[IN]
 *          - OMA-DM NIA Selection
 *              - 0x01 - Accept
 *              - 0x02 - RejectSession
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7750\n
 *          Timeout: 20 seconds
 */
ULONG SLQSOMADMSendSelection(
    ULONG selection );

/**
 *  Returns the OMA-DM settings.
 *
 *  \param  pbOMADMEnabled[OUT]
 *          - Device OMADM service enabled
 *              - 0x00000001 - Client-initiated device configuration
 *              - 0x00000002 - Network-initiated device configuration
 *              - 0x00000010 - Client-initiated FUMO
 *              - 0x00000020 - Network-initiated FUMO
 *
 *  \param  pbFOTAdownload[OUT]
 *          - Firmware AutoDownload
 *              - 0x00 - Firmware autodownload FALSE
 *              - 0x01 - Firmware autodownload TRUE
 *
 *  \param  pbFOTAUpdate[OUT]
 *          - Firmware AutoUpdate
 *              - 0x00 - Firmware autoupdate FALSE
 *              - 0x01 - Firmware autoupdate TRUE
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7750\n
 *          Timeout: 20 seconds
 */
ULONG SLQSOMADMGetSettings(
    ULONG *pbOMADMEnabled,
    ULONG *pbFOTAdownload,
    ULONG *pbFOTAUpdate );

/**
 *  Sets the OMA-DM settings requested.
 *
 *  \param  bFOTAdownload[IN]
 *          - Firmware AutoDownload
 *              - 0x00 - Firmware autodownload FALSE
 *              - 0x01 - Firmware autodownload TRUE
 *
 *  \param  bFOTAUpdate[IN]
 *          - Firmware AutoUpdate
 *              - 0x00 - Firmware autoupdate FALSE
 *              - 0x01 - Firmware autoupdate TRUE
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7750\n
 *          Timeout: 20 seconds
 */
ULONG SLQSOMADMSetSettings(
    ULONG bFOTAdownload,
    ULONG bFOTAUpdate );

/**
 *  Structure containing the OMA DM settings to be set on the device
 *
 *  \param  FOTAdownload
 *          - 1 Byte parameter indicating support for FOTA Automatic download
 *              - 0x00 - Firmware autodownload FALSE
 *              - 0x01 - Firmware autodownload TRUE
 *
 *  \param  FOTAUpdate
 *          - 1 byte parameter indicating FOTA Automatic update
 *              - 0x00 - Firmware autoupdate FALSE
 *              - 0x01 - Firmware autoupdate TRUE
 *
 *  \param  pAutosdm[IN]
 *          - Optional 1 byte parameter indicating OMA Automatic UI
 *            Alert Response
 *              - 0x00 - Disabled
 *              - 0x01 - Enabled Accept
 *              - 0x02 - Enabled Reject
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 */
typedef struct _SLQSOMADMSettingsReqParams
{
    BYTE  FOTAdownload;
    BYTE  FOTAUpdate;
    BYTE  *pAutosdm;
} SLQSOMADMSettingsReqParams;

/**
 * Sets the settings related to OMADM. These settings are saved on the modem
 * across power cycles.
 *
 *  \param pSLQSOMADMSettingsReqParams[IN]
 *         - See \ref SLQSOMADMSettingsReqParams for more information
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7750\n
 *          Timeout: 20 seconds
 */
ULONG SLQSOMADMSetSettings2(
    SLQSOMADMSettingsReqParams *pSLQSOMADMSettingsReqParams);

/**
 *  Structure containing the OMA DM settings retrieved from the device
 *
 *  \param  pOMADMEnabled[OUT]
 *          - Optional 4 byte parameter indicating OMADM service enabled
 *              - 0x00000001 - Client-initiated device configuration
 *              - 0x00000002 - Network-initiated device configuration
 *              - 0x00000010 - Client-initiated FUMO
 *              - 0x00000020 - Network-initiated FUMO
 *          - function SLQSOMADMGetSettings2() returns a default value
 *            0xFFFFFFFF in case this parameter is not returned by the modem.
 *
 *  \param  pFOTAdownload[OUT]
 *          - Optional 1 Byte parameter indicating support for FOTA Automatic
 *            download
 *              - 0x00 - Firmware autodownload FALSE
 *              - 0x01 - Firmware autodownload TRUE
 *          - function SLQSOMADMGetSettings2() returns a default value 0xFF
 *            in case this parameter is not returned by the modem.
 *
 *  \param  pFOTAUpdate[OUT]
 *          - Optional 1 byte parameter indicating FOTA Automatic update
 *              - 0x00 - Firmware autoupdate FALSE
 *              - 0x01 - Firmware autoupdate TRUE
 *          - function SLQSOMADMGetSettings2() returns a default value 0xFF
 *            in case this parameter is not returned by the modem.
 *
 *  \param  pAutosdm[OUT]
 *          - Optional 1 byte parameter indicating OMA Automatic UI Alert
 *            Response
 *              - 0x00 - Disabled
 *              - 0x01 - Enabled Accept
 *              - 0x02 - Enabled Reject
 *          - function SLQSOMADMGetSettings2() returns a default value 0xFF
 *            in case this parameter is not returned by the modem.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 */
typedef struct _SLQSOMADMSettings
{
    ULONG *pOMADMEnabled;
    BYTE  *pFOTAdownload;
    BYTE  *pFOTAUpdate;
    BYTE  *pAutosdm;
} SLQSOMADMSettings;

/**
 *  Retrieves the OMA-DM settings from the device.
 *
 *  \param  SLQSOMADMSettingsReqParams
 *          - See \ref SLQSOMADMSettings for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC7750\n
 *          Timeout: 20 seconds
 */
ULONG SLQSOMADMGetSettings2( SLQSOMADMSettings *pSLQSOMADMSettings );

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __GOBI_API_SWIOMADMS_H__ */
