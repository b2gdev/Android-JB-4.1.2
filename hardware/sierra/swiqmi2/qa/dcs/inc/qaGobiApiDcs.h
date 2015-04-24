/**
 * \ingroup dcs
 *
 * \file    qaGobiApiDcs.h
 *
 * \brief   Device Connectivity Service API function prototypes
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __GOBI_API_DCS_H__
#define __GOBI_API_DCS_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This structure contains the SLQSGetUsbPortNames Information
 *
 *  \param  AtCmdPort [OUT]
 *          - Name of AT command port
 *
 *  \param  NmeaPort [OUT]
 *          - Name of NMEA port
 *
 *  \param  DmPort [OUT]
 *          - Name of DM port
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/10/50\n
 *          Timeout: 2 seconds\n
 *          Port names are limited to 32 characters.
 */
#define PORTNAM_LEN 32
struct DcsUsbPortNames
{
    CHAR AtCmdPort[PORTNAM_LEN];
    CHAR NmeaPort [PORTNAM_LEN];
    CHAR DmPort   [PORTNAM_LEN];
};

/**
 * An enumeration of eAPI_DCS_GET_DEVICE_MODE states
 */
enum eAPI_DCS_GET_DEVICE_MODE {
    DCS_DEVICE_MODE_DISCONNECTED,
    DCS_DEVICE_MODE_READY,
    DCS_DEVICE_MODE_BOOT_READY
};

/**
 *  Enumerates the QC WWAN devices currently attached to the host. This API
 *  MUST be called before any other API.
 *
 *  \param  pDeviceSize[IN/OUT]
 *          - Upon input, maximum number of elements that the device array can
 *          contain.
 *          - Upon successful output, actual number of elements in the
 *          device array.
 *
 *  \param  pDevices[IN/OUT]
 *          - Device array; array elements are structures with the following elements:\n
 *          CHAR deviceID[256] - Device path (e.g. /dev/qcqmi0)\n
 *          CHAR deviceKey[16] - Device key stored in the device (e.g. A1000004B01051)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_NO_DEVICE on otherwise
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7750\n
 *          Timeout: 2 seconds\n
 *
 */
ULONG QCWWAN2kEnumerateDevices(
    BYTE    *pDevicesSize,
    BYTE    *pDevices );

/**
 *  Connects the Connection Manager API to the first detected QC WWAN device.
 *  This function MUST be called after QCWWAN2kEnumerateDevices has been called.
 *
 *  \param  pDeviceID[IN]
 *          - Device path pertaining to the device for which the API is being
 *          invoked e.g. /dev/qcqmi0.
 *
 *  \param  pDeviceKey[IN]
 *          - Device key pertaining to the device for which the API is being invoked
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_NO_DEVICE otherwise
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7750\n
 *          Timeout: 2 seconds\n
 *
 */
ULONG QCWWAN2kConnect(
    CHAR    *pDeviceID,
    CHAR    *pDeviceKey );

/**
 *  Disconnects the Connection Manager API from a previously connected QC device.
 *  This function de-registers all the callback functions that have been registered.
 *
 *  \param  none
 *
 *  \return eQCWWAN_ERR_NONE
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7750\n
 *          Timeout: 2 seconds\n
 *
 */
ULONG QCWWANDisconnect();

/**
 *  Returns the device ID and device key of the currently connected QC WWAN
 *  device.
 *
 *  \param  deviceIDSize
 *          - Maximum number of characters (including NULL terminator) that the
 *          device ID array can contain.
 *
 *  \param  pDeviceID[OUT]
 *          - Device path string
 *
 *  \param  deviceKeySize
 *          - Maximum number of characters (including NULL terminator) that the
 *          device key array can contain.
 *
 *  \param  pDeviceKey[OUT]
 *          - Device key string
 *
 *  \return eQCWWAN_ERR_NONE if device found, eQCWWAN_ERR_NO_DEVICE otherwise
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7750\n
 *          Timeout: 2 seconds\n
 *
 */
ULONG QCWWAN2kGetConnectedDeviceID(
    ULONG    deviceIDSize,
    CHAR    *pDeviceID,
    ULONG    deviceKeySize,
    CHAR    *pDeviceKey );

/**
 *  Enumerates the QC WWAN devices currently attached to the host. This API is
 *  deprecated; use QCWWAN2kEnumerateDevices instead.
 *
 *  \param  pDeviceSize[IN/OUT]
 *          - Upon input, maximum number of elements that the device array can
 *          contain.
 *          - Upon successful output, actual number of elements in the device
 *          array.
 *
 *  \param  pDevices[IN/OUT]
 *          - Device array; array elements are structures with the following elements:\n
 *          CHAR deviceID[256] - Device path (e.g. /dev/qcqmi0)\n
 *          CHAR deviceKey[16] - Device key stored in the device
 *
 *  \return eQCWWAN_ERR_NONE
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7750\n
 *          Timeout: 2 seconds\n
 *          This API must be called prior to any other APIs.
 *
 */
ULONG QCWWANEnumerateDevices(
    BYTE    *pDevicesSize,
    BYTE    *pDevices );

/**
 *  Enumerates the QC WWAN devices currently attached to the host.
 *  This API MUST be called before any other API.
 *
 *  \param  pDeviceID[IN]
 *          - Device path pertaining to the device for which the API is being
 *          invoked e.g. /dev/qcqmi0.
 *  \param  pDeviceKey[IN]
 *          - Device key pertaining to the device for which the API is being invoked
 *
 *  \return eQCWWAN_ERR_NONE if device found, eQCWWAN_ERR_NO_DEVICE otherwise
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7750\n
 *          Timeout: 2 seconds\n
 *          This API is deprecated; use QCWWAN2kConnect instead
 *
 */
ULONG  QCWWANConnect(
    CHAR *pDeviceID,
    CHAR *pDeviceKey);

/**
 *  Set the location of the SLQS executable
 *
 *  \param   pPath[IN]  - Pointer to fully qualified path of SLQS executable
 *                        (includes the executable file’s name)
 *
 *  \return  eQCWWAN_ERR_NONE
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7750\n
 *          Timeout: None\n
 *
 */
ULONG SetSDKImagePath (
    LPCSTR pPath );

/**
 *  Returns the Usb Port Names currently in use.
 *
 *  \param  pUsbPortNames[OUT]
 *          - Pointer to SLQS Usb Port Names Information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA/UMTS\n
 *          Device Supported: MC83x5, MC7700/10/50\n
 *          Timeout: 2 seconds
 *
 */
ULONG SLQSGetUsbPortNames(
    struct DcsUsbPortNames *pUsbPortNames );

/**
 *  Create the SDK process and IPC sockets for the Application and SDK
 *  processes to communicate over.
 *
 *  \param  none
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA/UMTS\n
 *          Device Supported: MC83x5, MC7700/10/50\n
 *          Timeout: N/A
 *
 */
ULONG SLQSStart();

/**
 *  Kill the SDK process
 *
 *  \param  none
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA/UMTS\n
 *          Device Supported: MC83x5, MC7700/10/50\n
 *          Timeout: N/A\n
 *          This API useful if the application was started with non root
 *          priveleges as subsequent attempt to start any application will fail
 *          because the SDK requires root permission to access /dev/qcqmi
 *          device special files.
 *
 */
ULONG SLQSKillSDKProcess();

/**
 *  Returns the Device Mode
 *
 *  \param  pDeviceMode[OUT]
 *          - Pointer to SLQS Device Mode of type \ref eAPI_DCS_GET_DEVICE_MODE
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     see qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA/UMTS\n
 *          Device Supported: MC83x5, MC7700/10/50\n
 *          Timeout: 2 seconds
 *
 */
ULONG SLQSGetDeviceMode(
    BYTE *pDeviceMode );

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __GOBI_API_DCS_H__ */
