/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceManageCalls.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaVoiceSLQSManageCalls.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_MANAGE_CALLS_H__
#define __VOICE_MANAGE_CALLS_H__

#include "qaGobiApiVoice.h"

/* enum declarations */

/*
 * An enumeration of eQMI_VOICE_MANAGE_CALLS request TLV IDs
 */
enum eQMI_VOICE_MANAGE_CALLS_REQ
{
    eTLV_SUPS_TYPE      = 0x01,
    eTLV_CALL_ID_MANAGE = 0x10
};

/*
 * An enumeration of eQMI_VOICE_MANAGE_CALLS request TLV IDs
 */
enum eQMI_VOICE_MANAGE_CALLS_RESP
{
    eTLV_FAILURE_CAUSE = 0x10
};

/*
 * This structure contains the VoiceManageCalls response parameters.
 */
struct QmiVoiceManageCallsResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
    voiceManageCallsResp *pVoiceManageCallsResp;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiVoiceManageCalls(
    WORD                *pMlength,
    BYTE                *pBuffer,
    voiceManageCallsReq *pVoiceManageCallsReq );

extern enum eQCWWANError UpkQmiVoiceManageCalls(
    BYTE                           *pMdmResp,
    struct QmiVoiceManageCallsResp *pApiResp );

#endif /* __VOICE_MANAGE_CALLS_H__ */
