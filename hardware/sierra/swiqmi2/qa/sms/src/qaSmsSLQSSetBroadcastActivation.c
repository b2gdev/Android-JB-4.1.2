/*
 * \ingroup sms
 *
 * \file    qaSmsSLQSSetBroadcastActivation.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_WMS_SET_BC_ACTIVATION message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */
#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaSmsSLQSSetBroadcastActivation.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the SetBroadcastActivation Address field
 * to the QMI message SDU
 *
 * \param pBuf   - Pointer to storage into which the packed
 *                 data will be placed by this function.
 *
 * \param pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError BuildTlvSMSBCInfo( BYTE *pBuf, BYTE *pParam )
{
    struct QmiSmsSetBroadcastActivationReq *pReq =
                     (struct QmiSmsSetBroadcastActivationReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Add TLV data */
    eRCode = PutByte( pBuf, pReq->mode );
    if ( eRCode != eQCWWAN_ERR_NONE )
    {
        return eRCode;
    }

    eRCode = PutByte( pBuf, pReq->bc_activate );
    return eRCode;
}

/*
 * This function packs the SetBroadcastActivation parameters to the 
 * QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                                    data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 * \sa     qaGobiApiSms.h for remaining parameter descriptions.
 *
 */
enum eQCWWANError PkQmiSmsSetBroadcastActivation(
    WORD *pMlength,
    BYTE *pParamField,
    BYTE mode,
    BYTE bc_activate )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_BC_ACTIVATION_INFO, &BuildTlvSMSBCInfo },
        { eTLV_TYPE_INVALID,       NULL } /* Important. Sentinel.
                                           * Signifies last item in map.
                                           */
    };

    enum   eQCWWANError            eRCode;
    struct QmiSmsSetBroadcastActivationReq req;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiSmsSetBroadcastActivationReq));
    req.mode = mode;
    req.bc_activate = bc_activate;

    eRCode = qmbuild ( pParamField,
                       (BYTE *)&req,
                       map,
                       eQMI_WMS_SET_BC_ACTIVATION,
                       pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the SetBroadcastActivation response
 * message to a user-provided response structure.
 *
 * \param  pMdmResp  [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp  [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError UpkQmiSmsSetBroadcastActivation(
    BYTE                                    *pMdmResp,
    struct QmiSmsSetBroadcastActivationResp *pApiResp )
{
    enum eQCWWANError eRCode;

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
                           eQMI_WMS_SET_BC_ACTIVATION );
    return eRCode;
}

