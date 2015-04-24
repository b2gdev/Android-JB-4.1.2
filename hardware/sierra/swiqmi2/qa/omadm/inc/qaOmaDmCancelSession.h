/*
 * \ingroup oma
 *
 * \file    qaOmaDmCancelSession.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaOmaDmCancelSession.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __OMA_DM_CANCEL_SESSION_H__
#define __OMA_DM_CANCEL_SESSION_H__

/*
 * This structure contains the OMADMCancelSession response parameters
 */
struct QmiOmaDmCancelSessionResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiOmaDmCancelSession(
    WORD *pMlength,
    BYTE *pBuffer);

extern enum eQCWWANError UpkQmiOmaDmCancelSession(
    BYTE                             *pMdmResp,
    struct QmiOmaDmCancelSessionResp *pApiResp );

#endif /* __OMA_DM_CANCEL_SESSION_H__ */
