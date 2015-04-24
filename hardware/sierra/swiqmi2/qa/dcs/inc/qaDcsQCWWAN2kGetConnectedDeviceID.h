/**
 * \ingroup dcs
 *
 * \file    qaDcsQCWWAN2kGetConnectedDeviceID.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaDcsQCWWAN2kGetConnectedDeviceID.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __DCS_QCWWAN2KGETCONNECTEDDEVICEID_H__
#define __DCS_QCWWAN2KGETCONNECTEDDEVICEID_H__

/* enum declarations */
/**
 *
 * An enumeration of eQMI_DCS_GET_CONNECTED_DEVICE_ID request TLV IDs
 *  *
 */
//enum eQMI_DCS_GET_CONNECTED_DEVICE_ID
//{
//    eTLV_DEVICES_SIZE   = 0x10,
//    eTLV_DEVICE_ARRAY   = 0x11
//};

/**
 * This structure contains the QCWWAN2kGetConnectedDeviceID response parameters.
 *
 * \sa qaGobiApiDcs.h for parameter descriptions
 *
 */
struct QmiDcsQCWWAN2kGetConnectedDeviceIDResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* QCWWAN2kGetConnectedDeviceID response parameters */
    ULONG   deviceIDSize;
    CHAR    *pDeviceID;
    ULONG   deviceKeySize;
    CHAR    *pDeviceKey;
};

/* Prototypes */
extern ULONG  QCWWAN2kGetConnectedDeviceID(
    ULONG   deviceIDSize,
    CHAR    *pDeviceID,
    ULONG   deviceKeySize,
    CHAR    *pDeviceKey );

extern enum eQCWWANError PkQmiDcsQCWWAN2kGetConnectedDeviceID(
    WORD    *pMlength,
    BYTE    *pParamField);

extern enum eQCWWANError UpkQmiDcsQCWWAN2kGetConnectedDeviceID(
    BYTE    *pMdmResp,
    struct qm_qmi_response_tlvs_values *pApiResp );

#endif /* __DCS_QCWWAN2KGETCONNECTEDDEVICEID_H__ */
