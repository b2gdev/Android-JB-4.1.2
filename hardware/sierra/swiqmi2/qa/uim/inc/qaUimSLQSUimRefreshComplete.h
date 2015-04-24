/*
 * \ingroup uim
 *
 * \file    qaUimSLQSUimRefreshComplete.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaUimSLQSUimRefreshComplete.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __UIM_SLQS_UIM_REFRESH_COMPLETE_H__
#define __UIM_SLQS_UIM_REFRESH_COMPLETE_H__

#include "qaGobiApiUim.h"

/* enum declarations */
/*
 * An enumeration of eQMI_REFRESH_COMP_REQ request TLV IDs
 */
enum eQMI_UIM_REFRESH_COMP_REQ
{
    eTLV_SESSION_INFO_REFRESH_COMP = 0x01,
    eTLV_REFRESH_COMPLETE          = 0x02
};

/*
 * This structure contains the SLQSUIMRefreshComplete response parameters.
 */
struct QmiUimSLQSRefreshCompleteResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiUimSlqsRefreshComplete(
    WORD                  *pMlength,
    BYTE                  *pBuffer,
    UIMRefreshCompleteReq *pUIMRefreshCompleteReq );

extern enum eQCWWANError UpkQmiUimSlqsRefreshComplete(
    BYTE                                 *pMdmResp,
    struct QmiUimSLQSRefreshCompleteResp *pApiResp );

#endif /* __UIM_SLQS_UIM_REFRESH_COMPLETE_H__ */
