/*
 * \ingroup rms
 *
 * \file    qaRmsSetEventSmsWake.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_RMS_SET_EVENT_SMS_WAKE message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */
#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaRmsSetEventSmsWake.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * Packs the SetSMSWake bEnable field to the QMI message SDU
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
enum eQCWWANError BuildTlvReqSmsWakeState( BYTE *pBuf, BYTE *pParam )
{
    struct QmiRmsSetEventSmsWakeReq *pReq =
                 (struct QmiRmsSetEventSmsWakeReq *)pParam;

    /* Add TLV data */
    return PutByte( pBuf, pReq->bEnable );
}

/*
 * Packs the SetSMSWake wakeMask field to the QMI message SDU
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
enum eQCWWANError BuildTlvReqSmsWakeMask( BYTE *pBuf, BYTE *pParam )
{
    struct QmiRmsSetEventSmsWakeReq *pReq =
                 (struct QmiRmsSetEventSmsWakeReq *)pParam;

    /* Add TLV data */
    return PutLong( pBuf, pReq->wakeMask );
}

/*
 * packs the SetSMSWake parameters to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                                    data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 * \sa     See qaGobiApiRms.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiRmsSetEventSmsWake(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG bEnable,
    ULONG wakeMask )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SMS_SET_WAKE_STATE, &BuildTlvReqSmsWakeState },
        { eTLV_SMS_SET_WAKE_MASK,  &BuildTlvReqSmsWakeMask },
        { eTLV_TYPE_INVALID,       NULL } /* Important. Sentinel.
                                           * Signifies last item in map.
                                           */
    };

    struct QmiRmsSetEventSmsWakeReq req;

    enum eQCWWANError eRCode;

    slmemset( (CHAR *)&req, 0, sizeof(struct QmiRmsSetEventSmsWakeReq) );
    req.bEnable  = bEnable;
    req.wakeMask = wakeMask;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_RMS_SET_SMS_WAKE,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the SetSMSWake response
 * message to a user-provided response structure.
 *
 * \param  pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp  [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError UpkQmiRmsSetEventSmsWake(
    BYTE                             *pMdmResp,
    struct QmiRmsSetEventSmsWakeResp *pApiResp )
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
                           eQMI_RMS_SET_SMS_WAKE );
    return eRCode;
}
