/*
 * \ingroup oma
 *
 * \file    qaOmaDmGetSessionInfo.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaOmaDmGetSessionInfo.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __OMA_DM_GET_SESSION_INFO_H__
#define __OMA_DM_GET_SESSION_INFO_H__

/* enum declarations */

/*
 * An enumeration of eQMI_OMA_DM_GET_SESSION_INFO response TLV IDs
 */
enum eQMI_OMA_DM_GET_SESSION_INFO_RESP
{
    eTLV_OMA_DM_GET_SESSION_INFO        = 0x10,
    eTLV_OMA_DM_SESSION_FAILURE_REASON  = 0x11,
    eTLV_OMA_DM_RETRY_INFO              = 0x12,
    eTLV_OMA_DM_NETWORK_INITIATED_ALERT = 0x13
};

/*
 * An enumeration of eQMI_OMA_DM_GET_SESSION_INFO response TLV Lengths
 */
enum eQMI_OMA_DM_GET_SESSION_INFO_RESP_LENGTH
{
    eTLV_OMA_DM_SESSION_INFO_LENGTH            = 0x02,
    eTLV_OMA_DM_FAILURE_REASON_LENGTH          = 0x01,
    eTLV_OMA_DM_RETRY_INFO_LENGTH              = 0x05,
    eTLV_OMA_DM_NETWORK_INITIATED_ALERT_LENGTH = 0x03
};

/*
 * This structure contains the OMADMGetSessionInfo response parameters
 */
struct QmiOmaDmGetSessionInfoResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* OMADMGetSessionInfo response parameters */
    ULONG  *pSessionState;
    ULONG  *pSessionType;
    ULONG  *pFailureReason;
    BYTE   *pRetryCount;
    WORD   *pSessionPause;
    WORD   *pTimeRemaining;
    ULONG  *pNIASessionType;
    USHORT *pNIASessionID;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiOmaDmGetSessionInfo(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiOmaDmGetSessionInfo(
    BYTE                              *pMdmResp,
    struct QmiOmaDmGetSessionInfoResp *pApiResp );

#endif /* __OMA_DM_GET_SESSION_INFO_H__ */
