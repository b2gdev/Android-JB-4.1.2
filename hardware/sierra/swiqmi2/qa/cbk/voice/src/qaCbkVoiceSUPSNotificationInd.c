/*
 * \ingroup cbk
 *
 * \file    qaCbkVoiceSUPSNotificationInd.c
 *
 * \brief   Contains UnPacking routines for the QMI_VOICE_SUPS_NOTIFICATION_IND
 *          message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCbkVoiceSUPSNotificationInd.h"

/* Locals */
static WORD   CUGIndex;
static ECTNum ECTNumberInfo;

/* Functions */
/*
 * This function unpacks the ECT Number information TLV from the QMI indication
 * message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvSUPSECTNumber(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceSUPSNotification *lResponse = (voiceSUPSNotification *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE   lCount = 0, lIndex = 0;

    /* Allocate memory to the pointer */
    lResponse->pECTNum = &ECTNumberInfo;

    /* Extract the ECT call state */
    eRCode = GetByte( pTlvData, &(lResponse->pECTNum->ECTCallState) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the presentation indicator */
    eRCode = GetByte( pTlvData, &(lResponse->pECTNum->presentationInd) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the number length */
    eRCode = GetByte( pTlvData, &lCount );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the ECT call state */
    while (lCount--)
    {
        eRCode = GetByte( pTlvData, &(lResponse->pECTNum->number[lIndex++]) );
        if( eRCode != eQCWWAN_ERR_NONE )
            return eRCode;
    }
    lResponse->pECTNum->number[lIndex] = EOS;
    return eRCode;
}

/*
 * This function unpacks the CUG Index information TLV from the QMI indication
 * message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvSUPSCUGIndex(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceSUPSNotification *lResponse = (voiceSUPSNotification *)pResp;
    lResponse->pCUGIndex = &CUGIndex;

    /* Extract the CUG Index */
    return GetWord( pTlvData, lResponse->pCUGIndex );
}

/*
 * This function unpacks the Supplementary service notification information
 * TLV from the QMI indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvSUPSNotificationInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceSUPSNotification *lResponse = (voiceSUPSNotification *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the Call ID */
    eRCode = GetByte( pTlvData, &(lResponse->callID) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the Notification Type */
    return GetByte( pTlvData, &(lResponse->notifType) );
}

/*
 * This function unpacks the SUPS Notification Indication message to a
 * user-provided response structure.
 *
 * \param   pMdmResp - Pointer to packed response from the modem.
 *
 * \param   pApiResp - Pointer to storage to unpack into.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UpkQmiCbkVoiceSUPSNotificationInd(
    BYTE                  *pMdmResp,
    voiceSUPSNotification *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_SUPS_NOTIFICATION_INFO, &UnpackCbkTlvSUPSNotificationInfo},
        { eTLV_SUPS_CUG_INDEX,         &UnpackCbkTlvSUPSCUGIndex },
        { eTLV_SUPS_ECT_NUMBER,        &UnpackCbkTlvSUPSECTNumber },
        { eTLV_TYPE_INVALID,           NULL } /* Important. Sentinel.
                                               * Signifies last item in map.
                                               */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_VOICE_SUPS_NOTIFICATION_IND );
    return eRCode;
}
