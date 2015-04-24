/*
 * \ingroup dms
 *
 * \file qaDmsGetTime.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaDmsGetTime.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DMS_GET_TIME_H__
#define __DMS_GET_TIME_H__

#define QMI_DMS_TWO_BYTES_SHIFT  16
#define QMI_DMS_SIX_BYTES_SHIFT  48

/*
 * An enumeration of eQMI_DMS_GET_TIME response TLV IDs
 *
 */
enum DMS_GET_TIME_RESP
{
    eTLV_DEVICE_TIME  = 0x01
};

/*
 * This structure contains the GetNetworkTime response parameters
 *
 * \sa qaGobiApiDms.h for parameter descriptions
 *
 */
struct QmiDmsGetTimeResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* GetNetworkTime response parameters */
    ULONGLONG *pDeviceTime;
};

/*
 * Prototypes
 *
 */
extern enum eQCWWANError PkQmiDmsGetTime(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiDmsGetTime(
    BYTE                     *pMdmResp,
    struct QmiDmsGetTimeResp *pApiResp );

#endif /* __DMS_GET_TIME_ID_H__ */
