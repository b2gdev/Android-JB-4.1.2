/*
 * \ingroup dms
 *
 * \file qaDmsGetDeviceRevisionID.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaDmsGetDeviceRevisionID.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DMS_GET_DEVICE_REVISION_ID_H__
#define __DMS_GET_DEVICE_REVISION_ID_H__

/*
 *
 * An enumeration of eQMI_DMS_GET_REV_ID response TLV IDs
 *
 */
enum eQMI_DMS_GET_DEVICE_REVISION_ID_RESP
{
    eTLV_DEVICE_REVISION_ID    = 0x01,
    eTLV_DEVICE_BOOT_CODE      = 0x10,
    eTLV_DEVICE_PRI            = 0x11,
};

/*
 * This structure contains the Get Revision ID response parameters.
 *
 * \sa qaGobiApiDms.h for parameter descriptions
 *
 */
struct QmiDmsGetDeviceRevIDResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* Response parameters */
    BYTE    amssSize;
    CHAR    *pAMSSString;
    BYTE    bootSize;
    CHAR    *pBootString;
    BYTE    priSize;
    CHAR    *pPRIString;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiDmsGetDeviceRevID (
    USHORT    *pParamLength,
    BYTE      *pParamField);

extern enum eQCWWANError UpkQmiDmsGetDeviceRevID (
    BYTE     *pMdmResp,
    struct   QmiDmsGetDeviceRevIDResp *pAipResp );

#endif /* __DMS_GET_DEVICE_REVISION_ID_H__ */
