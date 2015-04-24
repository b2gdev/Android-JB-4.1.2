/*
 * \ingroup wds
 *
 * \file qaWdsModifyProfile.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaWdsModifyProfile.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __WDS_MODIFY_PROFILE_MSG_H__
#define __WDS_MODIFY_PROFILE_MSG_H__

/* enum declarations */
/*
 *
 * An enumeration of eQMI_WDS_MODIFY_PROFILE request TLV IDs
 *  *
 */
enum eWDS_MODIFY_PROFILE_REQ
{
    eTLV_PROF_TYPE                = 0x01,
    eTLV_NAME                     = 0x10,
    eTLV_PDP_TYP                  = 0x11,
    eTLV_APN_NAM                  = 0x14,
    eTLV_PRIM_DNS                 = 0x15,
    eTLV_PRIM_DNS_V6              = QMI_TLV_PLACEHOLDER,
    eTLV_SECOND_DNS               = 0x16,
    eTLV_SECOND_DNS_V6            = QMI_TLV_PLACEHOLDER,
    eTLV_USERNM                   = 0x1B,
    eTLV_PASSWD                   = 0x1C,
    eTLV_AUTH_PREF                = 0x1D,
    eTLV_IP_ADD                   = 0x1E,
    eTLV_IP_ADD_V6                = QMI_TLV_PLACEHOLDER
};

/*
 * This structure contains the StartDataSession2 request parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */

struct QmiWdsModifyProfileReq
{
    ULONG     profileType;
    BYTE      profileID;
    ULONG     *pPDPType;
    ULONG     *pIPAddress;
    ULONG     *pPrimaryDNS;
    ULONG     *pSecondaryDNS;
    USHORT    *pIPAddressV6;
    USHORT    *pPrimaryDNSV6;
    USHORT    *pSecondaryDNSV6;
    ULONG     *pAuthentication;
    CHAR      *pName;
    CHAR      *pAPNName;
    CHAR      *pUsername;
    CHAR      *pPassword;
};

/*
 * This structure contains the SetDefaultProfile response parameters.
 *
 */
struct QmiWdsModifyProfileResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/************
 * Prototypes
 **************/
extern enum eQCWWANError PkQmiWdsModifyProfile(
    WORD      *pMlength,
    BYTE      *pBuffer,
    ULONG     profileType,
    BYTE      profileID,    
    ULONG     *pPDPType,
    ULONG     *pIPAddress,
    ULONG     *pPrimaryDNS,
    ULONG     *pSecondaryDNS,
    USHORT    *pIPAddressV6,
    USHORT    *pPrimaryDNSV6,
    USHORT    *pSecondaryDNSV6,
    ULONG     *pAuthentication,
    CHAR      *pName,
    CHAR      *pAPNName,
    CHAR      *pUsername,
    CHAR      *pPassword );

extern enum eQCWWANError UpkQmiWdsModifyProfile(
    BYTE     *pMdmResp,
    struct   QmiWdsModifyProfileResp *pAipResp);

#endif /* __WDS_MODIFY_PROFILE_MSG_H__ */
