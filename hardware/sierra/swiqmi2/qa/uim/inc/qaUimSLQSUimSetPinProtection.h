/*
 * \ingroup uim
 *
 * \file    qaUimSLQSUimSetPinProtection.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaUimSLQSUimSetPinProtection.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __UIM_SLQS_UIM_SET_PIN_PROTECTION_H__
#define __UIM_SLQS_UIM_SET_PIN_PROTECTION_H__

#include "qaGobiApiUim.h"

/* enum declarations */
/*
 * An enumeration of eUIM_SLQS_SET_PIN_PROTECTION_REQ request TLV IDs
 */
enum eUIM_SLQS_SET_PIN_PROTECTION_REQ
{
    eTLV_SET_PP_SESSION_INFO = 0x01,
    eTLV_SET_PIN_PROTECTION  = 0x02,
    eTLV_SET_PP_KEY_REF_ID   = 0x10,
    eTLV_SET_PP_RESP_IND_REQ = 0x11
};

/*
 * An enumeration of eUIM_SLQS_SET_PIN_PROTECTION_RESP request TLV IDs
 */
enum eUIM_SLQS_SET_PIN_PROTECTION_RESP
{
    eTLV_SET_PP_RETRIES_REM    = 0x10,
    eTLV_SET_PP_ENCRYPTED_PIN1 = 0x11,
    eTLV_SET_PP_RESP_IND_RESP  = 0x12
};

/*
 * This structure contains the UimSLQSSetPinProtection response parameters.
 */
struct QmiUimSLQSSetPinProtectionResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    UIMPinResp *pUIMSetPinProtectionResp;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiUimSlqsSetPinProtection(
    WORD                   *pMlength,
    BYTE                   *pBuffer,
    UIMSetPinProtectionReq *pUIMSetPinProtectionReq);

extern enum eQCWWANError UpkQmiUimSlqsSetPinProtection(
    BYTE                                  *pMdmResp,
    struct QmiUimSLQSSetPinProtectionResp *pApiResp );

#endif /* __UIM_SLQS_UIM_SET_PIN_PROTECTION_H__ */
