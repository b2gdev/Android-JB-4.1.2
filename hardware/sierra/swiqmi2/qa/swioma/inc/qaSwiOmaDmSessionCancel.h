/*
 * \ingroup swioma
 *
 * \file    qaSwiOmaDmSessionCancel.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaSwiOmaDmSessionCancel.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __SWIOMA_DM_SESSION_CANCEL_H__
#define __SWIOMA_DM_SESSION_CANCEL_H__

/* enum declarations */
/*
 * An enumeration of eQMI_SWIOMA_DM_SESSION_CANCEL request TLV IDs
 */
enum eSWIOMA_DM_SESSION_CANCEL_REQ
{
    eTLV_SESSION = 0x10
};

/*
 * This structure contains the SLQSOMADMCancelSession request parameters.
 */
struct QmiSwiOmaDmSessionCancelReq
{
    ULONG session;
};

/*
 * This structure contains the SLQSOMADMCancelSession response parameters
 */
struct QmiSwiOmaDmSessionCancelResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiSwiOmaDmSessionCancel(
    WORD *pMlength,
    BYTE *pBuffer,
    ULONG session );

extern enum eQCWWANError UpkQmiSwiOmaDmSessionCancel(
    BYTE                                *pMdmResp,
    struct QmiSwiOmaDmSessionCancelResp *pApiResp );

#endif /* __SWIOMA_DM_SESSION_CANCEL_H__ */
