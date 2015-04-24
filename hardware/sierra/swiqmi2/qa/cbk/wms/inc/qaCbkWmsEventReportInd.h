/*
 * \ingroup cbk
 *
 * \file    qaCbkWmsEventReportInd.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaCbkWmsEventReportInd.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __WMS_EVENT_REPORT_IND_H__
#define __WMS_EVENT_REPORT_IND_H__

/* An enumeration of eQMI_WMS_EVENT_REPORT_IND response TLV IDs */
enum eQMI_WMS_EVENT_REPORT_IND_TLV
{
    eTLV_MT_MESSAGE                = 0x10,
    eTLV_TRANSFER_ROUTE_MT_MESSAGE = 0x11,
    eTLV_MESSAGE_MODE              = 0x12,
    eTLV_MESSAGE_ETWS              = 0x13,
    eTLV_MESSAGE_ETWS_PLMN         = 0x14,
    eTLV_MESSAGE_SMSC              = 0x15,
    eTLV_MESSAGE_SMS_ON_IMS        = 0x16,
};

/* An enumeration of eQMI_WMS_EVENT_REPORT_IND response TLV Lengths */
/* From the QMI Document */
enum eQMI_WMS_EVENT_REPORT_IND_TLV_LENGTH
{
    eTLV_MT_MESSAGE_LENGTH          = 0x05,
    eTLV_MESSAGE_MODE_LENGTH        = 0x01,
    eTLV_MESSAGE_SMSC_LENGTH        = 0x08,
    eTLV_MESSAGE_ETWS_PLMN_LENGTH   = 0x04,
    eTLV_MESSAGE_SMS_ON_IMS_LENGTH  = 0x16,
};

/*
 * eQMI_WMS_EVENT_IND TLVs defined below
 */


/*
 * Name:    NewMTMessageTlv
 *
 * Purpose: Structure used to store all New Message TLV Value.
 *
 * Members: TlvPresent   - Boolean indicating the presence of the TLV
 *                         in the QMI response
 *
 *          MTMessageInfo - Refer to SMSMTMessageInfo
 */
struct NewMTMessageTlv
{
    BYTE    TlvPresent;
    SMSMTMessageInfo MTMessageInfo;
};

/*
 * Name:    TransferRouteMessageTlv
 *
 * Purpose: Structure used to store all transfer route message TLV value.
 *
 * Members: TlvPresent   - Boolean indicating the presence of the TLV
 *                         in the QMI response
 *
 *          TransferRouteMTMessageInfo - Refer to SMSTransferRouteMTMessageInfo
 *
 */
struct TransferRouteMessageTlv
{
    BYTE TlvPresent;
    SMSTransferRouteMTMessageInfo TransferRouteMTMessageInfo;
};

/*
 * Name:    MessageModeTlv
 *
 * Purpose: Structure used to store message mode TLV value.
 *
 * Members: TlvPresent - Boolean indicating the presence of the TLV
 *                       in the QMI response
 *
 *          MessageModeInfo - Refer to SMSMessageModeInfo
 *
 */
struct MessageModeTlv
{
    BYTE TlvPresent;
    SMSMessageModeInfo MessageModeInfo;
};

/*
 * Name:    SMSEtwsMessageTlv
 *
 * Purpose: Structure used to store all ETWS TLV Value.
 *
 * Members: TlvPresent      - Boolean indicating the presence of the TLV
 *                            in the QMI response
 *
 *          EtwsMessageInfo - Refer to SMSEtwsMessageInfo
 */
struct SMSEtwsMessageTlv
{
    BYTE    TlvPresent;
    SMSEtwsMessageInfo EtwsMessageInfo;
};

/*
 * Name:    ETWSPLMNInfoTlv
 *
 * Purpose: Structure used to store all ETWS PLMN TLV Value.
 *
 * Members: TlvPresent   - Boolean indicating the presence of the TLV
 *                          in the QMI response
 *
 *          ETWSPLMNInfo - Refer to SMSEtwsPlmnInfo
 */
struct ETWSPLMNInfoTlv
{
    BYTE    TlvPresent;
    SMSEtwsPlmnInfo ETWSPLMNInfo;
};

/*
 * Name:    SMSCAddressTlv
 *
 * Purpose: Structure used to store SMSC address Value.
 *
 * Members: TlvPresent    - Boolean indicating the presence of the TLV
 *                          in the QMI response
 *
 *          MTMessageInfo - Refer to SMSEtwsMessageInfo
 */
struct SMSCAddressTlv
{
    BYTE            TlvPresent;
    SMSCAddressInfo SMSCInfo;
};

/*
 * Name:    SMSOnIMSTlv
 *
 * Purpose: Structure used to store SMS on IMS TLV Value.
 *
 * Members: TlvPresent    - Boolean indicating the presence of the TLV
 *                          in the QMI response
 *
 *          IMSInfo       - Refer to SMSOnIMSInfo
 */
struct SMSOnIMSTlv
{
    BYTE         TlvPresent;
    SMSOnIMSInfo IMSInfo;
};

/*
 * Name:    QmiCbkWmsEventReportInd
 *
 * Purpose: Structure used to store all WMS Event Indication Parameters.
 *
 * Members: NewMMTlv - Structure containing the Data Bearer
 *                              Technology TLV
 * Note:    None
 */
struct QmiCbkWmsEventReportInd{
    struct NewMTMessageTlv         NewMMTlv;
    struct TransferRouteMessageTlv TRMessageTlv;
    struct MessageModeTlv          MMTlv;
    struct SMSEtwsMessageTlv       ETWSTlv;
    struct ETWSPLMNInfoTlv         ETWSPLMNTlv;
    struct SMSCAddressTlv          SMSCTlv;
    struct SMSOnIMSTlv             IMSTlv;
};

/*************
 * Prototypes
 **************/
enum eQCWWANError UpkQmiCbkWmsEventReportInd (
    BYTE                           *pMdmResp,
    struct QmiCbkWmsEventReportInd *pAipResp );

#endif /* __WMS_EVENT_REPORT_IND_H__ */

