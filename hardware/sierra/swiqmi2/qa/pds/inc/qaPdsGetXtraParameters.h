/*
 * \ingroup pds
 *
 * \file    qaPdsGetXtraParameters.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaPdsGetXtraParameters.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __PDS_GET_XTRA_PARAMETERS_MSG_H__
#define __PDS_GET_XTRA_PARAMETERS_MSG_H__

/*
 * An enumeration of eQMI_PDS_GET_XTRA_PARAMETERS response TLV IDs
 */
enum eQMI_PDS_GET_XTRA_PARAMETERS_RESP
{
    eTLV_GET_XTRA_DB_AUTODWNLD      = 0x10,
    eTLV_GET_XTRA_DB_NETWORK_CONFIG = 0x12,
    eTLV_GET_XTRA_DB_VALIDITY       = 0x13
};

/*
 * An enumeration of eQMI_PDS_GET_XTRA_PARAMETERS response TLV Lengths
 */
enum eQMI_PDS_GET_XTRA_PARAMETERS_RESP_LENGTH
{
    eTLV_GET_XTRA_DB_AUTODWNLD_LEN      = 0x03,
    eTLV_GET_XTRA_DB_NETWORK_CONFIG_LEN = 0x01,
    eTLV_GET_XTRA_DB_VALIDITY_LEN       = 0x06
};

/*
 * This structure contains the GetXtraParameters response parameters
 */
struct QmiPdsGetXtraParametersResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* GetXtraParameters response parameters */
    ULONG  *pbEnabled;
    USHORT *pInterval;
    ULONG  *pPreference;
    USHORT *pGPSWeek;
    USHORT *pGPSWeekOffset;
    USHORT *pDuration;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiPdsGetXtraParameters(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiPdsGetXtraParameters(
    BYTE                               *pMdmResp,
    struct QmiPdsGetXtraParametersResp *pApiResp );

#endif /* __PDS_GET_XTRA_PARAMETERS_MSG_H__ */
