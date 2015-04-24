/**
 * \internal
 * \ingroup nas
 *
 * \file  qaNasGetRFBandInfo.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaNasGetRFBandInfo.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_GET_RF_BAND_INFO_MSG_H__
#define __NAS_GET_RF_BAND_INFO_MSG_H__

#include "qaGobiApiNas.h"

/**
 * \internal
 * enum declarations
 */

/**
 * \internal
 *
 * An enumeration of eQMI_NAS_GET_RF_INFO response TLV IDs
 *
 */
enum eQMI_NAS_GET_RF_INFO_RESP
{
    eTLV_RF_BAND_INFO = 0x01

};


/**
 * \internal
 *
 * This structure contains the GetRFInfo response parameters
 *
 * \sa qaGobiApiNas.h for parameter descriptions
 *
 */
struct QmiNasGetRFBandInfoResp
{
    /**
     * \internal
     *
     * Every response message must have a results structure
     */
    struct qmTlvResult        results;
    BYTE                      *pInstancesSize;
    struct RFBandInfoElements *pInstances;
};

/**
 * \internal
 *
 * Prototypes
 *
 */
extern enum eQCWWANError PkQmiNasGetRFBandInfo(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiNasGetRFBandInfo(
    BYTE                           *pMdmResp,
    struct QmiNasGetRFBandInfoResp *pApiResp );

#endif /* __NAS_GET_RF_BAND_INFO_MSG_H__ */
