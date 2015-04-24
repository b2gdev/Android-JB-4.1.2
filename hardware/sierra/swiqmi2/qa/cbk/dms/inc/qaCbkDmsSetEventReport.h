/*
 * \ingroup cbk
 *
 * \file qaDmsSetEventReport.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaDmsSetEventReport.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DMS_SET_EVENT_REPORT_H__
#define __DMS_SET_EVENT_REPORT_H__

#include "qaQmiNotify.h"

/*
 * An enumeration of eQMI_DMS_SET_EVENT_REPORT response TLV IDs
 *
 */
enum eDMS_SET_EVENT_REPORT_REQ
{
    eTLV_POWER_STATE            = 0x10,
    eTLV_BATTERY_LEVEL          = 0x11,
    eTLV_PIN_STATE              = 0x12,
    eTLV_ACTIVATION_STATUS      = 0x13,
    eTLV_OPERATING_MODE         = 0x14,
    eTLV_UIM_STATE              = 0x15,
    eTLV_WIRELESS_DISABLE_STATE = 0x16
};

/*
 * This structure contains the DMS Set Event Status request parameters
 *
 * \param ePowerState
 *        - Power state reporting
 *
 * \param eBatteryLevel
 *        - Battery Level Report Limits
 *
 * \param ePinState
 *        - PIN State Reporting
 *
 * \param eActivationStatus
 *        - Activation State Reporting
 *
 * \param eOperatingMode
 *        - Operating Mode Reporting
 *
 * \param eUIMState
 *        - UIM State Reporting
 *
 * \param eWirelessDisableState
 *        - Wireless Disable State Reporting
 *
 */
struct QmiCbkDmsSetEventReportReq
{
    /* Parameters to be passed in the SET_EVENT_REQ structure */
    enum eQmiCbkSetStatus ePowerState;
    enum eQmiCbkSetStatus eBatteryLevel;
    enum eQmiCbkSetStatus ePinState;
    enum eQmiCbkSetStatus eActivationStatus;
    enum eQmiCbkSetStatus eOperatingMode;
    enum eQmiCbkSetStatus eUIMState;
    enum eQmiCbkSetStatus eWirelessDisableState;
};

/*
 * This structure contains the DMS Set Event Report
 * response parameters
 *
 */
struct QmiCbkDmsSetEventReportResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 *
 */
extern enum eQCWWANError PkQmiCbkDmsSetEventReport(
    WORD                  *pMlength,
    enum eQmiCbkSetStatus ePowerState,
    enum eQmiCbkSetStatus eBatteryLevel,
    enum eQmiCbkSetStatus ePinState,
    enum eQmiCbkSetStatus eActivationStatus,
    enum eQmiCbkSetStatus eOperatingMode,
    enum eQmiCbkSetStatus eUIMState,
    enum eQmiCbkSetStatus eWirelessDisableState,
    BYTE                  *pBuffer );

extern enum eQCWWANError UpkQmiCbkDmsSetEventReport(
    BYTE                               *pMdmResp,
    struct QmiCbkDmsSetEventReportResp *pApiResp );

#endif /* __DMS_SET_EVENT_REPORT_H__ */
