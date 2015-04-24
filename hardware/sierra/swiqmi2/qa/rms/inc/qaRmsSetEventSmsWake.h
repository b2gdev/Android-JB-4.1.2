/*
 * \ingroup rms
 *
 * \file    qaRmsSetEventSmsWake.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaRmsSetEventSmsWake.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __RMS_SET_EVENT_SMS_WAKE_H__
#define __RMS_SET_EVENT_SMS_WAKE_H__

/* enum declarations */

/*
 * An enumeration of eQMI_RMS_SET_EVENT_SMS_WAKE request TLV IDs
 *
 */
enum eRMS_SET_EVENT_SMS_WAKE_REQ
{
    eTLV_SMS_SET_WAKE_STATE = 0x10,
    eTLV_SMS_SET_WAKE_MASK  = 0x11
};

/*
 * This structure contains the SetSmsWake request parameters
 */
struct QmiRmsSetEventSmsWakeReq
{
   ULONG bEnable;
   ULONG wakeMask;
};

/*
 * This structure contains the SetSmsWake response parameters
 */
struct QmiRmsSetEventSmsWakeResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiRmsSetEventSmsWake(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG bEnable,
    ULONG wakeMask );

extern enum eQCWWANError UpkQmiRmsSetEventSmsWake(
    BYTE                             *pMdmResp,
    struct QmiRmsSetEventSmsWakeResp *pApiResp );

#endif /* __RMS_SET_EVENT_SMS_WAKE_H__ */
