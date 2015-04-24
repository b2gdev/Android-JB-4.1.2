/*
 * \ingroup pds
 *
 * \file    qaPdsSetAutoTrackingState.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaPdsSetAutoTrackingState.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __PDS_SET_AUTO_TRACKING_STATE_MSG_H__
#define __PDS_SET_AUTO_TRACKING_STATE_MSG_H__

/* enum declarations */
/*
 * An enumeration of eQMI_PDS_SET_AUTO_TRACKING_STATE request TLV IDs
 *
 */
enum ePDS_SET_AUTO_TRACKING_STATE_REQ
{
    eTLV_AUTO_TRACK_STATE = 0x01
};

/*
 * This structure contains the SetServiceAutomaticTracking
 * request parameters.
 */
struct QmiPdsSetAutoTrackingStateReq
{
    ULONG bAuto;
};

/*
 * This structure contains the SetServiceAutomaticTracking
 * response parameters.
 */
struct QmiPdsSetAutoTrackingStateResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiPdsSetAutoTrackingState(
    WORD  *pMlength,
    BYTE  *pBuffer,
    ULONG bAuto );

extern enum eQCWWANError UpkQmiPdsSetAutoTrackingState(
    BYTE                                  *pMdmResp,
    struct QmiPdsSetAutoTrackingStateResp *pApiResp );

#endif /* __PDS_SET_AUTO_TRACKING_STATE_MSG_H__ */
