/**
 * \ingroup oma
 *
 * \file    qaGobiApiOmadm.h
 *
 * \brief   Open Mobile Alliance Device Management Service API function
 *          prototypes
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __GOBI_API_OMADM_H__
#define __GOBI_API_OMADM_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  Starts an OMA-DM session.
 *
 *  \param  sessionType
 *          - Session type
 *              - 0x00 - Client-initiated device configure
 *              - 0x01 - Client-initiated PRL update
 *              - 0x02 - Client-initiated hands-free activation
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Device Supported: MC83x5\n
 *          Timeout: 2 seconds
 */
ULONG OMADMStartSession(
    ULONG sessionType );

/**
 *  Cancels an ongoing OMA-DM session.
 *
 *  \param  None
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Device Supported: MC83x5\n
 *          Timeout: 2 seconds
 */
ULONG OMADMCancelSession();

/**
 *  Returns information related to the current (or previous if no session is
 *  active) OMA-DM session.
 *
 *  \param  SessionState[OUT]
 *          - Session state
 *              - 0x00 - Complete, information was updated
 *              - 0x01 - Complete, update information is unavailable
 *              - 0x02 - Failed
 *              - 0x03 - Retrying
 *              - 0x04 - Connecting
 *              - 0x05 - Connected
 *              - 0x06 - Authenticated
 *              - 0x07 - Mobile Directory Number (MDN) downloaded
 *              - 0x08 - Mobile Station Identifier (MSID) downloaded
 *              - 0x09 - PRL downloaded
 *              - 0x0A - Mobile IP (MIP) profile downloaded
 *
 *  \param  sessionType[OUT]
 *          - Session State
 *              - 0x00 - Client-initiated device configure
 *              - 0x01 - Client-initiated PRL update
 *              - 0x02 - Client-initiated hands-free activation
 *              - 0x03 - Device-initiated hands-free activation
 *              - 0x04 - Network-initiated PRL update
 *              - 0x05 - Network-initiated device configure
 *
 *  \param   FailureReason[OUT]
 *          - Session failure reason (when state indicates failure)
 *              - 0x00 - Unknown
 *              - 0x01 - Network is unavailable
 *              - 0x02 - Server is unavailable
 *              - 0x03 - Authentication failed
 *              - 0x04 - Maximum retry exceeded
 *              - 0x05 - Session is cancelled
 *
 *  \param   RetryCount[OUT]
 *          - Session retry count (when state indicates retrying)
 *
 *  \param   SessionPause[OUT]
 *          - Session pause timer (in seconds , when state indicates retrying)
 *
 *  \param   TimeRemaining[OUT]
 *          - Pause time remaining (in seconds , when state indicates retrying)
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Device Supported: MC83x5\n
 *          Timeout: 2 seconds
 */
ULONG OMADMGetSessionInfo(
    ULONG *pSessionState,
    ULONG *pSessionType,
    ULONG *pFailureReason,
    BYTE  *pRetryCount,
    WORD  *pSessionPause,
    WORD  *pTimeRemaining );

/**
 *  Returns information about the pending network-initiated alert
 *
 *  \param SessionType[OUT]
 *         - Session Type
 *             - 0x04 - Network-initiated PRL update
 *             - 0x05 - Network-initiated device configure
 *
 *  \param SessionID[OUT]
 *         - Session Id
 *             - Unique session ID for NIA request
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: CDMA\n
 *          Device Supported: MC83x5\n
 *          Timeout: 2 seconds
 */
ULONG OMADMGetPendingNIA(
    ULONG  *pSessionType,
    USHORT *pSessionID );

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __GOBI_API_OMADM_H__ */
