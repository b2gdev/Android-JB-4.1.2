/**
 * \internal
 * \ingroup cbk
 *
 * \file qaCbkSwiOmaDmEventReportInd.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaCbkSwiOmaDmEventReportInd.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __SWIOMA_DM_EVENT_REPORT_IND_H__
#define __SWIOMA_DM_EVENT_REPORT_IND_H__

#include "qaGobiApiCbk.h"

#define QMI_SWIOMA_DM_FOTA   0x00
#define QMI_SWIOMA_DM_CONFIG 0x01

/**
 * \internal
 * An enumeration of eQMI_SWIOMA_DM_EVENT_REPORT_IND response TLV IDs
 *
 */
enum eQMI_SWIOMA_DM_EVENT_REPORT_IND
{
    eTLV_IND_OMA_DM_FOTA   = 0x10,
    eTLV_IND_OMA_DM_CONFIG = 0x11
};

/**
 * Structure used to store all sessionInfo(Union)TLV Value.
 *
 *  \param  TlvPresent - Boolean indicating the presence of the TLV
 *                       in the QMI response
 *
 *  \param  sessionType - sessiontype Value
 */
struct sessionInfoTlv
{
    BYTE               TlvPresent;
    ULONG              sessionType;
    sessionInformation sessionInfo;
};

/**
 * \internal
 * Name:    QmiCbkSwiOmaDmEventStatusReportInd
 *
 * Purpose: Structure used to store all Data Bearer Notification Parameters.
 *
 * Members: OMTlv - Operating Mode Tlv
 *          ASTlv - Activation Status Tlv
 *
 * Note:    None
 */
struct QmiCbkSwiOmaDmEventStatusReportInd{
    struct sessionInfoTlv SITlv;
};

/**
 * \internal
 * Prototypes
 *
 */
enum eQCWWANError UpkQmiCbkSwiOmaDmEventReportInd(
    BYTE                                      *pMdmResp,
    struct QmiCbkSwiOmaDmEventStatusReportInd *pApiResp );

#endif /* __SWIOMA_DM_EVENT_REPORT_IND_H__ */
