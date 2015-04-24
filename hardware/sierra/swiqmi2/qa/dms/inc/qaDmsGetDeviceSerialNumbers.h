 /*
 * \ingroup dms
 *
 * \file qaDmsGetDeviceSerialNumbers.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaDmsGetDeviceSerialNumbers.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DMS_GET_DEVICE_SERIAL_NUMBERS_H__
#define __DMS_GET_DEVICE_SERIAL_NUMBERS_H__

/*
 *
 * An enumeration of eQMI_DMS_GET_DEVICE_SERIAL_NUMBERS response TLV IDs
 *  
 */
enum DMS_GET_DEVICE_SERIAL_NUMBERS_RESP
{
    eTLV_DEVICE_ESN   = 0x10,
    eTLV_DEVICE_IMEI  = 0x11,
    eTLV_DEVICE_MEID  = 0x12
};

/*
 *
 * This structure contains the Get Device serial numbers
 * response parameters
 *
 * See qaGobiApiDms.h for parameter descriptions
 *
 */
struct QmiDmsGetDeviceSerialNumbersResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* Get Device Serial Numbers response parameters */
    BYTE     esnSize;
    CHAR    *pESNString;
    BYTE     imeiSize;
    CHAR    *pIMEIString;
    BYTE     meidSize;
    CHAR    *pMEIDString;
};

/*
 * Prototypes
 *
 */
extern enum eQCWWANError PkQmiDmsGetDeviceSerialNumbers(
    WORD    *pMlength, 
    BYTE    *pBuffer );

extern enum eQCWWANError UpkQmiDmsGetDeviceSerialNumbers(
    BYTE   *mdmresp,
    struct QmiDmsGetDeviceSerialNumbersResp *apiresp);

#endif /* __DMS_GET_DEVICE_SERIAL_NUMBERS_H__ */
