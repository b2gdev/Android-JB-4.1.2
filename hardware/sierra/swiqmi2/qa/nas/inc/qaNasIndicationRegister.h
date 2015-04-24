/*
 * \ingroup nas
 *
 * \file    qaNasIndicationRegister.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaNasIndicationRegister.h.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_INDICATION_REGISTER_H__
#define __NAS_INDICATION_REGISTER_H__

#include "qaGobiApiNas.h"

#define INVAL_PARAM_NOCHANGEIND 0xFF

/* enum declarations */
/*
 *
 * An enumeration of eQMI_NAS_INDICATION_REGISTER request TLV IDs
 *
 */
enum eNAS_NAS_INDICATION_REGISTER_REQ
{
    eTLV_SYSTEM_SELECTION_PREF     = 0x10,
    eTLV_DDTM_EVENTS               = 0x12,
    eTLV_SERVING_SYSTEM_EVENTS     = 0x13,
    eTLV_DUAL_STANDBY_PREF_EVENTS  = 0x14,
    eTLV_SUBSCRIPTION_INFO_EVENTS  = 0x15,
    eTLV_NETWORK_TIME_EVENTS       = 0x17,
    eTLV_SYS_INFO_EVENTS           = 0x18,
    eTLV_SIGNAL_STRENGTH_EVENTS    = 0x19,
    eTLV_ERROR_RATE_EVENTS         = 0x1A,
    eTLV_HDR_NEW_UATI_ASSGN_EVENTS = 0x1B,
    eTLV_HDR_SESSION_CLOSED_EVENTS = 0x1C,
    eTLV_MANAGED_ROAMING_EVENTS    = 0x1D
};

/*
 * This structure contains the NasIndicationRegister response parameters
 *
 */
struct QmiNasIndicationRegisterResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 *
 */
enum eQCWWANError PkQmiNasIndicationRegister(
    WORD                     *pMlength,
    BYTE                     *pBuffer,
    nasIndicationRegisterReq *pIndicationRegisterReq );

enum eQCWWANError UpkQmiNasIndicationRegister(
    BYTE                                *pMdmresp,
    struct QmiNasIndicationRegisterResp *pApiresp);

#endif /* __NAS_INDICATION_REGISTER_H__ */
