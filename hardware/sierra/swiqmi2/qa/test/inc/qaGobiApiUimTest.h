/**************
 *
 *  Filename: qaGobiApiUimTest.h
 *
 *  Purpose:  Contains prototypes for functions implementing specific tests
 *            for UIM called by the main() routine in qatest.c
 *
 * Copyright: © 2012 Sierra Wireless, Inc., all rights reserved
 *
 **************/

#ifndef __GOBI_API_UIM_TEST_H__
#define __GOBI_API_UIM_TEST_H__

typedef struct _UimSLQSUimPowerDownTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BYTE              slot;
    CHAR              desc[100];
} UimSLQSUimPowerDownTestCase_t;

typedef struct _UimSLQSUimGetCardStatusTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    cardStatus        *pCardStatus;
    hotSwapStatus     *pHotSwapStatus;
    CHAR              desc[100];
} UimSLQSUimGetCardStatusTestCase_t;

typedef struct _UimSLQSUimSetPinProtectionTestCase_t
{
    enum eQCWWANError     expectedReturnValue;
    ULONG                 testCaseId;
    UIMSessionInformation sessionInfo;
    setPINProtection      pinProtection;
    BYTE                  *pKeyReferenceID;
    ULONG                 *pIndicationTokenReq;
    remainingRetries      *pRemainingRetries;
    encryptedPIN1         *pEncryptedPIN1;
    ULONG                 *pIndicationToken;
    CHAR                  desc[100];
} UimSLQSUimSetPinProtectionTestCase_t;

typedef struct _UimSLQSUimVerifyPinTestCase_t
{
    enum eQCWWANError     expectedReturnValue;
    ULONG                 testCaseId;
    UIMSessionInformation sessionInfo;
    verifyUIMPIN          verifyPIN;
    encryptedPIN1         *pEncryptedPIN1Req;
    BYTE                  *pKeyReferenceID;
    ULONG                 *pIndicationTokenReq;
    remainingRetries      *pRemainingRetries;
    encryptedPIN1         *pEncryptedPIN1;
    ULONG                 *pIndicationToken;
    CHAR                  desc[100];
} UimSLQSUimVerifyPinTestCase_t;

typedef struct _UimSLQSUimChangePinTestCase_t
{
    enum eQCWWANError     expectedReturnValue;
    ULONG                 testCaseId;
    UIMSessionInformation sessionInfo;
    changeUIMPIN          changePIN;
    BYTE                  *pKeyReferenceID;
    ULONG                 *pIndicationTokenReq;
    remainingRetries      *pRemainingRetries;
    encryptedPIN1         *pEncryptedPIN1;
    ULONG                 *pIndicationToken;
    CHAR                  desc[100];
} UimSLQSUimChangePinTestCase_t;

typedef struct _UimSLQSUimUnblockPinTestCase_t
{
    enum eQCWWANError     expectedReturnValue;
    ULONG                 testCaseId;
    UIMSessionInformation sessionInfo;
    unblockUIMPIN         unblockPIN;
    BYTE                  *pKeyReferenceID;
    ULONG                 *pIndicationTokenReq;
    remainingRetries      *pRemainingRetries;
    encryptedPIN1         *pEncryptedPIN1;
    ULONG                 *pIndicationToken;
    CHAR                  desc[100];
} UimSLQSUimUnblockPinTestCase_t;

typedef struct _UimSLQSUimRefreshOKTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BYTE              sessionType;
    BYTE              aidLength;
    BYTE              aid[255];
    BYTE              OKtoRefresh;
    CHAR              desc[100];
} UimSLQSUimRefreshOKTestCase_t;

typedef struct _UimSLQSUimRefreshRegisterTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BYTE              sessionType;
    BYTE              aidLength;
    BYTE              aid[255];
    BYTE              registerFlag;
    BYTE              voteForInit;
    WORD              numFiles;
    WORD              fileID;
    BYTE              pathLen;
    WORD              path[255];
    CHAR              desc[100];
} UimSLQSUimRefreshRegisterTestCase_t;

typedef struct _UimSLQSUimEventRegisterTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             eventMask;
    CHAR              desc[100];
} UimSLQSUimEventRegisterTestCase_t;

typedef struct _UimSLQSUimRefreshCompleteTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BYTE              sessionType;
    BYTE              aidLength;
    BYTE              aid[255];
    BYTE              refreshComplete;
    CHAR              desc[100];
} UimSLQSUimRefreshCompleteTestCase_t;

typedef struct _UimSLQSUimRefreshGetLastEventTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BYTE              sessionType;
    BYTE              aidLength;
    BYTE              aid[255];
    CHAR              desc[100];
} UimSLQSUimRefreshGetLastEventTestCase_t;

typedef struct _UimSLQSUimGetFileAttributesTestCase_t
{
    enum eQCWWANError     expectedReturnValue;
    ULONG                 testCaseId;
    UIMSessionInformation sessionInfo;
    fileInfo              fileIndex;
    ULONG                 *pIndicationTokenReq;
    cardResult            *pCardResult;
    fileAttributes        *pFileAttributes;
    ULONG                 *pIndicationToken;
    CHAR                  desc[100];
} UimSLQSUimGetFileAttributesTestCase_t;

typedef struct _UimSLQSUimDepersonalizationTestCase_t
{
    enum eQCWWANError            expectedReturnValue;
    ULONG                        testCaseId;
    depersonalizationInformation depersonilisationInfo;
    remainingRetries             *pRemainingRetries;
    CHAR                         desc[100];
} UimSLQSUimDepersonalizationTestCase_t;

typedef struct _UimSLQSUimAuthenticateTestCase_t
{
    enum eQCWWANError     expectedReturnValue;
    ULONG                 testCaseId;
    UIMSessionInformation sessionInfo;
    authenticationData    authData;
    ULONG                 *pIndicationTokenReq;
    cardResult            *pCardResult;
    authenticateResult    *pAuthenticateResult;
    ULONG                 *pIndicationToken;
    CHAR                  desc[100];
} UimSLQSUimAuthenticateTestCase_t;

#endif /* __GOBI_API_UIM_TEST_H__ */
