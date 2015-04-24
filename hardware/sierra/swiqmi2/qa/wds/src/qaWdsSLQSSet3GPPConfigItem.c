/*
 * \ingroup wds
 *
 * \file qaWdsSLQSSet3GPPConfigItem.c
 *
 * \brief  Contains Packing and UnPacking routines for the
 *         eQMI_WDS_SWI_SET_3GPP_CFG_ITEM message.
 *
 * Copyright: © 2011-2012 Sierra Wireless, Inc. all rights reserved
 *
 */
/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaWdsSLQSSet3GPPConfigItem.h"

/****************************************************************************
 * Request handling
 ****************************************************************************/
/*
 * This function packs the LTE attach profile field to the QMI message SDU
 *
 * \param pBuf   - Pointer to storage into which the packed
 *                 data will be placed by this function.
 *
 * \param pParam - Pointer to structure containing data for this TLV.
 *
 */
enum eQCWWANError BuildTlvLTEAttachProfile( BYTE *pBuf, BYTE *pParam )
{
    struct QmiWdsSLQSSet3GPPConfigItemReq *pReq =
                         (struct QmiWdsSLQSSet3GPPConfigItemReq *)pParam;

    slqs3GPPConfigItem *pSlqs3GPPConfigItem = pReq->pSLQS3GPPConfigItem;

    enum eQCWWANError eRCode;

    if ( NULL == pSlqs3GPPConfigItem->pLTEAttachProfile )
        return eQCWWAN_ERR_NONE;

    eRCode = PutWord( pBuf, *(pSlqs3GPPConfigItem->pLTEAttachProfile) );

    return eRCode;
}

/*
 * This function packs the Profile List to the QMI message SDU
 *
 * \param pBuf   - Pointer to storage into which the packed
 *                 data will be placed by this function.
 *
 * \param pParam - Pointer to structure containing data for this TLV.
 *
 */
enum eQCWWANError BuildTlvProfileList( BYTE *pBuf, BYTE *pParam )
{
    struct QmiWdsSLQSSet3GPPConfigItemReq *pReq =
                         (struct QmiWdsSLQSSet3GPPConfigItemReq *)pParam;

    slqs3GPPConfigItem *pSlqs3GPPConfigItem = pReq->pSLQS3GPPConfigItem;

    enum eQCWWANError eRCode;
    BYTE              counter;

    if ( NULL == pSlqs3GPPConfigItem->pProfileList )
        return eQCWWAN_ERR_NONE;

    for( counter = 0; counter <= 4; counter++ )
    {
        eRCode = PutWord( pBuf,
                          (pSlqs3GPPConfigItem->pProfileList[counter]) );

        if( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;
    }

    return eRCode;
}

/*
 * This function packs the Always Connect Default PDN value to the
 * QMI message SDU
 *
 * \param pBuf   - Pointer to storage into which the packed
 *                 data will be placed by this function.
 *
 * \param pParam - Pointer to structure containing data for this TLV.
 *
 */
enum eQCWWANError BuildTlvDefaultPDN( BYTE *pBuf, BYTE *pParam )
{
    struct QmiWdsSLQSSet3GPPConfigItemReq *pReq =
                         (struct QmiWdsSLQSSet3GPPConfigItemReq *)pParam;

    slqs3GPPConfigItem *pSlqs3GPPConfigItem = pReq->pSLQS3GPPConfigItem;

    enum eQCWWANError eRCode;

    if ( NULL == pSlqs3GPPConfigItem->pDefaultPDNEnabled )
        return eQCWWAN_ERR_NONE;

    eRCode = PutByte( pBuf, *(pReq->pSLQS3GPPConfigItem->pDefaultPDNEnabled) );

    return eRCode;
}

/*
 * This function packs the 3GPP Release value to the QMI message SDU
 *
 * \param pBuf   - Pointer to storage into which the packed
 *                 data will be placed by this function.
 *
 * \param pParam - Pointer to structure containing data for this TLV.
 *
 */
enum eQCWWANError BuildTlv3gppRelease( BYTE *pBuf, BYTE *pParam )
{
    struct QmiWdsSLQSSet3GPPConfigItemReq *pReq =
                         (struct QmiWdsSLQSSet3GPPConfigItemReq *)pParam;

    slqs3GPPConfigItem *pSlqs3GPPConfigItem = pReq->pSLQS3GPPConfigItem;

    enum eQCWWANError eRCode;

    if ( NULL == pSlqs3GPPConfigItem->p3gppRelease )
        return eQCWWAN_ERR_NONE;

    eRCode = PutByte( pBuf, *(pSlqs3GPPConfigItem->p3gppRelease) );

    return eRCode;
}

/*
 * This function packs the SLQSSet3GPPConfigItem parameters
 * to the QMI message SDU
 *
 * \param  pParamField         [OUT] - Pointer to storage into which the packed
 *                                     data will be placed by this function.
 *
 * \param  pMlength            [OUT] - Total length of built message.
 *
 * \param  pSLQS3GPPConfigItem [IN]  - slqs3GPPConfigItem structure containing
 *                                     values to be packed in QMI request.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError PkQmiWdsSLQSSet3GPPConfigItem(
    WORD               *pMlength,
    BYTE               *pParamField,
    slqs3GPPConfigItem *pSLQS3GPPConfigItem )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SET_LTE_ATTACH_PROFILE,         &BuildTlvLTEAttachProfile },
        { eTLV_SET_PROFILE_LIST,               &BuildTlvProfileList },
        { eTLV_SET_ALWAYS_CONNECT_DEFAULT_PDN, &BuildTlvDefaultPDN },
        { eTLV_SET_3GPP_RELEASE,               &BuildTlv3gppRelease },
        { eTLV_TYPE_INVALID,                   NULL }  /* Important. Sentinel.
                                                        * Signifies last item
                                                        * in map.
                                                        */
    };

    struct QmiWdsSLQSSet3GPPConfigItemReq req;
    enum   eQCWWANError                   eRCode;

    /* Initialize the structure and populate the values */
    req.pSLQS3GPPConfigItem = pSLQS3GPPConfigItem;

    eRCode = qmbuild ( pParamField,
                       (BYTE *) &req,
                       map,
                       eQMI_WDS_SWI_SET_3GPP_CFG_ITEM,
                       pMlength );

    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the SLQSSet3GPPConfigItem response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp - Pointer to packed response from the modem.
 *
 * \param  pApiResp - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UpkQmiWdsSLQSSet3GPPConfigItem(
    BYTE                                   *pMdmResp,
    struct QmiWdsSLQSSet3GPPConfigItemResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item
                                      * in map.
                                      */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_WDS_SWI_SET_3GPP_CFG_ITEM );

    return eRCode;
}
