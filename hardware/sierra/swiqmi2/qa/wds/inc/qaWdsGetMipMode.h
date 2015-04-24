/**
 * \ingroup wds
 *
 * \file qaWdsGetMipMode.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaWdsGetMipMode.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __WDS_GET_MIP_MODE_MSG_H__
#define __WDS_GET_MIP_MODE_MSG_H__

/* enum declarations */
/**
 *
 * An enumeration of eQMI_WDS_GET_MIP_MODE response TLV IDs
 *
 */
enum WDS_GET_MIP_MODE_RESP
{
    eTLV_GET_MIP_MODE  = 0x01
};

/**
 * This structure contains the GetMobileIP response parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsGetMipModeResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* GetMobileIP response parameters */
    ULONG    *pMipMode;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiWdsGetMipMode(
    WORD    *pMlength,
    BYTE    *pBuffer);

extern enum eQCWWANError UpkQmiWdsGetMipMode(
    BYTE                        *pMdmResp,
    struct QmiWdsGetMipModeResp *pAipResp);

#endif /* __WDS_GET_MIP_MODE_MSG_H__ */
