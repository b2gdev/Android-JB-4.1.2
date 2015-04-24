/*
 * \ingroup swioma
 *
 * \file    qaSwiOmaDmSessionStart.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaSwiOmaDmSessionStart.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __SWIOMA_DM_SSSION_START_H__
#define __SWIOMA_DM_SSSION_START_H__

/* enum declarations */
/*
 * An enumeration of eQMI_SWIOMA_DM_SESSION_START request TLV IDs
 */
enum eSWIOMA_DM_SESSION_START_REQ
{
    eTLV_SESSION_TYPE = 0x10
};

/*
 * This structure contains the SLQSOMADMStartSession request parameters.
 */
struct QmiSwiOmaDmSessionStartReq
{
    ULONG sessionType;
};

/*
 * This structure contains the SLQSOMADMStartSession response parameters.
 */
struct QmiSwiOmaDmSessionStartResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiSwiOmaDmSessionStart(
    WORD  *pMlength,
    BYTE  *pBuffer,
    ULONG sessionType );

extern enum eQCWWANError UpkQmiSwiOmaDmSessionStart(
    BYTE                               *pMdmResp,
    struct QmiSwiOmaDmSessionStartResp *pApiResp );

#endif /* __SWIOMA_DM_SSSION_START_H__ */
