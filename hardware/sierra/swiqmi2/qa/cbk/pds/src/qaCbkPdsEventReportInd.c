/*
 * \ingroup cbk
 *
 * \file    qaCbkPdsEventReportInd.c
 *
 * \brief   Contains UnPacking routines for the
 *          QMI_PDS_EVENT_REPORT_IND message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCbkPdsEventReportInd.h"

/* Functions */

/*
 * This function unpacks the Position Data NMEA to a user provided
 * response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkPositionDataNMEA(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    struct PositionDataNMEATlv *lResp =
            &((struct QmiCbkPdsEventStatusReportInd *)pResp)->PDNMEATlv;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the String Parameter */
    /* The size of the string is stored to check for buffer size in API */
    eRCode = qmQmiExtractString ( pTlvData,
                                  lResp->PositionDataNMEA,
                                  QMI_MAX_POSITION_DATA_NMEA_BUFFER );
    lResp->TlvPresent = TRUE;
    return eRCode;
}

/*
 * This function unpacks the Event Report Indication message to a
 * user-provided response structure.
 *
 * \param   pMdmResp - Pointer to packed response from the modem.
 *
 * \param   pApiResp - Pointer to storage to unpack into.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 */
package enum eQCWWANError UpkQmiCbkPdsEventReportInd(
    BYTE   *pMdmResp,
    struct QmiCbkPdsEventStatusReportInd *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_POSITION_DATA_NMEA, &UnpackCbkPositionDataNMEA },
        { eTLV_TYPE_INVALID,       NULL }  /* Important. Sentinel.
                                            * Signifies last item in map.
                                            */
    };
    eRCode = qmunpackresp( pMdmResp,
                           (BYTE*)pApiResp,
                           map,
                           eQMI_PDS_EVENT_IND );
    return eRCode;
}
