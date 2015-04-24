/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceGetAllCallInfo.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaVoiceSLQSVoiceGetAllCallInfo.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_SLQS_VOICE_GET_ALL_CALL_INFO_H__
#define __VOICE_SLQS_VOICE_GET_ALL_CALL_INFO_H__

#include "qaGobiApiVoice.h"

/* enum declarations */
/*
 * An enumeration of eVOICE_SLQS_GET_ALL_CALL_INFO_RESP response TLV IDs
 */
enum eVOICE_SLQS_GET_ALL_CALL_INFO_RESP
{
    eTLV_ARRAY_CALL_INFO         = 0x10,
    eTLV_ARRAY_REMOTE_PARTY_NUM  = 0x11,
    eTLV_ARRAY_REMOTE_PARTY_NAME = 0x12,
    eTLV_ARRAY_ALERTING_TYPE     = 0x13,
    eTLV_ARRAY_UUS_INFO          = 0x14,
    eTLV_ARRAY_SERVICE_OPTION    = 0x15,
    eTLV_ALL_OTASP_STATUS        = 0x16,
    eTLV_ALL_VOICE_PRIVACY       = 0x17,
    eTLV_ARRAY_CALL_END_REASON   = 0x18,
    eTLV_ARRAY_ALPHA_IDENTIFIER  = 0x19,
    eTLV_ARRAY_CON_PARTY_NUM     = 0x1A,
    eTLV_ARRAY_DIAGNOSTIC_INFO   = 0x1B,
    eTLV_ARRAY_CALLED_PARTY_NUM  = 0x1C,
    eTLV_ARRAY_REDIR_PARTY_NUM   = 0x1D,
    eTLV_ARRAY_ALERTING_PATTERN  = 0x1E
};

/*
 * This structure contains the QmiVoiceSLQSGetAllCallInfoResp response parameters.
 */
struct QmiVoiceSLQSGetAllCallInfoResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    voiceGetAllCallInfo *pGetAllCallInfoResp;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiVoiceSlqsGetAllCallInfo(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiVoiceSlqsGetAllCallInfo(
    BYTE                                  *pMdmResp,
    struct QmiVoiceSLQSGetAllCallInfoResp *pApiResp );

#endif /* __VOICE_SLQS_VOICE_GET_ALL_CALL_INFO_H__ */
