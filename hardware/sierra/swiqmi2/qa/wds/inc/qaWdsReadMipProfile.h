/*
 * \ingroup wds
 *
 * \file    qaWdsReadMipProfile.h
 *
 * \brief   This file contains definitions, enumerations, structures and
 *          forward declarations for qaReadMipProfile.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef __WDS_READ_MIP_PROFILE_MSG_H__
#define __WDS_READ_MIP_PROFILE_MSG_H__ 


/* enum declarations */

/*
 * An enumeration of eWDS_GET_MIP_PROFILE_REQ request TLV IDs
 */
enum eWDS_GET_MIP_PROFILE_REQ
{
    eTLV_INDEX          = 0x01,
};

enum eWDS_GET_MIP_PROFILE_RESP
{
    eTLV_GSTATE            = 0x10,
    eTLV_GHOME_ADDRESS     = 0x11,
    eTLV_GHOME_AGENT_PRI   = 0x12,
    eTLV_GHOME_AGENT_SEC   = 0x13,
    eTLV_GREV_TUN_PREF     = 0x14,
    eTLV_GNAI              = 0x15,
    eTLV_GMN_HA_SPI        = 0x16,
    eTLV_GMN_AAA_SPI       = 0x17,
    eTLV_GMN_HA_KEY_STATE  = 0x1A,
    eTLV_GMN_AAA_KEY_STATE = 0x1B
};


/*
 * This structure contains the GetMobileIPProfile request parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */

struct QmiWdsReadMipProfileReq
{
     BYTE  index;
};

/*
 * This structure contains the GetMobileIPProfile response parameters.
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsReadMipProfileResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
    BYTE   *pEnabled;
    ULONG  *pAddress;
    ULONG  *pPrimaryHA;
    ULONG  *pSecondaryHA;
    BYTE   *pRevTunneling;
    BYTE   naiSize;
    CHAR   *pNAI;
    ULONG  *pHASPI;
    ULONG  *pAAASPI;
    ULONG  *pHAState;
    ULONG  *pAAAState;  
};

/*
 * Prototypes
 */
extern enum eQCWWANError PkQmiWdsReadMipProfile(
    WORD  *pMlength,
    BYTE  *pBuffer,
    BYTE  index );

extern enum eQCWWANError UpkQmiWdsReadMipProfile(
    BYTE   *pMdmResp,
    struct QmiWdsReadMipProfileResp *pApiResp );

#endif /*__WDS_READ_MIP_PROFILE_MSG_H__*/
