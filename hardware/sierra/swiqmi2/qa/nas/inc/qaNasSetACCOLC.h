/*
 * \ingroup nas
 *
 * \file  qaNasSetACCOLC.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaNasSetACCOLC.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_SET_ACCOLC_MSG_H__
#define __NAS_SET_ACCOLC_MSG_H__

/* enum declarations */

/*
 *
 * An enumeration of eQMI_NAS_SET_ACCOLC request TLV IDs
 *
 */
enum eNAS_SET_ACCOLC_REQ
{
    eTLV_SET_ACCOLC = 0x01
};

/*
 * This structure contains the SetACCOLC request parameters.
 *
 * \sa  qaGobiApiNas.h for parameter descriptions
 *
 */
struct QmiNasSetACCOLCReq
{
    CHAR *spc;
    BYTE accolc;
};


/*
 * This structure contains the SetACCOLC response parameters
 *
 */
struct QmiNasSetACCOLCResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 *
 */
extern enum eQCWWANError PkQmiNasSetACCOLC(
    WORD *pMlength,
    BYTE *pBuffer,
    CHAR *spc,
    BYTE accolc);

extern enum eQCWWANError UpkQmiNasSetACCOLC(
    BYTE                       *pMdmresp,
    struct QmiNasSetACCOLCResp *pApiresp);

#endif /* __NAS_SET_ACCOLC_MSG_H__ */
