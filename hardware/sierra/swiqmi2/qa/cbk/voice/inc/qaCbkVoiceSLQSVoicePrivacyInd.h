/*
 * \ingroup cbk
 *
 * \file    qaCbkVoiceSLQSVoicePrivacyInd.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaCbkVoiceSLQSVoicePrivacyInd.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_SET_PRIVACY_CHANGE_IND_H_
#define __VOICE_SET_PRIVACY_CHANGE_IND_H_

#include "qaGobiApiCbk.h"

/*
 * An enumeration of eQMI_VOICE_PRIVACY_IND response TLV IDs
 */
enum eQMI_VOICE_PRIVACY_IND
{
    eTLV_VOICE_PRIVACY_INFO = 0x01
};

/*
 * Prototypes
 */
enum eQCWWANError UpkQmiCbkVoicePrivacyInd(
    BYTE             *pMdmResp,
    voicePrivacyInfo *pApiResp );

#endif /* __VOICE_SET_PRIVACY_CHANGE_IND_H_ */
