/*
 * \ingroup uim
 *
 * \file    qaUimSLQSUimRefreshRegister.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaUimSLQSUimRefreshRegister.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __UIM_SLQS_UIM_REFRESH_REGISTER_H__
#define __UIM_SLQS_UIM_REFRESH_REGISTER_H__

#include "qaGobiApiUim.h"

/* enum declarations */
/*
 * An enumeration of eQMI_UIM_SLQS_REFRESH_REGISTER_REQ request TLV IDs
 */
enum eQMI_UIM_SLQS_REFRESH_REGISTER_REQ
{
    eTLV_SESSION_INFO     = 0x01,
    eTLV_REGISTER_REFRESH = 0x02
};

/*
 * This structure contains the SLQSUIMRefreshRegister response parameters.
 */
struct QmiUimSLQSRefreshRegisterResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiUimSlqsRefreshRegister(
    WORD                  *pMlength,
    BYTE                  *pBuffer,
    UIMRefreshRegisterReq *pUIMRefreshRegisterReq );

extern enum eQCWWANError UpkQmiUimSlqsRefreshRegister(
    BYTE                                 *pMdmResp,
    struct QmiUimSLQSRefreshRegisterResp *pApiResp );

#endif /* __UIM_SLQS_UIM_REFRESH_REGISTER_H__ */
