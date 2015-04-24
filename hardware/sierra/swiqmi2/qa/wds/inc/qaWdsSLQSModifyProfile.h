/*
 * \ingroup wds
 *
 * Filename: qaWdsModifyProfile.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaWdsModifyProfile.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __WDS_SLQS_MODIFY_PROFILE_MSG_H__
#define __WDS_SLQS_MODIFY_PROFILE_MSG_H__

/* For Parameter Validations */

#define PROFILE_3GPP    0
#define PROFILE_3GPP2   1
#define IPV4            4
#define IPV6            6

/* enum declarations */
/*
 *
 * An enumeration of eQMI_WDS_MODIFY_PROFILE request TLV IDs
 *
 */
enum eWDS_MODIFY_3GPP_PROFILE_REQ
{
    eTLV_PROF_TYPE_MOD            = 0x01,
    eTLV_PROF_NAME_MOD            = 0x10,
    eTLV_PDP_TYPE_MOD             = 0x11,
    eTLV_PDP_HEADER_COMPR_TYPE    = 0x12,
    eTLV_PDP_DATA_COMPR_TYPE      = 0x13,
    eTLV_APN_NAME_MOD             = 0x14,
    eTLV_PRI_DNS_IPV4_ADDR_PREF   = 0x15,
    eTLV_SEC_DNS_IPV4_ADDR_PREF   = 0x16,
    eTLV_UMTS_REQ_QOS             = 0x17,
    eTLV_UMTS_MIN_QOS             = 0x18,
    eTLV_GPRS_REQ_QOS             = 0x19,
    eTLV_GPRS_MIN_QOS             = 0x1A,
    eTLV_USERNAME_MOD             = 0x1B,
    eTLV_PASSWORD_MOD             = 0x1C,
    eTLV_AUTHENTICATION_PREF      = 0x1D,
    eTLV_IPV4_ADDR_PREF           = 0x1E,
    eTLV_PCSCF_ADDR_USI_PCO_MOD   = 0x1F,
    eTLV_PDP_ACCESS_CONT_FLAG     = 0x20,
    eTLV_PCSCF_ADDR_USI_DHCP_MOD  = 0x21,
    eTLV_IM_CN_FLAG_MOD           = 0x22,
    eTLV_TFT_ID1_PARAM            = 0x23,
    eTLV_TFT_ID2_PARAM            = 0x24,
    eTLV_PDP_CONTEXT_MOD          = 0x25,
    eTLV_PDP_CONTEXT_SEC_FLAG     = 0x26,
    eTLV_PDP_CONTEXT_PRI_ID       = 0x27,
    eTLV_IPV6_ADDRESS_PREF        = 0x28,
    eTLV_UMTS_REQ_QOS_SIG_IND_MOD = 0x29,
    eTLV_UMTS_MIN_QOS_SIG_IND_MOD = 0x2A,
    eTLV_PRI_DNS_IPV6_ADDR_PREF   = 0x2B,
    eTLV_SEC_DNS_IPV6_ADDR_PREF   = 0x2C,
    eTLV_ADDR_ALLOC_PREF          = 0x2D,
    eTLV_3GPP_LTE_QOS_PARAMS_MOD  = 0x2E
};

enum eWDS_MODIFY_3GPP2_PROFILE_REQ
{
    eTLV_3GPP2_PROF_TYPE          = 0x01,
    eTLV_NEGO_DNS_SERVER_PREF     = 0x90,
    eTLV_PPP_SESS_CLOSE_TIMER_DO  = 0x91,
    eTLV_PPP_SESS_CLOSE_TIMER_1X  = 0x92,
    eTLV_ALLOW_LINGER_MOD         = 0x93,
    eTLV_LCP_ACK_TIMEOUT_MOD      = 0x94,
    eTLV_IPCP_ACK_TIMEOUT_MOD     = 0x95,
    eTLV_AUTH_TIMEOUT_MOD         = 0x96,
    eTLV_LCP_CREQ_RET_COUNT       = 0x97,
    eTLV_IPCP_CREQ_RET_COUNT      = 0x98,
    eTLV_AUTH_RET_COUNT           = 0x99,
    eTLV_AUTH_PROTOCOl_MOD        = 0x9A,
    eTLV_USER_ID_MOD              = 0x9B,
    eTLV_AUTH_PASS_MOD            = 0x9C,
    eTLV_DATA_RATE_MOD            = 0x9D,
    eTLV_APP_TYPE_MOD             = 0x9E,
    eTLV_DATA_MODE_MOD            = 0x9F,
    eTLV_APP_PRIOR                = 0xA0,
    eTLV_APN_STRING_MOD           = 0xA1,
    eTLV_PDN_TYPE_MOD             = 0xA2,
    eTLV_IS_PCSCF_ADDR_NEDDED     = 0xA3,
    eTLV_PRIMARY_V4_DNS_ADDR      = 0xA4,
    eTLV_SECONDARY_V4_DNS_ADDR    = 0xA5,
    eTLV_PRIMARY_V6_DNS_ADDR      = 0xA6,
    eTLV_SECONDARY_V6_DNS_ADDR    = 0xA7
};

/*
 * This structure contains the SLQSModifyProfile request parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsSLQSModifyProfileReq
{
    /* SLQSProfile request parameters */
    struct ModifyProfileIn *pSlqsModifyProfileIn;
};

/*
 *
 * An enumeration of eQMI_WDS_MODIFY_PROFILE response TLV IDs
 *
 */
enum eWDS_MODIFY_PROFILE_RESP
{
    eTLV_PROFILE_EXTENDED_ERRORCODE2 = 0xE0,
};

/*
 * This structure contains the SLQSModifyProfile response parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsSLQSModifyProfileResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult      results;

    /* SLQSModifyProfile response parameters */
    struct ModifyProfileOut *pSlqsModifyProfileOut;
};

/************
 * Prototypes
 **************/
extern enum eQCWWANError PkQmiWdsSLQSModifyProfile(
    WORD                   *pMlength,
    BYTE                   *pBuffer,
    struct ModifyProfileIn *pModifyProfileIn );

extern enum eQCWWANError UpkQmiWdsSLQSModifyProfile(
    BYTE                               *pMdmResp,
    struct QmiWdsSLQSModifyProfileResp *pAipResp );

#endif /* __WDS_MODIFY_PROFILE_MSG_H__ */
