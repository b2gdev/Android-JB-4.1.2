/*
 * \ingroup cbk
 *
 * \file    qaCbkPdsGpsServiceStateInd.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaCbkPdsGpsServiceStateInd.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __PDS_GPS_SERVICE_STATE_IND_H__
#define __PDS_GPS_SERVICE_STATE_IND_H__

/* An enumeration of eQMI_PDS_GPS_SERVICE_STATE_IND response TLV IDs */
enum eQMI_PDS_GPS_SERVICE_STATE_IND_TLV
{
    eTLV_GPS_SERVICE_STATE  = 0x01
};

/* An enumeration of eQMI_PDS_GPS_SERVICE_STATE_IND response TLV Lengths */
enum eQMI_PDS_GPS_SERVICE_STATE_IND_TLV_LENGTH
{
    eTLV_GPS_SERVICE_STATE_LENGTH  = 0x02
};

/*
 * eQMI_PDS_GPS_SERVICE_STATE TLVs defined below
 */

/*
 * Name:    GPSServiceStateTlv
 *
 * Purpose: Structure used to store GPS Service State TLV Value.
 *
 * Members: TlvPresent      - Boolean indicating the presence of the TLV
 *                            in the QMI response
 *
 *          EnabledStatus   - GPS Enabled Status
 *
 *          TrackingStatus  - GPS Tracking Status
 */
struct GPSServiceStateTlv
{
    BYTE  TlvPresent;
    ULONG EnabledStatus;
    ULONG TrackingStatus;
};

/*
 * Name:    QmiCbkPdsGpsServiceStatusInd
 *
 * Purpose: Structure for storing all GPS Service State Notification Parameters
 *
 * Members: GPSSSTlv - Structure containing the Parameters for GPS Service
 *                     State TLV
 *
 * Note:    None
 */
struct QmiCbkPdsGpsServiceStatusInd{
    struct GPSServiceStateTlv GPSSSTlv;
};

/*************
 * Prototypes
 **************/
enum eQCWWANError UpkQmiCbkPdsGpsServiceStateInd (
    BYTE                                *pMdmResp,
    struct QmiCbkPdsGpsServiceStatusInd *pAipResp );

#endif /* __PDS_GPS_SERVICE_STATE_IND_H__ */

