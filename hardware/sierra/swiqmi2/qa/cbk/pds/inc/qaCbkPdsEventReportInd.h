/*
 * \ingroup cbk
 *
 * \file    qaCbkPdsEventReportInd.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaCbkPdsEventReportInd.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __PDS_EVENT_REPORT_IND_H__
#define __PDS_EVENT_REPORT_IND_H__

#include "qmudefs.h"

/* An enumeration of eQMI_PDS_EVENT_REPORT_IND response TLV IDs */
enum eQMI_PDS_EVENT_REPORT_IND_TLV
{
    eTLV_POSITION_DATA_NMEA         = 0x10,
    eTLV_POSITION_SESSION_STATUS    = 0x12
};

/* An enumeration of eQMI_PDS_EVENT_REPORT_IND response TLV Lengths */
enum eQMI_PDS_EVENT_REPORT_IND_TLV_LENGTH
{
    eTLV_POSITION_DATA_NMEA_LENGTH      = eTLV_LENGTH_VARIABLE,
    eTLV_POSITION_SESSION_STATUS_LENGTH = 0x01
};

/*
 * eQMI_PDS_EVENT_IND TLVs defined below
 */

#define QMI_MAX_POSITION_DATA_NMEA_BUFFER 200
/*
 * Name:    PositionDataNMEATlv
 *
 * Purpose: Structure used to store all Position Data NMEA TLV Value.
 *
 * Members: TlvPresent         - Boolean indicating the presence of the TLV
 *                               in the QMI response
 *
 *          PositionDataNMEA   - Position Data NMEA value
 */
struct PositionDataNMEATlv
{
    BYTE TlvPresent;
    CHAR PositionDataNMEA[QMI_MAX_POSITION_DATA_NMEA_BUFFER];
};

/*
 * Name:    QmiCbkPdsEventStatusReportInd
 *
 * Purpose: Structure used to store all Data Bearer Notification Parameters.
 *
 * Members: PDNMEATlv - Structure containing the Position Data NMEA TLV
 *
 * Note:    None
 */
struct QmiCbkPdsEventStatusReportInd{
    struct PositionDataNMEATlv PDNMEATlv;
};

/*************
 * Prototypes
 **************/
enum eQCWWANError UpkQmiCbkPdsEventReportInd (
    BYTE   *pMdmResp,
    struct QmiCbkPdsEventStatusReportInd *pAipResp );

#endif /* __PDS_EVENT_REPORT_IND_H__ */

