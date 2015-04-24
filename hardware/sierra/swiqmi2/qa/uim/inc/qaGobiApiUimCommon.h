/**
 * \ingroup uim
 *
 * \file    qaGobiApiUimCommon.h
 *
 * \brief   UIM Service Common Pack and Unpack function prototypes.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __GOBI_API_UIM_COMMON_H__
#define __GOBI_API_UIM_COMMON_H__

#include "qaGobiApiUim.h"

extern enum eQCWWANError PackTlvCommonSessionInfo(
    BYTE                  *pBuf,
    UIMSessionInformation *pReq );

extern enum eQCWWANError UnpackTlvCommonRetriesRem(
    BYTE             *pTlvData,
    remainingRetries *pResp );

extern enum eQCWWANError UnpackTlvCommonEncryptedPin1(
    BYTE          *pTlvData,
    encryptedPIN1 *pResp );

#endif /* __GOBI_API_UIM_COMMON_H__ */
