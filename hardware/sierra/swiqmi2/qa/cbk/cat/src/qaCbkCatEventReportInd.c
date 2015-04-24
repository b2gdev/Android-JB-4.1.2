/*
 * \ingroup cbk
 *
 * \file    qaCbkCatEventReportInd.c
 *
 * \brief   UnPacking routines for the QMI_CAT_EVENT_REPORT_IND message
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCbkCatEventReportInd.h"

/* Functions */

/*
 * Unpacks the End Proactive Session TLV
 *
 * \param   pTlvData(IN)    - pointer to source TLV structure
 *
 * \param   pResp(OUT)      - pointer to destination TLV structure
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackTlvEndProactiveSession(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    struct CatCommonEventTlv *lResp =
            ((struct QmiCbkCatEventStatusReportInd *)pResp)->CCETlv;
    BYTE lIndex = 0;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Get the current Index from the Structure */
    lIndex = ((struct QmiCbkCatEventStatusReportInd *)pResp)->event_Index;

    /* Verify whether the Length of the TLV is according to the Standard */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                           eTLV_END_PROACTIVE_SESSION_LENGTH );

    /* Get the required TLV structure to the local pointer */
    struct CatEndProactiveSessionTlv *pTlvEventInfo =
    &((struct QmiCbkCatEventStatusReportInd *)pResp)->CCETlv[lIndex].CatEvent.
                                                                     CatEndPS;

    /* Extract the Event ID Parameter */
    if (eRCode == eQCWWAN_ERR_NONE)
        lResp[lIndex].EventID = eTLV_END_PROACTIVE_SESSION;

    /* Extract the End Proactive Session Value */
    if (eRCode == eQCWWAN_ERR_NONE)
        eRCode = GetByte ( pTlvData, &pTlvEventInfo->EndProactiveSession );

    /* Assign the Event Length Parameter (Total Byte stream Size)*/
    lResp[lIndex].EventLength = eTLV_END_PROACTIVE_SESSION_LENGTH;

    if (eRCode == eQCWWAN_ERR_NONE)
        lResp[lIndex].TlvPresent = TRUE;

    /* Update the Index Value in the Structure */
    lIndex++;
    ((struct QmiCbkCatEventStatusReportInd *)pResp)->event_Index = lIndex;
    return eRCode;
}

/*
 * Unpacks the Refresh TLV
 *
 * \param   pTlvData(IN)    - pointer to source TLV structure
 *
 * \param   pResp(OUT)      - pointer to destination TLV structure
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkRefresh(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    struct CatCommonEventTlv *lResp =
            ((struct QmiCbkCatEventStatusReportInd *)pResp)->CCETlv;
    BYTE lIndex = 0;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Get the current Index from the Structure */
    lIndex = ((struct QmiCbkCatEventStatusReportInd *)pResp)->event_Index;

    /* Verify whether the Length of the TLV is according to the Standard */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                           eTLV_REFRESH_LENGTH );

    /* Get the required TLV structure to the local pointer */
    struct CatRefreshTlv *pTlvEventInfo =
    &((struct QmiCbkCatEventStatusReportInd *)pResp)->CCETlv[lIndex].CatEvent.
                                                                     CatRefresh;

    /* Extract the Event ID Parameter */
    if (eRCode == eQCWWAN_ERR_NONE)
        lResp[lIndex].EventID = eTLV_REFRESH;

    /* Extract the Refresh Event */
    if (eRCode == eQCWWAN_ERR_NONE)
        eRCode = GetWord ( pTlvData, &pTlvEventInfo->RefreshMode );

    /* Extract the Refresh Mode */
    if (eRCode == eQCWWAN_ERR_NONE)
        eRCode = GetByte ( pTlvData, &pTlvEventInfo->RefreshStage );

    /* Assign the Event Length Parameter (Total Byte stream Size)*/
    lResp[lIndex].EventLength = eTLV_REFRESH_LENGTH;

    if (eRCode == eQCWWAN_ERR_NONE)
        lResp[lIndex].TlvPresent = TRUE;

    /* Update the Index Value in the Structure */
    lIndex++;
    ((struct QmiCbkCatEventStatusReportInd *)pResp)->event_Index = lIndex;
    return eRCode;
}

/*
 * Unpacks the Setup Event List TLV
 *
 * \param   pTlvData(IN)    - pointer to source TLV structure
 *
 * \param   pResp(OUT)      - pointer to destination TLV structure
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackTlvEventList(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    struct CatCommonEventTlv *lResp =
            ((struct QmiCbkCatEventStatusReportInd *)pResp)->CCETlv;
    BYTE lIndex = 0;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Get the current Index from the Structure */
    lIndex = ((struct QmiCbkCatEventStatusReportInd *)pResp)->event_Index;

    /* Verify whether the Length of the TLV is according to the Standard */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                           eTLV_SETUP_EVENT_LIST_LENGTH);

    /* Get the required TLV structure to the local pointer */
    struct CatEventListTlv *pTlvEventInfo =
    &((struct QmiCbkCatEventStatusReportInd *)pResp)->CCETlv[lIndex].CatEvent.
                                                                    CatEventLst;

    /* Extract the Event ID Parameter */
    if (eRCode == eQCWWAN_ERR_NONE)
        lResp[lIndex].EventID = eTLV_ALPHA_IDENTIFIER;

    /* Extract the Event List */
    if (eRCode == eQCWWAN_ERR_NONE)
        eRCode = GetLong ( pTlvData, &pTlvEventInfo->SetupEventList );

    /* Assign the Event Length Parameter (Total Byte stream Size)*/
    lResp[lIndex].EventLength = eTLV_SETUP_EVENT_LIST_LENGTH;

    if (eRCode == eQCWWAN_ERR_NONE)
        lResp[lIndex].TlvPresent = TRUE;

    /* Update the Index Value in the Structure */
    lIndex++;
    ((struct QmiCbkCatEventStatusReportInd *)pResp)->event_Index = lIndex;
    return eRCode;
}

/*
 * Unpacks the Alpha Identifier Tlv from CAT Event Indication
 *
 * \param   pTlvData(IN)    - pointer to source TLV structure
 *
 * \param   pResp(OUT)      - pointer to destination TLV structure
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackTlvAlphaIdentifier(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    struct CatCommonEventTlv *lResp =
        ((struct QmiCbkCatEventStatusReportInd *)pResp)->CCETlv;
    BYTE lIndex = 0;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Get the current Index from the Structure */
    lIndex = ((struct QmiCbkCatEventStatusReportInd *)pResp)->event_Index;

    /* Get the required TLV structure to the local pointer */
    struct CatAlPhaIdentifierTlv *pTlvEventInfo =
    &((struct QmiCbkCatEventStatusReportInd *)pResp)->CCETlv[lIndex].CatEvent.
                                                            CatAlphaIdtfr;

    /* Extract the Event ID Parameter */
    lResp[lIndex].EventID = eTLV_ALPHA_IDENTIFIER;

    /* Extract the Reference ID ID Parameter */
    eRCode = GetByte ( pTlvData, &pTlvEventInfo->ReferenceID );

    /* Extract the Alpha ID Length Parameter */
    if (eRCode == eQCWWAN_ERR_NONE)
        eRCode = GetWord ( pTlvData, &pTlvEventInfo->AlphaIDLength );

    /* Extract the Alpha ID Parameter */
    if (eRCode == eQCWWAN_ERR_NONE)
        eRCode = qmQmiExtractString ( pTlvData,
                                      (CHAR *)pTlvEventInfo->AlphaID,
                                      QMI_MAX_CAT_EVENT_DATA_LENGTH );

    if (eRCode == eQCWWAN_ERR_NONE)
        lResp[lIndex].TlvPresent = TRUE;

    /* Assign the Event Length Parameter (Total Byte stream Size)*/
    lResp[lIndex].EventLength = pTlvEventInfo->AlphaIDLength + 6;

    /* Update the Index Value in the Structure */
    lIndex++;
    ((struct QmiCbkCatEventStatusReportInd *)pResp)->event_Index = lIndex;

    return eRCode;
}

/*
 * Unpacks the Comond TLVs received from CAT Event Indication
 *
 * \param   pTlvData(IN)    - pointer to source TLV structure
 *
 * \param   pResp(OUT)      - pointer to destination TLV structure
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkCatEventIndTlvCommom(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    struct CatCommonEventTlv *lResp =
        ((struct QmiCbkCatEventStatusReportInd *)pResp)->CCETlv;
    BYTE lIndex = 0, lTlvType;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Get the current Index from the Structure */
    lIndex = ((struct QmiCbkCatEventStatusReportInd *)pResp)->event_Index;

    /* Get the required TLV structure to the local pointer */
    struct CatEventIDDataTlv *pTlvEventInfo =
    &((struct QmiCbkCatEventStatusReportInd *)pResp)->CCETlv[lIndex].CatEvent.
                                                                    CatEvIDData;
    /* Extract the Event ID Parameter */
    eRCode = GetTlvType( (BYTE *)pTlvData, &lTlvType );
    lResp[lIndex].EventID = lTlvType;

    /* Get the Reference ID */
    eRCode = GetLong( pTlvData, &pTlvEventInfo->ReferenceID );

    /* Extract the Data Length Parameter */
    if (eRCode == eQCWWAN_ERR_NONE)
        eRCode = GetWord( pTlvData, &pTlvEventInfo->DataLength );

    /* Extract the Storage Type Parameter */
    if (eRCode == eQCWWAN_ERR_NONE)
        eRCode = qmQmiExtractString( pTlvData,
                                     (CHAR *)pTlvEventInfo->Data,
                                     QMI_MAX_CAT_EVENT_DATA_LENGTH );

    /* Assign the Event Length Parameter (Total Byte stream Size)*/
    lResp[lIndex].EventLength = pTlvEventInfo->DataLength + 6;

    if (eRCode == eQCWWAN_ERR_NONE)
        lResp[lIndex].TlvPresent = TRUE;

    /* Update the Index Value in the Structure */
    lIndex++;
    ((struct QmiCbkCatEventStatusReportInd *)pResp)->event_Index = lIndex;

    return eRCode;
}

/*
 * Unpacks the Event Report Indication message to a
 * user-provided response structure.
 *
 * \param   pMdmResp(IN)    - pointer to packed response from the modem.
 *
 * \param   pApiResp(OUT)   - pointer to storage into which the response is unpacked
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 */
package enum eQCWWANError UpkQmiCbkCatEventReportInd(
    BYTE                                 *pMdmResp,
    struct QmiCbkCatEventStatusReportInd *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_DISPLAY_TEXT,            &UnpackCbkCatEventIndTlvCommom },
        { eTLV_GET_IN_KEY,              &UnpackCbkCatEventIndTlvCommom },
        { eTLV_GET_INPUT,               &UnpackCbkCatEventIndTlvCommom },
        { eTLV_SETUP_MENU,              &UnpackCbkCatEventIndTlvCommom },
        { eTLV_SELECT_ITEM,             &UnpackCbkCatEventIndTlvCommom },
        { eTLV_ALPHA_IDENTIFIER,        &UnpackTlvAlphaIdentifier },
        { eTLV_SETUP_EVENT_LIST,        &UnpackTlvEventList },
        { eTLV_SETUP_IDLE_MODE_TEXT,    &UnpackCbkCatEventIndTlvCommom },
        { eTLV_LANGUAGE_NOTIFICATION,   &UnpackCbkCatEventIndTlvCommom },
        { eTLV_REFRESH,                 &UnpackCbkRefresh },
        { eTLV_END_PROACTIVE_SESSION,   &UnpackTlvEndProactiveSession },
        { eTLV_TYPE_INVALID,            NULL }  /* Important. Sentinel.
                                                 * Signifies last item in map.
                                                 */
    };

    /* Initialize the Event Index before the TLVs are unpacked */
    pApiResp->event_Index = 0;

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE*)pApiResp,
                           map,
                           eQMI_CAT_EVENT_IND );
    return eRCode;
}
