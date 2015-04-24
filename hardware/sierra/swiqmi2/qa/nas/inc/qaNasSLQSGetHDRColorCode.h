/*
 * \ingroup nas
 *
 * \file qaNasSLQSGetHDRColorCode.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaNasSLQSGetHDRColorCode.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_SLQS_GET_HDR_COLOR_CODE_MSG_H__
#define __NAS_SLQS_GET_HDR_COLOR_CODE_MSG_H__

#include "qaGobiApiNas.h"

/*
 *
 * An enumeration of eQMI_NAS_SLQS_GET_HDR_COLOR_CODE_RESP response TLV IDs
 *
 */
enum eNAS_SLQS_GET_HDR_COLOR_CODE_RESP
{
    eTLV_COLOR_CODE = 0x10
};

/*
 * This structure contains the SLQSGetHDRColorCode response parameters.
 *
 * \sa  qaGobiApiNas.h for parameter descriptions
 *
 */
struct QmiNasSlqsGetHDRColorCodeResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* Response parameters */
    nasGetHDRColorCodeResp *pGetHDRColorCodeResp;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiNasSlqsGetHDRColorCode(
    WORD *pMlength,
    BYTE *pParamField );

extern enum eQCWWANError UpkQmiNasSlqsGetHDRColorCode(
    BYTE                                 *pMdmResp,
    struct QmiNasSlqsGetHDRColorCodeResp *pApiResp );

#endif /* __NAS_SLQS_GET_HDR_COLOR_CODE_MSG_H__ */
