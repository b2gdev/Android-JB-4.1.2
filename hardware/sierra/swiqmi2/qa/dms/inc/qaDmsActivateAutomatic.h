/*
 * \ingroup dms
 *
 * \file qaDmsActivateAutomatic.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaDmsActivateAutomatic.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DMS_ACTIVATE_AUTOMATIC_H__
#define __DMS_ACTIVATE_AUTOMATIC_H__

/*
 * This enumeration contains the TLV IDs for QMI_DMS_ACTIVATE_AUTOMATIC
 * requests
 *
 */
enum DMS_ACTIVATE_AUTOMATIC_REQ
{
    eTLV_ACTIVATION_CODE = 0x01
};

/*
 * This structure contains the ActivateAutomatic request parameters
 *
 * \sa See SWIWWANCMAPI.h for parameter descriptions
 *
 */
struct QmiDmsActivateAutomaticReq
{
    CHAR *pActivationCode;
};

/*
 * This structure contains the Activate Automatic
 * response parameters
 *
 * \sa See SWIWWANCMAPI.h for parameter descriptions
 *
 */
struct QmiDmsActivateAutomaticResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 *
 */
extern enum eQCWWANError PkQmiDmsActivateAutomatic(
    WORD *pMlength,
    BYTE *pBuffer,
    CHAR *pActivationCode );

extern enum eQCWWANError UpkQmiDmsActivateAutomatic(
    BYTE                               *pMdmResp,
    struct QmiDmsActivateAutomaticResp *pApiResp);

#endif /* __DMS_ACTIVATE_AUTOMATIC_H__ */
