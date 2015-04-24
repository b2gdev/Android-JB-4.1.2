/*
 * \ingroup uim
 *
 * \file    qaUimSLQSUimGetCardStatus.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaUimSLQSUimGetCardStatus.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __UIM_SLQS_UIM_GET_CARD_STATUS_H__
#define __UIM_SLQS_UIM_GET_CARD_STATUS_H__

#include "qaGobiApiUim.h"

/* enum declarations */
/*
 * An enumeration of eUIM_SLQS_GET_CARD_STATUS_RESP request TLV IDs
 */
enum eUIM_SLQS_GET_CARD_STATUS_RESP
{
    eTLV_GET_CARD_STATUS     = 0x10,
    eTLV_GET_HOT_SWAP_STATUS = 0x11
};

/*
 * This structure contains the UimSLQSGetCardStatus response parameters.
 */
struct QmiUimSLQSGetCardStatusResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    UIMGetCardStatusResp *pUIMGetCardStatusResp;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiUimSlqsGetCardStatus(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiUimSlqsGetCardStatus(
    BYTE                               *pMdmResp,
    struct QmiUimSLQSGetCardStatusResp *pApiResp );

#endif /* __UIM_SLQS_UIM_GET_CARD_STATUS_H__ */
