/*
 * \ingroup cbk
 *
 * \file    qaCbkFmsEventReportInd.h
 *
 * \brief   Firmware Management Service (FMS) Indication support
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __FMS_EVENT_REPORT_IND_H__
#define __FMS_EVENT_REPORT_IND_H__

/*
 * Structure used to store all Firmware Download Completion TLV Values.
 *
 *  \param  TlvPresent - Boolean indicating the presence of the TLV in the QMI
 *                       response.
 *
 *  \param  fwDldCompletionStatus - firmware download completion status
 */
struct FwDldCompletionTlv
{
    BYTE TlvPresent;
    /* RILSTART */
    ULONG FwDldCompletionStatus;
    /* RILSTOP */
};

/*
 * eQMI_FMS_EVENT_REPORT_IND TLVs defined below
 *
 */

/*
 * Name:    QmiCbkFmsEventStatusReportInd
 *
 * Purpose: Structure used to store Device Connectivity notification parameters
 *
 * Members: FDCTlv   - Firmware Download Completion structure
 *
 */
struct QmiCbkFmsEventStatusReportInd{
     struct FwDldCompletionTlv FDCTlv;
};

/*
 * Prototypes
 *
 */
enum eQCWWANError UpkQmiCbkFmsEventReportInd(
    BYTE                                    *pMdmResp,
    struct QmiCbkFmsEventStatusReportInd    *pApiResp );

#endif /* __FMS_EVENT_REPORT_IND_H__ */
