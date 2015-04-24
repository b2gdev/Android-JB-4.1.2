/*
 * \ingroup dms
 *
 * \file    qaDmsUIMSetCkProtection.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaDmsUIMSetCkProtection.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DMS_UIM_SET_CK_PROTECTION_H__
#define __DMS_UIM_SET_CK_PROTECTION_H__

/* enum declarations */

/*
 * An enumeration of eQMI_DMS_UIM_SET_CK_PROTECTION request TLV IDs
 *
 */
enum eDMS_UIM_SET_CK_PROTECTION_REQ
{
    eTLV_SET_FACILITY = 0x01
};

/*
 * This structure contains the UIMSetControlKeyProtection request parameters.
 *
 * \sa qaGobiApiDms.h for parameter descriptions
 *
 */
struct QmiDmsUIMSetCkProtectionReq
{
    ULONG id;
    ULONG status;
    CHAR  *pValue;
};

/*
 * An enumeration of eQMI_DMS_UIM_SET_CK_PROTECTION response TLV IDs
 *
 */
enum eDMS_UIM_SET_CK_PROTECTION_RESP
{
    eTLV_VERIFY_RETRIES_LEFT = 0x10
};

/*
 * This structure contains the UIMSetControlKeyProtection
 * response parameters
 *
 * \sa qaGobiApiDms.h for parameter descriptions
 *
 */
struct QmiDmsUIMSetCkProtectionResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* UIMSetControlKeyProtection response parameters */
    ULONG *pVerifyRetriesLeft;
};

/*
 * Prototypes
 *
 */
extern enum eQCWWANError PkQmiDmsUIMSetCkProtection(
    WORD  *pMlength,
    BYTE  *pBuffer,
    ULONG id,
    ULONG status,
    CHAR  *pValue );

extern enum eQCWWANError UpkQmiDmsUIMSetCkProtection(
    BYTE                                *pMdmResp,
    struct QmiDmsUIMSetCkProtectionResp *pApiResp );

#endif /* __DMS_UIM_SET_CK_PROTECTION_H__ */
