/*
 * \ingroup : nas
 *
 * \file    : qaNasSetSysSelectPreference.c
 *
 * \brief   : Contains Packing and UnPacking routines for the
 *            eQMI_NAS_SET_SYS_SELECT_PREF message.
 *
 * Copyright: © 2011-2012 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaQmiBasic.h"

#include "qaNasSetSysSelectPreference.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the system selection preference Emergency Mode field
 * to the QMI message SDU
 *
 * \param pBuf   - Pointer to storage into which the packed
 *                 data will be placed by this function.
 *
 * \param pParam - Pointer to structure containing data for this TLV.
 *
 */
enum eQCWWANError BuildTlvEmergencyMode( BYTE *pBuf, BYTE *pParam )
{
    struct QmiNasSetSysSelectPrefReq *pReq =
        (struct QmiNasSetSysSelectPrefReq *)pParam;

    sysSelectPrefParams *lReq = pReq->pSysSelectPrefParams;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if( NULL == lReq->pEmerMode )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Emergency Mode */
    eRCode = PutByte( pBuf, *(lReq->pEmerMode) );
    return eRCode;
}

/*
 * This function packs the system selection preference Mode Preference field
 * to the QMI message SDU
 *
 * \param pBuf   - Pointer to storage into which the packed
 *                 data will be placed by this function.
 *
 * \param pParam - Pointer to structure containing data for this TLV.
 *
 */
enum eQCWWANError BuildTlvModePreference( BYTE *pBuf, BYTE *pParam )
{
    struct QmiNasSetSysSelectPrefReq *pReq =
        (struct QmiNasSetSysSelectPrefReq *)pParam;

    sysSelectPrefParams *lReq = pReq->pSysSelectPrefParams;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if( NULL == lReq->pModePref )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Mode Preference */
    eRCode = PutWord( pBuf, *(lReq->pModePref) );
    return eRCode;
}

/*
 * This function packs the system selection preference band preference field
 * to the QMI message SDU
 *
 * \param pBuf   - Pointer to storage into which the packed
 *                 data will be placed by this function.
 *
 * \param pParam - Pointer to structure containing data for this TLV.
 *
 */
enum eQCWWANError BuildTlvSetBandPreference( BYTE *pBuf, BYTE *pParam )
{
    struct QmiNasSetSysSelectPrefReq *pReq =
        (struct QmiNasSetSysSelectPrefReq *)pParam;

    sysSelectPrefParams *lReq = pReq->pSysSelectPrefParams;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if( NULL == lReq->pBandPref )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Band Preference */
    eRCode = Put64( pBuf, *(lReq->pBandPref) );
    return eRCode;
}

/*
 * This function packs the system selection preference CDMA PRL Preference
 * field to the QMI message SDU
 *
 * \param pBuf   - Pointer to storage into which the packed
 *                 data will be placed by this function.
 *
 * \param pParam - Pointer to structure containing data for this TLV.
 *
 */
enum eQCWWANError BuildTlvCDMAPRLPreference( BYTE *pBuf, BYTE *pParam )
{
    struct QmiNasSetSysSelectPrefReq *pReq =
        (struct QmiNasSetSysSelectPrefReq *)pParam;

    sysSelectPrefParams *lReq = pReq->pSysSelectPrefParams;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if( NULL == lReq->pPRLPref )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert PRL Preference */
    eRCode = PutWord( pBuf, *(lReq->pPRLPref) );
    return eRCode;
}

/*
 * This function packs the system selection preference Roaming Preference
 * field to the QMI message SDU
 *
 * \param pBuf   - Pointer to storage into which the packed
 *                 data will be placed by this function.
 *
 * \param pParam - Pointer to structure containing data for this TLV.
 *
 */
enum eQCWWANError BuildTlvRoamingPreference( BYTE *pBuf, BYTE *pParam )
{
    struct QmiNasSetSysSelectPrefReq *pReq =
        (struct QmiNasSetSysSelectPrefReq *)pParam;

    sysSelectPrefParams *lReq = pReq->pSysSelectPrefParams;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if( NULL == lReq->pRoamPref )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Roaming Preference */
    eRCode = PutWord( pBuf, *(lReq->pRoamPref) );
    return eRCode;
}

/*
 * This function packs the system selection preference LTE Band Preference
 * field to the QMI message SDU
 *
 * \param pBuf   - Pointer to storage into which the packed
 *                 data will be placed by this function.
 *
 * \param pParam - Pointer to structure containing data for this TLV.
 *
 */
enum eQCWWANError BuildTlvLTEBandPreference( BYTE *pBuf, BYTE *pParam )
{
    struct QmiNasSetSysSelectPrefReq *pReq =
        (struct QmiNasSetSysSelectPrefReq *)pParam;

    sysSelectPrefParams *lReq = pReq->pSysSelectPrefParams;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if( NULL == lReq->pLTEBandPref )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert LTE Band Preference */
    eRCode = Put64( pBuf, *(lReq->pLTEBandPref) );
    return eRCode;
}

/*
 * This function packs the system selection preference Network Selection
 * Preference field to the QMI message SDU
 *
 * \param pBuf   - Pointer to storage into which the packed
 *                 data will be placed by this function.
 *
 * \param pParam - Pointer to structure containing data for this TLV.
 *
 */
enum eQCWWANError BuildTlvNetSelectPreference( BYTE *pBuf, BYTE *pParam )
{
    struct QmiNasSetSysSelectPrefReq *pReq =
        (struct QmiNasSetSysSelectPrefReq *)pParam;

    sysSelectPrefParams     *lReq       = pReq->pSysSelectPrefParams;
    struct netSelectionPref *pNetSelPref = lReq->pNetSelPref;

    enum   eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if( NULL == pNetSelPref )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Network Selection Preference parameter - Network Registration */
    eRCode = PutByte( pBuf, pNetSelPref->netReg );
    if( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* Insert Network Selection Preference parameter - MCC */
    eRCode = PutWord( pBuf, pNetSelPref->mcc);
    if( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* Insert Network Selection Preference parameter - MNC */
    eRCode = PutWord( pBuf, pNetSelPref->mnc);
    return eRCode;
}

/*
 * This function packs the system selection preference Change Duration field
 * to the QMI message SDU
 *
 * \param pBuf   - Pointer to storage into which the packed
 *                 data will be placed by this function.
 *
 * \param pParam - Pointer to structure containing data for this TLV.
 *
 */
enum eQCWWANError BuildTlvChangeDuration( BYTE *pBuf, BYTE *pParam )
{
    struct QmiNasSetSysSelectPrefReq *pReq =
        (struct QmiNasSetSysSelectPrefReq *)pParam;

    sysSelectPrefParams *lReq  = pReq->pSysSelectPrefParams;
    enum   eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if( NULL == lReq->pChgDuration )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Change Duration */
    eRCode = PutByte( pBuf, *(lReq->pChgDuration) );
    return eRCode;
}

/*
 * This function packs the system selection preference MNC PCS Digit Include
 * status to the QMI message SDU
 *
 * \param pBuf   - Pointer to storage into which the packed
 *                 data will be placed by this function.
 *
 * \param pParam - Pointer to structure containing data for this TLV.
 *
 */
enum eQCWWANError BuildTlvMNCPCSDigitIncludeStatus( BYTE *pBuf, BYTE *pParam )
{
    struct QmiNasSetSysSelectPrefReq *pReq =
        (struct QmiNasSetSysSelectPrefReq *)pParam;

    sysSelectPrefParams *lReq  = pReq->pSysSelectPrefParams;
    enum   eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if( NULL == lReq->pMNCIncPCSDigStat )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert MNC Include PCS Digit Status */
    eRCode = PutByte( pBuf, *(lReq->pMNCIncPCSDigStat) );
    return eRCode;
}

/*
 * This function packs the system selection preference Service Domain
 * preference to the QMI message SDU
 *
 * \param pBuf   - Pointer to storage into which the packed
 *                 data will be placed by this function.
 *
 * \param pParam - Pointer to structure containing data for this TLV.
 *
 */
enum eQCWWANError BuildTlvServDomainPreference( BYTE *pBuf, BYTE *pParam )
{
    struct QmiNasSetSysSelectPrefReq *pReq =
        (struct QmiNasSetSysSelectPrefReq *)pParam;

    sysSelectPrefParams *lReq  = pReq->pSysSelectPrefParams;
    enum   eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if( NULL == lReq->pSrvDomainPref )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Service Domain Preference */
    eRCode = PutLong( pBuf, *(lReq->pSrvDomainPref) );
    return eRCode;
}

/*
 * This function packs the system selection preference GSM WCDMA
 * acquisition preference to the QMI message SDU
 *
 * \param pBuf   - Pointer to storage into which the packed
 *                 data will be placed by this function.
 *
 * \param pParam - Pointer to structure containing data for this TLV.
 *
 */
enum eQCWWANError BuildTlvGWACQOrderPreference( BYTE *pBuf, BYTE *pParam )
{
    struct QmiNasSetSysSelectPrefReq *pReq =
        (struct QmiNasSetSysSelectPrefReq *)pParam;

    sysSelectPrefParams *lReq  = pReq->pSysSelectPrefParams;
    enum   eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if( NULL == lReq->pGWAcqOrderPref )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert GSM/WCDMA Acquisition Order Preference */
    eRCode = PutLong( pBuf, *(lReq->pGWAcqOrderPref) );
    return eRCode;
}

/*
 * Packs the PkQmiNasSetSysSelectPref parameters to the QMI message SDU
 *
 * \param  pParamField[OUT]         - Pointer to storage into which the packed
 *                                    data will be placed by this function.
 *
 * \param  pMlength[OUT]            - Total length of built message.
 *
 * \param  pSysSelectPrefParams[IN] - System Selection Preferences
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError PkQmiNasSetSysSelectPref(
    WORD                *pMlength,
    BYTE                *pParamField,
    sysSelectPrefParams *pSysSelectPrefParams )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_EMERGENCY_MODE,          &BuildTlvEmergencyMode },
        { eTLV_MODE_PREFERENCE,         &BuildTlvModePreference },
        { eTLV_BAND_PREFERENCE,         &BuildTlvSetBandPreference },
        { eTLV_CDMA_PRL_PREFERENCE,     &BuildTlvCDMAPRLPreference },
        { eTLV_ROAMING_PREFERENCE,      &BuildTlvRoamingPreference },
        { eTLV_LTE_BAND_PREFERENCE,     &BuildTlvLTEBandPreference },
        { eTLV_NET_SELECT_PREFERENCE,   &BuildTlvNetSelectPreference },
        { eTLV_CHANGE_DURATION,         &BuildTlvChangeDuration},
        { eTLV_MNC_PCS_DIGIT_INCLUDE,   &BuildTlvMNCPCSDigitIncludeStatus },
        { eTLV_SERV_DOMAIN_PREFERENCE,  &BuildTlvServDomainPreference },
        { eTLV_GW_ACQ_ORDER_PREFERENCE, &BuildTlvGWACQOrderPreference },
        { eTLV_TYPE_INVALID,            NULL }  /* Important. Sentinel.
                                                 * Signifies last item in map.
                                                 */
    };

    struct QmiNasSetSysSelectPrefReq req;

    enum eQCWWANError eRCode;

    req.pSysSelectPrefParams = pSysSelectPrefParams;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_NAS_SET_SYS_SELECT_PREF,
                      pMlength );

    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************

 * This function unpacks the eQMI_NAS_SET_SYS_SELECT_PREF response
 * message to a user-provided response structure.
 *
 * \param  pMdmResp  [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp  [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE      - on success
 * \return eQCWWAN_ERR_NO_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError UpkQmiNasSetSysSelectPref(
    BYTE                              *pMdmResp,
    struct QmiNasSetSysSelectPrefResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_NAS_SET_SYS_SELECT_PREF );
    return eRCode;
}

