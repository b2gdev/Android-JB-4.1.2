/*
 * \ingroup sar
 *
 * \file    qaSarSLQSSetRFSarState.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaSarSLQSSetRFSarState.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __SAR_SLQS_SET_RF_SAR_STATE_H__
#define __SAR_SLQS_SET_RF_SAR_STATE_H__

/* enum declarations */

/*
 * An enumeration of eQMI_SAR_RF_SET_STATE request TLV IDs
 */
enum eQMI_SAR_RF_SET_STATE_REQ
{
    eTLV_SAR_RF_STATE = 0x01
};

/*
 * This structure contains the SarRFSetState request parameters.
 */
struct QmiSarSLQSSetRFStateReq
{
    ULONG rf_sar_state;
};

/*
 * This structure contains the SarRFSetState response parameters.
 */
struct QmiSarSLQSSetRFStateResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiSarSLQSSetRFState(
    WORD  *pMlength,
    BYTE  *pBuffer,
    ULONG RFSarState );

extern enum eQCWWANError UpkQmiSarSLQSSetRFState(
    BYTE                            *pMdmResp,
    struct QmiSarSLQSSetRFStateResp *pApiResp );

#endif /* __SAR_SLQS_SET_RF_SAR_STATE_H__ */
