/*
 * \ingroup wds
 *
 * \file qaWdsSLQSSetIPFamilyPreference.h
 *
 * \brief Header file for QMI_WDS_SET_CLIENT_IP_FAMILY_PREF interface
 *
 * Copyright: © 2010 Sierra Wireless, Inc. all rights reserved
 *
 */
#ifndef _WDS_SET_CLIENT_IP_FAMILY_PREF_MSG_H_
#define _WDS_SET_CLIENT_IP_FAMILY_PREF_MSG_H_

/*
 * This enumeration contains the TLV IDs for
 * QMI_WDS_SET_CLIENT_IP_FAMILY_PREF requests.
 */
enum eWDS_SET_CLIENT_IP_FAMILY_PREF
{
    eTLV_SET_CLIENT_IP_FAMILY_PREF = 0x01
};

/*
 * This structure contains the SLQSSetIPFamilyPreference request parameters
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsSLQSSetIPFamilyPreferenceReq
{
    BYTE IPFamilyPreference;
};

/*
 * This structure contains the SLQSSetIPFamilyPreference response parameters
 *
 * \sa qaGobiApiWds.h for parameter descriptions
 *
 */
struct QmiWdsSLQSSetIPFamilyPreferenceResp
{
    /* Every response message must have a results structure */
    struct qmTlvResult results;
};

/* Prototypes */
extern enum eQCWWANError PkQmiWdsSLQSSetIPFamilyPreference(
    WORD *pMlength,
    BYTE *pParamField,
    BYTE IPFamilyPreference );

extern enum eQCWWANError UpkQmiWdsSLQSSetIPFamilyPreference(
    BYTE                                       *pMdmResp,
    struct QmiWdsSLQSSetIPFamilyPreferenceResp *pApiResp );

#endif /* _WDS_SET_CLIENT_IP_FAMILY_PREF_MSG_H_ */

