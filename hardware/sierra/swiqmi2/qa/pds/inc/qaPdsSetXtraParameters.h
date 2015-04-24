/*
 * \ingroup pds
 *
 * \file    qaPdsSetXtraParameters.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaPdsSetXtraParameters.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __PDS_SET_XTRA_PARAMETERS_MSG_H__
#define __PDS_SET_XTRA_PARAMETERS_MSG_H__

/* enum declarations */
/*
 * An enumeration of eQMI_PDS_SET_XTRA_PARAMETERS request TLV IDs
 */
enum ePDS_SET_XTRA_PARAMETERS_REQ
{
    eTLV_SET_XTRA_DB_AUTODWNLD      = 0x10,
    eTLV_SET_XTRA_DB_NETWORK_CONFIG = 0x12
};

/*
 * This structure contains the SetXtraParameters request parameters.
 */
struct QmiPdsSetXtraParametersReq
{
    ULONG  *pbEnabled;
    USHORT *pinterval;
    ULONG  *ppreference;
};

/*
 * This structure contains the SetXtraParameters response parameters.
 */
struct QmiPdsSetXtraParametersResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiPdsSetXtraParameters(
    WORD   *pMlength,
    BYTE   *pBuffer,
    ULONG  *pbEnabled,
    USHORT *pinterval,
    ULONG  *ppreference );

extern enum eQCWWANError UpkQmiPdsSetXtraParameters(
    BYTE                               *pMdmResp,
    struct QmiPdsSetXtraParametersResp *pApiResp );

#endif /* __PDS_SET_XTRA_PARAMETERS_MSG_H__ */
