/*
 * \ingroup nas
 *
 * \file    qaNasInitiateAttach.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaNasInitiateAttach.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_INITIATE_ATTACH_MSG_H__
#define __NAS_INITIATE_ATTACH_MSG_H__


/* enum declarations */
/*
 *
 * An enumeration of eQMI_NAS_INITIATE_ATTACH request TLV IDs
 *
 */
enum eNAS_INITIATE_ATTACH_REQ
{
    eTLV_PS_ATTACH_ACTION = 0x10
};

/*
 * This structure contains the InitiateAttach request parameters.
 *
 * \sa qaGobiApiNas.h for parameter descriptions
 *
 */
struct QmiNasInitiateAttachReq
{
    BYTE action;
};

/*
 * This structure contains the InitiateAttach response parameters.
 *
 */
struct QmiNasInitiateAttachResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/************
 * Prototypes
 **************/
extern enum eQCWWANError PkQmiNasInitiateAttach(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG action );

extern enum eQCWWANError UpkQmiNasInitiateAttach(
    BYTE                            *pMdmResp,
    struct QmiNasInitiateAttachResp *pApiResp);

#endif /* __NAS_INITIATE_ATTACH_MSG_H__ */
