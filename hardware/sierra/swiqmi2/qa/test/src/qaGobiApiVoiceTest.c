/**************
 *  Filename: qaGobiApiVoiceTest.c
 *
 *  Purpose:  Voice API test driver function definitions
 *
 * Copyright: © 2011 Sierra Wireless, Inc., all rights reserved
 *
 **************/

/* Linux definitions */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* include files */
#include "SwiDataTypes.h"
#include "qmerrno.h"
#include "qmudefs.h"

#include "qaGobiApiVoice.h"
#include "qaGobiApiVoiceTest.h"
#include "qatesthelper.h"
#include "qatestproto.h"

/*******************
    TEST DATA
 *******************/
/* Constants */

/* Global Variables */

/* Dial and End Call Information parameters */
local CHAR              *pVoiceDialNumber = "8427994100";
local BYTE              callId = 1;
local BYTE              calltype = 0;
#if 0
local BYTE              calltypeEmer = 9;
#endif
local BYTE              clirtype = 1;
local BYTE              emergencycategory = 1;
local UUSInfo           uusinfo = { 0, 1, 5, { 0x2A, 0x32, 0x32, 0x35, 0x23 } };
local CUGInfo           cuginfo = { 0, 0, 0 };
local calledPartySubAdd cpsa = { 255, 0, 1, 5, { 0x2A, 0x32, 0x32, 0x35, 0x23 } };
local alphaIDInfo       alphaidinfo;
local BYTE              ccresulttype;
local ccSUPSType        ccsupstype;
local ULONG             svcType = 1;
local BYTE              endCallID = 1;

/* Supplementary Service Information parameters*/
#define VOICE_SRV_ACT 0x01
#define VOICE_SRV_DEA 0x02
#define VOICE_SRV_REG 0x03
#define VOICE_SRV_ERA 0x04

/* Service Class set as voice */
BYTE SrvClass      = VOICE_SUPS_SRV_CLASS_VOICE;
/* Service Class set as voice */
BYTE CallBarrPwd[] = "0000";
/* Call forwarding number */
BYTE CallFwdNu[]   = "8427994100";
/* Call forwarding timer value */
BYTE TimerVal      = 5;
/* Call forwarding for national plans */
callFwdTypeAndPlan CallFwdTypePlan = {0x02, 0x08};

/* Set Configuration parameters*/
local BYTE              autoanswer = 0x00;
local airTimer          airtimerconfig = { 1, 60 };
local roamTimer         roamtimerconfig = { 1, 60 };
local BYTE              TTYmode = 0;
local prefVoiceSO       prefvoiceso = { 1, 1, 0, 0, 0 };
local BYTE              prefvoicedomain = 0;

/* Get Supplementary Information parameters*/
local BYTE svcclass           = 0;
local BYTE reason             = 0x07;
local BYTE callBarReason      = 0x0B;
local BYTE callFWReason       = 0x06;
local CHAR *pOldPassword      = "0000";
local CHAR *pNewPassword      = "1234";
local CHAR *pInvalidPassword  = "ABCD";
local CHAR *pWrongPassword    = "4321";

/* Get Call and Get All Call Information parameters */
local BYTE               getcallinfocallId;

/* Voice manage calls params */
local BYTE CallID             = 1;
local WORD FailCause;

/* DTMF Information */
local BYTE              dtmfCallId = 1;
local burstDTMFInfo     DTMFInfoTest1    = {NULL, 9, "123456789"} ;
local DTMFLengths       DTMFLengthsTest1 = {0x01, 0x02};
local burstDTMFInfo     DTMFInfoTest2    = {&dtmfCallId, 3, "123"};
local DTMFLengths       DTMFLengthsTest2 = {0x02, 0x03};

/* Send Flash Parameters */
local BYTE flashCallID;
local BYTE FlashPayLd[] = "FlashPayLoad";
local BYTE flashType     = 0;
local BYTE flashType1    = 1;
local BYTE flashType2    = 2;

/* Indication Register Parameters */
local BYTE dtmfevent         = 0x01;
local BYTE voiceprivacyevent = 0x01;
local BYTE supsnotifyevent   = 0x01;

/* Get Configuration Parameters */
local BYTE setgetconf = 0x01;

/* Orig USSD No Wait Parameters */
local struct USSInfo ussinfo = { 1, 5, { 0x2A, 0x32, 0x32, 0x35, 0x23 }};

/* Bind Subscription Parameters */
#define SUBS_TYPE_PRIMARY   0x00
#define SUBS_TYPE_SECONDARY 0x01
#define SUBS_TYPE_INVALID   0x02

/* ALS Set Line Switching Parameters */
#define LINE_SWITCHING_NOT_ALLOWED 0x00
#define LINE_SWITCHING_ALLOWED     0x01

/* ALS Select Line Parameters */
#define ALS_LINE1 0x00
#define ALS_LINE2 0x01

/*******************
  TEST FUNCTIONS
 *******************/
/*
 * Name:     doprintccSupSType
 *
 * Purpose:  This is a wrapper for printing Call Control Supplementary Service
 *           Type descriptions.
 *
 * Parms:    fp          - pointer to FILE
 *           pCCSUPSType - pointer to ccSUPSType
 *
 * Return:   None
 *
 * Notes:    None.
 *
 */
void doprintccSupSType( FILE *fp, ccSUPSType *pCCSUPSType )
{
    FILE *localfp;
    localfp = fp;

    fprintf(localfp, "Service Type       : %x\n", pCCSUPSType->svcType);
    fprintf(localfp, "Reason             : %x\n", pCCSUPSType->reason);
}

/*
 * Name:     doprintAlphaID
 *
 * Purpose:  This is a wrapper for printing Alpha Identifier descriptions.
 *
 * Parms:    fp           - pointer to FILE
 *           pAlphaIDInfo - pointer to alphaIDInfo
 *
 * Return:   None
 *
 * Notes:    None.
 *
 */
void doprintAlphaID( FILE *fp, alphaIDInfo *pAlphaIDInfo )
{
    BYTE lcount;
    FILE *localfp;

    localfp = fp;
    fprintf(localfp, "Alpha DCS          : %x \n", pAlphaIDInfo->alphaDcs);
    fprintf(localfp, "Alpha Len          : %d \n", pAlphaIDInfo->alphaLen);
    fprintf(localfp, "Alpha Text         : ");
    for (lcount = 0;lcount < pAlphaIDInfo->alphaLen;lcount++)
    {
        fprintf(localfp, "%c", pAlphaIDInfo->alphaText[lcount]);
    }
    fprintf(localfp,"\n");
}

/*
 * Name:    doOriginateUSSD
 *
 * Purpose: Perform the tests that call the OriginateUSSD() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doOriginateUSSD( void )
{
    BYTE   tCaseNum = 0, Index = 0, USSLen;
    ULONG  nRet;
    FILE   *fp = NULL;
    struct USSInfo lUSSInfo;
    BYTE   ussdData[] = { 0x2A, 0x32, 0x32, 0x35, 0x23 };
    char   USSDTemp[1024];
    BYTE   *inChar;
    BOOL   bFlag = FALSE;

    /* Define the test cases to be executed */
    const VoiceOriginateUSSDTestCase_t \
        VoiceOriginateUSSDTestCases[MAX_VOICE_ORIGINATE_USSD_TESTCASE_NUM] =
    {
        {eQCWWAN_ERR_NONE, 1, 1, 5, ussdData, "All Valid - ASCII" }
    };

    fp = tfopen("../../voice/test/results/originateussd.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_VOICE_ORIGINATE_USSD_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_VOICE_ORIGINATE_USSD_TESTCASE_NUM,
                        VoiceOriginateUSSDTestCases[tCaseNum].desc);
            lUSSInfo.ussDCS
                = VoiceOriginateUSSDTestCases[tCaseNum].ussDCS;
            do
            {
                printf("\nPlease enter the ASCII USSD String (followed by" \
                       "<enter> key) \nor\nPress <enter> to send the" \
                       "default String :");
                //Read character wise until user presses <enter> key or exceeds
                //182 characters
                inChar = (BYTE *) fgets(USSDTemp, 1024, stdin);
                USSLen = strlen((char *)inChar); //includes terminal '\n'

                if(1 == USSLen && 10 == inChar[USSLen -1] )
                {
                    //User pressed <enter> key as the first input.
                    //Use default String
                    lUSSInfo.ussLen
                        = VoiceOriginateUSSDTestCases[tCaseNum].ussLen;
                    USSLen = lUSSInfo.ussLen; //Retrieve length from test case
                    for( Index  = 0; Index < USSLen; Index++)
                    {
                        lUSSInfo.ussData[Index] =
                        VoiceOriginateUSSDTestCases[tCaseNum].ussData[Index];
                    }
                    bFlag = TRUE;
                }
                else if( USSLen > 183 ||
                         ((183 == USSLen) &&  10 != inChar[USSLen - 1] ))
                {
                    //User has exceeded max. length of 182 characters .
                    //Prompt user to enter again.
                    printf ("\nError: Maximum length allowed for"\
                            " USSD String is 182 characters.\nPlease enter "\
                            "again.");
                    Index = 0;
                }
                else
                {
                   USSLen = USSLen - 1; //Exclude the last \n
                   for ( Index = 0; Index< USSLen; Index++ )
                   {
                       lUSSInfo.ussData[Index] = inChar[Index];
                   }
                   lUSSInfo.ussLen = USSLen ;
                   bFlag = TRUE;
                }
           }while( !bFlag );

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "USS DCS : 0x%x\n",\
                    lUSSInfo.ussDCS);
            fprintf(fp, "USS Len : 0x%x\n",\
                    lUSSInfo.ussLen);
            fprintf(fp, "USS Data :");
            for( Index = 0; Index < USSLen; Index++ )
            {
                fprintf(fp, "0x%x ",\
                    lUSSInfo.ussData[Index]);
            }
            fprintf(fp, "\n");

            nRet = OriginateUSSD(
                       (BYTE *)&lUSSInfo );

            fprintf(fp, "Return Code: %lu\n", nRet);
            doprintreason( fp, nRet );

            if ( !nRet )
                fprintf(fp, "OriginateUSSD Successful\n");

            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doAnswerUSSD
 *
 * Purpose: Perform the tests that call the AnswerUSSD() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doAnswerUSSD( void )
{
    BYTE  tCaseNum = 0, Index = 0, USSLen;
    ULONG nRet;
    FILE  *fp = NULL;
    struct USSInfo lUSSInfo;
    BYTE   ussdData[] = { 0x2A, 0x32, 0x32, 0x35, 0x23 };

    /* Define the test cases to be executed */
    const VoiceAnswerUSSDTestCase_t \
        VoiceAnswerUSSDTestCases[MAX_VOICE_ANSWER_USSD_TESTCASE_NUM] =
    {
        {eQCWWAN_ERR_NONE, 1, 1, 5, ussdData,  "All Valid - ASCII" }
    };
    fp = tfopen("../../voice/test/results/answerussd.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_VOICE_ANSWER_USSD_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_VOICE_ANSWER_USSD_TESTCASE_NUM,
                        VoiceAnswerUSSDTestCases[tCaseNum].desc);
            lUSSInfo.ussDCS
                = VoiceAnswerUSSDTestCases[tCaseNum].ussDCS;
            lUSSInfo.ussLen
                = VoiceAnswerUSSDTestCases[tCaseNum].ussLen;
            USSLen = lUSSInfo.ussLen;
            for( Index  = 0; Index < USSLen; Index++)
            {
                lUSSInfo.ussData[Index] =
                    VoiceAnswerUSSDTestCases[tCaseNum].ussData[Index];
            }

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "USS DCS : 0x%x\n",\
                    lUSSInfo.ussDCS);
            fprintf(fp, "USS Len : 0x%x\n",\
                    lUSSInfo.ussLen);
            fprintf(fp, "USS Data :");
            for( Index = 0; Index < USSLen; Index++ )
            {
                fprintf(fp, "0x%x ",\
                    lUSSInfo.ussData[Index]);
            }
            fprintf(fp, "\n");

            nRet = AnswerUSSD(
                       (BYTE *)&lUSSInfo );

            fprintf(fp, "Return Code: %lu\n", nRet);
            doprintreason( fp, nRet );

            if ( !nRet )
                fprintf(fp, "AnswerUSSD Successful\n");

            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doCancelUSSD
 *
 * Purpose: Perform the tests that call the CancelUSSD() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doCancelUSSD( void )
{
    BYTE   tCaseNum = 0;
    ULONG  nRet;
    FILE   *fp = NULL;

    /* Define the test cases to be executed */
    const VoiceCancelUSSDTestCase_t \
        VoiceCancelUSSDTestCases[MAX_VOICE_CANCEL_USSD_TESTCASE_NUM] =
    {
        {eQCWWAN_ERR_NONE, 1, "All Valid" }
    };
    fp = tfopen("../../voice/test/results/cancelussd.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_VOICE_CANCEL_USSD_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_VOICE_CANCEL_USSD_TESTCASE_NUM,
                        VoiceCancelUSSDTestCases[tCaseNum].desc);

            nRet = CancelUSSD();

            fprintf(fp, "Return Code: %lu\n", nRet);
            doprintreason( fp, nRet );

            if ( !nRet )
                fprintf(fp, "CancelUSSD Successful\n");

            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSVoiceDialCall
 *
 * Purpose: Perform the tests that call the SLQSVoiceDialCall() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSVoiceDialCall( void )
{
    /* Test Cases */
    VoiceSLQSDialCallTestCase_t VoiceSLQSDialCallTestCases[] =
    {
        { eQCWWAN_ERR_NONE, 1, pVoiceDialNumber, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, &callId, &alphaidinfo, &ccresulttype, &ccsupstype, "Valid Argument"}
    };

    FILE  *fp = tfopen("../../voice/test/results/slqsvoicedialcall.txt", "w");
    voiceCallRequestParams  req;
    voiceCallResponseParams resp;
    BYTE                    lcount;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( VoiceSLQSDialCallTestCases )/
                      sizeof( VoiceSLQSDialCallTestCases[0] );
        while ( tc < ncases )
        {
            VoiceSLQSDialCallTestCase_t *pd = &VoiceSLQSDialCallTestCases[tc];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tc + 1),
                        ncases,
                        pd->desc);

            fprintf(fp, "Calling Number : ");
            for ( lcount = 0 ; pd->pCallNumber[lcount] != '\0' ; lcount++ )
            {
                req.callNumber[lcount] = pd->pCallNumber[lcount];
                fprintf(fp, "%c",req.callNumber[lcount]);
            }
            fprintf(fp,"\n");

            req.pCallType          = pd->pCallType;
            req.pCLIRType          = pd->pCLIRType;
            req.pUUSInFo           = pd->pUUSInfo;
            req.pCUGInfo           = pd->pCUGInfo;
            req.pEmergencyCategory = pd->pEmergencyCategory;
            req.pCallPartySubAdd   = pd->pCallPartySubAdd;
            req.pSvcType           = pd->pSvcType;
            resp.pCallID           = pd->pCallID;
            resp.pAlphaIDInfo      = pd->pAlphaIDInfo;
            resp.pCCResultType     = pd->pCCResultType;
            resp.pCCSUPSType       = pd->pCCSUPSType;

            ULONG nRet = SLQSVoiceDialCall( &req , &resp );

            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSVoiceDialCall Successful\n");
                if ( NULL != resp.pCallID )
                {
                    /* Store the call Id of successful call */
                    endCallID         = *resp.pCallID;
                    getcallinfocallId = *resp.pCallID;
                    flashCallID       = *resp.pCallID;
                    dtmfCallId        = *resp.pCallID;
                }
                IFPRINTF(fp, "Call Id        : %x \n",resp.pCallID);

                if ( NULL != resp.pAlphaIDInfo )
                {
                    doprintAlphaID( fp, resp.pAlphaIDInfo );
                }
                IFPRINTF(fp, "CC Result Type : %x\n",resp.pCCResultType);

                if ( NULL != resp.pCCSUPSType )
                {
                    doprintccSupSType( fp, resp.pCCSUPSType );
                }
            }
            tc++;
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSVoiceEndCall
 *
 * Purpose: Perform the tests that call the SLQSVoiceEndCall() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSVoiceEndCall( void )
{
    /* Test Cases */
    SLQSVoiceEndCallTestCase_t SLQSVoiceEndCallTestCases[] =
    {
        {eQCWWAN_ERR_INVALID_ARG, 1, NULL, " Invalid Argument" },
        {eQCWWAN_ERR_NONE,        1, &endCallID, "All Valid" }
    };

    FILE *fp = tfopen("../../voice/test/results/SLQSVoiceEndCall.txt", "w");

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSVoiceEndCallTestCases )/
                      sizeof( SLQSVoiceEndCallTestCases[0] );
        while ( tc < ncases )
        {
            SLQSVoiceEndCallTestCase_t *pd = &SLQSVoiceEndCallTestCases[tc];

            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tc + 1),
                        ncases,
                        pd->desc);

            ULONG nRet = SLQSVoiceEndCall( pd->pCallId );

            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSVoiceEndCall Successful\n");
                fprintf(fp, "Call ID is : %d \n",*( pd->pCallId ) );
            }
            tc++;
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doFunctionalDialEndCall
 *
 * Purpose: Perform the tests that call the SLQSVoiceDialCall()
 *          and SLQSVoiceEndCall() APIs.
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doFunctionalDialEndCall( void )
{
    /* Test Cases */
    VoiceSLQSDialCallTestCase_t VoiceSLQSDialCallTestCases[] =
    {
        { eQCWWAN_ERR_NONE, 1, pVoiceDialNumber,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        &callId, &alphaidinfo, &ccresulttype, &ccsupstype, "Valid Argument"},
        { eQCWWAN_ERR_NONE, 2, pVoiceDialNumber,
        &calltype, NULL, NULL, NULL, NULL, NULL, NULL,
        &callId, &alphaidinfo, &ccresulttype, &ccsupstype, "Valid Argument"},
        { eQCWWAN_ERR_NONE, 3, pVoiceDialNumber,
        NULL, &clirtype, NULL, NULL, NULL, NULL, NULL,
        &callId, &alphaidinfo, &ccresulttype, &ccsupstype, "Valid Argument"},
        { eQCWWAN_ERR_NONE, 4, pVoiceDialNumber,
        NULL, NULL, &uusinfo, NULL, NULL, NULL, NULL,
        &callId, &alphaidinfo, &ccresulttype, &ccsupstype, "Valid Argument"},
        { eQCWWAN_ERR_NONE, 5, pVoiceDialNumber,
        NULL, NULL, NULL, &cuginfo, NULL, NULL, NULL,
        &callId, &alphaidinfo, &ccresulttype, &ccsupstype, "Valid Argument"},
#if 0
        { eQCWWAN_ERR_NONE, 6, pVoiceDialNumber,
        &calltypeEmer, NULL, NULL, NULL, &emergencycategory, NULL, NULL,
        &callId, &alphaidinfo, &ccresulttype, &ccsupstype, "Valid Argument"},
#endif
        { eQCWWAN_ERR_NONE, 7, pVoiceDialNumber,
        NULL, NULL, NULL, NULL, NULL, &cpsa, NULL,
        &callId, &alphaidinfo, &ccresulttype, &ccsupstype, "Valid Argument"},
        { eQCWWAN_ERR_NONE, 8, pVoiceDialNumber,
        NULL, NULL, NULL, NULL, NULL, NULL, &svcType,
        &callId, &alphaidinfo, &ccresulttype, &ccsupstype, "Valid Argument"},
        { eQCWWAN_ERR_NONE, 9, pVoiceDialNumber, &calltype, &clirtype,
        &uusinfo, &cuginfo, &emergencycategory, &cpsa, &svcType,
        &callId, &alphaidinfo, &ccresulttype, &ccsupstype, "All Valid"},
        { eQCWWAN_ERR_NONE, 10, pVoiceDialNumber, &calltype, &clirtype,
        &uusinfo, &cuginfo, &emergencycategory, &cpsa, NULL,
        &callId, &alphaidinfo, &ccresulttype, &ccsupstype, "All Valid"},
        { eQCWWAN_ERR_NONE, 11, pVoiceDialNumber, &calltype, &clirtype,
        &uusinfo, &cuginfo, &emergencycategory, &cpsa, &svcType,
        NULL, &alphaidinfo, &ccresulttype, &ccsupstype, "Valid Argument"},
        { eQCWWAN_ERR_NONE, 12, pVoiceDialNumber, &calltype, &clirtype,
        &uusinfo, &cuginfo, &emergencycategory, &cpsa, &svcType,
        &callId, NULL, &ccresulttype, &ccsupstype, "Valid Argument"},
        { eQCWWAN_ERR_NONE, 13, pVoiceDialNumber, &calltype, &clirtype,
        &uusinfo, &cuginfo, &emergencycategory, &cpsa, &svcType,
        &callId, &alphaidinfo, NULL, &ccsupstype, "Valid Argument"},
        { eQCWWAN_ERR_NONE, 14, pVoiceDialNumber, &calltype, &clirtype,
        &uusinfo, &cuginfo, &emergencycategory, &cpsa, &svcType,
        &callId, &alphaidinfo, &ccresulttype, NULL, "Valid Argument"},

    };

    FILE *fp = tfopen("../../voice/test/results/VoiceFunctionalDialEndCall.txt", "w");
    voiceCallRequestParams  req;
    voiceCallResponseParams resp;
    BYTE                    lcount;
    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( VoiceSLQSDialCallTestCases )/
                      sizeof( VoiceSLQSDialCallTestCases[0] );
        while ( tc < ncases )
        {
            VoiceSLQSDialCallTestCase_t *pd = &VoiceSLQSDialCallTestCases[tc];

            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tc + 1),
                        ncases,
                        pd->desc);


            fprintf(fp, "Calling Number : ");
            for ( lcount = 0 ; pd->pCallNumber[lcount] != '\0' ; lcount++ )
            {
                req.callNumber[lcount] = pd->pCallNumber[lcount];
                fprintf(fp, "%c",req.callNumber[lcount]);
            }
            fprintf(fp,"\n");
            req.pCallType  = pd->pCallType;
            IFPRINTF(fp, "Call Type      : %x \n", req.pCallType )

            req.pCLIRType  = pd->pCLIRType;
            IFPRINTF(fp, "CLIR Type      : %x\n", req.pCLIRType )

            req.pUUSInFo = pd->pUUSInfo;
            if ( NULL != req.pUUSInFo )
            {
                fprintf(fp, "UUS Type       : %x \n",req.pUUSInFo->UUSType);
                fprintf(fp, "UUS DCS        : %x \n",req.pUUSInFo->UUSDcs);
                fprintf(fp, "UUS DataLen    : %x \n",req.pUUSInFo->UUSDatalen);
                fprintf(fp, "UUS Type       : ");
                for ( lcount = 0 ; lcount < uusinfo.UUSDatalen ; lcount++ )
                {
                    fprintf(fp, "%x ",req.pUUSInFo->UUSData[lcount]);
                }
                fprintf(fp,"\n");
            }
            req.pCUGInfo = pd->pCUGInfo;
            req.pEmergencyCategory = pd->pEmergencyCategory;
            if ( NULL != req.pCUGInfo )
            {
                fprintf(fp, "CUG Index      : %x \n",req.pCUGInfo->CUGIndex);
                fprintf(fp, "Supp Pref CUG  : %x \n",req.pCUGInfo->SuppPrefCUG);
                fprintf(fp, "Supp OA        : %x \n",req.pCUGInfo->SuppOA);
            }
            IFPRINTF(fp, "Emergency Category: %x\n",req.pEmergencyCategory)

            req.pCallPartySubAdd = pd->pCallPartySubAdd;
            if ( NULL != req.pCallPartySubAdd )
            {
                fprintf(fp, "Extension Bit  : %x \n",req.pCallPartySubAdd->extBit);
                fprintf(fp, "Sub Add Type   : %x \n",req.pCallPartySubAdd->subAddrType);
                fprintf(fp, "Odd-Even Ind   : %x \n",req.pCallPartySubAdd->oddEvenInd);
                fprintf(fp, "Odd-Even Ind   : %x \n",req.pCallPartySubAdd->oddEvenInd);
                fprintf(fp, "Sub Address    : ");
                for (lcount = 0;lcount< cpsa.subAddrLen;lcount++)
                {
                    fprintf(fp, "%x ",req.pCallPartySubAdd->subAddr[lcount]);
                }
                fprintf(fp,"\n");
            }
            req.pSvcType = pd->pSvcType;
            IFPRINTF(fp, "Service Type   : %lu\n",req.pSvcType)

            resp.pCallID = pd->pCallID;
            resp.pAlphaIDInfo = pd->pAlphaIDInfo;
            resp.pCCResultType = pd->pCCResultType;
            resp.pCCSUPSType = pd->pCCSUPSType;

            ULONG nRet = SLQSVoiceDialCall( &req , &resp );

            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSVoiceDialCall Successful\n");
                IFPRINTF(fp, "Call Id        : %x \n",resp.pCallID);

                if ( NULL != resp.pAlphaIDInfo )
                {
                    doprintAlphaID( fp, resp.pAlphaIDInfo );
                }
                IFPRINTF(fp, "CC Result Type : %x\n",resp.pCCResultType);

                if ( NULL != resp.pCCSUPSType )
                {
                    doprintccSupSType( fp, resp.pCCSUPSType );
                }

                fprintf(fp, "Calling SLQSVoiceEndCall with callId received \n");
                nRet = SLQSVoiceEndCall( pd->pCallID );
                doprintreason( fp, nRet );
                if ( eQCWWAN_ERR_NONE == nRet )
                {
                    fprintf(fp, "SLQSVoiceEndCall Successful\n");
                    fprintf(fp, "Call ID is : %d \n",*( pd->pCallID ) );
                }
            }
            tc++;
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSVoiceSetSUPSService
 *
 * Purpose: Perform the tests that call the SLQSVoiceSetSUPSService() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSVoiceSetSUPSService( void )
{
    WORD failCause;

    /* Test Cases */
    VoiceSLQSSetSUPSServiceTestCase_t VoiceSLQSSetSUPSServiceTestCases[] =
    {
        /* Invalid Parameter - Voice Service */
        { eQCWWAN_ERR_NONE, 1, 0xFF, 0x0F, NULL, NULL, NULL, NULL, NULL,
         &failCause, &alphaidinfo, &ccresulttype, &callId, &ccsupstype, "Invalid Voice Service"},

        /* Invalid Parameter - Reason */
        { eQCWWAN_ERR_NONE, 2, VOICE_SRV_ACT, 0xFF, NULL, NULL, NULL, NULL, NULL,
         &failCause, &alphaidinfo, &ccresulttype, &callId, &ccsupstype, "Invalid Reason"},

        /* Activate Call Waiting */
        { eQCWWAN_ERR_NONE, 3, VOICE_SRV_ACT, 0x0F, NULL, NULL, NULL, NULL, NULL,
         &failCause, &alphaidinfo, &ccresulttype, &callId, &ccsupstype, "Activate Call Waiting"},

        /* Deactivate Call Waiting */
        { eQCWWAN_ERR_NONE, 4, VOICE_SRV_DEA, 0x0F, NULL, NULL, NULL, NULL, NULL,
         &failCause, &alphaidinfo, &ccresulttype, &callId, &ccsupstype, "Deactivate Call Waiting"},

        /* Register Call Forwarding */
        { eQCWWAN_ERR_NONE, 5, VOICE_SRV_REG, 0x03, &SrvClass, NULL, CallFwdNu, &TimerVal, &CallFwdTypePlan,
         &failCause, &alphaidinfo, &ccresulttype, &callId, &ccsupstype, "Register Call forwarding"},

        /* Erase Call Forwarding */
        { eQCWWAN_ERR_NONE, 6, VOICE_SRV_ERA, 0x03, &SrvClass, NULL, CallFwdNu, &TimerVal, &CallFwdTypePlan,
         &failCause, &alphaidinfo, &ccresulttype, &callId, &ccsupstype, "Erase Call forwarding"},

        /* Check ALL NULL response parameters */
        { eQCWWAN_ERR_NONE, 7, VOICE_SRV_ACT, 0x0F, NULL, NULL, NULL, NULL, NULL,
         NULL, NULL, NULL, NULL, NULL, "Check ALL NULL response parameters - Call Waiting Active"},
    };

    FILE  *fp = tfopen("../../voice/test/results/slqsvoicesetsupsservice.txt", "w");
    voiceSetSUPSServiceReq  req;
    voiceSetSUPSServiceResp resp;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( VoiceSLQSSetSUPSServiceTestCases )/
                      sizeof( VoiceSLQSSetSUPSServiceTestCases[0] );
        while ( tc < ncases )
        {
            VoiceSLQSSetSUPSServiceTestCase_t *pd =
                        &VoiceSLQSSetSUPSServiceTestCases[tc++];

            /* Update the request parameters */
            req.voiceSvc              = pd->voiceSvc;
            req.reason                = pd->reason;
            req.pServiceClass         = pd->pServiceClass;
            req.pCallBarringPasswd    = pd->pCallBarringPasswd;
            req.pCallForwardingNumber = pd->pCallForwardingNumber;
            req.pTimerVal             = pd->pTimerVal;
            req.pCallFwdTypeAndPlan   = pd->pCallFwdTypeAndPlan;

            /* Update the response parameters */
            resp.pFailCause    = pd->pFailCause;
            resp.pAlphaIDInfo  = pd->pAlphaIDInfo;
            resp.pCCResultType = pd->pCCResultType;
            resp.pCallID       = pd->pCallID;
            resp.pCCSUPSType   = pd->pCCSUPSType;

            fprintf(fp, "\nTest Case %d / %d : %s\n", tc, ncases, pd->desc);
            fprintf(fp, "Voice Service : %x\n", req.voiceSvc);
            fprintf(fp, "Reason : %x\n\n", req.reason);

            ULONG nRet = SLQSVoiceSetSUPSService( &req , &resp );

            doprintreason( fp, nRet );

            if (eQCWWAN_ERR_NONE != nRet)
            {
                IFPRINTF(fp, "Failure Cause : %x \n",resp.pFailCause);
                continue;
            }

            fprintf(fp, "SLQSVoiceSetSUPSService Successful\n");
            IFPRINTF(fp, "Call Id : %x \n",      resp.pCallID);
            IFPRINTF(fp, "CC Result : %x \n",    resp.pCCResultType);
            if ( NULL != resp.pAlphaIDInfo )
            {
                doprintAlphaID( fp, resp.pAlphaIDInfo );
            }
            if ( NULL != resp.pCCSUPSType )
            {
                doprintccSupSType( fp, resp.pCCSUPSType );
            }
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSVoiceSetConfig
 *
 * Purpose: Perform the tests that call the SLQSVoiceSetConfig() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSVoiceSetConfig( void )
{
    local BYTE autoansstatus;
    local BYTE airtimerstatus;
    local BYTE roamtimerstatus;
    local BYTE TTYconfigstatus;
    local BYTE prefvoicesostatus;
    local BYTE voicedomainprefstatus;

    /* Test Cases */
    VoiceSLQSSetConfigTestCase_t VoiceSLQSSetConfigTestCases[] =
    {
        { eQCWWAN_ERR_NONE, 1, &autoanswer, NULL, NULL,
        NULL, NULL, NULL, &autoansstatus,
        &airtimerstatus, &roamtimerstatus, &TTYconfigstatus, &prefvoicesostatus,
        &voicedomainprefstatus, "Setting Auto Answer" },
        { eQCWWAN_ERR_NONE, 2, NULL, &airtimerconfig, NULL,
        NULL, NULL, NULL, &autoansstatus,
        &airtimerstatus, &roamtimerstatus, &TTYconfigstatus, &prefvoicesostatus,
        &voicedomainprefstatus, "Setting Air Timer" },
        { eQCWWAN_ERR_NONE, 3, NULL, NULL, &roamtimerconfig,
        NULL, NULL, NULL, &autoansstatus,
        &airtimerstatus, &roamtimerstatus, &TTYconfigstatus, &prefvoicesostatus,
        &voicedomainprefstatus, "Setting Roam Timer" },
        { eQCWWAN_ERR_NONE, 4, NULL, NULL, NULL,
        &TTYmode, NULL, NULL, &autoansstatus,
        &airtimerstatus, &roamtimerstatus, &TTYconfigstatus, &prefvoicesostatus,
        &voicedomainprefstatus, "Setting TTY Mode" },
        { eQCWWAN_ERR_NONE, 5, NULL, NULL, NULL,
        NULL, &prefvoiceso, NULL, &autoansstatus,
        &airtimerstatus, &roamtimerstatus, &TTYconfigstatus, &prefvoicesostatus,
        &voicedomainprefstatus, "Setting Preferred Voice Service Option" },
        { eQCWWAN_ERR_NONE, 6, NULL, NULL, NULL,
        NULL, NULL, &prefvoicedomain, &autoansstatus,
        &airtimerstatus, &roamtimerstatus, &TTYconfigstatus, &prefvoicesostatus,
        &voicedomainprefstatus, "Setting Preferred Voice Domain" },
        { eQCWWAN_ERR_NONE, 7, &autoanswer, &airtimerconfig, &roamtimerconfig,
        &TTYmode, &prefvoiceso, &prefvoicedomain, &autoansstatus,
        &airtimerstatus, &roamtimerstatus, &TTYconfigstatus, &prefvoicesostatus,
        &voicedomainprefstatus, "Setting All" },

    };

    FILE  *fp = tfopen("../../voice/test/results/SLQSVoiceSetConfig.txt", "w");
    voiceSetConfigReq  req;
    voiceSetConfigResp resp;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( VoiceSLQSSetConfigTestCases )/
                      sizeof( VoiceSLQSSetConfigTestCases[0] );
        while ( tc < ncases )
        {
            VoiceSLQSSetConfigTestCase_t *pd = &VoiceSLQSSetConfigTestCases[tc];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tc + 1),
                        ncases,
                        pd->desc);

            req.pAutoAnswer = pd->pAutoAnswer;
            IFPRINTF(fp, "Auto Answer     :%x\n",req.pAutoAnswer)
            req.pAirTimerConfig = pd->pAirTimerConfig;
            if ( NULL != req.pAirTimerConfig )
            {
                fprintf(fp, "NAM-Id(AIR)     :%x\n",req.pAirTimerConfig->namID);
                fprintf(fp, "Air Timer       :%lu\n",req.pAirTimerConfig->airTimerValue);
            }
            req.pRoamTimerConfig = pd->pRoamTimerConfig;
            if ( NULL != req.pRoamTimerConfig )
            {
                fprintf(fp, "NAM-Id(ROAM)    :%x\n",req.pRoamTimerConfig->namID);
                fprintf(fp, "ROAM Timer      :%lu\n",req.pRoamTimerConfig->roamTimerValue);
            }
            req.pTTYMode = pd->pTTYMode;
            IFPRINTF(fp, "TTY Mode        :%x\n",req.pTTYMode)
            req.pPrefVoiceSO = pd->pPrefVoiceSO;
            if ( NULL != req.pPrefVoiceSO )
            {
                fprintf(fp, "NAM-Id(Voice SO):%x\n",req.pPrefVoiceSO->namID);
                fprintf(fp, "EVRC Capability :%x\n",req.pPrefVoiceSO->evrcCapability);
                fprintf(fp, "Home Page VSO   :%x\n",req.pPrefVoiceSO->homePageVoiceSO);
                fprintf(fp, "Home Orig VSO   :%x\n",req.pPrefVoiceSO->homeOrigVoiceSO);
                fprintf(fp, "Roam Orig VSO   :%x\n",req.pPrefVoiceSO->roamOrigVoiceSO);
            }
            req.pPrefVoiceDomain = pd->pPrefVoiceDomain;
            IFPRINTF(fp, "Voice Domain    :%x\n",req.pPrefVoiceDomain)

            resp.pAutoAnsStatus         = pd->pAutoAnsStatus;
            resp.pAirTimerStatus        = pd->pAirTimerStatus;
            resp.pRoamTimerStatus       = pd->pRoamTimerStatus;
            resp.pTTYConfigStatus       = pd->pTTYConfigStatus;
            resp.pPrefVoiceSOStatus     = pd->pPrefVoiceSOStatus;
            resp.pVoiceDomainPrefStatus = pd->pVoiceDomainPrefStatus;

            ULONG nRet = SLQSVoiceSetConfig( &req , &resp );

            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSVoiceSetConfig Successful\n");
                IFPRINTF(fp,"Auto Answer Outcome :%x\n",resp.pAutoAnsStatus)
                IFPRINTF(fp,"Air Timer Outcome   :%x\n",resp.pAirTimerStatus)
                IFPRINTF(fp,"Roam Timer Outcome  :%x\n",resp.pRoamTimerStatus)
                IFPRINTF(fp,"TTY Mode Outcome    :%x\n",resp.pTTYConfigStatus)
                IFPRINTF(fp,"Voice SO Outcome    :%x\n",resp.pPrefVoiceSOStatus)
                IFPRINTF(fp,"Voice Domain Outcome:%x\n",resp.pVoiceDomainPrefStatus)
            }
            tc++;
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSAnswerCall
 *
 * Purpose: Perform the tests that call the SLQSAnswerCall() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSAnswerCall( void )
{
    /* Test cases */
    SLQSAnswerCallTestCase_t SLQSAnswerCallTestCases[] =
    {
        {eQCWWAN_ERR_INVALID_ARG, 1,    NULL, "Invalid Argument" },
        {eQCWWAN_ERR_NONE,        2, &callId, "All Valid" }
    };

    FILE   *fp = NULL;
    fp = tfopen("../../voice/test/results/SLQSAnswerCall.txt", "w");

    if (fp)
    {
        BYTE tc = 0;

        BYTE ncases = sizeof( SLQSAnswerCallTestCases )/
                      sizeof( SLQSAnswerCallTestCases[0] );

        voiceAnswerCall objVoiceAnswerCall;
        while (tc < ncases )
        {
            SLQSAnswerCallTestCase_t *pd = &SLQSAnswerCallTestCases[tc];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tc + 1),
                        ncases,
                        pd->desc);

            objVoiceAnswerCall.pCallId = pd->pCallId;
            ULONG nRet = SLQSVoiceAnswerCall( &objVoiceAnswerCall );

            doprintreason( fp, nRet );

            if ( !nRet )
            {
                    fprintf(fp, "SLQSAnswerCall Successful\n");
                    IFPRINTF(fp, "Call ID is : %d \n",
                                  objVoiceAnswerCall.pCallId)
            }
            tc++;
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSVoiceGetCLIR
 *
 * Purpose: Perform the tests that call the SLQSVoiceGetCLIR() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSVoiceGetCLIR( void )
{
    local CLIRResp    clirresp;
    local WORD        failcause;
    local alphaIDInfo alphaidinfo;
    local BYTE        ccresulttype;
    local BYTE        respcallid;
    local ccSUPSType  ccsupstype;

    /* Test Cases */
    VoiceSLQSGetCLIRTestCase_t VoiceSLQSGetCLIRTestCases[]=
    {
        { eQCWWAN_ERR_NONE, 1, NULL, NULL, NULL, NULL, NULL, NULL,
        "Fetching None" },

        { eQCWWAN_ERR_NONE, 2, &clirresp, &failcause, &alphaidinfo,
        &ccresulttype, &respcallid, &ccsupstype,"Fetching All Values" }
    };

    FILE  *fp = tfopen("../../voice/test/results/slqsvoicegetclir.txt", "w");
    voiceGetCLIRResp resp;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( VoiceSLQSGetCLIRTestCases )/
                      sizeof( VoiceSLQSGetCLIRTestCases[0] );
        while ( tc < ncases )
        {
            VoiceSLQSGetCLIRTestCase_t *pd = &VoiceSLQSGetCLIRTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            resp.pCLIRResp    = pd->pCLIRResp;
            resp.pFailCause   = pd->pFailCause;
            resp.pAlphaIDInfo = pd->pAlphaIDInfo;
            resp.pCCResType   = pd->pCCResType;
            resp.pCallID      = pd->pCallID;
            resp.pCCSUPSType  = pd->pCCSUPSType;

            ULONG nRet = SLQSVoiceGetCLIR( &resp );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE != nRet )
            {
                IFPRINTF(fp, "Failure Cause      : %x\n",resp.pFailCause);
                continue;
            }

            fprintf(fp, "SLQSVoiceGetCLIR Successful\n");
            if ( NULL != resp.pCLIRResp )
            {
                fprintf(fp, "CLIR Response \n");
                fprintf(fp, "Activation Status  : %x\n",
                                resp.pCLIRResp->ActiveStatus);
                fprintf(fp, "Provision Status   : %x\n",
                                resp.pCLIRResp->ProvisionStatus);
            }
            if ( NULL != resp.pAlphaIDInfo )
            {
                doprintAlphaID( fp, resp.pAlphaIDInfo );
            }
            IFPRINTF(fp, "Call Control Result Type : %x\n",resp.pCCResType);
            IFPRINTF(fp, "Call Id            : %x\n",resp.pCallID);
            if ( NULL != resp.pCCSUPSType )
            {
                doprintccSupSType( fp, resp.pCCSUPSType );
            }
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSVoiceGetCLIP
 *
 * Purpose: Perform the tests that call the SLQSVoiceGetCLIP() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSVoiceGetCLIP( void )
{
    local CLIPResp    clipresp;
    local WORD        failcause;
    local alphaIDInfo alphaidinfo;
    local BYTE        ccresulttype;
    local BYTE        respcallid;
    local ccSUPSType  ccsupstype;

    /* Test Cases */
    VoiceSLQSGetCLIPTestCase_t VoiceSLQSGetCLIPTestCases[]=
    {
        { eQCWWAN_ERR_NONE, 1, NULL, NULL, NULL, NULL, NULL, NULL,
        "Fetching None" },

        { eQCWWAN_ERR_NONE, 2, &clipresp, &failcause, &alphaidinfo,
        &ccresulttype, &respcallid, &ccsupstype,"Fetching All Values" }
    };

    FILE  *fp = tfopen("../../voice/test/results/slqsvoicegetclip.txt", "w");
    voiceGetCLIPResp resp;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( VoiceSLQSGetCLIPTestCases )/
                      sizeof( VoiceSLQSGetCLIPTestCases[0] );
        while ( tc < ncases )
        {
            VoiceSLQSGetCLIPTestCase_t *pd = &VoiceSLQSGetCLIPTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            resp.pCLIPResp    = pd->pCLIPResp;
            resp.pFailCause   = pd->pFailCause;
            resp.pAlphaIDInfo = pd->pAlphaIDInfo;
            resp.pCCResType   = pd->pCCResType;
            resp.pCallID      = pd->pCallID;
            resp.pCCSUPSType  = pd->pCCSUPSType;

            ULONG nRet = SLQSVoiceGetCLIP( &resp );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE != nRet )
            {
                IFPRINTF(fp, "Failure Cause      : %x\n",resp.pFailCause);
                continue;
            }

            fprintf(fp, "SLQSVoiceGetCLIP Successful\n");
            if ( NULL != resp.pCLIPResp )
            {
                fprintf(fp, "CLIP Response \n");
                fprintf(fp, "Activation Status  : %x\n",
                                resp.pCLIPResp->ActiveStatus);
                fprintf(fp, "Provision Status   : %x\n",
                                resp.pCLIPResp->ProvisionStatus);
            }
            if ( NULL != resp.pAlphaIDInfo )
            {
                doprintAlphaID( fp, resp.pAlphaIDInfo );
            }
            IFPRINTF(fp, "Call Control Result Type : %x\n",resp.pCCResType);
            IFPRINTF(fp, "Call Id            : %x\n",resp.pCallID);
            if ( NULL != resp.pCCSUPSType )
            {
                doprintccSupSType( fp, resp.pCCSUPSType );
            }
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSVoiceGetCallWaiting
 *
 * Purpose: Perform the tests that call the SLQSVoiceGetCallWaiting() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSVoiceGetCallWaiting( void )
{
    local WORD        failcause;
    local alphaIDInfo alphaidinfo;
    local BYTE        ccresulttype;
    local BYTE        respcallid;
    local ccSUPSType  ccsupstype;

    /* Test Cases */
    VoiceSLQSGetCallWaitingTestCase_t VoiceSLQSGetCallWaitingTestCases[]=
    {
        { eQCWWAN_ERR_NONE, 1, &svcclass, &failcause, &alphaidinfo,
        &ccresulttype, &respcallid, &ccsupstype,"Service Class Specified" },

        { eQCWWAN_ERR_NONE, 2, NULL, &failcause, &alphaidinfo,
        &ccresulttype, &respcallid, &ccsupstype,
        "No Optional Parameter Specified" }
    };

    FILE  *fp = tfopen("../../voice/test/results/slqsvoicegetcallwaiting.txt", "w");
    voiceGetCallWaitInfo resp;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( VoiceSLQSGetCallWaitingTestCases )/
                      sizeof( VoiceSLQSGetCallWaitingTestCases[0] );
        while ( tc < ncases )
        {
            VoiceSLQSGetCallWaitingTestCase_t *pd =
                                &VoiceSLQSGetCallWaitingTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            /* Preventing global from being modified*/
            svcclass = 0;
            resp.pSvcClass    = pd->pSvcClass;
            IFPRINTF(fp, "Service Call       : %x\n",resp.pSvcClass)
            resp.pFailCause   = pd->pFailCause;
            resp.pAlphaIDInfo = pd->pAlphaIDInfo;
            resp.pCCResType   = pd->pCCResType;
            resp.pCallID      = pd->pCallID;
            resp.pCCSUPSType  = pd->pCCSUPSType;

            ULONG nRet = SLQSVoiceGetCallWaiting( &resp );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE != nRet )
            {
                IFPRINTF(fp, "Failure Cause      : %x\n",resp.pFailCause);
                continue;
            }

            fprintf(fp, "SLQSVoiceGetCallWaiting Successful\n");
            IFPRINTF(fp, "Service Call(resp) : %x\n",resp.pSvcClass)
            if ( NULL != resp.pAlphaIDInfo )
            {
                doprintAlphaID( fp, resp.pAlphaIDInfo );
            }
            IFPRINTF(fp, "Call Control Result Type : %x\n",resp.pCCResType);
            IFPRINTF(fp, "Call Id            : %x\n",resp.pCallID);
            if ( NULL != resp.pCCSUPSType )
            {
                doprintccSupSType( fp, resp.pCCSUPSType );
            }
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSVoiceGetCallBarring
 *
 * Purpose: Perform the tests that call the SLQSVoiceGetCallBarring() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSVoiceGetCallBarring( void )
{
    local WORD        failcause;
    local alphaIDInfo alphaidinfo;
    local BYTE        ccresulttype;
    local BYTE        respcallid;
    local ccSUPSType  ccsupstype;

    /* Test Cases */
    VoiceSLQSGetCallBarringTestCase_t VoiceSLQSGetCallBarringTestCases[]=
    {
        { eQCWWAN_ERR_NONE, 1, reason, &svcclass, &failcause, &alphaidinfo,
        &ccresulttype, &respcallid, &ccsupstype,"Service Class Specified" },

        { eQCWWAN_ERR_NONE, 2, reason, NULL, &failcause, &alphaidinfo,
        &ccresulttype, &respcallid, &ccsupstype,
        "No Optional Parameter Specified" }
    };

    FILE  *fp = tfopen("../../voice/test/results/slqsvoicegetcallbarring.txt", "w");
    voiceGetCallBarringReq  req;
    voiceGetCallBarringResp resp;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( VoiceSLQSGetCallBarringTestCases )/
                      sizeof( VoiceSLQSGetCallBarringTestCases[0] );
        while ( tc < ncases )
        {
            VoiceSLQSGetCallBarringTestCase_t *pd =
                                &VoiceSLQSGetCallBarringTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            req.reason        = pd->reason;
            fprintf(fp, "Call Barred Reason : %x\n",req.reason);
            /* Preventing global from being modified*/
            svcclass = 0;
            req.pSvcClass     = pd->pSvcClass;
            IFPRINTF(fp, "Service Call       : %x\n",req.pSvcClass)

            resp.pFailCause   = pd->pFailCause;
            resp.pAlphaIDInfo = pd->pAlphaIDInfo;
            resp.pCCResType   = pd->pCCResType;
            resp.pCallID      = pd->pCallID;
            resp.pCCSUPSType  = pd->pCCSUPSType;

            ULONG nRet = SLQSVoiceGetCallBarring( &req, &resp );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE != nRet )
            {
                IFPRINTF(fp, "Failure Cause      : %x\n",resp.pFailCause);
                continue;
            }

            fprintf(fp, "SLQSVoiceGetCallBarring Successful\n");
            IFPRINTF(fp, "Service Call(resp) : %x\n",resp.pSvcClass)
            if ( NULL != resp.pAlphaIDInfo )
            {
                doprintAlphaID( fp, resp.pAlphaIDInfo );
            }
            IFPRINTF(fp, "Call Control Result Type : %x\n",resp.pCCResType);
            IFPRINTF(fp, "Call Id            : %x\n",resp.pCallID);
            if ( NULL != resp.pCCSUPSType )
            {
                doprintccSupSType( fp, resp.pCCSUPSType );
            }
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSVoiceGetCallForwardingStatus
 *
 * Purpose: Perform the tests that call the SLQSVoiceGetCallForwardingStatus()
 *          API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSVoiceGetCallForwardingStatus( void )
{
    local getCallFWInfo    getcallfwinfo;
    local WORD             failcause;
    local alphaIDInfo      alphaidinfo;
    local BYTE             ccresulttype;
    local BYTE             respcallid;
    local ccSUPSType       ccsupstype;
    local getCallFWExtInfo getcallfwextinfo;

    /* Test Cases */
    VoiceSLQSGetCallFWStatusTestCase_t VoiceSLQSGetCallFWStatusTestCases[]=
    {
        { eQCWWAN_ERR_NONE, 1, callFWReason, &svcclass, &getcallfwinfo, &failcause,
        &alphaidinfo, &ccresulttype, &respcallid, &ccsupstype,
        &getcallfwextinfo,"Service Class Specified" },

        { eQCWWAN_ERR_NONE, 2, callFWReason, NULL, &getcallfwinfo, &failcause,
        &alphaidinfo, &ccresulttype, &respcallid, &ccsupstype,
        &getcallfwextinfo,"No Optional Parameter Specified" }
    };

    FILE  *fp = tfopen("../../voice/test/results/slqsvoicegetcallforwardingstatus.txt", "w");
    voiceGetCallFWReq  req;
    voiceGetCallFWResp resp;
    BYTE               lcount,lIcount;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( VoiceSLQSGetCallFWStatusTestCases )/
                      sizeof( VoiceSLQSGetCallFWStatusTestCases[0] );
        while ( tc < ncases )
        {
            VoiceSLQSGetCallFWStatusTestCase_t *pd =
                                &VoiceSLQSGetCallFWStatusTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            req.Reason        = pd->reason;
            fprintf(fp, "Call Forwarding Reason  : %x\n",req.Reason);

            req.pSvcClass    = pd->pSvcClass;
            IFPRINTF(fp, "Service Call       : %x\n",req.pSvcClass)

            resp.pGetCallFWInfo    = pd->pGetCallFWInfo;
            resp.pFailCause        = pd->pFailCause;
            resp.pAlphaIDInfo      = pd->pAlphaIDInfo;
            resp.pCCResType        = pd->pCCResType;
            resp.pCallID           = pd->pCallID;
            resp.pCCSUPSType       = pd->pCCSUPSType;
            resp.pGetCallFWExtInfo = pd->pGetCallFWExtInfo;

            ULONG nRet = SLQSVoiceGetCallForwardingStatus( &req, &resp );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE != nRet )
            {
                IFPRINTF(fp, "Failure Cause      : %x\n",resp.pFailCause);
                continue;
            }

            fprintf(fp, "SLQSVoiceGetCallForwardingStatus Successful\n");
            if ( NULL != resp.pGetCallFWInfo )
            {
                fprintf(fp, "Instances of Call Forwarding Info : %x\n",
                resp.pGetCallFWInfo->numInstances);
                for ( lcount = 0;lcount<resp.pGetCallFWInfo->numInstances;lcount++)
                {
                    callFWInfo *temp = &resp.pGetCallFWInfo->CallFWInfo[lcount];
                    fprintf(fp, "\tService Status   : %x \n",temp->SvcStatus);
                    fprintf(fp, "\tService Class    : %x \n",temp->SvcClass);
                    fprintf(fp, "\tNum Len          : %x \n",temp->numLen);
                    fprintf(fp, "\tNumber           : ");
                    for ( lIcount=0; lIcount<temp->numLen ;lIcount++ )
                    {
                        fprintf(fp,"%c",temp->number[lIcount]);
                    }
                    fprintf(fp,"\n");
                    fprintf(fp, "\tNo Reply Timer   : %x \n",temp->noReplyTimer);
                }
            }
            if ( NULL != resp.pAlphaIDInfo )
            {
                doprintAlphaID( fp, resp.pAlphaIDInfo );
            }
            IFPRINTF(fp, "Call Control Result Type : %x\n",resp.pCCResType);
            IFPRINTF(fp, "Call Id            : %x\n",resp.pCallID);
            if ( NULL != resp.pCCSUPSType )
            {
                doprintccSupSType( fp, resp.pCCSUPSType );
            }
            if ( NULL != resp.pGetCallFWExtInfo )
            {
                fprintf(fp, "Instances of Call Forwarding Ext Info : %x\n",
                resp.pGetCallFWExtInfo->numInstances);
                for ( lcount = 0;lcount<resp.pGetCallFWExtInfo->numInstances;lcount++)
                {
                    callFWExtInfo *temp = &resp.pGetCallFWExtInfo->CallFWExtInfo[lcount];
                    fprintf(fp, "\tService Status   : %x \n",temp->SvcStatus);
                    fprintf(fp, "\tService Class    : %x \n",temp->SvcClass);
                    fprintf(fp, "\tNo Reply Timer   : %x \n",temp->noReplyTimer);
                    fprintf(fp, "\tPI               : %x \n",temp->PI);
                    fprintf(fp, "\tSI               : %x \n",temp->SI);
                    fprintf(fp, "\tNum Type         : %x \n",temp->numType);
                    fprintf(fp, "\tNum Plan         : %x \n",temp->numPlan);
                    fprintf(fp, "\tNum Len          : %x \n",temp->numLen);
                    fprintf(fp, "\tNumber           : ");
                    for ( lIcount=0; lIcount<temp->numLen ;lIcount++ )
                    {
                        fprintf(fp,"%c",temp->number[lIcount]);
                    }
                    fprintf(fp,"\n");
                }
            }
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSVoiceSetCallBarringPassword
 *
 * Purpose: Perform the tests that call the SLQSVoiceSetCallBarringPassword()
 *          API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSVoiceSetCallBarringPassword( void )
{
    local WORD        failcause;
    local alphaIDInfo alphaidinfo;
    local BYTE        ccresulttype;
    local BYTE        respcallid;
    local ccSUPSType  ccsupstype;

    /* Test Cases */
    VoiceSLQSSetCallBarringPwdTestCase_t VoiceSLQSSetCallBarringPwdTestCases[]=
    {
        { eQCWWAN_ERR_NONE, 1, callBarReason, pInvalidPassword, pNewPassword,
        pNewPassword, &failcause, NULL, NULL, NULL, NULL,"Invalid Password" },

        { eQCWWAN_ERR_NONE, 2, callBarReason, pOldPassword, pNewPassword,
        pWrongPassword, &failcause, NULL, NULL, NULL, NULL,
        "New Passwords do not match" },

        { eQCWWAN_ERR_NONE, 3, callBarReason, pOldPassword, pNewPassword,
        pNewPassword, &failcause, &alphaidinfo, &ccresulttype, &respcallid,
        &ccsupstype,"Valid Arguments" }
    };

    FILE  *fp = tfopen("../../voice/test/results/slqsvoicegetcallforwardingstatus.txt", "w");
    voiceSetCallBarringPwdInfo req;
    voiceSetCallBarringPwdResp resp;
    BYTE                       lcount;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( VoiceSLQSSetCallBarringPwdTestCases )/
                      sizeof( VoiceSLQSSetCallBarringPwdTestCases[0] );
        while ( tc < ncases )
        {
            VoiceSLQSSetCallBarringPwdTestCase_t *pd =
                                &VoiceSLQSSetCallBarringPwdTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            req.Reason        = pd->reason;
            fprintf(fp, "Call Barring Reason : %x\n",req.Reason);
            fprintf(fp, "Old Password        :");
            for( lcount = 0 ; lcount < PASSWORD_LENGTH ; lcount++ )
            {
                req.oldPasswd[lcount] = pd->pOldPassword[lcount];
                fprintf(fp, "%c",req.oldPasswd[lcount]);
            }
            fprintf(fp, "\n");

            fprintf(fp, "New Password        :");
            for( lcount = 0 ; lcount < PASSWORD_LENGTH ; lcount++ )
            {
                req.newPasswd[lcount] = pd->pNewPassword[lcount];
                fprintf(fp, "%c",req.newPasswd[lcount]);
            }
            fprintf(fp, "\n");

            fprintf(fp, "New Password Again  :");
            for( lcount = 0 ; lcount < PASSWORD_LENGTH ; lcount++ )
            {
                req.newPasswdAgain[lcount] = pd->pNewPasswordAgain[lcount];
                fprintf(fp, "%c",req.newPasswdAgain[lcount]);
            }
            fprintf(fp, "\n");

            resp.pFailCause   = pd->pFailCause;
            resp.pAlphaIDInfo = pd->pAlphaIDInfo;
            resp.pCCResType   = pd->pCCResType;
            resp.pCallID      = pd->pCallID;
            resp.pCCSUPSType  = pd->pCCSUPSType;

            ULONG nRet = SLQSVoiceSetCallBarringPassword( &req, &resp );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE != nRet )
            {
                IFPRINTF(fp, "Failure Cause      : %x\n",resp.pFailCause);
                continue;
            }

            fprintf(fp, "SLQSVoiceSetCallBarringPassword Successful\n");
            if ( NULL != resp.pAlphaIDInfo )
            {
                doprintAlphaID( fp, resp.pAlphaIDInfo );
            }
            IFPRINTF(fp, "Call Control Result Type : %x\n",resp.pCCResType);
            IFPRINTF(fp, "Call Id            : %x\n",resp.pCallID);
            if ( NULL != resp.pCCSUPSType )
            {
                doprintccSupSType( fp, resp.pCCSUPSType );
            }
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSVoiceGetCallInfo
 *
 * Purpose: Perform the tests that call the SLQSVoiceGetCallInfo() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSVoiceGetCallInfo( void )
{
    local UUSInfo         getcalluusinfo;
    local callInfo        callinfo;
    local remotePartyNum  remotepartynum;
    local WORD            srvopt;
    local BYTE            voiceprivacy;
    local BYTE            otasptatus;
    local remotePartyName remotepartyname;
    local BYTE            alerttype;
    local connectNumInfo  connectnuminfo;
    local diagInfo        diaginfo;
    local ULONG           alertingpattern;

    /* Test Cases  */
    VoiceSLQSGetCallInfoTestCase_t VoiceSLQSGetCallInfoTestCases[] =
    {
        {eQCWWAN_ERR_NONE, 1, getcallinfocallId, &callinfo, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL,"Fetching Only Call Info"},

        {eQCWWAN_ERR_NONE, 2, getcallinfocallId, NULL, &remotepartynum, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL,"Fetching Only Remote Party Numbers"},

        {eQCWWAN_ERR_NONE, 3, getcallinfocallId, NULL, NULL, &srvopt, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL,"Fetching Only Service Option"},

        {eQCWWAN_ERR_NONE, 4, getcallinfocallId, NULL, NULL, NULL, &voiceprivacy, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL,"Fetching Only Voice Privacy"},

        {eQCWWAN_ERR_NONE, 5, getcallinfocallId, NULL, NULL, NULL, NULL, &otasptatus, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL,"Fetching Only OTASP Status"},

        {eQCWWAN_ERR_NONE, 6, getcallinfocallId, NULL, NULL, NULL, NULL, NULL, &remotepartyname,
        NULL, NULL, NULL, NULL, NULL, NULL,"Fetching Only Remote Party Names"},

        {eQCWWAN_ERR_NONE, 7, getcallinfocallId, NULL, NULL, NULL, NULL, NULL, NULL,
        &getcalluusinfo, NULL, NULL, NULL, NULL, NULL,"Fetching Only UUS Info"},

        {eQCWWAN_ERR_NONE, 8, getcallinfocallId, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, &alerttype, NULL, NULL, NULL, NULL,"Fetching Only Alert Type"},

        {eQCWWAN_ERR_NONE, 9, getcallinfocallId, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, &alphaidinfo, NULL, NULL, NULL,"Fetching only Alpha Id Info"},

        {eQCWWAN_ERR_NONE, 10, getcallinfocallId, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, &connectnuminfo, NULL, NULL,"Fetching only connected numbers info"},

        {eQCWWAN_ERR_NONE, 11, getcallinfocallId, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, &diaginfo, NULL,"Fetching only Diagnostic Info"},

        {eQCWWAN_ERR_NONE, 12, getcallinfocallId, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, &alertingpattern,"Fetching only Alerting Pattern"},

        {eQCWWAN_ERR_NONE, 13, getcallinfocallId, &callinfo, &remotepartynum, &srvopt,
        &voiceprivacy, &otasptatus, &remotepartyname, &getcalluusinfo, &alerttype,
        &alphaidinfo, &connectnuminfo, &diaginfo, &alertingpattern,"All Valid" }
    };

    FILE  *fp = tfopen("../../voice/test/results/slqsvoicegetcallinfo.txt", "w");
    voiceCallInfoReq  req;
    voiceCallInfoResp resp;
    BYTE              lcount=0;


    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( VoiceSLQSGetCallInfoTestCases )/
                      sizeof( VoiceSLQSGetCallInfoTestCases[0] );
        while ( tc < ncases )
        {
            VoiceSLQSGetCallInfoTestCase_t *pd = &VoiceSLQSGetCallInfoTestCases[tc];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tc + 1),
                        ncases,
                        pd->desc);

            req.callID = pd->callID;
            fprintf(fp, "Call Id          : %x \n",req.callID);

            resp.pCallInfo        = pd->pCallInfo;
            resp.pRemotePartyNum  = pd->pRemotePartyNum;
            resp.pSrvOpt          = pd->pSrvOpt;
            resp.pVoicePrivacy    = pd->pVoicePrivacy;
            resp.pOTASPStatus     = pd->pOTASPStatus;
            resp.pRemotePartyName = pd->pRemotePartyName;
            resp.pUUSInfo         = pd->pUUSInfo;
            resp.pAlertType       = pd->pAlertType;
            resp.pAlphaIDInfo     = pd->pAlphaIDInfo;
            resp.pConnectNumInfo  = pd->pConnectNumInfo;
            resp.pDiagInfo        = pd->pDiagInfo;
            resp.pAlertingPattern = pd->pAlertingPattern;

            ULONG nRet = SLQSVoiceGetCallInfo( &req , &resp );

            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSVoiceGetCallInfo Successful\n");

                if ( NULL != resp.pCallInfo )
                {
                    fprintf(fp, "Call Id(response): %x \n",resp.pCallInfo->callID);
                    fprintf(fp, "Call State       : %x \n",resp.pCallInfo->callState);
                    fprintf(fp, "Call Type        : %x \n",resp.pCallInfo->callType);
                    fprintf(fp, "Direction        : %x \n",resp.pCallInfo->direction);
                    fprintf(fp, "Mode             : %x \n",resp.pCallInfo->mode);
                }

                if ( NULL != resp.pRemotePartyNum )
                {
                    fprintf(fp, "PI               : %x \n",resp.pRemotePartyNum->presentationInd);
                    fprintf(fp, "Num Len          : %d \n",resp.pRemotePartyNum->numLen);
                    fprintf(fp, "Remote Party Number : ");
                    for ( lcount=0; lcount<resp.pRemotePartyNum->numLen ;lcount++ )
                    {
                        fprintf(fp,"%c",resp.pRemotePartyNum->remPartyNumber[lcount]);
                    }
                    fprintf(fp,"\n");
                }
                IFPRINTF(fp, "Service Option   : %x \n",resp.pSrvOpt)
                IFPRINTF(fp, "Voice Privacy    : %x \n",resp.pVoicePrivacy)
                IFPRINTF(fp, "OTASP Status     : %x \n",resp.pOTASPStatus)

                if ( NULL != resp.pRemotePartyName )
                {
                    fprintf(fp, "Name PI          : %x \n",resp.pRemotePartyName->namePI);
                    fprintf(fp, "Coding Scheme    : %x \n",resp.pRemotePartyName->codingScheme);
                    fprintf(fp, "Name Len         : %d \n",resp.pRemotePartyName->nameLen);
                    fprintf(fp, "Remote Party Name : ");
                    for ( lcount=0; lcount<resp.pRemotePartyName->nameLen ;lcount++ )
                    {
                        fprintf(fp,"%c",resp.pRemotePartyName->callerName[lcount]);
                    }
                    fprintf(fp,"\n");
                }
                if ( NULL != resp.pUUSInfo )
                {
                    fprintf(fp, "UUS Type         : %x \n",resp.pUUSInfo->UUSType);
                    fprintf(fp, "UUS DCS          : %x \n",resp.pUUSInfo->UUSDcs);
                    fprintf(fp, "UUS Datalen      : %d \n",resp.pUUSInfo->UUSDatalen);
                    fprintf(fp, "UUS Data         : ");
                    for ( lcount=0; lcount<resp.pUUSInfo->UUSDatalen ;lcount++ )
                    {
                        fprintf(fp,"%c",resp.pUUSInfo->UUSData[lcount]);
                    }
                    fprintf(fp,"\n");
                }
                IFPRINTF(fp, "Alerting Type    : %x \n",resp.pAlertType)
                if ( NULL != resp.pAlphaIDInfo )
                {
                    doprintAlphaID( fp, resp.pAlphaIDInfo );
                }
                if ( NULL != resp.pConnectNumInfo )
                {
                    fprintf(fp, "connumPI         : %x \n",resp.pConnectNumInfo->numPresInd);
                    fprintf(fp, "connumSI         : %x \n",resp.pConnectNumInfo->screeningInd);
                    fprintf(fp, "Num Type         : %x \n",resp.pConnectNumInfo->numType);
                    fprintf(fp, "Num Plan         : %x \n",resp.pConnectNumInfo->numPlan);
                    fprintf(fp, "Caller ID Len    : %d \n",resp.pConnectNumInfo->callerIDLen);
                    fprintf(fp, "Caller ID        : ");
                    for ( lcount=0; lcount<resp.pConnectNumInfo->callerIDLen ;lcount++ )
                    {
                        fprintf(fp,"%c",resp.pConnectNumInfo->callerID[lcount]);
                    }
                    fprintf(fp,"\n");
                }
                if ( NULL != resp.pDiagInfo )
                {
                    fprintf(fp, "Diagnostic Info Len: %x \n",resp.pDiagInfo->diagInfoLen);
                    fprintf(fp, "Diagnostic Information : ");
                    for ( lcount=0; lcount<resp.pDiagInfo->diagInfoLen ;lcount++ )
                    {
                        fprintf(fp,"%c",resp.pDiagInfo->diagnosticInfo[lcount]);
                    }
                    fprintf(fp,"\n");
                }
                IFPRINTF(fp, "Alerting Pattern : %lu \n",resp.pAlertingPattern);
            }
            tc++;
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSVoiceGetAllCallInfo
 *
 * Purpose: Perform the tests that call the SLQSVoiceGetAllCallInfo() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSVoiceGetAllCallInfo( void )
{
    local arrCallInfo        arrcallinfo;
    local arrRemotePartyNum  arrremotepartynum;
    local arrRemotePartyName arrremotepartyname;
    local arrAlertingType    arralertingtype;
    local arrUUSInfo         arruusinfo;
    local arrSvcOption       arrsvcoption;
    local BYTE               voiceprivacy;
    local BYTE               otasptatus;
    local arrCallEndReason   arrcallendreason;
    local arrAlphaID         arralphaid;
    local arrConnectPartyNum arrconnectpartynum;
    local arrDiagInfo        arrdiaginfo;
    local arrCalledPartyNum  arrcalledpartynum;
    local arrRedirPartyNum   arrredirpartynum;
    local arrAlertingPattern arralertingpattern;

    /* Test Cases  */
    VoiceSLQSGetAllCallInfoTestCase_t VoiceSLQSGetAllCallInfoTestCases[] =
    {
        {eQCWWAN_ERR_NONE , 1, &arrcallinfo, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        "Fetching Only Call Info"},

        {eQCWWAN_ERR_NONE , 2, NULL, &arrremotepartynum, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        "Fetching Only Remote Party Numbers"},

        {eQCWWAN_ERR_NONE , 3, NULL, NULL, &arrremotepartyname, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        "Fetching Only Remote Party Names"},

        {eQCWWAN_ERR_NONE , 4, NULL, NULL, NULL, &arralertingtype, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        "Fetching Only Alerting Type"},

        {eQCWWAN_ERR_NONE , 5, NULL, NULL, NULL, NULL, &arruusinfo, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        "Fetching Only UUS Info"},

        {eQCWWAN_ERR_NONE , 6, NULL, NULL, NULL, NULL, NULL, &arrsvcoption, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        "Fetching Only Service Option"},

        {eQCWWAN_ERR_NONE , 7, NULL, NULL, NULL, NULL, NULL, NULL, &otasptatus,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        "Fetching Only OTASP Status"},

        {eQCWWAN_ERR_NONE , 8, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        &voiceprivacy, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        "Fetching Only Voice Privacy"},

        {eQCWWAN_ERR_NONE , 9, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, &arrcallendreason, NULL, NULL, NULL, NULL, NULL, NULL,
        "Fetching Only Call End Reason"},

        {eQCWWAN_ERR_NONE , 10, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, &arralphaid, NULL, NULL, NULL, NULL, NULL,
        "Fetching only Alpha Id"},

        {eQCWWAN_ERR_NONE , 11, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, &arrconnectpartynum, NULL, NULL, NULL, NULL,
        "Fetching only Connected Party Numbers"},

        {eQCWWAN_ERR_NONE , 12, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, &arrdiaginfo, NULL, NULL, NULL,
        "Fetching only Diagnostic Info"},

        {eQCWWAN_ERR_NONE , 13, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, &arrcalledpartynum, NULL, NULL,
        "Fetching only Called Party Numbers"},

        {eQCWWAN_ERR_NONE , 14, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, &arrredirpartynum, NULL,
        "Fetching only Redirected Party Numbers"},

        {eQCWWAN_ERR_NONE , 15, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, &arralertingpattern,
        "Fetching only Alerting Pattern"},

        {eQCWWAN_ERR_NONE , 16, &arrcallinfo, &arrremotepartynum,
        &arrremotepartyname, &arralertingtype, &arruusinfo, &arrsvcoption,
        &otasptatus, &voiceprivacy, &arrcallendreason, &arralphaid,
        &arrconnectpartynum, &arrdiaginfo, &arrcalledpartynum,
        &arrredirpartynum, &arralertingpattern, "All Valid" }
    };

    FILE  *fp = tfopen("../../voice/test/results/slqsvoicegetallcallinfo.txt", "w");
    voiceGetAllCallInfo resp;
    BYTE                lcount,lIcount;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( VoiceSLQSGetAllCallInfoTestCases )/
                      sizeof( VoiceSLQSGetAllCallInfoTestCases[0] );
        while ( tc < ncases )
        {
            VoiceSLQSGetAllCallInfoTestCase_t *pd = &VoiceSLQSGetAllCallInfoTestCases[tc];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tc + 1),
                        ncases,
                        pd->desc);

            resp.pArrCallInfo        = pd->pArrCallInfo;
            resp.pArrRemotePartyNum  = pd->pArrRemotePartyNum;
            resp.pArrRemotePartyName = pd->pArrRemotePartyName;
            resp.pArrAlertingType    = pd->pArrAlertingType;
            resp.pArrUUSInfo         = pd->pArrUUSInfo;
            resp.pArrSvcOption       = pd->pArrSvcOption;
            resp.pOTASPStatus        = pd->pOTASPStatus;
            resp.pVoicePrivacy       = pd->pVoicePrivacy;
            resp.pArrCallEndReason   = pd->pArrCallEndReason;
            resp.pArrAlphaID         = pd->pArrAlphaID;
            resp.pArrConnectPartyNum = pd->pArrConnectPartyNum;
            resp.pArrDiagInfo        = pd->pArrDiagInfo;
            resp.pArrCalledPartyNum  = pd->pArrCalledPartyNum;
            resp.pArrRedirPartyNum   = pd->pArrRedirPartyNum;
            resp.pArrAlertingPattern = pd->pArrAlertingPattern;

            ULONG nRet = SLQSVoiceGetAllCallInfo( &resp );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSVoiceGetAllCallInfo Successful\n");

                if ( NULL != resp.pArrCallInfo )
                {
                    fprintf(fp, "Instances of call Info : %d\n",
                    resp.pArrCallInfo->numInstances);
                    for ( lcount = 0;lcount<resp.pArrCallInfo->numInstances;lcount++)
                    {
                        callInfo temp = resp.pArrCallInfo->getAllCallInfo[lcount].Callinfo;
                        fprintf(fp, "Call Id          : %x \n",temp.callID);
                        fprintf(fp, "\tCall State       : %x \n",temp.callState);
                        fprintf(fp, "\tCall Type        : %x \n",temp.callType);
                        fprintf(fp, "\tDirection        : %x \n",temp.direction);
                        fprintf(fp, "\tMode             : %x \n",temp.mode);
                        fprintf(fp, "\tMPI              : %x \n",
                        resp.pArrCallInfo->getAllCallInfo[lcount].isEmpty);
                        fprintf(fp, "\tALS              : %x \n",
                        resp.pArrCallInfo->getAllCallInfo[lcount].ALS);
                    }
                }
                if ( NULL != resp.pArrRemotePartyNum )
                {
                    fprintf(fp, "Instances of remote party numbers : %d\n",
                    resp.pArrRemotePartyNum->numInstances);
                    for ( lcount = 0;lcount<resp.pArrRemotePartyNum->numInstances;lcount++)
                    {
                        fprintf(fp, "Call Id          : %x \n",
                          resp.pArrRemotePartyNum->RmtPtyNum[lcount].callID);
                        remotePartyNum temp =
                          resp.pArrRemotePartyNum->RmtPtyNum[lcount].RemotePartyNum;
                        fprintf(fp, "\tPI               : %x \n",temp.presentationInd);
                        fprintf(fp, "\tNum Len          : %d \n",temp.numLen);
                        fprintf(fp, "\tRemote Party Number : ");
                        for ( lIcount=0; lIcount<temp.numLen ;lIcount++ )
                        {
                            fprintf(fp,"%c",temp.remPartyNumber[lIcount]);
                        }
                        fprintf(fp,"\n");
                    }
                }
                if ( NULL != resp.pArrRemotePartyName )
                {
                    fprintf(fp, "Instances of remote party names : %d\n",
                    resp.pArrRemotePartyName->numInstances);
                    for ( lcount = 0;lcount<resp.pArrRemotePartyName->numInstances;lcount++)
                    {
                        fprintf(fp, "Call Id          : %x \n",
                          resp.pArrRemotePartyName->GetAllCallRmtPtyName[lcount].callID);
                        remotePartyName temp =
                          resp.pArrRemotePartyName->GetAllCallRmtPtyName[lcount].RemotePartyName;
                        fprintf(fp, "\tName PI          : %x \n",temp.namePI);
                        fprintf(fp, "\tCoding Scheme    : %x \n",temp.codingScheme);
                        fprintf(fp, "\tName Len         : %d \n",temp.nameLen);
                        fprintf(fp, "\tRemote Party Name : ");
                        for ( lIcount=0; lIcount<temp.nameLen ;lIcount++ )
                        {
                            fprintf(fp,"%c",temp.callerName[lIcount]);
                        }
                        fprintf(fp,"\n");
                    }
                }
                if ( NULL != resp.pArrAlertingType )
                {
                    fprintf(fp, "Instances of Alerting Types : %d\n",
                    resp.pArrAlertingType->numInstances);
                    for ( lcount = 0;lcount<resp.pArrAlertingType->numInstances;lcount++)
                    {
                        fprintf(fp, "Call Id          : %x \t",
                          resp.pArrAlertingType->callID[lcount]);
                        fprintf(fp, "Alerting Type    : %x \n",
                          resp.pArrAlertingType->AlertingType[lcount]);
                    }
                }
                if ( NULL != resp.pArrUUSInfo )
                {
                    fprintf(fp, "Instances of UUS Info : %d\n",
                    resp.pArrUUSInfo->numInstances);
                    for ( lcount = 0;lcount<resp.pArrUUSInfo->numInstances;lcount++)
                    {
                        fprintf(fp, "Call Id          : %x \n",
                          resp.pArrUUSInfo->AllCallsUUSInfo[lcount].callID);
                        UUSInfo temp = resp.pArrUUSInfo->AllCallsUUSInfo[lcount].uusInfo;
                        fprintf(fp, "\tUUS Type         : %x \n",temp.UUSType);
                        fprintf(fp, "\tUUS DCS          : %x \n",temp.UUSDcs);
                        fprintf(fp, "\tUUS Datalen      : %d \n",temp.UUSDatalen);
                        fprintf(fp, "\tUUS Data         : ");
                        for ( lIcount=0; lIcount<temp.UUSDatalen ;lIcount++ )
                        {
                            fprintf(fp,"%c",temp.UUSData[lIcount]);
                        }
                        fprintf(fp,"\n");
                    }
                }
                if ( NULL != resp.pArrSvcOption )
                {
                    fprintf(fp, "Instances of Service Option : %d\n",
                    resp.pArrSvcOption->numInstances);
                    for ( lcount = 0;lcount<resp.pArrSvcOption->numInstances;lcount++)
                    {
                        fprintf(fp, "Call Id          : %x \t",
                          resp.pArrSvcOption->callID[lcount]);
                        fprintf(fp, "Service Option   : %d \n",
                          resp.pArrSvcOption->srvOption[lcount]);
                    }
                }
                IFPRINTF(fp, "OTASP Status     : %x \n",resp.pOTASPStatus)
                IFPRINTF(fp, "Voice Privacy    : %x \n",resp.pVoicePrivacy)

                if ( NULL != resp.pArrCallEndReason )
                {
                    fprintf(fp, "Instances of Call End Reason : %d\n",
                    resp.pArrCallEndReason->numInstances);
                    for ( lcount = 0;lcount<resp.pArrCallEndReason->numInstances;lcount++)
                    {
                        fprintf(fp, "Call Id          : %x \t",
                          resp.pArrCallEndReason->callID[lcount]);
                        fprintf(fp, "Call End Reason  : %d \n",
                          resp.pArrCallEndReason->callEndReason[lcount]);
                    }
                }
                if ( NULL != resp.pArrAlphaID )
                {
                    fprintf(fp, "Instances of Alpha Identifier : %d\n",
                    resp.pArrAlphaID->numInstances);
                    for ( lcount = 0;lcount<resp.pArrAlphaID->numInstances;lcount++)
                    {
                        fprintf(fp, "Call Id          : %x \n",
                          resp.pArrAlphaID->allCallsAlphaIDInfoArr[lcount].callID);
                        alphaIDInfo temp =
                          resp.pArrAlphaID->allCallsAlphaIDInfoArr[lcount].AlphaIDInfo;
                        fprintf(fp, "\tAlpha DCS        : %x \n",temp.alphaDcs);
                        fprintf(fp, "\tAlpha Len        : %d \n",temp.alphaLen);
                        fprintf(fp, "\tAlpha Text       : ");
                        for ( lIcount=0; lIcount<temp.alphaLen ;lIcount++ )
                        {
                            fprintf(fp,"%c",temp.alphaText[lIcount]);
                        }
                        fprintf(fp,"\n");
                    }
                }
                if ( NULL != resp.pArrConnectPartyNum )
                {
                    fprintf(fp, "Instances of Connected Party Numbers : %d\n",
                    resp.pArrConnectPartyNum->numInstances);
                    for ( lcount = 0;lcount<resp.pArrConnectPartyNum->numInstances;lcount++)
                    {
                        peerNumberInfo temp =
                          resp.pArrConnectPartyNum->ConnectedPartyNum[lcount];
                        fprintf(fp, "Call Id          : %x \n",temp.callID);
                        fprintf(fp, "\tNum PI           : %x \n",temp.numPI);
                        fprintf(fp, "\tNum SI           : %x \n",temp.numSI);
                        fprintf(fp, "\tNum Type         : %x \n",temp.numType);
                        fprintf(fp, "\tNum Plan         : %x \n",temp.numPlan);
                        fprintf(fp, "\tNumber Len       : %d \n",temp.numLen);
                        fprintf(fp, "\tNumber           : ");
                        for ( lIcount=0; lIcount<temp.numLen ;lIcount++ )
                        {
                            fprintf(fp,"%c",temp.number[lIcount]);
                        }
                        fprintf(fp,"\n");
                    }
                }
                if ( NULL != resp.pArrDiagInfo )
                {
                    fprintf(fp, "Instances of Diagnostic Information : %d\n",
                    resp.pArrDiagInfo->numInstances);
                    for ( lcount = 0;lcount<resp.pArrDiagInfo->numInstances;lcount++)
                    {
                        fprintf(fp, "Call Id          : %x \n",
                        resp.pArrDiagInfo->DiagInfo[lcount].callID);
                        diagInfo temp = resp.pArrDiagInfo->DiagInfo[lcount].DiagInfo;
                        fprintf(fp, "\tDiagnostic Info Len: %d \n",temp.diagInfoLen);
                        fprintf(fp, "\tDiagnostic Information : ");
                        for ( lIcount=0; lIcount<temp.diagInfoLen ;lIcount++ )
                        {
                            fprintf(fp,"%c",temp.diagnosticInfo[lIcount]);
                        }
                        fprintf(fp,"\n");
                    }
                }
                if ( NULL != resp.pArrCalledPartyNum )
                {
                    fprintf(fp, "Instances of Called Party Numbers : %d\n",
                    resp.pArrCalledPartyNum->numInstances);
                    for ( lcount = 0;lcount<resp.pArrCalledPartyNum->numInstances;lcount++)
                    {
                        peerNumberInfo temp =
                          resp.pArrCalledPartyNum->CalledPartyNum[lcount];
                        fprintf(fp, "Call Id          : %x \n",temp.callID);
                        fprintf(fp, "\tNum PI           : %x \n",temp.numPI);
                        fprintf(fp, "\tNum SI           : %x \n",temp.numSI);
                        fprintf(fp, "\tNum Type         : %x \n",temp.numType);
                        fprintf(fp, "\tNum Plan         : %x \n",temp.numPlan);
                        fprintf(fp, "\tNumber Len       : %d \n",temp.numLen);
                        fprintf(fp, "\tNumber           : ");
                        for ( lIcount=0; lIcount<temp.numLen ;lIcount++ )
                        {
                            fprintf(fp,"%c",temp.number[lIcount]);
                        }
                        fprintf(fp,"\n");
                    }
                }
                if ( NULL != resp.pArrRedirPartyNum )
                {
                    fprintf(fp, "Instances of Redirected Party Numbers : %d\n",
                    resp.pArrRedirPartyNum->numInstances);
                    for ( lcount = 0;lcount<resp.pArrRedirPartyNum->numInstances;lcount++)
                    {
                        peerNumberInfo temp =
                          resp.pArrRedirPartyNum->RedirPartyNum[lcount];
                        fprintf(fp, "Call Id          : %x \n",temp.callID);
                        fprintf(fp, "\tNum PI           : %x \n",temp.numPI);
                        fprintf(fp, "\tNum SI           : %x \n",temp.numSI);
                        fprintf(fp, "\tNum Type         : %x \n",temp.numType);
                        fprintf(fp, "\tNum Plan         : %x \n",temp.numPlan);
                        fprintf(fp, "\tNumber Len       : %d \n",temp.numLen);
                        fprintf(fp, "\tNumber           : ");
                        for ( lIcount=0; lIcount<temp.numLen ;lIcount++ )
                        {
                            fprintf(fp,"%c",temp.number[lIcount]);
                        }
                        fprintf(fp,"\n");
                    }
                }
                if ( NULL != resp.pArrAlertingPattern )
                {
                    fprintf(fp, "Instances of Alerting Pattern : %d\n",
                    resp.pArrAlertingPattern->numInstances);
                    for ( lcount = 0;lcount<resp.pArrAlertingPattern->numInstances;lcount++)
                    {
                        fprintf(fp, "Call Id          : %x \t",
                          resp.pArrAlertingPattern->callID[lcount]);
                        fprintf(fp, "Call End Reason  : %lu \n",
                          resp.pArrAlertingPattern->alertingPattern[lcount]);
                    }
                }
            }
            tc++;
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSVoiceManageCalls
 *
 * Purpose: Perform the tests that call the SLQSVoiceManageCalls() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSVoiceManageCalls( void )
{
    /* Define the test cases to be executed */
    VoiceSLQSVoiceManageCallsTestCase_t \
        VoiceSLQSVoiceManageCallsTestCases[] =
    {
        { eQCWWAN_ERR_NONE, 1, 0x02, NULL, NULL, " OUT Param Invalid" },
        { eQCWWAN_ERR_NONE, 2, 0x09, &CallID, &FailCause, "All Valid" },
    };

    ULONG  nRet;
    FILE   *fp = NULL;
    voiceManageCallsReq  objVMCallsReq;
    voiceManageCallsResp objVMCallsResp;
    fp = tfopen("TestReport/slqsvoiceManageCalls.txt", "w");

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( VoiceSLQSVoiceManageCallsTestCases )/
                      sizeof( VoiceSLQSVoiceManageCallsTestCases[0] );
        while ( tc < ncases )
        {
            VoiceSLQSVoiceManageCallsTestCase_t *pd =
                     &VoiceSLQSVoiceManageCallsTestCases[tc];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tc + 1),
                        ncases,
                        pd->desc);

            objVMCallsReq.SUPSType    = pd->SUPSType;
            objVMCallsReq.pCallID     = pd->pCallID;
            objVMCallsResp.pFailCause = pd->pFailCause;

            nRet = SLQSVoiceManageCalls( &objVMCallsReq,
                                         &objVMCallsResp );

            fprintf(fp, "Return Code: %lu\n", nRet);
            doprintreason( fp, nRet );

            if ( !nRet )
            {
                IFPRINTF(fp, "Failure Cause =%d\n",
                               pd->pFailCause);
            }
            tc++;
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSVoiceBurstDTMF
 *
 * Purpose: Perform the tests that call the SLQSVoiceBurstDTMF() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSVoiceBurstDTMF( void )
{
    /* Test Cases */
    SLQSVoiceBurstDTMFTestCase_t SLQSVoiceBurstDTMFTestCases[] =
    {
        {eQCWWAN_ERR_INVALID_ARG, 1, DTMFInfoTest1, &DTMFLengthsTest1,
                 "InValid Case: Mandatory Parameter Passed as NULL" },
        {eQCWWAN_ERR_NONE,        2, DTMFInfoTest2,
                                     &DTMFLengthsTest2, "All Valid" }
    };

    FILE *fp = tfopen("../../voice/test/results/SLQSVoiceBurstDTMF.txt", "w");

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSVoiceBurstDTMFTestCases )/
                      sizeof( SLQSVoiceBurstDTMFTestCases[0] );

        while ( tc < ncases )
        {
            SLQSVoiceBurstDTMFTestCase_t *pd =
                     &SLQSVoiceBurstDTMFTestCases[tc];
            BYTE lCount = 0;

            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tc + 1),
                        ncases,
                        pd->desc);

            /* Print parameters passed to API */
            fprintf(fp,"Parameter Set:\n");

            IFPRINTF(fp,"Call ID = %d\n",pd->BurstDTMFInfo.pCallID);

            fprintf(fp, "Digit Count: %d\n",
                            pd->BurstDTMFInfo.digitCnt);
            fprintf(fp, "DTMF digit buffer:");

            for( lCount = 0; lCount <= pd->BurstDTMFInfo.digitCnt; lCount++ )
            {
                fprintf(fp, "%c",pd->BurstDTMFInfo.pDigitBuff[lCount]);
            }
            fprintf(fp,"\n");
            if ( NULL != pd->pBurstDTMFLengths )
            {
                fprintf(fp, "DTMF On Length: = %d\n",
                        pd->pBurstDTMFLengths->DTMFPulseWidth);
                fprintf(fp, "DTMF Off Length: = %d\n",
                        pd->pBurstDTMFLengths->DTMFInterdigitInterval);
            }
            voiceBurstDTMFInfo objvoiceBurstDTMFInfo;

            objvoiceBurstDTMFInfo.BurstDTMFInfo    = pd->BurstDTMFInfo;
            objvoiceBurstDTMFInfo.pBurstDTMFLengths = pd->pBurstDTMFLengths;

            ULONG nRet = SLQSVoiceBurstDTMF( &objvoiceBurstDTMFInfo);

            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSVoiceBurstDTMF Successful\n");
                IFPRINTF(fp,"Call ID is : %d \n",pd->BurstDTMFInfo.pCallID);
            }
            tc++;
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSVoiceStartContDTMF
 *
 * Purpose: Perform the tests that call the doSLQSVoiceStartContDTMF() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSVoiceStartContDTMF( void )
{
    /* Test Cases */
    SLQSVoiceStartContDTMFTestCase_t SLQSVoiceStartContDTMFTestCases[] =
    {
        {eQCWWAN_ERR_INVALID_ARG, 1, NULL, '1', " Invalid Argument" },
        {eQCWWAN_ERR_NONE,        1, &dtmfCallId, '2', "All Valid" }
    };

    FILE *fp = tfopen("../../voice/test/results/SLQSVoiceStartContDTMF.txt", "w");

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSVoiceStartContDTMFTestCases )/
                      sizeof( SLQSVoiceStartContDTMFTestCases[0] );
        while ( tc < ncases )
        {
            voiceContDTMFinfo objvoiceContDTMFinfo;
            SLQSVoiceStartContDTMFTestCase_t *pd =
                 &SLQSVoiceStartContDTMFTestCases[tc];

            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tc + 1),
                        ncases,
                        pd->desc);
            fprintf(fp,"Parameter Set:\n");
            IFPRINTF(fp,"Call ID:  %d\n",pd->pCallID);
            fprintf(fp, "Digit: %c\n",pd->DTMFdigit);

            objvoiceContDTMFinfo.pCallID     = pd->pCallID;
            objvoiceContDTMFinfo.DTMFdigit   = pd->DTMFdigit;

            ULONG nRet = SLQSVoiceStartContDTMF( &objvoiceContDTMFinfo );

            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSVoiceStartContDTMF Successful\n");
                IFPRINTF(fp,"Call ID is : %d \n",pd->pCallID);
            }
            tc++;
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSVoiceStopContDTMF
 *
 * Purpose: Perform the tests that call the doSLQSVoiceStopContDTMF() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSVoiceStopContDTMF( void )
{
    /* Test Cases */
    SLQSVoiceStopContDTMFTestCase_t SLQSVoiceStopContDTMFTestCases[] =
    {
        {eQCWWAN_ERR_INVALID_ARG, 1, 0,       "Invalid Argument" },
        {eQCWWAN_ERR_NONE,        1, dtmfCallId, "All Valid" }
    };

    FILE *fp = tfopen("../../voice/test/results/SLQSVoiceStopContDTMF.txt", "w");

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSVoiceStopContDTMFTestCases )/
                      sizeof( SLQSVoiceStopContDTMFTestCases[0] );
        while ( tc < ncases )
        {
            voiceStopContDTMFinfo objvoiceStopContDTMFinfo;
            SLQSVoiceStopContDTMFTestCase_t *pd =
                &SLQSVoiceStopContDTMFTestCases[tc];

            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tc + 1),
                        ncases,
                        pd->desc);

            fprintf(fp,"Parameter Set:\n");
            fprintf(fp,"Call ID = %d\n",pd->callID);
            objvoiceStopContDTMFinfo.callID = pd->callID;

            ULONG nRet = SLQSVoiceStopContDTMF( &objvoiceStopContDTMFinfo );

            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSVoiceStopContDTMF Successful\n");
                fprintf(fp, "Call ID is : %d \n", pd->callID);
            }
            tc++;
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSVoiceSendFlash
 *
 * Purpose: Perform the tests that call the SLQSVoiceSendFlash() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSVoiceSendFlash( void )
{
    /* Test Cases */
    SLQSVoiceSendFlashTestCase_t SLQSVoiceSendFlashTestCases[]=
    {
        { eQCWWAN_ERR_INVALID_ARG, 1, NULL, FlashPayLd, &flashType,
        "Invalid Argument" },

        { eQCWWAN_ERR_NONE, 2, &flashCallID, NULL, &flashType,
        "Flash Type - Simple" },

        { eQCWWAN_ERR_NONE, 3, &flashCallID, NULL, &flashType1,
        "Flash Type - Activate Answer Hold " },

        { eQCWWAN_ERR_NONE, 4, &flashCallID, NULL, &flashType2,
        "Flash Type - Deactivate Answer Hold " },

        { eQCWWAN_ERR_NONE, 5, &flashCallID, FlashPayLd, NULL,
        "Valid Parameters" },

        { eQCWWAN_ERR_NONE, 6, &flashCallID, NULL, NULL,
        "Setting only the Call Id" },

        { eQCWWAN_ERR_NONE, 7, &flashCallID, FlashPayLd, &flashType,
        "Setting All the Parameters" }
    };

    FILE  *fp = tfopen("../../voice/test/results/slqsvoicesendflash.txt", "w");
    voiceFlashInfo req;
    BYTE           lCount;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSVoiceSendFlashTestCases )/
                      sizeof( SLQSVoiceSendFlashTestCases[0] );
        while ( tc < ncases )
        {
            SLQSVoiceSendFlashTestCase_t *pd = &SLQSVoiceSendFlashTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            req.pCallID     = pd->pCallID;
            IFPRINTF(fp, "Call Id       : %x\n",req.pCallID);
            req.pFlashPayLd = pd->pFlashPayLd;
            if ( NULL != req.pFlashPayLd )
            {
                fprintf(fp, "Flash PayLoad : ");
                for ( lCount=0 ; req.pFlashPayLd[lCount]!='\0';lCount++)
                {
                    fprintf(fp, "%c",req.pFlashPayLd[lCount]);
                }
                fprintf(fp, "\n");
            }
            req.pFlashType  = pd->pFlashType;
            IFPRINTF(fp, "Flash Type    : %x\n",req.pFlashType);

            ULONG nRet = SLQSVoiceSendFlash( &req );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE != nRet )
            {
                continue;
            }

            fprintf(fp, "SLQSVoiceSendFlash Successful\n");
            IFPRINTF(fp, "Call Id  (response): %x\n",req.pCallID);
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSVoiceSetPreferredPrivacy
 *
 * Purpose: Perform the tests that call the SLQSVoiceSetPreferredPrivacy() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSVoiceSetPreferredPrivacy( void )
{
    /* Test Cases */
    SLQSVoiceSetPrefPrivacyTestCase_t SLQSVoiceSetPrefPrivacyTestCases[]=
    {
        { eQCWWAN_ERR_NONE, 1, 0x00, "Set Standard Privacy" },

        { eQCWWAN_ERR_NONE, 2, 0x01, "Set Enhanced Privacy" },
    };

    FILE  *fp = tfopen("../../voice/test/results/slqsvoicesendflash.txt", "w");
    voiceSetPrefPrivacy req;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSVoiceSetPrefPrivacyTestCases )/
                      sizeof( SLQSVoiceSetPrefPrivacyTestCases[0] );
        while ( tc < ncases )
        {
            SLQSVoiceSetPrefPrivacyTestCase_t *pd =
                              &SLQSVoiceSetPrefPrivacyTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            req.privacyPref     = pd->privacyPref;
            fprintf( fp, "Privacy Preference : %x\n",req.privacyPref);

            ULONG nRet = SLQSVoiceSetPreferredPrivacy( &req );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSVoiceSetPrefferedPrivacy Successful\n");
            }
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSVoiceIndicationRegister
 *
 * Purpose: Perform the tests that call the SLQSVoiceIndicationRegister() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSVoiceIndicationRegister( void )
{
    /* Test Cases */
        SLQSVoiceIndicationRegisterTestCase_t SLQSVoiceIndicationRegisterTestCases[]=
    {
        { eQCWWAN_ERR_NONE, 1, NULL, NULL, NULL,
        "Error: At Least One Optional Parameter to be provided" },

        { eQCWWAN_ERR_NONE, 2, &dtmfevent, NULL, NULL,
        "Set DTMF Event" },

        { eQCWWAN_ERR_NONE, 3, NULL, &voiceprivacyevent, NULL,
        "Set Voice Privacy Event" },

        { eQCWWAN_ERR_NONE, 4, NULL, NULL, &supsnotifyevent,
        "Set Sups Notify Event" },

        { eQCWWAN_ERR_NONE, 5, &dtmfevent, &voiceprivacyevent, &supsnotifyevent,
        "Set All" },
    };

    FILE  *fp = tfopen("../../voice/test/results/slqsvoiceindicationregister.txt", "w");
    voiceIndicationRegisterInfo req;
    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSVoiceIndicationRegisterTestCases )/
                      sizeof( SLQSVoiceIndicationRegisterTestCases[0] );
        while ( tc < ncases )
        {
                SLQSVoiceIndicationRegisterTestCase_t *pd =
                                    &SLQSVoiceIndicationRegisterTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            req.pRegDTMFEvents         = pd->pRegDTMFEvents;
            req.pRegVoicePrivacyEvents = pd->pRegVoicePrivacyEvents;
            req.pSuppsNotifEvents      = pd->pSuppsNotifEvents;
            IFPRINTF(fp, "Reg DTMF Event               : %x\n",
                                                 req.pRegDTMFEvents);
            IFPRINTF(fp, "Reg Voice Privacy Event      : %x\n",
                                                 req.pRegVoicePrivacyEvents);
            IFPRINTF(fp, "Reg Supps Notification Event : %x\n",
                                                 req.pSuppsNotifEvents);

            ULONG nRet = SLQSVoiceIndicationRegister( &req );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSVoiceIndicationRegister Successful\n");
            }
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSVoiceSetConfig
 *
 * Purpose: Perform the tests that call the SLQSVoiceSetConfig() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSVoiceGetConfig( void )
{
    local BYTE         autoansstat;
    local airTimer     airtimercnt;
    local roamTimer    roamtimercnt;
    local BYTE         currttymode;
    local prefVoiceSO  currprefvoiceso;
    local curAMRConfig curramrconfig;
    local BYTE         currvoiceprivacy;
    local BYTE         currvoicedomain;

    /* Test Cases */
    VoiceSLQSGetConfigTestCase_t VoiceSLQSGetConfigTestCases[] =
    {
        { eQCWWAN_ERR_NONE, 1,  &setgetconf, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL,
        &autoansstat, &airtimercnt, &roamtimercnt, &currttymode, &currprefvoiceso,
        &curramrconfig, &currvoiceprivacy, &currvoicedomain, "Get Auto Answer Status" },

        { eQCWWAN_ERR_NONE, 2,  NULL, &setgetconf, NULL, NULL,
        NULL, NULL, NULL, &setgetconf, NULL,
        &autoansstat, &airtimercnt, &roamtimercnt, &currttymode, &currprefvoiceso,
        &curramrconfig, &currvoiceprivacy, &currvoicedomain, "Get Air Timer" },

        { eQCWWAN_ERR_NONE, 3,  NULL, NULL, &setgetconf, NULL,
        NULL, NULL, NULL, NULL, NULL,
        &autoansstat, &airtimercnt, &roamtimercnt, &currttymode, &currprefvoiceso,
        &curramrconfig, &currvoiceprivacy, &currvoicedomain, "Get Roam Timer" },

        { eQCWWAN_ERR_NONE, 4,  NULL, NULL, NULL, &setgetconf,
        NULL, NULL, NULL, NULL, NULL,
        &autoansstat, &airtimercnt, &roamtimercnt, &currttymode, &currprefvoiceso,
        &curramrconfig, &currvoiceprivacy, &currvoicedomain, "Get TTY Mode" },

        { eQCWWAN_ERR_NONE, 5,  NULL, NULL, NULL, NULL,
        &setgetconf, NULL, NULL, NULL, NULL,
        &autoansstat, &airtimercnt, &roamtimercnt, &currttymode, &currprefvoiceso,
        &curramrconfig, &currvoiceprivacy, &currvoicedomain, "Get Preferred Voice SO" },

        { eQCWWAN_ERR_NONE, 6,  NULL, NULL, NULL, NULL,
        NULL, &setgetconf, NULL, NULL, NULL,
        &autoansstat, &airtimercnt, &roamtimercnt, &currttymode, &currprefvoiceso,
        &curramrconfig, &currvoiceprivacy, &currvoicedomain, "Get AMR Status" },

        { eQCWWAN_ERR_NONE, 7,  NULL, NULL, NULL, NULL,
        NULL, NULL, &setgetconf, NULL, NULL,
        &autoansstat, &airtimercnt, &roamtimercnt, &currttymode, &currprefvoiceso,
        &curramrconfig, &currvoiceprivacy, &currvoicedomain, "Get Preferred Voice Privacy" },

        { eQCWWAN_ERR_NONE, 8,  NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, &setgetconf, NULL,
        &autoansstat, &airtimercnt, &roamtimercnt, &currttymode, &currprefvoiceso,
        &curramrconfig, &currvoiceprivacy, &currvoicedomain, "Providing NAM ID" },

        { eQCWWAN_ERR_NONE, 9,  NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, &setgetconf,
        &autoansstat, &airtimercnt, &roamtimercnt, &currttymode, &currprefvoiceso,
        &curramrconfig, &currvoiceprivacy, &currvoicedomain, "Get Preferred Voice Domain" },

        { eQCWWAN_ERR_NONE, 10,  &setgetconf, &setgetconf, &setgetconf, &setgetconf,
        &setgetconf, &setgetconf, &setgetconf, &setgetconf, &setgetconf,
        &autoansstat, &airtimercnt, &roamtimercnt, &currttymode, &currprefvoiceso,
        &curramrconfig, &currvoiceprivacy, &currvoicedomain, "Getting All" }
    };

    FILE  *fp = tfopen("../../voice/test/results/SLQSVoiceSetConfig.txt", "w");
    voiceGetConfigReq  req;
    voiceGetConfigResp resp;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( VoiceSLQSGetConfigTestCases )/
                      sizeof( VoiceSLQSGetConfigTestCases[0] );
        while ( tc < ncases )
        {
            VoiceSLQSGetConfigTestCase_t *pd = &VoiceSLQSGetConfigTestCases[tc];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tc + 1),
                        ncases,
                        pd->desc);

            req.pAutoAnswer       = pd->pAutoAnswer;
            req.pAirTimer         = pd->pAirTimer;
            req.pRoamTimer        = pd->pRoamTimer;
            req.pTTYMode          = pd->pTTYMode;
            req.pPrefVoiceSO      = pd->pPrefVoiceSO;
            req.pAMRStatus        = pd->pAMRStatus;
            req.pPrefVoicePrivacy = pd->pPrefVoicePrivacy;
            req.pNamID            = pd->pNamID;
            req.pVoiceDomainPref  = pd->pVoiceDomainPref;
            IFPRINTF(fp, "Auto Answer        :%x\n",req.pAutoAnswer)
            IFPRINTF(fp, "Air Timer          :%x\n",req.pAirTimer)
            IFPRINTF(fp, "Roam Timer         :%x\n",req.pRoamTimer)
            IFPRINTF(fp, "TTY Mode           :%x\n",req.pTTYMode)
            IFPRINTF(fp, "Pref Voice SO      :%x\n",req.pPrefVoiceSO)
            IFPRINTF(fp, "AMR Status         :%x\n",req.pAMRStatus)
            IFPRINTF(fp, "Pref Voice Privacy :%x\n",req.pPrefVoicePrivacy)
            IFPRINTF(fp, "Nam ID             :%x\n",req.pNamID)
            IFPRINTF(fp, "Pref Voice Domain  :%x\n",req.pVoiceDomainPref)

            resp.pAutoAnswerStat      = pd->pAutoAnswerStat;
            resp.pAirTimerCnt         = pd->pAirTimerCnt;
            resp.pRoamTimerCnt        = pd->pRoamTimerCnt;
            resp.pCurrTTYMode         = pd->pCurrTTYMode;
            resp.pCurPrefVoiceSO      = pd->pCurPrefVoiceSO;
            resp.pCurAMRConfig        = pd->pCurAMRConfig;
            resp.pCurVoicePrivacyPref = pd->pCurVoicePrivacyPref;
            resp.pCurVoiceDomainPref  = pd->pCurVoiceDomainPref;

            ULONG nRet = SLQSVoiceGetConfig( &req , &resp );

            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSVoiceGetConfig Successful\n");
                IFPRINTF(fp,"Auto Answer Status :%x\n",resp.pAutoAnswerStat)
                if ( NULL != resp.pAirTimerCnt )
                {
                    fprintf(fp, "NAM-Id(AIR)     :%x\n",resp.pAirTimerCnt->namID);
                    fprintf(fp, "Air Timer       :%lx\n",resp.pAirTimerCnt->airTimerValue);
                }
                if ( NULL != resp.pRoamTimerCnt )
                {
                    fprintf(fp, "NAM-Id(ROAM)    :%x\n",resp.pRoamTimerCnt->namID);
                    fprintf(fp, "ROAM Timer      :%lx\n",resp.pRoamTimerCnt->roamTimerValue);
                }
                IFPRINTF(fp,"Current TTY Mode:%x\n",resp.pCurrTTYMode)
                if ( NULL != resp.pCurPrefVoiceSO )
                {
                    fprintf(fp, "NAM-Id(Voice SO):%x\n",resp.pCurPrefVoiceSO->namID);
                    fprintf(fp, "EVRC Capability :%x\n",resp.pCurPrefVoiceSO->evrcCapability);
                    fprintf(fp, "Home Page VSO   :%x\n",resp.pCurPrefVoiceSO->homePageVoiceSO);
                    fprintf(fp, "Home Orig VSO   :%x\n",resp.pCurPrefVoiceSO->homeOrigVoiceSO);
                    fprintf(fp, "Roam Orig VSO   :%x\n",resp.pCurPrefVoiceSO->roamOrigVoiceSO);
                }
                if ( NULL != resp.pCurAMRConfig )
                {
                    fprintf(fp, "GSM AMR Status  :%x\n",resp.pCurAMRConfig->gsmAmrStat);
                    fprintf(fp, "WCDMA AMR Status:%x\n",resp.pCurAMRConfig->wcdmaAmrStat);
                }
                IFPRINTF(fp,"Current Voice Privacy:%x\n",resp.pCurVoicePrivacyPref)
                IFPRINTF(fp,"Current Voice Domain :%x\n",resp.pCurVoiceDomainPref)
            }
            tc++;
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSVoiceOrigUSSDNoWait
 *
 * Purpose: Perform the tests that call the SLQSVoiceOrigUSSDNoWait() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSVoiceOrigUSSDNoWait( void )
{
    /* Test Cases */
    SLQSVoiceOrigUSSDNoWaitTestCase_t SLQSVoiceOrigUSSDNoWaitTestCases[]=
    {
        { eQCWWAN_ERR_NONE, 1, ussinfo, "Valid Arguments" },
    };

    FILE  *fp = tfopen("../../voice/test/results/slqsvoiceorigussdnowait.txt", "w");
    voiceOrigUSSDNoWaitInfo req;
    WORD                    lCount;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSVoiceOrigUSSDNoWaitTestCases )/
                      sizeof( SLQSVoiceOrigUSSDNoWaitTestCases[0] );
        while ( tc < ncases )
        {
            SLQSVoiceOrigUSSDNoWaitTestCase_t *pd = &SLQSVoiceOrigUSSDNoWaitTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            req.USSInformation = pd->USSInformation;
            fprintf( fp, "USS DCS   : %x\n",req.USSInformation.ussDCS);
            fprintf( fp, "USS Length: %x\n",req.USSInformation.ussLen);
            fprintf( fp, "USS Data  : ");
            for ( lCount = 0 ; lCount < req.USSInformation.ussLen ; lCount++ )
            {
                fprintf( fp, "%c",req.USSInformation.ussData[lCount]);
            }
            fprintf( fp, "\n");

            ULONG nRet = SLQSVoiceOrigUSSDNoWait( &req );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSVoiceOrigUSSDNoWait Successful\n");
            }
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSVoiceBindSubscription
 *
 * Purpose: Perform the tests that call the SLQSVoiceBindSubscription() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSVoiceBindSubscription( void )
{
    /* Test Cases */
    SLQSVoiceBindSubscriptionTestCase_t SLQSVoiceBindSubscriptionTestCases[]=
    {
        { eQCWWAN_ERR_NONE, 1, SUBS_TYPE_PRIMARY, "Subs Type - Primary" },
        { eQCWWAN_ERR_NONE, 2, SUBS_TYPE_SECONDARY, "Subs Type - Secondary" },
        { eQCWWAN_ERR_NONE, 3, SUBS_TYPE_INVALID, "Invalid Subscription" }
    };

    FILE  *fp = tfopen("../../voice/test/results/slqsvoicebindsubscription.txt", "w");
    voiceBindSubscriptionInfo req;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSVoiceBindSubscriptionTestCases )/
                      sizeof( SLQSVoiceBindSubscriptionTestCases[0] );
        while ( tc < ncases )
        {
            SLQSVoiceBindSubscriptionTestCase_t *pd = &SLQSVoiceBindSubscriptionTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            req.subsType = pd->subsType;
            fprintf( fp, "Subscription Type : %x\n",req.subsType);

            ULONG nRet = SLQSVoiceBindSubscription( &req );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSVoiceBindSubscription Successful\n");
            }
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSVoiceALSSetLineSwitching
 *
 * Purpose: Perform the tests that call the SLQSVoiceALSSetLineSwitching() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSVoiceALSSetLineSwitching( void )
{
    /* Test Cases */
    SLQSVoiceALSSetLineSwitchingTestCase_t SLQSVoiceALSSetLineSwitchingTestCases[]=
    {
        { eQCWWAN_ERR_NONE, 1, LINE_SWITCHING_NOT_ALLOWED, "Not Allowed" },
        { eQCWWAN_ERR_NONE, 2, LINE_SWITCHING_ALLOWED, "Allowed" },
    };

    FILE  *fp = tfopen("../../voice/test/results/slqsvoicealssetlineswitching.txt", "w");
    voiceALSSetLineSwitchInfo req;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSVoiceALSSetLineSwitchingTestCases )/
                      sizeof( SLQSVoiceALSSetLineSwitchingTestCases[0] );
        while ( tc < ncases )
        {
            SLQSVoiceALSSetLineSwitchingTestCase_t *pd = &SLQSVoiceALSSetLineSwitchingTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            req.switchOption = pd->switchOption;
            fprintf( fp, "Switch Option : %x\n",req.switchOption);

            ULONG nRet = SLQSVoiceALSSetLineSwitching( &req );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSVoiceALSSetLineSwitching Successful\n");
            }
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSVoiceALSSelectLine
 *
 * Purpose: Perform the tests that call the SLQSVoiceALSSelectLine() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSVoiceALSSelectLine( void )
{
    /* Test Cases */
    SLQSVoiceALSSelectLineTestCase_t SLQSVoiceALSSelectLineTestCases[]=
    {
        { eQCWWAN_ERR_NONE, 1, ALS_LINE1, "ALS Line1" },
        { eQCWWAN_ERR_NONE, 2, ALS_LINE2, "ALS Line2" }
    };

    FILE  *fp = tfopen("../../voice/test/results/slqsvoicealsselectline.txt", "w");
    voiceALSSelectLineInfo req;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSVoiceALSSelectLineTestCases )/
                      sizeof( SLQSVoiceALSSelectLineTestCases[0] );
        while ( tc < ncases )
        {
            SLQSVoiceALSSelectLineTestCase_t *pd = &SLQSVoiceALSSelectLineTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            req.lineValue = pd->lineValue;
            fprintf( fp, "ALS Line Value : %x\n",req.lineValue);

            ULONG nRet = SLQSVoiceALSSelectLine( &req );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSVoiceALSSelectLine Successful\n");
            }
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSVoiceGetCOLP
 *
 * Purpose: Perform the tests that call the SLQSVoiceGetCOLP() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSVoiceGetCOLP( void )
{
    local COLPResp    colpresp;
    local WORD        failcause;
    local alphaIDInfo alphaidinfo;
    local BYTE        ccresulttype;
    local BYTE        respcallid;
    local ccSUPSType  ccsupstype;

    /* Test Cases */
    VoiceSLQSGetCOLPTestCase_t VoiceSLQSGetCOLPTestCases[]=
    {
        { eQCWWAN_ERR_NONE, 1, NULL, NULL, NULL, NULL, NULL, NULL,
        "Fetching None" },

        { eQCWWAN_ERR_NONE, 2, &colpresp, &failcause, &alphaidinfo,
        &ccresulttype, &respcallid, &ccsupstype,"Fetching All Values" }
    };

    FILE  *fp = tfopen("../../voice/test/results/slqsvoicegetcolp.txt", "w");
    voiceGetCOLPResp resp;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( VoiceSLQSGetCOLPTestCases )/
                      sizeof( VoiceSLQSGetCOLPTestCases[0] );
        while ( tc < ncases )
        {
            VoiceSLQSGetCOLPTestCase_t *pd = &VoiceSLQSGetCOLPTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            resp.pCOLPResp    = pd->pCOLPResp;
            resp.pFailCause   = pd->pFailCause;
            resp.pAlphaIDInfo = pd->pAlphaIDInfo;
            resp.pCCResType   = pd->pCCResType;
            resp.pCallID      = pd->pCallID;
            resp.pCCSUPSType  = pd->pCCSUPSType;

            ULONG nRet = SLQSVoiceGetCOLP( &resp );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE != nRet )
            {
                IFPRINTF(fp, "Failure Cause      : %x\n",resp.pFailCause);
                continue;
            }

            fprintf(fp, "SLQSVoiceGetCOLP Successful\n");
            if ( NULL != resp.pCOLPResp )
            {
                fprintf(fp, "COLP Response \n");
                fprintf(fp, "Activation Status  : %x\n",
                                resp.pCOLPResp->ActiveStatus);
                fprintf(fp, "Provision Status   : %x\n",
                                resp.pCOLPResp->ProvisionStatus);
            }
            if ( NULL != resp.pAlphaIDInfo )
            {
                doprintAlphaID( fp, resp.pAlphaIDInfo );
            }
            IFPRINTF(fp, "Call Control Result Type : %x\n",resp.pCCResType);
            IFPRINTF(fp, "Call Id            : %x\n",resp.pCallID);
            if ( NULL != resp.pCCSUPSType )
            {
                doprintccSupSType( fp, resp.pCCSUPSType );
            }
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSVoiceGetCOLR
 *
 * Purpose: Perform the tests that call the SLQSVoiceGetCOLR() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSVoiceGetCOLR( void )
{
    local COLRResp    colrresp;
    local WORD        failcause;
    local alphaIDInfo alphaidinfo;
    local BYTE        ccresulttype;
    local BYTE        respcallid;
    local ccSUPSType  ccsupstype;

    /* Test Cases */
    VoiceSLQSGetCOLRTestCase_t VoiceSLQSGetCOLRTestCases[]=
    {
        { eQCWWAN_ERR_NONE, 1, NULL, NULL, NULL, NULL, NULL, NULL,
        "Fetching None" },

        { eQCWWAN_ERR_NONE, 2, &colrresp, &failcause, &alphaidinfo,
        &ccresulttype, &respcallid, &ccsupstype,"Fetching All Values" }
    };

    FILE  *fp = tfopen("../../voice/test/results/slqsvoicegetcolr.txt", "w");
    voiceGetCOLRResp resp;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( VoiceSLQSGetCOLRTestCases )/
                      sizeof( VoiceSLQSGetCOLRTestCases[0] );
        while ( tc < ncases )
        {
            VoiceSLQSGetCOLRTestCase_t *pd = &VoiceSLQSGetCOLRTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            resp.pCOLRResp    = pd->pCOLRResp;
            resp.pFailCause   = pd->pFailCause;
            resp.pAlphaIDInfo = pd->pAlphaIDInfo;
            resp.pCCResType   = pd->pCCResType;
            resp.pCallID      = pd->pCallID;
            resp.pCCSUPSType  = pd->pCCSUPSType;

            ULONG nRet = SLQSVoiceGetCOLR( &resp );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE != nRet )
            {
                IFPRINTF(fp, "Failure Cause      : %x\n",resp.pFailCause);
                continue;
            }

            fprintf(fp, "SLQSVoiceGetCOLR Successful\n");
            if ( NULL != resp.pCOLRResp )
            {
                fprintf(fp, "COLR Response \n");
                fprintf(fp, "Activation Status  : %x\n",
                                resp.pCOLRResp->ActiveStatus);
                fprintf(fp, "Provision Status   : %x\n",
                                resp.pCOLRResp->ProvisionStatus);
            }
            if ( NULL != resp.pAlphaIDInfo )
            {
                doprintAlphaID( fp, resp.pAlphaIDInfo );
            }
            IFPRINTF(fp, "Call Control Result Type : %x\n",resp.pCCResType);
            IFPRINTF(fp, "Call Id            : %x\n",resp.pCallID);
            if ( NULL != resp.pCCSUPSType )
            {
                doprintccSupSType( fp, resp.pCCSUPSType );
            }
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSVoiceGetCNAP
 *
 * Purpose: Perform the tests that call the SLQSVoiceGetCNAP() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSVoiceGetCNAP( void )
{
    local CNAPResp    cnapresp;
    local WORD        failcause;
    local alphaIDInfo alphaidinfo;
    local BYTE        ccresulttype;
    local BYTE        respcallid;
    local ccSUPSType  ccsupstype;

    /* Test Cases */
    VoiceSLQSGetCNAPTestCase_t VoiceSLQSGetCNAPTestCases[]=
    {
        { eQCWWAN_ERR_NONE, 1, NULL, NULL, NULL, NULL, NULL, NULL,
        "Fetching None" },

        { eQCWWAN_ERR_NONE, 2, &cnapresp, &failcause, &alphaidinfo,
        &ccresulttype, &respcallid, &ccsupstype,"Fetching All Values" }
    };

    FILE  *fp = tfopen("../../voice/test/results/slqsvoicegetcnap.txt", "w");
    voiceGetCNAPResp resp;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( VoiceSLQSGetCNAPTestCases )/
                      sizeof( VoiceSLQSGetCNAPTestCases[0] );
        while ( tc < ncases )
        {
            VoiceSLQSGetCNAPTestCase_t *pd = &VoiceSLQSGetCNAPTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            resp.pCNAPResp    = pd->pCNAPResp;
            resp.pFailCause   = pd->pFailCause;
            resp.pAlphaIDInfo = pd->pAlphaIDInfo;
            resp.pCCResType   = pd->pCCResType;
            resp.pCallID      = pd->pCallID;
            resp.pCCSUPSType  = pd->pCCSUPSType;

            ULONG nRet = SLQSVoiceGetCNAP( &resp );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE != nRet )
            {
                IFPRINTF(fp, "Failure Cause      : %x\n",resp.pFailCause);
                continue;
            }

            fprintf(fp, "SLQSVoiceGetCNAP Successful\n");
            if ( NULL != resp.pCNAPResp )
            {
                fprintf(fp, "CNAP Response \n");
                fprintf(fp, "Activation Status  : %x\n",
                                resp.pCNAPResp->ActiveStatus);
                fprintf(fp, "Provision Status   : %x\n",
                                resp.pCNAPResp->ProvisionStatus);
            }
            if ( NULL != resp.pAlphaIDInfo )
            {
                doprintAlphaID( fp, resp.pAlphaIDInfo );
            }
            IFPRINTF(fp, "Call Control Result Type : %x\n",resp.pCCResType);
            IFPRINTF(fp, "Call Id            : %x\n",resp.pCallID);
            if ( NULL != resp.pCCSUPSType )
            {
                doprintccSupSType( fp, resp.pCCSUPSType );
            }
        }
    }
    if (fp)
        tfclose(fp);
}

