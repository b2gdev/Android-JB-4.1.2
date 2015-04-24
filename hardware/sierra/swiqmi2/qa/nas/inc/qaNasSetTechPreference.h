/*
 * \ingroup nas
 *
 * \file  qaNasSetTechPref.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaNasSetTechPref.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_SET_TECH_PREF_MSG_H__
#define __NAS_SET_TECH_PREF_MSG_H__

/* enum declarations */
/*
 *
 * An enumeration of eQMI_NAS_SET_TECH_PREF request TLV IDs
 *
 */
enum eNAS_SET_TECH_PREF_REQ
{
    eTLV_TECH_PREF_DUR = 0x01,

};

/*
 * This structure contains the Set Technology Preference request parameters.
 *
 * \sa  qaGobiApiNas.h for parameter descriptions
 *
 */
struct QmiNasSetTechPrefReq
{
    WORD technologyPref;
    BYTE duration;
};

/*
 * This structure contains the SetNetworkPreference response parameters
 *
 */
struct QmiNasSetTechPrefResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/*
 * Prototypes
 *
 */
extern enum eQCWWANError PkQmiNasSetTechPref(
    WORD  *pMlength,
    BYTE  *pBuffer,
    ULONG technologyPref,
    ULONG duration );

extern enum eQCWWANError UpkQmiNasSetTechPref(
    BYTE                         *pMdmresp,
    struct QmiNasSetTechPrefResp *pApiresp);

#endif /* __NAS_SET_TECH_PREF_MSG_H__ */
