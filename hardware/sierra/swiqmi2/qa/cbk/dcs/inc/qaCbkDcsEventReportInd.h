/*
 * \ingroup cbk
 *
 * \file    qaCbkDcsEventReportInd.h
 *
 * \brief   Device Connectivity Service (DCS) Indication support
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DCS_EVENT_REPORT_IND_H__
#define __DCS_EVENT_REPORT_IND_H__

#define MAX_SDK_EVENT_DATA_LENGTH 128
/*
 * Structure used to store all Device State change TLV Values.
 *
 *  \param  TlvPresent - Boolean indicating the presence of the TLV
 *                       in the QMI response
 *
 *  \param  DeviceState - State of the Device
 */
struct deviceStateChangeTlv
{
    BYTE TlvPresent;
    BYTE deviceState;
};

/*
 * Structure used to store all SDK Terminated TLV Values.
 *
 *  \param  TlvPresent - Boolean indicating the presence of the TLV
 *                       in the QMI response
 *
 *  \param  Data       - Termination eason string
 */
struct SDKTerminatedTlv
{
    BYTE TlvPresent;
    BYTE Data[MAX_SDK_EVENT_DATA_LENGTH];
};

/*
 * eQMI_DCS_EVENT_REPORT_IND TLVs defined below
 *
 */

/*
 * Name:    QmiCbkDcsEventStatusReportInd
 *
 * Purpose: Structure used to store Device Connectivity notification parameters
 *
 * Members: DSCTlv - Device State Change Structure
 *          SDKTlv - SDK Termination Structure
 *
 */
struct QmiCbkDcsEventStatusReportInd{
     struct deviceStateChangeTlv DSCTlv;
     struct SDKTerminatedTlv SDKTlv;
};

/*
 * Prototypes
 *
 */
enum eQCWWANError UpkQmiCbkDcsEventReportInd(
    BYTE                                 *pMdmResp,
    struct QmiCbkDcsEventStatusReportInd *pApiResp );

#endif /* __DCS_EVENT_REPORT_IND_H__ */
