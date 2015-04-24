/*
 * \ingroup cbk
 *
 * \file    qaCbkVoiceSLQSVoiceSUPSInd.c
 *
 * \brief   Contains UnPacking routines for the QMI_VOICE_SUPS_IND
 *          message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCbkVoiceSLQSVoiceSUPSInd.h"
#include "qaGobiApiVoiceCommon.h"

/* Locals */
local BYTE           SvcClass;
local BYTE           Reason;
local BYTE           CallFwNum[MAX_CALL_NO_LEN];
local BYTE           CallFWTimerVal;
local struct USSInfo SUPSUSSInfo;
local BYTE           SUPSCallID;
local alphaIDInfo    SUPSAlphaID;
local BYTE           CallBarPasswd[PASSWORD_LENGTH];
local newPwdData     NewPwdData;
local BYTE           DataSrc;
local WORD           FailCause;
local getCallFWInfo  GetCallFWInfo;
local CLIRResp       SUPSCLIRResp;
local CLIPResp       SUPSCLIPResp;
local COLPResp       SUPSCOLPResp;
local COLRResp       SUPSCOLRResp;
local CNAPResp       SUPSCNAPResp;

/* Functions */
/*
 * This function unpacks the SUPS information TLV from the QMI indication
 * message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvSUPSInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    SUPSInfo *lResp = &((voiceSUPSInfo *)pResp)->SUPSInformation;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the SUPS Information */
    eRCode = GetByte( pTlvData, &(lResp->svcType) );
    if( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetByte( pTlvData, &(lResp->isModByCC) );
}

/*
 * This function unpacks the SUPS Service Class TLV from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvSUPSServiceClass(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceSUPSInfo *lResp = (voiceSUPSInfo *)pResp;
    lResp->pSvcClass = &SvcClass;

    /* Extract the Service Class */
    return GetByte( pTlvData, lResp->pSvcClass );
}

/*
 * This function unpacks the SUPS Reason TLV from the QMI indication
 * message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvSUPSReason(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceSUPSInfo *lResp = (voiceSUPSInfo *)pResp;
    lResp->pReason = &Reason;

    /* Extract the Reason */
    return GetByte( pTlvData, lResp->pReason );
}

/*
 * This function unpacks the SUPS Call Forwarding Number TLV from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvSUPSCallFWNumber(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceSUPSInfo *lResp = (voiceSUPSInfo *)pResp;
    enum eQCWWANError eRCode;
    WORD              lCount;
    lResp->pCallFWNum = CallFwNum;

    /* Extract the Call Forwarding Number */
    for ( lCount = 0 ; lResp->pCallFWNum[lCount] != EOS ; lCount++ )
    {
        eRCode = GetByte( pTlvData, &lResp->pCallFWNum[lCount] );
        if( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function unpacks the SUPS Call Forwarding No Reply Timer TLV from the
 * QMI indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvSUPSCallFWNRTimer(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceSUPSInfo *lResp = (voiceSUPSInfo *)pResp;
    lResp->pCallFWTimerVal = &CallFWTimerVal;

    /* Extract the Call Forwarding No Reply Timer */
    return GetByte( pTlvData, lResp->pCallFWTimerVal );
}

/*
 * This function unpacks the SUPS USS Info TLV from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvSUPSUSSInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    struct USSInfo *lResp = ((voiceSUPSInfo *)pResp)->pUSSInfo;
    enum eQCWWANError eRCode;
    WORD              lCount;
    lResp = &SUPSUSSInfo;

    /* Extract the USS Info */
    eRCode = GetByte( pTlvData, &lResp->ussDCS );
    if( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->ussLen );
    if( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < lResp->ussLen ; lCount++ )
    {
        eRCode = GetByte( pTlvData, &lResp->ussData[lCount] );
        if( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function unpacks the SUPS Call ID TLV from the QMI indication message
 * to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvSUPSCallID(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceSUPSInfo *lResp = (voiceSUPSInfo *)pResp;
    lResp->pCallID = &SUPSCallID;

    /* Extract the Call Id */
    return GetByte( pTlvData, lResp->pCallID );
}

/*
 * This function unpacks the SUPS Alpha Identifier TLV from the QMI indication
 * message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvSUPSAlphaID(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceSUPSInfo *lResp = (voiceSUPSInfo *)pResp;
    lResp->pAlphaIDInfo = &SUPSAlphaID;

    /* Extract the Alpha Id */
    return UnpackTlvCommonAlphaIdentifier( pTlvData, lResp->pAlphaIDInfo );
}

/*
 * This function unpacks the SUPS Call Barring Password TLV from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvSUPSCBPassword(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceSUPSInfo *lResp = (voiceSUPSInfo *)pResp;
    enum eQCWWANError eRCode;
    WORD              lCount;
    lResp->pCallBarPasswd = CallBarPasswd;

    /* Extract the Call Barring Password */
    for ( lCount = 0 ; lCount < PASSWORD_LENGTH ; lCount++ )
    {
        eRCode = GetByte( pTlvData, &lResp->pCallBarPasswd[lCount] );
        if( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function unpacks the SUPS New Password Data TLV from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvSUPSNewPasswordData(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    newPwdData *lResp = ((voiceSUPSInfo *)pResp)->pNewPwdData;
    enum eQCWWANError eRCode;
    WORD              lCount;
    lResp = &NewPwdData;

    /* Extract the New Password Data */
    for ( lCount = 0 ; lCount < PASSWORD_LENGTH ; lCount++ )
    {
        eRCode = GetByte( pTlvData, &lResp->newPwd[lCount] );
        if( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    for ( lCount = 0 ; lCount < PASSWORD_LENGTH ; lCount++ )
    {
        eRCode = GetByte( pTlvData, &lResp->newPwdAgain[lCount] );
        if( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function unpacks the SUPS Data Source TLV from the QMI indication message
 * to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvSUPSDataSource(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceSUPSInfo *lResp = (voiceSUPSInfo *)pResp;
    lResp->pDataSrc = &DataSrc;

    /* Extract the Data Source */
    return GetByte( pTlvData, lResp->pDataSrc );
}

/*
 * This function unpacks the SUPS Failure Cause TLV from the QMI indication message
 * to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvSUPSFailureCause(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    voiceSUPSInfo *lResp = (voiceSUPSInfo *)pResp;
    lResp->pFailCause = &FailCause;

    /* Extract the Failure Cause */
    return GetWord( pTlvData, lResp->pFailCause );
}

/*
 * This function unpacks the SUPS Call Forwarding Data from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvSUPSCallFWData(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    getCallFWInfo *lResp = ((voiceSUPSInfo *)pResp)->pCallFwdInfo;
    enum eQCWWANError eRCode;
    WORD              lCount, lIcount;
    lResp = &GetCallFWInfo;

    /* Extract the number of instances */
    eRCode = GetByte( pTlvData, &lResp->numInstances );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lCount = 0 ; lCount < lResp->numInstances ; lCount++ )
    {
        callFWInfo *lresp = &lResp->CallFWInfo[lCount];
        /* Extract the call forwarding info responses into the list */
        eRCode = GetByte( pTlvData, &lresp->SvcStatus );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &lresp->SvcClass );
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
            eRCode = GetByte( pTlvData, &lresp->number[lIcount]);
            if ( eQCWWAN_ERR_NONE != eRCode )
            {
                return eRCode;
            }
        }

        eRCode = GetByte( pTlvData, &lresp->noReplyTimer );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function unpacks the SUPS CLIR Status TLV from the QMI indication
 * message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvSUPSCLIRStatus(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    CLIRResp *lResp = ((voiceSUPSInfo *)pResp)->pCLIRstatus;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    lResp = &SUPSCLIRResp;

    /* Extract the CLIR Status */
    eRCode = GetByte( pTlvData, &(lResp->ActiveStatus) );
    if( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetByte( pTlvData, &(lResp->ProvisionStatus) );
}

/*
 * This function unpacks the SUPS CLIP Status TLV from the QMI indication
 * message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvSUPSCLIPStatus(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    CLIPResp *lResp = ((voiceSUPSInfo *)pResp)->pCLIPstatus;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    lResp = &SUPSCLIPResp;

    /* Extract the CLIP Status */
    eRCode = GetByte( pTlvData, &(lResp->ActiveStatus) );
    if( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetByte( pTlvData, &(lResp->ProvisionStatus) );
}

/*
 * This function unpacks the SUPS COLP Status TLV from the QMI indication
 * message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvSUPSCOLPStatus(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    COLPResp *lResp = ((voiceSUPSInfo *)pResp)->pCOLPstatus;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    lResp = &SUPSCOLPResp;

    /* Extract the COLP Status */
    eRCode = GetByte( pTlvData, &(lResp->ActiveStatus) );
    if( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetByte( pTlvData, &(lResp->ProvisionStatus) );
}

/*
 * This function unpacks the SUPS COLR Status TLV from the QMI indication
 * message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvSUPSCOLRStatus(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    COLRResp *lResp = ((voiceSUPSInfo *)pResp)->pCOLRstatus;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    lResp = &SUPSCOLRResp;

    /* Extract the COLR Status */
    eRCode = GetByte( pTlvData, &(lResp->ActiveStatus) );
    if( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetByte( pTlvData, &(lResp->ProvisionStatus) );
}

/*
 * This function unpacks the SUPS CNAP Status TLV from the QMI indication
 * message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvSUPSCNAPStatus(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    CNAPResp *lResp = ((voiceSUPSInfo *)pResp)->pCNAPstatus;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    lResp = &SUPSCNAPResp;

    /* Extract the CNAP Status */
    eRCode = GetByte( pTlvData, &(lResp->ActiveStatus) );
    if( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetByte( pTlvData, &(lResp->ProvisionStatus) );
}

/*
 * This function unpacks the SUPS Indication message to a user-provided
 * response structure.
 *
 * \param   pMdmResp - Pointer to packed response from the modem.
 *
 * \param   pApiResp - Pointer to storage to unpack into.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UpkQmiCbkVoiceSUPSInd(
    BYTE          *pMdmResp,
    voiceSUPSInfo *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_SUPS_INFORMATION,      &UnpackCbkTlvSUPSInfo},
        { eTLV_SUPS_SERVICE_CLASS,    &UnpackCbkTlvSUPSServiceClass },
        { eTLV_SUPS_REASON,           &UnpackCbkTlvSUPSReason },
        { eTLV_SUPS_CALL_FW_NUMBER,   &UnpackCbkTlvSUPSCallFWNumber },
        { eTLV_SUPS_CALL_FW_NR_TIMER, &UnpackCbkTlvSUPSCallFWNRTimer },
        { eTLV_SUPS_USS_INFO,         &UnpackCbkTlvSUPSUSSInfo },
        { eTLV_SUPS_CALL_ID,          &UnpackCbkTlvSUPSCallID },
        { eTLV_SUPS_ALPHA_ID,         &UnpackCbkTlvSUPSAlphaID },
        { eTLV_SUPS_CB_PWD,           &UnpackCbkTlvSUPSCBPassword },
        { eTLV_SUPS_NEW_PWD_DATA,     &UnpackCbkTlvSUPSNewPasswordData },
        { eTLV_SUPS_DATA_SOURCE,      &UnpackCbkTlvSUPSDataSource },
        { eTLV_SUPS_FAILURE_CAUSE,    &UnpackCbkTlvSUPSFailureCause },
        { eTLV_SUPS_CALL_FW_DATA,     &UnpackCbkTlvSUPSCallFWData },
        { eTLV_SUPS_CLIR_STATUS,      &UnpackCbkTlvSUPSCLIRStatus },
        { eTLV_SUPS_CLIP_STATUS,      &UnpackCbkTlvSUPSCLIPStatus },
        { eTLV_SUPS_COLP_STATUS,      &UnpackCbkTlvSUPSCOLPStatus },
        { eTLV_SUPS_COLR_STATUS,      &UnpackCbkTlvSUPSCOLRStatus },
        { eTLV_SUPS_CNAP_STATUS,      &UnpackCbkTlvSUPSCNAPStatus },
        { eTLV_TYPE_INVALID,          NULL } /* Important. Sentinel.
                                              * Signifies last item in map.
                                              */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_VOICE_SUPS_IND );
    return eRCode;
}
