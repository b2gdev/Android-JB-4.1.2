/*
 * \ingroup pds
 *
 * \file qaPdsEndTrackingSession.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaPdsEndTrackingSession.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __PDS_END_TRACKING_SESSION_H__
#define __PDS_END_TRACKING_SESSION_H__

/*
 * This structure contains the StopPDSTrackingSession response parameters
 *
 */
struct QmiPdsEndTrackingSessionResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 *
 */
extern enum eQCWWANError PkQmiPdsEndTrackingSession(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiPdsEndTrackingSession(
    BYTE                                *pMdmResp,
    struct QmiPdsEndTrackingSessionResp *pApiResp );

#endif /* __PDS_END_TRACKING_SESSION_H__ */
