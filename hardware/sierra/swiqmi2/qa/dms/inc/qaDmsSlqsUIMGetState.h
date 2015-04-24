/**
 * \ingroup dms
 *
 * \file    qaDmsSlqsUIMGetState.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaDmsSlqsUIMGetState.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DMS_UIM_GET_STATE_H__
#define __DMS_UIM_GET_STATE_H__

/**
 * An enumeration of eQMI_DMS_UIM_GET_STATE response TLV IDs
 */
enum DMS_UIM_GET_STATE_RESP

{
    eTLV_UIM_GET_STATE = 0x01
};

/*
 * An enumeration of eQMI_UIM_GET_STATE response TLV Lengths
 */
enum eQMI_UIM_GET_STATE_RESP_LENGTH
{
    eTLV_UIM_GET_STATE_RESP_LEN      = 0x01
};

/**
 * This structure contains the UIMGetState response parameters
 */
struct QmiDmsUIMGetStateResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* UIMGetState response parameters */
    ULONG *pUIMState;
};

/**
 * Prototypes
 */
extern enum eQCWWANError PkQmiDmsUIMGetState(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiDmsUIMGetState(
    BYTE                                *pMdmResp,
    struct QmiDmsUIMGetStateResp *pApiResp );

#endif /* __DMS_UIM_GET_STATE_H__ */

