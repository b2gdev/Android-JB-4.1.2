/*
 * \ingroup rms
 *
 * \file    qaRmsGetEventSmsWake.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaRmsGetEventSmsWake.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __RMS_GET_EVENT_SMS_WAKE_H__
#define __RMS_GET_EVENT_SMS_WAKE_H__

/* enum declarations */

/*
 * An enumeration of eQMI_RMS_GET_EVENT_SMS_WAKE request TLV IDs
 *
 */
enum eRMS_GET_EVENT_SMS_WAKE_RESP
{
    eTLV_SMS_WAKE_STATE = 0x10,
    eTLV_SMS_WAKE_MASK  = 0x11
};

/*
 * An enumeration of eQMI_RMS_GET_EVENT_SMS_WAKE response TLV Lengths
 */
enum eRMS_GET_EVENT_SMS_WAKE_RESP_LENGTH
{
    eRMS_GET_EVENT_SMS_WAKE_ENABLE_LENGTH = 0x01,
    eRMS_GET_EVENT_SMS_WAKE_MASK_LENGTH   = 0x04
};

/*
 * This structure contains the GetSMSWake response parameters
 */
struct QmiRmsGetEventSmsWakeResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* GetSMSWake response parameters */
    ULONG *pEnabled;
    ULONG *pWakeMask;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiRmsGetEventSmsWake(
    WORD *pMlength,
    BYTE *pParamField );

extern enum eQCWWANError UpkQmiRmsGetEventSmsWake(
    BYTE                             *pMdmResp,
    struct QmiRmsGetEventSmsWakeResp *pApiResp );

#endif /* __RMS_GET_EVENT_SMS_WAKE_H__ */
