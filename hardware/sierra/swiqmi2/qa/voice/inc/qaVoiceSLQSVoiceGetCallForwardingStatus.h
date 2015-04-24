/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceGetCallForwardingStatus.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaVoiceSLQSVoiceGetCallForwardingStatus.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_SLQS_VOICE_GET_CALL_FORWARDING_STATUS_H__
#define __VOICE_SLQS_VOICE_GET_CALL_FORWARDING_STATUS_H__

#include "qaGobiApiVoice.h"

/* enum declarations */
/*
 * An enumeration of eVOICE_SLQS_GET_CALL_FORWARDING_STATUS_REQ response TLV IDs
 */
enum eVOICE_SLQS_GET_CALL_FORWARDING_STATUS_REQ
{
    eTLV_CFWS_REASON        = 0x01,
    eTLV_CFWS_SERVICE_CLASS = 0x10
};

/*
 * An enumeration of eVOICE_SLQS_GET_CALL_FORWARDING_STATUS_RESP response TLV IDs
 */
enum eVOICE_SLQS_GET_CALL_FORWARDING_STATUS_RESP
{
    eTLV_CFWS_INFO          = 0x10,
    eTLV_CFWS_FAILURE_CAUSE = 0x11,
    eTLV_CFWS_ALPHA_ID      = 0x12,
    eTLV_CFWS_RESULT_TYPE   = 0x13,
    eTLV_CFWS_CALL_ID       = 0x14,
    eTLV_CFWS_CC_SUP_S_TYPE = 0x15,
    eTLV_CFWS_EXT_INFO      = 0x16
};

/*
 * This structure contains the VoiceSLQSGetCallFWResp response parameters.
 */
struct QmiVoiceSLQSGetCallFWResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    voiceGetCallFWResp *pVoiceGetCallFWResp;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiVoiceSlqsGetCallFWStatus(
    WORD              *pMlength,
    BYTE              *pBuffer,
    voiceGetCallFWReq *pVoiceGetCallFWReq );

extern enum eQCWWANError UpkQmiVoiceSlqsGetCallFWStatus(
    BYTE                             *pMdmResp,
    struct QmiVoiceSLQSGetCallFWResp *pApiResp );

#endif /* __VOICE_SLQS_VOICE_GET_CALL_FORWARDING_STATUS_H__ */

