/*
 * \ingroup wds
 *
 * \file    qaWdsModifyMipProfile.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaWdsModifyMipProfile.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __WDS_MODIFY_MIP_PROFILE_MSG_H__
#define __WDS_MODIFY_MIP_PROFILE_MSG_H__


/* enum declarations */

/*
 * An enumeration of eWDS_SET_MIP_PROFILE_REQ request TLV IDs
 */
enum eWDS_SET_MIP_PROFILE_REQ
{
    eTLV_MIP_ID               = 0x01,
    eTLV_MIP_STATE            = 0x10,
    eTLV_MIP_HOME_ADDRESS     = 0x11,
    eTLV_MIP_HOME_AGENT_PRI   = 0x12,
    eTLV_MIP_HOME_AGENT_SEC   = 0x13,
    eTLV_MIP_REV_TUN_PREF     = 0x14,
    eTLV_MIP_NAI              = 0x15,
    eTLV_MIP_MN_HA_SPI        = 0x16,
    eTLV_MIP_MN_AAA_SPI       = 0x17,
    eTLV_MIP_MN_HA_KEY_STATE  = 0x18,
    eTLV_MIP_MN_AAA_KEY_STATE = 0x19
};

/*
 * This structure contains the SetMobileIPProfile request parameters.
 */
struct QmiWdsModifyMipProfileReq
{
    CHAR  *pSPC;
    BYTE  index;
    BYTE  *pEnabled;
    ULONG *pAddress;
    ULONG *pPrimaryHA;
    ULONG *pSecondaryHA;
    BYTE  *pRevTunneling;
    CHAR  *pNAI;
    ULONG *pHASPI;
    ULONG *pAAASPI;
    CHAR  *pMNHA;
    CHAR  *pMNAAA;
};

/*
 * This structure contains the SetMobileIPProfile response parameters.
 */
struct QmiWdsModifyMipProfileResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiWdsModifyMipProfile(
    WORD  *pMlength,
    BYTE  *pBuffer,
    CHAR  *pSPC,
    BYTE  index,
    BYTE  *pEnabled,
    ULONG *pAddress,
    ULONG *pPrimaryHA,
    ULONG *pSecondaryHA,
    BYTE  *pRevTunneling,
    CHAR  *pNAI,
    ULONG *pHASPI,
    ULONG *pAAASPI,
    CHAR  *pMNHA,
    CHAR  *pMNAAA );

extern enum eQCWWANError UpkQmiWdsModifyMipProfile(
    BYTE   *pMdmResp,
    struct QmiWdsModifyMipProfileResp *pApiResp );

#endif /*__WDS_MODIFY_MIP_PROFILE_MSG_H__*/
