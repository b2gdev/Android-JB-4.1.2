/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceGetCallWaiting.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaVoiceSLQSVoiceGetCallWaiting.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_SLQS_VOICE_GET_CALL_WAITING_H__
#define __VOICE_SLQS_VOICE_GET_CALL_WAITING_H__

#include "qaGobiApiVoice.h"

/* enum declarations */
/*
 * An enumeration of eVOICE_SLQS_GET_CALL_WAITING_REQ response TLV IDs
 */
enum eVOICE_SLQS_GET_CALL_WAITING_REQ
{
    eTLV_CWAIT_S_CLASS_REQ = 0x10
};

/*
 * An enumeration of eVOICE_SLQS_GET_CALL_WAITING_RESP response TLV IDs
 */
enum eVOICE_SLQS_GET_CALL_WAITING_RESP
{
    eTLV_CWAIT_S_CLASS_RESP  = 0x10,
    eTLV_CWAIT_FAILURE_CAUSE = 0x11,
    eTLV_CWAIT_ALPHA_ID      = 0x12,
    eTLV_CWAIT_RESULT_TYPE   = 0x13,
    eTLV_CWAIT_CALL_ID       = 0x14,
    eTLV_CWAIT_CC_SUP_S_TYPE = 0x15
};

/*
 * This structure contains the VoiceSLQSGetCallWaitingReq response parameters.
 */
struct QmiVoiceSLQSGetCallWaitingReq
{
    BYTE *pSvcClass;
};

/*
 * This structure contains the VoiceSLQSGetCallWaitingResp response parameters.
 */
struct QmiVoiceSLQSGetCallWaitingResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    voiceGetCallWaitInfo *pVoiceGetCallWaitInfo;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiVoiceSlqsGetCallWaiting(
    WORD *pMlength,
    BYTE *pBuffer,
    BYTE *pSvcClass);

extern enum eQCWWANError UpkQmiVoiceSlqsGetCallWaiting(
    BYTE                                  *pMdmResp,
    struct QmiVoiceSLQSGetCallWaitingResp *pApiResp );

#endif /* __VOICE_SLQS_VOICE_GET_CALL_WAITING_H__ */

