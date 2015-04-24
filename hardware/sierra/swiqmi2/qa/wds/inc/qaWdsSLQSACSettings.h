/*
 * \ingroup wds
 *
 * Filename: qaWdsSLQSACSettings.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaWdsSLQSACSettings.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __WDS_AUTOCONNECT_SETTINGS_MSG_H__
#define __WDS_AUTOCONNECT_SETTINGS_MSG_H__

/* enum declarations */
/*
 *
 * An enumeration of eQMI_WDS_AUTOCONNECT_SETTINGS TLV IDs
 *
 */
enum eWDS_AUTOCONNECT_SETTINGS
{
    eTLV_AUTOCONNECT_SETTING      = 0x01,
    eTLV_AUTOCONNECT_ROAM_SETTING = 0x10
};

/*
 * This structure contains the SLQSAutoConnect request parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsSLQSSetAutoConnectReq
{
    struct slqsautoconnect slqsautoconnectReq;
};

/*
 * This structure contains the SLQSSetAutoConnect Response parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsSLQSSetAutoConnectResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * This structure contains the SLQSGetAutoConnect response parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsSLQSGetAutoConnectResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
    struct slqsautoconnect slqsautoconnectResp;
};

/************
 * Prototypes
 **************/
extern enum eQCWWANError PkQmiWdsSLQSSetAutoConnect(
    WORD *pMlength,
    BYTE *pParamField,
    BYTE acsetting,
    BYTE acroamsetting );

extern enum eQCWWANError PkQmiWdsSLQSGetAutoConnect(
    WORD *pMlength,
    BYTE *pParamField );

extern enum eQCWWANError UpkQmiWdsSLQSSetAutoConnect(
    BYTE                                *pMdmResp,
    struct QmiWdsSLQSSetAutoConnectResp *pAipResp);

extern enum eQCWWANError UpkQmiWdsSLQSGetAutoConnect(
    BYTE                                *pMdmResp,
    struct QmiWdsSLQSGetAutoConnectResp *pAipResp);

#endif/* __WDS_AUTOCONNECT_SETTINGS_MSG_H__ */
