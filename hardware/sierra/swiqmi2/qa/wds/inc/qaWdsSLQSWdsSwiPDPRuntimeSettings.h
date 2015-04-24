/*
 * \ingroup wds
 *
 * Filename:   qaWdsSLQSWdsSwiPDPRuntimeSettings.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaWdsSLQSWdsSwiPDPRuntimeSettings.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __WDS_SLQS_SWI_PDP_RUNTIME_SETTINGS_MSG_H__
#define __WDS_SLQS_SWI_PDP_RUNTIME_SETTINGS_MSG_H__

#include "qaGobiApiWds.h"

/* enum declarations */
/*
 * An enumeration of eWDS_SLQS_SWI_PDP_RUNTIME_SETTINGS_REQ request TLV IDs
 *
 */
enum eWDS_SLQS_SWI_PDP_RUNTIME_SETTINGS_REQ
{
    eTLV_PDP_RS_CONTEXT_ID = 0x01
};

/*
 * An enumeration of eWDS_SLQS_SWI_PDP_RUNTIME_SETTINGS_RESP response TLV IDs
 */
enum eWDS_SLQS_SWI_PDP_RUNTIME_SETTINGS_RESP
{
    eTLV_PDP_RS_CONTEXT_ID_RESP       = 0x10,
    eTLV_PDP_RS_BEARER_ID             = 0x11,
    eTLV_PDP_RS_APN_NAME              = 0x12,
    eTLV_PDP_RS_IPV4_ADDRESS          = 0x13,
    eTLV_PDP_RS_IPV4_GW_ADDRESS       = 0x14,
    eTLV_PDP_RS_PR_DNS_IPV4_ADDRESS   = 0x15,
    eTLV_PDP_RS_SE_DNS_IPV4_ADDRESS   = 0x16,
    eTLV_PDP_RS_IPV6_ADDRESS          = 0x17,
    eTLV_PDP_RS_IPV6_GW_ADDRESS       = 0x18,
    eTLV_PDP_RS_PR_DNS_IPV6_ADDRESS   = 0x19,
    eTLV_PDP_RS_SE_DNS_IPV6_ADDRESS   = 0x1A,
    eTLV_PDP_RS_PR_PCSCF_IPV4_ADDRESS = 0x1B,
    eTLV_PDP_RS_SE_PCSCF_IPV4_ADDRESS = 0x1C,
    eTLV_PDP_RS_PR_PCSCF_IPV6_ADDRESS = 0x1D,
    eTLV_PDP_RS_SE_PCSCF_IPV6_ADDRESS = 0x1E
};

/*
 * This structure contains the SLQSSwiPDPRuntimeSettings response parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsSLQSSwiPDPRuntimeSettingsResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* SLQSWdsSwiPDPRuntimeSettings response parameters */
    swiPDPRuntimeSettingsResp *pPDPRuntimeSettingsResp;
};

/************
 * Prototypes
 **************/
extern enum eQCWWANError PkQmiWdsSLQSSwiPDPRuntimeSettings(
    WORD                     *pMlength,
    BYTE                     *pBuffer,
    swiPDPRuntimeSettingsReq *pPDPRuntimeSettingsReq );

extern enum eQCWWANError UpkQmiWdsSLQSSwiPDPRuntimeSettings(
    BYTE                                       *pMdmResp,
    struct QmiWdsSLQSSwiPDPRuntimeSettingsResp *pAipResp );

#endif /* __WDS_SLQS_SWI_PDP_RUNTIME_SETTINGS_MSG_H__ */
