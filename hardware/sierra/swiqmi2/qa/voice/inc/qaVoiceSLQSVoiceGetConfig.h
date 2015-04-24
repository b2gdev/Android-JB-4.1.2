/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceGetConfig.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaVoiceSLQSVoiceGetConfig.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_SLQS_VOICE_GET_CONFIG_H__
#define __VOICE_SLQS_VOICE_GET_CONFIG_H__

#include "qaGobiApiVoice.h"

/* enum declarations */

/*
 * An enumeration of eVOICE_SLQS_GET_CONFIG_REQ request TLV IDs
 */
enum eVOICE_SLQS_GET_CONFIG_REQ
{
    eTLV_GET_AUTO_ANSWER        = 0x10,
    eTLV_GET_AIR_TIMER          = 0x11,
    eTLV_GET_ROAM_TIMER         = 0x12,
    eTLV_GET_TTY_MODE           = 0x13,
    eTLV_GET_PREF_VSO           = 0x14,
    eTLV_GET_AMR_STATUS         = 0x15,
    eTLV_GET_VOICE_PRIVACY_PREF = 0x16,
    eTLV_GET_NAM_ID             = 0x17,
    eTLV_GET_PREF_VDOMAIN       = 0x18
};

/*
 * An enumeration of eVOICE_SLQS_GET_CONFIG_RESP response TLV IDs
 */
enum eVOICE_SLQS_GET_CONFIG_RESP
{
    eTLV_GET_AUTO_ANSWER_RESP       = 0x10,
    eTLV_GET_CUR_AIR_TIMER          = 0x11,
    eTLV_GET_CUR_ROAM_TIMER         = 0x12,
    eTLV_GET_CUR_TTY_MODE           = 0x13,
    eTLV_GET_CUR_PREF_VSO           = 0x14,
    eTLV_GET_CUR_AMR_CONFIG         = 0x15,
    eTLV_GET_CUR_VOICE_PRIVACY_PREF = 0x16,
    eTLV_GET_CUR_VOICE_DOMAIN_PREF  = 0x17,
};

/*
 * This structure contains the VoiceSLQSGetConfig response parameters.
 */
struct QmiVoiceSLQSGetConfigResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    voiceGetConfigResp *pVoiceGetConfigResp;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiVoiceSlqsGetConfig(
    WORD              *pMlength,
    BYTE              *pBuffer,
    voiceGetConfigReq *pVoiceGetConfigReq );

extern enum eQCWWANError UpkQmiVoiceSlqsGetConfig(
    BYTE                             *pMdmResp,
    struct QmiVoiceSLQSGetConfigResp *pApiResp );

#endif /* __VOICE_SLQS_VOICE_GET_CONFIG_H__ */
