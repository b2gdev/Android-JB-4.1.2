/*
 * \ingroup cbk
 *
 * \file    qaCbkVoiceSLQSVoiceSUPSInd.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaCbkVoiceSLQSVoiceSUPSInd.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_SUPS_IND_H_
#define __VOICE_SUPS_IND_H_

#include "qaGobiApiCbk.h"

/*
 * An enumeration of eQMI_VOICE_SUPS_IND response TLV IDs
 */
enum eQMI_VOICE_SUPS_IND
{
    eTLV_SUPS_INFORMATION      = 0x01,
    eTLV_SUPS_SERVICE_CLASS    = 0x10,
    eTLV_SUPS_REASON           = 0x11,
    eTLV_SUPS_CALL_FW_NUMBER   = 0x12,
    eTLV_SUPS_CALL_FW_NR_TIMER = 0x13,
    eTLV_SUPS_USS_INFO         = 0x14,
    eTLV_SUPS_CALL_ID          = 0x15,
    eTLV_SUPS_ALPHA_ID         = 0x16,
    eTLV_SUPS_CB_PWD           = 0x17,
    eTLV_SUPS_NEW_PWD_DATA     = 0x18,
    eTLV_SUPS_DATA_SOURCE      = 0x19,
    eTLV_SUPS_FAILURE_CAUSE    = 0x1A,
    eTLV_SUPS_CALL_FW_DATA     = 0x1B,
    eTLV_SUPS_CLIR_STATUS      = 0x1C,
    eTLV_SUPS_CLIP_STATUS      = 0x1D,
    eTLV_SUPS_COLP_STATUS      = 0x1E,
    eTLV_SUPS_COLR_STATUS      = 0x1F,
    eTLV_SUPS_CNAP_STATUS      = 0x20
};

/*
 * Prototypes
 */
enum eQCWWANError UpkQmiCbkVoiceSUPSInd(
    BYTE          *pMdmResp,
    voiceSUPSInfo *pApiResp );

#endif /* __VOICE_SUPS_IND_H_ */
