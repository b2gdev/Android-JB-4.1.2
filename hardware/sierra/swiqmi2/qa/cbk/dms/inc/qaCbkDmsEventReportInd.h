/*
 * \ingroup cbk
 *
 * \file qaCbkDmsEventReportInd.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaCbkDmsEventReportInd.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DMS_EVENT_REPORT_IND_H__
#define __DMS_EVENT_REPORT_IND_H__

/*
 * An enumeration of eQMI_DMS_EVENT_REPORT_IND response TLV IDs
 *
 */
enum eQMI_DMS_EVENT_REPORT_IND
{
    eTLV_IND_POWER_STATE            = 0x10,
    eTLV_IND_BATTERY_LEVEL          = 0x11,
    eTLV_IND_PIN_STATE              = 0x12,
    eTLV_IND_ACTIVATION_STATUS      = 0x13,
    eTLV_IND_OPERATING_MODE         = 0x14,
    eTLV_IND_UIM_STATE              = 0x15,
    eTLV_IND_WIRELESS_DISABLE_STATE = 0x16
};

/*
 * eQMI_DMS_EVENT_IND TLVs defined below
 */

/*
 * Name:    OperatingModeTlv
 *
 * Purpose: Structure used to store all Operating Mode TLV Value.
 *
 * Members: TlvPresent      - Boolean indicating the presence of the TLV
 *                            in the QMI response
 *
 *          OperatingMode   - Operating Mode Value
 */
struct OperatingModeTlv
{
    BYTE    TlvPresent;
    ULONG   operatingMode;
};

/*
 * Name:    ActivationStatusTlv
 *
 * Purpose: Structure used to store all Operating Mode TLV Value.
 *
 * Members: TlvPresent       - Boolean indicating the presence of the TLV
 *                             in the QMI response
 *
 *          activationStatus - Activation Status Value
 */
struct ActivationStatusTlv
{
    BYTE    TlvPresent;
    ULONG   activationStatus;
};

/*
 * Name:    QmiCbkDmsEventStatusReportInd
 *
 * Purpose: Structure used to store all Data Bearer Notification Parameters.
 *
 * Members: OMTlv - Oparating Mode Tlv
 *          ASTlv - Activation Status Tlv
 *
 * Note:    None
 */
struct QmiCbkDmsEventStatusReportInd{
    struct OperatingModeTlv    OMTlv;
    struct ActivationStatusTlv ASTlv;
};

/*
 * Prototypes
 *
 */
enum eQCWWANError UpkQmiCbkDmsEventReportInd(
    BYTE                                 *pMdmResp,
    struct QmiCbkDmsEventStatusReportInd *pAipResp );

#endif /* __DMS_EVENT_REPORT_IND_H__ */

