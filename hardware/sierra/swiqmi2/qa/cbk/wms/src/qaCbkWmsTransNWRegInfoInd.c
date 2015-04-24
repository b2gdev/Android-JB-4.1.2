/*
 * \ingroup cbk
 *
 * \file    qaCbkWmsTransNWRegInfoInd.c
 *
 * \brief   Contains UnPacking routines for eQMI_WMS_TRANS_NW_REG_INFO_IND
 *          message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCbkWmsTransNWRegInfoInd.h"

/* Functions */

/*
 * This function unpacks the Transport network registration information TLV
 * from the QMI indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvNWRegInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    transNWRegInfoNotification *lResponse = (transNWRegInfoNotification *)pResp;

    /* Extract the registration status */
    return GetByte( pTlvData, &(lResponse->NWRegStat) );
}

/*
 * This function unpacks the Transport network registration information
 * Indication to a user-provided response structure.
 *
 * \param   pMdmResp - Pointer to packed response from the modem.
 *
 * \param   pApiResp - Pointer to storage to unpack into.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UpkQmiCbkWmsTransNWRegInfoInd(
    BYTE                       *pMdmResp,
    transNWRegInfoNotification *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_TRANS_NW_REG_INFO, &UnpackCbkTlvNWRegInfo},
        { eTLV_TYPE_INVALID,      NULL } /* Important. Sentinel.
                                          * Signifies last item in map.
                                          */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_WMS_TRANS_NW_REG_INFO_IND );
    return eRCode;
}
