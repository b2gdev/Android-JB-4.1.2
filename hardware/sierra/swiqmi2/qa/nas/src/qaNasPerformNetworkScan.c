/*
 * \ingroup nas
 *
 * \file qaNasPerformNetworkScan.c
 *
 * \brief  Contains Packing and UnPacking routines for the 
 *         QMI_NAS_PERFORM_NETWORK_SCAN message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaNasPerformNetworkScan.h"

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
 * \param  pInstanceSize [IN] - Size of the Instance Array.
 *
 * \return eQCWWAN_ERR_NONE, on success
 * \return eQCWWAN_ERR_MEMORY, on error
 *
 */
enum eQCWWANError PkQmiNasPerformNetworkScan(
    WORD     *pMlength, 
    BYTE     *pParamField )
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
enum eQCWWANError UnpackTlv3GppNetworkInformation(
    BYTE    *pTlvData,
    BYTE    *pResp )
{ 
   struct QmiNasPerformNetworkScanResp *lResp =
              (struct QmiNasPerformNetworkScanResp *)pResp;
    
    /* To store the instances statically */
    USHORT lInstanceSize;
    BYTE lNetworkStatus, lNtwkDesclen;
    BYTE lArrayIndex = 0, lDescIdx = 0;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !lResp->pInstanceSize || !lResp->pInstances )
        return eRCode;

    /* First two bytes in the TLV value is the ArraySize */ 
    eRCode = GetWord(pTlvData, &lInstanceSize );
    if (eQCWWAN_ERR_NONE != eRCode)
    {
        return eRCode;
    }

    /* Check for the Buffer Undersize Error */
    if ( lInstanceSize > *(lResp->pInstanceSize) )
    {
        *(lResp->pInstanceSize) = lInstanceSize;
        return eQCWWAN_ERR_BUFFER_SZ;
    }

    *(lResp->pInstanceSize) = lInstanceSize;
    /* Fill the Network Scan array till lInstanceSize */
    while ( lInstanceSize-- )
    {
        /*
         *  Copy the Network Scan responses into the array 
         *  in the current occurance
         */
        eRCode = GetWord ( pTlvData,
                           (USHORT *)&lResp->pInstances[lArrayIndex].pMCC );
        if (eQCWWAN_ERR_NONE != eRCode)
        {
            return eRCode;
        }
        eRCode = GetWord( pTlvData,
                          (USHORT *)&lResp->pInstances[lArrayIndex].pMNC );
        if (eQCWWAN_ERR_NONE != eRCode)
        {
            return eRCode;
        }

        /*
         * The Network status contains Roaming/Forbidden/CurrentServing Status 
         * Refer to the Network Status Parameter in QMI_NAS_PERFORM_NETWORK_SCAN 
         * The network status parameter in the TLV is bitmasked for values 
         */ 
        eRCode = GetByte(pTlvData, (BYTE *)&lNetworkStatus);
        if (eQCWWAN_ERR_NONE != eRCode)
        {
            return eRCode;
        }
        
        /* Parameter for current serving Network */
        lResp->pInstances[lArrayIndex].pInUse = 
                lNetworkStatus & QMI_NAS_NETSTATUS_MASK;

        /* Parameter for Roaming Network */
        lResp->pInstances[lArrayIndex].pRoaming =
                ( lNetworkStatus >> ROAMING_INDEX ) & QMI_NAS_NETSTATUS_MASK;

        /* parameter for Forbidden Network */
        lResp->pInstances[lArrayIndex].pForbidden =
                ( lNetworkStatus >> FORBIDDEN_INDEX ) & QMI_NAS_NETSTATUS_MASK;

        /* Parameter for Preferred Network */
        lResp->pInstances[lArrayIndex].pPreferred =
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
             (BYTE *)&lResp->pInstances[lArrayIndex].pDesription[lDescIdx++]);
                                                       
            if (eQCWWAN_ERR_NONE != eRCode)
            {
                return eRCode;
            }
        }
        lResp->pInstances[lArrayIndex].pDesription[lDescIdx] = EOS;

        /* Arrange the Indexes appropriately for the corresponding array */ 
        lDescIdx = INDEX_ZERO;
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
enum eQCWWANError UpkQmiNasPerformNetworkScan(
    BYTE    *pMdmResp,
    struct  QmiNasPerformNetworkScanResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,             &qmUnpackTlvResultCode },
        { eTLV_3GPP_NETWORK_INFO,       &UnpackTlv3GppNetworkInformation },
        { eTLV_TYPE_INVALID,            NULL }  /* Important. Sentinel. 
                                                 * Signifies last item in map.
                                                 */
    };
    eRCode = qmunpackresp( pMdmResp,
                          (BYTE*)pApiResp,
                          map,
                          eQMI_NAS_SCAN_NETS );
    return eRCode;
}
