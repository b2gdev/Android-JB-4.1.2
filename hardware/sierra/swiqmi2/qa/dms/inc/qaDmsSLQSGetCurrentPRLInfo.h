/*
 * \ingroup dms
 *
 * \file  qaDmsSLQSGetCurrentPRLInfo.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaDmsSLQSGetCurrentPRLInfo.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DMS_SLQS_GET_CURRENT_PRL_INFO_H__
#define __DMS_SLQS_GET_CURRENT_PRL_INFO_H__

/* enum declarations */
/*
 *
 * An enumeration of eQMI_DMS_GET_CURRENT_PRL_INFO request TLV IDs
 *
 */
enum eNAS_SLQS_INTIATE_NETWORK_REGISTER_REQ
{
    eTLV_DMS_PRL_VERSION    = 0x10,
    eTLV_DMS_PRL_PREFERENCE = 0x11
};

/*
 * This structure contains the SLQSGetCurrentPRLInfo response parameters.
 *
 * \sa qaGobiApiNas.h for parameter descriptions
 *
 */
struct QmiDMSSLQSGetCurrentPRLInfoResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
    dmsCurrentPRLInfo  *pCurrentPRLInfo;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiDmsSLQSGetCurrentPRLInfo (
    WORD *pMlength,
    BYTE *pParamField );

extern enum eQCWWANError UpkQmiDmsSLQSGetCurrentPRLInfo (
    BYTE                                   *pMdmResp,
    struct QmiDMSSLQSGetCurrentPRLInfoResp *pApiResp );

#endif /* __DMS_SLQS_GET_CURRENT_PRL_INFO_H__ */
