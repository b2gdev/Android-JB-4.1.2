/*
 * \ingroup wds
 *
 * \file qaWdsGetDormancyState.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaWdsGetDormancyState.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __WDS_GET_DORMANCY_MSG_H__
#define __WDS_GET_DORMANCY_MSG_H__

/* enum declarations */
/*
 *
 * An enumeration of eQMI_WDS_GET_DORMANCY response TLV IDs
 *
 */
enum eWDS_GET_DORMANCY_RESP
{
    eTLV_DORMANCY_STATE	 = 0x01
};

/*
 * This structure contains the GetDormancyState response parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsGetDormancyResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* GetDormancyState response parameters */
    ULONG    *pDormancyState;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiWdsGetDormancyState(
    WORD    *pMlength,
    BYTE    *pBuffer);

extern enum eQCWWANError UpkQmiWdsGetDormancyState(
    BYTE     *pMdmResp,
    struct   QmiWdsGetDormancyResp *pAipResp);

#endif /* __WDS_GET_DORMANCY_MSG_H__ */
