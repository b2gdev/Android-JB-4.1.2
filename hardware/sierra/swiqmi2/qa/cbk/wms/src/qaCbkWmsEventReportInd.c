/*
 * \ingroup cbk
 *
 * \file    qaCbkWmsEventReportInd.c
 *
 * \brief   Contains UnPacking routines for the
 *          QMI_WMS_EVENT_REPORT_IND message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaGobiApiCbk.h"
#include "qaCbkWmsEventReportInd.h"

/* Functions */

/*
 * Unpacks the New Mobile Terminated Message
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing destination buffer
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkNewMTMessage(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    SMSMTMessageInfo *lResp =
            &((struct QmiCbkWmsEventReportInd *)pResp)->NewMMTlv.MTMessageInfo;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE lByte;

    /* validate TLV length */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                           eTLV_MT_MESSAGE_LENGTH );

    /* Extract the Storage Type Parameter */
    if (eRCode == eQCWWAN_ERR_NONE)
        eRCode = GetByte ( pTlvData, &lByte );

    /* Extract the Message Index */
    if (eRCode == eQCWWAN_ERR_NONE)
    {
        eRCode = GetLong ( pTlvData, &lResp->messageIndex );
        lResp->storageType = lByte;
        ((struct QmiCbkWmsEventReportInd *)pResp)->NewMMTlv.TlvPresent = TRUE;
    }
    return eRCode;
}

/*
 * Unpacks the Message mode
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing destination buffer
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkMessageMode(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    SMSMessageModeInfo *lResp =
        &((struct QmiCbkWmsEventReportInd *)pResp)->MMTlv.MessageModeInfo;
    enum eQCWWANError eRCode;

    /* validate TLV length */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                           eTLV_MESSAGE_MODE_LENGTH );

    if (eRCode == eQCWWAN_ERR_NONE)
        eRCode = GetByte ( pTlvData, &lResp->messageMode );

    if (eRCode == eQCWWAN_ERR_NONE)
        ((struct QmiCbkWmsEventReportInd *)pResp)->MMTlv.TlvPresent = TRUE;

    return eRCode;
}

/*
 * Unpacks the transfer route mobile terminated message
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing destination buffer
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTransferRouteMTMessage(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    SMSTransferRouteMTMessageInfo *lResp =
        &((struct QmiCbkWmsEventReportInd *)pResp)->TRMessageTlv.\
                                                TransferRouteMTMessageInfo;
    enum eQCWWANError eRCode;
    WORD lmessageSize;
    BYTE iIndex = 0;

    /* Extract the Ack Indicator */
    eRCode = GetByte ( pTlvData, &lResp->ackIndicator );

    /* Extract the transaction ID */
    if (eRCode == eQCWWAN_ERR_NONE)
        eRCode = GetLong ( pTlvData, &lResp->transactionID );

    /* Extract the format */
    if (eRCode == eQCWWAN_ERR_NONE)
        eRCode = GetByte ( pTlvData, &lResp->format );

    /* Extract the length and data */
    if (eRCode == eQCWWAN_ERR_NONE)
    {
        eRCode = GetWord ( pTlvData, &lResp->length );
        if ( lResp->length > QMI_WMS_MAX_PAYLOAD_LENGTH )
            return eQCWWAN_ERR_BUFFER_SZ;

        lmessageSize = lResp->length;

        /* Fill the message data */
        while ( lmessageSize-- )
        {
            eRCode = GetByte ( pTlvData, &lResp->data[iIndex++] );
            if (eRCode != eQCWWAN_ERR_NONE)
                return eRCode;
        }
    }

    if (eRCode == eQCWWAN_ERR_NONE)
        ((struct QmiCbkWmsEventReportInd *)pResp)->TRMessageTlv.TlvPresent =
                                                                         TRUE;
    return eRCode;
}

/*
 * Unpacks the ETWS information
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing destination buffer
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkSMSEtwsMessage(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    SMSEtwsMessageInfo *lResp =
        &((struct QmiCbkWmsEventReportInd *)pResp)->ETWSTlv.EtwsMessageInfo;
    enum eQCWWANError eRCode;
    WORD lmessageSize;
    BYTE iIndex = 0;

    eRCode = GetByte ( pTlvData, &lResp->notificationType );

    if (eRCode == eQCWWAN_ERR_NONE)
    {
        eRCode = GetWord ( pTlvData, &lResp->length );
        lmessageSize = lResp->length;

        /* Fill the message data */
        while ( lmessageSize-- )
        {
            eRCode = GetByte ( pTlvData, &lResp->data[iIndex++] );
            if (eRCode != eQCWWAN_ERR_NONE)
                return eRCode;
        }
    }

    if (eRCode == eQCWWAN_ERR_NONE)
        ((struct QmiCbkWmsEventReportInd *)pResp)->ETWSTlv.TlvPresent = TRUE;

    return eRCode;
}

/*
 * Unpacks the SMS Etws Plmn Information
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing destination buffer
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkSMSEtwsPlmn(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    SMSEtwsPlmnInfo *lResp =
        &((struct QmiCbkWmsEventReportInd *)pResp)->ETWSPLMNTlv.ETWSPLMNInfo;
    enum eQCWWANError eRCode;

    /* validate TLV length */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                           eTLV_MESSAGE_ETWS_PLMN_LENGTH );

    if (eRCode == eQCWWAN_ERR_NONE)
        eRCode = GetWord ( pTlvData, &lResp->mobileCountryCode );

    if (eRCode == eQCWWAN_ERR_NONE)
        eRCode = GetWord ( pTlvData, &lResp->mobileNetworkCode );

    if (eRCode == eQCWWAN_ERR_NONE)
        ((struct QmiCbkWmsEventReportInd *)pResp)->ETWSPLMNTlv.TlvPresent = TRUE;

    return eRCode;
}

/*
 * Unpacks the SMSC information
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing destination buffer
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkSMSCAddress(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    SMSCAddressInfo *lResp =
        &((struct QmiCbkWmsEventReportInd *)pResp)->SMSCTlv.SMSCInfo;
    enum eQCWWANError eRCode;
    WORD lmessageSize;
    BYTE iIndex = 0;

    eRCode = GetByte ( pTlvData, &lResp->length );
    lmessageSize = lResp->length;

    /* Fill the message data */
    while ( lmessageSize-- )
    {
        eRCode = GetByte ( pTlvData, &lResp->data[iIndex++] );
        if (eRCode != eQCWWAN_ERR_NONE)
            return eRCode;
    }

    if (eRCode == eQCWWAN_ERR_NONE)
        ((struct QmiCbkWmsEventReportInd *)pResp)->SMSCTlv.TlvPresent = TRUE;

    return eRCode;
}

/*
 * Unpacks the SMS on IMS Information
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing destination buffer
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkSMSOnIMS(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    SMSOnIMSInfo *lResp =
        &((struct QmiCbkWmsEventReportInd *)pResp)->IMSTlv.IMSInfo;
    enum eQCWWANError eRCode;

    /* validate TLV length */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                           eTLV_MESSAGE_SMS_ON_IMS_LENGTH );

    if (eRCode == eQCWWAN_ERR_NONE)
        eRCode = GetByte ( pTlvData, &lResp->smsOnIMS );

    if (eRCode == eQCWWAN_ERR_NONE)
        ((struct QmiCbkWmsEventReportInd *)pResp)->IMSTlv.TlvPresent = TRUE;

    return eRCode;
}

/*
 * Unpacks the Event Report Indication message to a
 * user-provided response structure.
 *
 * \param   pMdmResp - Pointer to packed response from the modem.
 *
 * \param   pApiResp - Pointer to storage to unpack into.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 */
package enum eQCWWANError UpkQmiCbkWmsEventReportInd(
    BYTE                           *pMdmResp,
    struct QmiCbkWmsEventReportInd *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_MT_MESSAGE,                &UnpackCbkNewMTMessage },
        { eTLV_TRANSFER_ROUTE_MT_MESSAGE, &UnpackCbkTransferRouteMTMessage },
        { eTLV_MESSAGE_MODE,              &UnpackCbkMessageMode },
        { eTLV_MESSAGE_ETWS,              &UnpackCbkSMSEtwsMessage },
        { eTLV_MESSAGE_ETWS_PLMN,         &UnpackCbkSMSEtwsPlmn },
        { eTLV_MESSAGE_SMSC,              &UnpackCbkSMSCAddress },
        { eTLV_MESSAGE_SMS_ON_IMS,        &UnpackCbkSMSOnIMS },
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };
    eRCode = qmunpackresp( pMdmResp,
                           (BYTE*)pApiResp,
                           map,
                           eQMI_WMS_EVENT_IND );
    return eRCode;
}
