/*
 * \ingroup wds
 *
 * \file qaWdsGetCurDataBearerTechnology.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaWdsGetCurDataBearerTechnology.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __WDS_GET_CUR_DATA_BEARER_MSG_H__
#define __WDS_GET_CUR_DATA_BEARER_MSG_H__

#include "qaGobiApiWds.h"

/* enum declarations */
/*
 *
 * An enumeration of eQMI_WDS_GET_CUR_DATA_BEARER response TLV IDs
 *
 */
enum eWDS_GET_CUR_DATA_BEARER_RESP
{
    eTLV_CURRENT_DATABEARER  = 0x01,
    eTLV_LASTCALL_DATABEARER = 0x10
};

/*
 * This structure contains the GetCurDataBearerTechnology response parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsGetCurDataBearerResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* GetCurDataBearerTechnology response parameters */
    QmiWDSDataBearers *pDataBearers;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiWdsGetCurDataBearerTechnology(
    WORD    *pMlength,
    BYTE    *pBuffer);

extern enum eQCWWANError UpkQmiWdsGetCurDataBearerTechnology(
    BYTE    *pMdmResp,
    struct  QmiWdsGetCurDataBearerResp *pApiResp);

#endif /* __WDS_GET_CUR_DATA_BEARER_MSG_H__ */
