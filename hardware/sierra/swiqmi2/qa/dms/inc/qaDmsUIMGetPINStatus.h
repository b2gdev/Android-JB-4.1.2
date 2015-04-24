/*
 * \ingroup dms
 *
 * \file qaDmsUIMGetPINStatus.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaDmsUIMGetPINStatus.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DMS_UIM_GET_PIN_STATUS_H__
#define __DMS_UIM_GET_PIN_STATUS_H__

/* enum declarations */

/*
 * An enumeration of eQMI_DMS_UIM_GET_PIN_STATUS response TLV IDs
 *
 */
enum eDMS_UIM_GET_PIN_STATUS__RESP
{
    eTLV_PIN_1_STATUS = 0x11,
    eTLV_PIN_2_STATUS = 0x12
};

/*
 * This structure contains the UIMGetPINStatus
 * response parameters
 *
 * \sa qaGobiApiDms.h for parameter descriptions
 *
 */
struct QmiDmsUIMGetPINStatusResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* UIMGetPINStatus response parameters */
    ULONG id;
    ULONG *pStatus;
    ULONG *pVerifyRetriesLeft;
    ULONG *pUnblockRetriesLeft;
};

/*
 * Prototypes
 *
 */
extern enum eQCWWANError PkQmiDmsUIMGetPINStatus(
    WORD  *pMlength,
    BYTE  *pBuffer );

extern enum eQCWWANError UpkQmiDmsUIMGetPINStatus(
    BYTE                             *pMdmResp,
    struct QmiDmsUIMGetPINStatusResp *pApiResp );

#endif /* __DMS_UIM_GET_PIN_STATUS_H__ */
