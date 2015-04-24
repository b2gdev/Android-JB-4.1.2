/*
 * \ingroup voice
 *
 * \file    qaGobiApiVoiceCommon.c
 *
 * \brief   Voice Service Common Pack and Unpack function definitions.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaGobiApiVoiceCommon.h"

/*
 * A common function to API and callback which unpacks the array of Call Info
 * from the QMI message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure of arrCallInfo.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCommonArrCallInfo (
    BYTE        *pTlvData,
    arrCallInfo *pResp )
{
    enum eQCWWANError eRCode;
    BYTE              lCount;

    /* Check For Invalid Parameter */
    if ( NULL == pResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    eRCode = GetByte( pTlvData, &pResp->numInstances );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < pResp->numInstances ; lCount++ )
    {
        getAllCallInformation *lresp = &pResp->getAllCallInfo[lCount];
        /* Copy the Call Info responses into the list */
        eRCode = GetByte( pTlvData, &lresp->Callinfo.callID );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->Callinfo.callState );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->Callinfo.callType );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->Callinfo.direction );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->Callinfo.mode );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->isEmpty );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->ALS );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * A common function to API and callback which unpacks the array of Remote
 * Party Numbers from the QMI message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure of arrRemotePartyNum.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCommonArrRemPartyNum (
    BYTE              *pTlvData,
    arrRemotePartyNum *pResp )
{
    enum eQCWWANError eRCode;
    BYTE              lCount, lIcount;

    /* Check For Invalid Parameter */
    if ( NULL == pResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    eRCode = GetByte( pTlvData, &pResp->numInstances );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < pResp->numInstances ; lCount++ )
    {
        /* Copy the remote party number responses into the list */
        eRCode = GetByte( pTlvData, &pResp->RmtPtyNum[lCount].callID );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        remotePartyNum *lresp = &pResp->RmtPtyNum[lCount].RemotePartyNum;

        eRCode = GetByte( pTlvData, &lresp->presentationInd );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->numLen );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        for ( lIcount = 0 ; lIcount < lresp->numLen ; lIcount++ )
        {
            eRCode = GetByte( pTlvData, &lresp->remPartyNumber[lIcount]);
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }
        }
    }
    return eRCode;
}

/*
 * A common function to API and callback which unpacks the array of Remote
 * Party Names from the QMI message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure of arrRemotePartyName.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCommonArrRemPartyName (
    BYTE               *pTlvData,
    arrRemotePartyName *pResp )
{
    enum eQCWWANError eRCode;
    BYTE              lCount, lIcount;

    /* Check For Invalid Parameter */
    if ( NULL == pResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    eRCode = GetByte( pTlvData, &pResp->numInstances );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < pResp->numInstances ; lCount++ )
    {
        /* Copy the remote party name responses into the list */
        eRCode = GetByte( pTlvData, &pResp->GetAllCallRmtPtyName[lCount].callID );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        remotePartyName *lresp = &pResp->GetAllCallRmtPtyName[lCount].RemotePartyName;

        eRCode = GetByte( pTlvData, &lresp->namePI );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->codingScheme );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->nameLen );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        for ( lIcount = 0 ; lIcount < lresp->nameLen ; lIcount++ )
        {
            eRCode = GetByte( pTlvData, &lresp->callerName[lIcount]);
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }
        }
    }
    return eRCode;
}

/*
 * A common function to API and callback which unpacks the array of Alert
 * Type from the QMI message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure of arrAlertingType.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCommonArrAlertType (
    BYTE            *pTlvData,
    arrAlertingType *pResp )
{
    enum eQCWWANError eRCode;
    BYTE              lCount;

    /* Check For Invalid Parameter */
    if ( NULL == pResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    eRCode = GetByte( pTlvData, &pResp->numInstances );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < pResp->numInstances ; lCount++ )
    {
        /* Copy the Alerting Type responses into the list */
        eRCode = GetByte( pTlvData, &pResp->callID[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &pResp->AlertingType[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }
    return eRCode;
}

/*
 * A common function to API and callback which unpacks the array of Service
 * Option from the QMI message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure of arrSvcOption.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCommonArrServiceOpt (
    BYTE         *pTlvData,
    arrSvcOption *pResp )
{
    enum eQCWWANError eRCode;
    BYTE              lCount;

    /* Check For Invalid Parameter */
    if ( NULL == pResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    eRCode = GetByte( pTlvData, &pResp->numInstances );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < pResp->numInstances ; lCount++ )
    {
        /* Copy the Alerting Type responses into the list */
        eRCode = GetByte( pTlvData, &pResp->callID[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetWord( pTlvData, &pResp->srvOption[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }
    return eRCode;
}

/*
 * A common function to API and callback which unpacks the array of Call
 * End Reason from the QMI message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure of arrCallEndReason.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCommonArrCallEndReason (
    BYTE             *pTlvData,
    arrCallEndReason *pResp )
{
    enum eQCWWANError eRCode;
    BYTE              lCount;

    /* Check For Invalid Parameter */
    if ( NULL == pResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    eRCode = GetByte( pTlvData, &pResp->numInstances );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < pResp->numInstances ; lCount++ )
    {
        /* Copy the Alerting Type responses into the list */
        eRCode = GetByte( pTlvData, &pResp->callID[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetWord( pTlvData, &pResp->callEndReason[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }
    return eRCode;
}

/*
 * A common function to API and callback which unpacks the array of Alpha
 * Identifier from the QMI message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure of arrAlphaID.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCommonArrAlphaID (
    BYTE       *pTlvData,
    arrAlphaID *pResp )
{
    enum eQCWWANError eRCode;
    WORD              lcount, lIcount;

    /* Check For Invalid Parameter */
    if ( NULL == pResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    eRCode = GetByte( pTlvData, &pResp->numInstances );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lcount = 0 ; lcount < pResp->numInstances ; lcount++ )
    {
        eRCode = GetByte( pTlvData, &pResp->allCallsAlphaIDInfoArr[lcount].callID );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
        alphaIDInfo *lresp = &pResp->allCallsAlphaIDInfoArr[lcount].AlphaIDInfo;
        /* Copy the Alpha Identifier responses into the list */
        eRCode = GetByte( pTlvData, &lresp->alphaDcs );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->alphaLen );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        for( lIcount = 0 ; lIcount < lresp->alphaLen ; lIcount++ )
        {
            eRCode = GetByte( pTlvData, &lresp->alphaText[lIcount] );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }
        }
    }
    return eRCode;
}

/*
 * A common function to API and callback which unpacks the array of Connected
 * Party Numbers from the QMI message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure of arrConnectPartyNum.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCommonArrConnPartyNum (
    BYTE               *pTlvData,
    arrConnectPartyNum *pResp )
{
    enum eQCWWANError eRCode;
    WORD              lcount, lIcount;

    /* Check For Invalid Parameter */
    if ( NULL == pResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    eRCode = GetByte( pTlvData, &pResp->numInstances );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lcount = 0 ; lcount < pResp->numInstances ; lcount++ )
    {
        eRCode = GetByte( pTlvData, &pResp->ConnectedPartyNum[lcount].callID);
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &pResp->ConnectedPartyNum[lcount].numPI);
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &pResp->ConnectedPartyNum[lcount].numSI);
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &pResp->ConnectedPartyNum[lcount].numType);
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &pResp->ConnectedPartyNum[lcount].numPlan);
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &pResp->ConnectedPartyNum[lcount].numLen);
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        for( lIcount = 0 ; lIcount < pResp->ConnectedPartyNum[lcount].numLen ; lIcount++ )
        {
            eRCode = GetByte( pTlvData,
                           &pResp->ConnectedPartyNum[lcount].number[lIcount] );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }
        }
    }
    return eRCode;
}

/*
 * A common function to API and callback which unpacks the array of Diagnostic
 * Information from the QMI message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure of arrDiagInfo.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCommonArrDiagInfo (
    BYTE        *pTlvData,
    arrDiagInfo *pResp )
{
    enum eQCWWANError eRCode;
    BYTE              lCount, lIcount;

    /* Check For Invalid Parameter */
    if ( NULL == pResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    eRCode = GetByte( pTlvData, &pResp->numInstances );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < pResp->numInstances ; lCount++ )
    {
        eRCode = GetByte( pTlvData, &pResp->DiagInfo[lCount].callID );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        /* Copy the Diagnostic Information responses into the list */
        diagInfo *lresp = &pResp->DiagInfo[lCount].DiagInfo;

        eRCode = GetByte( pTlvData, &lresp->diagInfoLen );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        for( lIcount = 0 ; lIcount < lresp->diagInfoLen ; lIcount++ )
        {
            eRCode = GetByte( pTlvData, &lresp->diagnosticInfo[lIcount] );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }
        }
    }
    return eRCode;
}

/*
 * A common function to API and callback which unpacks the array of Called
 * Party Numbers from the QMI message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure of arrCalledPartyNum.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCommonArrCalledPartyNum (
    BYTE              *pTlvData,
    arrCalledPartyNum *pResp )
{
    enum eQCWWANError eRCode;
    WORD              lcount, lIcount;

    /* Check For Invalid Parameter */
    if ( NULL == pResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    eRCode = GetByte( pTlvData, &pResp->numInstances );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lcount = 0 ; lcount < pResp->numInstances ; lcount++ )
    {
        eRCode = GetByte( pTlvData, &pResp->CalledPartyNum[lcount].callID);
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &pResp->CalledPartyNum[lcount].numPI);
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &pResp->CalledPartyNum[lcount].numSI);
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &pResp->CalledPartyNum[lcount].numType);
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &pResp->CalledPartyNum[lcount].numPlan);
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &pResp->CalledPartyNum[lcount].numLen);
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        for( lIcount = 0 ; lIcount < pResp->CalledPartyNum[lcount].numLen ; lIcount++ )
        {
            eRCode = GetByte( pTlvData,
                           &pResp->CalledPartyNum[lcount].number[lIcount] );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }
        }
    }
    return eRCode;
}

/*
 * A common function to API and callback which unpacks the array of Redirected
 * Party Numbers from the QMI message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure of arrRedirPartyNum.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCommonArrRedPartyNum (
    BYTE             *pTlvData,
    arrRedirPartyNum *pResp )
{
    enum eQCWWANError eRCode;
    WORD              lcount, lIcount;

    /* Check For Invalid Parameter */
    if ( NULL == pResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    eRCode = GetByte( pTlvData, &pResp->numInstances );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lcount = 0 ; lcount < pResp->numInstances ; lcount++ )
    {
        eRCode = GetByte( pTlvData, &pResp->RedirPartyNum[lcount].callID);
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &pResp->RedirPartyNum[lcount].numPI);
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &pResp->RedirPartyNum[lcount].numSI);
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &pResp->RedirPartyNum[lcount].numType);
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &pResp->RedirPartyNum[lcount].numPlan);
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &pResp->RedirPartyNum[lcount].numLen);
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        for( lIcount = 0 ; lIcount < pResp->RedirPartyNum[lcount].numLen ; lIcount++ )
        {
            eRCode = GetByte( pTlvData,
                           &pResp->RedirPartyNum[lcount].number[lIcount] );
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }
        }
    }
    return eRCode;
}

/*
 * A common function to API and callback which unpacks the array of Alerting
 * Pattern from the QMI message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure of arrAlertingPattern.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCommonArrAlertPattern (
    BYTE               *pTlvData,
    arrAlertingPattern *pResp )
{
    enum eQCWWANError eRCode;
    BYTE              lCount;

    /* Check For Invalid Parameter */
    if ( NULL == pResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    eRCode = GetByte( pTlvData, &pResp->numInstances );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < pResp->numInstances ; lCount++ )
    {
        /* Copy the Alerting Pattern responses into the list */
        eRCode = GetByte( pTlvData, &pResp->callID[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetLong( pTlvData, &pResp->alertingPattern[lCount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }
    return eRCode;
}

/*
 * A common function to API and callback which unpacks the Alpha Identifier
 * from the QMI message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure of alphaIDInfo.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCommonAlphaIdentifier(
    BYTE        *pTlvData,
    alphaIDInfo *pResp )
{
    enum eQCWWANError eRCode;
    WORD              lcount;

    /* Check For Invalid Parameter */
    if ( NULL == pResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Copy the Alpha Identifier responses into the list */
    eRCode = GetByte( pTlvData, &pResp->alphaDcs );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &pResp->alphaLen );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for( lcount = 0 ; lcount < pResp->alphaLen ; lcount++ )
    {
        eRCode = GetByte( pTlvData, &pResp->alphaText[lcount] );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * A common function to API and callback which unpacks the Call Control
 * Supplementary Service Type from the QMI message to a user provided response
 * structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure of ccSUPSType.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvCommonccSupsType(
    BYTE       *pTlvData,
    ccSUPSType *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Check For Invalid Parameter */
    if ( NULL == pResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Copy the CC Supplementary Service responses into the list */
    eRCode = GetByte( pTlvData, &pResp->svcType );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetByte( pTlvData, &pResp->reason );
}
