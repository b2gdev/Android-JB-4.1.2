 /**
 * \ingroup cbk
 *
 * \file    qaWdsSetEventReport.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaWdsSetEventReport.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __WDS_SET_EVENT_REPORT_H__
#define __WDS_SET_EVENT_REPORT_H__

#include "qaQmiNotify.h"

/* Added according to the revision in Gobi Addendum 3000 */
#define STATUS_MASK_TX_BYTES_OK 0x00000040
#define STATUS_MASK_RX_BYTES_OK 0x00000080

/**
 *
 * An enumeration of eQMI_DMS_GET_DEVICE_SERIAL_NUMBERS response TLV IDs
 */
enum eWDS_SET_EVENT_REPORT_REQ
{
    eTLV_DATA_BEARER_TECH           = 0x12,
    eTLV_DORMANCY_STATUS_REQ        = 0x13,
    eTLV_MOBILE_IP                  = 0x14,
    eTLV_TRANSFER_STATISTICS        = 0x11,
    eTLV_CURRENT_DATA_BEARER_TECH   = 0x15
};

/**
 *
 * This structure contains the WDS Set Evet Status request parameters
 *
 * \param  eDataBearer[IN]
 *         - Data Bearer Technology Indicator
 *
 * \param  eDormancyStatus[IN]
 *         - Dormancy Status Indicator
 *
 * \param  eMobileIP[IN]
 *         - MIP Status Indicator
 *
 * \param  eByteTotals[IN]
 *         - Byte Totals Indicator
 *
 * \param  interval[IN]
 *         - period between transfer statistics reports
 *
 * \param  eCurrentDataBearer[IN]
 *         - Current Data Bearer Technology Indicator
 *
 */
struct QmiCbkWdsSetEventReportReq
{
    /* Parameters to be passed in the WDS_SET_EVET_REQ structure */
    enum eQmiCbkSetStatus eDataBearer;
    enum eQmiCbkSetStatus eDormancyStatus;
    enum eQmiCbkSetStatus eMobileIP;
    enum eQmiCbkSetStatus eByteTotals;
    enum eQmiCbkSetStatus eCurrentDataBearer;
    BYTE                  interval;
};

/**
 * This structure contains the WDS Set Event Report
 * response parameters
 */
struct QmiCbkWdsSetEventReportResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiCbkWdsSetEventReport(
    WORD                  *pMlength,
    enum eQmiCbkSetStatus eDataBearer,
    enum eQmiCbkSetStatus eDormancyStatus,
    enum eQmiCbkSetStatus eMobileIP,
    enum eQmiCbkSetStatus eByteTotals,
    enum eQmiCbkSetStatus eCurrentDataBearer,
    BYTE                  interval,
    BYTE                  *pBuffer );

extern enum eQCWWANError UpkQmiCbkWdsSetEventReport(
    BYTE   *mdmresp,
    struct QmiCbkWdsSetEventReportResp *apiresp);

#endif /* __WDS_SET_EVENT_REPORT_H__ */
