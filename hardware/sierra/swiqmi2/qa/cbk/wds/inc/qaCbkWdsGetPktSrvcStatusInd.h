/*
 * \ingroup cbk
 *
 * \file    qaCbkWdsGetPktSrvcStatusInd.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaCbkWdsGetPktSrvcStatusInd.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __WDS_GET_PKT_SRVC_STATUS_IND_H__
#define __WDS_GET_PKT_SRVC_STATUS_IND_H__

/* An enumeration of eQMI_WDS_GET_PKT_SRVC_STATUS_IND response TLV IDs */
enum eQMI_WDS_GET_PKT_SRVC_STATUS_IND
{
    eTLV_PACKET_SERVICE_STATUS    = 0x01,
    eTLV_CALL_END_REASON          = 0x10,
    eTLV_VERBOSE_CALL_END_REASON  = 0x11,
};

/*
 * eQMI_WDS_PKT_STATUS_IND TLVs defined below
 */

/*
 * Name:    QmiCbkWdsSessionStateInd
 *
 * Purpose: Structure used to store all QMI Notification parameters.
 *
 * Members: state                - Session State
 *          sessionEndReason     - Session End Reason
 *          sessionEndReasonType - Session End Reason type
 *
 * Note:    None
 */
struct QmiCbkWdsSessionStateInd{
    ULONG state;
    ULONG sessionEndReason;
    ULONG sessionEndReasonType;
};

/*************
 * Prototypes
 **************/
enum eQCWWANError UpkQmiCbkWdsGetPktSrvcStatusInd (
    BYTE     *pMdmResp,
    struct   QmiCbkWdsSessionStateInd *pAipResp );

#endif /* __WDS_GET_PKT_SRVC_STATUS_IND_H__ */

