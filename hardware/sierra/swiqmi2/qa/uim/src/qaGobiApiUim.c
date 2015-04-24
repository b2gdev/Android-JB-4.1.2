/*
 * \ingroup uim
 *
 * \file    qaGobiApiUim.c
 *
 * \brief   Entry points for Gobi APIs for the Uim Service
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "amudefs.h"
#include "qaQmiBasic.h"
#include "qaUimSLQSUimReset.h"
#include "qaUimSLQSUimPowerDown.h"
#include "qaUimSLQSUimGetCardStatus.h"
#include "qaUimSLQSUimSetPinProtection.h"
#include "qaUimSLQSUimVerifyPin.h"
#include "qaUimSLQSUimChangePin.h"
#include "qaUimSLQSUimUnblockPin.h"
#include "qaUimSLQSUimEventRegister.h"
#include "qaUimSLQSUimRefreshOK.h"
#include "qaUimSLQSUimRefreshRegister.h"
#include "qaUimSLQSUimEventRegister.h"
#include "qaUimSLQSUimRefreshComplete.h"
#include "qaUimSLQSUimRefreshGetLastEvent.h"
#include "qaUimSLQSUimGetFileAttributes.h"
#include "qaUimSLQSUimDepersonalization.h"
#include "qaUimSLQSUimAuthenticate.h"

ULONG SLQSUIMReset()
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiUimSLQSResetResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiUimSlqsReset( &paramLength,
                                    pOutParam );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_UIM,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiUimSlqsReset( pInParam, &response );
    }
    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSUIMPowerDown ( UIMPowerDownReq *pUIMPowerDownReq )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiUimSLQSPowerDownResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pUIMPowerDownReq  )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiUimSlqsPowerDown( &paramLength,
                                        pOutParam,
                                        pUIMPowerDownReq );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_UIM,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiUimSlqsPowerDown( pInParam, &response );
    }
    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSUIMGetCardStatus ( UIMGetCardStatusResp *pUIMGetCardStatusResp )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiUimSLQSGetCardStatusResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pUIMGetCardStatusResp  )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiUimSlqsGetCardStatus( &paramLength,
                                            pOutParam );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_UIM,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        response.pUIMGetCardStatusResp = pUIMGetCardStatusResp;

        /* Initializing the OUT params */
        if ( pUIMGetCardStatusResp->pCardStatus )
        {
            pUIMGetCardStatusResp->pCardStatus->indexGwPri  = 0xFFFF;
            pUIMGetCardStatusResp->pCardStatus->indexGwSec  = 0xFFFF;
            pUIMGetCardStatusResp->pCardStatus->index1xPri  = 0xFFFF;
            pUIMGetCardStatusResp->pCardStatus->index1xSec  = 0xFFFF;
            pUIMGetCardStatusResp->pCardStatus->numSlot     = 0x00;
        }
        if ( pUIMGetCardStatusResp->pHotSwapStatus )
        {
            pUIMGetCardStatusResp->pHotSwapStatus->hotSwapLength = 0x00;
        }

        /* Copy to the caller's buffer */
        resultCode = UpkQmiUimSlqsGetCardStatus( pInParam, &response );
    }
    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSUIMGetFileAttributes (
                      UIMGetFileAttributesReq  *pUIMGetFileAttributesReq,
                      UIMGetFileAttributesResp *pUIMGetFileAttributesResp )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiUimSLQSGetFileAttributesResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pUIMGetFileAttributesReq ||
         NULL == pUIMGetFileAttributesResp )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiUimSlqsGetFileAttributes( &paramLength,
                                                pOutParam,
                                                pUIMGetFileAttributesReq );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_UIM,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        response.pUIMGetFileAttributesResp = pUIMGetFileAttributesResp;

        /* Initializing the OUT params */
        if ( pUIMGetFileAttributesResp->pCardResult )
        {
            pUIMGetFileAttributesResp->pCardResult->sw1 = 0xFF;
            pUIMGetFileAttributesResp->pCardResult->sw2 = 0xFF;
        }
        if ( pUIMGetFileAttributesResp->pFileAttributes )
        {
            pUIMGetFileAttributesResp->pFileAttributes->fileSize      = 0xFFFF;
            pUIMGetFileAttributesResp->pFileAttributes->fileID        = 0xFFFF;
            pUIMGetFileAttributesResp->pFileAttributes->fileType      = 0xFF;
            pUIMGetFileAttributesResp->pFileAttributes->recordSize    = 0xFFFF;
            pUIMGetFileAttributesResp->pFileAttributes->recordCount   = 0xFFFF;
            pUIMGetFileAttributesResp->pFileAttributes->secRead       = 0xFF;
            pUIMGetFileAttributesResp->pFileAttributes->secReadMask   = 0xFFFF;
            pUIMGetFileAttributesResp->pFileAttributes->secWrite      = 0xFF;
            pUIMGetFileAttributesResp->pFileAttributes->secWriteMask  = 0xFFFF;
            pUIMGetFileAttributesResp->pFileAttributes->secIncrease   = 0xFF;
            pUIMGetFileAttributesResp->pFileAttributes->secIncreaseMask
                                                                      = 0xFFFF;
            pUIMGetFileAttributesResp->pFileAttributes->secDeactivate = 0xFF;
            pUIMGetFileAttributesResp->pFileAttributes->secDeactivateMask
                                                                      = 0xFFFF;
            pUIMGetFileAttributesResp->pFileAttributes->secActivate   = 0xFF;
            pUIMGetFileAttributesResp->pFileAttributes->secActivateMask
                                                                      = 0xFFFF;
            pUIMGetFileAttributesResp->pFileAttributes->rawLen        = 0x00;
        }
        if ( pUIMGetFileAttributesResp->pIndicationToken )
        {
            *( pUIMGetFileAttributesResp->pIndicationToken ) = 0xFFFFFFFF;
        }

        /* Copy to the caller's buffer */
        resultCode = UpkQmiUimSlqsGetFileAttributes( pInParam, &response );
    }
    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSUIMSetPinProtection (
                UIMSetPinProtectionReq *pUIMSetPinProtectionReq,
                UIMPinResp             *pUIMSetPinProtectionResp )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiUimSLQSSetPinProtectionResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pUIMSetPinProtectionReq ||
         NULL == pUIMSetPinProtectionResp )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiUimSlqsSetPinProtection( &paramLength,
                                               pOutParam,
                                               pUIMSetPinProtectionReq);

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_UIM,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        response.pUIMSetPinProtectionResp = pUIMSetPinProtectionResp;

        /* Initializing the OUT params */
        if ( pUIMSetPinProtectionResp->pRemainingRetries )
        {
            pUIMSetPinProtectionResp->pRemainingRetries->verifyLeft  = 0xFF;
            pUIMSetPinProtectionResp->pRemainingRetries->unblockLeft = 0xFF;
        }
        if ( pUIMSetPinProtectionResp->pEncryptedPIN1 )
        {
            pUIMSetPinProtectionResp->pEncryptedPIN1->pin1Len = 0x00;
        }
        if ( pUIMSetPinProtectionResp->pIndicationToken )
        {
            *( pUIMSetPinProtectionResp->pIndicationToken ) = 0xFFFFFFFF;
        }

        /* Copy to the caller's buffer */
        resultCode = UpkQmiUimSlqsSetPinProtection( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSUIMVerifyPin (
                UIMVerifyPinReq *pUIMVerifyPinReq,
                UIMPinResp      *pUIMVerifyPinResp )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiUimSLQSVerifyPinResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pUIMVerifyPinReq ||
         NULL == pUIMVerifyPinResp )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiUimSlqsVerifyPin( &paramLength,
                                        pOutParam,
                                        pUIMVerifyPinReq );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_UIM,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        response.pUIMVerifyPinResp = pUIMVerifyPinResp;

        /* Initializing the OUT params */
        if ( pUIMVerifyPinResp->pRemainingRetries )
        {
            pUIMVerifyPinResp->pRemainingRetries->verifyLeft  = 0xFF;
            pUIMVerifyPinResp->pRemainingRetries->unblockLeft = 0xFF;
        }
        if ( pUIMVerifyPinResp->pEncryptedPIN1 )
        {
            pUIMVerifyPinResp->pEncryptedPIN1->pin1Len = 0x00;
        }
        if ( pUIMVerifyPinResp->pIndicationToken )
        {
            *( pUIMVerifyPinResp->pIndicationToken ) = 0xFFFFFFFF;
        }

        /* Copy to the caller's buffer */
        resultCode = UpkQmiUimSlqsVerifyPin( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSUIMChangePin ( UIMChangePinReq *pUIMChangePinReq,
                         UIMPinResp      *pUIMChangePinResp )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiUimSLQSChangePinResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pUIMChangePinReq ||
         NULL == pUIMChangePinResp )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiUimSlqsChangePin( &paramLength,
                                        pOutParam,
                                        pUIMChangePinReq );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_UIM,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        response.pUIMChangePinResp = pUIMChangePinResp;

        /* Initializing the OUT params */
        if ( pUIMChangePinResp->pRemainingRetries )
        {
            pUIMChangePinResp->pRemainingRetries->verifyLeft  = 0xFF;
            pUIMChangePinResp->pRemainingRetries->unblockLeft = 0xFF;
        }
        if ( pUIMChangePinResp->pEncryptedPIN1 )
        {
            pUIMChangePinResp->pEncryptedPIN1->pin1Len = 0x00;
        }
        if ( pUIMChangePinResp->pIndicationToken )
        {
            *( pUIMChangePinResp->pIndicationToken ) = 0xFFFFFFFF;
        }

        /* Copy to the caller's buffer */
        resultCode = UpkQmiUimSlqsChangePin( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSUIMUnblockPin ( UIMUnblockPinReq *pUIMUnblockPinReq,
                          UIMPinResp       *pUIMUnblockPinResp )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiUimSLQSUnblockPinResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pUIMUnblockPinReq ||
         NULL == pUIMUnblockPinResp )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiUimSlqsUnblockPin( &paramLength,
                                          pOutParam,
                                          pUIMUnblockPinReq );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_UIM,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        response.pUIMUnblockPinResp = pUIMUnblockPinResp;

        /* Initializing the OUT params */
        if ( pUIMUnblockPinResp->pRemainingRetries )
        {
            pUIMUnblockPinResp->pRemainingRetries->verifyLeft  = 0xFF;
            pUIMUnblockPinResp->pRemainingRetries->unblockLeft = 0xFF;
        }
        if ( pUIMUnblockPinResp->pEncryptedPIN1 )
        {
            pUIMUnblockPinResp->pEncryptedPIN1->pin1Len = 0x00;
        }
        if ( pUIMUnblockPinResp->pIndicationToken )
        {
            *( pUIMUnblockPinResp->pIndicationToken ) = 0xFFFFFFFF;
        }

        /* Copy to the caller's buffer */
        resultCode = UpkQmiUimSlqsUnblockPin( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSUIMEventRegister (
    UIMEventRegisterReqResp *pUIMEventRegisterReqResp )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiUimSLQSEventRegisterResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pUIMEventRegisterReqResp )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiUimSlqsEventRegister( &paramLength,
                                            pOutParam,
                                            pUIMEventRegisterReqResp );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_UIM,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        response.eventMask = pUIMEventRegisterReqResp->eventMask;
        response.eventMask = 0x00000000;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiUimSlqsEventRegister( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}


ULONG SLQSUIMRefreshOK (
    UIMRefreshOKReq *pUIMRefreshOKReq )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiUimSLQSRefreshOKResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pUIMRefreshOKReq )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiUimSlqsRefreshOK( &paramLength,
                                        pOutParam,
                                        pUIMRefreshOKReq );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_UIM,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiUimSlqsRefreshOK( pInParam, &response );
    }
    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSUIMRefreshRegister(
    UIMRefreshRegisterReq *pUIMRefreshRegisterReq )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiUimSLQSRefreshRegisterResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pUIMRefreshRegisterReq )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiUimSlqsRefreshRegister( &paramLength,
                                              pOutParam,
                                              pUIMRefreshRegisterReq );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_UIM,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiUimSlqsRefreshRegister( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSUIMRefreshComplete (
    UIMRefreshCompleteReq *pUIMRefreshCompleteReq )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiUimSLQSRefreshCompleteResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pUIMRefreshCompleteReq )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiUimSlqsRefreshComplete( &paramLength,
                                        pOutParam,
                                        pUIMRefreshCompleteReq );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_UIM,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 seconds */
                                  &pInParam,
                                  &paramLength );
    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiUimSlqsRefreshComplete( pInParam, &response );
    }
    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSUIMRefreshGetLastEvent(
    UIMRefreshGetLastEventReq  *pUIMRefreshGetLastEventReq,
    UIMRefreshGetLastEventResp *pUIMRefreshGetLastEventResp )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */
    BYTE   lCount;

    /* Storage for results and response variable */
    struct QmiUimSLQSRefreshGetLastEventResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pUIMRefreshGetLastEventReq ||
         NULL == pUIMRefreshGetLastEventResp )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiUimSlqsRefreshGetLastEvent( &paramLength,
                                        pOutParam,
                                        pUIMRefreshGetLastEventReq );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_UIM,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        response.pUIMRefreshGetLastEventResp = pUIMRefreshGetLastEventResp;

        /* Initializing the OUT params */
        if ( pUIMRefreshGetLastEventResp->pRefreshEvent )
        {
            pUIMRefreshGetLastEventResp->pRefreshEvent->stage = 0xFF;
            pUIMRefreshGetLastEventResp->pRefreshEvent->mode  = 0xFF;
            pUIMRefreshGetLastEventResp->pRefreshEvent->sessionType = 0xFF;
            pUIMRefreshGetLastEventResp->pRefreshEvent->aidLength   = 0xFF;

            for( lCount = 0; lCount < 0xFF; lCount++)
            {
                pUIMRefreshGetLastEventResp->pRefreshEvent->aid[lCount] = 0xFF;
            }
            pUIMRefreshGetLastEventResp->pRefreshEvent->numOfFiles = 0x00;
        }

        /* Copy to the caller's buffer */
        resultCode = UpkQmiUimSlqsRefreshGetLastEvent( pInParam, &response );
    }
    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSUIMDepersonalization (
                      UIMDepersonalizationReq  *pUIMDepersonalizationReq,
                      UIMDepersonalizationResp *pUIMDepersonalizationResp )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiUimSLQSDepersonalizationResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pUIMDepersonalizationReq ||
         NULL == pUIMDepersonalizationResp )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiUimSlqsDepersonalization( &paramLength,
                                                pOutParam,
                                                pUIMDepersonalizationReq );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_UIM,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        response.pUIMDepersonalizationResp = pUIMDepersonalizationResp;

        /* Initializing the OUT params */
        if ( pUIMDepersonalizationResp->pRemainingRetries )
        {
            pUIMDepersonalizationResp->pRemainingRetries->verifyLeft  = 0xFF;
            pUIMDepersonalizationResp->pRemainingRetries->unblockLeft = 0xFF;
        }

        /* Copy to the caller's buffer */
        resultCode = UpkQmiUimSlqsDepersonalization( pInParam, &response );
    }
    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSUIMAuthenticate ( UIMAuthenticateReq  *pUIMAuthenticateReq,
                            UIMAuthenticateResp *pUIMAuthenticateResp)
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiUimSLQSAuthenticateResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pUIMAuthenticateReq ||
         NULL == pUIMAuthenticateResp )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiUimSlqsAuthenticate( &paramLength,
                                           pOutParam,
                                           pUIMAuthenticateReq );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_UIM,
                                  paramLength,
                                  eQMI_TIMEOUT_30_S, /* 30 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        response.pUIMAuthenticateResp = pUIMAuthenticateResp;

        /* Initializing the OUT params */
        if ( pUIMAuthenticateResp->pCardResult )
        {
            pUIMAuthenticateResp->pCardResult->sw1 = 0xFF;
            pUIMAuthenticateResp->pCardResult->sw2 = 0xFF;
        }
        if ( pUIMAuthenticateResp->pAuthenticateResult )
        {
            pUIMAuthenticateResp->pAuthenticateResult->contentLen = 0;
        }
        if ( pUIMAuthenticateResp->pIndicationToken )
        {
            *( pUIMAuthenticateResp->pIndicationToken ) = 0xFFFFFFFF;
        }

        /* Copy to the caller's buffer */
        resultCode = UpkQmiUimSlqsAuthenticate( pInParam, &response );
    }
    qmrelreqbkp();
    return resultCode;
}
