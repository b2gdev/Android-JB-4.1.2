/*
 * \ingroup dms
 *
 * \file qaDmsUIMVerifyPIN.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaDmsUIMVerifyPIN.c
 *
 * Copyright: © 2010 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DMS_UIM_VERIFY_PIN_H__
#define __DMS_UIM_VERIFY_PIN_H__

/* enum declarations */

/*
 * An enumeration of eQMI_DMS_UIM_VERIFY_PIN request TLV IDs
 *
 */
enum eDMS_UIM_VERIFY_PIN_REQ
{
    eTLV_PIN_INFORMATION = 0x01
};

/*
 * This structure contains the UIMVerifyPIN request parameters.
 *
 * \sa qaGobiApiDms.h for parameter descriptions
 */
struct QmiDmsUIMVerifyPINReq
{
    ULONG id;
    CHAR  *pValue;
};

/*
 * An enumeration of eQMI_DMS_UIM_VERIFY_PIN response TLV IDs
 */
enum eDMS_UIM_VERIFY_PIN_RESP
{
    eTLV_LEFT_RETRIES = 0x10
};

/*
 * This structure contains the UIMVerifyPIN
 * response parameters
 *
 * \sa qaGobiApiDms.h for parameter descriptions
 */
struct QmiDmsUIMVerifyPINResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* UIMVerifyPIN response parameters */
    ULONG *pVerifyRetriesLeft;
    ULONG *pUnblockRetriesLeft;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiDmsUIMVerifyPIN(
    WORD  *pMlength,
    BYTE  *pBuffer,
    ULONG id,
    CHAR  *pValue);

extern enum eQCWWANError UpkQmiDmsUIMVerifyPIN(
    BYTE                          *pMdmResp,
    struct QmiDmsUIMVerifyPINResp *pApiResp);

#endif /* __DMS_UIM_VERIFY_PIN_H__ */
