/*
 * \ingroup nas
 *
 * \file   qaNasSLQSGetSignalStrength.c
 *
 * \brief  Contains Packing and UnPacking routines for the
 *         QMI_NAS_GET_SIGNAL_STRENGTH message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaQmiBasic.h"
#include "qaGobiApiNas.h"
#include "qaNasSLQSGetSignalStrength.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the SLQSGetSignalStrength Signal Strength Request Mask
 * field to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed data will be
 *                         placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSlqsRequestMask(
    BYTE    *pBuf,
    BYTE    *pParam )
{
    struct QmiNasSlqsGetSignalStrengthReq *pReq =
                        (struct QmiNasSlqsGetSignalStrengthReq *)pParam;

    /* Insert the Signal Strength mask into the QMI message */
    return PutWord( pBuf, pReq->reqMask );
}

/*
 * This function packs the SLQSGetSignalStrength parameters to the QMI message
 * SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \param  pArraySizes [IN]  - Size of the Signal Strength Array.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError PkQmiNasSlqsGetSignalStrength(
    WORD   *pMlength,
    BYTE   *pParamField,
    USHORT reqMask )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SIGNAL_STRENGTH_REQUEST_MASK, &BuildTlvSlqsRequestMask },
        { eTLV_TYPE_INVALID,                 NULL } /* Important. Sentinel.
                                                    * Signifies last item in map
                                                    */
    };

    struct QmiNasSlqsGetSignalStrengthReq req;
    enum eQCWWANError                     eRCode;

    /* Intialize the request structure */
    slmemset( (char *)&req, 0, sizeof(struct QmiNasSlqsGetSignalStrengthReq) );
    req.reqMask   = reqMask;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_NAS_GET_RSSI,
                      pMlength );
    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the SLQSGetSignalStrength Signal Strength from the
 * QMI response message to a user provided response structure. This is
 * part of the mandatory TLV.
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvSlqsSignalStrength(
    BYTE *pTlvData,
    BYTE *pResp )
{
    struct QmiNasSlqsGetSignalStrengthResp *lResp =
        (struct QmiNasSlqsGetSignalStrengthResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    INT8              signalStrength;

    /* Get Signal Strength in local variable */
    eRCode = GetByte( pTlvData, (BYTE *)&signalStrength );
    if (eQCWWAN_ERR_NONE != eRCode)
    {
        return eRCode;
    }

    /* Copy the current Signal Strength as the first element of Receive Signal
     * Strength list.
     */
    lResp->pSignalInfo->rxSignalStrengthList[0].rxSignalStrength =
                                                                 signalStrength;

    /* Copy the current radioif as the first element of Receive Signal
     * Strength list.
     */
    eRCode = GetByte( pTlvData, &( lResp->pSignalInfo->
                                   rxSignalStrengthList[0].radioIf ) );
    if (eQCWWAN_ERR_NONE == eRCode)
    {
        lResp->pSignalInfo->rxSignalStrengthListLen++;
    }

    return eRCode;
}

/*
 * This function unpacks the SLQSGetSignalStrength Receive Signal Strength list
 * from the QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvSlqsRSSIList(
    BYTE *pTlvData,
    BYTE *pResp )
{
    struct QmiNasSlqsGetSignalStrengthResp *lResp =
        (struct QmiNasSlqsGetSignalStrengthResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD              listLen = 0, idx = 0;
    BYTE              rssi;

    /* Get rssiListLen */
    eRCode = GetWord( pTlvData, &( listLen ) );

    if (eQCWWAN_ERR_NONE != eRCode)
    {
        return eRCode;
    }

    /* Receive Signal Strength list length = ListLen + 1 due to the inclusion of
     * Mandatory TLV signal strength member at the front (first array member)
     * of the rxSignalStrengthList (see UnpackTlvSlqsSignalStrength above).
     */
    lResp->pSignalInfo->rxSignalStrengthListLen += listLen;

    /* Update the length in local variable */
    listLen = lResp->pSignalInfo->rxSignalStrengthListLen;

    /* If the no. of elements are more than the space in the list */
    if( SLQS_SS_INFO_LIST_MAX_ELEMENTS <= listLen )
    {
        return eQCWWAN_ERR_BUFFER_SZ;
    }

    /* Fill the rssi and Radio Interface in Receive Signal Strength List */
    for ( idx = 1; idx < listLen; idx++ )
    {
        /* Get RSSI */
        eRCode = GetByte( pTlvData, &rssi );
        if (eQCWWAN_ERR_NONE != eRCode)
        {
            return eRCode;
        }

        /* Convert the retrieved value in dBm and copy it in the list */
        lResp->pSignalInfo->rxSignalStrengthList[idx].rxSignalStrength
                                                            = ( ( ~rssi ) + 1 );

        /* Get Radio Interface */
        eRCode = GetByte( pTlvData,&( lResp->pSignalInfo->
                                   rxSignalStrengthList[idx].radioIf ) );

        if (eQCWWAN_ERR_NONE != eRCode)
        {
            return eRCode;
        }
    }
    return eRCode;
}

/*
 * This function unpacks the SLQSGetSignalStrength ECIO list from the QMI
 * response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvSlqsECIOList(
    BYTE *pTlvData,
    BYTE *pResp )
{
    struct QmiNasSlqsGetSignalStrengthResp *lResp =
        (struct QmiNasSlqsGetSignalStrengthResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD              listLen = 0, idx = 0;
    BYTE              ecio;

    /* Get ecioListLen */
    eRCode = GetWord( pTlvData, &( lResp->pSignalInfo->ecioListLen ) );

    if (eQCWWAN_ERR_NONE != eRCode)
    {
        return eRCode;
    }

    /* Get the length in local variable */
    listLen = lResp->pSignalInfo->ecioListLen;

    /* If the no. of elemnts are more than the space in the list */
    if( SLQS_SS_INFO_LIST_MAX_ELEMENTS < listLen )
    {
        return eQCWWAN_ERR_BUFFER_SZ;
    }

    /* Fill the ECIO and Radio Interface in ECIO List */
    for ( idx = 0; idx < listLen; idx++ )
    {
        /* Get ECIO */
        eRCode = GetByte( pTlvData, &ecio );
        if (eQCWWAN_ERR_NONE != eRCode)
        {
            return eRCode;
        }

        /* Change the retrieved value in dBm and copy it in ECIO list */
        lResp->pSignalInfo->ecioList[idx].ecio = ~( ecio >> 1 ) + 1;

        /* Get Radio Interface */
        eRCode = GetByte( pTlvData, &( lResp->pSignalInfo->
                                       ecioList[idx].radioIf ) );

        if (eQCWWAN_ERR_NONE != eRCode)
        {
            return eRCode;
        }
    }
    return eRCode;
}

/*
 * This function unpacks the SLQSGetSignalStrength IO from the QMI response
 * message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvSlqsIO(
    BYTE *pTlvData,
    BYTE *pResp )
{
    struct QmiNasSlqsGetSignalStrengthResp *lResp =
        (struct QmiNasSlqsGetSignalStrengthResp *)pResp;

    /* Get IO */
    return GetLong( pTlvData, &( lResp->pSignalInfo->Io ) );
}

/*
 * This function unpacks the SLQSGetSignalStrength SINR list from the QMI
 * response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvSlqsSINRList(
    BYTE *pTlvData,
    BYTE *pResp )
{
    struct QmiNasSlqsGetSignalStrengthResp *lResp =
        (struct QmiNasSlqsGetSignalStrengthResp *)pResp;

    /* Get SINR */
    return GetByte( pTlvData, &( lResp->pSignalInfo->sinr ) );
}

/*
 * This function unpacks the SLQSGetSignalStrength Error Rate list from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvSlqsErrorRateList(
    BYTE *pTlvData,
    BYTE *pResp )
{
    struct QmiNasSlqsGetSignalStrengthResp *lResp =
        (struct QmiNasSlqsGetSignalStrengthResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD              listLen = 0, idx = 0;

    /* Get errorListLen */
    eRCode = GetWord( pTlvData, &( lResp->pSignalInfo->errorRateListLen ) );

    if (eQCWWAN_ERR_NONE != eRCode)
    {
        return eRCode;
    }

    /* Get the length in local variable */
    listLen = lResp->pSignalInfo->errorRateListLen;

    /* If the no. of elemnts are more than the space in the list */
    if( SLQS_SS_INFO_LIST_MAX_ELEMENTS < listLen )
    {
        return eQCWWAN_ERR_BUFFER_SZ;
    }

    /* Fill the Error Rate and Radio Interface in Error Rate List */
    for ( idx = 0; idx < listLen; idx++ )
    {
        /* Get Error Rate */
        eRCode = GetWord( pTlvData, &( lResp->pSignalInfo->
                                       errorRateList[idx].errorRate ) );

        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        /* Get Radio Interface */
        eRCode = GetByte( pTlvData, &( lResp->pSignalInfo->
                                       errorRateList[idx].radioIf ) );

        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }
    return eRCode;
}

/*
 * This function unpacks the SLQSGetSignalStrength RSRQ from the QMI response
 * message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvSlqsRSRQ(
    BYTE *pTlvData,
    BYTE *pResp )
{
    struct QmiNasSlqsGetSignalStrengthResp *lResp =
        (struct QmiNasSlqsGetSignalStrengthResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Get RSRQ */
    eRCode = GetByte( pTlvData,(BYTE *)&( lResp->pSignalInfo->rsrqInfo.rsrq ) );

    if (eQCWWAN_ERR_NONE != eRCode)
    {
        return eRCode;
    }

    /* Get Radio Interface */
    return GetByte( pTlvData, &( lResp->pSignalInfo->rsrqInfo.radioIf ) );
}

/*
 * This function unpacks the SLQSGetSignalStrength response message to a
 * user-provided response structure.
 *
 * \param     MdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp  [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UpkQmiNasSlqsGetSignalStrength(
    BYTE                                   *pMdmResp,
    struct QmiNasSlqsGetSignalStrengthResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,               &qmUnpackTlvResultCode },
        { eTLV_SLQS_SIGNAL_STRENGTH,      &UnpackTlvSlqsSignalStrength },
        { eTLV_SLQS_RSSI_LIST,            &UnpackTlvSlqsRSSIList },
        { eTLV_SLQS_ECIO_LIST,            &UnpackTlvSlqsECIOList },
        { eTLV_SLQS_IO,                   &UnpackTlvSlqsIO },
        { eTLV_SLQS_SINR_LIST,            &UnpackTlvSlqsSINRList },
        { eTLV_SLQS_ERROR_RATE_LIST,      &UnpackTlvSlqsErrorRateList },
        { eTLV_SLQS_RSRQ,                 &UnpackTlvSlqsRSRQ },
        { eTLV_TYPE_INVALID,              NULL } /* Important. Sentinel.
                                                  * Signifies last item in map.
                                                  */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE*)pApiResp,
                           map,
                           eQMI_NAS_GET_RSSI );
    return eRCode;
}
