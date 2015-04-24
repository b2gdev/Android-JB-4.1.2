/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceALSSetLineSwitching.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaVoiceSLQSVoiceALSSetLineSwitching.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_SLQS_VOICE_ALS_SET_LINE_SWITCHING_H__
#define __VOICE_SLQS_VOICE_ALS_SET_LINE_SWITCHING_H__

#include "qaGobiApiVoice.h"

/* enum declarations */
/*
 * An enumeration of eVOICE_SLQS_ALS_SET_LINE_SWITCHING_REQ request TLV IDs
 */
enum eVOICE_SLQS_ALS_SET_LINE_SWITCHING_REQ
{
    eTLV_ALS_SET_LINE_SWITCH_OPTION = 0x01
};

/*
 * This structure contains the VoiceSLQSALSSetLineSwitching response parameters.
 */
struct QmiVoiceSLQSALSSetLineSwitchingResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiVoiceSlqsALSSetLineSwitching(
    WORD                      *pMlength,
    BYTE                      *pBuffer,
    voiceALSSetLineSwitchInfo *pVoiceALSSetLineSwitchInfo );

extern enum eQCWWANError UpkQmiVoiceSlqsALSSetLineSwitching(
    BYTE                                       *pMdmResp,
    struct QmiVoiceSLQSALSSetLineSwitchingResp *pApiResp );

#endif /* __VOICE_SLQS_VOICE_ALS_SET_LINE_SWITCHING_H__ */
