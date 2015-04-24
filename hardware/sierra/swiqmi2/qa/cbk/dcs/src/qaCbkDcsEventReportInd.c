/*
 * \ingroup cbk
 *
 * \file    qaCbkDcsEventReportInd.c
 *
 * \brief   unpacking routines for the QMI_DCS_EVENT_REPORT_IND message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCbkDcsEventReportInd.h"

/*
 * Unpacks the device state change TLV from the QMI indication message to a
 * user provided response structure.
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvDeviceState(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    struct deviceStateChangeTlv *lResp =
            &((struct QmiCbkDcsEventStatusReportInd *)pResp)->DSCTlv;

    /* Extract the device state */
    enum eQCWWANError eRCode;
    eRCode = GetByte( pTlvData, &(lResp->deviceState) );

    if ( eRCode == eQCWWAN_ERR_NONE )
        lResp->TlvPresent = TRUE;

    return eRCode;
}

/*
 * Unpacks the SDK terminated TLV from the QMI indication message to a
 * user provided response structure.
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvSDKTerminated(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    struct SDKTerminatedTlv *lResp =
            &((struct QmiCbkDcsEventStatusReportInd *)pResp)->SDKTlv;

    /* Extract the termination reason */
    enum eQCWWANError eRCode;

    eRCode = qmQmiExtractString ( pTlvData,
                                  (CHAR *)lResp->Data,
                                  MAX_SDK_EVENT_DATA_LENGTH );

    if ( eRCode == eQCWWAN_ERR_NONE )
        lResp->TlvPresent = TRUE;

    return eRCode;
}

/*
 * Unpacks the Event Report Indication message to a user-provided response
 * structure.
 *
 * \param   pMdmResp - Pointer to packed response from the modem.
 *
 * \param   pApiResp - Pointer to storage to unpack into.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UpkQmiCbkDcsEventReportInd(
    BYTE                                 *pMdmResp,
    struct QmiCbkDcsEventStatusReportInd *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        {   eTLV_DCS_IND_DEVICE_STATE,   &UnpackCbkTlvDeviceState },
        {   eTLV_DCS_IND_SDK_TERMINATED, &UnpackCbkTlvSDKTerminated },
        {   eTLV_TYPE_INVALID,          NULL } /* signifies last item in map */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_DCS_EVENT_IND );
    return eRCode;
}
