/*
 * \ingroup wds
 *
 * \file qaWdsGetDataBearerTechnology.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaWdsGetDataBearerTechnology.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __WDS_GET_DATA_BEARER_MSG_H__
#define __WDS_GET_DATA_BEARER_MSG_H__

/* enum declarations */
/*
 *
 * An enumeration of eQMI_WDS_GET_DATA_BEARER response TLV IDs
 *
 */
enum eWDS_GET_DATA_BEARER_RESP
{
    eTLV_DATABEARER  = 0x01
};

/*
 * This structure contains the GetDataBearerTechnology response parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsGetDataBearerResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* GetDataBearerTechnology response parameters */
    ULONG    *pCurrentDataBearer;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiWdsGetDataBearerTechnology(
    WORD    *pMlength,
    BYTE    *pBuffer);

extern enum eQCWWANError UpkQmiWdsGetDataBearerTechnology(
    BYTE    *pMdmResp,
    struct  QmiWdsGetDataBearerResp *pApiResp);

#endif /* __WDS_GET_DATA_BEARER_MSG_H__ */
