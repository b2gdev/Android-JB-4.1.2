/*
 * \ingroup voice
 *
 * \file    qaVoiceSLQSVoiceSetSUPSService.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaVoiceSLQSVoiceSetSUPSService.c
 *
 * Copyright: © 2011-12 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_SLQS_VOICE_SET_SUPS_SERVICE_H__
#define __VOICE_SLQS_VOICE_SET_SUPS_SERVICE_H__

#define QMI_VOICE_CALL_BARRING_PWD_LEN 4
#define QMI_VOICE_CALL_FORWARDING_LEN  81

/* enum declarations */

/*
 * An enumeration of eVOICE_SLQS_SET_SUPS_SERVICE_REQ request TLV IDs
 */
enum eVOICE_SLQS_SET_SUPS_SERVICE_REQ
{
    eTLV_SUPS_SERVICE_INFO            = 0x01,
    eTLV_SERVICE_CLASS                = 0x10,
    eTLV_CALL_BARRING_PWD             = 0x11,
    eTLV_CALL_FORWARDING_NUMBER       = 0x12,
    eTLV_CALL_FORWARDING_NO_REPLY     = 0x13,
    eTLV_CALL_FORWARDING_NU_TYPE_PLAN = 0x14,
};

/*
 * An enumeration of eVOICE_SLQS_DIAL_CALL_RESP response TLV IDs
 */
enum eVOICE_SLQS_SET_SUPS_SERVICE_RESP
{
    eTLV_SUPS_FAILURE_CAUSE    = 0x10,
    eTLV_SUPS_ALPHA_ID         = 0x11,
    eTLV_SUPS_CALL_ID          = 0x12,
    eTLV_SUPS_CC_RESULT_TYPE   = 0x13,
    eTLV_SUPS_CC_SUP_S_TYPE    = 0x14
};

/*
 * This structure contains the QmiVoiceSLQSSUPSServiceReq request parameters.
 */
struct QmiVoiceSLQSSetSUPSServiceReq
{
    voiceSetSUPSServiceReq *pVoiceSetSUPSServiceReq;
};

/*
 * This structure contains the VoiceSLQSSUPSService response parameters.
 */
struct QmiVoiceSLQSSetSUPSServiceResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
    voiceSetSUPSServiceResp *pVoiceSetSUPSServiceResp;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiVoiceSlqsSetSUPSService(
    WORD                   *pMlength,
    BYTE                   *pBuffer,
    voiceSetSUPSServiceReq *pVoiceSetSUPSServiceReq );

extern enum eQCWWANError UpkQmiVoiceSlqsSetSUPSService(
    BYTE                                  *pMdmResp,
    struct QmiVoiceSLQSSetSUPSServiceResp *pApiResp );

#endif /* __VOICE_SLQS_VOICE_SET_SUPS_SERVICE_H__ */

