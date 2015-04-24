/*
 * \ingroup wds
 *
 * \file qaWdsSetMipParams.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaSetMipParams.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __WDS_SET_MIP_PARAMS_MSG_H__
#define __WDS_SET_MIP_PARAMS_MSG_H__

/* For Parameter Validations */

#define QMI_WDS_MIP_MODE_OFF       0x00
#define QMI_WDS_MIP_MODE_PREFERRED 0x01
#define QMI_WDS_MIP_MODE_ONLY      0x02
#define QMI_WDS_DISABLED           0x00
#define QMI_WDS_ENABLED            0x01

/* enum declarations */

/*
 * An enumeration of eQMI_WDS_SET_MIP_PARAMS request TLV IDs
 */
enum eWDS_SET_MIP_PARAMS_REQ
{
    eTLV_MIP_SPC                = 0x01,
    eTLV_MIP_MODE               = 0x10,
    eTLV_MIP_REG_RETRY_COUNT    = 0x11,
    eTLV_MIP_REG_RETRY_INTERVAL = 0x12,
    eTLV_MIP_RE_REG_PERIOD      = 0x13,
    eTLV_MIP_RE_REG_IF_TRAF     = 0x14,
    eTLV_MIP_QC_HANDSOFF        = 0x15,
    eTLV_MIP_RFC2002BIS         = 0x16
};

/*
 * This structure contains the SetMIPParams request parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */

struct QmiWdsSetMipParamsReq
{
    CHAR  *pSPC;
    ULONG *pMode;
    BYTE  *pRetryLimit;
    BYTE  *pRetryInterval;
    BYTE  *pReRegPeriod;
    BYTE  *pReRegTraffic;
    BYTE  *pHAAuthenticator;
    BYTE  *pHA2002bis;
};

/*
 * This structure contains the SetMobileIpParameters response parameters.
 *
 */
struct QmiWdsSetMipParamsResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiWdsSetMipParams(
    WORD  *pMlength,
    BYTE  *pBuffer,
    CHAR  *pSPC,
    ULONG *pMode,
    BYTE  *pRetryLimit,
    BYTE  *pRetryInterval,
    BYTE  *pReRegPeriod,
    BYTE  *pReRegTraffic,
    BYTE  *pHAAuthenticator,
    BYTE  *pHA2002bis );

extern enum eQCWWANError UpkQmiWdsSetMipParams(
    BYTE   *pMdmResp,
    struct QmiWdsSetMipParamsResp *pApiResp );

#endif /* _WDS_SET_MIP_PARAMS_MSG_H__ */
