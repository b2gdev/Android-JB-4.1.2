/*
 * \ingroup pds
 *
 * \file    qaPdsInjectRelativeTimeRef.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaPdsInjectRelativeTimeRef.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __PDS_INJECT_RELATIVE_TIME_REF_H__
#define __PDS_INJECT_RELATIVE_TIME_REF_H__

/* enum declarations */
/*
 * An enumeration of eQMI_PDS_INJECT_RELATIVE_TIME_REF request TLV IDs
 */
enum eQMI_PDS_INJECT_RELATIVE_TIME_REF_REQ
{
    eTLV_TIME_INJECTION = 0x01,
};

/*
 * This structure contains the ResetPDSData request parameters
 */
struct QmiPdsInjectRelativeTimeRefReq
{
    ULONGLONG systemTime;
    USHORT    systemDiscontinuities;
};

/*
 * This structure contains the ResetPDSData response parameters
 */
struct QmiPdsInjectRelativeTimeRefResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * prototypes
 */
extern enum eQCWWANError PkQmiPdsInjectRelativeTimeRef(
    WORD      *pMlength,
    BYTE      *pBuffer,
    ULONGLONG systemTime,
    USHORT    systemDiscontinuities );

extern enum eQCWWANError UpkQmiPdsInjectRelativeTimeRef(
    BYTE                                   *pMdmResp,
    struct QmiPdsInjectRelativeTimeRefResp *pApiResp );

#endif /* __PDS_INJECT_RELATIVE_TIME_REF_H__ */
