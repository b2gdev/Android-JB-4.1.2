/*
 * \ingroup wds
 *
 * \file qaWdsSLQSSetIPFamilyPreference.c
 *
 * \brief This file contains Packing and UnPacking routines for the
 *        QMI_WDS_SET_CLIENT_IP_FAMILY_PREF message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaWdsSLQSSetIPFamilyPreference.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the SLQSSetIPFamilyPreference IP Family Preference to
 * the QMI message SDU
 *
 * \param bufp   - Pointer to storage into which the packed
 *                 data will be placed by this function
 *
 * \param pParam - Pointer to structure containing data for this TLV
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 */
enum eQCWWANError BuildTlvIPFamilyPref( BYTE *bufp, BYTE *pParam )
{
    struct QmiWdsSLQSSetIPFamilyPreferenceReq *pReq =
                    (struct QmiWdsSLQSSetIPFamilyPreferenceReq *)pParam;

    /* Add IPFamilyPreference */
    return PutByte( bufp, pReq->IPFamilyPreference );
}

/*
 * This function packs the SLQSSetIPFamilyPreference parameters to the
 * QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 * \sa     qaGobiApiWds.h for remaining parameter descriptions.
 *
 */
enum eQCWWANError PkQmiWdsSLQSSetIPFamilyPreference(
    WORD *pMlength,
    BYTE *pParamField,
    BYTE IPFamilyPreference )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SET_CLIENT_IP_FAMILY_PREF, &BuildTlvIPFamilyPref },
        { eTLV_TYPE_INVALID,              NULL } /* Important. Sentinel.
                                                  * Signifies last item in map.
                                                  */
    };

    struct QmiWdsSLQSSetIPFamilyPreferenceReq req;
    enum eQCWWANError                         eRCode;

    slmemset( (char *)&req,
              0,
              sizeof(struct QmiWdsSLQSSetIPFamilyPreferenceReq) );

    req.IPFamilyPreference = IPFamilyPreference;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_WDS_SET_IP_FAMILY,
                      pMlength );
    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the PkQmiWdsSLQSSetIPFamilyPreference response message
 * to a user provided response structure
 *
 * \param  pMdmResp - Pointer to packed response from the modem
 *
 * \param  pApiResp - Pointer to storage to unpack into.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 */
extern enum eQCWWANError UpkQmiWdsSLQSSetIPFamilyPreference(
    BYTE                                       *pMdmResp,
    struct QmiWdsSLQSSetIPFamilyPreferenceResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };

    eRCode = qmunpackresp( pMdmResp,
                          (BYTE *)pApiResp,
                          map,
                          eQMI_WDS_SET_IP_FAMILY );

    return eRCode;
}
