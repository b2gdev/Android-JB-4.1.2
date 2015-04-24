/*
 * \ingroup wds
 *
 * \file qaWdsGetPktSrvcStatus.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaWdsGetPktSrvcStatus.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __WDS_GET_PKT_STATUS_MSG_H__
#define __WDS_GET_PKT_STATUS_MSG_H__

/* enum declarations */
/*
 *
 * An enumeration of eQMI_WDS_GET_PKT_STATUS response TLV IDs
 *
 */
enum eWDS_GET_PKT_STATUS_RESP
{
    eTLV_CONNECTION_STATUS  = 0x01
};

/*
 * This structure contains the GetPktSrvcStatus response parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsGetPktSrvcStatusResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* GetSessionState response parameters */
    ULONG    *pConnectionStatus;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiWdsGetPktSrvcStatus(
    WORD    *pMlength,
    BYTE    *pBuffer);

extern enum eQCWWANError UpkQmiWdsGetPktSrvcStatus(
    BYTE    *pMdmResp,
    struct  QmiWdsGetPktSrvcStatusResp *pAipResp);

#endif /* __WDS_GET_PKT_STATUS_MSG_H__ */
