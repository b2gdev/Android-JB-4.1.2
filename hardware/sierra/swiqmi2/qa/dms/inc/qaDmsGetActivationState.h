/**
 * \ingroup dms
 *
 * \file    qaDmsGetActivationState.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaDmsGetActivationState.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DMS_GET_ACTIVATION_STATE_H__
#define __DMS_GET_ACTIVATION_STATE_H__

/**
 * An enumeration of eQMI_DMS_GET_ACTIVATION_STATE response TLV IDs
 */
enum DMS_GET_ACTIVATION_STATE_RESP
{
    eTLV_ACTIVATION_STATE = 0x01
};

/**
 * This structure contains the GetActivationState response parameters
 */
struct QmiDmsGetActivationStateResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* GetActivationState response parameters */
    ULONG *pActivationState;
};

/**
 * Prototypes
 */
extern enum eQCWWANError PkQmiDmsGetActivationState(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiDmsGetActivationState(
    BYTE                                *pMdmResp,
    struct QmiDmsGetActivationStateResp *pApiResp );

#endif /* __DMS_GET_ACTIVATION_STATE_H__ */

