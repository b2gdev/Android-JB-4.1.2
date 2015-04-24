/*
 * \ingroup dms
 *
 * \file qaDmsGetPRLVersion.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaDmsGetPRLVersion.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DMS_GET_PRL_VERSION_H__
#define __DMS_GET_PRL_VERSION_H__

/*
 *
 * An enumeration of eQMI_DMS_GET_PRL_VERSION response TLV IDs
 *  
 */
enum DMS_GET_PRL_VERSION_RESP
{
    eTLV_PRL_VERSION  = 0x01
};

/*
 *
 * This structure contains the GetPRLVersion response parameters
 *
 * \sa qaGobiApiDms.h for parameter descriptions
 *
 */
struct QmiDmsGetPRLVersionResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* GetPRLVersion response parameters */
    WORD * pPRLVersion;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiDmsGetPRLVersion(
    WORD    *pMlength,
    BYTE    *pBuffer );

extern enum eQCWWANError UpkQmiDmsGetPRLVersion(
    BYTE     *mdmresp,
    struct   QmiDmsGetPRLVersionResp *apiresp);

#endif /* __DMS_GET_PRL_VERSION_ID_H__ */
