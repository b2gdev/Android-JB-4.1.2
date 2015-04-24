/*
 * \ingroup sms
 *
 * \file    qaSmsSetSMSCAddress.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaSmsSetSMSCAddress.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __SMS_SET_SMSC_ADDRESS_H__
#define __SMS_SET_SMSC_ADDRESS_H__

/* enum declarations */

/*
 * An enumeration of eQMI_SMS_SET_SMSC_ADDRESS request TLV IDs
 *
 */
enum eSMS_SET_SMSC_ADDRESS_REQ
{
    eTLV_ADDRESS_SMSC = 0x01,
    eTLV_TYPE_SMSC    = 0x10
};

/*
 * This structure contains the SetSMSCAddress request parameters
 */
struct QmiSmsSetSMSCAddressReq
{
    CHAR *pSMSCAddress;
    CHAR *pSMSCType;
};

/*
 * This structure contains the SetSMSCAddress response parameters
 */
struct QmiSmsSetSMSCAddressResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiSmsSetSMSCAddress(
    WORD *pMlength,
    BYTE *pParamField,
    CHAR *pSMSCAddress,
    CHAR *pSMSCType );

extern enum eQCWWANError UpkQmiSmsSetSMSCAddress(
    BYTE                            *pMdmResp,
    struct QmiSmsSetSMSCAddressResp *pApiResp );

#endif /* __SMS_SET_SMSC_ADDRESS_H__ */
