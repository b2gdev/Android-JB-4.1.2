/*
 * \ingroup pds
 *
 * \file qaPdsStartTrackingSession.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        eQMI_PDS_START_TRACKING_SESSION message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaQmiBasic.h"

#include "qaPdsStartTrackingSession.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the StartPDSTrackingSession FixInterval field to the
 * QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvTrackingSession( BYTE *pBuf, BYTE *pParam )
{
    struct QmiPdsStartTrackingSessionReq *pReq =
        (struct QmiPdsStartTrackingSessionReq *)pParam;

    /* Add TLV data */
    enum eQCWWANError eRCode;

    /* Session Control */
    eRCode = PutByte(pBuf, pReq->SessionControl);

    /* Session Type */
    if ( eRCode == eQCWWAN_ERR_NONE )
        eRCode = PutByte(pBuf, pReq->SessionType);

    /* Session Operation */
    if ( eRCode == eQCWWAN_ERR_NONE )
        eRCode = PutByte(pBuf, pReq->SessionOperation);

    /* Session Server Option */
    if ( eRCode == eQCWWAN_ERR_NONE )
        eRCode = PutByte(pBuf, pReq->SessionServerOption);

    /* Position Data Timeout */
    if ( eRCode == eQCWWAN_ERR_NONE )
        eRCode = PutByte(pBuf, pReq->PositionDataTimeout);

    /* Position Data Count */
    if ( eRCode == eQCWWAN_ERR_NONE )
        eRCode = PutLong(pBuf, pReq->PositionDataCount);

    /* Position Data Interval */
    if ( eRCode == eQCWWAN_ERR_NONE )
        eRCode = PutLong(pBuf, pReq->PositionDataInterval );

    /* Position Data Accuracy */
    if ( eRCode == eQCWWAN_ERR_NONE )
        eRCode = PutLong(pBuf, pReq->PositionDataAccuracy);
    return eRCode;
}

/*
 * This function packs the StartPDSTrackingSession parameters
 * to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                                  data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 * \sa     qaGobiApiPds.h for remaining parameter descriptions.
 *
 */
enum eQCWWANError PkQmiPdsStartTrackingSession(
    WORD  *pMlength,
    BYTE  *pParamField,
    BYTE  sessionControl,
    BYTE  sessionType,
    BYTE  sessionOperation,
    BYTE  sessionServerOption,
    BYTE  fixTimeout,
    ULONG fixInterval,
    ULONG fixCount,
    ULONG fixAccuracy )
{
    /* There is no mandatory TLV and hence the map is empty */
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TRACKING_SESSION, &BuildTlvTrackingSession },
        { eTLV_TYPE_INVALID,     NULL    } /* Important. Sentinel.
                                            * Signifies last item in map
                                            */
    };

    struct QmiPdsStartTrackingSessionReq req;

    enum eQCWWANError eRCode;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiPdsStartTrackingSessionReq));
    req.SessionControl       = sessionControl;
    req.SessionType          = sessionType;
    req.SessionOperation     = sessionOperation;
    req.SessionServerOption  = sessionServerOption;
    req.PositionDataTimeout  = fixTimeout;
    req.PositionDataInterval = fixInterval;
    req.PositionDataCount    = fixCount;
    req.PositionDataAccuracy = fixAccuracy;
    
    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_PDS_START_SESSION,
                      pMlength );
    return eRCode;
}


/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the StartPDSTrackingSession response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp - Pointer to packed response from the modem.
 *
 * \param  pApiResp - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError UpkQmiPdsStartTrackingSession(
    BYTE                                  *pMdmResp,
    struct QmiPdsStartTrackingSessionResp *pApiResp )
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
                           eQMI_PDS_START_SESSION );
    return eRCode;
}
