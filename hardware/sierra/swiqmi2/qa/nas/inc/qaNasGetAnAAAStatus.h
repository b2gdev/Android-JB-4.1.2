/*
 * \ingroup nas
 *
 * \file qaNasGetAnAAAStatus.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaNasGetAnAAAStatus.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_GET_AN_AAA_STATUS_MSG_H__
#define __NAS_GET_AN_AAA_STATUS_MSG_H__

/* enum declarations */
/*
 *
 * An enumeration of QMI_NAS_GET_AN_AAA_STATUS response TLV IDs
 *
 */
enum eNAS_INITIATE_ATTACH_RESP
{
    eTLV_AN_AAA_STATUS = 0x01
};

/*
 * This structure contains the GetANAAAAuthenticationStatus response parameters.
 *
 * \sa  qaGobiApiNas.h for parameter descriptions
 *
 */
struct QmiNasGetAnAAAStatusResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* Response parameters */
    ULONG *pStatus;
};

/************
 * Prototypes
 **************/
extern enum eQCWWANError PkQmiNasGetAnAAAStatus(
    WORD *pMlength,
    BYTE *pParamField );

extern enum eQCWWANError UpkQmiNasGetAnAAAStatus(
    BYTE                            *pMdmResp,
    struct QmiNasGetAnAAAStatusResp *pApiResp);

#endif /* __NAS_GET_AN_AAA_STATUS_MSG_H__ */
