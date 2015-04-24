/*
 * \ingroup pds
 *
 * \file    qaPdsSetGPSServiceState.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaPdsSetGPSServiceState.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __PDS_SET_SERVICE_STATE_MSG_H__
#define __PDS_SET_SERVICE_STATE_MSG_H__

/* enum declarations */
/*
 * An enumeration of eQMI_PDS_SET_SERVICE_STATE request TLV IDs
 *
 */
enum ePDS_SET_SERVICE_STATE_REQ
{
    eTLV_SET_GPS_SERVICE_STATE = 0x01
};

/*
 * This structure contains the SetPDSState request parameters.
 *
 * \sa qaGobiApiPds.h for parameter descriptions
 *
 */
struct QmiPdsSetServiceStateReq
{
    ULONG ServiceState;
};

/*
 * This structure contains the SetPDSState response parameters.
 *
 */
struct QmiPdsSetServiceStateResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 *
 */
extern enum eQCWWANError PkQmiPdsSetServiceState(
    WORD  *pMlength,
    BYTE  *pBuffer,
    ULONG ServiceState );

extern enum eQCWWANError UpkQmiPdsSetServiceState(
    BYTE                             *pMdmResp,
    struct QmiPdsSetServiceStateResp *pApiResp );

#endif /* __PDS_SET_SERVICE_STATE_MSG_H__ */
