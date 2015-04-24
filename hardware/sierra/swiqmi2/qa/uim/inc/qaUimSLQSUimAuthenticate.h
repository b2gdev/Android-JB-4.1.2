/*
 * \ingroup uim
 *
 * \file    qaUimSLQSUimAuthenticate.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaUimSLQSUimAuthenticate.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __UIM_SLQS_UIM_AUTHENTICATE_H__
#define __UIM_SLQS_UIM_AUTHENTICATE_H__

#include "qaGobiApiUim.h"

/* enum declarations */
/*
 * An enumeration of eUIM_SLQS_AUTHENTICATE_REQ request TLV IDs
 */
enum eUIM_SLQS_AUTHENTICATE_REQ
{
    eTLV_AUTH_SESS_INFO = 0x01,
    eTLV_AUTH_DATA      = 0x02,
    eTLV_AUTH_RESP_IND  = 0x10
};

/*
 * An enumeration of eUIM_SLQS_AUTHENTICATE_RESP request TLV IDs
 */
enum eUIM_SLQS_AUTHENTICATE_RESP
{
    eTLV_AUTH_CARD_RESULT   = 0x10,
    eTLV_AUTH_RESULT        = 0x11,
    eTLV_AUTH_RESP_IND_RESP = 0x12
};

/*
 * This structure contains the UimSLQSAuthenticate response parameters.
 */
struct QmiUimSLQSAuthenticateResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    UIMAuthenticateResp *pUIMAuthenticateResp;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiUimSlqsAuthenticate(
    WORD               *pMlength,
    BYTE               *pBuffer,
    UIMAuthenticateReq *pUIMAuthenticateReq);

extern enum eQCWWANError UpkQmiUimSlqsAuthenticate(
    BYTE                              *pMdmResp,
    struct QmiUimSLQSAuthenticateResp *pApiResp );

#endif /* __UIM_SLQS_UIM_AUTHENTICATE_H__ */
