/**
 * \ingroup cat
 *
 * \file    qaGobiApiCat.h
 *
 * \brief   Card Application Toolkit API function headers
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __GOBI_API_CAT_H__
#define __GOBI_API_CAT_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 *   Sends the envelope command to the device.
 *
 *  \param  cmdID
 *          - Envelope command type
 *              - 0x01 - Menu Selection
 *              - 0x02 - Event DL User activity
 *              - 0x03 - Event DL Idle Screen Available
 *              - 0x04 - Event DL Language Selection
 *
 *  \param  dataLen
 *          - Length of pData in bytes
 *
 *  \param  pData[IN]
 *          - Encoded envelope data as defined in ETSI TS 102 223, section 7\n
 *            [Smart Cards: Card Application Toolkit (CAT) – Release 4]
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa      See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: MC83x5\n
 *          Timeout: 2 seconds\n
 */
ULONG CATSendEnvelopeCommand(
    ULONG cmdID,
    ULONG dataLen,
    BYTE  *pData );

/**
 *  Sends the terminal response to the device.
 *
 *  \param  refID
 *          - Proactive command reference ID. The value should be the same as
 *            indicated in the CAT event callback data for the relevant
 *            proactive command.
 *
 *  \param  dataLen
 *          - Terminal response data length
 *
 *  \param  pData[IN]
 *          - Terminal response for the relevant proactive command encoded as
 *            per ETSI TS 102 223, section 6.8\n
 *            [Smart Cards: Card Application Toolkit (CAT) – Release 4]
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 *  \sa     See qmerrno.h for eQCWWAN_xxx error values
 *
 *  \note   Technology Supported: UMTS\n
 *          Device Supported: MC83x5\n
 *          Timeout: 2 seconds\n
 */
ULONG CATSendTerminalResponse(
    ULONG refID,
    ULONG dataLen,
    BYTE  *pData );

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __GOBI_API_CAT_H__ */
