/*
 * \ingroup cbk
 *
 * \file    qaCbkDmsEventReportInd.c
 *
 * \brief   Contains UnPacking routines for the
 *          QMI_DMS_EVENT_REPORT_IND message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCbkDmsEventReportInd.h"

/* Functions */

/*
 * This function unpacks the Power State from the
 * QMI indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvPowerState(
    BYTE *pTlvData,
    BYTE *pResp )
{
    UNUSEDPARAM( pTlvData );
    UNUSEDPARAM( pResp );

    /* To be completed when implementing the related Call-back */
    return eQCWWAN_ERR_NONE;
}

/*
 * This function unpacks the Battery Level from the
 * QMI indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvBatteryLevel(
    BYTE *pTlvData,
    BYTE *pResp )
{
    UNUSEDPARAM( pTlvData );
    UNUSEDPARAM( pResp );

    /* To be completed when implementing the related Call-back */
    return eQCWWAN_ERR_NONE;
}

/*
 * This function unpacks the Pin State from the
 * QMI indication message to a user provided response structure
 *
 * \param  pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
package enum eQCWWANError UnpackCbkTlvPinState(
    BYTE *pTlvData,
    BYTE *pResp )
{
    UNUSEDPARAM( pTlvData );
    UNUSEDPARAM( pResp );

    /* To be completed when implementing the related Call-back */
    return eQCWWAN_ERR_NONE;
}

/*
 * This function unpacks the Activation Status TLV from the QMI indication
 * message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvActivationStatus(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    struct ActivationStatusTlv *lResp =
        &((struct QmiCbkDmsEventStatusReportInd *)pResp)->ASTlv;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    BYTE lByte = 0;

    /* Extract the activation Status */
    eRCode = GetByte( pTlvData, &lByte );
    if ( eRCode == eQCWWAN_ERR_NONE )
    {
        lResp->activationStatus = lByte;
        lResp->TlvPresent = TRUE;
    }
    return eRCode;

}

/*
 * This function unpacks the Operating Mode TLV from the QMI indication
 * message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvOperatingMode(
    BYTE *pTlvData,
    BYTE *pResp )
{
	/* Get the required TLV structure to the local pointer */ 
	struct OperatingModeTlv *lResp =
        &((struct QmiCbkDmsEventStatusReportInd *)pResp)->OMTlv;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE lByte = 0;

    /* Extract the activation Status */
    eRCode = GetByte( pTlvData, &lByte );
    if ( eRCode == eQCWWAN_ERR_NONE )
    {
        lResp->operatingMode = lByte;
        lResp->TlvPresent = TRUE;
    }
    return eRCode;
}

/*
 * This function unpacks the UIM State from the
 * QMI indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvUIMState(
    BYTE *pTlvData,
    BYTE *pResp )
{
    UNUSEDPARAM( pTlvData );
    UNUSEDPARAM( pResp );

    /* To be completed when implementing the related Call-back */
    return eQCWWAN_ERR_NONE;
}

/*
 * This function unpacks the Wireless Disable State from the
 * QMI indication message to a user provided response structure
 *
 * \param  pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
package enum eQCWWANError UnpackCbkTlvWirelessDisableState(
    BYTE *pTlvData,
    BYTE *pResp )
{
    UNUSEDPARAM( pTlvData );
    UNUSEDPARAM( pResp );

    /* To be completed when implementing the related Call-back */
    return eQCWWAN_ERR_NONE;
}

/*
 * This function unpacks the Event Report Indication message to a
 * user-provided response structure.
 *
 * \param   pMdmResp - Pointer to packed response from the modem.
 *
 * \param   pApiResp - Pointer to storage to unpack into.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UpkQmiCbkDmsEventReportInd(
    BYTE                                 *pMdmResp,
    struct QmiCbkDmsEventStatusReportInd *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_IND_POWER_STATE,            &UnpackCbkTlvPowerState },
        { eTLV_IND_BATTERY_LEVEL,          &UnpackCbkTlvBatteryLevel },
        { eTLV_IND_PIN_STATE,              &UnpackCbkTlvPinState },
        { eTLV_IND_ACTIVATION_STATUS,      &UnpackCbkTlvActivationStatus },
        { eTLV_IND_OPERATING_MODE,         &UnpackCbkTlvOperatingMode },
        { eTLV_IND_UIM_STATE,              &UnpackCbkTlvUIMState },
        { eTLV_IND_WIRELESS_DISABLE_STATE, &UnpackCbkTlvWirelessDisableState },
        { eTLV_TYPE_INVALID,               NULL } /* Important. Sentinel.
                                                   * Signifies last item in map.
                                                   */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_DMS_EVENT_IND );
    return eRCode;
}
