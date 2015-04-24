/*
 * \ingroup wds
 *
 * \file   qaWdsSLQSACSettings.c
 *
 * \brief  Contains Packing and UnPacking routines for the
 *         QMI_SLQS_WDS_AUTOCONNCET_SETTINGS message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaGobiApiWds.h"
#include "qaWdsSLQSACSettings.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * function packs the SLQSAutoConnect acsetting field to the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSAcSetting(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsSLQSSetAutoConnectReq *pReq =
                (struct QmiWdsSLQSSetAutoConnectReq *)pParam;

    /* Add TLV data */
    return PutByte( pBuf, pReq->slqsautoconnectReq.acsetting );
}

/*
 * This function packs the SLQSAutoConnect acroamsetting field to
 * the QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError BuildTlvSLQSAcRoamSetting(BYTE *pBuf, BYTE *pParam)
{
    struct QmiWdsSLQSSetAutoConnectReq *pReq =
                (struct QmiWdsSLQSSetAutoConnectReq *)pParam;

    /* Add TLV data */
    return PutByte( pBuf, pReq->slqsautoconnectReq.acroamsetting);
}

/*
 * This function packs the SLQSAutoConnect parameters to the QMI message SDU

 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \param  acsetting         - Autoconnect Setting
 *
 * \param  acroamsetting     - Autoconnect Roam Setting
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa     qaGobiApiWds.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiWdsSLQSSetAutoConnect(
    WORD *pMlength,
    BYTE *pParamField,
    BYTE acsetting,
    BYTE acroamsetting )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_AUTOCONNECT_SETTING,      &BuildTlvSLQSAcSetting },
        { eTLV_AUTOCONNECT_ROAM_SETTING, &BuildTlvSLQSAcRoamSetting },
        { eTLV_TYPE_INVALID,             NULL } /* Important Sentinel
                                                 * Signifies last item in map
                                                 */
    };

    struct QmiWdsSLQSSetAutoConnectReq req;
    enum eQCWWANError                  eRCode;

    slmemset((char *)&req, 0, sizeof (struct QmiWdsSLQSSetAutoConnectReq));
    req.slqsautoconnectReq.acsetting     = acsetting;
    req.slqsautoconnectReq.acroamsetting = acroamsetting;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_WDS_SET_AUTOCONNECT,
                      pMlength );
    return eRCode;
}

/*
 * This function packs the SLQSAutoConnect parameters to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa     qaGobiApiWds.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiWdsSLQSGetAutoConnect(
    WORD                   *pMlength,
    BYTE                   *pParamField )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID,  NULL } /* Important Sentinel
                                      * Signifies last item in map
                                      */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      NULL,
                      map,
                      eQMI_WDS_GET_AUTOCONNECT,
                      pMlength );
    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the acsetting from the QMI response message to a
 * user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError qmUnpackTlvAcSetting(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsSLQSGetAutoConnectResp *pLresp =
                        (struct QmiWdsSLQSGetAutoConnectResp*)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pLresp )
    {
        return eRCode;
    }

    /* Extract the extended error code */
    eRCode = GetByte( pTlvData, &pLresp->slqsautoconnectResp.acsetting );

    return eRCode;
}

/*
 * This function unpacks the from acroamsetting the QMI response message
 * to a user provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError qmUnpackTlvAcRoamSetting(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsSLQSGetAutoConnectResp *pLresp =
                        (struct QmiWdsSLQSGetAutoConnectResp*)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pLresp )
    {
        return eRCode;
    }

    /* Extract the extended error code */
    eRCode = GetByte( pTlvData, &pLresp->slqsautoconnectResp.acroamsetting);

    return eRCode;
}

/*
 * This function unpacks the SLQSAutoConnect response message to a
 * user-provided response structure.
 *
 * \param     pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UpkQmiWdsSLQSGetAutoConnect(
    BYTE                                *pMdmResp,
    struct QmiWdsSLQSGetAutoConnectResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,              &qmUnpackTlvResultCode },
        { eTLV_AUTOCONNECT_SETTING,      &qmUnpackTlvAcSetting },
        { eTLV_AUTOCONNECT_ROAM_SETTING, &qmUnpackTlvAcRoamSetting},
        { eTLV_TYPE_INVALID,             NULL }/* Important. Sentinel.
                                                * Signifies last item in map.
                                                */
    };
    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_WDS_GET_AUTOCONNECT);
    return eRCode;
}

/*
 * This function unpacks the SLQSAutoConnect response message to a
 * user-provided response structure.
 *
 * \param     pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UpkQmiWdsSLQSSetAutoConnect(
    BYTE                                *pMdmResp,
    struct QmiWdsSLQSSetAutoConnectResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_TYPE_INVALID, NULL }/* Important Sentinel
                                    * Signifies last item in map
                                    */
    };
    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_WDS_SET_AUTOCONNECT );
    return eRCode;
}
