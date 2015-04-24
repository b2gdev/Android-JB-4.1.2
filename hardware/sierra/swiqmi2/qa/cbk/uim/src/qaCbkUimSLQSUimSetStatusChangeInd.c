/*
 * \ingroup cbk
 *
 * \file    qaCbkUIMSLQSUIMSetStatusChangeInd.h
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
#include "qaCbkUimSLQSUimSetStatusChangeInd.h"

/*
 * This function unpacks the Card Status TLV parameters from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvCardStatus(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    cardStatus *lResp = &((UIMStatusChangeInfo *)pResp)->statusChange;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE              lcount,lIcount,lIcount1;

    /* Copy the Card Status Values into the List */
    eRCode = GetWord( pTlvData, &lResp->indexGwPri );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &lResp->index1xPri );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &lResp->indexGwSec );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &lResp->index1xSec );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->numSlot );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lcount = 0; lcount < lResp->numSlot; lcount++ )
    {
        slotInfo *temp = &lResp->SlotInfo[lcount];
        eRCode = GetByte( pTlvData, &temp->cardState );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &temp->upinState );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &temp->upinRetries );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &temp->upukRetries );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &temp->errorState );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &temp->numApp );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        for ( lIcount = 0; lIcount < temp->numApp; lIcount++ )
        {
            appStatus *lresp = &temp->AppStatus[lIcount];

            /* Copy the Application Values into the List */
            eRCode = GetByte( pTlvData, &lresp->appType );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }

            eRCode = GetByte( pTlvData, &lresp->appState );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }

            eRCode = GetByte( pTlvData, &lresp->persoState );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }

            eRCode = GetByte( pTlvData, &lresp->persoFeature );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }

            eRCode = GetByte( pTlvData, &lresp->persoRetries );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }

            eRCode = GetByte( pTlvData, &lresp->persoUnblockRetries );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }

            eRCode = GetByte( pTlvData, &lresp->aidLength );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }

            for ( lIcount1=0; lIcount1 < lresp->aidLength ; lIcount1++ )
            {
                eRCode = GetByte( pTlvData, &lresp->aidVal[lIcount1] );
                if ( eQCWWAN_ERR_NONE != eRCode )
                {
                    return eRCode;
                }
            }

            eRCode = GetByte( pTlvData, &lresp->univPin );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }

            eRCode = GetByte( pTlvData, &lresp->pin1State );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }

            eRCode = GetByte( pTlvData, &lresp->pin1Retries );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }

            eRCode = GetByte( pTlvData, &lresp->puk1Retries );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }

            eRCode = GetByte( pTlvData, &lresp->pin2State );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }

            eRCode = GetByte( pTlvData, &lresp->pin2Retries );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }

            eRCode = GetByte( pTlvData, &lresp->puk2Retries );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }
        }
    }
    return eRCode;
}

/*
 * This function unpacks the Set Status Change Indication message to a
 * user-provided response structure.
 *
 * \param   pMdmResp - Pointer to packed response from the modem.
 *
 * \param   pApiResp - Pointer to storage to unpack into.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UpkQmiCbkUIMSetStatusChangeInd(
    BYTE                *pMdmResp,
    UIMStatusChangeInfo *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_CARD_STATUS,  &UnpackCbkTlvCardStatus },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_UIM_STATUS_CHANGE_IND );
    return eRCode;
}
