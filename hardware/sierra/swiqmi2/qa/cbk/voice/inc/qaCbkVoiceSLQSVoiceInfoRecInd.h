/*
 * \ingroup cbk
 *
 * \file    qaCbkVoiceSLQSVoiceInfoRecInd.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaCbkVoiceSLQSVoiceInfoRecInd.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_INFO_REC_IND_H_
#define __VOICE_INFO_REC_IND_H_

#include "qaGobiApiCbk.h"
#include "qaGobiApiVoice.h"

/*
 * An enumeration of eQMI_VOICE_INFO_REC_IND response TLV IDs
 */
enum eQMI_VOICE_INFO_REC_IND
{
     eTLV_CALL_ID_INFO_REC          = 0x01,
     eTLV_SIGNAL_INFO               = 0x10,
     eTLV_CALLER_ID_INFORMATION     = 0x11,
     eTLV_DISPLAY_INFORMATION       = 0x12,
     eTLV_EXTENDED_DISPLAY_INFO     = 0x13,
     eTLV_CALLER_NAME_INFO          = 0x14,
     eTLV_CALL_WAITING_IND          = 0x15,
     eTLV_CONNECTED_NUMBER_INFO     = 0x16,
     eTLV_CALLING_PARTY_NUMBER_INFO = 0x17,
     eTLV_CALLED_PARTY_NUMBER_INFO  = 0x18,
     eTLV_REDIRECTING_NUMBER_INFO   = 0x19,
     eTLV_NSS_CLIR                  = 0x1A,
     eTLV_NSS_AUDIO_CONTROL         = 0x1B,
     eTLV_NSS_RELEASE               = 0x1C,
     eTLV_LINE_CONTROL_INFO         = 0x1D,
     eTLV_EXT_DISP_REC_INFO         = 0x1E
};

/*
 * Prototypes
 */
enum eQCWWANError UpkQmiCbkVoiceInfoRecInd(
    BYTE         *pMdmResp,
    voiceInfoRec *pApiResp );

#endif /* __VOICE_INFO_REC_IND_H_ */
