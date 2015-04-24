/*
 * \ingroup sms
 *
 * \file    qaSmsSLQSSetIndicationRegister.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          eQMI_WMS_SET_IND_REGISTER message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "sludefs.h"
#include "qaSmsSLQSSetIndicationRegister.h"

/*****************************************************************************
 * Request handling
 *****************************************************************************/

/*
 * Packs the Transport Layer Info Events registration status field for
 * SLQSSetIndicationRegister API to the QMI message SDU
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvSetTransLayerInfoEvt( BYTE *pBuf, BYTE *pParam )
{
    struct QmiSmsSLQSSetIndicationRegisterReq *pReq =
        (struct QmiSmsSLQSSetIndicationRegisterReq *)pParam;

   setIndicationRegReq *lResp = pReq->pSetIndicationRegisterReq;

   if ( NULL == lResp->pRegTransLayerInfoEvt )
   {
       return eQCWWAN_ERR_NONE;
   }

    /* Add TLV data */
    return PutByte( pBuf, *(lResp->pRegTransLayerInfoEvt) );
}

/*
 * Packs the Transport NW Registration status field for
 * SLQSSetIndicationRegister API to the QMI message SDU
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvSetTransNWRegInfoEvt( BYTE *pBuf, BYTE *pParam )
{
    struct QmiSmsSLQSSetIndicationRegisterReq *pReq =
        (struct QmiSmsSLQSSetIndicationRegisterReq *)pParam;

   setIndicationRegReq *lResp = pReq->pSetIndicationRegisterReq;

   if ( NULL == lResp->pRegTransNWRegInfoEvt )
   {
       return eQCWWAN_ERR_NONE;
   }

    /* Add TLV data */
    return PutByte( pBuf, *(lResp->pRegTransNWRegInfoEvt) );
}

/*
 * Packs the Call status Information events field for
 * SLQSSetIndicationRegister API to the QMI message SDU
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvSetCallStatusInfoEvt( BYTE *pBuf, BYTE *pParam )
{
    struct QmiSmsSLQSSetIndicationRegisterReq *pReq =
        (struct QmiSmsSLQSSetIndicationRegisterReq *)pParam;

   setIndicationRegReq *lResp = pReq->pSetIndicationRegisterReq;

   if ( NULL == lResp->pRegCallStatInfoEvt )
   {
       return eQCWWAN_ERR_NONE;
   }

    /* Add TLV data */
    return PutByte( pBuf, *(lResp->pRegCallStatInfoEvt) );
}

/*
 * This function packs the SLQS Set Indication Register request parameters to
 * the QMI message SDU
 *
 * \param  pMlength                 [OUT] - Total length of built message.
 *
 * \param  pParamField              [OUT] - Pointer to storage into which
 *                                          the packed data will be placed by
 *                                          this function.
 *
 * \param pSetIndicationRegisterReq [IN]  - structure with values to be packed
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError PkQmiSmsSLQSSetIndicationReg(
    WORD                *pMlength,
    BYTE                *pParamField,
    setIndicationRegReq *pSetIndicationRegisterReq )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SET_TRANS_LAYER_INFO_EVENTS,  &BuildTlvSetTransLayerInfoEvt },
        { eTLV_SET_TRANS_NW_REG_INFO_EVENTS, &BuildTlvSetTransNWRegInfoEvt },
        { eTLV_SET_CALL_STATUS_INFO_EVENTS,  &BuildTlvSetCallStatusInfoEvt },
        { eTLV_TYPE_INVALID,                NULL } /* Important. Sentinel.
                                                    * Signifies last item
                                                    *  in map
                                                    */
    };

    struct QmiSmsSLQSSetIndicationRegisterReq req;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    slmemset( (CHAR *)&req,
             0,
             sizeof(struct QmiSmsSLQSSetIndicationRegisterReq) );

    req.pSetIndicationRegisterReq = pSetIndicationRegisterReq;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_WMS_SET_IND_REGISTER,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the SLQS Set registration indication response to
 * a user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiSmsSLQSSetIndicationReg(
    BYTE                                       *pMdmResp,
    struct QmiSmsSLQSSetIndicationRegisterResp *pApiResp )
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
                           eQMI_WMS_SET_IND_REGISTER );
    return eRCode;
}
