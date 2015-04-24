/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceBurstDTMF.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaVoiceSLQSVoiceBurstDTMF.c
 *
 * Copyright: © 2011-12 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_SLQS_VOICE_BURST_DTMF_H__
#define __VOICE_SLQS_VOICE_BURST_DTMF_H__

#include"qaGobiApiVoice.h"

/* enum declarations */

/*
 * An enumeration of eVOICE_SLQS_BURST_DTMF_REQ request TLV IDs
 */
enum eVOICE_SLQS_BURST_DTMF_REQ
{
    eTLV_BURST_DTMF_INFO_REQ = 0x01,
    eTLV_DTMF_LENGTH_REQ     = 0x10
};

/*
 * An enumeration of eVOICE_SLQS_BURST_DTMF_RESP response TLV IDs
 */
enum eVOICE_SLQS_BURST_DTMF_RESP
{
    eTLV_DTMF_CALL_ID_RESP = 0x10
};

/*s
 * This structure contains the SLQSEndCall response parameters.
 */
struct QmiVoiceSLQSBurstDTMFResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* Unique call identifier response parameter */
    BYTE *pCallId;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiVoiceSlqsBurstDTMF(
    WORD               *pMlength,
    BYTE               *pBuffer,
    voiceBurstDTMFInfo *pBurstDTMFInfo );

extern enum eQCWWANError UpkQmiVoiceSLQSBurstDTMF(
    BYTE                             *pMdmResp,
    struct QmiVoiceSLQSBurstDTMFResp *pApiResp );

#endif /* __VOICE_SLQS_VOICE_BURST_DTMF_H__ */
