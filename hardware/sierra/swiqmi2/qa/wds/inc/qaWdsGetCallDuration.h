/*
 * \ingroup wds
 *
 * \file qaWdsGetCallDuration.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaWdsGetCallDuration.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __WDS_GET_DURATION_MSG_H__
#define __WDS_GET_DURATION_MSG_H__

/* enum declarations */
/*
 *
 * An enumeration of eQMI_WDS_GET_DURATION response TLV IDs
 *
 */
enum eWDS_GET_DURATION_RESP
{
    eTLV_CALL_DURATION    = 0x01
};

/*
 * This structure contains the GetSessionDuration response parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsGetDurationResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* GetSessionDuration response parameters */
    ULONGLONG    *pCallDuration;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiWdsGetCallDuration(
    WORD    *pMlength,
    BYTE    *pBuffer);

extern enum eQCWWANError UpkQmiWdsGetCallDuration(
    BYTE     *pMdmResp,
    struct   QmiWdsGetDurationResp *pAipResp);

#endif /* __WDS_GET_DURATION_MSG_H__ */
