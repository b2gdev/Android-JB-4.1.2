/*
 * \ingroup wds
 *
 * \file qaWdsGetLastMipStatus.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaWdsGetLastMipStatus.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __WDS_GET_LAST_MIP_STATUS_MSG_H__
#define __WDS_GET_LAST_MIP_STATUS_MSG_H__

/* enum declarations */

/*
 * An enumeration of eQMI_WDS_GET_LAST_MIP_STATUS request TLV IDs
 */
enum eWDS_GET_LAST_MIP_STATUS_RESP
{
	eTLV_MIP_ERROR = 0x01
};

/*
 * This structure contains the GetLastMobileIPError response parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsGetLastMipStatusResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
    ULONG *pError;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiWdsGetLastMipStatus(
    WORD *pMlength,
    BYTE *pParamField );

extern enum eQCWWANError UpkQmiWdsGetLastMipStatus(
    BYTE   *pMdmResp,
    struct QmiWdsGetLastMipStatusResp *pApiResp );

#endif /* _WDS_GET_LAST_MIP_STATUS_MSG_H__ */
