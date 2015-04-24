/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceStopContDTMF.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaVoiceSLQSVoiceStopContDTMF.c
 *
 * Copyright: © 2011-12 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_SLQS_VOICE_STOP_CONT_DTMF_H__
#define __VOICE_SLQS_VOICE_STOP_CONT_DTMF_H__

#include"qaGobiApiVoice.h"

/* enum declarations */

/*
 * An enumeration of eVOICE_SLQS_STOP_CONT_DTMF_REQ request TLV IDs
 */
enum eVOICE_SLQS_STOP_CONT_DTMF_REQ
{
    eTLV_STOP_CONT_DTMF_REQ = 0x01
};

/*
 * An enumeration of eVOICE_SLQS_STOP_CONT_DTMF_RESP response TLV IDs
 */
enum eVOICE_SLQS_STOP_CONT_DTMF_RESP
{
    eTLV_STOP_CONT_DTMF_CALL_ID_RESP = 0x10
};

/*s
 * This structure contains the SLQSEndCall response parameters.
 */
struct QmiVoiceSLQSStopContDTMFResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* Unique call identifier response parameter */
    BYTE callId;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiVoiceSlqsStopContDTMF(
    WORD                  *pMlength,
    BYTE                  *pBuffer,
    voiceStopContDTMFinfo *pVoiceStopContDTMFinfo);

extern enum eQCWWANError UpkQmiVoiceSLQSStopContDTMF(
    BYTE                                *pMdmResp,
    struct QmiVoiceSLQSStopContDTMFResp *pApiResp );

#endif /* __VOICE_SLQS_VOICE_STOP_CONT_DTMF_H__ */
