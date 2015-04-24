/*
 * \ingroup sms
 *
 * \file    qaSmsGetSMSCAddress.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaSmsGetSMSCAddress.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __SMS_GET_SMSC_ADDRESS_H__
#define __SMS_GET_SMSC_ADDRESS_H__

/* To be used for SMSC Type Length by various APIs */
#define QMI_SMSC_TYPE_LEN 0x03

/* enum declarations */

/*
 * An enumeration of eQMI_SMS_GET_SMSC_ADDRESS request TLV IDs
 *
 */
enum eSMS_GET_SMSC_ADDRESS_RESP
{
    eTLV_SMSC_ADDRESS = 0x01
};

/*
 * This structure contains the GetSMSCAddress 
 * response parameters
 *
 * \sa qaGobiApiSms.h for parameter descriptions
 *
 */
struct QmiSmsGetSMSCAddressResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* GetSMSCAddress response parameters */
    BYTE addressSize;
    CHAR *pSMSCAddress;
    CHAR *pSMSCType;
};

/*
 * Prototypes
 *
 */
extern enum eQCWWANError PkQmiSmsGetSMSCAddress(
    WORD *pMlength,
    BYTE *pParamField );

extern enum eQCWWANError UpkQmiSmsGetSMSCAddress(
    BYTE                            *pMdmResp,
    struct QmiSmsGetSMSCAddressResp *pApiResp );

#endif /* __SMS_GET_SMSC_ADDRESS_H__ */
