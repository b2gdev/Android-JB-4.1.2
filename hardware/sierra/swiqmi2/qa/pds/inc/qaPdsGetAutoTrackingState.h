/*
 * \ingroup pds
 *
 * \file    qaPdsGetAutoTrackingState.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaPdsGetAutoTrackingState.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __PDS_GET_AUTO_TRACKING_STATE_H__
#define __PDS_GET_AUTO_TRACKING_STATE_H__

/*
 * An enumeration of eQMI_PDS_GET_AUTO_TRACKING_STATE response TLV IDs
 */
enum eQMI_PDS_GET_AUTO_TRACKING_STATE_RESP
{
    eTLV_AUTO_TRACKING_STATE = 0x01
};

/*
 * An enumeration of eQMI_PDS_GET_AUTO_TRACKING_STATE response TLV Lengths
 */
enum eQMI_PDS_GET_AUTO_TRACKING_STATE_RESP_LENGTH
{
    eTLV_AUTO_TRACKING_STATE_LENGTH = 0x01
};

/*
 * This structure contains the GetServiceAutomaticTracking response
 * parameters
 */
struct QmiPdsGetAutoTrackingStateResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* GetServiceAutomaticTracking response parameters */
    ULONG *pbAuto;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiPdsGetAutoTrackingState(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiPdsGetAutoTrackingState(
    BYTE                                  *pMdmResp,
    struct QmiPdsGetAutoTrackingStateResp *pApiResp );

#endif /* __PDS_GET_AUTO_TRACKING_STATE_H__ */
