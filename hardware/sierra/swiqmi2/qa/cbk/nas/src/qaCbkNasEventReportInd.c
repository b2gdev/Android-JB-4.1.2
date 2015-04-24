/*
 * \ingroup cbk
 *
 * \file qaCbkNasEventReportInd.c
 *
 * \brief Contains UnPacking routines for the
 *        QMI_NAS_EVENT_REPORT_IND message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "sludefs.h"
#include "qaCbkNasEventReportInd.h"

/* Functions */

/*
 * This function unpacks the Signal Strength from the
 * QMI indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvSignalStrength(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    struct SignalStrengthTlv *lResp =
        &((struct QmiCbkNasEventStatusReportInd *)pResp)->SSTlv;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    BYTE lByte = 0;

    /* Extract the Signal Strength*/
    eRCode = GetByte( pTlvData, &lByte );
    if ( eRCode == eQCWWAN_ERR_NONE )
    {
        lResp->signalStrength = lByte;
        eRCode = GetByte( pTlvData, &lByte );
        if ( eRCode == eQCWWAN_ERR_NONE )
        {
            lResp->radioInterface = lByte;
            lResp->TlvPresent     = TRUE;
        }
    }
    return eRCode;
}

/*
 * This function unpacks the RFBand from the
 * QMI indication message to a user provided response structure
 *
 * \param  pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvRFBand(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    struct RFInfoTlv *lResp =
        &((struct QmiCbkNasEventStatusReportInd *)pResp)->RFTlv;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD lWord = 0;
    BYTE lByte = 0;
    BYTE lSize = 0;

    /* Extract the Radio Interface Size */
    eRCode = GetByte( pTlvData, &lByte );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    lResp->radioInterfaceSize = lByte;
    while ( lResp->radioInterfaceSize-- )
    {
        /* Get the Radio Interface */
        eRCode = GetByte( pTlvData, &lByte );
        if ( eRCode != eQCWWAN_ERR_NONE )
            return eRCode;
        lResp->radioInterface[lSize] = lByte;

        /* Get the Active Band Class */
        eRCode = GetWord( pTlvData, &lWord );
        if ( eRCode != eQCWWAN_ERR_NONE )
            return eRCode;
        lResp->activeBandClass[lSize] = lWord;

        /* Get the Active Channel */
        eRCode = GetWord( pTlvData, &lWord );
        if ( eRCode != eQCWWAN_ERR_NONE )
            return eRCode;
        lResp->activeChannel[lSize++] = lWord;
    }

    lResp->radioInterfaceSize = lSize;
    lResp->TlvPresent = TRUE;
    return eRCode;
}

/*
 * This function unpacks the Reject Reason from the
 * QMI indication message to a user provided response structure
 *
 * \param  pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
  * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvRegRejectReason(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    struct RejectReasonTlv *lResp =
        &((struct QmiCbkNasEventStatusReportInd *)pResp)->RRTlv;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    BYTE lByte = 0;
    WORD lWord = 0;

    /* Extract the Reject Reason */
    eRCode = GetByte( pTlvData, &lByte );
    if ( eRCode == eQCWWAN_ERR_NONE )
    {
        lResp->serviceDomain = lByte;
        eRCode = GetWord( pTlvData, &lWord );
        if ( eRCode == eQCWWAN_ERR_NONE )
        {
            lResp->rejectCause = lWord;
            lResp->TlvPresent  = TRUE;
        }
    }
    return eRCode;
}

/*
 * This function unpacks the SLQSSSTlv TLV from the QMI indication message to a
 * user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvRSSI(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    struct SLQSSignalStrengthsTlv *lResp =
        &( (struct QmiCbkNasEventStatusReportInd *)pResp)->SLQSSSTlv;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE              rssi;

    /* Get RSSI */
    eRCode = GetByte( pTlvData, &rssi );
    if (eQCWWAN_ERR_NONE != eRCode)
    {
        return eRCode;
    }

    /* Convert the retrieved value in dBm and copy it in the list */
    lResp->sSLQSSignalStrengthsInfo.rxSignalStrengthInfo.rxSignalStrength =
                                                             ( ( ~rssi ) + 1 );

    /* Get Radio Interface */
    eRCode = GetByte( pTlvData, &( lResp->sSLQSSignalStrengthsInfo.
                                   rxSignalStrengthInfo.radioIf ) );
    if (eQCWWAN_ERR_NONE != eRCode)
    {
        return eRCode;
    }

    lResp->TlvPresent = TRUE;
    return eRCode;
}

/*
 * This function unpacks the ECIO TLV from the QMI indication message to a
 * user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvECIO(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    struct SLQSSignalStrengthsTlv *lResp =
        &( (struct QmiCbkNasEventStatusReportInd *)pResp)->SLQSSSTlv;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE              ecio;

    /* Get ECIO */
    eRCode = GetByte( pTlvData, &ecio );
    if (eQCWWAN_ERR_NONE != eRCode)
    {
        return eRCode;
    }

    /* Change the retrieved value in dBm and copy it in ECIO list */
    lResp->sSLQSSignalStrengthsInfo.ecioInfo.ecio = ~( ecio >> 1 ) + 1;

    /* Get Radio Interface */
    eRCode = GetByte( pTlvData, &( lResp->sSLQSSignalStrengthsInfo.
                                   ecioInfo.radioIf ) );
    if (eQCWWAN_ERR_NONE != eRCode)
    {
        return eRCode;
    }

    lResp->TlvPresent = TRUE;
    return eRCode;
}

/*
 * This function unpacks the IO TLV from the QMI indication message to a
 * user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvIO(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    struct SLQSSignalStrengthsTlv *lResp =
        &( (struct QmiCbkNasEventStatusReportInd *)pResp)->SLQSSSTlv;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Get IO */
    eRCode = GetLong( pTlvData, &( lResp->sSLQSSignalStrengthsInfo.io ) );
    if (eQCWWAN_ERR_NONE != eRCode)
    {
        return eRCode;
    }

    lResp->TlvPresent = TRUE;
    return eRCode;
}

/*
 * This function unpacks the SINR State from the QMI indication message to a
 * user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvSINR(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    struct SLQSSignalStrengthsTlv *lResp =
        &( (struct QmiCbkNasEventStatusReportInd *)pResp)->SLQSSSTlv;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Get SINR */
    eRCode = GetByte( pTlvData, &( lResp->sSLQSSignalStrengthsInfo.sinr ) );
    if (eQCWWAN_ERR_NONE != eRCode)
    {
        return eRCode;
    }

    lResp->TlvPresent = TRUE;
    return eRCode;
}

/*
 * This function unpacks the Error rate from the QMI indication message to a
 * user provided response structure
 *
 * \param  pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvErrorRate(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    struct SLQSSignalStrengthsTlv *lResp =
        &( (struct QmiCbkNasEventStatusReportInd *)pResp)->SLQSSSTlv;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Get Error Rate */
    eRCode = GetWord( pTlvData, &( lResp->sSLQSSignalStrengthsInfo.
                                    errorRateInfo.errorRate ) );

    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* Get Radio Interface */
    eRCode = GetByte( pTlvData, &( lResp->sSLQSSignalStrengthsInfo.
                                   errorRateInfo.radioIf ) );
    if (eQCWWAN_ERR_NONE != eRCode)
    {
        return eRCode;
    }

    lResp->TlvPresent = TRUE;
    return eRCode;
}

/*
 * This function unpacks the RSRQ from the QMI indication message to a
 * user provided response structure
 *
 * \param  pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvRSRQ(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    struct SLQSSignalStrengthsTlv *lResp =
        &( (struct QmiCbkNasEventStatusReportInd *)pResp)->SLQSSSTlv;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Get RSRQ */
    eRCode = GetByte( pTlvData,
                  (BYTE *)&( lResp->sSLQSSignalStrengthsInfo.rsrqInfo.rsrq ) );
    if (eQCWWAN_ERR_NONE != eRCode)
    {
        return eRCode;
    }

    /* Get Radio Interface */
    eRCode = GetByte( pTlvData,
                      &( lResp->sSLQSSignalStrengthsInfo.rsrqInfo.radioIf ) );
    if (eQCWWAN_ERR_NONE != eRCode)
    {
        return eRCode;
    }

    lResp->TlvPresent = TRUE;
    return eRCode;
}

/*
 * This function unpacks the Event Report Indication message to a
 * user-provided response structure.
 *
 * \param  pMdmResp - Pointer to packed response from the modem.
 *
 * \param  pApiResp - Pointer to storage to unpack into.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UpkQmiCbkNasEventReportInd(
    BYTE                                 *pMdmResp,
    struct QmiCbkNasEventStatusReportInd *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_IND_SIGNAL_STRENGTH,   &UnpackCbkTlvSignalStrength },
        { eTLV_IND_RF_BAND,           &UnpackCbkTlvRFBand },
        { eTLV_IND_REG_REJECT_REASON, &UnpackCbkTlvRegRejectReason },
        { eTLV_IND_RSSI,              &UnpackCbkTlvRSSI },
        { eTLV_IND_ECIO,              &UnpackCbkTlvECIO },
        { eTLV_IND_IO,                &UnpackCbkTlvIO },
        { eTLV_IND_SINR,              &UnpackCbkTlvSINR },
        { eTLV_IND_ERROR_RATE,        &UnpackCbkTlvErrorRate },
        { eTLV_IND_RSRQ,              &UnpackCbkTlvRSRQ },
        { eTLV_TYPE_INVALID,          NULL } /* Important. Sentinel.
                                              * Signifies last item in map.
                                              */
    };

    /* Intialize the buffers */
    slmemset( (char *)&pApiResp->SLQSSSTlv.sSLQSSignalStrengthsInfo,
              0,
              sizeof( pApiResp->SLQSSSTlv.sSLQSSignalStrengthsInfo ) );

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_NAS_EVENT_IND );
    return eRCode;
}
