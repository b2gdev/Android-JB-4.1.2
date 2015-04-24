/*
 * \ingroup dms
 *
 * \file qaGobiApiDms.c
 *
 * \brief  Entry points for Gobi APIs for the Device Management Service ( DMS )
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#include "SwiDataTypes.h"
#include "qaGobiApiDms.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "amudefs.h"
#include "sludefs.h"
#include "qaQmiBasic.h"
#include "qaDmsGetDeviceMfr.h"
#include "qaDmsGetDeviceModelID.h"
#include "qaDmsGetDeviceRevisionID.h"
#include "qaDmsGetPRLVersion.h"
#include "qaDmsGetMSISDN.h"
#include "qaDmsUIMGetIMSI.h"
#include "qaDmsGetDeviceSerialNumbers.h"
#include "qaDmsGetDeviceHardwareRev.h"
#include "qaDmsGetTime.h"
#include "qaDmsUIMSetPINProtection.h"
#include "qaDmsUIMVerifyPIN.h"
#include "qaDmsUIMUnblockPIN.h"
#include "qaDmsUIMChangePIN.h"
#include "qaDmsSetOperatingMode.h"
#include "qaDmsGetOperatingMode.h"
#include "qaDmsUIMGetCkStatus.h"
#include "qaDmsUIMGetICCID.h"
#include "qaDmsUIMGetPINStatus.h"
#include "qaDmsUIMSetCkProtection.h"
#include "qaDmsUIMUnblockCk.h"
#include "qaDmsGetDeviceCap.h"
#include "qaDmsRestoreFactoryDefaults.h"
#include "qaDmsValSerProgramCode.h"
#include "qaDmsActivateAutomatic.h"
#include "qaDmsGetActivationState.h"
#include "qaDmsSlqsUIMGetState.h"
#include "qaDmsGetBandCapability.h"
#include "qaDmsSLQSGetCustFeatures.h"
#include "qaDmsSLQSSetCustFeatures.h"
#include "qaDmsSLQSGetCurrentPRLInfo.h"

/* To be used by UIM PIN related APIs */
#define QMI_DMS_PIN_ONE 1
#define QMI_DMS_PIN_TWO 2

ULONG GetManufacturer(
    BYTE    stringSize,
    CHAR    *pString )
{
    ULONG              resultcode;     /* Result of SwiQmiSendnWait() */
    BYTE               *pInParam;      /* ptr to param field rx'd from modem */
    BYTE               *pOutParam;     /* ptr to outbound param field */
    BYTE               *pReqBuf;       /* Pointer to outgoing request buffer */
    USHORT             ParamLength;    /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiDmsGetDeviceMfrResp response;

    /* Device string is OUT parameter and hence should be NULL */
    if ( !pString  )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }
    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiDmsGetDeviceMfr( &ParamLength,
                                   pOutParam );
    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S,
                                  &pInParam,
                                  &ParamLength );
    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
    {
        /* Initialisation of Buffer */
        slmemset((char *)pString, 0, stringSize);
        /* Pass the pointer for the OUT parameters */
        response.pString     = pString;
        response.stringSize  = stringSize;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiDmsGetDeviceMfr(pInParam, &response);
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG GetModelID(
    BYTE    stringSize,
    CHAR    *pString )
{
    ULONG               resultcode;     /* Result of SwiQmiSendnWait() */
    BYTE                *pInParam;      /* ptr to param field rx'd from modem */
    BYTE                *pOutParam;     /* ptr to outbound param field */
    BYTE                *pReqBuf;       /* Pointer to outgoing request buffer */
    USHORT              ParamLength;    /* Ret'd length of the  QMI Param field */

    /* Storage for results and response variable */
    struct QmiDmsGetDeviceModelIDResp response;

    /* Device string is OUT parameter and hence should be NULL */
    if ( !pString  )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }
    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiDmsGetDeviceModelID( &ParamLength,
                                       pOutParam );
    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S,
                                  &pInParam,
                                  &ParamLength );
    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
    {
        /* Initialisation of Buffer */
        slmemset((char *)pString, 0, stringSize);
        /* Pass the pointer for the OUT parameters */
        response.pString        = pString;
        response.stringSize     = stringSize;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiDmsGetDeviceModelID(pInParam, &response);
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG GetFirmwareRevision(
    BYTE     stringSize,
    CHAR    *pString )
{
    CHAR lBootString, lPRIString;
    return GetFirmwareRevisions ( stringSize,
                                  pString,
                                  0,
                                  &lBootString,
                                  0,
                                  &lPRIString);
}

ULONG GetFirmwareRevisions(
    BYTE    amssSize,
    CHAR    *pAMSSString,
    BYTE    bootSize,
    CHAR    *pBootString,
    BYTE    priSize,
    CHAR    *pPRIString )
{
    ULONG             resultcode;  /* Result of SwiQmiSendnWait() */
    BYTE              *pInParam;    /* ptr to param field rx'd from modem */
    BYTE              *pOutParam;   /* ptr to outbound param field */
    BYTE              *pReqBuf;     /* Pointer to outgoing request buffer */
    USHORT            ParamLength; /* Ret'd length of the  QMI Param field */

    /* Storage for results and response variable */
    struct QmiDmsGetDeviceRevIDResp response;

    /* Device string is OUT parameter and hence should be NULL */
    if ( !pAMSSString || !pBootString || !pPRIString )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }
    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiDmsGetDeviceRevID( &ParamLength,
                                     pOutParam  );
    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S,
                                  &pInParam,
                                  &ParamLength );
    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
    {
        /* Initialisation of Buffer */
        slmemset((char *)pAMSSString, 0, amssSize);
        slmemset((char *)pBootString, 0, bootSize);
        slmemset((char *)pPRIString, 0, priSize);
        /* Pass the pointer for the OUT parameters */
        response.pAMSSString = pAMSSString;
        response.pBootString = pBootString;
        response.pPRIString  = pPRIString;
        response.amssSize    = amssSize;
        response.bootSize    = bootSize;
        response.priSize     = priSize;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiDmsGetDeviceRevID(pInParam, &response);
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG GetPRLVersion(
    WORD    *pPRLVersion )
{
    ULONG               resultcode;  /* Result of SwiQmiSendnWait() */
    BYTE                *pInParam;   /* ptr to param field rx'd from modem */
    BYTE                *pOutParam;  /* ptr to outbound param field */
    BYTE                *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT              ParamLength; /* Ret'd length of the QMI Parm field */

    /* Storage for results and response variable */
    struct QmiDmsGetPRLVersionResp response;

    /* Device string is OUT parameter and hence should be NULL */
    if ( !pPRLVersion )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Index points to first QMI header. */
    ParamLength = 0;

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiDmsGetPRLVersion( &ParamLength, pOutParam );
    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &ParamLength );


    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
    {
        /* Copy the obained values to the function OUT parameters */
        response.pPRLVersion = pPRLVersion;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiDmsGetPRLVersion( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG GetIMSI(
    BYTE   stringSize,
    CHAR   *pString )
{
    ULONG               resultcode;  /* Result of SwiQmiSendnWait() */
    BYTE                *pInParam;   /* ptr to param field rx'd from modem */
    BYTE                *pOutParam;  /* ptr to outbound param field */
    BYTE                *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT              ParamLength; /* Ret'd length of the QMI Parm field */

    /* Storage for results and response variable */
    struct QmiDmsUIMGetIMSIResp response;

    /* Device string is OUT parameter and hence should be NULL */
    if ( !pString )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Index points to first QMI header. */
    ParamLength = 0;

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiDmsUIMGetIMSI( &ParamLength, pOutParam );
    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &ParamLength );


    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
    {
        /* Initialising the Buffer */
        slmemset((char *)pString, 0, stringSize);
        /* Copy the obained values to the function OUT parameters */
        response.IMSISize            = stringSize;
        response.pIMSI               = pString;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiDmsUIMGetIMSI( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG GetSerialNumbers(
   BYTE     esnSize,
   CHAR    *pESNString,
   BYTE     imeiSize,
   CHAR    *pIMEIString,
   BYTE     meidSize,
   CHAR    *pMEIDString )
{
    ULONG               resultcode;  /* Result of SwiQmiSendnWait() */
    BYTE                *pInParam;   /* ptr to param field rx'd from modem */
    BYTE                *pOutParam;  /* ptr to outbound param field */
    BYTE                *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT              ParamLength; /* Ret'd length of the QMI Parm field */

    /* Storage for results and response variable */
    struct QmiDmsGetDeviceSerialNumbersResp response;

    /* Device string is OUT parameter and hence should be NULL */
    if ( !pESNString || !pIMEIString || !pMEIDString )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Index points to first QMI header. */
    ParamLength = 0;

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiDmsGetDeviceSerialNumbers( &ParamLength, pOutParam );
    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &ParamLength );


    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
    {
        /* Initialising the Buffer */
        slmemset((char *)pESNString, 0, esnSize);
        slmemset((char *)pIMEIString, 0, imeiSize);
        slmemset((char *)pMEIDString, 0, meidSize);
        /* Copy the obained values to the function OUT parameters */
        response.esnSize     = esnSize;
        response.imeiSize    = imeiSize;
        response.meidSize    = meidSize;
        response.pESNString  = pESNString;
        response.pIMEIString = pIMEIString;
        response.pMEIDString = pMEIDString;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiDmsGetDeviceSerialNumbers( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG GetHardwareRevision(
    BYTE   stringSize,
    CHAR * pString )
{
    ULONG               resultcode;  /* Result of SwiQmiSendnWait() */
    BYTE                *pInParam;   /* ptr to param field rx'd from modem */
    BYTE                *pOutParam;  /* ptr to outbound param field */
    BYTE                *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT              ParamLength; /* Ret'd length of the QMI Parm field */

    /* Storage for results and response variable */
    struct QmiDmsGetDeviceHardwareRevResp response;

    /* Device string is OUT parameter and hence should be NULL */
    if ( !pString )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Index points to first QMI header. */
    ParamLength = 0;

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiDmsGetDeviceHardwareRev( &ParamLength, pOutParam );
    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &ParamLength );


    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
    {
        /* Initialising the Buffer */
        slmemset((char *)pString, 0, stringSize);
        /* Copy the obained values to the function OUT parameters */
        response.pString    = pString;
        response.stringSize = stringSize;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiDmsGetDeviceHardwareRev( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG GetNetworkTime(
    ULONGLONG *pTimeStamp,
    ULONG     *pTimeSource )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */
    WORD   first2bytes;  /* To extract Timesource from Device Time */

    /* Storage for results and response variable */
    struct QmiDmsGetTimeResp response;

    /* pTimeStamp & pTimeSource are OUT parameters & hence should not be NULL */
    if ( !pTimeStamp || !pTimeSource )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiDmsGetTime( &paramLength, pOutParam );

    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode; /* No memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Copy the obtained values to the function OUT parameters */
        response.pDeviceTime = pTimeStamp;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiDmsGetTime( pInParam, &response );

        if ( resultCode == eQCWWAN_ERR_NONE )
        {
            /* Length of pTimeStamp is 6 bytes
             * And length of pTimeSource is 2 bytes
             * These two need to be extracted from Device Time
             */
            first2bytes  = *(response.pDeviceTime) >> QMI_DMS_SIX_BYTES_SHIFT;
            *pTimeSource = (ULONG)first2bytes;
            *pTimeStamp  = *(response.pDeviceTime) << QMI_DMS_TWO_BYTES_SHIFT;
            *pTimeStamp  = *(pTimeStamp) >> QMI_DMS_TWO_BYTES_SHIFT;
        }
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG UIMSetPINProtection(
    ULONG id,
    ULONG bEnable,
    CHAR  *pValue,
    ULONG *pVerifyRetriesLeft,
    ULONG *pUnblockRetriesLeft )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiDmsUIMSetPINProtectionResp response;

    /* pVerifyRetriesLeft & pUnblockRetriesLeft are OUT parameters
     * and hence should not be NULL
     */
    if ( !pVerifyRetriesLeft || !pUnblockRetriesLeft )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiDmsUIMSetPINProtection( &paramLength,
                                              pOutParam,
                                              id,
                                              bEnable,
                                              pValue );

    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode; /* No memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  paramLength,
                                  eQMI_TIMEOUT_5_S, /* 5 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Initialization to default value */
        *pVerifyRetriesLeft  = 0xFFFFFFFF;
        *pUnblockRetriesLeft = 0xFFFFFFFF;

        /* Copy the obtained values to the function OUT parameters */
        response.pVerifyRetriesLeft  = pVerifyRetriesLeft;
        response.pUnblockRetriesLeft = pUnblockRetriesLeft;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiDmsUIMSetPINProtection( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG UIMUnblockPIN(
    ULONG id,
    CHAR  *pPUKValue,
    CHAR  *pNewValue,
    ULONG *pVerifyRetriesLeft,
    ULONG *pUnblockRetriesLeft )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiDmsUIMUnblockPINResp response;

    /* pVerifyRetriesLeft & pUnblockRetriesLeft are OUT parameters
     * and hence should not be NULL
     */
    if ( !pVerifyRetriesLeft || !pUnblockRetriesLeft )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiDmsUIMUnblockPIN( &paramLength,
                                        pOutParam,
                                        id,
                                        pPUKValue,
                                        pNewValue );

    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode; /* No memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  paramLength,
                                  eQMI_TIMEOUT_5_S, /* 5 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Initialization to default value */
        *pVerifyRetriesLeft  = 0xFFFFFFFF;
        *pUnblockRetriesLeft = 0xFFFFFFFF;

        /* Copy the obtained values to the function OUT parameters */
        response.pVerifyRetriesLeft  = pVerifyRetriesLeft;
        response.pUnblockRetriesLeft = pUnblockRetriesLeft;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiDmsUIMUnblockPIN( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG UIMVerifyPIN(
    ULONG id,
    CHAR  *pValue,
    ULONG *pVerifyRetriesLeft,
    ULONG *pUnblockRetriesLeft )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiDmsUIMVerifyPINResp response;

    /* pVerifyRetriesLeft & pUnblockRetriesLeft are OUT parameters
     * and hence should not be NULL
     */
    if ( !pVerifyRetriesLeft || !pUnblockRetriesLeft )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiDmsUIMVerifyPIN( &paramLength,
                                       pOutParam,
                                       id,
                                       pValue );

    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode; /* No memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  paramLength,
                                  eQMI_TIMEOUT_5_S, /* 5 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Initialization to default value */
        *pVerifyRetriesLeft  = 0xFFFFFFFF;
        *pUnblockRetriesLeft = 0xFFFFFFFF;

        /* Copy the obtained values to the function OUT parameters */
        response.pVerifyRetriesLeft  = pVerifyRetriesLeft;
        response.pUnblockRetriesLeft = pUnblockRetriesLeft;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiDmsUIMVerifyPIN( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG UIMChangePIN(
    ULONG id,
    CHAR  *pOldValue,
    CHAR  *pNewValue,
    ULONG *pVerifyRetriesLeft,
    ULONG *pUnblockRetriesLeft )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiDmsUIMChangePINResp response;

    /* pVerifyRetriesLeft & pUnblockRetriesLeft are OUT parameters
     * and hence should not be NULL
     */
    if ( !pVerifyRetriesLeft || !pUnblockRetriesLeft )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiDmsUIMChangePIN( &paramLength,
                                       pOutParam,
                                       id,
                                       pOldValue,
                                       pNewValue );

    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode; /* No memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  paramLength,
                                  eQMI_TIMEOUT_5_S, /* 5 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Initialization to default value */
        *pVerifyRetriesLeft  = 0xFFFFFFFF;
        *pUnblockRetriesLeft = 0xFFFFFFFF;

        /* Copy the obtained values to the function OUT parameters */
        response.pVerifyRetriesLeft  = pVerifyRetriesLeft;
        response.pUnblockRetriesLeft = pUnblockRetriesLeft;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiDmsUIMChangePIN( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG GetVoiceNumber(
    BYTE voiceNumberSize,
    CHAR *pVoiceNumber,
    BYTE minSize,
    CHAR *pMIN )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiDmsGetMSISDNResp response;

    /* pVoiceNumber & pVoiceNumber are OUT parameters
     * and hence should not be NULL
     */
    if ( !pVoiceNumber || !pMIN )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiDmsGetMSISDN( &paramLength,
                                              pOutParam );

    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode; /* No memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Initialising the Buffer */
        slmemset((char *)pVoiceNumber, 0, voiceNumberSize);
        slmemset((char *)pMIN,         0, minSize);

        /* Copy the obtained values to the function OUT parameters */
        response.voiceNumberSize  = voiceNumberSize;
        response.pVoiceNumber     = pVoiceNumber;
        response.minSize          = minSize;
        response.pMIN             = pMIN;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiDmsGetMSISDN( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SetPower(
    ULONG powerMode )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiDmsSetOperatingModeResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiDmsSetOperatingMode( &paramLength,
                                           pOutParam,
                                           powerMode );

    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode; /* No memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  paramLength,
                                  eQMI_TIMEOUT_5_S, /* 5 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiDmsSetOperatingMode( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG GetPower(
    ULONG *pPowerMode )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiDmsGetOperatingModeResp response;

    /* pPowerMode is an OUT parameter and hence should not be NULL */
    if ( !pPowerMode )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs OMI message and sends the message. */
    resultCode = PkQmiDmsGetOperatingMode( &paramLength, pOutParam );

    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode; /* No memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Copy the obtained values to the function OUT parameters */
        response.pOperatingMode = pPowerMode;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiDmsGetOperatingMode( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG UIMGetControlKeyStatus(
    ULONG id,
    ULONG *pStatus,
    ULONG *pVerifyRetriesLeft,
    ULONG *pUnblockRetriesLeft )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiDmsUIMGetCkStatusResp response;

    /* pStatus, pVerifyRetriesLeft & pUnblockRetriesLeft are OUT parameters
     * and hence should not be NULL
     */
    if ( !pStatus || !pVerifyRetriesLeft || !pUnblockRetriesLeft )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiDmsUIMGetCkStatus( &paramLength,
                                         pOutParam,
                                         id );

    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode; /* No memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  paramLength,
                                  eQMI_TIMEOUT_5_S, /* 5 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Initialization to default value */
        *pVerifyRetriesLeft  = 0xFFFFFFFF;
        *pUnblockRetriesLeft = 0xFFFFFFFF;

        /* Copy the obtained values to the function OUT parameters */
        response.pStatus             = pStatus;
        response.pVerifyRetriesLeft  = pVerifyRetriesLeft;
        response.pUnblockRetriesLeft = pUnblockRetriesLeft;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiDmsUIMGetCkStatus( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG UIMGetICCID(
    BYTE stringSize,
    CHAR *pString )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiDmsUIMGetICCIDResp response;

    /* pSting is an OUT parameters and hence should not be NULL */
    if ( !pString )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiDmsUIMGetICCID( &paramLength, pOutParam );

    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode; /* No memory */
    }
    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Initialization of Buffer */
        slmemset((CHAR *)pString, 0, stringSize);

        /* Pass the pointers for the OUT parameters */
        response.pString    = pString;
        response.stringSize = stringSize;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiDmsUIMGetICCID( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG UIMGetPINStatus(
    ULONG id,
    ULONG *pStatus,
    ULONG *pVerifyRetriesLeft,
    ULONG *pUnblockRetriesLeft )
{
    ULONG  resultCode;  /* Result of SwiIntSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiDmsUIMGetPINStatusResp response;

    /* Input Parameter Validation */
    if ( id != QMI_DMS_PIN_ONE && id != QMI_DMS_PIN_TWO )
        return eQCWWAN_ERR_INVALID_ARG;
    else
    {
        /* This shall help to identify which PIN status needs to be fetched */
        response.id = id;
    }

    /* pStatus, pVerifyRetriesLeft & pUnblockRetriesLeft are OUT parameters
     * and hence should not be NULL
     */
    if ( !pStatus || !pVerifyRetriesLeft || !pUnblockRetriesLeft )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiDmsUIMGetPINStatus( &paramLength,
                                          pOutParam );

    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode; /* No memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  paramLength,
                                  eQMI_TIMEOUT_5_S, /* 5 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Initialization to default value */
        *pVerifyRetriesLeft  = 0xFFFFFFFF;
        *pUnblockRetriesLeft = 0xFFFFFFFF;

        /* Copy the obtained values to the function OUT parameters */
        response.pStatus             = pStatus;
        response.pVerifyRetriesLeft  = pVerifyRetriesLeft;
        response.pUnblockRetriesLeft = pUnblockRetriesLeft;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiDmsUIMGetPINStatus( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG GetOfflineReason(
    ULONG *pReasonMask,
    ULONG *pbPlatform )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */


    /* Storage for results and response variable */
    struct QmiDmsGetOperatingModeResp response;

    /*
     * OfflineReason and HardwareRestriction are OUT parameters and hence
     * should not be NULL
     */
    if ( !pReasonMask || !pbPlatform )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs OMI message */
    resultCode = PkQmiDmsGetOperatingMode( &paramLength, pOutParam );

    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode; /* No memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Copy the obtained values to the function OUT parameters */
        response.pOperatingMode          = NULL;
        response.pOfflineReason          = pReasonMask;
        response.pHardwareControlledMode = pbPlatform;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiDmsGetOperatingMode( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG UIMSetControlKeyProtection(
    ULONG id,
    ULONG status,
    CHAR  *pValue,
    ULONG *pVerifyRetriesLeft )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiDmsUIMSetCkProtectionResp response;

    /* pVerifyRetriesLeft is an OUT parameter and hence should not be NULL */
    if ( !pVerifyRetriesLeft )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiDmsUIMSetCkProtection( &paramLength,
                                             pOutParam,
                                             id,
                                             status,
                                             pValue );

    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode; /* No memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  paramLength,
                                  eQMI_TIMEOUT_5_S, /* 5 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Initialization to default value */
        *pVerifyRetriesLeft  = 0xFFFFFFFF;

        /* Copy the obtained values to the function OUT parameters */
        response.pVerifyRetriesLeft = pVerifyRetriesLeft;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiDmsUIMSetCkProtection( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG UIMUnblockControlKey(
    ULONG id,
    CHAR  *pValue,
    ULONG *pUnblockRetriesLeft )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiDmsUIMUnblockCkResp response;

    /* pUnblockRetriesLeft is an OUT parameter and hence should not be NULL */
    if ( !pUnblockRetriesLeft )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiDmsUIMUnblockCk( &paramLength,
                                       pOutParam,
                                       id,
                                       pValue );

    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode; /* No memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  paramLength,
                                  eQMI_TIMEOUT_5_S, /* 5 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Initialization to default value */
        *pUnblockRetriesLeft  = 0xFFFFFFFF;

        /* Copy the obtained values to the function OUT parameters */
        response.pUnblockRetriesLeft = pUnblockRetriesLeft;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiDmsUIMUnblockCk( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG GetDeviceCapabilities(
    ULONG *pMaxTXChannelRate,
    ULONG *pMaxRXChannelRate,
    ULONG *pDataServiceCapability,
    ULONG *pSimCapability,
    ULONG *pRadioIfacesSize,
    BYTE  *pRadioIfaces )
{
    ULONG  resultcode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT ParamLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiDmsGetDeviceCapResp response;

    /* OUT parameter should not be NULL  */
    if ( !pMaxTXChannelRate      ||
         !pMaxRXChannelRate      ||
         !pDataServiceCapability ||
         !pSimCapability         ||
         !pRadioIfacesSize       ||
         !pRadioIfaces )
    {
         return eQCWWAN_ERR_INVALID_ARG;
    }
    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiDmsGetDeviceCap( &ParamLength, pOutParam );

    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultcode == eQCWWAN_ERR_NONE )
    {
        /* Initialize the Array */
        slmemset( (char *) pRadioIfaces,
                  0,
                  *pRadioIfacesSize );

        /* Set the pointers for the function OUT parameters */
        response.pMaxTXChannelRate      = pMaxTXChannelRate;
        response.pMaxRXChannelRate      = pMaxRXChannelRate;
        response.pDataServiceCapability = pDataServiceCapability;
        response.pSimCapability         = pSimCapability;
        response.pRadioIfacesSize       = pRadioIfacesSize;
        response.pRadioIfaces           = pRadioIfaces;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiDmsGetDeviceCap( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG ResetToFactoryDefaults(
    CHAR *pSPC )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiDmsRestoreFactoryDefaultsResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultCode = PkQmiDmsRestoreFactoryDefaults( &paramLength,
                                                 pOutParam,
                                                 pSPC );

    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode; /* No memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  paramLength,
                                  eQMI_TIMEOUT_300_S, /* 5 minutes */
                                  &pInParam,
                                  &paramLength );


    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiDmsRestoreFactoryDefaults( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG ValidateSPC(
    CHAR *pSPC )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiDmsValSerProgramCodeResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultCode = PkQmiDmsValSerProgramCode( &paramLength,
                                            pOutParam,
                                            pSPC );
    /* No memory */
    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode;
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    /* Copy to the caller's buffer */
    if ( resultCode == eQCWWAN_ERR_NONE )
        resultCode = UpkQmiDmsValSerProgramCode( pInParam, &response );

    qmrelreqbkp();
    return resultCode;
}

/* APIs Not Implemented */
ULONG GetERIFile(
    ULONG *pFileSize,
    BYTE  *pFile )
{
    UNUSEDPARAM( pFileSize );
    UNUSEDPARAM( pFile );

    return eQCWWAN_ERR_SWICM_NOT_IMPLEMENTED;
}

ULONG ActivateAutomatic(
    CHAR *pActivationCode )
{
    ULONG  resultCode;  /* Result of SwiIntSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiDmsActivateAutomaticResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message. */
    resultCode = PkQmiDmsActivateAutomatic( &paramLength,
                                            pOutParam,
                                            pActivationCode );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  paramLength,
                                  eQMI_TIMEOUT_300_S, /* 5 Minutes */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiDmsActivateAutomatic( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG GetActivationState(
    ULONG *pActivationState)
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiDmsGetActivationStateResp response;

    /* pActivationState is an OUT parameter and hence should not be NULL.*/
    if ( !pActivationState )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultCode = PkQmiDmsGetActivationState( &paramLength, pOutParam );

    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode; /* No memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Copy the obtained values to the function OUT parameters */
        response.pActivationState = pActivationState;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiDmsGetActivationState( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG ActivateManual(
    CHAR  *pSPC,
    WORD  sid,
    CHAR  *pMDN,
    CHAR  *pMIN,
    ULONG prlSize,
    BYTE  *pPRL,
    CHAR  *pMNHA,
    CHAR  *pMNAAA )
{
    UNUSEDPARAM( pSPC );
    UNUSEDPARAM( sid );
    UNUSEDPARAM( pMDN );
    UNUSEDPARAM( pMIN );
    UNUSEDPARAM( prlSize );
    UNUSEDPARAM( pPRL );
    UNUSEDPARAM( pMNHA );
    UNUSEDPARAM( pMNAAA );

    return eQCWWAN_ERR_SWICM_NOT_IMPLEMENTED;
}

ULONG SLQSUIMGetState(
    ULONG *pUIMState)
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiDmsUIMGetStateResp response;

    /* pUIMState is an OUT parameter and hence should not be NULL.*/
    if ( !pUIMState )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultCode = PkQmiDmsUIMGetState( &paramLength, pOutParam );

    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode; /* No memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Copy the obtained values to the function OUT parameters */
        response.pUIMState = pUIMState;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiDmsUIMGetState( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSGetBandCapability( ULONGLONG *pBandCapability )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiDmsGetBandCapabilityResp response;

    /* pBandCapability is an OUT parameter and hence should not be NULL.*/
    if ( !pBandCapability )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultCode = PkQmiDmsGetBandCapability( &paramLength, pOutParam );

    if ( resultCode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultCode; /* No memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultCode == eQCWWAN_ERR_NONE )
    {
        /* Copy the obtained values to the function OUT parameters */
        response.pBandCapability = pBandCapability;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiDmsGetBandCapability( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSGetCustFeatures(
    custFeaturesInfo *pCustFeaturesInfo )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiDmsGetCustFeaturesResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pCustFeaturesInfo )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiDmsGetCustFeatures( &paramLength,
                                          pOutParam );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Set the pointers for the function OUT parameters */
        response.pCustFeaturesInfo = pCustFeaturesInfo;

        pCustFeaturesInfo->GpsEnable             = -1;
        if( pCustFeaturesInfo->pDisableIMSI )
            *(pCustFeaturesInfo->pDisableIMSI)   = 0xFF;
        if( pCustFeaturesInfo->pIPFamSupport )
            *(pCustFeaturesInfo->pIPFamSupport)  = 0xFFFF;
        if( pCustFeaturesInfo->pRMAutoConnect )
            *(pCustFeaturesInfo->pRMAutoConnect) = 0xFF;
        if( pCustFeaturesInfo->pGPSSel )
            *(pCustFeaturesInfo->pGPSSel)        = 0xFF;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiDmsGetCustFeatures( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSSetCustFeatures(
    custFeaturesSetting *pCustFeaturesSetting )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiDmsSetCustFeaturesResp response;

    /* Checking for Invalid Parameter */
    if ( NULL == pCustFeaturesSetting )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiDmsSetCustFeatures( &paramLength,
                                          pOutParam,
                                          pCustFeaturesSetting );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_DMS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiDmsSetCustFeatures( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSGetCurrentPRLInfo( dmsCurrentPRLInfo *pCurrentPRLInfo )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiDMSSLQSGetCurrentPRLInfoResp response;

    /* Checking for Invalid Parameter */
    if ( !pCurrentPRLInfo )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiDmsSLQSGetCurrentPRLInfo( &paramLength, pOutParam );

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
        /* Copy the obtained values to the function OUT parameters */
        response.pCurrentPRLInfo = pCurrentPRLInfo;
        if( pCurrentPRLInfo->pPRLVersion )
        {
            *(pCurrentPRLInfo->pPRLVersion) = 0xFFFF;
        }
        if( pCurrentPRLInfo->pPRLPreference )
        {
            *(pCurrentPRLInfo->pPRLPreference) = 0xFF;
        }
        resultCode = UpkQmiDmsSLQSGetCurrentPRLInfo( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}
