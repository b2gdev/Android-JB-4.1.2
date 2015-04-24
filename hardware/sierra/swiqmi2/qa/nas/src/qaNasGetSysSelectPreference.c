/*
 * \ingroup : nas
 *
 * \file    : qaNasGetSysSelectPreference.c
 *
 * \brief   : Contains Packing and UnPacking routines for the
 *            eQMI_NAS_GET_SYS_SELECT_PREF message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaQmiBasic.h"

#include "qaNasGetSysSelectPreference.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * Packs the PkQmiNasGetSysSelectPref parameters to the QMI message SDU
 *
 * \param  pParamField[OUT] - Pointer to storage into which the packed
 *                            data will be placed by this function.
 *
 * \param  pMlength[OUT]    - Total length of built message.
 *
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError PkQmiNasGetSysSelectPref(
    WORD                *pMlength,
    BYTE                *pParamField )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    enum eQCWWANError eRCode;

    eRCode = qmbuild( pParamField,
                      NULL,
                      map,
                      eQMI_NAS_GET_SYS_SELECT_PREF,
                      pMlength );

    return eRCode;
}

/*
 * This function unpacks the Emergency Mode TLV from the QMI response
 * message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackTlvEmergencyMode(
    BYTE *pTlvData,
    BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    sysSelectPrefInfo *lResp =
            ((struct QmiNasGetSysSelectPrefResp *)pResp)->pSysSelectPrefInfo;

    if ( NULL == lResp->pEmerMode )
        return eRCode;

     /* Get the Emergency Mode from Emergency Mode TLV */
    eRCode = GetByte( pTlvData, lResp->pEmerMode );
    return eRCode;
}

/*
 * This function unpacks the Mode Preference TLV from the QMI response
 * message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackTlvModePreference(
    BYTE *pTlvData,
    BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    sysSelectPrefInfo *lResp =
            ((struct QmiNasGetSysSelectPrefResp *)pResp)->pSysSelectPrefInfo;

    if ( NULL == lResp->pModePref )
        return eRCode;

    /* Get the Mode Preference from Mode Preference TLV */
    eRCode = GetWord( pTlvData, lResp->pModePref );
    return eRCode;
}

/*
 * This function unpacks the Band Preference TLV from the QMI response
 * message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackTlvBandPreference(
    BYTE *pTlvData,
    BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    sysSelectPrefInfo *lResp =
            ((struct QmiNasGetSysSelectPrefResp *)pResp)->pSysSelectPrefInfo;

    if ( NULL == lResp->pBandPref )
        return eRCode;

    /* Get the band Preference from Band Preference TLV */
    eRCode = GetLongLong ( pTlvData, lResp->pBandPref );
    return eRCode;
}

/*
 * This function unpacks the CDMA PRL Preference TLV from the QMI response
 * message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackTlvCDMAPRLPreference(
    BYTE *pTlvData,
    BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    sysSelectPrefInfo *lResp =
            ((struct QmiNasGetSysSelectPrefResp *)pResp)->pSysSelectPrefInfo;

    if ( NULL == lResp->pPRLPref )
        return eRCode;

    /* Get the CDMA PRL Preference from PRL Preference TLV */
    eRCode = GetWord( pTlvData, lResp->pPRLPref );
    return eRCode;
}

/*
 * This function unpacks the Roaming Preference TLV from the QMI response
 * message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackTlvRoamingPreference(
    BYTE *pTlvData,
    BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    sysSelectPrefInfo *lResp =
            ((struct QmiNasGetSysSelectPrefResp *)pResp)->pSysSelectPrefInfo;

    if ( NULL == lResp->pRoamPref )
        return eRCode;

    /* Get the roaming Preference from roaming Preference TLV */
    eRCode = GetWord( pTlvData, lResp->pRoamPref );
    return eRCode;
}

/*
 * This function unpacks the LTE Band Preference TLV from the QMI response
 * message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackTlvLTEBandPreference(
    BYTE *pTlvData,
    BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    sysSelectPrefInfo *lResp =
            ((struct QmiNasGetSysSelectPrefResp *)pResp)->pSysSelectPrefInfo;

    if ( NULL == lResp->pLTEBandPref )
        return eRCode;

    /* Get the LTE band Preference from LTE Band Preference TLV */
    eRCode = GetLongLong( pTlvData, lResp->pLTEBandPref );
    return eRCode;
}

/*
 * This function unpacks the Network Selection Preference TLV from the
 * QMI response message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackTlvNetSelectPreference(
    BYTE *pTlvData,
    BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    sysSelectPrefInfo *lResp =
            ((struct QmiNasGetSysSelectPrefResp *)pResp)->pSysSelectPrefInfo;

    if ( NULL == lResp->pNetSelPref )
        return eRCode;

    /* Get the Net select Preference from Net select preference TLV */
    eRCode = GetByte( pTlvData, lResp->pNetSelPref );
    return eRCode;
}

/*
 * This function unpacks the Service Domain Preference TLV from the QMI
 * response message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackTlvServDomainPreference(
    BYTE *pTlvData,
    BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    sysSelectPrefInfo *lResp =
            ((struct QmiNasGetSysSelectPrefResp *)pResp)->pSysSelectPrefInfo;

    if ( NULL == lResp->pSrvDomainPref )
        return eRCode;

    /* Get the Service Domain Preference from Service Domain Preference TLV */
    eRCode = GetLong( pTlvData, lResp->pSrvDomainPref );
    return eRCode;
}

/*
 * This function unpacks the GSM/WCDMA Acquisition Order Preference TLV
 * from the QMI response message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackTlvGWACQOrderPreference(
    BYTE *pTlvData,
    BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    sysSelectPrefInfo *lResp =
            ((struct QmiNasGetSysSelectPrefResp *)pResp)->pSysSelectPrefInfo;

    if ( NULL == lResp->pGWAcqOrderPref )
        return eRCode;

    /* Get GSM/WCDMA acquisition order preference from
     * GSM/WCDMA acquisition order preference TLV */
    eRCode = GetLong( pTlvData, lResp->pGWAcqOrderPref );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************

 * This function unpacks the eQMI_NAS_GET_SYS_SELECT_PREF response
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
enum eQCWWANError UpkQmiNasGetSysSelectPref(
    BYTE                              *pMdmResp,
    struct QmiNasGetSysSelectPrefResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,       &qmUnpackTlvResultCode },
        { eTLV_EMER_MODE,         &UnpackTlvEmergencyMode },
        { eTLV_MODE_PREF,         &UnpackTlvModePreference },
        { eTLV_BAND_PREF,         &UnpackTlvBandPreference },
        { eTLV_CDMA_PRL_PREF,     &UnpackTlvCDMAPRLPreference },
        { eTLV_ROAMING_PREF,      &UnpackTlvRoamingPreference },
        { eTLV_LTE_BAND_PREF,     &UnpackTlvLTEBandPreference },
        { eTLV_NET_SELECT_PREF,   &UnpackTlvNetSelectPreference },
        { eTLV_SERV_DOMAIN_PREF,  &UnpackTlvServDomainPreference },
        { eTLV_GW_ACQ_ORDER_PREF, &UnpackTlvGWACQOrderPreference },
        { eTLV_TYPE_INVALID,      NULL } /* Important. Sentinel.
                                          * Signifies last item in map.
                                          */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_NAS_GET_SYS_SELECT_PREF );
    return eRCode;
}

