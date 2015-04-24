/**
 * \ingroup dcs
 *
 * \file    qaGobiApiDcs.c
 *
 * \brief   Entry points for Gobi APIs for the Device Connectivity Service (DCS)
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#include "../../SWIWWANCMAPI.h"
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "amudefs.h"
#include "sludefs.h"
#include "swi_osapi.h"
#include "qaQmiBasic.h"
#include "qaGobiApiDcs.h"
#include "qaDcsQCWWAN2kEnumerateDevices.h"
#include "qaDcsQCWWAN2kConnect.h"
#include "qaDcsQCWWANDisconnect.h"
#include "qaDcsQCWWAN2kGetConnectedDeviceID.h"
#include "qaDcsSLQSGetUsbPortNames.h"
#include "qaDcsSLQSGetDeviceMode.h"
#include "qaQmiNotify.h"

/*
 * Fully qualified path of SLQS executable (includes the executable file’s name)
 */
static LPCSTR pSDKpath = NULL;  /* Note: cannot initialize to default here */

ULONG SLQSKillSDKProcess()
{
    BYTE    *pInParam;      /* ptr to param field rx'd from modem */
    BYTE    *pReqBuf;       /* Pointer to outgoing request buffer */
    USHORT  ParamLength;    /* Ret'd length of the Param field */
    enum eQCWWANError  eRCode;  /* Result of SwiSdkSendnWait() */

    /* Initialize the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* send the request to the SDK process */
    eRCode = SwiSdkSendnWait( pReqBuf,
                              AMSDKREQKILL,
                              ParamLength,
                              eQMI_TIMEOUT_2_S, /* 2 Seconds */
                              &pInParam,
                              &ParamLength );

    /* RILSTART */
    if (eRCode == eQCWWAN_ERR_SWICM_TIMEOUT)
        swi_osapilog("SLQSKillSDKProcess timeout");
    else
        swi_osapilog("SLQSKillSDKProcess with NO timeout");
    /* RILSTOP */

    /* There is nothing to check - the SDK is gone by this time */
    qmrelreqbkp();
    eRCode = eQCWWAN_ERR_NONE;
    return eRCode;
}

ULONG SLQSStart()
{
    enum eQCWWANError eRCode;
    static BOOL baminitapicalled = FALSE;
    /* RILSTART */
    BOOL sdkRunning = FALSE;
    /* RILSTOP */

    /* If caller never called SetSDKImagePath, set it to default here */
    if (pSDKpath == NULL)
        pSDKpath = SDKPATHDEFAULT;

    /* RILSTART */
    sdkRunning = swi_osapiverifySDKstatus();
    /* RILSTOP */

    /* Start up the SDK */
    if( swi_osapisdkprocesscreate( (const char *)pSDKpath ) )
    {
        eRCode = eQCWWAN_ERR_NONE;
    }
    else
    {
        eRCode =  eQCWWAN_ERR_INTERNAL;
        return eRCode;
    }

    /* SDK started. Initialize the IPC channels, calling it once only */
    if (baminitapicalled == FALSE)
    {
        if (!aminitapi())
        {
            /* Unable to reserve an IPC channel for this Application to use
             * for communicating with the SDK process.
             */
            return eQCWWAN_ERR_SWICM_SOCKET_IN_USE;
        }
        baminitapicalled = TRUE;

        /* RILSTART */
        /* 
         * Check if the SDK process was already running, in which case reset
         * the process and return error, so that API can re-enumerate
         */  
        if (sdkRunning) {
            /* SWI_TBD
             * Hopefully, we don't need the old RIL specific function anymore.
             * SwiQmiResetSDK();
             */
            SLQSKillSDKProcess();

            /* return error so that function is called again from the start */
            return eQCWWAN_ERR_NO_CONNECTION;
        }
        /* RILSTOP */
    }

    /* Initialize the QA Package */
    qaInit();
    return eQCWWAN_ERR_NONE;
}

ULONG QCWWAN2kEnumerateDevices(
    BYTE    *pDevicesSize,
    BYTE    *pDevices )
{
    BYTE    *pInParam;      /* ptr to param field rx'd from modem */
    BYTE    *pOutParam;     /* ptr to outbound param field */
    BYTE    *pReqBuf;       /* Pointer to outgoing request buffer */
    USHORT  ParamLength;    /* Ret'd length of the Param field */
    enum eQCWWANError  eRCode;  /* Result of SwiQmiSendnWait() */
    struct qm_qmi_response_tlvs_values response;

    if( pDevicesSize == NULL || *pDevicesSize == 0 )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Pack the QMI request */
    eRCode = PkQmiDcsQCWWAN2kEnumerateDevices( &ParamLength, pOutParam );

    if (eRCode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return eRCode;
    }

    /* send the request to the SDK process */
    eRCode = SwiQmiSendnWait( pReqBuf,
                              eQMI_SVC_DCS,
                              ParamLength,
                              eQMI_TIMEOUT_5_S, /* 5 Seconds */
                              &pInParam,
                              &ParamLength );

    if (eRCode == eQCWWAN_ERR_NONE)
    {
        /* RILSTART - response may contain garbage if TLVs are not unpacked */
        memset( &response, 0, sizeof(response) );
        /* RILSTOP */

        /* extract response data */
        eRCode = UpkQmiDcsQCWWAN2kEnumerateDevices( pInParam,
                                                    &response );

        /* check for unpack error */
        if( eRCode != eQCWWAN_ERR_NONE )
        {
            qmrelreqbkp();
            return eRCode;
        }

        /* Check that the mandatory response TLV was received */
        if( !response.qmiresult.validresponse )
        {
            qmrelreqbkp();
            return eQCWWAN_ERR_INVALID_QMI_RSP;
        }

        /* check mandatory response tlv result code */
        if( response.qmiresult.result != eQCWWAN_ERR_NONE )
        {
            qmrelreqbkp();
            return response.qmiresult.error;
        }

        /* RILSTART - check if both the TLVs were present */
        if ( !response.tlvvalues.qmdcstlvs.enumerate.dev.devnodestrsize ||
             !response.tlvvalues.qmdcstlvs.enumerate.dev.devkeystrsize )
        {
            qmrelreqbkp();
            return eQCWWAN_ERR_INVALID_QMI_RSP;
        }
        /* RILSTOP */

        /* device found */
        *pDevicesSize = 1;

        /* copy device info to caller's buffers */
        slmemcpy( ((struct device_info_param *)pDevices)->deviceNode,
                  response.tlvvalues.qmdcstlvs.enumerate.dev.devnode,
                  response.tlvvalues.qmdcstlvs.enumerate.dev.devnodestrsize );

        slmemcpy( ((struct device_info_param *)pDevices)->deviceKey,
                  response.tlvvalues.qmdcstlvs.enumerate.dev.devkey,
                  response.tlvvalues.qmdcstlvs.enumerate.dev.devkeystrsize );
    }
    qmrelreqbkp();
    return eRCode;
}

ULONG  QCWWAN2kConnect(
    CHAR    *pDeviceID,
    CHAR    *pDeviceKey )
{
    BYTE              *pInParam;    /* ptr to param field rx'd from modem */
    BYTE              *pOutParam;   /* ptr to outbound param field */
    BYTE              *pReqBuf;     /* Pointer to outgoing request buffer */
    USHORT             ParamLength; /* Ret'd length of the Param field */
    enum eQCWWANError  eRCode;      /* Result of SwiQmiSendnWait() */

    struct qm_qmi_response_tlvs_values response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message and sends the message. */
    eRCode = PkQmiDcsQCWWAN2kConnect( &ParamLength,
                                       pOutParam,
                                       pDeviceID,
                                       pDeviceKey);

    if (eRCode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return eRCode;
    }

    /* send the request to the SDK process */
    eRCode = SwiQmiSendnWait( pReqBuf,
                              eQMI_SVC_DCS,
                              ParamLength,
                              eQMI_TIMEOUT_2_S, /* 2 Seconds */
                              &pInParam,
                              &ParamLength );

    if (eRCode == eQCWWAN_ERR_NONE)
    {
        /* unpack QMI response */
        eRCode = qm_dcs_connect_response_unpack( pInParam, &response );

        /* check for unpack error */
        if( eRCode != eQCWWAN_ERR_NONE )
        {
            qmrelreqbkp();
            return eRCode;
        }

        /* Check that the mandatory response TLV was received */
        if( !response.qmiresult.validresponse )
        {
            qmrelreqbkp();
            return eQCWWAN_ERR_INVALID_QMI_RSP;
        }

        /* check mandatory response tlv result code */
        if( response.qmiresult.result != eQCWWAN_ERR_NONE )
        {
            qmrelreqbkp();
            return response.qmiresult.error;
        }
    }
    else
    {
        qmrelreqbkp();
        return eRCode;
    }

    /* Device available */
    qmrelreqbkp();
    return eRCode;
}

ULONG QCWWANDisconnect()
{
    BYTE              *pInParam;    /* ptr to param field rx'd from modem */
    BYTE              *pOutParam;   /* ptr to outbound param field */
    BYTE              *pReqBuf;     /* Pointer to outgoing request buffer */
    USHORT             ParamLength; /* Ret'd length of the Param field */
    enum eQCWWANError  eRCode;      /* Result of SwiQmiSendnWait() */

    struct QmiDcsQCWWANDisconnectResp response;

    /* Deregister all callbacks */
    qaQmiRemoveAllCallbacks();

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message and sends the message. */
    eRCode = PkQmiDcsQCWWANDisconnect( &ParamLength,
                                       pOutParam);

    if (eRCode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return eRCode;
    }

     /* send the request to the SDK process */
     eRCode = SwiQmiSendnWait( pReqBuf,
                              eQMI_SVC_DCS,
                              ParamLength,
                              eQMI_TIMEOUT_2_S, /* 2 Seconds */
                              &pInParam,
                              &ParamLength );

    if( eRCode == eQCWWAN_ERR_NONE )
    {
        /* Copy to the caller's buffer */
        eRCode = UpkQmiDcsQCWWANDisconnect( pInParam, &response );

        /* check for unpack error */
        if( eRCode != eQCWWAN_ERR_NONE )
        {
            qmrelreqbkp();
            return eRCode;
        }

        /* Check that the mandatory response TLV was received */
        if( !response.results.TlvResultCode )
        {
            qmrelreqbkp();
            return eQCWWAN_ERR_INVALID_QMI_RSP;
        }

        /* check mandatory response tlv result code */
        if( response.results.DeviceResult != eQCWWAN_ERR_NONE )
        {
            qmrelreqbkp();
            return response.results.DeviceError;
        }
    }
    qmrelreqbkp();
    return eRCode;
}

ULONG QCWWAN2kGetConnectedDeviceID(
    ULONG   deviceIDSize,
    CHAR    *pDeviceID,
    ULONG   deviceKeySize,
    CHAR    *pDeviceKey )
{
    BYTE              *pInParam;    /* ptr to param field rx'd from modem */
    BYTE              *pOutParam;   /* ptr to outbound param field */
    BYTE              *pReqBuf;     /* Pointer to outgoing request buffer */
    USHORT             ParamLength; /* Ret'd length of the Param field */
    enum eQCWWANError  eRCode;      /* Result of SwiQmiSendnWait() */

    struct qm_qmi_response_tlvs_values response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message and sends the message. */
    eRCode = PkQmiDcsQCWWAN2kGetConnectedDeviceID( &ParamLength,
                                                   pOutParam);

    if (eRCode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return eRCode;
    }

    /* send the request to the SDK process */
    eRCode = SwiQmiSendnWait( pReqBuf,
                              eQMI_SVC_DCS,
                              ParamLength,
                              eQMI_TIMEOUT_2_S, /* 2 Seconds */
                              &pInParam,
                              &ParamLength );

    if( eRCode == eQCWWAN_ERR_NONE )
    {
        /* unpack response from SDK */
        eRCode = UpkQmiDcsQCWWAN2kGetConnectedDeviceID( pInParam,
                                                        &response );

        /* check for unpack error */
        if( eRCode != eQCWWAN_ERR_NONE )
        {
            qmrelreqbkp();
            return eRCode;
        }

        /* Check that the mandatory response TLV was received */
        if( !response.qmiresult.validresponse )
        {
            qmrelreqbkp();
            return eQCWWAN_ERR_INVALID_QMI_RSP;
        }

        /* check mandatory response tlv result code */
        if( response.qmiresult.result != eQCWWAN_ERR_NONE )
        {
            qmrelreqbkp();
            return response.qmiresult.error;
        }

        /* check that client buffers are large enough for response data */
        if( deviceIDSize < response.tlvvalues.qmdcstlvs.enumerate.dev.devnodestrsize ||
            deviceKeySize < response.tlvvalues.qmdcstlvs.enumerate.dev.devkeystrsize )
        {
            qmrelreqbkp();
            return eQCWWAN_ERR_BUFFER_SZ;
        }

        /* Copy to the caller's buffers */
        slmemcpy( pDeviceID,
                  response.tlvvalues.qmdcstlvs.enumerate.dev.devnode,
                  response.tlvvalues.qmdcstlvs.enumerate.dev.devnodestrsize );

        slmemcpy( pDeviceKey,
                  response.tlvvalues.qmdcstlvs.enumerate.dev.devkey,
                  response.tlvvalues.qmdcstlvs.enumerate.dev.devkeystrsize );
    }
    qmrelreqbkp();
    return eRCode;
}

ULONG SetSDKImagePath (
    LPCSTR pPath )
{
    /* Set the executable path to default if not provided */
    pSDKpath = (LPCSTR) (pPath == NULL) ? SDKPATHDEFAULT : pPath;

    return eQCWWAN_ERR_NONE;
}

ULONG  SLQSGetUsbPortNames(
    struct DcsUsbPortNames *pUsbPortNames )
{
    BYTE              *pInParam;    /* ptr to param field rx'd from modem */
    BYTE              *pOutParam;   /* ptr to outbound param field */
    BYTE              *pReqBuf;     /* Pointer to outgoing request buffer */
    USHORT             ParamLength; /* Ret'd length of the Param field */
    enum eQCWWANError  eRCode;      /* Result of SwiQmiSendnWait() */

    struct qm_qmi_response_tlvs_values response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message and sends the message. */
    eRCode = PkQmiDcsSLQSGetUsbPortNames ( &ParamLength,
                                           pOutParam );
    if (eRCode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return eRCode;
    }

    /* send the request to the SDK process */
    eRCode = SwiQmiSendnWait( pReqBuf,
                              eQMI_SVC_DCS,
                              ParamLength,
                              eQMI_TIMEOUT_2_S, /* 2 Seconds */
                              &pInParam,
                              &ParamLength );

    if (eRCode == eQCWWAN_ERR_NONE)
    {
        /* unpack QMI response */
        slmemset ( (char *)&response,
                   EOS,
                   sizeof (struct qm_qmi_response_tlvs_values));

        eRCode = UpkQmiDcsSLQSGetUsbPortNames ( pInParam, &response );

        /* check for unpack error */
        if( eRCode != eQCWWAN_ERR_NONE )
        {
            qmrelreqbkp();
            return eRCode;
        }

        /* Check that the mandatory response TLV was received */
        if( !response.qmiresult.validresponse )
        {
            qmrelreqbkp();
            return eQCWWAN_ERR_INVALID_QMI_RSP;
        }

        /* check mandatory response tlv result code */
        if( response.qmiresult.result != eQCWWAN_ERR_NONE )
        {
            qmrelreqbkp();
            return response.qmiresult.error;
        }
    }
    else
    {
        qmrelreqbkp();
        return eRCode;
    }

    /* copy Port Names to caller's buffers */
    slmemcpy( pUsbPortNames->AtCmdPort,
              response.tlvvalues.qmdcstlvs.portnames.usbportnames.AtCmdPort,
              strlen(response.tlvvalues.qmdcstlvs.portnames.usbportnames.AtCmdPort) );

    slmemcpy( pUsbPortNames->NmeaPort,
              response.tlvvalues.qmdcstlvs.portnames.usbportnames.NmeaPort,
              strlen(response.tlvvalues.qmdcstlvs.portnames.usbportnames.NmeaPort) );

    slmemcpy( pUsbPortNames->DmPort,
              response.tlvvalues.qmdcstlvs.portnames.usbportnames.DmPort,
              strlen(response.tlvvalues.qmdcstlvs.portnames.usbportnames.DmPort) );

    qmrelreqbkp();
    return eRCode;
}

ULONG  SLQSGetDeviceMode(
    BYTE *pDeviceMode )
{
    BYTE              *pInParam;    /* ptr to param field rx'd from modem */
    BYTE              *pOutParam;   /* ptr to outbound param field */
    BYTE              *pReqBuf;     /* Pointer to outgoing request buffer */
    USHORT             ParamLength; /* Ret'd length of the Param field */
    enum eQCWWANError  eRCode;      /* Result of SwiQmiSendnWait() */

    struct qm_qmi_response_tlvs_values response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message and sends the message. */
    eRCode = PkQmiDcsSLQSGetDeviceMode ( &ParamLength,
                                           pOutParam );
    if (eRCode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return eRCode;
    }

    /* send the request to the SDK process */
    eRCode = SwiQmiSendnWait( pReqBuf,
                              eQMI_SVC_DCS,
                              ParamLength,
                              eQMI_TIMEOUT_2_S, /* 2 Seconds */
                              &pInParam,
                              &ParamLength );

    if (eRCode == eQCWWAN_ERR_NONE)
    {
        /* unpack QMI response */
        eRCode = UpkQmiDcsSLQSGetDeviceMode ( pInParam, &response );

        /* check for unpack error */
        if( eRCode != eQCWWAN_ERR_NONE )
        {
            qmrelreqbkp();
            return eRCode;
        }

        /* Check that the mandatory response TLV was received */
        if( !response.qmiresult.validresponse )
        {
            qmrelreqbkp();
            return eQCWWAN_ERR_INVALID_QMI_RSP;
        }

        /* check mandatory response tlv result code */
        if( response.qmiresult.result != eQCWWAN_ERR_NONE )
        {
            qmrelreqbkp();
            return response.qmiresult.error;
        }
    }
    else
    {
        qmrelreqbkp();
        return eRCode;
    }

    *pDeviceMode = response.tlvvalues.qmdcstlvs.devicestate.devstate;

    qmrelreqbkp();
    return eRCode;
}

/* Deprecated APIs */
ULONG  QCWWANEnumerateDevices(
    BYTE    *pDevicesSize,
    BYTE    *pDevices )
{
    return QCWWAN2kEnumerateDevices( pDevicesSize, pDevices );
}

ULONG  QCWWANConnect(
    CHAR *pDeviceID,
    CHAR *pDeviceKey)
{
    /* Call the newer API */
    return ( QCWWAN2kConnect( pDeviceID, pDeviceKey ) );
}
