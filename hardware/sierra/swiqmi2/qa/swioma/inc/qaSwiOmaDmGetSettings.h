/*
 * \ingroup swioma
 *
 * \file    qaSwiOmaDmGetSettings.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaSwiOmaDmGetSettings.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __SWIOMA_DM_GET_SETTINGS_H__
#define __SWIOMA_DM_GET_SETTINGS_H__

#include "qaGobiApiSwiOmadms.h"
/*
 * An enumeration of eQMI_SWIOMA_DM_GET_SETTINGS response TLV IDs
 */
enum eQMI_SWIOMA_DM_GET_SETTINGS_RESP
{
    eTLV_GET_OMA_DM_ENABLED          = 0x10,
    eTLV_GET_FOTA_AUTOMATIC_DOWNLOAD = 0x11,
    eTLV_GET_FOTA_AUTOMATIC_UPDATE   = 0x12,
    eTLV_GET_OMA_AUTOSDM             = 0x13
};

/*
 * An enumeration of eQMI_SWIOMA_DM_GET_SETTINGS response TLV Lengths
 */
enum eQMI_SWIOMA_DM_GET_SETTINGS_RESP_LENGTH
{
    eTLV_OMA_DM_ENABLED_LENGTH          = 0x04,
    eTLV_FOTA_AUTOMATIC_DOWNLOAD_LENGTH = 0x01,
    eTLV_FOTA_AUTOMATIC_UPDATE_LENGTH   = 0x01,
    eTLV_OMA_AUTOSDM_LENGTH             = 0x01,
};

/*
 * This structure contains the SLQSOMADMGetSettings response parameters
 */
struct QmiSwiOmaDmGetSettingsResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* SLQSOMADMGetSettings response parameters */
    SLQSOMADMSettings *pSLQSOMADMSettings;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiSwiOmaDmGetSettings(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiSwiOmaDmGetSettings(
    BYTE                              *pMdmResp,
    struct QmiSwiOmaDmGetSettingsResp *pApiResp );

#endif /* __SWIOMA_DM_GET_SETTINGS_H__ */
