/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceGetCallBarring.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaVoiceSLQSVoiceGetCallBarring.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_SLQS_VOICE_GET_CALL_BARRING_H__
#define __VOICE_SLQS_VOICE_GET_CALL_BARRING_H__

#include "qaGobiApiVoice.h"

/* enum declarations */
/*
 * An enumeration of eVOICE_SLQS_GET_CALL_BARRING_REQ response TLV IDs
 */
enum eVOICE_SLQS_GET_CALL_BARRING_REQ
{
    eTLV_CBAR_REASON      = 0x01,
    eTLV_CBAR_S_CLASS_REQ = 0x10
};

/*
 * An enumeration of eVOICE_SLQS_GET_CALL_BARRING_RESP response TLV IDs
 */
enum eVOICE_SLQS_GET_CALL_BARRING_RESP
{
    eTLV_CBAR_S_CLASS_RESP  = 0x10,
    eTLV_CBAR_FAILURE_CAUSE = 0x11,
    eTLV_CBAR_ALPHA_ID      = 0x12,
    eTLV_CBAR_RESULT_TYPE   = 0x13,
    eTLV_CBAR_CALL_ID       = 0x14,
    eTLV_CBAR_CC_SUP_S_TYPE = 0x15
};

/*
 * This structure contains the VoiceSLQSGetCallBarringResp response parameters.
 */
struct QmiVoiceSLQSGetCallBarringResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    voiceGetCallBarringResp *pVoiceGetCallBarringResp;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiVoiceSlqsGetCallBarring(
    WORD                   *pMlength,
    BYTE                   *pBuffer,
    voiceGetCallBarringReq *pVoiceGetCallBarringReq );

extern enum eQCWWANError UpkQmiVoiceSlqsGetCallBarring(
    BYTE                                  *pMdmResp,
    struct QmiVoiceSLQSGetCallBarringResp *pApiResp );

#endif /* __VOICE_SLQS_VOICE_GET_CALL_BARRING_H__ */

