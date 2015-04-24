/*
 * \internal
 * \ingroup cbk
 *
 * \file qaCbkOmaDmEventReportInd.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaCbkOmaDmEventReportInd.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __OMA_DM_EVENT_REPORT_IND_H__
#define __OMA_DM_EVENT_REPORT_IND_H__

/*
 * \internal
 * An enumeration of eQMI_OMA_DM_EVENT_REPORT_IND response TLV IDs
 *
 */
enum eQMI_OMA_DM_EVENT_REPORT_IND
{
    eTLV_IND_OMA_DM_SessionState  = 0x11,
    eTLV_IND_OMA_DM_FailureReason = 0x12
};

/*
 * \internal
 * eQMI_OMA_DM_EVENT_IND TLVs defined below
 */

/*
 * This structure will hold the Oma-Dm SessionState and OMA-DM Extended 
 * Session Info parameters information.
 *
 *  \param  sessionState
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
 *  \param  sessionFailReason
 *          - Session failure reason (when state indicates failure)
 *              - 0x00 - Unknown
 *              - 0x01 - Network is unavailable
 *              - 0x02 - Server is unavailable
 *              - 0x03 - Authentication failed
 *              - 0x04 - Maximum retry exceeded
 *              - 0x05 - Session is cancelled
 *
 *  \param  TlvPresent - Boolean indicating the presence of the TLV
 *                       in the QMI response
 */
struct sessionStateTlv
{
    BYTE  TlvPresent;
    ULONG sessionState;
    ULONG sessionFailReason;
};

/*
 * \internal
 * Name:    QmiCbkOmaDmEventStatusReportInd
 *
 * Purpose: Structure used to store Session State TLV.
 *
 * Members: sessionStateTlv - SessionState and Extended Session Info Tlv
 *          
 * Note:    None
 */
struct QmiCbkOmaDmEventStatusReportInd
{
    struct sessionStateTlv sSTlv;
};

/*
 * \internal
 * Prototypes
 *
 */
enum eQCWWANError UpkQmiCbkOmaDmEventReportInd(
    BYTE                                   *pMdmResp,
    struct QmiCbkOmaDmEventStatusReportInd *pApiResp );

#endif /* __OMA_DM_EVENT_REPORT_IND_H__ */
