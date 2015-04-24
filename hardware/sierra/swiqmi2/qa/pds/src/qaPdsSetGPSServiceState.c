/*
 * \ingroup pds
 *
 * \file qaPdsSetGPSServiceState.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        eQMI_PDS_SET_SERVICE_STATE message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaPdsSetGPSServiceState.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the SetPDSState Enable field to the
 * QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvServiceState( BYTE *pBuf, BYTE *pParam )
{
    struct QmiPdsSetServiceStateReq *pReq =
        (struct QmiPdsSetServiceStateReq *)pParam;

    /* Add TLV data */
    return PutByte(pBuf, pReq->ServiceState);
}

/*
 * This function packs the SetServiceState parameters
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
enum eQCWWANError PkQmiPdsSetServiceState(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG ServiceState )
{
    /* There is no mandatory TLV and hence the map is empty */
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SET_GPS_SERVICE_STATE, &BuildTlvServiceState },
        { eTLV_TYPE_INVALID,          NULL    } /* Important. Sentinel.
                                                 * Signifies last item in map
                                                 */
    };

    struct QmiPdsSetServiceStateReq req;

    enum eQCWWANError eRCode;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiPdsSetServiceStateReq));
    req.ServiceState = ServiceState;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_PDS_SET_STATE,
                      pMlength );
    return eRCode;
}


/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the SetPDSState response message to a
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
enum eQCWWANError UpkQmiPdsSetServiceState(
    BYTE                             *pMdmResp,
    struct QmiPdsSetServiceStateResp *pApiResp )
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
                           eQMI_PDS_SET_STATE );
    return eRCode;
}
