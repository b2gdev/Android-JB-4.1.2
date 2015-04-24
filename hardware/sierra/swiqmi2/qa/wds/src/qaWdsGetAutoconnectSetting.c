/*
 * \ingroup wds
 *
 * \file qaWdsGetAutoConnectSetting.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        QMI_WDS_GET_AUTOCONNECT message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"

#include "qaWdsGetAutoconnectSetting.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the GetAutoconnect parameters
 * to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQMI_ERR_NONE, on success
 * \return eQMI_ERR_NO_MEMORY, on error
 *
 * \notes  This API is only supported if the firmware contains support for
 *            QMI Gobi3000 addendum. The relevant QCT document is 80-VK268-3
 *            in which the QMI_WDS_GET_AUTOCONNECT_SETTING message is defined.
 *
 */
enum eQCWWANError PkQmiWdsGetAutoconnectSetting(
    WORD *pMlength,
    BYTE *pParamField )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    enum eQCWWANError eRCode;

    eRCode = qmbuild( pParamField, NULL, map,
                      eQMI_WDS_GET_AUTOCONNECT, pMlength);

    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the GetAutoconnect Setting from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                           to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvSetting(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsGetAutoconnectSettingResp *lResp =
                        (struct QmiWdsGetAutoconnectSettingResp *)pResp;
    BYTE lTemp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (!lResp->pSetting)
        return eRCode;

    eRCode = GetByte(pTlvData, &lTemp);
    *(lResp->pSetting) = lTemp;

    return eRCode;
}

/*
 * This function unpacks the getAutoconenct response message to to a
 * user-provided response structure.
 *
 * \param  pMdmResp - Pointer to packed response from the modem.
 *
 * \param  pApiResp - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UpkQmiWdsGetAutoconnectSetting(
    BYTE   *pMdmResp,
    struct QmiWdsGetAutoconnectSettingResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_SETTING,      &UnpackTlvSetting },
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                         * Signifies last item in map.
                                         */
    };

    eRCode = qmunpackresp( pMdmResp, (BYTE *)pApiResp, map,
                          eQMI_WDS_GET_AUTOCONNECT );

    return eRCode;
}
