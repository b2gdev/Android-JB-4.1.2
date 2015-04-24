/*
 * \ingroup cbk
 *
 * \file    qaCbkNasSystemSelectionPreferenceInd.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaCbkNasSystemSelectionPreferenceInd.c
 *
 * Copyright: © 2011-2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_SYSTEM_SELECTION_PREFERENCE_IND_H__
#define __NAS_SYSTEM_SELECTION_PREFERENCE_IND_H__
#include "qaGobiApiCbk.h"

/*
 * An enumeration of eQMI_NAS_SYSTEM_SELECTION_PREFERENCE_IND
 * response TLV IDs
 *
 */
enum eQMI_NAS_SYSTEM_SELECTION_PREFERENCE_IND
{
    eTLV_EMERGENCY_MODE          = 0x10,
    eTLV_MODE_PREFERENCE         = 0x11,
    eTLV_BAND_PREFERENCE         = 0x12,
    eTLV_CDMA_PRL_PREFERENCE     = 0x13,
    eTLV_ROAMING_PREFERENCE      = 0x14,
    eTLV_LTE_BAND_PREFERENCE     = 0x15,
    eTLV_NET_SELECT_PREFERENCE   = 0x16,
    eTLV_SERV_DOMAIN_PREFERENCE  = 0x18,
    eTLV_GW_ACQ_ORDER_PREFERENCE = 0x19,
};

/*
 * eQMI_NAS_SYSTEM_SELECTION_PREFERENCE_IND TLVs defined below
 */

/*
 * Name:    EmergencyModeTlv
 *
 * Purpose: Structure used to store Emergency Mode TLV Value.
 *
 * Members: TlvPresent - Boolean indicating the presence of the TLV
 *                       in the QMI response
 *
 *          EmerMode   - Emergency Mode Value
 */
struct EmergencyModeTlv
{
    BYTE TlvPresent;
    BYTE EmerMode;
};

/*
 * Name:    ModePreferenceTlv
 *
 * Purpose: Structure used to store Mode Preference TLV Value.
 *
 * Members: TlvPresent - Boolean indicating the presence of the TLV
 *                       in the QMI response
 *
 *          ModePref   - Mode Preference Value
 */
struct ModePreferenceTlv
{
    BYTE TlvPresent;
    WORD ModePref;
};

/*
 * Name:    BandPreferenceTlv
 *
 * Purpose: Structure used to store all Band Preference TLV Value.
 *
 * Members: TlvPresent - Boolean indicating the presence of the TLV
 *                       in the QMI response
 *
 *          band_pref  - Roaming Indication Value
 */
struct BandPreferenceTlv
{
    BYTE      TlvPresent;
    ULONGLONG band_pref;
};

/*
 * Name:    PRLPreferenceTlv
 *
 * Purpose: Structure used to store CDMA PRL Preference TLV Value.
 *
 * Members: TlvPresent - Boolean indicating the presence of the TLV
 *                       in the QMI response
 *
 *          PRLPref    - CDMA PRL Preference Value
 */
struct PRLPreferenceTlv
{
    BYTE TlvPresent;
    WORD PRLPref;
};

/*
 * Name:    RoamPreferenceTlv
 *
 * Purpose: Structure used to store Roaming Preference TLV Value.
 *
 * Members: TlvPresent - Boolean indicating the presence of the TLV
 *                       in the QMI response
 *
 *          RoamPref   - Roaming Preference Value
 */
struct RoamPreferenceTlv
{
    BYTE TlvPresent;
    WORD RoamPref;
};

/*
 * Name:    LTEBandPreferenceTlv
 *
 * Purpose: Structure used to store LTE Band Preference TLV Value.
 *
 * Members: TlvPresent  - Boolean indicating the presence of the TLV
 *                       in the QMI response
 *
 *          LTEBandPref - LTE Band Preference Value
 */
struct LTEBandPreferenceTlv
{
    BYTE      TlvPresent;
    ULONGLONG LTEBandPref;
};

/*
 * Name:    NetSelPreferenceTlv
 *
 * Purpose: Structure used to store Network Selection Preference TLV Value.
 *
 * Members: TlvPresent - Boolean indicating the presence of the TLV
 *                       in the QMI response
 *
 *          NetSelPref - Network Selection Preference Value
 */
struct NetSelPreferenceTlv
{
    BYTE TlvPresent;
    BYTE NetSelPref;
};

/*
 * Name:    ServDomainPrefTlv
 *
 * Purpose: Structure used to store Service domain preference TLV Value.
 *
 * Members: TlvPresent    - Boolean indicating the presence of the TLV
 *                          in the QMI response
 *
 *          SrvDomainPref - Service Domain Preference Value
 */
struct ServDomainPrefTlv
{
    BYTE TlvPresent;
    ULONG SrvDomainPref;
};

/*
 * Name:    GWAcqOrderPrefTlv
 *
 * Purpose: Structure used to store GSM/WCDMA acquisition order preference
 *          TLV Value.
 *
 * Members: TlvPresent     - Boolean indicating the presence of the TLV
 *                          in the QMI response
 *
 *          GWAcqOrderPref - GSM/WCDMA acquisition order Preference Value
 */
struct GWAcqOrderPrefTlv
{
    BYTE TlvPresent;
    ULONG GWAcqOrderPref;
};

/*
 * Name:    QmiCbkNasSystemSelPrefInd
 *
 * Purpose: Structure used to store all QMI Notification parameters.
 *
 * Members: EMTlv    - Emergency mode Tlv
 *          MPTlv    - Mode preference Tlv
 *          BPTlv    - Band preference Tlv
 *          PRLPTlv  - PRL preference Tlv
 *          RPTlv    - Roaming preference Tlv
 *          LBPTlv   - LTE band preference Tlv
 *          NSPTlv   - Network selection preference Tlv
 *          SDPTlv   - Service domain preference Tlv
 *          GWAOPTlv - GSM WCDMA Acquisition Order Preference Tlv
 *
 * Note:    None
 */
struct QmiCbkNasSystemSelPrefInd{
    struct EmergencyModeTlv     EMTlv;
    struct ModePreferenceTlv    MPTlv;
    struct BandPreferenceTlv    BPTlv;
    struct PRLPreferenceTlv     PRLPTlv;
    struct RoamPreferenceTlv    RPTlv;
    struct LTEBandPreferenceTlv LBPTlv;
    struct NetSelPreferenceTlv  NSPTlv;
    struct ServDomainPrefTlv    SDPTlv;
    struct GWAcqOrderPrefTlv    GWAOPTlv;

};

/*************
 * Prototypes
 **************/
enum eQCWWANError UpkQmiCbkNasSystemSelPrefInd (
    BYTE                             *pMdmResp,
    struct QmiCbkNasSystemSelPrefInd *pApiResp );

#endif /* __NAS_SYSTEM_SELECTION_PREFERENCE_IND_H__ */

