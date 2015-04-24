/**
 * \ingroup dcs
 *
 * \file    qaDcsSLQSGetDeviceMode.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaDcsSLQSGetDeviceMode.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __DCS_SLQSGETDEVICEMODE_H__
#define __DCS_SLQSGETDEVICEMODE_H__

/* enum declarations */

/* Prototypes */
extern enum eQCWWANError PkQmiDcsSLQSGetDeviceMode(
    WORD *pMlength,
    BYTE *pParamField );

extern enum eQCWWANError UpkQmiDcsSLQSGetDeviceMode(
    BYTE   *pMdmResp,
    struct qm_qmi_response_tlvs_values *pApiResp);

#endif /* __DCS_SLQSGETDEVICEMODE_H__ */
