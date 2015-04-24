/**
 * \ingroup dcs
 *
 * \file    qaDcsQCWWAN2kEnumerateDevices.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaDcsQCWWAN2kEnumerateDevices.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __DCS_QCWWAN2KENUMERATEDEVICES_H__
#define __DCS_QCWWAN2KENUMERATEDEVICES_H__

typedef struct device_info_param{
  CHAR deviceNode[256];
  CHAR deviceKey[16];
} device_info_t;

/* enum declarations */
/**
 *
 * An enumeration of eQMI_DCS_ENUMERATE_DEVICES response TLV IDs
 *  *
 */
enum eQMI_DCS_ENUMERATE_DEVICES_RESP
{
    eTLV_DEVICES_SIZE   = 0x10,
    eTLV_DEVICE_ARRAY   = 0x11
};

/**
 * This structure contains the QCWWAN2kEnumerateDevices response parameters.
 *
 * \sa qaGobiApiDcs.h for parameter descriptions
 *
 * */
struct QmiDcsQCWWAN2kEnumerateDevicesResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* QCWWAN2kEnumerateDevices response parameters */
    BYTE    *pDevicesSize;
    BYTE    *pDevices;
};

/* Prototypes */
enum eQCWWANError PkQmiDcsQCWWAN2kEnumerateDevices(
    WORD      *pMlength,
    BYTE      *pParamField );

extern enum eQCWWANError UpkQmiDcsQCWWAN2kEnumerateDevices(
        BYTE *pMdmResp,
        struct qm_qmi_response_tlvs_values *pApiResp);


extern ULONG  QCWWAN2kEnumerateDevices(
    BYTE    *pDevicesSize,
    BYTE    *pDevices );

#endif /* __DCS_QCWWAN2KENUMERATEDEVICES_H__ */
