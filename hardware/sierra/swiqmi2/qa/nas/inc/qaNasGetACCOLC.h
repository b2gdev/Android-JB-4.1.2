/*
 * \ingroup nas
 *
 * \file qaNasGetACCOLC.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaNasGetACCOLC.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_GET_ACCOLC_MSG_H__
#define __NAS_GET_ACCOLC_MSG_H__

/*
 *
 * An enumeration of eQMI_NAS_GET_ACCOLC response TLV IDs
 *
 */
enum eQMI_NAS_GET_ACCOLC_RESP_TLVs
{
    eTLV_GET_ACCOLC = 0x01
};

/*
 *
 * An enumeration of eQMI_NAS_GET_ACCOLC response TLV Lengths
 *
 */
enum eQMI_NAS_GET_ACCOLC_RESP_TLV_LENGTH
{
    eTLV_GET_ACCOLC_LENGTH = 0x01
};

/*
 * This structure contains the GetACCOLC response parameters.
 *
 * \sa  qaGobiApiNas.h for parameter descriptions
 *
 */
struct QmiNasGetACCOLCResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* Response parameters */
    BYTE *pACCOLC;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiNasGetACCOLC(
    WORD *pMlength,
    BYTE *pParamField );

extern enum eQCWWANError UpkQmiNasGetACCOLC(
    BYTE                       *pMdmResp,
    struct QmiNasGetACCOLCResp *pApiResp );

#endif /* __NAS_GET_ACCOLC_MSG_H__ */
