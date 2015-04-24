/*
 * \ingroup pds
 *
 * \file    qaPdsSetDefaultTrackingSession.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          eQMI_PDS_SET_DEFAULT_TRACKING_SESSION message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaPdsSetDefaultTrackingSession.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * Packs the SetPDSDefaults operation, timeout, interval and accuracy fields to
 * the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 */
enum eQCWWANError BuildTlvDefaultSessionInfo( BYTE *pBuf, BYTE *pParam )
{
    struct QmiPdsSetDefaultTrackingSessionReq *pReq =
        (struct QmiPdsSetDefaultTrackingSessionReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    /* Add TLV data */

    /* Operating Mode */
    eRCode = PutByte(pBuf, pReq->operation);
    if ( eRCode == eQCWWAN_ERR_NONE )
    {
        /* Max. Time to work on each Fix */
        eRCode = PutByte(pBuf, pReq->timeout);
        if ( eRCode == eQCWWAN_ERR_NONE )
        {
            /* Interval between Fix requests */
            eRCode = PutLong(pBuf, pReq->interval);
            if ( eRCode == eQCWWAN_ERR_NONE )
            {
                /* Desired Accuracy threshold */
                eRCode = PutLong(pBuf, pReq->accuracy);
            }
        }
    }
    return eRCode;
}

/*
 * Packs the SetPDSDefaults parameters to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                                  data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError PkQmiPdsSetDefaultTrackingSession(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG operation,
    BYTE  timeout,
    ULONG interval,
    ULONG accuracy )
{
    /* There is no mandatory TLV and hence the map is empty */
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SET_DEFAULT_SESSION_INFO, &BuildTlvDefaultSessionInfo },
        { eTLV_TYPE_INVALID,             NULL } /* Important. Sentinel.
                                                 * Signifies last item in map
                                                 */
    };

    struct QmiPdsSetDefaultTrackingSessionReq req;

    enum eQCWWANError eRCode;

    slmemset((CHAR *)&req, 0,\
             sizeof(struct QmiPdsSetDefaultTrackingSessionReq));
    req.operation = operation;
    req.timeout   = timeout;
    req.interval  = interval;
    req.accuracy  = accuracy;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_PDS_SET_DEFAULTS,
                      pMlength );
    return eRCode;
}


/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * Unpacks the SetPDSDefaults response message to a user-provided
 * response structure.
 *
 * \param  pMdmResp[IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp[OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError UpkQmiPdsSetDefaultTrackingSession(
    BYTE                                       *pMdmResp,
    struct QmiPdsSetDefaultTrackingSessionResp *pApiResp )
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
                           eQMI_PDS_SET_DEFAULTS );
    return eRCode;
}
