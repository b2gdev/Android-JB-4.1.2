/*
 * \ingroup cbk
 *
 * \file qaNasSetEventReport.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaNasSetEventReport.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_SET_EVENT_REPORT_H__
#define __NAS_SET_EVENT_REPORT_H__

#include "qaQmiNotify.h"

/*
 * An enumeration of eQMI_NAS_SET_EVENT_REPORT response TLV IDs
 *
 */
enum eNAS_SET_EVENT_REPORT_REQ
{
    eTLV_SIGNAL_STRENGTH      = 0x10,
    eTLV_RF_BAND              = 0x11,
    eTLV_REG_REJECT_REASON    = 0x12,
    eTLV_RSSI_INDICATOR       = 0x13,
    eTLV_ECIO_INDICATOR       = 0x14,
    eTLV_IO_INDICATOR         = 0x15,
    eTLV_SNIR_INDICATOR       = 0x16,
    eTLV_ERROR_RATE_INDICATOR = 0x17,
    eTLV_RSRQ_INDICATOR       = 0x18,
    eTLV_ECIO_TH_INDICATOR    = 0x19,
    eTLV_SINR_TH_INDICATOR    = 0x1A,
};

/*
 * This structure contains the NAS Set Event Status request parameters
 *
 * \param eSignalStrength
 *        - Signal Strength Indicator
 *
 * \param thresholdsSize
 *        - Number of thresholds following a maximum of five thresholds
 *        may be specified; if report_signal_strength is set, at least
 *        one threshold must be specified
 *
 * \param pThresholds
 *        - A sequence of thresholds delimiting signal strength bands;
 *        each threshold specifies the signal strength (in dBm) at
 *        which an event report indication, including the current
 *        signal strength, will be sent to the requesting control point.\n
 *        Threshold is a signed, 1 byte value; valid values are -128 dBm
 *        to +127 dBm
 *
 * \param eRFBand
 *        - RF Band Information
 *
 * \param eRegRejectReason
 *        - Registration Reject Reasons
 *
 * \param eRSSIIndicator
 *        - RSSI Indicator
 *
 * \param eECIOIndicator
 *        - ECIO Indicator
 *
 * \param eIOIndicator
 *        - IO Indicator
 *
 * \param eSNIRIndicator
 *        - SINR Indicator
 *
 * \param eErrorRateIndicator
 *        - Error Rate Indicator
 *
 * \param eRSRQIndicator
 *        - RSRQ Indicator
 *
 * \param eECIOThIndicator
 *        - ECIO Threshold Indicator
 *
 * \param eSINRThIndicator
 *        - SINR Threshold Indicator
 *
 * \param pSLQSSignalStrengthsIndReq
 *        - See \ref SLQSSignalStrengthsIndReq for more information
 *
 */
struct QmiCbkNasSetEventReportReq
{
    /* Parameters to be passed in the SET_EVENT_REQ structure */
    enum eQmiCbkSetStatus            eSignalStrength;
    BYTE                             thresholdsSize;
    INT8                             *pThresholds;
    enum eQmiCbkSetStatus            eRFBand;
    enum eQmiCbkSetStatus            eRegRejectReason;
    enum eQmiCbkSetStatus            eRSSIIndicator;
    enum eQmiCbkSetStatus            eECIOIndicator;
    enum eQmiCbkSetStatus            eIOIndicator;
    enum eQmiCbkSetStatus            eSNIRIndicator;
    enum eQmiCbkSetStatus            eErrorRateIndicator;
    enum eQmiCbkSetStatus            eRSRQIndicator;
    enum eQmiCbkSetStatus            eECIOThIndicator;
    enum eQmiCbkSetStatus            eSINRThIndicator;
    struct SLQSSignalStrengthsIndReq *pSLQSSignalStrengthsIndReq;
};

/*
 * This structure contains the NAS Set Event Report
 * response parameters
 *
 */
struct QmiCbkNasSetEventReportResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 *
 */
extern enum eQCWWANError PkQmiCbkNasSetEventReport(
    WORD                             *pMlength,
    enum eQmiCbkSetStatus            eSignalStrength,
    BYTE                             thresholdsSize,
    INT8                             *pThresholds,
    enum eQmiCbkSetStatus            eRFBand,
    enum eQmiCbkSetStatus            eRegRejectReason,
    enum eQmiCbkSetStatus            eRSSIIndicator,
    enum eQmiCbkSetStatus            eECIOIndicator,
    enum eQmiCbkSetStatus            eIOIndicator,
    enum eQmiCbkSetStatus            eSNIRIndicator,
    enum eQmiCbkSetStatus            eErrorRateIndicator,
    enum eQmiCbkSetStatus            eRSRQIndicator,
    enum eQmiCbkSetStatus            eECIOThIndicator,
    enum eQmiCbkSetStatus            eSINRThIndicator,
    struct SLQSSignalStrengthsIndReq *pSLQSSignalStrengthsIndReq,
    BYTE                             *pBuffer );

extern enum eQCWWANError UpkQmiCbkNasSetEventReport(
    BYTE                               *pMdmResp,
    struct QmiCbkNasSetEventReportResp *pApiResp );

#endif /* __NAS_SET_EVENT_REPORT_H__ */
