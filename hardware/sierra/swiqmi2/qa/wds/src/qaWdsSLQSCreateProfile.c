/*
 * \ingroup wds
 *
 * \file qaWdsSLQSCreateProfile.c
 *
 * \brief  Contains Packing and UnPacking routines for the
 *         QMI_SLQS_WDS_CREATE_PROFILE message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaGobiApiWds.h"
#include "qaWdsSLQSCreateProfile.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * Packs the SLQSCreateProfile profileType and profileID field to the
 * QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSprofiletype3GPP(BYTE *pBuf, BYTE *pParam)
{
    struct CreateProfileIn *pReq =
        ((struct QmiWdsSLQSCreateProfileReq *)pParam)->pSlqsCreateProfileIn;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Add TLV data */
    eRCode = PutByte( pBuf, *(pReq->pProfileType) );

    if ( eRCode == eQCWWAN_ERR_NONE )
    {
       if ( NULL == pReq->pProfileID )
       {
           return eQCWWAN_ERR_NONE;
       }
       else
       {
           /* Add TLV data */
           eRCode = PutByte( pBuf, *(pReq->pProfileID) );
       }
    }
    return eRCode;
}

/*
 * Packs the SLQSCreateProfile profileType and profileID field to the
 * QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSprofiletype3GPP2(BYTE *pBuf, BYTE *pParam)
{
    struct CreateProfileIn *pReq =
         ((struct QmiWdsSLQSCreateProfileReq *)pParam)->pSlqsCreateProfileIn;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Add TLV data */
    eRCode = PutByte( pBuf, *(pReq->pProfileType) );
    if ( eRCode == eQCWWAN_ERR_NONE )
    {
       if ( NULL == pReq->pProfileID )
       {
           return eQCWWAN_ERR_NONE;
       }

       /* Add TLV data */
       eRCode = PutByte( pBuf, *(pReq->pProfileID) );

       if ( eRCode != eQCWWAN_ERR_NONE )
       {
          return eRCode;
       }
    }
    return eRCode;
}

/*
 * Packs the SLQSCreateProfile profilename field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSprofilename1(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq =
     &((struct QmiWdsSLQSCreateProfileReq *)pParam)->pSlqsCreateProfileIn->\
     curProfile.SlqsProfile3GPP;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( NULL == pReq->pProfilename )
    {
        return eRCode;
    }
    /* Add TLV data */
    while (*(pReq->pProfilename) != EOS )
    {
        eRCode = PutByte( pBuf, *(pReq->pProfilename)++ );
        if( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;
    }
    return eRCode;
}

/*
 * Packs the SLQSCreateProfile PDPType field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSPDPType1(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq =
        &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP;

    if ( NULL == pReq->pPDPtype  )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pPDPtype) );
}

/*
 * Packs the SLQSCreateProfile PDPHdCompressType field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSPDPHdCompressType(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq =
         &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP;

    if ( NULL == pReq->pPdpHdrCompType )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pPdpHdrCompType) );
}

/*
 * Packs the SLQSCreateProfile PDPDataComType field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSPDPDataComType(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq =
        &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP;

    if ( NULL == pReq->pPdpDataCompType )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pPdpDataCompType));
}

/*
 * Packs the SLQSCreateProfile ApnName field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSApnName1(BYTE *pBuf, BYTE  *pParam)
{
    struct Profile3GPP *pReq =
        &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( NULL == pReq->pAPNName )
    {
        return eQCWWAN_ERR_NONE;
    }
    /* Add TLV data */
    while( *(pReq->pAPNName) != EOS )
    {
        PutByte(pBuf, *(pReq->pAPNName)++);
        if( eQCWWAN_ERR_NONE != eRCode )
            break;
    }
    return eRCode;
}

/*
 * Packs the SLQSCreateProfile PRIIpv4Pref field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSPRIIpv4Pref1(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq =
       &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP;

    if ( NULL == pReq->pPriDNSIPv4AddPref )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutLong( pBuf, *(pReq->pPriDNSIPv4AddPref) );
}

/*
 * Packs the SLQSCreateProfile SecIpv4Pref field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSSecIpv4Pref1(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP;

    if ( NULL == pReq->pSecDNSIPv4AddPref )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutLong( pBuf, *(pReq->pSecDNSIPv4AddPref) );
}

/*
 * Packs the SLQSCreateProfile UMTSReqQOS field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSUMTSReqQOS(BYTE *pBuf, BYTE *pParam)
{
    struct UMTSQoS *pUMTSQoS =
           ((struct QmiWdsSLQSCreateProfileReq *)pParam)->pSlqsCreateProfileIn\
                   ->curProfile.SlqsProfile3GPP.pUMTSReqQoS;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pUMTSQoS )
        return eRCode;

    /* pack Traffic class */
    eRCode = PutByte( pBuf, (pUMTSQoS->trafficClass) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Max uplink bit rate */
    eRCode = PutLong( pBuf, (pUMTSQoS->maxUplinkBitrate) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Max downlink bit rate */
    eRCode = PutLong( pBuf, (pUMTSQoS->maxDownlinkBitrate) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Granted uplink bit rate */
    eRCode = PutLong( pBuf, (pUMTSQoS->grntUplinkBitrate) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Granted downlink bit rate */
    eRCode = PutLong( pBuf, (pUMTSQoS->grntDownlinkBitrate) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Qos delievery order */
    eRCode = PutByte( pBuf, (pUMTSQoS->qosDeliveryOrder) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack the Max SDU size */
    eRCode = PutLong( pBuf, (pUMTSQoS->maxSDUSize) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

   /* pack the SDU error ratio */
    eRCode = PutByte( pBuf, (pUMTSQoS->sduErrorRatio) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack the Residual bit error ratio */
    eRCode = PutByte( pBuf, (pUMTSQoS->resBerRatio) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Delievery of errorneous SDU */
    eRCode = PutByte( pBuf, (pUMTSQoS->deliveryErrSDU) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack transfer delay */
    eRCode = PutLong( pBuf, (pUMTSQoS->transferDelay) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Traffic priority */
    eRCode = PutLong( pBuf, (pUMTSQoS->trafficPriority) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    return eRCode;
}

/*
 * Packs the SLQSCreateProfile UMTSMinqos field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSUMTSMinQos(BYTE *pBuf, BYTE *pParam)
{
    struct UMTSQoS *pUMTSQoS =
           ((struct QmiWdsSLQSCreateProfileReq *)pParam)->pSlqsCreateProfileIn\
                   ->curProfile.SlqsProfile3GPP.pUMTSMinQoS;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pUMTSQoS )
        return eRCode;

    /* pack Traffic class */
    eRCode = PutByte( pBuf, (pUMTSQoS->trafficClass) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Max uplink bit rate */
    eRCode = PutLong( pBuf, (pUMTSQoS->maxUplinkBitrate) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;
    /* pack Max downlink bit rate */
    eRCode = PutLong( pBuf, (pUMTSQoS->maxDownlinkBitrate) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Granted uplink bit rate */
    eRCode = PutLong( pBuf, (pUMTSQoS->grntUplinkBitrate) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Granted downlink bit rate */
    eRCode = PutLong( pBuf, (pUMTSQoS->grntDownlinkBitrate) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Qos delievery order */
    eRCode = PutByte( pBuf, (pUMTSQoS->qosDeliveryOrder) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack the Max SDU size */
    eRCode = PutLong( pBuf, (pUMTSQoS->maxSDUSize) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

   /* pack the SDU error ratio */
    eRCode = PutByte( pBuf, (pUMTSQoS->sduErrorRatio) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack the Residual bit error ratio */
    eRCode = PutByte( pBuf, (pUMTSQoS->resBerRatio) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Delievery of errorneous SDU */
    eRCode = PutByte( pBuf, (pUMTSQoS->deliveryErrSDU) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack transfer delay */
    eRCode = PutLong( pBuf, (pUMTSQoS->transferDelay) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Traffic priority */
    eRCode = PutLong( pBuf, (pUMTSQoS->trafficPriority) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    return eRCode;
}

/*
 * Packs the SLQSCreateProfile GPRSReqQOS from the QMI response message
 * to a user provided response structure
 *
 * \param  pBuf  [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp [OUT]     - Pointer to structure containing storage
 *                           to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSGPRSReqQos (BYTE *pBuf, BYTE *pParam)
{
    struct GPRSRequestedQoS *pGPRSRequestedQos=
           ((struct QmiWdsSLQSCreateProfileReq *)pParam)->pSlqsCreateProfileIn\
                   ->curProfile.SlqsProfile3GPP.pGPRSRequestedQos;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pGPRSRequestedQos )
        return eRCode;

    /* pack Precedence class */
    eRCode = PutLong( pBuf, (pGPRSRequestedQos->precedenceClass) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Delay class */
    eRCode = PutLong( pBuf, (pGPRSRequestedQos->delayClass) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Reliability class */
    eRCode = PutLong( pBuf, (pGPRSRequestedQos->reliabilityClass) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Peak throughput class */
    eRCode = PutLong( pBuf, (pGPRSRequestedQos->peakThroughputClass) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Mean throughput class */
    eRCode = PutLong( pBuf, (pGPRSRequestedQos->meanThroughputClass) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    return eRCode;
}

/*
 * Packs the SLQSCreateProfile GPRS QOS from the QMI response message
 * to a user provided response structure
 *
 * \param  pBuf [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pParam[OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSGPRSMinQos(BYTE *pBuf, BYTE *pParam)
{
    struct GPRSRequestedQoS *pGPRSMinimumQoS=
             ((struct QmiWdsSLQSCreateProfileReq *)pParam)->pSlqsCreateProfileIn\
                    ->curProfile.SlqsProfile3GPP.pGPRSMinimumQoS;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pGPRSMinimumQoS)
        return eRCode;

    /* pack Precedence class */
    eRCode = PutLong( pBuf, (pGPRSMinimumQoS->precedenceClass) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Delay class */
    eRCode = PutLong( pBuf, (pGPRSMinimumQoS->delayClass) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Reliability class */
    eRCode = PutLong( pBuf, (pGPRSMinimumQoS->reliabilityClass) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Peak throughput class */
    eRCode = PutLong( pBuf, (pGPRSMinimumQoS->peakThroughputClass) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Mean throughput class */
    eRCode = PutLong( pBuf, (pGPRSMinimumQoS->meanThroughputClass) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    return eRCode;
}

/*
 * Packs the SLQSCreateProfile Username field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvSLQSUsername1(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( NULL == pReq->pUsername )
        return eRCode;

    /* Add TLV data */
    while (*(pReq->pUsername) != EOS )
    {
        eRCode = PutByte(pBuf, *(pReq->pUsername)++ );
        if( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;
    }
    return eRCode;
}

/*
 * Packs the SetDefaultProfile Password field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvSLQSPassword1(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( NULL == pReq->pPassword )
        return eRCode;

    /* Add TLV data */
    while (*(pReq->pPassword) != EOS )
    {
        eRCode = PutByte(pBuf, *(pReq->pPassword)++);
        if( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;
    }
    return eRCode;
}

/*
 * Packs the SLQSCreateProfile authenticationPref field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSAutPref(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP;

    if ( NULL == pReq->pAuthenticationPref )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pAuthenticationPref) );
}

/*
 * Packs the SLQSCreateProfile IPv4AddrPref field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSAddPref(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP;

    if ( NULL == pReq->pIPv4AddrPref )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutLong( pBuf, *(pReq->pIPv4AddrPref) );
}

/*
 * Packs the SLQSCreateProfile AddrUsingPCO field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSAddrUsingPCO(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP;

    if ( NULL == pReq->pPcscfAddrUsingPCO )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pPcscfAddrUsingPCO) );
}

/*
 * Packs the SLQSCreateProfile PDPAccConFlag field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSPDPAccConFlag(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP;

    if ( NULL == pReq->pPdpAccessConFlag )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pPdpAccessConFlag) );
}

/*
 * Packs the SLQSCreateProfile PCSCFAddrDHCP field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSPCSCFAddrDHCP(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP;

    if ( NULL == pReq->pPcscfAddrUsingDhcp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pPcscfAddrUsingDhcp) );
}

/*
 * Packs the SLQSCreateProfile IMCNFLAG field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSIMCNFLAG(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP;

    if ( NULL == pReq->pImCnFlag )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pImCnFlag) );
}

/*
 * Packs the SLQSCreateProfile TFTIDParams field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvTftId1Params(BYTE *pBuf, BYTE *pParam)
{
    struct TFTIDParams *pTFTID1Params =
           ((struct QmiWdsSLQSCreateProfileReq *)pParam)->pSlqsCreateProfileIn\
                   ->curProfile.SlqsProfile3GPP.pTFTID1Params;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    USHORT idx = 0;

    if ( !pTFTID1Params )
        return eRCode;

    /* pack filterId */
    eRCode = PutByte( pBuf, (pTFTID1Params->filterId) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack eValid */
    eRCode = PutByte( pBuf, (pTFTID1Params->eValid) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack ipVersion */
    eRCode = PutByte( pBuf, (pTFTID1Params->ipVersion ) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    if ( NULL == pTFTID1Params->pSourceIP )
    {
        return eQCWWAN_ERR_NONE;
    }
    else
    {
        if ( IPV6 == pTFTID1Params->ipVersion )
        {
            for( idx = 0; idx < 8 ; idx++ )
            {
                eRCode = PutWordBe(pBuf, *(pTFTID1Params->pSourceIP)++ );
                if (eRCode != eQCWWAN_ERR_NONE)
                    return eRCode;
            }
        }
        else
        {
            eRCode = PutLong(pBuf, *( (ULONG *)pTFTID1Params->pSourceIP) );
            if (eRCode != eQCWWAN_ERR_NONE)
                return eRCode;

            /* Pack remaining buffer */
            for( idx = 0; idx < 6 ; idx++ )
            {
                eRCode = PutWord(pBuf, *(pTFTID1Params->pSourceIP)++ );
                if (eRCode != eQCWWAN_ERR_NONE)
                    return eRCode;
            }

        }
    }

    /* pack sourceIPMask */
    eRCode = PutByte( pBuf, (pTFTID1Params->sourceIPMask) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack nextHeader; */
    eRCode = PutByte( pBuf, (pTFTID1Params->nextHeader) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack the destPortRangeStart  */
    eRCode = PutWord( pBuf, (pTFTID1Params->destPortRangeStart) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

   /* pack the destPortRangeEnd  */
    eRCode = PutWord( pBuf, (pTFTID1Params->destPortRangeEnd) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack the srcPortRangeStart  */
    eRCode = PutWord( pBuf, (pTFTID1Params->srcPortRangeStart) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack srcPortRangeEnd */
    eRCode = PutWord( pBuf, (pTFTID1Params->srcPortRangeEnd) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack IPSECSPI */
    eRCode = PutLong( pBuf, (pTFTID1Params->IPSECSPI) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack tosMask */
    eRCode = PutWord( pBuf, (pTFTID1Params->tosMask ) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack tosMask */
    eRCode = PutLong( pBuf, (pTFTID1Params->flowLabel) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    return eRCode;
}

/*
 * Packs the SLQSCreateProfile TFTIDParams field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvTftId2Params(BYTE *pBuf, BYTE *pParam)
{
    struct TFTIDParams *pTFTID2Params =
        ((struct QmiWdsSLQSCreateProfileReq *)pParam)->pSlqsCreateProfileIn->\
          curProfile.SlqsProfile3GPP.pTFTID2Params;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    USHORT idx = 0;

    if ( !pTFTID2Params )
        return eRCode;

    /* pack filterId */
    eRCode = PutByte( pBuf, (pTFTID2Params->filterId) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack eValid */
    eRCode = PutByte( pBuf, (pTFTID2Params->eValid) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack ipVersion */
    eRCode = PutByte( pBuf, (pTFTID2Params->ipVersion ) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    if ( NULL == pTFTID2Params->pSourceIP )
    {
        return eQCWWAN_ERR_NONE;
    }
    else
    {
        if ( IPV6 == pTFTID2Params->ipVersion )
        {
            for( idx = 0; idx < 8 ; idx++ )
            {
                eRCode = PutWordBe(pBuf, *(pTFTID2Params->pSourceIP)++ );
                if (eRCode != eQCWWAN_ERR_NONE)
                    return eRCode;
            }
        }
        else
        {
            eRCode = PutLong(pBuf, *( (ULONG *)pTFTID2Params->pSourceIP) );
            if (eRCode != eQCWWAN_ERR_NONE)
                return eRCode;

            /* Pack remaining buffer */
            for( idx = 0; idx < 6 ; idx++ )
            {
                eRCode = PutWord(pBuf, *(pTFTID2Params->pSourceIP)++ );
                if (eRCode != eQCWWAN_ERR_NONE)
                    return eRCode;
            }

        }
    }

    /* pack sourceIPMask */
    eRCode = PutByte( pBuf, (pTFTID2Params->sourceIPMask) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack nextHeader; */
    eRCode = PutByte( pBuf, (pTFTID2Params->nextHeader) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack the destPortRangeStart  */
    eRCode = PutWord( pBuf, (pTFTID2Params->destPortRangeStart) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

   /* pack the destPortRangeEnd  */
    eRCode = PutWord( pBuf, (pTFTID2Params->destPortRangeEnd) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack the srcPortRangeStart  */
    eRCode = PutWord( pBuf, (pTFTID2Params->srcPortRangeStart) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack srcPortRangeEnd */
    eRCode = PutWord( pBuf, (pTFTID2Params->srcPortRangeEnd) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack IPSECSPI */
    eRCode = PutLong( pBuf, (pTFTID2Params->IPSECSPI) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack tosMask */
    eRCode = PutWord( pBuf, (pTFTID2Params->tosMask ) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;
    /* pack tosMask */
    eRCode = PutLong( pBuf, (pTFTID2Params->flowLabel) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    return eRCode;
}

/*
 * Packs the SLQSCreateProfile PDPContext field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSPDPContext(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP;

    if ( NULL == pReq->pPdpContext )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pPdpContext) );
}

/*
 * Packs the SLQSCreateProfile PDPConSecFlag field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSPDPConSecFlag(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP;

    if ( NULL == pReq->pSecondaryFlag )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pSecondaryFlag) );
}

/*
 * Packs the SLQSCreateProfile PrimaryID field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSPrimaryID(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP;

    if ( NULL == pReq->pPrimaryID )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pPrimaryID) );
}

/*
 * Packs the SLQSCreateProfile pIPv6AddPref field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvIPv6AddPref(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    USHORT idx = 0;

    if ( NULL == pReq->pIPv6AddPref )
    {
        return eQCWWAN_ERR_NONE;
    }
    for( idx = 0; idx < 8 ; idx++ )
    {
        eRCode = PutWordBe(pBuf, *(pReq->pIPv6AddPref)++ );
        if (eRCode != eQCWWAN_ERR_NONE)
            return eRCode;
    }
    return eRCode;
}

/*
 * Packs the SLQSCreateProfile UMTSReqQOS field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvUMTSReqQoSSigInd(BYTE *pBuf, BYTE *pParam)
{
    struct UMTSReqQoSSigInd *pUMTSReqQoSSigInd =
           ((struct QmiWdsSLQSCreateProfileReq *)pParam)->pSlqsCreateProfileIn\
                   ->curProfile.SlqsProfile3GPP.pUMTSReqQoSSigInd;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pUMTSReqQoSSigInd )
        return eRCode;

    /* pack Traffic class */
    eRCode = PutByte( pBuf, (pUMTSReqQoSSigInd->UMTSReqQoS.trafficClass) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Max uplink bit rate */
    eRCode = PutLong( pBuf, (pUMTSReqQoSSigInd->UMTSReqQoS.maxUplinkBitrate) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Max downlink bit rate */
    eRCode = PutLong( pBuf, (pUMTSReqQoSSigInd->UMTSReqQoS.maxDownlinkBitrate) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Granted uplink bit rate */
    eRCode = PutLong( pBuf, (pUMTSReqQoSSigInd->UMTSReqQoS.grntUplinkBitrate) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Granted downlink bit rate */
    eRCode = PutLong( pBuf, (pUMTSReqQoSSigInd->UMTSReqQoS.grntDownlinkBitrate) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Qos delievery order */
    eRCode = PutByte( pBuf, (pUMTSReqQoSSigInd->UMTSReqQoS.qosDeliveryOrder) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack the Max SDU size */
    eRCode = PutLong( pBuf, (pUMTSReqQoSSigInd->UMTSReqQoS.maxSDUSize) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

   /* pack the SDU error ratio */
    eRCode = PutByte( pBuf, (pUMTSReqQoSSigInd->UMTSReqQoS.sduErrorRatio) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack the Residual bit error ratio */
    eRCode = PutByte( pBuf, (pUMTSReqQoSSigInd->UMTSReqQoS.resBerRatio) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Delievery of errorneous SDU */
    eRCode = PutByte( pBuf, (pUMTSReqQoSSigInd->UMTSReqQoS.deliveryErrSDU) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack transfer delay */
    eRCode = PutLong( pBuf, (pUMTSReqQoSSigInd->UMTSReqQoS.transferDelay) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Traffic priority */
    eRCode = PutLong( pBuf, (pUMTSReqQoSSigInd->UMTSReqQoS.trafficPriority) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Traffic priority */
    eRCode = PutByte( pBuf, (pUMTSReqQoSSigInd->SigInd) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    return eRCode;
}

/*
 * Packs the SLQSCreateProfile UMTSMinQOS field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvUMTSMinQosSigInd(BYTE *pBuf, BYTE *pParam)
{
    struct UMTSReqQoSSigInd *pUMTSMinQosSigInd =
           ((struct QmiWdsSLQSCreateProfileReq *)pParam)->pSlqsCreateProfileIn\
                   ->curProfile.SlqsProfile3GPP.pUMTSMinQosSigInd;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pUMTSMinQosSigInd )
        return eRCode;

    /* pack Traffic class */
    eRCode = PutByte( pBuf, (pUMTSMinQosSigInd->UMTSReqQoS.trafficClass) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Max uplink bit rate */
    eRCode = PutLong( pBuf, (pUMTSMinQosSigInd->UMTSReqQoS.maxUplinkBitrate) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Max downlink bit rate */
    eRCode = PutLong( pBuf, (pUMTSMinQosSigInd->UMTSReqQoS.maxDownlinkBitrate) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Granted uplink bit rate */
    eRCode = PutLong( pBuf, (pUMTSMinQosSigInd->UMTSReqQoS.grntUplinkBitrate) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Granted downlink bit rate */
    eRCode = PutLong( pBuf, (pUMTSMinQosSigInd->UMTSReqQoS.grntDownlinkBitrate) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Qos delievery order */
    eRCode = PutByte( pBuf, (pUMTSMinQosSigInd->UMTSReqQoS.qosDeliveryOrder) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack the Max SDU size */
    eRCode = PutLong( pBuf, (pUMTSMinQosSigInd->UMTSReqQoS.maxSDUSize) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

   /* pack the SDU error ratio */
    eRCode = PutByte( pBuf, (pUMTSMinQosSigInd->UMTSReqQoS.sduErrorRatio) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack the Residual bit error ratio */
    eRCode = PutByte( pBuf, (pUMTSMinQosSigInd->UMTSReqQoS.resBerRatio) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Delievery of errorneous SDU */
    eRCode = PutByte( pBuf, (pUMTSMinQosSigInd->UMTSReqQoS.deliveryErrSDU) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack transfer delay */
    eRCode = PutLong( pBuf, (pUMTSMinQosSigInd->UMTSReqQoS.transferDelay) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Traffic priority */
    eRCode = PutLong( pBuf, (pUMTSMinQosSigInd->UMTSReqQoS.trafficPriority) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack Traffic priority */
    eRCode = PutByte( pBuf, (pUMTSMinQosSigInd->SigInd) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    return eRCode;
}

/*
 * Packs the SLQSCreateProfile PrimaryDNSIPv6addpref field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvPriDNSIPv6addpref(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    USHORT idx = 0;

    if ( NULL == pReq->pPriDNSIPv6addpref )
    {
        return eQCWWAN_ERR_NONE;
    }
    for( idx = 0; idx < 8 ; idx++ )
    {
        eRCode = PutWordBe(pBuf, *(pReq->pPriDNSIPv6addpref)++ );
        if (eRCode != eQCWWAN_ERR_NONE)
            return eRCode;
    }
    return eRCode;
}

/*
 * Packs the SLQSCreateProfile SecondaryDNSIPv6addpref field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSecDNSIPv6addpref(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    USHORT idx = 0;

    if ( NULL == pReq->pSecDNSIPv6addpref )
    {
        return eQCWWAN_ERR_NONE;
    }
    for( idx = 0; idx < 8 ; idx++ )
    {
        eRCode = PutWordBe(pBuf, *(pReq->pSecDNSIPv6addpref)++ );
        if (eRCode != eQCWWAN_ERR_NONE)
            return eRCode;
    }
    return eRCode;
}

/*
 * Packs the SLQSCreateProfile AddrAllocPref field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvAddrAllocPref(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP;

    if ( NULL == pReq->pAddrAllocPref)
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pAddrAllocPref) );
}

/*
 * Packs the SLQSCreateProfile QosClassID field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvQosClassID(BYTE *pBuf, BYTE *pParam)
{
    struct QosClassID *pQosClassID=
           ((struct QmiWdsSLQSCreateProfileReq *)pParam)->pSlqsCreateProfileIn\
                   ->curProfile.SlqsProfile3GPP.pQosClassID;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pQosClassID )
        return eRCode;

    /* pack Traffic class */
    eRCode = PutByte( pBuf, (pQosClassID->QCI) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Pack GDlBitRate */
    eRCode = PutLong( pBuf, (pQosClassID->gDlBitRate) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Pack Max downlink bit rate */
    eRCode = PutLong( pBuf, (pQosClassID->maxDlBitRate) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack gUlBitRate */
    eRCode = PutLong( pBuf, (pQosClassID->gUlBitRate) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* pack maxUlBitRate */
    eRCode = PutLong( pBuf, (pQosClassID->maxUlBitRate) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    return eRCode;
}

/*
 * Packs the SLQSCreateProfile NegoDnsSrvrPref field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvNegoDnsSrvrPref(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pNegoDnsSrvrPref )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pNegoDnsSrvrPref) );
}

/*
 * Packs the SLQSCreateProfile pppSessCloseTimerDO field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvPPPSessionCloseTimerDo(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pPppSessCloseTimerDO )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutLong( pBuf, *(pReq->pPppSessCloseTimerDO) );
}

/*
 * Packs the SLQSCreateProfile pppSessCloseTimer1x field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvPPPSessionCloseTimer1X(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pPppSessCloseTimer1x )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutLong( pBuf, *(pReq->pPppSessCloseTimer1x) );
}

/*
 * Packs the SLQSCreateProfile AllowLinger field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvAllowLinger(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pAllowLinger )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pAllowLinger) );
}

/*
 * Packs the SLQSCreateProfile LcpAckTimeout field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvLCPAckTimeOut(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pLcpAckTimeout )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutWord( pBuf, *(pReq->pLcpAckTimeout) );
}

/*
 * Packs the SLQSCreateProfile IpcpAckTimeout field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvIPCPAckTimeOut(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pIpcpAckTimeout )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutWord( pBuf, *(pReq->pIpcpAckTimeout) );
}

/*
 * Packs the SLQSCreateProfile pAuthTimeout field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvAuthTimeOut(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pAuthTimeout )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutWord( pBuf, *(pReq->pAuthTimeout) );
}

/*
 * Packs the SLQSCreateProfile LcpCreqRetryCount field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvLCPCreqRetCount(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pLcpCreqRetryCount )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pLcpCreqRetryCount) );
}

/*
 * Packs the SLQSCreateProfile IpcpCreqRetryCount field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvIPCPCreqRetCount(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pIpcpCreqRetryCount )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pIpcpCreqRetryCount) );
}

/*
 * Packs the SLQSCreateProfile AuthRetryCount field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvAuthRetCount(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pAuthRetryCount )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pAuthRetryCount) );
}

/*
 * Packs the SLQSCreateProfile AuthProtocol field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvAuthProtocol(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pAuthProtocol )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pAuthProtocol) );
}

/*
 * Packs the SLQSCreateProfile UserId field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvUserId(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP2;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( NULL == pReq->pUserId )
    {
        return eQCWWAN_ERR_NONE;
    }

    while ( *(pReq->pUserId) != EOS )
    {
        /* Add TLV data */
        eRCode = PutByte( pBuf, *(pReq->pUserId)++ );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            break;
        }
    }
    return eRCode;
}

/*
 * Packs the SLQSCreateProfile AuthPassword field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvAuthPassword(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP2;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( NULL == pReq->pAuthPassword )
    {
        return eQCWWAN_ERR_NONE;
    }
    while( *(pReq->pUserId)!= EOS )
    {
        /* Add TLV data */
        eRCode = PutByte( pBuf, *(pReq->pAuthPassword)++ );

        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            break;
        }
    }
    return eRCode;
}

/*
 * Packs the SLQSCreateProfile DataRate field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvDatarate(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pDataRate )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pDataRate) );
}

/*
 * Packs the SLQSCreateProfile AppType field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvAppType(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pAppType )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutLong( pBuf, *(pReq->pAppType) );
}

/*
 * Packs the SLQSCreateProfile DataMode field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvDataMode(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pDataMode )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pDataMode) );
}

/*
 * Packs the SLQSCreateProfile AppPriority field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvAppPriority(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pAppPriority )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pAppPriority) );
}

/*
 * Packs the SLQSCreateProfile ApnString field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildAPNString(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP2;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( NULL == pReq->pApnString )
    {
        return eQCWWAN_ERR_NONE;
    }
    while ( *(pReq->pApnString) != EOS )
    {
        /* Add TLV data */
        eRCode = PutByte( pBuf, *(pReq->pApnString)++ );

        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            break;
        }
    }

    return eRCode;
}

/*
 * Packs the SLQSCreateProfile PdnType field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildPDNtype(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pPdnType )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pPdnType) );
}

/*
 * Packs the SLQSCreateProfile IsPcscfAddressNedded field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvISPCSCFAddNedded(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pIsPcscfAddressNedded )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pIsPcscfAddressNedded) );
}

/*
 * Packs the SLQSCreateProfile PrimaryV4DnsAddress field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvPriV4DNSAddress(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pPrimaryV4DnsAddress )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutLong( pBuf, *(pReq->pPrimaryV4DnsAddress) );
}

/*
 * Packs the SLQSCreateProfile SecondaryV4DnsAddress field to the
 * QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSecV4DnsAddress (BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pSecondaryV4DnsAddress )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutLong( pBuf, *(pReq->pSecondaryV4DnsAddress) );
}

/*
 * Packs the SLQSCreateProfile PriV6DnsAddress field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvPriV6DnsAddress(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP2;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    USHORT idx = 0;

    if ( NULL == pReq->pPriV6DnsAddress )
    {
        return eQCWWAN_ERR_NONE;
    }
    for( idx = 0; idx < 8 ; idx++ )
    {
        eRCode = PutWordBe(pBuf, *(pReq->pPriV6DnsAddress)++ );
        if (eRCode != eQCWWAN_ERR_NONE)
            return eRCode;
    }
    return eRCode;
}

/*
 * Packs the SLQSCreateProfile PriV6DnsAddress field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSecV6DnsAddress(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSCreateProfileReq *)\
           pParam)->pSlqsCreateProfileIn->curProfile.SlqsProfile3GPP2;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    USHORT idx = 0;

    if ( NULL == pReq->pSecV6DnsAddress )
    {
        return eQCWWAN_ERR_NONE;
    }

    for( idx = 0; idx < 8 ; idx++ )
    {
        eRCode = PutWordBe(pBuf, *(pReq->pSecV6DnsAddress)++ );
        if (eRCode != eQCWWAN_ERR_NONE)
            return eRCode;
    }
    return eRCode;
}

/*
 * This function packs the SLQSCreateProfile parameters to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa     qaGobiApiWds.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiWdsSLQSCreateProfile(
    WORD                   *pMlength,
    BYTE                   *pParamField,
    struct CreateProfileIn *pCreateProfileIn )
{
    struct QmiWdsSLQSCreateProfileReq req;
    enum eQCWWANError   eRCode = eQCWWAN_ERR_QMI_INVALID_PROFILE_TYPE;
    USHORT messageID           = 0xFFFF;

    /* Profile type is mandatory parameter */
    if(!pCreateProfileIn->pProfileType)
        return eRCode;

    slmemset((char *)&req, 0, sizeof (struct QmiWdsSLQSCreateProfileReq));
    req.pSlqsCreateProfileIn = pCreateProfileIn;

    if ( PROFILE_3GPP == *(pCreateProfileIn->pProfileType) )
    {
        if ( NULL == pCreateProfileIn->pProfileID )
        {
            messageID = eQMI_WDS_CREATE_PROFILE;
        }
        else
        {
            messageID = eQMI_WDS_SWI_CREATE_PROFILE;
        }

        static struct qmTlvBuilderItem map[] =
        {
            { eTLV_PROFILE_TYPE1,               &BuildTlvSLQSprofiletype3GPP},
            { eTLV_PROFILE_NAME1,               &BuildTlvSLQSprofilename1},
            { eTLV_PDP_TYPE1,                   &BuildTlvSLQSPDPType1},
            { eTLV_PDP_HEADER_COMPRESSION_TYPE, &BuildTlvSLQSPDPHdCompressType},
            { eTLV_PDP_DATA_COMPRESSION_TYPE,   &BuildTlvSLQSPDPDataComType},
            { eTLV_APN_NAME1,                   &BuildTlvSLQSApnName1},
            { eTLV_PRI_DNS_IPV4_ADDRESS_PREF1,  &BuildTlvSLQSPRIIpv4Pref1},
            { eTLV_SEC_DNS_IPV4_ADDRESS_PREF1,  &BuildTlvSLQSSecIpv4Pref1},
            { eTLV_UMTS_REQUESTED_QOS,          &BuildTlvSLQSUMTSReqQOS},
            { eTLV_UMTS_MINIMUM_QOS,            &BuildTlvSLQSUMTSMinQos},
            { eTLV_GPRS_REQUESTED_QOS,          &BuildTlvSLQSGPRSReqQos},
            { eTLV_GPRS_MINIMUM_QOS,            &BuildTlvSLQSGPRSMinQos},
            { eTLV_USERNAME1,                   &BuildTlvSLQSUsername1},
            { eTLV_PASSWORD1,                   &BuildTlvSLQSPassword1},
            { eTLV_AUTHENTICATION_PREFERENCE,   &BuildTlvSLQSAutPref},
            { eTLV_IPV4_ADDRESS_PREFERENCE,     &BuildTlvSLQSAddPref},
            { eTLV_PCSCF_ADDR_USING_PCO,        &BuildTlvSLQSAddrUsingPCO},
            { eTLV_PDP_ACCESS_CONTROL_FLAG,     &BuildTlvSLQSPDPAccConFlag},
            { eTLV_PCSCF_ADDR_USING_DHCP,       &BuildTlvSLQSPCSCFAddrDHCP},
            { eTLV_IM_CN_FLAG,                  &BuildTlvSLQSIMCNFLAG},
            { eTLV_TFT_ID1_PARAMETERS,          &BuildTlvTftId1Params},
            { eTLV_TFT_ID2_PARAMETERS,          &BuildTlvTftId2Params},
            { eTLV_PDP_CONTEXT,                 &BuildTlvSLQSPDPContext},
            { eTLV_PDP_CONTEXT_SECONDARY_FLAG,  &BuildTlvSLQSPDPConSecFlag},
            { eTLV_PDP_CONTEXT_PRIMARY_ID,      &BuildTlvSLQSPrimaryID},
            { eTLV_IPV6_ADDRESS_PREFERENCE,     &BuildTlvIPv6AddPref},
            { eTLV_UMTS_REQ_QOS_SIG_IND,        &BuildTlvUMTSReqQoSSigInd},
            { eTLV_UMTS_MIN_QOS_SIG_IND,        &BuildTlvUMTSMinQosSigInd},
            { eTLV_PRI_DNS_IPV6_ADDRESS_PREF,   &BuildTlvPriDNSIPv6addpref},
            { eTLV_SEC_DNS_IPV6_ADDRESS_PREF,   &BuildTlvSecDNSIPv6addpref},
            { eTLV_ADDR_ALLOCATION_PREF,        &BuildTlvAddrAllocPref},
            { eTLV_3GPP_LTE_QOS_PARAMS,         &BuildTlvQosClassID},
            { eTLV_TYPE_INVALID,                NULL }/* Important Sentinel
                                                       * Signifies last item
                                                       * in map
                                                       */
        };
        eRCode = qmbuild( pParamField,
                         (BYTE *)&req,
                          map,
                          messageID,
                          pMlength );
    }
    else if (PROFILE_3GPP2 == *(pCreateProfileIn->pProfileType))
    {
        static struct qmTlvBuilderItem map[] =
        {
            { eTLV_PROFILE_TYPE1,              &BuildTlvSLQSprofiletype3GPP2},
            { eTLV_NEGO_DNS_SRVR_PREF,         &BuildTlvNegoDnsSrvrPref},
            { eTLV_PPP_SESSION_CLOSE_TIMER_DO, &BuildTlvPPPSessionCloseTimerDo},
            { eTLV_PPP_SESSION_CLOSE_TIMER_1X, &BuildTlvPPPSessionCloseTimer1X},
            { eTLV_ALLOW_LINGER,               &BuildTlvAllowLinger},
            { eTLV_LCP_ACK_TIMEOUT,            &BuildTlvLCPAckTimeOut},
            { eTLV_IPCP_ACK_TIMEOUT,           &BuildTlvIPCPAckTimeOut},
            { eTLV_AUTH_TIMEOUT,               &BuildTlvAuthTimeOut},
            { eTLV_LCP_CREQ_RETRY_COUNT,       &BuildTlvLCPCreqRetCount},
            { eTLV_IPCP_CREQ_RETRY_COUNT,      &BuildTlvIPCPCreqRetCount},
            { eTLV_AUTH_RETRY_COUNT,           &BuildTlvAuthRetCount},
            { eTLV_AUTH_PROTOCOl,              &BuildTlvAuthProtocol},
            { eTLV_USER_ID,                    &BuildTlvUserId},
            { eTLV_AUTH_PASSWORD,              &BuildTlvAuthPassword},
            { eTLV_DATA_RATE,                  &BuildTlvDatarate},
#if 0
            { eTLV_APP_TYPE,                   &BuildTlvAppType},
            { eTLV_APP_PRIORITY,               &BuildTlvAppPriority},
#endif
            { eTLV_DATA_MODE,                  &BuildTlvDataMode},
            { eTLV_APN_STRING,                 &BuildAPNString},
            { eTLV_PDN_TYPE,                   &BuildPDNtype},
            { eTLV_IS_PCSCF_ADD_NEDDED,        &BuildTlvISPCSCFAddNedded},
            { eTLV_PRIMARY_V4_DNS_ADDRESS,     &BuildTlvPriV4DNSAddress},
            { eTLV_SECONDARY_V4_DNS_ADDRESS,   &BuildTlvSecV4DnsAddress},
            { eTLV_PRIMARY_V6_DNS_ADDRESS,     &BuildTlvPriV6DnsAddress},
            { eTLV_SECONDARY_V6_DNS_ADDRESS,   &BuildTlvSecV6DnsAddress},
            { eTLV_TYPE_INVALID,               NULL} /* Important Sentinel
                                                      * Signifies last item
                                                      * in map
                                                      */
        };
        eRCode = qmbuild( pParamField,
                          (BYTE *)&req,
                          map,
                          eQMI_WDS_SWI_CREATE_PROFILE,
                          pMlength );
    }
    return eRCode;
}

/*****************************************************************************
* Response handling
******************************************************************************/
/*
 * This function unpacks the SLQSSLQSGetRuntimeSettings Primary DNS IPV4 from
 * the QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError qmUnpackTlvProfileIdentifier1(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsSLQSCreateProfileResp *pLresp =
                        (struct QmiWdsSLQSCreateProfileResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pLresp->pSlqsCreateProfileOut->pProfileType )
    {
        return eRCode;
    }

    /* Extract the profile type address */
    eRCode = GetByte( pTlvData, pLresp->pSlqsCreateProfileOut->pProfileType );

    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    if ( !pLresp->pSlqsCreateProfileOut->pProfileIndex )
    {
        return eRCode;
    }

    /* Extract the profile Index */
    eRCode = GetByte( pTlvData, pLresp->pSlqsCreateProfileOut->pProfileIndex );

    return eRCode;
}

/*
 * This function unpacks the Profile Extended Error from
 * the QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError qmUnpackTlvProfileExtErr(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsSLQSCreateProfileResp *pLresp =
                        (struct QmiWdsSLQSCreateProfileResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pLresp->pSlqsCreateProfileOut->pExtErrorCode )
    {
        return eRCode;
    }

    /* Extract the extended error code */
    return GetWord( pTlvData, pLresp->pSlqsCreateProfileOut->pExtErrorCode );
}

/*
 * This function unpacks the SLQSCreateProfile response message to a
 * user-provided response structure.
 *
 * \param     pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UpkQmiWdsSLQSCreateProfile(
    BYTE                               *pMdmResp,
    struct QmiWdsSLQSCreateProfileResp *pApiResp,
    BYTE                               *pProfileID )
{
    enum eQCWWANError eRCode;
    USHORT messageID           = 0xFFFF;

    if ( NULL == pProfileID )
    {
        messageID = eQMI_WDS_CREATE_PROFILE;
    }
    else
    {
        messageID = eQMI_WDS_SWI_CREATE_PROFILE;
    }

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,                &qmUnpackTlvResultCode },
        { eTLV_PROFILE_IDENTIFIER1,        &qmUnpackTlvProfileIdentifier1 },
        { eTLV_PROFILE_EXTENDED_ERRORCODE, &qmUnpackTlvProfileExtErr },
        { eTLV_TYPE_INVALID,               NULL }/* Important. Sentinel.
                                                  * Signifies last item in map.
                                                  */
    };
    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           messageID );
    return eRCode;
}
