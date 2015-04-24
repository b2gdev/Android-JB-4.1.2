/*
 * \ingroup sms
 *
 * \file    qaSmsSLQSGetTransNWRegInfo.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_WMS_GET_TRANS_NW_REG_INFO message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaSmsSLQSGetTransNWRegInfo.h"

/*****************************************************************************
 * Request handling
 *****************************************************************************/

/*
 * This function packs the SLQS Get Transport NW Reg Info request parameters to
 * the QMI message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError PkQmiSmsSLQSGetTransNWRegInfo(
    WORD *pMlength,
    BYTE *pParamField )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map
                                     */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      NULL,
                      map,
                      eQMI_WMS_GET_TRANS_NW_REG_INFO,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the transport network registration status from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]    - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvNWRegStat(
    BYTE *pTlvData,
    BYTE *pResp )
{
    getTransNWRegInfoResp *lResp =
        ((struct QmiSmsSLQSGetTransNWRegInfoResp *)pResp)->pTransNWRegInfo;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pRegStatus )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pRegStatus );
}

/*
 * This function unpacks the SLQS Get Transport NW Reg Info response to
 * a user-provided response structure.
 *
 * \param  pMdmResp - Pointer to packed response from the modem.
 *
 * \param  pApiResp - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiSmsSLQSGetTransNWRegInfo(
    BYTE                                   *pMdmResp,
    struct QmiSmsSLQSGetTransNWRegInfoResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,       &qmUnpackTlvResultCode },
        { eTLV_TRANS_NW_REG_STAT, &UnpackTlvNWRegStat },
        { eTLV_TYPE_INVALID,      NULL } /* Important. Sentinel.
                                          * Signifies last item in map.
                                          */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_WMS_GET_TRANS_NW_REG_INFO );
    return eRCode;
}
