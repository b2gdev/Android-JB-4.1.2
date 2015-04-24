/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceGetCallInfo.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaVoiceSLQSVoiceGetCallInfo.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_SLQS_VOICE_GET_CALL_INFO_H__
#define __VOICE_SLQS_VOICE_GET_CALL_INFO_H__

#include "qaGobiApiVoice.h"

/* enum declarations */

/*
 * An enumeration of eVOICE_SLQS_GET_CALL_INFO_REQ request TLV IDs
 */
enum eVOICE_SLQS_GET_CALL_INFO_REQ
{
    eTLV_GET_CALL_ID  = 0x01
};

/*
 * An enumeration of eVOICE_SLQS_GET_CALL_INFO_RESP response TLV IDs
 */
enum eVOICE_SLQS_GET_CALL_INFO_RESP
{
    eTLV_CALL_INFO         = 0x10,
    eTLV_REMOTE_PARTY_NUM  = 0x11,
    eTLV_SERVICE_OPTION    = 0x12,
    eTLV_VOICE_PRIVACY     = 0x13,
    eTLV_OTASP_STATUS      = 0x14,
    eTLV_REMOTE_PARTY_NAME = 0x15,
    eTLV_GET_UUS_INFO      = 0x16,
    eTLV_ALERTING_TYPE     = 0x17,
    eTLV_ALPHA_IDENTIFIER  = 0x18,
    eTLV_CONNECT_NUM_INFO  = 0x19,
    eTLV_DIAGNOSTIC_INFO   = 0x1A,
    eTLV_ALERTING_PATTERN  = 0x1B
};

/*
 * This structure contains the VoiceSLQSGetCallInfo response parameters.
 *
 * \sa qaGobiApiVoice.h for parameter descriptions
 *
 */
struct QmiVoiceSLQSGetCallInfoResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    voiceCallInfoResp *pGetCallInfoResp;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiVoiceSlqsGetCallInfo(
    WORD             *pMlength,
    BYTE             *pBuffer,
    voiceCallInfoReq *pGetCallInfoReq );

extern enum eQCWWANError UpkQmiVoiceSlqsGetCallInfo(
    BYTE                               *pMdmResp,
    struct QmiVoiceSLQSGetCallInfoResp *pApiResp );

#endif /* __VOICE_SLQS_VOICE_GET_CALL_INFO_H__ */
