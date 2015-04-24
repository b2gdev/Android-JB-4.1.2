/**
 * \ingroup rms
 *
 * \file    qaGobiApiRms.c
 *
 * \brief   Entry points for Gobi APIs for the Remote Management Service (RMS)
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "amudefs.h"
#include "qaQmiBasic.h"
#include "qaGobiApiRms.h"
#include "qaRmsGetEventSmsWake.h"
#include "qaRmsSetEventSmsWake.h"

ULONG GetSMSWake(
    ULONG *pEnabled,
    ULONG *pWakeMask )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiRmsGetEventSmsWakeResp response;

    /* pEnabled & pWakeMask are OUT parameters hence should not be NULL */
    if ( !pEnabled || !pWakeMask )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiRmsGetEventSmsWake( &paramLength,
                                          pOutParam );

    /* No memory */
    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode;
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_RMS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Set the pointers for function OUT parameters */
        response.pEnabled  = pEnabled;
        response.pWakeMask = pWakeMask;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiRmsGetEventSmsWake( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SetSMSWake(
    ULONG bEnable,
    ULONG wakeMask )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiRmsSetEventSmsWakeResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiRmsSetEventSmsWake( &paramLength,
                                          pOutParam,
                                          bEnable,
                                          wakeMask );

    /* No memory */
    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode;
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_RMS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiRmsSetEventSmsWake( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}
