 /**
 * \ingroup cbk
 *
 * \file    qaCbkVoiceIndicationRegister.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaCbkVoiceIndicationRegister.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_INDICATION_REGISTER_H__
#define __VOICE_INDICATION_REGISTER_H__

#include "qaQmiNotify.h"

/**
 *
 * An enumeration of eQMI_VOICE_INDICATION_REGISTER request TLV IDs
 */
enum eVOICE_INDICATION_REGISTER_REQ
{
    eTLV_DTMF_EVENTS              = 0x10,
    eTLV_VOICE_PRIVACY_EVENTS     = 0x11,
    eTLV_SUPS_NOTIFICATION_EVENTS = 0x12,
};

/*
 * This structure contains the Voice Notification request parameters
 *
 * \param  eDTMFEvents[IN]
 *         - Subscribe to DTMF events
 *
 * \param  eVoicePrivacyEvents[IN]
 *         - Subscribe to Voice privacy events
 *
 * \param  eSUPSNotificationEvents[IN]
 *         - Subscribe to supplementary service notification events
 */
struct QmiCbkVoiceIndicationRegisterReq
{
    enum eQmiCbkSetStatus eDTMFEvents;
    enum eQmiCbkSetStatus eVoicePrivacyEvents;
    enum eQmiCbkSetStatus eSUPSNotificationEvents;
};

/*
 * This structure contains the VOICE_INDICATION_REGISTER_RESP parameters
 */
struct QmiCbkVoiceIndicationRegisterResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiCbkVoiceIndicationRegister(
    WORD                   *pMlength,
    enum eQmiCbkSetStatus  eDTMFEvents,
    enum eQmiCbkSetStatus  eVoicePrivacyEvents,
    enum eQmiCbkSetStatus  eSUPSNotificationEvents,
    BYTE                   *pParamField );

extern enum eQCWWANError UpkQmiCbkVoiceIndicationRegister(
    BYTE    *pMdmResp,
    struct  QmiCbkVoiceIndicationRegisterResp *pApiResp);

#endif /* __VOICE_INDICATION_REGISTER_H__ */
