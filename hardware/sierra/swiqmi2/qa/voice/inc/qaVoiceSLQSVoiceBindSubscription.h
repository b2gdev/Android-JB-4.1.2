/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceBindSubscription.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaVoiceSLQSVoiceBindSubscription.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_SLQS_VOICE_BIND_SUBSCRIPTION_H__
#define __VOICE_SLQS_VOICE_BIND_SUBSCRIPTION_H__

#include "qaGobiApiVoice.h"

/* enum declarations */
/*
 * An enumeration of eVOICE_SLQS_BIND_SUBSCRIPTION_REQ request TLV IDs
 */
enum eVOICE_SLQS_BIND_SUBSCRIPTION_REQ
{
    eTLV_BIND_SUBSCRIPTION_TYPE = 0x01
};

/*
 * This structure contains the VoiceSLQSBindSubscription response parameters.
 */
struct QmiVoiceSLQSBindSubscriptionResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiVoiceSlqsBindSubscription(
    WORD                      *pMlength,
    BYTE                      *pBuffer,
    voiceBindSubscriptionInfo *pVoiceBindSubscriptionInfo );

extern enum eQCWWANError UpkQmiVoiceSlqsBindSubscription(
    BYTE                                    *pMdmResp,
    struct QmiVoiceSLQSBindSubscriptionResp *pApiResp );

#endif /* __VOICE_SLQS_VOICE_BIND_SUBSCRIPTION_H__ */
