/*
 * \ingroup sms
 *
 * \file    qaSmsSLQSSetBroadcastConfig.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaSmsSLQSSetBroadcastConfig.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __SMS_SET_BROADCAST_CONFIG_H__
#define __SMS_SET_BROADCAST_CONFIG_H__

/* enum declarations */
/*
 * An enumeration of eQMI_SMS_SET_BROADCAST_CONFIG request TLV IDs
 */
enum eQMI_SMS_SET_BROADCAST_CONFIG_REQ
{
    eTLV_SET_MODE                       = 0x01,
    eTLV_SET_BROADCAST_CONFIG_INFO      = 0x10,
    eTLV_SET_CDMA_BROADCAST_CONFIG_INFO = 0x11
};

/*
 * This structure contains the SetBroadcastConfig request parameters.
 */
struct QmiSmsSetBroadcastConfigReq
{
    BYTE                       mode;
    qaQmi3GPPBroadcastCfgInfo  *pBroadcastConfig;
    qaQmi3GPP2BroadcastCfgInfo *pCDMABroadcastConfig;
};

/*
 * This structure contains the SetImagesPreference response parameters.
 */
struct QmiSmsSetBroadcastConfigResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiSmsSetBroadcastConfig(
    WORD                       *pMlength,
    BYTE                       *pBuffer,
    BYTE                       mode,
    qaQmi3GPPBroadcastCfgInfo  *pBroadcastConfig,
    qaQmi3GPP2BroadcastCfgInfo *pCDMABroadcastConfig );

extern enum eQCWWANError UpkQmiSmsSetBroadcastConfig(
    BYTE                                *pMdmResp,
    struct QmiSmsSetBroadcastConfigResp *pApiResp );

#endif /* __SMS_SET_BROADCAST_CONFIG_H__ */
