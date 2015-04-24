/**************
 *  Filename: qaGobiApiUimTest.c
 *
 *  Purpose:  Uim API test driver function definitions
 *
 * Copyright: © 2012 Sierra Wireless, Inc., all rights reserved
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

#include "qaGobiApiUim.h"
#include "qaGobiApiUimTest.h"
#include "qatesthelper.h"
#include "qatestproto.h"

/*******************
    TEST DATA
 *******************/
/* Constants */

/* Global Variables */
registerRefresh arrRegRefresh;
UIMRefreshEvent RefreshEvent;
ULONG           eventMaskStatus = 0x00000001;
ULONG           eventMaskSap    = 0x00000002;
ULONG           eventMaskBoth   = 0x00000003;
ULONG           eventMaskNone   = 0x00000000;

/* UIM Get File Attributes parameters */
/* This parameter gives information about the session type - Primary GW provisioning */
local UIMSessionInformation sessionInfo = { 0x00, 0x00, "" };
/* This parameter contains the file information for GSM Dedicated File */
local fileInfo             fileDF_GSM  = { 0x7F20, 0x02, {0x3F00,0x7F20} };
/* This parameter contains the file information for SMS Elementary File */
local fileInfo             fileEF_SMS  = { 0x6F3C, 0x04, {0x3F00,0x7F10,0x6F3C,0x0000} };
/* This parameter contains the file information for Master File of SIM */
local fileInfo             fileMF      = { 0xFFFF, 0x02, {0x3F00,0xFFFF} };
local ULONG              indicationTokenReq = 5;

/* UIM Depersonalization parameters */
/* This parameter de-activates the depersonalization information */
local depersonalizationInformation depinfo  = { 0x00, 0x00, 0x06, "000000" };
/* This parameter unblocks the depersonalization information */
local depersonalizationInformation depinfo1 = { 0x00, 0x01, 0x06, "000000" };

/* UIM Authenticate parameters */
local authenticationData authdata = { 0x00, 0x04, "1234" };

/* UIM Set Pin Protection Parameters */
/* This parameter gives information about the session type - Primary GW provisioning */
local UIMSessionInformation sessioninfo     = { 0x00, 0x01, "1" };
/* The below two parameters are responsible for disabling and enabling contents
 * of SIM by a specific PIN */
local setPINProtection   pinprotection      = { 0x01, 0x00, 0x04, "0000" };
local setPINProtection   pinprotection1     = { 0x01, 0x01, 0x04, "0000" };
local BYTE               keyreferenceID     = 1;
local ULONG              indicationtokenreq = 5;

/* UIM Verify Pin Parameters */
/* This parameter verifies the SIM PIN Here, it is present as "0000" */
local verifyUIMPIN  verifyuimpin     = { 0x01, 0x04, "0000" };
local encryptedPIN1 encryptedpin1req = { 0x04, "9999" };

/* UIM Change Pin Parameters */
/* This parameter changes the SIM PIN from "0000" to "9999" */
local changeUIMPIN changeuimpin = { 0x01, 0x04, "0000",
                                          0x04, "9999" };
/* This parameter changes the SIM PIN from "9999" to "0000" */
local changeUIMPIN changeuimpin1 = { 0x01, 0x04, "9999",
                                           0x04, "0000" };

/* UIM Unblock Pin Parameters */
/* This parameter unblocks an already blocked SIM PIN by giving the
 * PUK no. "95955833" and new SIM PIN "0000" in our case */
local unblockUIMPIN unblockuimpin = { 0x01, 0x08, "95955833",
                                            0x04, "0000" };

/*******************
  TEST FUNCTIONS
 *******************/
/*
 * Name:     doprintUIMPinResp
 *
 * Purpose:  This is a wrapper for printing UIM PIN Response descriptions.
 *
 * Parms:    fp           - pointer to FILE
 *           pResp        - pointer to UIMPinResp
 *
 * Return:   None
 *
 * Notes:    None.
 *
 */
void doprintUIMPinResp ( FILE *fp, UIMPinResp *pResp )
{
    BYTE lcount;
    FILE *localfp;

    localfp = fp;
    if ( NULL !=  pResp->pRemainingRetries )
    {
        fprintf(localfp, "Retry Remaining(Verify PIN) : %x \n",
                          pResp->pRemainingRetries->verifyLeft);
        fprintf(localfp, "Retry Remaining(Unblock PIN): %x \n",
                          pResp->pRemainingRetries->unblockLeft);
    }
    if ( NULL !=  pResp->pEncryptedPIN1 )
    {
        fprintf(localfp, "Encrypted PIN1 Len          : %d \n",
                          pResp->pEncryptedPIN1->pin1Len);
        fprintf(localfp, "Encrypted PIN1 Value        : ");
        for (lcount = 0;lcount < pResp->pEncryptedPIN1->pin1Len ;lcount++)
        {
            fprintf(localfp, "%c", pResp->pEncryptedPIN1->pin1Val[lcount]);
        }
        fprintf(localfp,"\n");
    }

    IFPRINTF(localfp, "Indication Token            : %lx \n",
                                          pResp->pIndicationToken);
}

/*
 * Name:    doSLQSUIMReset
 *
 * Purpose: Perform the tests that call the SLQSUIMReset() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSUIMReset( void )
{
    FILE *fp = tfopen("../../uim/test/results/slqsuimreset.txt", "w");

    if (fp)
    {
        ULONG nRet = SLQSUIMReset();
        doprintreason( fp, nRet );

        if ( eQCWWAN_ERR_NONE == nRet )
        {
            fprintf(fp, "SLQSUIMReset Successful\n");
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSUIMPowerDown
 *
 * Purpose: Perform the tests that call the SLQSUIMPowerDown() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSUIMPowerDown( void )
{
    /* Test Cases */
    UimSLQSUimPowerDownTestCase_t UimSLQSUimPowerDownTestCases[]=
    {
        { eQCWWAN_ERR_NONE, 1, 1, "PowerDowns the Slot1" },

        { eQCWWAN_ERR_NONE, 2, 2, "PowerDowns the Slot2" },
    };

    FILE *fp = tfopen("../../uim/test/results/slqsuimpowerdown.txt", "w");
    UIMPowerDownReq req;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( UimSLQSUimPowerDownTestCases )/
                      sizeof( UimSLQSUimPowerDownTestCases[0] );
        while ( tc < ncases )
        {
            UimSLQSUimPowerDownTestCase_t *pd = &UimSLQSUimPowerDownTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            req.slot = pd->slot;
            fprintf( fp, "Slot Chosen : %x\n",req.slot );

            ULONG nRet = SLQSUIMPowerDown( &req );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSUIMPowerDown Successful\n");
            }
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSUIMGetCardStatus
 *
 * Purpose: Perform the tests that call the SLQSUIMGetCardStatus() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSUIMGetCardStatus( void )
{
    local cardStatus    CardStatus;
    local hotSwapStatus HotSwapStatus;

    /* Test Cases */
    UimSLQSUimGetCardStatusTestCase_t UimSLQSUimGetCardStatusTestCases[]=
    {
        { eQCWWAN_ERR_NONE, 1, NULL, NULL,"Fetching None" },

        { eQCWWAN_ERR_NONE, 2, &CardStatus, NULL,"Fetching Card Status Values" },

        { eQCWWAN_ERR_NONE, 3, NULL, &HotSwapStatus,"Fetching Hot Swap Status" },

        { eQCWWAN_ERR_NONE, 4, &CardStatus, &HotSwapStatus,"Fetching All" }
    };

    FILE *fp = tfopen("../../uim/test/results/slqsuimgetcardstatus.txt", "w");
    UIMGetCardStatusResp resp;
    WORD                 lcount,lIcount,lIcount1;
    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( UimSLQSUimGetCardStatusTestCases )/
                      sizeof( UimSLQSUimGetCardStatusTestCases[0] );
        while ( tc < ncases )
        {
            UimSLQSUimGetCardStatusTestCase_t *pd = &UimSLQSUimGetCardStatusTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            resp.pCardStatus    = pd->pCardStatus;
            resp.pHotSwapStatus = pd->pHotSwapStatus;

            ULONG nRet = SLQSUIMGetCardStatus( &resp );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSUIMGetCardStatus Successful\n");
                if ( NULL != resp.pCardStatus )
                {
                    fprintf(fp, "Index of the primary GW   : %x\n",
                                         resp.pCardStatus->indexGwPri);
                    fprintf(fp, "Index of the primary 1X   : %x\n",
                                         resp.pCardStatus->index1xPri);
                    fprintf(fp, "Index of the secondary GW : %x\n",
                                         resp.pCardStatus->indexGwSec);
                    fprintf(fp, "Index of the secondary 1X : %x\n",
                                         resp.pCardStatus->index1xSec);
                    fprintf(fp, "Slots Available           : %x\n",
                                         resp.pCardStatus->numSlot);
                    for ( lcount=0 ; lcount < resp.pCardStatus->numSlot; lcount++ )
                    {
                        slotInfo *temp = &resp.pCardStatus->SlotInfo[lcount];
                        fprintf(fp, "\tInformation for SLOT%d\n ",lcount+1);
                        fprintf(fp, "\tState of the Card         : %x\n", temp->cardState);
                        fprintf(fp, "\tState of the UPIN         : %x\n", temp->upinState);
                        fprintf(fp, "\tRetries Remaining(UPIN)   : %d\n", temp->upinRetries);
                        fprintf(fp, "\tRetries Remaining(UPUK)   : %d\n", temp->upukRetries);
                        fprintf(fp, "\tReason For Error          : %x\n", temp->errorState);
                        fprintf(fp, "\tNo. of Apps Allowed       : %d\n", temp->numApp);
                        for ( lIcount=0 ; lIcount < temp->numApp; lIcount++ )
                        {
                            appStatus *lresp = &temp->AppStatus[lIcount];
                            fprintf(fp, "\t\tApplication Status Information for App%d\n ",lIcount+1);
                            fprintf(fp, "\t\tType of Application       : %x\n", lresp->appType);
                            fprintf(fp, "\t\tState of Application      : %x\n", lresp->appState);
                            fprintf(fp, "\t\tState of perso for App    : %x\n", lresp->persoState);
                            fprintf(fp, "\t\tIndicates perso feature   : %x\n", lresp->persoFeature);
                            fprintf(fp, "\t\tRetries Remaining(Perso BL): %d\n",
                                                 lresp->persoRetries);
                            fprintf(fp, "\t\tRetries Remaining(Perso UB): %d\n",
                                                 lresp->persoUnblockRetries);
                            fprintf(fp, "\t\tApplication Identifier Len: %d\n", lresp->aidLength);
                            fprintf(fp, "\t\tApplication Identifier Value : ");
                            for ( lIcount1=0 ; lIcount1 < lresp->aidLength; lIcount1++ )
                            {
                                fprintf(fp, "%c", lresp->aidVal[lIcount1]);
                            }
                            fprintf(fp,"\n");
                            fprintf(fp, "\t\tIndication for UPIN       : %x\n", lresp->univPin);
                            fprintf(fp, "\t\tIndicates State of Pin1   : %x\n", lresp->pin1State);
                            fprintf(fp, "\t\tRetries Remaining(PIN1)   : %d\n", lresp->pin1Retries);
                            fprintf(fp, "\t\tRetries Remaining(PUK1)   : %d\n", lresp->puk1Retries);
                            fprintf(fp, "\t\tIndicates State of Pin2   : %x\n", lresp->pin2State);
                            fprintf(fp, "\t\tRetries Remaining(PIN2)   : %d\n", lresp->pin2Retries);
                            fprintf(fp, "\t\tRetries Remaining(PUK2)   : %d\n", lresp->puk2Retries);
                        }
                    }
                }
                if ( NULL != resp.pHotSwapStatus )
                {
                    fprintf(fp, "Hot Swap Length           : %d\n",
                                         resp.pHotSwapStatus->hotSwapLength);
                    fprintf(fp, "Status of HotSwap Switch  : ");
                    for ( lcount=0 ; lcount < resp.pHotSwapStatus->hotSwapLength; lcount++ )
                    {
                        fprintf(fp, "%x", resp.pHotSwapStatus->hotSwap[lcount]);
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
 * Name:    doSLQSUIMSetPinProtection
 *
 * Purpose: Perform the tests that call the SLQSUIMSetPinProtection() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSUIMSetPinProtection( void )
{
    remainingRetries remainingRetries;
    encryptedPIN1    encryptedPIN1;
    ULONG            indicationToken;

    /* Test Cases */
    UimSLQSUimSetPinProtectionTestCase_t UimSLQSUimSetPinProtectionTestCases[]=
    {
        { eQCWWAN_ERR_NONE, 1, sessioninfo, pinprotection, NULL,
        NULL, &remainingRetries, &encryptedPIN1, &indicationToken,
        "Disabling the PIN" },

        { eQCWWAN_ERR_NONE, 2, sessioninfo, pinprotection, &keyreferenceID,
        &indicationtokenreq, &remainingRetries, &encryptedPIN1, &indicationToken,
        "No Effect as already disabled" },

        { eQCWWAN_ERR_NONE, 3, sessioninfo, pinprotection1, &keyreferenceID,
        &indicationtokenreq, &remainingRetries, &encryptedPIN1, &indicationToken,
        "Enabling the PIN" }
    };

    FILE *fp = tfopen("../../uim/test/results/slqsuimsetpinprotection.txt", "w");
    UIMSetPinProtectionReq req;
    UIMPinResp             resp;
    WORD                   lCount;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( UimSLQSUimSetPinProtectionTestCases )/
                      sizeof( UimSLQSUimSetPinProtectionTestCases[0] );
        while ( tc < ncases )
        {
            UimSLQSUimSetPinProtectionTestCase_t *pd = &UimSLQSUimSetPinProtectionTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            req.sessionInfo = pd->sessionInfo;
            fprintf( fp, "Session Type        : %x\n",req.sessionInfo.sessionType);
            fprintf( fp, "Application ID Len  : %x\n",req.sessionInfo.aidLength);
            fprintf( fp, "Application ID Value: ");
            for ( lCount = 0; lCount < pd->sessionInfo.aidLength ; lCount++ )
            {
                fprintf( fp, "%c",req.sessionInfo.aid[lCount]);
            }
            fprintf( fp, "\n");

            req.pinProtection = pd->pinProtection;
            fprintf( fp, "PIN ID              : %x\n",req.pinProtection.pinID );
            fprintf( fp, "PIN Operation       : %x\n",req.pinProtection.pinOperation );
            fprintf( fp, "PIN Length          : %x\n",req.pinProtection.pinLength );
            fprintf( fp, "PIN Value           : ");
            for ( lCount = 0; lCount < req.pinProtection.pinLength ; lCount++ )
            {
                fprintf( fp, "%c",req.pinProtection.pinValue[lCount]);
            }
            fprintf( fp, "\n");

            req.pKeyReferenceID  = pd->pKeyReferenceID;
            req.pIndicationToken = pd->pIndicationTokenReq;
            IFPRINTF(fp, "Key Reference Id    : %d \n", req.pKeyReferenceID);
            IFPRINTF(fp, "Indication Token Req: %lu \n", req.pIndicationToken);

            resp.pRemainingRetries = pd->pRemainingRetries;
            resp.pEncryptedPIN1    = pd->pEncryptedPIN1;
            resp.pIndicationToken  = pd->pIndicationToken;

            ULONG nRet = SLQSUIMSetPinProtection( &req, &resp );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSUIMSetPinProtection Successful\n");
                doprintUIMPinResp( fp, &resp );
                continue;
            }
            fprintf(fp, "SLQSUIMSetPinProtection Unsuccessful\n");
            doprintUIMPinResp( fp, &resp );
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSUIMVerifyPin
 *
 * Purpose: Perform the tests that call the SLQSUIMVerifyPin() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSUIMVerifyPin( void )
{
    remainingRetries remainingRetries;
    encryptedPIN1    encryptedPIN1;
    ULONG            indicationToken;

    /* Test Cases */
    UimSLQSUimVerifyPinTestCase_t UimSLQSUimVerifyPinTestCases[]=
    {
        { eQCWWAN_ERR_NONE, 1, sessioninfo, verifyuimpin, NULL, NULL,
        NULL, &remainingRetries, &encryptedPIN1, &indicationToken,
        "Setting Mandatory Parameters" },

        { eQCWWAN_ERR_NONE, 2, sessioninfo, verifyuimpin, &encryptedpin1req,
        &keyreferenceID, &indicationtokenreq, &remainingRetries, &encryptedPIN1,
        &indicationToken, "Setting All" }
    };

    FILE *fp = tfopen("../../uim/test/results/slqsuimverifypin.txt", "w");
    UIMVerifyPinReq req;
    UIMPinResp      resp;
    WORD            lCount;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( UimSLQSUimVerifyPinTestCases )/
                      sizeof( UimSLQSUimVerifyPinTestCases[0] );
        while ( tc < ncases )
        {
            UimSLQSUimVerifyPinTestCase_t *pd = &UimSLQSUimVerifyPinTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            req.sessionInfo = pd->sessionInfo;
            fprintf( fp, "Session Type        : %x\n",req.sessionInfo.sessionType);
            fprintf( fp, "Application ID Len  : %x\n",req.sessionInfo.aidLength);
            fprintf( fp, "Application ID Value: ");
            for ( lCount = 0; lCount < pd->sessionInfo.aidLength ; lCount++ )
            {
                fprintf( fp, "%c",req.sessionInfo.aid[lCount]);
            }
            fprintf( fp, "\n");

            req.verifyPIN = pd->verifyPIN;
            fprintf( fp, "PIN ID              : %x\n",req.verifyPIN.pinID );
            fprintf( fp, "PIN Length          : %x\n",req.verifyPIN.pinLen );
            fprintf( fp, "PIN Value           : ");
            for ( lCount = 0; lCount < req.verifyPIN.pinLen ; lCount++ )
            {
                fprintf( fp, "%c",req.verifyPIN.pinVal[lCount]);
            }
            fprintf( fp, "\n");

            req.pEncryptedPIN1 = pd->pEncryptedPIN1Req;
            if ( NULL != req.pEncryptedPIN1 )
            {
                fprintf(fp, "Encrypted PIN1 Len  : %d \n",
                                         req.pEncryptedPIN1->pin1Len);
                fprintf(fp, "Encrypted PIN1 Value: ");
                for (lCount = 0;lCount < req.pEncryptedPIN1->pin1Len ;lCount++)
                {
                    fprintf(fp, "%c", req.pEncryptedPIN1->pin1Val[lCount]);
                }
                fprintf(fp,"\n");
            }

            req.pKeyReferenceID  = pd->pKeyReferenceID;
            req.pIndicationToken = pd->pIndicationTokenReq;
            IFPRINTF(fp, "Key Reference Id    : %d \n", req.pKeyReferenceID);
            IFPRINTF(fp, "Indication Token Req: %lu \n", req.pIndicationToken);

            resp.pRemainingRetries = pd->pRemainingRetries;
            resp.pEncryptedPIN1    = pd->pEncryptedPIN1;
            resp.pIndicationToken  = pd->pIndicationToken;

            ULONG nRet = SLQSUIMVerifyPin( &req, &resp );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSUIMVerifyPin Successful\n");
                doprintUIMPinResp( fp, &resp );
                continue;
            }
            fprintf(fp, "SLQSUIMVerifyPin Unsuccessful\n");
            doprintUIMPinResp( fp, &resp );
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSUIMChangePin
 *
 * Purpose: Perform the tests that call the SLQSUIMChangePin() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSUIMChangePin( void )
{
    remainingRetries remainingRetries;
    encryptedPIN1    encryptedPIN1;
    ULONG            indicationToken;

    /* Test Cases */
    UimSLQSUimChangePinTestCase_t UimSLQSUimChangePinTestCases[]=
    {
        { eQCWWAN_ERR_NONE, 1, sessioninfo, changeuimpin, NULL,
        NULL, &remainingRetries, &encryptedPIN1, &indicationToken,
        "Setting Mandatory Parameters" },

        { eQCWWAN_ERR_NONE, 2, sessioninfo, changeuimpin, &keyreferenceID,
        &indicationtokenreq, &remainingRetries, &encryptedPIN1, &indicationToken,
        "Incorrect PIN specified" },

        { eQCWWAN_ERR_NONE, 3, sessioninfo, changeuimpin1, &keyreferenceID,
        &indicationtokenreq, &remainingRetries, &encryptedPIN1, &indicationToken,
        "Setting All" },
    };

    FILE *fp = tfopen("../../uim/test/results/slqsuimchangepin.txt", "w");
    UIMChangePinReq req;
    UIMPinResp      resp;
    WORD            lCount;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( UimSLQSUimChangePinTestCases )/
                      sizeof( UimSLQSUimChangePinTestCases[0] );
        while ( tc < ncases )
        {
            UimSLQSUimChangePinTestCase_t *pd = &UimSLQSUimChangePinTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            req.sessionInfo = pd->sessionInfo;
            fprintf( fp, "Session Type        : %x\n",req.sessionInfo.sessionType);
            fprintf( fp, "Application ID Len  : %x\n",req.sessionInfo.aidLength);
            fprintf( fp, "Application ID Value: ");
            for ( lCount = 0; lCount < pd->sessionInfo.aidLength ; lCount++ )
            {
                fprintf( fp, "%c",req.sessionInfo.aid[lCount]);
            }
            fprintf( fp, "\n");

            req.changePIN = pd->changePIN;
            fprintf( fp, "PIN ID              : %x\n",req.changePIN.pinID );
            fprintf( fp, "OLD PIN Length      : %x\n",req.changePIN.oldPINLen );
            fprintf( fp, "OLD PIN Value       : ");
            for ( lCount = 0; lCount < req.changePIN.oldPINLen ; lCount++ )
            {
                fprintf( fp, "%c",req.changePIN.oldPINVal[lCount]);
            }
            fprintf( fp, "\n");
            fprintf( fp, "NEW PIN Length      : %x\n",req.changePIN.pinLen );
            fprintf( fp, "NEW PIN Value       : ");
            for ( lCount = 0; lCount < req.changePIN.pinLen ; lCount++ )
            {
                fprintf( fp, "%c",req.changePIN.pinVal[lCount]);
            }
            fprintf( fp, "\n");

            req.pKeyReferenceID  = pd->pKeyReferenceID;
            req.pIndicationToken = pd->pIndicationTokenReq;
            IFPRINTF(fp, "Key Reference Id    : %d \n", req.pKeyReferenceID);
            IFPRINTF(fp, "Indication Token Req: %lu \n", req.pIndicationToken);

            resp.pRemainingRetries = pd->pRemainingRetries;
            resp.pEncryptedPIN1    = pd->pEncryptedPIN1;
            resp.pIndicationToken  = pd->pIndicationToken;

            ULONG nRet = SLQSUIMChangePin( &req, &resp );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSUIMChangePin Successful\n");
                doprintUIMPinResp( fp, &resp );
                continue;
            }
            fprintf(fp, "SLQSUIMChangePin Unsuccessful\n");
            doprintUIMPinResp( fp, &resp );
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSUIMUnblockPin
 *
 * Purpose: Perform the tests that call the SLQSUIMUnblockPin() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSUIMUnblockPin( void )
{
    remainingRetries remainingRetries;
    encryptedPIN1    encryptedPIN1;
    ULONG            indicationToken;

    /* Test Cases */
    UimSLQSUimUnblockPinTestCase_t UimSLQSUimUnblockPinTestCases[]=
    {
        { eQCWWAN_ERR_NONE, 1, sessioninfo, unblockuimpin, NULL,
        NULL, &remainingRetries, &encryptedPIN1, &indicationToken,
        "Setting Mandatory Parameters" },

        { eQCWWAN_ERR_NONE, 2, sessioninfo, unblockuimpin, &keyreferenceID,
        &indicationtokenreq, &remainingRetries, &encryptedPIN1, &indicationToken,
        "Setting Mandatory Parameters" },
    };

    FILE *fp = tfopen("../../uim/test/results/slqsuimunblockpin.txt", "w");
    UIMUnblockPinReq req;
    UIMPinResp       resp;
    WORD             lCount;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( UimSLQSUimUnblockPinTestCases )/
                      sizeof( UimSLQSUimUnblockPinTestCases[0] );
        while ( tc < ncases )
        {
            UimSLQSUimUnblockPinTestCase_t *pd = &UimSLQSUimUnblockPinTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            req.sessionInfo = pd->sessionInfo;
            fprintf( fp, "Session Type        : %x\n",req.sessionInfo.sessionType);
            fprintf( fp, "Application ID Len  : %x\n",req.sessionInfo.aidLength);
            fprintf( fp, "Application ID Value: ");
            for ( lCount = 0; lCount < pd->sessionInfo.aidLength ; lCount++ )
            {
                fprintf( fp, "%c",req.sessionInfo.aid[lCount]);
            }
            fprintf( fp, "\n");

            req.unblockPIN = pd->unblockPIN;
            fprintf( fp, "PIN ID              : %x\n",req.unblockPIN.pinID );
            fprintf( fp, "PUK Length          : %x\n",req.unblockPIN.pukLen );
            fprintf( fp, "PUK Value           : ");
            for ( lCount = 0; lCount < req.unblockPIN.pukLen ; lCount++ )
            {
                fprintf( fp, "%c",req.unblockPIN.pukVal[lCount]);
            }
            fprintf( fp, "\n");
            fprintf( fp, "NEW PIN Length      : %x\n",req.unblockPIN.newPINLen );
            fprintf( fp, "NEW PIN Value       : ");
            for ( lCount = 0; lCount < req.unblockPIN.newPINLen ; lCount++ )
            {
                fprintf( fp, "%c",req.unblockPIN.newPINVal[lCount]);
            }
            fprintf( fp, "\n");

            req.pKeyReferenceID  = pd->pKeyReferenceID;
            req.pIndicationToken = pd->pIndicationTokenReq;
            IFPRINTF(fp, "Key Reference Id    : %d \n", req.pKeyReferenceID);
            IFPRINTF(fp, "Indication Token Req: %lu \n", req.pIndicationToken);

            resp.pRemainingRetries = pd->pRemainingRetries;
            resp.pEncryptedPIN1    = pd->pEncryptedPIN1;
            resp.pIndicationToken  = pd->pIndicationToken;

            ULONG nRet = SLQSUIMUnblockPin( &req, &resp );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSUIMUnblockPin Successful\n");
                doprintUIMPinResp( fp, &resp );
                continue;
            }
            fprintf(fp, "SLQSUIMUnblockPin Unsuccessful\n");
            doprintUIMPinResp( fp, &resp );
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSUIMRefreshOK
 *
 * Purpose: Perform the tests that call the SLQSUIMRefreshOK() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSUIMRefreshOK( void )
{
    /* Test Cases */
    UimSLQSUimRefreshOKTestCase_t UimSLQSUimRefreshOKTestCases[]=
    {
        { eQCWWAN_ERR_NONE, 1, 1, 1, "1", 0, "Not OK to refresh" },
        { eQCWWAN_ERR_NONE, 1, 1, 1, "1", 1, "OK to refresh" },

    };

    FILE *fp = tfopen("../../uim/test/results/slqsuimrefreshOK.txt", "w");

    if (fp)
    {
        UIMRefreshOKReq req;
        BYTE lCount = 0;
        BYTE tc = 0;
        BYTE ncases = sizeof( UimSLQSUimRefreshOKTestCases )/
                      sizeof( UimSLQSUimRefreshOKTestCases[0] );
        while ( tc < ncases )
        {
            UimSLQSUimRefreshOKTestCase_t *pd =
                 &UimSLQSUimRefreshOKTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            /* Fill request paramters and print them */
            req.sessionInfo.sessionType = pd->sessionType;
            fprintf(fp,"sessionType = %d\n",pd->sessionType);
            req.sessionInfo.aidLength   = pd->aidLength;
            fprintf(fp,"aidLength = %d\n",pd->aidLength);

            fprintf(fp,"aid = 0x");
            for( lCount = 0; lCount < pd->aidLength; lCount++ )
            {
                fprintf(fp,"%x",pd->aid[lCount]);
                req.sessionInfo.aid[lCount] = pd->aid[lCount];
            }
            fprintf(fp,"\n");

            req.OKtoRefresh = pd->OKtoRefresh;
            fprintf(fp,"OKtoRefresh = %d\n",pd->OKtoRefresh);

            ULONG nRet = SLQSUIMRefreshOK(&req);
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSUIMRefreshOK Successful\n");
            }
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSUIMRefreshRegister
 *
 * Purpose: Perform the tests that call the SLQSUIMRefreshRegister() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSUIMRefreshRegister( void )
{
    /* Test Cases */
    UimSLQSUimRefreshRegisterTestCase_t UimSLQSUimRefreshRegisterTestCases[]=
    {
        { eQCWWAN_ERR_NONE, 1, 0, 0, {}, 1, 1, 1, 0x3F00,
               0x02,{0x3F00,0x7FFF}, "Valid Req: Session Type - GW provisioning"},
        { eQCWWAN_ERR_NONE, 2, 1, 0, {}, 1, 1, 1, 0x3F00,
               0x02,{0x3F00,0x7FFF}, "Valid Req: Session Type - 1X provisioning"},
        { eQCWWAN_ERR_NONE, 3, 2, 0, {}, 1, 1, 1, 0x3F00,
                0x02,{0x3F00,0x7FFF}, "Valid Req: Session Type - GW provisioning"},
        { eQCWWAN_ERR_NONE, 4, 3, 0, {}, 1, 1, 1, 0x3F00,
                0x02,{0x3F00,0x7FFF}, "Valid Req: Session Type - 1X provisioning" }
    };

    FILE *fp = tfopen("../../uim/test/results/slqsuimrefreshregister.txt", "w");
    UIMRefreshRegisterReq req;
    BYTE lCount, lIcount;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( UimSLQSUimRefreshRegisterTestCases )/
                      sizeof( UimSLQSUimRefreshRegisterTestCases[0] );
        while ( tc < ncases )
        {
            UimSLQSUimRefreshRegisterTestCase_t *pd =
                    &UimSLQSUimRefreshRegisterTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            /* Fill request paramters and print them */
            req.sessionInfo.sessionType = pd->sessionType;
            fprintf(fp,"Session Info:%d\n",pd->sessionType);
            req.sessionInfo.aidLength   = pd->aidLength;
            fprintf(fp,"aid length:%d\n",pd->aidLength);

            fprintf(fp,"aid: 0x");
            for( lCount = 0; lCount < pd->aidLength; lCount++ )
            {
                fprintf(fp,"%x",pd->aid[lCount]);
                req.sessionInfo.aid[lCount] = pd->aid[lCount];
            }
            fprintf(fp,"\n");

            arrRegRefresh.registerFlag = pd->registerFlag;
            fprintf(fp," registerFlag:%d\n",pd->registerFlag);

            arrRegRefresh.voteForInit  = pd->voteForInit;
            fprintf(fp," voteForInit:%d\n",pd->voteForInit);

            arrRegRefresh.numFiles    = pd->numFiles;
            fprintf(fp,"numFiles:%d\n",pd->numFiles);

            for ( lCount = 0; lCount < pd->numFiles; lCount++ )
            {
                arrRegRefresh.arrfileInfo[lCount].fileID  = pd->fileID;
                fprintf(fp," fileID:%x\n",pd->fileID); 

                arrRegRefresh.arrfileInfo[lCount].pathLen = pd->pathLen;
                fprintf(fp," pathLen:%d\n",pd->pathLen);

                fprintf(fp," path:0x"); 
                for( lIcount = 0; lIcount < pd->pathLen; lIcount++ )
                {
                    arrRegRefresh.arrfileInfo[lCount].path[lIcount] =
                                                       pd->path[lIcount];
                    fprintf(fp,"%x",pd->path[lIcount]);
                }

            }
            req.regRefresh = arrRegRefresh;

            ULONG nRet = SLQSUIMRefreshRegister( &req );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSUIMRefreshRegister Successful\n");
            }
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSUIMEventRegister
 *
 * Purpose: Perform the tests that call the SLQSUIMEventRegister() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSUIMEventRegister( void )
{
    /* Test Cases */
    UimSLQSUimEventRegisterTestCase_t UimSLQSUimEventRegisterTestCases[]=
    {
        { eQCWWAN_ERR_NONE, 1, eventMaskStatus, "Card status Bit Mask"},
        { eQCWWAN_ERR_NONE, 2, eventMaskSap, "SAP connection Bit Mask" },
        { eQCWWAN_ERR_NONE, 3, eventMaskBoth, "Both Bit Mask" },
        { eQCWWAN_ERR_NONE, 4, eventMaskNone, "None Bit Mask" }
    };

    UIMEventRegisterReqResp req;
    FILE *fp = tfopen("../../uim/test/results/slqsuimeventregister.txt", "w");
    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( UimSLQSUimEventRegisterTestCases )/
                      sizeof( UimSLQSUimEventRegisterTestCases[0] );
        while ( tc < ncases )
        {
            UimSLQSUimEventRegisterTestCase_t *pd =
                   &UimSLQSUimEventRegisterTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            req.eventMask = pd->eventMask;
            fprintf(fp, "Event mask = %ld\n",req.eventMask);
            ULONG nRet = SLQSUIMEventRegister( &req );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSUIMEventRegister Successful\n");
                fprintf(fp, "Event mask = %ld\n",req.eventMask);
            }
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSUIMRefreshOK
 *
 * Purpose: Perform the tests that call the SLQSUIMRefreshOK() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSUIMRefreshComplete( void )
{
    /* Test Cases */
    UimSLQSUimRefreshCompleteTestCase_t UimSLQSUimRefreshCompleteTestCases[]=
    {
        { eQCWWAN_ERR_NONE, 1, 1, 1, "1", 0, "Session Type: 1X provisioning " },
        { eQCWWAN_ERR_NONE, 2, 2, 1, "1", 1, "Session Type: GW provisioning" },
        { eQCWWAN_ERR_NONE, 3, 4, 1, "1", 0, "Session Type:"
                                             "Nonprovisioning on slot 1" },
    };

    FILE *fp = tfopen("../../uim/test/results/slqsuimrefreshOK.txt", "w");
    UIMRefreshCompleteReq req;
    BYTE lCount = 0;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( UimSLQSUimRefreshCompleteTestCases )/
                      sizeof( UimSLQSUimRefreshCompleteTestCases[0] );
        while ( tc < ncases )
        {
            UimSLQSUimRefreshCompleteTestCase_t *pd =
                   &UimSLQSUimRefreshCompleteTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            req.sessionInfo.sessionType = pd->sessionType;
            fprintf(fp,"sessionType:%d\n",pd->sessionType);

            req.sessionInfo.aidLength   = pd->aidLength;
            fprintf(fp,"aidLength:%d\n",pd->aidLength);

            fprintf(fp,"aid:0x");
            for( lCount = 0; lCount < pd->aidLength; lCount++ )
            {
                req.sessionInfo.aid[lCount] = pd->aid[lCount];
                fprintf(fp,"%x",pd->aidLength);
            }
            fprintf(fp,"\n"); 

            req.refreshComplete = pd->refreshComplete;
            fprintf(fp,"refreshComplete:%d\n",pd->refreshComplete);

            ULONG nRet = SLQSUIMRefreshComplete(&req);
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSUIMRefreshComplete Successful\n");
            }
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSUIMRefreshGetLastEvent
 *
 * Purpose: Perform the tests that call the SLQSUIMRefreshGetLastEvent() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSUIMRefreshGetLastEvent( void )
{
    /* Test Cases */
    UimSLQSUimRefreshGetLastEventTestCase_t
              UimSLQSUimRefreshGetLastEventTestCases[]=
    {
        { eQCWWAN_ERR_NONE, 1, 1, 1, "1", "Session Type: 1X provisioning " },
        { eQCWWAN_ERR_NONE, 2, 2, 1, "1", "Session Type: GW provisioning" },
        { eQCWWAN_ERR_NONE, 3, 4, 1, "1", "Session Type:"
                                            "Nonprovisioning on slot 1" },

    };

    FILE *fp = tfopen("../../uim/test/results/slqsuimgetlastevent.txt", "w");
    UIMRefreshGetLastEventReq  req;
    UIMRefreshGetLastEventResp resp;
    BYTE lCount, lIcount;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( UimSLQSUimRefreshGetLastEventTestCases )/
                      sizeof( UimSLQSUimRefreshGetLastEventTestCases[0] );
        while ( tc < ncases )
        {
            UimSLQSUimRefreshGetLastEventTestCase_t *pd =
                  &UimSLQSUimRefreshGetLastEventTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            req.sessionInfo.sessionType = pd->sessionType;
            fprintf(fp,"sessionType:%d\n",pd->sessionType);
            req.sessionInfo.aidLength   = pd->aidLength;
            fprintf(fp,"aidLength:%d\n",pd->aidLength);

            fprintf(fp,"aid:");
            for( lCount = 0; lCount < pd->aidLength; lCount++ )
            {
                req.sessionInfo.aid[lCount] = pd->aid[lCount];
                fprintf(fp,"%x",pd->aid[lCount]);
            }
            resp.pRefreshEvent = &RefreshEvent;

            ULONG nRet = SLQSUIMRefreshGetLastEvent( &req, &resp );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSUIMRefreshGetLastEvent Successful\n");
                fprintf(fp, "Parameter Indication refresh\n"); 
                fprintf(fp, "Stage:      %d\n", resp.pRefreshEvent->stage );
                fprintf(fp, "Mode:       %d\n", resp.pRefreshEvent->mode );
                fprintf(fp, "sessionType:%d\n", resp.pRefreshEvent->sessionType);
                fprintf(fp, "aidLength: %d\n", resp.pRefreshEvent->aidLength);

                UIMRefreshEvent *pTemp = resp.pRefreshEvent;
                for ( lCount = 0; lCount < pTemp->aidLength ; lCount++ )
                {
                    fprintf(fp, "Mode of Refresh = %d\n",pTemp->aid[lCount]);
                }

                fprintf(fp, " Number of files = %d\n", pTemp->numOfFiles);
                for ( lCount = 0; lCount < pTemp->numOfFiles; lCount++ )
                {
                    fprintf(fp,"FileID= %d\n",
                                RefreshEvent.arrfileInfo[lCount].fileID);
                    fprintf(fp,"Path Length = %d\n",
                                RefreshEvent.arrfileInfo[lCount].pathLen);
                    for ( lIcount = 0;
                          lIcount < RefreshEvent.arrfileInfo[lCount].pathLen; lIcount++ )
                    {
                        fprintf(fp, " Path = %x",
                                    RefreshEvent.arrfileInfo[lCount].path[lIcount]);
                    }
                }
            }
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSUIMGetFileAttributes
 *
 * Purpose: Perform the tests that call the SLQSUIMGetFileAttributes() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSUIMGetFileAttributes( void )
{
    local cardResult     cardresult;
    local fileAttributes fileattributes;
    local ULONG          indicationtoken;

    /* Test Cases */
    UimSLQSUimGetFileAttributesTestCase_t UimSLQSUimGetFileAttributesTestCases[]=
    {
        { eQCWWAN_ERR_NONE, 1, sessionInfo, fileDF_GSM, NULL,
        &cardresult, &fileattributes, &indicationtoken,
        "Info about DF_GSM File " },

        { eQCWWAN_ERR_NONE, 2, sessionInfo, fileEF_SMS, &indicationTokenReq,
        &cardresult, &fileattributes, &indicationtoken,
        "Info about EF_SMS File " },

        { eQCWWAN_ERR_NONE, 3, sessionInfo, fileMF, &indicationTokenReq,
        &cardresult, &fileattributes, &indicationtoken,
        "Info about Master File(MF) " },
    };

    FILE *fp = tfopen("../../uim/test/results/slqsuimgetfileattributes.txt", "w");
    UIMGetFileAttributesReq  req;
    UIMGetFileAttributesResp resp;
    WORD                     lcount;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( UimSLQSUimGetFileAttributesTestCases )/
                      sizeof( UimSLQSUimGetFileAttributesTestCases[0] );
        while ( tc < ncases )
        {
            UimSLQSUimGetFileAttributesTestCase_t *pd =
                                  &UimSLQSUimGetFileAttributesTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            req.sessionInfo = pd->sessionInfo;

            fprintf( fp, "Session Type : %x\n",req.sessionInfo.sessionType );
            fprintf( fp, "APP ID Len   : %x\n",req.sessionInfo.aidLength );
            fprintf( fp, "Application ID : ");
            for ( lcount=0 ; lcount < req.sessionInfo.aidLength ; lcount++ )
            {
                fprintf( fp, "%c",req.sessionInfo.aid[lcount]);
            }
            fprintf( fp,"\n");

            req.fileIndex = pd->fileIndex;
            fprintf( fp, "File Id     : %x\n",req.fileIndex.fileID );
            fprintf( fp, "Path Len    : %x\n",req.fileIndex.pathLen );
            fprintf( fp, "Path        : ");
            for ( lcount=0 ; lcount < req.fileIndex.pathLen ; lcount++ )
            {
                fprintf( fp, "%x",req.fileIndex.path[lcount]);
            }
            fprintf( fp,"\n");

            req.pIndicationToken = pd->pIndicationTokenReq;
            IFPRINTF( fp, "Indication Token Req: %lx\n",req.pIndicationToken );

            resp.pCardResult      = pd->pCardResult;
            resp.pFileAttributes  = pd->pFileAttributes;
            resp.pIndicationToken = pd->pIndicationToken;

            ULONG nRet = SLQSUIMGetFileAttributes( &req, &resp );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE != nRet )
            {
                fprintf(fp, "SLQSUIMGetFileAttributes Unsuccessful\n");
                continue;
            }

            fprintf(fp, "SLQSUIMGetFileAttributes Successful\n");
            if ( NULL != resp.pCardResult )
            {
                fprintf( fp, "SW1 received from card :%x\n", resp.pCardResult->sw1 );
                fprintf( fp, "SW2 received from card :%x\n", resp.pCardResult->sw2 );
            }
            if ( NULL != resp.pFileAttributes )
            {
                fprintf( fp, "File Size        : %d\n",resp.pFileAttributes->fileSize);
                fprintf( fp, "File ID          : %x\n",resp.pFileAttributes->fileID);
                fprintf( fp, "File Type        : %x\n",resp.pFileAttributes->fileType);
                fprintf( fp, "Record Size      : %d\n",resp.pFileAttributes->recordSize);
                fprintf( fp, "Record Count     : %d\n",resp.pFileAttributes->recordCount);
                fprintf( fp, "Security Read    : %x\n",resp.pFileAttributes->secRead);
                fprintf( fp, "Security Read M  : %x\n",resp.pFileAttributes->secReadMask);
                fprintf( fp, "Security Write   : %x\n",resp.pFileAttributes->secWrite);
                fprintf( fp, "Security Write M : %x\n",resp.pFileAttributes->secWriteMask);
                fprintf( fp, "Security Inc     : %x\n",resp.pFileAttributes->secIncrease);
                fprintf( fp, "Security Inc M   : %x\n",resp.pFileAttributes->secIncreaseMask);
                fprintf( fp, "Security De-Act  : %x\n",resp.pFileAttributes->secDeactivate);
                fprintf( fp, "Security De-Act M: %x\n",resp.pFileAttributes->secDeactivateMask);
                fprintf( fp, "Security Act     : %x\n",resp.pFileAttributes->secActivate);
                fprintf( fp, "Security Act M   : %x\n",resp.pFileAttributes->secActivateMask);
                fprintf( fp, "Raw Len          : %d\n",resp.pFileAttributes->rawLen);
                fprintf( fp, "Raw Value        : ");
                for ( lcount=0 ; lcount < resp.pFileAttributes->rawLen ; lcount++ )
                {
                    fprintf( fp, "%x ",resp.pFileAttributes->rawValue[lcount]);
                }
                fprintf( fp,"\n");
            }
            IFPRINTF( fp, "Indication Token : %lx\n",resp.pIndicationToken );
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSUIMDepersonalization
 *
 * Purpose: Perform the tests that call the SLQSUIMDepersonalization() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSUIMDepersonalization( void )
{
    local remainingRetries remainingretries;

    /* Test Cases */
    UimSLQSUimDepersonalizationTestCase_t UimSLQSUimDepersonalizationTestCases[]=
    {
        { eQCWWAN_ERR_NONE, 1, depinfo, &remainingretries,
        "De-activate personalization" },

        { eQCWWAN_ERR_NONE, 2, depinfo1, &remainingretries,
        "Unblock personalization" },
    };

    FILE *fp = tfopen("../../uim/test/results/slqsuimdepersonalization.txt", "w");
    UIMDepersonalizationReq  req;
    UIMDepersonalizationResp resp;
    WORD                     lcount;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( UimSLQSUimDepersonalizationTestCases )/
                      sizeof( UimSLQSUimDepersonalizationTestCases[0] );
        while ( tc < ncases )
        {
            UimSLQSUimDepersonalizationTestCase_t *pd = &UimSLQSUimDepersonalizationTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            req.depersonilisationInfo = pd->depersonilisationInfo;
            fprintf( fp, "Feature            : %x\n",req.depersonilisationInfo.feature );
            fprintf( fp, "Operation          : %x\n",req.depersonilisationInfo.operation );
            fprintf( fp, "Control Key Length : %x\n",req.depersonilisationInfo.ckLen );
            fprintf( fp, "Control Key Value  : " );
            for ( lcount=0 ; lcount < req.depersonilisationInfo.ckLen ; lcount++ )
            {
                fprintf( fp, "%c",req.depersonilisationInfo.ckVal[lcount]);
            }
            fprintf( fp,"\n");

            resp.pRemainingRetries = pd->pRemainingRetries;

            ULONG nRet = SLQSUIMDepersonalization( &req, &resp );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf( fp, "SLQSUIMDepersonalization Successful\n");
                continue;
            }

            fprintf( fp, "SLQSUIMDepersonalization Unsuccessful\n");
            if ( NULL != resp.pRemainingRetries )
            {
                fprintf( fp, "Verify Left  :%d\n", resp.pRemainingRetries->verifyLeft );
                fprintf( fp, "Unblock Left :%d\n", resp.pRemainingRetries->unblockLeft );
            }
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSUIMAuthenticate
 *
 * Purpose: Perform the tests that call the SLQSUIMAuthenticate() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSUIMAuthenticate( void )
{
    cardResult         cardresult;
    authenticateResult authenticateresult;
    ULONG              indicationtoken;

    /* Test Cases */
    UimSLQSUimAuthenticateTestCase_t UimSLQSUimAuthenticateTestCases[]=
    {
        { eQCWWAN_ERR_NONE, 1, sessionInfo, authdata, NULL,
        &cardresult, &authenticateresult, &indicationtoken,
        "Providing mandatory parameters" },

        { eQCWWAN_ERR_NONE, 2, sessionInfo, authdata, &indicationTokenReq,
        &cardresult, &authenticateresult, &indicationtoken,
        "Providing all parameters" },
    };

    FILE *fp = tfopen("../../uim/test/results/slqsuimauthenticate.txt", "w");
    UIMAuthenticateReq  req;
    UIMAuthenticateResp resp;
    WORD                lcount;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( UimSLQSUimAuthenticateTestCases )/
                      sizeof( UimSLQSUimAuthenticateTestCases[0] );
        while ( tc < ncases )
        {
            UimSLQSUimAuthenticateTestCase_t *pd = &UimSLQSUimAuthenticateTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            req.sessionInfo = pd->sessionInfo;
            fprintf( fp, "Session Type : %x\n",req.sessionInfo.sessionType );
            fprintf( fp, "APP ID Len   : %x\n",req.sessionInfo.aidLength );
            fprintf( fp, "Application ID : ");
            for ( lcount=0 ; lcount < req.sessionInfo.aidLength ; lcount++ )
            {
                fprintf( fp, "%c",req.sessionInfo.aid[lcount]);
            }
            fprintf( fp,"\n");

            req.authData = pd->authData;
            fprintf( fp, "Context      : %x\n",req.authData.context );
            fprintf( fp, "Data Len     : %x\n",req.authData.dataLen );
            fprintf( fp, "Data         : ");
            for ( lcount=0 ; lcount < req.authData.dataLen ; lcount++ )
            {
                fprintf( fp, "%c",req.authData.data[lcount]);
            }
            fprintf( fp,"\n");

            req.pIndicationToken = pd->pIndicationTokenReq;
            IFPRINTF( fp, "Indication Token : %lx\n",req.pIndicationToken );

            resp.pCardResult         = pd->pCardResult;
            resp.pAuthenticateResult = pd->pAuthenticateResult;
            resp.pIndicationToken    = pd->pIndicationToken;

            ULONG nRet = SLQSUIMAuthenticate( &req, &resp );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE != nRet )
            {
                fprintf(fp, "SLQSUIMAuthenticate Unsuccessful\n");
                continue;
            }

            fprintf(fp, "SLQSUIMAuthenticate Successful\n");
            if ( NULL != resp.pCardResult )
            {
                fprintf( fp, "SW1 received from card :%x", resp.pCardResult->sw1 );
                fprintf( fp, "SW2 received from card :%x", resp.pCardResult->sw2 );
            }
            if ( NULL != resp.pAuthenticateResult )
            {
                fprintf( fp, "Content Len  : %d\n",resp.pAuthenticateResult->contentLen );
                fprintf( fp, "Content      : ");
                for ( lcount=0 ; lcount < resp.pAuthenticateResult->contentLen ; lcount++ )
                {
                    fprintf( fp, "%c",resp.pAuthenticateResult->content[lcount]);
                }
                fprintf( fp,"\n");
            }
            IFPRINTF( fp, "Indication Token : %lx\n",resp.pIndicationToken );

        }
    }
    if (fp)
        tfclose(fp);
}
