/*
 * \ingroup nas
 *
 * \file qaNasGetServingSystem.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaNasGetServingSystem.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_GET_SERVING_SYSTEM_MSG_H__
#define __NAS_GET_SERVING_SYSTEM_MSG_H__

#define QMI_NAS_MAX_RI_LIST 20

/*
 *
 * An enumeration of eQMI_NAS_GET_SERVING_SYSTEM response TLV IDs
 *
 */
enum eQMI_NAS_GET_SERVING_SYSTEM_RESP
{
    eTLV_SERVING_SYSTEM     = 0x01,
    eTLV_ROAMING_INDICATOR  = 0x10,
    eTLV_CURRENT_PLMN       = 0x12,
    eTLV_SERVICE_CAPABILITY = 0x11
};

/*
 * This structure contains the GetServingSystem response parameters.
 *
 * \sa  qaGobiApiNas.h for parameter descriptions
 *
 */
struct QmiNasGetServingSystemResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* Response parameters */
    ULONG *pRegistrationState;
    ULONG *pCSDomain;
    ULONG *pPSDomain;
    ULONG *pRAN;
    BYTE  *pRadioIfacesSize;
    BYTE  *pRadioIfaces;
    ULONG *pRoaming;
    WORD  *pMCC;
    WORD  *pMNC;
    BYTE  nameSize;
    CHAR  *pName;
    BYTE  *pDataCapsLen;
    ULONG *pDataCaps;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiNasGetServingSystem(
    WORD *pMlength,
    BYTE *pParamField );

extern enum eQCWWANError UpkQmiNasGetServingSystem(
    BYTE                              *pMdmResp,
    struct QmiNasGetServingSystemResp *pApiResp );

#endif /* __NAS_GET_SERVING_SYSTEM_MSG_H__ */
