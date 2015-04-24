/*
 * \ingroup sms
 *
 * \file    qaSmsSLQSSetRoutes.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaSmsSLQSSetRoutes.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __SMS_SLQS_SET_ROUTES_H__
#define __SMS_SLQS_SET_ROUTES_H__

#include "qaGobiApiSms.h"

/* enum declarations */

/*
 * An enumeration of eQMI_SMS_SLQS_SET_ROUTES_REQ request TLV IDs
 */
enum eQMI_SMS_SLQS_SET_ROUTES_REQ
{
    eTLV_ROUTE_LIST  = 0x01,
    eTLV_TRANSFER_SR = 0x10,
};

/*
 * This structure contains the SLQSGetIndicationRegister response parameters.
 */
struct QmiSmsSLQSSetRoutesResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiSmsSLQSSetRoutes(
    WORD *pMlength,
    BYTE *pBuffer,
    smsSetRoutesReq *pSetRoutesReq );

extern enum eQCWWANError UpkQmiSmsSLQSSetRoutes(
    BYTE                           *pMdmResp,
    struct QmiSmsSLQSSetRoutesResp *pApiResp );

#endif /* __SMS_SLQS_SET_ROUTES_H__ */

