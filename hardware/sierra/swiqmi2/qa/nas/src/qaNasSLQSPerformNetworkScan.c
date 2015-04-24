/*
 * \ingroup nas
 *
 * \file qaNasSLQSPerformNetworkScan.c
 *
 * \brief  Contains Packing and UnPacking routines for the
 *         NAS_SLQS_PERFORM_NETWORK_SCAN_RESP message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaGobiApiNas.h"
#include "qaNasSLQSPerformNetworkScan.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the PerformNetworkScan parameters
 * to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError PkQmiNasSlqsPerformNetworkScan(
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
                      eQMI_NAS_SCAN_NETS,
                      pMlength);
    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the 3GppNetworkInfo from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlv3GppNetworkInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    slqsNetworkScanInfo *lResp =
            ((struct QmiNasSlqsPerformNetworkScanResp *)pResp)->pNetworkInfo;

    /* To store the instances statically */
    WORD              lInstanceSize;
    BYTE              lNetworkStatus, lNtwkDesclen;
    BYTE              lArrayIndex = 0, lDescIdx = 0;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !lResp->pNetworkInfoInstances ||
         !lResp->pNetworkInfo )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* First two bytes in the TLV value is the ArraySize */
    eRCode = GetWord(pTlvData, &lInstanceSize );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* Check for the Buffer Undersize Error */
    if ( lInstanceSize > *( lResp->pNetworkInfoInstances ) )
    {
        *( lResp->pNetworkInfoInstances ) = lInstanceSize;
        return eQCWWAN_ERR_BUFFER_SZ;
    }

    /* Check for value, if zero exit */
    if ( NO_NW_INSTANCE == lInstanceSize )
    {
        /* Set Network Info Instances and RAT Instances to NO_NW_INSTANCE */
        *( lResp->pNetworkInfoInstances ) = NO_NW_INSTANCE;

        /* Set RAT Instances only if NOT NULL */
        if ( lResp->pRATInstances != NULL )
        {
            *( lResp->pRATInstances ) = NO_NW_INSTANCE;
        }
        return eQCWWAN_ERR_NONE;
    }

    *( lResp->pNetworkInfoInstances ) = lInstanceSize;

    /* Fill the Network Scan array till lInstanceSize */
    while ( lInstanceSize-- )
    {
        /* Copy the Network Scan responses into the list */
        eRCode = GetWord ( pTlvData , &lResp->pNetworkInfo[lArrayIndex].MCC );
        if (eQCWWAN_ERR_NONE != eRCode)
        {
            return eRCode;
        }
        eRCode = GetWord( pTlvData , &lResp->pNetworkInfo[lArrayIndex].MNC );
        if (eQCWWAN_ERR_NONE != eRCode)
        {
            return eRCode;
        }

        /* The Network status contains Roaming/Forbidden/CurrentServing Status */
        eRCode = GetByte(pTlvData, &lNetworkStatus);
        if (eQCWWAN_ERR_NONE != eRCode)
        {
            return eRCode;
        }

        /* Parameter for current serving Network */
        lResp->pNetworkInfo[lArrayIndex].InUse =
                lNetworkStatus & QMI_NAS_NETSTATUS_MASK;

        /* Parameter for Roaming Network */
        lResp->pNetworkInfo[lArrayIndex].Roaming =
                ( lNetworkStatus >> ROAMING_INDEX ) & QMI_NAS_NETSTATUS_MASK;

        /* Parameter for Forbidden Network */
        lResp->pNetworkInfo[lArrayIndex].Forbidden =
                ( lNetworkStatus >> FORBIDDEN_INDEX ) & QMI_NAS_NETSTATUS_MASK;

        /* Parameter for Preferred Network */
        lResp->pNetworkInfo[lArrayIndex].Preferred =
                ( lNetworkStatus >> PREFERRED_INDEX ) & QMI_NAS_NETSTATUS_MASK;

        /* Get the Network Description into the Instance array */
        eRCode  = GetByte(pTlvData, &lNtwkDesclen);
        if (eQCWWAN_ERR_NONE != eRCode)
        {
            return eRCode;
        }

        while ( lNtwkDesclen-- )
        {
             eRCode = GetByte( pTlvData,
             (BYTE *)&lResp->pNetworkInfo[lArrayIndex].Description[lDescIdx++]);

            if (eQCWWAN_ERR_NONE != eRCode)
            {
                return eRCode;
            }
        }
        lResp->pNetworkInfo[lArrayIndex].Description[lDescIdx] = EOS;

        /* Arrange the Indexes appropriately for the corresponding array */
        lDescIdx = INDEX_ZERO;
        lArrayIndex++;
    }

    return eRCode;
}

/*
 * This function unpacks the 3GppNetworkInformation from the
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
enum eQCWWANError UnpackTlv3GppNetworkRATInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    slqsNetworkScanInfo *lResp =
              ((struct QmiNasSlqsPerformNetworkScanResp *)pResp)->pNetworkInfo;

    /* To store the instances statically */
    WORD              lRATInstanceSize;
    BYTE              lArrayIndex = 0;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !lResp->pRATInstances ||
         !lResp->pRATInfo )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* These two bytes in the TLV value is the ArraySize for RAT */
    eRCode = GetWord( pTlvData, &lRATInstanceSize );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
       return eRCode;
    }

    /* Check for the Buffer Undersize Error */
    if ( lRATInstanceSize > *( lResp->pRATInstances ) )
    {
        *( lResp->pRATInstances ) = lRATInstanceSize;
        return eQCWWAN_ERR_BUFFER_SZ;
    }

    /* Check for value, if zero exit */
    if ( NO_NW_INSTANCE == lRATInstanceSize )
    {
        /* Set RAT Instances to value received */
        *( lResp->pRATInstances ) = lRATInstanceSize;
        return eQCWWAN_ERR_NONE;
    }

    *( lResp->pRATInstances ) = lRATInstanceSize;

    while ( lRATInstanceSize-- )
    {
        /* Copy the Network Scan RAT responses into the list */
        eRCode = GetWord ( pTlvData , &lResp->pRATInfo[lArrayIndex].MCC );
        if (eQCWWAN_ERR_NONE != eRCode)
        {
            return eRCode;
        }

        eRCode = GetWord( pTlvData , &lResp->pRATInfo[lArrayIndex].MNC );
        if (eQCWWAN_ERR_NONE != eRCode)
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData , &lResp->pRATInfo[lArrayIndex].RAT );
        if (eQCWWAN_ERR_NONE != eRCode)
        {
            return eRCode;
        }

        lArrayIndex++;
    }

    return eRCode;
}

/*
 * This function unpacks the PerfromNetworkScan response message to to a
 * user-provided response structure.
 *
 * \param     pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp  [OUT] - Pointer to storage to unpack into.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 * \return    eQCWWAN_ERR_MEMORY, on error
 */
enum eQCWWANError UpkQmiNasSlqsPerformNetworkScan(
    BYTE                                    *pMdmResp,
    struct QmiNasSlqsPerformNetworkScanResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,              &qmUnpackTlvResultCode },
        { eTLV_3GPP_NETWORK_INFORMATION, &UnpackTlv3GppNetworkInfo },
        { eTLV_3GPP_NETWORK_RAT,         &UnpackTlv3GppNetworkRATInfo },
        { eTLV_TYPE_INVALID,             NULL }  /* Important. Sentinel.
                                                 * Signifies last item in map.
                                                 */
    };
    eRCode = qmunpackresp( pMdmResp,
                          (BYTE*)pApiResp,
                          map,
                          eQMI_NAS_SCAN_NETS );
    return eRCode;
}
