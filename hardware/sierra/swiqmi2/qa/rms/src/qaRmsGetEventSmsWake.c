/*
 * \ingroup rms
 *
 * \file    qaRmsGetEventSmsWake.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_RMS_GET_EVENT_SMS_WAKE message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"

#include "qaRmsGetEventSmsWake.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * Packs the GetSMSWake parameters to the QMI message SDU
 *
 * \param  pParamField [OUT]  - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \param  pMlength    [OUT]  - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError PkQmiRmsGetEventSmsWake(
    WORD *pMlength,
    BYTE *pParamField )
{
    /* There is no mandatory TLV hence MAP is empty */
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };

    enum eQCWWANError eRCode;

    eRCode = qmbuild( pParamField,
                      NULL,
                      map,
                      eQMI_RMS_GET_SMS_WAKE,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * Unpacks the GetSMSWake pEnabled from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    - Pointer to structure containing storage
 *                    to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError qmUnpackTlvSmsWakeState( BYTE *pTlvData, BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    struct QmiRmsGetEventSmsWakeResp *lResp =
                 (struct QmiRmsGetEventSmsWakeResp *)pResp;

    BYTE lByte = 0;

    if ( !lResp->pEnabled )
        return eRCode;

    /* Validate TLV parameter length field */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                       eRMS_GET_EVENT_SMS_WAKE_ENABLE_LENGTH );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    eRCode = GetByte( pTlvData, &lByte );

    if ( eQCWWAN_ERR_NONE == eRCode )
        *(lResp->pEnabled) = lByte;

    return eRCode;
}

/*
 * Unpacks the GetSMSWake pWakeMask from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    - Pointer to structure containing storage
 *                    to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError qmUnpackTlvSmsWakeMask( BYTE *pTlvData, BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    struct QmiRmsGetEventSmsWakeResp *lResp =
                 (struct QmiRmsGetEventSmsWakeResp *)pResp;

    if ( !lResp->pWakeMask )
        return eRCode;

    /* Validate TLV parameter length field */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                         eRMS_GET_EVENT_SMS_WAKE_MASK_LENGTH );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    return GetLong( pTlvData, lResp->pWakeMask );
}

/*
 * Unpacks the GetSMSWake response
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
enum eQCWWANError UpkQmiRmsGetEventSmsWake(
    BYTE                             *pMdmResp,
    struct QmiRmsGetEventSmsWakeResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,    &qmUnpackTlvResultCode },
        { eTLV_SMS_WAKE_STATE, &qmUnpackTlvSmsWakeState },
        { eTLV_SMS_WAKE_MASK,  &qmUnpackTlvSmsWakeMask },
        { eTLV_TYPE_INVALID,   NULL } /* Important. Sentinel.
                                       * Signifies last item in map.
                                       */
    };
    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_RMS_GET_SMS_WAKE );
    return eRCode;
}
