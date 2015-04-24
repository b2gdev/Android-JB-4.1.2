/*
 * \ingroup wds
 *
 * \file qaWdsSlqsDeleteProfile.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaWdsSLQSDeleteProfile.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __WDS_SLQS_DELETE_PROFILE_MSG_H__
#define __WDS_SLQS_DELETE_PROFILE_MSG_H__

/* enum declarations */

/*
 * An enumeration of eQMI_WDS_DELETE_PROFILE request TLV IDs
 */
enum eWDS_DELETE_PROFILE_REQ
{
    eTLV_DELETE_PROFILE_IDENTIFIER = 0x01
};

/*
 * An enumeration of eQMI_WDS_DELETE_PROFILE response TLV IDs
 */
enum eWDS_DELETE_PROFILE_RSP
{
    eTLV_DELETE_PROFILE_EXTENDED_ERROR = 0xE0
};

/*
 * This structure contains the SLQSDeleteProfile request parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsSlqsDeleteProfileReq
{
    BYTE profileType;
    BYTE profileIndex;
};

/*
 * This structure contains the SLQSDeleteProfile response parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsSlqsDeleteProfileResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* SLQSDeleteProfile response parameters */
    WORD *pExtendedErrorCode;
};

/* Prototypes */
extern enum eQCWWANError PkQmiWdsSlqsDeleteProfile(
    WORD *pMlength,
    BYTE *pBuffer,
    BYTE profileType,
    BYTE profileIndex );

extern enum eQCWWANError UpkQmiWdsSlqsDeleteProfile(
    BYTE                               *pMdmResp,
    struct QmiWdsSlqsDeleteProfileResp *pAipResp );

#endif /* __WDS_SLQS_DELETE_PROFILE_MSG_H__ */
