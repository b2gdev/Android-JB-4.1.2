/*
 * \ingroup : nas
 *
 * \file    : qaNasSetACCOLC.c
 *
 * \brief   : Contains Packing and UnPacking routines for the
 *            eQMI_NAS_SET_ACCOLC message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */
/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaQmiBasic.h"

#include "qaNasSetACCOLC.h"


/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the qaNasSetACCOLC SPC, ACCOLC field
 * to the QMI message SDU
 *
 * \param  pBuf   - Pointer to storage into which the packed
 *                  data will be placed by this function.
 *
 * \param  pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvACCOLC( BYTE *pBuf, BYTE *pParam )
{
    struct QmiNasSetACCOLCReq *pReq =
                     (struct QmiNasSetACCOLCReq *)pParam;
    BYTE              count;
    enum eQCWWANError eRCode;
    /* Add TLV data */
    if ( pReq->spc == NULL )
        return eQCWWAN_ERR_NONE;

    /* Insert the Service Programming Code */
    for ( count = 0; count < 6; count++ )
    {
        eRCode = PutByte( pBuf, *(pReq->spc)++ );
        if( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;
    }

    /* Insert the Access Overload Class */
    return PutByte( pBuf, pReq->accolc );
}

/*
 * This function packs the SetACCOLC parameters to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                                    data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \param  spc[IN]
 *          - service programming code NULL-terminated string of six digit
 *
 * \param  ACCOLC
 *          - ACCOLC : Valid range is 0 to 15
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError PkQmiNasSetACCOLC(
    WORD *pMlength,
    BYTE *pParamField,
    CHAR *spc,
    BYTE accolc)
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SET_ACCOLC,       &BuildTlvACCOLC },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };
    struct QmiNasSetACCOLCReq req;
    enum eQCWWANError         eRCode;

    /* Initialize the structure and assign values */
    slmemset((char *)&req, 0, sizeof(struct QmiNasSetACCOLCReq));
    req.spc    = spc;
    req.accolc = accolc;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_NAS_SET_ACCOLC,
                      pMlength );

    return eRCode;
}

/******************************************************************************
 * Response handling
 *****************************************************************************/

/*
 * This function unpacks the eQMI_NAS_SET_ACCOLC response
 * message to a user-provided response structure.
 *
 * \param  pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp  [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiNasSetACCOLC(
    BYTE                       *pMdmResp,
    struct QmiNasSetACCOLCResp *pApiResp)
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
                           eQMI_NAS_SET_ACCOLC );
    return eRCode;
}

