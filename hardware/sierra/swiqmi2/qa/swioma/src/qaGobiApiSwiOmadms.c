/**
 * \ingroup oma
 *
 * \file    qaGobiApiSwiOmadms.c
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
#include "qaGobiApiSwiOmadms.h"
#include "qaSwiOmaDmSessionStart.h"
#include "qaSwiOmaDmSessionCancel.h"
#include "qaSwiOmaDmSessionGetInfo.h"
#include "qaSwiOmaDmSelection.h"
#include "qaSwiOmaDmGetSettings.h"
#include "qaSwiOmaDmSetSettings.h"

ULONG SLQSOMADMStartSession(
    ULONG sessionType )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiSwiOmaDmSessionStartResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiSwiOmaDmSessionStart( &paramLength,
                                            pOutParam,
                                            sessionType );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_SWIOMA,
                                  paramLength,
                                  eQMI_TIMEOUT_20_S, /* 20 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiSwiOmaDmSessionStart( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSOMADMCancelSession(
    ULONG session )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiSwiOmaDmSessionCancelResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiSwiOmaDmSessionCancel( &paramLength,
                                             pOutParam,
                                             session );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_SWIOMA,
                                  paramLength,
                                  eQMI_TIMEOUT_20_S, /* 20 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiSwiOmaDmSessionCancel( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSOMADMGetSessionInfo(
    ULONG *pSessionType,
    ULONG *pSessionState )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiSwiOmaDmSessionGetInfoResp response;

    /*
     * pSessionState is OUT parameters and hence should not be NULL
     */
    if ( !pSessionType ||
         !pSessionState )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message and sends the message. */
    resultCode = PkQmiSwiOmaDmSessionGetInfo( &paramLength,
                                              pOutParam );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_SWIOMA,
                                  paramLength,
                                  eQMI_TIMEOUT_20_S, /* 20 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy the obtained values to the function OUT parameters */
        response.pSessionType  = pSessionType;
        response.pSessionState = pSessionState;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiSwiOmaDmSessionGetInfo( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSOMADMSendSelection(
    ULONG selection )
{
    ULONG  resultCode;  /* Result of SwiQmiSendnWait() */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiSwiOmaDmSelectionResp response;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiSwiOmaDmSelection( &paramLength,
                                         pOutParam,
                                         selection );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_SWIOMA,
                                  paramLength,
                                  eQMI_TIMEOUT_20_S, /* 20 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiSwiOmaDmSelection( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}

ULONG SLQSOMADMGetSettings(
    ULONG *pbOMADMEnabled,
    ULONG *pbFOTAdownload,
    ULONG *pbFOTAUpdate )
{
    ULONG  resultCode;  /* Result of SLQSOMADMGetSettings2() API call */

    SLQSOMADMSettings  slqsOMADMSettings;
    BYTE               AutoSdm; /* local variable not returned to user*/

    slqsOMADMSettings.pOMADMEnabled = pbOMADMEnabled;
    slqsOMADMSettings.pFOTAdownload = (BYTE *)pbFOTAdownload;
    slqsOMADMSettings.pFOTAUpdate   = (BYTE *)pbFOTAUpdate;
    slqsOMADMSettings.pAutosdm      = &AutoSdm;

    resultCode = SLQSOMADMGetSettings2( &slqsOMADMSettings );

    return resultCode;
}

ULONG SLQSOMADMSetSettings(
    ULONG bFOTAdownload,
    ULONG bFOTAUpdate )
{
    ULONG                      resultCode;  /* Result of SLQSOMADMSetSettings2 */
    SLQSOMADMSettingsReqParams slqsOMADMSettings;

    slqsOMADMSettings.FOTAdownload = bFOTAdownload;
    slqsOMADMSettings.FOTAUpdate   = bFOTAUpdate;
    slqsOMADMSettings.pAutosdm      = NULL;

    resultCode = SLQSOMADMSetSettings2( &slqsOMADMSettings );

    return resultCode;
}

ULONG SLQSOMADMSetSettings2(
    SLQSOMADMSettingsReqParams *pSLQSOMADMSettingsReqParams)
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiSwiOmaDmSetSettingsResp response;

    /* Check for NULL for input structure*/
    if ( NULL == pSLQSOMADMSettingsReqParams )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs the QMI message */
    resultCode = PkQmiSwiOmaDmSetSettings( &paramLength,
                                           pOutParam,
                                           pSLQSOMADMSettingsReqParams );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_SWIOMA,
                                  paramLength,
                                  eQMI_TIMEOUT_20_S, /* 20 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Copy to the caller's buffer */
        resultCode = UpkQmiSwiOmaDmSetSettings( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;

}

ULONG SLQSOMADMGetSettings2(
    SLQSOMADMSettings  *pSLQSOMADMSettings )
{
    ULONG  resultCode;  /* Result code to be returned by this function */
    BYTE   *pInParam;   /* ptr to param field rx'd from modem */
    BYTE   *pOutParam;  /* ptr to outbound param field */
    BYTE   *pReqBuf;    /* Pointer to outgoing request buffer */
    USHORT paramLength; /* Ret'd length of the QMI Param field */

    /* Storage for results and response variable */
    struct QmiSwiOmaDmGetSettingsResp response;

    /* Check for NULL for input structure*/
    if ( NULL == pSLQSOMADMSettings )
        return eQCWWAN_ERR_INVALID_ARG;

    /* Initialise response structure */
    response.pSLQSOMADMSettings = pSLQSOMADMSettings;

    /* Initialize the pointer to the outgoing request buffer pointer */
    pReqBuf = qmgetreqbkp();

    /* Get a pointer to the start of the outbound QMI Parameter field */
    pOutParam = amgetparamp( AMTYPEQMIREQUEST, pReqBuf );

    /* Invoke the function which packs QMI message and sends the message. */
    resultCode = PkQmiSwiOmaDmGetSettings( &paramLength, pOutParam );

    if ( eQCWWAN_ERR_NONE != resultCode )
    {
        qmrelreqbkp();
        return resultCode; /* No Memory */
    }

    /* Prepare and send the blocking call */
    resultCode = SwiQmiSendnWait( pReqBuf,
                                  eQMI_SVC_SWIOMA,
                                  paramLength,
                                  eQMI_TIMEOUT_20_S, /* 20 Seconds */
                                  &pInParam,
                                  &paramLength );

    /* Only parse out the response data if we got a positive return */
    if ( eQCWWAN_ERR_NONE == resultCode )
    {
        /* Initialize parameters to default values */
        if( pSLQSOMADMSettings->pOMADMEnabled )
            *(pSLQSOMADMSettings->pOMADMEnabled) = 0xFFFFFFFF;
        if( pSLQSOMADMSettings->pFOTAdownload )
            *(pSLQSOMADMSettings->pFOTAdownload) = 0xFF;
        if( pSLQSOMADMSettings->pFOTAUpdate )
            *(pSLQSOMADMSettings->pFOTAUpdate) = 0xFF;
        if( pSLQSOMADMSettings->pAutosdm )
            *(pSLQSOMADMSettings->pAutosdm) = 0xFF;

        /* Copy to the caller's buffer */
        resultCode = UpkQmiSwiOmaDmGetSettings( pInParam, &response );
    }

    qmrelreqbkp();
    return resultCode;
}
