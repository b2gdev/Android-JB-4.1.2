/*
 * \ingroup wds
 *
 * \file qaWdsSLQSWdsSetEventReport.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaWdsSLQSWdsSetEventReport.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __WDS_SLQS_WDS_SET_EVENT_REPORT_MSG_H__
#define __WDS_SLQS_WDS_SET_EVENT_REPORT_MSG_H__

#include "qaGobiApiWds.h"

/* enum declarations */
/*
 *
 * An enumeration of eWDS_SLQS_SET_EVENT_REPORT_REQ response TLV IDs
 *
 */
enum eWDS_SLQS_SET_EVENT_REPORT_REQ
{
    eTLV_CURR_CHANNEL_RATE_IND        = 0x10,
    eTLV_TRANSFER_STATS_IND           = 0x11,
    eTLV_DATA_BEARER_TECH_IND         = 0x12,
    eTLV_DORMANCY_STATUS_IND          = 0x13,
    eTLV_MIP_STATUS_IND               = 0x14,
    eTLV_CURR_DATA_BEARER_TECH_IND    = 0x15,
    eTLV_DATA_CALL_STATUS_CHANGE_IND  = 0x17,
    eTLV_CURR_PREF_DATA_SYS_IND       = 0x18,
    eTLV_EVDO_PAGE_MON_PER_CHANGE_IND = 0x19,
    eTLV_DATA_SYS_STATUS_CHANGE_IND   = 0x1A
};

/*
 * This structure contains the SLQSWdsSetEventReport response parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsSLQSSetEventReportResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiWdsSLQSSetEventReport(
    WORD                 *pMlength,
    BYTE                 *pBuffer,
    wdsSetEventReportReq *pSetEventReportReq );

extern enum eQCWWANError UpkQmiWdsSLQSSetEventReport(
    BYTE                                *pMdmResp,
    struct QmiWdsSLQSSetEventReportResp *pAipResp);

#endif /* __WDS_SLQS_WDS_SET_EVENT_REPORT_MSG_H__ */
