/*
 * \ingroup nas
 *
 * \file qaNasGetHomeNetwork.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaNasGetHomeNetwork.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_GET_HOME_NETWORK_MSG_H__
#define __NAS_GET_HOME_NETWORK_MSG_H__

/*
 *
 * An enumeration of eQMI_NAS_GET_HOME_NETWORK response TLV IDs
 *
 */
enum eQMI_NAS_GET_HOME_NETWORK_RESP
{
    eTLV_HOME_NETWORK        = 0x01,
    eTLV_HOME_SYSTEM_ID      = 0x10
};

/*
 * This structure contains the GetHomeNetwork response parameters.
 *
 * \sa qaGobiApiNas.h for parameter descriptions
 *
 */
struct QmiNasGetHomeNetworkResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* Response parameters */
    WORD    *pMCC;
    WORD    *pMNC;
    BYTE     nameSize;
    CHAR    *pName;
    WORD    *pSID;
    WORD    *pNID;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiNasGetHomeNetwork (
    WORD    *pMlength, 
    BYTE    *pParamField );

extern enum eQCWWANError UpkQmiNasGetHomeNetwork (
    BYTE     *pMdmResp,
    struct   QmiNasGetHomeNetworkResp *pAipResp );

#endif /* __NAS_GET_HOME_NETWORK_MSG_H__ */
