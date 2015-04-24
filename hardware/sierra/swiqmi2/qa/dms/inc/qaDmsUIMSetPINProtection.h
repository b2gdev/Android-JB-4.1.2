/*
 * \ingroup dms
 *
 * \file qaDmsUIMSetPINProtection.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaDmsUIMSetPINProtection.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DMS_UIM_SET_PIN_PROTECTION_H__
#define __DMS_UIM_SET_PIN_PROTECTION_H__

/* enum declarations */

/*
 * An enumeration of eQMI_DMS_UIM_SET_PIN_PROTECTION request TLV IDs
 *
 */
enum eDMS_UIM_SET_PIN_PROTECTION_REQ
{
    eTLV_PIN_PROTECTION_INFORMATION = 0x01
};

/*
 * This structure contains the UIMSetPINProtection request parameters.
 *
 * \sa qaGobiApiDms.h for parameter descriptions
 *
 */
struct QmiDmsUIMSetPINProtectionReq
{
    ULONG id;
    ULONG bEnable;
    CHAR  *pValue;
};

/*
 * An enumeration of eQMI_DMS_UIM_SET_PIN_PROTECTION response TLV IDs
 *
 */
enum eDMS_UIM_SET_PIN_PROTECTION_RESP
{
    eTLV_RETRIES_LEFT = 0x10
};

/*
 * This structure contains the UIMSetPINProtection
 * response parameters
 *
 * \sa qaGobiApiDms.h for parameter descriptions
 *
 */
struct QmiDmsUIMSetPINProtectionResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* UIMSetPINProtection response parameters */
    ULONG *pVerifyRetriesLeft;
    ULONG *pUnblockRetriesLeft;
};

/*
 * Prototypes
 *
 */
extern enum eQCWWANError PkQmiDmsUIMSetPINProtection(
    WORD  *pMlength,
    BYTE  *pBuffer,
    ULONG id,
    ULONG bEnable,
    CHAR  *pValue);

extern enum eQCWWANError UpkQmiDmsUIMSetPINProtection(
    BYTE                                 *pMdmResp,
    struct QmiDmsUIMSetPINProtectionResp *pApiResp);

#endif /* __DMS_UIM_SET_PIN_PROTECTION_H__ */
