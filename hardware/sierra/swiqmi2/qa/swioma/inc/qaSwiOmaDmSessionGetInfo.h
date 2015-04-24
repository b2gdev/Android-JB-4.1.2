/*
 * \ingroup swioma
 *
 * \file    qaSwiOmaDmSessionGetInfo.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaSwiOmaDmSessionGetInfo.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __SWIOMA_DM_SESSION_GET_INFO_H__
#define __SWIOMA_DM_SESSION_GET_INFO_H__

/* enum declarations */

/*
 * An enumeration of eQMI_SWIOMA_DM_SESSION_GET_INFO response TLV IDs
 */
enum eQMI_SWIOMA_DM_SESSION_GET_INFO_RESP
{
    eTLV_GET_OMA_DM_SESSION_INFO = 0x10
};

/*
 * An enumeration of eQMI_SWIOMA_DM_SESSION_GET_INFO response TLV Lengths
 */
enum eQMI_SWIOMA_DM_SESSION_GET_INFO_RESP_LENGTH
{
    eTLV_GET_OMA_DM_SESSION_INFO_LENGTH = 0x02
};

/*
 * This structure contains the SLQSOMADMGetSessionInfo response parameters
 */
struct QmiSwiOmaDmSessionGetInfoResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* SLQSOMADMGetSessionInfo response parameters */
    ULONG *pSessionType;
    ULONG *pSessionState;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiSwiOmaDmSessionGetInfo(
    WORD  *pMlength,
    BYTE  *pBuffer );

extern enum eQCWWANError UpkQmiSwiOmaDmSessionGetInfo(
    BYTE                                 *pMdmResp,
    struct QmiSwiOmaDmSessionGetInfoResp *pApiResp );

#endif /* __SWIOMA_DM_SESSION_GET_INFO_H__ */
