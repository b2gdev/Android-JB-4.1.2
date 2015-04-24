/*
 * \ingroup dms
 *
 * \file    qaDmsGetMSISDN.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaDmsGetMSISDN.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __DMS_GET_MSISDN_H__
#define __DMS_GET_MSISDN_H__

/*
 *
 * An enumeration of eQMI_DMS_GET_MSISDN response TLV IDs
 *  
 */
enum eDMS_GET_MSISDN_RESP
{
    eTLV_VOICE_NUMBER  = 0x01,
    eTLV_MOBILE_ID     = 0x10
};

/*
 *
 * This structure contains the GetMSISDN response parameters
 *
 * \sa qaGobiApiDms.h for parameter descriptions
 *
 */
struct QmiDmsGetMSISDNResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* GetMSISDN response parameters */
    BYTE     voiceNumberSize;
    CHAR    *pVoiceNumber;
    BYTE     minSize;
    CHAR    *pMIN;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiDmsGetMSISDN(
    WORD    *pMlength, 
    BYTE    *pBuffer );

extern enum eQCWWANError UpkQmiDmsGetMSISDN(
    BYTE   *mdmresp,
    struct QmiDmsGetMSISDNResp *apiresp);

#endif /* __DMS_GET_MSISDN_H__ */
