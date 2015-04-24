/*
 * \ingroup nas
 *
 * \file qaNasSLQSGetTxRxInfo.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaNasSLQSGetTxRxInfo.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_SLQS_GET_TX_RX_INFO_MSG_H__
#define __NAS_SLQS_GET_TX_RX_INFO_MSG_H__

#include "qaGobiApiNas.h"

/* enum declarations */
/*
 * An enumeration of eQMI_NAS_SLQS_GET_TX_RX_INFO_REQ response TLV IDs
 */
enum eVOICE_SLQS_GET_TX_RX_INFO_REQ
{
    eTLV_RADIO_INTERFACE = 0x01
};

/*
 * An enumeration of eQMI_NAS_SLQS_GET_TX_RX_INFO_RESP response TLV IDs
 */
enum eVOICE_SLQS_GET_TX_RX_INFO_RESP
{
    eTLV_RX_CHAIN_0_INFO = 0x10,
    eTLV_RX_CHAIN_1_INFO = 0x11,
    eTLV_TX_INFO         = 0x12
};

/*
 * This structure contains the NasSLQSGetTxRxInfo response parameters.
 */
struct QmiNasSLQSGetTxRxInfoResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    nasGetTxRxInfoResp *pGetTxRxInfoResp;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiNasSlqsGetTxRxInfo(
    WORD               *pMlength,
    BYTE               *pBuffer,
    nasGetTxRxInfoReq  *pGetTxRxInfoReq );

extern enum eQCWWANError UPkQmiNasSlqsGetTxRxInfo(
    BYTE                             *pMdmResp,
    struct QmiNasSLQSGetTxRxInfoResp *pApiResp );

#endif /* __NAS_SLQS_GET_TX_RX_INFO_MSG_H__ */
