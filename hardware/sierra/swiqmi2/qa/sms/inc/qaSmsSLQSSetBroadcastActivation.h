/*
 * \ingroup sms
 *
 * \file    qaSmsSLQSSetBroadcastActivation.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaSmsSLQSSetBroadcastActivation.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __SMS_SET_BROADCAST_ACTIVATION_H__
#define __SMS_SET_BROADCAST_ACTIVATION_H__

/* enum declarations */

/*
 * An enumeration of eQMI_SMS_SET_BROADCAST_ACTIVATION request TLV IDs
 *
 */
enum eSMS_SET_BROADCAST_ACTIVATION_REQ
{
    eTLV_BC_ACTIVATION_INFO = 0x01
};

/*
 * This structure contains the SetBroadcastActivation request parameters
 */
struct QmiSmsSetBroadcastActivationReq
{
    BYTE mode;
    BYTE bc_activate;
};

/*
 * This structure contains the SetBroadcastActivation response parameters
 */
struct QmiSmsSetBroadcastActivationResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiSmsSetBroadcastActivation(
    WORD *pMlength,
    BYTE *pParamField,
    BYTE mode,
    BYTE bc_activate );

extern enum eQCWWANError UpkQmiSmsSetBroadcastActivation(
    BYTE                                    *pMdmResp,
    struct QmiSmsSetBroadcastActivationResp *pApiResp );

#endif /* __SMS_SET_BROADCAST_ACTIVATION_H__ */
