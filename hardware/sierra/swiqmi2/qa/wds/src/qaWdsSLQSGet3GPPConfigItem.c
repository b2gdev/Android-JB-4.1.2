/*
 * \ingroup wds
 *
 * \file qaWdsSLQSGet3GPPConfigItem.c
 *
 * \brief  Contains Packing and UnPacking routines for the
 *         eQMI_WDS_SWI_GET_3GPP_CFG_ITEM message.
 *
 * Copyright: © 2011-2012 Sierra Wireless, Inc. all rights reserved
 *
 */
/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaWdsSLQSGet3GPPConfigItem.h"

/*****************************************************************************
 * Request handling
 *****************************************************************************/
/*
 * This function packs the SLQSGet3GPPConfigItem parameters
 * to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError PkQmiWdsSLQSGet3GPPConfigItem(
    WORD    *pMlength,
    BYTE    *pParamField)
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    enum eQCWWANError eRCode;

    eRCode = qmbuild ( pParamField,
                       NULL,
                       map,
                       eQMI_WDS_SWI_GET_3GPP_CFG_ITEM,
                      pMlength );

    return eRCode;
}

/*****************************************************************************
 * Response handling
 *****************************************************************************/
/*
 * This function unpacks the SLQSGet3GPPConfigItem LTE attach profile from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvLTEAttachProfile( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiWdsSLQSGet3GPPConfigItemResp *lResp =
           (struct QmiWdsSLQSGet3GPPConfigItemResp *)pResp;

    slqs3GPPConfigItem *pSlqs3GPPConfigItem = lResp->pSLQS3GPPConfigItem;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if( NULL == pSlqs3GPPConfigItem->pLTEAttachProfile )
       return eRCode;

    eRCode = GetWord( pTlvData, pSlqs3GPPConfigItem->pLTEAttachProfile );

    return eRCode;
}

/*
 * This function unpacks the SLQSGet3GPPConfigItem ProfileList from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvProfileList( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiWdsSLQSGet3GPPConfigItemResp *lResp =
           (struct QmiWdsSLQSGet3GPPConfigItemResp *)pResp;

    slqs3GPPConfigItem *pSlqs3GPPConfigItem = lResp->pSLQS3GPPConfigItem;

    enum eQCWWANError eRCode  = eQCWWAN_ERR_NONE;
    BYTE              counter = 0;

    if( NULL == pSlqs3GPPConfigItem->pProfileList )
       return eRCode;

    for( counter = 0; counter <= 4; counter++ )
    {
        eRCode = GetWord( pTlvData,
                          &pSlqs3GPPConfigItem->pProfileList[counter] );

        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function unpacks the SLQSGet3GPPConfigItem Always Connect Default PDN
 * from the QMI response message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvDefaultPDN( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiWdsSLQSGet3GPPConfigItemResp *lResp =
           (struct QmiWdsSLQSGet3GPPConfigItemResp *)pResp;

    slqs3GPPConfigItem *pSlqs3GPPConfigItem = lResp->pSLQS3GPPConfigItem;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if( NULL == pSlqs3GPPConfigItem->pDefaultPDNEnabled )
       return eRCode;

    eRCode = GetByte( pTlvData,
                      pSlqs3GPPConfigItem->pDefaultPDNEnabled );

    return eRCode;
}

/*
 * This function unpacks the SLQSGet3GPPConfigItem 3gpp release from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlv3gppRelease( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiWdsSLQSGet3GPPConfigItemResp *lResp =
           (struct QmiWdsSLQSGet3GPPConfigItemResp *)pResp;

    slqs3GPPConfigItem *pSlqs3GPPConfigItem = lResp->pSLQS3GPPConfigItem;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if( NULL == pSlqs3GPPConfigItem->p3gppRelease )
       return eRCode;

    eRCode = GetByte( pTlvData,
                      pSlqs3GPPConfigItem->p3gppRelease );

    return eRCode;
}

/*
 * This function unpacks the SLQSGet3GPPConfigItem response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp - Pointer to packed response from the modem.
 *
 * \param  pApiResp - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UpkQmiWdsSLQSGet3GPPConfigItem(
    BYTE                                   *pMdmResp,
    struct QmiWdsSLQSGet3GPPConfigItemResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,                    &qmUnpackTlvResultCode },
        { eTLV_GET_LTE_ATTACH_PROFILE,         &UnpackTlvLTEAttachProfile },
        { eTLV_GET_PROFILE_LIST,               &UnpackTlvProfileList },
        { eTLV_GET_ALWAYS_CONNECT_DEFAULT_PDN, &UnpackTlvDefaultPDN },
        { eTLV_GET_3GPP_RELEASE,               &UnpackTlv3gppRelease },
        { eTLV_TYPE_INVALID,                   NULL }  /* Important. Sentinel.
                                                        * Signifies last item
                                                        * in map.
                                                        */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_WDS_SWI_GET_3GPP_CFG_ITEM );

    return eRCode;
}
