/*
 * \ingroup uim
 *
 * \file    qaUimSLQSUimRefreshOK.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaUimSLQSUimRefreshOK.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __UIM_SLQS_UIM_REFRESH_OK_H__
#define __UIM_SLQS_UIM_REFRESH_OK_H__

#include "qaGobiApiUim.h"

/* enum declarations */
/*
 * An enumeration of eQMI_UIM_REFRESH_OK_REQ request TLV IDs
 */
enum eQMI_UIM_REFRESH_OK_REQ
{
    eTLV_SESSION_INFO_REFRESH_OK = 0x01,
    eTLV_OK_FOR_REFRESH          = 0x02
};

/*
 * This structure contains the SLQSUIMRefreshOK response parameters.
 */
struct QmiUimSLQSRefreshOKResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiUimSlqsRefreshOK(
    WORD            *pMlength,
    BYTE            *pBuffer,
    UIMRefreshOKReq *pUIMRefreshOKReq );

extern enum eQCWWANError UpkQmiUimSlqsRefreshOK(
    BYTE                           *pMdmResp,
    struct QmiUimSLQSRefreshOKResp *pApiResp );

#endif /* __UIM_SLQS_UIM_REFRESH_OK_H__ */
