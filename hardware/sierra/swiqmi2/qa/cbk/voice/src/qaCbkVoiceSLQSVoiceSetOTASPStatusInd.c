/*
 * \ingroup cbk
 *
 * \file    qaCbkVoiceSLQSVoiceSetOTASPStatusInd.c
 *
 * \brief   Contains UnPacking routines for the QMI_VOICE_OTASP_STATUS_IND
 *          message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCbkVoiceSLQSVoiceSetOTASPStatusInd.h"

/* Functions */

/*
 * This function unpacks the OTASP Status TLV parameters from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvOTASPStatusinfo (
    BYTE *pTlvData,
    BYTE *pResp )
{
    enum eQCWWANError eRCode;

    /* Get the required TLV structure to the local pointer */
    voiceOTASPStatusInfo *lResponse = ((voiceOTASPStatusInfo *)pResp);

    /* Check For Invalid Parameter; Param is mandatory thus can not be NULL */
    if ( NULL == pResp )
    {
        return eQCWWAN_ERR_INVALID_ARG ;
    }

    eRCode = GetByte( pTlvData, &lResponse->callID );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }
    return GetByte( pTlvData, &lResponse->OTASPStatus );
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
 */
package enum eQCWWANError UpkQmiCbkVoiceSetOTASPStatusInd(
    BYTE                 *pMdmResp,
    voiceOTASPStatusInfo *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_OTASP_STATUS_INFO, &UnpackCbkTlvOTASPStatusinfo },
        { eTLV_TYPE_INVALID,      NULL } /* Important. Sentinel.
                                          * Signifies last item in map.
                                          */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_VOICE_OTASP_STATUS_IND );
    return eRCode;
}
