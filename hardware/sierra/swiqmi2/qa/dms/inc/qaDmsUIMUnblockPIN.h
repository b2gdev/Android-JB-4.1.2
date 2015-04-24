/*
 * \ingroup dms
 *
 * \file qaDmsUIMUnblockPIN.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaDmsUIMUnblockPIN.c
 *
 * Copyright: © 2010 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DMS_UIM_UNBLOCK_PIN_H__
#define __DMS_UIM_UNBLOCK_PIN_H__

/* enum declarations */

/*
 * An enumeration of eQMI_DMS_UIM_UNBLOCK_PIN request TLV IDs
 *
 */
enum eDMS_UIM_UNBLOCK_PIN_REQ
{
    eTLV_PIN_UNBLOCK_INFORMATION = 0x01
};

/*
 * This structure contains the UIMUnblockPIN request parameters.
 *
 * \sa qaGobiApiDms.h for parameter descriptions
 */
struct QmiDmsUIMUnblockPINReq
{
    ULONG id;
    CHAR  *pPUKValue;
    CHAR  *pNewValue;
};

/*
 * An enumeration of eQMI_DMS_UIM_UNBLOCK_PIN response TLV IDs
 */
enum eDMS_UIM_UNBLOCK_PIN__RESP
{
    eTLV_RETRIES_REMAINING = 0x10
};

/*
 * This structure contains the UIMUnblockPIN response parameters
 *
 * \sa qaGobiApiDms.h for parameter descriptions
 */
struct QmiDmsUIMUnblockPINResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* UIMUnblockPIN response parameters */
    ULONG *pVerifyRetriesLeft;
    ULONG *pUnblockRetriesLeft;
};

/*
 * Prototypes
 *
 */
extern enum eQCWWANError PkQmiDmsUIMUnblockPIN(
    WORD  *pMlength,
    BYTE  *pBuffer,
    ULONG id,
    CHAR  *pPUKValue,
    CHAR  *pNewValue );

extern enum eQCWWANError UpkQmiDmsUIMUnblockPIN(
    BYTE                           *pMdmResp,
    struct QmiDmsUIMUnblockPINResp *pApiResp);

#endif /* __DMS_UIM_UNBLOCK_PIN_H__ */
