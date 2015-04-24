/*
 * \ingroup cbk
 *
 * \file    qaCbkNasSystemSelectionPreferenceInd.c
 *
 * \brief   Contains UnPacking routines for the
 *          eQMI_NAS_SYS_SELECT_IND message.
 *
 * Copyright: © 2011-2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCbkNasSystemSelectionPreferenceInd.h"

/* Functions */

/*
 * This function unpacks the Emergency Mode TLV from the QMI indication
 * message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvEmergencyMode(
    BYTE *pTlvData,
    BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    struct EmergencyModeTlv *lEMTlv=
            &((struct QmiCbkNasSystemSelPrefInd *)pResp)->EMTlv;

    /* Get the Emergency Mode from Emergency Mode TLV */
    eRCode = GetByte( pTlvData, &lEMTlv->EmerMode );

    lEMTlv->TlvPresent = TRUE;
    return eRCode;
}

/*
 * This function unpacks the Mode Preference TLV from the QMI indication
 * message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvModePreference(
    BYTE *pTlvData,
    BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    struct ModePreferenceTlv *lMPTlv=
            &((struct QmiCbkNasSystemSelPrefInd *)pResp)->MPTlv;

    /* Get the Mode Preference from Mode Preference TLV */
    eRCode = GetWord( pTlvData, &lMPTlv->ModePref );

    lMPTlv->TlvPresent = TRUE;
    return eRCode;
}

/*
 * This function unpacks the Band Preference TLV from the QMI indication
 * message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvBandPreference(
    BYTE *pTlvData,
    BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    struct BandPreferenceTlv *lBPTlv=
            &((struct QmiCbkNasSystemSelPrefInd *)pResp)->BPTlv;

    /* Get the band Preference from Band Preference TLV */
    eRCode = GetLongLong ( pTlvData, &lBPTlv->band_pref );

    lBPTlv->TlvPresent = TRUE;
    return eRCode;
}

/*
 * This function unpacks the CDMA PRL Preference TLV from the QMI indication
 * message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvCDMAPRLPreference(
    BYTE *pTlvData,
    BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    struct PRLPreferenceTlv *lPPTlv=
            &((struct QmiCbkNasSystemSelPrefInd *)pResp)->PRLPTlv;

    /* Get the CDMA PRL Preference from PRL Preference TLV */
    eRCode = GetWord( pTlvData, &lPPTlv->PRLPref );

    lPPTlv->TlvPresent = TRUE;
    return eRCode;
}

/*
 * This function unpacks the Roaming Preference TLV from the QMI indication
 * message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvRoamingPreference(
    BYTE *pTlvData,
    BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    struct RoamPreferenceTlv *lRPTlv=
            &((struct QmiCbkNasSystemSelPrefInd *)pResp)->RPTlv;

    /* Get the roaming Preference from roaming Preference TLV */
    eRCode = GetWord( pTlvData, &lRPTlv->RoamPref );

    lRPTlv->TlvPresent = TRUE;
    return eRCode;
}

/*
 * This function unpacks the LTE Band Preference TLV from the QMI indication
 * message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvLTEBandPreference(
    BYTE *pTlvData,
    BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    struct LTEBandPreferenceTlv *lBPTlv=
            &((struct QmiCbkNasSystemSelPrefInd *)pResp)->LBPTlv;

    /* Get the LTE band Preference from LTE Band Preference TLV */
    eRCode = GetLongLong( pTlvData, &lBPTlv->LTEBandPref );

    lBPTlv->TlvPresent = TRUE;
    return eRCode;
}

/*
 * This function unpacks the Network Selection Preference TLV from the
 * QMI indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvNetSelectPreference(
    BYTE *pTlvData,
    BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    struct NetSelPreferenceTlv *lNSTlv=
            &((struct QmiCbkNasSystemSelPrefInd *)pResp)->NSPTlv;

    /* Get the Net select Preference from Net select preference TLV */
    eRCode = GetByte( pTlvData, &lNSTlv->NetSelPref );

    lNSTlv->TlvPresent = TRUE;
    return eRCode;
}

/*
 * This function unpacks the Service Domain Preference TLV from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvServDomainPreference(
    BYTE *pTlvData,
    BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    struct ServDomainPrefTlv *lSDTlv=
            &((struct QmiCbkNasSystemSelPrefInd *)pResp)->SDPTlv;

    /* Get the Service Domain Preference from Service Domain Preference TLV */
    eRCode = GetLong( pTlvData, &lSDTlv->SrvDomainPref );

    lSDTlv->TlvPresent = TRUE;
    return eRCode;
}

/*
 * This function unpacks the GSM/WCDMA Acquisition Order Preference TLV
 * from the QMI indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvGWACQOrderPreference(
    BYTE *pTlvData,
    BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    struct GWAcqOrderPrefTlv *lGWAOPTlv=
            &((struct QmiCbkNasSystemSelPrefInd *)pResp)->GWAOPTlv;

    /* Get GSM/WCDMA acquisition order preference from
     * GSM/WCDMA acquisition order preference TLV */
    eRCode = GetLong( pTlvData, &lGWAOPTlv->GWAcqOrderPref );

    lGWAOPTlv->TlvPresent = TRUE;
    return eRCode;
}

/*
 * This function unpacks the System Selection Preference Indication to a
 * user-provided response structure.
 *
 * \param   pMdmResp - Pointer to packed response from the modem.
 *
 * \param   pApiResp - Pointer to storage to unpack into.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UpkQmiCbkNasSystemSelPrefInd(
    BYTE                             *pMdmResp,
    struct QmiCbkNasSystemSelPrefInd *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_EMERGENCY_MODE,          &UnpackCbkTlvEmergencyMode },
        { eTLV_MODE_PREFERENCE,         &UnpackCbkTlvModePreference },
        { eTLV_BAND_PREFERENCE,         &UnpackCbkTlvBandPreference },
        { eTLV_CDMA_PRL_PREFERENCE,     &UnpackCbkTlvCDMAPRLPreference },
        { eTLV_ROAMING_PREFERENCE,      &UnpackCbkTlvRoamingPreference },
        { eTLV_LTE_BAND_PREFERENCE,     &UnpackCbkTlvLTEBandPreference },
        { eTLV_NET_SELECT_PREFERENCE,   &UnpackCbkTlvNetSelectPreference },
        { eTLV_SERV_DOMAIN_PREFERENCE,  &UnpackCbkTlvServDomainPreference },
        { eTLV_GW_ACQ_ORDER_PREFERENCE, &UnpackCbkTlvGWACQOrderPreference },
        { eTLV_TYPE_INVALID,            NULL } /* Important. Sentinel.
                                                * Signifies last item in map.
                                                */
    };
    eRCode = qmunpackresp( pMdmResp,
                           (BYTE*)pApiResp,
                           map,
                           eQMI_NAS_SYS_SELECT_IND );
    return eRCode;
}
