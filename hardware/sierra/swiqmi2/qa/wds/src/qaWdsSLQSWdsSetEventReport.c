/*
 * \ingroup wds
 *
 * \file    qaWdsSLQSWdsSetEventReport.c
 *
 * \brief   Contains Packing and UnPacking routines for the eQMI_WDS_SET_EVENT
 *          message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaWdsSLQSWdsSetEventReport.h"

/*****************************************************************************
 * Request handling
 *****************************************************************************/
/*
 * This function packs the Current Channel Rate Indicator to the QMI message
 * SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvCurrChannelRateInd( BYTE *pBuf, BYTE *pParam )
{
    wdsSetEventReportReq *pReq = (wdsSetEventReportReq *)pParam;

    if ( NULL == pReq->pCurrChannelRateInd )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Current Channel Rate Indicator */
    return PutByte( pBuf, *pReq->pCurrChannelRateInd );
}

/*
 * This function packs the Transfer Statistics Indicator to the QMI message
 * SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvTransferStatisticsInd( BYTE *pBuf, BYTE *pParam )
{
    TrStatInd *pReq = ((wdsSetEventReportReq *)pParam)->pTransferStatInd;
    enum eQCWWANError eRCode;

    if ( NULL == pReq )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Transfer Status Indicator */
    eRCode = PutByte( pBuf, pReq->statsPeriod );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return PutLong( pBuf, pReq->statsMask );
}

/*
 * This function packs the Data Bearer Technology Indicator to the QMI message
 * SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvDataBearerTechInd( BYTE *pBuf, BYTE *pParam )
{
    wdsSetEventReportReq *pReq = (wdsSetEventReportReq *)pParam;

    if ( NULL == pReq->pDataBearerTechInd )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Data Bearer Technology Indicator */
    return PutByte( pBuf, *pReq->pDataBearerTechInd );
}

/*
 * This function packs the Dormancy Status Indicator to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvDormancyStatusInd( BYTE *pBuf, BYTE *pParam )
{
    wdsSetEventReportReq *pReq = (wdsSetEventReportReq *)pParam;

    if ( NULL == pReq->pDormancyStatusInd )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Dormancy Status Indicator */
    return PutByte( pBuf, *pReq->pDormancyStatusInd );
}

/*
 * This function packs the MIP Status Indicator to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvMIPStatusInd( BYTE *pBuf, BYTE *pParam )
{
    wdsSetEventReportReq *pReq = (wdsSetEventReportReq *)pParam;

    if ( NULL == pReq->pMIPStatusInd )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert MIP Status Indicator */
    return PutByte( pBuf, *pReq->pMIPStatusInd );
}

/*
 * This function packs the Current Data Bearer Technology Indicator to the QMI
 * message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvCurrDataBearerTechInd( BYTE *pBuf, BYTE *pParam )
{
    wdsSetEventReportReq *pReq = (wdsSetEventReportReq *)pParam;

    if ( NULL == pReq->pCurrDataBearerTechInd )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Current Data Bearer Technology Indicator */
    return PutByte( pBuf, *pReq->pCurrDataBearerTechInd );
}

/*
 * This function packs the Data Call Status Change Indicator to the QMI message
 * SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvDataCallStatusChngeInd( BYTE *pBuf, BYTE *pParam )
{
    wdsSetEventReportReq *pReq = (wdsSetEventReportReq *)pParam;

    if ( NULL == pReq->pDataCallStatusChangeInd )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Data Call Status Change Indicator */
    return PutByte( pBuf, *pReq->pDataCallStatusChangeInd );
}

/*
 * This function packs the Current Preferred Data System Indicator to the
 * QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvCurrPrefDataSysInd( BYTE *pBuf, BYTE *pParam )
{
    wdsSetEventReportReq *pReq = (wdsSetEventReportReq *)pParam;

    if ( NULL == pReq->pCurrPrefDataSysInd )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Current Preferred Data System Indicator */
    return PutByte( pBuf, *pReq->pCurrPrefDataSysInd );
}

/*
 * This function packs the EV-DO Page Monitor Period Change Indicator to the
 * QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvEVDOPageMonPerChngeInd( BYTE *pBuf, BYTE *pParam )
{
    wdsSetEventReportReq *pReq = (wdsSetEventReportReq *)pParam;

    if ( NULL == pReq->pEVDOPageMonPerChangeInd )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert EV-DO Page Monitor Period Change Indicator */
    return PutByte( pBuf, *pReq->pEVDOPageMonPerChangeInd );
}

/*
 * This function packs the Data System Status Change Indicator to the QMI
 * message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvDataSysStatusChngeInd( BYTE *pBuf, BYTE *pParam )
{
    wdsSetEventReportReq *pReq = (wdsSetEventReportReq *)pParam;

    if ( NULL == pReq->pDataSystemStatusChangeInd )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert Data System Status Change Indicator */
    return PutByte( pBuf, *pReq->pDataSystemStatusChangeInd );
}

/*
 * This function packs the SLQS Set Event Report request parameters to the QMI
 * message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaWdsSLQSWdsSetEventReport.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiWdsSLQSSetEventReport(
    WORD                 *pMlength,
    BYTE                 *pParamField,
    wdsSetEventReportReq *pSetEventReportReq )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_CURR_CHANNEL_RATE_IND,        &BuildTlvCurrChannelRateInd },
        { eTLV_TRANSFER_STATS_IND,           &BuildTlvTransferStatisticsInd },
        { eTLV_DATA_BEARER_TECH_IND,         &BuildTlvDataBearerTechInd },
        { eTLV_DORMANCY_STATUS_IND,          &BuildTlvDormancyStatusInd },
        { eTLV_MIP_STATUS_IND,               &BuildTlvMIPStatusInd },
        { eTLV_CURR_DATA_BEARER_TECH_IND,    &BuildTlvCurrDataBearerTechInd },
        { eTLV_DATA_CALL_STATUS_CHANGE_IND,  &BuildTlvDataCallStatusChngeInd },
        { eTLV_CURR_PREF_DATA_SYS_IND,       &BuildTlvCurrPrefDataSysInd },
        { eTLV_EVDO_PAGE_MON_PER_CHANGE_IND, &BuildTlvEVDOPageMonPerChngeInd },
        { eTLV_DATA_SYS_STATUS_CHANGE_IND,   &BuildTlvDataSysStatusChngeInd },
        { eTLV_TYPE_INVALID,                 NULL } /* Important. Sentinel.
                                                     * Signifies last item in map
                                                     */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pSetEventReportReq,
                      map,
                      eQMI_WDS_SET_EVENT,
                      pMlength );
    return eRCode;
}

/*****************************************************************************
 * Response handling
 *****************************************************************************/
/*
 * This function unpacks the SLQS Set Event Report response message to a user-
 * provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiWdsSLQSSetEventReport(
    BYTE                                *pMdmResp,
    struct QmiWdsSLQSSetEventReportResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

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
                           eQMI_WDS_SET_EVENT );
    return eRCode;
}
