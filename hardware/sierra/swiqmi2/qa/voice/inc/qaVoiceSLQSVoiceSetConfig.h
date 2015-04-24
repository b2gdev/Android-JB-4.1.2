/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceSetConfig.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaVoiceSLQSVoiceSetConfig.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_SLQS_VOICE_SET_CONFIG_H__
#define __VOICE_SLQS_VOICE_SET_CONFIG_H__

#include "qaGobiApiVoice.h"

/* enum declarations */

/*
 * An enumeration of eVOICE_SLQS_SET_CONFIG_REQ request TLV IDs
 */
enum eVOICE_SLQS_SET_CONFIG_REQ
{
    eTLV_SET_AUTO_ANSWER  = 0x10,
    eTLV_SET_AIR_TIMER    = 0x11,
    eTLV_SET_ROAM_TIMER   = 0x12,
    eTLV_SET_TTY_MODE     = 0x13,
    eTLV_SET_PREF_VSO     = 0x14,
    eTLV_SET_PREF_VDOMAIN = 0x15
};

/*
 * An enumeration of eVOICE_SLQS_SET_CONFIG_RESP response TLV IDs
 */
enum eVOICE_SLQS_SET_CONFIG_RESP
{
    eTLV_AUTO_ANSWER_OC  = 0x10,
    eTLV_AIR_TIMER_OC    = 0x11,
    eTLV_ROAM_TIMER_OC   = 0x12,
    eTLV_TTY_MODE_OC     = 0x13,
    eTLV_PREF_VSO_OC     = 0x14,
    eTLV_PREF_VDOMAIN_OC = 0x15,
};

/*
 * This structure contains the VoiceSLQSSetConfig response parameters.
 */
struct QmiVoiceSLQSSetConfigResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    voiceSetConfigResp *pVoiceSetConfigResp;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiVoiceSlqsSetConfig(
    WORD              *pMlength,
    BYTE              *pBuffer,
    voiceSetConfigReq *pVoiceSetConfigReq );

extern enum eQCWWANError UpkQmiVoiceSlqsSetConfig(
    BYTE                             *pMdmResp,
    struct QmiVoiceSLQSSetConfigResp *pApiResp );

#endif /* __VOICE_SLQS_VOICE_SET_CONFIG_H__ */


