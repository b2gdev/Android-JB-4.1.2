/*
 * \ingroup wds
 *
 * \file qaWdsSetMipMode.c
 *
 * \brief This file contains Packing and UnPacking routines for the
 *        QMI_WDS_SET_MIP_MODE message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"

#include "qaWdsSetMipMode.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the SetMobileIP Mode to the
 * QMI message SDU
 *
 * \param bufp   - Pointer to storage into which the packed
 *                 data will be placed by this function
 *
 * \param pParam - Pointer to structure containing data for this TLV
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlvMipMode (BYTE *bufp, BYTE *pParam)
{
    struct QmiWdsSetMipModeReq *pReq =
                    (struct QmiWdsSetMipModeReq *)pParam;
    enum eQCWWANError eRCode;
    /* Add TLV data. */
    eRCode = PutByte( bufp, pReq->mode );

    return eRCode;
}

/*
 * This function packs the SetMobileIP parameters to the
 * QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE   - Increment Successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed limit attempted
 *
 * \sa     qaGobiApiWds.h for remaining parameter descriptions.
 *
 */
enum eQCWWANError PkQmiWdsSetMipMode(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG mode )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SET_MIP_MODE, &BuildTlvMipMode },
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    struct QmiWdsSetMipModeReq req;

    enum eQCWWANError eRCode;

    slmemset((char *)&req, 0, sizeof(struct QmiWdsSetMipModeReq));
    req.mode = mode;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_WDS_SET_MIP,
                      pMlength );
    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the SetMobileIP response message to a user
 * provided response structure
 *
 * \param  pMdmResp - Pointer to packed response from the modem
 *
 * \param  pApiResp - Pointer to storage to unpack into.
 *
 * \return    eQCWWAN_ERR_NONE   - Increment Successful
 * \return    eQCWWAN_ERR_MEMORY - Access beyond allowed limit attempted
 *
 */
enum eQCWWANError UpkQmiWdsSetMipMode(
    BYTE                        *pMdmResp,
    struct QmiWdsSetMipModeResp *pApiResp )
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
                          eQMI_WDS_SET_MIP );

    return eRCode;
}
