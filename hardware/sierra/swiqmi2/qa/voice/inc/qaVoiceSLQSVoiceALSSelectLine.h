/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceALSSelectLine.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaVoiceSLQSVoiceALSSelectLine.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_SLQS_VOICE_ALS_SELECT_LINE_H__
#define __VOICE_SLQS_VOICE_ALS_SELECT_LINE_H__

#include "qaGobiApiVoice.h"

/* enum declarations */
/*
 * An enumeration of eVOICE_SLQS_ALS_SELECT_LINE_REQ request TLV IDs
 */
enum eVOICE_SLQS_ALS_SELECT_LINE_REQ
{
    eTLV_ALS_SET_LINE_VALUE = 0x01
};

/*
 * This structure contains the VoiceSLQSALSSelectLine response parameters.
 */
struct QmiVoiceSLQSALSSelectLineResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiVoiceSlqsALSSelectLine(
    WORD                   *pMlength,
    BYTE                   *pBuffer,
    voiceALSSelectLineInfo *pVoiceALSSelectLineInfo );

extern enum eQCWWANError UpkQmiVoiceSlqsALSSelectLine(
    BYTE                                 *pMdmResp,
    struct QmiVoiceSLQSALSSelectLineResp *pApiResp );

#endif /* __VOICE_SLQS_VOICE_ALS_SELECT_LINE_H__ */
