/*
 * \ingroup nas
 *
 * \file qaNasGetSignalStrength.c
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
#include "qaNasGetSignalStrength.h"

static BYTE  firstTime = 1;
static ULONG arraySize = 0;

/*****************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the GetSignalStrength BuildTlvRequestMask
 * field to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return      eQCWWAN_ERR_NONE, on success
 * \return      eQCWWAN_ERR_MEMORY, on error
 *
 */
enum eQCWWANError BuildTlvRequestMask(
    BYTE    *pBuf,
    BYTE    *pParam )
{
    UNUSEDPARAM( pParam );

    /* Varible to hold the static mask */
    USHORT *pSignalStrengthMask = SIGNAL_STRENGTH_REQ_MASK;

    /* Insert the Signal Strength mask into the QMI message */
    /* Additional Request for RSSI is present in the message */
    return PutWord(pBuf, *pSignalStrengthMask);
}

/*
 * This function packs the GetSignalStrength parameters
 * to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \param  pArraySizes [IN]  - Size of the Signal Strength Array.
 *
 * \return eQCWWAN_ERR_NONE, on success
 * \return eQCWWAN_ERR_MEMORY, on error
 *
 */
enum eQCWWANError PkQmiNasGetSignalStrengths(
    WORD     *pMlength,
    BYTE     *pParamField )
{
    static struct qmTlvBuilderItem map[] =
    {
        /* Not required for Signal Strength - kept for future use */
        /* { eTLV_REQUEST_MASK, &BuildTlvRequestMask }, */
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };
    enum eQCWWANError eRCode;

    eRCode = qmbuild( pParamField,
                      NULL,
                      map,
                      eQMI_NAS_GET_RSSI,
                      pMlength );
    return eRCode;
}


/*****************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the GetSignalStrength Signal Strength from the
 * QMI response message to a user provided response structure. This is
 * part of the mandatory TLV.
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return      eQCWWAN_ERR_NONE, on success
 * \return      eQCWWAN_ERR_MEMORY, on error
 */
enum eQCWWANError UnpackTlvSignalStrength(
    BYTE    *pTlvData,
    BYTE    *pResp )
{
    struct QmiNasGetSignalStrengthResp *lResp =
        (struct QmiNasGetSignalStrengthResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE lByte;

    if( firstTime )
    {
        firstTime = 0;
        arraySize = *(lResp->pArraySizes);
    }

    if( arraySize == 0 )
        return eRCode;

    *(lResp->pArraySizes) = 1;
    /* Copy the Signal Strength and the Radio interface
     * in the current occurance */
    eRCode = GetByte( pTlvData, (BYTE *)&(lResp->pSignalStrengths[0]) );
    if (eQCWWAN_ERR_NONE != eRCode)
        return eRCode;

    eRCode = GetByte( pTlvData, &lByte);
    if (eQCWWAN_ERR_NONE == eRCode)
    { 
        (lResp->pRadioInterfaces[0]) = lByte;
    } 
    return eRCode;
}

/*
 * This function unpacks the GetSignalStrength Signal Strength list from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return      eQCWWAN_ERR_NONE, on success
 * \return      eQCWWAN_ERR_MEMORY, on error
 */
enum eQCWWANError UnpackTlvSignalStrengthList(
    BYTE    *pTlvData,
    BYTE    *pResp )
{
    struct QmiNasGetSignalStrengthResp *lResp =
        (struct QmiNasGetSignalStrengthResp *)pResp;
    ULONG lArraySize, lArrayIndex = 0;
    WORD lTemp;
    BYTE lByte;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if( firstTime )
    {
        firstTime = 0;
        arraySize = *(lResp->pArraySizes);
    }

    if( arraySize == 0 )
        return eRCode;

    /* First two bytes in the TLV value is the ArraySize */
    eRCode = GetWord(pTlvData, &lTemp);
    lArraySize = lTemp;

    if (eQCWWAN_ERR_NONE != eRCode)
        return eRCode;

    if( *(lResp->pArraySizes) != arraySize )
    {
        /* Check for the Buffer Undersize Error */
        if( (lArraySize + *(lResp->pArraySizes)) > arraySize )
        {
            *(lResp->pArraySizes) = lArraySize + *(lResp->pArraySizes);
            return eQCWWAN_ERR_BUFFER_SZ;
        }
        lArrayIndex = *(lResp->pArraySizes);
        *(lResp->pArraySizes) += lArraySize;
    }
    else
    {
        /* Check for the Buffer Undersize Error */
        if ( lArraySize > *(lResp->pArraySizes) )
        {
            *(lResp->pArraySizes) = lArraySize;
            return eQCWWAN_ERR_BUFFER_SZ;
        }
        *(lResp->pArraySizes) = lArraySize;
    }

    /* Fill the Signal Strength and Radio Interface array till lArraySize */
    while ( lArraySize-- )
    {
        /* Copy the Signal Strength and the Radio interface
         * in the current occurance */
        eRCode = GetByte( pTlvData,
                          (BYTE *)&(lResp->pSignalStrengths[lArrayIndex]));
        if (eQCWWAN_ERR_NONE != eRCode)
            return eRCode;

        eRCode = GetByte( pTlvData, &lByte);
        if (eQCWWAN_ERR_NONE == eRCode)
        {
            lResp->pRadioInterfaces[lArrayIndex++] = lByte; 
        }
    }
    return eRCode;
}

/*
 * This function unpacks the GetSignalStrength response message to to a
 * user-provided response structure.
 *
 * \param     MdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp  [OUT] - Pointer to storage to unpack into.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 * \return    eQCWWAN_ERR_MEMORY, on error
 */
enum eQCWWANError UpkQmiNasGetSignalStrengths(
    BYTE    *pMdmResp,
    struct  QmiNasGetSignalStrengthResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,          &qmUnpackTlvResultCode },
        { eTLV_SIGNAL_STRENGTH,      &UnpackTlvSignalStrength },
        { eTLV_SIGNAL_STRENGTH_LIST, &UnpackTlvSignalStrengthList },
        { eTLV_TYPE_INVALID,         NULL }  /* Important. Sentinel.
                                              * Signifies last item in map.
                                              */
    };

    firstTime = 1;
    eRCode = qmunpackresp( pMdmResp,
                          (BYTE*)pApiResp,
                          map,
                          eQMI_NAS_GET_RSSI );
    return eRCode;
}
