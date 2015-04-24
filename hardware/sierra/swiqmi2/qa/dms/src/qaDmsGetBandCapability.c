/*
 * \ingroup dms
 *
 * \file    qaDmsGetBandCapability.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_DMS_GET_BAND_CAPABILITY message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaDmsGetBandCapability.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the BandCapability parametersto the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError PkQmiDmsGetBandCapability( WORD *pMlength, BYTE *pParamField )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    return qmbuild( pParamField,
                      NULL,
                      map,
                      eQMI_DMS_GET_BAND_CAPS,
                      pMlength );
}


/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * Unpacks the SLQSGetBandCapability BandCapability from the QMI response
 * message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]    - Pointer to structure containing storage
 *                             to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on failure
 */
enum eQCWWANError qmUnpackTlvBandCapability( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiDmsGetBandCapabilityResp *lResp =
        (struct QmiDmsGetBandCapabilityResp *)pResp;

    if ( lResp->pBandCapability == NULL )
        return eQCWWAN_ERR_NONE;

    return GetLongLong( pTlvData, lResp->pBandCapability );
}

/*
 * This function unpacks the ValidateSPC response message to a
 * user-provided response structure.
 *
 * \param   pMdmResp - Pointer to packed response from the modem.
 *
 * \param   pApiResp - Pointer to the structure where TLVs are unpacked
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError UpkQmiDmsGetBandCapability(
    BYTE                               *pMdmResp,
    struct QmiDmsGetBandCapabilityResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,     &qmUnpackTlvResultCode },
        { eTLV_BAND_CAPABILITY, &qmUnpackTlvBandCapability },
        { eTLV_TYPE_INVALID,    NULL } /* Important. Sentinel.
                                        * Signifies last item in map.
                                        */
    };
    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_DMS_GET_BAND_CAPS );
    return eRCode;
}
