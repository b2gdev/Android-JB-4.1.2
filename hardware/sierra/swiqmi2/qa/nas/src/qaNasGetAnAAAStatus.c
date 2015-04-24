/*
 * \ingroup nas
 *
 * \file qaNasGetAnAAAStatus.c
 *
 * \brief  Contains Packing and UnPacking routines for the
 *         QMI_NAS_GET_AN_AAA_STATUS message.
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

#include "qaNasGetAnAAAStatus.h"

/*
 * This function packs the GetANAAAAuthenticationStatus parameters
 * to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE, on success
 * \return eQCWWAN_ERR_NO_MEMORY, accessed beyond allowed size attempted
 *
 */
enum eQCWWANError PkQmiNasGetAnAAAStatus(
    WORD *pMlength,
    BYTE *pParamField )
{
    static struct qmTlvBuilderItem map[] =
    {
        /* There is no mandatory TLV so map is empty  */
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };
    enum eQCWWANError eRCode;

    eRCode = qmbuild( pParamField,
                      NULL,
                      map,
                      eQMI_NAS_GET_AAA_AUTH_STATUS,
                      pMlength );
    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the GetANAAAAuthenticationStatus pStatus from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param pResp    [OUT]     - Pointer to structure containing storage
 *                               to return data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE
 */
enum eQCWWANError UnpackTlvANAAAStatus( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetAnAAAStatusResp *lResp =
                       (struct QmiNasGetAnAAAStatusResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE lTemp;

    if ( lResp->pStatus == NULL )
        return eRCode;

    /* pStatus is a ULONG * but its size as per the document is 1 byte */
    eRCode            = GetByte( pTlvData, &lTemp );
    *(lResp->pStatus) = lTemp;

    return eRCode;
}


/*****************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the GetANAAAAuthenticationStatus response message to a
 * user-provided response structure.
 *
 * \param     MdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp  [OUT] - Pointer to storage to unpack into.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 * \return    eQCWWAN_ERR_NO_MEMORY, accessed beyond allowed size attempted
 */
enum eQCWWANError UpkQmiNasGetAnAAAStatus(
    BYTE                            *pMdmResp,
    struct QmiNasGetAnAAAStatusResp *pApiResp)
{
	enum eQCWWANError rCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,   &qmUnpackTlvResultCode },
        { eTLV_AN_AAA_STATUS, &UnpackTlvANAAAStatus},
        { eTLV_TYPE_INVALID,  NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };
    rCode = qmunpackresp( pMdmResp,
                          (BYTE *)pApiResp,
                          map,
                          eQMI_NAS_GET_AAA_AUTH_STATUS );
    return rCode;
}
