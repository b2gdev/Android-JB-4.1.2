
/*
 * \ingroup wds
 *
 * Filename: qaWdsGetRuntimeSettings.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaWdsGetRuntimeSettings.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __WDS_GET_RUNTIME_SETTINGS_MSG_H__
#define __WDS_GET_RUNTIME_SETTINGS_MSG_H__
#define  BIT8    (0x00000001 << 8)

/* enum declarations */
/*
 *
 * An enumeration of eQMI_WDS_GET_RUNTIME_SETTINGS request TLV IDs
 *  *
 */
enum eWDS_GET_RUNTIME_SETTINGS_REQ
{
    eTLV_REQUESTED_SETTINGS        = 0x10
};

/*
 * This structure contains the GetIPAddress request parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */

struct QmiWdsGetRuntimeSettingsReq
{
    ULONG    *pRequestedSettings;
};

/*
 *
 * An enumeration of eQMI_WDS_START_NET response TLV IDs
 *  *
 */
enum eWDS_GET_RUNTIME_SETTINGS_RESP
{
    eTLV_IP_ADDRESS        = 0x1E,
    eTLV_IP_ADDRESS_V6     = 0x25
};

/*
 * This structure contains the GetIPAddress response parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsGetRuntimeSettingsResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* GetIPAddress response parameters */
    ULONG   *pIPAddress;
    USHORT  *pIPAddressV6;
    BYTE    *pIPv6prefixlen;
};

/************
 * Prototypes
 **************/
extern enum eQCWWANError PkQmiWdsGetRuntimeSettings(
    WORD    *pMlength,
    BYTE    *pBuffer,
    ULONG   *pRequestedSettings );

extern enum eQCWWANError UpkQmiWdsGetRuntimeSettings(
    BYTE      *pMdmResp,
    struct    QmiWdsGetRuntimeSettingsResp *pAipResp);

#endif /* __WDS_GET_RUNTIME_SETTINGS_MSG_H__ */
