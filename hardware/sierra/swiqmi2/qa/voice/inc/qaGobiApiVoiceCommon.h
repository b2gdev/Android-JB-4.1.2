/**
 * \ingroup voice
 *
 * \file    qaGobiApiVoiceCommon.h
 *
 * \brief   Voice Service Common Pack and Unpack function prototypes.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __GOBI_API_VOICE_COMMON_H__
#define __GOBI_API_VOICE_COMMON_H__

#include "qaGobiApiVoice.h"

extern enum eQCWWANError UnpackTlvCommonArrCallInfo (
    BYTE        *pTlvData,
    arrCallInfo *pResp );

extern enum eQCWWANError UnpackTlvCommonArrRemPartyNum (
    BYTE              *pTlvData,
    arrRemotePartyNum *pResp );

extern enum eQCWWANError UnpackTlvCommonArrRemPartyName (
    BYTE               *pTlvData,
    arrRemotePartyName *pResp );

extern enum eQCWWANError UnpackTlvCommonArrAlertType (
    BYTE            *pTlvData,
    arrAlertingType *pResp );

extern enum eQCWWANError UnpackTlvCommonArrServiceOpt (
    BYTE         *pTlvData,
    arrSvcOption *pResp );

extern enum eQCWWANError UnpackTlvCommonArrCallEndReason (
    BYTE             *pTlvData,
    arrCallEndReason *pResp );

extern enum eQCWWANError UnpackTlvCommonArrAlphaID (
    BYTE       *pTlvData,
    arrAlphaID *pResp );

extern enum eQCWWANError UnpackTlvCommonArrConnPartyNum (
    BYTE               *pTlvData,
    arrConnectPartyNum *pResp );

extern enum eQCWWANError UnpackTlvCommonArrDiagInfo (
    BYTE        *pTlvData,
    arrDiagInfo *pResp );

extern enum eQCWWANError UnpackTlvCommonArrCalledPartyNum (
    BYTE              *pTlvData,
    arrCalledPartyNum *pResp );

extern enum eQCWWANError UnpackTlvCommonArrRedPartyNum (
    BYTE             *pTlvData,
    arrRedirPartyNum *pResp );

extern enum eQCWWANError UnpackTlvCommonArrAlertPattern (
    BYTE               *pTlvData,
    arrAlertingPattern *pResp );

extern enum eQCWWANError UnpackTlvCommonAlphaIdentifier(
    BYTE        *pTlvData,
    alphaIDInfo *pResp );

extern enum eQCWWANError UnpackTlvCommonccSupsType(
    BYTE       *pTlvData,
    ccSUPSType *pResp );

#endif /* __GOBI_API_VOICE_COMMON_H__ */
