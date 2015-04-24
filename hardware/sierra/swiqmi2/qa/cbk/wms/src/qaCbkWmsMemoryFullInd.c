/*
 * \ingroup cbk
 *
 * \file    qaCbkWmsMemoryFullInd.c
 *
 * \brief   Contains UnPacking routines for the
 *          QMI_WMS_MEMORY_FULL_IND message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaGobiApiCbk.h"
#include "qaCbkWmsMemoryFullInd.h"

/* Functions */

/*
 * This function unpacks the Memory Full information content parameter
 * from the QMI indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvMemoryInfoInd(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    SMSMemoryInfo *lResp = (SMSMemoryInfo *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the storage type */
    eRCode = GetByte( pTlvData, &(lResp->storageType) );
    if( eRCode != eQCWWAN_ERR_NONE )
    {
        return eRCode;
    }

    /* Extract the message mode */
    eRCode = GetByte( pTlvData, &(lResp->messageMode) );
    if( eRCode != eQCWWAN_ERR_NONE )
    {
        return eRCode;
    }
    return eRCode;
}

/*
 * This function unpacks the Memory Full Status information Indication message to
 * a user-provided response structure.
 *
 * \param   pMdmResp - Pointer to packed response from the modem.
 *
 * \param   pApiResp - Pointer to storage to unpack into.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UpkQmiCbkWmsMemoryFullInfoInd(
    BYTE          *pMdmResp,
    SMSMemoryInfo *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_MEMORY_FULL_INFO, &UnpackCbkTlvMemoryInfoInd},
        { eTLV_TYPE_INVALID,     NULL } /* Important. Sentinel.
                                         * Signifies last item in map.
                                         */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_WMS_MEMORY_FULL_IND );
    return eRCode;
}
