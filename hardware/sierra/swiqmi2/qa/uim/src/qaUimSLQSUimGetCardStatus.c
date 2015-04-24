/*
 * \ingroup uim
 *
 * \file    qaUimSLQSUimGetCardStatus.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_UIM_GET_CARD_STATUS message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaUimSLQSUimGetCardStatus.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the SLQS Get Card Status request parameters to the
 * QMI message SDU.
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaUimSLQSUimGetCardStatus.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiUimSlqsGetCardStatus(
    WORD *pMlength,
    BYTE *pParamField )
{
    /* There is no mandatory TLV and hence the map is empty */
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
                      eQMI_UIM_GET_CARD_STATUS,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the Card Status Values from the QMI response message
 * to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCardStatus(
    BYTE *pTlvData,
    BYTE *pResp )
{
    cardStatus *lResp = ((struct QmiUimSLQSGetCardStatusResp *)
                               pResp)->pUIMGetCardStatusResp->pCardStatus;
    enum eQCWWANError eRCode;
    BYTE              lcount,lIcount,lIcount1;

    /* Check For Invalid Parameter */
    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

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
 * This function unpacks the Hot Swap Status Values from the QMI response
 * message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvHotSwapStatus(
    BYTE *pTlvData,
    BYTE *pResp )
{
    hotSwapStatus *lResp = ((struct QmiUimSLQSGetCardStatusResp *)
                                   pResp)->pUIMGetCardStatusResp->pHotSwapStatus;
    enum eQCWWANError eRCode;
    BYTE              lcount;

    /* Check For Invalid Parameter */
    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Copy the Hot Swap Status Values into the List */
    eRCode = GetByte( pTlvData, &lResp->hotSwapLength );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lcount=0; lcount < lResp->hotSwapLength ; lcount++ )
    {
        eRCode = GetByte( pTlvData, &lResp->hotSwap[lcount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function unpacks the SLQS Get Card Status response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiUimSlqsGetCardStatus(
    BYTE                               *pMdmResp,
    struct QmiUimSLQSGetCardStatusResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,         &qmUnpackTlvResultCode },
        { eTLV_GET_CARD_STATUS,     &UnpackTlvCardStatus },
        { eTLV_GET_HOT_SWAP_STATUS, &UnpackTlvHotSwapStatus },
        { eTLV_TYPE_INVALID,        NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_UIM_GET_CARD_STATUS );
    return eRCode;
}
