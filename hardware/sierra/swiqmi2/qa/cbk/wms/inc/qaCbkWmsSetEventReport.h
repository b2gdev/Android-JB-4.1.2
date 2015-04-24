 /*
 * \ingroup cbk
 *
 * \file    qaCbkWmsSetEventReport.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaCbkWmsSetEventReport.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __WMS_SET_EVENT_REPORT_H__
#define __WMS_SET_EVENT_REPORT_H__

#include "qaQmiNotify.h"

/*
 *
 * An enumeration of eQMI_WMS_SET_EVENT_REPORT request TLV IDs
 *
 */
enum eWMS_SET_EVENT_REPORT_REQ
{
    eTLV_NEW_MT_MESSAGE = 0x10,
};

/*
 * This structure contains the WMS Set Evet Status request parameters
 */
struct QmiCbkWmsSetEventReportReq
{
    /* Parameters to be passed in the SET_EVENT_REQ structure */
    enum eQmiCbkSetStatus eNewMTMessage;
};

/*
 * This structure contains the WMS Set Event Report response parameters
 */
struct QmiCbkWmsSetEventReportResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiCbkWmsSetEventReport(
    WORD                  *pMlength,
    enum eQmiCbkSetStatus eNewMTMessage,
    BYTE                  *pBuffer );

extern enum eQCWWANError UpkQmiCbkWmsSetEventReport(
    BYTE   *mdmresp,
    struct QmiCbkWmsSetEventReportResp *apiresp);

#endif /* __WMS_SET_EVENT_REPORT_H__ */
