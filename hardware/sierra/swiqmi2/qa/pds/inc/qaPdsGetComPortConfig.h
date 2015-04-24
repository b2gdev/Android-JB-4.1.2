/*
 * \ingroup pds
 *
 * \file    qaPdsGetComPortConfig.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaPdsGetComPortConfig.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __PDS_GET_COM_PORT_CONFIG_H__
#define __PDS_GET_COM_PORT_CONFIG_H__

/*
 * An enumeration of eQMI_PDS_GET_COM_PORT_CONFIG response TLV IDs
 */
enum eQMI_PDS_GET_COM_PORT_CONFIG_RESP
{
    eTLV_GET_COM_PORT_CONFIG = 0x01
};

/*
 * An enumeration of eQMI_PDS_GET_COM_PORT_CONFIG response TLV Lengths
 */
enum eQMI_PDS_GET_COM_PORT_CONFIG_RESP_LENGTH
{
    eTLV_GET_COM_PORT_CONFIG_LENGTH = 0x01
};

/*
 * This structure contains the GetPortAutomaticTracking response
 * parameters
 */
struct QmiPdsGetComPortConfigResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* GetPortAutomaticTracking response parameters */
    ULONG *pbAuto;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiPdsGetComPortConfig(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiPdsGetComPortConfig(
    BYTE                              *pMdmResp,
    struct QmiPdsGetComPortConfigResp *pApiResp );

#endif /* __PDS_GET_COM_PORT_CONFIG_H__ */
