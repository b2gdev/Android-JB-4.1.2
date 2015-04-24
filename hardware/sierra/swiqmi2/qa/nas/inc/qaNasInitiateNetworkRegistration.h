
/*
 * \ingroup nas
 *
 * \file qaNasInitiateNetworkregistration.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaNasIntiateNetworkregistration.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_INITIATE_NETWORK_REGISTER_MSG_H__
#define __NAS_INITIATE_NETWORK_REGISTER_MSG_H__


#define QMI_NAS_AUTOMATIC_REGISTRATION  0x01
#define QMI_NAS_MANUAL_REGISTRATION     0x02

#define QMI_NAS_ACESS_TECHNOLOGY_GSM    0x04
#define QMI_NAS_ACESS_TECHNOLOGY_UMTS   0x05
#define QMI_NAS_ACESS_TECHNOLOGY_LTE    0x08

/* enum declarations */
/*
 *
 * An enumeration of eQMI_NAS_INTIATE_NETWORK_REGISTRATION request TLV IDs
 *
 */
enum eNAS_INTIATE_NETWORK_REGISTER_REQ
{
    eTLV_REGISTER_ACTION         = 0x01,
    eTLV_MANUAL_REGISTER_ACTION  = 0x10
};

/*
 * This structure contains the InitiateNetworkRegistration request parameters.
 *
 * \sa qaGobiApiNas.h for parameter descriptions
 *
 */
struct QmiNasInitiateNetworkRegistrationReq
{
    ULONG regType;
    WORD  mcc;
    WORD  mnc;
    ULONG rat;
};

/*
 * This structure contains the InitiateNetworkRegistration response parameters.
 *
 */
struct QmiNasInitiateNetworkRegistrationResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiNasInitiateNetworkRegistration (
    WORD      *pMlength, 
    BYTE      *pParamField, 
    ULONG     regType,
    WORD      mcc,
    WORD      mnc,
    ULONG     rat );

extern enum eQCWWANError UpkQmiNasInitiateNetworkRegistration(
    BYTE     *pMdmResp,
    struct   QmiNasInitiateNetworkRegistrationResp *pAipResp);

#endif /* __NAS_INITIATE_NETWORK_REGISTER_MSG_H__ */
