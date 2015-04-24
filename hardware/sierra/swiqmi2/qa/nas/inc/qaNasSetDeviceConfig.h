/*
 * \ingroup nas
 *
 * \file  qaNasSetDeviceConfig.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaNasSetDeviceConfig.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_SET_DEVICE_CONFIG_MSG_H__
#define __NAS_SET_DEVICE_CONFIG_MSG_H__

/* enum declarations */

/*
 *
 * An enumeration of eQMI_NAS_SET_NET_PARAMS request TLV IDs
 *
 */
enum eQMI_NAS_SET_NET_PARAMS_REQ
{
    eTLV_SET_SPC               = 0x10,
    eTLV_FORCE_HDR_REV         = 0x14,
    eTLV_HDR_SCP_CUSTOM_CONFIG = 0x15,
    eTLV_ROAM_PREF             = 0x16
};

/*
 * This structure contains the SetDeviceConfig request parameters.
 *
 * \sa qaGobiApiNas.h for parameter descriptions
 *
 */
struct QmiNasSetDeviceConfigReq
{
    CHAR  *pSpc;
    BYTE  *pForceHdrRev;
    BYTE  *pState;
    ULONG *pProtocol;
    ULONG *pBroadcast;
    ULONG *pApplication;
    ULONG *pRoaming;
};

/*
 * This structure contains the SetDeviceConfig response parameters
 *
 * \sa qaGobiApiNas.h for parameter descriptions
 *
 */
struct QmiNasSetDeviceConfigResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 *
 */
extern enum eQCWWANError PkQmiNasSetDeviceConfig(
    WORD  *pMlength,
    BYTE  *pBuffer,
    CHAR  *pSpc,
    BYTE  *pForceHdrRev,
    BYTE  *pState,
    ULONG *pProtocol,
    ULONG *pBroadcast,
    ULONG *pApplication,
    ULONG *pRoaming);

extern enum eQCWWANError UpkQmiNasSetDeviceConfig(
    BYTE                             *pMdmresp,
    struct QmiNasSetDeviceConfigResp *pApiresp);

#endif /* __NAS_SET_DEVICE_CONFIG_MSG_H__ */
