/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceIndicationRegister.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaVoiceSLQSVoiceIndicationRegister.c
 *
 * Copyright: © 2011-12 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_SLQS_VOICE_INDICATION_REGISTER_H__
#define __VOICE_SLQS_VOICE_INDICATION_REGISTER_H__

#include"qaGobiApiVoice.h"

/* enum declarations */

/*
 * An enumeration of eVOICE_SLQS_INDICATION_REGISTER_REQ request TLV IDs
 */
enum eVOICE_SLQS_INDICATION_REGISTER_REQ
{
    eTLV_VOICE_DTMF_EVENTS               = 0x10,
    eTLV_VOICE_PRIVACY_EVENTS            = 0x11,
    eTLV_VOICE_SUPPS_NOTIFICATION_EVENTS = 0x12
};

/*
 * This structure contains the SLQSEndCall response parameters.
 */
struct QmiVoiceSLQSIndicationRegisterResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiVoiceSLQSIndicationRegister(
    WORD                        *pMlength,
    BYTE                        *pBuffer,
    voiceIndicationRegisterInfo *pIndicationRegisterInfo );

extern enum eQCWWANError UpkQmiVoiceSLQSIndicationRegister(
    BYTE                                      *pMdmResp,
    struct QmiVoiceSLQSIndicationRegisterResp *pApiResp );

#endif /* __VOICE_SLQS_VOICE_INDICATION_REGISTER_H__ */
