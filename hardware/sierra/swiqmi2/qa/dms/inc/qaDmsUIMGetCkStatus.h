/*
 * \ingroup dms
 *
 * \file qaDmsUIMGetCkStatus.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaDmsUIMGetCkStatus.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DMS_UIM_GET_CK_STATUS_H__
#define __DMS_UIM_GET_CK_STATUS_H__

/* enum declarations */

/*
 * An enumeration of eQMI_DMS_UIM_GET_CK_STATUS request TLV IDs
 *
 */
enum eDMS_UIM_GET_CK_STATUS_REQ
{
    eTLV_FACILITY = 0x01
};

/*
 * This structure contains the UIMGetControlkeyStatus request parameters.
 *
 * \sa qaGobiApiDms.h for parameter descriptions
 *
 */
struct QmiDmsUIMGetCkStatusReq
{
    ULONG id;
};

/*
 * An enumeration of eQMI_DMS_UIM_GET_CK_STATUS response TLV IDs
 *
 */
enum eDMS_UIM_GET_CK_STATUS_RESP
{
    eTLV_CK_STATUS = 0x11
};

/*
 * This structure contains the UIMGetControlkeyStatus
 * response parameters
 *
 * \sa qaGobiApiDms.h for parameter descriptions
 *
 */
struct QmiDmsUIMGetCkStatusResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* UIMGetCkStatus response parameters */
    ULONG *pStatus;
    ULONG *pVerifyRetriesLeft;
    ULONG *pUnblockRetriesLeft;
};

/*
 * Prototypes
 *
 */
extern enum eQCWWANError PkQmiDmsUIMGetCkStatus(
    WORD  *pMlength,
    BYTE  *pBuffer,
    ULONG id );

extern enum eQCWWANError UpkQmiDmsUIMGetCkStatus(
    BYTE                            *pMdmResp,
    struct QmiDmsUIMGetCkStatusResp *pApiResp);

#endif /* __DMS_UIM_GET_CK_STATUS_H__ */
