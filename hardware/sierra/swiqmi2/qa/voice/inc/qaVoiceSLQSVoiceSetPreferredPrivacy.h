/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceSetPreferredPrivacy.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaVoiceSLQSVoiceSetPreferredPrivacy.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_SLQS_VOICE_SET_PREF_PRIVACY_H__
#define __VOICE_SLQS_VOICE_SET_PREF_PRIVACY_H__

#include "qaGobiApiVoice.h"

/* enum declarations */
/*
 * An enumeration of eVOICE_SLQS_SET_PREF_PRIVACY_REQ response TLV IDs
 */
enum eVOICE_SLQS_SET_PREF_PRIVACY_REQ
{
    eTLV_SET_VOICE_PRIVACY_PREF = 0x01,
};

/*
 * This structure contains the VoiceSLQSSetPrefPrivacyResp response parameters.
 */
struct QmiVoiceSLQSSetPrefPrivacyResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiVoiceSlqsSetPrefPrivacy(
    WORD                *pMlength,
    BYTE                *pBuffer,
    voiceSetPrefPrivacy *pSetPrefPrivacy );

extern enum eQCWWANError UpkQmiVoiceSlqsSetPrefPrivacy(
    BYTE                                  *pMdmResp,
    struct QmiVoiceSLQSSetPrefPrivacyResp *pApiResp );

#endif /* __VOICE_SLQS_VOICE_SET_PREF_PRIVACY_H__ */
