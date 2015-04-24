/*
 * \ingroup cbk
 *
 * \file    qaCbkVoiceSLQSVoiceInfoRecInd.c
 *
 * \brief   Contains UnPacking routines for the QMI_VOICE_INFO_REC_IND message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCbkVoiceSLQSVoiceInfoRecInd.h"

/* Locals */
static signalInfo      SignalInfo ;
static callerIDInfo    CallerIDInfo;
static connectNumInfo  ConnectNumInfo;
static connectNumInfo  CallingPartyInfo;
static calledPartyInfo CalledPartyInfo;
static redirNumInfo    RedirNumInfo;
static NSSAudioCtrl    NSSaudioCtrl;
static lineCtrlInfo    LineCtrlInfo;
static extDispRecInfo  ExtDispRecInfo;
static BYTE            displayInfo;
static BYTE            extDispInfo;
static BYTE            callNameInfo;
static BYTE            callWaitInd;
static BYTE            CLIRCause;
static BYTE            NSSRelease;

/* Functions */

/*
 * This function unpacks the Call identifier TLV parameters from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvCallId(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceInfoRec *lResponse = ((voiceInfoRec *)pResp);

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the Call identifier */
    eRCode = GetByte( pTlvData, &(lResponse->callID) );
    return eRCode;
}

/*
 * This function unpacks the signal Information TLV parameters from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvSignalInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceInfoRec *lResponse = ((voiceInfoRec *)pResp);

    lResponse->pSignalInfo = &SignalInfo;
    signalInfo *lResp = lResponse->pSignalInfo;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the signalType */
    eRCode = GetByte( pTlvData, &(lResp->signalType) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the alertpitch */
    eRCode = GetByte( pTlvData, &(lResp->alertPitch) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the signal */
    eRCode = GetByte( pTlvData, &(lResp->signal) );
    return eRCode;
}

/*
 * This function unpacks the Caller Id Information TLV parameters from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvCallerIDInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceInfoRec *lResponse = ((voiceInfoRec *)pResp);

    lResponse->pCallerIDInfo = &CallerIDInfo;
    callerIDInfo *lResp = lResponse->pCallerIDInfo;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE lCount = 0, lIndex = 0;

    /* Extract the Presentation Indicator */
    eRCode = GetByte( pTlvData, &(lResp->PI) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the Called ID Length */
    eRCode = GetByte( pTlvData, &(lResp->callerIDLen) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    lCount = lResp->callerIDLen;

    /* Extract the Caller ID call state */
    while (lCount--)
    {
        eRCode = GetByte( pTlvData, &(lResp->callerID[lIndex++]) );
        if( eRCode != eQCWWAN_ERR_NONE )
            return eRCode;
    }
    lResp->callerID[lIndex] = EOS;
    return eRCode;
}

/*
 * This function unpacks the Display Info TLV parameters from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvDispInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceInfoRec *lResponse = ((voiceInfoRec *)pResp);
    lResponse->pDispInfo = &displayInfo;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the Display Info */
    eRCode = GetByte( pTlvData, (lResponse->pDispInfo) );
    return eRCode;
}

/*
 * This function unpacks the extended Display Info TLV parameters from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvExtDispInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceInfoRec *lResponse = ((voiceInfoRec *)pResp);
    lResponse->pExtDispInfo = &extDispInfo;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the Extended Display Info */
    eRCode = GetByte( pTlvData, (lResponse->pExtDispInfo) );
    return eRCode;
}

/*
 * This function unpacks the caller name Info TLV parameters from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvCallerNameInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceInfoRec *lResponse = ((voiceInfoRec *)pResp);
    lResponse->pCallerNameInfo = &callNameInfo;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the Caller Name Info */
    eRCode = GetByte( pTlvData, (lResponse->pCallerNameInfo) );
    return eRCode;
}

/*
 * This function unpacks the call wait Indication TLV parameters from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvCallWaitInd(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceInfoRec *lResponse = ((voiceInfoRec *)pResp);
    lResponse->pCallWaitInd = &callWaitInd;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the Call Waiting Indication */
    eRCode = GetByte( pTlvData, (lResponse->pCallWaitInd) );
    return eRCode;
}

/*
 * This function unpacks the Connected Number Information from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
package enum eQCWWANError UnpackTlvConnectNumInfoRec(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceInfoRec *lResponse = ((voiceInfoRec *)pResp);

    lResponse->pConnectNumInfo = &ConnectNumInfo;
    connectNumInfo *lResp = lResponse->pConnectNumInfo;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE lCount = 0, lIndex = 0;

    /* Copy the Connected Number Information responses into the list */
    eRCode = GetByte( pTlvData, &lResp->numPresInd );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->screeningInd );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->numType );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->numPlan );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->callerIDLen );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    lCount = lResp->callerIDLen;

    /* Extract the Caller ID */
    while (lCount--)
    {
        eRCode = GetByte( pTlvData, &(lResp->callerID[lIndex++]) );
        if( eRCode != eQCWWAN_ERR_NONE )
            return eRCode;
    }
    lResp->callerID[lIndex] = EOS;
    return eRCode;
}

/*
 * This function unpacks the calling Party Number Information TLV parameters
 * from the QMI indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackTlvCallingPartyNumInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceInfoRec *lResponse = ((voiceInfoRec *)pResp);

    lResponse->pCallingPartyInfo = &CallingPartyInfo;
    connectNumInfo *lResp = lResponse->pCallingPartyInfo;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE lCount = 0, lIndex = 0;

    /* Extract the Presentation indicator*/
    eRCode = GetByte( pTlvData, &(lResp->numPresInd) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the Screening Indicator */
    eRCode = GetByte( pTlvData, &(lResp->screeningInd) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the number type */
    eRCode = GetByte( pTlvData, &(lResp->numType) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the Number Plan*/
    eRCode = GetByte( pTlvData, &(lResp->numPlan) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the caller Identifier Length */
    eRCode = GetByte( pTlvData, &(lResp->callerIDLen) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    lCount = lResp->callerIDLen;

    /* Extract the Caller ID */
    while (lCount--)
    {
        eRCode = GetByte( pTlvData, &(lResp->callerID[lIndex++]) );
        if( eRCode != eQCWWAN_ERR_NONE )
            return eRCode;
    }
    lResp->callerID[lIndex] = EOS;
    return eRCode;
}

/*
 * This function unpacks the called Party Number Information TLV parameters
 * from the QMI indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackTlvCalledPartyNumInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceInfoRec *lResponse = ((voiceInfoRec *)pResp);

    lResponse->pCalledPartyInfo = &CalledPartyInfo;
    calledPartyInfo *lResp = lResponse->pCalledPartyInfo;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE lCount = 0, lIndex = 0;

    /* Extract the Presentation indicator*/
    eRCode = GetByte( pTlvData, &(lResp->PI) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the Screening Indicator */
    eRCode = GetByte( pTlvData, &(lResp->SI) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the numType */
    eRCode = GetByte( pTlvData, &(lResp->numType) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the numType */
    eRCode = GetByte( pTlvData, &(lResp->numType) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the numLen */
    eRCode = GetByte( pTlvData, &(lResp->numLen) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    lCount = lResp->numLen;

    /* Extract the Number */
    while (lCount--)
    {
        eRCode = GetByte( pTlvData, &(lResp->number[lIndex++]) );
        if( eRCode != eQCWWAN_ERR_NONE )
            return eRCode;
    }
    lResp->number[lIndex] = EOS;
    return eRCode;
}

/*
 * This function unpacks the Redirected Number Information TLV parameters
 * from the QMI indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackTlvRedirNumInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceInfoRec *lResponse = ((voiceInfoRec *)pResp);

    lResponse->pRedirNumInfo = &RedirNumInfo;
    redirNumInfo *lResp = lResponse->pRedirNumInfo;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE lCount = 0, lIndex = 0;

    /* Extract the Presentation indicator*/
    eRCode = GetByte( pTlvData, &(lResp->PI) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the Screening Indicator */
    eRCode = GetByte( pTlvData, &(lResp->SI) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the numType */
    eRCode = GetByte( pTlvData, &(lResp->numType) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the numPlan */
    eRCode = GetByte( pTlvData, &(lResp->numPlan) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the reason */
    eRCode = GetByte( pTlvData, &(lResp->reason) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the number Length */
    eRCode = GetByte( pTlvData, &(lResp->numLen) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    lCount = lResp->numLen;

    /* Extract the Number */
    while (lCount--)
    {
        eRCode = GetByte( pTlvData, &(lResp->number[lIndex++]) );
        if( eRCode != eQCWWAN_ERR_NONE )
            return eRCode;
    }
    lResp->number[lIndex] = EOS;
    return eRCode;

}

/*
 * This function unpacks the CLIRCause TLV parameters from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvCLIRCause(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceInfoRec *lResponse = ((voiceInfoRec *)pResp);
    lResponse->pCLIRCause = &CLIRCause;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the Display Info */
    eRCode = GetByte( pTlvData, (lResponse->pCLIRCause) );
    return eRCode;
}

/*
 * This function unpacks the NSSAudioCtrl TLV parameters
 * from the QMI indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackTlvNSSAudioCtrl(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceInfoRec *lResponse = ((voiceInfoRec *)pResp);

    lResponse->pNSSAudioCtrl = &NSSaudioCtrl;
    NSSAudioCtrl *lResp = lResponse->pNSSAudioCtrl;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the upLink */
    eRCode = GetByte( pTlvData, &(lResp->upLink) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the downLink */
    eRCode = GetByte( pTlvData, &(lResp->downLink) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    return eRCode;
}

/*
 * This function unpacks the NSSRelease TLV parameters from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvNSSRelease(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceInfoRec *lResponse = ((voiceInfoRec *)pResp);

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    lResponse->pNSSRelease = &NSSRelease;
    /* Extract the NSSRelease Info */
    eRCode = GetByte( pTlvData, (lResponse->pNSSRelease) );
    return eRCode;
}

/*
 * This function unpacks the LineCtrlInfo TLV parameters
 * from the QMI indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackTlvLineCtrlInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceInfoRec *lResponse = ((voiceInfoRec *)pResp);

    lResponse->pLineCtrlInfo = &LineCtrlInfo ;
    lineCtrlInfo  *lResp = lResponse->pLineCtrlInfo;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;


    /* Extract the polarityIncluded */
    eRCode = GetByte( pTlvData, &(lResp->polarityIncluded) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the toggleMode */
    eRCode = GetByte( pTlvData, &(lResp->toggleMode) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the revPolarity */
    eRCode = GetByte( pTlvData, &(lResp->revPolarity) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the pwrDenialTime */
    eRCode = GetByte( pTlvData, &(lResp->pwrDenialTime) );
    return eRCode;
}

/*
 * This function unpacks the extended Display Info TLV parameters
 * from the QMI indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvExtDispRecInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceInfoRec *lResponse = ((voiceInfoRec *)pResp);

    lResponse->pExtDispRecInfo = &ExtDispRecInfo;
    extDispRecInfo  *lResp = lResponse->pExtDispRecInfo;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE lCount = 0, lIndex = 0;

    /* Extract the dispType */
    eRCode = GetByte( pTlvData, &(lResp->dispType) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the extDispInfoLen */
    eRCode = GetByte( pTlvData, &(lResp->extDispInfoLen) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    lCount = lResp->extDispInfoLen;

    /* Extract the extended display Info */
    while (lCount--)
    {
        eRCode = GetByte( pTlvData, &(lResp->extDispInfo[lIndex++]) );
        if( eRCode != eQCWWAN_ERR_NONE )
            return eRCode;
    }
    lResp->extDispInfo[lIndex] = EOS;
    return eRCode;
}

/*
 * This function unpacks the Event Report Indication message to a
 * user-provided response structure.
 *
 * \param   pMdmResp - Pointer to packed response from the modem.
 *
 * \param   pApiResp - Pointer to storage to unpack into.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UpkQmiCbkVoiceInfoRecInd(
    BYTE         *pMdmResp,
    voiceInfoRec *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_CALL_ID_INFO_REC,          &UnpackCbkTlvCallId},
        { eTLV_SIGNAL_INFO,               &UnpackCbkTlvSignalInfo},
        { eTLV_CALLER_ID_INFORMATION,     &UnpackCbkTlvCallerIDInfo},
        { eTLV_DISPLAY_INFORMATION,       &UnpackCbkTlvDispInfo},
        { eTLV_EXTENDED_DISPLAY_INFO,     &UnpackCbkTlvExtDispInfo },
        { eTLV_CALLER_NAME_INFO,          &UnpackCbkTlvCallerNameInfo },
        { eTLV_CALL_WAITING_IND,          &UnpackCbkTlvCallWaitInd},
        { eTLV_CONNECTED_NUMBER_INFO,     &UnpackTlvConnectNumInfoRec},
        { eTLV_CALLING_PARTY_NUMBER_INFO, &UnpackTlvCallingPartyNumInfo},
        { eTLV_REDIRECTING_NUMBER_INFO,   &UnpackTlvRedirNumInfo },
        { eTLV_CALLED_PARTY_NUMBER_INFO,  &UnpackTlvCalledPartyNumInfo},
        { eTLV_NSS_CLIR,                  &UnpackCbkTlvCLIRCause },
        { eTLV_NSS_AUDIO_CONTROL,         &UnpackTlvNSSAudioCtrl },
        { eTLV_NSS_RELEASE,               &UnpackCbkTlvNSSRelease },
        { eTLV_LINE_CONTROL_INFO,         &UnpackTlvLineCtrlInfo },
        { eTLV_EXT_DISP_REC_INFO,         &UnpackCbkTlvExtDispRecInfo },
        { eTLV_TYPE_INVALID,              NULL } /* Important. Sentinel.
                                                  * Signifies last item in map.
                                                  */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_VOICE_ALL_CALL_STATUS_IND );
    return eRCode;
}
