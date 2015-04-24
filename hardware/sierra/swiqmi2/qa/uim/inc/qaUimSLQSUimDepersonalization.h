/*
 * \ingroup uim
 *
 * \file    qaUimSLQSUimDepersonalization.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaUimSLQSUimDepersonalization.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __UIM_SLQS_UIM_DEPERSONALIZATION_H__
#define __UIM_SLQS_UIM_DEPERSONALIZATION_H__

#include "qaGobiApiUim.h"

/* enum declarations */
/*
 * An enumeration of eUIM_SLQS_DEPERSONALIZATION_REQ request TLV IDs
 */
enum eUIM_SLQS_DEPERSONALIZATION_REQ
{
    eTLV_DEPERSONALIZATION = 0x01
};

/*
 * An enumeration of eUIM_SLQS_DEPERSONALIZATION_RESP request TLV IDs
 */
enum eUIM_SLQS_DEPERSONALIZATION_RESP
{
    eTLV_DP_RETRIES_REMAINING = 0x10
};

/*
 * This structure contains the UimSLQSDepersonalization response parameters.
 */
struct QmiUimSLQSDepersonalizationResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    UIMDepersonalizationResp *pUIMDepersonalizationResp;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiUimSlqsDepersonalization(
    WORD                    *pMlength,
    BYTE                    *pBuffer,
    UIMDepersonalizationReq *pUIMDepersonalizationReq );

extern enum eQCWWANError UpkQmiUimSlqsDepersonalization(
    BYTE                                   *pMdmResp,
    struct QmiUimSLQSDepersonalizationResp *pApiResp );

#endif /* __UIM_SLQS_UIM_DEPERSONALIZATION_H__ */
