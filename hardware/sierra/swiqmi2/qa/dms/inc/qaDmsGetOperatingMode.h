/*
 * \ingroup dms
 *
 * \file qaDmsGetOperatingMode.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaDmsGetOperatingMode.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DMS_GET_OPERATING_MODE_H__
#define __DMS_GET_OPERATING_MODE_H__

/*
 * An enumeration of eQMI_DMS_GET_OPERATING_MODE response TLV IDs
 *
 */
enum DMS_GET_OPERATING_MODE_RESP
{
    eTLV_GET_OPERATING_MODE  = 0x01,
    eTLV_GET_OFFLINE_REASON  = 0x10,
    eTLV_GET_HW_CONTROL_MODE = 0x11
};

/*
 * This structure contains the GetPower response parameters
 *
 * \sa qaGobiApiDms.h for parameter descriptions
 *
 */
struct QmiDmsGetOperatingModeResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* GetPower response parameters */
    ULONG *pOperatingMode;
    ULONG *pOfflineReason;
    ULONG *pHardwareControlledMode;
};

/*
 * Prototypes
 *
 */
extern enum eQCWWANError PkQmiDmsGetOperatingMode(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiDmsGetOperatingMode(
    BYTE                              *pMdmResp,
    struct QmiDmsGetOperatingModeResp *pApiResp );

#endif /* __DMS_GET_OPERATING_MODE_H__ */
