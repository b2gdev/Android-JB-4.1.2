/*
 * \ingroup cbk
 *
 * \file    qaCbkVoiceSLQSVoiceDTMFInd.c
 *
 * \brief   Contains UnPacking routines for the QMI_VOICE_DTMF_IND
 *          message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCbkVoiceSLQSVoiceDTMFInd.h"

/* Locals */
local BYTE OnLength;
local BYTE OffLength;

/* Functions */
/*
 * This function unpacks the DTMF notification information TLV from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvDTMFInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    DTMFInfo *lResp = &((voiceDTMFEventInfo *)pResp)->DTMFInformation;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD              lCount;

    /* Extract the DTMF Information */
    eRCode = GetByte( pTlvData, &(lResp->callID) );
    if( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &(lResp->DTMFEvent) );
    if( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &(lResp->digitCnt) );
    if( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < lResp->digitCnt ; lCount++ )
    {
        eRCode = GetByte( pTlvData, &(lResp->digitBuff[lCount]) );
        if( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }
    return eRCode;
}

/*
 * This function unpacks the Pulse Width information TLV from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvPulseWidth(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceDTMFEventInfo *lResp = (voiceDTMFEventInfo *)pResp;
    lResp->pOnLength = &OnLength;

    /* Extract the Pulse Width */
    return GetByte( pTlvData, lResp->pOnLength );
}

/*
 * This function unpacks the InterDigit Interval information TLV from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvIDInterval(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceDTMFEventInfo *lResp = (voiceDTMFEventInfo *)pResp;
    lResp->pOffLength = &OffLength;

    /* Extract the InterDigit Interval */
    return GetByte( pTlvData, lResp->pOffLength );
}

/*
 * This function unpacks the DTMF Indication message to a user-provided
 * response structure.
 *
 * \param   pMdmResp - Pointer to packed response from the modem.
 *
 * \param   pApiResp - Pointer to storage to unpack into.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UpkQmiCbkVoiceDTMFEventInd(
    BYTE               *pMdmResp,
    voiceDTMFEventInfo *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_DTMF_INFORMATION,         &UnpackCbkTlvDTMFInfo},
        { eTLV_DTMF_PULSE_WIDTH,         &UnpackCbkTlvPulseWidth },
        { eTLV_DTMF_INTERDIGIT_INTERVAL, &UnpackCbkTlvIDInterval },
        { eTLV_TYPE_INVALID,             NULL } /* Important. Sentinel.
                                                 * Signifies last item in map.
                                                 */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_VOICE_DTMF_IND );
    return eRCode;
}
