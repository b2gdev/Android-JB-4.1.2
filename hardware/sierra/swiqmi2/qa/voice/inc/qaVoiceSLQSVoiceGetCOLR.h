/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceGetCOLR.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaVoiceSLQSVoiceGetCOLR.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef QAVOICESLQSVOICEGETCOLR_H_
#define QAVOICESLQVSOICEGETCOLR_H_

#include "qaGobiApiVoice.h"

/* enum declarations */
/*
 * An enumeration of eVOICE_SLQS_GET_COLR_RESP response TLV IDs
 */
enum eVOICE_SLQS_GET_COLR_RESP
{
    eTLV_COLR_RESPONSE            = 0x10,
    eTLV_COLR_FAILURE_CAUSE       = 0x11,
    eTLV_COLR_ALPHA_IDENTIFIER    = 0x12,
    eTLV_COLR_CONTROL_RESULT_TYPE = 0x13,
    eTLV_COLR_CALL_ID             = 0x14,
    eTLV_COLR_CC_SUPP_SER_TYPE    = 0x15,
 };

/*
 * This structure contains the VoiceSLQSGetCOLR response parameters.
 *
 * \sa qaGobiApiVoice.h for parameter descriptions
 *
 */
struct QmiVoiceSLQSGetCOLRResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    voiceGetCOLRResp *pVoiceGetCOLRResp;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiVoiceSlqsGetCOLR(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiVoiceSlqsGetCOLR(
    BYTE                           *pMdmResp,
    struct QmiVoiceSLQSGetCOLRResp *pApiResp );

#endif /* __VOICE_SLQS_VOICE_GET_COLR_H__ */
