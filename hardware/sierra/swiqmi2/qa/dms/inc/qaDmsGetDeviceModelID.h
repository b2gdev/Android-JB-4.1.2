/*
 * \ingroup dms
 *
 * \file qaDmsGetDeviceModelID.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaDmsGetDeviceModelID.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DMS_GET_DEVICE_MODEL_ID_H__
#define __DMS_GET_DEVICE_MODEL_ID_H__

/*
 *
 * An enumeration of eQMI_DMS_GET_MODE_ID response TLV IDs
 * 
 */
enum eQMI_DMS_GET_DEVICE_MODEL_ID_RESP
{
    eTLV_DEVICE_MODEL    = 0x01,
};

/*
 * This structure contains the Get Model ID response parameters.
 *
 * \sa qaGobiApiDms.h for parameter descriptions
 *
 */
struct QmiDmsGetDeviceModelIDResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* Response parameters */
    CHAR    *pString;
    BYTE    stringSize;
};

/************
 * Prototypes
 **************/
extern enum eQCWWANError PkQmiDmsGetDeviceModelID (
    WORD    *pMlength,
    BYTE    *pParamField );

extern enum eQCWWANError UpkQmiDmsGetDeviceModelID (
    BYTE     *pMdmResp,
    struct   QmiDmsGetDeviceModelIDResp *pAipResp );

#endif /* __DMS_GET_DEVICE_MODEL_ID_H__ */
