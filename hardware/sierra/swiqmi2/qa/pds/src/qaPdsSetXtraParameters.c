/*
 * \ingroup pds
 *
 * \file    qaPdsSetXtraParameters.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          eQMI_PDS_SET_XTRA_PARAMETERS message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaPdsSetXtraParameters.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * Packs the SetXtraParameters enabled and interval fields to the
 * QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvXtraDatabaseAutoDwnld( BYTE *pBuf, BYTE *pParam )
{
    struct QmiPdsSetXtraParametersReq *pReq =
        (struct QmiPdsSetXtraParametersReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE              lbyte;
    WORD              lword;
    if ( !pReq->pbEnabled ||
         !pReq->pinterval )
        return eRCode;

    /* Add TLV data */
    /* Auto Download State */
    lbyte = *(pReq->pbEnabled);
    eRCode = PutByte( pBuf, lbyte );

    /* Interval between Xtra Database Download */
    if ( eQCWWAN_ERR_NONE == eRCode )
    {
        lword = *(pReq->pinterval);
        eRCode = PutWord( pBuf, lword );
    }
    return eRCode;
}

/*
 * Packs the SetXtraParameters preference fields to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvXtraDatabaseNetworkConfig( BYTE *pBuf, BYTE *pParam )
{
    struct QmiPdsSetXtraParametersReq *pReq =
        (struct QmiPdsSetXtraParametersReq *)pParam;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE              lbyte;

    /* Add TLV data */
    if ( !pReq->ppreference )
        return eRCode;

    /* XTRA WWAN network Preference */
    lbyte = *(pReq->ppreference);
    return PutByte( pBuf, lbyte );
}

/*
 * Packs the SetXtraParameters parameters to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                                  data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError PkQmiPdsSetXtraParameters(
    WORD   *pMlength,
    BYTE   *pParamField,
    ULONG  *pbEnabled,
    USHORT *pinterval,
    ULONG  *ppreference )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SET_XTRA_DB_AUTODWNLD,      &BuildTlvXtraDatabaseAutoDwnld },
        { eTLV_SET_XTRA_DB_NETWORK_CONFIG, &BuildTlvXtraDatabaseNetworkConfig },
        { eTLV_TYPE_INVALID,               NULL } /* Important. Sentinel.
                                                   * Signifies last item in map
                                                   */
    };

    struct QmiPdsSetXtraParametersReq req;

    enum eQCWWANError eRCode;

    slmemset((CHAR *)&req, 0,\
             sizeof(struct QmiPdsSetXtraParametersReq));
    req.pbEnabled   = pbEnabled;
    req.pinterval   = pinterval;
    req.ppreference = ppreference;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_PDS_SET_XTRA_PARAMS,
                      pMlength );
    return eRCode;
}


/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * Unpacks the SetXtraParameters response message to a user-provided
 * response structure.
 *
 * \param  pMdmResp - Pointer to packed response from the modem.
 *
 * \param  pApiResp - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiPdsSetXtraParameters(
    BYTE                               *pMdmResp,
    struct QmiPdsSetXtraParametersResp *pApiResp )
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
                           eQMI_PDS_SET_XTRA_PARAMS );
    return eRCode;
}
