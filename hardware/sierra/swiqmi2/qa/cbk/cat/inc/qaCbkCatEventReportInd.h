/**
 * \internal
 * \ingroup cbk
 *
 * \file    qaCbkCatEventReportInd.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaCbkCatEventReportInd.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __CAT_EVENT_REPORT_IND_H__
#define __CAT_EVENT_REPORT_IND_H__

/**
 * \internal
 * An enumeration of eQMI_CAT_EVENT_REPORT_IND response TLV IDs
 */
enum eQMI_CAT_EVENT_REPORT_IND_TLV
{
    eTLV_DISPLAY_TEXT          = 0x10,
    eTLV_GET_IN_KEY            = 0x11,
    eTLV_GET_INPUT             = 0x12,
    eTLV_SETUP_MENU            = 0x13,
    eTLV_SELECT_ITEM           = 0x14,
    eTLV_ALPHA_IDENTIFIER      = 0x15,
    eTLV_SETUP_EVENT_LIST      = 0x16,
    eTLV_SETUP_IDLE_MODE_TEXT  = 0x17,
    eTLV_LANGUAGE_NOTIFICATION = 0x18,
    eTLV_REFRESH               = 0x19,
    eTLV_END_PROACTIVE_SESSION = 0x1A
};

/**
 * \internal
 * An enumeration of eQMI_CAT_EVENT_REPORT_IND response TLV Lengths
 */
enum eQMI_CAT_EVENT_REPORT_IND_TLV_LENGTH
{
    eTLV_SETUP_EVENT_LIST_LENGTH        = 0x04,
    eTLV_REFRESH_LENGTH                 = 0x03,
    eTLV_END_PROACTIVE_SESSION_LENGTH   = 0x01
};

/**
 * \internal
 * eQMI_CAT_EVENT_IND TLVs defined below
 */
#define QMI_MAX_CAT_EVENT_DATA_LENGTH   255
#define QMI_CAN_COMMON_EVENT_TLV_NUMBER 11

/**
 * \internal
 * push current alignment to stack
 * set alignment to 1 byte boundary
 */
#pragma pack(push)
#pragma pack(1)

/**
 * structure used to store all Common CAT Event parameters.
 *
 * \param   ReferenceID - proactive command reference ID.
 * \param   DataLength  - length of pData ( in Bytes )
 * \param   Data        - command specific to the CAT event ID, encoded as in
 *                        ETSI TS 102 223 [Section 6.6.X]
 */
struct CatEventIDDataTlv
{
    ULONG  ReferenceID;
    USHORT DataLength;
    BYTE   Data[QMI_MAX_CAT_EVENT_DATA_LENGTH];
};

/**
 * structure used to store all Alpha Identifier parameters.
 *
 * \param   ReferenceID   - proactive command type that included the alpha
 *                          identifier – 0x01; sends SMS proactive command
 * \param   AlphaIDLength - length of AlphaID ( in bytes )
 * \param   AlphaID       - alpha identifier, encoded as in
 *                          ETSI TS 102 223 [Section 8.2]
 */
struct CatAlPhaIdentifierTlv
{
    BYTE    ReferenceID;
    USHORT  AlphaIDLength;
    BYTE    AlphaID[QMI_MAX_CAT_EVENT_DATA_LENGTH];
};

/**
 * structure used to store all Event List parameters.
 *
 * \param SetupEventList - Setup event list bit mask
 *                          - 0x00000001 – User Activity Notify
 *                          - 0x00000002 – Idle Screen Available
 *                          - 0x00000004 – Lang Selection Notify
 *                         Each set bit indicates the availability of the
 *                         corresponding event in Setup Event list proactive
 *                         command; all unlisted bits are reserved for future
 *                         use and will be ignored
 *
 */
struct CatEventListTlv
{
    ULONG SetupEventList;
};

/**
 * structure used to store all Refresh Event parameters.
 *
 * \param  RefreshMode  - The Refresh Event as in ETSI TS 102 223 [Section 8.6]
 * \param  RefreshStage - Stage of a refresh procedure
 *                          - 0x01 – Refresh start
 *                          - 0x02 – Refresh success
 *                          - 0x03 – Refresh failed
 */
struct CatRefreshTlv
{
    USHORT RefreshMode;
    BYTE   RefreshStage;
};

/**
 * structure used to store End Proactive Session event parameters.
 *
 * \param   EndProactiveSession - The proactive session end type values are:
 *              - 0x01 – End proactive session command type
 *                       received from the card
 *              - 0x02 – End proactive session internal to ME
 *
 */
struct CatEndProactiveSessionTlv
{
    BYTE EndProactiveSession;
};

/**
 * \internal
 * restore original alignment from stack
 */
#pragma pack(pop)

/**
 *  Union used to represent the current CAT Event Data. Choose the structure
 *  based on the EventID received.
 *      - Use \ref CatEventIDDataTlv if the Event ID is any of the below.
 *          - 16
 *          - 17
 *          - 18
 *          - 19
 *          - 20
 *          - 23
 *          - 24\n
 *      - Use \ref CatAlPhaIdentifierTlv if the Event ID is 21\n
 *      - Use \ref CatEventListTlv if the Event ID is 22\n
 *      - Use \ref CatRefreshTlv if the Event ID is 25\n
 *      - Use \ref CatEndProactiveSessionTlv if the Event ID is 26\n
 *
 */
union currentCatEvent
{
    struct CatEventIDDataTlv         CatEvIDData;
    struct CatAlPhaIdentifierTlv     CatAlphaIdtfr;
    struct CatEventListTlv           CatEventLst;
    struct CatRefreshTlv             CatRefresh;
    struct CatEndProactiveSessionTlv CatEndPS;
};

/**
 * \internal
 * Name:    CatCommonEventTlv
 *
 * Purpose: Structure used to store all Common CAT Event TLV Value.
 *
 * Members: TlvPresent  - Boolean indicating the presence of the TLV
 *                        in the QMI response
 *
 *          EventID     - Event ID. Can be any of the following\n
 *                          16 – Display Text
 *                          17 – Get In-Key
 *                          18 – Get Input
 *                          19 – Setup Menu
 *                          20 – Select Item
 *                          21 – Send SMS – Alpha Identifier
 *                          22 – Setup Event List
 *                          23 – Setup Idle Mode Text
 *                          24 – Language Notification
 *                          25 – Refresh
 *                          26 – End Proactive Session
 *
 *          EventLength - Length of pData ( in Bytes )
 *
 *          CatEvent    - Structure to the Data specific to the CAT event ID
 */
struct CatCommonEventTlv
{
    BYTE                  TlvPresent;
    BYTE                  EventID;
    WORD                  EventLength;
    union currentCatEvent CatEvent;
};

/**
 * \internal
 * Name:    QmiCbkCatEventStatusReportInd
 *
 * Purpose: Structure used to store all CAT Notification Parameters.
 *
 * Members: PDNMEATlv - Structure containing the Position Data NMEA TLV
 *
 * Note:    None
 */
struct QmiCbkCatEventStatusReportInd{
    BYTE   event_Index;
    struct CatCommonEventTlv CCETlv[QMI_CAN_COMMON_EVENT_TLV_NUMBER];
};

/**
 * \internal
 * Prototypes
 */
enum eQCWWANError UpkQmiCbkCatEventReportInd (
    BYTE                                 *pMdmResp,
    struct QmiCbkCatEventStatusReportInd *pAipResp );

#endif /* __CAT_EVENT_REPORT_IND_H__ */

