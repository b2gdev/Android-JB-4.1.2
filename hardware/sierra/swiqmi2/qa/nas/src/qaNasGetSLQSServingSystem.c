/*
 * \ingroup nas
 *
 * \file qaNasGetSLQSServingSystem.c
 *
 * \brief  Contains Packing and UnPacking routines for the
 *         QMI_NAS_GET_SERVING_SYSTEM message for CDMA.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaQmiBasic.h"
#include "qaNasGetSLQSServingSystem.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the SLQSGetServingSystem parameters
 * to the QMI message SDU
 *
 * \param  pParamField      [OUT] - Pointer to storage into which the packed
 *                                  data will be placed by this function.
 *
 * \param  pMlength         [OUT] - Total length of built message.
 *
 * \param  pRadioIfacesSize [IN]  - Radio Interface Array Size.
 *
 * \param  nameSize         [IN]  - Size of the name field.
 *
 * \return eQCWWAN_ERR_NONE, on success
 * \return eQCWWAN_ERR_NO_MEMORY, on error
 *
 */
enum eQCWWANError PkQmiNasGetSLQSServingSystem (
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
                      eQMI_NAS_GET_SS_INFO,
                      pMlength );
    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the Serving System information from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */

enum eQCWWANError UnpackTlvServingSystem( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetSLQSServingSystemResp *lResp =
              (struct QmiNasGetSLQSServingSystemResp *)pResp;
    servSystem *pServSystem = lResp->pServingSystem;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE              Ctr;
    BYTE              lTemp1;

    /* Extract the parameters into the structure */
    /* registration state : Byte */
    eRCode = GetByte( pTlvData, &pServSystem->regState );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* CS Attach State : Byte */
    eRCode = GetByte( pTlvData, &pServSystem->csAttachState );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* ps attach state : Byte */
    eRCode = GetByte( pTlvData, &pServSystem->psAttachState );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* Selected network : Byte */
    eRCode = GetByte( pTlvData, &pServSystem->selNetwork );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* In use radio if list number: Byte */
    eRCode = GetByte( pTlvData, &pServSystem->numRadioInterfaces );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    lTemp1 = pServSystem->numRadioInterfaces;
    for( Ctr = 0; Ctr < lTemp1; Ctr++ )
    {
        /* Radio if: Byte */
        eRCode = GetByte( pTlvData, &pServSystem->radioInterface[Ctr] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function unpacks the Roaming Indicator value from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */

enum eQCWWANError UnpackTlvRoamIndVal( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetSLQSServingSystemResp *lResp =
              (struct QmiNasGetSLQSServingSystemResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the parameters into the structure */
    /* roaming indicator : Byte */
    eRCode = GetByte( pTlvData, lResp->pRoamIndicatorVal );

    return eRCode;
}

/*
 * This function unpacks the Data Service Capability from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */

enum eQCWWANError UnpackTlvDataSrvCapability( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetSLQSServingSystemResp *lResp =
              (struct QmiNasGetSLQSServingSystemResp *)pResp;
    dataSrvCapabilities *pDataSrvCap = lResp->pDataSrvCapabilities;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE              Ctr;
    BYTE              lTemp1;

    /* Extract the parameters into the structure */
    /* Data capability list length: Byte */
    eRCode = GetByte( pTlvData, &pDataSrvCap->dataCapabilitiesLen );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    lTemp1 = pDataSrvCap->dataCapabilitiesLen;
    for( Ctr = 0; Ctr < lTemp1; Ctr++ )
    {
        /* data capabilities: Byte */
        eRCode = GetByte( pTlvData, &pDataSrvCap->dataCapabilities[Ctr] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function unpacks the Current PLMN from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */

enum eQCWWANError UnpackTlvCurrPLMN( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetSLQSServingSystemResp *lResp =
              (struct QmiNasGetSLQSServingSystemResp *)pResp;
    currentPLMN *pCurrPLMN = lResp->pCurrentPLMN;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE              Ctr;
    BYTE              lTemp1;

    /* Extract the parameters into the structure */
    /* mobile country code: Word */
    eRCode = GetWord( pTlvData, &pCurrPLMN->MCC );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* mobile network code: Word */
    eRCode = GetWord( pTlvData, &pCurrPLMN->MNC );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* network description length: Byte */
    eRCode = GetByte( pTlvData, &pCurrPLMN->netDescrLength );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    lTemp1 = pCurrPLMN->netDescrLength;
    for( Ctr = 0; Ctr < lTemp1; Ctr++ )
    {
        /* Network description: Byte */
        eRCode = GetByte( pTlvData, &pCurrPLMN->netDescr[Ctr] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function unpacks the CDMA System ID from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */

enum eQCWWANError UnpackTlvCDMASystemID( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetSLQSServingSystemResp *lResp =
              (struct QmiNasGetSLQSServingSystemResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the parameters into the structure */
    /* System ID : Word */
    eRCode = GetWord( pTlvData,lResp->pSystemID );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* Network ID : Word */
    eRCode = GetWord( pTlvData, lResp->pNetworkID );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return eQCWWAN_ERR_NONE;
}

/*
 * This function unpacks the CDMA BaseStation Information from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]    - Pointer to structure containing storage
 *                             to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvCDMABaseStationInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    struct QmiNasGetSLQSServingSystemResp *lResp =
              (struct QmiNasGetSLQSServingSystemResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the parameters into the structure */
    /* Base Station Id : Word */
    eRCode = GetWord( pTlvData, lResp->pBasestationID );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* Base Station Latitude : ULong */
    eRCode = GetLong( pTlvData, lResp->pBasestationLatitude );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* Base Station Longitude : ULong */
    eRCode = GetLong( pTlvData, lResp->pBasestationLongitude );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return eQCWWAN_ERR_NONE;
}

/*
 * This function unpacks the Roaming Indicator List from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */

enum eQCWWANError UnpackTlvRoamIndList( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetSLQSServingSystemResp *lResp =
              (struct QmiNasGetSLQSServingSystemResp *)pResp;
    roamIndList *pRoamIndList = lResp->pRoamingIndicatorList;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE              Ctr;
    BYTE              lTemp1;

    /* Extract the parameters into the structure */
    /* num Instances: Byte */
    eRCode = GetByte( pTlvData, &pRoamIndList->numInstances );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    lTemp1 = pRoamIndList->numInstances;
    for( Ctr = 0; Ctr < lTemp1; Ctr++ )
    {
        /* radio interface: Byte */
        eRCode = GetByte( pTlvData, &pRoamIndList->radioInterface[Ctr] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        /* roaming indicator: Byte */
        eRCode = GetByte( pTlvData, &pRoamIndList->roamIndicator[Ctr] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function unpacks the Default Roaming Indicator from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */

enum eQCWWANError UnpackTlvDefaultRoamInd( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetSLQSServingSystemResp *lResp =
              (struct QmiNasGetSLQSServingSystemResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the parameters into the structure */
    /* Default Roaming Indicator: Byte */
    eRCode = GetByte( pTlvData, lResp->pDefaultRoamInd );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return eRCode;
}

/*
 * This function unpacks the 3Gpp2TimeZone from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]    - Pointer to structure containing storage
 *                             to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlv3Gpp2TimeZone( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetSLQSServingSystemResp *lResp =
              (struct QmiNasGetSLQSServingSystemResp *)pResp;

    enum               eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    qaQmi3Gpp2TimeZone              *lTemp = lResp->p3Gpp2TimeZone;

    /* Extract the parameters into the structure */
    /* Leap Seconds : Byte */
    eRCode = GetByte( pTlvData, &lTemp->leapSeconds );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* Local Time Offset : Byte */
    eRCode = GetByte( pTlvData, &lTemp->localTimeOffset );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* Day light savings : Byte */
    eRCode = GetByte( pTlvData, &lTemp->daylightSavings );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return eRCode;
}

/*
 * This function unpacks the CDMA P_Rev in use from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]    - Pointer to structure containing storage
 *                             to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvCDMAPRev( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetSLQSServingSystemResp *lResp =
              (struct QmiNasGetSLQSServingSystemResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the parameters into the structure */
    /* P-Rev in use : Byte */
    eRCode = GetByte( pTlvData, lResp->pCDMA_P_Rev );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return eRCode;
}

/*
 * This function unpacks the 3GppTimeZone from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]    - Pointer to structure containing storage
 *                             to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 * \return eQCWWAN_ERR_NO_MEMORY, on error
 */
enum eQCWWANError UnpackTlv3GppTimeZone( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetSLQSServingSystemResp *lResp =
              (struct QmiNasGetSLQSServingSystemResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the parameters into the structure */
    /* 3Gpp Time Zone : Byte */
    eRCode = GetByte( pTlvData, lResp->p3GppTimeZone );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return eRCode;
}

/*
 * This function unpacks the 3GppDSTAdjustment from the QMI response message
 * to a user provided response structure
 *
 * \param  pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]    - Pointer to structure containing storage
 *                             to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 */
enum eQCWWANError UnpackTlv3GppDSTAdjustment( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetSLQSServingSystemResp *lResp =
              (struct QmiNasGetSLQSServingSystemResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the parameters into the structure */
    /* 3Gpp Time Zone adjustment : Byte */
    eRCode =  GetByte( pTlvData, lResp->pGppNetworkDSTAdjustment );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return eRCode;
}

/*
 * This function unpacks the 3GPP Location Area Code from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]    - Pointer to structure containing storage
 *                             to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */

enum eQCWWANError UnpackTlv3GPPLac( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetSLQSServingSystemResp *lResp =
              (struct QmiNasGetSLQSServingSystemResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the parameters into the structure */
    /* 3GPP Location Area Code : Word */
    eRCode = GetWord( pTlvData, lResp->pLac );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return eRCode;
}

/*
 * This function unpacks the 3GPP Cell Id from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]    - Pointer to structure containing storage
 *                             to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlv3GPPCellID( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetSLQSServingSystemResp *lResp =
              (struct QmiNasGetSLQSServingSystemResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the parameters into the structure */
    /* 3GPP Cell ID : Long */
    eRCode = GetLong( pTlvData, lResp->pCellID );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return eRCode;
}

/*
 * This function unpacks the 3GPP2 Concurrent Service Info from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]    - Pointer to structure containing storage
 *                             to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvConcSvcInfo( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetSLQSServingSystemResp *lResp =
              (struct QmiNasGetSLQSServingSystemResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the parameters into the structure */
    /* Concurrent Service Info : Byte */
    eRCode = GetByte( pTlvData, lResp->pConcSvcInfo );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return eRCode;
}

/*
 * This function unpacks the 3GPP2 PRL Indicator from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]    - Pointer to structure containing storage
 *                             to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvPRLIndicator( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetSLQSServingSystemResp *lResp =
              (struct QmiNasGetSLQSServingSystemResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the parameters into the structure */
    /* PRL Indicator : Byte */
    eRCode = GetByte( pTlvData, lResp->pPRLInd );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return eRCode;
}

/*
 * This function unpacks the Dual Transfer Mode Indication from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]    - Pointer to structure containing storage
 *                             to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvDTMIndication( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetSLQSServingSystemResp *lResp =
              (struct QmiNasGetSLQSServingSystemResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the parameters into the structure */
    /* DTM Indication : Byte */
    eRCode = GetByte( pTlvData, lResp->pDTMInd );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return eRCode;
}

/*
 * This function unpacks the Detailed Service Information from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]    - Pointer to structure containing storage
 *                             to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvDetailSvcInfo( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetSLQSServingSystemResp *lResp =
              (struct QmiNasGetSLQSServingSystemResp *)pResp;
    detailSvcInfo *pDetSvcInfo = lResp->pDetailedSvcInfo;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the parameters into the structure */
    /* srv status : Byte */
    eRCode = GetByte( pTlvData, &pDetSvcInfo->srvStatus );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* srv capability : Byte */
    eRCode = GetByte( pTlvData, &pDetSvcInfo->srvCapability );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* HDR service status : Byte */
    eRCode = GetByte( pTlvData, &pDetSvcInfo->hdrSrvStatus );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* HDR hybrid information : Byte */
    eRCode = GetByte( pTlvData, &pDetSvcInfo->hdrHybrid );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* Forbidden system information : Byte */
    eRCode = GetByte( pTlvData, &pDetSvcInfo->isSysForbidden );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return eRCode;
}

/*
 * This function unpacks the CDMA System Info Ext from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]    - Pointer to structure containing storage
 *                             to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvCDMASysInfoExt( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetSLQSServingSystemResp *lResp =
              (struct QmiNasGetSLQSServingSystemResp *)pResp;

    CDMASysInfoExt  *pCDMASysInfo = lResp->pCDMASystemInfoExt;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the parameters into the structure */
    /* MCC : Word */
    eRCode = GetWord( pTlvData, &pCDMASysInfo->MCC );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* IMSI_11_12 : Byte */
    eRCode = GetByte( pTlvData, &pCDMASysInfo->imsi_11_12 );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return eRCode;
}

/*
 * This function unpacks the HDR Personality from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]    - Pointer to structure containing storage
 *                             to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvHDRPersonality( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetSLQSServingSystemResp *lResp =
              (struct QmiNasGetSLQSServingSystemResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the parameters into the structure */
    /* HDR Personality : Byte */
    eRCode = GetByte( pTlvData, lResp->pHdrPersonality );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return eRCode;
}

/*
 * This function unpacks the TAC Information for LTE from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]    - Pointer to structure containing storage
 *                             to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvTACInfoLTE( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetSLQSServingSystemResp *lResp =
              (struct QmiNasGetSLQSServingSystemResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the parameters into the structure */
    /* Tracking Area code : Word */
    eRCode = GetWord( pTlvData, lResp->pTrackAreaCode );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return eRCode;
}

/*
 * This function unpacks the Call Barring Status from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]    - Pointer to structure containing storage
 *                             to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvCallBarringStatus( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetSLQSServingSystemResp *lResp =
              (struct QmiNasGetSLQSServingSystemResp *)pResp;
    callBarStatus *CBStat = lResp->pCallBarStatus;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the parameters into the structure */
    /* CS call bar status : ULong */
    eRCode = GetLong( pTlvData, &CBStat->csBarStatus );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* PS call bar status : ULong */
    eRCode = GetLong( pTlvData, &CBStat->psBarStatus );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return eRCode;
}

/*
 * This function unpacks the SLQSGetServingSystem response message to a
 * user-provided response structure.
 *
 * \param  MdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp  [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success
 * \return eQCWWAN_ERR_NO_MEMORY, on error
 */
enum eQCWWANError UpkQmiNasGetSLQSServingSystem(
    BYTE                                  *pMdmResp,
    struct QmiNasGetSLQSServingSystemResp *pApiResp)
{
    enum eQCWWANError rCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,                   &qmUnpackTlvResultCode },
        { eTLV_SERV_SYSTEM,                   &UnpackTlvServingSystem },
        { eTLV_ROAMING_IND_VALUE,             &UnpackTlvRoamIndVal },
        { eTLV_DATA_SRV_CAPABILITY,           &UnpackTlvDataSrvCapability },
        { eTLV_CURR_PLMN,                     &UnpackTlvCurrPLMN },
        { eTLV_CDMA_SYSTEM_ID,                &UnpackTlvCDMASystemID },
        { eTLV_CDMA_BASE_STATION_INFORMATION, &UnpackTlvCDMABaseStationInfo },
        { eTLV_ROAMING_IND_LIST,              &UnpackTlvRoamIndList },
        { eTLV_DEFAULT_ROAMING_INDICATOR,     &UnpackTlvDefaultRoamInd },
        { eTLV_3GPP2_TIME_ZONE,               &UnpackTlv3Gpp2TimeZone },
        { eTLV_CDMA_P_REV_IN_USE,             &UnpackTlvCDMAPRev },
        { eTLV_3GPP_TIME_ZONE,                &UnpackTlv3GppTimeZone },
        { eTLV_3GPP2_DST_ADJUSTMENT,          &UnpackTlv3GppDSTAdjustment },
        { eTLV_3GPP_LAC,                      &UnpackTlv3GPPLac },
        { eTLV_3GPP_CELL_ID,                  &UnpackTlv3GPPCellID },
        { eTLV_CONCURRENT_SVC_INFO,           &UnpackTlvConcSvcInfo },
        { eTLV_PRL_INDICATOR,                 &UnpackTlvPRLIndicator },
        { eTLV_DTM_INDICATION,                &UnpackTlvDTMIndication },
        { eTLV_DETAILED_SVC_INFO,             &UnpackTlvDetailSvcInfo },
        { eTLV_CDMA_SYSTEM_INFO_EXT,          &UnpackTlvCDMASysInfoExt },
        { eTLV_HDR_PERSONALITY,               &UnpackTlvHDRPersonality },
        { eTLV_TAC_INFO_LTE,                  &UnpackTlvTACInfoLTE },
        { eTLV_CALL_BARRING_STATUS,           &UnpackTlvCallBarringStatus },
        { eTLV_TYPE_INVALID,                  NULL }  /* Important. Sentinel.
                                                       * Signifies last
                                                       * item in map.
                                                       */
    };
    rCode = qmunpackresp( pMdmResp,
                          (BYTE *)pApiResp,
                          map,
                          eQMI_NAS_GET_SS_INFO );
    return rCode;
}
