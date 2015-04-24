/*
 * \ingroup voice
 *
 * \file    qaVoiceOriginateUSSD.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaVoiceOriginateUSSD.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_ORIGINATE_USSD_H__
#define __VOICE_ORIGINATE_USSD_H__

#include "qaGobiApiVoice.h"

/* enum declarations */

/*
 * An enumeration of eQMI_VOICE_ORIG_USSD request TLV IDs
 */
enum eQMI_VOICE_ORIG_USSD_REQ
{
    eTLV_USS_Information = 0x01
};

/*
 * This structure contains the VoiceOriginateUSSD request parameters.
 */
struct QmiVoiceOriginateUSSDReq
{
    struct USSInfo *pUSSInfo;
};

/*
 * This structure contains the VoiceOriginateUSSD response parameters.
 */
struct QmiVoiceOriginateUSSDResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiVoiceOriginateUSSD(
    WORD           *pMlength,
    BYTE           *pBuffer,
    struct USSInfo *pUSSInfo );

extern enum eQCWWANError UpkQmiVoiceOriginateUSSD(
    BYTE                             *pMdmResp,
    struct QmiVoiceOriginateUSSDResp *pApiResp );

#endif /* __VOICE_ORIGINATE_USSD_H__ */
