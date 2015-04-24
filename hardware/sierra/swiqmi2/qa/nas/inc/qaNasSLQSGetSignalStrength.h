/*
 * \ingroup nas
 *
 * \file   qaNasSLQSGetSignalStrength.h
 *
 * \brief  This file contains definitions, enumerations, structures and
 *         forward declarations for qaNasSlqsGetSignalStrength.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_SLQS_GET_SIGNAL_STRENGTH_MSG_H__
#define __NAS_SLQS_GET_SIGNAL_STRENGTH_MSG_H__

/* enum declarations */
/*
 * An enumeration of eQMI_NAS_GET_SIGNAL_STRENGTH request TLV IDs
 *
 */
enum eNAS_SLQS_GET_SIGNAL_STRENGTH_REQ
{
    eTLV_SIGNAL_STRENGTH_REQUEST_MASK = 0x10
};

/*
 * An enumeration of eQMI_NAS_SIGNAL_STRENGTH response TLV IDs
 */
enum eNAS_SLQS_GET_SIGNAL_STRENGTH_RESP
{
    eTLV_SLQS_SIGNAL_STRENGTH      = 0x01,
    eTLV_SLQS_RSSI_LIST            = 0x11,
    eTLV_SLQS_ECIO_LIST            = 0x12,
    eTLV_SLQS_IO                   = 0x13,
    eTLV_SLQS_SINR_LIST            = 0x14,
    eTLV_SLQS_ERROR_RATE_LIST      = 0x15,
    eTLV_SLQS_RSRQ                 = 0x16,
};

/*
 * This structure contains the SLQSGetSignalStrength request parameter.
 *
 * \sa qaGobiApiNas.h for parameter descriptions
 *
 */
struct QmiNasSlqsGetSignalStrengthReq
{
    USHORT reqMask;
};

/*
 * This structure contains the SLQSGetSignalStrength response parameters.
 *
 * \sa qaGobiApiNas.h for parameter descriptions
 *
 */
struct QmiNasSlqsGetSignalStrengthResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* SLQSGetSignalStrengths response parameters */
    struct slqsSignalStrengthInfo *pSignalInfo;
};

/************
 * Prototypes
 **************/
extern enum eQCWWANError PkQmiNasSlqsGetSignalStrength(
    WORD   *pMlength,
    BYTE   *pParamField,
    USHORT reqMask );

extern enum eQCWWANError UpkQmiNasSlqsGetSignalStrength(
    BYTE                                   *pMdmResp,
    struct QmiNasSlqsGetSignalStrengthResp *pAipResp);

#endif /* __NAS_SLQS_GET_SIGNAL_STRENGTH_MSG_H__ */
