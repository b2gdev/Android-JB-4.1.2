/*
 * \ingroup voice
 *
 * \file    qaGobiApiVoice.c
 *
 * \brief   Entry points for Gobi APIs for the Voice Service
 *
 * Copyright: © 2011-12 Sierra Wireless, Inc. all rights reserved
 *
 */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "amudefs.h"
#include "qaQmiBasic.h"
#include "qaVoiceOriginateUSSD.h"
#include "qaVoiceAnswerUSSD.h"
#include "qaVoiceCancelUSSD.h"
#include "qaVoiceSLQSVoiceDialCall.h"
#include "qaVoiceSLQSVoiceEndCall.h"
#include "qaVoiceSLQSVoiceSetSUPSService.h"
#include "qaVoiceSLQSVoiceSetConfig.h"
#include "qaVoiceSLQSVoiceAnswerCall.h"
#include "qaVoiceSLQSVoiceGetCLIR.h"
#include "qaVoiceSLQSVoiceGetCLIP.h"
#include "qaVoiceSLQSVoiceGetCallWaiting.h"
#include "qaVoiceSLQSVoiceGetCallBarring.h"
#include "qaVoiceSLQSVoiceGetCallForwardingStatus.h"
#include "qaVoiceSLQSVoiceSetCallBarringPassword.h"
#include "qaVoiceSLQSVoiceGetCallInfo.h"
#include "qaVoiceSLQSVoiceGetAllCallInfo.h"
#include "qaVoiceSLQSVoiceManageCalls.h"
#include "qaVoiceSLQSVoiceBurstDTMF.h"
#include "qaVoiceSLQSVoiceStartContDTMF.h"
#include "qaVoiceSLQSVoiceStopContDTMF.h"
#include "qaVoiceSLQSVoiceSendFlash.h"
#include "qaVoiceSLQSVoiceSetPreferredPrivacy.h"
#include "qaVoiceSLQSVoiceIndicationRegister.h"
#include "qaVoiceSLQSVoiceGetConfig.h"
#include "qaVoiceSLQSVoiceOrigUSSDNoWait.h"
#include "qaVoiceSLQSVoiceBindSubscription.h"
#include "qaVoiceSLQSVoiceALSSetLineSwitching.h"
#include "qaVoiceSLQSVoiceALSSelectLine.h"
#include "qaVoiceSLQSVoiceGetCOLP.h"
#include "qaVoiceSLQSVoiceGetCOLR.h"
#include "qaVoiceSLQSVoiceGetCNAP.h"

ULONG OriginateUSSD(
    BYTE *pInfo )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiVoiceOriginateUSSDResp response;

    /* Checking for Invalid Parameter */
    if ( !pInfo  )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiVoiceOriginateUSSD( &paramLength,
                                          pOutParam,
                                          (struct USSInfo *)pInfo );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_VOICE,
                                  paramLength,
                                  eQMI_TIMEOUT_300_S, /* 5 minutes */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiVoiceOriginateUSSD( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG AnswerUSSD(
    BYTE *pInfo )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiVoiceAnswerUSSDResp response;

    /* Checking for Invalid Parameter */
    if ( !pInfo  )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiVoiceAnswerUSSD( &paramLength,
                                       pOutParam,
                                       (struct USSInfo *)pInfo );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_VOICE,
                                  paramLength,
                                  eQMI_TIMEOUT_300_S, /* 5 minutes */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiVoiceAnswerUSSD( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG CancelUSSD()
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiVoiceCancelUSSDResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiVoiceCancelUSSD( &paramLength,
                                       pOutParam );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_VOICE,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiVoiceCancelUSSD( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSVoiceDialCall( voiceCallRequestParams  *pCallRequestParams,
                         voiceCallResponseParams *pCallResponseParams )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiVoiceSLQSDialCallResp response;

    /* Checking for Invalid Parameter */
    if ( !pCallRequestParams || !pCallResponseParams )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiVoiceSlqsDialCall( &paramLength,
                                         pOutParam,
                                         pCallRequestParams );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_VOICE,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Set the pointers for the function OUT parameters */
        response.pCallResponseParams =  pCallResponseParams;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiVoiceSlqsDialCall( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSVoiceEndCall( BYTE *pCallId )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiVoiceSLQSEndCallResp response;

    /* Checking for Invalid Parameter */
    if ( !pCallId  )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiVoiceSlqsEndCall( &paramLength,
                                        pOutParam,
                                        pCallId );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_VOICE,
                                  paramLength,
                                  eQMI_TIMEOUT_10_S, /* 10 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Pass the pointer for the OUT parameters */
        response.pCallId = pCallId;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiVoiceSlqsEndCall( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSVoiceSetSUPSService (
    voiceSetSUPSServiceReq  *pVoiceSetSUPSServiceReq,
    voiceSetSUPSServiceResp *pVoiceSetSUPSServiceResp)
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiVoiceSLQSSetSUPSServiceResp response;

    /* Checking for Invalid Parameter */
    if ( !pVoiceSetSUPSServiceReq || !pVoiceSetSUPSServiceResp )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiVoiceSlqsSetSUPSService( &paramLength,
                                               pOutParam,
                                               pVoiceSetSUPSServiceReq );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_VOICE,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Set the pointers for the function OUT parameters */
        response.pVoiceSetSUPSServiceResp =  pVoiceSetSUPSServiceResp;

        /* Initialize all the response parameters to default values */
        if (pVoiceSetSUPSServiceResp->pCCSUPSType)
        {
            pVoiceSetSUPSServiceResp->pCCSUPSType->svcType = 0xFF;
            pVoiceSetSUPSServiceResp->pCCSUPSType->reason  = 0xFF;
        }
        if (pVoiceSetSUPSServiceResp->pAlphaIDInfo)
        {
            pVoiceSetSUPSServiceResp->pAlphaIDInfo->alphaDcs = 0xFF;
            pVoiceSetSUPSServiceResp->pAlphaIDInfo->alphaLen = 0x00;
        }
        if (pVoiceSetSUPSServiceResp->pFailCause)
        {
            *(pVoiceSetSUPSServiceResp->pFailCause) = 0xFFFF;
        }
        if (pVoiceSetSUPSServiceResp->pCCResultType)
        {
            *(pVoiceSetSUPSServiceResp->pCCResultType) = 0xFF;
        }
        if (pVoiceSetSUPSServiceResp->pCallID)
        {
            *(pVoiceSetSUPSServiceResp->pCallID) = 0x00;
        }

        /* Copy to the caller's buffer */
        resultCode = UpkQmiVoiceSlqsSetSUPSService( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSVoiceSetConfig (
        voiceSetConfigReq  *pVoiceSetConfigReq,
        voiceSetConfigResp *pVoiceSetConfigResp )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiVoiceSLQSSetConfigResp response;

    /* Checking for Invalid Parameter */
    if ( !pVoiceSetConfigReq ||
         !pVoiceSetConfigResp )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiVoiceSlqsSetConfig( &paramLength,
                                          pOutParam,
                                          pVoiceSetConfigReq );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_VOICE,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Pass the pointer for the OUT parameters */
        response.pVoiceSetConfigResp = pVoiceSetConfigResp;

        /* Initializing optional parameters */
        if ( NULL != pVoiceSetConfigResp->pAutoAnsStatus )
        {
            *(pVoiceSetConfigResp->pAutoAnsStatus) = 0xFF;
        }
        if ( NULL != pVoiceSetConfigResp->pAirTimerStatus )
        {
            *(pVoiceSetConfigResp->pAirTimerStatus) = 0xFF;
        }
        if ( NULL != pVoiceSetConfigResp->pRoamTimerStatus )
        {
            *(pVoiceSetConfigResp->pRoamTimerStatus) = 0xFF;
        }
        if ( NULL != pVoiceSetConfigResp->pTTYConfigStatus )
        {
            *(pVoiceSetConfigResp->pTTYConfigStatus) = 0xFF;
        }
        if ( NULL != pVoiceSetConfigResp->pPrefVoiceSOStatus )
        {
            *(pVoiceSetConfigResp->pPrefVoiceSOStatus) = 0xFF;
        }
        if ( NULL != pVoiceSetConfigResp->pVoiceDomainPrefStatus )
        {
            *(pVoiceSetConfigResp->pVoiceDomainPrefStatus) = 0xFF;
        }

        /* Copy to the caller's buffer */
        resultCode = UpkQmiVoiceSlqsSetConfig( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSVoiceAnswerCall(
    voiceAnswerCall *pVoiceAnswerCall )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiVoiceSLQSAnswerCallResp response;

    /* Checking for Invalid Parameter */
    if ( !pVoiceAnswerCall->pCallId  )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiVoiceSlqsAnswerCall( &paramLength,
                                           pOutParam,
                                           pVoiceAnswerCall );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_VOICE,
                                  paramLength,
                                  eQMI_TIMEOUT_10_S, /* 10 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Set the pointers for the function OUT parameters */
        response.pCallId = pVoiceAnswerCall->pCallId;
        
        if ( NULL != pVoiceAnswerCall->pCallId )
        {
             /* Invalid CallId assigned to response */
             *(response.pCallId) = 0;
        }
        /* Copy to the caller's buffer */
        resultCode = UpkQmiVoiceSlqsAnswerCall( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSVoiceGetCLIR( voiceGetCLIRResp  *pVoiceGetCLIRResp )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiVoiceSLQSGetCLIRResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pVoiceGetCLIRResp  )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiVoiceSlqsGetCLIR( &paramLength,
                                        pOutParam );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_VOICE,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Pass the pointer for the OUT parameters */
        response.pVoiceGetCLIRResp = pVoiceGetCLIRResp;

        /* Initializing the response */
        if ( NULL != pVoiceGetCLIRResp->pCLIRResp )
        {
            pVoiceGetCLIRResp->pCLIRResp->ActiveStatus    = 0xFF;
            pVoiceGetCLIRResp->pCLIRResp->ProvisionStatus = 0xFF;
        }
        if ( NULL != pVoiceGetCLIRResp->pFailCause )
        {
            *(pVoiceGetCLIRResp->pFailCause) = 0xFFFF;
        }
        if ( NULL != pVoiceGetCLIRResp->pAlphaIDInfo )
        {
            pVoiceGetCLIRResp->pAlphaIDInfo->alphaDcs = 0xFF;
            pVoiceGetCLIRResp->pAlphaIDInfo->alphaLen = 0;
        }
        if ( NULL != pVoiceGetCLIRResp->pCCResType )
        {
            *(pVoiceGetCLIRResp->pCCResType) = 0xFF;
        }
        if ( NULL != pVoiceGetCLIRResp->pCallID )
        {
            *(pVoiceGetCLIRResp->pCallID) = 0;
        }
        if ( NULL != pVoiceGetCLIRResp->pCCSUPSType )
        {
            pVoiceGetCLIRResp->pCCSUPSType->svcType = 0xFF;
            pVoiceGetCLIRResp->pCCSUPSType->reason  = 0xFF;
        }

        /* Copy to the caller's buffer */
        resultCode = UpkQmiVoiceSlqsGetCLIR( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSVoiceGetCLIP( voiceGetCLIPResp  *pVoiceGetCLIPResp )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiVoiceSLQSGetCLIPResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pVoiceGetCLIPResp  )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiVoiceSlqsGetCLIP( &paramLength,
                                        pOutParam );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_VOICE,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Pass the pointer for the OUT parameters */
        response.pVoiceGetCLIPResp = pVoiceGetCLIPResp;

        /* Initializing the response */
        if ( NULL != pVoiceGetCLIPResp->pCLIPResp )
        {
            pVoiceGetCLIPResp->pCLIPResp->ActiveStatus    = 0xFF;
            pVoiceGetCLIPResp->pCLIPResp->ProvisionStatus = 0xFF;
        }
        if ( NULL != pVoiceGetCLIPResp->pFailCause )
        {
            *(pVoiceGetCLIPResp->pFailCause) = 0xFFFF;
        }
        if ( NULL != pVoiceGetCLIPResp->pAlphaIDInfo )
        {
            pVoiceGetCLIPResp->pAlphaIDInfo->alphaDcs = 0xFF;
            pVoiceGetCLIPResp->pAlphaIDInfo->alphaLen = 0;
        }
        if ( NULL != pVoiceGetCLIPResp->pCCResType )
        {
            *(pVoiceGetCLIPResp->pCCResType) = 0xFF;
        }
        if ( NULL != pVoiceGetCLIPResp->pCallID )
        {
            *(pVoiceGetCLIPResp->pCallID) = 0;
        }
        if ( NULL != pVoiceGetCLIPResp->pCCSUPSType )
        {
            pVoiceGetCLIPResp->pCCSUPSType->svcType = 0xFF;
            pVoiceGetCLIPResp->pCCSUPSType->reason  = 0xFF;
        }

        /* Copy to the caller's buffer */
        resultCode = UpkQmiVoiceSlqsGetCLIP( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSVoiceGetCallWaiting( voiceGetCallWaitInfo *pVoiceGetCallWaitInfo )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiVoiceSLQSGetCallWaitingResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pVoiceGetCallWaitInfo  )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiVoiceSlqsGetCallWaiting( &paramLength,
                                               pOutParam,
                                            pVoiceGetCallWaitInfo->pSvcClass );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_VOICE,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Pass the pointer for the OUT parameters */
        response.pVoiceGetCallWaitInfo = pVoiceGetCallWaitInfo;

        /* Initializing the response */
        if ( NULL != pVoiceGetCallWaitInfo->pSvcClass )
        {
            *(pVoiceGetCallWaitInfo->pSvcClass) = 0xFF;
        }
        if ( NULL != pVoiceGetCallWaitInfo->pFailCause )
        {
            *(pVoiceGetCallWaitInfo->pFailCause) = 0xFFFF;
        }
        if ( NULL != pVoiceGetCallWaitInfo->pAlphaIDInfo )
        {
            pVoiceGetCallWaitInfo->pAlphaIDInfo->alphaDcs = 0xFF;
            pVoiceGetCallWaitInfo->pAlphaIDInfo->alphaLen = 0;
        }
        if ( NULL != pVoiceGetCallWaitInfo->pCCResType )
        {
            *(pVoiceGetCallWaitInfo->pCCResType) = 0xFF;
        }
        if ( NULL != pVoiceGetCallWaitInfo->pCallID )
        {
            *(pVoiceGetCallWaitInfo->pCallID) = 0;
        }
        if ( NULL != pVoiceGetCallWaitInfo->pCCSUPSType )
        {
            pVoiceGetCallWaitInfo->pCCSUPSType->svcType = 0xFF;
            pVoiceGetCallWaitInfo->pCCSUPSType->reason  = 0xFF;
        }

        /* Copy to the caller's buffer */
        resultCode = UpkQmiVoiceSlqsGetCallWaiting( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSVoiceGetCallBarring( voiceGetCallBarringReq  *pVoiceGetCallBarringReq,
                               voiceGetCallBarringResp *pVoiceGetCallBarringResp )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiVoiceSLQSGetCallBarringResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pVoiceGetCallBarringReq ||
         NULL == pVoiceGetCallBarringResp )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiVoiceSlqsGetCallBarring( &paramLength,
                                               pOutParam,
                                               pVoiceGetCallBarringReq );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_VOICE,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Pass the pointer for the OUT parameters */
        response.pVoiceGetCallBarringResp = pVoiceGetCallBarringResp;

        /* Initializing the response */
        if ( NULL != pVoiceGetCallBarringResp->pSvcClass )
        {
            *(pVoiceGetCallBarringResp->pSvcClass) = 0xFF;
        }
        if ( NULL != pVoiceGetCallBarringResp->pFailCause )
        {
            *(pVoiceGetCallBarringResp->pFailCause) = 0xFFFF;
        }
        if ( NULL != pVoiceGetCallBarringResp->pAlphaIDInfo )
        {
            pVoiceGetCallBarringResp->pAlphaIDInfo->alphaDcs = 0xFF;
            pVoiceGetCallBarringResp->pAlphaIDInfo->alphaLen = 0;
        }
        if ( NULL != pVoiceGetCallBarringResp->pCCResType )
        {
            *(pVoiceGetCallBarringResp->pCCResType) = 0xFF;
        }
        if ( NULL != pVoiceGetCallBarringResp->pCallID )
        {
            *(pVoiceGetCallBarringResp->pCallID) = 0;
        }
        if ( NULL != pVoiceGetCallBarringResp->pCCSUPSType )
        {
            pVoiceGetCallBarringResp->pCCSUPSType->svcType = 0xFF;
            pVoiceGetCallBarringResp->pCCSUPSType->reason  = 0xFF;
        }

        /* Copy to the caller's buffer */
        resultCode = UpkQmiVoiceSlqsGetCallBarring( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSVoiceGetCallForwardingStatus( voiceGetCallFWReq  *pVoiceGetCallFWReq,
                                      voiceGetCallFWResp *pVoiceGetCallFWResp )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiVoiceSLQSGetCallFWResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pVoiceGetCallFWReq ||
         NULL == pVoiceGetCallFWResp )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiVoiceSlqsGetCallFWStatus( &paramLength,
                                                pOutParam,
                                                pVoiceGetCallFWReq );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_VOICE,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Pass the pointer for the OUT parameters */
        response.pVoiceGetCallFWResp = pVoiceGetCallFWResp;

        /* Initializing the response */
        if ( NULL != pVoiceGetCallFWResp->pGetCallFWInfo )
        {
            pVoiceGetCallFWResp->pGetCallFWInfo->numInstances = 0;
        }
        if ( NULL != pVoiceGetCallFWResp->pFailCause )
        {
            *(pVoiceGetCallFWResp->pFailCause) = 0xFFFF;
        }
        if ( NULL != pVoiceGetCallFWResp->pAlphaIDInfo )
        {
            pVoiceGetCallFWResp->pAlphaIDInfo->alphaDcs = 0xFF;
            pVoiceGetCallFWResp->pAlphaIDInfo->alphaLen = 0;
        }
        if ( NULL != pVoiceGetCallFWResp->pCCResType )
        {
            *(pVoiceGetCallFWResp->pCCResType) = 0xFF;
        }
        if ( NULL != pVoiceGetCallFWResp->pCallID )
        {
            *(pVoiceGetCallFWResp->pCallID) = 0;
        }
        if ( NULL != pVoiceGetCallFWResp->pCCSUPSType )
        {
            pVoiceGetCallFWResp->pCCSUPSType->svcType = 0xFF;
            pVoiceGetCallFWResp->pCCSUPSType->reason  = 0xFF;
        }
        if ( NULL != pVoiceGetCallFWResp->pGetCallFWExtInfo )
        {
            pVoiceGetCallFWResp->pGetCallFWExtInfo->numInstances = 0;
        }

        /* Copy to the caller's buffer */
        resultCode = UpkQmiVoiceSlqsGetCallFWStatus( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSVoiceSetCallBarringPassword(
        voiceSetCallBarringPwdInfo *pVoiceSetCallBarringPwdInfo,
        voiceSetCallBarringPwdResp *pSetCallBarringPwdResp )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiVoiceSLQSSetCallBarringPwdResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pVoiceSetCallBarringPwdInfo ||
         NULL == pSetCallBarringPwdResp )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiVoiceSlqsSetCallBarringPwd( &paramLength,
                                                  pOutParam,
                                                  pVoiceSetCallBarringPwdInfo );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_VOICE,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Pass the pointer for the OUT parameters */
        response.pSetCallBarringPwdResp = pSetCallBarringPwdResp;

        if ( NULL != pSetCallBarringPwdResp->pFailCause )
        {
            *(pSetCallBarringPwdResp->pFailCause) = 0xFFFF;
        }
        if ( NULL != pSetCallBarringPwdResp->pAlphaIDInfo )
        {
            pSetCallBarringPwdResp->pAlphaIDInfo->alphaDcs = 0xFF;
            pSetCallBarringPwdResp->pAlphaIDInfo->alphaLen = 0;
        }
        if ( NULL != pSetCallBarringPwdResp->pCCResType )
        {
            *(pSetCallBarringPwdResp->pCCResType) = 0xFF;
        }
        if ( NULL != pSetCallBarringPwdResp->pCallID )
        {
            *(pSetCallBarringPwdResp->pCallID) = 0;
        }
        if ( NULL != pSetCallBarringPwdResp->pCCSUPSType )
        {
            pSetCallBarringPwdResp->pCCSUPSType->svcType = 0xFF;
            pSetCallBarringPwdResp->pCCSUPSType->reason  = 0xFF;
        }

        /* Copy to the caller's buffer */
        resultCode = UpkQmiVoiceSlqsSetCallBarringPwd( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSVoiceGetCallInfo( voiceCallInfoReq  *pGetCallInfoReq,
                            voiceCallInfoResp *pGetCallInfoResp )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiVoiceSLQSGetCallInfoResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pGetCallInfoResp  )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiVoiceSlqsGetCallInfo( &paramLength,
                                            pOutParam,
                                            pGetCallInfoReq );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_VOICE,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Set the pointers for the function OUT parameters */
        response.pGetCallInfoResp = pGetCallInfoResp;

        /* Initializing response parameters */
        if ( NULL != pGetCallInfoResp->pCallInfo )
        {
            pGetCallInfoResp->pCallInfo->callID    = 0;
            pGetCallInfoResp->pCallInfo->callState = 0xFF;
            pGetCallInfoResp->pCallInfo->callType  = 0xFF;
            pGetCallInfoResp->pCallInfo->direction = 0xFF;
            pGetCallInfoResp->pCallInfo->mode      = 0xFF;
        }
        if ( NULL != pGetCallInfoResp->pRemotePartyNum )
        {
            pGetCallInfoResp->pRemotePartyNum->presentationInd = 0xFF;
            pGetCallInfoResp->pRemotePartyNum->numLen          = 0;
        }
        if ( NULL != pGetCallInfoResp->pSrvOpt )
        {
            *(pGetCallInfoResp->pSrvOpt) = 0xFFFF;
        }
        if ( NULL != pGetCallInfoResp->pVoicePrivacy )
        {
            *(pGetCallInfoResp->pVoicePrivacy) = 0xFF;
        }
        if ( NULL != pGetCallInfoResp->pOTASPStatus )
        {
            *(pGetCallInfoResp->pOTASPStatus) = 0xFF;
        }
        if ( NULL != pGetCallInfoResp->pRemotePartyName )
        {
            pGetCallInfoResp->pRemotePartyName->namePI       = 0xFF;
            pGetCallInfoResp->pRemotePartyName->codingScheme = 0xFF;
            pGetCallInfoResp->pRemotePartyName->nameLen      = 0;
        }
        if ( NULL != pGetCallInfoResp->pUUSInfo )
        {
            pGetCallInfoResp->pUUSInfo->UUSType    = 0xFF;
            pGetCallInfoResp->pUUSInfo->UUSDcs     = 0xFF;
            pGetCallInfoResp->pUUSInfo->UUSDatalen = 0;
        }
        if ( NULL != pGetCallInfoResp->pAlertType )
        {
            *(pGetCallInfoResp->pAlertType) = 0xFF;
        }
        if ( NULL != pGetCallInfoResp->pAlphaIDInfo )
        {
            pGetCallInfoResp->pAlphaIDInfo->alphaDcs = 0xFF;
            pGetCallInfoResp->pAlphaIDInfo->alphaLen = 0;
        }
        if ( NULL != pGetCallInfoResp->pConnectNumInfo )
        {
            pGetCallInfoResp->pConnectNumInfo->numPresInd   = 0xFF;
            pGetCallInfoResp->pConnectNumInfo->screeningInd = 0xFF;
            pGetCallInfoResp->pConnectNumInfo->numType      = 0xFF;
            pGetCallInfoResp->pConnectNumInfo->numPlan      = 0xFF;
            pGetCallInfoResp->pConnectNumInfo->callerIDLen  = 0;
        }
        if ( NULL != pGetCallInfoResp->pDiagInfo )
        {
            pGetCallInfoResp->pDiagInfo->diagInfoLen = 0;
        }
        if ( NULL != pGetCallInfoResp->pAlertingPattern )
        {
            *(pGetCallInfoResp->pAlertingPattern) = 0xFFFFFFFF;
        }

        /* Copy to the caller's buffer */
        resultCode = UpkQmiVoiceSlqsGetCallInfo( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSVoiceGetAllCallInfo( voiceGetAllCallInfo *pGetAllCallInfo )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiVoiceSLQSGetAllCallInfoResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pGetAllCallInfo  )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiVoiceSlqsGetAllCallInfo( &paramLength,
                                               pOutParam );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_VOICE,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Set the pointers for the function OUT parameters */
        response.pGetAllCallInfoResp = pGetAllCallInfo;

        if ( NULL != pGetAllCallInfo->pArrCallInfo )
        {
            pGetAllCallInfo->pArrCallInfo->numInstances = 0;
        }
        if ( NULL != pGetAllCallInfo->pArrRemotePartyNum )
        {
            pGetAllCallInfo->pArrRemotePartyNum->numInstances = 0;
        }
        if ( NULL != pGetAllCallInfo->pArrRemotePartyName )
        {
            pGetAllCallInfo->pArrRemotePartyName->numInstances = 0;
        }
        if ( NULL != pGetAllCallInfo->pArrAlertingType )
        {
            pGetAllCallInfo->pArrAlertingType->numInstances = 0;
        }
        if ( NULL != pGetAllCallInfo->pArrUUSInfo )
        {
            pGetAllCallInfo->pArrUUSInfo->numInstances = 0;
        }
        if ( NULL != pGetAllCallInfo->pArrSvcOption )
        {
            pGetAllCallInfo->pArrSvcOption->numInstances = 0;
        }
        if ( NULL != pGetAllCallInfo->pOTASPStatus )
        {
            *( pGetAllCallInfo->pOTASPStatus ) = 0xFF;
        }
        if ( NULL != pGetAllCallInfo->pVoicePrivacy )
        {
            *( pGetAllCallInfo->pVoicePrivacy ) = 0xFF;
        }
        if ( NULL != pGetAllCallInfo->pArrCallEndReason )
        {
            pGetAllCallInfo->pArrCallEndReason->numInstances = 0;
        }
        if ( NULL != pGetAllCallInfo->pArrAlphaID )
        {
            pGetAllCallInfo->pArrAlphaID->numInstances = 0;
        }
        if ( NULL != pGetAllCallInfo->pArrConnectPartyNum )
        {
            pGetAllCallInfo->pArrConnectPartyNum->numInstances = 0;
        }
        if ( NULL != pGetAllCallInfo->pArrDiagInfo )
        {
            pGetAllCallInfo->pArrDiagInfo->numInstances = 0;
        }
        if ( NULL != pGetAllCallInfo->pArrCalledPartyNum )
        {
            pGetAllCallInfo->pArrCalledPartyNum->numInstances = 0;
        }
        if ( NULL != pGetAllCallInfo->pArrRedirPartyNum )
        {
            pGetAllCallInfo->pArrRedirPartyNum->numInstances = 0;
        }
        if ( NULL != pGetAllCallInfo->pArrAlertingPattern )
        {
            pGetAllCallInfo->pArrAlertingPattern->numInstances = 0;
        }

        /* Copy to the caller's buffer */
        resultCode = UpkQmiVoiceSlqsGetAllCallInfo( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSVoiceManageCalls(
    voiceManageCallsReq  *pVoiceManageCallsReq,
    voiceManageCallsResp *pVoiceManageCallsResp )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiVoiceManageCallsResp response;

    /* Checking for Invalid Parameter */
    if ( !pVoiceManageCallsReq ||
         !pVoiceManageCallsResp )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiVoiceManageCalls( &paramLength,
                                        pOutParam,
                                        pVoiceManageCallsReq );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_VOICE,
                                  paramLength,
                                  eQMI_TIMEOUT_10_S, /* 10 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        response.pVoiceManageCallsResp = pVoiceManageCallsResp;

        /* Initialize response parameters to default values */
        if ( pVoiceManageCallsResp->pFailCause )
        {
             *(pVoiceManageCallsResp->pFailCause) = 0xFFFF;
        }

        /* Copy to the caller's buffer */
        resultCode = UpkQmiVoiceManageCalls( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSVoiceBurstDTMF(
    voiceBurstDTMFInfo *pBurstDTMFInfo )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiVoiceSLQSBurstDTMFResp response;

    /* Checking for Invalid Parameter */
    if ( NULL != pBurstDTMFInfo )
    {
        if( !pBurstDTMFInfo->BurstDTMFInfo.pCallID )
        {
            return eQCWWAN_ERR_INVALID_ARG;   
        }
    }
    else
    { 
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiVoiceSlqsBurstDTMF( &paramLength,
                                          pOutParam,
                                          pBurstDTMFInfo );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_VOICE,
                                  paramLength,
                                  eQMI_TIMEOUT_10_S, /* 10 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Pass the pointer for the OUT parameters */
        response.pCallId = pBurstDTMFInfo->BurstDTMFInfo.pCallID;

        /* pCallId validated already against NULL as It is OUT params*/
        /* Assign Invalid Call ID */
        *(response.pCallId) = 0;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiVoiceSLQSBurstDTMF( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSVoiceStartContDTMF (
    voiceContDTMFinfo *pContDTMFInfo )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiVoiceSLQSStartContDTMFResp response;

    /* Mandatory Parameter Checked against NULL */
    if ( NULL != pContDTMFInfo )
    {
        if ( !pContDTMFInfo->pCallID )
        {
            return eQCWWAN_ERR_INVALID_ARG;
        }
    }
    else
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiVoiceSlqsStartContDTMF( &paramLength,
                                              pOutParam,
                                              pContDTMFInfo );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_VOICE,
                                  paramLength,
                                  eQMI_TIMEOUT_10_S, /* 10 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Pass the pointer for the OUT parameters */
        response.pCallId = pContDTMFInfo->pCallID;

        /* pCallId Validated Already against NULL as It is OUT params*/
        /* Assingn Invalid callid */
        *(response.pCallId ) = 0;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiVoiceSLQSStartContDTMF( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSVoiceStopContDTMF (
    voiceStopContDTMFinfo *pVoiceStopContDTMFinfo )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiVoiceSLQSStopContDTMFResp response;

    /* structure params has IN/OUT Param, hence validated against NULL */
    if ( !pVoiceStopContDTMFinfo )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiVoiceSlqsStopContDTMF( &paramLength,
                                              pOutParam,
                                              pVoiceStopContDTMFinfo );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_VOICE,
                                  paramLength,
                                  eQMI_TIMEOUT_10_S, /* 10 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Pass the pointer for the OUT parameters */
        response.callId = pVoiceStopContDTMFinfo->callID;

        /* pCallId Validated Already against NULL as It is OUT params*/
        /* Assign Invalid Call ID */
        response.callId = 0;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiVoiceSLQSStopContDTMF( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSVoiceSendFlash ( voiceFlashInfo *pFlashInfo )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiVoiceSLQSSendFlashResp response;

    /* structure params has IN/OUT Param, hence validated against NULL */
    if ( NULL == pFlashInfo )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiVoiceSlqsSendFlash( &paramLength,
                                          pOutParam,
                                          pFlashInfo );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_VOICE,
                                  paramLength,
                                  eQMI_TIMEOUT_10_S, /* 10 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Pass the pointer for the OUT parameters */
        response.pCallId = pFlashInfo->pCallID;

        /* pCallId Validated Already against NULL as it is IN/OUT params */
        /* Assign Invalid Call ID */
        *( response.pCallId ) = 0;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiVoiceSlqsSendFlash( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSVoiceSetPreferredPrivacy ( voiceSetPrefPrivacy *pSetPrefPrivacy )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiVoiceSLQSSetPrefPrivacyResp response;

    /* structure params has IN/OUT Param, hence validated against NULL */
    if ( NULL == pSetPrefPrivacy )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiVoiceSlqsSetPrefPrivacy( &paramLength,
                                               pOutParam,
                                               pSetPrefPrivacy );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_VOICE,
                                  paramLength,
                                  eQMI_TIMEOUT_10_S, /* 10 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiVoiceSlqsSetPrefPrivacy( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSVoiceIndicationRegister(
    voiceIndicationRegisterInfo *pVoiceIndicationRegisterInfo )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiVoiceSLQSIndicationRegisterResp response;

    /* structure params has IN/OUT Param, hence validated against NULL */
    if ( !pVoiceIndicationRegisterInfo )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }


    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiVoiceSLQSIndicationRegister(
                               &paramLength,
                               pOutParam,
                               pVoiceIndicationRegisterInfo );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_VOICE,
                                  paramLength,
                                  eQMI_TIMEOUT_10_S, /* 10 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiVoiceSLQSIndicationRegister( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSVoiceGetConfig (
        voiceGetConfigReq  *pVoiceGetConfigReq,
        voiceGetConfigResp *pVoiceGetConfigResp )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiVoiceSLQSGetConfigResp response;

    /* Checking for Invalid Parameter */
    if ( !pVoiceGetConfigReq ||
         !pVoiceGetConfigResp )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiVoiceSlqsGetConfig( &paramLength,
                                          pOutParam,
                                          pVoiceGetConfigReq );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_VOICE,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Pass the pointer for the OUT parameters */
        response.pVoiceGetConfigResp = pVoiceGetConfigResp;

        /* Initializing optional parameters */
        if ( NULL != pVoiceGetConfigResp->pAutoAnswerStat )
        {
            *(pVoiceGetConfigResp->pAutoAnswerStat) = 0xFF;
        }
        if ( NULL != pVoiceGetConfigResp->pAirTimerCnt )
        {
            pVoiceGetConfigResp->pAirTimerCnt->namID         = 0xFF;
            pVoiceGetConfigResp->pAirTimerCnt->airTimerValue = 0xFFFFFFFF;
        }
        if ( NULL != pVoiceGetConfigResp->pRoamTimerCnt )
        {
            pVoiceGetConfigResp->pRoamTimerCnt->namID          = 0xFF;
            pVoiceGetConfigResp->pRoamTimerCnt->roamTimerValue = 0xFFFFFFFF;
        }
        if ( NULL != pVoiceGetConfigResp->pCurrTTYMode )
        {
            *(pVoiceGetConfigResp->pCurrTTYMode) = 0xFF;
        }
        if ( NULL != pVoiceGetConfigResp->pCurPrefVoiceSO )
        {
            pVoiceGetConfigResp->pCurPrefVoiceSO->namID           = 0xFF;
            pVoiceGetConfigResp->pCurPrefVoiceSO->evrcCapability  = 0xFF;
            pVoiceGetConfigResp->pCurPrefVoiceSO->homePageVoiceSO = 0xFFFF;
            pVoiceGetConfigResp->pCurPrefVoiceSO->homeOrigVoiceSO = 0xFFFF;
            pVoiceGetConfigResp->pCurPrefVoiceSO->roamOrigVoiceSO = 0xFFFF;
        }
        if ( NULL != pVoiceGetConfigResp->pCurAMRConfig )
        {
            pVoiceGetConfigResp->pCurAMRConfig->gsmAmrStat   = 0xFF;
            pVoiceGetConfigResp->pCurAMRConfig->wcdmaAmrStat = 0xFF;
        }
        if ( NULL != pVoiceGetConfigResp->pCurVoicePrivacyPref )
        {
            *(pVoiceGetConfigResp->pCurVoicePrivacyPref) = 0xFF;
        }
        if ( NULL != pVoiceGetConfigResp->pCurVoiceDomainPref )
        {
            *(pVoiceGetConfigResp->pCurVoiceDomainPref) = 0xFF;
        }

        /* Copy to the caller's buffer */
        resultCode = UpkQmiVoiceSlqsGetConfig( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSVoiceOrigUSSDNoWait (
          voiceOrigUSSDNoWaitInfo *pVoiceOrigUSSDNoWaitInfo )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiVoiceSLQSOrigUSSDNoWaitResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pVoiceOrigUSSDNoWaitInfo  )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiVoiceSlqsOrigUSSDNoWait( &paramLength,
                                               pOutParam,
                                               pVoiceOrigUSSDNoWaitInfo );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_VOICE,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiVoiceSlqsOrigUSSDNoWait( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSVoiceBindSubscription (
          voiceBindSubscriptionInfo *pVoiceBindSubscriptionInfo )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiVoiceSLQSBindSubscriptionResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pVoiceBindSubscriptionInfo  )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiVoiceSlqsBindSubscription( &paramLength,
                                                 pOutParam,
                                                 pVoiceBindSubscriptionInfo );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_VOICE,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiVoiceSlqsBindSubscription( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSVoiceALSSetLineSwitching (
          voiceALSSetLineSwitchInfo *pVoiceALSSetLineSwitchInfo )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiVoiceSLQSALSSetLineSwitchingResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pVoiceALSSetLineSwitchInfo  )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiVoiceSlqsALSSetLineSwitching(
                                         &paramLength,
                                         pOutParam,
                                         pVoiceALSSetLineSwitchInfo );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_VOICE,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiVoiceSlqsALSSetLineSwitching( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSVoiceALSSelectLine (
          voiceALSSelectLineInfo *pVoiceALSSelectLineInfo )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiVoiceSLQSALSSelectLineResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pVoiceALSSelectLineInfo  )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiVoiceSlqsALSSelectLine( &paramLength,
                                              pOutParam,
                                              pVoiceALSSelectLineInfo );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_VOICE,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiVoiceSlqsALSSelectLine( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSVoiceGetCOLP( voiceGetCOLPResp *pVoiceGetCOLPResp )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiVoiceSLQSGetCOLPResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pVoiceGetCOLPResp  )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    resultCode = PkQmiVoiceSlqsGetCOLP( &paramLength,
                                        pOutParam );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_VOICE,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Pass the pointer for the OUT parameters */
        response.pVoiceGetCOLPResp = pVoiceGetCOLPResp;

        /* Initializing the response */
        if ( NULL != pVoiceGetCOLPResp->pCOLPResp )
        {
            pVoiceGetCOLPResp->pCOLPResp->ActiveStatus    = 0xFF;
            pVoiceGetCOLPResp->pCOLPResp->ProvisionStatus = 0xFF;
        }
        if ( NULL != pVoiceGetCOLPResp->pFailCause )
        {
            *(pVoiceGetCOLPResp->pFailCause) = 0xFFFF;
        }
        if ( NULL != pVoiceGetCOLPResp->pAlphaIDInfo )
        {
            pVoiceGetCOLPResp->pAlphaIDInfo->alphaDcs = 0xFF;
            pVoiceGetCOLPResp->pAlphaIDInfo->alphaLen = 0;
        }
        if ( NULL != pVoiceGetCOLPResp->pCCResType )
        {
              *(pVoiceGetCOLPResp->pCCResType) = 0xFF;
        }
        if ( NULL != pVoiceGetCOLPResp->pCallID )
        {
           *(pVoiceGetCOLPResp->pCallID) = 0;
        }
        if ( NULL != pVoiceGetCOLPResp->pCCSUPSType )
        {
            pVoiceGetCOLPResp->pCCSUPSType->svcType = 0xFF;
            pVoiceGetCOLPResp->pCCSUPSType->reason  = 0xFF;
        }

        /* Copy to the caller's buffer */
        resultCode = UpkQmiVoiceSlqsGetCOLP( pInParam, &response );
    }
    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSVoiceGetCOLR( voiceGetCOLRResp *pVoiceGetCOLRResp )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiVoiceSLQSGetCOLRResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pVoiceGetCOLRResp  )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    resultCode = PkQmiVoiceSlqsGetCOLR( &paramLength,
                                        pOutParam );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_VOICE,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Pass the pointer for the OUT parameters */
        response.pVoiceGetCOLRResp = pVoiceGetCOLRResp;

        /* Initializing the response */
        if ( NULL != pVoiceGetCOLRResp->pCOLRResp )
        {
            pVoiceGetCOLRResp->pCOLRResp->ActiveStatus    = 0xFF;
            pVoiceGetCOLRResp->pCOLRResp->ProvisionStatus = 0xFF;
        }
        if ( NULL != pVoiceGetCOLRResp->pFailCause )
        {
            *(pVoiceGetCOLRResp->pFailCause) = 0xFFFF;
        }
        if ( NULL != pVoiceGetCOLRResp->pAlphaIDInfo )
        {
            pVoiceGetCOLRResp->pAlphaIDInfo->alphaDcs = 0xFF;
            pVoiceGetCOLRResp->pAlphaIDInfo->alphaLen = 0;
        }
        if ( NULL != pVoiceGetCOLRResp->pCCResType )
        {
              *(pVoiceGetCOLRResp->pCCResType) = 0xFF;
        }
        if ( NULL != pVoiceGetCOLRResp->pCallID )
        {
           *(pVoiceGetCOLRResp->pCallID) = 0;
        }
        if ( NULL != pVoiceGetCOLRResp->pCCSUPSType )
        {
            pVoiceGetCOLRResp->pCCSUPSType->svcType = 0xFF;
            pVoiceGetCOLRResp->pCCSUPSType->reason  = 0xFF;
        }

        /* Copy to the caller's buffer */
        resultCode = UpkQmiVoiceSlqsGetCOLR( pInParam, &response );
    }
    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSVoiceGetCNAP( voiceGetCNAPResp *pVoiceGetCNAPResp )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiVoiceSLQSGetCNAPResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pVoiceGetCNAPResp  )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    resultCode = PkQmiVoiceSlqsGetCNAP( &paramLength,
                                        pOutParam );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_VOICE,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Pass the pointer for the OUT parameters */
        response.pVoiceGetCNAPResp = pVoiceGetCNAPResp;

        /* Initializing the response */
        if ( NULL != pVoiceGetCNAPResp->pCNAPResp )
        {
            pVoiceGetCNAPResp->pCNAPResp->ActiveStatus    = 0xFF;
            pVoiceGetCNAPResp->pCNAPResp->ProvisionStatus = 0xFF;
        }
        if ( NULL != pVoiceGetCNAPResp->pFailCause )
        {
            *(pVoiceGetCNAPResp->pFailCause) = 0xFFFF;
        }
        if ( NULL != pVoiceGetCNAPResp->pAlphaIDInfo )
        {
            pVoiceGetCNAPResp->pAlphaIDInfo->alphaDcs = 0xFF;
            pVoiceGetCNAPResp->pAlphaIDInfo->alphaLen = 0;
        }
        if ( NULL != pVoiceGetCNAPResp->pCCResType )
        {
              *(pVoiceGetCNAPResp->pCCResType) = 0xFF;
        }
        if ( NULL != pVoiceGetCNAPResp->pCallID )
        {
           *(pVoiceGetCNAPResp->pCallID) = 0;
        }
        if ( NULL != pVoiceGetCNAPResp->pCCSUPSType )
        {
            pVoiceGetCNAPResp->pCCSUPSType->svcType = 0xFF;
            pVoiceGetCNAPResp->pCCSUPSType->reason  = 0xFF;
        }

        /* Copy to the caller's buffer */
        resultCode = UpkQmiVoiceSlqsGetCNAP( pInParam, &response );
    }
    qmrelreqbkp();
    return resultCode;
}
