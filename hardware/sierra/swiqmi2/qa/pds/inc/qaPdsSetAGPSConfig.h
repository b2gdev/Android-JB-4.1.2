/*
 * \ingroup pds
 *
 * \file    qaPdsSetAGPSConfig.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaPdsSetAGPSConfig.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __PDS_SET_AGPS_CONFIG_H__
#define __PDS_SET_AGPS_CONFIG_H__

/* enum declarations */
/*
 * An enumeration of eQMI_PDS_SET_AGPS_CONFIG request TLV IDs
 */
enum ePDS_SET_AGPS_CONFIG_REQ
{
    eTLV_SET_AGPS_LOCATION_SERVER     = 0x10,
    eTLV_SET_AGPS_LOCATION_SERVER_URL = 0x11
};

/*
 * This structure contains the SetAGPSConfig request parameters.
 */
struct QmiPdsSetAGPSConfigReq
{
    ULONG *pServerAddress;
    ULONG *pServerPort;
    BYTE  *pServerURL;
    BYTE  *pServerURLLength;
};

/*
 * This structure contains the SetAGPSConfig response parameters.
 */
struct QmiPdsSetAGPSConfigResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiPdsSetAGPSConfig(
    WORD  *pMlength,
    BYTE  *pBuffer,
    ULONG *pServerAddress,
    ULONG *pServerPort,
    BYTE  *pServerURL,
    BYTE  *pServerURLLength );

extern enum eQCWWANError UpkQmiPdsSetAGPSConfig(
    BYTE                           *pMdmResp,
    struct QmiPdsSetAGPSConfigResp *pApiResp );

#endif /* __PDS_SET_AGPS_CONFIG_H__ */
