/*
 * \ingroup uim
 *
 * \file    qaUimSLQSUimPowerDown.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaUimSLQSUimPowerDown.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __UIM_SLQS_UIM_POWER_DOWN_H__
#define __UIM_SLQS_UIM_POWER_DOWN_H__

#include "qaGobiApiUim.h"

/* enum declarations */
/*
 * An enumeration of eUIM_SLQS_POWER_DOWN_REQ request TLV IDs
 */
enum eUIM_SLQS_POWER_DOWN_REQ
{
    eTLV_POWER_DOWN_SLOT  = 0x01,
};

/*
 * This structure contains the UimSLQSPowerDown response parameters.
 */
struct QmiUimSLQSPowerDownResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiUimSlqsPowerDown(
    WORD            *pMlength,
    BYTE            *pBuffer,
    UIMPowerDownReq *pUIMPowerDownReq );

extern enum eQCWWANError UpkQmiUimSlqsPowerDown(
    BYTE                           *pMdmResp,
    struct QmiUimSLQSPowerDownResp *pApiResp );

#endif /* __UIM_SLQS_UIM_POWER_DOWN_H__ */
