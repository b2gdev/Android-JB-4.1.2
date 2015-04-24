/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceGetCLIR.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaVoiceSLQSVoiceGetCLIR.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_SLQS_VOICE_GET_CLIR_H__
#define __VOICE_SLQS_VOICE_GET_CLIR_H__

#include "qaGobiApiVoice.h"

/* enum declarations */

/*
 * An enumeration of eVOICE_SLQS_GET_CLIR_RESP response TLV IDs
 */
enum eVOICE_SLQS_GET_CLIR_RESP
{
    eTLV_CLIR_RESPONSE      = 0x10,
    eTLV_CLIR_FAILURE_CAUSE = 0x11,
    eTLV_CLIR_ALPHA_ID      = 0x12,
    eTLV_CLIR_RESULT_TYPE   = 0x13,
    eTLV_CLIR_CALL_ID       = 0x14,
    eTLV_CLIR_CC_SUP_S_TYPE = 0x15
};

/*
 * This structure contains the VoiceSLQSGetCLIR response parameters.
 */
struct QmiVoiceSLQSGetCLIRResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    voiceGetCLIRResp *pVoiceGetCLIRResp;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiVoiceSlqsGetCLIR(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiVoiceSlqsGetCLIR(
    BYTE                           *pMdmResp,
    struct QmiVoiceSLQSGetCLIRResp *pApiResp );

#endif /* __VOICE_SLQS_VOICE_GET_CLIR_H__ */
