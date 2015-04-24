/*
 * \ingroup : nas
 *
 * \file    : qaNasIndicationRegister.c
 *
 * \brief   : Contains Packing and UnPacking routines for the
 *            eQMI_NAS_INDICATION_REGISTER message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaNasIndicationRegister.h"

/******************************************************************************
 * Request handling
 *****************************************************************************/
/*
 * Packs the System Selection Preference field to the QMI message
 *
 * \param  pBuf   - Pointer to storage into which the packed
 *                  data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 */
enum eQCWWANError BuildTlvSystemSelectionPreference( BYTE *pBuf, BYTE *pParam )
{
    nasIndicationRegisterReq *pReq = ( nasIndicationRegisterReq *)pParam;

    if ( NULL == pReq->pSystemSelectionInd )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Keep compatibility with old implementation */
    if( INVAL_PARAM_NOCHANGEIND == *(pReq->pSystemSelectionInd) )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert system selection indication event */
    return PutByte( pBuf, *(pReq->pSystemSelectionInd) );
}

/*
 * Packs the DDTM events field to the QMI message
 *
 * \param  pBuf   - Pointer to storage into which the packed
 *                  data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 */
enum eQCWWANError BuildTlvDDTMEvents( BYTE *pBuf, BYTE *pParam )
{
    nasIndicationRegisterReq *pReq = ( nasIndicationRegisterReq *)pParam;

    if ( NULL == pReq->pDDTMInd )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Keep compatibility with old implementation */
    if( INVAL_PARAM_NOCHANGEIND == *(pReq->pSystemSelectionInd) )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert DDTM events */
    return PutByte( pBuf, *(pReq->pDDTMInd) );
}

/*
 * Packs the serving system events field to the QMI message
 *
 * \param  pBuf   - Pointer to storage into which the packed
 *                  data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 */
enum eQCWWANError BuildTlvServingSystemEvents( BYTE *pBuf, BYTE *pParam )
{
    nasIndicationRegisterReq *pReq = ( nasIndicationRegisterReq *)pParam;

    if ( NULL == pReq->pServingSystemInd )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Keep compatibility with old implementation */
    if( INVAL_PARAM_NOCHANGEIND == *(pReq->pSystemSelectionInd) )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert serving system events */
    return PutByte( pBuf, *(pReq->pServingSystemInd) );
}

/*
 * Packs the Dual Standby Preference events field to the QMI message
 *
 * \param  pBuf   - Pointer to storage into which the packed
 *                  data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 */
enum eQCWWANError BuildTlvDualStandByPrefEvents( BYTE *pBuf, BYTE *pParam )
{
    nasIndicationRegisterReq *pReq = ( nasIndicationRegisterReq *)pParam;

    if ( NULL == pReq->pDualStandByPrefInd )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Dual Standby Preference */
    return PutByte( pBuf, *(pReq->pDualStandByPrefInd) );
}

/*
 * Packs the Subscription Information Events field to the QMI message
 *
 * \param  pBuf   - Pointer to storage into which the packed
 *                  data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 */
enum eQCWWANError BuildTlvSubsInfoEvents( BYTE *pBuf, BYTE *pParam )
{
    nasIndicationRegisterReq *pReq = ( nasIndicationRegisterReq *)pParam;

    if ( NULL == pReq->pSubscriptionInfoInd )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Subscription Information Events */
    return PutByte( pBuf, *(pReq->pSubscriptionInfoInd) );
}

/*
 * Packs the Network Time Events field to the QMI message
 *
 * \param  pBuf   - Pointer to storage into which the packed
 *                  data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 */
enum eQCWWANError BuildTlvNetworkTimeEvents( BYTE *pBuf, BYTE *pParam )
{
    nasIndicationRegisterReq *pReq = ( nasIndicationRegisterReq *)pParam;

    if ( NULL == pReq->pNetworkTimeInd )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Network Time Events */
    return PutByte( pBuf, *(pReq->pNetworkTimeInd) );
}

/*
 * Packs the System Info Events field to the QMI message
 *
 * \param  pBuf   - Pointer to storage into which the packed
 *                  data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 */
enum eQCWWANError BuildTlvSysInfoEvents( BYTE *pBuf, BYTE *pParam )
{
    nasIndicationRegisterReq *pReq = ( nasIndicationRegisterReq *)pParam;

    if ( NULL == pReq->pSysInfoInd )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert System Info Events */
    return PutByte( pBuf, *(pReq->pSysInfoInd) );
}

/*
 * Packs the Signal Strength Events field to the QMI message
 *
 * \param  pBuf   - Pointer to storage into which the packed
 *                  data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 */
enum eQCWWANError BuildTlvSignalStrengthEvents( BYTE *pBuf, BYTE *pParam )
{
    nasIndicationRegisterReq *pReq = ( nasIndicationRegisterReq *)pParam;

    if ( NULL == pReq->pSignalStrengthInd )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Signal Strength Events */
    return PutByte( pBuf, *(pReq->pSignalStrengthInd) );
}

/*
 * Packs the Error Rate Events field to the QMI message
 *
 * \param  pBuf   - Pointer to storage into which the packed
 *                  data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 */
enum eQCWWANError BuildTlvErrorRateEvents( BYTE *pBuf, BYTE *pParam )
{
    nasIndicationRegisterReq *pReq = ( nasIndicationRegisterReq *)pParam;

    if ( NULL == pReq->pErrorRateInd )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Error Rate Events */
    return PutByte( pBuf, *(pReq->pErrorRateInd) );
}

/*
 * Packs the HDR New UATI Assignment Events field to the QMI message
 *
 * \param  pBuf   - Pointer to storage into which the packed
 *                  data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 */
enum eQCWWANError BuildTlvHDRNewUATIAssgnEvents( BYTE *pBuf, BYTE *pParam )
{
    nasIndicationRegisterReq *pReq = ( nasIndicationRegisterReq *)pParam;

    if ( NULL == pReq->pHDRNewUATIAssInd )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert HDR New UATI Assignment Events */
    return PutByte( pBuf, *(pReq->pHDRNewUATIAssInd) );
}

/*
 * Packs the HDR Session Closed Events field to the QMI message
 *
 * \param  pBuf   - Pointer to storage into which the packed
 *                  data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 */
enum eQCWWANError BuildTlvHDRSessionClosedEvents( BYTE *pBuf, BYTE *pParam )
{
    nasIndicationRegisterReq *pReq = ( nasIndicationRegisterReq *)pParam;

    if ( NULL == pReq->pHDRSessionCloseInd )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert HDR Session Closed Events */
    return PutByte( pBuf, *(pReq->pHDRSessionCloseInd) );
}

/*
 * Packs the Managed Roaming Events field to the QMI message
 *
 * \param  pBuf   - Pointer to storage into which the packed
 *                  data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 */
enum eQCWWANError BuildTlvManagedRoamingEvents( BYTE *pBuf, BYTE *pParam )
{
    nasIndicationRegisterReq *pReq = ( nasIndicationRegisterReq *)pParam;

    if ( NULL == pReq->pManagedRoamingInd )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Managed Roaming Events */
    return PutByte( pBuf, *(pReq->pManagedRoamingInd) );
}

/*
 * Packs the Nas Indication register parameters to the QMI message SDU
 *
 * \param   pParamField[OUT]   - Pointer to storage into which the packed
 *                               data will be placed by this function.
 *
 * \param   pMlength[OUT]      - Total length of built message.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError PkQmiNasIndicationRegister(
    WORD                     *pMlength,
    BYTE                     *pParamField,
    nasIndicationRegisterReq *pIndicationRegisterReq )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SYSTEM_SELECTION_PREF,     &BuildTlvSystemSelectionPreference },
        { eTLV_DDTM_EVENTS,               &BuildTlvDDTMEvents },
        { eTLV_SERVING_SYSTEM_EVENTS,     &BuildTlvServingSystemEvents },
        { eTLV_DUAL_STANDBY_PREF_EVENTS,  &BuildTlvDualStandByPrefEvents },
        { eTLV_SUBSCRIPTION_INFO_EVENTS,  &BuildTlvSubsInfoEvents },
        { eTLV_NETWORK_TIME_EVENTS,       &BuildTlvNetworkTimeEvents },
        { eTLV_SYS_INFO_EVENTS,           &BuildTlvSysInfoEvents },
        { eTLV_SIGNAL_STRENGTH_EVENTS,    &BuildTlvSignalStrengthEvents },
        { eTLV_ERROR_RATE_EVENTS,         &BuildTlvErrorRateEvents },
        { eTLV_HDR_NEW_UATI_ASSGN_EVENTS, &BuildTlvHDRNewUATIAssgnEvents },
        { eTLV_HDR_SESSION_CLOSED_EVENTS, &BuildTlvHDRSessionClosedEvents },
        { eTLV_MANAGED_ROAMING_EVENTS,    &BuildTlvManagedRoamingEvents },
        { eTLV_TYPE_INVALID,              NULL } /* Important. Sentinel.
                                                  * Signifies last item in map.
                                                  */
    };

    enum eQCWWANError eRCode;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pIndicationRegisterReq,
                      map,
                      eQMI_NAS_SET_REG_EVENT,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 *****************************************************************************/
/*
 * This function unpacks the eQMI_NAS_INDICATION_REGISTER response
 * message to a user-provided response structure.
 *
 * \param  pMdmResp  [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp  [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError UpkQmiNasIndicationRegister(
    BYTE                                *pMdmResp,
    struct QmiNasIndicationRegisterResp *pApiResp)
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
                           eQMI_NAS_SET_REG_EVENT );
    return eRCode;
}
