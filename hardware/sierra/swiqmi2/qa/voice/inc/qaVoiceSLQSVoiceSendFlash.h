/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceSendFlash.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaVoiceSLQSVoiceSendFlash.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_SLQS_VOICE_SEND_FLASH_H__
#define __VOICE_SLQS_VOICE_SEND_FLASH_H__

#include "qaGobiApiVoice.h"

/* enum declarations */
/*
 * An enumeration of eVOICE_SLQS_SEND_FLASH_REQ response TLV IDs
 */
enum eVOICE_SLQS_SEND_FLASH_REQ
{
    eTLV_SEND_FLASH_CALL_ID = 0x01,
    eTLV_SEND_FLASH_PAYLOAD = 0x10,
    eTLV_SEND_FLASH_TYPE    = 0x11
};

/*
 * An enumeration of eVOICE_SLQS_SEND_FLASH_RESP response TLV IDs
 */
enum eVOICE_SLQS_SEND_FLASH_RESP
{
    eTLV_SEND_FLASH_CALL_ID_RSP  = 0x10
};

/*
 * This structure contains the VoiceSLQSSendFlashResp response parameters.
 */
struct QmiVoiceSLQSSendFlashResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    BYTE *pCallId;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiVoiceSlqsSendFlash(
    WORD           *pMlength,
    BYTE           *pBuffer,
    voiceFlashInfo *pFlashInfo );

extern enum eQCWWANError UpkQmiVoiceSlqsSendFlash(
    BYTE                             *pMdmResp,
    struct QmiVoiceSLQSSendFlashResp *pApiResp );

#endif /* __VOICE_SLQS_VOICE_SEND_FLASH_H__ */

