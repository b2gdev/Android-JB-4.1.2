/*
 * \ingroup wds
 *
 * \file qaWdsGetLastMipStatus.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *       QMI_WDS_GET_LAST_MIP_STATUS message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"

#include "qaWdsGetLastMipStatus.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the GetLastMobileIPError parameters
 * to the QMI message SDU
 *
 * \param     pParamField [OUT] - Pointer to storage into which the packed
 *                                    data will be placed by this function.
 *
 * \param     pMlength    [OUT] - Total length of built message.
 *
 * \return    eQCWWAN_ERR_NONE, success
 * \return    eQCWWAN_ERR_MEMORY,accessed beyond allowed size attempted
 *
 */
enum eQCWWANError PkQmiWdsGetLastMipStatus(
    WORD *pMlength,
    BYTE *pParamField )
{
    static struct qmTlvBuilderItem map[] =
    {
    	/* No mandatory TLV is present hence map is empty*/
        { eTLV_TYPE_INVALID, NULL }/* Important. Sentinel.
                                    * Signifies last item in map.
                                    */
    };

    enum eQCWWANError eRCode;

    eRCode = qmbuild( pParamField,
                      NULL,
                      map,
                      eQMI_WDS_GET_LAST_MIP_STATUS,
                      pMlength );
    return eRCode;
}

/*
 * This function unpacks the  pError GetLastMobileIPError  from the
 * QMI response message to a user provided response structure.
 *
 * \param pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]     - Pointer to structure containing storage
 *                             to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UnpackTlvMipError(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsGetLastMipStatusResp *pLresp =
                       (struct QmiWdsGetLastMipStatusResp*)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE lTemp;

    if ( pLresp->pError )
    {
        /* Fill in the Error parameter value */
        eRCode = GetByte( pTlvData, &lTemp );
        if( eRCode == eQCWWAN_ERR_NONE )
            *(pLresp->pError) = lTemp;
    }
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the response message to GetLastMobileIPError to a
 * user-provided response structure.
 *
 * \param  pMdmResp - Pointer to packed response from the modem.
 *
 * \param  pApiResp - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UpkQmiWdsGetLastMipStatus(
    BYTE   *pMdmResp,
    struct QmiWdsGetLastMipStatusResp *pApiResp )
{
	enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_MIP_ERROR,    &UnpackTlvMipError },
        { eTLV_TYPE_INVALID, NULL }/* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };
    eRCode = qmunpackresp( pMdmResp,
                          (BYTE *)pApiResp,
                          map,
                          eQMI_WDS_GET_LAST_MIP_STATUS );
    return eRCode;
}
