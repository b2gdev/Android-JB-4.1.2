/**
 * \ingroup dcs
 *
 * \file    qaDcsSLQSGetUsbPortNames.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaDcsSLQSGetUsbPortNames.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __DCS_SLQSGETUSBPORTNAMES_H__
#define __DCS_SLQSGETUSBPORTNAMES_H__

/* enum declarations */

/* Prototypes */
enum eQCWWANError PkQmiDcsSLQSGetUsbPortNames(
    WORD      *pMlength,
    BYTE      *pParamField );

extern enum eQCWWANError UpkQmiDcsSLQSGetUsbPortNames(
    BYTE *pMdmResp,
    struct qm_qmi_response_tlvs_values *pApiResp);

extern ULONG  SLQSGetUsbPortNames(
    struct DcsUsbPortNames *pDcsSLQSGetUsbPortNames );

#endif /* __DCS_SLQSGETUSBPORTNAMES_H__ */
