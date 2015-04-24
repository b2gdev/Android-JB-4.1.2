/**
 * \ingroup rms
 *
 * \file    qaGobiApiRms.h
 *
 * \brief   Remote Management Service API function prototypes
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __GOBI_API_RMS_H__
#define __GOBI_API_RMS_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  Queries the state of the SMS wake functionality. When enabled SMS wake
 *  functionality results in incoming messages being searched for the
 *  configured mask. Upon detection of the mask the incoming message is deleted
 *  (i.e. not stored in memory) and the device attempts to wake the host
 *  (requires host platform support).
 *
 *  \param  pEnabled[OUT]
 *          - SMS wake functionality enabled
 *              - 0 - Disabled
 *              - 1 - Enabled
 *
 *  \param  pWakeMask[OUT]
 *          - SMS wake mask to search for incoming messages
 *            (only relevant when enabled)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\UMTS\n
 *          Device Supported: MC83x5\n
 *          Timeout: 2 seconds\n
 *
 */
ULONG GetSMSWake(
    ULONG *pEnabled,
    ULONG *pWakeMask );

/**
 *  Configures the SMS wake functionality. When enabled SMS wake
 *  functionality results in incoming messages being searched for the
 *  configured mask. Upon detection of the mask the incoming message is deleted
 *  (i.e. not stored in memory) and the device attempts to wake the host
 *  (requires host platform support).
 *
 *  \param  bEnable
 *          - Enable SMS wake functionality
 *              - Zero     - Disable
 *              - Non-Zero - Enable
 *
 *  \param  wakeMask
 *          - SMS wake mask to search for incoming messages
 *            (only relevant when enabling)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\UMTS\n
 *          Device Supported: MC83x5\n
 *          Timeout: 2 seconds\n
 *
 */
ULONG SetSMSWake(
    ULONG bEnable,
    ULONG wakeMask );

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __GOBI_API_RMS_H__ */
