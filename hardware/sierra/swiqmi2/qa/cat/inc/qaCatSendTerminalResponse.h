/*
 * \ingroup cat
 *
 * \file    qaCatSendTerminalResponse.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaCatSendTerminalResponse.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __CAT_SEND_TR_H__
#define __CAT_SEND_TR_H__

/* enum declarations */

/*
 * An enumeration of eCAT_SEND_TR_REQ request TLV IDs
 *
 */
enum eCAT_SEND_TR_REQ
{
    eTLV_TERMINAL_RESPONSE = 0x01
};

/*
 * This structure contains the CATSendTerminalResponse request parameters.
 * \param  refID
 *         - UIM reference ID
 *
 * \param  dataLen
 *         - Terminal response data length
 *
 * \param  pData[IN]
 *         - Terminal response data
 *  
 */
struct QmiCatSendTerminalResponseReq
{
     ULONG refID;
     ULONG dataLen;
     BYTE  *pData;
};

/*
 * This structure contains the CATSendTerminalResponse response parameters
 */
struct QmiCatSendTerminalResponseResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 *
 * \param  pMlength[OUT]
 *         - Total length of built message.
 *
 *  \param  pParamField [OUT]
 *          - Pointer to storage into which the packed
 *            data will be placed by this function.
 *
 * \param  refID
 *         - UIM reference ID 
 *
 * \param  dataLen
 *         - Length of pData in bytes
 *
 * \param  pData[IN]
 *         - Encoded envelope data
 *
 */
enum eQCWWANError PkQmiCatSendTerminalResponse(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG refID,
    ULONG dataLen,
    BYTE  *pData );

extern enum eQCWWANError UpkQmiCatSendTerminalResponse(
    BYTE                                  *pMdmResp,
    struct QmiCatSendTerminalResponseResp *pApiResp);

#endif /* __CAT_SEND_TR_H__ */
