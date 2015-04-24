/*
 * \ingroup  dms
 *
 * \file  qaDmsGetDeviceCap.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaDmsGetDeviceCap.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DMS_GET_DEVICE_CAP_MSG_H__
#define __DMS_GET_DEVICE_CAP_MSG_H__

/* enum declarations */

/*
 *
 * An enumeration of eQMI_DMS_GET_RF_INFO response TLV IDs
 *
 */
enum eQMI_DMS_GET_RF_INFO_RESP
{
    eTLV_DEVICE_CAP = 0x01,

};
/*
 *
 * This structure contains the GetDeviceCapabilities response parameters
 *
 * \sa qaGobiApiDms.h for parameter descriptions
 *
 */
struct QmiDmsGetDeviceCapResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
    /* Response message Mandatory TLVs */
    ULONG *pMaxTXChannelRate;
    ULONG *pMaxRXChannelRate;
    ULONG *pDataServiceCapability;
    ULONG *pSimCapability;
    ULONG *pRadioIfacesSize;
    BYTE  *pRadioIfaces;
};

/*
 * Prototypes
 *
 */
extern enum eQCWWANError PkQmiDmsGetDeviceCap(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiDmsGetDeviceCap(
    BYTE                           *pMdmResp,
    struct QmiDmsGetDeviceCapResp *pApiResp );

#endif /* __DMS_GET_DEVICE_CAP_MSG_H__ */
