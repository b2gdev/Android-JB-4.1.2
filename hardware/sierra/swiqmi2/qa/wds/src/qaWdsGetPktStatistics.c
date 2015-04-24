/*
 * \ingroup wds
 *
 * \file qaWdsGetPktStatistics.c
 *
 * \brief  Contains Packing and UnPacking routines for the
 *         QMI_WDS_GET_STATISTICS message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"

#include "qaWdsGetPktStatistics.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the GetPacketStatus StatMask field to the
 * QMI message SDU
 *
 * \param pBuf    [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param pParam  [IN]  - Pointer to structure containing data for
 *                        this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvStatMask(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsGetPktStatisticsReq *pReq =
                (struct QmiWdsGetPktStatisticsReq *)pParam;

    /* Add TLV data */
    return PutLong(pBuf, pReq->statmask);
}

/*
 * This function packs the GetPacketStatus parameters
 * to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 * \sa     qaGobiApiWds.h for remaining parameter descriptions.
 *
 */
enum eQCWWANError PkQmiWdsGetPktStatistics(
    WORD    *pMlength,
    BYTE    *pParamField,
    ULONG   statmask)
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_STAT_MASK, &BuildTlvStatMask },
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    struct QmiWdsGetPktStatisticsReq req;
    enum eQCWWANError eRCode;

    slmemset((char *)&req, 0, sizeof (struct QmiWdsGetPktStatisticsReq));
    req.statmask = statmask;

    eRCode = qmbuild(pParamField,
                     (BYTE *)&req,
                     map,
                     eQMI_WDS_GET_STATISTICS,
                     pMlength);
    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the GetPacketStatus TXPacketSuccesses from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                           to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvTxPktSuccessses(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsGetPktStatisticsResp *lResp =
        (struct QmiWdsGetPktStatisticsResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (!lResp->pTXPacketSuccesses)
        return eRCode;

    eRCode = GetLong(pTlvData, lResp->pTXPacketSuccesses);
    return eRCode;
}

/*
 * This function unpacks the GetPacketStatus RXPacketSuccesses from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                           to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvRxPktSuccessses(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsGetPktStatisticsResp *lResp =
        (struct QmiWdsGetPktStatisticsResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (!lResp->pRXPacketSuccesses)
        return eRCode;

    eRCode = GetLong(pTlvData, lResp->pRXPacketSuccesses);

    return eRCode;
}

/*
 * This function unpacks the GetPacketStatus TXPacketErrors from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                           to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvTxPktError(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsGetPktStatisticsResp *lResp =
        (struct QmiWdsGetPktStatisticsResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (!lResp->pTXPacketErrors)
       return eRCode;

    eRCode = GetLong(pTlvData, lResp->pTXPacketErrors);

    return eRCode;
}

/*
 * This function unpacks the GetPacketStatus RXPacketErrors from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                           to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvRxPktError(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsGetPktStatisticsResp *lResp =
        (struct QmiWdsGetPktStatisticsResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (!lResp->pRXPacketErrors)
        return eRCode;

    eRCode = GetLong(pTlvData, lResp->pRXPacketErrors);

    return eRCode;
}

/*
 * This function unpacks the GetPacketStatus TXPacketOverflows from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                           to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvTxPktOverflows(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsGetPktStatisticsResp *lResp =
        (struct QmiWdsGetPktStatisticsResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (!lResp->pTXPacketOverflows)
        return eRCode;

    eRCode = GetLong(pTlvData, lResp->pTXPacketOverflows);

    return eRCode;
}

/*
 * This function unpacks the GetPacketStatus RXPacketOverflows from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                           to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvRxPktOverflows(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsGetPktStatisticsResp *lResp =
        (struct QmiWdsGetPktStatisticsResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (!lResp->pRXPacketOverflows)
        return eRCode;

    eRCode = GetLong(pTlvData, lResp->pRXPacketOverflows);

    return eRCode;
}

/*
 * This function unpacks the GetPacketStatus TXTotalBytes from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                           to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvTxTotalBytes(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsGetPktStatisticsResp *lResp =
        (struct QmiWdsGetPktStatisticsResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (!lResp->pTXOkBytesCount)
        return eRCode;

    eRCode = GetLongLong(pTlvData, lResp->pTXOkBytesCount);

    return eRCode;
}

/*
 * This function unpacks the GetPacketStatus RXTotalBytes from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                           to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvRxTotalBytes(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsGetPktStatisticsResp *lResp =
        (struct QmiWdsGetPktStatisticsResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (!lResp->pRXOkBytesCount)
        return eRCode;

    eRCode = GetLongLong(pTlvData, lResp->pRXOkBytesCount);

    return eRCode;
}

/*
 * This function unpacks the GetPacketStatus response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp - Pointer to packed response from the modem.
 *
 * \param  pApiResp - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UpkQmiWdsGetPktStatistics(
    BYTE *pMdmResp,
    struct QmiWdsGetPktStatisticsResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,         &qmUnpackTlvResultCode },
        { eTLV_TX_OK_COUNT,         &UnpackTlvTxPktSuccessses },
        { eTLV_RX_OK_COUNT,         &UnpackTlvRxPktSuccessses },
        { eTLV_TX_ERR_COUNT,        &UnpackTlvTxPktError },
        { eTLV_RX_ERR_COUNT,        &UnpackTlvRxPktError },
        { eTLV_TX_OFL_COUNT,        &UnpackTlvTxPktOverflows },
        { eTLV_RX_OFL_COUNT,        &UnpackTlvRxPktOverflows },
        { eTLV_TX_OK_BYTE_COUNT,    &UnpackTlvTxTotalBytes },
        { eTLV_RX_OK_BYTE_COUNT,    &UnpackTlvRxTotalBytes },
        { eTLV_TYPE_INVALID,        NULL }  /* Important. Sentinel.
                                             * Signifies last item in map.
                                             */
    };
    eRCode = qmunpackresp( pMdmResp,
                          (BYTE *)pApiResp,
                          map,
                          eQMI_WDS_GET_STATISTICS );
    return eRCode;
}
