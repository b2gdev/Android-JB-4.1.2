/*
 * \ingroup cbk
 *
 * \file qaDmsSetEventReport.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        DMS_SET_EVENT_REPORT message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCbkDmsSetEventReport.h"

/* Functions */

/* Request handlers */

/*
 * This function packs the Power State field to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
local enum eQCWWANError BuildTlvPowerState(
    BYTE *pBuf,
    BYTE *pParam )
{
    struct QmiCbkDmsSetEventReportReq *pReq =
        (struct QmiCbkDmsSetEventReportReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* If the parameter has not been set do not fill the TLV */
    if (QMI_CBK_PARAM_NOCHANGE == pReq->ePowerState)
        return eRCode;

    eRCode = PutByte( pBuf, pReq->ePowerState );
    return eRCode;
}

/*
 * This function packs the Battery Level field to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
local enum eQCWWANError BuildTlvBatteryLevel(
    BYTE *pBuf,
    BYTE *pParam )
{
    struct QmiCbkDmsSetEventReportReq *pReq =
        (struct QmiCbkDmsSetEventReportReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* If the parameter has not been set do not fill the TLV */
    if (QMI_CBK_PARAM_NOCHANGE == pReq->eBatteryLevel)
        return eRCode;

    eRCode = PutByte( pBuf, pReq->eBatteryLevel);
    return eRCode;
}

/*
 * This function packs the Pin State field to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
local enum eQCWWANError BuildTlvPinState(
    BYTE *pBuf,
    BYTE *pParam )
{
    struct QmiCbkDmsSetEventReportReq *pReq =
        (struct QmiCbkDmsSetEventReportReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* If the parameter has not been set do not fill the TLV */
    if (QMI_CBK_PARAM_NOCHANGE == pReq->ePinState)
        return eRCode;

    eRCode = PutByte( pBuf, pReq->ePinState );
    return eRCode;
}

/*
 * This function packs the Activation Status field to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
local enum eQCWWANError BuildTlvActivationStatus(
    BYTE *pBuf,
    BYTE *pParam )
{
    struct QmiCbkDmsSetEventReportReq *pReq =
        (struct QmiCbkDmsSetEventReportReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* If the parameter has not been set do not fill the TLV */
    if (QMI_CBK_PARAM_NOCHANGE == pReq->eActivationStatus)
        return eRCode;

    eRCode = PutByte( pBuf, pReq->eActivationStatus );
    return eRCode;
}

/*
 * This function packs the Operating Mode field to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
local enum eQCWWANError BuildTlvOperatingMode(
    BYTE *pBuf,
    BYTE *pParam )
{
    struct QmiCbkDmsSetEventReportReq *pReq =
        (struct QmiCbkDmsSetEventReportReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* If the parameter has not been set do not fill the TLV */
    if (QMI_CBK_PARAM_NOCHANGE == pReq->eOperatingMode)
        return eRCode;

    eRCode = PutByte( pBuf, pReq->eOperatingMode );
    return eRCode;
}

/*
 * This function packs the UIM State field to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
local enum eQCWWANError BuildTlvUIMState(
    BYTE *pBuf,
    BYTE *pParam )
{
    struct QmiCbkDmsSetEventReportReq *pReq =
        (struct QmiCbkDmsSetEventReportReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* If the parameter has not been set do not fill the TLV */
    if (QMI_CBK_PARAM_NOCHANGE == pReq->eUIMState)
        return eRCode;

    eRCode = PutByte( pBuf, pReq->eUIMState );
    return eRCode;
}

/*
 * This function packs the Wireless Disable State field to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
local enum eQCWWANError BuildTlvWirelessDisableState(
    BYTE *pBuf,
    BYTE *pParam )
{
    struct QmiCbkDmsSetEventReportReq *pReq =
        (struct QmiCbkDmsSetEventReportReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* If the parameter has not been set do not fill the TLV */
    if (QMI_CBK_PARAM_NOCHANGE == pReq->eWirelessDisableState)
        return eRCode;

    eRCode = PutByte( pBuf, pReq->eWirelessDisableState );
    return eRCode;
}

/*
 * This function packs the DMS Set Event Report parameters to the
 * QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \param  ePowerState           [IN] - Set/Reset
 *
 * \param  eBatteryLevel         [IN] - Set/Reset
 *
 * \param  ePinState             [IN] - Set/Reset
 *
 * \param  eActivationStatus     [IN] - Set/Reset
 *
 * \param  eOperatingMode        [IN] - Set/Reset
 *
 * \param  eUIMState             [IN] - Set/Reset
 *
 * \param  eWirelessDisableState [IN] - Set/Reset
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
package enum eQCWWANError PkQmiCbkDmsSetEventReport(
    WORD                  *pMlength,
    enum eQmiCbkSetStatus ePowerState,
    enum eQmiCbkSetStatus eBatteryLevel,
    enum eQmiCbkSetStatus ePinState,
    enum eQmiCbkSetStatus eActivationStatus,
    enum eQmiCbkSetStatus eOperatingMode,
    enum eQmiCbkSetStatus eUIMState,
    enum eQmiCbkSetStatus eWirelessDisableState,
    BYTE                  *pParamField )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_POWER_STATE,            &BuildTlvPowerState },
        { eTLV_BATTERY_LEVEL,          &BuildTlvBatteryLevel },
        { eTLV_PIN_STATE,              &BuildTlvPinState },
        { eTLV_ACTIVATION_STATUS,      &BuildTlvActivationStatus },
        { eTLV_OPERATING_MODE,         &BuildTlvOperatingMode },
        { eTLV_UIM_STATE,              &BuildTlvUIMState },
        { eTLV_WIRELESS_DISABLE_STATE, &BuildTlvWirelessDisableState },
        { eTLV_TYPE_INVALID,           NULL } /* Important. Sentinel.
                                               * Signifies last item in map.
                                               */
    };

    struct QmiCbkDmsSetEventReportReq req;
    slmemset( (CHAR *)&req,
              0,
              sizeof (struct QmiCbkDmsSetEventReportReq) );

    req.ePowerState           = ePowerState;
    req.eBatteryLevel         = eBatteryLevel;
    req.ePinState             = ePinState;
    req.eActivationStatus     = eActivationStatus;
    req.eOperatingMode        = eOperatingMode;
    req.eUIMState             = eUIMState;
    req.eWirelessDisableState = eWirelessDisableState;


    enum eQCWWANError eRCode;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_DMS_SET_EVENT,
                      pMlength );

    return eRCode;
}

/* Response handlers */

/*
 * This function unpacks the DMS_SET_EVENT_REPORT response
 * message to a user-provided response structure.
 *
 * \param  pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
package enum eQCWWANError UpkQmiCbkDmsSetEventReport(
    BYTE                               *pMdmResp,
    struct QmiCbkDmsSetEventReportResp *pApiResp )
{
    ULONG eRCode;

    /* The unpack of the Callback contains only result code */
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
                           eQMI_DMS_SET_EVENT );
    return eRCode;
}

