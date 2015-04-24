/*
 * \ingroup nas
 *
 * \file  qaNasSLQSInitiateNetworkregistration.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaNasSLQSInitiateNetworkregistration.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_SLQS_INITIATE_NETWORK_REGISTER_MSG_H__
#define __NAS_SLQS_INITIATE_NETWORK_REGISTER_MSG_H__

#define AUTOMATIC_REGISTRATION  0x01
#define MANUAL_REGISTRATION     0x02
#define ACESS_TECHNOLOGY_GSM    0x04
#define ACESS_TECHNOLOGY_UMTS   0x05
#define ACESS_TECHNOLOGY_LTE    0x08

/* enum declarations */
/*
 *
 * An enumeration of eQMI_NAS_INTIATE_NETWORK_REGISTRATION request TLV IDs
 *
 */
enum eNAS_SLQS_INTIATE_NETWORK_REGISTER_REQ
{
    eTLV_NAS_REGISTER_ACTION        = 0x01,
    eTLV_NAS_MANUAL_REGISTER_ACTION = 0x10,
    eTLV_NAS_CHANGE_DURATION        = 0x11,
    eTLV_NAS_MNC_PCS_DIGIT_STATUS   = 0x12
};

/*
 * This structure contains the SLQSInitiateNetworkRegistration request parameters.
 *
 * \sa qaGobiApiNas.h for parameter descriptions
 *
 */
struct QmiNasSLQSInitiateNetworkRegistrationReq
{
    nasInitNetworkReg *pNasInitNetworkReg;
};

/*
 * This structure contains the SLQSInitiateNetworkRegistration response parameters.
 *
 */
struct QmiNasSLQSInitiateNetworkRegistrationResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiNasSLQSInitiateNetworkRegistration (
    WORD              *pMlength,
    BYTE              *pParamField,
    nasInitNetworkReg *pNasInitNetReg );

extern enum eQCWWANError UpkQmiNasSLQSInitiateNetworkRegistration (
    BYTE                                             *pMdmResp,
    struct QmiNasSLQSInitiateNetworkRegistrationResp *pApiResp );

#endif /* __NAS_SLQS_INITIATE_NETWORK_REGISTER__MSG_H__ */
