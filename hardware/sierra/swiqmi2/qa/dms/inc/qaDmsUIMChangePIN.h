/*
 * \ingroup dms
 *
 * \file qaDmsUIMChangePIN.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaDmsUIMChangePIN.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DMS_UIM_CHANGE_PIN_H__
#define __DMS_UIM_CHANGE_PIN_H__

/* enum declarations */

/*
 * An enumeration of eQMI_DMS_UIM_CHANGE_PIN request TLV IDs
 *
 */
enum eDMS_UIM_CHANGE_PIN_REQ
{
    eTLV_PIN_CHANGE_INFORMATION = 0x01
};

/*
 * This structure contains the UIMChangePIN request parameters.
 *
 * \sa qaGobiApiDms.h for parameter descriptions
 *
 */
struct QmiDmsUIMChangePINReq
{
    ULONG id;
    CHAR  *pOldValue;
    CHAR  *pNewValue;
};

/*
 * An enumeration of eQMI_DMS_UIM_CHANGE_PIN response TLV IDs
 *
 */
enum eDMS_UIM_CHANGE_PIN_RESP
{
    eTLV_REMAINING_RETRIES = 0x10
};

/*
 * This structure contains the UIMChangePIN
 * response parameters
 *
 * \sa qaGobiApiDms.h for parameter descriptions
 *
 */
struct QmiDmsUIMChangePINResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* UIMChangePIN response parameters */
    ULONG *pVerifyRetriesLeft;
    ULONG *pUnblockRetriesLeft;
};

/*
 * Prototypes
 *
 */
extern enum eQCWWANError PkQmiDmsUIMChangePIN(
    WORD  *pMlength,
    BYTE  *pBuffer,
    ULONG id,
    CHAR  *pOldValue,
    CHAR  *pNewValue );

extern enum eQCWWANError UpkQmiDmsUIMChangePIN(
    BYTE                          *pMdmResp,
    struct QmiDmsUIMChangePINResp *pApiResp);

#endif /* __DMS_UIM_CHANGE_PIN_H__ */
