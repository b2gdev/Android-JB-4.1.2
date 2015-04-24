/**
 * \ingroup sms
 *
 * \file    qaGobiApiSms.c
 *
 * \brief   Entry points for Gobi APIs for the Short Message Service (SMS)
 *
 * Copyright: © 2011-2012 Sierra Wireless, Inc. all rights reserved
 *
 */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "amudefs.h"
#include "sludefs.h"
#include "qaQmiBasic.h"
#include "qaGobiApiSms.h"
#include "qaSmsListMessages.h"
#include "qaSmsDelete.h"
#include "qaSmsGetSMSCAddress.h"
#include "qaSmsSetSMSCAddress.h"
#include "qaSmsRawSend.h"
#include "qaSmsRawWrite.h"
#include "qaSmsRawRead.h"
#include "qaSmsModifyTag.h"
#include "qaSmsSLQSGetBroadcastConfig.h"
#include "qaSmsSLQSSetBroadcastConfig.h"
#include "qaSmsSLQSSetBroadcastActivation.h"
#include "qaEncodeDecodePDU.h"
#include "qaSmsSLQSGetTransLayerInfo.h"
#include "qaSmsSLQSGetTransNWRegInfo.h"
#include "qaSmsSLQSGetIndicationRegister.h"
#include "qaSmsSLQSSetIndicationRegister.h"
#include "qaSmsSLQSSetRoutes.h"
#include "qaSmsSLQSGetMessageProtocol.h"
#include "qaSmsSLQSGetMaxStorageSize.h"

/* To be used for message mode by various APIs */
#define QMI_MSG_MOD_CDMA 0x00
#define QMI_MSG_MOD_GW   0x01
#define WCDMA_MSG_FORMAT      0x06
#define CDMA_MSG_FORMAT       0x00
#define SMS_ASCII_STRING_SIZE 400
#define MAX_LEN_MOBILE        22
#define SMS_TEXT_SIZE         160
#define DEFAULT_BYTE_VALUE    0xFF
#define DEFAULT_LONG_VALUE    0xFFFFFFFF

ULONG SLQSDeleteSMS(
    ULONG storageType,
    ULONG *pMessageIndex,
    ULONG *pMessageTag,
    BYTE  *pMessageMode )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiSmsDeleteResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiSmsDelete( &paramLength,
                                 pOutParam,
                                 storageType,
                                 pMessageIndex,
                                 pMessageTag,
                                 pMessageMode );

    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode; /* No memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WMS,
                                  paramLength,
                                  eQMI_TIMEOUT_10_S, /* 10 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
        resultCode = UpkQmiSmsDelete( pInParam, &response );

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSGetSMS(
    ULONG storageType,
    ULONG messageIndex,
    ULONG *pMessageTag,
    ULONG *pMessageFormat,
    ULONG *pMessageSize,
    BYTE  *pMessage,
    BYTE  *pMessageMode )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiSmsRawReadResp response;

    /* pMessageTag, pMessageFormat, pMessageSize & pMessage are OUT parameters
     * and hence should not be NULL
     */
    if ( !pMessageTag || !pMessageFormat || !pMessageSize || !pMessage )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiSmsRawRead( &paramLength,
                                  pOutParam,
                                  storageType,
                                  messageIndex,
                                  pMessageMode );

    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode; /* No memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WMS,
                                  paramLength,
                                  eQMI_TIMEOUT_5_S, /* 5 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Copy the obtained values to the function OUT parameters */
        response.pMessageTag    = pMessageTag;
        response.pMessageFormat = pMessageFormat;
        response.pMessageSize   = pMessageSize;
        response.pMessage       = pMessage;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiSmsRawRead( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SendSMS(
    ULONG messageFormat,
    ULONG messageSize,
    BYTE  *pMessage,
    ULONG *pMessageFailureCode )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiSmsRawSendResp response;

    /* pMessageFailureCode is an OUT parameter and hence should not be NULL */
    if ( !pMessageFailureCode )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiSmsRawSend( &paramLength,
                                  pOutParam,
                                  messageFormat,
                                  messageSize,
                                  NULL,
                                  pMessage );

    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode; /* No memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WMS,
                                  paramLength,
                                  eQMI_TIMEOUT_300_S, /* 5 Minutes */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Copy the obtained values to the function OUT parameters */
        *pMessageFailureCode         = 0xFFFFFFFF;
        response.pMessageFailureCode = pMessageFailureCode;

        /* Message ID is a mandatory TLV but it is not a part of
         * API argument list hence keeping it NULL
         */
        response.pMessageID = NULL;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiSmsRawSend( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSSendSMS( slqssendsmsparams_s *pSendSmsParams )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiSmsRawSendResp response;

    /* pSendSmsParams should not be NULL */
    if ( !pSendSmsParams )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiSmsRawSend( &paramLength,
                                  pOutParam,
                                  pSendSmsParams->messageFormat,
                                  pSendSmsParams->messageSize,
                                  pSendSmsParams->linktimer,
                                  pSendSmsParams->pMessage );

    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode; /* No memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WMS,
                                  paramLength,
                                  eQMI_TIMEOUT_300_S, /* 5 Minutes */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Copy the obtained values to the function OUT parameters */
        pSendSmsParams->messageFailureCode = 0xFFFFFFFF;
        response.pMessageFailureCode       = &pSendSmsParams->messageFailureCode;
        response.pMessageID                = &pSendSmsParams->messageID;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiSmsRawSend( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG GetSMSCAddress(
    BYTE addressSize,
    CHAR *pSMSCAddress,
    BYTE typeSize,
    CHAR *pSMSCType )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiSmsGetSMSCAddressResp response;

    /* Input parameter validation */
    /* SAMSC Type Size is fixed to 3 bytes
     * hence reserved buffer size should not be less than 4 (accomodate NULL)
     */
    if ( typeSize < QMI_SMSC_TYPE_LEN + 1 )
        return eQCWWAN_ERR_BUFFER_SZ;

    /* pSMSCAddress & pSMSCType are OUT parameters & hence should not be NULL */
    if ( !pSMSCAddress || !pSMSCType )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiSmsGetSMSCAddress( &paramLength,
                                         pOutParam );

    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode; /* No memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WMS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Copy the obtained values to the function OUT parameters */
        response.addressSize  = addressSize;
        response.pSMSCAddress = pSMSCAddress;
        response.pSMSCType    = pSMSCType;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiSmsGetSMSCAddress( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SetSMSCAddress(
    CHAR *pSMSCAddress,
    CHAR *pSMSCType )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiSmsSetSMSCAddressResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiSmsSetSMSCAddress( &paramLength,
                                         pOutParam,
                                         pSMSCAddress,
                                         pSMSCType );
    /* No memory */
    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode;
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WMS,
                                  paramLength,
                                  eQMI_TIMEOUT_5_S, /* 5 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
        resultCode = UpkQmiSmsSetSMSCAddress( pInParam, &response );

    qmrelreqbkp();
    return resultCode;
}

ULONG SaveSMS(
    ULONG storageType,
    ULONG messageFormat,
    ULONG messageSize,
    BYTE  *pMessage,
    ULONG *pMessageIndex )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiSmsRawWriteResp response;

    /* pMessageIndex is an OUT parameter and hence should not be NULL */
    if ( !pMessageIndex )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiSmsRawWrite( &paramLength,
                                   pOutParam,
                                   storageType,
                                   messageFormat,
                                   messageSize,
                                   pMessage );

    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode; /* No memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WMS,
                                  paramLength,
                                  eQMI_TIMEOUT_10_S, /* 10 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Copy the obtained values to the function OUT parameters */
        response.pMessageIndex = pMessageIndex;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiSmsRawWrite( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSGetSMSList(
    ULONG storageType,
    ULONG *pRequestedTag,
    ULONG *pMessageListSize,
    BYTE  *pMessageList,
    BYTE  *pMessageMode )
{
    ULONG  resultcode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT ParamLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiSmsListMessagesResp response;

    if ( !pMessageListSize || !pMessageList )
         return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiSmsListMessages( &ParamLength,
                                       pOutParam,
                                       storageType,
                                       pRequestedTag,
                                       pMessageMode );

    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WMS,
                                  ParamLength,
                                  eQMI_TIMEOUT_5_S,/*5 Seconds */
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultcode == eQCWWAN_ERR_NONE )
    {
        /* Initialize the Structure */
        slmemset( (char *) pMessageList,
                  0,
                  *pMessageListSize);

        /* Set the pointers for the function OUT parameters */
        response.pMessageListSize = pMessageListSize;
        response.pMessageList = (struct QmiSmsMessageList* ) pMessageList;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiSmsListMessages( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG SLQSModifySMSStatus(
    ULONG storageType,
    ULONG messageIndex,
    ULONG messageTag,
    BYTE  *pMessageMode )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiSmsModifyTagResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiSmsModifyTag( &paramLength,
                                    pOutParam,
                                    storageType,
                                    messageIndex,
                                    messageTag,
                                    pMessageMode );
    /* No memory */
    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode;
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WMS,
                                  paramLength,
                                  eQMI_TIMEOUT_5_S, /* 5 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
        resultCode = UpkQmiSmsModifyTag( pInParam, &response );

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSGetSmsBroadcastConfig(
    BYTE                       mode,
    qaQmi3GPPBroadcastCfgInfo  *pBroadcastConfig,
    qaQmi3GPP2BroadcastCfgInfo *pCDMABroadcastConfig )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiSmsGetBroadcastConfigResp response;

    /* Check if mode is valid or not */
    if (!( mode == QMI_MSG_MOD_CDMA || mode == QMI_MSG_MOD_GW ))
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* pCDMABroadcastConfig ,pCDMABroadcastConfigLen are out parameters
     * and should not be NULL. */
    if ( QMI_MSG_MOD_CDMA == mode )
    {
        if ( !pCDMABroadcastConfig )
        {
            return eQCWWAN_ERR_INVALID_ARG;
        }
        pBroadcastConfig    = NULL;
    }

    /* pBroadcastConfig ,pBroadcastConfigLen are out parameters and
     * should not be NULL. */
    if ( QMI_MSG_MOD_GW == mode )
    {
        if ( !pBroadcastConfig )
        {
            return eQCWWAN_ERR_INVALID_ARG;
        }
        pCDMABroadcastConfig    = NULL;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiSmsGetBroadcastConfig( &paramLength,
                                             pOutParam,
                                             mode );

    /* No memory */
    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode;
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WMS,
                                  paramLength,
                                  eQMI_TIMEOUT_5_S, /* 5 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Initializing memory with zero */
        if ( pBroadcastConfig != NULL )
        {
            slmemset( (char *)pBroadcastConfig,0,
                       sizeof(qaQmi3GPPBroadcastCfgInfo) );
        }

        if ( pCDMABroadcastConfig != NULL )
        {
            slmemset( (char *)pCDMABroadcastConfig,0,
                       sizeof(qaQmi3GPP2BroadcastCfgInfo) );
        }

        /* Set the pointers for the function OUT parameters */
        response.pBroadcastConfig     = pBroadcastConfig;
        response.pCDMABroadcastConfig = pCDMABroadcastConfig;

        /* Copy to Caller's Buffer */
        resultCode = UpkQmiSmsGetBroadcastConfig( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSSetSmsBroadcastConfig(
    BYTE                       mode,
    qaQmi3GPPBroadcastCfgInfo  *pBroadcastConfig,
    qaQmi3GPP2BroadcastCfgInfo *pCDMABroadcastConfig )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiSmsSetBroadcastConfigResp response;

    /* Check if the mode is correct */
    if ( QMI_MSG_MOD_CDMA == mode )
    {
    /* In CDMA mode pCDMABroadcastConfig is IN parameter should not be NULL */
        if ( !pCDMABroadcastConfig )
            return eQCWWAN_ERR_INVALID_ARG;
        /* Assign NULL to broadcast config as it is invalid in CDMA mode*/
        pBroadcastConfig = NULL;
    }
    else if ( QMI_MSG_MOD_GW == mode )
    {
    /* In GW mode pBroadcastConfig is IN parameter should not be NULL */
        if ( !pBroadcastConfig )
            return eQCWWAN_ERR_INVALID_ARG;
        /* Assign NULL to CDMA broadcast config as it is invalid in GW mode*/
        pCDMABroadcastConfig = NULL;
    }
    else
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiSmsSetBroadcastConfig( &paramLength,
                                             pOutParam,
                                             mode,
                                             pBroadcastConfig,
                                             pCDMABroadcastConfig );
    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiSmsSetBroadcastConfig( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSSetSmsBroadcastActivation(
    BYTE mode,
    BYTE broadcastActivate )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiSmsSetBroadcastActivationResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiSmsSetBroadcastActivation( &paramLength,
                                                 pOutParam,
                                                 mode,
                                                 broadcastActivate );
    /* No memory */
    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode;
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WMS,
                                  paramLength,
                                  eQMI_TIMEOUT_5_S, /* 5 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
        resultCode = UpkQmiSmsSetBroadcastActivation( pInParam, &response );

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSCDMAEncodeMOTextMsg(
    struct cdmaMsgEncodingParams *pcdmaMsgEncodingParams )
{
    ULONG resultCode;

    /* IN/OUT Parameters checked for NULL */
    if ( !pcdmaMsgEncodingParams               ||
         !pcdmaMsgEncodingParams->pMessageSize ||
         !pcdmaMsgEncodingParams->pMessage     ||
         !pcdmaMsgEncodingParams->pTextMsg     ||
         !pcdmaMsgEncodingParams->pDestAddr )
    {
         return eQCWWAN_ERR_INVALID_ARG;
    }

    if ( slstrlen(pcdmaMsgEncodingParams->pDestAddr) > MAX_LEN_MOBILE )
    {
        pcdmaMsgEncodingParams->pDestAddr[MAX_LEN_MOBILE] ='\0';
    }

    if ( slstrlen((CHAR*)pcdmaMsgEncodingParams->pTextMsg) > MAXSMS_SIZE )
    {
        pcdmaMsgEncodingParams->pTextMsg[SMS_TEXT_SIZE] ='\0';
        pcdmaMsgEncodingParams->textMsgLength = MAXSMS_SIZE;
        return eQCWWAN_ERR_SWISMS_MSG_LEN_TOO_LONG;
    }

    /* Execute funtion to encode text message to CDMA PDU */
    resultCode = SwiCDMAEncodeMOTextMsg(
                          (BYTE*)pcdmaMsgEncodingParams->pMessageSize,
                          (BYTE*)pcdmaMsgEncodingParams->pMessage,
                          pcdmaMsgEncodingParams->messageId,
                          pcdmaMsgEncodingParams->pDestAddr,
                          pcdmaMsgEncodingParams->pCallbackAddr,
                          pcdmaMsgEncodingParams->textMsgLength,
                          (wchar_t*)pcdmaMsgEncodingParams->pTextMsg,
                          pcdmaMsgEncodingParams->pPriority,
                          pcdmaMsgEncodingParams->pEncodingAlphabet,
                          pcdmaMsgEncodingParams->pRelValidity );

    return resultCode;
}

ULONG SLQSCDMADecodeMTTextMsg(
    struct cdmaMsgDecodingParams *pCdmaMsgDecodingParams)
{
    ULONG resultCode;

    /* Check if IN/OUT structure is NULL */
    if ( !pCdmaMsgDecodingParams                    ||
         !pCdmaMsgDecodingParams->pMessage          ||
         !pCdmaMsgDecodingParams->pSenderAddrLength ||
         !pCdmaMsgDecodingParams->pSenderAddr       ||
         !pCdmaMsgDecodingParams->pTextMsgLength    ||
         !pCdmaMsgDecodingParams->pTextMsg )
    {
         return eQCWWAN_ERR_INVALID_ARG;
    }

    if ( sizeof(pCdmaMsgDecodingParams->mcTimeStamp) < TIME_STAMP_BUF )
    {
        return eQCWWAN_ERR_BUFFER_SZ;
    }

    /* Execute funrtion to decode CDMA SMS */
    resultCode = SwiCDMADecodeMTTextMsg(
                         pCdmaMsgDecodingParams->messageLength,
                         pCdmaMsgDecodingParams->pMessage,
                         pCdmaMsgDecodingParams->pMessageID,
                         pCdmaMsgDecodingParams->pSenderAddrLength,
                         pCdmaMsgDecodingParams->pSenderAddr,
                         pCdmaMsgDecodingParams->pTextMsgLength,
                         pCdmaMsgDecodingParams->pTextMsg,
                         pCdmaMsgDecodingParams->pPriority,
                         pCdmaMsgDecodingParams->pPrivacy,
                         pCdmaMsgDecodingParams->pLanguage,
                         pCdmaMsgDecodingParams->mcTimeStamp,
                         pCdmaMsgDecodingParams->absoluteValidity,
                         pCdmaMsgDecodingParams->pRelativeValidity,
                         pCdmaMsgDecodingParams->pDisplayMode,
                         pCdmaMsgDecodingParams->pUserAcknowledgementReq,
                         pCdmaMsgDecodingParams->pReadAcknowledgementReq,
                         pCdmaMsgDecodingParams->pAlertPriority,
                         pCdmaMsgDecodingParams->pCallbkAddrLength,
                         pCdmaMsgDecodingParams->pCallbkAddr );

    return resultCode;
}

ULONG SLQSWCDMAEncodeMOTextMsg(
    struct wcdmaMsgEncodingParams *pWcdmaMsgEncodingParams )
{
    ULONG resultCode;
    CHAR  pdu[SMS_ASCII_STRING_SIZE] = {0};

    /* IN/OUT Parameters checked for NULL */
    if ( !pWcdmaMsgEncodingParams||
         !pWcdmaMsgEncodingParams->pDestAddr||
         !pWcdmaMsgEncodingParams->pTextMsg||
         !pWcdmaMsgEncodingParams->pPDUMessage )
    {
         return eQCWWAN_ERR_INVALID_ARG;
    }

    if ( strlen(pWcdmaMsgEncodingParams->pTextMsg) > MAXSMS_SIZE )
    {
        pWcdmaMsgEncodingParams->pTextMsg[SMS_TEXT_SIZE] ='\0';
        return eQCWWAN_ERR_SWISMS_MSG_LEN_TOO_LONG;
    }

    resultCode = SwiMakePdu( pWcdmaMsgEncodingParams->pDestAddr,
                             pWcdmaMsgEncodingParams->pTextMsg,
                             strlen(pWcdmaMsgEncodingParams->pTextMsg),
                             pdu,
                             0 );

    /* Convert string in hex dump */
    SwiAsc2Hex((BYTE*)pWcdmaMsgEncodingParams->pPDUMessage, pdu);
    pWcdmaMsgEncodingParams->messageSize = strlen(pdu)/2;

    return resultCode;
}

ULONG SLQSWCDMADecodeMTTextMsg(
    struct wcdmaMsgDecodingParams *pWcdmaMsgDecodingParams )
{
    if ( !pWcdmaMsgDecodingParams->pMessage          ||
         !pWcdmaMsgDecodingParams->pSenderAddrLength ||
         !pWcdmaMsgDecodingParams->pSenderAddr       ||
         !pWcdmaMsgDecodingParams->Date              ||
         !pWcdmaMsgDecodingParams->Time              ||
         !pWcdmaMsgDecodingParams->pTextMsgLength    ||
         !pWcdmaMsgDecodingParams->pTextMsg          ||
         !pWcdmaMsgDecodingParams->pScAddrLength     ||
         !pWcdmaMsgDecodingParams->pScAddr )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    ULONG resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE  smsString[SMS_ASCII_STRING_SIZE] = {'\0'};

    /* Covert message into dump of hex string */
    SwiHex2Asc(smsString,pWcdmaMsgDecodingParams->pMessage,\
                         SMS_ASCII_STRING_SIZE );

    /* Extract message details */
    resultCode = SwiExtractpdu((CHAR *)smsString,
                                pWcdmaMsgDecodingParams->pSenderAddrLength,
                                pWcdmaMsgDecodingParams->pSenderAddr,
                                pWcdmaMsgDecodingParams->pTextMsgLength,
                                pWcdmaMsgDecodingParams->pScAddrLength,
                                (CHAR *)pWcdmaMsgDecodingParams->Date,
                                (CHAR *)pWcdmaMsgDecodingParams->Time,
                                pWcdmaMsgDecodingParams->pTextMsg,
                                pWcdmaMsgDecodingParams->pScAddr );

    return resultCode;
}

ULONG SLQSGetTransLayerInfo (
    getTransLayerInfoResp  *pGetTransLayerInfoResp )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiSmsSLQSGetTransLayerInfoResp response;

    /* Checking for Invalid Parameter */
    if ( !pGetTransLayerInfoResp )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiSmsSLQSGetTransLayerInfo( &paramLength,
                                                pOutParam );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WMS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Set the pointers for the function OUT parameters */
        response.pGetTransLayerResp = pGetTransLayerInfoResp;

        /* Initialize the response structure */
        if( pGetTransLayerInfoResp->pRegInd )
            *(pGetTransLayerInfoResp->pRegInd) = 0xFF;
        if( pGetTransLayerInfoResp->pTransLayerInfo )
        {
            pGetTransLayerInfoResp->pTransLayerInfo->TransCap  = 0xFF;
            pGetTransLayerInfoResp->pTransLayerInfo->TransType = 0xFF;
        }

        /* Copy to the caller's buffer */
        resultCode = UpkQmiSmsSLQSGetTransLayerInfo( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSGetTransNWRegInfo (
    getTransNWRegInfoResp *pTransNWRegInfo )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiSmsSLQSGetTransNWRegInfoResp response;

    /* Checking for Invalid Parameter */
    if ( !pTransNWRegInfo )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiSmsSLQSGetTransNWRegInfo( &paramLength,
                                                pOutParam );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WMS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Set the pointers for the function OUT parameters */
        response.pTransNWRegInfo = pTransNWRegInfo;

        /* Initialize the response structure */
        if( pTransNWRegInfo->pRegStatus )
            *(pTransNWRegInfo->pRegStatus) = 0xFF;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiSmsSLQSGetTransNWRegInfo( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSGetIndicationRegister (
    getIndicationRegResp  *pGetIndicationRegInfo )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiSmsSLQSGetIndicationRegisterResp response;

    /* Checking for Invalid Parameter */
    if ( !pGetIndicationRegInfo )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiSmsSLQSGetIndicationReg( &paramLength,
                                               pOutParam );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WMS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Set the pointers for the function OUT parameters */
        response.pGetIndicationRegInfo = pGetIndicationRegInfo;

        /* Initialize the response parameters */
        if( pGetIndicationRegInfo->pRegTransLayerInfoEvt )
            *(pGetIndicationRegInfo->pRegTransLayerInfoEvt) = 0xFF;
        if( pGetIndicationRegInfo->pRegTransNWRegInfoEvt )
            *(pGetIndicationRegInfo->pRegTransLayerInfoEvt) = 0xFF;
        if( pGetIndicationRegInfo->pRegCallStatInfoEvt )
            *(pGetIndicationRegInfo->pRegCallStatInfoEvt) = 0xFF;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiSmsSLQSGetIndicationReg( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSSetIndicationRegister (
    setIndicationRegReq  *pSetIndicationRegReqInfo )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiSmsSLQSSetIndicationRegisterResp response;

    /* Checking for Invalid Parameter */
    if ( !pSetIndicationRegReqInfo )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiSmsSLQSSetIndicationReg( &paramLength,
                                               pOutParam,
                                               pSetIndicationRegReqInfo);

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WMS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiSmsSLQSSetIndicationReg( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSSmsSetRoutes (smsSetRoutesReq *pSetRoutesReq)
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiSmsSLQSSetRoutesResp response;

    /* Checking for Invalid Parameter */
    if ( !pSetRoutesReq )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiSmsSLQSSetRoutes (&paramLength, pOutParam, pSetRoutesReq);

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WMS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiSmsSLQSSetRoutes( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSSmsGetMessageProtocol( smsMsgprotocolResp *pMessageProtocol )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiSmsSLQSGetMsgProtocolResp response;

    /* Checking for Invalid Parameter */
    if ( !pMessageProtocol )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiSmsSLQSGetMessageProtocol( &paramLength, pOutParam );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WMS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy the obtained values to the function OUT parameters */
        response.pMessageProtocol = pMessageProtocol;
        pMessageProtocol->msgProtocol = DEFAULT_BYTE_VALUE;
        resultCode = UpkQmiSmsSLQSMessageProtocol( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSSmsGetMaxStorageSize(
    smsMaxStorageSizeReq *pMaxStorageSizeReq,
    smsMaxStorageSizeResp *pMaxStorageSizeResp )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiSmsSLQSGetMaxStorageSizeResp response;

    /* Checking for Invalid Parameter */
    if ( (NULL == pMaxStorageSizeReq) || (NULL == pMaxStorageSizeResp) )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiSmsSLQSGetMaxStorageSize( &paramLength,
                                              pOutParam,
                                              pMaxStorageSizeReq );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_WMS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Set the pointers for the function OUT parameters */
        response.pMaxStorageSizeResp = pMaxStorageSizeResp;

        /* Initialize the response parameters */
        pMaxStorageSizeResp->maxStorageSize = DEFAULT_LONG_VALUE;
        pMaxStorageSizeResp->freeSlots = DEFAULT_LONG_VALUE;

        /* Copy the obtained values to the function OUT parameters */
        resultCode = UpkQmiSmsSLQSGetmaxStorageSize( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}
