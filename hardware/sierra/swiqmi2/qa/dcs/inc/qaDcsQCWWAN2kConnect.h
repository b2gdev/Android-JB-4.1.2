/**
 * \ingroup dcs
 *
 * \file    qaDcsQCWWAN2kConnect.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaDcsQCWWAN2kConnect.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __DCS_QCWWAN2KCONNECT_H__
#define __DCS_QCWWAN2KCONNECT_H__

/* enum declarations */
/**
 * This structure contains the QmiDcsQCWWAN2kConnect request parameters.
 *
 * \sa qaGobiApiDcs.h for parameter descriptions
 *
 */
struct QmiDcsQCWWAN2kConnectReq
{
    CHAR    *pDeviceID;
    CHAR    *pDeviceKey;
};

/**
 * This structure contains the QmiDcsQCWWAN2kConnect response parameters.
 *
 */
struct QmiDcsQCWWAN2kConnectResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/* Prototypes */
extern ULONG  QCWWAN2kConnect(
    CHAR    *pDeviceID,
    CHAR    *pDeviceKey );

extern  enum eQCWWANError PkQmiDcsQCWWAN2kConnect(
    WORD    *pMlength,
    BYTE    *pParamField,
    CHAR    *pDeviceID,
    CHAR    *pDeviceKey );

extern enum eQCWWANError UpkQmiDcsQCWWAN2kConnect(
    BYTE   *pMdmResp,
    struct  QmiDcsQCWWAN2kConnectResp *pApiResp);


#endif /* __DCS_QCWWAN2KCONNECT_H__ */
