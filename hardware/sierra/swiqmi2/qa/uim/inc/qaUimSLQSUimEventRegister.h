/*
 * \ingroup uim
 *
 * \file    qaUimSLQSUimEventRegister.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaUimSLQSUimEventRegister.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __UIM_SLQS_UIM_EVENT_REG_H__
#define __UIM_SLQS_UIM_EVENT_REG_H__

#include "qaGobiApiUim.h"

/* enum declarations */
/*
 * An enumeration of eQMI_UIM_SLQS_EVENT_REG_REQ request TLV IDs
 */
enum eQMI_UIM_SLQS_EVENT_REG_REQ
{
    eTLV_EVENT_MASK = 0x01
};

/*
 * An enumeration of eQMI_UIM_SLQS_EVENT_REG_RESP request TLV IDs
 */
enum eQMI_UIM_SLQS_EVENT_REG_RESP
{
    eTLV_EVENT_MASK_RESP = 0x10
};

/*
 * This structure contains the UimSLQSEventRegister response parameters.
 */
struct QmiUimSLQSEventRegisterResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
    ULONG              eventMask;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiUimSlqsEventRegister(
    WORD                    *pMlength,
    BYTE                    *pBuffer,
    UIMEventRegisterReqResp *pUIMEventRegisterReqResp );

extern enum eQCWWANError UpkQmiUimSlqsEventRegister(
    BYTE                               *pMdmResp,
    struct QmiUimSLQSEventRegisterResp *pApiResp );

#endif /* __UIM_SLQS_UIM_EVENT_REG_H__ */
