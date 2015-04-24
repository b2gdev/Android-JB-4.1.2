/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceDialCall.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaVoiceSLQSDialCall.c
 *
 * Copyright: © 2011-12 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_SLQS_VOICE_DIAL_CALL_H__
#define __VOICE_SLQS_VOICE_DIAL_CALL_H__

#define CALLING_NUMBER_MAX_INDEX 81
#define CC_RESULT_TYPE_VOICE     0x00
#define CUG_INDEX_MAX            0x7FFF
#define CALL_TYPE_EMERGENCY      0x09

/* enum declarations */

/*
 * An enumeration of eVOICE_SLQS_DIAL_CALL_REQ request TLV IDs
 */
enum eVOICE_SLQS_DIAL_CALL_REQ
{
    eTLV_CALLING_NUMBER  = 0x01,
    eTLV_CALL_TYPE       = 0x10,
    eTLV_CLIR_TMP_MODE   = 0x11,
    eTLV_UUS_INFO        = 0x12,
    eTLV_CUG_INFO        = 0x13,
    eTLV_EMERGENCY_CAT   = 0x14,
    eTLV_CALLED_PARTY_SA = 0x15,
    eTLV_SERVICE_TYPE    = 0x16
};

/*
 * An enumeration of eVOICE_SLQS_DIAL_CALL_RESP response TLV IDs
 */
enum eVOICE_SLQS_DIAL_CALL_RESP
{
    eTLV_CALL_ID          = 0x10,
    eTLV_ALPHA_ID         = 0x11,
    eTLV_CC_RESULT_TYPE   = 0x12,
    eTLV_CC_SUP_S_TYPE    = 0x13
};

/*
 * This structure contains the QmiVoiceSLQSDialCallReq request parameters.
 */
struct QmiVoiceSLQSDialCallReq
{
    voiceCallRequestParams *pCallRequestParams;
};

/*
 * This structure contains the VoiceSLQSDialCall response parameters.
 */
struct QmiVoiceSLQSDialCallResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    voiceCallResponseParams *pCallResponseParams;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiVoiceSlqsDialCall(
    WORD                   *pMlength,
    BYTE                   *pBuffer,
    voiceCallRequestParams *pCallRequestParams );

extern enum eQCWWANError UpkQmiVoiceSlqsDialCall(
    BYTE                            *pMdmResp,
    struct QmiVoiceSLQSDialCallResp *pApiResp );

#endif /* __VOICE_SLQS_VOICE_DIAL_CALL_H__ */

