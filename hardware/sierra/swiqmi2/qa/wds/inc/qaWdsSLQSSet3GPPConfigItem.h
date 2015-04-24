/*
 * \ingroup wds
 *
 * \file qaWdsSLQSSet3GPPConfigItem.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaWdsSLQSSet3GPPConfigItem.c
 *
 * Copyright: © 2011-2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __WDS_SLQS_SET_3GPP_CONFIG_ITEM_H__
#define __WDS_SLQS_SET_3GPP_CONFIG_ITEM_H__

#include "qaGobiApiWds.h"

/* enum declarations */
/*
 *
 * An enumeration of eQMI_WDS_SLQS_SET_3GPP_CFG_ITEM request TLV IDs
 *
 */
enum eWDS_SLQS_SET_3GPP_CONFIG_ITEM_REQ
{
    eTLV_SET_LTE_ATTACH_PROFILE            = 0x10,
    eTLV_SET_PROFILE_LIST                  = 0x11,
    eTLV_SET_ALWAYS_CONNECT_DEFAULT_PDN    = 0x12,
    eTLV_SET_3GPP_RELEASE                  = 0x13
};

/*
 * This structure contains the SLQSSet3GPPConfigItem request parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsSLQSSet3GPPConfigItemReq
{
    /* Set3gppconfigItem request parameters */
    slqs3GPPConfigItem *pSLQS3GPPConfigItem;
};

/*
 * This structure contains the SLQSSet3GPPConfigItem response parameters.
 *
 *
 */
struct QmiWdsSLQSSet3GPPConfigItemResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};
/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiWdsSLQSSet3GPPConfigItem(
    WORD               *pMlength,
    BYTE               *pBuffer,
    slqs3GPPConfigItem *pSLQS3GPPConfigItem );

extern enum eQCWWANError UpkQmiWdsSLQSSet3GPPConfigItem(
    BYTE                                   *pMdmResp,
    struct QmiWdsSLQSSet3GPPConfigItemResp *pApiResp );

#endif /* __WDS_SLQS_SET_3GPP_CONFIG_ITEM_H__ */
