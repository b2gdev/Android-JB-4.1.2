/*
 * \ingroup wds
 *
 * \file qaGetCurrentChannelRate.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        QMI_WDS_GET_RATES message.
 *
 * Copyright : © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"

#include "qaWdsGetCurrentChannelRate.h"

/* Request handling */

/*
 * This function packs the GetConnectionRate parameters
 * to the QMI message SDU
 *
 * \param  pParamField - Pointer to storage into which the packed
 *                       data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 * \sa     qaGobiApiWds.h for remaining parameter descriptions.
 *
 */
enum eQCWWANError PkQmiWdsGetCurrentChannelRate(
    WORD *pMlength,
    BYTE *pParamField )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    enum eQCWWANError eRCode;

    eRCode = qmbuild( pParamField, NULL, map,
                      eQMI_WDS_GET_RATES, pMlength);

    return eRCode;
}

/* Response handling */

/*
 * This function unpacks the GetConnectionRate ChannelRate from the
 * QMI response message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                           to return data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvChannelRate(BYTE *pTlvData, BYTE *resp)
{
    struct QmiWdsGetCurrentChannelRateResp *lresp =
                       (struct QmiWdsGetCurrentChannelRateResp *)resp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if(!lresp->pCurrentChannelTXRate ||
       !lresp->pCurrentChannelRXRate ||
       !lresp->pMaxChannelTXRate ||
       !lresp->pMaxChannelRXRate)
        return eQCWWAN_ERR_NONE;

    eRCode = GetLong(pTlvData, lresp->pCurrentChannelTXRate);
    if (eRCode != eQCWWAN_ERR_NONE)
        return eRCode;

    eRCode = GetLong(pTlvData, lresp->pCurrentChannelRXRate);
    if (eRCode != eQCWWAN_ERR_NONE)
        return eRCode;

    eRCode = GetLong(pTlvData, lresp->pMaxChannelTXRate);
    if (eRCode != eQCWWAN_ERR_NONE)
        return eRCode;

    eRCode = GetLong(pTlvData, lresp->pMaxChannelRXRate);
    if (eRCode != eQCWWAN_ERR_NONE)
        return eRCode;

    return eRCode;
}
 /*
  * This function unpacks the GetConnectionRate response message
  * to a user provided response structure
  *
  * \param mdmresp - Pointer to packed response from the modem
  *
  * \param apiresp - Pointer to storage to unpack into.
  *
  * \return    eQCWWAN_ERR_NONE, on success
  *
  */
enum eQCWWANError UpkQmiWdsGetCurrentChannelRate(
    BYTE   *mdmresp,
    struct QmiWdsGetCurrentChannelRateResp *apiresp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_CHANNEL_RATE, &UnpackTlvChannelRate },
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    eRCode = qmunpackresp( mdmresp, (BYTE *)apiresp, map,
                          eQMI_WDS_GET_RATES );
    return eRCode;
}

