/*
 * \ingroup nas
 *
 * \file qaNasGetACCOLC.c
 *
 * \brief  Contains Packing and UnPacking routines for the
 *         QMI_NAS_GET_ACCOLC message.
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

#include "qaNasGetACCOLC.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the GetACCOLC parameters
 * to the QMI message SDU
 *
 * \param       pParamField [OUT] - Pointer to storage into which the packed
 *                                    data will be placed by this function.
 *
 * \param       pMlength    [OUT] - Total length of built message.
 *
 * \return      eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError PkQmiNasGetACCOLC(
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
                      eQMI_NAS_GET_ACCOLC,
                      pMlength );
    return eRCode;
}


/*****************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the GetACCOLC from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return      eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvGetACCOLC( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetACCOLCResp *lResp =
        (struct QmiNasGetACCOLCResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( lResp->pACCOLC == NULL )
        return eRCode;

    /* Validate TLV parameter length field */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                           eTLV_GET_ACCOLC_LENGTH );
    if (eRCode != eQCWWAN_ERR_NONE)
        return eRCode;

    eRCode = GetByte( pTlvData, lResp->pACCOLC );

    return eRCode;
}


/*
 * This function unpacks the GetACCOLC response message to a
 * user-provided response structure.
 *
 * \param     pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp  [OUT] - Pointer to storage to unpack into.
 *
 * \return    eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiNasGetACCOLC(
    BYTE                       *pMdmResp,
    struct QmiNasGetACCOLCResp *pApiResp)
{
	enum eQCWWANError rCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_GET_ACCOLC,   &UnpackTlvGetACCOLC },
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };
    rCode = qmunpackresp( pMdmResp,
                          (BYTE *)pApiResp,
                          map,
                          eQMI_NAS_GET_ACCOLC );
    return rCode;
}
