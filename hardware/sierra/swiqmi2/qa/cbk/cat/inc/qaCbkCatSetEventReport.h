 /*
 * \ingroup cbk
 *
 * \file    qaCbkCatSetEventReport.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaCbkCatSetEventReport.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __CAT_SET_EVENT_REPORT_H__
#define __CAT_SET_EVENT_REPORT_H__

#include "qaQmiNotify.h"

/*
 *
 * An enumeration of eQMI_CAT_SET_EVENT_REPORT request TLV IDs
 *
 */
enum eCAT_SET_EVENT_REPORT_REQ
{
    eTLV_EVENT_REPORT_REQ = 0x10,
};

/*
 * This structure contains the CAT Set Event Status request parameters
 */
struct QmiCbkCatSetEventReportReq
{
    /* Parameters to be passed in the SET_EVENT_REQ structure */
    ULONG eventMask;
};

/*
 *
 * An enumeration of eQMI_CAT_SET_EVENT_REPORT Response TLV IDs
 *
 */
enum eCAT_SET_EVENT_REPORT_RESP
{
    eTLV_EVENT_REPORT_REG_STATUS = 0x10,
};

/*
 *
 * An enumeration of eQMI_CAT_SET_EVENT_REPORT Response TLV ID Lengths
 *
 */
enum eCAT_SET_EVENT_REPORT_RESP_TLV_LENGTH
{
    eTLV_EVENT_REPORT_REG_STATUS_LENGTH = 0x04,
};

/*
 * This structure contains the CAT Set Event Report response parameters
 */
struct QmiCbkCatSetEventReportResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
    ULONG              *pErrorMask;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiCbkCatSetEventReport(
    WORD  *pMlength,
    ULONG eventMask,
    BYTE  *pBuffer );

extern enum eQCWWANError UpkQmiCbkCatSetEventReport(
    BYTE   *mdmresp,
    struct QmiCbkCatSetEventReportResp *apiresp);

#endif /* __CAT_SET_EVENT_REPORT_H__ */
