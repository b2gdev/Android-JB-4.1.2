/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceOrigUSSDNoWait.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaVoiceSLQSVoiceOrigUSSDNoWait.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_SLQS_VOICE_ORIG_USSD_NO_WAIT_H__
#define __VOICE_SLQS_VOICE_ORIG_USSD_NO_WAIT_H__

#include "qaGobiApiVoice.h"

/* enum declarations */
/*
 * An enumeration of eVOICE_SLQS_ORIG_USSD_NO_WAIT_REQ request TLV IDs
 */
enum eVOICE_SLQS_ORIG_USSD_NO_WAIT_REQ
{
    eTLV_ORIG_USSD_NW_USS_INFO = 0x01
};

/*
 * This structure contains the VoiceSLQSOrigUSSDNoWait response parameters.
 */
struct QmiVoiceSLQSOrigUSSDNoWaitResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiVoiceSlqsOrigUSSDNoWait(
    WORD                    *pMlength,
    BYTE                    *pBuffer,
    voiceOrigUSSDNoWaitInfo *pVoiceOrigUSSDNoWaitInfo );

extern enum eQCWWANError UpkQmiVoiceSlqsOrigUSSDNoWait(
    BYTE                                  *pMdmResp,
    struct QmiVoiceSLQSOrigUSSDNoWaitResp *pApiResp );

#endif /* __VOICE_SLQS_VOICE_ORIG_USSD_NO_WAIT_H__ */
