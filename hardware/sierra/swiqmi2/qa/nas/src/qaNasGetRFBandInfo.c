/*
 * \ingroup : nas
 *
 * \file    : qaNasGetRFBandInfo.c
 *
 * \brief   : Contains Packing and UnPacking routines for the
 *            eQMI_NAS_GET_RF_INFO message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaQmiBasic.h"

#include "qaNasGetRFBandInfo.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the GetRFInfo parameters
 * to the QMI message SDU
 *
 * \param  pParamField    [OUT] - Pointer to storage into which the packed
 *                                data will be placed by this function.
 *
 * \param  pMlength       [OUT] - Total length of built message.
 *
 * \param  pInstancesSize [IN]  - Size of the Instance Array.
 *
 * \return eQCWWAN_ERR_NONE, on success
 * \return eQCWWAN_ERR_NO_MEMORY, Access beyond allowed size attempted
 *
 *
 */
enum eQCWWANError PkQmiNasGetRFBandInfo(
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
                      eQMI_NAS_GET_RF_INFO,
                      pMlength);
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the RFBandInfoList from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return      eQCWWAN_ERR_NONE, on success
 * \return      eQCWWAN_ERR_NO_MEMORY, Access beyond allowed size attempted
 */
enum eQCWWANError UnpackTlvRFBandInfo( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetRFBandInfoResp *lResp =
              ( struct QmiNasGetRFBandInfoResp *)pResp;
    BYTE lRfBandInfoLen = 0, lArrayIndex = 0, lRadioIf = 0;
    WORD lActiveBand = 0, lActiveChannel = 0;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( lResp->pInstancesSize == NULL ||
         lResp->pInstances     == NULL )
        return eRCode;

    /* Extract the parameters into the structure */
    eRCode  = GetByte( pTlvData, &lRfBandInfoLen );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* Check for the Buffer Undersize Error */
    if ( lRfBandInfoLen > *(lResp->pInstancesSize) )
    {
        *(lResp->pInstancesSize) = lRfBandInfoLen;
        return eQCWWAN_ERR_BUFFER_SZ;
    }

    *(lResp->pInstancesSize) = lRfBandInfoLen;
    /* Fill the RFBandInfo*/
    while ( lRfBandInfoLen )
    {
        /* Copy the RF Info Responses into the array
         * in the current occurance
         */
        eRCode = GetByte( pTlvData, &lRadioIf );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
        lResp->pInstances[lArrayIndex].radioInterface = lRadioIf;

        eRCode = GetWord( pTlvData, &lActiveBand );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
        lResp->pInstances[lArrayIndex].activeBandClass = lActiveBand;

        eRCode = GetWord( pTlvData, &lActiveChannel );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
        lResp->pInstances[lArrayIndex].activeChannel = lActiveChannel;

        lArrayIndex++;
        lRfBandInfoLen--;
    }

    return eRCode;
}


/******************************************************************************
 * Response handling
 *****************************************************************************/
/*
 * This function unpacks the eQMI_NAS_GET_RF_INFO_RESP response
 * message to a user-provided response structure.
 *
 * \param  pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp  [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE      - Increment successful
 * \return eQCWWAN_ERR_NO_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError UpkQmiNasGetRFBandInfo(
    BYTE                           *pMdmResp,
    struct QmiNasGetRFBandInfoResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_RF_BAND_INFO, &UnpackTlvRFBandInfo},
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_NAS_GET_RF_INFO);
    return eRCode;
}

