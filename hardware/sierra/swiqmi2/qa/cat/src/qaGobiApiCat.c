/**
 * \ingroup cat
 *
 * \file    qaGobiApiCat.c
 *
 * \brief   Entry points for Gobi APIs for the Card Application Toolkit (CAT)
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "amudefs.h"
#include "qaQmiBasic.h"
#include "qaGobiApiCat.h"
#include "qaCatSendEnvelopeCmd.h"
#include "qaCatSendTerminalResponse.h"

ULONG CATSendEnvelopeCommand(
    ULONG cmdID,
    ULONG dataLen,
    BYTE  *pData )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiCatSendEnvelopeCmdResp response;


    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiCatSendEnvelopeCmd( &paramLength,
                                          pOutParam,
                                          cmdID,
                                          dataLen,
                                          pData );
    /* No memory */
    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode;
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_CAT,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiCatSendEnvelopeCmd( pInParam, &response );
    }
    qmrelreqbkp();
    return resultCode;
}

ULONG CATSendTerminalResponse(
    ULONG refID,
    ULONG dataLen,
    BYTE  *pData )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiCatSendTerminalResponseResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiCatSendTerminalResponse( &paramLength,
                                               pOutParam,
                                               refID,
                                               dataLen,
                                               pData );
    /* No memory */
    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode;
    }
    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_CAT,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );


    /* Only parse out the response data if we got a positive return */
    /* Copy to the caller's buffer */
    if ( resultCode == eQCWWAN_ERR_NONE )
        resultCode = UpkQmiCatSendTerminalResponse( pInParam, &response );
    qmrelreqbkp();
    return resultCode;
}
