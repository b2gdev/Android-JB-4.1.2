 /*
 * \ingroup cbk
 *
 * \file    qaPdsSetEventReport.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaPdsSetEventReport.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __PDS_SET_EVENT_REPORT_H__
#define __PDS_SET_EVENT_REPORT_H__

#include "qaQmiNotify.h"

/*
 *
 * An enumeration of eQMI_PDS_SET_EVENT_REPORT request TLV IDs
 *
 */
enum ePDS_SET_EVENT_REPORT_REQ
{
    eTLV_POSITION_DATA_NMEA_REQ             = 0x10,
    eTLV_PARSED_POSITION_DATA               = 0x12,
    eTLV_EXTRA_DATA_REQUEST                 = 0x13,
    eTLV_TIME_INJECTION_REQUEST             = 0x14,
    eTLV_WIFI_POSITION                      = 0x15,
    eTLV_SATELITE_INFORMATION               = 0x16,
    eTLV_NETWORK_INTIATED_PROMPT_VX         = 0x17,
    eTLV_NETWORK_INTIATED_PROMPT_SUPL       = 0x18,
    eTLV_NETWORK_INTIATED_PROMPT_UMTS_CP    = 0x19,
    eTLV_PDS_COMMON_EVENTS                  = 0x1A,
};

/*
 * This structure contains the PDS Set Evet Status request parameters
 */
struct QmiCbkPdsSetEventReportReq
{
    /* Parameters to be passed in the SET_EVENT_REQ structure */
    enum eQmiCbkSetStatus ePositionDataNMEA;
    enum eQmiCbkSetStatus eParsedPositionData;
};

/*
 * This structure contains the PDS Set Event Report response parameters
 */
struct QmiCbkPdsSetEventReportResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiCbkPdsSetEventReport(
    WORD                  *pMlength,
    enum eQmiCbkSetStatus ePositionDataNMEA,
    enum eQmiCbkSetStatus eParsedPositionData,
    BYTE                  *pBuffer );

extern enum eQCWWANError UpkQmiCbkPdsSetEventReport(
    BYTE   *mdmresp,
    struct QmiCbkPdsSetEventReportResp *apiresp);

#endif /* __PDS_SET_EVENT_REPORT_H__ */
