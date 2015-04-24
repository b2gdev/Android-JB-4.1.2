/*
 * \ingroup wds
 *
 * Filename: qaWdsCreateProfile.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaWdsCreateProfile.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __WDS_CREATE_PROFILE_MSG_H__
#define __WDS_CREATE_PROFILE_MSG_H__

/* For Parameter Validations */

#define PROFILE_3GPP    0
#define PROFILE_3GPP2   1
#define IPV4            4
#define IPV6            6
#define IPV4V6          7

/* enum declarations */
/*
 *
 * An enumeration of eQMI_WDS_CREATE_PROFILE request TLV IDs
 *
 */
enum eWDS_CREATE_3GPP_PROFILE_REQ
{
    eTLV_PROFILE_TYPE1                = 0x01,
    eTLV_PROFILE_NAME1                = 0x10,
    eTLV_PDP_TYPE1                    = 0x11,
    eTLV_PDP_HEADER_COMPRESSION_TYPE  = 0x12,
    eTLV_PDP_DATA_COMPRESSION_TYPE    = 0x13,
    eTLV_APN_NAME1                    = 0x14,
    eTLV_PRI_DNS_IPV4_ADDRESS_PREF1   = 0x15,
    eTLV_SEC_DNS_IPV4_ADDRESS_PREF1   = 0x16,
    eTLV_UMTS_REQUESTED_QOS           = 0x17,
    eTLV_UMTS_MINIMUM_QOS             = 0x18,
    eTLV_GPRS_REQUESTED_QOS           = 0x19,
    eTLV_GPRS_MINIMUM_QOS             = 0x1A,
    eTLV_USERNAME1                    = 0x1B,
    eTLV_PASSWORD1                    = 0x1C,
    eTLV_AUTHENTICATION_PREFERENCE    = 0x1D,
    eTLV_IPV4_ADDRESS_PREFERENCE      = 0x1E,
    eTLV_PCSCF_ADDR_USING_PCO         = 0x1F,
    eTLV_PDP_ACCESS_CONTROL_FLAG      = 0x20,
    eTLV_PCSCF_ADDR_USING_DHCP        = 0x21,
    eTLV_IM_CN_FLAG                   = 0x22,
    eTLV_TFT_ID1_PARAMETERS           = 0x23,
    eTLV_TFT_ID2_PARAMETERS           = 0x24,
    eTLV_PDP_CONTEXT                  = 0x25,
    eTLV_PDP_CONTEXT_SECONDARY_FLAG   = 0x26,
    eTLV_PDP_CONTEXT_PRIMARY_ID       = 0x27,
    eTLV_IPV6_ADDRESS_PREFERENCE      = 0x28,
    eTLV_UMTS_REQ_QOS_SIG_IND         = 0x29,
    eTLV_UMTS_MIN_QOS_SIG_IND         = 0x2A,
    eTLV_PRI_DNS_IPV6_ADDRESS_PREF    = 0x2B,
    eTLV_SEC_DNS_IPV6_ADDRESS_PREF    = 0x2C,
    eTLV_ADDR_ALLOCATION_PREF         = 0x2D,
    eTLV_3GPP_LTE_QOS_PARAMS          = 0x2E
};

enum eWDS_CREATE_3GPP2_PROFILE_REQ
{
    eTLV_3GPP2_PROFILE_TYPE          = 0x01,
    eTLV_NEGO_DNS_SRVR_PREF          = 0x90,
    eTLV_PPP_SESSION_CLOSE_TIMER_DO  = 0x91,
    eTLV_PPP_SESSION_CLOSE_TIMER_1X  = 0x92,
    eTLV_ALLOW_LINGER                = 0x93,
    eTLV_LCP_ACK_TIMEOUT             = 0x94,
    eTLV_IPCP_ACK_TIMEOUT            = 0x95,
    eTLV_AUTH_TIMEOUT                = 0x96,
    eTLV_LCP_CREQ_RETRY_COUNT        = 0x97,
    eTLV_IPCP_CREQ_RETRY_COUNT       = 0x98,
    eTLV_AUTH_RETRY_COUNT            = 0x99,
    eTLV_AUTH_PROTOCOl               = 0x9A,
    eTLV_USER_ID                     = 0x9B,
    eTLV_AUTH_PASSWORD               = 0x9C,
    eTLV_DATA_RATE                   = 0x9D,
    eTLV_APP_TYPE                    = 0x9E,
    eTLV_DATA_MODE                   = 0x9F,
    eTLV_APP_PRIORITY                = 0xA0,
    eTLV_APN_STRING                  = 0xA1,
    eTLV_PDN_TYPE                    = 0xA2,
    eTLV_IS_PCSCF_ADD_NEDDED         = 0xA3,
    eTLV_PRIMARY_V4_DNS_ADDRESS      = 0xA4,
    eTLV_SECONDARY_V4_DNS_ADDRESS    = 0xA5,
    eTLV_PRIMARY_V6_DNS_ADDRESS      = 0xA6,
    eTLV_SECONDARY_V6_DNS_ADDRESS    = 0xA7
};

/*
 * This structure contains the SLQSCreateProfile request parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsSLQSCreateProfileReq
{
    /* SLQSCreateProfile request parameters */
    struct CreateProfileIn *pSlqsCreateProfileIn;
};

/*
 *
 * An enumeration of eQMI_WDS_CREATE_PROFILE response TLV IDs
 *
 */
enum eWDS_CREATE_PROFILE_RESP
{
    eTLV_PROFILE_IDENTIFIER1        = 0x01,
    eTLV_PROFILE_EXTENDED_ERRORCODE = 0xE0,
};

/*
 * This structure contains the SLQSCreateProfile response parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsSLQSCreateProfileResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* SLQSGetRuntimeSettings response parameters */
    struct CreateProfileOut *pSlqsCreateProfileOut;
};

/************
 * Prototypes
 **************/
extern enum eQCWWANError PkQmiWdsSLQSCreateProfile(
    WORD                           *pMlength,
    BYTE                            *pBuffer,
    struct CreateProfileIn *pCreateProfileIn );

extern enum eQCWWANError UpkQmiWdsSLQSCreateProfile(
    BYTE                               *pMdmResp,
    struct QmiWdsSLQSCreateProfileResp *pAipResp,
    BYTE                               *pProfileID );

#endif /* __WDS_CREATE_PROFILE_MSG_H__ */
