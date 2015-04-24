/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceSetCallBarringPassword.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaVoiceSLQSVoiceSetCallBarringPassword.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_SLQS_VOICE_SET_CALL_BARRING_PASSWORD_H__
#define __VOICE_SLQS_VOICE_SET_CALL_BARRING_PASSWORD_H__

#include "qaGobiApiVoice.h"

/* enum declarations */
/*
 * An enumeration of eVOICE_SLQS_SET_CALL_BARRING_PASSWORD_REQ response TLV IDs
 */
enum eVOICE_SLQS_SET_CALL_BARRING_PASSWORD_REQ
{
    eTLV_CBPWD_INFO        = 0x01
};

/*
 * An enumeration of eVOICE_SLQS_SET_CALL_BARRING_PASSWORD_RESP response TLV IDs
 */
enum eVOICE_SLQS_SET_CALL_BARRING_PASSWORD_RESP
{
    eTLV_CBPWD_FAILURE_CAUSE = 0x10,
    eTLV_CBPWD_ALPHA_ID      = 0x11,
    eTLV_CBPWD_RESULT_TYPE   = 0x12,
    eTLV_CBPWD_CALL_ID       = 0x13,
    eTLV_CBPWD_CC_SUP_S_TYPE = 0x14,
};

/*
 * This structure contains the VoiceSLQSSetCallBarringPwdReq response parameters.
 */
struct QmiVoiceSLQSSetCallBarringPwdReq
{
    voiceSetCallBarringPwdInfo *pVoiceSetCallBarringPwdInfo;
};

/*
 * This structure contains the VoiceSLQSSetCallBarringPwdResp response parameters.
 */
struct QmiVoiceSLQSSetCallBarringPwdResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    voiceSetCallBarringPwdResp *pSetCallBarringPwdResp;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiVoiceSlqsSetCallBarringPwd(
    WORD                       *pMlength,
    BYTE                       *pBuffer,
    voiceSetCallBarringPwdInfo *pVoiceSETCallFWReq );

extern enum eQCWWANError UpkQmiVoiceSlqsSetCallBarringPwd(
    BYTE                                     *pMdmResp,
    struct QmiVoiceSLQSSetCallBarringPwdResp *pApiResp );

#endif /* __VOICE_SLQS_VOICE_SET_CALL_BARRING_PASSWORD_H__ */

