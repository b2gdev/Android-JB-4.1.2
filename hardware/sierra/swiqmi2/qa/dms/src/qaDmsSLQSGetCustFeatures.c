/*
 * \ingroup dms
 *
 * \file    qaDmsSLQSGetCustFeatures.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_DMS_SWI_GET_CUST_FEATURES message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaDmsSLQSGetCustFeatures.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the Get Custom Features request parameters to the QMI
 * message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaGobiApiDms.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiDmsGetCustFeatures(
    WORD *pMlength,
    BYTE *pParamField )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID,   NULL } /* Important. Sentinel.
                                       * Signifies last item in map
                                       */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      NULL,
                      map,
                      eQMI_DMS_SWI_GET_CUST_FEATURES,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the GPS Enable parameter from the QMI response
 * message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE
 */
enum eQCWWANError UnpackTlvGPSEnable(BYTE *pTlvData, BYTE *pResp)
{
    custFeaturesInfo *lResp =
        ((struct QmiDmsGetCustFeaturesResp *)pResp)->pCustFeaturesInfo;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = GetLong( pTlvData, &(lResp->GpsEnable) );
    return eRCode;
}

/*
 * This function unpacks the Disable IMSI parameter from the QMI response
 * message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE
 */
enum eQCWWANError UnpackTlvDisableIMSI(BYTE *pTlvData, BYTE *pResp)
{
    custFeaturesInfo *lResp =
        ((struct QmiDmsGetCustFeaturesResp *)pResp)->pCustFeaturesInfo;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Check For Null Parameter */
    if ( NULL == lResp->pDisableIMSI )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, lResp->pDisableIMSI );
    return eRCode;
}

/*
 * This function unpacks the IP Family Support parameter from the QMI response
 * message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE
 */
enum eQCWWANError UnpackTlvIPFamSupport(BYTE *pTlvData, BYTE *pResp)
{
    custFeaturesInfo *lResp =
        ((struct QmiDmsGetCustFeaturesResp *)pResp)->pCustFeaturesInfo;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Check For Null Parameter */
    if ( NULL == lResp->pIPFamSupport )
    {
        return eRCode;
    }

    eRCode = GetWord( pTlvData, lResp->pIPFamSupport );
    return eRCode;
}

/*
 * This function unpacks the RM Auto Connect parameter from the QMI response
 * message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE
 */
enum eQCWWANError UnpackTlvRMAutoConnect(BYTE *pTlvData, BYTE *pResp)
{
    custFeaturesInfo *lResp =
        ((struct QmiDmsGetCustFeaturesResp *)pResp)->pCustFeaturesInfo;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Check For Null Parameter */
    if ( NULL == lResp->pRMAutoConnect )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, lResp->pRMAutoConnect );
    return eRCode;
}

/*
 * This function unpacks the GPS Select parameter from the QMI response
 * message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE
 */
enum eQCWWANError UnpackTlvGPSSel(BYTE *pTlvData, BYTE *pResp)
{
    custFeaturesInfo *lResp =
        ((struct QmiDmsGetCustFeaturesResp *)pResp)->pCustFeaturesInfo;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Check For Null Parameter */
    if ( NULL == lResp->pGPSSel )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, lResp->pGPSSel );
    return eRCode;
}

/*
 * This function unpacks the get custom features response message to a user
 * provided response structure.
 *
 * \param  pMdmResp - Pointer to packed response from the modem.
 *
 * \param  pApiResp - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiDmsGetCustFeatures(
    BYTE                             *pMdmResp,
    struct QmiDmsGetCustFeaturesResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,       &qmUnpackTlvResultCode },
        { eTLV_GPS_ENABLE,        &UnpackTlvGPSEnable },
        { eTLV_DISABLE_IMSI,      &UnpackTlvDisableIMSI },
        { eTLV_IP_FAMILY_SUPPORT, &UnpackTlvIPFamSupport },
        { eTLV_RM_AUTO_CONNECT,   &UnpackTlvRMAutoConnect },
        { eTLV_GPS_SEL,           &UnpackTlvGPSSel },
        { eTLV_TYPE_INVALID,      NULL } /* Important. Sentinel.
                                          * Signifies last item in map.
                                          */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_DMS_SWI_GET_CUST_FEATURES );
    return eRCode;
}
