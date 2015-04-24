/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceGetCNAP.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaVoiceSLQSVoiceGetCNAP.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef QAVOICESLQSVOICEGETCNAP_H_
#define QAVOICESLQVSOICEGETCNAP_H_

#include "qaGobiApiVoice.h"

/* enum declarations */
/*
 * An enumeration of eVOICE_SLQS_GET_CNAP_RESP response TLV IDs
 */
enum eVOICE_SLQS_GET_CNAP_RESP
{
    eTLV_CNAP_RESPONSE            = 0x10,
    eTLV_CNAP_FAILURE_CAUSE       = 0x11,
    eTLV_CNAP_ALPHA_IDENTIFIER    = 0x12,
    eTLV_CNAP_CONTROL_RESULT_TYPE = 0x13,
    eTLV_CNAP_CALL_ID             = 0x14,
    eTLV_CNAP_CC_SUPP_SER_TYPE    = 0x15,
 };

/*
 * This structure contains the VoiceSLQSGetCNAP response parameters.
 *
 * \sa qaGobiApiVoice.h for parameter descriptions
 *
 */
struct QmiVoiceSLQSGetCNAPResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    voiceGetCNAPResp *pVoiceGetCNAPResp;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiVoiceSlqsGetCNAP(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiVoiceSlqsGetCNAP(
    BYTE                           *pMdmResp,
    struct QmiVoiceSLQSGetCNAPResp *pApiResp );

#endif /* __VOICE_SLQS_VOICE_GET_CNAP_H__ */
