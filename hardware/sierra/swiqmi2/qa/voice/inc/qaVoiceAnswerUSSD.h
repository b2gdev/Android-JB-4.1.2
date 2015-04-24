/*
 * \ingroup voice
 *
 * \file    qaVoiceAnswerUSSD.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaVoiceAnswerUSSD.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_ANSWER_USSD_H__
#define __VOICE_ANSWER_USSD_H__

#include "qaGobiApiVoice.h"

/* enum declarations */

/*
 * An enumeration of eQMI_VOICE_ANSWER_USSD request TLV IDs
 */
enum eQMI_VOICE_ANSWER_USSD_REQ
{
    eTLV_USSD_ANSWER = 0x01
};

/*
 * This structure contains the VoiceAnswerUSSD request parameters.
 */
struct QmiVoiceAnswerUSSDReq
{
    struct USSInfo *pUSSInfo;
};

/*
 * This structure contains the VoiceAnswerUSSD response parameters.
 */
struct QmiVoiceAnswerUSSDResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiVoiceAnswerUSSD(
    WORD           *pMlength,
    BYTE           *pBuffer,
    struct USSInfo *pUSSInfo );

extern enum eQCWWANError UpkQmiVoiceAnswerUSSD(
    BYTE                          *pMdmResp,
    struct QmiVoiceAnswerUSSDResp *pApiResp );

#endif /* __VOICE_ANSWER_USSD_H__ */
