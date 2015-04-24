/*
 * \ingroup cbk
 *
 * \file    qaCbkUIMSLQSUIMSetRefreshCallBack.h
 *
 * \brief   Contains UnPacking routines for the QMI_UIM_REFRESH_IND message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCbkUimSLQSUimSetRefreshInd.h"

/*
 * This function unpacks the Refresh Event TLV parameters from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvRefreshEvent(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    UIMRefreshEvent *lResponse = (UIMRefreshEvent *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE lIcount, lCount;

    /* Extract the Stage Refresh procedure */
    eRCode = GetByte( pTlvData, &(lResponse->stage) );
    if( eRCode != eQCWWAN_ERR_NONE )
    {
        return eRCode;
    }

    /* Extract the Refresh mode */
    eRCode = GetByte( pTlvData, &(lResponse->mode) );
    if( eRCode != eQCWWAN_ERR_NONE )
    {
        return eRCode;
    }

    /* Extract the Session type */
    eRCode = GetByte( pTlvData, &(lResponse->sessionType) );
    if( eRCode != eQCWWAN_ERR_NONE )
    {
        return eRCode;
    }

    /* Extract the AID Len  */
    eRCode = GetByte( pTlvData, &(lResponse->aidLength) );
    if( eRCode != eQCWWAN_ERR_NONE )
    {
        return eRCode;
    }

    for ( lIcount = 0 ; lIcount < lResponse->aidLength; lIcount++ )
    {
        eRCode = GetByte( pTlvData, &lResponse->aid[lIcount]);
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    /* Extract the Number of files */
    eRCode = GetWord( pTlvData, &(lResponse->numOfFiles) );
    if( eRCode != eQCWWAN_ERR_NONE )
    {
        return eRCode;
    }

    for( lIcount = 0; lIcount < lResponse->numOfFiles; lIcount++ )
    {
        fileInfo *pTemp = (fileInfo *)&lResponse->arrfileInfo[lIcount];

        /* Extract the File ID */
        eRCode = GetWord( pTlvData, &pTemp->fileID );
        if( eRCode != eQCWWAN_ERR_NONE )
        {
            return eRCode;
        }

        /* Extract the Path length */
        eRCode = GetByte( pTlvData, &pTemp->pathLen );
        if( eRCode != eQCWWAN_ERR_NONE )
        {
            return eRCode;
        }

        for ( lCount = 0; lCount < pTemp->pathLen; lCount++ )
        {
            eRCode = GetWord( pTlvData, &pTemp->path[lCount]);
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }
        }
    }
    return eRCode;
}

/*
 * This function unpacks the Set Refresh Indication message to a
 * user-provided response structure.
 *
 * \param   pMdmResp - Pointer to packed response from the modem.
 *
 * \param   pApiResp - Pointer to storage to unpack into.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UpkQmiCbkUIMSetRefreshInd(
    BYTE            *pMdmResp,
    UIMRefreshEvent *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_REFRESH_EVENT, &UnpackCbkTlvRefreshEvent },
        { eTLV_TYPE_INVALID,  NULL } /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_UIM_REFRESH_IND );
    return eRCode;
}
