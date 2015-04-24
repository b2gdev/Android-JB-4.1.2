/*
 * \ingroup uim
 *
 * \file    qaUimSLQSUimReset.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaUimSLQSUimReset.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __UIM_SLQS_UIM_RESET_H__
#define __UIM_SLQS_UIM_RESET_H__

#include "qaGobiApiUim.h"

/*
 * This structure contains the UimSLQSReset response parameters.
 */
struct QmiUimSLQSResetResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiUimSlqsReset(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiUimSlqsReset(
    BYTE                       *pMdmResp,
    struct QmiUimSLQSResetResp *pApiResp );

#endif /* __UIM_SLQS_UIM_RESET_H__ */
