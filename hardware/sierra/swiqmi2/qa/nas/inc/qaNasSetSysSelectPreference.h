/*
 * \ingroup nas
 *
 * \file  qaNasSetSysSelectPreference.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaNasSetSysSelectPreference.c
 *
 * Copyright: © 2011-2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_SET_SYS_SELECT_PREF_MSG_H__
#define __NAS_SET_SYS_SELECT_PREF_MSG_H__

#include "qaGobiApiNas.h"

/* enum declarations */
/*
 *
 * An enumeration of eQMI_NAS_SET_SYS_SELECT_PREF request TLV IDs
 *
 */
enum eNAS_SET_SYS_SELECT_PREF_REQ
{
    eTLV_EMERGENCY_MODE          = 0x10,
    eTLV_MODE_PREFERENCE         = 0x11,
    eTLV_BAND_PREFERENCE         = 0x12,
    eTLV_CDMA_PRL_PREFERENCE     = 0x13,
    eTLV_ROAMING_PREFERENCE      = 0x14,
    eTLV_LTE_BAND_PREFERENCE     = 0x15,
    eTLV_NET_SELECT_PREFERENCE   = 0x16,
    eTLV_CHANGE_DURATION         = 0x17,
    eTLV_MNC_PCS_DIGIT_INCLUDE   = 0x1A,
    eTLV_SERV_DOMAIN_PREFERENCE  = 0x1B,
    eTLV_GW_ACQ_ORDER_PREFERENCE = 0x1C,
};

/*
 * This structure contains the Set System Select Preference request parameters.
 *
 * \sa  qaGobiApiNas.h for parameter descriptions
 *
 */
struct QmiNasSetSysSelectPrefReq
{
    sysSelectPrefParams *pSysSelectPrefParams;
};

/*
 * This structure contains the SetSystemSelectPreference response parameters
 *
 */
struct QmiNasSetSysSelectPrefResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 *
 */
enum eQCWWANError PkQmiNasSetSysSelectPref(
    WORD                *pMlength,
    BYTE                *pBuffer,
    sysSelectPrefParams *pSysSelectPrefParams
     );

enum eQCWWANError UpkQmiNasSetSysSelectPref(
    BYTE                              *pMdmresp,
    struct QmiNasSetSysSelectPrefResp *pApiresp);

#endif /* __NAS_SET_SYS_SELECT_PREF_MSG_H__ */
