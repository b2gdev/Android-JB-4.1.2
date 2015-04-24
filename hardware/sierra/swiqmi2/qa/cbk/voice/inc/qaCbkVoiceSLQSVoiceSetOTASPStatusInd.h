/*
 * \ingroup cbk
 *
 * \file    qaCbkVoiceSLQSVoiceSetOTASPStatusInd.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaCbkVoiceSetOTASPStatusInd.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_OTASP_STATUS_IND_H_
#define __VOICE_OTASP_STATUS_IND_H_


#include "qaGobiApiCbk.h"
#include "qaGobiApiVoice.h"

/*
 * An enumeration of eQMI_VOICE_OTASP_STATUS_IND response TLV IDs
 */
enum eQMI_VOICE_OTASP_STATUS_IND
{
    eTLV_OTASP_STATUS_INFO = 0x01
};

/*
 * Prototypes
 */
enum eQCWWANError UpkQmiCbkVoiceSetOTASPStatusInd(
    BYTE                 *pMdmResp,
    voiceOTASPStatusInfo *pApiResp );

#endif /* __VOICE_OTASP_STATUS_IND_H_ */
