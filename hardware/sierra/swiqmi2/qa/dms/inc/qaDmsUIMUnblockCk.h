/*
 * \ingroup dms
 *
 * \file    qaDmsUIMUnblockCk.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaDmsUIMUnblockCk.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DMS_UIM_UNBLOCK_CK_H__
#define __DMS_UIM_UNBLOCK_CK_H__

/* enum declarations */

/*
 * An enumeration of eQMI_DMS_UIM_UNBLOCK_CK request TLV IDs
 *
 */
enum eDMS_UIM_UNBLOCK_CK_REQ
{
    eTLV_FACILITY_INFO = 0x01
};

/*
 * This structure contains the UIMUnblockCk request parameters.
 *
 * \sa qaGobiApiDms.h for parameter descriptions
 *
 */
struct QmiDmsUIMUnblockCkReq
{
    ULONG id;
    CHAR  *pValue;
};

/*
 * An enumeration of eQMI_DMS_UIM_UNBLOCK_CK response TLV IDs
 *
 */
enum eDMS_UIM_UNBLOCK_CK_RESP
{
    eTLV_UNBLOCK_RETRIES_LEFT = 0x10
};

/*
 * This structure contains the UIMUnblockCk
 * response parameters
 *
 * \sa qaGobiApiDms.h for parameter descriptions
 *
 */
struct QmiDmsUIMUnblockCkResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* UIMUnblockCk response parameters */
    ULONG *pUnblockRetriesLeft;
};

/*
 * Prototypes
 *
 */
extern enum eQCWWANError PkQmiDmsUIMUnblockCk(
    WORD  *pMlength,
    BYTE  *pBuffer,
    ULONG id,
    CHAR  *pValue );

extern enum eQCWWANError UpkQmiDmsUIMUnblockCk(
    BYTE                          *pMdmResp,
    struct QmiDmsUIMUnblockCkResp *pApiResp );

#endif /* __DMS_UIM_UNBLOCK_CK_H__ */
