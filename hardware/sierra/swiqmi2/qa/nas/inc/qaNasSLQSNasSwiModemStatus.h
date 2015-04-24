/*
 * \ingroup nas
 *
 * \file   qaNasSLQSNasSwiModemStatus.h
 *
 * \brief  This file contains definitions, enumerations, structures and
 *         forward declarations for qaNasSLQSNasSwiModemStatus.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_SLQS_SWI_MODEM_STATUS_H__
#define __NAS_SLQS_SWI_MODEM_STATUS_H__

#include "qaGobiApiNas.h"

/* enum declarations */
/*
 * An enumeration of eNAS_SLQS_SWI_MODEM_STATUS_RESP response TLV IDs
 */
enum eNAS_SLQS_SWI_MODEM_STATUS_RESP
{
    eTLV_MODEM_COMMOM_INFO = 0x01,
    eTLV_MODEM_LTE_INFO    = 0x10
};

/*
 * This structure contains the SLQSNasSwiModemStatus response parameters.
 */
struct QmiNasSLQSSwiModemStatusResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    swiModemStatusResp *pModemStatus;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiNasSlqsSwiModemStatus(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiNasSlqsSwiModemStatus(
    BYTE                                *pMdmResp,
    struct QmiNasSLQSSwiModemStatusResp *pApiResp );

#endif /* __NAS_SLQS_SWI_MODEM_STATUS_H__ */
