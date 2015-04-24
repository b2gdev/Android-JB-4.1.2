/*
 * \ingroup voice
 *
 * \file    qaVoiceCancelUSSD.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaVoiceCancelUSSD.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_CANCEL_USSD_H__
#define __VOICE_CANCEL_USSD_H__

#include "qaGobiApiVoice.h"

/*
 * This structure contains the VoiceCancelUSSD request parameters.
 */
struct QmiVoiceCancelUSSDReq
{
    struct USSInfo *pUSSInfo;
};

/*
 * This structure contains the VoiceCancelUSSD response parameters.
 */
struct QmiVoiceCancelUSSDResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiVoiceCancelUSSD(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiVoiceCancelUSSD(
    BYTE                          *pMdmResp,
    struct QmiVoiceCancelUSSDResp *pApiResp );

#endif /* __VOICE_CANCEL_USSD_H__ */
