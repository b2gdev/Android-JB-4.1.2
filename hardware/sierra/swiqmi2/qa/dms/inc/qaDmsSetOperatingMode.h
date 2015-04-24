/*
 * \ingroup dms
 *
 * \file qaDmsSetOperatingMode.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaDmsSetOperatingMode.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DMS_SET_OPERATING_MODE_MSG_H__
#define __DMS_SET_OPERATING_MODE_MSG_H__

/* enum declarations */
/*
 * An enumeration of eQMI_DMS_SET_OPERATING_MODE request TLV IDs
 *
 */
enum eDMS_SET_OPERATING_MODE_REQ
{
    eTLV_OPERATING_MODE = 0x01
};

/*
 * This structure contains the SetPower request parameters.
 *
 * \sa qaGobiApiDms.h for parameter descriptions
 *
 */
struct QmiDmsSetOperatingModeReq
{
    ULONG operatingMode;
};

/*
 * This structure contains the SetPower response parameters.
 *
 */
struct QmiDmsSetOperatingModeResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 *
 */
extern enum eQCWWANError PkQmiDmsSetOperatingMode(
    WORD  *pMlength,
    BYTE  *pBuffer,
    ULONG operatingMode );

extern enum eQCWWANError UpkQmiDmsSetOperatingMode(
    BYTE                              *pMdmResp,
    struct QmiDmsSetOperatingModeResp *pApiResp );

#endif /* __DMS_SET_OPERATING_MODE_MSG_H__ */
