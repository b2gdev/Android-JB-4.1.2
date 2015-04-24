/*
 * \ingroup swioma
 *
 * \file    qaSwiOmaDmSetSettings.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_SWIOMA_DM_SET_SETTINGS message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaSwiOmaDmSetSettings.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the SLQSOMADMSetSettings FOTA Download field to the
 * QMI message SDU
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvFOTADownload( BYTE *pBuf, BYTE *pParam )
{
    SLQSOMADMSettingsReqParams *pReq = (SLQSOMADMSettingsReqParams *)pParam;

    /* Add TLV data */
    return PutByte( pBuf, pReq->FOTAdownload );
}

/*
 * This function packs the SLQSOMADMSetSettings FOTA Update field to the
 * QMI message SDU
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvFOTAUpdate( BYTE *pBuf, BYTE *pParam )
{
    SLQSOMADMSettingsReqParams *pReq = (SLQSOMADMSettingsReqParams *)pParam;

    /* Add TLV data */
    return PutByte( pBuf, pReq->FOTAdownload );
}

/*
 * This function packs the SLQSOMADMSetSettings FOTA Update field to the
 * QMI message SDU
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvAutosdm( BYTE *pBuf, BYTE *pParam )
{
    SLQSOMADMSettingsReqParams *pReq = (SLQSOMADMSettingsReqParams *)pParam;

    if( NULL == pReq->pAutosdm )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Add TLV data */
    return PutByte( pBuf, *(pReq->pAutosdm) );
}
/*
 * This function packs the SLQSOMADMSetSettings parameters to the QMI
 * message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \param  pReq         [OUT] - Pointer to the structure containing the data
 *                              to be packed.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaGobiApiSwiOmadms.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiSwiOmaDmSetSettings(
    WORD                       *pMlength,
    BYTE                       *pParamField,
    SLQSOMADMSettingsReqParams *pReq )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SET_FOTA_AUTOMATIC_DOWNLOAD, &BuildTlvFOTADownload },
        { eTLV_SET_FOTA_AUTOMATIC_UPDATE,   &BuildTlvFOTAUpdate },
        { eTLV_SET_OMA_AUTO_UI_ALERT_RESP,  &BuildTlvAutosdm },
        { eTLV_TYPE_INVALID,                NULL } /* Important. Sentinel.
                                                    * Signifies last item in map
                                                    */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pReq,
                      map,
                      eQMI_SWIOMA_SET_SETTINGS,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the SLQSOMADMSetSettings response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiSwiOmaDmSetSettings(
    BYTE                              *pMdmResp,
    struct QmiSwiOmaDmSetSettingsResp *pApiResp )
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
                           eQMI_SWIOMA_SET_SETTINGS );
    return eRCode;
}
