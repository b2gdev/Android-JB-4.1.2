/*
 * \ingroup dms
 *
 * \file qaDmsUIMGetICCID.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        QMI_DMS_UIM_GET_ICCID message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaQmiBasic.h"

#include "qaDmsUIMGetICCID.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the UIMGetICCID parameters to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                                    data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError PkQmiDmsUIMGetICCID(
    WORD *pMlength,
    BYTE *pParamField )
{
    /* There is no mandatory TLV and hence the map is empty */
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    enum eQCWWANError eRCode;

    eRCode = qmbuild( pParamField,
                      NULL,
                      map,
                      eQMI_DMS_UIM_GET_ICCID,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the UIMGetICCID Time from the QMI response
 * message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError UnpackTlvUIMICCID( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiDmsUIMGetICCIDResp *lResp =
        (struct QmiDmsUIMGetICCIDResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( (lResp->stringSize == 0) || !lResp->pString )
        return eRCode;

    /* Extract the String Parameter */
    eRCode = qmQmiExtractString( pTlvData,
                                 lResp->pString,
                                 lResp->stringSize );
    return eRCode;
}

/*
 * This function unpacks the UIMGetICCID response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError UpkQmiDmsUIMGetICCID(
    BYTE                         *pMdmResp,
    struct QmiDmsUIMGetICCIDResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_UIM_ICCID,    &UnpackTlvUIMICCID },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_DMS_UIM_GET_ICCID );
    return eRCode;
}

