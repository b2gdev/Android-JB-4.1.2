/*
 * \ingroup pds
 *
 * \file    qaPdsInjectAbsoluteTimeRef.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaPdsInjectAbsoluteTimeRef.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __PDS_INJECT_ABSOLUTE_TIME_REF_H__
#define __PDS_INJECT_ABSOLUTE_TIME_REF_H__

/* enum declarations */
/*
 * An enumeration of eQMI_PDS_INJECT_ABSOLUTE_TIME_REF request TLV IDs
 */
enum eQMI_PDS_INJECT_ABSOLUTE_TIME_REF_REQ
{
    eTLV_ABSOLUTE_TIME_INJECTION = 0x01,
};

/*
 * This structure contains the PDSInjectAbsoluteTimeReference request parameters
 */
struct QmiPdsInjectAbsoluteTimeRefReq
{
    ULONGLONG timeMsec;
    ULONG     timeUncMsec;
    BYTE      timeBase;
    BYTE      forceFlag;
};

/*
 * This structure contains the ResetPDSData response parameters
 */
struct QmiPdsInjectAbsoluteTimeRefResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * prototypes
 */
extern enum eQCWWANError PkQmiPdsInjectAbsoluteTimeRef(
    WORD      *pMlength,
    BYTE      *pBuffer,
    ULONGLONG timeMsec,
    ULONG     timeUncMsec,
    BYTE      timeBase,
    BYTE      forceFlag );

extern enum eQCWWANError UpkQmiPdsInjectAbsoluteTimeRef(
    BYTE                                   *pMdmResp,
    struct QmiPdsInjectAbsoluteTimeRefResp *pApiResp );

#endif /* __PDS_INJECT_ABSOLUTE_TIME_REF_H__ */
