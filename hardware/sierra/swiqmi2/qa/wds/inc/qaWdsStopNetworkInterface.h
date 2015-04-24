/*
 *\ingroup wds
 *
 * \file qaWdsStopNetworkInterface.h
 *
 * \brief Header file for WDS_STOP_NETWORK_INTERFACE interface
 *
 * Copyright: © 2010 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __WDS_STOP_NETWORK_INTERFACE_H__
#define __WDS_STOP_NETWORK_INTERFACE_H__

/*
 * This enumeration contains the TLV IDs for
 * WDS_STOP_NETWORK_INTERFACE requests
 */
enum eWDS_STOP_NETWORK_INTERFACE_REQ
{
    eTLV_PKT_DATA_HANDLE     = 0x01,
    eTLV_DISABLE_AUTOCONNECT = 0x10
};

/*
 * This structure contains the StopDataSession request parameters
 *
 * \See QCWWANCMAPI2k.h for parameter descriptions
 */
struct QmiWdsStopNetworkInterfaceReq
{
    ULONG pktDataHandle;
    BYTE  disableAutoconnect;
};

/*
 * This structure contains the StopDataSession response parameters
 *
 * \See QCWWANCMAPI2k.h for parameter descriptions
 */
struct QmiWdsStopNetworkInterfaceResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiWdsStopNetworkInterface(
    WORD    *pMlength,
    BYTE    *pBuffer,
    ULONG   pktDataHandle,
    BYTE    disableAutoconnect );

extern enum eQCWWANError UpkQmiWdsStopNetworkInterface(
    BYTE   *pMdmResp,
    struct QmiWdsStopNetworkInterfaceResp *pApiResp );

#endif /* __WDS_STOP_NETWORK_INTERFACE_MSG_H__ */
