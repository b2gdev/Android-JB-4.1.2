/*
 * \ingroup nas
 *
 * \file qaGobiApiNas.c
 *
 * \brief  Entry points for Gobi APIs for the Network Access Service ( NAS )
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
#include "qaGobiApiNas.h"
#include "qaNasGetSignalStrength.h"
#include "qaNasPerformNetworkScan.h"
#include "qaNasInitiateNetworkRegistration.h"
#include "qaNasGetServingSystem.h"
#include "qaNasGetHomeNetwork.h"
#include "qaNasSetTechPreference.h"
#include "qaNasGetTechnologyPreference.h"
#include "qaNasGetRFBandInfo.h"
#include "qaNasInitiateAttach.h"
#include "qaNasGetACCOLC.h"
#include "qaNasSetACCOLC.h"
#include "qaNasSetDeviceConfig.h"
#include "qaNasGetDeviceConfig.h"
#include "qaNasGetAnAAAStatus.h"
#include "qaNasGetSLQSServingSystem.h"
#include "qaNasSetSysSelectPreference.h"
#include "qaNasIndicationRegister.h"
#include "qaNasSLQSGetSignalStrength.h"
#include "qaNasSLQSPerformNetworkScan.h"
#include "qaNasGetSysSelectPreference.h"
#include "qaNasSLQSNasGetSysInfo.h"
#include "qaNasSLQSNasSwiModemStatus.h"
#include "qaNasSLQSGetHDRColorCode.h"
#include "qaNasSLQSGetTxRxInfo.h"
#include "qaNasSLQSGetSigInfo.h"
#include "qaNasSLQSGetPLMNName.h"
#include "qaNasGetOperatorNameData.h"
#include "qaNasSLQSGet3GPP2Subscription.h"
#include "qaNasSLQSNasGetCellLocationInfo.h"
#include "qaNasSLQSInitiateNetworkRegistration.h"

#define DEFAULTBYTEVALUE   0xFF
#define DEFAULTWORDVALUE   0xFFFF
#define DEFAULTLONGVALUE   0xFFFFFFFF

ULONG GetSignalStrength(
    INT8  *pSignalStrength,
    ULONG *pRadioInterface )
{
    ULONG resultcode; /* Result of GetSignalStrengths() */
    ULONG ArraySize = 1;

    /* Prepare and send the blocking call */
    /* Make the array size as 1 to invoke GetSignalStrengths */
    resultcode = GetSignalStrengths( &ArraySize,
                                     pSignalStrength,
                                     pRadioInterface );
    return resultcode;
}

ULONG GetSignalStrengths(
   ULONG *pArraySizes,
   INT8  *pSignalStrengths,
   ULONG *pRadioInterfaces )
{
    ULONG  resultcode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to ougoing request buffer */
    USHORT ParamLength; /* Ret'd length of the Param field */

    /* Storage for results and response variable */
    struct QmiNasGetSignalStrengthResp response;

    /* Singal Strength and Radio Interfaces are OUT parameters
     * pArraySizes should hold the size of the array
     */
    if ( !pSignalStrengths || !pRadioInterfaces || !pArraySizes )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiNasGetSignalStrengths( &ParamLength,
                                             pOutParam );
    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S,
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
    {
        /* Initialise the Arrays */
        slmemset( (char *) pRadioInterfaces, 0, *pArraySizes );
        slmemset( (char *) pSignalStrengths, 0, *pArraySizes );

        /* Copy the obained values to the function OUT parameters */
        response.pArraySizes        = pArraySizes;
        response.pSignalStrengths   = pSignalStrengths;
        response.pRadioInterfaces   = pRadioInterfaces;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiNasGetSignalStrengths(pInParam, &response);
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG PerformNetworkScan(
    BYTE *pInstanceSize,
    BYTE *pInstances )
{
    ULONG  resultcode;   /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;    /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;   /* ptr to outbound param field */
    BYTE   *pReqBuf;     /* Pointer to ougoing request buffer */
    USHORT ParamLength;  /* Ret'd length of the Param field */

    /* Storage for results and response variable */
    struct QmiNasPerformNetworkScanResp response;

    /* pInstances is an OUT parameter and hence should be NULL */
    /* pInstanceSize should not be NULL */
    if ( !pInstances || !pInstanceSize )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiNasPerformNetworkScan( &ParamLength,
                                             pOutParam );
    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  ParamLength,
                                  eQMI_TIMEOUT_300_S,
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
    {
        /* Initialise the Structure */
        slmemset( (char *) pInstances,
                  0,
                  ((sizeof(struct QmiNas3GppNetworkInfo))*(*pInstanceSize)));

        /* Set the pointers for the function OUT parameters */
        response.pInstanceSize = pInstanceSize;
        response.pInstances    = (struct QmiNas3GppNetworkInfo *) pInstances;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiNasPerformNetworkScan(pInParam, &response);
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG InitiateNetworkRegistration(
   ULONG regType,
   WORD  mcc,
   WORD  mnc,
   ULONG rat )
{
    ULONG  resultcode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to ougoing request buffer */
    USHORT ParamLength; /* Ret'd length of the Param field */

    /* Storage for results and response variable */
    struct QmiNasInitiateNetworkRegistrationResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiNasInitiateNetworkRegistration ( &ParamLength,
                                                       pOutParam,
                                                       regType,
                                                       mcc,
                                                       mnc,
                                                       rat );
    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  ParamLength,
                                  eQMI_TIMEOUT_30_S,
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
        /* Copy to the caller's buffer */
        resultcode = UpkQmiNasInitiateNetworkRegistration (pInParam, &response);

    qmrelreqbkp();
    return resultcode;
}

ULONG GetServingNetwork(
    ULONG *pRegistrationState,
    ULONG *pCSDomain,
    ULONG *pPSDomain,
    ULONG *pRAN,
    BYTE  *pRadioIfacesSize,
    BYTE  *pRadioIfaces,
    ULONG *pRoaming,
    WORD  *pMCC,
    WORD  *pMNC,
    BYTE  nameSize,
    CHAR  *pName )
{
    ULONG  resultcode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to ougoing request buffer */
    USHORT ParamLength; /* Ret'd length of the Param field */

    /* Storage for results and response variable */
    struct QmiNasGetServingSystemResp response;

    /* Checking for Parameter ERROR */
    if ( !pRegistrationState ||
         !pCSDomain          ||
         !pPSDomain          ||
         !pRAN               ||
         !pRadioIfaces       ||
         !pRoaming           ||
         !pMCC               ||
         !pMNC               ||
         !pName )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiNasGetServingSystem( &ParamLength, pOutParam );

    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S,
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
    {
        /* Initialize the Arrays */
        if ( pRadioIfacesSize )
            slmemset( (char *) pRadioIfaces, 0, *pRadioIfacesSize );

        /* Initialize the Arrays */
        slmemset( (char *) pName, 0, nameSize );

        /* Fill All the OUT parameters */
        response.pRegistrationState = pRegistrationState;
        response.pCSDomain          = pCSDomain;
        response.pPSDomain          = pPSDomain;
        response.pRAN               = pRAN;
        response.pRadioIfacesSize   = pRadioIfacesSize;
        response.pRadioIfaces       = pRadioIfaces;
        response.pRoaming           = pRoaming;
        response.pMCC               = pMCC;
        response.pMNC               = pMNC;
        response.pName              = pName;
        response.nameSize           = nameSize;
        response.pDataCapsLen       = NULL;
        response.pDataCaps          = NULL;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiNasGetServingSystem( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG GetHomeNetwork(
    WORD *pMCC,
    WORD *pMNC,
    BYTE nameSize,
    CHAR *pName,
    WORD *pSID,
    WORD *pNID )
{
    ULONG  resultcode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to ougoing request buffer */
    USHORT ParamLength; /* Ret'd length of the Param field */

    /* Storage for results and response variable */
    struct QmiNasGetHomeNetworkResp response;

    /* Checking for Parameter ERROR */
    if ( !pMCC  ||
         !pMNC  ||
         !pName ||
         !pSID  ||
         !pNID )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiNasGetHomeNetwork ( &ParamLength,
                                          pOutParam );
    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S,
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
    {
        /* Initialise the Arrays */
        slmemset( (char *) pName,
                  0,
                  nameSize );
        /* Fill All the OUT parameters */
        response.pMCC     = pMCC;
        response.pMNC     = pMNC;
        response.pName    = pName;
        response.nameSize = nameSize;
        response.pSID     = pSID;
        response.pNID     = pNID;
        /* Initialise the SID and NID */
        *(response.pSID)  = 0;
        *(response.pNID)  = 0;
        /* Copy to the caller's buffer */
        resultcode = UpkQmiNasGetHomeNetwork (pInParam, &response);
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG GetServingNetworkCapabilities(
    BYTE *pDataCapsSize,
    BYTE *pDataCaps )
{
    ULONG  resultcode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT ParamLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiNasGetServingSystemResp response;

    /* Checking for Parameter ERROR */
    if ( !pDataCaps)
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiNasGetServingSystem( &ParamLength, pOutParam );

    if ( resultcode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S,
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultcode == eQCWWAN_ERR_NONE )
    {
        /* Initialize the Array */
        if ( pDataCapsSize )
            slmemset((char *) pDataCaps, 0, *pDataCapsSize);

        /* Fill All the OUT parameters */
        response.pDataCapsLen       = pDataCapsSize;
        response.pDataCaps          = (ULONG *)pDataCaps;
        response.pRegistrationState = NULL;
        response.pCSDomain          = NULL;
        response.pPSDomain          = NULL;
        response.pRAN               = NULL;
        response.pRadioIfacesSize   = NULL;
        response.pRadioIfaces       = NULL;
        response.pRoaming           = NULL;
        response.pMCC               = NULL;
        response.pMNC               = NULL;
        response.pName              = NULL;
        response.nameSize           = 0;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiNasGetServingSystem( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG SetNetworkPreference(
    ULONG technologyPref,
    ULONG duration )
{
    ULONG  resultcode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT ParamLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiNasSetTechPrefResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiNasSetTechPref( &ParamLength,
                                      pOutParam,
                                      technologyPref,
                                      duration );
    if ( resultcode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S,
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultcode == eQCWWAN_ERR_NONE )
    {
        /* Copy to the caller's buffer */
        resultcode = UpkQmiNasSetTechPref( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG GetNetworkPreference(
    ULONG *pTechnologyPref,
    ULONG *pDuration,
    ULONG *pPersistentTechnologyPref )
{
    ULONG  resultcode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT ParamLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiNasGetTechPrefResp response;

    /* Technology Preference, Duration and Persistent Technology Preference
     * are OUT parameters and hence should not be NULL
     */
    if ( !pTechnologyPref ||
         !pDuration       ||
         !pPersistentTechnologyPref )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiNasGetTechPref( &ParamLength, pOutParam );

    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S,
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
    {
        /* Copy the obtained values to the function OUT parameters */
        response.pActiveTechPref     = pTechnologyPref;
        response.pDuration           = pDuration;
        response.pPersistentTechPref = pPersistentTechnologyPref;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiNasGetTechPref( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG GetRFInfo(
    BYTE *pInstancesSize,
    BYTE *pInstances)
{
    ULONG  resultcode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT ParamLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiNasGetRFBandInfoResp response;

    /* pInstances is an OUT parameter and hence should be NULL */
    if ( !pInstances )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiNasGetRFBandInfo( &ParamLength, pOutParam );

    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S,/*2 Seconds */
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultcode == eQCWWAN_ERR_NONE )
    {
        /* Initialize the Structure */
        if ( pInstancesSize )
            slmemset( (char *) pInstances, 0, *pInstancesSize );

        /* Set the pointers for the function OUT parameters */
        response.pInstancesSize = pInstancesSize;
        response.pInstances     = (struct RFBandInfoElements *) pInstances;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiNasGetRFBandInfo( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG InitiateDomainAttach(
    ULONG action )
{
    ULONG  resultcode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT ParamLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiNasInitiateAttachResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiNasInitiateAttach( &ParamLength,
                                         pOutParam,
                                         action );
    if ( resultcode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  ParamLength,
                                  eQMI_TIMEOUT_30_S,/* 30 Seconds */
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultcode == eQCWWAN_ERR_NONE )
    {
        /* Copy to the caller's buffer */
        resultcode = UpkQmiNasInitiateAttach( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG GetACCOLC(
    BYTE *pACCOLC )
{
    ULONG  resultcode;  /* Result of SwiIntSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT ParamLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiNasGetACCOLCResp response;

    /* Checking for Parameter ERROR */
    if ( !pACCOLC )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiNasGetACCOLC( &ParamLength, pOutParam );
    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S,
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultcode == eQCWWAN_ERR_NONE )
    {
        /* Fill All the OUT parameters */
        response.pACCOLC = pACCOLC;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiNasGetACCOLC( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG SetACCOLC(
    CHAR *spc,
    BYTE accolc )
{
    ULONG  resultcode;  /* Result of SwiIntSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT ParamLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiNasSetACCOLCResp response;

    /* Checking for Parameter ERROR */
    if ( !spc  )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiNasSetACCOLC( &ParamLength,
                                    pOutParam,
                                    spc,
                                    accolc );
    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  ParamLength,
                                  eQMI_TIMEOUT_5_S,
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultcode == eQCWWAN_ERR_NONE )
    {
        /* Copy to the caller's buffer */
        resultcode = UpkQmiNasSetACCOLC( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG SetCDMANetworkParameters(
    CHAR  *pSPC,
    BYTE  *pForceRev0,
    BYTE  *pCustomSCP,
    ULONG *pProtocol,
    ULONG *pBroadcast,
    ULONG *pApplication,
    ULONG *pRoaming)
{
    ULONG  resultcode;  /* Result of SwiIntSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT ParamLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiNasSetDeviceConfigResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiNasSetDeviceConfig( &ParamLength,
                                          pOutParam,
                                          pSPC,
                                          pForceRev0,
                                          pCustomSCP,
                                          pProtocol,
                                          pBroadcast,
                                          pApplication,
                                          pRoaming );
    if ( resultcode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  ParamLength,
                                  eQMI_TIMEOUT_5_S,
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we got a positive return */
    /* Copy to the caller's buffer */
    if ( resultcode == eQCWWAN_ERR_NONE )
        resultcode = UpkQmiNasSetDeviceConfig( pInParam, &response );

    qmrelreqbkp();
    return resultcode;
}

ULONG GetCDMANetworkParameters(
    BYTE  *pSCI,
    BYTE  *pSCM,
    BYTE  *pRegHomeSID,
    BYTE  *pRegForeignSID,
    BYTE  *pRegForeignNID,
    BYTE  *pForceRev0,
    BYTE  *pCustomSCP,
    ULONG *pProtocol,
    ULONG *pBroadcast,
    ULONG *pApplication,
    ULONG *pRoaming )
{
    ULONG  resultcode;  /* Result of SwiIntSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT ParamLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiNasGetDeviceConfigResp response;

    /* Checking for Parameter ERROR */
    if (!pSCI           ||
        !pSCM           ||
        !pRegHomeSID    ||
        !pRegForeignSID ||
        !pRegForeignNID ||
        !pForceRev0     ||
        !pCustomSCP     ||
        !pProtocol      ||
        !pBroadcast     ||
        !pApplication   ||
        !pRoaming)
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiNasGetDeviceConfig( &ParamLength, pOutParam );
    if ( resultcode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S,
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
    {
        /* Fill All the OUT parameters */
        response.pSCI           = pSCI;
        response.pSCM           = pSCM;
        response.pRegHomeSID    = pRegHomeSID;
        response.pRegForeignSID = pRegForeignSID;
        response.pRegForeignNID = pRegForeignNID;
        response.pForceRev0     = pForceRev0;
        response.pCustomSCP     = pCustomSCP;
        response.pProtocol      = pProtocol;
        response.pBroadcast     = pBroadcast;
        response.pApplication   = pApplication;
        response.pRoaming       = pRoaming;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiNasGetDeviceConfig( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG GetANAAAAuthenticationStatus(
    ULONG *pStatus )
{
    ULONG  resultcode;  /* Result of SwiIntSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT ParamLength; /* Ret'd length of the QMI Param field */

    if (!pStatus )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Storage for results and response variable */
    struct QmiNasGetAnAAAStatusResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiNasGetAnAAAStatus ( &ParamLength, pOutParam );
    if ( resultcode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S,/* 2 Seconds */
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if ( resultcode == eQCWWAN_ERR_NONE )
    {
        /* Fill All the OUT parameters */
        response.pStatus = pStatus;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiNasGetAnAAAStatus( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

local void InitServingSystemInfo( struct QmiNasGetSLQSServingSystemResp *response )
{
    BYTE   ctr = 0;     /* Counter for initialization for loop */

    /* Serving System */
    response->pServingSystem->regState        = ~0;
    response->pServingSystem->csAttachState   = ~0;
    response->pServingSystem->psAttachState   = ~0;
    response->pServingSystem->selNetwork      = ~0;
    response->pServingSystem->numRadioInterfaces = 0;
    for( ctr = 0; ctr < MAX_SERV_SYSTEM_RADIO_INTERFACES; ctr++ )
        response->pServingSystem->radioInterface[ctr] = ~0;

    /* Roaming Indicator value */
    *(response->pRoamIndicatorVal) = ~0;

    /* Data Services Capability */
    response->pDataSrvCapabilities->dataCapabilitiesLen = 0;
    for( ctr = 0; ctr < MAX_DATA_SRV_CAPABILITIES; ctr++ )
        response->pDataSrvCapabilities->dataCapabilities[ctr] = ~0;

    /* Current PLMN */
    response->pCurrentPLMN->MCC            = ~0;
    response->pCurrentPLMN->MNC            = ~0;
    response->pCurrentPLMN->netDescrLength = 0;
    for( ctr = 0; ctr < 255; ctr++ )
        response->pCurrentPLMN->netDescr[ctr] = ~0;

    /* CDMA System ID */
    *(response->pSystemID)                 = ~0;
    *(response->pNetworkID)                = ~0;

    /* CDMA Base Station Information */
    *(response->pBasestationID)            = ~0;
    *(response->pBasestationLatitude)      = ~0;
    *(response->pBasestationLongitude)     = ~0;

    /* Roaming Indicator List */
    response->pRoamingIndicatorList->numInstances   = 0;
    for( ctr = 0; ctr < MAX_SERV_SYSTEM_RADIO_INTERFACES; ctr++ )
    {
        response->pRoamingIndicatorList->radioInterface[ctr] = ~0;
        response->pRoamingIndicatorList->roamIndicator[ctr]  = ~0;
    }

    /* Default Roaming Indicator */
    *(response->pDefaultRoamInd) = ~0;

    /* 3GPP2 Time Zone */
    response->p3Gpp2TimeZone->leapSeconds     = ~0;
    response->p3Gpp2TimeZone->localTimeOffset = ~0;
    response->p3Gpp2TimeZone->daylightSavings = ~0;

    /* CDMA P_Rev */
    *(response->pCDMA_P_Rev) = ~0;

    /* 3GPP Time Zone */
    *(response->p3GppTimeZone)                = ~0;

    /* 3GPP Network Daylight Saving Adjustment */
    *(response->pGppNetworkDSTAdjustment)     = ~0;

    /* Location Area Code */
    *(response->pLac)    = ~0;

    /* Cell ID */
    *(response->pCellID) = ~0;

    /* Concurrent Service Info */
    *(response->pConcSvcInfo) = ~0;

    /* PRL Indicator */
    *(response->pPRLInd) = ~0;

    /* Dual Transfer Mode Indication */
    *(response->pDTMInd) = ~0;

    /* Detailed Service Information */
    response->pDetailedSvcInfo->srvStatus      = ~0;
    response->pDetailedSvcInfo->srvCapability  = ~0;
    response->pDetailedSvcInfo->hdrSrvStatus   = ~0;
    response->pDetailedSvcInfo->hdrHybrid      = ~0;
    response->pDetailedSvcInfo->isSysForbidden = ~0;

    /* CDMA System Info Ext. */
    response->pCDMASystemInfoExt->MCC        = ~0;
    response->pCDMASystemInfoExt->imsi_11_12 = ~0;

    /* HDR Personality */
    *(response->pHdrPersonality) = ~0;

    /* TAC information for LTE */
    *(response->pTrackAreaCode) = ~0;

    /* Call Barring Status */
    response->pCallBarStatus->csBarStatus = ~0;
    response->pCallBarStatus->psBarStatus = ~0;

}

ULONG SLQSGetServingSystem( qaQmiServingSystemParam *pServingSystem )
{
    ULONG  resultcode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to ougoing request buffer */
    USHORT paramLength; /* Ret'd length of the Param field */

    /* Storage for results and response variable */
    struct QmiNasGetSLQSServingSystemResp response;

    /* Checking for Parameter ERROR */
    if ( !pServingSystem )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiNasGetSLQSServingSystem( &paramLength, pOutParam );

    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S,
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we get a positive return */
    if (resultcode == eQCWWAN_ERR_NONE)
    {
        response.pServingSystem              = &pServingSystem->ServingSystem;
        response.pRoamIndicatorVal           = &pServingSystem->roamIndicatorVal;
        response.pDataSrvCapabilities        = &pServingSystem->DataSrvCapabilities;
        response.pCurrentPLMN                = &pServingSystem->CurrentPLMN;
        response.pSystemID                   = &pServingSystem->SystemID;
        response.pNetworkID                  = &pServingSystem->NetworkID;
        response.pBasestationID              = &pServingSystem->BasestationID;
        response.pBasestationLatitude        = &pServingSystem->BasestationLatitude;
        response.pBasestationLongitude       = &pServingSystem->BasestationLongitude;
        response.pRoamingIndicatorList       = &pServingSystem->RoamingIndicatorList;
        response.pDefaultRoamInd             = &pServingSystem->defaultRoamInd;
        response.p3Gpp2TimeZone              = &pServingSystem->Gpp2TimeZone;
        response.pCDMA_P_Rev                 = &pServingSystem->CDMA_P_Rev;
        response.p3GppTimeZone               = &pServingSystem->GppTimeZone;
        response.pGppNetworkDSTAdjustment    = &pServingSystem->GppNetworkDSTAdjustment;
        response.pLac                        = &pServingSystem->Lac;
        response.pCellID                     = &pServingSystem->CellID;
        response.pConcSvcInfo                = &pServingSystem->concSvcInfo;
        response.pPRLInd                     = &pServingSystem->PRLInd;
        response.pDTMInd                     = &pServingSystem->DTMInd;
        response.pDetailedSvcInfo            = &pServingSystem->DetailedSvcInfo;
        response.pCDMASystemInfoExt          = &pServingSystem->CDMASystemInfoExt;
        response.pHdrPersonality             = &pServingSystem->hdrPersonality;
        response.pTrackAreaCode              = &pServingSystem->trackAreaCode;
        response.pCallBarStatus              = &pServingSystem->CallBarStatus;

        /* Initialize the values */
        InitServingSystemInfo( &response );

        /* Copy to the caller's buffer */
        resultcode = UpkQmiNasGetSLQSServingSystem( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG SLQSSetBandPreference( ULONGLONG bandpreference )
{
    ULONG  resultcode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to ougoing request buffer */
    USHORT paramLength; /* Ret'd length of the Param field */

    /* Storage for results and response variable */
    struct QmiNasSetSysSelectPrefResp response;

    /* Storage for the band preference value to be packed */
    sysSelectPrefParams SysSelectPrefParams;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Copy the received band preference into the structure */
    slmemset( (char*) &SysSelectPrefParams, 0, sizeof( sysSelectPrefParams) );
    SysSelectPrefParams.pBandPref = &bandpreference;

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiNasSetSysSelectPref( &paramLength,
                                           pOutParam ,
                                           &SysSelectPrefParams );

    if ( resultcode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultcode; /* No memory */
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we get a positive return */
    if ( eQCWWAN_ERR_NONE == resultcode )
    {
        /* Copy to the caller's buffer */
        resultcode = UpkQmiNasSetSysSelectPref( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG SLQSNasIndicationRegister(
    BYTE systemSelectionInd,
    BYTE DDTMInd,
    BYTE servingSystemInd )
{
    nasIndicationRegisterReq req;
    memset( &req, 0, sizeof(req));
    req.pSystemSelectionInd = &systemSelectionInd;
    req.pDDTMInd            = &DDTMInd;
    req.pServingSystemInd   = &servingSystemInd;

    return SLQSNasIndicationRegisterExt( &req );
}

ULONG SLQSGetSignalStrength(
    struct slqsSignalStrengthInfo *pSignalInfo )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to ougoing request buffer */
    USHORT ParamLength; /* Ret'd length of the Param field */
    USHORT reqMask;     /* Local storage of Signal Strength Mask */

    /* Storage for results and response variable */
    struct QmiNasSlqsGetSignalStrengthResp response;

    /* pSignalInfo should not be NULL */
    if ( !pSignalInfo )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultCode = PkQmiNasSlqsGetSignalStrength( &ParamLength,
                                                pOutParam,
                                                pSignalInfo->
                                                signalStrengthReqMask );
    if (resultCode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultCode;
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S,
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if (resultCode == eQCWWAN_ERR_NONE)
    {
        /* Cache user request mask */
        reqMask = pSignalInfo->signalStrengthReqMask;

        /* Initialize the user structure */
        slmemset( (char *)( pSignalInfo),
                  0,
                  ( sizeof( struct slqsSignalStrengthInfo ) ) );

        /* Restore request mask */
        pSignalInfo->signalStrengthReqMask = reqMask;

        /* Copy to the caller's buffer */
        response.pSignalInfo = pSignalInfo;
        resultCode = UpkQmiNasSlqsGetSignalStrength( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSPerformNetworkScan(
    slqsNetworkScanInfo *pNetworkInfo )
{
    ULONG  resultCode;   /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;    /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;   /* ptr to outbound param field */
    BYTE   *pReqBuf;     /* Pointer to ougoing request buffer */
    USHORT ParamLength;  /* Ret'd length of the Param field */

    /* Storage for results and response variable */
    struct QmiNasSlqsPerformNetworkScanResp response;

    /* pNetworkInfo should not be NULL */
    if ( !pNetworkInfo )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultCode = PkQmiNasSlqsPerformNetworkScan( &ParamLength,
                                                 pOutParam );
    if (resultCode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultCode;
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  ParamLength,
                                  eQMI_TIMEOUT_300_S, /* 5 minutes*/
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if (resultCode == eQCWWAN_ERR_NONE)
    {
        /* Set the pointers for the function OUT parameters */
        response.pNetworkInfo =  pNetworkInfo;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiNasSlqsPerformNetworkScan( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSSetSysSelectionPref( sysSelectPrefParams *pSysSelectPrefParams )
{
    ULONG  resultcode;  /* Result code to be returned*/
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to ougoing request buffer */
    USHORT paramLength; /* Ret'd length of the Param field */

    /* Storage for results and response variable */
    struct QmiNasSetSysSelectPrefResp response;

    /* Check if all parameters are null */
    if( !pSysSelectPrefParams->pEmerMode         &&
        !pSysSelectPrefParams->pModePref         &&
        !pSysSelectPrefParams->pBandPref         &&
        !pSysSelectPrefParams->pPRLPref          &&
        !pSysSelectPrefParams->pRoamPref         &&
        !pSysSelectPrefParams->pLTEBandPref      &&
        !pSysSelectPrefParams->pNetSelPref       &&
        !pSysSelectPrefParams->pChgDuration      &&
        !pSysSelectPrefParams->pMNCIncPCSDigStat &&
        !pSysSelectPrefParams->pSrvDomainPref    &&
        !pSysSelectPrefParams->pGWAcqOrderPref )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiNasSetSysSelectPref( &paramLength,
                                           pOutParam ,
                                           pSysSelectPrefParams );

    if ( resultcode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultcode; /* No memory */
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we get a positive return */
    if ( eQCWWAN_ERR_NONE == resultcode )
    {
        /* Copy to the caller's buffer */
        resultcode = UpkQmiNasSetSysSelectPref( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG SLQSGetSysSelectionPref( sysSelectPrefInfo *pSysSelectPrefInfo )
{
    ULONG  resultcode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiNasGetSysSelectPrefResp response;

    /* Check against NULL for out params */
    if ( !pSysSelectPrefInfo )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Atleast one OUT param should not be NULL */
    if( !pSysSelectPrefInfo->pEmerMode      &&
        !pSysSelectPrefInfo->pModePref      &&
        !pSysSelectPrefInfo->pBandPref      &&
        !pSysSelectPrefInfo->pPRLPref       &&
        !pSysSelectPrefInfo->pRoamPref      &&
        !pSysSelectPrefInfo->pLTEBandPref   &&
        !pSysSelectPrefInfo->pNetSelPref    &&
        !pSysSelectPrefInfo->pSrvDomainPref &&
        !pSysSelectPrefInfo->pGWAcqOrderPref )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* pack QMI request */
    resultcode = PkQmiNasGetSysSelectPref( &paramLength, pOutParam );

    if ( eQCWWAN_ERR_NONE != resultcode )
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* send QMI request and receive the response */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &paramLength );

    if ( eQCWWAN_ERR_NONE == resultcode )
    {
        /* initialize response structure */
        response.pSysSelectPrefInfo = pSysSelectPrefInfo;
        if( pSysSelectPrefInfo->pEmerMode )
            *(pSysSelectPrefInfo->pEmerMode) = 0xFF;
        if( pSysSelectPrefInfo->pModePref )
            *(pSysSelectPrefInfo->pModePref) = 0xFF;
        if( pSysSelectPrefInfo->pBandPref )
            *(pSysSelectPrefInfo->pBandPref) = -1;
        if( pSysSelectPrefInfo->pPRLPref )
            *(pSysSelectPrefInfo->pPRLPref) = 0xFFFF;
        if( pSysSelectPrefInfo->pRoamPref )
            *(pSysSelectPrefInfo->pRoamPref) = 0xFFFF;
        if( pSysSelectPrefInfo->pLTEBandPref )
            *(pSysSelectPrefInfo->pLTEBandPref) = -1;
        if( pSysSelectPrefInfo->pNetSelPref )
            *(pSysSelectPrefInfo->pNetSelPref) = 0xFF;
        if( pSysSelectPrefInfo->pSrvDomainPref )
            *(pSysSelectPrefInfo->pSrvDomainPref) = 0xFFFFFFFF;
        if( pSysSelectPrefInfo->pGWAcqOrderPref )
            *(pSysSelectPrefInfo->pGWAcqOrderPref) = 0xFFFFFFFF;

        /* Copy to the caller's buffer */
        resultcode = UpkQmiNasGetSysSelectPref( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

local void InitialiseGetSysInfoParams( nasGetSysInfoResp *pGetSysInfoResp )
{
    /* Initializing the response parameters*/
    if ( pGetSysInfoResp->pCDMASrvStatusInfo )
    {
        pGetSysInfoResp->pCDMASrvStatusInfo->srvStatus      = 0xFF;
        pGetSysInfoResp->pCDMASrvStatusInfo->isPrefDataPath = 0xFF;
    }
    if ( pGetSysInfoResp->pHDRSrvStatusInfo )
    {
        pGetSysInfoResp->pHDRSrvStatusInfo->srvStatus      = 0xFF;
        pGetSysInfoResp->pHDRSrvStatusInfo->isPrefDataPath = 0xFF;
    }
    if ( pGetSysInfoResp->pGSMSrvStatusInfo )
    {
        pGetSysInfoResp->pGSMSrvStatusInfo->srvStatus      = 0xFF;
        pGetSysInfoResp->pGSMSrvStatusInfo->trueSrvStatus  = 0xFF;
        pGetSysInfoResp->pGSMSrvStatusInfo->isPrefDataPath = 0xFF;
    }
    if ( pGetSysInfoResp->pWCDMASrvStatusInfo )
    {
        pGetSysInfoResp->pWCDMASrvStatusInfo->srvStatus      = 0xFF;
        pGetSysInfoResp->pWCDMASrvStatusInfo->trueSrvStatus  = 0xFF;
        pGetSysInfoResp->pWCDMASrvStatusInfo->isPrefDataPath = 0xFF;
    }
    if ( pGetSysInfoResp->pLTESrvStatusInfo )
    {
        pGetSysInfoResp->pLTESrvStatusInfo->srvStatus      = 0xFF;
        pGetSysInfoResp->pLTESrvStatusInfo->trueSrvStatus  = 0xFF;
        pGetSysInfoResp->pLTESrvStatusInfo->isPrefDataPath = 0xFF;
    }
    if ( pGetSysInfoResp->pCDMASysInfo )
    {
        pGetSysInfoResp->pCDMASysInfo->sysInfoCDMA.srvDomainValid   = 0xFF;
        pGetSysInfoResp->pCDMASysInfo->sysInfoCDMA.srvDomain        = 0xFF;
        pGetSysInfoResp->pCDMASysInfo->sysInfoCDMA.srvCapabilityValid
                                                                    = 0xFF;
        pGetSysInfoResp->pCDMASysInfo->sysInfoCDMA.srvCapability    = 0xFF;
        pGetSysInfoResp->pCDMASysInfo->sysInfoCDMA.roamStatusValid  = 0xFF;
        pGetSysInfoResp->pCDMASysInfo->sysInfoCDMA.roamStatus       = 0xFF;
        pGetSysInfoResp->pCDMASysInfo->sysInfoCDMA.isSysForbiddenValid
                                                                    = 0xFF;
        pGetSysInfoResp->pCDMASysInfo->sysInfoCDMA.isSysForbidden   = 0xFF;
        pGetSysInfoResp->pCDMASysInfo->isSysPrlMatchValid  = 0xFF;
        pGetSysInfoResp->pCDMASysInfo->isSysPrlMatch       = 0xFF;
        pGetSysInfoResp->pCDMASysInfo->pRevInUseValid      = 0xFF;
        pGetSysInfoResp->pCDMASysInfo->pRevInUse           = 0xFF;
        pGetSysInfoResp->pCDMASysInfo->bsPRevValid         = 0xFF;
        pGetSysInfoResp->pCDMASysInfo->bsPRev              = 0xFF;
        pGetSysInfoResp->pCDMASysInfo->ccsSupportedValid   = 0xFF;
        pGetSysInfoResp->pCDMASysInfo->ccsSupported        = 0xFF;
        pGetSysInfoResp->pCDMASysInfo->cdmaSysIdValid      = 0xFF;
        pGetSysInfoResp->pCDMASysInfo->systemID            = 0xFFFF;
        pGetSysInfoResp->pCDMASysInfo->networkID           = 0xFFFF;
        pGetSysInfoResp->pCDMASysInfo->bsInfoValid         = 0xFF;
        pGetSysInfoResp->pCDMASysInfo->baseId              = 0xFFFF;
        pGetSysInfoResp->pCDMASysInfo->baseLat             = 0xFFFFFFFF;
        pGetSysInfoResp->pCDMASysInfo->baseLong            = 0xFFFFFFFF;
        pGetSysInfoResp->pCDMASysInfo->packetZoneValid     = 0xFF;
        pGetSysInfoResp->pCDMASysInfo->packetZone          = 0xFFFF;
        pGetSysInfoResp->pCDMASysInfo->networkIdValid      = 0xFF;
    }
    if ( pGetSysInfoResp->pHDRSysInfo )
    {
        pGetSysInfoResp->pHDRSysInfo->sysInfoHDR.srvDomainValid  = 0xFF;
        pGetSysInfoResp->pHDRSysInfo->sysInfoHDR.srvDomain       = 0xFF;
        pGetSysInfoResp->pHDRSysInfo->sysInfoHDR.srvCapabilityValid
                                                                 = 0xFF;
        pGetSysInfoResp->pHDRSysInfo->sysInfoHDR.srvCapability   = 0xFF;
        pGetSysInfoResp->pHDRSysInfo->sysInfoHDR.roamStatusValid = 0xFF;
        pGetSysInfoResp->pHDRSysInfo->sysInfoHDR.roamStatus      = 0xFF;
        pGetSysInfoResp->pHDRSysInfo->sysInfoHDR.isSysForbiddenValid
                                                                 = 0xFF;
        pGetSysInfoResp->pHDRSysInfo->sysInfoHDR.isSysForbidden  = 0xFF;
        pGetSysInfoResp->pHDRSysInfo->isSysPrlMatchValid         = 0xFF;
        pGetSysInfoResp->pHDRSysInfo->isSysPrlMatch              = 0xFF;
        pGetSysInfoResp->pHDRSysInfo->hdrPersonalityValid        = 0xFF;
        pGetSysInfoResp->pHDRSysInfo->hdrPersonality             = 0xFF;
        pGetSysInfoResp->pHDRSysInfo->hdrActiveProtValid         = 0xFF;
        pGetSysInfoResp->pHDRSysInfo->hdrActiveProt              = 0xFF;
        pGetSysInfoResp->pHDRSysInfo->is856SysIdValid            = 0xFF;
    }
    if ( pGetSysInfoResp->pGSMSysInfo )
    {
        pGetSysInfoResp->pGSMSysInfo->sysInfoGSM.srvDomainValid  = 0xFF;
        pGetSysInfoResp->pGSMSysInfo->sysInfoGSM.srvDomain       = 0xFF;
        pGetSysInfoResp->pGSMSysInfo->sysInfoGSM.srvCapabilityValid
                                                                 = 0xFF;
        pGetSysInfoResp->pGSMSysInfo->sysInfoGSM.srvCapability   = 0xFF;
        pGetSysInfoResp->pGSMSysInfo->sysInfoGSM.roamStatusValid = 0xFF;
        pGetSysInfoResp->pGSMSysInfo->sysInfoGSM.roamStatus      = 0xFF;
        pGetSysInfoResp->pGSMSysInfo->sysInfoGSM.isSysForbiddenValid
                                                                 = 0xFF;
        pGetSysInfoResp->pGSMSysInfo->sysInfoGSM.isSysForbidden  = 0xFF;
        pGetSysInfoResp->pGSMSysInfo->lacValid           = 0xFF;
        pGetSysInfoResp->pGSMSysInfo->lac                = 0xFFFF;
        pGetSysInfoResp->pGSMSysInfo->cellIdValid        = 0xFF;
        pGetSysInfoResp->pGSMSysInfo->cellId             = 0xFFFFFFFF;
        pGetSysInfoResp->pGSMSysInfo->regRejectInfoValid = 0xFF;
        pGetSysInfoResp->pGSMSysInfo->rejectSrvDomain    = 0xFF;
        pGetSysInfoResp->pGSMSysInfo->rejCause           = 0xFF;
        pGetSysInfoResp->pGSMSysInfo->networkIdValid     = 0xFF;
        pGetSysInfoResp->pGSMSysInfo->egprsSuppValid     = 0xFF;
        pGetSysInfoResp->pGSMSysInfo->egprsSupp          = 0xFF;
        pGetSysInfoResp->pGSMSysInfo->dtmSuppValid       = 0xFF;
        pGetSysInfoResp->pGSMSysInfo->dtmSupp            = 0xFF;
    }
    if ( pGetSysInfoResp->pWCDMASysInfo )
    {
        pGetSysInfoResp->pWCDMASysInfo->sysInfoWCDMA.srvDomainValid = 0xFF;
        pGetSysInfoResp->pWCDMASysInfo->sysInfoWCDMA.srvDomain      = 0xFF;
        pGetSysInfoResp->pWCDMASysInfo->sysInfoWCDMA.srvCapabilityValid
                                                                 = 0xFF;
        pGetSysInfoResp->pWCDMASysInfo->sysInfoWCDMA.srvCapability  = 0xFF;
        pGetSysInfoResp->pWCDMASysInfo->sysInfoWCDMA.roamStatusValid
                                                                 = 0xFF;
        pGetSysInfoResp->pWCDMASysInfo->sysInfoWCDMA.roamStatus  = 0xFF;
        pGetSysInfoResp->pWCDMASysInfo->sysInfoWCDMA.isSysForbiddenValid
                                                                 = 0xFF;
        pGetSysInfoResp->pWCDMASysInfo->sysInfoWCDMA.isSysForbidden = 0xFF;
        pGetSysInfoResp->pWCDMASysInfo->lacValid           = 0xFF;
        pGetSysInfoResp->pWCDMASysInfo->lac                = 0xFFFF;
        pGetSysInfoResp->pWCDMASysInfo->cellIdValid        = 0xFF;
        pGetSysInfoResp->pWCDMASysInfo->cellId             = 0xFFFFFFFF;
        pGetSysInfoResp->pWCDMASysInfo->regRejectInfoValid = 0xFF;
        pGetSysInfoResp->pWCDMASysInfo->rejectSrvDomain    = 0xFF;
        pGetSysInfoResp->pWCDMASysInfo->rejCause           = 0xFF;
        pGetSysInfoResp->pWCDMASysInfo->networkIdValid     = 0xFF;
        pGetSysInfoResp->pWCDMASysInfo->hsCallStatusValid  = 0xFF;
        pGetSysInfoResp->pWCDMASysInfo->hsCallStatus       = 0xFF;
        pGetSysInfoResp->pWCDMASysInfo->hsIndValid         = 0xFF;
        pGetSysInfoResp->pWCDMASysInfo->hsInd              = 0xFF;
        pGetSysInfoResp->pWCDMASysInfo->pscValid           = 0xFF;
        pGetSysInfoResp->pWCDMASysInfo->psc                = 0xFFFF;
    }
    if ( pGetSysInfoResp->pLTESysInfo )
    {
        pGetSysInfoResp->pLTESysInfo->sysInfoLTE.srvDomainValid = 0xFF;
        pGetSysInfoResp->pLTESysInfo->sysInfoLTE.srvDomain      = 0xFF;
        pGetSysInfoResp->pLTESysInfo->sysInfoLTE.srvCapabilityValid
                                                                = 0xFF;
        pGetSysInfoResp->pLTESysInfo->sysInfoLTE.srvCapability  = 0xFF;
        pGetSysInfoResp->pLTESysInfo->sysInfoLTE.roamStatusValid
                                                                = 0xFF;
        pGetSysInfoResp->pLTESysInfo->sysInfoLTE.roamStatus     = 0xFF;
        pGetSysInfoResp->pLTESysInfo->sysInfoLTE.isSysForbiddenValid
                                                                = 0xFF;
        pGetSysInfoResp->pLTESysInfo->sysInfoLTE.isSysForbidden = 0xFF;
        pGetSysInfoResp->pLTESysInfo->lacValid           = 0xFF;
        pGetSysInfoResp->pLTESysInfo->lac                = 0xFFFF;
        pGetSysInfoResp->pLTESysInfo->cellIdValid        = 0xFF;
        pGetSysInfoResp->pLTESysInfo->cellId             = 0xFFFFFFFF;
        pGetSysInfoResp->pLTESysInfo->regRejectInfoValid = 0xFF;
        pGetSysInfoResp->pLTESysInfo->rejectSrvDomain    = 0xFF;
        pGetSysInfoResp->pLTESysInfo->rejCause           = 0xFF;
        pGetSysInfoResp->pLTESysInfo->networkIdValid     = 0xFF;
        pGetSysInfoResp->pLTESysInfo->tacValid           = 0xFF;
        pGetSysInfoResp->pLTESysInfo->tac                = 0xFFFF;
    }
    if ( pGetSysInfoResp->pAddCDMASysInfo )
    {
        pGetSysInfoResp->pAddCDMASysInfo->geoSysIdx = 0xFFFF;
        pGetSysInfoResp->pAddCDMASysInfo->regPrd    = 0xFFFF;
    }
    if ( pGetSysInfoResp->pAddHDRSysInfo )
    {
        *( pGetSysInfoResp->pAddHDRSysInfo ) = 0xFFFF;
    }
    if ( pGetSysInfoResp->pAddGSMSysInfo )
    {
        pGetSysInfoResp->pAddGSMSysInfo->geoSysIdx        = 0xFFFF;
        pGetSysInfoResp->pAddGSMSysInfo->cellBroadcastCap = 0xFFFFFFFF;
    }
    if ( pGetSysInfoResp->pAddWCDMASysInfo )
    {
        pGetSysInfoResp->pAddWCDMASysInfo->geoSysIdx        = 0xFFFF;
        pGetSysInfoResp->pAddWCDMASysInfo->cellBroadcastCap = 0xFFFFFFFF;
    }
    if ( pGetSysInfoResp->pAddLTESysInfo )
    {
        *( pGetSysInfoResp->pAddLTESysInfo ) = 0xFFFF;
    }
    if ( pGetSysInfoResp->pGSMCallBarringSysInfo )
    {
        pGetSysInfoResp->pGSMCallBarringSysInfo->csBarStatus = 0xFFFFFFFF;
        pGetSysInfoResp->pGSMCallBarringSysInfo->psBarStatus = 0xFFFFFFFF;
    }
    if ( pGetSysInfoResp->pWCDMACallBarringSysInfo )
    {
        pGetSysInfoResp->pWCDMACallBarringSysInfo->csBarStatus =0xFFFFFFFF;
        pGetSysInfoResp->pWCDMACallBarringSysInfo->psBarStatus =0xFFFFFFFF;
    }
    if ( pGetSysInfoResp->pLTEVoiceSupportSysInfo )
    {
        *( pGetSysInfoResp->pLTEVoiceSupportSysInfo ) = 0xFF;
    }
    if ( pGetSysInfoResp->pGSMCipherDomainSysInfo )
    {
        *( pGetSysInfoResp->pGSMCipherDomainSysInfo ) = 0xFF;
    }
    if ( pGetSysInfoResp->pWCDMACipherDomainSysInfo )
    {
        *( pGetSysInfoResp->pWCDMACipherDomainSysInfo ) = 0xFF;
    }
}

ULONG SLQSNasGetSysInfo( nasGetSysInfoResp *pGetSysInfoResp )
{
    ULONG  resultcode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiNasSLQSGetSysInfoResp response;

    /* Check against NULL for out params */
    if ( !pGetSysInfoResp )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* pack QMI request */
    resultcode = PkQmiNasSlqsGetSysInfo( &paramLength, pOutParam );

    if ( eQCWWAN_ERR_NONE != resultcode )
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* send QMI request and receive the response */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &paramLength );

    if ( eQCWWAN_ERR_NONE == resultcode )
    {
        /* Pass the pointer for the OUT parameters */
        response.pSysInfoResp = pGetSysInfoResp;

        /* Initializing the response */
        InitialiseGetSysInfoParams( pGetSysInfoResp );

        /* Copy to the caller's buffer */
        resultcode = UpkQmiNasSlqsGetSysInfo( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG SLQSNasSwiModemStatus( swiModemStatusResp *pModemStatusResp )
{
    ULONG  resultcode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiNasSLQSSwiModemStatusResp response;

    /* Check against NULL for out params */
    if ( !pModemStatusResp )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* pack QMI request */
    resultcode = PkQmiNasSlqsSwiModemStatus( &paramLength, pOutParam );

    if ( eQCWWAN_ERR_NONE != resultcode )
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* send QMI request and receive the response */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &paramLength );

    if ( eQCWWAN_ERR_NONE == resultcode )
    {
        /* Pass the pointer for the OUT parameters */
        response.pModemStatus = pModemStatusResp;

        /* Initializing the response */
        pModemStatusResp->commonInfo.temperature = 0xFF;
        pModemStatusResp->commonInfo.modemMode   = 0xFF;
        pModemStatusResp->commonInfo.systemMode  = 0xFF;
        pModemStatusResp->commonInfo.imsRegState = 0xFF;
        pModemStatusResp->commonInfo.psState     = 0xFF;

        if( pModemStatusResp->pLTEInfo )
        {
            pModemStatusResp->pLTEInfo->band         = 0xFF;
            pModemStatusResp->pLTEInfo->bandwidth    = 0xFF;
            pModemStatusResp->pLTEInfo->RXChan       = 0xFFFF;
            pModemStatusResp->pLTEInfo->TXChan       = 0xFFFF;
            pModemStatusResp->pLTEInfo->emmState     = 0xFF;
            pModemStatusResp->pLTEInfo->emmSubState  = 0xFF;
            pModemStatusResp->pLTEInfo->emmConnState = 0xFF;
        }

        /* Copy to the caller's buffer */
        resultcode = UpkQmiNasSlqsSwiModemStatus( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG SLQSNasGetHDRColorCode( nasGetHDRColorCodeResp *pGetHDRColorCodeResp )
{
    ULONG  resultcode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiNasSlqsGetHDRColorCodeResp response;

    /* Checking for Invalid Parameter */
    if ( !pGetHDRColorCodeResp )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* pack QMI request */
    resultcode = PkQmiNasSlqsGetHDRColorCode( &paramLength, pOutParam );

    if ( resultcode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* send QMI request and receive the response */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &paramLength );

    if ( resultcode == eQCWWAN_ERR_NONE )
    {
        /* Pass the pointer for the OUT parameters */
        response.pGetHDRColorCodeResp = pGetHDRColorCodeResp;

        /* Initializing the response */
        if( pGetHDRColorCodeResp->pColorCode )
        {
            *(pGetHDRColorCodeResp->pColorCode) = 0xFF;
        }

        /* Copy to the caller's buffer */
        resultcode = UpkQmiNasSlqsGetHDRColorCode( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG SLQSNasGetTxRxInfo( nasGetTxRxInfoReq  *pGetTxRxInfoReq,
                          nasGetTxRxInfoResp *pGetTxRxInfoResp )
{
    ULONG  resultcode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to ougoing request buffer */
    USHORT ParamLength; /* Ret'd length of the Param field */

    /* Storage for results and response variable */
    struct QmiNasSLQSGetTxRxInfoResp response;

    /* Checking for Invalid Parameter */
    if ( !pGetTxRxInfoReq || !pGetTxRxInfoResp )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiNasSlqsGetTxRxInfo( &ParamLength,
                                          pOutParam,
                                          pGetTxRxInfoReq );
    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  ParamLength,
                                  eQMI_TIMEOUT_2_S,
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultcode )
    {
        /* Pass the pointer for the OUT parameters */
        response.pGetTxRxInfoResp = pGetTxRxInfoResp;

        /* Initializing the response with default values */
        if( pGetTxRxInfoResp->pRXChain0Info )
        {
            pGetTxRxInfoResp->pRXChain0Info->isRadioTuned = DEFAULTBYTEVALUE;
            pGetTxRxInfoResp->pRXChain0Info->rxPower      = DEFAULTLONGVALUE;
            pGetTxRxInfoResp->pRXChain0Info->ecio         = DEFAULTLONGVALUE;
            pGetTxRxInfoResp->pRXChain0Info->rscp         = DEFAULTLONGVALUE;
            pGetTxRxInfoResp->pRXChain0Info->rsrp         = DEFAULTLONGVALUE;
            pGetTxRxInfoResp->pRXChain0Info->phase        = DEFAULTLONGVALUE;
        }
        if( pGetTxRxInfoResp->pRXChain1Info )
        {
            pGetTxRxInfoResp->pRXChain1Info->isRadioTuned = DEFAULTBYTEVALUE;
            pGetTxRxInfoResp->pRXChain1Info->rxPower      = DEFAULTLONGVALUE;
            pGetTxRxInfoResp->pRXChain1Info->ecio         = DEFAULTLONGVALUE;
            pGetTxRxInfoResp->pRXChain1Info->rscp         = DEFAULTLONGVALUE;
            pGetTxRxInfoResp->pRXChain1Info->rsrp         = DEFAULTLONGVALUE;
            pGetTxRxInfoResp->pRXChain1Info->phase        = DEFAULTLONGVALUE;
        }
        if( pGetTxRxInfoResp->pTXInfo )
        {
            pGetTxRxInfoResp->pTXInfo->isInTraffic = DEFAULTBYTEVALUE;
            pGetTxRxInfoResp->pTXInfo->txPower     = DEFAULTLONGVALUE;
        }

        /* Copy to the caller's buffer */
        resultcode = UPkQmiNasSlqsGetTxRxInfo(pInParam, &response);
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG SLQSNasGetSigInfo( nasGetSigInfoResp *pGetSigInfoResp )
{
    ULONG  resultcode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiNasSlqsGetSigInfoResp response;

    /* Checking for Invalid Parameter */
    if ( !pGetSigInfoResp )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* pack QMI request */
    resultcode = PkQmiNasSlqsGetSigInfo( &paramLength, pOutParam );

    if ( resultcode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* send QMI request and receive the response */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &paramLength );

    if ( eQCWWAN_ERR_NONE == resultcode )
    {
        /* Pass the pointer for the OUT parameters */
        response.pGetSigInfoResp = pGetSigInfoResp;

        /* Initializing the response parameters with default values */
        if( pGetSigInfoResp->pCDMASSInfo )
        {
            pGetSigInfoResp->pCDMASSInfo->rssi = DEFAULTBYTEVALUE;
            pGetSigInfoResp->pCDMASSInfo->ecio = DEFAULTWORDVALUE;
        }
        if( pGetSigInfoResp->pHDRSSInfo )
        {
            pGetSigInfoResp->pHDRSSInfo->rssi = DEFAULTBYTEVALUE;
            pGetSigInfoResp->pHDRSSInfo->ecio = DEFAULTWORDVALUE;
            pGetSigInfoResp->pHDRSSInfo->sinr = DEFAULTBYTEVALUE;
            pGetSigInfoResp->pHDRSSInfo->io   = DEFAULTLONGVALUE;
        }
        if( pGetSigInfoResp->pGSMSSInfo )
        {
            *(pGetSigInfoResp->pGSMSSInfo) = DEFAULTBYTEVALUE;
        }
        if( pGetSigInfoResp->pWCDMASSInfo )
        {
            pGetSigInfoResp->pWCDMASSInfo->rssi = DEFAULTBYTEVALUE;
            pGetSigInfoResp->pWCDMASSInfo->ecio = DEFAULTWORDVALUE;
        }
        if( pGetSigInfoResp->pLTESSInfo )
        {
            pGetSigInfoResp->pLTESSInfo->rssi = DEFAULTBYTEVALUE;
            pGetSigInfoResp->pLTESSInfo->rsrq = DEFAULTBYTEVALUE;
            pGetSigInfoResp->pLTESSInfo->rsrp = DEFAULTWORDVALUE;
            pGetSigInfoResp->pLTESSInfo->snr  = DEFAULTWORDVALUE;
        }

        /* Copy to the caller's buffer */
        resultcode = UpkQmiNasSlqsGetSigInfo( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG SLQSNasIndicationRegisterExt(
    nasIndicationRegisterReq *pIndicationRegisterReq )
{
    ULONG  resultcode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to ougoing request buffer */
    USHORT paramLength; /* Ret'd length of the Param field */

    /* Storage for results and response variable */
    struct QmiNasIndicationRegisterResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiNasIndicationRegister( &paramLength,
                                             pOutParam ,
                                             pIndicationRegisterReq );

    if ( resultcode != eQCWWAN_ERR_NONE )
    {
        qmrelreqbkp();
        return resultcode; /* No memory */
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we get a positive return */
    if ( eQCWWAN_ERR_NONE == resultcode )
    {
        resultcode = UpkQmiNasIndicationRegister( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG SLQSGetPLMNName(
    nasPLMNNameReq  *pPLMNNameReq,
    nasPLMNNameResp *pPLMNNameResp )
{
    ULONG  resultcode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiNasGetPLMNNameResp response;

    /* Check against NULL for out params */
    if (!pPLMNNameReq)
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* pack QMI request */
    resultcode = PkQmiNasGetPLMNName( &paramLength,
                                      pOutParam,
                                      pPLMNNameReq );

    if ( eQCWWAN_ERR_NONE != resultcode )
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* send QMI request and receive the response */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  paramLength,
                                  eQMI_TIMEOUT_10_S, /* 10 seconds */
                                  &pInParam,
                                  &paramLength );

    if ( eQCWWAN_ERR_NONE == resultcode )
    {
        /* initialize response structure */
        response.pPLMNNameResp = pPLMNNameResp;
        if (pPLMNNameResp)
        {
            pPLMNNameResp->spnLength    = 0;
            pPLMNNameResp->shortNameLen = 0;
            pPLMNNameResp->longNameLen  = 0;
        }

        /* Copy to the caller's buffer */
        resultcode = UpkQmiNasGetPLMNName( pInParam, &response );
    }
    qmrelreqbkp();
    return resultcode;
}

ULONG SLQSGetOperatorNameData( nasOperatorNameResp *pOperatorNameData )
{
    ULONG  resultcode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiNasGetOperatorNameDataResp response;

    /* Check against NULL for out params */
    if (!pOperatorNameData)
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* pack QMI request */
    resultcode = PkQmiNasGetOperatorNameData( &paramLength, pOutParam );

    if ( eQCWWAN_ERR_NONE != resultcode )
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* send QMI request and receive the response */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &paramLength );

    if ( eQCWWAN_ERR_NONE == resultcode )
    {
        /* initialize response structure */
        response.pOperatorNameData = pOperatorNameData;
        if (pOperatorNameData->pSrvcProviderName)
        {
            pOperatorNameData->pSrvcProviderName->displayCondition = 0xFF;
            pOperatorNameData->pSrvcProviderName->spnLength        = 0;
        }
        if (pOperatorNameData->pOperatorPLMNList)
        {
            pOperatorNameData->pOperatorPLMNList->numInstance = 0;
        }
        if (pOperatorNameData->pPLMNNetworkName)
        {
            pOperatorNameData->pPLMNNetworkName->numInstance = 0;
        }
        if (pOperatorNameData->pOperatorNameString)
        {
            pOperatorNameData->pOperatorNameString->PLMNName[0] = EOS;
        }
        if (pOperatorNameData->pNITZInformation)
        {
            pOperatorNameData->pNITZInformation->codingScheme       = 0xFF;
            pOperatorNameData->pNITZInformation->countryInitials    = 0xFF;
            pOperatorNameData->pNITZInformation->longNameSpareBits  = 0xFF;
            pOperatorNameData->pNITZInformation->shortNameSpareBits = 0xFF;
            pOperatorNameData->pNITZInformation->longNameLen        = 0;
            pOperatorNameData->pNITZInformation->shortNameLen       = 0;
        }

        /* Copy to the caller's buffer */
        resultcode = UpkQmiNasGetOperatorNameData( pInParam, &response );
    }
    qmrelreqbkp();
    return resultcode;
}

ULONG SLQSNasGet3GPP2Subscription (
    nasGet3GPP2SubscriptionInfoReq  *pGet3GPP2SubsInfoReq,
    nasGet3GPP2SubscriptionInfoResp *pGet3GPP2SubsInfoResp )
{
    ULONG  resultcode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiNasSLQSGet3GPP2SubscriptionInfoResp response;

    /* Checking for Invalid Parameter */
    if ( !pGet3GPP2SubsInfoReq || !pGet3GPP2SubsInfoResp )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* pack QMI request */
    resultcode = PkQmiNasSlqsGet3GPP2SubscriptionInfo( &paramLength,
                                                       pOutParam,
                                                       pGet3GPP2SubsInfoReq );
    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  paramLength,
                                  eQMI_TIMEOUT_10_S, /* 10 seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultcode )
    {
        /* Pass the pointer for the OUT parameters */
        response.pNas3GPP2SubscriptionInfoResp = pGet3GPP2SubsInfoResp;

        /* Initializing the response with default values */
        if( pGet3GPP2SubsInfoResp->pNAMNameInfo )
        {
            pGet3GPP2SubsInfoResp->pNAMNameInfo->namNameLen = 0;
        }
        if( pGet3GPP2SubsInfoResp->pDirNum )
        {
            pGet3GPP2SubsInfoResp->pDirNum->dirNumLen = 0;
        }
        if( pGet3GPP2SubsInfoResp->pHomeSIDNID )
        {
            pGet3GPP2SubsInfoResp->pHomeSIDNID->numInstances = 0;
        }
        if( pGet3GPP2SubsInfoResp->pMinBasedIMSI )
        {
            pGet3GPP2SubsInfoResp->pMinBasedIMSI->imsiM1112 = DEFAULTWORDVALUE;
        }
        if( pGet3GPP2SubsInfoResp->pTrueIMSI )
        {
            pGet3GPP2SubsInfoResp->pTrueIMSI->imsiT1112    = DEFAULTWORDVALUE;
            pGet3GPP2SubsInfoResp->pTrueIMSI->imsiTaddrNum = DEFAULTBYTEVALUE;
        }
        if( pGet3GPP2SubsInfoResp->pCDMAChannel )
        {
            pGet3GPP2SubsInfoResp->pCDMAChannel->priChA = DEFAULTWORDVALUE;
            pGet3GPP2SubsInfoResp->pCDMAChannel->priChB = DEFAULTWORDVALUE;
            pGet3GPP2SubsInfoResp->pCDMAChannel->secChA = DEFAULTWORDVALUE;
            pGet3GPP2SubsInfoResp->pCDMAChannel->secChB = DEFAULTWORDVALUE;
        }

        /* Copy to the caller's buffer */
        resultcode = UpkQmiNasSlqsGet3GPP2SubscriptionInfo(pInParam,&response);
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG SLQSNasGetCellLocationInfo(
    nasCellLocationInfoResp *pNasCellLocationInfoResp )
{
    ULONG  resultcode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiNasSLQSGetCellLocationInfoResp response;

    /* Check against NULL for out params */
    if ( NULL == pNasCellLocationInfoResp )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* pack QMI request */
    resultcode = PkQmiNasSlqsGetCellLocationInfo( &paramLength, pOutParam );

    if ( eQCWWAN_ERR_NONE != resultcode )
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* send QMI request and receive the response */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  paramLength,
                                  eQMI_TIMEOUT_2_S, /* 2 seconds */
                                  &pInParam,
                                  &paramLength );

    if ( eQCWWAN_ERR_NONE == resultcode )
    {
        /* Pass the pointer for the OUT parameters */
        response.pNasCellLocationInfoResp = pNasCellLocationInfoResp;

        /* Initializing the response */
        if( pNasCellLocationInfoResp->pGERANInfo )
        {
            pNasCellLocationInfoResp->pGERANInfo->cellID        = 0xFFFFFFFF;
            pNasCellLocationInfoResp->pGERANInfo->lac           = 0xFFFF;
            pNasCellLocationInfoResp->pGERANInfo->arfcn         = 0xFFFF;
            pNasCellLocationInfoResp->pGERANInfo->bsic          = 0xFF;
            pNasCellLocationInfoResp->pGERANInfo->timingAdvance = 0xFFFFFFFF;
            pNasCellLocationInfoResp->pGERANInfo->rxLev            = 0xFFFF;
            pNasCellLocationInfoResp->pGERANInfo->nmrInst       = 0x00;
        }

        if( pNasCellLocationInfoResp->pUMTSInfo )
        {
            pNasCellLocationInfoResp->pUMTSInfo->cellID    = 0xFFFFFFFF;
            pNasCellLocationInfoResp->pUMTSInfo->lac       = 0xFFFF;
            pNasCellLocationInfoResp->pUMTSInfo->uarfcn    = 0xFFFF;
            pNasCellLocationInfoResp->pUMTSInfo->psc       = 0xFFFF;
            pNasCellLocationInfoResp->pUMTSInfo->rscp      = 0xFFFF;
            pNasCellLocationInfoResp->pUMTSInfo->ecio       = 0xFFFF;
            pNasCellLocationInfoResp->pUMTSInfo->umtsInst  = 0x00;
            pNasCellLocationInfoResp->pUMTSInfo->geranInst = 0x00;
        }

        if( pNasCellLocationInfoResp->pCDMAInfo )
        {
            pNasCellLocationInfoResp->pCDMAInfo->sid      = 0xFFFF;
            pNasCellLocationInfoResp->pCDMAInfo->nid      = 0xFFFF;
            pNasCellLocationInfoResp->pCDMAInfo->baseId   = 0xFFFF;
            pNasCellLocationInfoResp->pCDMAInfo->refpn    = 0xFFFF;
            pNasCellLocationInfoResp->pCDMAInfo->baseLat  = 0xFFFFFFFF;
            pNasCellLocationInfoResp->pCDMAInfo->baseLong = 0xFFFFFFFF;
        }

        if( pNasCellLocationInfoResp->pLTEInfoIntrafreq )
        {
            pNasCellLocationInfoResp->pLTEInfoIntrafreq->ueInIdle = 0xFF;
            pNasCellLocationInfoResp->pLTEInfoIntrafreq->tac      = 0xFFFF;
            pNasCellLocationInfoResp->pLTEInfoIntrafreq->globalCellId
                                                                  = 0xFFFFFFFF;
            pNasCellLocationInfoResp->pLTEInfoIntrafreq->earfcn   = 0xFFFF;
            pNasCellLocationInfoResp->pLTEInfoIntrafreq->servingCellId
                                                                  = 0xFFFF;
            pNasCellLocationInfoResp->pLTEInfoIntrafreq->cellReselPriority
                                                                  = 0xFF;
            pNasCellLocationInfoResp->pLTEInfoIntrafreq->sNonIntraSearch
                                                                  = 0xFF;
            pNasCellLocationInfoResp->pLTEInfoIntrafreq->threshServingLow
                                                                  = 0xFF;
            pNasCellLocationInfoResp->pLTEInfoIntrafreq->sIntraSearch = 0xFF;
            pNasCellLocationInfoResp->pLTEInfoIntrafreq->cellsLen = 0x00;
        }

        if( pNasCellLocationInfoResp->pLTEInfoInterfreq )
        {
            pNasCellLocationInfoResp->pLTEInfoInterfreq->ueInIdle = 0xFF;
            pNasCellLocationInfoResp->pLTEInfoInterfreq->freqsLen = 0x00;
        }

        if( pNasCellLocationInfoResp->pLTEInfoNeighboringGSM )
        {
            pNasCellLocationInfoResp->pLTEInfoNeighboringGSM->ueInIdle = 0xFF;
            pNasCellLocationInfoResp->pLTEInfoNeighboringGSM->freqsLen = 0x00;
        }

        if( pNasCellLocationInfoResp->pLTEInfoNeighboringWCDMA )
        {
            pNasCellLocationInfoResp->pLTEInfoNeighboringWCDMA->ueInIdle
                                                                        = 0xFF;
            pNasCellLocationInfoResp->pLTEInfoNeighboringWCDMA->freqsLen
                                                                        = 0x00;
        }

        /* Copy to the caller's buffer */
        resultcode = UpkQmiNasSlqsGetCellLocationInfo( pInParam, &response );
    }

    qmrelreqbkp();
    return resultcode;
}

ULONG SLQSInitiateNetworkRegistration(
    nasInitNetworkReg *pNasInitNetRegistrationReq )
{
    ULONG resultcode;  /* Result of SwiQmiSendnWait() */
    BYTE  *pInParam;   /* ptr to param field rx'd from modem */
    BYTE  *pOutParam;  /* ptr to outbound param field */
    BYTE  *pReqBuf;    /* Pointer to ougoing request buffer */
    WORD  ParamLength; /* Ret'd length of the Param field */

    /* Storage for results and response variable */
    struct QmiNasSLQSInitiateNetworkRegistrationResp response;

    if ( NULL == pNasInitNetRegistrationReq )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message */
    resultcode = PkQmiNasSLQSInitiateNetworkRegistration (
                     &ParamLength,
                     pOutParam,
                     pNasInitNetRegistrationReq );

    if (resultcode != eQCWWAN_ERR_NONE)
    {
        qmrelreqbkp();
        return resultcode;
    }

    /* Prepare and send the blocking call */
    resultcode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_NAS,
                                  ParamLength,
                                  eQMI_TIMEOUT_30_S,
                                  &pInParam,
                                  &ParamLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultcode )
    {
        /* Copy to the caller's buffer */
        resultcode = UpkQmiNasSLQSInitiateNetworkRegistration (pInParam, &response);
    }
    qmrelreqbkp();
    return resultcode;
}
