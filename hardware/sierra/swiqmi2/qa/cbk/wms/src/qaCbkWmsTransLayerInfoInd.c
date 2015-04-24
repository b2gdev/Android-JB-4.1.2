/*
 * \ingroup cbk
 *
 * \file    qaCbkWmsTransLayerInfoInd.c
 *
 * \brief   Contains UnPacking routines for QMI_WMS_TRANSPORT_LAYER_INFO_IND
 *          message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCbkWmsTransLayerInfoInd.h"

/* Functions */

/*
 * This function unpacks the Transport Layer registration information TLV
 * from the QMI indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvRegInd(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    transLayerNotification *lResponse = (transLayerNotification *)pResp;

    /* Extract the regInd */
    return GetByte( pTlvData, &(lResponse->regInd) );
}

/*
 * This function unpacks the Transport layer information TLV from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvTransLayerInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    transLayerNotification *lResponse = (transLayerNotification *)pResp;
    transLayerInfo         tResp;
    lResponse->pTransLayerInfo = &tResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE bTemp;

    /* Extract the TransType */
    eRCode = GetByte( pTlvData, &bTemp );
    if( eRCode != eQCWWAN_ERR_NONE )
    {
        return eRCode;
    }
    tResp.TransType = bTemp;

    /* Extract the TransCap */
    eRCode = GetByte( pTlvData, &(bTemp) );
    if( eRCode != eQCWWAN_ERR_NONE )
    {
        return eRCode;
    }
    tResp.TransCap = bTemp;

    return eRCode;
}

/*
 * This function unpacks the Transport Layer information Indication message to
 * a user-provided response structure.
 *
 * \param   pMdmResp - Pointer to packed response from the modem.
 *
 * \param   pApiResp - Pointer to storage to unpack into.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UpkQmiCbkWmsTransLayerInfoInd(
    BYTE                   *pMdmResp,
    transLayerNotification *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_TRANS_LAYER_REG_INFO, &UnpackCbkTlvRegInd},
        { eTLV_TRANS_LAYER_INFO,     &UnpackCbkTlvTransLayerInfo },
        { eTLV_TYPE_INVALID,         NULL } /* Important. Sentinel.
                                             * Signifies last item in map.
                                             */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_WMS_TRANS_LAYER_INFO_IND );
    return eRCode;
}
