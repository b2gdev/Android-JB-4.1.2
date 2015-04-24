/*
 * \ingroup wds
 *
 * \file qaWdsGetAutoconnectSetting.h
 *
 * \brief This file contains definitions, enumerations, structures
 *        and forward declarations for qaWdsStartNetworkInterface.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __WDS_GET_AUTOCONNECT_SETTING_MSG_H__
#define __WDS_GET_AUTOCONNECT_SETTING_MSG_H__

/* enum declarations */

/*
 * An enumeration of eQMI_WDS_GET_AUTOCONNECT response TLV IDs
 */
enum eWDS_GET_AUTOCONNECT_SETTING_RESP
{
    eTLV_SETTING = 0x01
};

/*
 * This structure contains the GetAutoconnect response parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsGetAutoconnectSettingResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* GetAutoconnect response parameters */
    ULONG *pSetting;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiWdsGetAutoconnectSetting(
    WORD *pMlength,
    BYTE *pBuffer );

extern enum eQCWWANError UpkQmiWdsGetAutoconnectSetting(
    BYTE   *pMdmResp,
    struct QmiWdsGetAutoconnectSettingResp *pAipResp );

#endif /* _WDS_GET_AUTOCONNECT_SETTING_MSG_H__ */
