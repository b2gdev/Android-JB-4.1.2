/*
 * \ingroup pds
 *
 * \file    qaPdsGetDefaultTrackingSession.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaPdsGetDefaultTrackingSession.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __PDS_GET_DEFAULT_TRACKING_SESSION_H__
#define __PDS_GET_DEFAULT_TRACKING_SESSION_H__

/*
 * An enumeration of eQMI_PDS_GET_DEFAULT_TRACKING_SESSION response TLV IDs
 */
enum eQMI_PDS_GET_DEFAULT_TRACKING_SESSION_RESP
{
    eTLV_GET_DEFAULT_SESSION_INFO = 0x01
};

/*
 * An enumeration of eQMI_PDS_GET_DEFAULT_TRACKING_SESSION response TLV Lengths
 */
enum eQMI_PDS_GET_DEFAULT_TRACKING_SESSION_RESP_LENGTH
{
    eTLV_GET_DEFAULT_SESSION_INFO_LEN = 0x0A
};

/*
 * This structure contains the GetPDSDefaults response parameters
 */
struct QmiPdsGetDefaultTrackingSessionResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* GetPDSDefaults response parameters */
    ULONG *pOperation;
    BYTE  *pTimeout;
    ULONG *pInterval;
    ULONG *pAccuracy;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiPdsGetDefaultTrackingSession(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiPdsGetDefaultTrackingSession(
    BYTE                                       *pMdmResp,
    struct QmiPdsGetDefaultTrackingSessionResp *pApiResp );

#endif /* __PDS_GET_DEFAULT_TRACKING_SESSION_H__ */
