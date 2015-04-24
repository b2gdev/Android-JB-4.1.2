/*
 * \ingroup nas
 *
 * \file    qaNasSLQSGet3GPP2Subscription.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaNasSLQSGet3GPP2Subscription.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_SLQS_GET_3GPP2_SUBSCRIPTION_H__
#define __NAS_SLQS_GET_3GPP2_SUBSCRIPTION_H__

#include "qaGobiApiNas.h"

/* enum declarations */
/*
 * An enumeration of eNAS_SLQS_GET_3GPP2_SUBSCRIPTION_INFO_REQ response
 * TLVIDs
 */
enum eNAS_SLQS_GET_3GPP2_SUBSCRIPTION_REQ
{
    eTLV_NAM_ID_REQ = 0x01
};

/*
 * An enumeration of eNAS_SLQS_GET_3GPP2_SUBSCRIPTION_INFO_RESP response
 * TLVIDs
 */
enum eNAS_SLQS_GET_3GPP2_SUBSCRIPTION_RESP
{
    eTLV_NAMNAME_3GPP2_INFO        = 0x10,
    eTLV_DIR_NUM_3GPP2_INFO        = 0x11,
    eTLV_HOME_SID_NID_3GPP2_INFO   = 0x12,
    eTLV_MIN_BASED_IMSI_3GPP2_INFO = 0x13,
    eTLV_TRUE_IMSI_3GPP2_INFO      = 0x14,
    eTLV_CDMA_CHANNEL_3GPP2_INFO   = 0x15
};

/*
 * This structure contains the SLQSNas3GPP2SubscriptionInfo response parameters.
 */
struct QmiNasSLQSGet3GPP2SubscriptionInfoResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    nasGet3GPP2SubscriptionInfoResp *pNas3GPP2SubscriptionInfoResp;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiNasSlqsGet3GPP2SubscriptionInfo(
    WORD                           *pMlength,
    BYTE                           *pBuffer,
    nasGet3GPP2SubscriptionInfoReq *pGet3GPP2SubsInfoReq );

extern enum eQCWWANError UpkQmiNasSlqsGet3GPP2SubscriptionInfo(
    BYTE                                          *pMdmResp,
    struct QmiNasSLQSGet3GPP2SubscriptionInfoResp *pApiResp );

#endif /*__NAS_SLQS_GET_3GPP2_SUBSCRIPTION_H__*/
