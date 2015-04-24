/*
 * \ingroup wds
 *
 * \file qaWdsSLQSModifyProfile.c
 *
 * \brief  Contains Packing and UnPacking routines for the
 *         QMI_SLQS_WDS_MODIFY_PROFILE message.
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
#include "qaWdsSLQSModifyProfile.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * Packs the SLQSModifyProfile profileType and profileID field to the
 * QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSModifyproftype(BYTE *pBuf, BYTE *pParam)
{
    struct ModifyProfileIn *pReq =
        ((struct QmiWdsSLQSModifyProfileReq *)pParam)->pSlqsModifyProfileIn;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Add TLV data */
    eRCode = PutByte( pBuf, *(pReq->pProfileType) );
    if ( eRCode == eQCWWAN_ERR_NONE )
    {
        /* Add TLV data */
        eRCode = PutByte( pBuf, *(pReq->pProfileID) );
    }
    return eRCode;
}

/*
 * Packs the SLQSModifyProfile profilename field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSModifyprofilename(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq =
     &((struct QmiWdsSLQSModifyProfileReq *)pParam)->pSlqsModifyProfileIn->\
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
 * Packs the SLQSModifyProfile PDPType field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSModifyPDPType(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq =
        &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP;

    if ( NULL == pReq->pPDPtype  )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pPDPtype) );
}

/*
 * Packs the SLQSModifyProfile PDPHdCompressType field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSModifyPDPHdComprType(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq =
         &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP;

    if ( NULL == pReq->pPdpHdrCompType )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pPdpHdrCompType) );
}

/*
 * Packs the SLQSModifyProfile PDPDataComType field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSModifyPDPDataComType(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq =
        &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP;

    if ( NULL == pReq->pPdpDataCompType )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pPdpDataCompType));
}

/*
 * Packs the SLQSModifyProfile ApnName field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSModifyApnName(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq =
        &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP;

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
 * Packs the SLQSModifyProfile PRIIpv4Pref field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSModifyPRIIpv4Pref(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq =
       &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP;

    if ( NULL == pReq->pPriDNSIPv4AddPref )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutLong( pBuf, *(pReq->pPriDNSIPv4AddPref) );
}

/*
 * Packs the SLQSModifyProfile SecIpv4Pref field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSModifySecIpv4Pref(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP;

    if ( NULL == pReq->pSecDNSIPv4AddPref )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutLong( pBuf, *(pReq->pSecDNSIPv4AddPref) );
}

/*
 * Packs the SLQSModifyProfile UMTSReqQOS field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSModifyUMTSReqQOS(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP;

    struct UMTSQoS *pUMTSQoS = pReq->pUMTSReqQoS;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pUMTSQoS )
        return eRCode;

    /* pack Traffic class */
    eRCode = PutByte( pBuf, (pUMTSQoS->trafficClass) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack Max uplink bit rate */
    eRCode = PutLong( pBuf, (pUMTSQoS->maxUplinkBitrate) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack Max downlink bit rate */
    eRCode = PutLong( pBuf, (pUMTSQoS->maxDownlinkBitrate) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack guaranteed uplink bit rate */
    eRCode = PutLong( pBuf, (pUMTSQoS->grntUplinkBitrate) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack guaranteed downlink bit rate */
    eRCode = PutLong( pBuf, (pUMTSQoS->grntDownlinkBitrate) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack Qos delievery order */
    eRCode = PutByte( pBuf, (pUMTSQoS->qosDeliveryOrder) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack the Max SDU size */
    eRCode = PutLong( pBuf, (pUMTSQoS->maxSDUSize) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

   /* pack the SDU error ratio */
    eRCode = PutByte( pBuf, (pUMTSQoS->sduErrorRatio) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack the Residual bit error ratio */
    eRCode = PutByte( pBuf, (pUMTSQoS->resBerRatio) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack Deliveryerrorneous SDU */
    eRCode = PutByte( pBuf, (pUMTSQoS->deliveryErrSDU) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack transfer delay */
    eRCode = PutLong( pBuf, (pUMTSQoS->transferDelay) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack Traffic priority */
    eRCode = PutLong( pBuf, (pUMTSQoS->trafficPriority) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    return eRCode;
}

/*
 * Packs the SLQSModifyProfile UMTSMinqos field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSModifyUMTSMinQos(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP;

    struct UMTSQoS *pUMTSQoS = pReq->pUMTSReqQoS;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pUMTSQoS )
        return eRCode;

    /* pack Traffic class */
    eRCode = PutByte( pBuf, (pUMTSQoS->trafficClass) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack Max uplink bit rate */
    eRCode = PutLong( pBuf, (pUMTSQoS->maxUplinkBitrate) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack Max downlink bit rate */
    eRCode = PutLong( pBuf, (pUMTSQoS->maxDownlinkBitrate) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack guaranteed uplink bit rate */
    eRCode = PutLong( pBuf, (pUMTSQoS->grntUplinkBitrate) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack guaranteed downlink bit rate */
    eRCode = PutLong( pBuf, (pUMTSQoS->grntDownlinkBitrate) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack Qos delievery order */
    eRCode = PutByte( pBuf, (pUMTSQoS->qosDeliveryOrder) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack the Max SDU size */
    eRCode = PutLong( pBuf, (pUMTSQoS->maxSDUSize) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

   /* pack the SDU error ratio */
    eRCode = PutByte( pBuf, (pUMTSQoS->sduErrorRatio) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack the Residual bit error ratio */
    eRCode = PutByte( pBuf, (pUMTSQoS->resBerRatio) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack Delievery of errorneous SDU */
    eRCode = PutByte( pBuf, (pUMTSQoS->deliveryErrSDU) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack transfer delay */
    eRCode = PutLong( pBuf, (pUMTSQoS->transferDelay) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack Traffic priority */
    eRCode = PutLong( pBuf, (pUMTSQoS->trafficPriority) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    return eRCode;
}

/*
 * Packs the SLQSModifyProfile GPRSReqQOS to the QMI message SDU
 *
 * \param  pBuf    [OUT] - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSModifyGPRSReqQos (BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP;

    struct GPRSRequestedQoS *pGPRSRequestedQos = pReq->pGPRSRequestedQos;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pGPRSRequestedQos )
        return eRCode;

    /* pack Precedence class */
    eRCode = PutLong( pBuf, (pGPRSRequestedQos->precedenceClass) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack Delay class */
    eRCode = PutLong( pBuf, (pGPRSRequestedQos->delayClass) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack Reliability class */
    eRCode = PutLong( pBuf, (pGPRSRequestedQos->reliabilityClass) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack Peak throughput class */
    eRCode = PutLong( pBuf, (pGPRSRequestedQos->peakThroughputClass) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack Mean throughput class */
    eRCode = PutLong( pBuf, (pGPRSRequestedQos->meanThroughputClass) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    return eRCode;
}

/*
 * Packs the SLQSModifyProfile GPRSMinQOS to the QMI message SDU
 *
 * \param  pBuf    [OUT] - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSModifyGPRSMinQos(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP;

    struct GPRSRequestedQoS *pGPRSMinimumQoS = pReq->pGPRSMinimumQoS;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pGPRSMinimumQoS)
        return eRCode;

    /* pack Precedence class */
    eRCode = PutLong( pBuf, (pGPRSMinimumQoS->precedenceClass) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack Delay class */
    eRCode = PutLong( pBuf, (pGPRSMinimumQoS->delayClass) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack Reliability class */
    eRCode = PutLong( pBuf, (pGPRSMinimumQoS->reliabilityClass) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack Peak throughput class */
    eRCode = PutLong( pBuf, (pGPRSMinimumQoS->peakThroughputClass) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack Mean throughput class */
    eRCode = PutLong( pBuf, (pGPRSMinimumQoS->meanThroughputClass) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    return eRCode;
}

/*
 * Packs the SLQSModifyProfile Username field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvSLQSModifyUsername(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP;

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
 * Packs the SLQSModifyProfile Password field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvSLQSModifyPassword(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP;

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
 * Packs the SLQSModifyProfile authenticationPref field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSModifyAutPref(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP;

    if ( NULL == pReq->pAuthenticationPref )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pAuthenticationPref) );
}

/*
 * Packs the SLQSModifyProfile IPv4AddrPref field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSModifyAddPref(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP;

    if ( NULL == pReq->pIPv4AddrPref )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutLong( pBuf, *(pReq->pIPv4AddrPref) );
}

/*
 * Packs the SLQSModifyProfile PCSCF Address using PCO field to the
 * QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSModifyAddrUsingPCO(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP;

    if ( NULL == pReq->pPcscfAddrUsingPCO )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pPcscfAddrUsingPCO) );
}

/*
 * Packs the SLQSModifyProfile PDPAccConFlag field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSModifyPDPAccConFlag(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP;

    if ( NULL == pReq->pPdpAccessConFlag )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pPdpAccessConFlag) );
}

/*
 * Packs the SLQSModifyProfile PCSCF Address DHCP field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSModifyPCSCFAddrDHCP(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP;

    if ( NULL == pReq->pPcscfAddrUsingDhcp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pPcscfAddrUsingDhcp) );
}

/*
 * Packs the SLQSModifyProfile IMCNFLAG field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSModifyIMCNFLAG(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP;

    if ( NULL == pReq->pImCnFlag )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pImCnFlag) );
}

/*
 * Packs the SLQSModifyProfile TFTID1Params field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvModifyTftId1Params(BYTE *pBuf, BYTE *pParam)
{
    struct TFTIDParams *pTFTID1Params =
           ((struct QmiWdsSLQSModifyProfileReq *)pParam)->pSlqsModifyProfileIn\
                   ->curProfile.SlqsProfile3GPP.pTFTID1Params;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    USHORT idx = 0;

    if ( !pTFTID1Params )
        return eRCode;

    /* pack filterId */
    eRCode = PutByte( pBuf, (pTFTID1Params->filterId) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack eValid */
    eRCode = PutByte( pBuf, (pTFTID1Params->eValid) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack ipVersion */
    eRCode = PutByte( pBuf, (pTFTID1Params->ipVersion ) );
    if ( eQCWWAN_ERR_NONE != eRCode )
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
                if (eQCWWAN_ERR_NONE != eRCode)
                    return eRCode;
            }
        }
        else
        {
            eRCode = PutLong(pBuf, *( (ULONG*)pTFTID1Params->pSourceIP) );
            if (eQCWWAN_ERR_NONE != eRCode)
                return eRCode;

            /* Pack remaining buffer */
            for( idx = 0; idx < 6 ; idx++ )
            {
                eRCode = PutWord(pBuf, *(pTFTID1Params->pSourceIP +idx) );
                if (eQCWWAN_ERR_NONE != eRCode)
                    return eRCode;
            }

        }
    }

    /* pack sourceIPMask */
    eRCode = PutByte( pBuf, (pTFTID1Params->sourceIPMask) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack nextHeader; */
    eRCode = PutByte( pBuf, (pTFTID1Params->nextHeader) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack the destPortRangeStart  */
    eRCode = PutWord( pBuf, (pTFTID1Params->destPortRangeStart) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

   /* pack the destPortRangeEnd  */
    eRCode = PutWord( pBuf, (pTFTID1Params->destPortRangeEnd) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack the srcPortRangeStart  */
    eRCode = PutWord( pBuf, (pTFTID1Params->srcPortRangeStart) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack srcPortRangeEnd */
    eRCode = PutWord( pBuf, (pTFTID1Params->srcPortRangeEnd) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack IPSECSPI */
    eRCode = PutLong( pBuf, (pTFTID1Params->IPSECSPI) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack tosMask */
    eRCode = PutWord( pBuf, (pTFTID1Params->tosMask ) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack flowLabel */
    eRCode = PutLong( pBuf, (pTFTID1Params->flowLabel) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    return eRCode;
}

/*
 * Packs the SLQSModifyProfile TFTID2Params field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvModifyTftId2Params(BYTE *pBuf, BYTE *pParam)
{
    struct TFTIDParams *pTFTID2Params =
        ((struct QmiWdsSLQSModifyProfileReq *)pParam)->pSlqsModifyProfileIn->\
          curProfile.SlqsProfile3GPP.pTFTID2Params;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    USHORT idx = 0;

    if ( !pTFTID2Params )
        return eRCode;

    /* pack filterId */
    eRCode = PutByte( pBuf, (pTFTID2Params->filterId) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack eValid */
    eRCode = PutByte( pBuf, (pTFTID2Params->eValid) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack ipVersion */
    eRCode = PutByte( pBuf, (pTFTID2Params->ipVersion ) );
    if ( eQCWWAN_ERR_NONE != eRCode )
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
                if (eQCWWAN_ERR_NONE != eRCode)
                    return eRCode;
            }
        }
        else
        {
            eRCode = PutLong(pBuf, *((ULONG*)pTFTID2Params->pSourceIP) );
            if (eQCWWAN_ERR_NONE != eRCode)
                return eRCode;

            /* Pack remaining buffer */
            for( idx = 0; idx < 6 ; idx++ )
            {
                eRCode = PutWord(pBuf, *(pTFTID2Params->pSourceIP)++ );
                if (eQCWWAN_ERR_NONE != eRCode)
                    return eRCode;
            }
        }
    }

    /* pack sourceIPMask */
    eRCode = PutByte( pBuf, (pTFTID2Params->sourceIPMask) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack nextHeader; */
    eRCode = PutByte( pBuf, (pTFTID2Params->nextHeader) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack the destPortRangeStart  */
    eRCode = PutWord( pBuf, (pTFTID2Params->destPortRangeStart) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

   /* pack the destPortRangeEnd  */
    eRCode = PutWord( pBuf, (pTFTID2Params->destPortRangeEnd) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack the srcPortRangeStart  */
    eRCode = PutWord( pBuf, (pTFTID2Params->srcPortRangeStart) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack srcPortRangeEnd */
    eRCode = PutWord( pBuf, (pTFTID2Params->srcPortRangeEnd) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack IPSECSPI */
    eRCode = PutLong( pBuf, (pTFTID2Params->IPSECSPI) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack tosMask */
    eRCode = PutWord( pBuf, (pTFTID2Params->tosMask ) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack flowLabel */
    eRCode = PutLong( pBuf, (pTFTID2Params->flowLabel) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    return eRCode;
}

/*
 * Packs the SLQSModifyProfile PDPContext field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSModifyPDPContext(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP;

    if ( NULL == pReq->pPdpContext )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pPdpContext) );
}

/*
 * Packs the SLQSModifyProfile PDPConSecFlag field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSModifyPDPConSecFlag(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP;

    if ( NULL == pReq->pSecondaryFlag )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pSecondaryFlag) );
}

/*
 * Packs the SLQSModifyProfile PrimaryID field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSModifyPrimaryID(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP;

    if ( NULL == pReq->pPrimaryID )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pPrimaryID) );
}

/*
 * Packs the SLQSModifyProfile pIPv6AddPref field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvModifyIPv6AddPref(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    USHORT idx = 0;

    if ( NULL == pReq->pIPv6AddPref )
    {
        return eQCWWAN_ERR_NONE;
    }
    for( idx = 0; idx < 8 ; idx++ )
    {
        eRCode = PutWordBe(pBuf, *(pReq->pIPv6AddPref)++ );
        if (eQCWWAN_ERR_NONE != eRCode)
            return eRCode;
    }
    return eRCode;
}

/*
 * Packs the SLQSModifyProfile UMTSReqQOS field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvModifyUMTSReqQoSSigInd(BYTE *pBuf, BYTE *pParam)
{
    struct UMTSReqQoSSigInd *pUMTSReqQoSSigInd =
           ((struct QmiWdsSLQSModifyProfileReq *)pParam)->pSlqsModifyProfileIn\
                   ->curProfile.SlqsProfile3GPP.pUMTSReqQoSSigInd;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pUMTSReqQoSSigInd )
        return eRCode;

    /* pack Traffic class */
    eRCode = PutByte( pBuf, (pUMTSReqQoSSigInd->UMTSReqQoS.trafficClass) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack Max uplink bit rate */
    eRCode = PutLong( pBuf, (pUMTSReqQoSSigInd->UMTSReqQoS.maxUplinkBitrate) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack Max downlink bit rate */
    eRCode = PutLong( pBuf, (pUMTSReqQoSSigInd->UMTSReqQoS.maxDownlinkBitrate) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack guaranteed uplink bit rate */
    eRCode = PutLong( pBuf, (pUMTSReqQoSSigInd->UMTSReqQoS.grntUplinkBitrate) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack guaranteed downlink bit rate */
    eRCode = PutLong( pBuf, (pUMTSReqQoSSigInd->UMTSReqQoS.grntDownlinkBitrate) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack Qos delievery order */
    eRCode = PutByte( pBuf, (pUMTSReqQoSSigInd->UMTSReqQoS.qosDeliveryOrder) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack the Max SDU size */
    eRCode = PutLong( pBuf, (pUMTSReqQoSSigInd->UMTSReqQoS.maxSDUSize) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

   /* pack the SDU error ratio */
    eRCode = PutByte( pBuf, (pUMTSReqQoSSigInd->UMTSReqQoS.sduErrorRatio) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack the Residual bit error ratio */
    eRCode = PutByte( pBuf, (pUMTSReqQoSSigInd->UMTSReqQoS.resBerRatio) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack Delievery of errorneous SDU */
    eRCode = PutByte( pBuf, (pUMTSReqQoSSigInd->UMTSReqQoS.deliveryErrSDU) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack transfer delay */
    eRCode = PutLong( pBuf, (pUMTSReqQoSSigInd->UMTSReqQoS.transferDelay) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack Traffic priority */
    eRCode = PutLong( pBuf, (pUMTSReqQoSSigInd->UMTSReqQoS.trafficPriority) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack signal Indication */
    eRCode = PutByte( pBuf, (pUMTSReqQoSSigInd->SigInd) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    return eRCode;
}

/*
 * Packs the SLQSModifyProfile UMTSMinQOS field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvModifyUMTSMinQosSigInd(BYTE *pBuf, BYTE *pParam)
{
    struct UMTSReqQoSSigInd *pUMTSMinQosSigInd =
           ((struct QmiWdsSLQSModifyProfileReq *)pParam)->pSlqsModifyProfileIn\
                   ->curProfile.SlqsProfile3GPP.pUMTSMinQosSigInd;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pUMTSMinQosSigInd )
        return eRCode;

    /* pack Traffic class */
    eRCode = PutByte( pBuf, (pUMTSMinQosSigInd->UMTSReqQoS.trafficClass) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack Max uplink bit rate */
    eRCode = PutLong( pBuf, (pUMTSMinQosSigInd->UMTSReqQoS.maxUplinkBitrate) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack Max downlink bit rate */
    eRCode = PutLong( pBuf, (pUMTSMinQosSigInd->UMTSReqQoS.maxDownlinkBitrate) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack guaranteed uplink bit rate */
    eRCode = PutLong( pBuf, (pUMTSMinQosSigInd->UMTSReqQoS.grntUplinkBitrate) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack guaranteed downlink bit rate */
    eRCode = PutLong( pBuf, (pUMTSMinQosSigInd->UMTSReqQoS.grntDownlinkBitrate) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack Qos delievery order */
    eRCode = PutByte( pBuf, (pUMTSMinQosSigInd->UMTSReqQoS.qosDeliveryOrder) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack the Max SDU size */
    eRCode = PutLong( pBuf, (pUMTSMinQosSigInd->UMTSReqQoS.maxSDUSize) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

   /* pack the SDU error ratio */
    eRCode = PutByte( pBuf, (pUMTSMinQosSigInd->UMTSReqQoS.sduErrorRatio) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack the Residual bit error ratio */
    eRCode = PutByte( pBuf, (pUMTSMinQosSigInd->UMTSReqQoS.resBerRatio) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack Delievery of errorneous SDU */
    eRCode = PutByte( pBuf, (pUMTSMinQosSigInd->UMTSReqQoS.deliveryErrSDU) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack transfer delay */
    eRCode = PutLong( pBuf, (pUMTSMinQosSigInd->UMTSReqQoS.transferDelay) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack Traffic priority */
    eRCode = PutLong( pBuf, (pUMTSMinQosSigInd->UMTSReqQoS.trafficPriority) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack Signal Indiaction priority */
    eRCode = PutByte( pBuf, (pUMTSMinQosSigInd->SigInd) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    return eRCode;
}

/*
 * Packs the SLQSModifyProfile PrimaryDNSIPv6addpref field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvModifyPriDNSIPv6addpref(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    USHORT idx = 0;

    if ( NULL == pReq->pPriDNSIPv6addpref )
    {
        return eQCWWAN_ERR_NONE;
    }
    for( idx = 0; idx < 8 ; idx++ )
    {
        eRCode = PutWordBe(pBuf, *(pReq->pPriDNSIPv6addpref)++ );
        if (eQCWWAN_ERR_NONE != eRCode)
            return eRCode;
    }
    return eRCode;
}

/*
 * Packs the SLQSModifyProfile SecondaryDNSIPv6addpref field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvModifySecDNSIPv6addpref(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    USHORT idx = 0;

    if ( NULL == pReq->pSecDNSIPv6addpref )
    {
        return eQCWWAN_ERR_NONE;
    }
    for( idx = 0; idx < 8 ; idx++ )
    {
        eRCode = PutWordBe(pBuf, *(pReq->pSecDNSIPv6addpref)++ );
        if (eQCWWAN_ERR_NONE != eRCode)
            return eRCode;
    }
    return eRCode;
}

/*
 * Packs the SLQSModifyProfile AddrAllocPref field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvModifyAddrAllocPref(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP;

    if ( NULL == pReq->pAddrAllocPref)
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pAddrAllocPref) );
}

/*
 * Packs the SLQSModifyProfile QosClassID field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvModifyQosClassID(BYTE *pBuf, BYTE *pParam)
{
    struct QosClassID *pQosClassID=
           ((struct QmiWdsSLQSModifyProfileReq *)pParam)->pSlqsModifyProfileIn\
                   ->curProfile.SlqsProfile3GPP.pQosClassID;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pQosClassID )
        return eRCode;

    /* pack QCI class */
    eRCode = PutByte( pBuf, (pQosClassID->QCI) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Pack GDlBitRate */
    eRCode = PutLong( pBuf, (pQosClassID->gDlBitRate) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Pack Max downlink bit rate */
    eRCode = PutLong( pBuf, (pQosClassID->maxDlBitRate) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack gUlBitRate */
    eRCode = PutLong( pBuf, (pQosClassID->gUlBitRate) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* pack maxUlBitRate */
    eRCode = PutLong( pBuf, (pQosClassID->maxUlBitRate) );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    return eRCode;
}

/*
 * Packs the SLQSModifyProfile NegoDnsSrvrPref field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvModifyNegoDnsSrvrPref(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pNegoDnsSrvrPref )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pNegoDnsSrvrPref) );
}

/*
 * Packs the SLQSModifyProfile pppSessCloseTimerDO field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvModifyPPPSessCloseTimerDo(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pPppSessCloseTimerDO )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutLong( pBuf, *(pReq->pPppSessCloseTimerDO) );
}

/*
 * Packs the SLQSModifyProfile pppSessCloseTimer1x field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvModifyPPPSessCloseTimer1X(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pPppSessCloseTimer1x )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutLong( pBuf, *(pReq->pPppSessCloseTimer1x) );
}

/*
 * Packs the SLQSModifyProfile AllowLinger field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvModifyAllowLinger(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pAllowLinger )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pAllowLinger) );
}

/*
 * Packs the SLQSModifyProfile LcpAckTimeout field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvModifyLCPAckTimeOut(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pLcpAckTimeout )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutWord( pBuf, *(pReq->pLcpAckTimeout) );
}

/*
 * Packs the SLQSModifyProfile IpcpAckTimeout field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvModifyIPCPAckTimeOut(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pIpcpAckTimeout )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutWord( pBuf, *(pReq->pIpcpAckTimeout) );
}

/*
 * Packs the SLQSModifyProfile pAuthTimeout field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvModifyAuthTimeOut(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pAuthTimeout )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutWord( pBuf, *(pReq->pAuthTimeout) );
}

/*
 * Packs the SLQSModifyProfile LcpCreqRetryCount field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvModifyLCPCreqRetCount(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pLcpCreqRetryCount )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pLcpCreqRetryCount) );
}

/*
 * Packs the SLQSModifyProfile IpcpCreqRetryCount field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvModifyIPCPCreqRetCount(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pIpcpCreqRetryCount )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pIpcpCreqRetryCount) );
}

/*
 * Packs the SLQSModifyProfile AuthRetryCount field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvModifyAuthRetCount(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pAuthRetryCount )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pAuthRetryCount) );
}

/*
 * Packs the SLQSModifyProfile AuthProtocol field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvModifyAuthProtocol(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pAuthProtocol )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pAuthProtocol) );
}

/*
 * Packs the SLQSModifyProfile UserId field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvModifyUserId(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP2;

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
 * Packs the SLQSModifyProfile AuthPassword field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvModifyAuthPassword(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP2;

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
 * Packs the SLQSModifyProfile DataRate field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvModifyDatarate(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pDataRate )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pDataRate) );
}

/*
 * Packs the SLQSModifyProfile AppType field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvModifyAppType(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pAppType )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutLong( pBuf, *(pReq->pAppType) );
}

/*
 * Packs the SLQSModifyProfile DataMode field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvModifyDataMode(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pDataMode )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pDataMode) );
}

/*
 * Packs the SLQSModifyProfile AppPriority field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvModifyAppPriority(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pAppPriority )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pAppPriority) );
}

/*
 * Packs the SLQSModifyProfile ApnString field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildModifyAPNString(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP2;
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
 * Packs the SLQSModifyProfile PdnType field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildModifyPDNtype(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pPdnType )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pPdnType) );
}

/*
 * Packs the SLQSModifyProfile IsPcscfAddressNedded field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvModifyISPCSCFAddNedded(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pIsPcscfAddressNedded )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pIsPcscfAddressNedded) );
}

/*
 * Packs the SLQSModifyProfile PrimaryV4DnsAddress field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvModifyPriV4DNSAddress(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pPrimaryV4DnsAddress )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutLong( pBuf, *(pReq->pPrimaryV4DnsAddress) );
}

/*
 * Packs the SLQSModifyProfile SecondaryV4DnsAddress field to the
 * QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvModifySecV4DnsAddress (BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP2;

    if ( NULL == pReq->pSecondaryV4DnsAddress )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutLong( pBuf, *(pReq->pSecondaryV4DnsAddress) );
}

/*
 * Packs the SLQSModifyProfile PriV6DnsAddress field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvModifyPriV6DnsAddress(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP2;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    USHORT idx = 0;

    if ( NULL == pReq->pPriV6DnsAddress )
    {
        return eQCWWAN_ERR_NONE;
    }
    for( idx = 0; idx < 8 ; idx++ )
    {
        eRCode = PutWordBe(pBuf, *(pReq->pPriV6DnsAddress)++ );
        if (eQCWWAN_ERR_NONE != eRCode)
            return eRCode;
    }
    return eRCode;
}

/*
 * Packs the SLQSModifyProfile PriV6DnsAddress field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvModifySecV6DnsAddress(BYTE *pBuf, BYTE *pParam)
{
    struct Profile3GPP2 *pReq = &((struct QmiWdsSLQSModifyProfileReq *)\
           pParam)->pSlqsModifyProfileIn->curProfile.SlqsProfile3GPP2;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    USHORT idx = 0;

    if ( NULL == pReq->pSecV6DnsAddress )
    {
        return eQCWWAN_ERR_NONE;
    }

    for( idx = 0; idx < 8 ; idx++ )
    {
        eRCode = PutWordBe(pBuf, *(pReq->pSecV6DnsAddress)++ );
        if (eQCWWAN_ERR_NONE != eRCode)
            return eRCode;
    }
    return eRCode;
}

/*
 * This function packs the SLQSModifyProfile parameters to the QMI message SDU
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
enum eQCWWANError PkQmiWdsSLQSModifyProfile(
    WORD                   *pMlength,
    BYTE                   *pParamField,
    struct ModifyProfileIn *pModifyProfileIn )
{
    struct QmiWdsSLQSModifyProfileReq req;
    enum eQCWWANError   eRCode = eQCWWAN_ERR_QMI_INVALID_PROFILE_TYPE;

    /* Profile type is mandatory parameter */
    if(!pModifyProfileIn->pProfileType)
        return eRCode;

    slmemset((char *)&req, 0, sizeof (struct QmiWdsSLQSModifyProfileReq));
    req.pSlqsModifyProfileIn = pModifyProfileIn;

    if ( PROFILE_3GPP == *(pModifyProfileIn->pProfileType) )
    {
        static struct qmTlvBuilderItem map[] =
        {
            { eTLV_PROF_TYPE_MOD,            &BuildTlvSLQSModifyproftype},
            { eTLV_PROF_NAME_MOD,            &BuildTlvSLQSModifyprofilename},
            { eTLV_PDP_TYPE_MOD,             &BuildTlvSLQSModifyPDPType},
            { eTLV_PDP_HEADER_COMPR_TYPE,    &BuildTlvSLQSModifyPDPHdComprType},
            { eTLV_PDP_DATA_COMPR_TYPE,      &BuildTlvSLQSModifyPDPDataComType},
            { eTLV_APN_NAME_MOD,             &BuildTlvSLQSModifyApnName},
            { eTLV_PRI_DNS_IPV4_ADDR_PREF,   &BuildTlvSLQSModifyPRIIpv4Pref},
            { eTLV_SEC_DNS_IPV4_ADDR_PREF,   &BuildTlvSLQSModifySecIpv4Pref},
            { eTLV_UMTS_REQ_QOS,             &BuildTlvSLQSModifyUMTSReqQOS},
            { eTLV_UMTS_MIN_QOS,             &BuildTlvSLQSModifyUMTSMinQos},
            { eTLV_GPRS_REQ_QOS,             &BuildTlvSLQSModifyGPRSReqQos},
            { eTLV_GPRS_MIN_QOS,             &BuildTlvSLQSModifyGPRSMinQos},
            { eTLV_USERNAME_MOD,             &BuildTlvSLQSModifyUsername},
            { eTLV_PASSWORD_MOD,             &BuildTlvSLQSModifyPassword},
            { eTLV_AUTHENTICATION_PREF,      &BuildTlvSLQSModifyAutPref},
            { eTLV_IPV4_ADDR_PREF,           &BuildTlvSLQSModifyAddPref},
            { eTLV_PCSCF_ADDR_USI_PCO_MOD,   &BuildTlvSLQSModifyAddrUsingPCO},
            { eTLV_PDP_ACCESS_CONT_FLAG,     &BuildTlvSLQSModifyPDPAccConFlag},
            { eTLV_PCSCF_ADDR_USI_DHCP_MOD,  &BuildTlvSLQSModifyPCSCFAddrDHCP},
            { eTLV_IM_CN_FLAG_MOD,           &BuildTlvSLQSModifyIMCNFLAG},
            { eTLV_TFT_ID1_PARAM,            &BuildTlvModifyTftId1Params},
            { eTLV_TFT_ID2_PARAM,            &BuildTlvModifyTftId2Params},
            { eTLV_PDP_CONTEXT_MOD,          &BuildTlvSLQSModifyPDPContext},
            { eTLV_PDP_CONTEXT_SEC_FLAG,     &BuildTlvSLQSModifyPDPConSecFlag},
            { eTLV_PDP_CONTEXT_PRI_ID,       &BuildTlvSLQSModifyPrimaryID},
            { eTLV_IPV6_ADDRESS_PREF,        &BuildTlvModifyIPv6AddPref},
            { eTLV_UMTS_REQ_QOS_SIG_IND_MOD, &BuildTlvModifyUMTSReqQoSSigInd},
            { eTLV_UMTS_MIN_QOS_SIG_IND_MOD, &BuildTlvModifyUMTSMinQosSigInd},
            { eTLV_PRI_DNS_IPV6_ADDR_PREF,   &BuildTlvModifyPriDNSIPv6addpref},
            { eTLV_SEC_DNS_IPV6_ADDR_PREF,   &BuildTlvModifySecDNSIPv6addpref},
            { eTLV_ADDR_ALLOC_PREF,          &BuildTlvModifyAddrAllocPref},
            { eTLV_3GPP_LTE_QOS_PARAMS_MOD,  &BuildTlvModifyQosClassID},
            { eTLV_TYPE_INVALID,             NULL } /* Important Sentinel
                                                     * Signifies last item
                                                     * in map
                                                     */
        };
        eRCode = qmbuild( pParamField,
                         (BYTE *)&req,
                          map,
                          eQMI_WDS_MODIFY_PROFILE,
                          pMlength );
    }
    else if (PROFILE_3GPP2 == *(pModifyProfileIn->pProfileType))
    {
        static struct qmTlvBuilderItem map[] =
        {
            { eTLV_3GPP2_PROF_TYPE,         &BuildTlvSLQSModifyproftype},
            { eTLV_NEGO_DNS_SERVER_PREF,    &BuildTlvModifyNegoDnsSrvrPref},
            { eTLV_PPP_SESS_CLOSE_TIMER_DO, &BuildTlvModifyPPPSessCloseTimerDo},
            { eTLV_PPP_SESS_CLOSE_TIMER_1X, &BuildTlvModifyPPPSessCloseTimer1X},
            { eTLV_ALLOW_LINGER_MOD,        &BuildTlvModifyAllowLinger},
            { eTLV_LCP_ACK_TIMEOUT_MOD,     &BuildTlvModifyLCPAckTimeOut},
            { eTLV_IPCP_ACK_TIMEOUT_MOD,    &BuildTlvModifyIPCPAckTimeOut},
            { eTLV_AUTH_TIMEOUT_MOD,        &BuildTlvModifyAuthTimeOut},
            { eTLV_LCP_CREQ_RET_COUNT,      &BuildTlvModifyLCPCreqRetCount},
            { eTLV_IPCP_CREQ_RET_COUNT,     &BuildTlvModifyIPCPCreqRetCount},
            { eTLV_AUTH_RET_COUNT,          &BuildTlvModifyAuthRetCount},
            { eTLV_AUTH_PROTOCOl_MOD,       &BuildTlvModifyAuthProtocol},
            { eTLV_USER_ID_MOD,             &BuildTlvModifyUserId},
            { eTLV_AUTH_PASS_MOD,           &BuildTlvModifyAuthPassword},
            { eTLV_DATA_RATE_MOD,           &BuildTlvModifyDatarate},
#if 0
            { eTLV_APP_TYPE_MOD,            &BuildTlvModifyAppType},
            { eTLV_APP_PRIOR,               &BuildTlvModifyAppPriority},
#endif
            { eTLV_DATA_MODE_MOD,           &BuildTlvModifyDataMode},
            { eTLV_APN_STRING_MOD,          &BuildModifyAPNString},
            { eTLV_PDN_TYPE_MOD,            &BuildModifyPDNtype},
            { eTLV_IS_PCSCF_ADDR_NEDDED,    &BuildTlvModifyISPCSCFAddNedded},
            { eTLV_PRIMARY_V4_DNS_ADDR,     &BuildTlvModifyPriV4DNSAddress},
            { eTLV_SECONDARY_V4_DNS_ADDR,   &BuildTlvModifySecV4DnsAddress},
            { eTLV_PRIMARY_V6_DNS_ADDR,     &BuildTlvModifyPriV6DnsAddress},
            { eTLV_SECONDARY_V6_DNS_ADDR,   &BuildTlvModifySecV6DnsAddress},
            { eTLV_TYPE_INVALID,            NULL} /* Important Sentinel
                                                   * Signifies last item
                                                   * in map
                                                   */
        };
        eRCode = qmbuild( pParamField,
                          (BYTE *)&req,
                          map,
                          eQMI_WDS_MODIFY_PROFILE,
                          pMlength );
    }
    return eRCode;
}

/*****************************************************************************
* Response handling
******************************************************************************/
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
enum eQCWWANError qmUnpackTlvProfileExtErr2(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsSLQSModifyProfileResp *pLresp =
                        (struct QmiWdsSLQSModifyProfileResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pLresp->pSlqsModifyProfileOut->pExtErrorCode )
    {
        return eRCode;
    }

    /* Extract the extended error code */
    return GetWord( pTlvData, pLresp->pSlqsModifyProfileOut->pExtErrorCode );
}

/*
 * This function unpacks the SLQSModifyProfile response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UpkQmiWdsSLQSModifyProfile(
    BYTE                               *pMdmResp,
    struct QmiWdsSLQSModifyProfileResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,                 &qmUnpackTlvResultCode },
        { eTLV_PROFILE_EXTENDED_ERRORCODE2, &qmUnpackTlvProfileExtErr2 },
        { eTLV_TYPE_INVALID,                NULL }/* Important. Sentinel.
                                                   * Signifies last item in map.
                                                   */
    };
    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_WDS_MODIFY_PROFILE );
    return eRCode;
}
