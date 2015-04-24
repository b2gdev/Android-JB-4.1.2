/*
 * \ingroup uim
 *
 * \file    qaUimSLQSUimUnblockPin.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaUimSLQSUimUnblockPin.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __UIM_SLQS_UIM_UNBLOCK_PIN_H__
#define __UIM_SLQS_UIM_UNBLOCK_PIN_H__

#include "qaGobiApiUim.h"

/* enum declarations */
/*
 * An enumeration of eUIM_SLQS_UNBLOCK_PIN_REQ request TLV IDs
 */
enum eUIM_SLQS_UNBLOCK_PIN_REQ
{
    eTLV_UP_SESSION_INFO = 0x01,
    eTLV_UNBLOCK_PIN     = 0x02,
    eTLV_UP_KEY_REF_ID   = 0x10,
    eTLV_UP_TOKEN_IND    = 0x11
};

/*
 * An enumeration of eUIM_SLQS_UNBLOCK_PIN_RESP request TLV IDs
 */
enum eUIM_SLQS_UNBLOCK_PIN_RESP
{
    eTLV_UP_RETRIES_REM    = 0x10,
    eTLV_UP_ENCRYPTED_PIN1 = 0x11,
    eTLV_UP_RESP_IND_RESP  = 0x12
};

/*
 * This structure contains the UimSLQSUnblockPin response parameters.
 */
struct QmiUimSLQSUnblockPinResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    UIMPinResp *pUIMUnblockPinResp;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiUimSlqsUnblockPin(
    WORD             *pMlength,
    BYTE             *pBuffer,
    UIMUnblockPinReq *pUIMUnblockPinReq);

extern enum eQCWWANError UpkQmiUimSlqsUnblockPin(
    BYTE                           *pMdmResp,
    struct QmiUimSLQSUnblockPinResp *pApiResp );

#endif /* __UIM_SLQS_UIM_UNBLOCK_PIN_H__ */
