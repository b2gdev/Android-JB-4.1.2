/*
 * \ingroup pds
 *
 * \file qaPdsStartTrackingSession.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaPdsStartTrackingSession.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __PDS_START_TRACKING_SESSION_MSG_H__
#define __PDS_START_TRACKING_SESSION_MSG_H__

/* enum declarations */
/*
 * An enumeration of eQMI_PDS_START_TRACKING_SESSION request TLV IDs
 *
 */
enum ePDS_START_TRACKING_SESSION_REQ
{
    eTLV_TRACKING_SESSION = 0x01
};

/*
 * This structure contains the StartPDSTrackingSession request parameters.
 *
 * \sa qaGobiApiPds.h for parameter descriptions
 *
 */
struct QmiPdsStartTrackingSessionReq
{
    BYTE  SessionControl;
    BYTE  SessionType;
    BYTE  SessionOperation;
    BYTE  SessionServerOption;
    BYTE  PositionDataTimeout;
    ULONG PositionDataInterval;
    ULONG PositionDataCount;
    ULONG PositionDataAccuracy;
};

/*
 * This structure contains the StartPDSTrackingSession response parameters.
 *
 */
struct QmiPdsStartTrackingSessionResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 *
 */
extern enum eQCWWANError PkQmiPdsStartTrackingSession(
    WORD  *pMlength,
    BYTE  *pBuffer,
    BYTE  sessionControl,
    BYTE  sessionType,
    BYTE  sessionOperation,
    BYTE  sessionServerOption,
    BYTE  fixTimeout,
    ULONG fixInterval,
    ULONG fixCount,
    ULONG fixAccuracy );

extern enum eQCWWANError UpkQmiPdsStartTrackingSession(
    BYTE                                  *pMdmResp,
    struct QmiPdsStartTrackingSessionResp *pApiResp );

#endif /* __PDS_START_TRACKING_SESSION_MSG_H__ */
