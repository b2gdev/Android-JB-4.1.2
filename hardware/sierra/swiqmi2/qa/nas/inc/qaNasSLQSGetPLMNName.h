/*
 * \ingroup nas
 *
 * \file  qaNasSLQSGetPLMNName.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaNasSLQSGetPLMNName.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_GET_PLMN_NAME_H__
#define __NAS_GET_PLMN_NAME_H__

#include "qaGobiApiNas.h"

/* enum declarations */
/*
 * An enumeration of eQMI_NAS_GET_PLMN_NAME response TLV IDs
 */
enum eNAS_GET_PLMN_NAME_REQ
{
    eTLV_PLMN = 0x01,
};

/*
 * An enumeration of eQMI_NAS_GET_PLMN_NAME response TLV IDs
 */
enum eNAS_GET_PLMN_NAME_RESP
{
    eTLV_3GPP_EONS_PLMN_NAME = 0x10,
};


/*
 * This structure contains the SLQSGetPLMNName response parameters.
 *
 * \sa  qaGobiApiNas.h for parameter descriptions
 *
 */
struct QmiNasGetPLMNNameResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* Response parameters */
    nasPLMNNameResp  *pPLMNNameResp;
};

/*
 * Prototypes
 */
enum eQCWWANError PkQmiNasGetPLMNName(
    WORD            *pMlength,
    BYTE            *pBuffer,
    nasPLMNNameReq  *pPLMNNameReq);

enum eQCWWANError UpkQmiNasGetPLMNName(
    BYTE                         *pMdmresp,
    struct QmiNasGetPLMNNameResp *pApiresp);

#endif /* __NAS_GET_PLMN_NAME_H__ */
