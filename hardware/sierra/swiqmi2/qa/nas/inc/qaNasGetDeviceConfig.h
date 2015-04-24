/*
 * \ingroup nas
 *
 * \file qaNasGetDeviceConfig.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaNasGetDeviceConfig.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_GET_DEVICE_CONFIG_MSG_H__
#define __NAS_GET_DEVICE_CONFIG_MSG_H__

/*
 *
 * An enumeration of eQMI_NAS_GET_NET_PARAMS response TLV IDs
 *
 */
enum eQMI_NAS_GET_NET_PARAMS_RESP
{
    eTLV_GET_SCI                   = 0x11,
    eTLV_GET_SCM                   = 0x12,
    eTLV_REG_PARAMS                = 0x13,
    eTLV_GET_FORCE_HDR_REV         = 0x14,
    eTLV_GET_HDR_SPC_CUSTOM_CONFIG = 0x15,
    eTLV_GET_ROAM_PREF             = 0x16
};

/*
 * This structure contains the GetDeviceConfig response parameters.
 *
 * \sa  qaGobiApiNas.h for parameter descriptions
 *
 */
struct QmiNasGetDeviceConfigResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* Response parameters */
    BYTE  *pSCI;
    BYTE  *pSCM;
    BYTE  *pRegHomeSID;
    BYTE  *pRegForeignSID;
    BYTE  *pRegForeignNID;
    BYTE  *pForceRev0;
    BYTE  *pCustomSCP;
    ULONG *pProtocol;
    ULONG *pBroadcast;
    ULONG *pApplication;
    ULONG *pRoaming;
};


/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiNasGetDeviceConfig(
    WORD *pMlength,
    BYTE *pParamField );

extern enum eQCWWANError UpkQmiNasGetDeviceConfig(
    BYTE                             *pMdmResp,
    struct QmiNasGetDeviceConfigResp *pApiResp );

#endif /* __NAS_GET_DEVICE_CONFIG_MSG_H__ */
