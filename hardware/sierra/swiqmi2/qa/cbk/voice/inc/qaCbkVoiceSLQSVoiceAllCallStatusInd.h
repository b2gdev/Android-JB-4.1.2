/*
 * \ingroup cbk
 *
 * \file    qaCbkVoiceSLQSVoiceAllCallStatusInd.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaCbkVoiceAllCallStatusInd.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_ALL_CALL_STATUS_IND_H_
#define __VOICE_ALL_CALL_STATUS_IND_H_

#include "qaGobiApiVoice.h"
#include "qaGobiApiCbk.h"

/*
 * An enumeration of eQMI_VOICE_ALL_CALL_STATUS_IND response TLV IDs
 */
enum eQMI_VOICE_ALL_CALL_STATUS_IND
{
    eTLV_ARR_CALL_INFORM      = 0x01,
    eTLV_ARR_REM_PARTY_NUM    = 0x10,
    eTLV_ARR_REM_PARTY_NAME   = 0x11,
    eTLV_ARR_AlERT_TYPE       = 0x12,
    eTLV_ARR_SERVICE_OPT      = 0x13,
    eTLV_ARR_CALL_END_REASON  = 0x14,
    eTLV_ARR_ALPHA_IDEN       = 0x15,
    eTLV_ARR_CONN_PARTY_NUM   = 0x16,
    eTLV_ARR_DIAGNOS_INFORM   = 0x17,
    eTLV_ARR_CALLED_PARTY_NUM = 0x18,
    eTLV_ARR_REDIR_PARTY_NUM  = 0x19,
    eTLV_ARR_ALERT_PATTERN    = 0x1A

};

/*
 * Prototypes
 */
enum eQCWWANError UpkQmiCbkVoiceAllCallStatusInd(
    BYTE                         *pMdmResp,
    voiceSetAllCallStatusCbkInfo *pApiResp );

#endif /* __VOICE_ALL_CALL_STATUS_IND_H_ */
