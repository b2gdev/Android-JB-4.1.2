/*
 * \ingroup nas
 *
 * \file qaNasSLQSGetHDRColorCode.c
 *
 * \brief  Contains Packing and UnPacking routines for the
 *         eQMI_NAS_GET_HDR_COLOR_CODE message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaNasSLQSGetHDRColorCode.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the GetHDRColorCode parameters to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 */
enum eQCWWANError PkQmiNasSlqsGetHDRColorCode (
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
                      eQMI_NAS_GET_HDR_COLOR_CODE,
                      pMlength );
    return eRCode;
}


/*****************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the Color Code from the QMI response message to a user
 * provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 */
enum eQCWWANError UnpackTlvColorCode( BYTE *pTlvData, BYTE *pResp )
{
    nasGetHDRColorCodeResp *lResp = ((struct QmiNasSlqsGetHDRColorCodeResp *)
                                                pResp)->pGetHDRColorCodeResp;

    if ( lResp->pColorCode == NULL )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Fill in the Color code parameter value */
    return GetByte( pTlvData, lResp->pColorCode );
}

/*
 * This function unpacks the GetHDRColorCode response message to to a
 * user-provided response structure.
 *
 * \param     MdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp  [OUT] - Pointer to storage to unpack into.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 * \return    eQCWWAN_ERR_NO_MEMORY, on error
 */
enum eQCWWANError UpkQmiNasSlqsGetHDRColorCode(
    BYTE                                 *pMdmResp,
    struct QmiNasSlqsGetHDRColorCodeResp *pApiResp)
{
    enum eQCWWANError rCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_COLOR_CODE,   &UnpackTlvColorCode },
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };
    rCode = qmunpackresp( pMdmResp,
                          (BYTE *)pApiResp,
                          map,
                          eQMI_NAS_GET_HDR_COLOR_CODE );
    return rCode;
}
