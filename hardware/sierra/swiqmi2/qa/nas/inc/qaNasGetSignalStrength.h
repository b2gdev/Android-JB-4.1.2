/*
 * \ingroup nas
 *
 * \file qaNasGetSignalStrength.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaNasGetSignalStrength.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_GET_SIGNAL_STRENGTH_MSG_H__
#define __NAS_GET_SIGNAL_STRENGTH_MSG_H__

/* Constants */
/* The request mask needs to be defined */
#define SIGNAL_STRENGTH_REQ_MASK 0x0000

/* enum declarations */
/*
 *
 * An enumeration of eQMI_NAS_GET_SIGNAL_STRENGTH request TLV IDs
 *
 */
enum eNAS_GET_SIGNAL_STRENGTH_REQ
{
    eTLV_REQUEST_MASK                 = 0x10
};

/*
 *
 * An enumeration of eQMI_NAS_SIGNAL_STRENGTH response TLV IDs
 *
 */
enum eNAS_GET_SIGNAL_STRENGTH_RESP
{
    eTLV_SIGNAL_STRENGTH          = 0x01,
    eTLV_SIGNAL_STRENGTH_LIST     = 0x10
};

/*
 * This structure contains the GetSignalStrength response parameters.
 *
 * \sa qaGobiApiNas.h for parameter descriptions
 *
 */
struct QmiNasGetSignalStrengthResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* GetSignalStrengths response parameters */
    ULONG   *pArraySizes;
    INT8    *pSignalStrengths;
    ULONG   *pRadioInterfaces;
};

/************
 * Prototypes
 **************/
extern enum eQCWWANError PkQmiNasGetSignalStrengths (
    WORD     *pMlength, 
    BYTE     *pParamField );

extern enum eQCWWANError UpkQmiNasGetSignalStrengths(
    BYTE     *pMdmResp,
    struct   QmiNasGetSignalStrengthResp *pAipResp);

#endif /* __NAS_GET_SIGNAL_STRENGTH_MSG_H__ */
