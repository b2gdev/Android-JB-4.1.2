/*
 * \ingroup nas
 *
 * \file  qaNasGetSysSelectPreference.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaNasGetSysSelectPreference.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_GET_SYS_SELECT_PREF_MSG_H__
#define __NAS_GET_SYS_SELECT_PREF_MSG_H__

#include "qaGobiApiNas.h"

/* enum declarations */
/*
 *
 * An enumeration of eQMI_NAS_GET_SYS_SELECT_PREF response TLV IDs
 *
 */
enum eNAS_GET_SYS_SELECT_PREF_RESP
{
    eTLV_EMER_MODE         = 0x10,
    eTLV_MODE_PREF         = 0x11,
    eTLV_BAND_PREF         = 0x12,
    eTLV_CDMA_PRL_PREF     = 0x13,
    eTLV_ROAMING_PREF      = 0x14,
    eTLV_LTE_BAND_PREF     = 0x15,
    eTLV_NET_SELECT_PREF   = 0x16,
    eTLV_SERV_DOMAIN_PREF  = 0x18,
    eTLV_GW_ACQ_ORDER_PREF = 0x19,
};


/*
 * This structure contains the GetSysSelectPreferences response parameters.
 *
 * \sa  qaGobiApiNas.h for parameter descriptions
 *
 */
struct QmiNasGetSysSelectPrefResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* Response parameters */
    sysSelectPrefInfo  *pSysSelectPrefInfo;
};

/*
 * Prototypes
 *
 */
enum eQCWWANError PkQmiNasGetSysSelectPref(
    WORD *pMlength,
    BYTE *pBuffer
     );

enum eQCWWANError UpkQmiNasGetSysSelectPref(
    BYTE                              *pMdmresp,
    struct QmiNasGetSysSelectPrefResp *pApiresp);

#endif /* __NAS_GET_SYS_SELECT_PREF_MSG_H__ */
