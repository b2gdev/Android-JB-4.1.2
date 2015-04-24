/*
 * \ingroup dms
 *
 * \file    qaDmsValSerProgramCode.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaDmsValSerProgramCode.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DMS_VALIDATE_SERVICE_PROGRAMMING_CODE_H__
#define __DMS_VALIDATE_SERVICE_PROGRAMMING_CODE_H__

/*
 *
 * An enumeration ofeQMI_DMS_VAL_SER_PROGRAM_CODE response TLV IDs
 *
 */
enum eQMI_DMS_VAL_SER_PROGRAM_CODE_REQ
{
    eTLV_SPC_VALID = 0x01
};

struct QmiDmsValSerProgramCodeReq
{
    char *pSPC;
};

/*
 * This structure contains the ValidateSPC response parameters.
 *
 * \sa qaGobiApiDms.h for parameter descriptions
 */
struct QmiDmsValSerProgramCodeResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*************
 * Prototypes
 **************/
extern enum eQCWWANError PkQmiDmsValSerProgramCode (
    WORD *pMlength,
    BYTE *pParamField,
    CHAR *pSPC );

extern enum eQCWWANError UpkQmiDmsValSerProgramCode (
    BYTE                               *pMdmResp,
    struct QmiDmsValSerProgramCodeResp *pAipResp );

#endif /* __DMS_VALIDATE_SERVICE_PROGRAMMING_CODE_H__ */
