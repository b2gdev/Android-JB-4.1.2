/*
 * \ingroup sms
 *
 * \file    qaSmsSLQSGetIndicationRegister.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          eQMI_WMS_GET_IND_REGISTER message.
 *
 * Copyright: © 2011-12 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaSmsSLQSGetIndicationRegister.h"

/*****************************************************************************
 * Request handling
 *****************************************************************************/

/*
 * This function packs the SLQS Get Indication Register request parameters to
 * the QMI message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError PkQmiSmsSLQSGetIndicationReg(
    WORD *pMlength,
    BYTE *pParamField )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID,    NULL } /* Important. Sentinel.
                                        * Signifies last item in map
                                        */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      NULL,
                      map,
                      eQMI_WMS_GET_IND_REGISTER,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the registration status of Transport layer information
 * events from the QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvTransLayerInfoEvt(
    BYTE *pTlvData,
    BYTE *pResp )
{
    getIndicationRegResp *lResp =
        ((struct QmiSmsSLQSGetIndicationRegisterResp *)
        pResp)->pGetIndicationRegInfo;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pRegTransLayerInfoEvt )
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pRegTransLayerInfoEvt );
}

/*
 * This function unpacks the registration status of NW Registration information
 * events from the QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvTransNWRegInfoEvt(
    BYTE *pTlvData,
    BYTE *pResp )
{
    getIndicationRegResp *lResp =
        ((struct QmiSmsSLQSGetIndicationRegisterResp *)
        pResp)->pGetIndicationRegInfo;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pRegTransNWRegInfoEvt)
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pRegTransNWRegInfoEvt );

}

/*
 * This function unpacks the registration status of call status information
 * events from the QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCallStatusInfoEvt(
    BYTE *pTlvData,
    BYTE *pResp )
{
    getIndicationRegResp *lResp =
        ((struct QmiSmsSLQSGetIndicationRegisterResp *)
        pResp)->pGetIndicationRegInfo;

    /* Check For Invalid Parameter */
    if ( NULL == lResp->pRegCallStatInfoEvt)
    {
        return eQCWWAN_ERR_NONE;
    }

    return GetByte( pTlvData, lResp->pRegCallStatInfoEvt );

}
/*
 * This function unpacks the SLQS Get registration indication response to
 * a user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiSmsSLQSGetIndicationReg(
    BYTE                                       *pMdmResp,
    struct QmiSmsSLQSGetIndicationRegisterResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,              &qmUnpackTlvResultCode },
        { eTLV_TRANS_LAYER_INFO_EVENTS,  &UnpackTlvTransLayerInfoEvt },
        { eTLV_TRANS_NW_REG_INFO_EVENTS, &UnpackTlvTransNWRegInfoEvt },
        { eTLV_CALL_STATUS_INFO_EVENTS,  &UnpackTlvCallStatusInfoEvt},
        { eTLV_TYPE_INVALID,             NULL } /* Important. Sentinel.
                                                 * Signifies last item in map.
                                                 */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_WMS_GET_IND_REGISTER );
    return eRCode;
}
