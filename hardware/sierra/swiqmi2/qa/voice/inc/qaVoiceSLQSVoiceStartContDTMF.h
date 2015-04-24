/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceStartContDTMF.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaVoiceSLQSVoiceStartContDTMF.c
 *
 * Copyright: © 2011-12 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_SLQS_VOICE_START_CONT_DTMF_H__
#define __VOICE_SLQS_VOICE_START_CONT_DTMF_H__

#include"qaGobiApiVoice.h"

/* enum declarations */

/*
 * An enumeration of eVOICE_SLQS_START_CONT_DTMF_REQ request TLV IDs
 */
enum eVOICE_SLQS_START_CONT_DTMF_REQ
{
    eTLV_START_CONT_DTMF_REQ = 0x01
};

/*
 * An enumeration of eVOICE_SLQS_START_CONT_DTMF_RESP response TLV IDs
 */
enum eVOICE_SLQS_START_CONT_DTMF_RESP
{
    eTLV_START_CONT_DTMF_CALL_ID_RESP = 0x10
};

/*
 * This structure contains the SLQSEndCall response parameters.
 */
struct QmiVoiceSLQSStartContDTMFResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* Unique call identifier response parameter */
    BYTE *pCallId;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiVoiceSlqsStartContDTMF(
    WORD              *pMlength,
    BYTE              *pBuffer,
    voiceContDTMFinfo *pContDTMFInfo );

extern enum eQCWWANError UpkQmiVoiceSLQSStartContDTMF(
    BYTE                                 *pMdmResp,
    struct QmiVoiceSLQSStartContDTMFResp *pApiResp );

#endif /* __VOICE_SLQS_VOICE_START_CONT_DTMF_H__ */
