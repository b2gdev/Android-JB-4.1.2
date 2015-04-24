/*
 * \ingroup pds
 *
 * \file    qaPdsSetComPortConfig.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaPdsSetComPortConfig.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __PDS_SET_COM_PORT_CONFIG_MSG_H__
#define __PDS_SET_COM_PORT_CONFIG_MSG_H__

/* enum declarations */
/*
 * An enumeration of eQMI_PDS_SET_COM_PORT_CONFIG request TLV IDs
 */
enum ePDS_SET_COM_PORT_CONFIG_REQ
{
    eTLV_SET_COM_PORT_CONFIG = 0x01
};

/*
 * This structure contains the SetPortAutomaticTracking request
 * parameters.
 */
struct QmiPdsSetComPortConfigReq
{
    ULONG bAuto;
};

/*
 * This structure contains the SetPortAutomaticTracking response
 * parameters.
 */
struct QmiPdsSetComPortConfigResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiPdsSetComPortConfig(
    WORD  *pMlength,
    BYTE  *pBuffer,
    ULONG bAuto );

extern enum eQCWWANError UpkQmiPdsSetComPortConfig(
    BYTE                              *pMdmResp,
    struct QmiPdsSetComPortConfigResp *pApiResp );

#endif /* __PDS_SET_COM_PORT_CONFIG_MSG_H__ */
