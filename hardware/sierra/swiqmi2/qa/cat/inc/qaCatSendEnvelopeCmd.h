/*
 * \ingroup cat
 *
 * \file    qaCatSendEnvelopeCmd.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaCatSendEnvelopeCmd.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __CAT_SEND_ENVELOPE_CMD_H__
#define __CAT_SEND_ENVELOPE_CMD_H__

/* enum declarations */

/*
 * An enumeration of eCAT_SEND_ENVELOPE_CMD_REQ request TLV IDs
 *
 */
enum eCAT_SEND_ENVELOPE_CMD_REQ
{
    eTLV_ENVELOPE_COMMAND = 0x01
};

/*
 * This structure contains the CATSendEnvelopeCommand request parameters.
 *
 * \param  cmdID
 *         - Envelope command type
 *
 * \param  dataLen
 *         - Length of pData in bytes
 *
 * \param  pData[IN]
 *         - Encoded envelope data
 *
 */
struct QmiCatSendEnvelopeCmdReq
{
     ULONG cmdID;
     ULONG dataLen;
     BYTE  *pData;
};

/*
 * This structure contains the CATSendEnvelopeCommand
 * response parameters
 *
 */
struct QmiCatSendEnvelopeCmdResp
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
 * \param  cmdID
 *         - Envelope command type
 *
 * \param  dataLen
 *         - Length of pData in bytes
 *
 * \param  pData[IN]
 *         - Encoded envelope data
 *
 */
enum eQCWWANError PkQmiCatSendEnvelopeCmd(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG cmdID,
    ULONG dataLen,
    BYTE  *pData );

extern enum eQCWWANError UpkQmiCatSendEnvelopeCmd(
    BYTE                                 *pMdmResp,
    struct QmiCatSendEnvelopeCmdResp     *pApiResp);

#endif /* __CAT_SEND_ENVELOPE_CMD_H__ */
