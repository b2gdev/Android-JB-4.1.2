/*
 * \ingroup wds
 *
 * \file qaWdsSLQSGet3GPPConfigItem.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaWdsSLQSGet3GPPConfigItem.c
 *
 * Copyright: © 2011-2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __WDS_SLQS_GET_3GPP_CONFIG_ITEM_H__
#define __WDS_SLQS_GET_3GPP_CONFIG_ITEM_H__

#include "qaGobiApiWds.h"

/* enum declarations */
/*
 *
 * An enumeration of eQMI_WDS_SLQS_GET_3GPP_CFG_ITEM response TLV IDs
 *
 */
enum eWDS_SLQS_GET_3GPP_CONFIG_ITEM_RESP
{
    eTLV_GET_LTE_ATTACH_PROFILE            = 0x10,
    eTLV_GET_PROFILE_LIST                  = 0x11,
    eTLV_GET_ALWAYS_CONNECT_DEFAULT_PDN    = 0x12,
    eTLV_GET_3GPP_RELEASE                  = 0x13
};

/*
 * This structure contains the SLQSGet3GPPConfigItem response parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsSLQSGet3GPPConfigItemResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* Get3gppconfigItem response parameters */
    slqs3GPPConfigItem *pSLQS3GPPConfigItem;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiWdsSLQSGet3GPPConfigItem(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiWdsSLQSGet3GPPConfigItem(
    BYTE                                   *pMdmResp,
    struct QmiWdsSLQSGet3GPPConfigItemResp *pApiResp );

#endif /* __WDS_SLQS_GET_3GPP_CONFIG_ITEM_H__ */
