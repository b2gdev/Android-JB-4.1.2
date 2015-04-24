/*
 * \ingroup cbk
 *
 * \file qaOmaDmSetEventReport.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaOmaDmSetEventReport.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __OMA_DM_SET_EVENT_REPORT_H__
#define __OMA_DM_SET_EVENT_REPORT_H__

#include "qaQmiNotify.h"

/*
 *
 * An enumeration of eQMI_OMA_SET_EVENT_REPORT request TLV IDs
 *
 */
enum eOMA_SET_EVENT_REPORT_REQ
{
    eTLV_OMA_SESSION_STATE_REPORTING_REQ = 0x11
};

/*
 * This structure contains the OMA-DM Set Event Report request parameters
 */
struct QmiCbkOmaDmSetEventReportReq
{
    /* Parameters to be passed in the SET_EVENT_REQ structure */
    enum eQmiCbkSetStatus eOmaDmSessionStateEvt;
};

/*
 * This structure contains the OMA-DM Set Event Report
 * response parameters
 *
 */
struct QmiCbkOmaDmSetEventReportResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 *
 */
extern enum eQCWWANError PkQmiCbkOmaDmSetEventReport(
    WORD                  *pMlength,
    enum eQmiCbkSetStatus eOmaDmSessionStateEvt,
    BYTE                  *pBuffer);

extern enum eQCWWANError UpkQmiCbkOmaDmSetEventReport(
    BYTE                                 *pMdmResp,
    struct QmiCbkOmaDmSetEventReportResp *pApiResp );

#endif /* __OMA_DM_SET_EVENT_REPORT_H__ */
