/*
 * \ingroup wds
 *
 * \file qaWdsGetCurrentChannelRate.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaWdsGetCurrentChannelRate.c
 *
 * Copyright: © 2010 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __WDS_GET_CURRENT_CHANNEL_RATE_MSG_H_
#define __WDS_GET_CURRENT_CHANNEL_RATE_MSG_H_

/*
 * This enumeration contains the TLV IDs for
 * QMI_WDS_GET_CURRENT_CHANNEL_RATE response
 */
enum eWDS_GET_CURRENT_CHANNEL_RATE_RESP
{
    eTLV_CHANNEL_RATE = 0x01
};

/*
 * This structure contains the GetConnectionRate response parameters
 *
 * \See QCWWANCMAPI2k.h for parameter descriptions
 */
struct QmiWdsGetCurrentChannelRateResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* GetConnectionRate response parameters */
    ULONG *pCurrentChannelTXRate;
    ULONG *pCurrentChannelRXRate;
    ULONG *pMaxChannelTXRate;
    ULONG *pMaxChannelRXRate;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiWdsGetCurrentChannelRate(
    WORD *pMlength,
    BYTE *pParamField  );

extern enum eQCWWANError UpkQmiWdsGetCurrentChannelRate(
    BYTE   *mdmresp,
    struct QmiWdsGetCurrentChannelRateResp *apiresp );

#endif /* __WDS_GET_CURRENT_CHANNEL_RATE_MSG_H__ */
