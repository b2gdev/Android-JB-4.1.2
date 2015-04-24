/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceGetCOLP.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaVoiceSLQSVoiceGetCOLP.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef QAVOICESLQSVOICEGETCOLP_H_
#define QAVOICESLQVSOICEGETCOLP_H_

#include "qaGobiApiVoice.h"

/* enum declarations */
/*
 * An enumeration of eVOICE_SLQS_GET_COLP_RESP response TLV IDs
 */
enum eVOICE_SLQS_GET_COLP_RESP
{
    eTLV_COLP_RESPONSE              = 0x10,
    eTLV_COLP_FAILURE_CAUSE         = 0x11,
    eTLV_COLP_ALPHA_IDENTIFIER      = 0x12,
    eTLV_CALL_CONTROL_RESULT_TYPE   = 0x13,
    eTLV_COLP_CALL_ID               = 0x14,
    eTLV_CALL_CONTROL_SUPP_SER_TYPE = 0x15,
 };

/*
 * This structure contains the VoiceSLQSGetCOLP response parameters.
 *
 * \sa qaGobiApiVoice.h for parameter descriptions
 *
 */
struct QmiVoiceSLQSGetCOLPResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    voiceGetCOLPResp *pVoiceGetCOLPResp;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiVoiceSlqsGetCOLP(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiVoiceSlqsGetCOLP(
    BYTE                           *pMdmResp,
    struct QmiVoiceSLQSGetCOLPResp *pApiResp );

#endif /* __VOICE_SLQS_VOICE_GET_COLP_H__ */
