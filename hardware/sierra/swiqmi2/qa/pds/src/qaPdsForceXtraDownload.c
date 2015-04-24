/*
 * \ingroup pds
 *
 * \file  qaPDSForceXtraDownload.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        eQMI_PDS_FORCE_XTRA_DOWNLOAD message.
 *
 * Copyright: © 2010 Sierra Wireless, Inc. all rights reserved
 */

/* include files */


#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaPdsForceXtraDownload.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * Packs the ForceXTRADownload parameters to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError PkQmiPdsForceXtraDownload
(
    WORD  *pMlength,
    BYTE  *pParamField )

{
    /* There is no mandatory TLV and hence the map is empty */
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };

    enum eQCWWANError eRCode;

    eRCode = qmbuild( pParamField,
                      NULL,
                      map,
                      eQMI_PDS_FORCE_XTRA_DL,
                      pMlength );

    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * Unpacks the ForceXTRADownload response message to a user-provided response
 *
 * \param  pMdmResp[IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp[OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError UpkQmiPdsForceXtraDownload(
    BYTE                               *pMdmResp,
    struct QmiPdsForceXtraDownloadResp *pApiResp )
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
                           eQMI_PDS_FORCE_XTRA_DL );
    return eRCode;
}
