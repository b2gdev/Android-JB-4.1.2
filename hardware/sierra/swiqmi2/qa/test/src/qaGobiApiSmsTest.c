/**************
 *
 *  Filename: qaGobiApiSmsTest.c
 *
 *  Purpose:  Short Message Service (SMS) API test driver function definitions
 *
 * Copyright: © 2011-2012 Sierra Wireless, Inc., all rights reserved
 *
 **************/

/* include files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "SwiDataTypes.h"
#include "qmerrno.h"
#include "qmudefs.h"
#include "qaGobiApiSms.h"
#include "sludefs.h"
#include "qaGobiApiSmsTest.h"
#include "qatesthelper.h"
#include "qatestproto.h"

/*******************
    TEST DATA
 *******************/

/* Constants */
#define TRANSTYPE -1
#define TRANSCAP  -1

/* Local Variables */

local BYTE regInd;
local transLayerInfo TransLayerInfo = { TRANSTYPE, TRANSCAP};
local BYTE *pRegInd = &regInd;
local transLayerInfo *pTransLayerInfo = &TransLayerInfo;
local getTransLayerInfoResp GetTransLayerInfoResp;
local getTransNWRegInfoResp GetTransNWRegInfoResp;

local BYTE TransLayerInfoStat;
local BYTE TransNWRegInfoStat;
local BYTE CallInfoStat;
local BYTE *pTransLayerInfoStat = &TransLayerInfoStat;
local BYTE *pTransNWRegInfoStat = &TransNWRegInfoStat;
local BYTE *pCallInfoStat = &CallInfoStat;
local getIndicationRegResp  GetIndRegisterResp;

local BYTE TransLayerInfoEvtStat = 1;
local BYTE TransNWRegInfoEvtStat = 1;
local BYTE CallInfoEvtStat = 1;

local BYTE *pTransLayerInfoEvtStat = &TransLayerInfoEvtStat;
local BYTE *pTransNWRegInfoEvtStat = &TransNWRegInfoEvtStat;
local BYTE *pCallInfoEvtStat = &CallInfoEvtStat;
local setIndicationRegReq SetIndicationRegReq;

/**
 * Name:     doSLQSDeleteSMS
 *
 * Purpose:  Perform the tests that call the SLQSDeleteSMS() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSDeleteSMS( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../sms/test/results/slqsdeletesms.txt", "w");

    if (fp)
    {
        while ( tCaseNum < MAX_SLQS_DELETE_SMS_TESTCASE_NUM )
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_SLQS_DELETE_SMS_TESTCASE_NUM,
                        SLQSDeleteSMSTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Storage Type : %lx\n",\
                SLQSDeleteSMSTestCases[tCaseNum].storageType);
            if ( SLQSDeleteSMSTestCases[tCaseNum].pMessageIndex )
                fprintf(fp, "Message Index : %lx\n",\
                    *(SLQSDeleteSMSTestCases[tCaseNum].pMessageIndex) );
            if ( SLQSDeleteSMSTestCases[tCaseNum].pMessageTag )
                fprintf(fp, "Message Tag : %lx\n",\
                    *(SLQSDeleteSMSTestCases[tCaseNum].pMessageTag) );
            if ( SLQSDeleteSMSTestCases[tCaseNum].pMessageMode )
                fprintf(fp, "Message Tag : %x\n",\
                    *(SLQSDeleteSMSTestCases[tCaseNum].pMessageMode) );

            nRet = SLQSDeleteSMS(\
                       SLQSDeleteSMSTestCases[tCaseNum].storageType,
                       SLQSDeleteSMSTestCases[tCaseNum].pMessageIndex,
                       SLQSDeleteSMSTestCases[tCaseNum].pMessageTag,
                       SLQSDeleteSMSTestCases[tCaseNum].pMessageMode );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
                fprintf(fp, "SLQS Delete SMS Successful\n");

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/**
 * Name:     doSLQSGetSMS
 *
 * Purpose:  Perform the tests that call the SLQSGetSMS() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSGetSMS( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;
    ULONG count;

    fp = tfopen("../../sms/test/results/slqsgetsms.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_SLQS_GET_SMS_TESTCASE_NUM)
        {
            memset( message, 0, sizeof(message) );

            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_SLQS_GET_SMS_TESTCASE_NUM,
                        SLQSGetSMSTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Storage Type : %lu\n",\
                SLQSGetSMSTestCases[tCaseNum].storageType);
            fprintf(fp, "Message Index : %lu\n",\
                SLQSGetSMSTestCases[tCaseNum].messageIndex);
            fprintf(fp, "Message Size : %lu\n",\
                SLQSGetSMSTestCases[tCaseNum].messageSize);
            if ( SLQSGetSMSTestCases[tCaseNum].pMessageMode )
                fprintf(fp, "Message Tag : %x\n",\
                    *(SLQSGetSMSTestCases[tCaseNum].pMessageMode) );

            nRet = SLQSGetSMS(\
                       SLQSGetSMSTestCases[tCaseNum].storageType,
                       SLQSGetSMSTestCases[tCaseNum].messageIndex,
                       &messageTag,
                       &messageFormat,
                       (ULONG *)&(SLQSGetSMSTestCases[tCaseNum].messageSize),
                       &message[0],
                       SLQSGetSMSTestCases[tCaseNum].pMessageMode );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "SLQS Get SMS Successful\n");
                fprintf(fp, "Message Tag : %lu\n",messageTag);
                fprintf(fp, "Message Format : %lu\n",messageFormat);
                fprintf(fp, "Message Size : %lu\n",\
                            SLQSGetSMSTestCases[tCaseNum].messageSize);
                fprintf(fp, "Message : ");
                for ( count=0;
                      count < SLQSGetSMSTestCases[tCaseNum].messageSize;
                      count++ )
                    fprintf(fp, " %x ",(message[count]));
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/**
 * Name:     doSendSMS
 *
 * Purpose:  Perform the tests that call the DeleteSMS() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSendSMS( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;
    ULONG messageSize = 0;
    BYTE  *message;

    fp = tfopen("../../sms/test/results/sendsms.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_SEND_SMS_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_SEND_SMS_TESTCASE_NUM,
                        SendSMSTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Message Format : %lu\n",\
                SendSMSTestCases[tCaseNum].messageFormat);
            fprintf(fp, "Message Size : %lu\n",\
                SendSMSTestCases[tCaseNum].messageSize);

            messageSize = SendSMSTestCases[tCaseNum].messageSize;
            message     = SendSMSTestCases[tCaseNum].message;
            fprintf(fp,"Message: ");
            while (messageSize--)
            {
                fprintf(fp, "%x ", *message);
                message++;
            }
            fprintf(fp,"\n");

            nRet = SendSMS( SendSMSTestCases[tCaseNum].messageFormat,
                            SendSMSTestCases[tCaseNum].messageSize,
                            SendSMSTestCases[tCaseNum].message,
                            SendSMSTestCases[tCaseNum].messageFailureCode );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "Send SMS Successful\n");
            }
            else if ( SendSMSTestCases[tCaseNum].messageFailureCode )
                fprintf(fp, "Message Failure reason : %lx\n",\
                    *(SendSMSTestCases[tCaseNum].messageFailureCode) );

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/**
 * Name:     doSLQSSendSMS
 *
 * Purpose:  Perform the tests that call the SLQSSendSMS() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSSendSMS( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;
    ULONG messageSize = 0;
    BYTE  *message;
    slqssendsmsparams_s messageParams;

    fp = tfopen("../../sms/test/results/sendsms.txt", "w");


    if (fp)
    {
        while (tCaseNum < MAX_SEND_SMS_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_SEND_SMS_TESTCASE_NUM,
                        SendSMSTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Message Format : %lu\n",\
                SendSMSTestCases[tCaseNum].messageFormat);
            fprintf(fp, "Message Size : %lu\n",\
                SendSMSTestCases[tCaseNum].messageSize);

            messageSize = SendSMSTestCases[tCaseNum].messageSize;
            message     = SendSMSTestCases[tCaseNum].message;
            fprintf(fp,"Message: ");
            while (messageSize--)
            {
                fprintf(fp, "%x ", *message);
                message++;
            }
            fprintf(fp,"\n");

            messageParams.messageFormat      = SendSMSTestCases[tCaseNum].messageFormat;
            messageParams.messageSize        = SendSMSTestCases[tCaseNum].messageSize;
            messageParams.pMessage           = SendSMSTestCases[tCaseNum].message;

            nRet = SLQSSendSMS( &messageParams );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "Send SMS Successful\n");
                fprintf(fp, "Message Reference ID : %d\n",\
                    messageParams.messageID );
            }
            else if ( SendSMSTestCases[tCaseNum].messageFailureCode )
                fprintf(fp, "Message Failure reason : %lx\n",\
                    messageParams.messageFailureCode);

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetSMSCAddress
 *
 * Purpose:  Perform the tests that call the GetSMSCAddress() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetSMSCAddress( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../sms/test/results/getsmscaddress.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_GET_SMSC_ADDRESS_TESTCASE_NUM)
        {
            fprintf( fp,
                     "\nTest Case %d / %d : %s\n",
                     (tCaseNum + 1),
                     MAX_GET_SMSC_ADDRESS_TESTCASE_NUM,
                     GetSMSCAddressTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Address Size : %d\n",\
                GetSMSCAddressTestCases[tCaseNum].addressSize);
            fprintf(fp, "Type Size : %d\n",\
                GetSMSCAddressTestCases[tCaseNum].typeSize);

            nRet = GetSMSCAddress( GetSMSCAddressTestCases[tCaseNum].addressSize,
                                   GetSMSCAddressTestCases[tCaseNum].pSMSCAddress,
                                   GetSMSCAddressTestCases[tCaseNum].typeSize,
                                   GetSMSCAddressTestCases[tCaseNum].pSMSCType );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "GetSMSCAddress Successful\n");
                fprintf(fp, "\nSMSC Address : ");
                fprintf(fp, "%s",GetSMSCAddressTestCases[tCaseNum].pSMSCAddress);
                fprintf(fp, "\nSMSC Type : ");
                fprintf(fp, "%s\n", GetSMSCAddressTestCases[tCaseNum].pSMSCType);
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doSetSMSCAddress
 *
 * Purpose:  Perform the tests that call the SetSMSCAddress() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSetSMSCAddress( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../sms/test/results/setsmscaddress.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_SET_SMSC_ADDRESS_TESTCASE_NUM)
        {
            fprintf( fp,
                     "\nTest Case %d / %d : %s\n",
                     (tCaseNum + 1),
                     MAX_SET_SMSC_ADDRESS_TESTCASE_NUM,
                     SetSMSCAddressTestCases[tCaseNum].desc );

            fprintf(fp, "Parameter Set:\n");
            fprintf( fp,
                     "Address : %s\n",
                     SetSMSCAddressTestCases[tCaseNum].pSMSCAddress );
            fprintf(fp, "Type : %s\n",\
                SetSMSCAddressTestCases[tCaseNum].pSMSCType);

            nRet = SetSMSCAddress( SetSMSCAddressTestCases[tCaseNum].pSMSCAddress,
                                   SetSMSCAddressTestCases[tCaseNum].pSMSCType );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "SetSMSCAddress Successful\n");
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/**
 * Name:     doSaveSMS
 *
 * Purpose:  Perform the tests that call the SaveSMS() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSaveSMS( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    ULONG messageIndex;
    FILE  *fp = NULL;
    ULONG count;
    BYTE  *msg;

    fp = tfopen("../../sms/test/results/savesms.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_SAVE_SMS_TESTCASE_NUM)
        {

            fprintf( fp,
                     "\nTest Case %d / %d : %s\n",
                     (tCaseNum + 1),
                     MAX_SAVE_SMS_TESTCASE_NUM,
                     SaveSMSTestCases[tCaseNum].desc );

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Storage Type : %lu\n", SaveSMSTestCases[tCaseNum].storageType);
            fprintf(fp, "Message Format : %lu\n", SaveSMSTestCases[tCaseNum].messageFormat);
            fprintf(fp, "Message Size : %lu\n", SaveSMSTestCases[tCaseNum].messageSize);
            if ( SaveSMSTestCases[tCaseNum].message )
            {
                msg = SaveSMSTestCases[tCaseNum].message;
                fprintf(fp, "Message : ");
                for ( count=0; count<SaveSMSTestCases[tCaseNum].messageSize; count++ )
                    fprintf(fp, " %x ", *(msg)++);
                fprintf(fp, "\n");
            }

            nRet = SaveSMS( SaveSMSTestCases[tCaseNum].storageType,
                            SaveSMSTestCases[tCaseNum].messageFormat,
                            SaveSMSTestCases[tCaseNum].messageSize,
                            SaveSMSTestCases[tCaseNum].message,
                            &messageIndex );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "Save SMS Successful\n");
                fprintf(fp, "Message Index is : %lu\n", messageIndex );
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/**
 * Name:     doSLQSGetSMSList
 *
 * Purpose:  Perform the tests that call the SLQSGetSMSList() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSGetSMSList( void )
{
    BYTE   tCaseNum = 0;
    ULONG  nRet;
    BYTE   count;
    ULONG  messageListSize;
    struct messageList
    {
        ULONG messageIndex;
        BYTE  messageTag;
    } messageLists[nMaxStrLen];

    FILE  *fp = NULL;

    fp = tfopen("../../sms/test/results/slqsgetsmslist.txt", "w");
    fprintf(fp, "\n\n<GetSMSList>");

    if (fp)
    {
        while (tCaseNum < MAX_SLQS_GET_SMS_LIST_TESTCASE_NUM)
        {
            messageListSize = SLQSGetSMSListTestCases[tCaseNum].messageListSize;

            fprintf( fp,
                     "\nTest Case %d / %d : %s\n",
                     (tCaseNum + 1),
                     MAX_SLQS_GET_SMS_LIST_TESTCASE_NUM,
                     SLQSGetSMSListTestCases[tCaseNum].desc );

            fprintf(fp, "Parameter Set :\n");
            fprintf(fp, "Storage Type : %lu\n", SLQSGetSMSListTestCases \
                                          [tCaseNum].storageType);
            fprintf(fp, "Requested Tag: %lu\n", SLQSGetSMSListTestCases \
                                          [tCaseNum].requestedTag);
            fprintf(fp, "MessageListSize  : %lx\n", messageListSize);
            if ( SLQSGetSMSListTestCases[tCaseNum].pMessageMode )
                fprintf(fp, "MessageMode  : %d\n",
                        *(SLQSGetSMSListTestCases[tCaseNum].pMessageMode));

            nRet = SLQSGetSMSList(
                        SLQSGetSMSListTestCases[tCaseNum].storageType,
                        (ULONG *)&(SLQSGetSMSListTestCases[tCaseNum]
                                                           .requestedTag),
                        &messageListSize,
                        (BYTE *)&messageLists[0],
                        SLQSGetSMSListTestCases[tCaseNum].pMessageMode );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "MessageListSize  : %lx\n", messageListSize);
                for(count = 0; count < messageListSize; count++)
                {
                    fprintf(fp, "messageIndex[%d] : %lx\n",
                            count, messageLists[count].messageIndex);
                    fprintf(fp, "messageTag[%d] : %x\n",
                            count, messageLists[count].messageTag);
                }
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/**
 * Name:     doSLQSModifySMSStatus
 *
 * Purpose:  Perform the tests that call the SLQSModifySMSStatus() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSModifySMSStatus( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../sms/test/results/slqsmodifysmsstatus.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_SLQS_MODIFY_SMS_STATUS_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_SLQS_MODIFY_SMS_STATUS_TESTCASE_NUM,
                        SLQSModifySMSStatusTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Storage Type : %lu\n",\
                SLQSModifySMSStatusTestCases[tCaseNum].storageType);
            fprintf(fp, "Message Index : %lu\n",\
                SLQSModifySMSStatusTestCases[tCaseNum].messageIndex);
            fprintf(fp, "Message Tag : %lu\n",\
                SLQSModifySMSStatusTestCases[tCaseNum].messageTag);
            if ( SLQSModifySMSStatusTestCases[tCaseNum].pMessageMode )
                fprintf(fp, "MessageMode  : %d\n",
                        *(SLQSModifySMSStatusTestCases[tCaseNum].pMessageMode));
            nRet = SLQSModifySMSStatus(
                           SLQSModifySMSStatusTestCases[tCaseNum].storageType,
                           SLQSModifySMSStatusTestCases[tCaseNum].messageIndex,
                           SLQSModifySMSStatusTestCases[tCaseNum].messageTag,
                           SLQSModifySMSStatusTestCases[tCaseNum].pMessageMode );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "SLQS Modify SMS Status Successful\n");
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/**
 * Name:     doSLQSGetSMSBroadcastConfig
 *
 * Purpose:  Perform the tests that call the SLQSGetSMSBroadcastConfig() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSGetSMSBroadcastConfig( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;
    WORD lTemp = 0;

    fp = tfopen("../../sms/test/results/getsmsbroadcastconfig.txt", "w");

    if (fp)
    {
        while ( tCaseNum < MAX_SLQS_GET_SMS_BROADCAST_CONFIG_TESTCASE_NUM )
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_SLQS_GET_SMS_BROADCAST_CONFIG_TESTCASE_NUM,
                        SLQSGetSMSBroadcastConfigTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Mode is  : %d\n",\
                    SLQSGetSMSBroadcastConfigTestCases[tCaseNum].mode);

            nRet = SLQSGetSmsBroadcastConfig(
            SLQSGetSMSBroadcastConfigTestCases[tCaseNum].mode,
            SLQSGetSMSBroadcastConfigTestCases[tCaseNum].q3GPPBroadcastCfgInfo,
            SLQSGetSMSBroadcastConfigTestCases[tCaseNum].q3GPP2BroadcastCfgInfo );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "Get SMS Broadcast Configuration Successful\n");
                if ( MODE_GW == SLQSGetSMSBroadcastConfigTestCases[tCaseNum].mode)
                {
                fprintf(fp, "3GPP Broadcast Configuration Info\n");
                fprintf(fp, "Activated Index  :%x\n",
                    q3GPPBroadcastCfgInfo.activated_ind);
                fprintf(fp, "Number of Sets   :%x\n",
                    q3GPPBroadcastCfgInfo.num_instances);
                for (lTemp=0;lTemp<q3GPPBroadcastCfgInfo.num_instances;lTemp++)
                {
                    fprintf(fp, "From Service Id  [%d]:%x\n",
                    lTemp,q3GPPBroadcastCfgInfo.broadcastConfig[lTemp].fromServiceId);
                    fprintf(fp, "To Service Id    [%d]:%x\n",
                    lTemp,q3GPPBroadcastCfgInfo.broadcastConfig[lTemp].toServiceId);
                    fprintf(fp, "Selected         [%d]:%x\n",
                    lTemp,q3GPPBroadcastCfgInfo.broadcastConfig[lTemp].selected);
                }
                }


                if ( MODE_CDMA == SLQSGetSMSBroadcastConfigTestCases[tCaseNum].mode)
                {
                fprintf(fp, "3GPP2 Broadcast Configuration Info\n");
                fprintf(fp, "Activated Index  :%x\n",
                    q3GPP2BroadcastCfgInfo.activated_ind);
                fprintf(fp, "Number of Sets   :%x\n",
                    q3GPP2BroadcastCfgInfo.num_instances);
                for (lTemp=0;lTemp<q3GPP2BroadcastCfgInfo.num_instances;lTemp++)
                {
                    fprintf(fp, "Service Category [%d]:%x\n",
                    lTemp,q3GPP2BroadcastCfgInfo.CDMABroadcastConfig[lTemp].serviceCategory);
                    fprintf(fp, "Language         [%d]:%x\n",
                    lTemp,q3GPP2BroadcastCfgInfo.CDMABroadcastConfig[lTemp].language);
                    fprintf(fp, "Selected         [%d]:%x\n",
                    lTemp,q3GPP2BroadcastCfgInfo.CDMABroadcastConfig[lTemp].selected);
                }
                }
            }
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/**
 * Name:     doSLQSSetSmsBroadcastConfig
 *
 * Purpose:  Perform the tests that call the SLQSSetSmsBroadcastConfig() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSSetSmsBroadcastConfig( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    lBroadcastConfig.num_instances = 0x02;
    lBroadcastConfig.broadcastConfig[0].fromServiceId = 0x0102;
    lBroadcastConfig.broadcastConfig[0].toServiceId = 0x0304;
    lBroadcastConfig.broadcastConfig[0].selected = 0x01;
    lBroadcastConfig.broadcastConfig[1].fromServiceId = 0x0102;
    lBroadcastConfig.broadcastConfig[1].toServiceId = 0x0304;
    lBroadcastConfig.broadcastConfig[1].selected = 0x01;

    lCDMABroadcastConfig.num_instances = 0x02;
    lCDMABroadcastConfig.CDMABroadcastConfig[0].serviceCategory = 0x0102;
    lCDMABroadcastConfig.CDMABroadcastConfig[0].language = 0x0304;
    lCDMABroadcastConfig.CDMABroadcastConfig[0].selected = 0x01;
    lCDMABroadcastConfig.CDMABroadcastConfig[1].serviceCategory = 0x0102;
    lCDMABroadcastConfig.CDMABroadcastConfig[1].language = 0x0304;
    lCDMABroadcastConfig.CDMABroadcastConfig[1].selected = 0x01;

    fp = tfopen("../../sms/test/results/slqssetsmsbroadcastconfig.txt", "w");

    if (fp)
    {
        while ( tCaseNum < MAX_SLQS_SET_SMS_BROADCAST_CONFIG_TESTCASE_NUM )
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_SLQS_SET_SMS_BROADCAST_CONFIG_TESTCASE_NUM,
                        SLQSSetBroadcastConfigTestCases[tCaseNum].desc);
            fprintf(fp, "Parameter Set:\n");


            nRet = SLQSSetSmsBroadcastConfig(
                         SLQSSetBroadcastConfigTestCases[tCaseNum].mode,
                         SLQSSetBroadcastConfigTestCases[tCaseNum].
                                                       pBroadcastConfig,
                         SLQSSetBroadcastConfigTestCases[tCaseNum].
                                                       pCDMABroadcastConfig );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "SetImagesPreference Successful\n");
            }
            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/**
 * Name:     doSetSmsBroadcastActivation
 *
 * Purpose:  Perform the tests that call the SetSmsBroadcastActivation() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSetSmsBroadcastActivation( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../sms/test/results/setsmsbroadcastactivation.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_SET_SMS_BC_ACTIVATION_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_SET_SMS_BC_ACTIVATION_TESTCASE_NUM,
                        SetSMSBCActivationTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Mode : 0x%x\n",\
                    SetSMSBCActivationTestCases[tCaseNum].mode);
            fprintf(fp, "bc_activate : %x\n",\
                    SetSMSBCActivationTestCases[tCaseNum].bc_activate);

            nRet = SLQSSetSmsBroadcastActivation(
                       SetSMSBCActivationTestCases[tCaseNum].mode,
                       SetSMSBCActivationTestCases[tCaseNum].bc_activate );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "Set SMS Broadcast ActivationStatus Successful\n");
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/**
 * Name:     doSLQSCDMAEncodeMOTextMsg
 *
 * Purpose:  Perform the tests that call the SLQSCDMAEncodeMOTextMsg() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSCDMAEncodeMOTextMsg( void )
{
    BYTE                      tCaseNum = 0;
    ULONG                     nRet;
    FILE                      *fp = NULL;
    BYTE                      hexa[HEX_BUFFER_SIZE] = { 0 };
    ULONG                     messageFailureCode = 0;
    ULONG                     messageFormat = 0 ;
    struct cdmaMsgEncodingParams lEncodeTextToPDUCDMA;

    fp = tfopen("../../sms/test/results/slqssendsms.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_ENCODE_TEXT_TESTCASE_NUM)
        {
            sleep(20);
            memset(hexa, 0, HEX_BUFFER_SIZE);
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_ENCODE_TEXT_TESTCASE_NUM,
                        SLQSCDMAEncodeMOTextMsgTestCases[tCaseNum].desc);

            /* Required Structure parameters for encoding are filled */
            lEncodeTextToPDUCDMA.pMessageSize      =
                   SLQSCDMAEncodeMOTextMsgTestCases[tCaseNum].pMessageSize;
            lEncodeTextToPDUCDMA.messageId         =
                   SLQSCDMAEncodeMOTextMsgTestCases[tCaseNum].messageId;
            lEncodeTextToPDUCDMA.pDestAddr         =
                   SLQSCDMAEncodeMOTextMsgTestCases[tCaseNum].pDestAddr;
            lEncodeTextToPDUCDMA.pCallbackAddr     =
                   SLQSCDMAEncodeMOTextMsgTestCases[tCaseNum].pCallbackAddr;
            lEncodeTextToPDUCDMA.textMsgLength     =
                   SLQSCDMAEncodeMOTextMsgTestCases[tCaseNum].textMsgLength;
            lEncodeTextToPDUCDMA.pTextMsg          =
                   SLQSCDMAEncodeMOTextMsgTestCases[tCaseNum].pTextMsg;
            lEncodeTextToPDUCDMA.pPriority         =
                   SLQSCDMAEncodeMOTextMsgTestCases[tCaseNum].pPriority;
            lEncodeTextToPDUCDMA.pEncodingAlphabet =
                   SLQSCDMAEncodeMOTextMsgTestCases[tCaseNum].pEncodingAlphabet;
            lEncodeTextToPDUCDMA.pRelValidity      =
                   SLQSCDMAEncodeMOTextMsgTestCases[tCaseNum].pRelValidity;

            lEncodeTextToPDUCDMA.pMessage = hexa;

            fprintf(fp,"Message: ");
            fprintf(fp, "%s ", (CHAR*)lEncodeTextToPDUCDMA.pTextMsg);
            fprintf(fp,"\n");

            nRet = SLQSCDMAEncodeMOTextMsg( &lEncodeTextToPDUCDMA );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "Encoding of Text is Successful\n");

                /* Check if SMS sedning is successful? */
                nRet = SendSMS( messageFormat,
                                *lEncodeTextToPDUCDMA.pMessageSize,
                                (BYTE*)lEncodeTextToPDUCDMA.pMessage,
                                &messageFailureCode );

                fprintf(fp, "Return Code= %lx\n",nRet);
                if ( !nRet )
                {
                    fprintf(fp, "SMS sending is Successful\n");
                }
                else
                {
                    fprintf(fp, "SMS Sending fail:");
                    fprintf(fp, "Failure reason : %lx,SMS Cause code: %lx\n",\
                                               nRet,messageFailureCode );
                }
            }
            else
            {
                fprintf(fp, "Encoding of Text Failed\n");
                fprintf(fp, "Message Failure reason : %lx\n",\
                             nRet );
            }
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/**
 * Name:     doSLQSCDMADecodeMTTextMsg
 *
 * Purpose:  Perform the tests that call the SLQSCDMADecodeMTTextMsg() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSCDMADecodeMTTextMsg( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;
    BYTE  message[nMaxStrLen+2];
    CHAR  SenderAddr[16];
    WORD  TextMsg[162];
    ULONG messageTag[5]= {'\0'};
    ULONG messageFormat[5] = {'\0'};
    ULONG messageSize = nMaxStrLen + 2 ;
    BYTE  messageIndex[5]     = {  1, 2,   3,   4, 5};
    BYTE  SenderAddrLength[5] = { 16, 17,  2,  17, 18};
    BYTE  TextMsgLength[5]    = {163, 174, 2,   5, 163};
    BYTE  mcTimeStamp[8] = {0};
    ULONG messageID;
    BYTE  relativeValidity;
    BYTE  displayMode;
    BOOL  userAcknowledgementReq;
    BOOL  readAcknowledgementReq;
    BYTE  alertPriority;
    BYTE  callbkAddrLength;
    CHAR  callbkAddr;
    BYTE  priority;
    BYTE  privacy;
    BYTE  language;

    struct cdmaMsgDecodingParams lcdmaMsgDecodingParams;

    fp = tfopen("../../sms/test/results/slqscdmadecodetextmsg.txt", "w");
    if (fp)
    {
        while (tCaseNum <  MAX_SLQS_DECODE_CDMA_SMS_TESTCASE_NUM)
        {
            memset( message, 0, sizeof(message) );
            memset( TextMsg, 0, sizeof(TextMsg) );
            memset( SenderAddr, 0, sizeof(SenderAddr) );
            messageSize = nMaxStrLen + 2;

            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_SLQS_DECODE_CDMA_SMS_TESTCASE_NUM,
                        SLQSDecodeCDMASMSTestCases[tCaseNum].desc);

            /* Read SMS and store in buffer */
            nRet = SLQSGetSMS( STORAGEINDEX_UIM,
                               messageIndex[tCaseNum],
                               &messageTag[tCaseNum],
                               &messageFormat[tCaseNum],
                               (ULONG *)&messageSize,
                               &message[0],
                               &msgModeCDMA );

            if ( !nRet )
            {
                fprintf(fp,"GetSMS Successful\n");

                if ( tCaseNum < 1 )
                {
                    lcdmaMsgDecodingParams.messageLength           = messageSize;
                    lcdmaMsgDecodingParams.pMessage                = message;
                    lcdmaMsgDecodingParams.pMessageID              = NULL;
                    lcdmaMsgDecodingParams.pSenderAddrLength       =\
                                                   &SenderAddrLength[tCaseNum];
                    lcdmaMsgDecodingParams.pSenderAddr             =\
                                                   &SenderAddr[tCaseNum];
                    lcdmaMsgDecodingParams.pTextMsgLength          =\
                                                   &TextMsgLength[tCaseNum];
                    lcdmaMsgDecodingParams.pTextMsg                = TextMsg;
                    lcdmaMsgDecodingParams.pPriority               = NULL;
                    lcdmaMsgDecodingParams.pPrivacy                = NULL;
                    lcdmaMsgDecodingParams.pLanguage               = NULL;
                    lcdmaMsgDecodingParams.pRelativeValidity       = NULL;
                    lcdmaMsgDecodingParams.pDisplayMode            = NULL;
                    lcdmaMsgDecodingParams.pUserAcknowledgementReq = NULL;
                    lcdmaMsgDecodingParams.pReadAcknowledgementReq = NULL;
                    lcdmaMsgDecodingParams.pAlertPriority          = NULL;
                    lcdmaMsgDecodingParams.pCallbkAddrLength       = NULL;
                    lcdmaMsgDecodingParams.pCallbkAddr             = NULL;
                }
                else
                {
                    lcdmaMsgDecodingParams.messageLength           = messageSize;
                    lcdmaMsgDecodingParams.pMessage                = message;
                    lcdmaMsgDecodingParams.pMessageID              = &messageID;
                    lcdmaMsgDecodingParams.pSenderAddrLength       =\
                                                   &SenderAddrLength[tCaseNum];
                    lcdmaMsgDecodingParams.pSenderAddr             =
                                                   &SenderAddr[tCaseNum];
                    lcdmaMsgDecodingParams.pTextMsgLength          =\
                                                   &TextMsgLength[tCaseNum];
                    lcdmaMsgDecodingParams.pTextMsg                = TextMsg;
                    lcdmaMsgDecodingParams.pPriority               = &priority;
                    lcdmaMsgDecodingParams.pPrivacy                = &privacy;
                    lcdmaMsgDecodingParams.pLanguage               = &language;
                    lcdmaMsgDecodingParams.pRelativeValidity       =\
                                                   &relativeValidity;
                    lcdmaMsgDecodingParams.pDisplayMode            = &displayMode;
                    lcdmaMsgDecodingParams.pUserAcknowledgementReq =\
                                                   &userAcknowledgementReq;
                    lcdmaMsgDecodingParams.pReadAcknowledgementReq =\
                                                   &readAcknowledgementReq;
                    lcdmaMsgDecodingParams.pAlertPriority          =\
                                                   &alertPriority;
                    lcdmaMsgDecodingParams.pCallbkAddrLength       =\
                                                   &callbkAddrLength;
                    lcdmaMsgDecodingParams.pCallbkAddr             =\
                                                   &callbkAddr;
                }

                nRet = SLQSCDMADecodeMTTextMsg( &lcdmaMsgDecodingParams );

                if ( !nRet )
                {
                    fprintf(fp, " CDMA Decoding Successful\n");
                    doprintreason (fp, nRet);
                    fprintf(fp, "Message : ");
                    fprintf(stderr,"Sender Number: %s\n",SenderAddr);
                    fprintf(stderr,"SMS Text: %s\n",(CHAR*)TextMsg);
                    fprintf(stderr,"priority: %x\n",priority);
                    fprintf(stderr,"privacy: %x\n",privacy);
                    fprintf(stderr,"language: %x\n",language);
                    strncpy((CHAR*)mcTimeStamp,\
                                  (CHAR*)lcdmaMsgDecodingParams.mcTimeStamp, 7 );
                    fprintf(stderr,"relativeValidity: %c\n",
                                   relativeValidity);
                    fprintf(stderr,"displayMode: %x\n",
                                   displayMode);
                    fprintf(stderr,"userAcknowledgementReq: %x\n",
                                   userAcknowledgementReq);
                    fprintf(stderr,"readAcknowledgementReq: %x\n",
                                   readAcknowledgementReq);
                    fprintf(stderr,"alertPriority: %x\n",alertPriority);
                    fprintf(stderr,"callbkAddrLength: %x\n",callbkAddrLength);
                    fprintf(stderr,"callbkAddr: %x\n",callbkAddr);

                    fprintf(stderr,"Time Stamp: %.2X:%d:%d:%d:%d:%d\n",
                                    mcTimeStamp[0],mcTimeStamp[1],
                                    mcTimeStamp[2],mcTimeStamp[3],
                                    mcTimeStamp[4],mcTimeStamp[5]);
                }
                else
                {
                    fprintf(fp, "CDMA SMS Decoding fail\n");
                    doprintreason (fp, nRet);
                }
            }
            else
            {
                fprintf(fp, "SLQS Get SMS failed\n");
                doprintreason (fp, nRet);
            }
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/**
 * Name:     doSLQSWCDMAEncodeMOTextMsg
 *
 * Purpose:  Perform the tests that call the SLQSWCDMAEncodeMOTextMsg() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSWCDMAEncodeMOTextMsg( void )
{
    BYTE                          tCaseNum = 0;
    ULONG                         nRet;
    FILE                          *fp = NULL;
    BYTE                          hexa[200] = { 0 };
    ULONG                         MessageFailureCode = 0;
    ULONG                         messageFormat = 6;
    struct wcdmaMsgEncodingParams lEncodeTextToPDUWCDMA;

    fp = tfopen("../../sms/test/results/slqswcdmaencodetextmsg.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_ENCODE_WCDMA_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_ENCODE_WCDMA_TESTCASE_NUM,
                        SLQSWCDMAEncodeMOTextMsgTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Message Format : %lu\n",\
                SLQSWCDMAEncodeMOTextMsgTestCases[tCaseNum].messageFormat);
            fprintf(fp, "Message Size : %lu\n",\
                SLQSWCDMAEncodeMOTextMsgTestCases[tCaseNum].messageSize);

            /* Structure parameters required for encoding are filled */
            lEncodeTextToPDUWCDMA.messageSize =
                   SLQSWCDMAEncodeMOTextMsgTestCases[tCaseNum].messageSize;
            lEncodeTextToPDUWCDMA.pDestAddr   =
                   SLQSWCDMAEncodeMOTextMsgTestCases[tCaseNum].pDestAddr;
            lEncodeTextToPDUWCDMA.pTextMsg    =
                   SLQSWCDMAEncodeMOTextMsgTestCases[tCaseNum].pTextMsg;
            lEncodeTextToPDUWCDMA.pPDUMessage = (CHAR*)hexa;

            fprintf(fp,"Message: ");
            fprintf(fp, "%s ", lEncodeTextToPDUWCDMA.pTextMsg);
            fprintf(fp,"\n");

            nRet = SLQSWCDMAEncodeMOTextMsg( &lEncodeTextToPDUWCDMA );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "Encoding of Text is Successful\n");

                /* Check if SMS sending is successful? */
                nRet = SendSMS( (ULONG)(messageFormat),
                                lEncodeTextToPDUWCDMA.messageSize,
                                (BYTE*)lEncodeTextToPDUWCDMA.pPDUMessage,
                                &MessageFailureCode );
                if ( !nRet )
                {
                    fprintf(fp, "SMS sending is Successful\n");
                }
                else
                {
                    fprintf(fp, "Send SMS Failed\n" );
                    fprintf(fp, "Message Failure reason : %lx\n",\
                                                 nRet );
                    fprintf(fp, "SMS cause Code is = %ld\n",MessageFailureCode);
                }
            }
            else
            {
                fprintf(fp, "Encoding of Text Failed\n");
                fprintf(fp, "Message Failure reason : %lx\n",\
                             nRet );
            }
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/**
 * Name:     doSLQSWCDMADecodeMTTextMsg
 *
 * Purpose:  Perform the tests that call the SLQSWCDMADecodeMTTextMsg() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSWCDMADecodeMTTextMsg( void )
{
    BYTE                          tCaseNum = 0;
    ULONG                         nRet;
    FILE                          *fp = NULL;
    BYTE                          Message[nMaxStrLen+2];
    CHAR                          pSenderAddr[16];
    CHAR                          pScAddr[16];
    CHAR                          TextMsg[162];
    CHAR                          Date[9];
    CHAR                          Time[9];
    ULONG                         messageTag[5]= {'\0'};
    ULONG                         messageFormat[5] = {'\0'};
    ULONG                         messageSize = nMaxStrLen + 2 ;
    BYTE                          messageIndex[5]     = {  1, 2,   3,   4, 5};
    BYTE                          SenderAddrLength[5] = { 16, 2,  20,  17, 3};
    BYTE                          TextMsgLength[5]    = {163, 2, 174, 165, 3};
    BYTE                          ScAddrLength[5]     = { 16, 2,  20,  17, 3};
    struct wcdmaMsgDecodingParams lwcdmaMsgDecodingParams;

    fp = tfopen("../../sms/test/results/slqswcdmadecodemttextmsg.txt", "w");

    if (fp)
    {
        slmemset((CHAR*)Message, 0, sizeof(Message));
        slmemset(pSenderAddr,    0, sizeof(pSenderAddr));
        slmemset(TextMsg,        0, sizeof(TextMsg));
        slmemset(pScAddr,        0, sizeof(pScAddr));
        slmemset((CHAR*)Date,    0, sizeof(Date));
        slmemset(Time,           0, sizeof(Time));

        while (tCaseNum < MAX_DECODE_WCDMA_TESTCASE_NUM)
        {
            messageSize = nMaxStrLen;
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_DECODE_WCDMA_TESTCASE_NUM,
                        SLQSWCDMADecodeMTTextMsgTestCases[tCaseNum].desc);

            nRet = SLQSGetSMS( STORAGEINDEX_NV,
                               messageIndex[tCaseNum],
                               &messageTag[tCaseNum],
                               &messageFormat[tCaseNum],
                               (ULONG *)&messageSize,
                               &Message[0],
                               &msgModeGW );
            if ( !nRet )
            {
                fprintf(fp, "SLQS GetSMS Successful\n");

                lwcdmaMsgDecodingParams.pMessage          =\
                          Message;
                lwcdmaMsgDecodingParams.pSenderAddrLength =\
                          &SenderAddrLength[tCaseNum];
                lwcdmaMsgDecodingParams.pSenderAddr       = pSenderAddr;
                lwcdmaMsgDecodingParams.pTextMsgLength    =\
                           &TextMsgLength[tCaseNum];
                lwcdmaMsgDecodingParams.pScAddrLength     =\
                           &ScAddrLength[tCaseNum];
                lwcdmaMsgDecodingParams.pTextMsg          = TextMsg;
                lwcdmaMsgDecodingParams.pScAddr           = pScAddr;

                nRet = SLQSWCDMADecodeMTTextMsg( &lwcdmaMsgDecodingParams );

                if ( !nRet )
                {
                    fprintf(fp, "WCDMA Decoding Successful\n");
                    doprintreason (fp, nRet);
                    fprintf(fp, "\nService Center Number = %s",pScAddr);
                    fprintf(fp, "\nSender Number = %s",pSenderAddr);

                    strncpy(Time, (CHAR*)lwcdmaMsgDecodingParams.Time, 9 );
                    strncpy(Date, (CHAR*)lwcdmaMsgDecodingParams.Date, 9 );

                    fprintf(fp, "\nDate = %s",Date);
                    fprintf(fp, "\nTime = %s",Time);
                    fprintf(fp, "\nMessage: %s\n\n",TextMsg );
                }
                else
                {
                    fprintf(fp, "SLQS WCDMA Decoding failed \n");
                    doprintreason (fp, nRet);

                }
            }
            else
            {
                fprintf(fp, "SLQS Get SMS failed\n");
                doprintreason (fp, nRet);
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}
void doExecuteSmsStubs()
{
}

/*
 * Name:    doSLQSGetTransLayerInfo
 *
 * Purpose: Perform the tests that call the SLQSGetTransLayerInfo() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSGetTransLayerInfo( void )
{
    /* Test Cases */
    SLQSGetTransLayerInfoTestCase_t SLQSGetTransLayerInfoTestCases[] =
    {
        { eQCWWAN_ERR_NONE, 1, NULL, "Invalid Argument - Response parameter NULL"},
        { eQCWWAN_ERR_NONE, 2, &GetTransLayerInfoResp, "Valid test case"},
    };

    FILE  *fp = tfopen("../../sms/test/results/slqsgettranslayerinfo.txt", "w");
    getTransLayerInfoResp *pResp;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSGetTransLayerInfoTestCases )/
                      sizeof( SLQSGetTransLayerInfoTestCases[0] );
        while ( tc < ncases )
        {
            SLQSGetTransLayerInfoTestCase_t *pd = &SLQSGetTransLayerInfoTestCases[tc];
            fprintf( fp, "\nTest Case %d / %d : %s\n",
                        (tc + 1),
                        ncases,
                        pd->desc );

            pResp = pd->pGetTransLayerInfoResp;
            if( pResp )
            {
                pResp->pRegInd = pRegInd;
                pResp->pTransLayerInfo = pTransLayerInfo;
            }
            ULONG nRet = SLQSGetTransLayerInfo( pResp );

            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSGetTransLayerInfo Successful\n");

                IFPRINTF( fp, "Registered Indication        : 0x%x \n", pResp->pRegInd );

                if ( NULL != pResp->pTransLayerInfo )
                {
                    transLayerInfo *pTransLayerInfo = pResp->pTransLayerInfo;

                    fprintf(fp, "Transport Type        : 0x%x \n",
                            pTransLayerInfo->TransType);
                    fprintf(fp, "Transport Capabilities: 0x%x \n",
                            pTransLayerInfo->TransCap);
                }
            }
            tc++;
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSGetTransNWRegInfo
 *
 * Purpose: Perform the tests that call the SLQSGetTransNWRegInfo() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSGetTransNWRegInfo( void )
{
    /* Test Cases */
    SLQSGetTransNWRegInfoTestCase_t SLQSGetTransNWRegInfoTestCases[] =
    {
        { eQCWWAN_ERR_NONE, 1, NULL, "Invalid Argument - Response parameter NULL"},
        { eQCWWAN_ERR_NONE, 2, &GetTransNWRegInfoResp, "Valid test case"},
    };

    FILE  *fp = tfopen("../../sms/test/results/slqsgettransnwreginfo.txt", "w");
    getTransNWRegInfoResp *pResp;
    BYTE NWRegStat;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSGetTransNWRegInfoTestCases )/
                      sizeof( SLQSGetTransNWRegInfoTestCases[0] );
        while ( tc < ncases )
        {
            SLQSGetTransNWRegInfoTestCase_t *pd = &SLQSGetTransNWRegInfoTestCases[tc];
            fprintf( fp, "\nTest Case %d / %d : %s\n",
                        (tc + 1),
                        ncases,
                        pd->desc );

            pResp = pd->pTransNWRegInfo;
            if( pResp )
            {
                pResp->pRegStatus = &NWRegStat;
            }

            ULONG nRet = SLQSGetTransNWRegInfo( pResp );

            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf( fp, "SLQSGetTransNWRegInfo Successful\n" );
                IFPRINTF( fp, "Registration Status        : 0x%x \n", pResp->pRegStatus );
            }
            tc++;
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSGetIndicationRegister
 *
 * Purpose: Perform the tests that call the SLQSGetIndicationRegister() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSGetIndicationRegister( void )
{
    /* Test Cases */
    SLQSGetIndicationRegisterTestCase_t SLQSGetIndicationRegisterTestCases[] =
    {
        { eQCWWAN_ERR_NONE, 1, NULL, "Invalid Argument - Response parameter NULL"},
        { eQCWWAN_ERR_NONE, 2, &GetIndRegisterResp, "Valid test case"},
    };

    FILE  *fp = tfopen("../../sms/test/results/slqsgetindicationregister.txt", "w");
    getIndicationRegResp *pResp;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSGetIndicationRegisterTestCases )/
                      sizeof( SLQSGetIndicationRegisterTestCases[0] );
        while ( tc < ncases )
        {
            SLQSGetIndicationRegisterTestCase_t *pd = &SLQSGetIndicationRegisterTestCases[tc];
            fprintf( fp, "\nTest Case %d / %d : %s\n",
                        (tc + 1),
                        ncases,
                        pd->desc );

            pResp = pd->pGetIndicationRegResp;
            if( pResp )
            {

                pResp->pRegTransLayerInfoEvt = pTransLayerInfoStat;
                pResp->pRegTransNWRegInfoEvt = pTransNWRegInfoStat;
                pResp->pRegCallStatInfoEvt   = pCallInfoStat;
            }

            ULONG nRet = SLQSGetIndicationRegister( pResp );

            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf( fp, "SLQSGetIndicationRegister Successful\n" );
                IFPRINTF( fp, "Transport layer Info events registration status: 0x%x \n",
                          pResp->pRegTransLayerInfoEvt );
                IFPRINTF( fp, "Transport Network Reg events registration Status: 0x%x \n",
                          pResp->pRegTransNWRegInfoEvt );
                IFPRINTF( fp, "Call Status Info events: 0x%x \n",
                          pResp->pRegCallStatInfoEvt );
            }
            tc++;
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSSetIndicationRegister
 *
 * Purpose: Perform the tests that call the SLQSSetIndicationRegister() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSSetIndicationRegister( void )
{
    /* Test Cases */
    SLQSSetIndicationRegisterTestCase_t SLQSSetIndicationRegisterTestCases[] =
    {
        { eQCWWAN_ERR_NONE, 1, NULL, "Invalid Test Case - "},

        { eQCWWAN_ERR_NONE, 2, &SetIndicationRegReq, "Valid test case - All Events enabled"},

    };

    FILE  *fp = tfopen("../../sms/test/results/slqssetindicationregister.txt", "w");
    setIndicationRegReq *pResp;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSSetIndicationRegisterTestCases )/
                      sizeof( SLQSSetIndicationRegisterTestCases[0] );
        while ( tc < ncases )
        {
            SLQSSetIndicationRegisterTestCase_t *pd = &SLQSSetIndicationRegisterTestCases[tc];

            pResp = pd->pSetIndicationRegReq;

            if( pResp )
            {
                pResp->pRegTransLayerInfoEvt = pTransLayerInfoEvtStat;
                pResp->pRegTransNWRegInfoEvt = pTransNWRegInfoEvtStat;
                pResp->pRegCallStatInfoEvt   = pCallInfoEvtStat;
            }
            fprintf( fp, "\nTest Case %d / %d : %s\n",
                        (tc + 1),
                        ncases,
                        pd->desc );

            fprintf( fp, "SLQSSetIndicationRegister parameters set:\n" );
            if( pResp )
            {
                IFPRINTF( fp, "Transport layer Info events registration status: 0x%x \n",
                          pResp->pRegTransLayerInfoEvt );
                IFPRINTF( fp, "Transport Network Reg events registration Status: 0x%x \n",
                          pResp->pRegTransNWRegInfoEvt );
                IFPRINTF( fp, "Call Status Info events: 0x%x \n",
                          pResp->pRegCallStatInfoEvt );
            }

            ULONG nRet = SLQSSetIndicationRegister( pResp );
            doprintreason( fp, nRet );

            if ( !nRet )
            {
                fprintf(fp, "SLQSSetIndicationRegister Successful\n");
            }
            tc++;
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSSmsSetRoutes
 *
 * Purpose: Perform the tests that call the SLQSSmsSetRoutes() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSSmsSetRoutes( void )
{
    WORD lCount;
    smsSetRoutesReq SetRoutesReq;

    memset (&SetRoutesReq, 0, sizeof(SetRoutesReq));
    SetRoutesReq.numOfRoutes = 4;
    SetRoutesReq.routeList[0].messageType   = 0x00;
    SetRoutesReq.routeList[0].messageClass  = 0x00;
    SetRoutesReq.routeList[0].routeStorage  = 0xFF;
    SetRoutesReq.routeList[0].receiptAction = 0x03;

    SetRoutesReq.routeList[1].messageType   = 0x00;
    SetRoutesReq.routeList[1].messageClass  = 0x01;
    SetRoutesReq.routeList[1].routeStorage  = 0xFF;
    SetRoutesReq.routeList[1].receiptAction = 0x03;

    SetRoutesReq.routeList[2].messageType   = 0x00;
    SetRoutesReq.routeList[2].messageClass  = 0x02;
    SetRoutesReq.routeList[2].routeStorage  = 0xFF;
    SetRoutesReq.routeList[2].receiptAction = 0x03;

    SetRoutesReq.routeList[3].messageType   = 0x00;
    SetRoutesReq.routeList[3].messageClass  = 0x03;
    SetRoutesReq.routeList[3].routeStorage  = 0xFF;
    SetRoutesReq.routeList[3].receiptAction = 0x03;

    smsSetRoutesReq SetRoutesReq1;

    memset (&SetRoutesReq1, 0, sizeof(SetRoutesReq1));
    SetRoutesReq1.numOfRoutes = 4;

    SetRoutesReq1.routeList[0].messageType   = 0x00;
    SetRoutesReq1.routeList[0].messageClass  = 0x00;
    SetRoutesReq1.routeList[0].routeStorage  = 0x00;
    SetRoutesReq1.routeList[0].receiptAction = 0x01;

    SetRoutesReq1.routeList[1].messageType   = 0x00;
    SetRoutesReq1.routeList[1].messageClass  = 0x01;
    SetRoutesReq1.routeList[1].routeStorage  = 0x00;
    SetRoutesReq1.routeList[1].receiptAction = 0x01;

    SetRoutesReq1.routeList[2].messageType   = 0x00;
    SetRoutesReq1.routeList[2].messageClass  = 0x02;
    SetRoutesReq1.routeList[2].routeStorage  = 0x00;
    SetRoutesReq1.routeList[2].receiptAction = 0x01;

    SetRoutesReq1.routeList[3].messageType   = 0x00;
    SetRoutesReq1.routeList[3].messageClass  = 0x03;
    SetRoutesReq1.routeList[3].routeStorage  = 0x00;
    SetRoutesReq1.routeList[3].receiptAction = 0x01;

    smsSetRoutesReq SetRoutesReq2;

    memset (&SetRoutesReq2, 0, sizeof(SetRoutesReq2));
    SetRoutesReq2.numOfRoutes = 4;
    SetRoutesReq2.routeList[0].messageType   = 0x00;
    SetRoutesReq2.routeList[0].messageClass  = 0x00;
    SetRoutesReq2.routeList[0].routeStorage  = 0x01;
    SetRoutesReq2.routeList[0].receiptAction = 0x01;

    SetRoutesReq2.routeList[1].messageType   = 0x00;
    SetRoutesReq2.routeList[1].messageClass  = 0x01;
    SetRoutesReq2.routeList[1].routeStorage  = 0x01;
    SetRoutesReq2.routeList[1].receiptAction = 0x01;

    SetRoutesReq2.routeList[2].messageType   = 0x00;
    SetRoutesReq2.routeList[2].messageClass  = 0x02;
    SetRoutesReq2.routeList[2].routeStorage  = 0x01;
    SetRoutesReq2.routeList[2].receiptAction = 0x01;

    SetRoutesReq2.routeList[3].messageType   = 0x00;
    SetRoutesReq2.routeList[3].messageClass  = 0x03;
    SetRoutesReq2.routeList[3].routeStorage  = 0x01;
    SetRoutesReq2.routeList[3].receiptAction = 0x01;

    SLQSSetsmsRouteTestCase_t SLQSSetsmsRouteTestCases[] =
    {
        { eQCWWAN_ERR_NONE, 1, &SetRoutesReq,  "Valid Test Case - "},
        { eQCWWAN_ERR_NONE, 2, &SetRoutesReq1, "Valid Test Case - "},
        { eQCWWAN_ERR_NONE, 3, &SetRoutesReq2, "Valid Test Case - "},
    };

    FILE  *fp = tfopen("TestReport/slqssmssetroutes.txt", "w");
    smsSetRoutesReq *pReq;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSSetsmsRouteTestCases )/
                    sizeof( SLQSSetsmsRouteTestCases[0] );
        while ( tc < ncases )
        {
            SLQSSetsmsRouteTestCase_t *pd = &SLQSSetsmsRouteTestCases[tc++];
            fprintf( fp, "\nTest Case %d / %d : %s\n",
                tc,
                ncases,
                pd->desc );

            fprintf( fp, "SLQSSmsSetRoutes parameters set:\n" );

            pReq = pd->pSetRoutesReq;
            fprintf( fp, "Number of Routes : %d\n",pReq->numOfRoutes);

            for(lCount=0;lCount<pReq->numOfRoutes;lCount++)
            {
                fprintf( fp, "\nMessage Type     : %d\n",
                                        pReq->routeList[lCount].messageType);
                fprintf( fp, "Message Class    : %d\n",
                                        pReq->routeList[lCount].messageClass);
                fprintf( fp, "Route Storage    : %d\n",
                                        pReq->routeList[lCount].routeStorage);
                fprintf( fp, "Receipt Action   : %d\n",
                                        pReq->routeList[lCount].receiptAction);
            }

            IFPRINTF( fp, "Transfer Status Report : %x\n",
                                        pReq->pTransferStatusReport );

            ULONG nRet = SLQSSmsSetRoutes( pReq );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf( fp, "SLQSSmsSetRoutes Successful \n ");
                continue;
            }
            fprintf( fp, "SLQSSmsSetRoutes Unsuccessful \n ");
        }
    }
    if (fp)
        tfclose(fp);
}

void doSLQSSmsGetMessageProtocol(void)
{
    smsMsgprotocolResp resp;

    /* Test Cases */
    SLQSSmsGetMessageProtocolTestCase_t SLQSSmsGetMessageProtocolTestCases[] =
    {
        { eQCWWAN_ERR_NONE, 1, NULL, "Invalid Test Case"},
        { eQCWWAN_ERR_NONE, 2, &resp, "Valid test Case"},
    };

    FILE  *fp = tfopen("TestReport/slqssmsgetmessageprotocol.txt", "w");
    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSSmsGetMessageProtocolTestCases )/
                      sizeof( SLQSSmsGetMessageProtocolTestCases[0] );

        while ( tc < ncases )
        {
            SLQSSmsGetMessageProtocolTestCase_t *pd =
                    &SLQSSmsGetMessageProtocolTestCases[tc++];

            fprintf( fp, "\nTest Case %d / %d : %s\n",
                        (tc),
                        ncases,
                        pd->desc );

            ULONG nRet = SLQSSmsGetMessageProtocol( pd->pMessageProtocol );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSSmsGetMessageProtocol Successful\n");
                fprintf(fp, "Message Protocol: %x\n", resp.msgProtocol );
            }
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSSmsGetMaxStorageSize
 *
 * Purpose: Perform the tests that call the SLQSSmsGetMaxStorageSize() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSSmsGetMaxStorageSize(void)
{
    smsMaxStorageSizeResp resp;
    smsMaxStorageSizeResp *pResp;

    smsMaxStorageSizeReq req_uim;
    slmemset((char*)&req_uim, 0, sizeof(req_uim));
    req_uim.storageType = QMI_WMS_UIM;
    req_uim.pMessageMode = NULL;

    smsMaxStorageSizeReq req_nv;
    slmemset((char*)&req_nv, 0, sizeof(req_nv));
    req_nv.storageType = QMI_WMS_NV;
    req_nv.pMessageMode = NULL;

    smsMaxStorageSizeReq req_uim_gw;
    slmemset((char*)&req_uim_gw, 0, sizeof(req_uim_gw));
    req_uim_gw.storageType = QMI_WMS_UIM;
    req_uim_gw.pMessageMode = &msgModeGW;

    smsMaxStorageSizeReq req_uim_cdma;
    slmemset((char*)&req_uim_cdma, 0, sizeof(req_uim_cdma));
    req_uim_cdma.storageType = QMI_WMS_UIM;
    req_uim_cdma.pMessageMode = &msgModeCDMA;

    smsMaxStorageSizeReq req_nv_gw;
    slmemset((char*)&req_nv_gw, 0, sizeof(req_nv_gw));
    req_nv_gw.storageType = QMI_WMS_UIM;
    req_nv_gw.pMessageMode = &msgModeGW;

    smsMaxStorageSizeReq req_nv_cdma;
    slmemset((char*)&req_nv_cdma, 0, sizeof(req_nv_cdma));
    req_nv_cdma.storageType = QMI_WMS_UIM;
    req_nv_cdma.pMessageMode = &msgModeCDMA;

    /* Test Cases */
    SLQSSmsGetMaxStorageSizeTestCase_t SLQSSmsGetMaxStorageSizeTestCases[] =
    {
        { eQCWWAN_ERR_INVALID_ARG, 1, NULL,\
          NULL,  "Invalid Argument - NULL Request & Response"},
        { eQCWWAN_ERR_INVALID_ARG, 2, &req_uim,\
          NULL,  "Invalid Argument - NULL Response"},
        { eQCWWAN_ERR_INVALID_ARG, 3, NULL,\
          &resp, "Invalid Argument - NULL Request"},
        { eQCWWAN_ERR_NONE, 4, &req_uim,\
          &resp, "Valid test Case UIM"},
        { eQCWWAN_ERR_NONE, 5, &req_nv, \
          &resp, "Valid test Case NV"},
        { eQCWWAN_ERR_NONE, 6, &req_uim_gw, \
          &resp, "Valid test Case UIM GW"},
        { eQCWWAN_ERR_NONE, 7, &req_uim_cdma, \
          &resp, "Valid test Case UIM CDMA"},
        { eQCWWAN_ERR_NONE, 8, &req_nv_gw, \
          &resp, "Valid test Case NV GW"},
        { eQCWWAN_ERR_NONE, 9, &req_nv_cdma, \
          &resp, "Valid test Case NV CDMA"},
    };

    FILE  *fp = tfopen("TestReport/slqssmsgetstoremaxsize.txt", "w");

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSSmsGetMaxStorageSizeTestCases )/
                      sizeof( SLQSSmsGetMaxStorageSizeTestCases[0] );

        while ( tc < ncases )
        {
            SLQSSmsGetMaxStorageSizeTestCase_t *pd =
                    &SLQSSmsGetMaxStorageSizeTestCases[tc++];

            fprintf( fp, "\nTest Case %d / %d : %s\n",
                        (tc),
                        ncases,
                        pd->desc );

            if (pd->pReq)
            {
                fprintf(fp, "Storage Type : 0x%02x\n",\
                        pd->pReq->storageType);

                if ( pd->pReq->pMessageMode )
                    fprintf(fp, "Message Mode : 0x%02x\n",\
                            *( pd->pReq->pMessageMode ) );
            }

            pResp = pd->pResp;
            ULONG nRet = SLQSSmsGetMaxStorageSize( pd->pReq,
                                                 pd->pResp );

            /* Display result code and text */
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSSmsGetMaxStorageSize Successful\n");
                fprintf( fp, "Max Storage Size: %lu\n", pResp->maxStorageSize );
                fprintf( fp, "Storage Free Slots: 0x%lx \n", pResp->freeSlots );
            }
        }
    }
    if (fp)
        tfclose(fp);
}
