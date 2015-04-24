/*
 * \ingroup : nas
 *
 * \file    : qaNasSetTechPreference.c
 *
 * \brief   : Contains Packing and UnPacking routines for the
 *            eQMI_NAS_SET_TECH_PREF message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaQmiBasic.h"

#include "qaNasSetTechPreference.h"


/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the SetNetworkPreference technologyPref field
 * to the QMI message SDU
 *
 * \param pBuf   - Pointer to storage into which the packed
 *                data will be placed by this function.
 *
 * \param pParam - Pointer to structure containing data for this TLV.
 *
 */
enum eQCWWANError BuildTlvTechPrefDur( BYTE *pBuf, BYTE *pParam )
{
    struct QmiNasSetTechPrefReq *pReq =
                     (struct QmiNasSetTechPrefReq *)pParam;
    enum eQCWWANError eRCode;
    /* Add TLV data */
    eRCode = PutWord( pBuf, pReq->technologyPref );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;
    return PutByte( pBuf, pReq->duration );
}

/*
 * This function packs the SetNetworkPreference parameters to the QMI message
 * SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                                    data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE      - Increment successful
 * \return eQCWWAN_ERR_NO_MEMORY - Access beyond allowed size attempted
 *
 * \sa     qaGobiApiNas.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiNasSetTechPref(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG technologyPref,
    ULONG duration )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TECH_PREF_DUR, &BuildTlvTechPrefDur },
        { eTLV_TYPE_INVALID,  NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };

    struct QmiNasSetTechPrefReq req;
    enum eQCWWANError           eRCode;

    /* Initialise the structure and populate the values */
    slmemset((CHAR *)&req, 0, sizeof(struct QmiNasSetTechPrefReq));
    req.technologyPref = technologyPref;
    req.duration       = duration;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_NAS_SET_TECH_PREF,
                      pMlength );

    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************

 * This function unpacks the eQMI_NAS_SET_TECH_PREF response
 * message to a user-provided response structure.
 *
 * \param  pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp  [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE      - Increment successful
 * \return eQCWWAN_ERR_NO_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError UpkQmiNasSetTechPref(
    BYTE                         *pMdmResp,
    struct QmiNasSetTechPrefResp *pApiResp)
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
                           eQMI_NAS_SET_TECH_PREF);
    return eRCode;
}

