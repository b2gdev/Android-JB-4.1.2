/*
 * \ingroup fms
 *
 * \file    qaFmsGetImageInfo.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaFmsGetImageInfo.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __FMS__GET_IMAGE_INFO_H__
#define __FMS__GET_IMAGE_INFO_H__

/* Prototypes */
enum eQCWWANError PkQmiGetMbnInfo(
    WORD    *pMlength,
    BYTE    *pParamField,
    LPCSTR  path,
    BYTE    imgtype );

enum eQCWWANError UpkQmiGetMbnInfo(
    BYTE                               *pMdmResp,
    struct qm_qmi_response_tlvs_values *pApiResp);

#endif /* __FMS_GET_IMAGE_INFO_H__ */
