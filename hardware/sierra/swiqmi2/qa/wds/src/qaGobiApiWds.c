/*
 * \ingroup wds
 *
 * \file qaGobiApiWds.c
 *
 * \brief  Entry points for Gobi APIs for the Wireless Data Service (WDS)
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "amudefs.h"
#include "sludefs.h"
#include "qaQmiBasic.h"
#include "qaGobiApiWds.h"
#include "qaGobiApiFms.h"
#include "qaWdsStartNetworkInterface.h"
#include "qaWdsStopNetworkInterface.h"
#include "qaWdsSetMipMode.h"
#include "qaWdsSetMipParams.h"
#include "qaWdsGetAutoconnectSetting.h"
#include "qaWdsGetDefaultSettings.h"
#include "qaWdsGetPktSrvcStatus.h"
#include "qaWdsGetPktStatistics.h"
#include "qaWdsGetDormancyState.h"
#include "qaWdsGetDataBearerTechnology.h"
#include "qaWdsGetCallDuration.h"
#include "qaWdsGetRuntimeSettings.h"
#include "qaWdsGetCurrentChannelRate.h"
#include "qaWdsModifyProfile.h"
#include "qaWdsGetMipMode.h"
#include "qaWdsReadMipProfile.h"
#include "qaWdsGetLastMipStatus.h"
#include "qaWdsSLQSGetRuntimeSettings.h"
#include "qaWdsSLQSGetProfileSettings.h"
#include "qaWdsSLQSSetIPFamilyPreference.h"
#include "qaWdsSLQSDeleteProfile.h"
#include "qaWdsSLQSCreateProfile.h"
#include "qaWdsSLQSACSettings.h"
#include "qaGobiApiDms.h"
#include "qaWdsGetCurDataBearerTechnology.h"
#include "qaWdsSLQSModifyProfile.h"
#include "os/swi_osapi.h"
#include "qaWdsSLQSSet3GPPConfigItem.h"
#include "qaWdsSLQSGet3GPPConfigItem.h"
#include "qaWdsModifyMipProfile.h"
#include "qaWdsSLQSWdsSetEventReport.h"
#include "qaWdsSLQSWdsSwiPDPRuntimeSettings.h"

#define PKT_STAT_STAT_MASK 0X0000003F
#define BYT_STAT_STAT_MASK 0X000000C0
#define MIN_PROFILE_ID     1
#define MAX_PROFILE_ID     16
#define START_DATA_SESSION 1
#define STOP_DATA_SESSION  0
#define SET_AUTO_CONNECT   1
#define GET_AUTO_CONNECT   0

/* DEBUG defines */
#define DBG_WDS
#ifdef DBG_WDS
#include <syslog.h>
#endif

/* enumerations */
enum pdpidx{
    WDSPDP1IDX,
    WDSPDP2IDX,
    WDSPDP3IDX,
    WDSNUMPDPS
};

/* For an IPv4v6 session, we abstract the added complexity of managing two data
 * sessions, a v4 and a v6, from the application by storing both session handles
 * internally. When the session is subsequently terminated, the user will pass in
 * the IPv6 session handle which will be matched up with the correct IPv4 session
 * handle. We do not register for callback notifications when the session state
 * changes. The application, if registered will receive these; if not, the attempt
 * to stop the data session will simply return a relevant error.
 */
struct qaWdsPDPSessionData{
    enum pdpidx pdpIdx;         /* get to know myself */

    BYTE IPFamilyPreference;    /* IP Family Preference for subsequent Data Session Calls.
                                 * Do not modify this directly; it should only be changed
                                 * in iSLQSSetIPFfamilyPreference.
                                 */

    ULONG v4sessionId;          /* IPv4 QMI client session handle */
    ULONG v6sessionId;          /* IPv6 QMI client session handle */

   /* mutual exclusion */
    struct swi_osmutex mutex;
    void (*lock)(enum pdpidx);
    void (*unlock)(enum pdpidx);
};

/* internal data session manager */
local struct qaWdsPDPSessionData wdsSessionManager[WDSNUMPDPS];

local void qaWdsPDPSessionDataLock(enum pdpidx idx)
{
    swi_osapi_mutexlock(&wdsSessionManager[idx].mutex);
}

local void qaWdsPDPSessionDataUnLock(enum pdpidx idx)
{
    swi_osapi_mutexunlock(&wdsSessionManager[idx].mutex);
}

#ifdef DBG_WDS
local void wdsSessionDataLog(
    struct qaWdsPDPSessionData  *ps,
    struct ssdatasession_params *pd,
    void *prc )
{
    syslog( LOG_DEBUG,
            "%s/%d: IP Family: %d, v4SH: %lu, v6SH: %lu",
            __func__,
            __LINE__,
            ps->IPFamilyPreference,
            ps->v4sessionId,
            ps->v6sessionId );

    if( NULL != pd )
    {
        syslog( LOG_DEBUG,
                "%s/%d: action: %d, sessionID: %lu, v4SessionId: %lu, v6SessionId: %lu",
                __func__,
                __LINE__,
                pd->action,
                pd->sessionId,
                pd->v4sessionId,
                pd->v6sessionId );
    }

    if( NULL != prc )
    {
        syslog( LOG_DEBUG,
                "%s/%d: Error Code: %lu (0x%lX)\n",
                __func__,
                __LINE__,
                *(ULONG *)prc,
                *(ULONG*)prc );
    }
}
#endif

ULONG SetMobileIP( ULONG mode )
{
    ULONG  resultcode;  /* result code */
    BYTE  *pInParam;   /* ptr to param field rx'd from modem */
    BYTE  *pOutParam;  /* ptr to outbound param field */
    BYTE  *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT ParamLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiWdsSetMipModeResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiWdsSetMipMode( &ParamLength,
                                     pOutParam,
                                     mode );

    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WDS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)

        /* Copy to the caller's buffer */
        resultcode = UpkQmiWdsSetMipMode( pInParam, &response );

    qmrelreqbkp();
    return resultcode;
}
ULONG SetMobileIPParameters(
    CHAR  *pSPC,
    ULONG *pMode,
    BYTE  *pRetryLimit,
    BYTE  *pRetryInterval,
    BYTE  *pReRegPeriod,
    BYTE  *pReRegTraffic,
    BYTE  *pHAAuthenticator,
    BYTE  *pHA2002bis )
{
    ULONG             resultcode;  /* result code */
    BYTE              *pInParam;   /* ptr to param field rx'd from modem */
    BYTE              *pOutParam;  /* ptr to outbound param field */
    BYTE              *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT            ParamLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiWdsSetMipParamsResp response;

    /* SPC is Mandatory Parameter */
    if( !pSPC )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiWdsSetMipParams( &ParamLength,
                                   pOutParam,
                                   pSPC,
                                   pMode,
                                   pRetryLimit,
                                   pRetryInterval,
                                   pReRegPeriod,
                                   pReRegTraffic,
                                   pHAAuthenticator,
                                   pHA2002bis );

    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WDS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
        /* Copy to the caller's buffer */
        resultcode = UpkQmiWdsSetMipParams(pInParam, &response);

    qmrelreqbkp();
    return resultcode;
}

ULONG GetAutoconnect( ULONG *pSetting )
{
    ULONG               resultcode;  /* result code */
    BYTE                *pInParam;   /* ptr to param field rx'd from modem */
    BYTE                *pOutParam;  /* ptr to outbound param field */
    BYTE                *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT              ParamLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiWdsGetAutoconnectSettingResp response;

    /* pSetting is an OUT parameter and hence should not be NULL */
    if ( !pSetting )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiWdsGetAutoconnectSetting( &ParamLength, pOutParam );

    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WDS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
    {
        /* Pass the pointer for the OUT parameters */
        response.pSetting = pSetting;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiWdsGetAutoconnectSetting( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG GetDefaultProfile(
    ULONG profileType,
    ULONG *pPDPType,
    ULONG *pIPAddress,
    ULONG *pPrimaryDNS,
    ULONG *pSecondaryDNS,
    ULONG *pAuthentication,
    BYTE  nameSize,
    CHAR  *pName,
    BYTE  apnSize,
    CHAR  *pAPNName,
    BYTE  userSize,
    CHAR  *pUsername )
{
    ULONG             resultcode;  /* result code */
    BYTE              *pInParam;   /* ptr to param field rx'd from modem */
    BYTE              *pOutParam;  /* ptr to outbound param field */
    BYTE              *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT            ParamLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiWdsGetDefaultSettingsResp response;

    /* Check against NULL for out params */
    if ( !pPDPType          ||
         !pIPAddress        ||
         !pPrimaryDNS       ||
         !pSecondaryDNS     ||
         !pAuthentication   ||
         !pName             ||
         !pAPNName          ||
         !pUsername )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiWdsGetDefaultSettings( &ParamLength,
                                         pOutParam,
                                         profileType );

    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WDS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
    {
        /* Initialisation of Buffer */
        slmemset((char *)pName,     0, nameSize);
        slmemset((char *)pAPNName,  0, apnSize);
        slmemset((char *)pUsername, 0, userSize);
        /* Pass the pointer for the OUT parameters */
        response.pPDPType        = pPDPType;
        response.pIPAddress      = pIPAddress;
        response.pPrimaryDNS     = pPrimaryDNS;
        response.pSecondaryDNS   = pSecondaryDNS;
        response.pIPAddressv6    = NULL;
        response.pPrimaryDNSv6   = NULL;
        response.pSecondaryDNSv6 = NULL;
        response.pAuthentication = pAuthentication;
        response.pName           = pName;
        response.pAPNName        = pAPNName;
        response.pUserName       = pUsername;
        response.nameSize        = nameSize;
        response.apnSize         = apnSize;
        response.userSize        = userSize;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiWdsGetDefaultSettings( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG GetDefaultProfileLTE(
    ULONG  profileType,
    ULONG  *pPDPType,
    ULONG  *pIPAddressv4,
    ULONG  *pPrimaryDNSv4,
    ULONG  *pSecondaryDNSv4,
    USHORT *pIPAddressv6,
    USHORT *pPrimaryDNSv6,
    USHORT *pSecondaryDNSv6,
    ULONG  *pAuthentication,
    BYTE   nameSize,
    CHAR   *pName,
    BYTE   apnSize,
    CHAR   *pAPNName,
    BYTE   userSize,
    CHAR   *pUsername)
{
    ULONG             resultcode;  /* result code */
    BYTE              *pInParam;   /* ptr to param field rx'd from modem */
    BYTE              *pOutParam;  /* ptr to outbound param field */
    BYTE              *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT            ParamLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiWdsGetDefaultSettingsResp response;

    /* Check against NULL for out params */
    if ( !pPDPType          ||
         !pIPAddressv4      ||
         !pPrimaryDNSv4     ||
         !pSecondaryDNSv4   ||
         !pIPAddressv6      ||
         !pPrimaryDNSv6     ||
         !pSecondaryDNSv6   ||
         !pAuthentication   ||
         !pName             ||
         !pAPNName          ||
         !pUsername )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiWdsGetDefaultSettings( &ParamLength,
                                             pOutParam,
                                             profileType );

    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WDS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
    {
        /* Initialisation of Buffer */
        slmemset((char *)pName,     0, nameSize);
        slmemset((char *)pAPNName,  0, apnSize);
        slmemset((char *)pUsername, 0, userSize);
        /* Pass the pointer for the OUT parameters */
        response.pPDPType        = pPDPType;
        response.pIPAddress      = pIPAddressv4;
        response.pPrimaryDNS     = pPrimaryDNSv4;
        response.pSecondaryDNS   = pSecondaryDNSv4;
        response.pIPAddressv6    = pIPAddressv6;
        response.pPrimaryDNSv6   = pPrimaryDNSv6;
        response.pSecondaryDNSv6 = pSecondaryDNSv6;
        response.pAuthentication = pAuthentication;
        response.pName           = pName;
        response.pAPNName        = pAPNName;
        response.pUserName       = pUsername;
        response.nameSize        = nameSize;
        response.apnSize         = apnSize;
        response.userSize        = userSize;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiWdsGetDefaultSettings( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG  GetSessionState(
    ULONG    *pState )
{
    ULONG              resultcode;  /* result code */
    BYTE               *pInParam;    /* ptr to param field rx'd from modem */
    BYTE               *pOutParam;   /* ptr to outbound param field */
    BYTE               *pReqBuf;     /* Pointer to outgoing request buffer */
    USHORT             ParamLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiWdsGetPktSrvcStatusResp response;

    /* pState is OUT parameter and hence should not be NULL.*/
    if ( !pState )
    {
        return eQCWWAN_ERR_INVALID_ARG; /* QCWWAN_ERR_INVALID_ARGUMENT; */
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiWdsGetPktSrvcStatus(
                &ParamLength,
                pOutParam);

    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
     resultcode = SwiQmiSendnWait( pReqBuf,
                                   eQMI_SVC_WDS,
                                   ParamLength,
                                   eQMI_TIMEOUT_2_S,
                                   &pInParam,
                                   &ParamLength );
    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
    {
        /* Pass the pointer for the OUT parameters */
        response.pConnectionStatus = pState;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiWdsGetPktSrvcStatus(pInParam, &response);
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG  GetPacketStatus(
    ULONG    *pTXPacketSuccesses,
    ULONG    *pRXPacketSuccesses,
    ULONG    *pTXPacketErrors,
    ULONG    *pRXPacketErrors,
    ULONG    *pTXPacketOverflows,
    ULONG    *pRXPacketOverflows )
{
    ULONG              resultcode;  /* result code */
    BYTE               *pInParam;   /* ptr to param field rx'd from modem */
    BYTE               *pOutParam;  /* ptr to outbound param field */
    BYTE               *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT             ParamLength; /* Ret'd length of the QMI Param field */
    ULONG              StatMask;

    /* Storage for results and response variable */
    struct QmiWdsGetPktStatisticsResp response;

    /* stat_mask to be set according to following settings
     * Requested statistic bit mask
     *    0x00000001  Tx packets OK
     *    0x00000002  Rx packets OK
     *    0x00000004  Tx packet errors
     *    0x00000008  Rx packet errors
     *    0x00000010  Tx overflows
     *    0x00000020  Rx overflows
     */
    StatMask = PKT_STAT_STAT_MASK;

    /*
     * pTXPacketSuccesses, pRXPacketSuccesses
     * pTXPacketErrors,    pRXPacketErrors
     * pTXPacketOverflows, pRXPacketOverflows
     * are OUT parameters and hence should not be NULL.
     */
    if ( !pTXPacketSuccesses   ||
         !pRXPacketSuccesses   ||
         !pTXPacketErrors      ||
         !pRXPacketErrors      ||
         !pTXPacketOverflows   ||
         !pRXPacketOverflows )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }
    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiWdsGetPktStatistics( &ParamLength,
                                       pOutParam,
                                       StatMask );

    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
     resultcode = SwiQmiSendnWait( pReqBuf,
                                   eQMI_SVC_WDS,
                                   ParamLength,
                                   eQMI_TIMEOUT_2_S,
                                   &pInParam,
                                   &ParamLength );
    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
    {
        /* Pass the pointer for the OUT parameters */
        response.pTXPacketSuccesses = pTXPacketSuccesses;
        response.pRXPacketSuccesses = pRXPacketSuccesses;
        response.pTXPacketErrors    = pTXPacketErrors;
        response.pRXPacketErrors    = pRXPacketErrors;
        response.pTXPacketOverflows = pTXPacketOverflows;
        response.pRXPacketOverflows = pRXPacketOverflows;

        *(response.pTXPacketSuccesses) = 0;
        *(response.pRXPacketSuccesses) = 0;
        *(response.pTXPacketErrors)    = 0;
        *(response.pRXPacketErrors)    = 0;
        *(response.pTXPacketOverflows) = 0;
        *(response.pRXPacketOverflows) = 0;
        /* Copy to the caller's buffer */
        resultcode = UpkQmiWdsGetPktStatistics(pInParam, &response);
    }

    qmrelreqbkp();
    return resultcode;

}

ULONG GetByteTotals(
    ULONGLONG    *pTXTotalBytes,
    ULONGLONG    *pRXTotalBytes )
{
    ULONG              resultcode;  /* result code */
    BYTE               *pInParam;   /* ptr to param field rx'd from modem */
    BYTE               *pOutParam;  /* ptr to outbound param field */
    BYTE               *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT             ParamLength; /* Ret'd length of the QMI Param field */
    ULONG              StatMask;

    /* Storage for results and response variable */
    struct QmiWdsGetPktStatisticsResp response;

    /* stat_mask to be set according to following settings
     * Requested statistic bit mask
     *    0x00000040  Tx bytes OK
     *    0x00000080  Rx bytes OK
     */
    StatMask = BYT_STAT_STAT_MASK;

    /*
     * pTXTotalBytes, pRXTotalBytes
     * are OUT parameters and hence should not be NULL.
     */
    if ( !pTXTotalBytes ||
         !pRXTotalBytes )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }
    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiWdsGetPktStatistics(
                &ParamLength,
                pOutParam,
                StatMask);

    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
     resultcode = SwiQmiSendnWait( pReqBuf,
                                   eQMI_SVC_WDS,
                                   ParamLength,
                                   eQMI_TIMEOUT_2_S,
                                   &pInParam,
                                   &ParamLength );
    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
    {
        /* Pass the pointer for the OUT parameters */
        response.pTXOkBytesCount = pTXTotalBytes;
        response.pRXOkBytesCount = pRXTotalBytes;

        /* Initialise the Values */
        *(response.pTXOkBytesCount) = 0;
        *(response.pRXOkBytesCount) = 0;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiWdsGetPktStatistics(pInParam, &response);
    }

    qmrelreqbkp();
    return resultcode;

}

ULONG  GetDormancyState(
   ULONG    *pDormancyState )
{
    ULONG              resultcode;  /* result code */
    BYTE               *pInParam;   /* ptr to param field rx'd from modem */
    BYTE               *pOutParam;  /* ptr to outbound param field */
    BYTE               *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT             ParamLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiWdsGetDormancyResp response;

    /* pDormancyState is OUT parameter and hence should not be NULL.*/
    if ( !pDormancyState )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }
    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiWdsGetDormancyState(
                &ParamLength,
                pOutParam);

    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WDS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S,
                                  &pInParam,
                                  &ParamLength );
    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
    {
        /* Pass the pointer for the OUT parameters */
        response.pDormancyState  = pDormancyState;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiWdsGetDormancyState(pInParam, &response);
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG  GetDataBearerTechnology(
    ULONG    *pDataBearer )
{
    ULONG              resultcode;   /* result code */
    BYTE               *pInParam;     /* ptr to param field rx'd from modem */
    BYTE               *pOutParam;    /* ptr to outbound param field */
    BYTE               *pReqBuf;      /* Pointer to outgoing request buffer */
    USHORT             ParamLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiWdsGetDataBearerResp response;

    /* pDataBearer is OUT parameter and hence should not be NULL.*/
    if ( !pDataBearer )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiWdsGetDataBearerTechnology(
                &ParamLength,
                pOutParam);

    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
     resultcode = SwiQmiSendnWait( pReqBuf,
                                   eQMI_SVC_WDS,
                                   ParamLength,
                                   eQMI_TIMEOUT_2_S,
                                   &pInParam,
                                   &ParamLength );
    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
    {
        /* Pass the pointer for the OUT parameters */
        response.pCurrentDataBearer = pDataBearer;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiWdsGetDataBearerTechnology(pInParam, &response);
    }

    qmrelreqbkp();
    return resultcode;

}

ULONG SLQSGetDataBearerTechnology( QmiWDSDataBearers *pDataBearers )
{
    ULONG  resultcode;  /* result code */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT ParamLength; /* Ret'd length of the Param field */

    /* Storage for results and response variable */
    struct QmiWdsGetCurDataBearerResp response;

    /* OUT parameters should not be NULL.*/
    if (!pDataBearers)
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiWdsGetCurDataBearerTechnology(&ParamLength, pOutParam);

    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WDS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S,
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
    {
        /* Reset the data bearer mask, this will be set while unpacking */
        pDataBearers->dataBearerMask = 0x00;
        response.pDataBearers        = pDataBearers;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiWdsGetCurDataBearerTechnology(pInParam, &response);
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG  GetSessionDuration(
    ULONGLONG    *pDuration )
{
    ULONG              resultcode;  /* result code */
    BYTE               *pInParam;   /* ptr to param field rx'd from modem */
    BYTE               *pOutParam;  /* ptr to outbound param field */
    BYTE               *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT             ParamLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiWdsGetDurationResp response;

    /* pDataBearer is OUT parameter and hence should not be NULL.*/
    if ( !pDuration )
    {
        return eQCWWAN_ERR_INVALID_ARG; /* QCWWAN_ERR_INVALID_ARGUMENT; */
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiWdsGetCallDuration( &ParamLength,
                                      pOutParam );

    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
     resultcode = SwiQmiSendnWait( pReqBuf,
                                   eQMI_SVC_WDS,
                                   ParamLength,
                                   eQMI_TIMEOUT_2_S,
                                   &pInParam,
                                   &ParamLength );
    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
    {
        /* Pass the pointer for the OUT parameters */
        response.pCallDuration    = pDuration;
        *(response.pCallDuration) = 0;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiWdsGetCallDuration(pInParam, &response);
    }

    qmrelreqbkp();
    return resultcode;

}

ULONG GetConnectionRate(
    ULONG     *pCurrentChannelTXRate,
    ULONG     *pCurrentChannelRXRate,
    ULONG     *pMaxChannelTXRate,
    ULONG     *pMaxChannelRXRate )
{
    ULONG             resultcode;  /* result code */
    BYTE              *pInParam;   /* ptr to param field rx'd from modem */
    BYTE              *pOutParam;  /* ptr to outbound param field */
    BYTE              *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT            ParamLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiWdsGetCurrentChannelRateResp response;

    if ( !pCurrentChannelTXRate ||
         !pCurrentChannelRXRate ||
         !pMaxChannelTXRate     ||
         !pMaxChannelRXRate)
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Index points to first QMI header*/
    ParamLength = 0;

    resultcode = PkQmiWdsGetCurrentChannelRate( &ParamLength , pOutParam );
    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WDS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
    {
        response.pCurrentChannelTXRate  = pCurrentChannelTXRate;
        response.pCurrentChannelRXRate  = pCurrentChannelRXRate;
        response.pMaxChannelTXRate      = pMaxChannelTXRate;
        response.pMaxChannelRXRate      = pMaxChannelRXRate;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiWdsGetCurrentChannelRate(pInParam, &response);
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG  SetAutoconnect(
    ULONG    setting )
{
    ULONG              resultcode;  /* result code */

    /* Storage for request parameters */
    struct slqsautoconnect Req;

    /* Initialize the request structure*/
    Req.action = SET_AUTO_CONNECT;
    Req.acsetting = ( BYTE ) setting;
    Req.acroamsetting = 0x00;

    /* Call Api SLQSAutoConnect */
    resultcode = SLQSAutoConnect( &Req );

    return resultcode;
}

ULONG  SetDefaultProfile(
    ULONG profileType,
    ULONG *pPDPType,
    ULONG *pIPAddress,
    ULONG *pPrimaryDNS,
    ULONG *pSecondaryDNS,
    ULONG *pAuthentication,
    CHAR  *pName,
    CHAR  *pAPNName,
    CHAR  *pUsername,
    CHAR  *pPassword )
{
    ULONG  resultcode;            /* result code */
    BYTE  *pInParam;              /* ptr to param field rx'd from modem */
    BYTE  *pOutParam;             /* ptr to outbound param field */
    BYTE  *pReqBuf;               /* Pointer to outgoing request buffer */
    USHORT ParamLength;           /* Ret'd length of the QMI Param field */
    /* Storage for results and response variable */
    struct QmiWdsModifyProfileResp response;
    BYTE ProfileID = 1;
    char modelType[128];

    /*
     * Profile ID will be determined on the basis of device type
     * MC7750 device - Default Profile ID is 0
     * Other devices - Default Profile ID is 1
     */
    if( eQCWWAN_ERR_NONE == GetModelID( sizeof(modelType), &modelType[0] ) )
    {
        if( 0 == slstrncmp( "MC7750", modelType, 6) )
            ProfileID = 0;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Index points to first QMI header. */
    ParamLength = 0;

    resultcode = PkQmiWdsModifyProfile( &ParamLength,
                                        pOutParam,
                                        profileType,
                                        ProfileID,
                                        pPDPType,
                                        pIPAddress,
                                        pPrimaryDNS,
                                        pSecondaryDNS,
                                        NULL,
                                        NULL,
                                        NULL,
                                        pAuthentication,
                                        pName,
                                        pAPNName,
                                        pUsername,
                                        pPassword );
    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
     resultcode = SwiQmiSendnWait( pReqBuf,
                                   eQMI_SVC_WDS,
                                   ParamLength,
                                   eQMI_TIMEOUT_2_S,
                                   &pInParam,
                                   &ParamLength );
    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
        /* Copy to the caller's buffer */
        resultcode = UpkQmiWdsModifyProfile(pInParam, &response);

    qmrelreqbkp();
    return resultcode;
}

ULONG  SetDefaultProfileLTE(
    ULONG  profileType,
    ULONG  *pPDPType,
    ULONG  *pIPAddressv4,
    ULONG  *pPrimaryDNSv4,
    ULONG  *pSecondaryDNSv4,
    USHORT *pIPAddressv6,
    USHORT *PrimaryDNSv6,
    USHORT *pSecondaryDNSv6,
    ULONG  *pAuthentication,
    CHAR   *pName,
    CHAR   *pAPNName,
    CHAR   *pUsername,
    CHAR   *pPassword )
{
    ULONG     resultcode; /* result code */
    BYTE      *pInParam;  /* ptr to param field rx'd from modem */
    BYTE      *pOutParam; /* ptr to outbound param field */
    BYTE      *pReqBuf;   /* Pointer to outgoing request buffer */
    USHORT    ParamLength;/* Ret'd length of the QMI Param field */
    BYTE      ProfileID = 1;  /* Profile ID */
    char      modelType[128];

    /* Storage for results and response variable */
    struct QmiWdsModifyProfileResp response;

    /*
     * Profile ID will be determined on the basis of device type
     * MC7750 device - Default Profile ID is 0
     * Other devices - Default Profile ID is 1
     */
    if( eQCWWAN_ERR_NONE == GetModelID( sizeof(modelType), &modelType[0] ) )
    {
        if( 0 == slstrncmp( "MC7750", modelType, 6) )
            ProfileID = 0;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Index points to first QMI header. */
    ParamLength = 0;

    resultcode = PkQmiWdsModifyProfile( &ParamLength,
                                        pOutParam,
                                        profileType,
                                        ProfileID,
                                        pPDPType,
                                        pIPAddressv4,
                                        pPrimaryDNSv4,
                                        pSecondaryDNSv4,
                                        pIPAddressv6,
                                        PrimaryDNSv6,
                                        pSecondaryDNSv6,
                                        pAuthentication,
                                        pName,
                                        pAPNName,
                                        pUsername,
                                        pPassword );

    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
     resultcode = SwiQmiSendnWait( pReqBuf,
                                   eQMI_SVC_WDS,
                                   ParamLength,
                                   eQMI_TIMEOUT_2_S,
                                   &pInParam,
                                   &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
        /* Copy to the caller's buffer */
        resultcode = UpkQmiWdsModifyProfile(pInParam, &response);

    qmrelreqbkp();
    return resultcode;
}

ULONG  GetMobileIP(
   ULONG    *pMode )
{
    ULONG  resultcode;  /* result code */
    BYTE  *pInParam;   /* ptr to param field rx'd from modem */
    BYTE  *pOutParam;  /* ptr to outbound param field */
    BYTE  *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT ParamLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiWdsGetMipModeResp response;

    /* pMode is OUT parameter and hence should not be NULL.*/
    if ( !pMode )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }
    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiWdsGetMipMode( &ParamLength,
                                     pOutParam );
    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WDS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S,
                                  &pInParam,
                                  &ParamLength );
    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
    {
        /* Pass the pointer for the OUT parameters */
        response.pMipMode = pMode;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiWdsGetMipMode(pInParam, &response);
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG GetMobileIPProfile(
      BYTE   index,
      BYTE  *pEnabled,
      ULONG *pAddress,
      ULONG *pPrimaryHA,
      ULONG *pSecondaryHA,
      BYTE  *pRevTunneling,
      BYTE  naiSize,
      CHAR  *pNAI,
      ULONG *pHASPI,
      ULONG *pAAASPI,
      ULONG *pHAState,
      ULONG *pAAAState)
{
        ULONG resultcode;   /* result code */
        BYTE  *pInParam;    /* ptr to param field rx'd from modem */
        BYTE  *pOutParam;   /* ptr to outbound param field */
        BYTE  *pReqBuf;     /* Pointer to outgoing request buffer */
        USHORT ParamLength; /* Ret'd length of the QMI Param field */

        /* Storage for results and response variable */
        struct QmiWdsReadMipProfileResp response;

        if ( !pEnabled      ||
             !pAddress      ||
             !pPrimaryHA    ||
             !pSecondaryHA  ||
             !pRevTunneling ||
             !pNAI          ||
             !pHASPI        ||
             !pAAASPI       ||
             !pHAState      ||
             !pAAAState )
            return eQCWWAN_ERR_INVALID_ARG;

        /* Initialize the pointer to the outgoing request buffer pointer */
        pReqBuf = qmgetreqbkp();

        /* Get a pointer to the start of the outbound QMI Parameter field */
        pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

        resultcode = PkQmiWdsReadMipProfile( &ParamLength,
                                             pOutParam,
                                             index );
        if (resultcode != eQCWWAN_ERR_NONE)
        {
            qmrelreqbkp();
            return resultcode;
        }


        /* Prepare and send the blocking call */
         resultcode = SwiQmiSendnWait( pReqBuf,
                                       eQMI_SVC_WDS,
                                       ParamLength,
                                       eQMI_TIMEOUT_2_S,/* 2 Seconds */
                                       &pInParam,
                                       &ParamLength );

        /* Only parse out the response data if we got a positive return */
        if (resultcode == eQCWWAN_ERR_NONE)
        {
            response.pEnabled       = pEnabled;
            response.pAddress       = pAddress;
            response.pPrimaryHA     = pPrimaryHA;
            response.pSecondaryHA   = pSecondaryHA;
            response.pRevTunneling  = pRevTunneling;
            response.naiSize        = naiSize;
            response.pNAI           = pNAI;
            response.pHASPI         = pHASPI;
            response.pAAASPI        = pAAASPI;
            response.pHAState       = pHAState;
            response.pAAAState      = pAAAState;

            /* Copy to the caller's buffer */
            resultcode = UpkQmiWdsReadMipProfile( pInParam, &response );
        }

        qmrelreqbkp();
        return resultcode;
}

ULONG GetLastMobileIPError(
    ULONG *pError)
{
    ULONG resultcode;   /* result code */
    BYTE  *pInParam;    /* ptr to param field rx'd from modem */
    BYTE  *pOutParam;   /* ptr to outbound param field */
    BYTE  *pReqBuf;     /* Pointer to outgoing request buffer */
    USHORT ParamLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiWdsGetLastMipStatusResp response;

    /* pError is OUT parameter and
     * hence should not be NULL */
    if ( !pError )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    resultcode = PkQmiWdsGetLastMipStatus( &ParamLength, pOutParam );
    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
     resultcode = SwiQmiSendnWait( pReqBuf,
                                   eQMI_SVC_WDS,
                                   ParamLength,
                                   eQMI_TIMEOUT_2_S,/* 2 Seconds */
                                   &pInParam,
                                   &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
    {
        /* Pass the pointer for the OUT parameters */
        response.pError = pError;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiWdsGetLastMipStatus( pInParam, &response );
    }
    qmrelreqbkp();
    return resultcode;
}
local ULONG iSLQSSetIPFamilyPreference(
    BYTE IPFamilyPreference )
{
    ULONG  rc;          /* result code */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT ParamLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiWdsSLQSSetIPFamilyPreferenceResp response;


    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    rc = PkQmiWdsSLQSSetIPFamilyPreference( &ParamLength,
                                            pOutParam,
                                            IPFamilyPreference );

    if ( eQCWWAN_ERR_NONE != rc )
    {
        qmrelreqbkp();
        return rc;
    }

    /* send QMI request */
    rc = SwiQmiSendnWait( pReqBuf,
                          eQMI_SVC_WDS,
                          ParamLength,
                          eQMI_TIMEOUT_2_S, /* 2 seconds */
                          &pInParam,
                          &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == rc )
    {
        rc = UpkQmiWdsSLQSSetIPFamilyPreference( pInParam,
                                                 &response );
    }

    qmrelreqbkp();
    return rc;
}

ULONG SLQSSetIPFamilyPreference(
    BYTE IPFamilyPreference )
{
    enum eQCWWANError rc;

    /* For now, we only support a single PDP context. Therefore, do not
       allow the IP family preference to be changed while we are in a
       data session in order to ensure the proper client is used on the
       SDK process side when an attempt to stop the data session is made.
       Failure to do so will result in failure to stop the current data
       session if the chosen ip family preference does not match that
       which was used to initiate the data session */

    wdsSessionManager[WDSPDP1IDX].lock(WDSPDP1IDX);
#ifdef DBG_WDS
    if( IPV4V6 == IPFamilyPreference )
    {
        syslog( LOG_DEBUG,
                "%s: IP Family Preference: V4V6",
                __func__ );
    }
    else
    {
        syslog( LOG_DEBUG,
                "%s: IP Family Preference: V%d",
                __func__,
                IPFamilyPreference );
    }
#endif
    if( IPV4V6 == IPFamilyPreference )
    {
        /* QMI services do not define a v4v6 IP family type but
         * we track this internally in order to know that we need
         * to issue both v4 and a v6 data session requests for
         * subsequent invocations of data session request APIs.
         */
        wdsSessionManager[WDSPDP1IDX].IPFamilyPreference = IPFamilyPreference;
        wdsSessionManager[WDSPDP1IDX].unlock(WDSPDP1IDX);
        return eQCWWAN_ERR_NONE;
    }

    if( eQCWWAN_ERR_NONE ==
        (rc = iSLQSSetIPFamilyPreference(IPFamilyPreference) ) )
    {
        wdsSessionManager[WDSPDP1IDX].IPFamilyPreference = IPFamilyPreference;
    }

    wdsSessionManager[WDSPDP1IDX].unlock(WDSPDP1IDX);
    return rc;
}

ULONG iSLQSGetRuntimeSettings (
    struct WdsRunTimeSettings *pRunTimeSettings )
{
    ULONG  resultcode;  /* result code */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT ParamLength; /* Ret'd length of the QMI Param field */
    ULONG  *pRequestedSettings;

    if ( !pRunTimeSettings )
    {
         return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Set all bits to request all settings */
    ULONG settings = 0xFFFF;
    pRequestedSettings = &settings;

    /* Storage for results and response variable */
    struct QmiWdsSLQSGetRuntimeSettingsResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Index points to first QMI header. */
    ParamLength = 0;
    resultcode = PkQmiWdsSLQSGetRuntimeSettings( &ParamLength,
                                                 pOutParam,
                                                 pRequestedSettings );

    if ( resultcode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
     resultcode = SwiQmiSendnWait( pReqBuf,
                                   eQMI_SVC_WDS,
                                   ParamLength,
                                   eQMI_TIMEOUT_2_S,
                                   &pInParam,
                                   &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultcode == eQCWWAN_ERR_NONE )
    {
        memcpy( (void *)&response.WdsSlqsRunTimeSettings,
                (void *)pRunTimeSettings,
                sizeof(response.WdsSlqsRunTimeSettings) );

        /* Unpack the parameters */
        resultcode = UpkQmiWdsSLQSGetRuntimeSettings( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

local void InitRunTimeSettings(
    struct WdsRunTimeSettings *pRunTimeSettings,
    struct WdsRunTimeSettings *pRunTimeSettingsV4,
    struct WdsRunTimeSettings *pRunTimeSettingsV6 )
{
    struct UMTSQoS *pUMTSQoS;
    struct GPRSQoS *pGPRSQoS;
    BYTE           idx        = 0;

    /* Initialise Profile Name - IPv4 info */
    if( pRunTimeSettings->pProfileName )
    {
        *pRunTimeSettings->pProfileName    = EOS;
        pRunTimeSettingsV4->pProfileName   = pRunTimeSettings->pProfileName;
    }
    /* Initialise PDP Type - IPv4 info*/
    if( pRunTimeSettings->pPDPType )
    {
        *pRunTimeSettings->pPDPType        = ~0;
        pRunTimeSettingsV4->pPDPType       = pRunTimeSettings->pPDPType;
    }
    /* Initialise APN Name - IPv4 info*/
    if( pRunTimeSettings->pAPNName )
    {
        *pRunTimeSettings->pAPNName        = EOS;
        pRunTimeSettingsV4->pAPNName       = pRunTimeSettings->pAPNName;
    }
    /* Initialise Primary DNS V4 - IPv4 info*/
    if( pRunTimeSettings->pPrimaryDNSV4 )
    {
        *pRunTimeSettings->pPrimaryDNSV4   = ~0;
        pRunTimeSettingsV4->pPrimaryDNSV4  = pRunTimeSettings->pPrimaryDNSV4;
    }
    /* Initialise Secondary DNS V4 - IPv4 info*/
    if( pRunTimeSettings->pSecondaryDNSV4 )
    {
        *pRunTimeSettings->pSecondaryDNSV4  = ~0;
        pRunTimeSettingsV4->pSecondaryDNSV4 = pRunTimeSettings->pSecondaryDNSV4;
    }
    /* Initialise UMTS Req QoS - IPv4 info*/
    if( pRunTimeSettings->pUMTSGrantedQoS )
    {
        pUMTSQoS = pRunTimeSettings->pUMTSGrantedQoS;

        pUMTSQoS->trafficClass        = ~0;
        pUMTSQoS->maxUplinkBitrate    = ~0;
        pUMTSQoS->maxDownlinkBitrate  = ~0;
        pUMTSQoS->grntUplinkBitrate   = ~0;
        pUMTSQoS->grntDownlinkBitrate = ~0;
        pUMTSQoS->qosDeliveryOrder    = ~0;
        pUMTSQoS->maxSDUSize          = ~0;
        pUMTSQoS->sduErrorRatio       = ~0;
        pUMTSQoS->resBerRatio         = ~0;
        pUMTSQoS->deliveryErrSDU      = ~0;
        pUMTSQoS->transferDelay       = ~0;
        pUMTSQoS->trafficPriority     = ~0;

        pRunTimeSettingsV4->pUMTSGrantedQoS = pRunTimeSettings->pUMTSGrantedQoS;
    }
    /* Initialise GPRS Req QoS - IPv4 info*/
    if( pRunTimeSettings->pGPRSGrantedQoS )
    {
        pGPRSQoS = pRunTimeSettings->pGPRSGrantedQoS;

        pGPRSQoS->precedenceClass     = ~0;
        pGPRSQoS->delayClass          = ~0;
        pGPRSQoS->reliabilityClass    = ~0;
        pGPRSQoS->peakThroughputClass = ~0;
        pGPRSQoS->meanThroughputClass = ~0;

        pRunTimeSettingsV4->pGPRSGrantedQoS = pRunTimeSettings->pGPRSGrantedQoS;
    }
    /* Initialise User Name - IPv4 info*/
    if( pRunTimeSettings->pUsername )
    {
        pRunTimeSettings->pUsername   = EOS;
        pRunTimeSettingsV4->pUsername = pRunTimeSettings->pUsername;
    }
    /* Initialise Authentication - IPv4 info*/
    if( pRunTimeSettings->pAuthentication )
    {
        *pRunTimeSettings->pAuthentication  = ~0;
        pRunTimeSettingsV4->pAuthentication = pRunTimeSettings->pAuthentication;
    }
    /* Initialise IPAddress IPv4 - IPv4 info*/
    if( pRunTimeSettings->pIPAddressV4 )
    {
        *pRunTimeSettings->pIPAddressV4  = ~0;
        pRunTimeSettingsV4->pIPAddressV4 = pRunTimeSettings->pIPAddressV4;
    }
    /* Initialise ProfileID - IPv4 info*/
    if( pRunTimeSettings->pProfileID )
    {
        pRunTimeSettings->pProfileID->profileType  = ~0;
        pRunTimeSettings->pProfileID->profileIndex = ~0;

        pRunTimeSettingsV4->pProfileID = pRunTimeSettings->pProfileID;
    }

    /* Initialise IPv4 Gateway Address - IPv4 info*/
    if( pRunTimeSettings->pGWAddressV4 )
    {
        *pRunTimeSettings->pGWAddressV4  = ~0;
        pRunTimeSettingsV4->pGWAddressV4 = pRunTimeSettings->pGWAddressV4;
    }
    /* Initialise IPv4 subnet mask - IPv4 info*/
    if( pRunTimeSettings->pSubnetMaskV4 )
    {
        *pRunTimeSettings->pSubnetMaskV4  = ~0;
        pRunTimeSettingsV4->pSubnetMaskV4 = pRunTimeSettings->pSubnetMaskV4;
    }
    /* Initialise PCSCF Address using PCO Flag - IPv4 info*/
    if( pRunTimeSettings->pPCSCFAddrPCO )
    {
        *pRunTimeSettings->pPCSCFAddrPCO  = ~0;
        pRunTimeSettingsV4->pPCSCFAddrPCO = pRunTimeSettings->pPCSCFAddrPCO;
    }
    /* Initialise PCSCF Server Address List - IPv4 info*/
    if( pRunTimeSettings->pServerAddrList )
    {
        pRunTimeSettingsV4->pServerAddrList = pRunTimeSettings->pServerAddrList;
    }
    /* Initialise PCSCF FQDN List - IPv4 info */
    if( pRunTimeSettings->pPCSCFFQDNAddrList )
    {
        pRunTimeSettingsV4->pPCSCFFQDNAddrList =
            pRunTimeSettings->pPCSCFFQDNAddrList;
    }
    /* Initialise Primary DNS IPV6 - IPv6 info */
    if( pRunTimeSettings->pPrimaryDNSV6 )
    {
        for( idx = 0; idx < 8; idx++ )
        {
            pRunTimeSettings->pPrimaryDNSV6[idx] = ~0;
        }
        pRunTimeSettingsV6->pPrimaryDNSV6 = pRunTimeSettings->pPrimaryDNSV6;
    }
    /* Initialise Secondary DNS IPV6 - IPv6 info */
    if( pRunTimeSettings->pSecondaryDNSV6 )
    {
        for( idx = 0; idx < 8; idx++ )
        {
            pRunTimeSettings->pPrimaryDNSV6[idx] = ~0;
        }
        pRunTimeSettingsV6->pSecondaryDNSV6 = pRunTimeSettings->pSecondaryDNSV6;
    }
    /* Initialise Mtu - IPv4 info */
    if( pRunTimeSettings->pMtu )
    {
        *(pRunTimeSettings->pMtu) = ~0;
        pRunTimeSettingsV4->pMtu = pRunTimeSettings->pMtu;
    }
    /* Initialise Domain List - IPv4 info */
    if( pRunTimeSettings->pDomainList )
    {
        pRunTimeSettingsV4->pDomainList = pRunTimeSettings->pDomainList;
    }
    /* Initialise IP Family Preference - IPv4 info */
    if( pRunTimeSettings->pIPFamilyPreference )
    {
        *(pRunTimeSettings->pIPFamilyPreference) = ~0;
        pRunTimeSettingsV4->pIPFamilyPreference =
            pRunTimeSettings->pIPFamilyPreference;
    }
    /* Initialise IM CN flag - IPv4 info */
    if( pRunTimeSettings->pIMCNflag )
    {
        *(pRunTimeSettings->pIMCNflag) = ~0;
        pRunTimeSettingsV4->pIMCNflag = pRunTimeSettings->pIMCNflag;
    }
    /* Initialise Technology - IPv4 info */
    if( pRunTimeSettings->pTechnology )
    {
        *(pRunTimeSettings->pTechnology) = ~0;
        pRunTimeSettingsV4->pTechnology = pRunTimeSettings->pTechnology;
    }
    /* Initialise IPV6 address info - IPv6 info */
    if( pRunTimeSettings->pIPV6AddrInfo )
    {
        struct IPV6AddressInfo *pIPV6AddrInfo;
        pIPV6AddrInfo = pRunTimeSettings->pIPV6AddrInfo;
        pIPV6AddrInfo->IPV6PrefixLen = ~0;
        for( idx = 0; idx < 8; idx++ )
        {
            pIPV6AddrInfo->IPAddressV6[idx] = ~0;
        }
        pRunTimeSettingsV6->pIPV6AddrInfo = pRunTimeSettings->pIPV6AddrInfo;
    }
    /* Initialise IPV6 Gateway address info - IPv6 info */
    if( pRunTimeSettings->pIPV6GWAddrInfo )
    {
        struct IPV6GWAddressInfo *pIPV6GWAddrInfo;
        pIPV6GWAddrInfo = pRunTimeSettings->pIPV6GWAddrInfo;
        pIPV6GWAddrInfo->gwV6PrefixLen = ~0;
        for( idx = 0; idx < 8; idx++ )
        {
            pIPV6GWAddrInfo->gwAddressV6[idx] = ~0;
        }
        pRunTimeSettingsV6->pIPV6GWAddrInfo = pRunTimeSettings->pIPV6GWAddrInfo;
    }
}

ULONG SLQSGetRuntimeSettings (
    struct WdsRunTimeSettings *pRunTimeSettings )
{
    /* Response struct to fetch IPv4 runtime settings */
    struct WdsRunTimeSettings RunTimeSettingsV4;
    /* Response struct to fetch IPv6 runtime settings */
    struct WdsRunTimeSettings RunTimeSettingsV6;

    slmemset( (char*) &RunTimeSettingsV4, 0, sizeof( struct WdsRunTimeSettings ) );
    slmemset( (char*) &RunTimeSettingsV6, 0, sizeof( struct WdsRunTimeSettings ) );

    /* Set input parameters to default values and initialise
     * response structures
     */
    InitRunTimeSettings( pRunTimeSettings,
                         &RunTimeSettingsV4,
                         &RunTimeSettingsV6 );

    struct qaWdsPDPSessionData *pd  = &wdsSessionManager[WDSPDP1IDX];
    enum   eQCWWANError        rcv4 = eQCWWAN_ERR_NONE;
    enum   eQCWWANError        rcv6 = eQCWWAN_ERR_NONE;
    enum   eQCWWANError        rc   = eQCWWAN_ERR_NONE;

    /* Set IP family to v4 */
    if( IPV4   == pd->IPFamilyPreference ||
        IPV4V6 == pd->IPFamilyPreference)
    {
        if( !IsGobiDevice() &&
            eQCWWAN_ERR_NONE != (rc = iSLQSSetIPFamilyPreference(IPV4)) )
        {
#ifdef DBG_WDS
wdsSessionDataLog(pd, NULL, &rc);
#endif
            return rc;
        }
        /* Get Runtime settings related to IPv4 */
        rcv4 = iSLQSGetRuntimeSettings( &RunTimeSettingsV4 );
    }

    /* set IP family to v6 */
    if( IPV6   == pd->IPFamilyPreference ||
        IPV4V6 == pd->IPFamilyPreference)
    {
        if( !IsGobiDevice() &&
            eQCWWAN_ERR_NONE != (rc = iSLQSSetIPFamilyPreference(IPV6)) )
        {
#ifdef DBG_WDS
wdsSessionDataLog(pd, NULL, &rc);
#endif
            return rc;
        }
        /* Get Runtime settings related to IPv6 */
        rcv6 = iSLQSGetRuntimeSettings( &RunTimeSettingsV6 );
    }

    switch( pd->IPFamilyPreference )
    {
        case IPV4:
            return rcv4;
            break;
        case IPV6:
            return rcv6;
            break;
        case IPV4V6:
            /* For IPv4v6, set the family preference to be returned as IPv4v6 */
            if( pRunTimeSettings->pIPFamilyPreference )
            {
                *pRunTimeSettings->pIPFamilyPreference = IPV4V6;
            }
            break;
    }

    /* IPV4V6 - Determine return codes to pass back to the caller */
    if( eQCWWAN_ERR_NONE == rcv4 &&
        eQCWWAN_ERR_NONE == rcv6 )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* v6 settings available */
    if( eQCWWAN_ERR_NONE == rcv6 )
    {
        if( eQCWWAN_ERR_QMI_OUT_OF_CALL == rcv4 )
        {
            /* v4 connection down */
            return eQCWWAN_ERR_SWICM_V4DWN_V6UP;
        }
        else
        {
            /* Error retrieving v4 settings */
            return rcv4;
        }
    }

    /* v4 settings available */
    if( eQCWWAN_ERR_NONE == rcv4 )
    {
        if( eQCWWAN_ERR_QMI_OUT_OF_CALL == rcv6 )
        {
            /* v6 connection down */
            return eQCWWAN_ERR_SWICM_V4UP_V6DWN;
        }
        else
        {
            /* Error retrieving v6 settings */
            return rcv6;
        }
    }

    /* v4 connection down */
    if( eQCWWAN_ERR_QMI_OUT_OF_CALL == rcv4 )
    {
        /* v6 connection down */
        if( eQCWWAN_ERR_QMI_OUT_OF_CALL == rcv6 )
        {
            return eQCWWAN_ERR_QMI_OUT_OF_CALL;
        }
        else
        {
            /* Error retrieving v6 settings */
            return rcv6;
        }
    }

    /* v6 connection down */
    if( eQCWWAN_ERR_QMI_OUT_OF_CALL == rcv6 )
    {
        /* v4 connection down */
        if( eQCWWAN_ERR_QMI_OUT_OF_CALL == rcv4 )
        {
            return eQCWWAN_ERR_QMI_OUT_OF_CALL;
        }
        else
        {
            /* Error retrieving v4 settings */
            return rcv4;
        }
    }

    /* Error retrieving both IP addresses & both sessions appear to be up.
     * Return first point of failure
     */
    return rcv4;
}

ULONG SLQSSetProfile(
    ULONG   profileType,
    BYTE    profileId,
    ULONG   *pPDPType,
    ULONG   *pIPAddress,
    ULONG   *pPrimaryDNS,
    ULONG   *pSecondaryDNS,
    ULONG   *pAuthentication,
    CHAR    *pName,
    CHAR    *pAPNName,
    CHAR    *pUsername,
    CHAR    *pPassword )
{
    ULONG  resultcode;            /* result code */
    BYTE   *pInParam;              /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;             /* ptr to outbound param field */
    BYTE   *pReqBuf;               /* Pointer to outgoing request buffer */
    USHORT ParamLength;           /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiWdsModifyProfileResp response;

    /* profileId should be between 1 - 16 */
    if( MIN_PROFILE_ID > profileId || MAX_PROFILE_ID < profileId )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Index points to first QMI header. */
    ParamLength = 0;

    /* pack QMI request */
    resultcode = PkQmiWdsModifyProfile( &ParamLength,
                                        pOutParam,
                                        profileType,
                                        profileId,
                                        pPDPType,
                                        pIPAddress,
                                        pPrimaryDNS,
                                        pSecondaryDNS,
                                        NULL,
                                        NULL,
                                        NULL,
                                        pAuthentication,
                                        pName,
                                        pAPNName,
                                        pUsername,
                                        pPassword );
    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* send QMI request */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WDS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S,
                                  &pInParam,
                                  &ParamLength );

    if (resultcode == eQCWWAN_ERR_NONE)
        resultcode = UpkQmiWdsModifyProfile(pInParam, &response);

    qmrelreqbkp();
    return resultcode;
}

ULONG SLQSGetProfile(
    ULONG profileType,
    BYTE  profileId,
    ULONG *pPDPType,
    ULONG *pIPAddress,
    ULONG *pPrimaryDNS,
    ULONG *pSecondaryDNS,
    ULONG *pAuthentication,
    BYTE  nameSize,
    CHAR  *pName,
    BYTE  apnSize,
    CHAR  *pAPNName,
    BYTE  userSize,
    CHAR  *pUsername,
    WORD  *pExtendedErrorCode )
{
    ULONG  resultcode;  /* result code */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT ParamLength; /* Ret'd length of the QMI Param field */
    WORD   passSize = 0; /* Set to 0 for compatibility */
    BYTE   lPDPType;    /* To retrieve PDP Type - for compatibility */
    BYTE   lAuth;       /* To retrieve Authentication - for compatibility */

    /* Storage for results and response variable */
    struct QmiWdsSlqsGetProfileSettingsResp response;
    struct Profile3GPP                      *pProfile3GPP;
    GetProfileSettingIn                     ProfileSettingReqParams;
    GetProfileSettingOut                    ProfileSettings;

    /* profileId should be between 1 - 16 */
    if( MIN_PROFILE_ID > profileId || MAX_PROFILE_ID < profileId )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Check against NULL for out params */
    if ( !pPDPType          ||
         !pIPAddress        ||
         !pPrimaryDNS       ||
         !pSecondaryDNS     ||
         !pAuthentication   ||
         !pName             ||
         !pAPNName          ||
         !pUsername )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Initialize request structure */
    ProfileSettingReqParams.ProfileID = profileId;
    ProfileSettingReqParams.ProfileType = profileType;

    /* pack QMI request */
    resultcode = PkQmiWdsSlqsGetProfileSettings( &ParamLength,
                                                 pOutParam,
                                                 &ProfileSettingReqParams );

    if ( eQCWWAN_ERR_NONE != resultcode )
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* send QMI request */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WDS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &ParamLength );

    if ( eQCWWAN_ERR_NONE == resultcode )
    {
        slmemset( (char *)&ProfileSettings,
                  0,
                  sizeof(GetProfileSettingOut) );

        /* initialize response structure */
        response.pProfileSettings = &ProfileSettings;

        pProfile3GPP = &(ProfileSettings.curProfile.SlqsProfile3GPP);

        pProfile3GPP->pPDPtype            = &lPDPType;
        pProfile3GPP->pIPv4AddrPref       = pIPAddress;
        pProfile3GPP->pPriDNSIPv4AddPref  = pPrimaryDNS;
        pProfile3GPP->pSecDNSIPv4AddPref  = pSecondaryDNS;
        pProfile3GPP->pAuthenticationPref = &lAuth;
        pProfile3GPP->pProfilename        = pName;
        pProfile3GPP->pAPNName            = pAPNName;
        pProfile3GPP->pUsername           = pUsername;
        pProfile3GPP->pProfilenameSize    = (WORD*) &nameSize;
        pProfile3GPP->pAPNnameSize        = (WORD*) &apnSize;
        pProfile3GPP->pUsernameSize       = (WORD*) &userSize;

        /* Set password size to zero as it isn't returned to user
         * This is done to make it compatible with SLQSGetProfileSettings()
         */
        pProfile3GPP->pPasswordSize       = &passSize;
        ProfileSettings.pExtErrCode     = pExtendedErrorCode;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiWdsSlqsGetProfileSettings( pInParam,
                                                      (BYTE*) &profileType,
                                                      &response );

        /* The below code has been added to maintain compatibility between
         * existing API interface to the user and the current unpacking
         * routines for QMI_WDS_GET_PROFILE_SETTINGS
         */
        if ( eQCWWAN_ERR_NONE == resultcode )
        {
            *pPDPType        = lPDPType;
            *pAuthentication = lAuth;
        }
    }

    qmrelreqbkp();
    return resultcode;
}

/* APIs Not Implemented */
ULONG SetActiveMobileIPProfile(
    CHAR *pSPC,
    BYTE index )
{
    UNUSEDPARAM( pSPC );
    UNUSEDPARAM( index );

    return eQCWWAN_ERR_SWICM_NOT_IMPLEMENTED;
}

ULONG GetActiveMobileIPProfile(
    BYTE *pIndex )
{
    UNUSEDPARAM( pIndex );

    return eQCWWAN_ERR_SWICM_NOT_IMPLEMENTED;
}

ULONG GetMobileIPProfile2(
    BYTE  index,
    BYTE  *pEnabled,
    ULONG *pAddress,
    ULONG *pPrimaryHA,
    ULONG *pSecondaryHA,
    BYTE  *pRevTunneling,
    BYTE  naiSize,
    CHAR  *pNAI,
    ULONG *pHASPI,
    ULONG *pAAASPI,
    ULONG *pHAState,
    ULONG *pAAAState )
{
    UNUSEDPARAM( index );
    UNUSEDPARAM( pEnabled );
    UNUSEDPARAM( pAddress );
    UNUSEDPARAM( pPrimaryHA );
    UNUSEDPARAM( pSecondaryHA );
    UNUSEDPARAM( pRevTunneling );
    UNUSEDPARAM( naiSize );
    UNUSEDPARAM( pNAI );
    UNUSEDPARAM( pHASPI );
    UNUSEDPARAM( pAAASPI );
    UNUSEDPARAM( pHAState );
    UNUSEDPARAM( pAAAState );

    return eQCWWAN_ERR_SWICM_NOT_IMPLEMENTED;
}

ULONG GetMobileIPParameters(
   ULONG *pMode,
   BYTE  *pRetryLimit,
   BYTE  *pRetryInterval,
   BYTE  *pReRegPeriod,
   BYTE  *pReRegTraffic,
   BYTE  *pHAAuthenticator,
   BYTE  *pHA2002bis )
{
    UNUSEDPARAM( pMode );
    UNUSEDPARAM( pRetryLimit );
    UNUSEDPARAM( pRetryInterval );
    UNUSEDPARAM( pReRegPeriod );
    UNUSEDPARAM( pReRegTraffic );
    UNUSEDPARAM( pHAAuthenticator );
    UNUSEDPARAM( pHA2002bis );

    return eQCWWAN_ERR_SWICM_NOT_IMPLEMENTED;
}

ULONG iGetIPAddressLTE(
    ULONG     *pIPAddressV4,
    USHORT    *pIPAddressV6,
    BYTE      *pIPv6prefixlen )
{
    ULONG          resultcode;  /* result code */
    BYTE          *pInParam;    /* ptr to param field rx'd from modem */
    BYTE          *pOutParam;   /* ptr to outbound param field */
    BYTE          *pReqBuf;     /* Pointer to outgoing request buffer */
    USHORT         ParamLength; /* Ret'd length of the QMI Param field */
    ULONG         *pRequestedSettings;


    /* Storage for results and response variable */
    struct QmiWdsGetRuntimeSettingsResp response;

    /* Bit-8 needs to be set in RequestedSettings inorder to fetch */
    ULONG settings = 0;
    settings |= BIT8;
    pRequestedSettings = &settings;

    if ( !pIPAddressV4 &&
         !pIPAddressV6 &&
         !pIPv6prefixlen )
    {
         return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Index points to first QMI header. */
    ParamLength = 0;
    resultcode = PkQmiWdsGetRuntimeSettings( &ParamLength,
                                         pOutParam,
                                         pRequestedSettings );
    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
     resultcode = SwiQmiSendnWait( pReqBuf,
                                   eQMI_SVC_WDS,
                                   ParamLength,
                                   eQMI_TIMEOUT_2_S,
                                   &pInParam,
                                   &ParamLength );
    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
    {
        /* Copy to the caller's buffer */
        response.pIPAddress     = pIPAddressV4;
        response.pIPAddressV6   = pIPAddressV6;
        response.pIPv6prefixlen = pIPv6prefixlen;

        resultcode = UpkQmiWdsGetRuntimeSettings(pInParam, &response);
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG GetIPAddressLTE(
    ULONG     *pIPAddressV4,
    USHORT    *pIPAddressV6,
    BYTE      *pIPv6prefixlen )
{
    /* Set input parameters to default values */
    if( pIPAddressV4 )
    {
        *pIPAddressV4 = 0;
    }

    if( pIPAddressV6 )
    {
        BYTE idx;
        for( idx = 0; idx < 8; idx++ )
        {
            pIPAddressV6[idx] = 0xFFFF;
        }
    }

    if( pIPv6prefixlen )
    {
        *pIPv6prefixlen = 0xFF;
    }

    struct qaWdsPDPSessionData *pd = &wdsSessionManager[WDSPDP1IDX];
    enum eQCWWANError rcv4 = eQCWWAN_ERR_NONE;
    enum eQCWWANError rcv6 = eQCWWAN_ERR_NONE;
    enum eQCWWANError rc   = eQCWWAN_ERR_NONE;

    /* Set IP family to v4 */
    if( IPV4   == pd->IPFamilyPreference ||
        IPV4V6 == pd->IPFamilyPreference)
    {
        if( !IsGobiDevice() &&
            eQCWWAN_ERR_NONE != (rc = iSLQSSetIPFamilyPreference(IPV4)) )
        {
#ifdef DBG_WDS
wdsSessionDataLog(pd, NULL, &rc);
#endif
            return rc;
        }
        /* Get v4 IP address */
        rcv4 = iGetIPAddressLTE( pIPAddressV4, NULL, NULL );
    }

    /* set IP family to v6 */
    if( IPV6   == pd->IPFamilyPreference ||
        IPV4V6 == pd->IPFamilyPreference)
    {
        if( !IsGobiDevice() &&
            eQCWWAN_ERR_NONE != (rc = iSLQSSetIPFamilyPreference(IPV6)) )
        {
#ifdef DBG_WDS
wdsSessionDataLog(pd, NULL, &rc);
#endif
            return rc;
        }
        /* Get v6 IP address */
        rcv6 = iGetIPAddressLTE( NULL, pIPAddressV6, pIPv6prefixlen );
    }

    switch( pd->IPFamilyPreference )
    {
        case IPV4:
            return rcv4;
            break;
        case IPV6:
            return rcv6;
            break;
        case IPV4V6:
            /* Logic handled below switch statement*/
            break;
    }

    /* IPV4V6 - Determine return codes to pass back to the caller */
    if( eQCWWAN_ERR_NONE == rcv4 &&
        eQCWWAN_ERR_NONE == rcv6 )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* v6 address available */
    if( eQCWWAN_ERR_NONE == rcv6 )
    {
        if( eQCWWAN_ERR_QMI_OUT_OF_CALL == rcv4 )
        {
            /* v4 connection down */
            return eQCWWAN_ERR_SWICM_V4DWN_V6UP;
        }
        else
        {
            /* Error retrieving v4 address */
            return rcv4;
        }
    }

    /* v4 address available */
    if( eQCWWAN_ERR_NONE == rcv4 )
    {
        if( eQCWWAN_ERR_QMI_OUT_OF_CALL == rcv6 )
        {
            /* v6 connection down */
            return eQCWWAN_ERR_SWICM_V4UP_V6DWN;
        }
        else
        {
            /* Error retrieving v6 address */
            return rcv6;
        }
    }

    /* v4 connection down */
    if( eQCWWAN_ERR_QMI_OUT_OF_CALL == rcv4 )
    {
        /* v6 connection down */
        if( eQCWWAN_ERR_QMI_OUT_OF_CALL == rcv6 )
        {
            return eQCWWAN_ERR_QMI_OUT_OF_CALL;
        }
        else
        {
            /* Error retrieving v6 address */
            return rcv6;
        }
    }

    /* v6 connection down */
    if( eQCWWAN_ERR_QMI_OUT_OF_CALL == rcv6 )
    {
        /* v4 connection down */
        if( eQCWWAN_ERR_QMI_OUT_OF_CALL == rcv4 )
        {
            return eQCWWAN_ERR_QMI_OUT_OF_CALL;
        }
        else
        {
            /* Error retrieving v4 address */
            return rcv4;
        }
    }

    /* Error retrieving both IP addresses & both sessions appear to be up.
     * Return first point of failure
     */
    return rcv4;
}

ULONG iGetIPAddress(ULONG *pIPAddress)
{
    ULONG  resultcode;  /* result code */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT ParamLength; /* Ret'd length of the QMI Param field */
    ULONG  *pRequestedSettings;

    if ( !pIPAddress )
    {
         return eQCWWAN_ERR_INVALID_ARG;
    }

    ULONG settings = 0;
    settings |= BIT8;
    pRequestedSettings = &settings;

    /* Storage for results and response variable */
    struct QmiWdsGetRuntimeSettingsResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Index points to first QMI header. */
    ParamLength = 0;
    resultcode = PkQmiWdsGetRuntimeSettings(
                &ParamLength,
                pOutParam,
                pRequestedSettings);

    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
     resultcode = SwiQmiSendnWait( pReqBuf,
                                    eQMI_SVC_WDS,
                                    ParamLength,
                                    eQMI_TIMEOUT_2_S,
                                    &pInParam,
                                    &ParamLength );
    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
    {
        /* Copy to the caller's buffer */
        response.pIPAddress     = pIPAddress;
        response.pIPAddressV6   = NULL;

        *(response.pIPAddress)  = 0;

        resultcode = UpkQmiWdsGetRuntimeSettings(pInParam, &response);
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG GetIPAddress(
    ULONG *pIPAddress )
{
    enum eQCWWANError eRCode;

    /* Change IP Family Preference to IPv4 to retrieve v4 address*/
    if( !IsGobiDevice() &&
        eQCWWAN_ERR_NONE != (eRCode = iSLQSSetIPFamilyPreference(IPV4)) )
    {
        return eRCode;
    }

    eRCode = iGetIPAddress( pIPAddress );

    /* Restore IP family preference */
    struct qaWdsPDPSessionData *pd = &wdsSessionManager[WDSPDP1IDX];
    enum eQCWWANError rc;
    if( !IsGobiDevice() &&
        eQCWWAN_ERR_NONE !=
        (rc = iSLQSSetIPFamilyPreference(pd->IPFamilyPreference)) )
    {
#ifdef DBG_WDS
    wdsSessionDataLog(pd, NULL, &rc);
#endif
    }

    return eRCode;
}

ULONG StartDataSession(
    ULONG *pTechnology,
    ULONG *pPrimaryDNS,
    ULONG *pSecondaryDNS,
    ULONG *pPrimaryNBNS,
    ULONG *pSecondaryNBNS,
    CHAR  *pAPNName,
    ULONG *pIPAddress,
    ULONG *pAuthentication,
    CHAR  *pUsername,
    CHAR  *pPassword,
    ULONG *pSessionId )
{
    ULONG FailureReason;
    ULONG rc;

    rc = StartDataSession2(  pTechnology,
                             pPrimaryDNS,
                             pSecondaryDNS,
                             pPrimaryNBNS,
                             pSecondaryNBNS,
                             pAPNName,
                             pIPAddress,
                             pAuthentication,
                             pUsername,
                             pPassword,
                             pSessionId,
                             &FailureReason );

    return rc;
}

ULONG  StartDataSession2(
    ULONG *pTechnology,
    ULONG *pPrimaryDNS,
    ULONG *pSecondaryDNS,
    ULONG *pPrimaryNBNS,
    ULONG *pSecondaryNBNS,
    CHAR  *pAPNName,
    ULONG *pIPAddress,
    ULONG *pAuthentication,
    CHAR  *pUsername,
    CHAR  *pPassword,
    ULONG *pSessionId,
    ULONG *pFailureReason )
{
    ULONG   rc;          /* result code */
    BYTE    *pInParam;   /* ptr to param field rx'd from modem */
    BYTE    *pOutParam;  /* ptr to outbound param field */
    BYTE    *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT  ParamLength; /* Ret'd length of the QMI Param field */

    struct qaWdsPDPSessionData *pd = &wdsSessionManager[WDSPDP1IDX];
#ifdef DBG_WDS
        wdsSessionDataLog(pd, NULL, NULL);
#endif
    /* input parameter validation */
    if( NULL == pSessionId || NULL == pFailureReason)
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* select v4 ip family */
    if( !IsGobiDevice() &&
        eQCWWAN_ERR_NONE != (rc = SLQSSetIPFamilyPreference(IPV4)) )
    {
        return rc;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    rc = PkQmiWdsStartNetworkInterface( &ParamLength,
                                        pOutParam,
                                        pTechnology,
                                        pPrimaryDNS,
                                        pSecondaryDNS,
                                        pPrimaryNBNS,
                                        pSecondaryNBNS,
                                        NULL,
                                        NULL,
                                        pAPNName,
                                        pIPAddress,
                                        NULL,
                                        pAuthentication,
                                        pUsername,
                                        pPassword,
                                        NULL,
                                        NULL,
                                        NULL,
                                        0);

    if( eQCWWAN_ERR_NONE != rc )
    {
        qmrelreqbkp();
        return rc;
    }

    /* Prepare and send the blocking call */
    rc = SwiQmiSendnWait( pReqBuf,
                          eQMI_SVC_WDS,
                          ParamLength,
                          eQMI_TIMEOUT_300_S,
                          &pInParam,
                          &ParamLength );

    if( eQCWWAN_ERR_NONE == rc )
    {
        struct QmiWdsStartNetworkInterfaceResp response;
        response.pPktDataHandle = pSessionId;
        response.pFailureReason = pFailureReason;

        /* Copy to the caller's buffer */
        rc = UpkQmiWdsStartNetworkInterface2(pInParam, &response);
        if( eQCWWAN_ERR_NONE == rc )
        {
            /* Store the IPv4 Session Handle */
            pd->lock(WDSPDP1IDX);
            pd->v4sessionId = *pSessionId;
            pd->unlock(WDSPDP1IDX);
        }
        else if( eQCWWAN_ERR_QMI_NO_EFFECT == rc )
        {
            /* if starting a session has no effect, assume the session is
             * active.
             */
            *pSessionId = pd->v4sessionId;
        }
        else
        {
            /* Otherwise, clear the caller's session handle */
            *pSessionId = 0;
            if( response.pVerboseFailureReason )
            {
                response.pFailureReason = response.pVerboseFailureReason;
            }
        }
    }
#ifdef DBG_WDS
    wdsSessionDataLog(pd, NULL, NULL);
#endif
    qmrelreqbkp();
    return rc;
}

local ULONG iSLQSStartStopDataSession(
    struct ssdatasession_params *pSessionParams )
{
    ULONG   rc;          /* result code */
    BYTE    *pInParam;   /* ptr to param field rx'd from modem */
    BYTE    *pOutParam;  /* ptr to outbound param field */
    BYTE    *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT  ParamLength; /* Ret'd length of the QMI Param field */
    ULONG   timeout;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Start Data Session */
    if( START_DATA_SESSION == pSessionParams->action )
    {
        /* Invoke the function which packs QMI message */
        rc = PkQmiWdsStartNetworkInterface2( &ParamLength,
                                             pOutParam,
                                             pSessionParams->pTechnology,
                                             pSessionParams->pProfileId3GPP,
                                             pSessionParams->pProfileId3GPP2 );

        timeout = eQMI_TIMEOUT_300_S; /* 5 minutes */
    }
    else /* Stop data session */
    {
        rc = PkQmiWdsStopNetworkInterface( &ParamLength,
                                           pOutParam,
                                           pSessionParams->sessionId,
                                           eWDS_STOP_NETWORK_INTERFACE_AUTOCONNECT_SETTING_UNCHANGED );

        timeout = eQMI_TIMEOUT_60_S; /* 1 minute */
    }

    if ( eQCWWAN_ERR_NONE != rc )
    {
        qmrelreqbkp();
        return rc;
    }

    /* send QMI request */
    rc = SwiQmiSendnWait( pReqBuf,
                          eQMI_SVC_WDS,
                          ParamLength,
                          timeout,
                          &pInParam,
                          &ParamLength );

    if ( eQCWWAN_ERR_NONE == rc )
    {
        if( START_DATA_SESSION == pSessionParams->action )
        {
            struct QmiWdsStartNetworkInterfaceResp response;
            response.pPktDataHandle = &pSessionParams->sessionId;
            response.pFailureReason = &pSessionParams->failureReason;

            /* Copy to the caller's buffer */
            rc = UpkQmiWdsStartNetworkInterface2( pInParam, &response );
            if ( response.pVerboseFailureReason )
            {
                response.pFailureReason = response.pVerboseFailureReason;
            }
        }
        else
        {
            struct QmiWdsStopNetworkInterfaceResp response;
            rc = UpkQmiWdsStopNetworkInterface( pInParam, &response );
        }
    }

    qmrelreqbkp();
    return rc;
}

enum eQCWWANError iSLQSStartDataSessionV4(
    struct qaWdsPDPSessionData *pd,
    struct ssdatasession_params *ps,
    BOOL *pSessionActive )
{
    enum eQCWWANError rc;
#ifdef DBG_WDS
    wdsSessionDataLog(pd, ps, NULL);
#endif
    *pSessionActive = FALSE;

    /* select v4 ip family */
    if( !IsGobiDevice() &&
        eQCWWAN_ERR_NONE != (rc = iSLQSSetIPFamilyPreference(IPV4) ) )
    {
        /* set v4 error code */
        ps->rcv4 = rc;
        /* clear failure reason */
        ps->failureReasonv4 = 0;
#ifdef DBG_WDS
    wdsSessionDataLog(pd, ps, &rc);
#endif
        return rc;
    }

    /* start IPv4 session */
    if( eQCWWAN_ERR_NONE !=
        (rc = iSLQSStartStopDataSession(ps) ) )
    {
        /* store v4 failure reason */
        ps->failureReasonv4 = ps->failureReason;

        if( eQCWWAN_ERR_QMI_NO_EFFECT != rc )
        {
            ps->sessionId = 0;
            ps->v4sessionId = 0;
        }
        else
        {
            /* if starting a session has no effect, assume the session is
             * active.
             */
            ps->sessionId = ps->v4sessionId = pd->v4sessionId;
            *pSessionActive = TRUE;
        }
    }
    else /* IPv4 successful */
    {
        /* mark session as active */
        *pSessionActive = TRUE;
        /* clear failures */
        ps->failureReason = ps->failureReasonv4 = 0;
        /* store the IPv4 Session Handle */
        ps->v4sessionId = ps->sessionId;
        pd->lock(pd->pdpIdx);
        pd->v4sessionId = ps->sessionId;
        pd->unlock(pd->pdpIdx);
    }
#ifdef DBG_WDS
    wdsSessionDataLog(pd, ps, NULL);
#endif
    /* set v4 error code */
    ps->rcv4 = rc;

    return rc;
}

enum eQCWWANError iSLQSStartDataSessionV6(
    struct qaWdsPDPSessionData *pd,
    struct ssdatasession_params *ps,
    BOOL *pSessionActive )
{
    enum eQCWWANError rc;
#ifdef DBG_WDS
    wdsSessionDataLog(pd, ps, NULL);
#endif
    *pSessionActive = FALSE;

    /* select v6 ip family */
    if( eQCWWAN_ERR_NONE !=
        (rc = iSLQSSetIPFamilyPreference(IPV6) ) )
    {
#ifdef DBG_WDS
    wdsSessionDataLog(pd, NULL, &rc);
#endif
        return rc;
    }

    /* start IPv6 session */
    if( eQCWWAN_ERR_NONE !=
        (rc = iSLQSStartStopDataSession(ps) ) )
    {
        ps->failureReasonv6 = ps->failureReason; /* store v6 failure reason */

        if( eQCWWAN_ERR_QMI_NO_EFFECT != rc )
        {
            ps->sessionId = 0;
            ps->v6sessionId = 0;
        }
        else
        {
            /* if starting a session has no effect, assume the session is
             * active.
             */
            ps->sessionId = ps->v6sessionId = pd->v6sessionId;
            *pSessionActive = TRUE;
        }
    }
    else /* IPv6 successful */
    {
        /* mark session as active */
        *pSessionActive = TRUE;
        /* clear failures */
        ps->failureReason = ps->failureReasonv6 = 0;
        /* store the IPv6 Session Handle */
        ps->v6sessionId = ps->sessionId;
        pd->lock(pd->pdpIdx);
        pd->v6sessionId = ps->sessionId;
        pd->unlock(pd->pdpIdx);
    }
#ifdef DBG_WDS
    wdsSessionDataLog(pd, ps, &rc);
#endif
    ps->rcv6 = rc;
    return rc;
}

local enum eQCWWANError iSLQSStartDataSession(
    struct ssdatasession_params *ps )
{
    /* active session flags */
    BOOL v4SA;
    BOOL v6SA;

    if( IPV4V6 == wdsSessionManager[WDSPDP1IDX].IPFamilyPreference ||
        IPV4   == wdsSessionManager[WDSPDP1IDX].IPFamilyPreference )
    {
        /*----------------*/
        /*-- V4 Handler --*/
        /*----------------*/
        iSLQSStartDataSessionV4(  &wdsSessionManager[WDSPDP1IDX],
                                  ps,
                                  &v4SA );
    }

    if( IPV4V6 == wdsSessionManager[WDSPDP1IDX].IPFamilyPreference ||
        IPV6   == wdsSessionManager[WDSPDP1IDX].IPFamilyPreference )
    {
        /*----------------*/
        /*-- V6 Handler --*/
        /*----------------*/
        iSLQSStartDataSessionV6(  &wdsSessionManager[WDSPDP1IDX],
                                  ps,
                                  &v6SA);
    }

    switch( wdsSessionManager[WDSPDP1IDX].IPFamilyPreference )
    {
        case IPV4:
            return ps->rcv4;
            break;

        case IPV6:
            return ps->rcv6;
            break;

        case IPV4V6:
            /* Handle IPV4V6 logic below switch */
            break;
        default:
            break;
    }

    /* IPV4V6 - Determine return codes to pass back to the caller */
    enum eQCWWANError rc;
    if( v4SA && v6SA ) /* v4 and v6 up */
    {
        rc = eQCWWAN_ERR_SWICM_V4UP_V6UP;
    }

    else if(v4SA) /* v4 up, v6 failed */
    {
        rc = eQCWWAN_ERR_SWICM_V4UP_V6DWN;
    }
    else if(v6SA) /* v4 fail, v6 up */
    {
        rc = eQCWWAN_ERR_SWICM_V4DWN_V6UP;
    }
    else /* v4 fail, v6 fail */
    {
        /* return first point of failure */
        rc = eQCWWAN_ERR_SWICM_V4DWN_V6DWN;
    }
    return rc;
}

local enum eQCWWANError iSLQSStopDataSessionV4(
    struct qaWdsPDPSessionData *pd,
    struct ssdatasession_params *ps,
    BOOL *pSessionInactive )
{
    enum eQCWWANError rc;

#ifdef DBG_WDS
    wdsSessionDataLog(pd, ps, NULL);
#endif
    *pSessionInactive = FALSE;

    if( pd->v4sessionId )
    {
        /* select v4 ip family */
        if( !IsGobiDevice() &&
            eQCWWAN_ERR_NONE != (rc = iSLQSSetIPFamilyPreference(IPV4)) )
        {
            /* set v4 error code */
            ps->rcv4 = rc;
            /* clear failure reason */
            ps->failureReasonv4 = 0;
#ifdef DBG_WDS
    wdsSessionDataLog(pd, ps, &rc);
#endif
            return rc;
        }

        /* cache caller's session id */
        const ULONG tempsid = ps->sessionId;

        /* stop session */
        ps->sessionId = pd->v4sessionId;
        if( eQCWWAN_ERR_NONE !=
            (rc = iSLQSStartStopDataSession(ps)) )
        {
            /* store v4 failure reason */
            ps->failureReasonv4 = ps->failureReason;

            if( eQCWWAN_ERR_QMI_NO_EFFECT != rc )
            {
                /* restore caller's session id */
                ps->sessionId = tempsid;
            }
            else
            {
                /* if stopping a session has no effect, assume the session is
                 * inactive.
                 */
                *pSessionInactive = TRUE;
            }
        }
        else
        {
            /* mark session as inactive */
            *pSessionInactive = TRUE;
        }
    }
    else
    {
        /* mark session as inactive */
        *pSessionInactive = TRUE;
        rc = eQCWWAN_ERR_QMI_NO_EFFECT;
    }
    if( *pSessionInactive )
    {
        /* clear caller's session id */
        ps->sessionId = 0;
        /* clear caller's IPv4 Session id */
        ps->v4sessionId = 0;
        /* clear session manager IPv4 Session id */
        pd->lock(pd->pdpIdx);
        pd->v4sessionId = 0;
        pd->unlock(pd->pdpIdx);
    }
#ifdef DBG_WDS
    wdsSessionDataLog(pd, ps, &rc);
#endif

    /* set v4 error code */
    ps->rcv4 = rc;
    return rc;
}

local enum eQCWWANError iSLQSStopDataSessionV6(
    struct qaWdsPDPSessionData *pd,
    struct ssdatasession_params *ps,
    BOOL *pSessionInactive )
{
    enum eQCWWANError rc;

#ifdef DBG_WDS
    wdsSessionDataLog(pd, ps, NULL);
#endif
    *pSessionInactive = FALSE;

    if( pd->v6sessionId )
    {
        /* select v6 ip family */
        if( eQCWWAN_ERR_NONE !=
            (rc = iSLQSSetIPFamilyPreference(IPV6)) )
        {
            /* set v6 error code */
            ps->rcv6 = rc;
            /* clear failure reason */
            ps->failureReasonv6 = 0;
#ifdef DBG_WDS
    wdsSessionDataLog(pd, ps, &rc);
#endif
            return rc;
        }

        /* cache caller's session id */
        const ULONG tempsid = ps->sessionId;

        /* stop session */
        ps->sessionId = pd->v6sessionId;
        if( eQCWWAN_ERR_NONE !=
            (rc = iSLQSStartStopDataSession(ps)) )
        {
            /* store v6 failure reason */
            ps->failureReasonv6 = ps->failureReason;

            if( eQCWWAN_ERR_QMI_NO_EFFECT != rc )
            {
                /* restore caller's session id */
                ps->sessionId = tempsid;
            }
            else
            {
                /* if stopping a session has no effect, assume the session is
                 * inactive.
                 */
                *pSessionInactive = TRUE;
            }
        }
        else
        {
            /* mark session as inactive */
            *pSessionInactive = TRUE;
        }
    }
    else
    {
        /* mark session as inactive */
        *pSessionInactive = TRUE;
        rc = eQCWWAN_ERR_QMI_NO_EFFECT;
    }
    if( *pSessionInactive )
    {
        /* clear caller's session id */
        ps->sessionId = 0;
        /* clear caller's IPv6 Session id */
        ps->v6sessionId = 0;
        /* clear session manager IPv6 Session id */
        pd->lock(pd->pdpIdx);
        pd->v6sessionId = 0;
        pd->unlock(pd->pdpIdx);
    }
#ifdef DBG_WDS
    wdsSessionDataLog(pd, ps, &rc);
#endif

    /* set v6 error code */
    ps->rcv6 = rc;
    return rc;
}

local enum eQCWWANError iSLQSStopDataSession(
    struct ssdatasession_params *ps )
{
    /* inactive session flags */
    BOOL v4SI = FALSE;
    BOOL v6SI = FALSE;

    if( IPV4V6 == wdsSessionManager[WDSPDP1IDX].IPFamilyPreference ||
        IPV4   == wdsSessionManager[WDSPDP1IDX].IPFamilyPreference )
    {
        /*----------------*/
        /*-- V4 Handler --*/
        /*----------------*/
        iSLQSStopDataSessionV4( &wdsSessionManager[WDSPDP1IDX],
                                ps,
                                &v4SI );
    }

    if( IPV4V6 == wdsSessionManager[WDSPDP1IDX].IPFamilyPreference ||
        IPV6   == wdsSessionManager[WDSPDP1IDX].IPFamilyPreference )
    {
        /*----------------*/
        /*-- V6 Handler --*/
        /*----------------*/
        iSLQSStopDataSessionV6(  &wdsSessionManager[WDSPDP1IDX],
                                 ps,
                                 &v6SI );
    }

    switch( wdsSessionManager[WDSPDP1IDX].IPFamilyPreference )
    {
        case IPV4:
            return ps->rcv4;
            break;

        case IPV6:
            return ps->rcv6;
            break;

        case IPV4V6:
            /* Handle IPV4V6 logic below switch */
            break;
        default:
            break;
    }

    /* IPV4V6 - Determine return codes to pass back to the caller */
    enum eQCWWANError rc;
    if( v4SI && v6SI ) /* v4 and v6 down */
    {
        rc = eQCWWAN_ERR_NONE;
    }

    else if(v4SI) /* v4 down, v6 up */
    {
        rc = eQCWWAN_ERR_SWICM_V4DWN_V6UP;
    }
    else if(v6SI) /* v4 up, v6 down */
    {
        rc = eQCWWAN_ERR_SWICM_V4UP_V6DWN;
    }
    else /* v4 up, v6 up */
    {
        rc = eQCWWAN_ERR_SWICM_V4UP_V6UP;
    }

    return rc;
}

ULONG SLQSStartStopDataSession(
    struct ssdatasession_params *pSSDataSession )
{
    ULONG rc;  /* result code */

    /* input parameter validation - start */
    if ( NULL == pSSDataSession )
    {
         return eQCWWAN_ERR_INVALID_ARG;
    }

    if( START_DATA_SESSION != pSSDataSession->action &&
        STOP_DATA_SESSION  != pSSDataSession->action )
    {
         return eQCWWAN_ERR_INVALID_ARG;
    }

    /* perform requested action */
    switch(pSSDataSession->action)
    {
        case START_DATA_SESSION:
            rc = iSLQSStartDataSession(pSSDataSession);
            break;

        case STOP_DATA_SESSION:
            rc = iSLQSStopDataSession(pSSDataSession);
            break;

        default:
            break;
    }

    return rc;
}

local ULONG iStartDataSessionLTE(
    ULONG  *pTechnology,
    ULONG  *pPrimaryDNSv4,
    ULONG  *pSecondaryDNSv4,
    ULONG  *pPrimaryNBNSv4,
    ULONG  *pSecondaryNBNSv4,
    USHORT *pPrimaryDNSv6,
    USHORT *pSecondaryDNSv6,
    CHAR   *pAPNName,
    ULONG  *pIPAddressv4,
    USHORT *pIPAddressv6,
    ULONG  *pAuthentication,
    CHAR   *pUsername,
    CHAR   *pPassword,
    ULONG  *pSessionId,
    ULONG  *pFailureReason )
{
    ULONG   rc;             /* result code */
    BYTE    *pInParam;      /* ptr to param field rx'd from modem */
    BYTE    *pOutParam;     /* ptr to outbound param field */
    BYTE    *pReqBuf;       /* Pointer to outgoing request buffer */
    USHORT  ParamLength;    /* Ret'd length of the QMI Param field */

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    rc = PkQmiWdsStartNetworkInterface( &ParamLength,
                                        pOutParam,
                                        pTechnology,
                                        pPrimaryDNSv4,
                                        pSecondaryDNSv4,
                                        pPrimaryNBNSv4,
                                        pSecondaryNBNSv4,
                                        pPrimaryDNSv6,
                                        pSecondaryDNSv6,
                                        pAPNName,
                                        pIPAddressv4,
                                        pIPAddressv6,
                                        pAuthentication,
                                        pUsername,
                                        pPassword,
                                        NULL,
                                        NULL,
                                        NULL,
                                        0);

    if (rc != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return rc;
    }

    /* Prepare and send the blocking call */
    rc = SwiQmiSendnWait( pReqBuf,
                          eQMI_SVC_WDS,
                          ParamLength,
                          eQMI_TIMEOUT_300_S,
                          &pInParam,
                          &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if (rc == eQCWWAN_ERR_NONE)
    {
        struct QmiWdsStartNetworkInterfaceResp response;

        /* Pass the pointer for the OUT parameters */
        response.pPktDataHandle = pSessionId;
        response.pFailureReason = pFailureReason;

        /* Copy to the caller's buffer */
        rc = UpkQmiWdsStartNetworkInterface2(pInParam, &response);
    }

    qmrelreqbkp();
    return rc;
}

enum eQCWWANError iStartDataSessionLTEV4(
    struct qaWdsPDPSessionData *pd,
    BOOL   *pSessionActive,
    ULONG  *pTechnology,
    ULONG  *pPrimaryDNSv4,
    ULONG  *pSecondaryDNSv4,
    ULONG  *pPrimaryNBNSv4,
    ULONG  *pSecondaryNBNSv4,
    USHORT *pPrimaryDNSv6,
    USHORT *pSecondaryDNSv6,
    CHAR   *pAPNName,
    ULONG  *pIPAddressv4,
    USHORT *pIPAddressv6,
    ULONG  *pAuthentication,
    CHAR   *pUsername,
    CHAR   *pPassword,
    ULONG  *pSessionId,
    ULONG  *pFailureReason )
{
    enum eQCWWANError rc;
#ifdef DBG_WDS
    wdsSessionDataLog(pd, NULL, NULL);
#endif
    *pSessionActive = FALSE;

    /* select v4 ip family */
    if( !IsGobiDevice() &&
        eQCWWAN_ERR_NONE != (rc = iSLQSSetIPFamilyPreference(IPV4) ) )
    {
#ifdef DBG_WDS
        wdsSessionDataLog(pd, NULL, &rc);
#endif
        return rc;
    }

    /* start IPv4 session */
    if( eQCWWAN_ERR_NONE !=
        (rc = iStartDataSessionLTE( pTechnology,
                                    pPrimaryDNSv4,
                                    pSecondaryDNSv4,
                                    pPrimaryNBNSv4,
                                    pSecondaryNBNSv4,
                                    pPrimaryDNSv6,
                                    pSecondaryDNSv6,
                                    pAPNName,
                                    pIPAddressv4,
                                    pIPAddressv6,
                                    pAuthentication,
                                    pUsername,
                                    pPassword,
                                    pSessionId,
                                    pFailureReason)) )
    {
        if( eQCWWAN_ERR_QMI_NO_EFFECT != rc )
        {
            *pSessionId = 0;
        }
        else
        {
            /* if starting a session has no effect, assume the session is
             * active.
             */
            *pSessionId = pd->v4sessionId;
            *pSessionActive = TRUE;
        }
    }
    else /* IPv4 successful */
    {
        /* mark session as active */
        *pSessionActive = TRUE;
        /* store the IPv4 Session Handle */
        pd->lock(pd->pdpIdx);
        pd->v4sessionId = *pSessionId;
        pd->unlock(pd->pdpIdx);
    }
#ifdef DBG_WDS
    wdsSessionDataLog(pd, NULL, NULL);
#endif
    return rc;
}

enum eQCWWANError iStartDataSessionLTEV6(
    struct qaWdsPDPSessionData *pd,
    BOOL   *pSessionActive,
    ULONG  *pTechnology,
    ULONG  *pPrimaryDNSv4,
    ULONG  *pSecondaryDNSv4,
    ULONG  *pPrimaryNBNSv4,
    ULONG  *pSecondaryNBNSv4,
    USHORT *pPrimaryDNSv6,
    USHORT *pSecondaryDNSv6,
    CHAR   *pAPNName,
    ULONG  *pIPAddressv4,
    USHORT *pIPAddressv6,
    ULONG  *pAuthentication,
    CHAR   *pUsername,
    CHAR   *pPassword,
    ULONG  *pSessionId,
    ULONG  *pFailureReason )
{
    enum eQCWWANError rc;
#ifdef DBG_WDS
    wdsSessionDataLog(pd, NULL, NULL);
#endif
    *pSessionActive = FALSE;

    /* select v6 ip family */
    if( eQCWWAN_ERR_NONE !=
        (rc = iSLQSSetIPFamilyPreference(IPV6) ) )
    {
#ifdef DBG_WDS
    wdsSessionDataLog(pd, NULL, &rc);
#endif
        return rc;
    }

    /* start IPv6 session */
    if( eQCWWAN_ERR_NONE !=
        (rc = iStartDataSessionLTE( pTechnology,
                                    pPrimaryDNSv4,
                                    pSecondaryDNSv4,
                                    pPrimaryNBNSv4,
                                    pSecondaryNBNSv4,
                                    pPrimaryDNSv6,
                                    pSecondaryDNSv6,
                                    pAPNName,
                                    pIPAddressv4,
                                    pIPAddressv6,
                                    pAuthentication,
                                    pUsername,
                                    pPassword,
                                    pSessionId,
                                    pFailureReason)) )
    {
        if( eQCWWAN_ERR_QMI_NO_EFFECT != rc )
        {
            *pSessionId = 0;
        }
        else
        {
            /* if starting a session has no effect, assume the session is
             * active.
             */
            *pSessionId = pd->v6sessionId;
            *pSessionActive = TRUE;
        }
    }
    else /* IPv6 successful */
    {
        /* store the IPv6 Session Handle */
        *pSessionActive = TRUE;
        pd->lock(pd->pdpIdx);
        pd->v6sessionId = *pSessionId;
        pd->unlock(pd->pdpIdx);
    }
#ifdef DBG_WDS
    wdsSessionDataLog(pd, NULL, NULL);
#endif
    return rc;
}

ULONG StartDataSessionLTE(
    ULONG  *pTechnology,
    ULONG  *pPrimaryDNSv4,
    ULONG  *pSecondaryDNSv4,
    ULONG  *pPrimaryNBNSv4,
    ULONG  *pSecondaryNBNSv4,
    USHORT *pPrimaryDNSv6,
    USHORT *pSecondaryDNSv6,
    CHAR   *pAPNName,
    ULONG  *pIPAddressv4,
    USHORT *pIPAddressv6,
    ULONG  *pAuthentication,
    CHAR   *pUsername,
    CHAR   *pPassword,
    ULONG  *pSessionId,
    ULONG  *pFailureReason,
    BYTE   IPFamilyPreference )
{
    ULONG rc;

    UNUSEDPARAM(IPFamilyPreference);

   /* input parameter validation */
    if( NULL == pSessionId || NULL == pFailureReason )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* active session flags */
    BOOL v4SA = FALSE;
    BOOL v6SA = FALSE;

    /* session IDs */
    ULONG v4SID;
    ULONG v6SID;

    /* failure reasons */
    ULONG v4FR;
    ULONG v6FR;

    /* return codes */
    enum eQCWWANError rc4;
    enum eQCWWANError rc6;

    if( IPV4V6 == wdsSessionManager[WDSPDP1IDX].IPFamilyPreference ||
        IPV4   == wdsSessionManager[WDSPDP1IDX].IPFamilyPreference )
    {
        /*----------------*/
        /*-- V4 Handler --*/
        /*----------------*/
        rc4 = iStartDataSessionLTEV4(   &wdsSessionManager[WDSPDP1IDX],
                                        &v4SA,
                                        pTechnology,
                                        pPrimaryDNSv4,
                                        pSecondaryDNSv4,
                                        pPrimaryNBNSv4,
                                        pSecondaryNBNSv4,
                                        pPrimaryDNSv6,
                                        pSecondaryDNSv6,
                                        pAPNName,
                                        pIPAddressv4,
                                        pIPAddressv6,
                                        pAuthentication,
                                        pUsername,
                                        pPassword,
                                        &v4SID,
                                        &v4FR );
        *pSessionId = v4SID;
    }

    if( IPV4V6 == wdsSessionManager[WDSPDP1IDX].IPFamilyPreference ||
        IPV6   == wdsSessionManager[WDSPDP1IDX].IPFamilyPreference )
    {
        /*----------------*/
        /*-- V6 Handler --*/
        /*----------------*/
        rc6 = iStartDataSessionLTEV6(   &wdsSessionManager[WDSPDP1IDX],
                                        &v6SA,
                                        pTechnology,
                                        pPrimaryDNSv4,
                                        pSecondaryDNSv4,
                                        pPrimaryNBNSv4,
                                        pSecondaryNBNSv4,
                                        pPrimaryDNSv6,
                                        pSecondaryDNSv6,
                                        pAPNName,
                                        pIPAddressv4,
                                        pIPAddressv6,
                                        pAuthentication,
                                        pUsername,
                                        pPassword,
                                        &v6SID,
                                        &v6FR );
        *pSessionId = v6SID;
    }

    switch( wdsSessionManager[WDSPDP1IDX].IPFamilyPreference )
    {
        case IPV4:
            return rc4;
            break;

        case IPV6:
            return rc6;
            break;

        case IPV4V6:
            /* Handle IPV4V6 logic below switch */
            break;
        default:
            break;
    }

    /* IPV4V6 - Determine what session ID, failure reason, and return codes to
     * pass back to the caller.
     */

    if( v4SA && v6SA ) /* v4 and v6 up */
    {
        *pSessionId = v6SID; /* doesn't really matter which SID goes back */
        *pFailureReason = 0; /* clear failure reason */
        rc = eQCWWAN_ERR_NONE;
    }

    else if(v4SA) /* v4 up, v6 failed */
    {
        *pSessionId = v4SID;
        *pFailureReason = v6FR;
        rc = rc6;
    }
    else if(v6SA) /* v4 fail, v6 up */
    {
        *pSessionId = v6SID;
        *pFailureReason = v4FR;
        rc = rc4;
    }
    else /* v4 fail, v6 fail */
    {
        *pSessionId = 0;
        /* return first point of failure */
        *pFailureReason = v4FR;
        rc = rc4;
    }
    return rc;
}

local ULONG iStopDataSession(ULONG sessionId)
{
    ULONG   rc;  /* result code */
    BYTE    *pInParam;   /* ptr to param field rx'd from modem */
    BYTE    *pOutParam;  /* ptr to outbound param field */
    BYTE    *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT  ParamLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiWdsStopNetworkInterfaceResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    rc = PkQmiWdsStopNetworkInterface( &ParamLength,
                                       pOutParam,
                                       sessionId,
                                       0 );

    if (rc != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return rc;
    }

    /* Prepare and send the blocking call */
    rc = SwiQmiSendnWait( pReqBuf,
                          eQMI_SVC_WDS,
                          ParamLength,
                          eQMI_TIMEOUT_60_S, /* 1 minute */
                          &pInParam,
                          &ParamLength );

    if (rc == eQCWWAN_ERR_NONE)
    {
        rc = UpkQmiWdsStopNetworkInterface( pInParam, &response );
    }

    qmrelreqbkp();
    return rc;
}

enum eQCWWANError iStopDataSessionV4(
    struct qaWdsPDPSessionData *pd,
    BOOL *pv4SessionDown,
    BOOL *pv4SessionTerminated )
{
    enum eQCWWANError rc4;

#ifdef DBG_WDS
        wdsSessionDataLog(pd, NULL, NULL);
#endif
    /* start with the assumption that there's nothing to do */
    *pv4SessionDown = TRUE;

    if( pd->v4sessionId )
    {
        *pv4SessionDown = FALSE;

        /* select v4 ip family */
        if( !IsGobiDevice() &&
            eQCWWAN_ERR_NONE != (rc4 = iSLQSSetIPFamilyPreference(IPV4)) )
        {
            return rc4;
        }

        /* stop session */
        if( eQCWWAN_ERR_NONE !=
            (rc4 = iStopDataSession(pd->v4sessionId)) )

        {
            if( eQCWWAN_ERR_QMI_NO_EFFECT == rc4 )
            {
                /* handle a lost connection by clearing the v4 session id */
                pd->v4sessionId = 0;
                *pv4SessionTerminated = TRUE;
            }
        }
        else
        {
            /* clear v4 session id */
            pd->v4sessionId = 0;
            *pv4SessionTerminated = TRUE;
        }
    }
    else
    {
        rc4 = eQCWWAN_ERR_QMI_NO_EFFECT;
    }
#ifdef DBG_WDS
        wdsSessionDataLog(pd, NULL, &rc4);
#endif

    return rc4;
}

enum eQCWWANError iStopDataSessionV6(
    struct qaWdsPDPSessionData *pd,
    BOOL *pv6SessionDown,
    BOOL *pv6SessionTerminated )
{
    enum eQCWWANError rc6;

#ifdef DBG_WDS
    wdsSessionDataLog(pd, NULL, NULL);
#endif
    /* start with the assumption that there's nothing to do */
    *pv6SessionDown = TRUE;

    if( pd->v6sessionId )
    {
        *pv6SessionDown = FALSE;

        /* select v6 ip family */
        rc6 = iSLQSSetIPFamilyPreference(IPV6);

        /* disconnect active IPv6 session */
        if( eQCWWAN_ERR_NONE !=
            (rc6 = iStopDataSession(pd->v6sessionId)) )
        {
            if( eQCWWAN_ERR_QMI_NO_EFFECT == rc6 )
            {
                /* handle a lost connection by clearing the v6 session id */
                pd->v6sessionId = 0;
                *pv6SessionTerminated = TRUE;
            }
        }
        else
        {
            /* clear v6 session id */
            pd->v6sessionId = 0;
            *pv6SessionTerminated = TRUE;
        }
    }
    else
    {
        rc6 = eQCWWAN_ERR_QMI_NO_EFFECT;
    }
#ifdef DBG_WDS
    wdsSessionDataLog(pd, NULL, &rc6);
#endif
    return rc6;
}

ULONG StopDataSession(ULONG sessionId)
{
    UNUSEDPARAM(sessionId);
#ifdef DBG_WDS
    wdsSessionDataLog(&wdsSessionManager[WDSPDP1IDX], NULL, NULL);
#endif
    /*----------------*/
    /*-- V4 Handler --*/
    /*----------------*/
    BOOL v4SD;
    BOOL v4ST;
    ULONG rc4 = iStopDataSessionV4( &wdsSessionManager[WDSPDP1IDX], &v4SD, &v4ST);

    /*----------------*/
    /*-- V6 Handler --*/
    /*----------------*/
    BOOL v6SD;
    BOOL v6ST;
    ULONG rc6 = iStopDataSessionV6( &wdsSessionManager[WDSPDP1IDX], &v6SD, &v6ST);

/*
Legend: 4=IPv4, 6=IPv6, D=Session Down, T=Session Terminated, x = not applicable
        TS=Termination Successful, TU=Termination Unsuccessful

4 D 6   4 T 6   Error Code  Explanation
1   1   x   x   no effect   "v4 (was) down, v6 (was) down, v4/v6 termination n/a"
1   0   x   0   from API    "v4 down, v6 up, v6 TU"
1   0   x   1   success     "v4 down, v6 up, v6 TS"
0   1   0   x   from API    "v4 up, v6 down, v4 TU"
0   1   1   x   success     "v4 up, v6 down, v4 TS"
0   0   0   0   both up     "v4 up, v6 up, v4 TU v6 TU"
0   0   0   1   4u6d        "v4 up, v6 up, v4 TU v6 TS"
0   0   1   0   4d6u        "v4 up, v6 up, v4 TS v6 TU"
0   0   1   1   4u6u        "v4 up, v6 up, v4 TS v6 TS"
*/
    BYTE D = v4SD && v6SD ? 3 : (v4SD ? 2 : (v6SD ? 1 : 0) );
    enum eQCWWANError rc;
    switch(D)
    {
        case 0: /* v4 & v6 were both up */
            rc = v4ST && v6ST
                 ? eQCWWAN_ERR_NONE /* both sessions stopped */
                 : TRUE == v4ST
                    ? eQCWWAN_ERR_SWICM_V4DWN_V6UP /* v4 session stopped */
                    : TRUE == v6ST
                        ? eQCWWAN_ERR_SWICM_V4UP_V6DWN /* v6 session stopped */
                        : eQCWWAN_ERR_SWICM_V4UP_V6UP ; /* no session stopped */
            break;

        case 1: /* v4 was up */
            rc = rc4; /* single session, return QMI error code */
            break;

        case 2: /* v6 was up */
            rc = rc6; /* single session, return QMI error code */
            break;

        case 3: /* v4 & v6 were both down */
            rc = eQCWWAN_ERR_QMI_NO_EFFECT;
            break;

        default:
            break;
    }

    return rc;
}

ULONG SLQSDeleteProfile(
    struct SLQSDeleteProfileParams *pProfileToDelete,
    WORD                           *pExtendedErrorCode )
{
    ULONG  resultCode;  /* result code */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT ParamLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiWdsSlqsDeleteProfileResp response;

    /* validate input parameters  */
    if( !pProfileToDelete )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* pack QMI request */
    resultCode = PkQmiWdsSlqsDeleteProfile( &ParamLength,
                                            pOutParam,
                                            pProfileToDelete->profileType,
                                            pProfileToDelete->profileIndex );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode;
    }

    /* send QMI request */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WDS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &ParamLength );

    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        response.pExtendedErrorCode = pExtendedErrorCode;
        resultCode = UpkQmiWdsSlqsDeleteProfile( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSCreateProfile(
    struct CreateProfileIn  *pCreateProfileIn,
    struct CreateProfileOut *pCreateProfileOut )
{
    ULONG  resultcode;  /* result code */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT ParamLength; /* Ret'd length of the CnS Param field */

    /* NULL check of IN and OUT Parameters */
    if ( !pCreateProfileIn ||
         !pCreateProfileOut )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Storage for results and response variable */
    struct QmiWdsSLQSCreateProfileResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Index points to first QMI header. */
    ParamLength = 0;

    /* pack QMI request */
    resultcode = PkQmiWdsSLQSCreateProfile(
                     &ParamLength,
                     pOutParam,
                     pCreateProfileIn );

    if ( eQCWWAN_ERR_NONE != resultcode )
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WDS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S,
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultcode )
    {
        response.pSlqsCreateProfileOut = pCreateProfileOut;

        /* Unpack the parameters */
        resultcode = UpkQmiWdsSLQSCreateProfile( pInParam,
                                                 &response,
                                                 pCreateProfileIn->pProfileID );
    }
    qmrelreqbkp();
    return resultcode;
}

ULONG SLQSAutoConnect( struct slqsautoconnect *pAcreq )
{
    ULONG  resultcode;  /* result code */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT ParamLength; /* Ret'd length of the Param field */
    ULONG  timeout;

    /* NULL check for OUT Parameters */
    if ( !pAcreq )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* validate action value passed by the user */
    if( ( SET_AUTO_CONNECT != pAcreq->action ) &&
        ( GET_AUTO_CONNECT != pAcreq->action ) )
    {
         return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Index points to first QMI header. */
    ParamLength = 0;

    /* Set Autoconnect */
    if( SET_AUTO_CONNECT == pAcreq->action )
    {
        /* pack QMI request */
        resultcode = PkQmiWdsSLQSSetAutoConnect( &ParamLength,
                                                 pOutParam,
                                                 pAcreq->acsetting,
                                                 pAcreq->acroamsetting );

        timeout = eQMI_TIMEOUT_300_S; /* 5 minutes */
    }
    else
    {
        /* pack QMI request */
        resultcode = PkQmiWdsSLQSGetAutoConnect( &ParamLength,
                                                 pOutParam );

        timeout = eQMI_TIMEOUT_2_S; /* 2 second */
    }

    if ( eQCWWAN_ERR_NONE != resultcode )
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WDS,
                                  ParamLength,
                                  timeout,
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultcode )
    {
        if( SET_AUTO_CONNECT == pAcreq->action )
        {
            /* Unpack the parameters */
            struct QmiWdsSLQSSetAutoConnectResp response;
            resultcode = UpkQmiWdsSLQSSetAutoConnect( pInParam, &response );
        }
        else
        {
            struct QmiWdsSLQSGetAutoConnectResp response;
            response.slqsautoconnectResp.acroamsetting = 0;

            /* Unpack the parameters */
            resultcode = UpkQmiWdsSLQSGetAutoConnect( pInParam, &response );

            /* Assign fectched parameters to structure */
            pAcreq->acsetting     = response.slqsautoconnectResp.acsetting;
            pAcreq->acroamsetting = response.slqsautoconnectResp.acroamsetting;
        }
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG SLQSModifyProfile(
    struct ModifyProfileIn  *pModifyProfileIn,
    struct ModifyProfileOut *pModifyProfileOut )
{
    ULONG  resultcode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT ParamLength; /* Ret'd length of the CnS Param field */

    /* NULL check of IN and OUT Parameters */
    if ( !pModifyProfileIn ||
         !pModifyProfileIn->pProfileID ||
         !pModifyProfileIn->pProfileType ||
         !pModifyProfileOut )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Storage for results and response variable */
    struct QmiWdsSLQSModifyProfileResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Index points to first QMI header. */
    ParamLength = 0;

    /* pack QMI request */
    resultcode = PkQmiWdsSLQSModifyProfile( &ParamLength,
                                            pOutParam,
                                            pModifyProfileIn );

    if ( eQCWWAN_ERR_NONE != resultcode )
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WDS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S,
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultcode )
    {
        response.pSlqsModifyProfileOut = pModifyProfileOut;

        /* Unpack the parameters */
        resultcode = UpkQmiWdsSLQSModifyProfile( pInParam,
                                                 &response );
    }
    qmrelreqbkp();
    return resultcode;
}

ULONG SLQSSet3GPPConfigItem( slqs3GPPConfigItem *pSLQS3GPPConfigItem )
{
    ULONG  resultcode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiWdsSLQSSet3GPPConfigItemResp response;

    /* Check against NULL for in params */
    if ( !pSLQS3GPPConfigItem )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Atleast one IN param should not be NULL */
    if( !pSLQS3GPPConfigItem->pLTEAttachProfile  &&
        !pSLQS3GPPConfigItem->pProfileList       &&
        !pSLQS3GPPConfigItem->pDefaultPDNEnabled &&
        !pSLQS3GPPConfigItem->p3gppRelease )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* pack QMI request */
    resultcode = PkQmiWdsSLQSSet3GPPConfigItem( &paramLength,
                                                pOutParam,
                                                pSLQS3GPPConfigItem );

    if ( eQCWWAN_ERR_NONE != resultcode )
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* send QMI request and receive the response */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WDS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &paramLength );

    if ( eQCWWAN_ERR_NONE == resultcode )
    {
        /* Copy to the caller's buffer */
        resultcode = UpkQmiWdsSLQSSet3GPPConfigItem( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG SLQSGet3GPPConfigItem( slqs3GPPConfigItem *pSLQS3GPPConfigItem )
{
    ULONG  resultcode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */
    BYTE   counter = 0; /* counter for loop used to initialize profileList */

    /* Storage for results and response variable */
    struct QmiWdsSLQSGet3GPPConfigItemResp response;

    /* Check against NULL for out params */
    if ( !pSLQS3GPPConfigItem )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Atleast one OUT param should not be NULL */
    if( !pSLQS3GPPConfigItem->pLTEAttachProfile  &&
        !pSLQS3GPPConfigItem->pProfileList       &&
        !pSLQS3GPPConfigItem->pDefaultPDNEnabled &&
        !pSLQS3GPPConfigItem->p3gppRelease )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* pack QMI request */
    resultcode = PkQmiWdsSLQSGet3GPPConfigItem( &paramLength, pOutParam );

    if ( eQCWWAN_ERR_NONE != resultcode )
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* send QMI request and receive the response */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WDS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &paramLength );

    if ( eQCWWAN_ERR_NONE == resultcode )
    {
        /* initialize response structure */
        response.pSLQS3GPPConfigItem = pSLQS3GPPConfigItem;
        if( pSLQS3GPPConfigItem->pProfileList )
        {
            for( counter = 0; counter <= 4; counter++ )
            {
                pSLQS3GPPConfigItem->pProfileList[counter] = 0xFF;
            }
        }
        if( pSLQS3GPPConfigItem->pLTEAttachProfile )
            *(response.pSLQS3GPPConfigItem->pLTEAttachProfile) = 0xFF;
        if( pSLQS3GPPConfigItem->pDefaultPDNEnabled )
            *(response.pSLQS3GPPConfigItem->pDefaultPDNEnabled) = 0xFF;
        if( pSLQS3GPPConfigItem->p3gppRelease )
            *(response.pSLQS3GPPConfigItem->p3gppRelease) = 0xFF;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiWdsSLQSGet3GPPConfigItem( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

package void qaWdsInit(void)
{
    int i = WDSPDP3IDX;
    do{
        swi_osapi_mutexinit(&wdsSessionManager[i].mutex);
        wdsSessionManager[i].IPFamilyPreference = IPV4;
        wdsSessionManager[i].lock = qaWdsPDPSessionDataLock;
        wdsSessionManager[i].unlock = qaWdsPDPSessionDataUnLock;
        wdsSessionManager[i].pdpIdx = i;
    }while( i--);
}

local void initProf3GPP( struct Profile3GPP *pProfile3GPP )
{
    BYTE idx = 0;
    if( pProfile3GPP->pProfilename )
    {
        *(pProfile3GPP->pProfilename) = EOS;
    }
    if( pProfile3GPP->pPDPtype )
    {
        *(pProfile3GPP->pPDPtype) = ~0;
    }
    if( pProfile3GPP->pPdpHdrCompType )
    {
        *(pProfile3GPP->pPdpHdrCompType) = ~0;
    }
    if( pProfile3GPP->pPdpDataCompType )
    {
        *(pProfile3GPP->pPdpDataCompType) = ~0;
    }
    if( pProfile3GPP->pAPNName )
    {
        *(pProfile3GPP->pAPNName) = EOS;
    }
    if( pProfile3GPP->pPriDNSIPv4AddPref )
    {
        *(pProfile3GPP->pPriDNSIPv4AddPref) = ~0;
    }
    if( pProfile3GPP->pSecDNSIPv4AddPref )
    {
        *(pProfile3GPP->pSecDNSIPv4AddPref) = ~0;
    }
    if( pProfile3GPP->pUMTSReqQoS )
    {
        struct UMTSQoS *pUMTSQoS = pProfile3GPP->pUMTSReqQoS;
        pUMTSQoS->trafficClass        = ~0;
        pUMTSQoS->maxUplinkBitrate    = ~0;
        pUMTSQoS->maxDownlinkBitrate  = ~0;
        pUMTSQoS->grntUplinkBitrate   = ~0;
        pUMTSQoS->grntDownlinkBitrate = ~0;
        pUMTSQoS->qosDeliveryOrder    = ~0;
        pUMTSQoS->maxSDUSize          = ~0;
        pUMTSQoS->sduErrorRatio       = ~0;
        pUMTSQoS->resBerRatio         = ~0;
        pUMTSQoS->deliveryErrSDU      = ~0;
        pUMTSQoS->transferDelay       = ~0;
        pUMTSQoS->trafficPriority     = ~0;
    }
    if( pProfile3GPP->pUMTSMinQoS )
    {
        struct UMTSQoS *pUMTSQoS = pProfile3GPP->pUMTSMinQoS;
        pUMTSQoS->trafficClass        = ~0;
        pUMTSQoS->maxUplinkBitrate    = ~0;
        pUMTSQoS->maxDownlinkBitrate  = ~0;
        pUMTSQoS->grntUplinkBitrate   = ~0;
        pUMTSQoS->grntDownlinkBitrate = ~0;
        pUMTSQoS->qosDeliveryOrder    = ~0;
        pUMTSQoS->maxSDUSize          = ~0;
        pUMTSQoS->sduErrorRatio       = ~0;
        pUMTSQoS->resBerRatio         = ~0;
        pUMTSQoS->deliveryErrSDU      = ~0;
        pUMTSQoS->transferDelay       = ~0;
        pUMTSQoS->trafficPriority     = ~0;
    }
    if( pProfile3GPP->pGPRSRequestedQos )
    {
        struct GPRSRequestedQoS *pGPRSReqQoS = pProfile3GPP->pGPRSRequestedQos;
        pGPRSReqQoS->precedenceClass     = ~0;
        pGPRSReqQoS->delayClass          = ~0;
        pGPRSReqQoS->reliabilityClass    = ~0;
        pGPRSReqQoS->peakThroughputClass = ~0;
        pGPRSReqQoS->meanThroughputClass = ~0;
    }
    if( pProfile3GPP->pGPRSMinimumQoS )
    {
        struct GPRSRequestedQoS *pGPRSReqQoS = pProfile3GPP->pGPRSMinimumQoS;
        pGPRSReqQoS->precedenceClass     = ~0;
        pGPRSReqQoS->delayClass          = ~0;
        pGPRSReqQoS->reliabilityClass    = ~0;
        pGPRSReqQoS->peakThroughputClass = ~0;
        pGPRSReqQoS->meanThroughputClass = ~0;
    }
    if( pProfile3GPP->pUsername )
    {
        *(pProfile3GPP->pUsername) = EOS;
    }
    if( pProfile3GPP->pPassword )
    {
        *(pProfile3GPP->pPassword) = EOS;
    }
    if( pProfile3GPP->pAuthenticationPref )
    {
        *(pProfile3GPP->pAuthenticationPref) = ~0;
    }
    if( pProfile3GPP->pIPv4AddrPref )
    {
        *(pProfile3GPP->pIPv4AddrPref) = ~0;
    }
    if( pProfile3GPP->pPcscfAddrUsingPCO )
    {
        *(pProfile3GPP->pPcscfAddrUsingPCO) = ~0;
    }
    if( pProfile3GPP->pPdpAccessConFlag )
    {
        *(pProfile3GPP->pPdpAccessConFlag) = ~0;
    }
    if( pProfile3GPP->pPcscfAddrUsingDhcp )
    {
        *(pProfile3GPP->pPcscfAddrUsingDhcp) = ~0;
    }
    if( pProfile3GPP->pImCnFlag )
    {
        *(pProfile3GPP->pImCnFlag) = ~0;
    }
    if( pProfile3GPP->pTFTID1Params )
    {
        struct TFTIDParams *pTFTIDPar = pProfile3GPP->pTFTID1Params;

        pTFTIDPar->filterId           = ~0;
        pTFTIDPar->eValid             = ~0;
        pTFTIDPar->ipVersion          = ~0;
        for( idx = 0; idx < 8; idx++ )
        {
            pTFTIDPar->pSourceIP[idx] = ~0;
        }
        pTFTIDPar->sourceIPMask       = ~0;
        pTFTIDPar->nextHeader         = ~0;
        pTFTIDPar->destPortRangeStart = ~0;
        pTFTIDPar->destPortRangeEnd   = ~0;
        pTFTIDPar->srcPortRangeStart  = ~0;
        pTFTIDPar->srcPortRangeEnd    = ~0;
        pTFTIDPar->IPSECSPI           = ~0;
        pTFTIDPar->tosMask            = ~0;
        pTFTIDPar->flowLabel          = ~0;
    }
    if( pProfile3GPP->pTFTID2Params )
    {
        struct TFTIDParams *pTFTIDPar = pProfile3GPP->pTFTID2Params;

        pTFTIDPar->filterId           = ~0;
        pTFTIDPar->eValid             = ~0;
        pTFTIDPar->ipVersion          = ~0;
        for( idx = 0; idx < 8; idx++ )
        {
            pTFTIDPar->pSourceIP[idx] = ~0;
        }
        pTFTIDPar->sourceIPMask       = ~0;
        pTFTIDPar->nextHeader         = ~0;
        pTFTIDPar->destPortRangeStart = ~0;
        pTFTIDPar->destPortRangeEnd   = ~0;
        pTFTIDPar->srcPortRangeStart  = ~0;
        pTFTIDPar->srcPortRangeEnd    = ~0;
        pTFTIDPar->IPSECSPI           = ~0;
        pTFTIDPar->tosMask            = ~0;
        pTFTIDPar->flowLabel          = ~0;
    }
    if( pProfile3GPP->pPdpContext )
    {
        *(pProfile3GPP->pPdpContext) = ~0;
    }
    if( pProfile3GPP->pSecondaryFlag )
    {
        *(pProfile3GPP->pSecondaryFlag) = ~0;
    }
    if( pProfile3GPP->pPrimaryID )
    {
        *(pProfile3GPP->pPrimaryID) = ~0;
    }
    if( pProfile3GPP->pIPv6AddPref )
    {
        for( idx = 0; idx < 8; idx++ )
        {
            pProfile3GPP->pIPv6AddPref[idx] = ~0;
        }
    }
    if( pProfile3GPP->pUMTSReqQoSSigInd )
    {
        struct UMTSReqQoSSigInd *pUMTSReqQoSSigInd =
            pProfile3GPP->pUMTSReqQoSSigInd;

        struct UMTSQoS *pUMTSQoS = &(pUMTSReqQoSSigInd->UMTSReqQoS);

        pUMTSQoS->trafficClass        = ~0;
        pUMTSQoS->maxUplinkBitrate    = ~0;
        pUMTSQoS->maxDownlinkBitrate  = ~0;
        pUMTSQoS->grntUplinkBitrate   = ~0;
        pUMTSQoS->grntDownlinkBitrate = ~0;
        pUMTSQoS->qosDeliveryOrder    = ~0;
        pUMTSQoS->maxSDUSize          = ~0;
        pUMTSQoS->sduErrorRatio       = ~0;
        pUMTSQoS->resBerRatio         = ~0;
        pUMTSQoS->deliveryErrSDU      = ~0;
        pUMTSQoS->transferDelay       = ~0;
        pUMTSQoS->trafficPriority     = ~0;
        pUMTSReqQoSSigInd->SigInd     = ~0;
    }
    if( pProfile3GPP->pUMTSMinQosSigInd )
    {
        struct UMTSReqQoSSigInd *pUMTSReqQoSSigInd =
            pProfile3GPP->pUMTSMinQosSigInd;

        struct UMTSQoS *pUMTSQoS =  &(pUMTSReqQoSSigInd->UMTSReqQoS);

        pUMTSQoS->trafficClass        = ~0;
        pUMTSQoS->maxUplinkBitrate    = ~0;
        pUMTSQoS->maxDownlinkBitrate  = ~0;
        pUMTSQoS->grntUplinkBitrate   = ~0;
        pUMTSQoS->grntDownlinkBitrate = ~0;
        pUMTSQoS->qosDeliveryOrder    = ~0;
        pUMTSQoS->maxSDUSize          = ~0;
        pUMTSQoS->sduErrorRatio       = ~0;
        pUMTSQoS->resBerRatio         = ~0;
        pUMTSQoS->deliveryErrSDU      = ~0;
        pUMTSQoS->transferDelay       = ~0;
        pUMTSQoS->trafficPriority     = ~0;
        pUMTSReqQoSSigInd->SigInd     = ~0;
    }
    if( pProfile3GPP->pPriDNSIPv6addpref )
    {
        for( idx = 0; idx < 8; idx++ )
        {
            pProfile3GPP->pPriDNSIPv6addpref[idx] = ~0;
        }
    }
    if( pProfile3GPP->pSecDNSIPv6addpref )
    {
        for( idx = 0; idx < 8; idx++ )
        {
            pProfile3GPP->pSecDNSIPv6addpref[idx] = ~0;
        }
    }
    if( pProfile3GPP->pAddrAllocPref )
    {
        *(pProfile3GPP->pAddrAllocPref) = ~0;
    }
    if( pProfile3GPP->pQosClassID )
    {
        struct QosClassID *pQosClsID = pProfile3GPP->pQosClassID;
        pQosClsID->QCI          = ~0;
        pQosClsID->gDlBitRate   = ~0;
        pQosClsID->maxDlBitRate = ~0;
        pQosClsID->gUlBitRate   = ~0;
        pQosClsID->maxUlBitRate = ~0;
    }
    if( pProfile3GPP->pAPNDisabledFlag )
    {
        *(pProfile3GPP->pAPNDisabledFlag) = ~0;
    }
    if( pProfile3GPP->pPDNInactivTimeout )
    {
        *(pProfile3GPP->pPDNInactivTimeout) = ~0;
    }
    if( pProfile3GPP->pAPNClass )
    {
        *(pProfile3GPP->pAPNClass) = ~0;
    }
}

local void initProf3GPP2( struct Profile3GPP2 *pProfile3GPP2 )
{
    BYTE idx = 0;
    if( pProfile3GPP2->pNegoDnsSrvrPref )
    {
        *(pProfile3GPP2->pNegoDnsSrvrPref) = ~0;
    }
    if( pProfile3GPP2->pPppSessCloseTimerDO )
    {
        *(pProfile3GPP2->pPppSessCloseTimerDO) = ~0;
    }
    if( pProfile3GPP2->pPppSessCloseTimer1x )
    {
        *(pProfile3GPP2->pPppSessCloseTimer1x) = ~0;
    }
    if( pProfile3GPP2->pAllowLinger )
    {
        *(pProfile3GPP2->pAllowLinger) = ~0;
    }
    if( pProfile3GPP2->pLcpAckTimeout )
    {
        *(pProfile3GPP2->pLcpAckTimeout) = ~0;
    }
    if( pProfile3GPP2->pIpcpAckTimeout )
    {
        *(pProfile3GPP2->pIpcpAckTimeout) = ~0;
    }
    if( pProfile3GPP2->pAuthTimeout )
    {
        *(pProfile3GPP2->pAuthTimeout) = ~0;
    }
    if( pProfile3GPP2->pLcpCreqRetryCount )
    {
        *(pProfile3GPP2->pLcpCreqRetryCount) = ~0;
    }
    if( pProfile3GPP2->pIpcpCreqRetryCount )
    {
        *(pProfile3GPP2->pIpcpCreqRetryCount) = ~0;
    }
    if( pProfile3GPP2->pAuthRetryCount )
    {
        *(pProfile3GPP2->pAuthRetryCount) = ~0;
    }
    if( pProfile3GPP2->pAuthProtocol )
    {
        *(pProfile3GPP2->pAuthProtocol) = ~0;
    }
    if( pProfile3GPP2->pUserId )
    {
        *(pProfile3GPP2->pUserId) = EOS;
    }
    if( pProfile3GPP2->pAuthPassword )
    {
        *(pProfile3GPP2->pAuthPassword) = EOS;
    }
    if( pProfile3GPP2->pDataRate )
    {
        *(pProfile3GPP2->pDataRate) = ~0;
    }
    if( pProfile3GPP2->pAppType )
    {
        *(pProfile3GPP2->pAppType) = ~0;
    }
    if( pProfile3GPP2->pDataMode )
    {
        *(pProfile3GPP2->pDataMode) = ~0;
    }
    if( pProfile3GPP2->pAppPriority )
    {
        *(pProfile3GPP2->pAppPriority) = ~0;
    }
    if( pProfile3GPP2->pApnString )
    {
        *(pProfile3GPP2->pApnString) = EOS;
    }
    if( pProfile3GPP2->pPdnType )
    {
        *(pProfile3GPP2->pPdnType) = ~0;
    }
    if( pProfile3GPP2->pIsPcscfAddressNedded )
    {
        *(pProfile3GPP2->pIsPcscfAddressNedded) = ~0;
    }
    if( pProfile3GPP2->pPrimaryV4DnsAddress )
    {
        *(pProfile3GPP2->pPrimaryV4DnsAddress) = ~0;
    }
    if( pProfile3GPP2->pSecondaryV4DnsAddress )
    {
        *(pProfile3GPP2->pSecondaryV4DnsAddress) = ~0;
    }
    if( pProfile3GPP2->pPriV6DnsAddress )
    {
        for( idx = 0; idx < 8; idx++ )
        {
            pProfile3GPP2->pPriV6DnsAddress[idx] = ~0;
        }
    }
    if( pProfile3GPP2->pSecV6DnsAddress )
    {
        for( idx = 0; idx < 8; idx++ )
        {
            pProfile3GPP2->pSecV6DnsAddress[idx] = ~0;
        }
    }
    if( pProfile3GPP2->pRATType )
    {
        *(pProfile3GPP2->pRATType) = ~0;
    }
    if( pProfile3GPP2->pAPNEnabled3GPP2 )
    {
        *(pProfile3GPP2->pAPNEnabled3GPP2) = ~0;
    }
    if( pProfile3GPP2->pPDNInactivTimeout3GPP2 )
    {
        *(pProfile3GPP2->pPDNInactivTimeout3GPP2) = ~0;
    }
    if( pProfile3GPP2->pAPNEnabled3GPP2 )
    {
        *(pProfile3GPP2->pAPNEnabled3GPP2) = ~0;
    }
}

ULONG SLQSGetProfileSettings(
    GetProfileSettingIn  *pProfileSettingIn,
    GetProfileSettingOut *pProfileSettingsOut )
{
    ULONG  resultcode;  /* result code */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT ParamLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiWdsSlqsGetProfileSettingsResp response;
    struct Profile3GPP                      *pProfile3GPP;
    struct Profile3GPP2                     *pProfile3GPP2;
    BYTE                                    profileType;

    /* Check against NULL for params */
    if ( !pProfileSettingIn ||
         !pProfileSettingsOut )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Store the profile type */
    profileType = pProfileSettingIn->ProfileType;

    /* pack QMI request */
    resultcode = PkQmiWdsSlqsGetProfileSettings( &ParamLength,
                                                 pOutParam,
                                                 pProfileSettingIn );

    if ( eQCWWAN_ERR_NONE != resultcode )
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* send QMI request */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WDS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &ParamLength );

    if ( eQCWWAN_ERR_NONE == resultcode )
    {
        /* initialize response structure */
        response.pProfileSettings = pProfileSettingsOut;

        /* Initialize all optional out params to default values */
        if( PROFILE_3GPP == profileType )
        {
            pProfile3GPP = &(pProfileSettingsOut->curProfile.SlqsProfile3GPP);
            initProf3GPP( pProfile3GPP );
        }
        else
        {
            pProfile3GPP2 = &(pProfileSettingsOut->curProfile.SlqsProfile3GPP2);
            initProf3GPP2( pProfile3GPP2 );
        }

        /* Copy to the caller's buffer */
        resultcode = UpkQmiWdsSlqsGetProfileSettings( pInParam,
                                                      &profileType,
                                                      &response );
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG SetMobileIPProfile(
    CHAR  *pSPC,
    BYTE  index,
    BYTE  *pEnabled,
    ULONG *pAddress,
    ULONG *pPrimaryHA,
    ULONG *pSecondaryHA,
    BYTE  *pRevTunneling,
    CHAR  *pNAI,
    ULONG *pHASPI,
    ULONG *pAAASPI,
    CHAR  *pMNHA,
    CHAR  *pMNAAA )
{
    ULONG  resultCode;  /* result code */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT ParamLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiWdsModifyMipProfileResp response;

    /* Check against NULL for mandatory parameter */
    if (!pSPC)
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Index points to first QMI header. */
    ParamLength = 0;

    /* pack QMI request */
    resultCode = PkQmiWdsModifyMipProfile( &ParamLength,
                                           pOutParam,
                                           pSPC,
                                           index,
                                           pEnabled,
                                           pAddress,
                                           pPrimaryHA,
                                           pSecondaryHA,
                                           pRevTunneling,
                                           pNAI,
                                           pHASPI,
                                           pAAASPI,
                                           pMNHA,
                                           pMNAAA );
    if (resultCode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultCode;
    }

    /* send QMI request */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WDS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S,
                                  &pInParam,
                                  &ParamLength );

    if (resultCode == eQCWWAN_ERR_NONE)
    {
        resultCode = UpkQmiWdsModifyMipProfile(pInParam, &response);
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSWdsSetEventReport( wdsSetEventReportReq *pSetEventReportReq )
{
    ULONG  resultcode;  /* result code */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT ParamLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiWdsSLQSSetEventReportResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pSetEventReportReq )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiWdsSLQSSetEventReport( &ParamLength,
                                             pOutParam,
                                             pSetEventReportReq );

    if ( eQCWWAN_ERR_NONE != resultcode )
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WDS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S,
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we get a positive return */
    if ( eQCWWAN_ERR_NONE == resultcode )
    {
        /* Copy to the caller's buffer */
        resultcode = UpkQmiWdsSLQSSetEventReport( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG SLQSWdsSwiPDPRuntimeSettings(
    swiPDPRuntimeSettingsReq  *pPDPRuntimeSettingsReq,
    swiPDPRuntimeSettingsResp *pPDPRuntimeSettingsResp )
{
    ULONG  resultCode;  /* result code */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT ParamLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiWdsSLQSSwiPDPRuntimeSettingsResp response;

    /* Check against NULL for mandatory parameter */
    if ( NULL == pPDPRuntimeSettingsReq || NULL == pPDPRuntimeSettingsResp )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* pack QMI request */
    resultCode = PkQmiWdsSLQSSwiPDPRuntimeSettings( &ParamLength,
                                                    pOutParam,
                                                    pPDPRuntimeSettingsReq );

    if (resultCode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultCode;
    }

    /* Send QMI request */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WDS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S,
                                  &pInParam,
                                  &ParamLength );

    if (resultCode == eQCWWAN_ERR_NONE)
    {
        /* Pass the pointer for the OUT parameters */
        response.pPDPRuntimeSettingsResp = pPDPRuntimeSettingsResp;

        /* Initializing the response */
        if ( pPDPRuntimeSettingsResp->pContextId )
        {
            *(pPDPRuntimeSettingsResp->pContextId) = 0xFF;
        }
        if ( pPDPRuntimeSettingsResp->pBearerId )
        {
            *(pPDPRuntimeSettingsResp->pBearerId) = 0xFF;
        }
        if ( pPDPRuntimeSettingsResp->pAPNName )
        {
            *(pPDPRuntimeSettingsResp->pAPNName) = EOS;
        }
        if ( pPDPRuntimeSettingsResp->pIPv4Address )
        {
            *(pPDPRuntimeSettingsResp->pIPv4Address) = 0xFFFFFFFF;
        }
        if ( pPDPRuntimeSettingsResp->pIPv4GWAddress )
        {
            *(pPDPRuntimeSettingsResp->pIPv4GWAddress) = 0xFFFFFFFF;
        }
        if ( pPDPRuntimeSettingsResp->pPrDNSIPv4Address )
        {
            *(pPDPRuntimeSettingsResp->pPrDNSIPv4Address) = 0xFFFFFFFF;
        }
        if ( pPDPRuntimeSettingsResp->pSeDNSIPv4Address )
        {
            *(pPDPRuntimeSettingsResp->pSeDNSIPv4Address) = 0xFFFFFFFF;
        }
        if ( pPDPRuntimeSettingsResp->pIPv6Address )
        {
            pPDPRuntimeSettingsResp->pIPv6Address->IPV6PrefixLen = 0xFF;
        }
        if ( pPDPRuntimeSettingsResp->pIPv6GWAddress )
        {
            pPDPRuntimeSettingsResp->pIPv6GWAddress->IPV6PrefixLen = 0xFF;
        }
        if ( pPDPRuntimeSettingsResp->pPrPCSCFIPv4Address )
        {
            *(pPDPRuntimeSettingsResp->pPrPCSCFIPv4Address) = 0xFFFFFFFF;
        }
        if ( pPDPRuntimeSettingsResp->pSePCSCFIPv4Address )
        {
            *(pPDPRuntimeSettingsResp->pSePCSCFIPv4Address) = 0xFFFFFFFF;
        }

        resultCode = UpkQmiWdsSLQSSwiPDPRuntimeSettings( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}
