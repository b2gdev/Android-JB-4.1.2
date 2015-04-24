/**
 * \ingroup dms
 *
 * \file    qaGobiApiDms.h
 *
 * \brief   Device Management Service API function prototypes
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __GOBI_API_DMS_H__
#define __GOBI_API_DMS_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  Returns the device manufacturer name
 *
 *  \param  stringSize
 *          - The maximum number of characters (including NULL terminator) that
 *            the string array can contain.
 *
 *  \param  pString[OUT]
 *          - NULL terminated string
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: 2 seconds
 *
 */
ULONG GetManufacturer(
    BYTE     stringSize,
    CHAR    *pString );

/**
 *  Returns the device model ID
 *
 *  \param  stringSize
 *          - The maximum number of characters (including NULL terminator) that
 *            the string array can contain
 *
 *  \param  pString[OUT]
 *          - NULL terminated string
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: 2 seconds
 *
 */
ULONG GetModelID(
    BYTE     stringSize,
    CHAR    *pString );

/**
 *  Returns the device firmware revision
 *
 *  \param  stringSize
 *          - The maximum number of characters (including NULL terminator) that
 *            the string array can contain
 *
 *  \param  pString[OUT]
 *          - NULL terminated string
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: 2 seconds
 *
 */
ULONG GetFirmwareRevision(
    BYTE     stringSize,
    CHAR    *pString );

/**
 *  Returns the device firmmware revisions (AMSS, boot, and PRI)
 *
 *  \param  amssSize
 *          - Maximum number of characters (including NULL terminator) that
 *            the AMSS string array can contain
 *
 *  \param  pAMSSString[OUT]
 *          - NULL-terminated AMSS revision string
 *
 *  \param  bootSize
 *          - Maximum number of characters (including NULL terminator) that
 *            the boot string array can contain
 *
 *  \param  pBootString[OUT]
 *          - NULL-terminated boot code revision string
 *
 *  \param  priSize
 *          - Maximum number of characters (including NULL terminator) that
 *            the PRI string array can contain
 *
 *  \param  pPRIString[OUT]
 *          - NULL-terminated PRI revision string
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: 2 seconds
 *
 */
ULONG GetFirmwareRevisions(
    BYTE   amssSize,
    CHAR * pAMSSString,
    BYTE   bootSize,
    CHAR * pBootString,
    BYTE   priSize,
    CHAR * pPRIString );

/**
 *  Returns the version of the active Preferred Roaming List (PRL) in use by
 *  the device.
 *
 *  \param  pPRLVersion[OUT]
 *          - PRL version number
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Device Supported: MC83x5, MC7750\n
 *          Timeout: 2 seconds
 *
 */
ULONG GetPRLVersion(
    WORD * pPRLVersion );

/**
 *  Returns the device IMSI
 *
 *  \param  stringSize
 *          - The maximum number of characters (including NULL terminator) that
 *            the string array can contain
 *
 *  \param  pString[OUT]
 *          - NULL terminated string
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: 2 seconds
 *
 */
ULONG GetIMSI(
    BYTE   stringSize,
    CHAR * pString );

/**
 *  Returns all the serial numbers assigned to the device. These serial numbers
 *  include the ESN (Electronic serial number of the device), the IMEI
 *  (International Mobile Equipment Identity) and MEID (Mobile Equipment
 *  Identifier).
 *
 *  \param  esnSize
 *          - The maximum number of characters (including NULL terminator) that
 *            the ESN string array can contain
 *
 *  \param  pESNString[OUT]
 *          - NULL-terminated ESN string. Empty string is returned when ESN is
 *            not supported/programmed
 *
 *  \param  imeiSize
 *          - The maximum number of characters (including NULL terminator) that
 *            the IMEI string array can contain
 *
 *  \param  pIMEIString[OUT]
 *          - NULL terminated IMEI string. Empty string is returned when IMEI
 *            is not supported/programmed
 *
 *  \param  meidSize
 *          - The maximum number of characters (including NULL terminator) that
 *            the MEID string array can contain
 *
 *  \param  pMEIDString[OUT]
 *          - NULL-terminated MEID string. Empty string is returned when MEID
 *            is not supported/programmed
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: 2 seconds
 *
 */
ULONG GetSerialNumbers(
   BYTE     esnSize,
   CHAR    *pESNString,
   BYTE     imeiSize,
   CHAR    *pIMEIString,
   BYTE     meidSize,
   CHAR    *pMEIDString );

/**
 *  Returns the hardware revision of the device
 *
 *  \param  stringSize
 *          - The maximum number of characters (including NULL terminator) that
 *            the string array can contain
 *
 *  \param  pString[OUT]
 *          - NULL terminated string
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: 2 seconds
 *
 */
ULONG GetHardwareRevision(
    BYTE stringSize,
    CHAR *pString );

/**
 *  Returns the current time of the device based on the value supported by the
 *  network.
 *
 *  \param  pTimeStamp[OUT]
 *          - Count of 1.25 ms that have elapsed from the start of GPS time
 *            (Jan 6, 1980)
 *
 *  \param  pTimeSource[OUT]
 *          - Source of timestamp\n
 *            0 - 32 kHz device clock\n
 *            1 - CDMA network\n
 *            2 - cdma2000 1xEV-DO network
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: 2 seconds
 *
 *  \note   The source of the timestamp provided specifies how the timestamp
 *          was determined. The first network time that is available will be
 *          returned. If no network time is available, the timestamp is taken
 *          from the 32 kHz slow-clock of the device.
 *
 */
ULONG GetNetworkTime(
    ULONGLONG *pTimeStamp,
    ULONG     *pTimeSource );

/**
 *  Enables or disables protection of SIM contents for a given PIN
 *
 *  \param  id
 *          - PIN ID
 *              - 1 ( PIN1 / CHV1 )
 *              - 2 ( PIN2 / CHV2 )
 *
 *  \param  bEnable
 *          - Enable/disable PIN protection, 0 = Disable
 *
 *  \param  pValue[IN]
 *          - PIN value of the PIN to be enabled/disabled
 *
 *  \param  pVerifyRetriesLeft[OUT]
 *          - Upon operational failure, this will indicate number of retries
 *            left, after which PIN will be blocked.
 *              - 0xFFFFFFFF - Unknown
 *
 *  \param  pUnblockRetriesLeft[OUT]
 *          - Upon operational failure, this will indicate number of unblock
 *            retries left, after which the PIN will be permanently blocked
 *            i.e. UIM is unusable.
 *              - 0xFFFFFFFF - Unknown
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: 5 seconds
 *
 */
ULONG UIMSetPINProtection(
    ULONG id,
    ULONG bEnable,
    CHAR  *pValue,
    ULONG *pVerifyRetriesLeft,
    ULONG *pUnblockRetriesLeft );

/**
 *  Unblocks a blocked SIM
 *
 *  \param  id
 *          - PIN ID
 *              - 1 ( PIN1 / CHV1 )
 *              - 2 ( PIN2 / CHV2 )
 *
 *  \param  pPUKValue[IN]
 *          - PUK value of PIN to unblock
 *
 *  \param  pNewValue[IN]
 *          - New PIN value of PIN to unblock
 *
 *  \param  pVerifyRetriesLeft[OUT]
 *          - Upon operational failure, this will indicate number of retries
 *            left, after which the PIN will be blocked.
 *              - 0xFFFFFFFF - Unknown
 *
 *  \param  pUnblockRetriesLeft[OUT]
 *          - Upon operational failure, this will indicate number of unblock
 *            retries left, after which the PIN will be permanently blocked;
 *            i.e. UIM is unusable
 *              - 0xFFFFFFFF - Unknown
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: 5 seconds
 *
 */
ULONG UIMUnblockPIN(
    ULONG id,
    CHAR  *pPUKValue,
    CHAR  *pNewValue,
    ULONG *pVerifyRetriesLeft,
    ULONG *pUnblockRetriesLeft );

/**
 *  Verifies a SIM PIN.
 *
 *  \param  id
 *          - PIN ID
 *              - 1 ( PIN1 / CHV1 )
 *              - 2 ( PIN2 / CHV2 )
 *
 *  \param  pValue[IN]
 *          - Value of PIN to verify
 *
 *  \param  pVerifyRetriesLeft[OUT]
 *          - Upon operational failure, this will indicate number of retries
 *            left, after which the PIN will be blocked.
 *              - 0xFFFFFFFF - Unknown
 *
 *  \param  pUnblockRetriesLeft[OUT]
 *          - Upon operational failure, this will indicate number of unblock
 *            retries left, after which the PIN will be permanently blocked;
 *            i.e. UIM is unusable
 *              - 0xFFFFFFFF - Unknown
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: 5 seconds
 *
 */
ULONG UIMVerifyPIN(
    ULONG id,
    CHAR  *pValue,
    ULONG *pVerifyRetriesLeft,
    ULONG *pUnblockRetriesLeft );

/**
 *  Changes the PIN value for a given PIN
 *
 *  \param  id
 *          - PIN ID
 *              - 1 ( PIN1 / CHV1 )
 *              - 2 ( PIN2 / CHV2 )
 *
 *  \param  pOldValue[IN]
 *          - Old PIN value of PIN to change
 *
 *  \param  pNewValue[IN]
 *          - New PIN value of PIN to change
 *
 *  \param  pVerifyRetriesLeft[OUT]
 *          - Upon operational failure, this will indicate number of retries
 *            left, after which PIN will be blocked.
 *              - 0xFFFFFFFF - Unknown
 *
 *  \param  pUnblockRetriesLeft[OUT]
 *          - Upon operational failure, this will indicate number of unblock
 *            retries left, after which the PIN will be permanently blocked;
 *            i.e. UIM is unusable.
 *              - 0xFFFFFFFF - Unknown
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: 5 seconds
 *
 */
ULONG UIMChangePIN(
    ULONG id,
    CHAR  *pOldValue,
    CHAR  *pNewValue,
    ULONG *pVerifyRetriesLeft,
    ULONG *pUnblockRetriesLeft );

/**
 *  Returns the voice number in use by the device
 *
 *  \param  voiceNumberSize
 *          - Maximum number of characters (including NULL terminator) that the
 *            voice number array can contain.
 *
 *  \param  pVoiceNumber[OUT]
 *          - Voice number string: MDN or MS ISDN
 *
 *  \param  minSize
 *          - Maximum number of characters (including NULL terminator) that the
 *            MIN array can contain.
 *
 *  \param  pMIN
 *          - MIN string: Empty string returned when MIN is not supported/
 *            programmed.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: 2 seconds
 *
 */
ULONG GetVoiceNumber(
    BYTE voiceNumberSize,
    CHAR *pVoiceNumber,
    BYTE minSize,
    CHAR *pMIN );

/**
 *  Sets the operating mode of the device.
 *
 *  \param  powerMode
 *          - Selected operating mode
 *             - See qaGobiApiTablePowerModes.h for power modes
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: 2 seconds
 *
 */
ULONG SetPower(
    ULONG powerMode );

/**
 *  Returns the operating mode of the device.
 *
 *  \param  pPowerMode[OUT]
 *          - Selected operating mode
 *              - See qaGobiApiTablePowerModes.h for power modes
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: 2 Seconds
 *
 */
ULONG GetPower(
    ULONG *pPowerMode );

/**
 *  Returns the status of the specified UIM facility control key.
 *
 *  \param  id
 *          - Facility ID
 *              - 0 - Network Personalization (PN)
 *              - 1 - Network Subset Personalization (PU)
 *              - 2 - Service Provider Personalization (PP)
 *              - 3 - Corporate Personalization (PC)
 *              - 4 - UIM Personalization (PF)
 *
 *  \param  pStatus[OUT]
 *          - Control key status
 *              - 0 - Deactivated
 *              - 1 - Activated
 *              - 2 - Blocked
 *
 *  \param  pVerifyRetriesLeft[OUT]
 *          - The number of retries left, after which the control key will be
 *            blocked
 *              - 0xFFFFFFFF - Unknown
 *  \param  pUnblockRetriesLeft[OUT]
 *          - The number of unblock retries left, after which the control key
 *            will be permanently blocked
 *              - 0xFFFFFFFF - Unknown
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: MC83x5, MC7700\n
 *          Timeout: 5 seconds
 *
 */
ULONG UIMGetControlKeyStatus(
    ULONG id,
    ULONG *pStatus,
    ULONG *pVerifyRetriesLeft,
    ULONG *pUnblockRetriesLeft );

/**
 *  Returns the UIM ICCID.
 *
 *  \param  stringSize
 *          - The maximum number of characters (including NULL terminator) that
 *            the string array can contain.
 *
 *  \param  pString[OUT]
 *          - NULL terminated string
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: MC83x5, MC7700\n
 *          Timeout: 2 seconds
 *
 */
ULONG UIMGetICCID(
    BYTE stringSize,
    CHAR *pString );

/**
 *  Gets the status of the SIM PINs
 *
 *  \param  id
 *          - PIN ID
 *              - 1 ( PIN1 / CHV1 )
 *              - 2 ( PIN2 / CHV2 )
 *
 *  \param  pStatus[OUT]
 *          - PIN status(0xFFFFFFFF - Unknown)
 *              - 0 - PIN not initialized
 *              - 1 - PIN enabled, not verified
 *              - 2 - PIN enabled, verified
 *              - 3 - PIN disabled
 *              - 4 - PIN blocked
 *              - 5 - PIN permanently blocked
 *
 *  \param  pVerifyRetriesLeft[OUT]
 *          - Upon operational failure, this will indicate number of retries
 *            left, after which PIN will be blocked.
 *              - 0xFFFFFFFF - Unknown
 *
 *  \param  pUnblockRetriesLeft[OUT]
 *          - Upon operational failure, this will indicate number of unblock
 *            retries left, after which the PIN will be permanently blocked;
 *            i.e., UIM is unusable.
 *              - 0xFFFFFFFF - Unknown
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: MC83x5, MC7700\n
 *          Timeout: 5 seconds
 *
 */
ULONG UIMGetPINStatus(
    ULONG id,
    ULONG *pStatus,
    ULONG *pVerifyRetriesLeft,
    ULONG *pUnblockRetriesLeft );

/**
 *  Returns reason why the operating mode of the device is currently offline.
 *
 *  \param  pReasonMask[OUT]
 *          - Bitmask of offline reasons
 *              - 0x00000001 - Host image configuration issue
 *              - 0x00000002 - PRI image configuration issue
 *              - 0x00000004 - PRI version incompatible
 *              - 0x00000008 - PRI copy issue
 *              - All others - Reserved
 *
 *  \param  pbPlatform[OUT]
 *          - Is the device offline due to a platform restriction?
 *              - 0 - No
 *              - 1 - Yes
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: 2 Seconds
 *
 */
ULONG GetOfflineReason(
    ULONG *pReasonMask,
    ULONG *pbPlatform );

/**
 *  Changes the specified UIM facility control key
 *
 *  \param  id
 *          - Facility ID
 *              - 0 - Network Personalization (PN)
 *              - 1 - Network Subset Personalization (PU)
 *              - 2 - Service Provider Personalization (PP)
 *              - 3 - Corporate Personalization (PC)
 *              - 4 - UIM Personalization (PF)
 *
 *  \param  status
 *          - Control key status
 *              - 0 - Deactivated
 *
 *  \param  pValue[IN]
 *          - Control key de-personalization string (maximum length of 8
 *            characters)
 *
 *  \param  pVerifyRetriesLeft[OUT]
 *          - Upon operational failure, this will indicate number of retries
 *            left, after which the control key will be blocked
 *              - 0xFFFFFFFF - Unknown
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: None\n
 *          Timeout: 5 seconds
 *
 */
ULONG UIMSetControlKeyProtection(
    ULONG id,
    ULONG status,
    CHAR  *pValue,
    ULONG *pVerifyRetriesLeft );

/**
 *  Unblocks the specified UIM facility control key
 *
 *  \param  id
 *          - Facility ID
 *              - 0 - Network Personalization (PN)
 *              - 1 - Network Subset Personalization (PU)
 *              - 2 - Service Provider Personalization (PP)
 *              - 3 - Corporate Personalization (PC)
 *              - 4 - UIM Personalization (PF)
 *
 *  \param  pValue[IN]
 *          - Control key de-personalization string (maximum length of 8
 *            characters)
 *
 *  \param  pUnblockRetriesLeft[OUT]
 *          - Upon operational failure, this will indicate number of unblock
 *            retries left, after which the control key will be blocked
 *              - 0xFFFFFFFF - Unknown
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: None\n
 *          Timeout: 5 seconds
 *
 */
ULONG UIMUnblockControlKey(
    ULONG id,
    CHAR  *pValue,
    ULONG *pUnblockRetriesLeft );

/**
 *  Gets the device capabilities
 *
 *  \param  pMaxTXChannelRate[OUT]
 *          - Maximum transmission rate (in bps) supported by the device
 *          - In multi-technology devices, this value will be the greatest rate
 *            among all supported technologies
 *
 *  \param  pMaxRXChannelRate[OUT]
 *          - Maximum reception rate (in bps) supported by the device
 *          - In multi-technology devices, this value will be the greatest
 *            rate among all supported technologies
 *
 *  \param  pDataServiceCapability[OUT]
 *          - CS/PS data service capability
 *              - 0 - No data services supported
 *              - 1 - Only Circuit Switched (CS) services supported
 *              - 2 - Only Packet Switched (PS) services supported
 *              - 3 - Simultaneous CS and PS
 *              - 4 - Non-simultaneous CS and PS
 *
 *  \param  pSimCapability[OUT]
 *          - Device SIM capability
 *              - 0 - SIM not supported
 *              - 1 - SIM supported
 *
 *  \param  pRadioIfacesSize[IN/OUT]
 *          - Upon input, the maximum number of elements that the radio
 *            interface array can contain
 *          - Upon successful output, actual number of elements in the radio
 *            interface array
 *
 *  \param  pRadioIfaces[OUT]
 *          - Radio interface array. This is a structure of array containing the
 *            elements below.\n
 *            ULONG radioInterface
 *              - See qaGobiApiTableRadioInterfaces.h for Radio Interfaces
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: 2 seconds
 *
 */
ULONG GetDeviceCapabilities(
    ULONG *pMaxTXChannelRate,
    ULONG *pMaxRXChannelRate,
    ULONG *pDataServiceCapability,
    ULONG *pSimCapability,
    ULONG *pRadioIfacesSize,
    BYTE  *pRadioIfaces );

/**
 *  Resets to default factory settings of the device
 *
 *  \param  pSPC[IN]
 *          - NULL-terminated string representing a six-digit service
 *          programming code
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5\n
 *          Timeout: 5 minutes
 *
 */
ULONG ResetToFactoryDefaults(
    CHAR *pSPC );

/**
 * This function Validates Service Programming code of the device
 *
 *  \param  pSPC[IN]
 *          - NULL-terminated string representing a six-digit service
 *          programming code
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Device Supported: MC83x5, MC7750\n
 *          Timeout: 2 seconds
 *
 */
ULONG ValidateSPC(
    CHAR *pSPC );

/**
 *  Requests the device to perform automatic service activation
 *
 *  \param  pActivationCode[IN]
 *          - NULL-terminated string representing activation code
 *          (maximum string length of 12);
 *          specific carrier requirements may dictate actual
 *          activation code that is applicable, e.g., "*22899"
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Device Supported: MC83x5, MC7750\n
 *          Timeout: 5 Minutes
 *
 */
ULONG ActivateAutomatic(
    CHAR *pActivationCode );

/**
 *  Requests the device perform a manual activation using the
 *  provided parameters.
 *
 *  \param  pSPC[IN]
 *          - NULL-terminated string representing the six-digit
 *          service programming code
 *
 *  \param  sid[IN]
 *          - System identification number
 *
 *  \param  pMDN[IN]
 *          - Pointer to a NULL-terminated character string that
 *          contains mobile directory number
 *
 *  \param  pMIN[IN]
 *          - Pointer to a NULL-terminated character string that
 *          contains the mobile identification number
 *
 *  \param  prlSize[IN]
 *          - (Optional) Size of PRL file array
 *
 *  \param  pPRL[IN]
 *          - (Optional) PRL file contents
 *
 *  \param  pMNHA[IN]
 *          - (Optional) Pointer to a NULL-terminated character
 *          string that contains MN-HA
 *
 *  \param  pMNAAA[IN]
 *          - (Optional) Pointer to a NULL-terminated character
 *          string that contains the MN-AAA
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Device Supported: None\n
 *          Timeout: When no PRL is specified - Five minutes\n
 *                   When a PRL is specified  - Five minutes multiplied by the
 *                   number of PRL block writes required (maximum block size is
 *                   currently 256 bytes)
 *          Additional Timeout : 2 Seconds for Device Reset
 *
 */
ULONG ActivateManual(
    CHAR  *pSPC,
    WORD  sid,
    CHAR  *pMDN,
    CHAR  *pMIN,
    ULONG prlSize,
    BYTE  *pPRL,
    CHAR  *pMNHA,
    CHAR  *pMNAAA );

/**
 *  Returns the Enhanced Roaming Indicator (ERI) file that
 *  is stored in EFS on the device at a predetermined location.
 *  See the carrier requirements for specific details.
 *
 *  \param  pFileSize[IN/OUT]
 *          - Upon input, the maximum number of bytes that file
 *          contents array can contain.\n
 *          Upon successful output, actual number of bytes written to
 *          file contents array
 *
 *  \param  pFile[OUT]
 *          - File contents
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Device Supported: None\n
 *          Timeout: 2 Seconds
 *
 */
ULONG GetERIFile(
    ULONG *pFileSize,
    BYTE  *pFile );

/**
 *  Returns the device activation state.
 *
 *  \param  pActivationState[OUT]
 *          -  Service Activation Code\n
 *              0 - Service not activated\n
 *              1 - Service activated\n
 *              2 - Activation connecting\n
 *              3 - Activation connected\n
 *              4 - OTASP security authenticated\n
 *              5 - OTASP NAM downloaded\n
 *              6 - OTASP MDN downloaded\n
 *              7 - OTASP IMSI downloaded\n
 *              8 - OTASP PRL downloaded\n
 *              9 - OTASP SPC downloaded\n
 *              10 - OTASP settings committed
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Device Supported: MC83x5\n
 *          Timeout: 2 Seconds
 *
 */
ULONG GetActivationState(
    ULONG *pActivationState);

/**
 *  Returns the UIM state.
 *
 *  \param  pUIMState[OUT]
 *          -  UIM state:\n
 *             - 0x00 - UIM initialization completed\n
 *             - 0x01 - UIM locked or failed\n
 *             - 0x02 - UIM not present\n
 *             - 0x03 - 0xFE - Reserved\n
 *             - 0xFF - UIM state currently unavailable\n
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: MC83x5, MC7700\n
 *          Timeout: 2 Seconds
 *
 */
ULONG SLQSUIMGetState(
    ULONG *pUIMState);

/**
 *  Returns the band capability of the device.
 *
 *  \param  pBandCapability[OUT]
 *          Bitmask of bands supported by the device
 *              - Bit 0 - Band class 0, A-system
 *              - Bit 1 - Band class 0, B-system
 *              - Bit 2 - Band class 1, all blocks
 *              - Bit 3 - Band class 2
 *              - Bit 4 - Band class 3, A-system
 *              - Bit 5 - Band class 4, all blocks
 *              - Bit 6 - Band class 5, all blocks
 *              - Bit 7 - GSM DCS band (1800)
 *              - Bit 8 - GSM Extended GSM (E-GSM) band (900)
 *              - Bit 9 - GSM Primary GSM (P-GSM) band (900)
 *              - Bit 10 - Band class 6
 *              - Bit 11 - Band class 7
 *              - Bit 12 - Band class 8
 *              - Bit 13 - Band class 9
 *              - Bit 14 - Band class 10
 *              - Bit 15 - Band class 11
 *              - Bit 16 - GSM 450 band
 *              - Bit 17 - GSM 480 band
 *              - Bit 18 - GSM 750 band
 *              - Bit 19 - GSM 850 band
 *              - Bit 20 - GSM railways GSM band (900)
 *              - Bit 21 - GSM PCS band (1900)
 *              - Bit 22 - WCDMA (Europe, Japan, and China) 2100 band
 *              - Bit 23 - WCDMA US PCS 1900 band
 *              - Bit 24 - WCDMA (Europe and China) DCS 1800 band
 *              - Bit 25 - WCDMA US 1700 band
 *              - Bit 26 - WCDMA US 850 band
 *              - Bit 27 - WCDMA Japan 800 band
 *              - Bit 28 - Band class 12
 *              - Bit 29 - Band class 14
 *              - Bit 30 - Reserved
 *              - Bit 31 - Band class 15
 *              - Bits 32 through 47 - Reserved
 *              - Bit 48 - WCDMA Europe 2600 band
 *              - Bit 49 - WCDMA Europe and Japan 900 band
 *              - Bit 50 - WCDMA Japan 1700 band
 *              - Bits 51 through 55 - Reserved
 *              - Bit 56 - Band class 16
 *              - Bit 57 - Band class 17
 *              - Bit 58 - Band class 18
 *              - Bit 59 - Band class 19
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS/CDMA\n
 *          Device Supported: MC83x5, MC7700/50\n
 *          Timeout: 2 Seconds
 *
 */
ULONG SLQSGetBandCapability( ULONGLONG *pBandCapability );

/**
 *  This structure contains current settings of custom features
 *
 *  \param  GpsEnable
 *          - describes if GPS is enabled or disabled
 *          - values:
 *              - 0x00 - GPS is disabled
 *              - 0x01 - GPS is enabled
 *          - function SLQSGetCustFeatures() returns a default value FFFFFFFF
 *            if no value is returned by the modem
 *
 *  \param  pDisableIMSI
 *          - optional 1 byte parameter
 *          - describes if IMSI display is enabled or disabled
 *          - values:
 *              - 0x00 - Allow display of IMSI
 *              - 0x01 - Do not display IMSI
 *          - function SLQSGetCustFeatures() returns a default value FF
 *            if no value is returned by the modem
 *
 *  \param  pIPFamSupport
 *          - optional 2 byte BitMask
 *          - bitmask representing the IP families supported
 *          - values:
 *              - 0x01 - IPv4
 *              - 0x02 - IPv6
 *              - 0x04 - IPv4v6
 *          - function SLQSGetCustFeatures() returns a default value FFFF
 *            if no value is returned by the modem
 *
 *  \param  pRMAutoConnect
 *          - optional 1 byte parameter
 *          - QMI Mode RM Net AutoConnect Support
 *          - values:
 *              - 0x00 - Not Supported
 *              - 0x01 - Supported
 *          - function SLQSGetCustFeatures() returns a default value FF
 *            if no value is returned by the modem
 *
 *  \param  pGPSSel
 *          - optional 1 byte parameter
 *          - GPS Antenna Select
 *          - values:
 *              - 0x00 - Dedicated GPS Port
 *              - 0x01 - GPS Rx over AUX Port
 *              - 0x02 - GPS Rx over dedicated GPS port with no bias
 *                       voltage applied
 *          - function SLQSGetCustFeatures() returns a default value FF
 *            if no value is returned by the modem
 *
 */
typedef struct custFeaturesInfo
{
    ULONG GpsEnable;
    BYTE  *pDisableIMSI;
    WORD  *pIPFamSupport;
    BYTE  *pRMAutoConnect;
    BYTE  *pGPSSel;
}custFeaturesInfo;

/**
 *  This API fetches the current settings of custom features
 *
 *  \param  pCustFeaturesInfo
 *          - Structure containing settings of custom features.
 *          - See \ref custFeaturesInfo for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: All\n
 *          Device Supported: MC7700/10/50\n
 *          Timeout: 2 Secs
 *
 */
ULONG SLQSGetCustFeatures(
    custFeaturesInfo *pCustFeaturesInfo );

/**
 *  This structure contains settings to be used for custom features
 *
 *  \param  pGPSSel
 *          - optional 1 byte parameter
 *          - GPS Antenna Select
 *          - values:
 *              - 0x00 - Dedicated GPS Port
 *              - 0x01 - GPS Rx over AUX Port
 *              - 0x02 - GPS Rx over dedicated GPS port with no bias
 *                       voltage applied
 *
 *  \param  pGPSEnable
 *          - optional 4 byte parameter
 *          - GPS Enable/Disable
 *          - values: The value of 7 least significant bits:
 *              - 0 - Disabled
 *              - 1 - MT & MO enabled
 *              - 2 - MO enabled
 *              - 3 - MT enabled
 *              - 4 - MT & MO enabled if GPS_DISABLE pin is not asserted
 *              - 5 - MO GPS enabled if GPS_DISABLE pin is not asserted
 *              - 6 - MT GPS enabled if GPS_DISABLE pin is not asserted
 *
 *  \note   Only MC7750 3.5.x firmware supports above 0x04, 0x05 and 0x06
 *          settings. To disable GLONASS, set the most significant bit - 0x80.
 *          This setting is only applicable if GPS is not Disabled.
 *
 */
typedef struct custFeaturesSetting
{
    BYTE *pGPSSel;
    ULONG *pGPSEnable;
}custFeaturesSetting;

/**
 *  This API changes the settings of custom features
 *
 *  \param  pCustFeaturesSetting
 *          - Structure containing settings of custom features.
 *          - See \ref custFeaturesSetting for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: All\n
 *          Device Supported: MC7700/10/50\n
 *          Timeout: 2 Secs
 *
 */
ULONG SLQSSetCustFeatures(
    custFeaturesSetting *pCustFeaturesSetting );

/**
 *  This structure contains GetCurrentPRLInfo response parameter
 *
 *  \param  pPRLVersion[OUT] - Optional
 *          - PRL version of device.
 *
 *  \param  pPRLPreference [OUT]- OptionaL
 *          - PRL Preference
 *              - 0 - Unset
 *              - 1 - Set
 */
typedef struct dmsCurrentPRLInfo
{
    WORD *pPRLVersion;
    BYTE *pPRLPreference;
} dmsCurrentPRLInfo;

/**
 *  This API get the currently active PRL information of the device.
 *
 *  \param  pCurrentPRLInfo
 *          - Pointer to structure dmsCurrentPRLInfo
 *          - See \ref dmsCurrentPRLInfo for more information
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Device Supported: MC83x5, MC7750\n
 *          Timeout: 2 Secs
 *
 */
ULONG SLQSGetCurrentPRLInfo(
    dmsCurrentPRLInfo *pCurrentPRLInfo );

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __GOBI_API_DMS_H__ */
