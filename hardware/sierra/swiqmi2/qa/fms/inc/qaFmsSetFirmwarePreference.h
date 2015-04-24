/*
 * \ingroup fms
 *
 * \file    qaFmsSetFirmwarePreference.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaFmsSetFirmwarePreference.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __FMS_SETFIRWMAREPREFERENCE_H__
#define __FMS_SETFIRWMAREPREFERENCE_H__

/* Prototypes */
ULONG SetFirmwarePreference();

enum eQCWWANError PkQmiFmsSetFirmwarePreference(
    WORD    *pMlength,
    BYTE    *pParamField );

enum eQCWWANError UpkQmiFmsSetFirmwarePreference(
    BYTE   *pMdmResp,
    struct qm_qmi_response_tlvs_values *pApiResp );

#endif /* __FMS_SETFIRWMAREPREFERENCE_H__ */
