 /*
 * \ingroup dms
 *
 * \file    qaDmsGetDeviceHardwareRev.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaDmsGetDeviceHardwareRev.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
 
#ifndef __DMS_GET_DEVICE_HARDWARE_REV_H__
#define __DMS_GET_DEVICE_HARDWARE_REV_H__

/*
 *
 * An enumeration of eQMI_DMS_GET_DEVICE_HARDWARE_REV response TLV IDs
 *  
 */
enum DMS_GET_DEVICE_HARDWARE_REV_RESP
{
    eTLV_HARQWARE_REV  = 0x01,
};

/*
 *
 * This structure contains the Get Device hardware version
 * response parameters
 *
 * \sa qaGobiApiDms.h for parameter descriptions
 *
 */
struct QmiDmsGetDeviceHardwareRevResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* GetMSISDN response parameters */
    BYTE     stringSize;
    CHAR    *pString;
};

/*
 * Prototypes
 *
 */
extern enum eQCWWANError PkQmiDmsGetDeviceHardwareRev(
    WORD    *pMlength, 
    BYTE    *pBuffer );

extern enum eQCWWANError UpkQmiDmsGetDeviceHardwareRev(
    BYTE   *mdmresp,
    struct QmiDmsGetDeviceHardwareRevResp *apiresp);

#endif /* __DMS_GET_DEVICE_HARDWARE_REV_H__ */
