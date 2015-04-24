/**
 * \ingroup dms
 *
 * \file qaDmsRestoreFactoryDefaults.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaDmsRestoreFactoryDefaults.c
 *
 * Copyright: © 2010 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DMS_RESTORE_FACTORY_DEFAULTS_H__
#define __DMS_RESTORE_FACTORY_DEFAULTS_H__

/**
 *
 * An enumeration of eQMI_DMS_FACTORY_DEFAULTS response TLV IDs
 *
 */
enum eQMI_DMS_FACTORY_DEFAULTS_REQ
{
    eTLV_SPC = 0x01
};

struct QmiDmsRestoreFactoryDefaultsReq
{
	char *pSPC;
};

/**
 * This structure contains the ValidateSPC response parameters.
 *
 * \sa qaGobiApiDms.h for parameter descriptions
 */
struct QmiDmsRestoreFactoryDefaultsResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*************
 * Prototypes
 **************/
extern enum eQCWWANError PkQmiDmsRestoreFactoryDefaults (
    WORD *pMlength,
    BYTE *pParamField,
    CHAR *pSPC );

extern enum eQCWWANError UpkQmiDmsRestoreFactoryDefaults (
    BYTE                                    *pMdmResp,
    struct QmiDmsRestoreFactoryDefaultsResp *pAipResp );

#endif /* __DMS_RESTORE_FACTORY_DEFAULTS_H__ */
