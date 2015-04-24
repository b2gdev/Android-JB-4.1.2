/*
 * \ingroup sar
 *
 * \file    qaGobiApiSar.c
 *
 * \brief   Entry points for Gobi APIs for the Specific Absorption Rate
 *          API (SAR)
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "amudefs.h"
#include "qaQmiBasic.h"
#include "qaGobiApiSar.h"
#include "qaSarSLQSGetRFSarState.h"
#include "qaSarSLQSSetRFSarState.h"

ULONG SLQSGetRfSarState(
    ULONG *pRfSarState )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiSarSLQSRFGetStateResp response;

    /* Parameter is OUT hence checked against NULL */
    if ( !pRfSarState )
    {
       return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiSarSLQSRFGetState( &paramLength,
                                         pOutParam );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_SAR,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        response.pRfSarState = pRfSarState;
        /* Copy to the caller's buffer */
        resultCode = UpkQmiSarSLQSRFGetState( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSSetRfSarState(
    ULONG RfSarState )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiSarSLQSSetRFStateResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiSarSLQSSetRFState( &paramLength,
                                         pOutParam,
                                         RfSarState );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_SAR,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiSarSLQSSetRFState( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}
