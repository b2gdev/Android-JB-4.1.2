/*
 * \ingroup uim
 *
 * \file    qaUimSLQSUimChangePin.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaUimSLQSUimChangePin.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __UIM_SLQS_UIM_CHANGE_PIN_H__
#define __UIM_SLQS_UIM_CHANGE_PIN_H__

#include "qaGobiApiUim.h"

/* enum declarations */
/*
 * An enumeration of eUIM_SLQS_CHANGE_PIN_REQ request TLV IDs
 */
enum eUIM_SLQS_CHANGE_PIN_REQ
{
    eTLV_CP_SESSION_INFO   = 0x01,
    eTLV_CHANGE_PIN        = 0x02,
    eTLV_CP_KEY_REF_ID     = 0x10,
    eTLV_CP_TOKEN_IND      = 0x11
};

/*
 * An enumeration of eUIM_SLQS_CHANGE_PIN_RESP request TLV IDs
 */
enum eUIM_SLQS_CHANGE_PIN_RESP
{
    eTLV_CP_RETRIES_REM    = 0x10,
    eTLV_CP_ENCRYPTED_PIN1 = 0x11,
    eTLV_CP_RESP_IND_RESP  = 0x12
};

/*
 * This structure contains the UimSLQSChangePin response parameters.
 */
struct QmiUimSLQSChangePinResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    UIMPinResp *pUIMChangePinResp;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiUimSlqsChangePin(
    WORD            *pMlength,
    BYTE            *pBuffer,
    UIMChangePinReq *pUIMChangePinReq);

extern enum eQCWWANError UpkQmiUimSlqsChangePin(
    BYTE                           *pMdmResp,
    struct QmiUimSLQSChangePinResp *pApiResp );

#endif /* __UIM_SLQS_UIM_CHANGE_PIN_H__ */
