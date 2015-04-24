/*
 * \ingroup uim
 *
 * \file    qaUimSLQSUimRefreshGetLastEvent.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaUimSLQSUimRefreshGetLastEvent.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __UIM_SLQS_UIM_REFRESH_GET_LAST_EVENT_H__
#define __UIM_SLQS_UIM_REFRESH_GET_LAST_EVENT_H__

#include "qaGobiApiUim.h"

/* enum declarations */
/*
 * An enumeration of eQMI_REFRESH_GET_LAST_EVENT_REQ request TLV IDs
 */
enum eQMI_UIM_REFRESH_GET_LAST_EVENT_REQ
{
    eTLV_SESSION_REF_GET_LAST_EVENT = 0x01
};

/*
 * An enumeration of eQMI_PDS_SET_COM_PORT_CONFIG request TLV IDs
 */
enum eQMI_UIM_REFRESH_EVENT_RESP
{
    eTLV_REFRESH_EVENT = 0x10
};

/*
 * This structure contains the UimSLQSRefreshGetLastEvent response parameters.
 */
struct QmiUimSLQSRefreshGetLastEventResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult         results;
    UIMRefreshGetLastEventResp *pUIMRefreshGetLastEventResp;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiUimSlqsRefreshGetLastEvent(
    WORD            *pMlength,
    BYTE            *pBuffer,
    UIMRefreshGetLastEventReq *pUIMRefreshGetLastEventReq );

extern enum eQCWWANError UpkQmiUimSlqsRefreshGetLastEvent(
    BYTE                           *pMdmResp,
    struct QmiUimSLQSRefreshGetLastEventResp *pApiResp );

#endif /* __UIM_SLQS_UIM_REFRESH_GET_LAST_EVENT_H__ */
