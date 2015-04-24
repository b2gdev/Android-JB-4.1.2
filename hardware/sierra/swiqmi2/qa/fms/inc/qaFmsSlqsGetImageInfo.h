/*
 * \ingroup fms
 *
 * \file    qaFmsSlqsGetImageInfo.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaFmsSlqsGetImageInfo.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __FMS_SLQS_GET_IMAGE_INFO_H__
#define __FMS_SLQS_GET_IMAGE_INFO_H__

#include "qaGobiApiFms.h"

/* Prototypes */
enum eQCWWANError PkQmiGetCweSpkgsInfo(
    WORD    *pMlength,
    BYTE    *pParamField,
    LPCSTR  path );

enum eQCWWANError UpkQmiGetCweSpkgsInfo(
    BYTE                               *pMdmResp,
    struct qm_qmi_response_tlvs_values *pApiResp);

#endif /* __FMS_SLQS_GET_IMAGE_INFO_H__ */
