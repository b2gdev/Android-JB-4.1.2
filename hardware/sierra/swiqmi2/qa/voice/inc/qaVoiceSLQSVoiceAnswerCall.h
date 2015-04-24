/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSAnswerCall.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaVoiceSLQSAnswerCall.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_SLQS_ANSWER_CALL_H__
#define __VOICE_SLQS_ANSWER_CALL_H__

#include"qaGobiApiVoice.h"

/* enum declarations */

/*
 * An enumeration of eVOICE_SLQS_ANSWER_CALL_REQ request TLV IDs
 */
enum eVOICE_SLQS_ANSWER_CALL_REQ
{
    eTLV_ANSWER_CALL_ID_REQ = 0x01
};

/*
 * An enumeration of eVOICE_SLQS_ANSWER_CALL_RESP response TLV IDs
 */
enum eVOICE_SLQS_ANSWER_CALL_RESP
{
    eTLV_ANSWER_CALL_ID_RESP = 0x10
};

/*
 * This structure contains the QmiVoiceSLQSAnswerCallReq request parameters.
 *
 * \sa qaGobiApiVoice.h for parameter descriptions
 *
 */
struct QmiVoiceSLQSAnswerCallReq
{
    /* Unique call request identifier */
    voiceAnswerCall *pVoiceAnswerCall;
};

/*
 * This structure contains the VoiceSLQSAnswerCall response parameters.
 */
struct QmiVoiceSLQSAnswerCallResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* Unique call response identifier */
    BYTE *pCallId;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiVoiceSlqsAnswerCall(
    WORD            *pMlength,
    BYTE            *pBuffer,
    voiceAnswerCall *pVoiceAnswerCall);

extern enum eQCWWANError UpkQmiVoiceSlqsAnswerCall(
    BYTE                              *pMdmResp,
    struct QmiVoiceSLQSAnswerCallResp *pApiResp );

#endif /* __VOICE_SLQS_ANSWER_CALL_H__ */
