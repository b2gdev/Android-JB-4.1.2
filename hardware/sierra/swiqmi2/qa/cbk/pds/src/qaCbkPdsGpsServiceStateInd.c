/*
 * \ingroup cbk
 *
 * \file    qaCbkPdsGpsServiceStateInd.c
 *
 * \brief   Contains UnPacking routines for the
 *          QMI_PDS_GPS_SERVICE_STATE_IND message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaGobiApiCbk.h"
#include "qaCbkPdsGpsServiceStateInd.h"

/* Functions */

/*
 * Unpacks the Service State Indication
 *
 * \param   pTlvData - Pointer to response TLV
 *
 * \param   pResp    - Pointer to destination buffer
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkGPSServiceState(
    BYTE *pTlvData,
    BYTE *pResp )
{
    struct GPSServiceStateTlv *lResp =
            &((struct QmiCbkPdsGpsServiceStatusInd *)pResp)->GPSSSTlv;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE extractByte;

    /* validate TLV length */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                           eTLV_GPS_SERVICE_STATE_LENGTH );

    /* extract the EnabledStatus Parameter */
    /* Need a temporary variable as EnabledStatus is ULONG type */ 
    if (eRCode == eQCWWAN_ERR_NONE)
        eRCode = GetByte ( pTlvData, &extractByte );

    /* extract the TrackingStatus Parameter */
    /* Need a temporary variable as TrackingStatus is ULONG type */ 
    if (eRCode == eQCWWAN_ERR_NONE)
    {
        lResp->EnabledStatus = extractByte;
        eRCode = GetByte ( pTlvData, &extractByte );
        if (eRCode == eQCWWAN_ERR_NONE)
        {
            lResp->TrackingStatus = extractByte;
            lResp->TlvPresent = TRUE;
        }
    }
    return eRCode;
}

/*
 * Unpacks the GPS Service State Indication
 *
 * \param   pMdmResp - Pointer to packed response from the modem.
 *
 * \param   pApiResp - Pointer to destination buffer
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 */
package enum eQCWWANError UpkQmiCbkPdsGpsServiceStateInd(
    BYTE                                *pMdmResp,
    struct QmiCbkPdsGpsServiceStatusInd *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_GPS_SERVICE_STATE, &UnpackCbkGPSServiceState },
        { eTLV_TYPE_INVALID,      NULL }  /* Important. Sentinel.
                                           * Signifies last item in map.
                                           */
    };
    eRCode = qmunpackresp( pMdmResp,
                           (BYTE*)pApiResp,
                           map,
                           eQMI_PDS_STATE_IND );
    return eRCode;
}
