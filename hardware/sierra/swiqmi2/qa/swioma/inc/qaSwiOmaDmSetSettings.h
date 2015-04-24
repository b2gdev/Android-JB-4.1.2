/*
 * \ingroup swioma
 *
 * \file    qaSwiOmaDmSetSettings.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaSwiOmaDmSetSettings.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __SWIOMA_DM_SET_SETTINGS_H__
#define __SWIOMA_DM_SET_SETTINGS_H__

#include "qaGobiApiSwiOmadms.h"

/* enum declarations */
/*
 * An enumeration of eQMI_SWIOMA_DM_SET_SETTINGS request TLV IDs
 */
enum eSWIOMA_DM_SET_SETTINGS_REQ
{
    eTLV_SET_FOTA_AUTOMATIC_DOWNLOAD = 0x11,
    eTLV_SET_FOTA_AUTOMATIC_UPDATE   = 0x12,
    eTLV_SET_OMA_AUTO_UI_ALERT_RESP  = 0x13
};

/*
 * This structure contains the SLQSOMADMSetSettings response parameters.
 */
struct QmiSwiOmaDmSetSettingsResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiSwiOmaDmSetSettings(
    WORD                       *pMlength,
    BYTE                       *pBuffer,
    SLQSOMADMSettingsReqParams *pSLQSOMADMSettings );

extern enum eQCWWANError UpkQmiSwiOmaDmSetSettings(
    BYTE                              *pMdmResp,
    struct QmiSwiOmaDmSetSettingsResp *pApiResp );

#endif /* __SWIOMA_DM_SET_SETTINGS_H__ */
