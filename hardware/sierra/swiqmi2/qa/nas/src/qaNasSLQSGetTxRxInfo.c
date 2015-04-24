/*
 * \ingroup voice
 *
 * \file    qaNasSLQSGetTxRxInfo.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          eQMI_NAS_GET_TX_RX_INFO message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaNasSLQSGetTxRxInfo.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the Radio Interface Information to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvRadioInterface( BYTE *pBuf, BYTE *pParam )
{
    nasGetTxRxInfoReq *pReq = ( nasGetTxRxInfoReq * )pParam;

    /* Insert Radio Interference */
    return PutByte( pBuf, pReq->radio_if );
}

/*
 * This function packs the SLQS Get Tx/Rx information request parameters
 * to the QMI message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError PkQmiNasSlqsGetTxRxInfo(
    WORD              *pMlength,
    BYTE              *pParamField,
    nasGetTxRxInfoReq *pGetTxRxInfoReq  )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_RADIO_INTERFACE, &BuildTlvRadioInterface },
        { eTLV_TYPE_INVALID,    NULL } /* Important. Sentinel.
                                        * Signifies last item in map
                                        */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pGetTxRxInfoReq,
                      map,
                      eQMI_NAS_GET_TX_RX_INFO,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the Rx Chain0 Info from the QMI response message to a
 * user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvRxChain0Info(
    BYTE *pTlvData,
    BYTE *pResp )
{
    rxInfo *lResp = ((struct QmiNasSLQSGetTxRxInfoResp *)
                              pResp)->pGetTxRxInfoResp->pRXChain0Info;

    enum eQCWWANError eRCode;

    /* Check for Invalid Parameter */
    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the RX Chain 0 Info */
    eRCode = GetByte( pTlvData, &lResp->isRadioTuned );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetLong( pTlvData, &lResp->rxPower );
    if ( eQCWWAN_ERR_NONE != eRCode  )
    {
        return eRCode;
    }

    eRCode = GetLong( pTlvData, &lResp->ecio );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetLong( pTlvData, &lResp->rscp );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetLong( pTlvData, &lResp->rsrp );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetLong( pTlvData, &lResp->phase );
}

/*
 * This function unpacks the Rx Chain1 Info from the QMI response message to a
 * user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvRxChain1Info(
    BYTE *pTlvData,
    BYTE *pResp )
{
    rxInfo *lResp = ((struct QmiNasSLQSGetTxRxInfoResp *)
                             pResp)->pGetTxRxInfoResp->pRXChain1Info;
    enum eQCWWANError eRCode;

    /* Check For Invalid Parameter */
    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the RX Chain 0 Info */
    eRCode = GetByte( pTlvData, &lResp->isRadioTuned );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetLong( pTlvData, &lResp->rxPower );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetLong( pTlvData, &lResp->ecio );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetLong( pTlvData, &lResp->rscp );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetLong( pTlvData, &lResp->rsrp );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetLong( pTlvData, &lResp->phase );
}

/*
 * This function unpacks the Transaction Info from the QMI response message to
 * a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvTxInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    txInfo *lResp = ((struct QmiNasSLQSGetTxRxInfoResp *)
                             pResp)->pGetTxRxInfoResp->pTXInfo;

    enum eQCWWANError eRCode;

    /* Check For Invalid Parameter */
    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    eRCode = GetByte( pTlvData, &lResp->isInTraffic );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetLong( pTlvData, &lResp->txPower );
}

/*
 * This function unpacks the SLQS Get Tx/Rx Info response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp  - Pointer to packed response from the modem.
 *
 * \param  pApiResp  - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UPkQmiNasSlqsGetTxRxInfo(
    BYTE                             *pMdmResp,
    struct QmiNasSLQSGetTxRxInfoResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,     &qmUnpackTlvResultCode },
        { eTLV_RX_CHAIN_0_INFO, &UnpackTlvRxChain0Info },
        { eTLV_RX_CHAIN_1_INFO, &UnpackTlvRxChain1Info },
        { eTLV_TX_INFO,         &UnpackTlvTxInfo },
        { eTLV_TYPE_INVALID,    NULL } /* Important. Sentinel.
                                        * Signifies last item in map.
                                        */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_NAS_GET_TX_RX_INFO );
    return eRCode;
}
