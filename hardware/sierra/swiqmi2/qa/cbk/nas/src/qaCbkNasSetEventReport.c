/*
 * \ingroup cbk
 *
 * \file qaNasSetEventReport.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        NAS_SET_EVENT_REPORT message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCbkNasSetEventReport.h"

/* Functions */

/* Request handlers */

/*
 * This function packs the Signal Strength field to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
local enum eQCWWANError BuildTlvSignalStrength(
    BYTE *pBuf,
    BYTE *pParam )
{
    struct QmiCbkNasSetEventReportReq *pReq =
        (struct QmiCbkNasSetEventReportReq *)pParam;

    enum eQCWWANError eRCode         = eQCWWAN_ERR_NONE;
    BYTE              thresholdsSize = 0;

    /* If the parameter has not been set do not fill the TLV */
    if (QMI_CBK_PARAM_NOCHANGE == pReq->eSignalStrength)
        return eRCode;

    eRCode = PutByte( pBuf, pReq->eSignalStrength );

    if ( eQCWWAN_ERR_NONE != eRCode )
         return eRCode;

    /* For disabling Signal Strength info pack zero as threshold size */
    if( QMI_CBK_PARAM_RESET == pReq->eSignalStrength )
    {
        eRCode = PutByte( pBuf, thresholdsSize );
        return eRCode;
    }

    eRCode = PutByte( pBuf, pReq->thresholdsSize);
    if ( eQCWWAN_ERR_NONE != eRCode )
         return eRCode;

    if( pReq->pThresholds == NULL )
         return eRCode;

    while( (pReq->thresholdsSize) && (eRCode == eQCWWAN_ERR_NONE) )
    {

        eRCode = PutByte(pBuf,*(pReq->pThresholds)++);
        (pReq->thresholdsSize)--;
    }
    return eRCode;
}

/*
 * This function packs the RF Band field to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
local enum eQCWWANError BuildTlvRFBand(
    BYTE *pBuf,
    BYTE *pParam )
{
    struct QmiCbkNasSetEventReportReq *pReq =
        (struct QmiCbkNasSetEventReportReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* If the parameter has not been set do not fill the TLV */
    if (QMI_CBK_PARAM_NOCHANGE == pReq->eRFBand)
        return eRCode;

    eRCode = PutByte( pBuf, pReq->eRFBand);
    return eRCode;
}

/*
 * This function packs the Reject Reason field to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
local enum eQCWWANError BuildTlvRegRejectReason(
    BYTE *pBuf,
    BYTE *pParam )
{
    struct QmiCbkNasSetEventReportReq *pReq =
        (struct QmiCbkNasSetEventReportReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* If the parameter has not been set do not fill the TLV */
    if (QMI_CBK_PARAM_NOCHANGE == pReq->eRegRejectReason)
         return eRCode;

    eRCode = PutByte( pBuf, pReq->eRegRejectReason );
    return eRCode;
}

/*
 * This function packs the RSSI Indicator field to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
local enum eQCWWANError BuildTlvRSSIIndicator(
    BYTE *pBuf,
    BYTE *pParam )
{
    struct QmiCbkNasSetEventReportReq *pReq =
        (struct QmiCbkNasSetEventReportReq *)pParam;

    enum eQCWWANError eRCode    = eQCWWAN_ERR_NONE;
    BYTE              rxSSDelta = 0;

    /* If the parameter has not been set do not fill the TLV */
    if ( QMI_CBK_PARAM_NOCHANGE == pReq->eRSSIIndicator )
    {
        return eRCode;
    }

    /* Enable the reporting of RSSI indication */
    eRCode = PutByte( pBuf, pReq->eRSSIIndicator );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* For disabling RSSI pack zero as Signal strength delta */
    if( QMI_CBK_PARAM_RESET == pReq->eRSSIIndicator )
    {
        eRCode = PutByte( pBuf, rxSSDelta );
        return eRCode;
    }

    /* Put RSSI delta value */
    eRCode = PutByte( pBuf,
                     pReq->pSLQSSignalStrengthsIndReq->rxSignalStrengthDelta );
    return eRCode;
}

/*
 * This function packs the ECIO Indicator field to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
local enum eQCWWANError BuildTlvECIOIndicator(
    BYTE *pBuf,
    BYTE *pParam )
{
    struct QmiCbkNasSetEventReportReq *pReq =
        (struct QmiCbkNasSetEventReportReq *)pParam;

    enum eQCWWANError eRCode    = eQCWWAN_ERR_NONE;
    BYTE              ecioDelta = 0;

    /* If the parameter has not been set do not fill the TLV */
    if (QMI_CBK_PARAM_NOCHANGE == pReq->eECIOIndicator)
    {
        return eRCode;
    }

    /* Enable the reporting of ECIO indication */
    eRCode = PutByte( pBuf, pReq->eECIOIndicator );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* For disabling ECIO pack zero as ECIO delta */
    if( QMI_CBK_PARAM_RESET == pReq->eECIOIndicator )
    {
        eRCode = PutByte( pBuf, ecioDelta );
        return eRCode;
    }

    /* Put ECIO delta value */
    eRCode = PutByte( pBuf, pReq->pSLQSSignalStrengthsIndReq->ecioDelta );
    return eRCode;
}

/*
 * This function packs the IO Indicator to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
local enum eQCWWANError BuildTlvIOIndicator(
    BYTE *pBuf,
    BYTE *pParam )
{
    struct QmiCbkNasSetEventReportReq *pReq =
        (struct QmiCbkNasSetEventReportReq *)pParam;

    enum eQCWWANError eRCode  = eQCWWAN_ERR_NONE;
    BYTE              ioDelta = 0;

    /* If the parameter has not been set do not fill the TLV */
    if (QMI_CBK_PARAM_NOCHANGE == pReq->eIOIndicator)
    {
        return eRCode;
    }

    /* Enable the reporting of IO indication */
    eRCode = PutByte( pBuf, pReq->eIOIndicator );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* For disabling IO pack zero as IO delta */
    if( QMI_CBK_PARAM_RESET == pReq->eIOIndicator )
    {
        eRCode = PutByte( pBuf, ioDelta );
        return eRCode;
    }

    /* Put IO delta value */
    eRCode = PutByte( pBuf, pReq->pSLQSSignalStrengthsIndReq->ioDelta );
    return eRCode;
}

/*
 * This function packs the SINR Indicator field to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
local enum eQCWWANError BuildTlvSNIRIndicator(
    BYTE *pBuf,
    BYTE *pParam )
{
    struct QmiCbkNasSetEventReportReq *pReq =
        (struct QmiCbkNasSetEventReportReq *)pParam;

    enum eQCWWANError eRCode    = eQCWWAN_ERR_NONE;
    BYTE              sinrDelta = 0;

    /* If the parameter has not been set do not fill the TLV */
    if ( QMI_CBK_PARAM_NOCHANGE == pReq->eSNIRIndicator )
    {
        return eRCode;
    }

    /* Enable the reporting of SINR indication */
    eRCode = PutByte( pBuf, pReq->eSNIRIndicator );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* For disabling SINR pack zero as SINR delta */
    if( QMI_CBK_PARAM_RESET == pReq->eSNIRIndicator )
    {
        eRCode = PutByte( pBuf, sinrDelta );
        return eRCode;
    }

    /* Put SINR delta value */
    eRCode = PutByte( pBuf, pReq->pSLQSSignalStrengthsIndReq->sinrDelta );
    return eRCode;
}

/*
 * This function packs the Error rate field to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
local enum eQCWWANError BuildTlvErrorRateIndicator(
    BYTE *pBuf,
    BYTE *pParam )
{
    struct QmiCbkNasSetEventReportReq *pReq =
        (struct QmiCbkNasSetEventReportReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* If the parameter has not been set do not fill the TLV */
    if ( QMI_CBK_PARAM_NOCHANGE == pReq->eErrorRateIndicator )
    {
        return eRCode;
    }

    /* Enable the reporting of Error Rate indication */
    eRCode = PutByte( pBuf, pReq->eErrorRateIndicator );
    return eRCode;
}

/*
 * This function packs the RSRQ Indicator field to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
local enum eQCWWANError BuildTlvRSRQIndicator(
    BYTE *pBuf,
    BYTE *pParam )
{
    struct QmiCbkNasSetEventReportReq *pReq =
        (struct QmiCbkNasSetEventReportReq *)pParam;

    enum eQCWWANError eRCode    = eQCWWAN_ERR_NONE;
    BYTE              rsrqDelta = 0;

    /* If the parameter has not been set do not fill the TLV */
    if ( QMI_CBK_PARAM_NOCHANGE == pReq->eRSRQIndicator )
    {
        return eRCode;
    }

    /* Enable the reporting of RSRQ indication */
    eRCode = PutByte( pBuf, pReq->eRSRQIndicator );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* For disabling RSRQ pack zero as RSRQ delta */
    if( QMI_CBK_PARAM_RESET == pReq->eRSRQIndicator )
    {
        eRCode = PutByte( pBuf, rsrqDelta );
        return eRCode;
    }

    /* Put RSRQ delta value */
    eRCode = PutByte( pBuf, pReq->pSLQSSignalStrengthsIndReq->rsrqDelta );
    return eRCode;
}

/*
 * This function packs the ECIO threshold field to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
local enum eQCWWANError BuildTlvECIOThIndicator(
    BYTE *pBuf,
    BYTE *pParam )
{
    struct QmiCbkNasSetEventReportReq *pReq =
        (struct QmiCbkNasSetEventReportReq *)pParam;

    enum eQCWWANError eRCode    = eQCWWAN_ERR_NONE;
    BYTE              idx       = 0;
    BYTE              eciothLen = 0;

    /* If the parameter has not been set do not fill the TLV */
    if ( QMI_CBK_PARAM_NOCHANGE == pReq->eECIOThIndicator )
    {
        return eRCode;
    }

    /* Enable the reporting of ECIO */
    eRCode = PutByte( pBuf, pReq->eECIOThIndicator );

    if ( eQCWWAN_ERR_NONE != eRCode )
    {
         return eRCode;
    }

    /* For disabling ECIO Threshold pack zero as ECIO Threshold length */
    if( QMI_CBK_PARAM_RESET == pReq->eECIOThIndicator )
    {
        eRCode = PutByte( pBuf, eciothLen );
        return eRCode;
    }

    /* Put the number of elements in ECIO threshold list */
    eRCode = PutByte( pBuf,
                      pReq->pSLQSSignalStrengthsIndReq->ecioThresholdListLen );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
         return eRCode;
    }

    /* Put the elements in ECIO threshold list */
    for( idx = 0;
         idx < pReq->pSLQSSignalStrengthsIndReq->ecioThresholdListLen;
         idx++ )
    {
        eRCode = PutWord( pBuf,
                          pReq->pSLQSSignalStrengthsIndReq->
                          ecioThresholdList[idx] );
        if( eQCWWAN_ERR_NONE != eRCode )
        {
            break;
        }
    }
    return eRCode;
}

/*
 * This function packs the SINR threshold field to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
local enum eQCWWANError BuildTlvSINRThIndicator(
    BYTE *pBuf,
    BYTE *pParam )
{
    struct QmiCbkNasSetEventReportReq *pReq =
        (struct QmiCbkNasSetEventReportReq *)pParam;

    enum eQCWWANError eRCode    = eQCWWAN_ERR_NONE;
    BYTE              idx       = 0;
    BYTE              sinrthLen = 0;

    /* If the parameter has not been set do not fill the TLV */
    if ( QMI_CBK_PARAM_NOCHANGE == pReq->eSINRThIndicator )
    {
        return eRCode;
    }

    /* Enable the reporting of SINR */
    eRCode = PutByte( pBuf, pReq->eSINRThIndicator );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
         return eRCode;
    }

    /* For disabling SINR Threshold pack zero as SINR Threshold length */
    if( QMI_CBK_PARAM_RESET == pReq->eSINRThIndicator )
    {
        eRCode = PutByte( pBuf, sinrthLen );
        return eRCode;
    }

    /* Put the number of elements in SINR threshold list */
    eRCode = PutByte( pBuf,
                      pReq->pSLQSSignalStrengthsIndReq->sinrThresholdListLen );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
         return eRCode;
    }

    /* Put the elements in SINR threshold list */
    for( idx = 0;
         idx < pReq->pSLQSSignalStrengthsIndReq->sinrThresholdListLen;
         idx++ )
    {
        eRCode = PutByte( pBuf,
                          pReq->pSLQSSignalStrengthsIndReq->
                          sinrThresholdList[idx] );
        if( eQCWWAN_ERR_NONE != eRCode )
        {
            break;
        }
    }
    return eRCode;
}

/*
 * This function packs the NAS Set Event Report parameters to the QMI
 * message SDU
 *
 * \param  pParamField                [OUT] - Pointer to storage into which the
 *                                            packed data will be placed by
 *                                            this function.
 *
 * \param  pMlength                   [OUT] - Total length of built message.
 *
 * \param  eSignalStrenght            [IN] - Set/Reset
 *
 * \param  eRFBand                    [IN] - Set/Reset
 *
 * \param  eRegRejectReason           [IN] - Set/Reset
 *
 * \param  eRSSIIndicator             [IN] - Set/Reset
 *
 * \param  eECIOIndicator             [IN] - Set/Reset
 *
 * \param  eIOIndicator               [IN] - Set/Reset
 *
 * \param  eErrorRateIndicator        [IN] - Set/Reset
 *
 * \param  RSRQIndicator              [IN] - Set/Reset
 *
 * \param  eECIOThIndicator           [IN] - Set/Reset
 *
 * \param  eSNIRThIndicator           [IN] - Set/Reset
 *
 * \param  pSLQSSignalStrengthsIndReq [IN] - See SLQSSignalStrengthsIndReq in
 *                                           qaGobiApiCbk.h for more
 *                                           information.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
package enum eQCWWANError PkQmiCbkNasSetEventReport(
    WORD                             *pMlength,
    enum eQmiCbkSetStatus            eSignalStrength,
    BYTE                             thresholdsSize,
    INT8                             *pThresholds,
    enum eQmiCbkSetStatus            eRFBand,
    enum eQmiCbkSetStatus            eRegRejectReason,
    enum eQmiCbkSetStatus            eRSSIIndicator,
    enum eQmiCbkSetStatus            eECIOIndicator,
    enum eQmiCbkSetStatus            eIOIndicator,
    enum eQmiCbkSetStatus            eSNIRIndicator,
    enum eQmiCbkSetStatus            eErrorRateIndicator,
    enum eQmiCbkSetStatus            eRSRQIndicator,
    enum eQmiCbkSetStatus            eECIOThIndicator,
    enum eQmiCbkSetStatus            eSINRThIndicator,
    struct SLQSSignalStrengthsIndReq *pSLQSSignalStrengthsIndReq,
    BYTE                             *pParamField )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SIGNAL_STRENGTH,      &BuildTlvSignalStrength },
        { eTLV_RF_BAND,              &BuildTlvRFBand },
        { eTLV_REG_REJECT_REASON,    &BuildTlvRegRejectReason },
        { eTLV_RSSI_INDICATOR,       &BuildTlvRSSIIndicator },
        { eTLV_ECIO_INDICATOR,       &BuildTlvECIOIndicator },
        { eTLV_IO_INDICATOR,         &BuildTlvIOIndicator },
        { eTLV_SNIR_INDICATOR,       &BuildTlvSNIRIndicator },
        { eTLV_ERROR_RATE_INDICATOR, &BuildTlvErrorRateIndicator },
        { eTLV_RSRQ_INDICATOR,       &BuildTlvRSRQIndicator },
        { eTLV_ECIO_TH_INDICATOR,    &BuildTlvECIOThIndicator },
        { eTLV_SINR_TH_INDICATOR,    &BuildTlvSINRThIndicator },
        { eTLV_TYPE_INVALID,         NULL } /* Important. Sentinel.
                                             * Signifies last item in map.
                                             */
    };

    struct QmiCbkNasSetEventReportReq req;
    slmemset( (CHAR *)&req,
              0,
              sizeof (struct QmiCbkNasSetEventReportReq) );

    req.eSignalStrength            = eSignalStrength;
    req.thresholdsSize             = thresholdsSize;
    req.pThresholds                = pThresholds;
    req.eRFBand                    = eRFBand;
    req.eRegRejectReason           = eRegRejectReason;
    req.eRSSIIndicator             = eRSSIIndicator;
    req.eECIOIndicator             = eECIOIndicator;
    req.eIOIndicator               = eIOIndicator;
    req.eSNIRIndicator             = eSNIRIndicator;
    req.eErrorRateIndicator        = eErrorRateIndicator;
    req.eRSRQIndicator             = eRSRQIndicator;
    req.eECIOThIndicator           = eECIOThIndicator;
    req.eSINRThIndicator           = eSINRThIndicator;
    req.pSLQSSignalStrengthsIndReq = pSLQSSignalStrengthsIndReq;

    enum eQCWWANError eRCode;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_NAS_SET_EVENT,
                      pMlength );

    return eRCode;
}

/* Response handlers */

/*
 * This function unpacks the NAS_SET_EVENT_REPORT response
 * message to a user-provided response structure.
 *
 * \param  pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
package enum eQCWWANError UpkQmiCbkNasSetEventReport(
    BYTE                               *pMdmResp,
    struct QmiCbkNasSetEventReportResp *pApiResp )
{
    ULONG eRCode;

    /* The unpack of the Callback contains only result code */
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
                           eQMI_NAS_SET_EVENT);
    return eRCode;
}

