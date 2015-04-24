/*
 * \ingroup dms
 *
 * \file    qaDmsGetDeviceMfr.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaDmsGetDeviceMfr.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DMS_GET_DEVICE_MFR_H__
#define __DMS_GET_DEVICE_MFR_H__

/*
 *
 * An enumeration of eQMI_DMS_GET_DEVICE_MFR response TLV IDs
 *
 */
enum eQMI_DMS_GET_DEVICE_MFR_RESP
{
    eTLV_DEVICE_MANUFACTURER    = 0x01,
};

/*
 * This structure contains the GetDeviceManufacturer() response parameters.
 *
 * \sa qaGobiApiDms.h for parameter descriptions
 *
 */
struct QmiDmsGetDeviceMfrResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* Response parameters */
    BYTE     stringSize;
    CHAR    *pString;
};

/************
 * Prototypes
 **************/
extern enum eQCWWANError PkQmiDmsGetDeviceMfr (
    WORD    *pLength,
    BYTE    *pParamField );

extern enum eQCWWANError UpkQmiDmsGetDeviceMfr (
    BYTE     *pMdmResp,
    struct   QmiDmsGetDeviceMfrResp *pAipResp );

#endif /* __DMS_GET_DEVICE_MFR_H__ */
