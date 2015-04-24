/*
 * \ingroup wds
 *
 * \file qaWdsSlqsGetProfileSettings.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaWdsSlqsGetProfileSettings.c
 *
 * Copyright: © 2011-2012 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __WDS_SLQS_GET_PROFILE_SETTINGS_MSG_H__
#define __WDS_SLQS_GET_PROFILE_SETTINGS_MSG_H__

#include "qaGobiApiWds.h"

#define PROFILE_3GPP  0
#define PROFILE_3GPP2 1
#define IPv6          0x6

/* enum declarations */

/*
 * An enumeration of eQMI_WDS_GET_PROFILE_SETTINGS request TLV IDs
 */
enum eWDS_GET_PROFILE_SETTINGS_REQ
{
    eTLV_PROFILE_IDENTIFIER = 0x01
};

/*
 * An enumeration of eQMI_WDS_GET_PROFILE_SETTINGS response TLV IDs
 */
enum eWDS_GET_PROFILE_RESP_3GPP
{
    eTLV_GET_PROFILE_NAME           = 0x10,
    eTLV_GET_PDP_TYPE               = 0x11,
    eTLV_GET_PDP_HEADER_COMPR_TYPE  = 0x12,
    eTLV_GET_PDP_DATA_COMPR_TYPE    = 0x13,
    eTLV_GET_APN_NAME               = 0x14,
    eTLV_GET_PRI_DNS_IPV4_ADDR_PREF = 0x15,
    eTLV_GET_SEC_DNS_IPV4_ADDR_PREF = 0x16,
    eTLV_GET_UMTS_REQ_QOS           = 0x17,
    eTLV_GET_UMTS_MIN_QOS           = 0x18,
    eTLV_GET_GPRS_REQ_QOS           = 0x19,
    eTLV_GET_GPRS_MIN_QOS           = 0x1A,
    eTLV_GET_USERNAME               = 0x1B,
    eTLV_GET_PASSWORD               = 0x1C,
    eTLV_GET_AUTHENTICATION_PREF    = 0x1D,
    eTLV_GET_IPV4_ADDR_PREF         = 0x1E,
    eTLV_GET_PCSCF_ADDR_USI_PCO     = 0x1F,
    eTLV_GET_PDP_ACCESS_CONT_FLAG   = 0x20,
    eTLV_GET_PCSCF_ADDR_USI_DHCP    = 0x21,
    eTLV_GET_IM_CN_FLAG             = 0x22,
    eTLV_GET_TFT_ID1_PARAM          = 0x23,
    eTLV_GET_TFT_ID2_PARAM          = 0x24,
    eTLV_GET_PDP_CONTEXT            = 0x25,
    eTLV_GET_PDP_CONTEXT_SEC_FLAG   = 0x26,
    eTLV_GET_PDP_CONTEXT_PRI_ID     = 0x27,
    eTLV_GET_IPV6_ADDRESS_PREF      = 0x28,
    eTLV_GET_UMTS_REQ_QOS_SIG_IND   = 0x29,
    eTLV_GET_UMTS_MIN_QOS_SIG_IND   = 0x2A,
    eTLV_GET_PRI_DNS_IPV6_ADDR_PREF = 0x2B,
    eTLV_GET_SEC_DNS_IPV6_ADDR_PREF = 0x2C,
    eTLV_GET_ADDR_ALLOC_PREF        = 0x2D,
    eTLV_GET_3GPP_LTE_QOS_PARAMS    = 0x2E,
    eTLV_GET_APN_DISABLED_FLAG      = 0x2F,
    eTLV_GET_PDN_INACTIVITY_TIMEOUT = 0x30,
    eTLV_GET_APN_CLASS              = 0x31,
};

enum eWDS_GET_PROFILE_RESP_3GPP2
{
    eTLV_GET_NEGO_DNS_SERVER_PREF    = 0x90,
    eTLV_GET_PPP_SESS_CLOSE_TIMER_DO = 0x91,
    eTLV_GET_PPP_SESS_CLOSE_TIMER_1X = 0x92,
    eTLV_GET_ALLOW_LINGER            = 0x93,
    eTLV_GET_LCP_ACK_TIMEOUT         = 0x94,
    eTLV_GET_IPCP_ACK_TIMEOUT        = 0x95,
    eTLV_GET_AUTH_TIMEOUT            = 0x96,
    eTLV_GET_LCP_CREQ_RET_COUNT      = 0x97,
    eTLV_GET_IPCP_CREQ_RET_COUNT     = 0x98,
    eTLV_GET_AUTH_RET_COUNT          = 0x99,
    eTLV_GET_AUTH_PROTOCOl           = 0x9A,
    eTLV_GET_USER_ID                 = 0x9B,
    eTLV_GET_AUTH_PASS               = 0x9C,
    eTLV_GET_DATA_RATE               = 0x9D,
    eTLV_GET_APP_TYPE                = 0x9E,
    eTLV_GET_DATA_MODE               = 0x9F,
    eTLV_GET_APP_PRIOR               = 0xA0,
    eTLV_GET_APN_STRING              = 0xA1,
    eTLV_GET_PDN_TYPE                = 0xA2,
    eTLV_GET_IS_PCSCF_ADDR_NEEDED    = 0xA3,
    eTLV_GET_PRIMARY_V4_DNS_ADDR     = 0xA4,
    eTLV_GET_SECONDARY_V4_DNS_ADDR   = 0xA5,
    eTLV_GET_PRIMARY_V6_DNS_ADDR     = 0xA6,
    eTLV_GET_SECONDARY_V6_DNS_ADDR   = 0xA7,
    eTLV_GET_RAT_TYPE                = 0xA8,
    eTLV_GET_APN_ENABLED_3GPP2       = 0xA9,
    eTLV_GET_PDN_INACTIV_TIMER_3GPP2 = 0xAA,
    eTLV_GET_APN_CLASS_3GPP2         = 0xAB,
    eTLV_GET_PROFILE_EXTENDED_ERROR  = 0xE0
};

/*
 * This structure contains the SLQSGetProfile request parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsSlqsGetProfileSettingsReq
{
    ULONG profileType;
    BYTE  profileId;
};

/*
 * This structure contains the SLQSGetProfile response parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsSlqsGetProfileSettingsResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* Get Profile Settings response structure */
    GetProfileSettingOut *pProfileSettings;
};

/* Prototypes */
extern enum eQCWWANError PkQmiWdsSlqsGetProfileSettings(
    WORD                *pMlength,
    BYTE                *pBuffer,
    GetProfileSettingIn *pProfileSettingReqParams );

extern enum eQCWWANError UpkQmiWdsSlqsGetProfileSettings(
    BYTE                                    *pMdmResp,
    BYTE                                    *pProfileType,
    struct QmiWdsSlqsGetProfileSettingsResp *pAipResp );

#endif /* _WDS_SLQS_GET_PROFILE_SETTINGS_MSG_H__ */
