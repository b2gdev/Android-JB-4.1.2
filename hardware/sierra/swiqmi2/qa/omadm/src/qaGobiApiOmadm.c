/*
 * \ingroup oma
 *
 * \file    qaGobiApiOmadm.c
 *
 * \brief   Entry points for Gobi APIs for the Open Mobile Alliance Device
 *          Management Service (OMADMS)
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "amudefs.h"
#include "qaQmiBasic.h"
#include "qaGobiApiOmadm.h"
#include "qaOmaDmCancelSession.h"
#include "qaOmaDmStartSession.h"
#include "qaOmaDmGetSessionInfo.h"

ULONG OMADMStartSession(
    ULONG sessionType )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiOmaDmStartSessionResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiOmaDmStartSession( &paramLength,
                                         pOutParam,
                                         sessionType );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_OMA,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiOmaDmStartSession( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG OMADMCancelSession()
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiOmaDmCancelSessionResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiOmaDmCancelSession( &paramLength,
                                          pOutParam );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_OMA,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiOmaDmCancelSession( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG OMADMGetSessionInfo(
    ULONG *pSessionState,
    ULONG *pSessionType,
    ULONG *pFailureReason,
    BYTE  *pRetryCount,
    WORD  *pSessionPause,
    WORD  *pTimeRemaining )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiOmaDmGetSessionInfoResp response;

    /*
     * OUT parameters should not be NULL
     */
    if ( !pSessionState ||
         !pSessionType||
         !pFailureReason||
         !pRetryCount ||
         !pSessionPause||
         !pTimeRemaining)
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message and sends the message. */
    resultCode = PkQmiOmaDmGetSessionInfo( &paramLength,
                                           pOutParam );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_OMA,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy the obtained values to the function OUT parameters */
        response.pSessionState  = pSessionState;
        response.pSessionType   = pSessionType;
        response.pFailureReason = pFailureReason;
        response.pRetryCount    = pRetryCount;
        response.pSessionPause  = pSessionPause;
        response.pTimeRemaining = pTimeRemaining;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiOmaDmGetSessionInfo( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG OMADMGetPendingNIA(
    ULONG  *pSessionType,
    USHORT *pSessionID )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiOmaDmGetSessionInfoResp response;

    /* OUT parameters should not be NULL */
    if ( !pSessionType ||
         !pSessionID )
    {
       return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message and sends the message. */
    resultCode = PkQmiOmaDmGetSessionInfo( &paramLength,
                                           pOutParam );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_OMA,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
   if( eQCWWAN_ERR_NONE == resultCode )
   {
       /* Copy the obtained values to the function OUT parameters */
       response.pSessionState   = NULL;
       response.pSessionType    = NULL;
       response.pFailureReason  = NULL;
       response.pRetryCount     = NULL;
       response.pSessionPause   = NULL;
       response.pTimeRemaining  = NULL;
       response.pNIASessionType = pSessionType;
       response.pNIASessionID   = pSessionID;

       /* Copy to the caller's buffer */
       resultCode = UpkQmiOmaDmGetSessionInfo( pInParam, &response );
   }

   qmrelreqbkp();
   return resultCode;
}
