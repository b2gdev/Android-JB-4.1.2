/*
 * \ingroup wds
 *
 * \file qaWdsStartNetworkInterface.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaWdsStartNetworkInterface.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __WDS_START_NETWORK_INTERFACE_MSG_H__
#define __WDS_START_NETWORK_INTERFACE_MSG_H__

#define QMI_WDS_TECH_3GPP  0x01
#define QMI_WDS_TECH_3GPP2 0x02

#define QMI_WDS_TECH_UMTS  0x01
#define QMI_WDS_TECH_CDMA  0x02
#define QMI_WDS_TECH_eMBMS 0x03
#define QMI_WDS_TECH_mLINK 0x04

#define QMI_WDS_TECH_UMTS_VALUE  0x8004
#define QMI_WDS_TECH_CDMA_VALUE  0x8001
#define QMI_WDS_TECH_eMBMS_VALUE 0x8801
#define QMI_WDS_TECH_mLINK_VALUE 0x7FFF

#define QMI_WDS_IP_FAMILY_V6 0x06
#define QMI_WDS_IP_FAMILY_V4 0x04

#define eWDS_STOP_NETWORK_INTERFACE_AUTOCONNECT_SETTING_UNCHANGED 0
#define eWDS_STOP_NETWORK_INTERFACE_AUTOCONNECT_SETTING_DISABLE 1

/* enum declarations */

/* eQMI_WDS_START_NET request TLV IDs */
enum eWDS_START_NETWORK_INTERFACE_REQ
{
    eTLV_TECH_PREFERENCE             = 0x34,
    eTLV_3GPP_CONFIG_PROFILE_ID      = 0x31,
    eTLV_3GPP2_CONFIG_PROFILE_ID     = 0x32,
    eTLV_PRIMARY_DNS_PREFERENCE      = 0x10,
    eTLV_SECONDARY_DNS_PREFERENCE    = 0x11,
    eTLV_PRIMARY_NBNS_PREFERENCE     = 0x12,
    eTLV_SECONDARY_NBNS_PREFERENCE   = 0x13,
    eTLV_PRIMARY_DNS_PREFERENCE_V6   = 0x27,
    eTLV_SECONDARY_DNS_PREFERENCE_V6 = 0x28,
    eTLV_APN_NAME                    = 0x14,
    eTLV_ADDR_PREFERENCE             = 0x15,
    eTLV_ADDR_PREFERENCE_V6          = 0x25,
    eTLV_AUTH_PREFERENCE             = 0x16,
    eTLV_USERNAME                    = 0x17,
    eTLV_PASSWORD                    = 0x18,
    eTLV_IP_FAMILY_PREFERENCE        = 0x19,
    eTLV_ENABLE_AUTOCONNECT          = 0x33
};

/*
 * StartDataSession2 request structure
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsStartNetworkInterfaceReq
{
    ULONG  *pTechnology;
    ULONG  *pPrimaryDNS;     /* Ipv4 */
    ULONG  *pSecondaryDNS;   /* Ipv4 */
    ULONG  *pPrimaryNBNS;    /* Ipv4 */
    ULONG  *pSecondaryNBNS;
    USHORT *pPrimaryDNSv6;
    USHORT *pSecondaryDNSv6;
    CHAR   *pAPNName;
    ULONG  *pIPAddress;
    USHORT *pIPAddressv6;
    ULONG  *pAuthentication;
    CHAR   *pUsername;
    CHAR   *pPassword;
    ULONG  *pSessionId;
    ULONG  *pFailureReason;
    BYTE   *pIPFamilyPreference;
    BYTE   enableAutoconnect;
};

/*
 * SLQSStartStopDataSession request structure
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsStartNwkInterfaceReq
{
    ULONG *pTechnology;
    ULONG *pProfileId3GPP;
    ULONG *pProfileId3GPP2;
};

/* eQMI_WDS_START_NET response TLV IDs */
enum WDS_START_NETWORK_INTERFACE_RESP
{
    eTLV_PACKET_DATA_HANDLE      = 0x01,
    eTLV_CALL_END_REASON         = 0x10,
    eTLV_VERBOSE_CALL_END_REASON = 0x11
};

/*
 * StartDataSession2 response structure
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsStartNetworkInterfaceResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* StartDataSession2 response parameters */
    ULONG *pPktDataHandle;
    ULONG *pFailureReason;
    ULONG *pVerboseFailureReason;
};

/* Prototypes */
extern enum eQCWWANError PkQmiWdsStartNetworkInterface(
    WORD   *pMlength,
    BYTE   *pParamField,
    ULONG  *pTechnology,
    ULONG  *pPrimaryDNS,
    ULONG  *pSecondaryDNS,
    ULONG  *pPrimaryNBNS,
    ULONG  *pSecondaryNBNS,
    USHORT *pPrimaryDNSv6,
    USHORT *pSecondaryDNSv6,
    CHAR   *pApnName,
    ULONG  *pIpAddressPreference,
    USHORT *pIPAddressv6,
    ULONG  *pAuthenticationPreference,
    CHAR   *pUserName,
    CHAR   *pPassword,
    ULONG  *pSessionId,
    ULONG  *pFailureReason,
    BYTE   *pIPFamilyPreference,
    BYTE   enableAutoconnect );

extern enum eQCWWANError PkQmiWdsStartNetworkInterface2(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG *pTechnology,
    ULONG *pProfileId3GPP,
    ULONG *pProfileId3GPP2 );

extern enum eQCWWANError UpkQmiWdsStartNetworkInterface2(
    BYTE                                   *pMdmResp,
    struct QmiWdsStartNetworkInterfaceResp *pAipResp );

#endif /* _WDS_START_NETWORK_INTERFACE_MSG_H__ */
