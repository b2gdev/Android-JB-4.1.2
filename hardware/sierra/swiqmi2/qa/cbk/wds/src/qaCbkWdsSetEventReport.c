/*
 *
 * \ingroup : cbk
 *
 * \file    : qaWdsSetEventReport.c
 *
 * \brief   : Contains Packing and UnPacking routines for the
 *            WDS_SET_EVENT_REPORT message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCbkWdsSetEventReport.h"

/* Functions */

/* Request handlers */

/*
 * This function packs the Total Byte field to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE    - Increment successful
 * \return eQCWWAN_ERR_MEMORY   - Increment beyond allowed size attempted
 *
 */
local enum eQCWWANError BuildTlvByteTotals(
    BYTE    *pBuf,
    BYTE    *pParam )
{
    struct QmiCbkWdsSetEventReportReq *pReq =
        (struct QmiCbkWdsSetEventReportReq *)pParam;
    ULONG lStatusMask;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* If the parameter has not been set do not fill the TLV */
    if( QMI_CBK_PARAM_NOCHANGE == pReq->eByteTotals )
        return eRCode;

    /* Insert the interval parameter */
    eRCode = PutByte ( pBuf, pReq->interval);
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    /* Insert the status mask */
    if( QMI_CBK_PARAM_SET == pReq->eByteTotals )
        lStatusMask = STATUS_MASK_TX_BYTES_OK | STATUS_MASK_RX_BYTES_OK;
    else if( QMI_CBK_PARAM_RESET == pReq->eByteTotals )
        lStatusMask = 0;

    eRCode = PutLong ( pBuf, lStatusMask );
    return eRCode;
}

/*
 * This function packs the Mobile IP field to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE    - Increment successful
 * \return eQCWWAN_ERR_MEMORY   - Increment beyond allowed size attempted
 *
 */
local enum eQCWWANError BuildTlvMobileIP(
    BYTE    *pBuf,
    BYTE    *pParam )
{
    struct QmiCbkWdsSetEventReportReq *pReq =
        (struct QmiCbkWdsSetEventReportReq *)pParam;

    /* If the parameter has not been set do not fill the TLV */
    if (QMI_CBK_PARAM_NOCHANGE == pReq->eMobileIP)
        return eQCWWAN_ERR_NONE;

    return PutByte ( pBuf, pReq->eMobileIP );
}

/*
 * This function packs the Dormancy Status field to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE    - Increment successful
 * \return eQCWWAN_ERR_MEMORY   - Increment beyond allowed size attempted
 *
 */
local enum eQCWWANError BuildTlvDormancyStatus(
    BYTE    *pBuf,
    BYTE    *pParam )
{
    struct QmiCbkWdsSetEventReportReq *pReq =
        (struct QmiCbkWdsSetEventReportReq *)pParam;

    /* If the parameter has not been set do not fill the TLV */
    if (QMI_CBK_PARAM_NOCHANGE == pReq->eDormancyStatus)
        return eQCWWAN_ERR_NONE;

    return PutByte ( pBuf, pReq->eDormancyStatus);
}

/*
 * This function packs the Data Bearer field to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE    - Increment successful
 * \return eQCWWAN_ERR_MEMORY   - Increment beyond allowed size attempted
 *
 */
local enum eQCWWANError BuildTlvDataBearer(
    BYTE    *pBuf,
    BYTE    *pParam )
{
    struct QmiCbkWdsSetEventReportReq *pReq =
        (struct QmiCbkWdsSetEventReportReq *)pParam;

    /* If the parameter has not been set do not fill the TLV */
    if (QMI_CBK_PARAM_NOCHANGE == pReq->eDataBearer)
        return eQCWWAN_ERR_NONE;

    return PutByte ( pBuf, pReq->eDataBearer );
}

/*
 * This function packs the Current Data Bearer field to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE    - Increment successful
 * \return eQCWWAN_ERR_MEMORY   - Increment beyond allowed size attempted
 *
 */
local enum eQCWWANError BuildTlvCurrentDataBearer(
    BYTE    *pBuf,
    BYTE    *pParam )
{
    struct QmiCbkWdsSetEventReportReq *pReq =
        (struct QmiCbkWdsSetEventReportReq *)pParam;

    /* If the parameter has not been set do not fill the TLV */
    if (QMI_CBK_PARAM_NOCHANGE == pReq->eCurrentDataBearer)
        return eQCWWAN_ERR_NONE;

    return PutByte ( pBuf, pReq->eCurrentDataBearer );
}

/*
 * This function packs the WDS Set Event Report parameters to the
 * QMI message SDU
 *
 * \param  pMlength[OUT]
 *         - Total length of built message.
 *
 * \param  eDataBearer[IN]
 *         - Data Bearer Technology Indicator
 *
 * \param  eDormancyStatus[IN]
 *         - Dormancy Status Indicator
 *
 * \param  eMobileIP[IN]
 *         - MIP Status Indicator
 *
 * \param  eByteTotals[IN]
 *         - Transfer statistics indicator
 *
 * \param  eCurrentDataBearer[IN]
 *         - Current Data Bearer Technology Indicator
 *
 * \param  pParamField[OUT]
 *         - Pointer to storage into which the packed
 *         data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Increment beyond allowed size attempted
 *
 */
package enum eQCWWANError PkQmiCbkWdsSetEventReport(
    WORD                   *pMlength,
    enum eQmiCbkSetStatus  eDataBearer,
    enum eQmiCbkSetStatus  eDormancyStatus,
    enum eQmiCbkSetStatus  eMobileIP,
    enum eQmiCbkSetStatus  eByteTotals,
    enum eQmiCbkSetStatus  eCurrentDataBearer,
    BYTE                   interval,
    BYTE                   *pParamField )
{
    static struct qmTlvBuilderItem map[] =
    {
         { eTLV_DATA_BEARER_TECH, BuildTlvDataBearer },
         { eTLV_DORMANCY_STATUS_REQ,      BuildTlvDormancyStatus },
         { eTLV_MOBILE_IP,                BuildTlvMobileIP },
         { eTLV_TRANSFER_STATISTICS,      BuildTlvByteTotals },
         { eTLV_CURRENT_DATA_BEARER_TECH, BuildTlvCurrentDataBearer },
         { eTLV_TYPE_INVALID,             NULL }  /* Important. Sentinel.
                                                   * Signifies last item in map.
                                                   */
    };

    struct QmiCbkWdsSetEventReportReq req;
    slmemset( (char *)&req,
              0,
              sizeof (struct QmiCbkWdsSetEventReportReq) );

    req.eDataBearer        = eDataBearer;
    req.eDormancyStatus    = eDormancyStatus;
    req.eMobileIP          = eMobileIP;
    req.eByteTotals        = eByteTotals;
    req.eCurrentDataBearer = eCurrentDataBearer;
    req.interval           = interval;

    enum eQCWWANError eRCode;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_WDS_SET_EVENT,
                      pMlength );
    return eRCode;
}

/* Response handlers */

/*
 *
 * This function unpacks the WDS_SET_EVENT_REPORT response
 * message to a user-provided response structure.
 *
 * \param     pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 *
 */
package enum eQCWWANError UpkQmiCbkWdsSetEventReport(
    BYTE    *pMdmResp,
    struct  QmiCbkWdsSetEventReportResp *pApiResp)
{
    enum eQCWWANError eRCode;

    /* The unpack of the Callback contains only result code */
    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE       ,&qmUnpackTlvResultCode },
        { eTLV_TYPE_INVALID      ,NULL }  /* Important. Sentinel.
                                           * Signifies last item in map.
                                           */
    };
    eRCode =  qmunpackresp( pMdmResp,
                           (BYTE*)pApiResp,
                           map,
                           eQMI_WDS_SET_EVENT);
    return eRCode;
}

