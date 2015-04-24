/*
 * \ingroup uim
 *
 * \file    qaUimSLQSUimGetFileAttributes.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaUimSLQSUimGetFileAttributes.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __UIM_SLQS_UIM_GET_FILE_ATTRIBUTES_H__
#define __UIM_SLQS_UIM_GET_FILE_ATTRIBUTES_H__

#include "qaGobiApiUim.h"

/* enum declarations */
/*
 * An enumeration of eUIM_SLQS_GET_FILE_ATTRIBUTES_REQ request TLV IDs
 */
enum eUIM_SLQS_GET_FILE_ATTRIBUTES_REQ
{
    eTLV_GET_FA_SESS_INFO = 0x01,
    eTLV_GET_FA_FILE_ID   = 0x02,
    eTLV_GET_FA_RESP_IND  = 0x10
};

/*
 * An enumeration of eUIM_SLQS_GET_FILE_ATTRIBUTES_RESP request TLV IDs
 */
enum eUIM_SLQS_GET_FILE_ATTRIBUTES_RESP
{
    eTLV_GET_FA_CARD_RESULT   = 0x10,
    eTLV_GET_FILE_ATTRIBUTES  = 0x11,
    eTLV_GET_FA_RESP_IND_RESP = 0x12
};

/*
 * This structure contains the UimSLQSGetFileAttributes response parameters.
 */
struct QmiUimSLQSGetFileAttributesResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    UIMGetFileAttributesResp *pUIMGetFileAttributesResp;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiUimSlqsGetFileAttributes(
    WORD                    *pMlength,
    BYTE                    *pBuffer,
    UIMGetFileAttributesReq *pUIMGetFileAttributesReq);

extern enum eQCWWANError UpkQmiUimSlqsGetFileAttributes(
    BYTE                                   *pMdmResp,
    struct QmiUimSLQSGetFileAttributesResp *pApiResp );

#endif /* __UIM_SLQS_UIM_GET_FILE_ATTRIBUTES_H__ */
