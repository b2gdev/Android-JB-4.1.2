/*
 * \ingroup dms
 *
 * \file qaDmsUIMGetIMSI.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaDmsUIMGetIMSI.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DMS_UIM_GET_IMSI_H__
#define __DMS_UIM_GET_IMSI_H__

/*
 *
 * An enumeration of eDMS_UIM_GET_IMSI response TLV IDs
 *  
 */
enum eDMS_UIM_GET_IMSI
{
    eTLV_MOBILE_SUBSCRIBER_ID  = 0x01
};

/*
 *
 * This structure contains the GetIMSI response parameters
 *
 * \sa qaGobiApiDms.h for parameter descriptions
 *
 */
struct QmiDmsUIMGetIMSIResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* GetMSISDN response parameters */
    BYTE     IMSISize;
    CHAR    *pIMSI;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiDmsUIMGetIMSI(
    WORD    *pMlength, 
    BYTE    *pBuffer );

extern enum eQCWWANError UpkQmiDmsUIMGetIMSI(
    BYTE   *mdmresp,
    struct QmiDmsUIMGetIMSIResp *apiresp);

#endif /* __DMS_UIM_GET_IMSI_H__ */
