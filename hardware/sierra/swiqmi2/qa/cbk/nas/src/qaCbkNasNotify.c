/*
 * \ingroup cbk
 *
 * \file    qaCbkNasNotify.c
 *
 * \brief   Contains routines for the NAS Notifications.
 *
 * Copyright: © 2011-2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaGobiApiCbk.h"
#include "qaCbkNasEventReportInd.h"
#include "qaCbkNasServingSystemInd.h"
#include "qaCbkNasSystemSelectionPreferenceInd.h"
#include "qaQmiNotify.h"
#include "sludefs.h"

/* Functions */

/*************
 *
 * Name:    qaQmiNasServingSystemNotify
 *
 * Purpose: Unpacks the received NAS indication and invokes the appropriate
 *          callback based on the QMI message type.
 *
 * Parms:   QmiIndication [IN] - Structure used to store all QMI Notification
 *                               parameters.
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
package void qaQmiNasServingSystemNotify(
    struct QmiNotification *pQmiIndication )
{
    void                    *pCallback;
    enum  eQMICallbackIndex CbkIndex;
    ULONG                   CbkSSParamOne;
    BYTE                    CbkDCParamOne;
    BYTE                    *CbkDCParamTwo;

    CbkIndex  = eQMI_CB_ROAMING_INDICATOR;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        struct RoamingIndicationTlv *pResp =
                &pQmiIndication->QmiInd.qaQmiCbkNasServingSystemInd.RITlv;

        /* The TLV was present in the Indication - hence process */
        /* Included for TLVs which are not implemented */
        if ( pResp->TlvPresent )
        {
            /* Extract the Parameters */
            CbkSSParamOne = pResp->roaming_ind;

            /* Invoke the callback */
            ( (tFNRoamingIndicator)pCallback)( CbkSSParamOne );
        }
    }

    CbkIndex  = eQMI_CB_DATA_CAPABILITIES;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        struct DataCapabilitiesTlv *pResp =
                &pQmiIndication->QmiInd.qaQmiCbkNasServingSystemInd.DCTlv;

        /* The TLV was present in the Indication - hence process */
        /* Included for TLVs which are not implemented */
        if ( pResp->TlvPresent )
        {
            /* Extract the Parameters */
            CbkDCParamOne = pResp->datacaps_len;
            CbkDCParamTwo = pResp->datacaps;
            /* Invoke the callback */
            ( (tFNDataCapabilities)pCallback)( CbkDCParamOne, CbkDCParamTwo );
        }
    }

    CbkIndex  = eQMI_CB_NAS_SERV_SYS;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        struct ServingSystemTlv *pResp =
            &pQmiIndication->QmiInd.qaQmiCbkNasServingSystemInd.SSTlv;

        /* The TLV was present in the Indication - hence process */
        /* Included for TLVs which are not implemented */
        if ( pResp->TlvPresent )
        {
            /* Invoke the callback */
            ( (tFNServingSystem)pCallback)( &pResp->ServingSystem );
        }
    }
}

/*********
 * qaQmiNasEventNotify
 *
 * Purpose: To notify all the Callbacks associated with the NAS Event Report
 *          Notification.
 *
 * Parms:   pQmiIndication  [IN] - pointer to structure used to store all QMI
 *                                 Notification parameters.
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   Must be called *after* qminit() has been called
 *
 **************/
local void qaQmiNasEventNotify(
    struct QmiNotification *pQmiIndication )
{
    void *pCallback;
    enum eQMICallbackIndex CbkIndex;

    CbkIndex = eQMI_CB_SIGNAL_STRENGTH;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        INT8 CbkParamOne;
        BYTE CbkParamTwo;

        struct SignalStrengthTlv *pResp =
              &pQmiIndication->QmiInd.qaQmiCbkNasEventStatusReportInd.SSTlv;
        if ( pResp->TlvPresent )
        {
            /* Extract the Parameters */
            CbkParamOne = pResp->signalStrength;
            CbkParamTwo = pResp->radioInterface;

            /* Invoke the callback */
            ((tFNSignalStrength)pCallback) ( CbkParamOne, CbkParamTwo);
        }
    }

    CbkIndex  = eQMI_CB_RF_INFO;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        ULONG CbkParamOne;
        ULONG CbkParamTwo;
        ULONG CbkParamThree;

        struct RFInfoTlv *pResp =
            &pQmiIndication->QmiInd.qaQmiCbkNasEventStatusReportInd.RFTlv;

        if ( pResp->TlvPresent )
        {
            BYTE lIndex = pResp->radioInterfaceSize;
            while ( lIndex-- )
            {
                /* Extract the Parameters */
                CbkParamOne   = pResp->radioInterface[lIndex];
                CbkParamTwo   = pResp->activeBandClass[lIndex];
                CbkParamThree = pResp->activeChannel[lIndex];

                /* Invoke the callback */
                ((tFNRFInfo)pCallback) ( CbkParamOne,
                                         CbkParamTwo,
                                         CbkParamThree );
            }
        }
    }

    CbkIndex  = eQMI_CB_LU_REJECT;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        ULONG CbkParamOne;
        ULONG CbkParamTwo;

        struct RejectReasonTlv *pResp =
              &pQmiIndication->QmiInd.qaQmiCbkNasEventStatusReportInd.RRTlv;

        if ( pResp->TlvPresent )
        {
            /* Extract the Parameters */
            CbkParamOne = pResp->serviceDomain;
            CbkParamTwo = pResp->rejectCause;

            /* Invoke the callback */
            ((tFNLUReject)pCallback) ( CbkParamOne,
                                       CbkParamTwo );
        }
    }

    CbkIndex  = eQMI_CB_RSSI_INFO;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        struct SLQSSignalStrengthsInformation CbkParamOne;

        struct SLQSSignalStrengthsTlv *pResp =
             &pQmiIndication->QmiInd.qaQmiCbkNasEventStatusReportInd.SLQSSSTlv;

        if ( pResp->TlvPresent )
        {
            /* Extract the Parameters */
            slmemcpy( (void *)&CbkParamOne,
                      (void *)&pResp->sSLQSSignalStrengthsInfo,
                      sizeof( CbkParamOne ) );

            /* Invoke the callback */
            ((tFNSLQSSignalStrengths)pCallback) ( CbkParamOne );
        }
    }
}

/*************
 *
 * Name:    qaQmiNasSystemSelectionPreferenceNotify
 *
 * Purpose: Unpacks the received NAS indication and invokes the appropriate
 *          callback based on the QMI message type.
 *
 * Parms:   QmiIndication [IN] - Structure used to store all QMI Notification
 *                               parameters.
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
package void qaQmiNasSystemSelectionPreferenceNotify(
    struct QmiNotification *pQmiIndication )
{
    void *pCallback;
    enum eQMICallbackIndex CbkIndex;
    ULONGLONG CbkParamOne;

    CbkIndex  = eQMI_CB_NAS_BAND_PREF;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        struct BandPreferenceTlv *pResp =
                &pQmiIndication->QmiInd.qaQmiCbkNasSystemSelPrefInd.BPTlv;

        /* The TLV was present in the Indication - hence process */
        /* Included for TLVs which are not implemented */
        if ( pResp->TlvPresent )
        {
            /* Extract the Parameters */
            CbkParamOne = pResp->band_pref;

            /* Invoke the callback */
            ( (tFNBandPreference)pCallback)( CbkParamOne );
        }
    }

    CbkIndex  = eQMI_CB_NAS_SYS_SEL_PREFERENCE;
    pCallback = qaQmiGetCallback( CbkIndex );
    if( pCallback )
    {
        sysSelectPrefInfo SysSelectPrefInfo;

        /* Initialize the data structure */
        slmemset( (char*) &SysSelectPrefInfo, 0, sizeof( sysSelectPrefInfo) );

        /* Fill the structure with values received */
        struct EmergencyModeTlv *pEModeResp =
                &pQmiIndication->QmiInd.qaQmiCbkNasSystemSelPrefInd.EMTlv;
        /* The TLV was present in the Indication - hence process */
        if( pEModeResp->TlvPresent )
        {
            SysSelectPrefInfo.pEmerMode = &(pEModeResp->EmerMode);
        }

        struct ModePreferenceTlv *pModePrefResp =
                &pQmiIndication->QmiInd.qaQmiCbkNasSystemSelPrefInd.MPTlv;
        /* The TLV was present in the Indication - hence process */
        if( pModePrefResp->TlvPresent )
        {
            SysSelectPrefInfo.pModePref = &(pModePrefResp->ModePref);
        }

        struct BandPreferenceTlv *pBandPrefResp =
                   &pQmiIndication->QmiInd.qaQmiCbkNasSystemSelPrefInd.BPTlv;
        /* The TLV was present in the Indication - hence process */
        if( pBandPrefResp->TlvPresent )
        {
            SysSelectPrefInfo.pBandPref = &(pBandPrefResp->band_pref);
        }

        struct PRLPreferenceTlv *pPRLPrefResp =
                   &pQmiIndication->QmiInd.qaQmiCbkNasSystemSelPrefInd.PRLPTlv;
        /* The TLV was present in the Indication - hence process */
        if( pPRLPrefResp->TlvPresent )
        {
            SysSelectPrefInfo.pPRLPref = &(pPRLPrefResp->PRLPref);
        }

        struct RoamPreferenceTlv *pRoamPrefResp =
                   &pQmiIndication->QmiInd.qaQmiCbkNasSystemSelPrefInd.RPTlv;
        /* The TLV was present in the Indication - hence process */
        if( pRoamPrefResp->TlvPresent )
        {
            SysSelectPrefInfo.pRoamPref = &(pRoamPrefResp->RoamPref);
        }

        struct LTEBandPreferenceTlv *pLTEBPResp =
                   &pQmiIndication->QmiInd.qaQmiCbkNasSystemSelPrefInd.LBPTlv;
        /* The TLV was present in the Indication - hence process */
        if( pLTEBPResp->TlvPresent )
        {
            SysSelectPrefInfo.pLTEBandPref = &(pLTEBPResp->LTEBandPref);
        }

        struct NetSelPreferenceTlv *pNSPResp =
                   &pQmiIndication->QmiInd.qaQmiCbkNasSystemSelPrefInd.NSPTlv;
        /* The TLV was present in the Indication - hence process */
        if( pNSPResp->TlvPresent )
        {
            SysSelectPrefInfo.pNetSelPref = &(pNSPResp->NetSelPref);
        }

        struct ServDomainPrefTlv *pSDPResp =
                   &pQmiIndication->QmiInd.qaQmiCbkNasSystemSelPrefInd.SDPTlv;
        /* The TLV was present in the Indication - hence process */
        if( pSDPResp->TlvPresent )
        {
            SysSelectPrefInfo.pSrvDomainPref = &(pSDPResp->SrvDomainPref);
        }

        struct GWAcqOrderPrefTlv *pGWAOPResp =
                   &pQmiIndication->QmiInd.qaQmiCbkNasSystemSelPrefInd.GWAOPTlv;
        /* The TLV was present in the Indication - hence process */
        if( pGWAOPResp->TlvPresent )
        {
            SysSelectPrefInfo.pGWAcqOrderPref = &(pGWAOPResp->GWAcqOrderPref);
        }

        /* Invoke the callback */
        ( (tFNSysSelectionPref)pCallback) ( &SysSelectPrefInfo );
    }
}

/*************
 *
 * Name:    qaQmiNasSysInfoNotify
 *
 * Purpose: To notify all the Callbacks associated with the Sys Info event.
 *
 * Parms:   nasSysInfo  [IN] - pointer to structure used to store all nas
 *                             System Information
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void qaQmiNasSysInfoNotify(
    nasSysInfo *pResp )
{
    void  *pCallback;
    enum  eQMICallbackIndex CbkIndex;

    CbkIndex  = eQMI_CB_NAS_SYS_INFO_IND;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        /* Invoke the callback */
        ((tFNSysInfo)pCallback) ( pResp );
    }
}

/*************
 *
 * Name:    qaQmiNasNetworkTimeNotify
 *
 * Purpose: To notify all the Callbacks associated with the Network Time event.
 *
 * Parms:   nasNetworkTime  [IN] - pointer to structure used to store the nas
 *                                 Network Time
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void qaQmiNasNetworkTimeNotify(
    nasNetworkTime *pResp )
{
    void  *pCallback;
    enum  eQMICallbackIndex CbkIndex;

    CbkIndex  = eQMI_CB_NAS_NETWORK_TIME_IND;
    pCallback = qaQmiGetCallback( CbkIndex );
    if ( pCallback )
    {
        /* Invoke the callback */
        ((tFNNetworkTime)pCallback) ( pResp );
    }
}

/*************
 *
 * Name:    UpkQmiCbkNasNotification
 *
 * Purpose: Unpacks the received NAS indication and invokes the appropriate
 *          callback based on the QMI message type.
 *
 * Parms:   QmiMsgID   [IN] - QMI Message ID
 *          pMdmResp   [IN] - Pointer to packed response from the modem.
 *          pNotifResp [IN] - Notification Structure to fill in the parameters.
 *
 * Return:  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
package enum eQCWWANError UpkQmiCbkNasNotification(
    USHORT                 lQmiMessageID,
    BYTE                   *pMdmResp,
    struct QmiNotification *pNotifResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    switch (lQmiMessageID)
    {
        case eQMI_NAS_SS_INFO_IND:
        {
            struct QmiCbkNasServingSystemInd *pResp =
                &pNotifResp->QmiInd.qaQmiCbkNasServingSystemInd;

            /* Set all the TLVs to be in Not-Present State */
            pResp->RITlv.TlvPresent = FALSE;
            pResp->DCTlv.TlvPresent = FALSE;
            pResp->SSTlv.TlvPresent = FALSE;

            /* Unpack the NAS Serving System Indication */
            eRCode = UpkQmiCbkNasServingSystemInd( pMdmResp, pResp );
            /* Notify to the Callbacks associated */
            qaQmiNasServingSystemNotify( pNotifResp );
            break;
        }
        case eQMI_NAS_EVENT_IND:
        {
            struct QmiCbkNasEventStatusReportInd *pResp =
                  &pNotifResp->QmiInd.qaQmiCbkNasEventStatusReportInd;

            /* Set all the TLVs to be in Not-Present State */
            pResp->SSTlv.TlvPresent     = FALSE;
            pResp->RFTlv.TlvPresent     = FALSE;
            pResp->RRTlv.TlvPresent     = FALSE;
            pResp->SLQSSSTlv.TlvPresent = FALSE;

            /* Unpack the NAS Event Indication */
            eRCode = UpkQmiCbkNasEventReportInd( pMdmResp,
                                                 pResp );

            /* Notify to the Callbacks associated */
            qaQmiNasEventNotify( pNotifResp );
            break;
        }
        case eQMI_NAS_SYS_SELECT_IND:
        {
            struct QmiCbkNasSystemSelPrefInd *pResp =
                  &pNotifResp->QmiInd.qaQmiCbkNasSystemSelPrefInd;

            /* Set all the TLVs to be in Not-Present State */
            pResp->EMTlv.TlvPresent    = FALSE;
            pResp->MPTlv.TlvPresent    = FALSE;
            pResp->BPTlv.TlvPresent    = FALSE;
            pResp->PRLPTlv.TlvPresent  = FALSE;
            pResp->RPTlv.TlvPresent    = FALSE;
            pResp->LBPTlv.TlvPresent   = FALSE;
            pResp->NSPTlv.TlvPresent   = FALSE;
            pResp->SDPTlv.TlvPresent   = FALSE;
            pResp->GWAOPTlv.TlvPresent = FALSE;

            /* Unpack the NAS Event Indication */
            eRCode = UpkQmiCbkNasSystemSelPrefInd( pMdmResp,
                                                   pResp );
            /* Notify to the Callbacks associated */
            qaQmiNasSystemSelectionPreferenceNotify( pNotifResp );
            break;
        }
        case eQMI_NAS_SYS_INFO_IND:
        {
            nasSysInfo *pResp = &pNotifResp->QmiInd.qaQmiCbkNasSysInfo;

            /* Set all the TLVs to be in Not-Present State */
            memset( pResp, 0, sizeof(nasSysInfo));

            /* Unpack the System Information Indication */
            eRCode = UpkQmiCbkNasSysInfoInd( pMdmResp,
                                             pResp );

            /* Notify to the Callbacks associated */
            qaQmiNasSysInfoNotify( pResp );
            break;
        }
        case eQMI_NAS_NETWORK_TIME_IND:
        {
            nasNetworkTime *pResp = &pNotifResp->QmiInd.qaQmiCbkNasNetworkTime;

            /* Set all the TLVs to be in Not-Present State */
            pResp->pTimeZone    = NULL;
            pResp->pDayltSavAdj = NULL;

            /* Unpack the System Information Indication */
            eRCode = UpkQmiCbkNasNetworkTimeInd( pMdmResp,
                                                 pResp );

            /* Notify to the Callbacks associated */
            qaQmiNasNetworkTimeNotify( pResp );
            break;
        }
        default:
        {
            eRCode = eQCWWAN_ERR_INVALID_ARG;
            break;
        }
    }
    return eRCode;
}

