/*
 * \ingroup nas
 *
 * \file    qaNasSLQSNasGetCellLocationInfo.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          eQMI_NAS_GET_CELL_LOCATION_INFO message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaNasSLQSNasGetCellLocationInfo.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the GetCellLocationInfo parameters
 * to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError PkQmiNasSlqsGetCellLocationInfo(
    WORD *pMlength,
    BYTE *pParamField )
{
    /* There is no mandatory TLV and hence the map is empty */
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    enum eQCWWANError eRCode;
    eRCode = qmbuild( pParamField,
                      NULL,
                      map,
                      eQMI_NAS_GET_CELL_LOCATION_INFO,
                      pMlength);
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the Geran Cell Information from the QMI response
 * message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvGeranCellInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    GERANInfo *lResp = ((struct QmiNasSLQSGetCellLocationInfoResp *)
                                 pResp)->pNasCellLocationInfoResp->pGERANInfo;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD              lCount, lICount;

    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the Geran Info */
    eRCode = GetLong( pTlvData, &lResp->cellID );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }


    for ( lCount = 0 ; lCount < PLMN_LENGTH ; lCount++ )
    {
        eRCode = GetByte( pTlvData, &lResp->plmn[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    eRCode = GetWord( pTlvData, &lResp->lac );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &lResp->arfcn );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->bsic );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetLong( pTlvData, &lResp->timingAdvance );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &lResp->rxLev );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->nmrInst );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < lResp->nmrInst ; lCount++ )
    {
        nmrCellInfo *lresp = &lResp->insNmrCellInfo[lCount];

        eRCode = GetLong( pTlvData, &lresp->nmrCellID );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        for ( lICount = 0 ; lICount < PLMN_LENGTH ; lICount++ )
        {
            eRCode = GetByte( pTlvData, &lresp->nmrPlmn[lICount] );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }
        }


        eRCode = GetWord( pTlvData, &lresp->nmrLac );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetWord( pTlvData, &lresp->nmrArfcn );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->nmrBsic );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetWord( pTlvData, &lresp->nmrRxLev );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }
    return eRCode;
}

/*
 * This function unpacks the UMTS Cell Information from the QMI response
 * message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvUMTSCellInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    UMTSInfo *lResp = ((struct QmiNasSLQSGetCellLocationInfoResp *)
                                 pResp)->pNasCellLocationInfoResp->pUMTSInfo;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD              lCount;

    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the UMTS Info */
    eRCode = GetLong( pTlvData, &lResp->cellID );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < PLMN_LENGTH ; lCount++ )
    {
        eRCode = GetByte( pTlvData, &lResp->plmn[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    eRCode = GetWord( pTlvData, &lResp->lac );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &lResp->uarfcn );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &lResp->psc );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &lResp->rscp );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &lResp->ecio );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->umtsInst );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < lResp->umtsInst ; lCount++ )
    {
        UMTSinstInfo *lresp = &lResp->UMTSInstInfo[lCount];

        eRCode = GetWord( pTlvData, &lresp->umtsUarfcn );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetWord( pTlvData, &lresp->umtsPsc );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetWord( pTlvData, &lresp->umtsRscp );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetWord( pTlvData, &lresp->umtsEcio );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    eRCode = GetByte( pTlvData, &lResp->geranInst );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < lResp->geranInst ; lCount++ )
    {
        geranInstInfo *lresp = &lResp->GeranInstInfo[lCount];

        eRCode = GetWord( pTlvData, &lresp->geranArfcn );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->geranBsicNcc );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->geranBsicBcc );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetWord( pTlvData, &lresp->geranRssi );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }
    return eRCode;
}

/*
 * This function unpacks the CDMA Cell Information from the QMI response
 * message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvCDMACellInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    CDMAInfo *lResp = ((struct QmiNasSLQSGetCellLocationInfoResp *)
                                 pResp)->pNasCellLocationInfoResp->pCDMAInfo;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the CDMA Info */
    eRCode = GetWord( pTlvData, &lResp->sid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &lResp->nid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &lResp->baseId );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &lResp->refpn );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetLong( pTlvData, &lResp->baseLat );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetLong( pTlvData, &lResp->baseLong );
}

/*
 * This function unpacks the LTE Intra Frequency Cell Information from the QMI
 * response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvLTEIntraFrqCellInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    LTEInfoIntrafreq *lResp = ((struct QmiNasSLQSGetCellLocationInfoResp *)
                           pResp)->pNasCellLocationInfoResp->pLTEInfoIntrafreq;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD              lCount;

    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the LTE Intra Frequency Cell Info */
    eRCode = GetByte( pTlvData, &lResp->ueInIdle );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < PLMN_LENGTH ; lCount++ )
    {
        eRCode = GetByte( pTlvData, &lResp->plmn[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    eRCode = GetWord( pTlvData, &lResp->tac );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetLong( pTlvData, &lResp->globalCellId );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &lResp->earfcn );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &lResp->servingCellId );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->cellReselPriority );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->sNonIntraSearch );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->threshServingLow );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->sIntraSearch );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->cellsLen );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < lResp->cellsLen ; lCount++ )
    {
        cellParams *lresp = &lResp->CellParams[lCount];

        eRCode = GetWord( pTlvData, &lresp->pci );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetWord( pTlvData, &lresp->rsrq );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetWord( pTlvData, &lresp->rsrp );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetWord( pTlvData, &lresp->rssi );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetWord( pTlvData, &lresp->srxlev );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }
    return eRCode;
}

/*
 * This function unpacks the LTE Inter Frequency Cell Information from the QMI
 * response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvLTEInterFrqCellInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    LTEInfoInterfreq *lResp = ((struct QmiNasSLQSGetCellLocationInfoResp *)
                           pResp)->pNasCellLocationInfoResp->pLTEInfoInterfreq;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD              lCount, lICount;

    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the LTE Inter Frequency Cell Info */
    eRCode = GetByte( pTlvData, &lResp->ueInIdle );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->freqsLen );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < lResp->freqsLen ; lCount++ )
    {
        infoInterFreq *lresp = &lResp->InfoInterfreq[lCount];

        eRCode = GetWord( pTlvData, &lresp->earfcn );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->threshXLow );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->threshXHigh );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->cell_resel_priority );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->cells_len );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        for ( lICount = 0 ; lICount < lresp->cells_len ; lICount++ )
        {
            cellParams *lCellParams = &lresp->cellInterFreqParams[lICount];

            eRCode = GetWord( pTlvData, &lCellParams->pci );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }

            eRCode = GetWord( pTlvData, &lCellParams->rsrq );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }

            eRCode = GetWord( pTlvData, &lCellParams->rsrp );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }

            eRCode = GetWord( pTlvData, &lCellParams->rssi );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }

            eRCode = GetWord( pTlvData, &lCellParams->srxlev );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }
        }
    }
    return eRCode;
}

/*
 * This function unpacks the LTE Neighboring GSM Cell Information from the QMI
 * response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvLTENebGSMCellInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    LTEInfoNeighboringGSM *lResp =((struct QmiNasSLQSGetCellLocationInfoResp *)
                      pResp)->pNasCellLocationInfoResp->pLTEInfoNeighboringGSM;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD              lCount, lICount;

    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the LTE Neighboring GSM Cell Info */
    eRCode = GetByte( pTlvData, &lResp->ueInIdle );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->freqsLen );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < lResp->freqsLen ; lCount++ )
    {
        lteGsmCellInfo *lresp = &lResp->LteGsmCellInfo[lCount];

        eRCode = GetByte( pTlvData, &lresp->cellReselPriority );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->threshGsmHigh );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->threshGsmLow );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->nccPermitted );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->cells_len );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        for ( lICount = 0 ; lICount < lresp->cells_len ; lICount++ )
        {
            gsmCellInfo *lGSMCellInfo = &lresp->GsmCellInfo[lICount];

            eRCode = GetWord( pTlvData, &lGSMCellInfo->arfcn );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }

            eRCode = GetByte( pTlvData, &lGSMCellInfo->band1900 );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }

            eRCode = GetByte( pTlvData, &lGSMCellInfo->cellIdValid );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }

            eRCode = GetByte( pTlvData, &lGSMCellInfo->bsicId );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }

            eRCode = GetWord( pTlvData, &lGSMCellInfo->rssi );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }

            eRCode = GetWord( pTlvData, &lGSMCellInfo->srxlev );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }
        }
    }
    return eRCode;
}

/*
 * This function unpacks the LTE Neighboring WCDMA Cell Information from the QMI
 * response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvLTENebWCDMACellInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    LTEInfoNeighboringWCDMA *lResp =
    ((struct QmiNasSLQSGetCellLocationInfoResp *)pResp)->
                           pNasCellLocationInfoResp->pLTEInfoNeighboringWCDMA;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD              lCount, lICount;

    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the LTE Neighboring WCDMA Cell Info */
    eRCode = GetByte( pTlvData, &lResp->ueInIdle );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->freqsLen );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < lResp->freqsLen ; lCount++ )
    {
        lteWcdmaCellInfo *lresp = &lResp->LTEWCDMACellInfo[lCount];

        eRCode = GetWord( pTlvData, &lresp->uarfcn );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->cellReselPriority );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetWord( pTlvData, &lresp->threshXhigh );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetWord( pTlvData, &lresp->threshXlow );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->cellsLen );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        for ( lICount = 0 ; lICount < lresp->cellsLen ; lICount++ )
        {
            wcdmaCellInfo *lWCDMACellInfo = &lresp->WCDMACellInfo[lICount];

            eRCode = GetWord( pTlvData, &lWCDMACellInfo->psc );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }

            eRCode = GetWord( pTlvData, &lWCDMACellInfo->cpich_rscp );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }

            eRCode = GetWord( pTlvData, &lWCDMACellInfo->cpich_ecno );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }

            eRCode = GetWord( pTlvData, &lWCDMACellInfo->srxlev );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }
        }
    }
    return eRCode;
}

/*
 * This function unpacks the SlqsGetCellLocationInfo response message to to a
 * user-provided response structure.
 *
 * \param     pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp  [OUT]  - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiNasSlqsGetCellLocationInfo(
    BYTE                                     *pMdmResp,
    struct QmiNasSLQSGetCellLocationInfoResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,                &qmUnpackTlvResultCode },
        { eTLV_GERAN_CELL_INFO,            &UnpackTlvGeranCellInfo },
        { eTLV_UMTS_CELL_INFO,             &UnpackTlvUMTSCellInfo },
        { eTLV_CDMA_CELL_INFO,             &UnpackTlvCDMACellInfo },
        { eTLV_LTE_INTRA_FREQ_CELL_INFO,   &UnpackTlvLTEIntraFrqCellInfo },
        { eTLV_LTE_INTER_FREQ_CELL_INFO,   &UnpackTlvLTEInterFrqCellInfo },
        { eTLV_LTE_NEIGHB_GSM_CELL_INFO,   &UnpackTlvLTENebGSMCellInfo },
        { eTLV_LTE_NEIGHB_WCDMA_CELL_INFO, &UnpackTlvLTENebWCDMACellInfo },
        { eTLV_TYPE_INVALID,               NULL }  /* Important. Sentinel.
                                                    * Signifies last item in map.
                                                    */
    };
    eRCode = qmunpackresp( pMdmResp,
                          (BYTE*)pApiResp,
                          map,
                          eQMI_NAS_GET_CELL_LOCATION_INFO );
    return eRCode;
}
