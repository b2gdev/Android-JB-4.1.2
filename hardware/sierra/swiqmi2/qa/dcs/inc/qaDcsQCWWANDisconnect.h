/**
 * \ingroup dcs
 *
 * \file    qaDcsQCWWANDisConnect.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaDcsQCWWANDisConnect.c
 *
 * Copyright: © 2010 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __DCS_QCWWANDISCONNECT_H__
#define __DCS_QCWWANDISCONNECT_H__

/* enum declarations */
/**
 *
 * This structure contains the QmiDcsQCWWANDisconnect response parameters
 *
 */
struct QmiDcsQCWWANDisconnectResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/* Prototypes */
extern ULONG  QCWWANDisconnect();

extern enum eQCWWANError PkQmiDcsQCWWANDisconnect(
    WORD    *pMlength,
    BYTE    *pParamField );

extern enum eQCWWANError UpkQmiDcsQCWWANDisconnect(
    BYTE   *pMdmResp,
    struct  QmiDcsQCWWANDisconnectResp *pApiResp);

#endif /* __DCS_QCWWANDISCONNECT_H__ */
