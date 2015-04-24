/*
 * \ingroup sms
 *
 * \file    qaSmsSLQSGetBroadcastConfig.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaSmsSLQSGetBroadcastConfig.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __SMS_GET_BROADCAST_CONFIG_H__
#define __SMS_GET_BROADCAST_CONFIG_H__

#include "qaGobiApiSms.h"

/* enum declarations */

/*
 * An enumeration of eQMI_WMS_GET_BROADCAST_CONFIG request TLV IDs
 *
 */
enum eSMS_GET_BROADCAST_CONFIG_REQ
{
    eTLV_GET_BROADCAST_CONFIG_INFO = 0x01
};

/*
 * An enumeration of eQMI_WMS_GET_BROADCAST_CONFIG response TLV IDs
 *
 */
enum eSMS_GET_BROADCAST_CONFIG_RESP
{
    eTLV_3GPP_BROADCAST_CONFIG_INFO  = 0x10,
    eTLV_3GPP2_BROADCAST_CONFIG_INFO = 0x11
};

/*
 * This structure contains the GetBroadcastConfig request parameters.
 */
struct QmiSmsGetBroadcastConfigReq
{
    BYTE mode;
};

/*
 * This structure contains the GetBroadcastConfig response parameters
 */
struct QmiSmsGetBroadcastConfigResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* GetBroadcastConfig response parameters*/
    qaQmi3GPPBroadcastCfgInfo  *pBroadcastConfig;
    qaQmi3GPP2BroadcastCfgInfo *pCDMABroadcastConfig;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiSmsGetBroadcastConfig(
    WORD *pMlength,
    BYTE *pParamField,
    BYTE mode );

extern enum eQCWWANError UpkQmiSmsGetBroadcastConfig(
    BYTE                                *pMdmResp,
    struct QmiSmsGetBroadcastConfigResp *pApiResp );

#endif /* __SMS_GET_BROADCAST_CONFIG_H__ */
