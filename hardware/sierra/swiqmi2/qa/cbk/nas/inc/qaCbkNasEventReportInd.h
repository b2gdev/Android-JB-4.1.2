/*
 * \ingroup cbk
 *
 * \file qaCbkNasEventReportInd.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaCbkNasEventReportInd.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_EVENT_REPORT_IND_H__
#define __NAS_EVENT_REPORT_IND_H__

#include "qaGobiApiCbk.h"
/*
 * An enumeration of eQMI_NAS_EVENT_REPORT_IND response TLV IDs
 *
 */
enum eQMI_NAS_EVENT_REPORT_IND
{
    eTLV_IND_SIGNAL_STRENGTH    = 0x10,
    eTLV_IND_RF_BAND            = 0x11,
    eTLV_IND_REG_REJECT_REASON  = 0x12,
    eTLV_IND_RSSI               = 0x13,
    eTLV_IND_ECIO               = 0x14,
    eTLV_IND_IO                 = 0x15,
    eTLV_IND_SINR               = 0x16,
    eTLV_IND_ERROR_RATE         = 0x17,
    eTLV_IND_RSRQ               = 0x18
};

/*
 * eQMI_NAS_EVENT_IND TLVs defined below
 */

#define NAS_MAX_RI_SIZE 255
/*
 * Name:    RejectReasonTlv
 *
 * Purpose: Structure used to store all RFInfo Mode TLV Value.
 *
 * Members: TlvPresent      - Boolean indicating the presence of the TLV
 *                            in the QMI response
 *
 *          serviceDomain - Service domain : 1 - Circuit switched
 *
 *          rejectCause   - Reject cause
 */
struct RejectReasonTlv
{
    BYTE  TlvPresent;
    ULONG serviceDomain;
    ULONG rejectCause;
};

/*
 * Name:   SignalStrengthTlv
 *
 * Purpose: Structure used to store all SignalStrengthTlv Mode TLV Value.
 *

 * Members: TlvPresent     - Boolean indicating the presence of the TLV
 *                           in the QMI response
 *
 *          signalStrength - Received signal strength (in dBm)
 *
 *          radioInterface - Radio interface technology of the signal being
 *                           measured
 */
struct SignalStrengthTlv
{
    BYTE  TlvPresent;
    INT8  signalStrength;
    ULONG radioInterface;
};

/*
 * Name:   RFInfoTlv
 *
 * Purpose: Structure used to store all RFInfo Mode TLV Value.
 *
 * Members: TlvPresent      - Boolean indicating the presence of the TLV
 *                            in the QMI response
 *
 *          radioInterface  - Radio interface technology of the signal
 *                            being measured
 *          activeBandClass - Active band class
 *
 *          activeChannel   - Active channel,
 *                            0 - Channel is not relevant to the reported
 *                            technology
 *
 */
struct RFInfoTlv
{
    BYTE  TlvPresent;
    BYTE  radioInterfaceSize;
    ULONG radioInterface[NAS_MAX_RI_SIZE];
    ULONG activeBandClass[NAS_MAX_RI_SIZE];
    ULONG activeChannel[NAS_MAX_RI_SIZE];
};

/*
 * Name:   SLQSSignalStrengthsTlv
 *
 * Purpose: Structure used to store all SLQSSignalStrengths Mode TLV Value.
 *
 * Members: TlvPresent               - Boolean indicating the presence of the
 *                                     TLV in the QMI response.
 *
 *          sSLQSSignalStrengthsInfo - See SLQSSignalStrengthsInformation in
 *                                     qaGobiApiCbk.h for more information.
 *
 */
struct SLQSSignalStrengthsTlv
{
    BYTE                                  TlvPresent;
    struct SLQSSignalStrengthsInformation sSLQSSignalStrengthsInfo;
};

/*
 * Name:    QmiCbkNasEventStatusReportInd
 *
 * Purpose: Structure used to store all Data Bearer Notification Parameters.
 *
 * Members: SSTlv     - See SignalStrengthTlv for more info.
 *          RFTlv     - See RFInfoTlv for more info.
 *          RRTlv     - See RejectReasonTlv for more info.
 *          SLQSSSTlv - See SLQSSignalStrengthsTlv for more info.
 *
 * Note:    None
 */
struct QmiCbkNasEventStatusReportInd{
    struct SignalStrengthTlv      SSTlv;
    struct RFInfoTlv              RFTlv;
    struct RejectReasonTlv        RRTlv;
    struct SLQSSignalStrengthsTlv SLQSSSTlv;
};

/*
 * Prototypes
 *
 */
enum eQCWWANError UpkQmiCbkNasEventReportInd(
    BYTE                                 *pMdmResp,
    struct QmiCbkNasEventStatusReportInd *pApiResp );

#endif /* __NAS_EVENT_REPORT_IND_H__ */

