/*
 * \ingroup cbk
 *
 * \file    qaCbkVoiceSUPSNotificationInd.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaCbkVoiceSUPSNotificationInd.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __VOICE_SUPS_NOTIFICATION_IND_H_
#define __VOICE_SUPS_NOTIFICATION_IND_H_

#include "qaGobiApiCbk.h"

/*
 * An enumeration of eQMI_VOICE_SUPS_NOTIFICATION_IND response TLV IDs
 */
enum eQMI_VOICE_SUPS_NOTIFICATION_IND
{
    eTLV_SUPS_NOTIFICATION_INFO = 0x01,
    eTLV_SUPS_CUG_INDEX         = 0x10,
    eTLV_SUPS_ECT_NUMBER        = 0x11,
};

/*
 * Prototypes
 */
enum eQCWWANError UpkQmiCbkVoiceSUPSNotificationInd(
    BYTE                  *pMdmResp,
    voiceSUPSNotification *pApiResp );

#endif /* __VOICE_SUPS_NOTIFICATION_IND_H_ */
