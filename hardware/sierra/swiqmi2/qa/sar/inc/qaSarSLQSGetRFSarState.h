/*
 * \ingroup sar
 *
 * \file    qaSarSLQSRFGetState.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaSarSLQSRFGetState.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __SAR_SLQS_RF_GETSTATE_H__
#define __SAR_SLQS_RF_GETSTATE_H__

/* enum declarations */

/*
 * An enumeration of eQMI_SAR_RF_GET_STATE response TLV IDs
 */
enum eQMI_SAR_RF_GET_STATE_RESP
{
    eTLV_SAR_RF_GET_STATE = 0x10
};

/*
 * This structure contains the SarRFGetState response parameters.
 */
struct QmiSarSLQSRFGetStateResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
    ULONG  *pRfSarState;
};

/*
 * An enumeration of eQMI_SAR_RF_GET_STATE response TLV Lengths
 */
enum eQMI_SAR_RF_SATATE_RESP_LENGTH
{
    eTLV_SAR_RF_SATATE_LENGTH = 0x04
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiSarSLQSRFGetState(
    WORD  *pMlength,
    BYTE  *pBuffer );

extern enum eQCWWANError UpkQmiSarSLQSRFGetState(
    BYTE                            *pMdmResp,
    struct QmiSarSLQSRFGetStateResp *pApiResp );

#endif /* __SAR_SLQS_RF_GETSTATE_H__ */
