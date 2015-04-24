/*
 * \ingroup wds
 *
 * \file qaWdsSLQSGetRuntimeSettings.c
 *
 * \brief  Contains Packing and UnPacking routines for the
 *         QMI_WDS_SLQS_GET_RUNTIME_SETTINGS message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc., all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaGobiApiWds.h"
#include "qaWdsSLQSGetRuntimeSettings.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the SLQSGetRuntimeSettings Requested Settings field to
 * the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvSLQSRequestedSettings(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsSLQSGetRuntimeSettingsReq *pReq =
                (struct QmiWdsSLQSGetRuntimeSettingsReq *)pParam;

    if ( pReq->pRequestedSettings == NULL )
        return eQCWWAN_ERR_NONE;

    /* Add TLV data */
    return PutLong( pBuf, *(pReq->pRequestedSettings) );
}

/*
 * This function packs the SLQSGetRuntimeSettings parameters
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
 */
enum eQCWWANError PkQmiWdsSLQSGetRuntimeSettings(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG *pRequestedSettings)
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SLQS_REQUESTED_SETTINGS, &BuildTlvSLQSRequestedSettings },
        { eTLV_TYPE_INVALID,            NULL }  /* Important. Sentinel.
                                                 * Signifies last item in map.
                                                 */
    };

    struct QmiWdsSLQSGetRuntimeSettingsReq req;
    enum eQCWWANError                      eRCode;

    slmemset((char *)&req, 0, sizeof (struct QmiWdsSLQSGetRuntimeSettingsReq));
    req.pRequestedSettings = pRequestedSettings;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_WDS_GET_SETTINGS,
                      pMlength );
    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the SLQSGetRuntimeSettings Profile Name from the
 * QMI response message to a user provided response structure
 *
 * \param     pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param     pResp    [OUT]     - Pointer to structure containing storage
 *                                 to return data for this TLV.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvSLQSProfileName( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSLQSGetRuntimeSettingsResp *pLresp =
                        (struct QmiWdsSLQSGetRuntimeSettingsResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    swi_uint16        tlvLength;
   
    if (!pLresp->WdsSlqsRunTimeSettings.pProfileName)
            return eRCode;

    /* Get the Length of string as the string might not be NULL terminated */
    eRCode = GetStringLen( (swi_uint8 *)pTlvData, &tlvLength );
    if( !eRCode )
    {
        /* Extract the String Parameter */
        eRCode = qmQmiExtractString( pTlvData,
                                     pLresp->WdsSlqsRunTimeSettings.
                                     pProfileName,
                                     tlvLength + 1 );
    }
    return eRCode;
}

/*
 * This function unpacks the SLQSGetRuntimeSettings PDPType from the
 * QMI response message to a user provided response structure
 *
 * \param     pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param     pResp    [OUT]     - Pointer to structure containing storage
 *                                 to return data for this TLV.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvSLQSPDPType( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSLQSGetRuntimeSettingsResp *pLresp =
                        (struct QmiWdsSLQSGetRuntimeSettingsResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE              lTemp;

    if (!pLresp->WdsSlqsRunTimeSettings.pPDPType)
        return eRCode;

    /* Extract the PDPType */
    eRCode = GetByte( pTlvData, &lTemp );
    if(eQCWWAN_ERR_NONE == eRCode)
        *(pLresp->WdsSlqsRunTimeSettings.pPDPType) = lTemp;

    return eRCode;
}

/*
 * This function unpacks the SLQSGetRuntimeSettings APN Name from the
 * QMI response message to a user provided response structure
 *
 * \param     pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param     pResp    [OUT]     - Pointer to structure containing storage
 *                               to return data for this TLV.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvSLQSApnName( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSLQSGetRuntimeSettingsResp *pLresp =
                        (struct QmiWdsSLQSGetRuntimeSettingsResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    swi_uint16        tlvLength;

    if (!pLresp->WdsSlqsRunTimeSettings.pAPNName)
            return eRCode;

    /* Get the Length of string as the string might not be NULL terminated */
    eRCode = GetStringLen( (swi_uint8 *)pTlvData, &tlvLength );
    if( !eRCode )
    {
        /* Extract the String Parameter */
        eRCode = qmQmiExtractString( pTlvData,
                                     pLresp->WdsSlqsRunTimeSettings.pAPNName,
                                     tlvLength + 1 );
    }
    return eRCode;
}

/*
 * This function unpacks the SLQSSLQSGetRuntimeSettings Primary DNS IPV4 from
 * the QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvSLQSPrimDNSV4(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsSLQSGetRuntimeSettingsResp *pLresp =
                        (struct QmiWdsSLQSGetRuntimeSettingsResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pLresp->WdsSlqsRunTimeSettings.pPrimaryDNSV4 )
        return eRCode;

    /* Extract the primary DNS address */
    eRCode = GetLong( pTlvData, pLresp->WdsSlqsRunTimeSettings.pPrimaryDNSV4 );

    return eRCode;
}

/*
 * This function unpacks the SLQSSLQSGetRuntimeSettings Secondary DNS IPV4 from
 * the QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvSLQSSecondDNSV4(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsSLQSGetRuntimeSettingsResp *pLresp =
                        (struct QmiWdsSLQSGetRuntimeSettingsResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pLresp->WdsSlqsRunTimeSettings.pSecondaryDNSV4 )
        return eRCode;

    /* Extract the Secondary DNS Address */
    eRCode = GetLong( pTlvData,
                      pLresp->WdsSlqsRunTimeSettings.pSecondaryDNSV4 );

    return eRCode;
}

/*
 * This function unpacks the SLQSSLQSGetRuntimeSettings UMTS Granted QOS
 * from the QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvSLQSUMTSGrantedQos (BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsSLQSGetRuntimeSettingsResp *pLresp =
                        (struct QmiWdsSLQSGetRuntimeSettingsResp *)pResp;
    struct UMTSQoS *pUMTSQoS = 
             (struct UMTSQoS *)pLresp->WdsSlqsRunTimeSettings.pUMTSGrantedQoS;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pUMTSQoS )
        return eRCode;

    /* Extract Traffic class */
    eRCode = GetByte( pTlvData, &(pUMTSQoS->trafficClass) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract Max uplink bit rate */
    eRCode = GetLong( pTlvData, &(pUMTSQoS->maxUplinkBitrate) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract Max downlink bit rate */
    eRCode = GetLong( pTlvData, &(pUMTSQoS->maxDownlinkBitrate) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract Granted uplink bit rate */
    eRCode = GetLong( pTlvData, &(pUMTSQoS->grntUplinkBitrate) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract Granted downlink bit rate */
    eRCode = GetLong( pTlvData, &(pUMTSQoS->grntDownlinkBitrate) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract Qos delievery order */
    eRCode = GetByte( pTlvData, &(pUMTSQoS->qosDeliveryOrder) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the Max SDU size */
    eRCode = GetLong( pTlvData, &(pUMTSQoS->maxSDUSize) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

   /* Extract the SDU error ratio */
    eRCode = GetByte( pTlvData, &(pUMTSQoS->sduErrorRatio) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the Residual bit error ratio */
    eRCode = GetByte( pTlvData, &(pUMTSQoS->resBerRatio) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract Delievery of errorneous SDU */
    eRCode = GetByte( pTlvData, &(pUMTSQoS->deliveryErrSDU) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract transfer delay */
    eRCode = GetLong( pTlvData, &(pUMTSQoS->transferDelay) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract Traffic priority */
    eRCode = GetLong( pTlvData, &(pUMTSQoS->trafficPriority) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    return eRCode;
}

/*
 * This function unpacks the SLQSSLQSGetRuntimeSettings GPRS Granted QOS
 * from the QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvSLQSGPRSGrantedQos(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsSLQSGetRuntimeSettingsResp *pLresp =
                        (struct QmiWdsSLQSGetRuntimeSettingsResp *)pResp;
    struct GPRSQoS *pGPRSQoS = 
            (struct GPRSQoS *)pLresp->WdsSlqsRunTimeSettings.pGPRSGrantedQoS;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pGPRSQoS )
        return eRCode;

    /* Extract Precedence class */
    eRCode = GetLong( pTlvData, &(pGPRSQoS->precedenceClass) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract Delay class */
    eRCode = GetLong( pTlvData, &(pGPRSQoS->delayClass) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract Reliability class */
    eRCode = GetLong( pTlvData, &(pGPRSQoS->reliabilityClass) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract Peak throughput class */
    eRCode = GetLong( pTlvData, &(pGPRSQoS->peakThroughputClass) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract Mean throughput class */
    eRCode = GetLong( pTlvData, &(pGPRSQoS->meanThroughputClass) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    return eRCode;
}

/*
 * This function unpacks the SLQSGetRuntimeSettings User Name from the
 * QMI response message to a user provided response structure
 *
 * \param     pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param     pResp    [OUT]     - Pointer to structure containing storage
 *                                 to return data for this TLV.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvSLQSUserName( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSLQSGetRuntimeSettingsResp *pLresp =
                        (struct QmiWdsSLQSGetRuntimeSettingsResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;  
    swi_uint16        tlvLength;

    if (!pLresp->WdsSlqsRunTimeSettings.pUsername)
        return eRCode;

    /* Get the Length of string as the string might not be NULL terminated */
    eRCode = GetStringLen( (swi_uint8 *)pTlvData, &tlvLength );
    if( !eRCode )
    {
        /* Extract the String Parameter */
        eRCode = qmQmiExtractString( pTlvData,
                                     pLresp->WdsSlqsRunTimeSettings.pUsername,
                                     tlvLength + 1 );
    }
    return eRCode;
}

/*
 * This function unpacks the SLQSGetRuntimeSettings Authentication from the
 * QMI response message to a user provided response structure
 *
 * \param     pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param     pResp    [OUT]     - Pointer to structure containing storage
 *                                 to return data for this TLV.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvSLQSAuthentication( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSLQSGetRuntimeSettingsResp *pLresp =
                        (struct QmiWdsSLQSGetRuntimeSettingsResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE              lTemp;

    if (!pLresp->WdsSlqsRunTimeSettings.pAuthentication)
        return eRCode;

    /* Extract the Authentication type */
    eRCode = GetByte( pTlvData, &lTemp );
    if( eQCWWAN_ERR_NONE == eRCode )
        *(pLresp->WdsSlqsRunTimeSettings.pAuthentication) = lTemp;

    return eRCode;
}

/*
 * This function unpacks the SLQSGetRuntimeSettings IPV4 Address from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvSLQSIPAddressV4( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSLQSGetRuntimeSettingsResp *pLresp =
                        (struct QmiWdsSLQSGetRuntimeSettingsResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (!pLresp->WdsSlqsRunTimeSettings.pIPAddressV4)
        return eRCode;

    /* Extract IPV4 address */
    eRCode = GetLong( pTlvData, pLresp->WdsSlqsRunTimeSettings.pIPAddressV4 );

    return eRCode;
}

/*
 * This function unpacks the SLQSGetRuntimeSettings Profile Information from
 * the QMI response message to a user provided response structure
 *
 * \param     pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param     pResp    [OUT]     - Pointer to structure containing storage
 *                                 to return data for this TLV.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvSLQSProfileid( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSLQSGetRuntimeSettingsResp *pLresp =
                        (struct QmiWdsSLQSGetRuntimeSettingsResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE              lTemp;

    if ( !pLresp->WdsSlqsRunTimeSettings.pProfileID )
        return eRCode;

    /* Extract the Profile type */
    eRCode = GetByte( pTlvData, &lTemp );
    if ( eQCWWAN_ERR_NONE == eRCode )
    {
        pLresp->WdsSlqsRunTimeSettings.pProfileID->profileType = lTemp;
 
        /* Extract the Profile id */
        eRCode = GetByte( pTlvData, &lTemp );
        if ( eQCWWAN_ERR_NONE == eRCode )
            pLresp->WdsSlqsRunTimeSettings.pProfileID->profileIndex = lTemp;
    }
    return eRCode;
}

/*
 * This function unpacks the SLQSGetRuntimeSettings IPV4 Gateway Address from
 * the QMI response message to a user provided response structure
 *
 * \param     pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param     pResp    [OUT]     - Pointer to structure containing storage
 *                                 to return data for this TLV.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvSLQSGatewayV4( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSLQSGetRuntimeSettingsResp *pLresp =
                        (struct QmiWdsSLQSGetRuntimeSettingsResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (!pLresp->WdsSlqsRunTimeSettings.pGWAddressV4)
        return eRCode;

    /* Extract IPV4 Gateway address */
    eRCode = GetLong( pTlvData, pLresp->WdsSlqsRunTimeSettings.pGWAddressV4 );

    return eRCode;
}

/*
 * This function unpacks the SLQSGetRuntimeSettings Subnet Mask from the
 * QMI response message to a user provided response structure
 *
 * \param     pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param     pResp    [OUT]     - Pointer to structure containing storage
 *                                 to return data for this TLV.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvSLQSMaskV4( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSLQSGetRuntimeSettingsResp *pLresp =
                        (struct QmiWdsSLQSGetRuntimeSettingsResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (!pLresp->WdsSlqsRunTimeSettings.pSubnetMaskV4)
        return eRCode;

    /* Extract IPV4 Subnet mask */
    eRCode = GetLong( pTlvData, pLresp->WdsSlqsRunTimeSettings.pSubnetMaskV4 );

    return eRCode;
}

/*
 * This function unpacks the SLQSGetRuntimeSettings PCSCFAddrPCO from the
 * QMI response message to a user provided response structure
 *
 * \param     pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param     pResp    [OUT]     - Pointer to structure containing storage
 *                                 to return data for this TLV.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvSLQSPCSCFAddrPCO( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSLQSGetRuntimeSettingsResp *pLresp =
                        (struct QmiWdsSLQSGetRuntimeSettingsResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (!pLresp->WdsSlqsRunTimeSettings.pPCSCFAddrPCO)
        return eRCode;

    /* Extract PCSF address using PCO values */
    eRCode = GetByte( pTlvData, pLresp->WdsSlqsRunTimeSettings.pPCSCFAddrPCO );

    return eRCode;
}

/*
 * This function unpacks the SLQSSLQSGetRuntimeSettings PCSCF IPV4 Server
 * Address List from the QMI response message to a user provided response
 * structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvSLQSServerAddrList(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsSLQSGetRuntimeSettingsResp *pLresp =
                        (struct QmiWdsSLQSGetRuntimeSettingsResp *)pResp;
    enum  eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE               lServerListLen = 0;
    ULONG              *pBuffServerAddr = NULL;

    if ( !pLresp->WdsSlqsRunTimeSettings.pServerAddrList )
        return eRCode;

    /* Extract the no. instances of PCSF IPV4 Server address */
    eRCode  = GetByte( pTlvData, &lServerListLen );
    if ( eQCWWAN_ERR_NONE == eRCode )
    {
        pLresp->WdsSlqsRunTimeSettings.pServerAddrList->numInstances =
                                                              lServerListLen;

        /* get the server address list buffer to local pointer */
        pBuffServerAddr = pLresp->WdsSlqsRunTimeSettings.pServerAddrList->
                          pscsfIPv4Addr;

        while ( lServerListLen )
        {
            /* Extract the PCSCFIPv4Addr */
            eRCode  = GetLong( pTlvData, pBuffServerAddr );
            if ( eQCWWAN_ERR_NONE != eRCode )
                return eRCode;

            pBuffServerAddr++;
            lServerListLen--;
        }
    }
    return eRCode;
}

/*
 * This function unpacks the SLQSSLQSGetRuntimeSettings PCSCF-FQDN Address List
 * from the QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvSLQSPCSCFFQDNAddrList(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsSLQSGetRuntimeSettingsResp *pLresp =
                        (struct QmiWdsSLQSGetRuntimeSettingsResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE              lAddrListLen = 0, lArrayIndex = 0;
    WORD              FqdnLen = 0;
    CHAR              *pFQDNAddress = NULL;

    if ( !pLresp->WdsSlqsRunTimeSettings.pPCSCFFQDNAddrList )
        return eRCode;

    /* Extract the no. of instances of FQDN Addresses */
    eRCode  = GetByte( pTlvData, &lAddrListLen );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    pLresp->WdsSlqsRunTimeSettings.pPCSCFFQDNAddrList->numInstances =
                                                                lAddrListLen;
    while ( lAddrListLen )
    {
        /* Extract the PCSCFIPv4Addr length */
        eRCode  = GetWord( pTlvData, &FqdnLen );
        if ( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;

        pLresp->WdsSlqsRunTimeSettings.pPCSCFFQDNAddrList->
        pcsfFQDNAddress[lArrayIndex].fqdnLen = FqdnLen;

        /* get the buffer address in local pointer */
        pFQDNAddress = pLresp->WdsSlqsRunTimeSettings.
                       pPCSCFFQDNAddrList->
                       pcsfFQDNAddress[lArrayIndex].fqdnAddr;

        /* Extract the FQDN Address */
        eRCode  = qmQmiExtractString( pTlvData,
                                      pFQDNAddress,
                                      FqdnLen + 1 );

        if ( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;

        lArrayIndex++;
        lAddrListLen--;
    }
    return eRCode;
}

/*
 * This function unpacks the SLQSSLQSGetRuntimeSettings Primary DNS IPV6 from
 * the QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvSLQSPrimDNSV6(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsSLQSGetRuntimeSettingsResp *pLresp =
                        (struct QmiWdsSLQSGetRuntimeSettingsResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    USHORT            idx = 0;

    if ( !pLresp->WdsSlqsRunTimeSettings.pPrimaryDNSV6 )
        return eRCode;

    /* Extract the IPV6 Primary DNS */
    for( idx = 0; idx < 8 ; idx++ )
        eRCode = GetWordBe( pTlvData,
                            (pLresp->WdsSlqsRunTimeSettings.
                             pPrimaryDNSV6)++ );

    return eRCode;
}

/*
 * This function unpacks the SLQSSLQSGetRuntimeSettings Secondary DNS IPV6 from
 * the QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvSLQSSecondDNSV6(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsSLQSGetRuntimeSettingsResp *pLresp =
                        (struct QmiWdsSLQSGetRuntimeSettingsResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    USHORT            idx = 0;

    if ( !pLresp->WdsSlqsRunTimeSettings.pSecondaryDNSV6 )
        return eRCode;

    /* Extract the IPV6 Secondary DNS */
    for( idx = 0; idx < 8 ; idx++ )
        eRCode = GetWordBe( pTlvData,
                            (pLresp->WdsSlqsRunTimeSettings.
                             pSecondaryDNSV6)++ );

    return eRCode;
}

/*
 * This function unpacks the SLQSGetRuntimeSettings MTU from the
 * QMI response message to a user provided response structure
 *
 * \param     pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param     pResp    [OUT]     - Pointer to structure containing storage
 *                                 to return data for this TLV.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvSLQSMtu( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSLQSGetRuntimeSettingsResp *pLresp =
                        (struct QmiWdsSLQSGetRuntimeSettingsResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pLresp->WdsSlqsRunTimeSettings.pMtu )
        return eRCode;

    /* Extract the MTU */
    eRCode = GetLong( pTlvData, pLresp->WdsSlqsRunTimeSettings.pMtu );

    return eRCode;
}

/*
 * This function unpacks the SLQSSLQSGetRuntimeSettings DomainNameList
 * from the QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvSLQSDomainNameList(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsSLQSGetRuntimeSettingsResp *pLresp =
                        (struct QmiWdsSLQSGetRuntimeSettingsResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE              lDomainListLen = 0, lArrayIndex = 0;
    WORD              domainNamelen = 0;
    CHAR              *pDomainName = NULL;

    if ( !pLresp->WdsSlqsRunTimeSettings.pDomainList )
        return eRCode;

    /* Extract the no. of domain address instances */
    eRCode  = GetByte( pTlvData, &lDomainListLen );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    pLresp->WdsSlqsRunTimeSettings.pDomainList->numInstances = lDomainListLen;
    while ( lDomainListLen )
    {
        /* Extract the domain address length */
        eRCode  = GetWord( pTlvData, &domainNamelen );
        if ( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;

        /* copy the domain name length into structure */
        pLresp->WdsSlqsRunTimeSettings.pDomainList->
        domain[lArrayIndex].domainLen = domainNamelen ;

        /* get the buffer address in local pointer */
        pDomainName = pLresp->WdsSlqsRunTimeSettings.pDomainList->
                       domain[lArrayIndex].domainName;

        /* if domain name exist */
        if ( 0 != domainNamelen ) 
        {
            /* Extract the doamin name string */
            eRCode  = qmQmiExtractString( pTlvData,
                                          pDomainName,
                                          domainNamelen + 1 );

            if ( eQCWWAN_ERR_NONE != eRCode )
                return eRCode;
        }                

        lArrayIndex++;
        lDomainListLen--;
    }
    return eRCode;
}

/*
 * This function unpacks the SLQSGetRuntimeSettings IP Family Preference from
 * the QMI response message to a user provided response structure
 *
 * \param     pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param     pResp    [OUT]     - Pointer to structure containing storage
 *                                 to return data for this TLV.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvSLQSIPPref( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSLQSGetRuntimeSettingsResp *pLresp =
                        (struct QmiWdsSLQSGetRuntimeSettingsResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (!pLresp->WdsSlqsRunTimeSettings.pIPFamilyPreference)
        return eRCode;

    /* Extract the IP Family prefrence */
    eRCode = GetByte( pTlvData,
                      pLresp->WdsSlqsRunTimeSettings.pIPFamilyPreference );

    return eRCode;
}

/*
 * This function unpacks the SLQSGetRuntimeSettings IMCNflag from the
 * QMI response message to a user provided response structure
 *
 * \param     pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param     pResp    [OUT]     - Pointer to structure containing storage
 *                                 to return data for this TLV.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvSLQSIMCNflag( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiWdsSLQSGetRuntimeSettingsResp *pLresp =
                        (struct QmiWdsSLQSGetRuntimeSettingsResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (!pLresp->WdsSlqsRunTimeSettings.pIMCNflag)
        return eRCode;

    /* Extract the IMNC Flag */
    eRCode = GetByte( pTlvData, pLresp->WdsSlqsRunTimeSettings.pIMCNflag );

    return eRCode;
}

/*
 * This function unpacks the SLQSSLQSGetRuntimeSettings Technology name
 * from the QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvSLQSTechName(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsSLQSGetRuntimeSettingsResp *pLresp =
                        (struct QmiWdsSLQSGetRuntimeSettingsResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pLresp->WdsSlqsRunTimeSettings.pTechnology )
        return eRCode;

    /* Extract the Technology name */
    eRCode = GetWord( pTlvData,
                     (swi_uint16 *)pLresp->WdsSlqsRunTimeSettings.
                                   pTechnology );

    return eRCode;
}

/*
 * This function unpacks the SLQSGetRuntimeSettings IPV6 Address from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvSLQSIPAddressV6(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsSLQSGetRuntimeSettingsResp *pLresp =
                        (struct QmiWdsSLQSGetRuntimeSettingsResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    USHORT            idx = 0, *pIPV6Addr = NULL;

    if ( !pLresp->WdsSlqsRunTimeSettings.pIPV6AddrInfo )
        return eRCode;

    /* get the buffer address in local pointer */
    pIPV6Addr = pLresp->WdsSlqsRunTimeSettings.pIPV6AddrInfo->IPAddressV6;

    /* Extract the IPV6 address */
    for( idx = 0; idx < 8 ; idx++ )
    {
        eRCode = GetWordBe( pTlvData, (pIPV6Addr)++ );
        if ( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;
    }

    /* Extract the IPV6 address length */
    eRCode = GetByte( pTlvData, 
                      &( pLresp->WdsSlqsRunTimeSettings.
                         pIPV6AddrInfo->IPV6PrefixLen ) );

    return eRCode;
}

/*
 * This function unpacks the SLQSGetRuntimeSettings IPV6 Gateway Address from
 * the QMI response message to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvSLQSGWAddressV6(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsSLQSGetRuntimeSettingsResp *pLresp =
                        (struct QmiWdsSLQSGetRuntimeSettingsResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    USHORT            idx = 0, *pIPV6GWAddr = NULL;

    if ( !pLresp->WdsSlqsRunTimeSettings.pIPV6GWAddrInfo )
        return eRCode;

    /* get the buffer address in local pointer */
    pIPV6GWAddr = pLresp->WdsSlqsRunTimeSettings.pIPV6GWAddrInfo->gwAddressV6;

    /* Extract the IPV6 Gateway address */
    for( idx = 0; idx < 8 ; idx++ )
    {
        eRCode = GetWordBe( pTlvData, (pIPV6GWAddr)++ );
        if ( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;
    }

    /* Extract the IPV6 Gateway address length */
    eRCode = GetByte( pTlvData, 
                      &( pLresp->WdsSlqsRunTimeSettings.
                         pIPV6GWAddrInfo->gwV6PrefixLen ) );

    return eRCode;
}

/*
 * This function unpacks the StartDataSession2 response message to a
 * user-provided response structure.
 *
 * \param     pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UpkQmiWdsSLQSGetRuntimeSettings(
    BYTE                                    *pMdmResp,
    struct QmiWdsSLQSGetRuntimeSettingsResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,                &qmUnpackTlvResultCode },
        { eTLV_SLQS_PROFILE_NAME,          &UnpackTlvSLQSProfileName},
        { eTLV_SLQS_PDP_TYPE,              &UnpackTlvSLQSPDPType},
        { eTLV_SLQS_APN_NAME,              &UnpackTlvSLQSApnName},
        { eTLV_SLQS_PRIM_DNSV4,            &UnpackTlvSLQSPrimDNSV4},
        { eTLV_SLQS_SECOND_DNSV4,          &UnpackTlvSLQSSecondDNSV4},
        { eTLV_SLQS_UMTS_GRANTED_QOS,      &UnpackTlvSLQSUMTSGrantedQos},
        { eTLV_SLQS_GPRS_GRANTED_QOS,      &UnpackTlvSLQSGPRSGrantedQos},
        { eTLV_SLQS_USERNAME,              &UnpackTlvSLQSUserName},
        { eTLV_SLQS_AUTH_PREF,             &UnpackTlvSLQSAuthentication},
        { eTLV_SLQS_IP_ADDRESSV4,          &UnpackTlvSLQSIPAddressV4},
        { eTLV_SLQS_PROFILE_ID,            &UnpackTlvSLQSProfileid},
        { eTLV_SLQS_GW_ADDRV4,             &UnpackTlvSLQSGatewayV4},
        { eTLV_SLQS_SUBNET_MASKV4,         &UnpackTlvSLQSMaskV4},
        { eTLV_SLQS_PCSCF_ADDR_PCO,        &UnpackTlvSLQSPCSCFAddrPCO},
        { eTLV_SLQS_PCSCF_SRVR_ADDRLISTV4, &UnpackTlvSLQSServerAddrList},
        { eTLV_SLQS_PCSCF_FQDN_LIST,       &UnpackTlvSLQSPCSCFFQDNAddrList},
        { eTLV_SLQS_PRIM_DNSV6,            &UnpackTlvSLQSPrimDNSV6},
        { eTLV_SLQS_SECOND_DNSV6,          &UnpackTlvSLQSSecondDNSV6},
        { eTLV_SLQS_MTU,                   &UnpackTlvSLQSMtu},
        { eTLV_SLQS_DOMAIN_NAME_LIST,      &UnpackTlvSLQSDomainNameList},
        { eTLV_SLQS_IP_FAMILY,             &UnpackTlvSLQSIPPref},
        { eTLV_SLQS_IM_CN_FLAG,            &UnpackTlvSLQSIMCNflag},
        { eTLV_SLQS_TECH_NAME,             &UnpackTlvSLQSTechName},
        { eTLV_SLQS_IP_ADDRESS_V6,         &UnpackTlvSLQSIPAddressV6},
        { eTLV_SLQS_GW_ADDRV6,             &UnpackTlvSLQSGWAddressV6},
        { eTLV_TYPE_INVALID,               NULL } /* Important. Sentinel.
                                                   * Signifies last item in map.
                                                   */
    };
    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_WDS_GET_SETTINGS );
    return eRCode;
}
