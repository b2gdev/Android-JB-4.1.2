/*
 * \ingroup pds
 *
 * \file    qaPdsInjectPositionData.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaPdsInjectPositionData.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __PDS_INJECT_POSITION_DATA_H__
#define __PDS_INJECT_POSITION_DATA_H__

#include "qaGobiApiPds.h"

/* enum declarations */
/*
 * An enumeration of eQMI_PDS_INJECT_POSITION_DATA request TLV IDs
 */
enum eQMI_PDS_INJECT_POSITION_DATA_REQ
{
    eTLV_TIMESTAMP               = 0x10,
    eTLV_LATITUE                 = 0x11,
    eTLV_LONGITUDE               = 0x12,
    eTLV_ALTITUDE_FROM_ELLIPSOID = 0x13,
    eTLV_ALTITUDE_FROM_SEALEVEL  = 0x14,
    eTLV_HORIZONDAL_UNC_CIRCULAR = 0x15,
    eTLV_VERTICAL_UNC            = 0x16,
    eTLV_HORIZONDAL_CONFIDENCE   = 0x17,
    eTLV_VERTICAL_CONFIDENCE     = 0x18,
    eTLV_POSITION_SOURCE         = 0x19,
    eTLV_TIME_TYPE               = 0x1A,
};

/*
 * This structure contains the PdsInjectPositionDataResp response parameters
 */
struct QmiPdsInjectPositionDataResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * prototypes
 */
extern enum eQCWWANError PkQmiPdsInjectPositionData(
    WORD                   *pMlength,
    BYTE                   *pBuffer,
    struct PDSPositionData *pPositionData );

extern enum eQCWWANError UpkQmiPdsInjectPositionData(
    BYTE                                *pMdmResp,
    struct QmiPdsInjectPositionDataResp *pApiResp );

#endif /* __PDS_INJECT_POSITION_DATA_H__ */
