/*
 * \ingroup dms
 *
 * \file    qaDmsSLQSSetCustFeatures.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_DMS_SWI_SET_CUST_FEATURES message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "sludefs.h"
#include "qaDmsSLQSSetCustFeatures.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the GPS Antenna Select value to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvGPSSel( BYTE *pBuf, BYTE *pParam )
{
    custFeaturesSetting *pReq =
    ((struct QmiDmsSetCustFeaturesReq *)pParam)->pCustFeaturesSetting;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Check For Null Parameter */
    if ( NULL == pReq->pGPSSel )
    {
        return eRCode;
    }

    /* Insert GPS Select parameter */
    eRCode = PutByte( pBuf, *(pReq->pGPSSel) );

    return eRCode;
}

/*
 * This function packs the GPS Enable/Disable value to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvGPSEnable( BYTE *pBuf, BYTE *pParam )
{
    custFeaturesSetting *pReq =
    ((struct QmiDmsSetCustFeaturesReq *)pParam)->pCustFeaturesSetting;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Check For Null Parameter */
    if ( NULL == pReq->pGPSEnable )
    {
        return eRCode;
    }

    /* Insert GPS Enable parameter */
    eRCode = PutLong( pBuf, *(pReq->pGPSEnable) );

    return eRCode;
}

/*
 * This function packs the Set Custom Features request parameters to the QMI
 * message SDU
 *
 * \param  pMlength            [OUT] - Total length of built message.
 *
 * \param  pParamField         [OUT] - Pointer to storage into which the packed
 *                                     data will be placed by this function.
 *
 * \param  pCustfeatureSetting [IN]  - structure containing data to be packed.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaGobiApiDms.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiDmsSetCustFeatures(
    WORD                *pMlength,
    BYTE                *pParamField,
    custFeaturesSetting *pCustFeaturesSetting )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_GPS_SELECT,     &BuildTlvGPSSel },
        { eTLV_GPS_SET_ENABLE, &BuildTlvGPSEnable },
        { eTLV_TYPE_INVALID,   NULL } /* Important. Sentinel.
                                       * Signifies last item in map
                                       */
    };

    struct QmiDmsSetCustFeaturesReq req;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiDmsSetCustFeaturesReq));
    req.pCustFeaturesSetting = pCustFeaturesSetting;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_DMS_SWI_SET_CUST_FEATURES,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the set custom features response message to a user
 * provided response structure.
 *
 * \param  pMdmResp - Pointer to packed response from the modem.
 *
 * \param  pApiResp - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiDmsSetCustFeatures(
    BYTE                             *pMdmResp,
    struct QmiDmsSetCustFeaturesResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_DMS_SWI_SET_CUST_FEATURES );
    return eRCode;
}
