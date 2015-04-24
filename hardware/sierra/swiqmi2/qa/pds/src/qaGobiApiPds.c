/**
 * \ingroup pds
 *
 * \file    qaGobiApiPds.c
 *
 * \brief   Entry points for Gobi APIs for the Position
 *          Determination Service (PDS)
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "sludefs.h"
#include "amudefs.h"
#include "qaQmiBasic.h"
#include "qaGobiApiPds.h"
#include "qaPdsGetGPSServiceState.h"
#include "qaPdsSetGPSServiceState.h"
#include "qaPdsStartTrackingSession.h"
#include "qaPdsEndTrackingSession.h"
#include "qaPdsGetDefaultTrackingSession.h"
#include "qaPdsSetDefaultTrackingSession.h"
#include "qaPdsSetXtraParameters.h"
#include "qaPdsGetAGPSConfig.h"
#include "qaPdsSetAGPSConfig.h"
#include "qaPdsGetAutoTrackingState.h"
#include "qaPdsSetAutoTrackingState.h"
#include "qaPdsGetComPortConfig.h"
#include "qaPdsSetComPortConfig.h"
#include "qaPdsForceXtraDownload.h"
#include "qaPdsGetXtraParameters.h"
#include "qaPdsResetGPSServicedata.h"
#include "qaPdsInjectRelativeTimeRef.h"
#include "qaPdsInjectAbsoluteTimeRef.h"
#include "qaPdsInjectPositionData.h"

ULONG GetPDSState(
    ULONG *pEnabledStatus,
    ULONG *pTrackingStatus )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiPdsGetServiceStateResp response;

    /* pEnabledStatus and pTrackingStatus are OUT parameter and
     * hence should not be NULL
     */
    if ( !pEnabledStatus ||
         !pTrackingStatus )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message and sends the message. */
    resultCode = PkQmiPdsGetServiceState( &paramLength, pOutParam );

    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode; /* No memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_PDS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Copy the obtained values to the function OUT parameters */
        response.pEnabledStatus  = pEnabledStatus;
        response.pTrackingStatus = pTrackingStatus;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiPdsGetServiceState( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SetPDSState(
    ULONG enable )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiPdsSetServiceStateResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiPdsSetServiceState( &paramLength,
                                          pOutParam,
                                          enable );
    /* no memory error */
    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode;
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_PDS,
                                  paramLength,
                                  eQMI_TIMEOUT_5_S, /* 5 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiPdsSetServiceState( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG StartPDSTrackingSessionExt(
    BYTE  sessionControl,
    BYTE  sessionType,
    BYTE  sessionOperation,
    BYTE  sessionServerOption,
    BYTE  fixTimeout,
    ULONG fixInterval,
    ULONG fixCount,
    ULONG fixAccuracy )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiPdsStartTrackingSessionResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiPdsStartTrackingSession( &paramLength,
                                               pOutParam,
                                               sessionControl,
                                               sessionType,
                                               sessionOperation,
                                               sessionServerOption,
                                               fixTimeout,
                                               fixInterval,
                                               fixCount,
                                               fixAccuracy );

    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode; /* No memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_PDS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiPdsStartTrackingSession( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG StopPDSTrackingSession()
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiPdsEndTrackingSessionResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message and sends the message. */
    resultCode = PkQmiPdsEndTrackingSession( &paramLength, pOutParam );

    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode; /* No memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_PDS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiPdsEndTrackingSession( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG GetServiceAutomaticTracking(
    ULONG *pbAuto )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiPdsGetAutoTrackingStateResp response;

    /* pbAuto is an OUT parameter and hence should not be NULL */
    if ( !pbAuto )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message and sends the message. */
    resultCode = PkQmiPdsGetAutoTrackingState( &paramLength, pOutParam );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_PDS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy the obtained values to the function OUT parameters */
        response.pbAuto = pbAuto;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiPdsGetAutoTrackingState( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SetServiceAutomaticTracking(
    ULONG bAuto )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiPdsSetAutoTrackingStateResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiPdsSetAutoTrackingState( &paramLength,
                                               pOutParam,
                                               bAuto );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_PDS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiPdsSetAutoTrackingState( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG GetPortAutomaticTracking(
    ULONG *pbAuto )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiPdsGetComPortConfigResp response;

    /* pbAuto is an OUT parameter and hence should not be NULL */
    if ( !pbAuto )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message and sends the message. */
    resultCode = PkQmiPdsGetComPortConfig( &paramLength, pOutParam );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_PDS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy the obtained values to the function OUT parameters */
        response.pbAuto = pbAuto;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiPdsGetComPortConfig( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SetPortAutomaticTracking(
    ULONG bAuto )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiPdsSetComPortConfigResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiPdsSetComPortConfig( &paramLength,
                                           pOutParam,
                                           bAuto );
    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_PDS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiPdsSetComPortConfig( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG PDSInjectTimeReference(
    ULONGLONG systemTime,
    USHORT    systemDiscontinuities )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiPdsInjectRelativeTimeRefResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiPdsInjectRelativeTimeRef( &paramLength,
                                                pOutParam,
                                                systemTime,
                                                systemDiscontinuities );
    /* No memory */
    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode;
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_PDS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiPdsInjectRelativeTimeRef( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG GetPDSDefaults(
    ULONG *pOperation,
    BYTE  *pTimeout,
    ULONG *pInterval,
    ULONG *pAccuracy )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiPdsGetDefaultTrackingSessionResp response;

    /* pOperation, pTimeout, pInterval and pAccuracy are OUT parameters and
     * hence should not be NULL
     */
    if ( !pOperation ||
         !pTimeout   ||
         !pInterval  ||
         !pAccuracy )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message and sends the message. */
    resultCode = PkQmiPdsGetDefaultTrackingSession( &paramLength, pOutParam );

    /* No Memory */
    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode;
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_PDS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Copy the obtained values to the function OUT parameters */
        response.pOperation = pOperation;
        response.pTimeout   = pTimeout;
        response.pInterval  = pInterval;
        response.pAccuracy  = pAccuracy;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiPdsGetDefaultTrackingSession( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SetPDSDefaults(
    ULONG operation,
    BYTE  timeout,
    ULONG interval,
    ULONG accuracy )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiPdsSetDefaultTrackingSessionResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiPdsSetDefaultTrackingSession( &paramLength,
                                                    pOutParam,
                                                    operation,
                                                    timeout,
                                                    interval,
                                                    accuracy );
    /* No Memory */
    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode;
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_PDS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiPdsSetDefaultTrackingSession( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG GetXTRAAutomaticDownload(
    ULONG  *pbEnabled,
    USHORT *pInterval )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiPdsGetXtraParametersResp response;

    /* pbEnabled and pInterval are OUT parameters and
     * hence should not be NULL
     */
    if ( !pbEnabled ||
         !pInterval )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message and sends the message. */
    resultCode = PkQmiPdsGetXtraParameters( &paramLength, pOutParam );

    /* No Memory */
    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode;
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_PDS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Copy the obtained values to the function OUT parameters */
        response.pbEnabled      = pbEnabled;
        response.pInterval      = pInterval;
        response.pPreference    = NULL;
        response.pGPSWeek       = NULL;
        response.pGPSWeekOffset = NULL;
        response.pDuration      = NULL;
        /* Copy to the caller's buffer */
        resultCode = UpkQmiPdsGetXtraParameters( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SetXTRAAutomaticDownload(
    ULONG  bEnabled,
    USHORT interval )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiPdsSetXtraParametersResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiPdsSetXtraParameters( &paramLength,
                                            pOutParam,
                                            &bEnabled,
                                            &interval,
                                            NULL );
    /* No Memory */
    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode;
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_PDS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiPdsSetXtraParameters( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG GetXTRANetwork(
    ULONG *pPreference )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiPdsGetXtraParametersResp response;

    /* pPreference is OUT parameter and
     * hence should not be NULL
     */
    if ( !pPreference )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message and sends the message. */
    resultCode = PkQmiPdsGetXtraParameters( &paramLength, pOutParam );

    /* No Memory */
    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode;
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_PDS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Copy the obtained values to the function OUT parameters */
        response.pbEnabled      = NULL;
        response.pInterval      = NULL;
        response.pPreference    = pPreference;
        response.pGPSWeek       = NULL;
        response.pGPSWeekOffset = NULL;
        response.pDuration      = NULL;
        /* Copy to the caller's buffer */
        resultCode = UpkQmiPdsGetXtraParameters( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SetXTRANetwork(
    ULONG preference )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiPdsSetXtraParametersResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiPdsSetXtraParameters( &paramLength,
                                            pOutParam,
                                            NULL,
                                            NULL,
                                            &preference );
    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_PDS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiPdsSetXtraParameters( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG GetXTRAValidity(
    USHORT *pGPSWeek,
    USHORT *pGPSWeekOffset,
    USHORT *pDuration )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiPdsGetXtraParametersResp response;

    /* pGPSWeek,pGPSWeekOffset,pDuration and are OUT parameters and
     * hence should not be NULL
     */
    if ( !pGPSWeek       ||
         !pGPSWeekOffset ||
         !pDuration )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message and sends the message. */
    resultCode = PkQmiPdsGetXtraParameters( &paramLength, pOutParam );

    /* No memory */
    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode;
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_PDS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy the obtained values to the function OUT parameters */
        response.pbEnabled      = NULL;
        response.pInterval      = NULL;
        response.pPreference    = NULL;
        response.pGPSWeek       = pGPSWeek;
        response.pGPSWeekOffset = pGPSWeekOffset;
        response.pDuration      = pDuration;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiPdsGetXtraParameters( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG ForceXTRADownload()
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiPdsForceXtraDownloadResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiPdsForceXtraDownload( &paramLength,
                                            pOutParam );
    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode; /* no memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_PDS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiPdsForceXtraDownload( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG ResetPDSData(
    ULONG *pGPSDataMask,
    ULONG *pCellDataMask )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiPdsResetGPSServicedataResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiPdsResetGPSServicedata( &paramLength,
                                              pOutParam,
                                              pGPSDataMask,
                                              pCellDataMask );
    /* No memory */
    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode;
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_PDS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiPdsResetGPSServicedata( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSSetAGPSConfig(
    ULONG *pServerAddress,
    ULONG *pServerPort,
    BYTE  *pServerURL,
    BYTE  *pServerURLLength )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiPdsSetAGPSConfigResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiPdsSetAGPSConfig( &paramLength,
                                        pOutParam,
                                        pServerAddress,
                                        pServerPort,
                                        pServerURL,
                                        pServerURLLength );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_PDS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiPdsSetAGPSConfig( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSPDSInjectAbsoluteTimeReference(
    ULONGLONG timeMsec,
    ULONG     timeUncMsec,
    BYTE      timeBase,
    BYTE      forceFlag )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiPdsInjectAbsoluteTimeRefResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiPdsInjectAbsoluteTimeRef( &paramLength,
                                                pOutParam,
                                                timeMsec,
                                                timeUncMsec,
                                                timeBase,
                                                forceFlag );
    /* No memory */
    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode;
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_PDS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiPdsInjectAbsoluteTimeRef( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSGetAGPSConfig(
    ULONG *pServerAddress,
    ULONG *pServerPort,
    BYTE  *pServerURL,
    BYTE  *pServerURLLength )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiPdsGetAGPSConfigResp response;

    /* check for out parameter validation */
    if ( !pServerAddress ||
         !pServerPort    ||
         !pServerURL     ||
         !pServerURLLength )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiPdsGetAGPSConfig( &paramLength,
                                        pOutParam );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_PDS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        response.pServerAddress   = pServerAddress;
        response.pServerPort      = pServerPort;
        response.pServerURL       = pServerURL;
        response.pServerURLLength = pServerURLLength;

        /* initialize the result values */
        slmemset( (CHAR *)response.pServerURL, 0, *pServerURLLength );
        *(response.pServerAddress)   = 0;
        *(response.pServerPort)      = 0;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiPdsGetAGPSConfig( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSPDSInjectPositionData( struct PDSPositionData *pPositionData )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiPdsInjectPositionDataResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiPdsInjectPositionData( &paramLength,
                                             pOutParam,
                                             pPositionData );
    /* No memory */
    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode;
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_PDS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiPdsInjectPositionData( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}
