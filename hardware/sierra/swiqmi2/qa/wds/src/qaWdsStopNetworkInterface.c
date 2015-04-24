/*
 * \ingroup wds
 *
 * \file qaWdsStopNetworkInterface.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        QMI_WDS_STOP_NET message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"

#include "qaWdsStopNetworkInterface.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the StopDataSession PktDataHandle to the
 * QMI message SDU
 *
 * \param     bufp   - Pointer to storage into which the packed
 *                     data will be placed by this function
 *
 * \param     paramp - Pointer to structure containing data for this TLV
 *
 * \return    eQCWWAN_ERR_NONE, on success
 * \return    eQCWWAN_ERR_MEMORY, on error
 *
 */
enum eQCWWANError BuildTlvPktDataHandle(BYTE *bufp, BYTE *paramp)
{
    struct QmiWdsStopNetworkInterfaceReq *reqp =
                      (struct QmiWdsStopNetworkInterfaceReq *)paramp;

    if (!reqp->pktDataHandle)
        return eQCWWAN_ERR_NONE;

    /* Add TLV data. */
    return PutLong(bufp, reqp->pktDataHandle);
}

/*
 * This function packs the StopDataSession disableAutoconnect to the
 * QMI message SDU
 *
 * \param     bufp   - Pointer to storage into which the packed
 *                     data will be placed by this function
 *
 * \param     paramp - Pointer to structure containing data for this TLV
 *
 * \return    eQCWWAN_ERR_NONE, on success
 * \return    eQCWWAN_ERR_MEMORY, on error
 *
 */
enum eQCWWANError BuildTlvDisableAutoconnect(BYTE *bufp, BYTE *paramp)
{
    struct QmiWdsStopNetworkInterfaceReq *reqp =
                      (struct QmiWdsStopNetworkInterfaceReq *)paramp;

    if (reqp->disableAutoconnect != 1)
        return eQCWWAN_ERR_NONE;

    /* Add TLV data. */
    return PutLong(bufp, reqp->disableAutoconnect);
}

/*
 * This function packs the StopDataSession parameters to the
 * QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE, on success
 * \return eQCWWAN_ERR_MEMORY, on error
 *
 * \sa     qaGobiApiWds.h for remaining parameter descriptions.
 *
 */
enum eQCWWANError PkQmiWdsStopNetworkInterface(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG pktDataHandle,
    BYTE  disableAutoconnect )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_PKT_DATA_HANDLE,     &BuildTlvPktDataHandle },
        { eTLV_DISABLE_AUTOCONNECT, &BuildTlvDisableAutoconnect },
        { eTLV_TYPE_INVALID,        NULL }  /* Important. Sentinel.
                                             * Signifies last item in map.
                                             */
    };

    struct QmiWdsStopNetworkInterfaceReq req;

    enum eQCWWANError eRCode;

    slmemset((char *)&req, 0, sizeof(struct QmiWdsStopNetworkInterfaceReq));
    req.pktDataHandle       = pktDataHandle;
    req.disableAutoconnect  = disableAutoconnect;

    eRCode = qmbuild( pParamField, (BYTE *)&req, map,
                      eQMI_WDS_STOP_NET, pMlength);

    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the StopDataSession response message
 * to a user provided response structure
 *
 * \param  pMdmResp - Pointer to packed response from the modem
 *
 * \param  pApiResp - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UpkQmiWdsStopNetworkInterface(
    BYTE   *pMdmResp,
    struct QmiWdsStopNetworkInterfaceResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    eRCode = qmunpackresp( pMdmResp, (BYTE *)pApiResp, map,
                          eQMI_WDS_STOP_NET);

    return eRCode;
}
