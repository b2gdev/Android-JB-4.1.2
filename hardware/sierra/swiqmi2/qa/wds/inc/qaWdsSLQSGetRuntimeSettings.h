
/*
 * \ingroup wds
 *
 * Filename: qaWdsSLQSGetRuntimeSettings.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaWdsSLQSGetRuntimeSettings.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __WDS_SLQS_GET_RUNTIME_SETTINGS_MSG_H__
#define __WDS_SLQS_GET_RUNTIME_SETTINGS_MSG_H__

/* enum declarations */
/*
 *
 * An enumeration of eQMI_WDS_SLQS_GET_RUNTIME_SETTINGS request TLV IDs
 *  *
 */
enum eWDS_SLQS_GET_RUNTIME_SETTINGS_REQ
{
    eTLV_SLQS_REQUESTED_SETTINGS = 0x10
};

/*
 * This structure contains the SLQSGetRuntimeSettings request parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */

struct QmiWdsSLQSGetRuntimeSettingsReq
{
    ULONG *pRequestedSettings;
};

/*
 *
 * An enumeration of eQMI_WDS_START_NET response TLV IDs
 *  *
 */
enum eWDS_SLQS_GET_RUNTIME_SETTINGS_RESP
{
    eTLV_SLQS_PROFILE_NAME              = 0x10,
    eTLV_SLQS_PDP_TYPE                  = 0x11,
    eTLV_SLQS_APN_NAME                  = 0x14,
    eTLV_SLQS_PRIM_DNSV4                = 0x15,
    eTLV_SLQS_SECOND_DNSV4              = 0x16,
    eTLV_SLQS_UMTS_GRANTED_QOS          = 0x17,
    eTLV_SLQS_GPRS_GRANTED_QOS          = 0x19,
    eTLV_SLQS_USERNAME                  = 0x1B,
    eTLV_SLQS_AUTH_PREF                 = 0x1D,
    eTLV_SLQS_IP_ADDRESSV4              = 0x1E,
    eTLV_SLQS_PROFILE_ID                = 0x1F,
    eTLV_SLQS_GW_ADDRV4                 = 0x20,
    eTLV_SLQS_SUBNET_MASKV4             = 0x21,
    eTLV_SLQS_PCSCF_ADDR_PCO            = 0x22,
    eTLV_SLQS_PCSCF_SRVR_ADDRLISTV4     = 0x23,
    eTLV_SLQS_PCSCF_FQDN_LIST           = 0x24,
    eTLV_SLQS_IP_ADDRESS_V6             = 0x25,
    eTLV_SLQS_GW_ADDRV6                 = 0x26,
    eTLV_SLQS_PRIM_DNSV6                = 0x27,
    eTLV_SLQS_SECOND_DNSV6              = 0x28,
    eTLV_SLQS_MTU                       = 0x29,
    eTLV_SLQS_DOMAIN_NAME_LIST          = 0x2A,
    eTLV_SLQS_IP_FAMILY                 = 0x2B,
    eTLV_SLQS_IM_CN_FLAG                = 0x2C,
    eTLV_SLQS_TECH_NAME                 = 0x2D
};

/*
 * This structure contains the SLQSGetRuntimeSettings response parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsSLQSGetRuntimeSettingsResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* SLQSGetRuntimeSettings response parameters */
    struct WdsRunTimeSettings WdsSlqsRunTimeSettings;
};

/************
 * Prototypes
 **************/
extern enum eQCWWANError PkQmiWdsSLQSGetRuntimeSettings(
    WORD  *pMlength,
    BYTE  *pBuffer,
    ULONG *pRequestedSettings );

extern enum eQCWWANError UpkQmiWdsSLQSGetRuntimeSettings(
    BYTE                                    *pMdmResp,
    struct QmiWdsSLQSGetRuntimeSettingsResp *pAipResp );

#endif /* __WDS_SLQS_GET_RUNTIME_SETTINGS_MSG_H__ */
