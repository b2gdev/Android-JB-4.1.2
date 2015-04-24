/*
 * \ingroup pds
 *
 * \file qaPdsGetGPSServiceState.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaPdsGetGPSServiceState.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __PDS_GET_SERVICE_STATE_H__
#define __PDS_GET_SERVICE_STATE_H__

/*
 * An enumeration of eQMI_PDS_GET_SERVICE_STATE response TLV IDs
 */
enum eQMI_PDS_GET_SERVICE_STATE_RESP
{
    eTLV_GET_GPS_SERVICE_STATE = 0x01
};

/*
 * An enumeration of eQMI_PDS_GET_SERVICE_STATE response TLV Lengths
 */
enum eQMI_PDS_GET_SERVICE_STATE_RESP_LENGTH
{
    eTLV_GET_GPS_SERVICE_STATE_LENGTH = 0x02
};

/*
 * This structure contains the GetPDSState response parameters
 *
 * \sa qaGobiApiPds.h for parameter descriptions
 *
 */
struct QmiPdsGetServiceStateResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* GetPDSState response parameters */
    ULONG *pEnabledStatus;
    ULONG *pTrackingStatus;
};

/*
 * Prototypes
 *
 */
extern enum eQCWWANError PkQmiPdsGetServiceState(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiPdsGetServiceState(
    BYTE                             *pMdmResp,
    struct QmiPdsGetServiceStateResp *pApiResp );

#endif /* __PDS_GET_SERVICE_STATE_H__ */
