/*
 * \ingroup cbk
 *
 * \file    qaCbkVoiceSLQSVoiceDTMFInd.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaCbkVoiceSLQSVoiceDTMFInd.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_DTMF_IND_H_
#define __VOICE_DTMF_IND_H_

#include "qaGobiApiCbk.h"

/*
 * An enumeration of eQMI_VOICE_DTMF_IND response TLV IDs
 */
enum eQMI_VOICE_DTMF_IND
{
    eTLV_DTMF_INFORMATION         = 0x01,
    eTLV_DTMF_PULSE_WIDTH         = 0x10,
    eTLV_DTMF_INTERDIGIT_INTERVAL = 0x11
};

/*
 * Prototypes
 */
enum eQCWWANError UpkQmiCbkVoiceDTMFEventInd(
    BYTE               *pMdmResp,
    voiceDTMFEventInfo *pApiResp );

#endif /* __VOICE_DTMF_IND_H_ */
