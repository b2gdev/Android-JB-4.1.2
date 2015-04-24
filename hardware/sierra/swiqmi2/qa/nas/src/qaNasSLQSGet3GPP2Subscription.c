/*
 * \ingroup nas
 *
 * \file qaNasSLQSGet3GPP2Subscription.c
 *
 * \brief  Contains Packing and UnPacking routines for the
 *         eQMI_NAS_GET_3GPP2_SUBSCRIPTION_INFO message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaNasSLQSGet3GPP2Subscription.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the NAM ID Information to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvNamID( BYTE *pBuf, BYTE *pParam )
{
    nasGet3GPP2SubscriptionInfoReq *pReq =
                                ( nasGet3GPP2SubscriptionInfoReq * )pParam;

    /* Insert NAM ID */
    return PutByte( pBuf, pReq->namID );
}

/*
 * This function packs the Get3GPP2Subscription parameters to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 *  \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 */
enum eQCWWANError PkQmiNasSlqsGet3GPP2SubscriptionInfo (
    WORD                           *pMlength,
    BYTE                           *pParamField,
    nasGet3GPP2SubscriptionInfoReq *pGet3GPP2SubsInfoReq )
{
    /* There is no mandatory TLV and hence the map is empty */
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_NAM_ID_REQ,   &BuildTlvNamID },
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    enum eQCWWANError eRCode;
    eRCode = qmbuild( pParamField,
                      (BYTE *)pGet3GPP2SubsInfoReq,
                      map,
                      eQMI_NAS_GET_3GPP2_SUBSCRIPTION,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling*/
/******************************************************************************/
/*
 * This function unpacks the NAMName Subscription Information from the QMI
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
enum eQCWWANError UnpackTlvNAMNameSubscriptionInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    namName *lResp = ((struct QmiNasSLQSGet3GPP2SubscriptionInfoResp *)
                              pResp)->pNas3GPP2SubscriptionInfoResp->pNAMNameInfo;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD lCount;

    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

     /* Extract the NAM Name Info */
     eRCode = GetByte( pTlvData, &lResp->namNameLen );
     if ( eQCWWAN_ERR_NONE != eRCode )
     {
           return eRCode;
     }

     for ( lCount = 0 ; lCount < lResp->namNameLen ; lCount++ )
     {
         eRCode = GetByte( pTlvData, &lResp->namName[lCount] );
         if ( eQCWWAN_ERR_NONE != eRCode )
         {
            return eRCode;
         }
     }

     return eRCode;
 }

/*
 * This function unpacks the Directory Number Subscription Information from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */

enum eQCWWANError UnpackTlvDirNumSubscriptionInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    dirNum *lResp = ((struct QmiNasSLQSGet3GPP2SubscriptionInfoResp *)
                         pResp)->pNas3GPP2SubscriptionInfoResp->pDirNum;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD lCount;

    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the DIR Number Info */
    eRCode = GetByte( pTlvData, &lResp->dirNumLen );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < lResp->dirNumLen ; lCount++ )
    {
        eRCode = GetByte( pTlvData, &lResp->dirNum[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function unpacks the Home SID/NID Subscription Information from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvHomeSIDNIDSubsInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    homeSIDNID *lResp = ((struct QmiNasSLQSGet3GPP2SubscriptionInfoResp *)
                            pResp)->pNas3GPP2SubscriptionInfoResp->pHomeSIDNID;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD lCount;

    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the Home SID/NID Info */
    eRCode = GetByte( pTlvData, &lResp->numInstances );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < lResp->numInstances ; lCount++ )
    {
        sidNid *lresp = &lResp->SidNid[lCount];

        eRCode = GetWord( pTlvData, &lresp->nid );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
        eRCode = GetWord( pTlvData, &lresp->sid );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function unpacks the MIN-based IMSI Subscription Information from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
*/
enum eQCWWANError UnpackTlvMinBasedIMSISubsInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    minBasedIMSI *lResp = ((struct QmiNasSLQSGet3GPP2SubscriptionInfoResp *)
                                   pResp)->pNas3GPP2SubscriptionInfoResp->pMinBasedIMSI ;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD lCount;

    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the MIN-based IMSI Info */
    for ( lCount = 0 ; lCount < PLMN_LENGTH ; lCount++ )
    {
        eRCode = GetByte( pTlvData, &lResp->mccM[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    eRCode = GetWord( pTlvData, &lResp->imsiM1112 );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < IMSI_M_S1_LENGTH ; lCount++ )
    {
        eRCode = GetByte( pTlvData, &lResp->imsiMS1[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
           return eRCode;
        }
    }

    for ( lCount = 0 ; lCount < IMSI_M_S2_LENGTH ; lCount++ )
    {
        eRCode = GetByte( pTlvData, &lResp->imsiMS2[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function unpacks the True IMSI Subscription Information from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvTrueIMSISubscriptionInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    trueIMSI *lResp = ((struct QmiNasSLQSGet3GPP2SubscriptionInfoResp *)
                               pResp)->pNas3GPP2SubscriptionInfoResp->pTrueIMSI;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD lCount;

    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the True IMSI IMSI Info */
    for ( lCount = 0 ; lCount < PLMN_LENGTH ; lCount++ )
    {
        eRCode = GetByte( pTlvData, &lResp->mccT[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    eRCode = GetWord( pTlvData, &lResp->imsiT1112);
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < IMSI_M_S1_LENGTH ; lCount++ )
    {
        eRCode = GetByte( pTlvData, &lResp->imsiTS1[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    for ( lCount = 0 ; lCount < IMSI_M_S2_LENGTH ; lCount++ )
    {
        eRCode = GetByte( pTlvData, &lResp->imsiTS2[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return GetByte( pTlvData, &lResp->imsiTaddrNum);
}

/*
 * This function unpacks the CDMA Channel Subscription Information from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvCDMAChannelSubsInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    CDMAChannel *lResp = ((struct QmiNasSLQSGet3GPP2SubscriptionInfoResp *)
                           pResp)->pNas3GPP2SubscriptionInfoResp->pCDMAChannel;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the CDMA Channel Info */
    eRCode = GetWord( pTlvData, &lResp->priChA );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &lResp->priChB );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, &lResp->secChA );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetWord( pTlvData, &lResp->secChB );
}

/*
 * This function unpacks the SlqsGet3GPP2SubscriptionInfoResp response message
 * to to a user-provided response structure.
 *
 * \param     pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp  [OUT]  - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiNasSlqsGet3GPP2SubscriptionInfo(
    BYTE                                          *pMdmResp,
    struct QmiNasSLQSGet3GPP2SubscriptionInfoResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,               &qmUnpackTlvResultCode },
        { eTLV_NAMNAME_3GPP2_INFO,        &UnpackTlvNAMNameSubscriptionInfo },
        { eTLV_DIR_NUM_3GPP2_INFO,        &UnpackTlvDirNumSubscriptionInfo },
        { eTLV_HOME_SID_NID_3GPP2_INFO,   &UnpackTlvHomeSIDNIDSubsInfo },
        { eTLV_MIN_BASED_IMSI_3GPP2_INFO, &UnpackTlvMinBasedIMSISubsInfo },
        { eTLV_TRUE_IMSI_3GPP2_INFO,      &UnpackTlvTrueIMSISubscriptionInfo },
        { eTLV_CDMA_CHANNEL_3GPP2_INFO,   &UnpackTlvCDMAChannelSubsInfo },
        { eTLV_TYPE_INVALID,              NULL }  /* Important. Sentinel.
                                                   * Signifies last item in map.
                                                   */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE*)pApiResp,
                           map,
                           eQMI_NAS_GET_3GPP2_SUBSCRIPTION );
    return eRCode;
}
