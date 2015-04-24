/*
 * \ingroup nas
 *
 * \file qaNasGetTechnologyPreference.h
 *
 * \brief This file contains definitions, enumerations, structures and
 *        forward declarations for qaNasGetTechnologyPreference.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __NAS_GET_TECHNOLOGY_PREFERENCE_MSG_H__
#define __NAS_GET_TECHNOLOGY_PREFERENCE_MSG_H__

/* enum declarations */
/*
 *
 * An enumeration of eQMI_NAS_GET_TECH_PREF response TLV IDs
 *
 */
enum eQMI_NAS_GET_TECH_PREF_RESP
{
    eTLV_ACTIVE_TECH_PREF     = 0x01,
    eTLV_PERSISTENT_TECH_PREF = 0x10
};

/*
 * This structure contains the Get Technology Preference response parameters.
 *
 * \sa  qaGobiApiNas.h for parameter descriptions
 *
 */
struct QmiNasGetTechPrefResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;

    /* Get Technology Preference response parameters */
    ULONG *pActiveTechPref;
    ULONG *pDuration;
    ULONG *pPersistentTechPref;
};

/*************
 * Prototypes
 **************/
extern enum eQCWWANError PkQmiNasGetTechPref(
    WORD *pMlength,
    BYTE *pParamField );

extern enum eQCWWANError UpkQmiNasGetTechPref(
    BYTE                         *pMdmResp,
    struct QmiNasGetTechPrefResp *pApiResp );

#endif /* __NAS_GET_TECHNOLOGY_PREFERENCE_MSG_H__ */
