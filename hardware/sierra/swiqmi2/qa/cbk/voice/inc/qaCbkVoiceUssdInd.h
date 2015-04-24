/*
 * \ingroup cbk
 *
 * \file    qaCbkVoiceUssdInd.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaCbkVoiceUssdInd.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_USSD_IND_H_
#define __VOICE_USSD_IND_H_

#include "qaGobiApiVoice.h"

/*
 * An enumeration of eQMI_VOICE_USSD_IND response TLV IDs
 */
enum eQMI_VOICE_USSD_IND
{
    eTLV_USSD_NOTIFICATION_TYPE = 0x01,
    eTLV_USSD_DATA_NETWORK      = 0x10
};

/*
 * eQMI_VOICE_USSD_IND TLVs defined below
 */

/*
 * Name:    VoiceUssdTlv
 *
 * Purpose: Structure used to store all VoiceUssdTlv TLV Value
 *
 * Members: TlvPresent         - Boolean indicating the presence of the TLV
 *                               in the QMI response
 *
 *          Notification_Type  - 0x01 - No action required
 *                               0x02 - Action required
 *
 *          NetworkInfo        - USSD information from the network
 *                               See qaGobiApiVoice.h for more info.
 *
 * Note:    None
 */
struct VoiceUssdTlv
{
    BYTE           TlvPresent;
    ULONG          Notification_Type;
    struct USSInfo NetworkInfo;
};

/*
 * Name:    QmiCbkVoiceUssdInd
 *
 * Purpose: Structure used to store all Notification Parameters.
 *
 * Members: VSTlv - USSD Notification Tlv
 *
 * Note:    None
 */
struct QmiCbkVoiceUssdInd{
    struct VoiceUssdTlv VSTlv;
};

/*
 * Prototypes
 */
enum eQCWWANError UpkQmiCbkVoiceUssdInd(
    BYTE                      *pMdmResp,
    struct QmiCbkVoiceUssdInd *pApiResp );

#endif /* __VOICE_USSD_IND_H_ */
