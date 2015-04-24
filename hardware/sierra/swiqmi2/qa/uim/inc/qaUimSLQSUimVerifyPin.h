/*
 * \ingroup uim
 *
 * \file    qaUimSLQSUimVerifyPin.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaUimSLQSUimVerifyPin.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __UIM_SLQS_UIM_VERIFY_PIN_H__
#define __UIM_SLQS_UIM_VERIFY_PIN_H__

#include "qaGobiApiUim.h"

/* enum declarations */
/*
 * An enumeration of eUIM_SLQS_VERIFY_PIN_REQ request TLV IDs
 */
enum eUIM_SLQS_VERIFY_PIN_REQ
{
    eTLV_VP_SESSION_INFO   = 0x01,
    eTLV_VERIFY_PIN        = 0x02,
    eTLV_VP_ENCRYPTED_PIN1 = 0x10,
    eTLV_VP_KEY_REF_ID     = 0x11,
    eTLV_VP_TOKEN_IND      = 0x12
};

/*
 * An enumeration of eUIM_SLQS_VERIFY_PIN_RESP request TLV IDs
 */
enum eUIM_SLQS_VERIFY_PIN_RESP
{
    eTLV_VP_RETRIES_REM         = 0x10,
    eTLV_VP_ENCRYPTED_PIN1_RESP = 0x11,
    eTLV_VP_RESP_IND_RESP       = 0x12
};

/*
 * This structure contains the UimSLQSVerifyPin response parameters.
 */
struct QmiUimSLQSVerifyPinResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    UIMPinResp *pUIMVerifyPinResp;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiUimSlqsVerifyPin(
    WORD            *pMlength,
    BYTE            *pBuffer,
    UIMVerifyPinReq *pUIMVerifyPinReq);

extern enum eQCWWANError UpkQmiUimSlqsVerifyPin(
    BYTE                           *pMdmResp,
    struct QmiUimSLQSVerifyPinResp *pApiResp );

#endif /* __UIM_SLQS_UIM_VERIFY_PIN_H__ */
