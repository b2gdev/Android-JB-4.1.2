/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceEndCall.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaVoiceSLQSVoiceEndCall.c
 *
 * Copyright: © 2011-12 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_SLQS_VOICE_END_CALL_H__
#define __VOICE_SLQS_VOICE_END_CALL_H__

/* enum declarations */

/*
 * An enumeration of eVOICE_SLQS_END_CALL_REQ request TLV IDs
 */
enum eVOICE_SLQS_END_CALL_REQ
{
    eTLV_CALL_ID_REQ = 0x01
};

/*
 * An enumeration of eVOICE_SLQS_END_CALL_RESP response TLV IDs
 */
enum eVOICE_SLQS_END_CALL_RESP
{
    eTLV_CALL_ID_RESP = 0x10
};

/*
 * This structure contains the QmiVoiceSLQSEndCallReq request parameters.
 */
struct QmiVoiceSLQSEndCallReq
{
    /* Unique call identifier response parameter */
    BYTE *pCallId;
};

/*
 * This structure contains the SLQSEndCall response parameters.
 */
struct QmiVoiceSLQSEndCallResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* Unique call identifier response parameter */
    BYTE *pCallId;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiVoiceSlqsEndCall(
    WORD *pMlength,
    BYTE *pBuffer,
    BYTE *pCallId );

extern enum eQCWWANError UpkQmiVoiceSlqsEndCall(
    BYTE                           *pMdmResp,
    struct QmiVoiceSLQSEndCallResp *pApiResp );

#endif /* __VOICE_SLQS_VOICE_END_CALL_H__ */
