/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceGetCLIP.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaVoiceSLQSVoiceGetCLIP.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_SLQS_VOICE_GET_CLIP_H__
#define __VOICE_SLQS_VOICE_GET_CLIP_H__

#include "qaGobiApiVoice.h"

/* enum declarations */

/*
 * An enumeration of eVOICE_SLQS_GET_CLIP_RESP response TLV IDs
 */
enum eVOICE_SLQS_GET_CLIP_RESP
{
    eTLV_CLIP_RESPONSE      = 0x10,
    eTLV_CLIP_FAILURE_CAUSE = 0x11,
    eTLV_CLIP_ALPHA_ID      = 0x12,
    eTLV_CLIP_RESULT_TYPE   = 0x13,
    eTLV_CLIP_CALL_ID       = 0x14,
    eTLV_CLIP_CC_SUP_S_TYPE = 0x15
};

/*
 * This structure contains the VoiceSLQSGetCLIP response parameters.
 */
struct QmiVoiceSLQSGetCLIPResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    voiceGetCLIPResp *pVoiceGetCLIPResp;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiVoiceSlqsGetCLIP(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiVoiceSlqsGetCLIP(
    BYTE                           *pMdmResp,
    struct QmiVoiceSLQSGetCLIPResp *pApiResp );

#endif /* __VOICE_SLQS_VOICE_GET_CLIP_H__ */

