/*
 * \ingroup dms
 *
 * \file    qaDmsUIMGetICCID.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaDmsUIMGetICCID.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DMS_UIM_GET_ICCID_H__
#define __DMS_UIM_GET_ICCID_H__

/*
 * An enumeration of eQMI_DMS_UIM_GET_ICCID response TLV IDs
 *
 */
enum eDMS_UIM_GET_ICCID_RESP
{
    eTLV_UIM_ICCID = 0x01
};

/*
 * This structure contains the UIMGetICCID response parameters
 *
 * \sa qaGobiApiDms.h for parameter descriptions
 *
 */
struct QmiDmsUIMGetICCIDResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* UIMGetICCID response parameters */
    BYTE stringSize;
    CHAR *pString;
};

/*
 * Prototypes
 *
 */
extern enum eQCWWANError PkQmiDmsUIMGetICCID(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiDmsUIMGetICCID(
    BYTE                         *pMdmResp,
    struct QmiDmsUIMGetICCIDResp *pApiResp );

#endif /* __DMS_UIM_GET_ICCID_H__ */
