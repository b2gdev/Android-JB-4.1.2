/*
 * \ingroup pds
 *
 * \file    qaPdsSetDefaultTrackingSession.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaPdsSetDefaultTrackingSession.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __PDS_SET_DEFAULT_TRACKING_SESSION_MSG_H__
#define __PDS_SET_DEFAULT_TRACKING_SESSION_MSG_H__

/* enum declarations */
/*
 * An enumeration of eQMI_PDS_SET_DEFAULT_TRACKING_SESSION request TLV IDs
 *
 */
enum ePDS_SET_DEFAULT_TRACKING_SESSION_REQ
{
    eTLV_SET_DEFAULT_SESSION_INFO = 0x01
};

/*
 * This structure contains the SetPDSDefaults request parameters.
 */
struct QmiPdsSetDefaultTrackingSessionReq
{
    ULONG operation;
    BYTE  timeout;
    ULONG interval;
    ULONG accuracy;
};

/*
 * This structure contains the SetPDSDefaults response parameters.
 */
struct QmiPdsSetDefaultTrackingSessionResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiPdsSetDefaultTrackingSession(
    WORD  *pMlength,
    BYTE  *pBuffer,
    ULONG operation,
    BYTE  timeout,
    ULONG interval,
    ULONG accuracy );

extern enum eQCWWANError UpkQmiPdsSetDefaultTrackingSession(
    BYTE                                       *pMdmResp,
    struct QmiPdsSetDefaultTrackingSessionResp *pApiResp );

#endif /* __PDS_SET_DEFAULT_TRACKING_SESSION_MSG_H__ */
