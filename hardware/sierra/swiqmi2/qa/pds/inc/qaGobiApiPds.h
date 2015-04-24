/**
 * \ingroup pds
 *
 * \file    qaGobiApiPds.h
 *
 * \brief   Position Determination Service API function prototypes
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __GOBI_API_PDS_H__
#define __GOBI_API_PDS_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  Returns the current PDS state.
 *
 *  \param  pEnabledStatus[OUT]
 *          - Current PDS state
 *              - 0 - disable
 *              - 1 - enable
 *
 *  \param  pTrackingStatus[OUT]
 *          - Current PDS tracking session state
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/10/50\n
 *          Timeout: 2 seconds
 */
ULONG GetPDSState(
    ULONG *pEnabledStatus,
    ULONG *pTrackingStatus );

/**
 *  Sets the PDS state.
 *
 *  \param  enable
 *          - Desired PDS state
 *              - Zero     - disable
 *              - Non-Zero - enable
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/10\n
 *          Timeout: 5 Seconds
 *
 */
ULONG SetPDSState(
    ULONG enable );

/**
 *  This function starts a PDS tracking session.
 *
 *  \param  sessionControl
 *          - Control method:
 *              - 0x0 - Manual
 *
 *  \param  sessionType
 *          - Type:
 *              - 0x0 - New
 *
 *  \param  sessionOperation
 *          - Operating mode:
 *              - 0x00 - Standalone
 *              - 0x01 - MS-based
 *
 *  \param  sessionServerOption
 *          - Location server option:
 *              - 0x0 - Default
 *
 *  \param  fixTimeout
 *          - Maximum time to work on each fix (in seconds, max 255)
 *
 *  \param  fixCount
 *          - Count of position fix requests for this session
 *            (must be at least 1)
 *
 *  \param  fixTimeout
 *          - interval between position fix requests (in seconds)
 *
 *  \param  fixAccuracy
 *          - Time between fixes (in ms)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5,MC7700/10\n
 *          Timeout: 2 Seconds
 *
 */
ULONG StartPDSTrackingSessionExt(
    BYTE  sessionControl,
    BYTE  sessionType,
    BYTE  sessionOperation,
    BYTE  sessionServerOption,
    BYTE  fixTimeout,
    ULONG fixInterval,
    ULONG fixCount,
    ULONG fixAccuracy );

/**
 *  This function stops a PDS tracking session.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5,MC7700/10\n
 *          Timeout: 2 Seconds
 *
 */
ULONG StopPDSTrackingSession();

/**
 *  Injects a system time into the PDS engine.
 *
 *  \param  systemTime
 *          - System time( milliseconds )
 *
 *  \param  systemDiscontinuities
 *          - Number of system time discontinuities
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5,MC7700/10\n
 *          Timeout: 2 Seconds
 *
 */
ULONG PDSInjectTimeReference(
    ULONGLONG systemTime,
    USHORT    systemDiscontinuities );

/**
 *  Returns the default tracking session configuration.
 *  The tracking session configuration is used when a tracking session is
 *  automatically started using SetServiceAutomaticTracking or due to the
 *  device detecting an application opening the NMEA port.
 *
 *  \param  pOperation[OUT]
 *          - Current session operating mode
 *              - 0 - Standalone
 *              - 1 - MS based
 *
 *  \param  pTimeout[OUT]
 *          - Maximum amount of time (seconds) to work on each fix
 *
 *  \param  pInterval[OUT]
 *          - Interval (milliseconds) between fix requests
 *
 *  \param  pAccuracy[OUT]
 *          - Current accuracy threshold (meters)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/10/50\n
 *          Timeout: 2 Seconds
 *
 */
ULONG GetPDSDefaults(
    ULONG *pOperation,
    BYTE  *pTimeout,
    ULONG *pInterval,
    ULONG *pAccuracy );

/**
 *  Sets the default tracking session configuration.
 *  The tracking session configuration is used when a tracking session is
 *  automatically started using SetServiceAutomaticTracking or due to the
 *  device detecting an application opening the NMEA port.
 *
 *  \param  operation
 *          - Current session operating mode
 *              - 0 - Standalone
 *              - 1 - MS based
 *
 *  \param  timeout
 *          - Maximum amount of time (seconds) to work on each fix
 *
 *  \param  interval
 *          - Interval (milliseconds) between fix requests
 *
 *  \param  accuracy
 *          - Current accuracy threshold (meters)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/10\n
 *          Timeout: 2 Seconds
 *
 */
ULONG SetPDSDefaults(
    ULONG operation,
    BYTE  timeout,
    ULONG interval,
    ULONG accuracy );

/**
 *  Returns the XTRA automatic database download configuration.
 *
 *  \param  pbEnabled[OUT]
 *          - Automatic XTRA download status
 *              - 0 - Disabled
 *              - 1 - Enabled
 *
 *  \param  pInterval[OUT]
 *          - Interval (hours) between XTRA downloads
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/10\n
 *          Timeout: 2 Seconds
 *
 */
ULONG GetXTRAAutomaticDownload(
    ULONG  *pbEnabled,
    USHORT *pInterval );

/**
 *  Sets the XTRA automatic database download configuration.
 *
 *  \param  bEnabled
 *          - Automatic XTRA download status
 *              - 0 - Disabled
 *              - 1 - Enabled
 *
 *  \param  interval
 *          - Interval (hours) between XTRA downloads
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700\n
 *          Timeout: 2 Seconds
 *
 */
ULONG SetXTRAAutomaticDownload(
    ULONG  bEnabled,
    USHORT interval );

/**
 *  Returns the XTRA WWAN network preference.
 *  When automatic XTRA database downloading is enabled this preference
 *  determines which WWAN networks will be used to perform the XTRA
 *  database download.
 *
 *  \param  pPreference[OUT]
 *          - XTRA WWAN network preference
 *              - 0x00 - None (any available network)
 *              - 0x01 - Home-only, only when on home systems
 *              - 0x02 - Roam-only, only when on nonhome systems
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/10/50\n
 *          Timeout: 2 Seconds
 *
 */
ULONG GetXTRANetwork(
    ULONG *pPreference );

/**
 *  Sets the XTRA WWAN network preference.
 *  When automatic XTRA database downloading is enabled this preference
 *  determines which WWAN networks will be used to perform the XTRA
 *  database download.
 *
 *  \param  preference
 *          - XTRA WWAN network preference
 *              - 0x00 - None (any available network)
 *              - 0x01 - Home-only, only when on home systems
 *              - 0x02 - Roam-only, only when on non-home systems
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/10\n
 *          Timeout: 2 Seconds
 *
 */
ULONG SetXTRANetwork(
    ULONG preference );

/**
 *  Returns the XTRA database validity period.
 *  When automatic XTRA database downloading is enabled the validity
 *  period determines when the XTRA database will be updated through
 *  a new download.
 *
 *  \param  pGPSWeek[OUT]
 *          - Starting GPS week of validity period
 *
 *  \param  pGPSWeekOffset[OUT]
 *          - Starting GPS week offset (minutes) of validity period
 *
 *  \param  pDuration[OUT]
 *          - Length of validity period (hours)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/10\n
 *          Timeout: 2 Seconds
 *
 */
ULONG GetXTRAValidity(
    USHORT *pGPSWeek,
    USHORT *pGPSWeekOffset,
    USHORT *pDuration );

/**
 *  Forces the XTRA database to be downloaded to the device.
 *
 *  \param  none
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/10\n
 *          Timeout: 2 Seconds
 *
 */
ULONG ForceXTRADownload();

/**
 *  Returns the automatic tracking state for the service.
 *
 *  \param  pbAuto[OUT]
 *          - Automatic tracking session started for service
 *              - 0x00 - Disabled
 *              - 0x01 - Enabled
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/10/50\n
 *          Timeout: 2 Seconds
 *
 */
ULONG GetServiceAutomaticTracking(
    ULONG *pbAuto );

/**
 *  Sets the automatic tracking state for the service.
 *
 *  \param  bAuto
 *          - Automatic tracking session started for service
 *              - 0x00 - Disabled
 *              - 0x01 - Enabled
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/10/50\n
 *          Timeout: 2 Seconds
 *
 */
ULONG SetServiceAutomaticTracking(
    ULONG bAuto );

/**
 *  Returns the automatic tracking configuration for the NMEA COM port.
 *
 *  \param  pbAuto[OUT]
 *          - Automatic tracking enabled for NMEA COM port
 *              - 0x00 - Disabled
 *              - 0x01 - Enabled
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/10/50\n
 *          Timeout: 2 Seconds
 *
 */
ULONG GetPortAutomaticTracking(
    ULONG *pbAuto );

/**
 *  Sets the automatic tracking configuration for the NMEA COM port.
 *
 *  \param  bAuto
 *          - Enable automatic tracking for NMEA COM port
 *              - 0x00 - Disabled
 *              - 0x01 - Enabled
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/10/50\n
 *          Timeout: 2 Seconds
 *
 */
ULONG SetPortAutomaticTracking(
    ULONG bAuto );

/**
 *  Resets the specified PDS data.
 *
 *  \param  pGPSDataMask[IN]
 *          - Bitmask of GPS data to clear (optional)
 *              - 0x00000001 - EPH
 *              - 0x00000002 - ALM
 *              - 0x00000004 - POS
 *              - 0x00000008 - TIME
 *              - 0x00000010 - IONO
 *              - 0x00000020 - UTC
 *              - 0x00000040 - HEALTH
 *              - 0x00000080 - SVDIR
 *              - 0x00000100 - SVSTEER
 *              - 0x00000200 - SADATA
 *              - 0x00000400 - RTI
 *              - 0x00000800 - ALM_CORR
 *              - 0x00001000 - FREQ_BIAS_EST
 *
 *  \param  pCellDataMask[IN]
 *          - Bitmask of cell data to clear (optional)
 *              - 0x00000001 - POS
 *              - 0x00000002 - LATEST_GPS_POS
 *              - 0x00000004 - OTA_POS
 *              - 0x00000008 - EXT_REF_POS
 *              - 0x00000010 - TIMETAG
 *              - 0x00000020 - CELLID
 *              - 0x00000040 - CACHED_CELLID
 *              - 0x00000080 - LAST_SRV_CELL
 *              - 0x00000100 - CUR_SRV_CELL
 *              - 0x00000200 - NEIGHBOR_INFO
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/10/50\n
 *          Timeout: 2 Seconds
 *
 */
ULONG ResetPDSData(
    ULONG *pGPSDataMask,
    ULONG *pCellDataMask );

/**
 *  Sets the PDS AGPS (MS-based) configuration.
 *
 *  \param  pServerAddress[IN]
 *          - IPv4 address of AGPS server [optional]
 *
 *  \param  pServerPort[IN]
 *          - Port number of AGPS server [optional - should be present
 *            when pServerAddress is present]
 *
 *  \param  pServerURL[IN]
 *          - URL of the AGPS server [optional]
 *
 *  \param  pServerURLLength[IN]
 *          - URL length of AGPS server [optional - should be present
 *            when pServerURL is present]
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/10\n
 *          Timeout: 2 Seconds
 *
 */
ULONG SLQSSetAGPSConfig(
    ULONG *pServerAddress,
    ULONG *pServerPort,
    BYTE  *pServerURL,
    BYTE  *pServerURLLength );

/**
 *  Injects a absolute time reference into the PDS engine.
 *
 *  \param  timeMsec
 *          - Represents the number of milliseconds elapsed since either
 *            a GPS or UTC time base. If the time base is UTC, this value
 *            should NOT include leap seconds
 *
 *  \param  timeUncMsec
 *          - Time uncertainty in milliseconds
 *
 *  \param  timeBase
 *          - Time base
 *              - 0x00 - GPS (midnight, Jan 6, 1980)
 *              - 0x01 - UTC (midnight, Jan 1, 1970)
 *
 *  \param  forceFlag
 *          - Force acceptance of data
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/10/50\n
 *          Timeout: 2 Seconds
 *
 */
ULONG SLQSPDSInjectAbsoluteTimeReference(
    ULONGLONG timeMsec,
    ULONG     timeUncMsec,
    BYTE      timeBase,
    BYTE      forceFlag );

/**
 *  Gets the PDS AGPS (MS-based) configuration.
 *
 *  \param  pServerAddress[OUT]
 *          - IPv4 address of AGPS server. "0" if not set
 *
 *  \param  pServerPort[OUT]
 *          - Port number of AGPS server. "0" if not set
 *
 *  \param  pServerURL[OUT]
 *          - URL of the AGPS server. "0" if not set
 *
 *  \param  pServerURLLength[OUT]
 *          - URL length of AGPS server. "0" if not set
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/10\n
 *          Timeout: 2 Seconds
 *
 */
ULONG SLQSGetAGPSConfig(
    ULONG *pServerAddress,
    ULONG *pServerPort,
    BYTE  *pServerURL,
    BYTE  *pServerURLLength );

/**
 *  Position Data Parameters from the external source to be injected to PDS
 *  engine.
 *
 *  \param  pTimeStamp
 *          - Timestamp of the injected position in msec. The time can be of
 *            type UTC, GPS, or Age and is defined in the pTimeType parameter.
 *            If the pTimeType is not present, the timestamp shall be
 *            assumed to be UTC time
 *
 *  \param  pLatitude
 *          - Latitude position referenced to the WGS-84 reference ellipsoid,
 *            counting positive angles north of the equator and negative angles
 *            south of the equator. Value (in decimal degrees) in the range
 *            from -90 degrees to +90 degrees.Value in double float format
 *            (refer toIEEE Std 754-1985)
 *
 *  \param  pLongitude
 *          - Longitude position referenced to the WGS-84 reference ellipsoid,
 *            counting positive angles east of the Greenwich Meridian and
 *            negative angles west of Greenwich meridian. Value (in decimal
 *            degrees) in the range from -180 degrees to +180 degrees.
 *
 *  \param  pAltitudeWrtEllipsoid
 *          - Height above the WGS-84 reference ellipsoid. Value conveys
 *          height (in meters). When injecting altitude information, the
 *          control point should include either this parameter or the
 *          pAltitudeWrtSealevel parameter. Value in single float format
 *          (refer to IEEE Std 754-1985)
 *
 *  \param  pAltitudeWrtSealevel
 *          - Height of MS above the mean sea level in units (in meters).
 *          When injecting altitude information, the control point should
 *          include either this parameter or the pAltitudeWrtEllipsoid
 *          parameter. Value in single float format (refer to IEEE Std 754-1985)
 *
 *  \param  pHorizontalUncCircular
 *          - Circular horizontal uncertainty (in meters). This parameter must
 *          be included if the latitude and longitude parameters are specified.
 *          Value in single float format (refer to IEEE Std 754-1985)
 *
 *  \param  pVerticalUnc
 *          - Vertical uncertainty (in meters). This parameter must be included
 *          if one of the altitude parameter are specified.Value in single float
 *          format (refer to IEEE Std 754-1985)
 *
 *  \param  pHorizontalConfidence
 *          - Confidence value of the location horizontal uncertainty,
 *          specified as percentage, 1 to 100. This parameter must be included
 *          if the latitude and longitude parameters are specified.
 *
 *  \param  pVerticalConfidence
 *          - Confidence value of the location vertical uncertainty, specified
 *          as percentage, 1 to 100. This parameter must be included if one of
 *          the altitude paramters are specified.
 *
 *  \param  pPositionSource
 *          - Source of injected position:
 *              - 0x00 - Unknown
 *              - 0x01 - GPS
 *              - 0x02 - Cell ID
 *              - 0x03 - Enhanced cell ID
 *              - 0x04 - WiFi
 *              - 0x05 - Terrestrial
 *              - 0x06 - Terrestrial hybrid
 *              - 0x07 - Other
 *
 *  \param  pTimeType
 *          - Defines the time value set in the pTimeStamp parameter.
 *              - 0x00 - UTC Time: starting Jan 1, 1970
 *              - 0x01 - GPS Time: starting Jan 6, 1980
 *              - 0x02 - Age: Age of position information
 *
 */
struct PDSPositionData
{
    ULONGLONG *pTimeStamp;
    ULONGLONG *pLatitude;
    ULONGLONG *pLongitude;
    ULONG     *pAltitudeWrtEllipsoid;
    ULONG     *pAltitudeWrtSealevel;
    ULONG     *pHorizontalUncCircular;
    ULONG     *pVerticalUnc;
    BYTE      *pHorizontalConfidence;
    BYTE      *pVerticalConfidence;
    BYTE      *pPositionSource;
    BYTE      *pTimeType;
};

/**
 *  Injects position data into the PDS engine.
 *
 *  \param  pPositionData
 *          - contains the position data to be injected to the PDS engine
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: None\n
 *          Timeout: 2 Seconds
 *
 */
ULONG SLQSPDSInjectPositionData( struct PDSPositionData *pPositionData );

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __GOBI_API_PDS_H__ */
