/*
 * \ingroup oma
 *
 * \file    qaOmaDmSessionStart.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaOmaDmSessionStart.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __OMA_DM_START_SESSION_H__
#define __OMA_DM_START_SESSION_H__

/* enum declarations */

/*
 * An enumeration of eQMI_OMA_DM_START_SESSION request TLV IDs
 */
enum eQMI_OMA_DM_START_SESSION
{
    eTLV_SESSION_TYPE = 0x10
};

/*
 * This structure contains the OMADMStartSession request parameters.
 */
struct QmiOmaDmStartSessionReq
{
    ULONG sessionType;
};

/*
 * This structure contains the OMADMStartSession response parameters.
 */
struct QmiOmaDmStartSessionResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiOmaDmStartSession(
    WORD  *pMlength,
    BYTE  *pBuffer,
    ULONG sessionType );

extern enum eQCWWANError UpkQmiOmaDmStartSession(
    BYTE                            *pMdmResp,
    struct QmiOmaDmStartSessionResp *pApiResp );

#endif /* __OMA_DM_START_SESSION_H___ */
