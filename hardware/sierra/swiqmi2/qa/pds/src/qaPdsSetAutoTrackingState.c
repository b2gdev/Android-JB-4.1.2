/*
 * \ingroup pds
 *
 * \file    qaPdsSetAutoTrackingState.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          eQMI_PDS_SET_AUTO_TRACKING_STATE message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaPdsSetAutoTrackingState.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the SetServiceAutomaticTracking bAuto field to the
 * QMI message SDU
 *
 * \param  pBuf         [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \param  pParam       [IN]  - Pointer to structure containing data for
 *                              this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvAutoTrackState( BYTE *pBuf, BYTE *pParam )
{
    struct QmiPdsSetAutoTrackingStateReq *pReq =
        (struct QmiPdsSetAutoTrackingStateReq *)pParam;

    /* Add TLV data */
    return PutByte( pBuf, pReq->bAuto );
}

/*
 * This function packs the SetServiceAutomaticTracking parameters
 * to the QMI message SDU
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError PkQmiPdsSetAutoTrackingState(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG bAuto )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_AUTO_TRACK_STATE, &BuildTlvAutoTrackState },
        { eTLV_TYPE_INVALID,     NULL } /* Important. Sentinel.
                                         * Signifies last item in map
                                         */
    };

    struct QmiPdsSetAutoTrackingStateReq req;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiPdsSetAutoTrackingStateReq));
    req.bAuto = bAuto;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_PDS_SET_SVC_AUTOTRACK,
                      pMlength );
    return eRCode;
}


/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the SetServiceAutomaticTracking response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp - Pointer to packed response from the modem.
 *
 * \param  pApiResp - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiPdsSetAutoTrackingState(
    BYTE                                  *pMdmResp,
    struct QmiPdsSetAutoTrackingStateResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

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
                           eQMI_PDS_SET_SVC_AUTOTRACK );
    return eRCode;
}
