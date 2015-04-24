/*
 * \ingroup wds
 *
 * \file qaWdsGetDefaultSettings.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaWdsGetDefaultSettings.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __WDS_GET_DEFAULT_SETTINGS_MSG_H__
#define __WDS_GET_DEFAULT_SETTINGS_MSG_H__

/* For Parameter Validations */

#define QMI_WDS_PROFILE_TYPE_UMTS 0x00

/* enum declarations */

/*
 * An enumeration of eQMI_WDS_GET_DEFAULTS request TLV IDs
 */
enum eWDS_GET_DEFAULT_SETTINGS_REQ
{
    eTLV_PROFILE_TYPE = 0x01
};

/*
 * This structure contains the GetDefaultProfile request parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsGetDefaultSettingsReq
{
    ULONG profileType;
};

/*
 * An enumeration of eQMI_WDS_GET_DEFAULTS response TLV IDs
 */
enum eWDS_GET_DEFAULT_SETTINGS_RESP
{
    eTLV_PROFILE_NAME     = 0x10,
    eTLV_PDP_TYPE         = 0x11,
    eTLV_IP_ADDR          = 0x1E, /* IPv4 */
    eTLV_PRIMARY_DNS      = 0x15, /* IPv4 */
    eTLV_SECONDARY_DNS    = 0x16, /* IPv4 */
    eTLV_IP_ADDR_V6       = QMI_TLV_PLACEHOLDER,
    eTLV_PRIMARY_DNS_V6   = QMI_TLV_PLACEHOLDER,
    eTLV_SECONDARY_DNS_V6 = QMI_TLV_PLACEHOLDER,
    eTLV_AUTHENTICATION   = 0x1D,
    eTLV_APNNAME          = 0x14,
    eTLV_USER_NAME        = 0x1B
};

/*
 * This structure contains the GetDefaultProfile response
 * parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsGetDefaultSettingsResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* GetDefaultSettings response parameters */
    ULONG  *pPDPType;
    ULONG  *pIPAddress;
    ULONG  *pPrimaryDNS;
    ULONG  *pSecondaryDNS;
    USHORT *pIPAddressv6;
    USHORT *pPrimaryDNSv6;
    USHORT *pSecondaryDNSv6;
    ULONG  *pAuthentication;
    BYTE   nameSize;
    CHAR   *pName;
    BYTE   apnSize;
    CHAR   *pAPNName;
    BYTE   userSize;
    CHAR   *pUserName;
};

/* Prototypes */
extern enum eQCWWANError PkQmiWdsGetDefaultSettings(
    WORD  *pMlength,
    BYTE  *pBuffer,
    ULONG profileType );

extern enum eQCWWANError UpkQmiWdsGetDefaultSettings(
    BYTE   *pMdmResp,
    struct QmiWdsGetDefaultSettingsResp *pAipResp );

#endif /* _WDS_GET_DEFAULT_SETTINGS_MSG_H__ */
