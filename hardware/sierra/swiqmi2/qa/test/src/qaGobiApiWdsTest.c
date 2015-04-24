/*
 *
 *  Filename: qaGobiApiWdsTest.c
 *
 *  Purpose:  Contains functions implementing specific tests for
 *            Wireless Data Service (WDS)
 *            called by the main() routine in qatest.c
 *
 *  Copyright: © 2011-12 Sierra Wireless, Inc., all rights reserved
 *
 **************/

/* include files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* include files */
#include "SwiDataTypes.h"
#include "qmerrno.h"
#include "qaGobiApiWds.h"
#include "qaGobiApiWdsTest.h"
#include "qatesthelper.h"
#include "qatestproto.h"
#include "sludefs.h"

/*******************
    TEST DATA
 *******************/
/* Constants */
#define IPv6 6

/* Global Variables */
local ULONG SessionID = 0;
local ULONG FailureReason = 0;
local struct ssdatasession_params s;

local WORD SetProfileList[5] = { 0x01, 0x02, 0x04, 0x03, 0x05 };
local WORD SetLTEAttachProfile = 1;
local BYTE SetDefaultPDNEnabled = 1;
local BYTE SetRelease3GPP = 2;

local WORD LTEAttachProfile;
local WORD ProfileList[5];
local BYTE defaultPDNEnabled;
local BYTE Release3GPP;

/* Get ProfileSettings test data */
local BYTE                    profType3GPP = 0;
local BYTE                    profIndex = 1;
local CHAR                    profName[255];
local WORD                    ValprofNameSize = 255;
local WORD                    InValprofNameSize = 0;
local BYTE                    PdpType;
local BYTE                    PDPHDRCompType;
local BYTE                    PDPDataCompType;
local CHAR                    ApnName[255];
local WORD                    ValApnNameSize = 255;
local WORD                    InValApnNameSize = 0;
local ULONG                   PriDNSIPv4AddPref;
local ULONG                   SecDNSIPv4AddPref;
local struct UMTSQoS          UMTSReqQoS;
local struct UMTSQoS          UMTSMinQos;
local struct GPRSRequestedQoS GPRSReqQoS;
local struct GPRSRequestedQoS GPRSminQoS;
local CHAR                    UserName[255];
local WORD                    ValUserNameSize = 255;
local WORD                    InValUserNameSize = 0;
local CHAR                    Passwd[255];
local WORD                    ValPasswdSize = 255;
local WORD                    InValPasswdSize = 0;
local BYTE                    AuthPref;
local ULONG                   IPv4AddPref;
local BYTE                    PCSCFAddPCO;
local BYTE                    PDPAccCtrlFlag;
local BYTE                    PCSCFAddDHCP;
local BYTE                    IMCNFlag;
local struct TFTIDParams      TFTID1;
local struct TFTIDParams      TFTID2;
local BYTE                    PDPCon;
local BYTE                    SecFlag;
local BYTE                    priID;
local USHORT                  IPv6AddrPref[8];
local struct UMTSReqQoSSigInd UMTSReqQoSsigInd;
local struct UMTSReqQoSSigInd UMTSMinQoSsigInd;
local USHORT                  priDNSIPv6AddPref[8];
local USHORT                  secDNSIPv6AddPref[8];
local BYTE                    addrAllocPref;
local struct QosClassID       QoSCLSID;
local BYTE                    ApnDisFlag;
local ULONG                   PDNInactivTim;
local BYTE                    APNClass;
local WORD                    extErrCode;

local BYTE   profType3GPP2 = 1;
local BYTE   profIndex3GPP2 = 0;
local BYTE   NegoDNSServPref;
local ULONG  PPPSessCloTimD0;
local ULONG  PPPSessCloTim1X;
local BYTE   AllowLinger;
local USHORT LcpAckTimeout;
local USHORT IpcpAckTimeout;
local USHORT AuthTimeout;
local BYTE   LcpCreqRetCt;
local BYTE   IpCpCReqRetCt;
local BYTE   authRetryCt;
local BYTE   authProt;
local CHAR   userID[127];
local WORD   ValuserIDSize = 255;
local WORD   InValuserIDSize = 0;
local CHAR   authPwd[127];
local WORD   ValauthPwdSize = 255;
local WORD   InValauthPwdSize = 0;
local BYTE   DataRate;
local ULONG  AppType;
local BYTE   DataMode;
local BYTE   appPrio;
local CHAR   ApnStr[100];
local WORD   ValApnStrSize = 255;
local WORD   InValApnStrSize = 0;
local BYTE   PdnType;
local BYTE   isPcscfAdd;
local ULONG  PriDNSIPv4Add;
local ULONG  SecDNSIPv4Add;
local USHORT PriDNSIPv6Add[8];
local USHORT SecDNSIPv6Add[8];
local BYTE   RatType;
local BYTE   ApnEnable3GPP2;

/* Wds Set Event Report parameters */
local BYTE      unSetInd        = 0x00;
local BYTE      setInd          = 0x01;
local TrStatInd transferStatInd = { 0x10, 0x00000080 };

/*******************
  TEST FUNCTIONS
 *******************/

/*
 * Name:     doStartDataSession2
 *
 * Purpose:  Perform the tests that call the StartDataSession2() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doStartDataSession2(void)
{
    ULONG techpreference_UMTS  = 1 ;
    ULONG techpreference_CDMA  = 2 ;
    ULONG techpreference_eMBMS = 3 ;
    ULONG techpreference_mLink = 4 ;

    /* Test Cases */
    StartDataSession2TestCase_t StartDataSession2TestCases[] =
    {
#if 0
        { eQCWWAN_ERR_INVALID_ARG,1,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
          &FailureReason,"Session ID NULL" },

        { eQCWWAN_ERR_INVALID_ARG,2,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
          &SessionID,NULL,"Failure Reason NULL" },

        { eQCWWAN_ERR_INVALID_ARG,3,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
          NULL,"Session ID and Failure Reason NULL" },
#endif
        { eQCWWAN_ERR_NONE,1,&techpreference_UMTS,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
          &SessionID,&FailureReason,"All Valid 3GPP - UMTS" },
        { eQCWWAN_ERR_NONE,2,&techpreference_CDMA,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
          &SessionID,&FailureReason,"All Valid 3GPP - CDMA" },
        { eQCWWAN_ERR_NONE,3,&techpreference_eMBMS,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
          &SessionID,&FailureReason,"All Valid 3GPP - eMBMS " },
        { eQCWWAN_ERR_NONE,4,&techpreference_mLink,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
          &SessionID,&FailureReason,"All Valid 3GPP - mLINK" },

    };

    FILE *fp = tfopen("../../wds/test/results/startdatasession.txt","w");
    if(fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( StartDataSession2TestCases ) /
                      sizeof( StartDataSession2TestCases[0] );
        while(tc < ncases )
        {
            StartDataSession2TestCase_t *pd = StartDataSession2TestCases + tc;

            fprintf( fp, "\n\nTest Case %d / %d : %s\n",
                     tc + 1,
                     ncases,
                     pd->desc);

            fprintf(fp,"Parameter Set:\n");
            SFPRINTF(fp, "Technology: %ld\n",       pd->pTechnology );
            SFPRINTF(fp, "Primary DNS: %ld\n",      pd->pPrimaryDNS );
            SFPRINTF(fp, "Secondary DNS: %ld\n",    pd->pSecondaryDNS );
            SFPRINTF(fp, "Primary NBNS: %ld\n",     pd->pPrimaryNBNS );
            SFPRINTF(fp, "Secondary NBNS: %ld\n",   pd->pSecondaryNBNS );
            VFPRINTF(fp, "APN Name: %s\n",          pd->pAPNName );
            SFPRINTF(fp, "IP Address: %ld\n",       pd->pIPAddress );
            SFPRINTF(fp, "Authentication: %ld\n",   pd->pAuthentication );
            VFPRINTF(fp, "Username: %s\n",          pd->pUsername );
            VFPRINTF(fp, "Password: %s\n",          pd->pPassword );

            ULONG nRet = StartDataSession2( pd->pTechnology,
                                            pd->pPrimaryDNS,
                                            pd->pSecondaryDNS,
                                            pd->pPrimaryNBNS,
                                            pd->pSecondaryNBNS,
                                            pd->pAPNName,
                                            pd->pIPAddress,
                                            pd->pAuthentication,
                                            pd->pUsername,
                                            pd->pPassword,
                                            pd->pSessionID,
                                            pd->pFailureReason );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( eQCWWAN_ERR_NONE == nRet)
            {
                fprintf(fp, "session id: %lu\n", *pd->pSessionID );
                SessionID = *pd->pSessionID;
            }
            else
            {
                fprintf(fp,
                        "failure reason: %lu\nsession id: %lu\n",
                        *pd->pFailureReason,
                        *pd->pSessionID );
            }
            tc++;
        }
    }

    if(fp)
        tfclose(fp);
}

/*
 * Name:     doStopDataSession
 *
 * Purpose:  Perform the tests that call the StopDataSession() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doStopDataSession( void )
{
    /* Test Cases */
    const StopDataSessionTestCase_t StopDataSessionTestCases[] =
    {
        { eQCWWAN_ERR_NONE,1,0,
          "Stop a session using most recent session id" },
    };

    FILE *fp = tfopen("../../wds/test/results/stopdatasession.txt","w");

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( StopDataSessionTestCases ) /
                      sizeof( StopDataSessionTestCases[0] );
        while(tc < ncases )
        {
            fprintf(fp,"\n\nTest Case %d / %d : %s\n",
                       (tc + 1),
                       ncases,
                       StopDataSessionTestCases[tc].desc);

            ULONG nRet = StopDataSession(SessionID);

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp,"\nSuccessfully Stopped the session");
            }

            tc++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 *
 * Name:     doStartDataSessionLTE
 *
 * Purpose:  Perform the tests that call the StartDataSessionLTE() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doStartDataSessionLTE( void )
{
    ULONG  nRet;
    /* StartDataSessionLTE */
    StartDataSessionLTETestCase_t StartDataSessionLTETestCases[] =
    {
        { eQCWWAN_ERR_NONE,1,1,0,0,0,0,0,0,"",0,{ 0,0,0,0,0,0,0,0 },0,"","",
          "All Valid IPv4v6 - UMTS ",7 },
        { eQCWWAN_ERR_NONE,2,2,0,0,0,0,0,0,"",0,{ 0,0,0,0,0,0,0,0 },0,"","",
          "All Valid IPv4v6 - CDMA",7 },
        { eQCWWAN_ERR_NONE,3,3,0,0,0,0,0,0,"",0,{ 0,0,0,0,0,0,0,0 },0,"","",
          "All Valid IPv4v6 - eMBMS",7 },
        { eQCWWAN_ERR_NONE,4,4,0,0,0,0,0,0,"",0,{ 0,0,0,0,0,0,0,0 },0,"","",
          "All Valid IPv4v6 - mLINK",7 }
#if 0
        { eQCWWAN_ERR_NONE,1,1,0,0,0,0,0,0,"",0,{ 0,0,0,0,0,0,0,0 },0,
          "","","All Valid 3GPP",6 }
#endif
    };

    FILE *fp= tfopen("../../wds/test/results/startdatasessionlte.txt","w");

    if( NULL != fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( StartDataSessionLTETestCases ) /
                      sizeof( StartDataSessionLTETestCases[0] );
        while( tc < ncases )
        {
            StartDataSessionLTETestCase_t *pd = &StartDataSessionLTETestCases[tc];

            fprintf( fp, "\n\nTest Case %d / %d : %s\n",
                                 tc + 1,
                                 ncases,
                                 pd->desc);

            fprintf(fp, "Parameter Set:\nTechnology :%ld\nPrimaryDNSv4 :%ld\n"\
                        "SecondaryDNSv4: %ld\nPrimaryNBNSv4: %ld\n"\
                        "SecondaryNBNSv4: %ld\nPrimaryDNSv6: %d\n"\
                        "SecondaryDNSv6: %d\nAPNName: %s\nIPAddressv4: %ld\n",
                        pd->Technology,     pd->PrimaryDNSv4,    pd->SecondaryDNSv4,
                        pd->PrimaryNBNSv4,  pd->SecondaryNBNSv4, pd->PrimaryDNSv6,
                        pd->SecondaryDNSv6, pd->APNName,         pd->IPAddressv4 );

            fprintf(fp, "IPV6 Address: ");
            BYTE idx;
            for (idx = 0; idx < 8; idx++)
            {
                fprintf(fp, "%hx", pd->IPAddressv6[idx] );
            }

            fprintf(fp, "\nAuthentication: %ld\nUsername: %s\nPassword: %s\n",
                        pd->Authentication, pd->Username, pd->Password );
#if 0
            SLQSSetIPFamilyPreference(pd->ipfamily);
#endif
            nRet = StartDataSessionLTE( NULL, NULL, NULL,
                                        NULL, NULL, NULL,
                                        NULL, NULL, NULL,
                                        NULL, NULL, NULL,
                                        NULL, &SessionID, &FailureReason,
                                        0 );
#if 0
            nRet = StartDataSessionLTE( &pd->Technology,     &pd->PrimaryDNSv4,    &pd->SecondaryDNSv4,
                                        &pd->PrimaryNBNSv4,  &pd->SecondaryNBNSv4, &pd->PrimaryDNSv6,
                                        &pd->SecondaryDNSv6, pd->APNName,          &pd->IPAddressv4,
                                        pd->IPAddressv6,     &pd->Authentication,  pd->Username,
                                        pd->Password,        &SessionID,           &FailureReason,
                                        pd->ipfamily );
#endif
             /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "session id: %lu\n", SessionID );
            }
            else
            {
                fprintf( fp,
                         "failure reason: %lu\nsession id: %lu\n",
                         FailureReason,
                         SessionID );
            }

            tc++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetDefaultProfile
 *
 * Purpose:  Perform the tests that call the GetDefaultProfile() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetDefaultProfile( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
#if 0
    ULONG rCodeSetDP;
    ULONG setPDPType = 0;
    ULONG setIPAddress = 0;
    ULONG setPrimaryDNS = 0;
    ULONG setSecondaryDNS = 0;
    ULONG setAuthentication = 0;
    CHAR  setName[nMaxStrLen] = "Infosys";
    CHAR  setAPNName[nMaxStrLen] = "internet.com";
    CHAR  setUsername[nMaxStrLen] = "wapuser";
    CHAR  setPassword[nMaxStrLen] = "w";
#endif
    ULONG profileType;
    ULONG PDPType;
    ULONG IPAddress;
    ULONG PrimaryDNS;
    ULONG SecondaryDNS;
    ULONG Authentication;
    BYTE  nameSize = nMaxStrLen-1;
    CHAR  Name[nMaxStrLen];
    BYTE  apnSize = nMaxStrLen-1;
    CHAR  APNName[nMaxStrLen];
    BYTE  userSize = nMaxStrLen-1;
    CHAR  Username[nMaxStrLen];

    FILE  *fp=NULL;

    fp = tfopen("../../wds/test/results/getdefaultprofile.txt","w");

    if (fp)
    {
        /*
         * Assuming SetDefaultProfile needs to be called atleast once
         * before making calls to GetDefaultProfile
         */
        /*rCodeSetDP = SetDefaultProfile(0,
                                   &setPDPType,
                                   &setIPAddress,
                                   &setPrimaryDNS,
                                   &setSecondaryDNS,
                                   &setAuthentication,
                                   &setName[0],
                                   &setAPNName[0],
                                   &setUsername[0],
                                   &setPassword[0]);

        if (rCodeSetDP != eQCWWAN_ERR_NONE)
        {
            fprintf(fp,"As Profile is not set No further test cases can be run\n");
            fprintf(fp,"Hence Quiting\n");
            exit(1);
        }*/

        while(tCaseNum < MAX_GET_DEFAULT_PROFILE_TESTCASE_NUM)
        {
            profileType     = GetDefaultProfileTestCases[tCaseNum].profileType;

            fprintf(fp,"\n\nTest Case %d / %d : %s\n",
                       (tCaseNum + 1),
                       MAX_GET_DEFAULT_PROFILE_TESTCASE_NUM,
                       GetDefaultProfileTestCases[tCaseNum].desc);

            nRet = GetDefaultProfile(profileType,
                                     &PDPType,
                                     &IPAddress,
                                     &PrimaryDNS,
                                     &SecondaryDNS,
                                     &Authentication,
                                     nameSize,
                                     &Name[0],
                                     apnSize,
                                     &APNName[0],
                                     userSize,
                                     &Username[0]);

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp,"Received Profile Details are as follows:\n");
                fprintf(fp,"\nPDPType :%lx",PDPType);
                fprintf(fp,"\nIPAddress :%lx",IPAddress);
                fprintf(fp,"\nPrimaryDNS :%lx",PrimaryDNS);
                fprintf(fp,"\nSecondaryDNS :%lx",SecondaryDNS);
                fprintf(fp,"\nAuthentication :%lx",Authentication);
                fprintf(fp,"\nName : %s",Name);
                fprintf(fp,"\nAPNName :%s",APNName);
                fprintf(fp,"\nUsername :%s",Username);
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetDefaultProfileLTE
 *
 * Purpose:  Perform the tests that call the GetDefaultProfileLTE() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetDefaultProfileLTE( void )
{
    BYTE   tCaseNum = 0;
    ULONG  nRet;
    ULONG  profileType;
    ULONG  PDPType;
    ULONG  IPAddressv4;
    ULONG  PrimaryDNSv4;
    ULONG  SecondaryDNSv4;
    USHORT IPAddressv6[8] = { 0,0,0,0,0,0,0,0 };
    USHORT PrimaryDNSv6 = 0;
    USHORT SecondaryDNSv6 = 0;
    ULONG  Authentication;
    BYTE   nameSize = nMaxStrLen-1;
    CHAR   Name[nMaxStrLen];
    BYTE   apnSize = nMaxStrLen-1;
    CHAR   APNName[nMaxStrLen];
    BYTE   userSize = nMaxStrLen-1;
    CHAR   Username[nMaxStrLen];
    USHORT idx;
    FILE  *fp=NULL;

    fp = tfopen("../../wds/test/results/getdefaultprofilelte.txt","w");

    if (fp)
    {
        /*
         * Assuming SetDefaultProfile needs to be called atleast once
         * before making calls to GetDefaultProfile
         */
        /*rCodeSetDP = SetDefaultProfile(0,
                                   &setPDPType,
                                   &setIPAddress,
                                   &setPrimaryDNSv4,
                                   &setSecondaryDNSv4,
                                   &setAuthenticationv4,
                                   &setPrimaryDNSv6,
                                   &setSecondaryDNSv6,
                                   &setAuthenticationv6,
                                   &setName[0],
                                   &setAPNName[0],
                                   &setUsername[0],
                                   &setPassword[0]);

        if (rCodeSetDP != eQCWWAN_ERR_NONE)
        {
            fprintf(fp,"As Profile is not set No further test cases can be run\n");
            fprintf(fp,"Hence Quiting\n");
            exit(1);
        }*/

        while(tCaseNum < MAX_GET_DEFAULT_PROFILE_TESTCASE_NUM)
        {
            profileType     = GetDefaultProfileTestCases[tCaseNum].profileType;

            fprintf(fp,"\n\nTest Case %d / %d : %s\n",
                       (tCaseNum + 1),
                       MAX_GET_DEFAULT_PROFILE_TESTCASE_NUM,
                       GetDefaultProfileTestCases[tCaseNum].desc);

            nRet = GetDefaultProfileLTE(profileType,
                                     &PDPType,
                                     &IPAddressv4,
                                     &PrimaryDNSv4,
                                     &SecondaryDNSv4,
                                     IPAddressv6,
                                     &PrimaryDNSv6,
                                     &SecondaryDNSv6,
                                     &Authentication,
                                     nameSize,
                                     &Name[0],
                                     apnSize,
                                     &APNName[0],
                                     userSize,
                                     &Username[0]);

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp,"Received Profile Details are as follows:\n");
                fprintf(fp,"\nPDPType :%lx",PDPType);
                fprintf(fp,"\nIPAddressv4 :%lx",IPAddressv4);
                fprintf(fp,"\nPrimaryDNSv4 :%lx",PrimaryDNSv4);
                fprintf(fp,"\nSecondaryDNSv4 :%lx",SecondaryDNSv4);

                for (idx = 0; idx < 8; idx++)
                fprintf(fp, "IP V6 Address is: %hx\n", IPAddressv6[idx] );

                fprintf(fp,"\nPrimaryDNSv6 :%d",PrimaryDNSv6);
                fprintf(fp,"\nSecondaryDNSv6 :%d",SecondaryDNSv6);
                fprintf(fp,"\nAuthentication :%lx",Authentication);
                fprintf(fp,"\nName : %s",Name);
                fprintf(fp,"\nAPNName :%s",APNName);
                fprintf(fp,"\nUsername :%s",Username);
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetAutoconnect
 *
 * Purpose:  Perform the tests that call the GetAutoconnect() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetAutoconnect( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    ULONG setting;
    FILE  *fp=NULL;

    fp = tfopen("../../wds/test/results/getautoconnect.txt","w");

    if (fp)
    {
        while(tCaseNum < MAX_GET_AUTOCONNECT_TESTCASE_NUM)
        {
            fprintf(fp,"\n\nTest Case %d / %d : %s\n",
                       (tCaseNum + 1),
                       MAX_GET_AUTOCONNECT_TESTCASE_NUM,
                       GetAutoconnectTestCases[tCaseNum].desc);

            nRet = GetAutoconnect ( &setting );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp,"Autoconnect Setting is : %lu\n",setting);
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetSessionState
 *
 * Purpose:  Perform the test that calls the GetSessionState() API
 *
 * Parms:    not used - for future variable parameters ?
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetSessionState( void )
{
    BYTE   tCaseNum = 0;
    ULONG  nRet;
    FILE   *fp=NULL;
    ULONG  State = 0;

    fp = tfopen("../../wds/test/results/getsessionstate.txt","w");
    if (fp)
    {
        while(tCaseNum < MAX_GET_SESSION_STATE_TESTCASE_NUM)
        {
            fprintf(fp,"\nTest Case %d / %d : %s\n",
                       tCaseNum + 1,
                       MAX_GET_SESSION_STATE_TESTCASE_NUM,
                       GetSessionStateTestCases[tCaseNum].desc);

            /* Calling GetSessionState */
            nRet = GetSessionState( &State );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "session state: %lu\n", State );
            }

            tCaseNum++;
        }

    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetPacketStatus
 *
 * Purpose:  Perform the test that calls the GetPacketStatus() API
 *
 * Parms:    not used - for future variable parameters ?
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetPacketStatus( void )
{
    BYTE   tCaseNum = 0;
    ULONG  TXPacketSuccesses = 0;
    ULONG  RXPacketSuccesses = 0;
    ULONG  TXPacketErrors = 0;
    ULONG  RXPacketErrors = 0;
    ULONG  TXPacketOverflows = 0;
    ULONG  RXPacketOverflows = 0;
    ULONG  nRet;
    FILE   *fp=NULL;

    fp = tfopen("../../wds/test/results/getpacketstatus.txt","w");
    if (fp)
    {

        while(tCaseNum < MAX_GET_PACKET_STATUS_TESTCASE_NUM)
        {
            fprintf(fp,"Test Case %d / %d : %s\n",
                        tCaseNum + 1,
                        MAX_GET_SESSION_STATE_TESTCASE_NUM,
                        GetPacketStatusTestCases[tCaseNum].desc);

            nRet = GetPacketStatus (
                        &TXPacketSuccesses,
                        &RXPacketSuccesses,
                        &TXPacketErrors,
                        &RXPacketErrors,
                        &TXPacketOverflows,
                        &RXPacketOverflows);

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "TXPacketSuccesses: %lx\n", TXPacketSuccesses );
                fprintf(fp, "RXPacketSuccesses: %lx\n", RXPacketSuccesses );
                fprintf(fp, "TXPacketErrors: %lx\n", TXPacketErrors );
                fprintf(fp, "RXPacketErrors: %lx\n", RXPacketErrors );
                fprintf(fp, "TXPacketOverflows: %lx\n", TXPacketOverflows );
                fprintf(fp, "RXPacketOverflows: %lx\n", RXPacketOverflows );
            }

            tCaseNum++;
        }

    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetByteTotals
 *
 * Purpose:  Perform the test that calls the GetByteTotals() API
 *
 * Parms:    not used - for future variable parameters ?
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetByteTotals( void )
{
    BYTE       tCaseNum = 0;
    ULONGLONG  TXTotalBytes;
    ULONGLONG  RXTotalBytes;
    ULONG      nRet;
    FILE       *fp=NULL;

    fp = tfopen("../../wds/test/results/getbytetotals.txt","w");
    if (fp)
    {
        while(tCaseNum < MAX_GET_BYTE_TOTALS_TESTCASE_NUM)
        {
            fprintf(fp,"Test Case %d / %d : %s\n",
                        tCaseNum + 1,
                        MAX_GET_BYTE_TOTALS_TESTCASE_NUM,
                        GetByteTotalsTestCases[tCaseNum].desc);

            nRet = GetByteTotals (
                        &TXTotalBytes,
                        &RXTotalBytes);

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "TXTotalBytes: %llx\n", TXTotalBytes );
                fprintf(fp, "RXTotalBytes: %llx\n", RXTotalBytes );
            }

            tCaseNum++;
        }

    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetDormancyState
 *
 * Purpose:  Perform the test that calls the GetDormancyState() API
 *
 * Parms:    not used - for future variable parameters ?
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetDormancyState( void )
{
    BYTE   tCaseNum = 0;
    ULONG  DormancyState;
    ULONG  nRet;
    FILE   *fp=NULL;

    fp = tfopen("../../wds/test/results/getdormancystate.txt","w");
    if (fp)
    {
        while(tCaseNum < MAX_GET_DORMANCY_STATE_TESTCASE_NUM)
        {
            fprintf(fp,"Test Case %d / %d : %s\n",
                        tCaseNum + 1,
                        MAX_GET_DORMANCY_STATE_TESTCASE_NUM,
                        GetDormancyStateTestCases[tCaseNum].desc);

            nRet = GetDormancyState (
                        &DormancyState );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "Dormancy State: %lx\n", DormancyState );
            }

            tCaseNum++;
        }

    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetDataBearerTechnology
 *
 * Purpose:  Perform the test that calls the GetDataBearerTechnology() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetDataBearerTechnology( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    ULONG *pDataBearer;
    FILE   *fp=NULL;

    fp = tfopen("../../wds/test/results/getdatabearer.txt","w");
     if (fp)
    {
        while(tCaseNum < MAX_GET_DATA_BR_TECH_TESTCASE_NUM)
        {
            fprintf(fp,"Test Case %d / %d : %s\n",
                        tCaseNum + 1,
                        MAX_GET_DATA_BR_TECH_TESTCASE_NUM,
                        GetDataBrTechTestCases[tCaseNum].desc);

            pDataBearer = GetDataBrTechTestCases[tCaseNum].pDataBearer;
            nRet = GetDataBearerTechnology (pDataBearer);

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
                fprintf(fp, "Databearer %lu\n", *pDataBearer );

            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:     doSLQSGetDataBearerTechnology
 *
 * Purpose:  Perform the test that calls the SLQSGetDataBearerTechnology() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSGetDataBearerTechnology( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE   *fp=NULL;
    QmiWDSDataBearers          *pDataBearers;
    QmiWDSDataBearerTechnology *pCurDBTech;
    QmiWDSDataBearerTechnology *pLastCallDBTech;

    fp = tfopen("../../wds/test/results/getdatabearer.txt","w");
     if (fp)
    {
        while(tCaseNum < MAX_GET_CUR_DATA_BR_TECH_TESTCASE_NUM)
        {
            fprintf(fp,"Test Case %d / %d : %s\n",
                        tCaseNum + 1,
                        MAX_GET_CUR_DATA_BR_TECH_TESTCASE_NUM,
                        GetCurDataBrTechTestCases[tCaseNum].desc);

            pDataBearers    = GetCurDataBrTechTestCases[tCaseNum].pDataBearers;
            pCurDBTech      =
                GetCurDataBrTechTestCases[tCaseNum].pCurDataBearerTechnology;
            pLastCallDBTech =
              GetCurDataBrTechTestCases[tCaseNum].pLastCallDataBearerTechnology;

            if(pDataBearers != NULL)
            {
                pDataBearers->pCurDataBearerTechnology = pCurDBTech;
                pDataBearers->pLastCallDataBearerTechnology = pLastCallDBTech;
            }

            nRet = SLQSGetDataBearerTechnology (pDataBearers);

            /* Display result code and text */
            doprintreason (fp, nRet);

            /* Display the results based on the data bearer mask */
            if (!nRet)
            {
                if (pDataBearers)
                {
                    fprintf(fp, "DataBearerMask %x\n",
                                            pDataBearers->dataBearerMask );
                    if (pCurDBTech &&
                       (pDataBearers->dataBearerMask & QMI_WDS_CURRENT_CALL_DB_MASK))
                    {
                        fprintf(fp, "Current data bearer details\n" );
                        fprintf(fp, "Network  %x\n",
                                 pCurDBTech->currentNetwork );
                        fprintf(fp, "Rat Mask %lu\n", pCurDBTech->ratMask);
                        fprintf(fp, "So Mask  %lu\n", pCurDBTech->soMask );
                    }
                    if (pLastCallDBTech &&
                       (pDataBearers->dataBearerMask & QMI_WDS_LAST_CALL_DB_MASK))
                    {
                        fprintf(fp, "Last Call data bearer details\n" );
                        fprintf(fp, "Network  %x\n",
                                    pLastCallDBTech->currentNetwork );
                        fprintf(fp, "Rat Mask %lu\n", pLastCallDBTech->ratMask);
                        fprintf(fp, "So Mask  %lu\n", pLastCallDBTech->soMask );
                    }
                }
            }
            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetSessionDuration
 *
 * Purpose:  Perform the test that calls the GetSessionDuration() API
 *
 * Parms:    not used - for future variable parameters ?
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetSessionDuration( void )
{
    BYTE       tCaseNum = 0;
    ULONGLONG  Duration;
    ULONG      nRet;
    FILE       *fp=NULL;

    fp = tfopen("../../wds/test/results/getsessionduration.txt","w");
    if (fp)
    {
        while(tCaseNum < MAX_GET_SESSION_DURATION_TESTCASE_NUM)
        {
            fprintf(fp,"Test Case %d / %d : %s\n",
                        tCaseNum + 1,
                        MAX_GET_SESSION_DURATION_TESTCASE_NUM,
                        GetSessionDurationTestCases[tCaseNum].desc);

            nRet = GetSessionDuration (
                        &Duration );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "Session Duration: %llx\n", Duration );
            }

            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetIPAddress
 *
 * Purpose:  Perform the tests that call the GetIPAddress() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetIPAddress( void )
{
    BYTE  appCurTestCaseStepNum = 0;
    ULONG IPAddress;
    FILE  *fp=NULL;

    fp = tfopen("../../wds/test/results/getipaddress.txt","w");


    if (fp)
    {
        while (appCurTestCaseStepNum < MAX_GET_IP_ADDRESS_TESTCASE_NUM )
        {
            fprintf(fp,"\n\nTest Case %d / %d : %s\n",
                       (appCurTestCaseStepNum + 1),
                       MAX_GET_IP_ADDRESS_TESTCASE_NUM,
                       GetIPAddressTestCases[appCurTestCaseStepNum].desc);

            ULONG nRet = GetIPAddress ( &IPAddress );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "IP Address is: %lx\n",IPAddress);
            }

            appCurTestCaseStepNum++;
        }

    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetIPAdressLTE
 *
 * Purpose:  Perform the tests that call the doGetIPAddressLTE() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetIPAddressLTE( void )
{
    BYTE   appCurTestCaseStepNum = 0;
    ULONG  IPAddressV4;
    USHORT IPAddressV6[8] = { 0,0,0,0,0,0,0,0 };
    BYTE   IPv6prefixlen = 0;
    FILE   *fp=NULL;

    fp = tfopen("../../wds/test/results/getipaddresslte.txt","w");

    if (fp)
    {
        while (appCurTestCaseStepNum < MAX_GET_IP_ADDRESSLTE_TESTCASE_NUM )
        {
            fprintf(fp,"\n\nTest Case %d / %d : %s\n",
                       (appCurTestCaseStepNum + 1),
                       MAX_GET_IP_ADDRESSLTE_TESTCASE_NUM,
                       GetIPAddressLTETestCases[appCurTestCaseStepNum].desc);

            ULONG nRet = GetIPAddressLTE ( &IPAddressV4,
                                           IPAddressV6,
                                           &IPv6prefixlen );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet ||
                 eQCWWAN_ERR_SWICM_V4DWN_V6UP == nRet ||
                 eQCWWAN_ERR_SWICM_V4UP_V6DWN == nRet )
            {
                int i = 0;
                fprintf(fp, "IP V4 Address is: %lx\n",IPAddressV4);
                for (i = 0; i < 8; i++)
                    fprintf(fp, "IP V6 Address is: %hx\n", IPAddressV6[i] );
                fprintf(fp, "IP Address Length is: %d\n",IPv6prefixlen);
            }

            appCurTestCaseStepNum++;
        }

    }

    if (fp)
        tfclose(fp);

}


/*
 * Name:     doGetConnectionRate
 *
 * Purpose:  Perform the tests that call the doGetConnectionRate() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetConnectionRate( void )
{
    BYTE   appCurTestCaseStepNum = 0;
    ULONG  CurrentChannelTXRate=0;
    ULONG  CurrentChannelRXRate=0;
    ULONG  MaxChannelTXRate=0;
    ULONG  MaxChannelRXRate=0;
    FILE   *fp=NULL;

    fp = tfopen("../../wds/test/results/getconnectionrate.txt","w");

    if (fp)
    {
        while (appCurTestCaseStepNum < MAX_GET_CONNECTION_RATE_TESTCASE_NUM )
        {
            fprintf(fp,"\n\nTest Case %d / %d : %s\n",
                       (appCurTestCaseStepNum + 1),
                       MAX_GET_CONNECTION_RATE_TESTCASE_NUM ,
                       GetConnectionRateTestCases[appCurTestCaseStepNum].desc);

            ULONG nRet = GetConnectionRate( &CurrentChannelTXRate,
                                            &CurrentChannelRXRate,
                                            &MaxChannelTXRate,
                                            &MaxChannelRXRate );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "Current channel Tx rate :%lx\n",CurrentChannelTXRate);
                fprintf(fp, "Current channel Rx rate :%lx\n",CurrentChannelRXRate);
                fprintf(fp, "Maximum Tx rate :%lx\n",MaxChannelTXRate);
                fprintf(fp, "Maximum Rx rate : %lx\n",MaxChannelRXRate);
            }

            appCurTestCaseStepNum++;
        }
   }

   if (fp)
       tfclose(fp);
}

/*
 * Name:   doSetAutoconnect
 *
 * Purpose:  Perform the tests that call the SetAutoconnect() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSetAutoconnect( void )
{
    BYTE  appCurTestCaseStepNum = 0;
    ULONG setting=0;
    FILE  *fp=NULL;
    ULONG nRet;

    fp = tfopen("../../wds/test/results/setautoconnect.txt","w");

    if (fp)
    {
        while (appCurTestCaseStepNum < MAX_SET_AUTOCONNECT_TESTCASE_NUM  )
        {
            fprintf(fp,"\n\nTest Case %d / %d : %s\n",
                       (appCurTestCaseStepNum + 1),
                       MAX_SET_AUTOCONNECT_TESTCASE_NUM ,
                       SetAutoconnectTestCases[appCurTestCaseStepNum].desc);

            setting = SetAutoconnectTestCases[appCurTestCaseStepNum].Setting;
            setting = SetAutoconnectTestCases[appCurTestCaseStepNum].Setting;

            fprintf(fp, " Parameters Set\n");
            fprintf(fp, "Auto Connect Setting: %lu\n",setting);

            nRet = SetAutoconnect(setting);

            /* Display result code and text */
            doprintreason (fp, nRet);
            appCurTestCaseStepNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:  doSetDefaultProfile
 *
 * Purpose:  Perform the tests that call theSetDefaultProfile() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSetDefaultProfile( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    ULONG profileType;
    ULONG PDPType;
    ULONG IPAddress;
    ULONG PrimaryDNS;
    ULONG SecondaryDNS;
    ULONG Authentication;
    CHAR  Name[nMaxStrLen];
    CHAR  APNName[nMaxStrLen];
    CHAR  Username[nMaxStrLen];
    CHAR  Password[nMaxStrLen];
    FILE  *fp = NULL;

    fp = tfopen("../../wds/test/results/setdefaultprofile.txt", "w");

    if(fp)
    {
         while (tCaseNum < MAX_SET_DEFAULT_PROFILE_TESTCASE_NUM)
         {
             profileType    = SetDefaultProfileTestCases[tCaseNum].profileType;
             PDPType        = SetDefaultProfileTestCases[tCaseNum].PDPType;
             IPAddress      = SetDefaultProfileTestCases[tCaseNum].IPAddress;
             PrimaryDNS     = SetDefaultProfileTestCases[tCaseNum].\
                              PrimaryDNS;
             SecondaryDNS   = SetDefaultProfileTestCases[tCaseNum].\
                              SecondaryDNS;
             Authentication = SetDefaultProfileTestCases[tCaseNum].\
                              Authentication;
             strcpy(Name,     SetDefaultProfileTestCases[tCaseNum].Name);
             strcpy(APNName,  SetDefaultProfileTestCases[tCaseNum].APNName);
             strcpy(Username, SetDefaultProfileTestCases[tCaseNum].Username);
             strcpy(Password, SetDefaultProfileTestCases[tCaseNum].Password);

             fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_SET_DEFAULT_PROFILE_TESTCASE_NUM,
                        SetDefaultProfileTestCases[tCaseNum].desc);
             fprintf(fp, "Parameter Set:\n");
             fprintf(fp, "profileType :%ld\n", profileType);
             fprintf(fp, "PDPType :%ld\n", PDPType);
             fprintf(fp, "IPAddress :%ld\n", IPAddress);
             fprintf(fp, "PrimaryDNS :%ld\n", PrimaryDNS);
             fprintf(fp, "SecondaryDNS :%ld\n", SecondaryDNS);
             fprintf(fp, "Authentication :%ld\n", Authentication);
             fprintf(fp, "Name :%s\n", Name);
             fprintf(fp, "APNName :%s\n", APNName);
             fprintf(fp, "Username :%s\n", Username);
             fprintf(fp, "Password :%s\n", Password);

             nRet = SetDefaultProfile(profileType,
                                      &PDPType,
                                      &IPAddress,
                                      &PrimaryDNS,
                                      &SecondaryDNS,
                                      &Authentication,
                                      &Name[0],
                                      &APNName[0],
                                      &Username[0],
                                      &Password[0]);

             /* Display result code and text */
             doprintreason (fp, nRet);

             tCaseNum++;
         }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name: doSetDefaultProfileLTE
 *
 * Purpose:  Perform the tests that call the GetAutoconnect() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSetDefaultProfileLTE( void )
{
    BYTE   tCaseNum = 0;
    ULONG  nRet;
    ULONG  profileType;
    ULONG  PDPType;
    ULONG  IPAddressv4;
    ULONG  PrimaryDNSv4;
    ULONG  SecondaryDNSv4;
    USHORT IPAddressv6[8];
    USHORT PrimaryDNSv6;
    USHORT SecondaryDNSv6;
    ULONG  Authentication;
    CHAR   Name[nMaxStrLen];
    CHAR   APNName[nMaxStrLen];
    CHAR   Username[nMaxStrLen];
    CHAR   Password[nMaxStrLen];
    USHORT idx;
    FILE   *fp = NULL;

    fp = tfopen("../../wds/test/results/setdefaultprofilelte.txt", "w");

    if(fp)
    {
         while (tCaseNum < MAX_SET_DEFAULT_PROFILELTE_TESTCASE_NUM)
         {
             profileType    = SetDefaultProfileLTETestCases[tCaseNum].\
                              profileType;
             PDPType        = SetDefaultProfileLTETestCases[tCaseNum].\
                              PDPType;
             IPAddressv4    = SetDefaultProfileLTETestCases[tCaseNum].\
                              IPAddressv4;
             PrimaryDNSv4   = SetDefaultProfileLTETestCases[tCaseNum].\
                              PrimaryDNSv4;
             SecondaryDNSv4 = SetDefaultProfileLTETestCases[tCaseNum].\
                              SecondaryDNSv4;

             memcpy(IPAddressv6,SetDefaultProfileLTETestCases[tCaseNum].\
                 IPAddressv6,sizeof(SetDefaultProfileLTETestCases[tCaseNum].\
                 IPAddressv6) );

             PrimaryDNSv6   = SetDefaultProfileLTETestCases[tCaseNum].\
                              PrimaryDNSv6;
             SecondaryDNSv6 = SetDefaultProfileLTETestCases[tCaseNum].\
                              SecondaryDNSv6;
             Authentication = SetDefaultProfileLTETestCases[tCaseNum].\
                              Authentication;
             strcpy(Name,     SetDefaultProfileLTETestCases[tCaseNum].Name);
             strcpy(APNName,  SetDefaultProfileLTETestCases[tCaseNum].APNName);
             strcpy(Username, SetDefaultProfileLTETestCases[tCaseNum].Username);
             strcpy(Password, SetDefaultProfileLTETestCases[tCaseNum].Password);

             fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_SET_DEFAULT_PROFILELTE_TESTCASE_NUM,
                        SetDefaultProfileLTETestCases[tCaseNum].desc);

             fprintf(fp, "Parameter Set:\n");
             fprintf(fp, "profileType :%ld\n", profileType);
             fprintf(fp, "PDPType :%ld\n", PDPType);
             fprintf(fp, "IPAddressv4 :%ld\n", IPAddressv4);
             fprintf(fp, "PrimaryDNSv4 :%ld\n", PrimaryDNSv4);
             fprintf(fp, "SecondaryDNSv4 :%ld\n", SecondaryDNSv4);

             for (idx = 0; idx < 8; idx++)
                fprintf(fp, "IP V6 Address is: %hx\n", IPAddressv6[idx] );

             fprintf(fp, "PrimaryDNSv6 :%d\n", PrimaryDNSv6);
             fprintf(fp, "SecondaryDNSv6 :%d\n", SecondaryDNSv6);
             fprintf(fp, "Authentication :%ld\n", Authentication);
             fprintf(fp, "Name :%s\n", Name);
             fprintf(fp, "APNName :%s\n", APNName);
             fprintf(fp, "Username :%s\n", Username);
             fprintf(fp, "Password :%s\n", Password);

             nRet = SetDefaultProfileLTE(profileType,
                                         &PDPType,
                                         &IPAddressv4,
                                         &PrimaryDNSv4,
                                         &SecondaryDNSv4,
                                         IPAddressv6,
                                         &PrimaryDNSv6,
                                         &SecondaryDNSv6,
                                         &Authentication,
                                         &Name[0],
                                         &APNName[0],
                                         &Username[0],
                                         &Password[0]);

             /* Display result code and text */
             doprintreason (fp, nRet);

             tCaseNum++;
         }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSetMobileIP
 *
 * Purpose:  Perform the tests that call the SetMobileIP() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSetMobileIP( void )
{
    BYTE   appCurTestCaseStepNum = 0;
    ULONG  mode;
    FILE   *fp=NULL;

    fp = tfopen("../../wds/test/results/setmobileip.txt","w");

    if (fp)
    {
        while (appCurTestCaseStepNum < MAX_SET_MOBILE_IP_TESTCASE_NUM)
        {
            fprintf(fp,"\n\nTest Case %d / %d : %s\n",
                       (appCurTestCaseStepNum + 1),
                       MAX_SET_MOBILE_IP_TESTCASE_NUM,
                       SetMobileIPTestCases[appCurTestCaseStepNum].desc);

            mode = SetMobileIPTestCases[appCurTestCaseStepNum].mode;
            ULONG nRet = SetMobileIP( mode );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "Set Mobile IP is %lx\n",mode);
            }

            appCurTestCaseStepNum++;
        }
   }

   if (fp)
       tfclose(fp);
}
/*
 * Name:     doGetMobileIP
 *
 * Purpose:  Perform the tests that call the GetMobileIP() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetMobileIP( void )
{
    BYTE   appCurTestCaseStepNum = 0;
    ULONG  mode=0;
    FILE   *fp=NULL;

    fp = tfopen("../../wds/test/results/Getmobileip.txt","w");

    if (fp)
    {
        while (appCurTestCaseStepNum < MAX_GET_MOBILE_IP_TESTCASE_NUM)
        {
            fprintf(fp,"\n\nTest Case %d / %d : %s\n",
                   (appCurTestCaseStepNum + 1),
                   MAX_GET_MOBILE_IP_TESTCASE_NUM,
                   GetMobileIPTestCases[appCurTestCaseStepNum].desc);

            ULONG nRet = GetMobileIP( &mode );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "Get Mobile IP is %lx\n",mode);
            }

            appCurTestCaseStepNum++;
        }
   }

   if (fp)
       tfclose(fp);
}

/*
 * Name:     GetMobileIPProfile
 *
 * Purpose:  Perform the tests that call the doGetMobileIPProfile() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetMobileIPProfile( void )
{
    BYTE appCurTestCaseStepNum = 0;
    BYTE  index;
    BYTE  enabled = 0;
    ULONG address = 0;
    ULONG primaryHA = 0;
    ULONG secondaryHA = 0;
    BYTE  revTunneling = 0;
    BYTE  naiSize;
    CHAR  nAI[nMaxStrLen]="0";
    ULONG hASPI = 0;
    ULONG aAASPI = 0;
    ULONG hAState = 0;
    ULONG aAAState = 0;
    FILE  *fp=NULL;

    fp = tfopen("../../wds/test/results/getmobileipprofile.txt","w");

    if (fp)
    {
        while (appCurTestCaseStepNum < MAX_GET_MOBILE_IP_PROFILE_TESTCASE_NUM )
        {
            index = GetMobileIPProfileTestCases
                                      [appCurTestCaseStepNum].index;
            naiSize = GetMobileIPProfileTestCases
                                      [appCurTestCaseStepNum].naiSize;
            fprintf(fp,"\n\nTest Case %d / %d : %s\n",
                      (appCurTestCaseStepNum + 1),
                      MAX_GET_MOBILE_IP_PROFILE_TESTCASE_NUM,
                      GetMobileIPProfileTestCases[appCurTestCaseStepNum].desc);

            fprintf(fp,"Parameter Set:\n");
            fprintf(fp,"index :%d\n",index);
            fprintf(fp,"enabled :%d\n",enabled);

            ULONG nRet = GetMobileIPProfile( index,
                                             &enabled,
                                             &address,
                                             &primaryHA,
                                             &secondaryHA,
                                             &revTunneling,
                                             naiSize,
                                             nAI,
                                             &hASPI,
                                             &aAASPI,
                                             &hAState,
                                             &aAAState );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "Enabled %d\n",  enabled);
                fprintf(fp, "Ip Address: %lx\n",address);
                fprintf(fp, "Primary Home address: %lx\n", primaryHA);
                fprintf(fp, "Seconday Home Address:%lx\n", secondaryHA);
                fprintf(fp, "Rev Tunneling: %d\n", revTunneling);
                fprintf(fp, "HASPI: %lx\n", hASPI);
                fprintf(fp, "NAI: %s\n", nAI);
                fprintf(fp, "AASPI: %lx\n", aAASPI);
                fprintf(fp, "HA State: %lx\n", hAState);
                fprintf(fp, "AAA State: %lx\n", aAAState );

            }

            appCurTestCaseStepNum++;
        }
   }

   if (fp)
       tfclose(fp);
}

/*
 * Name:     doGetLastMobileIPError
 *
 * Purpose:  Perform the tests that call the GetLastMobileIPError() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */

void doGetLastMobileIPError( void )
{
    BYTE   appCurTestCaseStepNum = 0;
    ULONG  Error=0;
    FILE   *fp=NULL;

    fp = tfopen("../../wds/test/results/Getlastmobileiperror.txt","w");

    if (fp)
    {
        while (appCurTestCaseStepNum < MAX_GET_LAST_MOBILE_IP_ERROR_TESTCASE_NUM)
        {
            fprintf(fp,"\n\nTest Case %d / %d : %s\n",
                 (appCurTestCaseStepNum + 1),
                  MAX_GET_LAST_MOBILE_IP_ERROR_TESTCASE_NUM,
                  GetLastMobileIPErrorTestCases[appCurTestCaseStepNum].desc);

            ULONG nRet = GetLastMobileIPError( &Error );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet )
            {
                fprintf(fp, "Last Mobile IP Error is %lx\n",Error);
            }

            appCurTestCaseStepNum++;
        }
   }

   if (fp)
       tfclose(fp);
}

/*
 * Name:     doSLQSGetRuntimeSettings
 *
 * Purpose:  Perform the tests that call the SLQSGetRuntimeSettings() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSGetRuntimeSettings( void )
{
    BYTE                        tCaseNum = 0, idx = 0;
    ULONG                       nRet;
    FILE                        *fp = NULL;
    struct WdsRunTimeSettings   lRunTimeSettings;

    fp = tfopen("../../wds/test/results/slqsgetruntimesettings.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_SLQS_GET_RUNTIME_SETTINGS_TESTCASE_NUM)
        {
            fprintf(fp,
                    "\nTest Case %d / %d : %s\n",
                    (tCaseNum + 1),
                    MAX_SLQS_GET_RUNTIME_SETTINGS_TESTCASE_NUM,
                    SLQSGetRuntimeSettingsTestCases[tCaseNum].desc);
            /* Intialize the structure to be sent to API */
            /* Intialize Profile Name */
            lRunTimeSettings.pProfileName   =
                  SLQSGetRuntimeSettingsTestCases[tCaseNum].pProfileName;

            /* Intialize PDP Type */
            lRunTimeSettings.pPDPType   =
                  SLQSGetRuntimeSettingsTestCases[tCaseNum].pPDPType;

            /* Intialize APN Name */
            lRunTimeSettings.pAPNName   =
                  SLQSGetRuntimeSettingsTestCases[tCaseNum].pAPNName;

            /* Intialize IPV4 Primary DNS */
            lRunTimeSettings.pPrimaryDNSV4   =
                  SLQSGetRuntimeSettingsTestCases[tCaseNum].pPrimaryDNSV4;

            /* Intialize IPV4 Secondary DNS */
            lRunTimeSettings.pSecondaryDNSV4   =
                  SLQSGetRuntimeSettingsTestCases[tCaseNum].pSecondaryDNSV4;

            /* Intialize UMTS Granted QOS */
            lRunTimeSettings.pUMTSGrantedQoS   =
                  SLQSGetRuntimeSettingsTestCases[tCaseNum].pUMTSGrantedQos;

            /* Intialize GPRS Granted QOS */
            lRunTimeSettings.pGPRSGrantedQoS   =
                  SLQSGetRuntimeSettingsTestCases[tCaseNum].pGPRSGrantedQos;

            /* Intialize User name */
            lRunTimeSettings.pUsername   =
                  SLQSGetRuntimeSettingsTestCases[tCaseNum].pUsername;

            /* Intialize Authentication */
            lRunTimeSettings.pAuthentication   =
                  SLQSGetRuntimeSettingsTestCases[tCaseNum].pAuthentication;

            /* Intialize IPV4 address */
            lRunTimeSettings.pIPAddressV4   =
                  SLQSGetRuntimeSettingsTestCases[tCaseNum].pIPAddressV4;

            /* Intialize Profile Id */
            lRunTimeSettings.pProfileID   =
                  SLQSGetRuntimeSettingsTestCases[tCaseNum].pProfileID;

            /* Intialize IPV4 Gateway address */
            lRunTimeSettings.pGWAddressV4   =
                  SLQSGetRuntimeSettingsTestCases[tCaseNum].pGWAddressV4;

            /* Intialize IPV4 Subnet Mask */
            lRunTimeSettings.pSubnetMaskV4   =
                  SLQSGetRuntimeSettingsTestCases[tCaseNum].pSubnetMaskV4;

            /* Intialize PCSF address */
            lRunTimeSettings.pPCSCFAddrPCO   =
                  SLQSGetRuntimeSettingsTestCases[tCaseNum].pPCSCFAddrPCO;

            /* Intialize IPV4 Server address list */
            lRunTimeSettings.pServerAddrList =
                  SLQSGetRuntimeSettingsTestCases[tCaseNum].pServerAddrList;

            /* Intialize FQDN address structure */
            lRunTimeSettings.pPCSCFFQDNAddrList =
                 SLQSGetRuntimeSettingsTestCases[tCaseNum].pPCSCFFQDNAddrList;

            /* Intialize IPV6 Primary DNS */
            lRunTimeSettings.pPrimaryDNSV6   =
                 SLQSGetRuntimeSettingsTestCases[tCaseNum].pPrimDNSV6;

            /* Intialize IPV6 Secondary DNS */
            lRunTimeSettings.pSecondaryDNSV6 =
                 SLQSGetRuntimeSettingsTestCases[tCaseNum].pSecondDNSV6;

            /* Intialize Mtu */
            lRunTimeSettings.pMtu =
                 SLQSGetRuntimeSettingsTestCases[tCaseNum].pMtu;

            /* Intialize Domain list */
            lRunTimeSettings.pDomainList =
                SLQSGetRuntimeSettingsTestCases[tCaseNum].pDomainList;

            /* Intialize IPFamily Prefrence */
            lRunTimeSettings.pIPFamilyPreference   =
                  SLQSGetRuntimeSettingsTestCases[tCaseNum].pIPFamilyPreference;

            /* Intialize IMCN Flag */
            lRunTimeSettings.pIMCNflag   =
                  SLQSGetRuntimeSettingsTestCases[tCaseNum].pIMCNflag;

            /* Intialize Technology */
            lRunTimeSettings.pTechnology =
                  SLQSGetRuntimeSettingsTestCases[tCaseNum].pTechnology;

            /* Intialize IPV6 address */
            lRunTimeSettings.pIPV6AddrInfo =
                  SLQSGetRuntimeSettingsTestCases[tCaseNum].pIPV6AddrInfo;

            /* Intialize the IPV6 Gateway address */
            lRunTimeSettings.pIPV6GWAddrInfo =
                  SLQSGetRuntimeSettingsTestCases[tCaseNum].pIPV6GWAddrInfo;

            /* call the API, get the values */
            nRet = SLQSGetRuntimeSettings( &lRunTimeSettings );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet ||
                 eQCWWAN_ERR_SWICM_V4DWN_V6UP == nRet ||
                 eQCWWAN_ERR_SWICM_V4UP_V6DWN == nRet )
            {
                fprintf(fp, "SLQSGetRuntimeSettings Successful\n");

                fprintf(fp, "ProfileName    : %s\n",
                             lRunTimeSettings.pProfileName);

                fprintf(fp, "PDPType        : %lx\n",
                             *(lRunTimeSettings.pPDPType) );

                fprintf(fp, "APNName        : %s\n",
                             lRunTimeSettings.pAPNName);

                fprintf(fp, "PrimaryDNSV4   : %lx\n",
                             *(lRunTimeSettings.pPrimaryDNSV4) );

                fprintf(fp, "SecondaryDNSV4 : %lx\n",
                             *(lRunTimeSettings.pSecondaryDNSV4) );

                fprintf(fp, "UMTS Granted QoS Parameters\n");
                fprintf(fp, "\ttrafficClass       : %d\n",
                            lRunTimeSettings.pUMTSGrantedQoS->trafficClass);
                fprintf(fp, "\tmaxUplinkBitrate   : %lx\n",
                            lRunTimeSettings.pUMTSGrantedQoS->
                                             maxUplinkBitrate);
                fprintf(fp, "\tmaxDownlinkBitrate : %lx\n",
                            lRunTimeSettings.pUMTSGrantedQoS->
                                             maxDownlinkBitrate);
                fprintf(fp, "\tgrntUplinkBitrate  : %lx\n",
                            lRunTimeSettings.pUMTSGrantedQoS->
                                             grntUplinkBitrate);
                fprintf(fp, "\tgrntDownlinkBitrate: %lx\n",
                            lRunTimeSettings.pUMTSGrantedQoS->
                                             grntDownlinkBitrate);
                fprintf(fp, "\tqosDeliveryOrder   : %d\n",
                            lRunTimeSettings.pUMTSGrantedQoS->qosDeliveryOrder);
                fprintf(fp, "\tmaxSDUSize         : %lx\n",
                            lRunTimeSettings.pUMTSGrantedQoS->maxSDUSize);
                fprintf(fp, "\tsduErrorRatio      : %d\n",
                            lRunTimeSettings.pUMTSGrantedQoS->sduErrorRatio);
                fprintf(fp, "\tresBerRatio        : %d\n",
                            lRunTimeSettings.pUMTSGrantedQoS->resBerRatio);
                fprintf(fp, "\tdeliveryErrSDU     : %d\n",
                            lRunTimeSettings.pUMTSGrantedQoS->deliveryErrSDU);
                fprintf(fp, "\ttransferDelay      : %lx\n",
                            lRunTimeSettings.pUMTSGrantedQoS->transferDelay);
                fprintf(fp, "\ttrafficPriority    : %lx\n",
                            lRunTimeSettings.pUMTSGrantedQoS->trafficPriority);

                fprintf(fp, "GPRS Granted QoS Parameters\n");
                fprintf(fp, "\tprecedenceClass     : %lx\n",
                            lRunTimeSettings.pGPRSGrantedQoS->precedenceClass);
                fprintf(fp, "\tdelayClass          : %lx\n",
                            lRunTimeSettings.pGPRSGrantedQoS->delayClass);
                fprintf(fp, "\treliabilityClass    : %lx\n",
                            lRunTimeSettings.pGPRSGrantedQoS->reliabilityClass);
                fprintf(fp, "\tpeakThroughputClass : %lx\n",
                            lRunTimeSettings.pGPRSGrantedQoS->
                                             peakThroughputClass);
                fprintf(fp, "\tmeanThroughputClass : %lx\n",
                            lRunTimeSettings.pGPRSGrantedQoS->
                                             meanThroughputClass);

                fprintf(fp, "Username       : %s\n",
                            lRunTimeSettings.pUsername);
                fprintf(fp, "Authentication : %lx\n",
                            *(lRunTimeSettings.pAuthentication) );
                fprintf(fp, "IPAddressV4    : %lx\n",
                            *(lRunTimeSettings.pIPAddressV4) );

                fprintf(fp, "Profile Identifier Parameters \n");
                fprintf(fp, "\tProfile Type : %d\n",
                            lRunTimeSettings.pProfileID->profileType);
                fprintf(fp, "\tProfile ID   : %d\n",
                            lRunTimeSettings.pProfileID->profileIndex);

                fprintf(fp, "GWAddressV4    : %lx\n",
                            *(lRunTimeSettings.pGWAddressV4) );
                fprintf(fp, "SubnetMaskV4   : %lx\n",
                            *(lRunTimeSettings.pSubnetMaskV4) );
                fprintf(fp, "PCSCFAddrPCO   : %d\n",
                            *(lRunTimeSettings.pPCSCFAddrPCO) );

                fprintf(fp, "IPV4 Server AddressList \n");
                fprintf(fp, "\tPCSCF Server AddressList Count %d\n",
                            lRunTimeSettings.pServerAddrList->numInstances);
                for ( idx = 0; idx < ServerAddrList.numInstances; idx++ )
                    fprintf(fp, "\tPCSCF Server Address[%d] %lx\n",
                                idx,
                                lRunTimeSettings.pServerAddrList->
                                pscsfIPv4Addr[idx] );

                fprintf(fp, "PCSCFFQDNAddressList \n");
                fprintf(fp, "\tPCSCFFQDNAddressList Count %d\n",
                            lRunTimeSettings.pPCSCFFQDNAddrList->numInstances);
                for ( idx = 0;
                      idx < lRunTimeSettings.pPCSCFFQDNAddrList->numInstances ;
                      idx++ )
                {
                    fprintf(fp, "\tPCSCFFQDNAddressLength[%d]  %d\n",
                                idx,
                                lRunTimeSettings.pPCSCFFQDNAddrList->
                                pcsfFQDNAddress[idx].fqdnLen );
                    fprintf(fp, "\tPCSCFFQDNAddress[%d]        %s\n",
                                idx,
                                lRunTimeSettings.pPCSCFFQDNAddrList->
                                pcsfFQDNAddress[idx].fqdnAddr );
                }

                fprintf(fp, "Primary DNS V6   : ");
                for (idx = 0; idx < 8; idx++)
                    fprintf(fp, "%hx ", lRunTimeSettings.pPrimaryDNSV6[idx] );
                    fprintf(fp, "\n");

                fprintf(fp, "Secondary DNS V6 : ");
                for (idx = 0; idx < 8; idx++)
                    fprintf(fp, "%hx ", lRunTimeSettings.pSecondaryDNSV6[idx] );
                    fprintf(fp, "\n");

                fprintf(fp, "Mtu            : %lx\n",
                            *(lRunTimeSettings.pMtu) );

                fprintf(fp, "DomainNameList \n");
                fprintf(fp, "\tDomainNameList Count %d\n",
                            lRunTimeSettings.pDomainList->numInstances);

                for ( idx = 0; idx < DomainList.numInstances; idx++ )
                {
                    fprintf(fp, "\tDomainLen[%d]   %d\n",
                                idx,
                                lRunTimeSettings.pDomainList->
                                domain[idx].domainLen);
                    fprintf(fp, "\tDomainName[%d]  %s\n",
                                idx,
                                lRunTimeSettings.pDomainList->
                                domain[idx].domainName);
                }

                fprintf(fp, "IPFamilyPref   : %d\n",
                            *(lRunTimeSettings.pIPFamilyPreference) );

                fprintf(fp, "IMCNflag       : %d\n",
                            *(lRunTimeSettings.pIMCNflag) );

                fprintf(fp, "Technology     : %d\n",
                            *(lRunTimeSettings.pTechnology) );

                fprintf(fp, "IPAddressV6 Information\n");
                fprintf(fp, "\tIPAddressV6 Address   :");
                for (idx = 0; idx < 8; idx++)
                    fprintf(fp, "%hx ",
                                 lRunTimeSettings.pIPV6AddrInfo->
                                 IPAddressV6[idx] );
                fprintf(fp, "\n");
                fprintf(fp, "\tIPAddressV6 Length    : %d\n",
                            lRunTimeSettings.pIPV6AddrInfo->IPV6PrefixLen );

                fprintf(fp, "IPV6 Gateway Address Information\n");
                fprintf(fp, "\tIPV6 Gateway Address   :");
                for (idx = 0; idx < 8; idx++)
                    fprintf(fp, "%hx ",
                                lRunTimeSettings.pIPV6GWAddrInfo->
                                gwAddressV6[idx] );
                fprintf(fp, "\n");
                fprintf(fp, "\tIPV6 Gateway Address Length    : %d\n",
                            lRunTimeSettings.pIPV6GWAddrInfo->gwV6PrefixLen );
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doSLQSSetProfile
 *
 * Purpose:  Perform the tests that call the SLQSSetProfile() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSSetProfile( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    ULONG profileType;
    BYTE  profileId;
    ULONG PDPType;
    ULONG IPAddress;
    ULONG PrimaryDNS;
    ULONG SecondaryDNS;
    ULONG Authentication;
    CHAR  Name[nMaxStrLen];
    CHAR  APNName[nMaxStrLen];
    CHAR  Username[nMaxStrLen];
    CHAR  Password[nMaxStrLen];
    FILE  *fp = NULL;

    fp = tfopen("TestReport/slqssetprofile.txt", "w");

    if(fp)
    {
         while (tCaseNum < MAX_SLQS_SET_PROFILE_TESTCASE_NUM)
         {
             profileType    = SLQSSetProfileTestCases[tCaseNum].profileType;
             profileId      = SLQSSetProfileTestCases[tCaseNum].profileId;
             PDPType        = SLQSSetProfileTestCases[tCaseNum].PDPType;
             IPAddress      = SLQSSetProfileTestCases[tCaseNum].IPAddress;
             PrimaryDNS     = SLQSSetProfileTestCases[tCaseNum].\
                              PrimaryDNS;
             SecondaryDNS   = SLQSSetProfileTestCases[tCaseNum].\
                              SecondaryDNS;
             Authentication = SLQSSetProfileTestCases[tCaseNum].\
                              Authentication;
             strcpy(Name,     SLQSSetProfileTestCases[tCaseNum].Name);
             strcpy(APNName,  SLQSSetProfileTestCases[tCaseNum].APNName);
             strcpy(Username, SLQSSetProfileTestCases[tCaseNum].Username);
             strcpy(Password, SLQSSetProfileTestCases[tCaseNum].Password);

             fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_SLQS_SET_PROFILE_TESTCASE_NUM,
                        SLQSSetProfileTestCases[tCaseNum].desc);
             fprintf(fp, "Parameter Set:\n");
             fprintf(fp, "profileType :%ld\n", profileType);
             fprintf(fp, "profileId :%d\n", profileId);
             fprintf(fp, "PDPType :%ld\n", PDPType);
             fprintf(fp, "IPAddress :%ld\n", IPAddress);
             fprintf(fp, "PrimaryDNS :%ld\n", PrimaryDNS);
             fprintf(fp, "SecondaryDNS :%ld\n", SecondaryDNS);
             fprintf(fp, "Authentication :%ld\n", Authentication);
             fprintf(fp, "Name :%s\n", Name);
             fprintf(fp, "APNName :%s\n", APNName);
             fprintf(fp, "Username :%s\n", Username);
             fprintf(fp, "Password :%s\n", Password);

             nRet = SLQSSetProfile(profileType,
                                   profileId,
                                   &PDPType,
                                   &IPAddress,
                                   &PrimaryDNS,
                                   &SecondaryDNS,
                                   &Authentication,
                                   &Name[0],
                                   &APNName[0],
                                   &Username[0],
                                   &Password[0]);

             /* Display result code and text */
             doprintreason (fp, nRet);

             tCaseNum++;
         }
    }

    if (fp)
        tfclose(fp);
}

/*

 * Name:     doSLQSGetProfile
 *
 * Purpose:  Perform the tests that call the SLQSGetProfile() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSGetProfile( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    ULONG profileType;
    BYTE  profileId;
    ULONG PDPType;
    ULONG IPAddress;
    ULONG PrimaryDNS = 0;
    ULONG SecondaryDNS = 0;
    ULONG Authentication;
    BYTE  nameSize = nMaxStrLen-1;
    CHAR  Name[nMaxStrLen];
    BYTE  apnSize = nMaxStrLen-1;
    CHAR  APNName[nMaxStrLen];
    BYTE  userSize = nMaxStrLen-1;
    CHAR  Username[nMaxStrLen];
    WORD  extendedErrorCode = 0;

    FILE  *fp=NULL;

    fp = tfopen("TestReport/slqsgetprofile.txt","w");

    if (fp)
    {
        while(tCaseNum < MAX_SLQS_GET_PROFILE_TESTCASE_NUM)
        {
            profileType = SlqsGetProfileSettingsTestCases[tCaseNum].profileType;
            profileId   = SlqsGetProfileSettingsTestCases[tCaseNum].profileId;

            fprintf(fp,"\n\nTest Case %d / %d : %s\n",
                       (tCaseNum + 1),
                       MAX_SLQS_GET_PROFILE_TESTCASE_NUM,
                       SlqsGetProfileSettingsTestCases[tCaseNum].desc);

            nRet = SLQSGetProfile(profileType,
                                  profileId,
                                  &PDPType,
                                  &IPAddress,
                                  &PrimaryDNS,
                                  &SecondaryDNS,
                                  &Authentication,
                                  nameSize,
                                  &Name[0],
                                  apnSize,
                                  &APNName[0],
                                  userSize,
                                  &Username[0],
                                  &extendedErrorCode );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp,"Details for Profile Id : %d",profileId);
                fprintf(fp,"\nPDPType :%lx",PDPType);
                fprintf(fp,"\nIPAddress :%lx",IPAddress);
                fprintf(fp,"\nPrimaryDNS :%lx",PrimaryDNS);
                fprintf(fp,"\nSecondaryDNS :%lx",SecondaryDNS);
                fprintf(fp,"\nAuthentication :%lx",Authentication);
                fprintf(fp,"\nName : %s",Name);
                fprintf(fp,"\nAPNName :%s",APNName);
                fprintf(fp,"\nUsername :%s",Username);
            }
            else
            {
                fprintf( fp,"Extended Error Code :%d",extendedErrorCode );
                extendedErrorCode = 0;
            }
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doSLQSStartStopDataSession_stop
 *
 * Purpose:  Perform the tests that call the SLQSStartStopDataSession() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSStartStopDataSession_stop(void)
{
    ULONG nRet;

    /* Define the test cases to be executed */
    SLQSStartStopDataSessTestCase_t SLQSStartStopDataSessTestCase[]=
    {
        { eQCWWAN_ERR_NONE, 1, 0, 1, 0, 0,
          "All Valid 3GPP - Stop Data Session"},
#if 0
        { eQCWWAN_ERR_NONE, 2, 0, 0x2, 0, 0,
          "All Valid 3GPP2 - Stop Data Session"},
#endif
    };

    FILE *fp = tfopen("../../wds/test/results/slqsstartstopdatasession_stop.txt","w");
    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSStartStopDataSessTestCase ) /
                      sizeof( SLQSStartStopDataSessTestCase[0] );
        while(tc < ncases )
        {
            SLQSStartStopDataSessTestCase_t *ps = &SLQSStartStopDataSessTestCase[tc];

            fprintf( fp,
                     "\n\nTest Case %d / %d : %s\n",
                     (tc + 1),
                     ncases,
                     ps->desc);

            fprintf( fp,
                     "Parameter Set:\nAction :%d\nTechnology :%lu\n"\
                     "profID3GPP :%lu\nprofID3GPP2 :%lu\n",
                     ps->action,
                     ps->Technology,
                     ps->profID3GPP,
                     ps->profID3GPP2 );

            /* Call the API */
            s.action = ps->action;
            s.pTechnology = &ps->Technology;
#if 0
            s.pProfileId3GPP = &ps->profID3GPP;
            s.pProfileId3GPP2 = &ps->profID3GPP2;
#endif
            s.pProfileId3GPP = NULL;
            s.pProfileId3GPP2 = NULL;
            nRet = SLQSStartStopDataSession(&s);

            /* Display result code and text */
            doprintreason (fp, nRet);

            tc++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doSLQSStartStopDataSession_start
 *
 * Purpose:  Perform the tests that call the SLQSStartStopDataSession() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSStartStopDataSession_start(void)
{
    ULONG nRet;

    /* Define the test cases to be executed */
    SLQSStartStopDataSessTestCase_t SLQSStartStopDataSessTestCase[]=
    {
        /* 3GPP test cases */
        { eQCWWAN_ERR_NONE, 1, 1, 1, 0, 0,
          "All Valid 3GPP - Start Data Session - UMTS " },
        { eQCWWAN_ERR_NONE, 2, 1, 2, 0, 0,
          "All Valid 3GPP - Start Data Session - CDMA" },
        { eQCWWAN_ERR_NONE, 3, 1, 3, 0, 0,
          "All Valid 3GPP - Start Data Session - eMBMS" },
        { eQCWWAN_ERR_NONE, 4, 1, 4, 0, 0,
          "All Valid 3GPP - Start Data Session - mLINK" },

#if 0
        { eQCWWAN_ERR_NONE, 2, 0, 1, 0, 0,
          "All Valid 3GPP - Stop Data Session"},

        { eQCWWAN_ERR_QMI_INVALID_PROFILE, 3, 1, 0x1, 20,0,
          "3GPP - Invalid profileid"},

        /* 3GPP2 test cases */
        { eQCWWAN_ERR_NONE, 3, 1, 0x2, 0, 0,
          "All Valid 3GPP2 - Start Data Session"},

        { eQCWWAN_ERR_QMI_INVALID_PROFILE, 4, 1, 0x2, 0, 20,
          "3GPP2 - Invalid profileid"},

        /* Mix test cases */
        { eQCWWAN_ERR_INVALID_ARG, 5,  3, 0x3, 1, 1, "Invalid Action value"},
        { eQCWWAN_ERR_NONE, 8,  1, 0x3, 1, 1, "All Valid - Start Data Session"},
        { eQCWWAN_ERR_NONE, 9,  0, 0x3, 1, 1, "All Valid - Stop Data Session"},
#endif
    };

    FILE *fp = tfopen("../../wds/test/results/slqsstartstopdatasession_start.txt","w");
    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSStartStopDataSessTestCase ) /
                      sizeof( SLQSStartStopDataSessTestCase[0] );
        while(tc < ncases )
        {
            SLQSStartStopDataSessTestCase_t *ps = &SLQSStartStopDataSessTestCase[tc];

            fprintf( fp,
                     "\n\nTest Case %d / %d : %s\n",
                     (tc + 1),
                     ncases,
                     ps->desc);

            fprintf( fp,
                     "Parameter Set:\nAction :%d\nTechnology :%lu\n"\
                     "profID3GPP :%lu\nprofID3GPP2 :%lu\n",
                     ps->action,
                     ps->Technology,
                     ps->profID3GPP,
                     ps->profID3GPP2 );

            /* Call the API */
            s.v4sessionId = s.v6sessionId = 0;
            s.action = ps->action;
            s.pTechnology = &ps->Technology;
#if 0
            s.pProfileId3GPP = &ps->profID3GPP;
            s.pProfileId3GPP2 = &ps->profID3GPP2;
#endif
            s.pProfileId3GPP = NULL;
            s.pProfileId3GPP2 = NULL;
            nRet = SLQSStartStopDataSession(&s);

            /* Display result code and text */
            doprintreason (fp, nRet);

            tc++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doSetMobileIPParameters
 *
 * Purpose:  Perform the tests that call the SetMobileIPParameters() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSetMobileIPParameters( void )
{
    BYTE  tCaseNum = 0;
    CHAR  SPC[7];
    ULONG Mode;
    BYTE  RetryLimit;
    BYTE  RetryInterval;
    BYTE  ReRegPeriod;
    BYTE  ReRegTraffic;
    BYTE  HAAuthenticator;
    BYTE  HA2002bis;
    FILE  *fp=NULL;

    fp = tfopen("../../wds/test/results/setmobileipparameters.txt","w");

    if (fp)
    {
        while (tCaseNum < MAX_SET_MOBILE_IP_PARAM_TESTCASE_NUM)
        {
            strcpy(SPC,SetMobileIPParamTestCases[tCaseNum].SPC);
            Mode = SetMobileIPParamTestCases[tCaseNum].Mode;
            RetryLimit = SetMobileIPParamTestCases[tCaseNum].RetryLimit;
            RetryInterval = SetMobileIPParamTestCases[tCaseNum].RetryInterval;
            ReRegPeriod = SetMobileIPParamTestCases[tCaseNum].ReRegPeriod;
            ReRegTraffic = SetMobileIPParamTestCases[tCaseNum].ReRegTraffic;
            HAAuthenticator = SetMobileIPParamTestCases[tCaseNum].
                                                             HAAuthenticator;
            HA2002bis = SetMobileIPParamTestCases[tCaseNum].HA2002bis;

            fprintf(fp,"\n\nTest Case %d / %d : %s\n",
                       (tCaseNum + 1),
                       MAX_SET_MOBILE_IP_PARAM_TESTCASE_NUM,
                       SetMobileIPParamTestCases[tCaseNum].desc);
            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "SPC:%s\n",SPC );
            fprintf(fp, "Mode:%lu\n",Mode);
            fprintf(fp, "RetryLimit:%d\n",RetryLimit);
            fprintf(fp, "RetryInterval:%d\n",RetryInterval);
            fprintf(fp, "ReRegPeriod:%d\n",ReRegPeriod);
            fprintf(fp, "ReRegTraffic:%d\n",ReRegTraffic);
            fprintf(fp, "HAAuthenticator:%d\n",HAAuthenticator);
            fprintf(fp, "HA2002bis:%d\n",HA2002bis);

            ULONG nRet = SetMobileIPParameters( SPC,
                                                &Mode,
                                                &RetryLimit,
                                                &RetryInterval,
                                                &ReRegPeriod,
                                                &ReRegTraffic,
                                                &HAAuthenticator,
                                                &HA2002bis );

            fprintf(fp, "Return Code: %lu\n", nRet);
            doprintreason( fp, nRet );

            tCaseNum++;
        }
   }

   if (fp)
       tfclose(fp);
}

/*
 * Name:     doSLQSSetIPFamilyPreference_v4
 *
 * Purpose:  Perform the tests that call the SLQSSetIPFamilyPreference() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSSetIPFamilyPreference_v4( void )
{
    /* Define the test cases to be executed */
    SLQSSetIPFamilyPreferenceTestCase_t SLQSSetIPFamilyPreferenceTestCase[] =
    {
        { eQCWWAN_ERR_NONE, 1, 4, "All Valid IPv4 Family Preference" },
    };

    FILE  *fp = tfopen( "../../wds/test/results/slqssetipfamilypreference.txt", "w" );
    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSSetIPFamilyPreferenceTestCase ) /
                      sizeof( SLQSSetIPFamilyPreferenceTestCase[0] );
        while( tc < ncases )
        {
            SLQSSetIPFamilyPreferenceTestCase_t *pd = &SLQSSetIPFamilyPreferenceTestCase[tc];
            fprintf( fp,
                     "\nTest Case %d / %d : %s\n",
                     (tc + 1),
                     ncases,
                     pd->desc );

            fprintf( fp,
                     "Parameter Set:\nIP Family Preference: %d\n",
                     pd->IPFamilyPreference );

            /* Call the API */
            ULONG nRet = SLQSSetIPFamilyPreference(pd->IPFamilyPreference);

            /* Display result code and text */
            doprintreason(fp, nRet);
            tc++;
        }
    }
    if (fp)
        tfclose(fp);
}
/*
 * Name:     doSLQSSetIPFamilyPreference_v6
 *
 * Purpose:  Perform the tests that call the SLQSSetIPFamilyPreference() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSSetIPFamilyPreference_v6( void )
{
    /* Define the test cases to be executed */
    SLQSSetIPFamilyPreferenceTestCase_t SLQSSetIPFamilyPreferenceTestCase[] =
    {
        { eQCWWAN_ERR_NONE, 1, 6, "All Valid IPv6 Family Preference" },
    };

    FILE  *fp = tfopen( "../../wds/test/results/slqssetipfamilypreference.txt", "w" );
    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSSetIPFamilyPreferenceTestCase ) /
                      sizeof( SLQSSetIPFamilyPreferenceTestCase[0] );
        while( tc < ncases )
        {
            SLQSSetIPFamilyPreferenceTestCase_t *pd = &SLQSSetIPFamilyPreferenceTestCase[tc];
            fprintf( fp,
                     "\nTest Case %d / %d : %s\n",
                     (tc + 1),
                     ncases,
                     pd->desc );

            fprintf( fp,
                     "Parameter Set:\nIP Family Preference: %d\n",
                     pd->IPFamilyPreference );

            /* Call the API */
            ULONG nRet = SLQSSetIPFamilyPreference(pd->IPFamilyPreference);

            /* Display result code and text */
            doprintreason(fp, nRet);
            tc++;
        }
    }
    if (fp)
        tfclose(fp);
}
/*
 * Name:     doSLQSSetIPFamilyPreference_v4v6
 *
 * Purpose:  Perform the tests that call the SLQSSetIPFamilyPreference() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSSetIPFamilyPreference_v4v6( void )
{
    /* Define the test cases to be executed */
    SLQSSetIPFamilyPreferenceTestCase_t SLQSSetIPFamilyPreferenceTestCase[] =
    {
        { eQCWWAN_ERR_NONE, 1, 7, "All Valid IPv4v6 Family Preference" },
    };

    FILE  *fp = tfopen( "../../wds/test/results/slqssetipfamilypreference.txt", "w" );
    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSSetIPFamilyPreferenceTestCase ) /
                      sizeof( SLQSSetIPFamilyPreferenceTestCase[0] );
        while( tc < ncases )
        {
            SLQSSetIPFamilyPreferenceTestCase_t *pd = &SLQSSetIPFamilyPreferenceTestCase[tc];
            fprintf( fp,
                     "\nTest Case %d / %d : %s\n",
                     (tc + 1),
                     ncases,
                     pd->desc );

            fprintf( fp,
                     "Parameter Set:\nIP Family Preference: %d\n",
                     pd->IPFamilyPreference );

            /* Call the API */
            ULONG nRet = SLQSSetIPFamilyPreference(pd->IPFamilyPreference);

            /* Display result code and text */
            doprintreason(fp, nRet);
            tc++;
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:     doSLQSDeleteProfile
 *
 * Purpose:  Perform the tests that call the SLQSDeleteProfile() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSDeleteProfile( void )
{
    struct SLQSDeleteProfileParams deleteProfile;
    WORD                           extendedErrorCode = 0;
    BYTE                           tCaseNum = 0;
    ULONG                          nRet;
    FILE                           *fp = NULL;

    fp = tfopen( "../../wds/test/results/slqsdeleteprofile.txt", "w" );

    /* Define the test cases to be executed */
    const SLQSDeleteProfileTestCase_t SLQSDeleteProfileTestCase
          [MAX_SLQS_DELETE_PROFILE_TESTCASE_NUM] =
    {
        { eQCWWAN_ERR_QMI_EXTENDED_INTERNAL, 2, 0x18, &deleteProfile,
                                           "Invalid Profile Id" },
        { eQCWWAN_ERR_NONE,                  1, 0x05, &deleteProfile,
                                           "All Valid - Valid profile Id" },
        { eQCWWAN_ERR_NONE,                  4, 0x03, &deleteProfile,
                                           "All Valid - Valid profile Id" } };

    if (fp)
    {
        while( tCaseNum < MAX_SLQS_DELETE_PROFILE_TESTCASE_NUM )
        {
            extendedErrorCode = 0;
            fprintf( fp, "\n\nTest Case %d / %d : %s\n",
                       (tCaseNum + 1),
                       MAX_SLQS_DELETE_PROFILE_TESTCASE_NUM,
                       SLQSDeleteProfileTestCase[tCaseNum].desc );

            fprintf( fp, "Parameter Set:\n" );
            SLQSDeleteProfileTestCase[tCaseNum].pDeleteProfile->profileIndex =
            SLQSDeleteProfileTestCase[tCaseNum].profileIndex;

            SLQSDeleteProfileTestCase[tCaseNum].pDeleteProfile->profileType =0;
            fprintf( fp, "Profile Type : 0x%x\n",
            SLQSDeleteProfileTestCase[tCaseNum].pDeleteProfile->profileType );
            fprintf( fp, "Profile Index : 0x%x\n",
            SLQSDeleteProfileTestCase[tCaseNum].pDeleteProfile->profileIndex );

            /* Call the API */
            nRet = SLQSDeleteProfile (
                          SLQSDeleteProfileTestCase[tCaseNum].pDeleteProfile,
                          &extendedErrorCode );

            /* Display result code and text */
            doprintreason ( fp, nRet );
            fprintf( fp, "Extended Error Code : %d\n", extendedErrorCode );
            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:     doSLQSCreate3GPPProfile
 *
 * Purpose:  Perform the tests that call the SLQSCreateProfile() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSCreate3GPPProfile( void )
{
    BYTE   tCaseNum  = 0;
    ULONG  nRet;
    FILE   *fp = NULL;
    BYTE   profileType  = 0;
    BYTE   profileIndex = 0;
    USHORT extErrorCode = 0;

    struct CreateProfileIn  lCreateProfileIn;
    struct CreateProfileOut lWdsCreateProfileOut;
    lWdsCreateProfileOut.pProfileType   = &profileType;
    lWdsCreateProfileOut.pProfileIndex  = &profileIndex;
    lWdsCreateProfileOut.pExtErrorCode  = &extErrorCode;

    fp = tfopen("../../wds/test/results/slqscreateprofile.txt", "w");
    if (fp)
    {
        while (tCaseNum < MAX_SLQS_CREATE_PROFILE_TESTCASE_NUM)
        {
            fprintf(fp,
                    "\nTest Case %d / %d : %s\n",
                    (tCaseNum + 1),
                    MAX_SLQS_CREATE_PROFILE_TESTCASE_NUM,
                    SLQSCreateProfileTestCases[tCaseNum].desc);

            /*Intialize the structure to be sent to API */
            /*Intialize ProfileID: reserved for future use */
            lCreateProfileIn.pProfileID =
                   SLQSCreateProfileTestCases[tCaseNum].pProfileID;;

            /* Intialize Profile Type*/
            lCreateProfileIn.pProfileType =
                   SLQSCreateProfileTestCases[tCaseNum].pProfileType;

            /* Intialize pProfilename */
            lCreateProfileIn.curProfile.SlqsProfile3GPP.pProfilename =
                   SLQSCreateProfileTestCases[tCaseNum].pProfilename;

            /* Intialize PDPType*/
            lCreateProfileIn.curProfile.SlqsProfile3GPP.pPDPtype =
                   SLQSCreateProfileTestCases[tCaseNum].pPDPtype;

            /* Intialize PdpHdrCompType */
            lCreateProfileIn.curProfile.SlqsProfile3GPP.pPdpHdrCompType =
                   SLQSCreateProfileTestCases[tCaseNum].pPdpHdrCompType;

            lCreateProfileIn.curProfile.SlqsProfile3GPP.pPdpDataCompType =
                   SLQSCreateProfileTestCases[tCaseNum].pPdpDataCompType;

            /* Intialize APNName */
            lCreateProfileIn.curProfile.SlqsProfile3GPP.pAPNName =
                   SLQSCreateProfileTestCases[tCaseNum].pAPNName;

            /* Intialize priDNSIPv4AddPref */
            lCreateProfileIn.curProfile.SlqsProfile3GPP.pPriDNSIPv4AddPref =
                   SLQSCreateProfileTestCases[tCaseNum].pPriDNSIPv4AddPref;

            /* Intialize secDNSIPv4AddPref */
            lCreateProfileIn.curProfile.SlqsProfile3GPP.pSecDNSIPv4AddPref =
                   SLQSCreateProfileTestCases[tCaseNum].pSecDNSIPv4AddPref;

            /* Intialize UMTSReqQoS */
            lCreateProfileIn.curProfile.SlqsProfile3GPP.pUMTSReqQoS =
                   SLQSCreateProfileTestCases[tCaseNum].pUMTSReqQoS;

            /* Intialize UMTSMinQoS */
            lCreateProfileIn.curProfile.SlqsProfile3GPP.pUMTSMinQoS =
                   SLQSCreateProfileTestCases[tCaseNum].pUMTSMinQoS;

            /* Intialize GPRSRequestedQos */
            lCreateProfileIn.curProfile.SlqsProfile3GPP.pGPRSRequestedQos =
                   SLQSCreateProfileTestCases[tCaseNum].pGPRSRequestedQos;

            /* Intialize GPRSMinimumQoS */
            lCreateProfileIn.curProfile.SlqsProfile3GPP.pGPRSMinimumQoS =
                   SLQSCreateProfileTestCases[tCaseNum].pGPRSMinimumQoS;

            /* Intialize Username */
            lCreateProfileIn.curProfile.SlqsProfile3GPP.pUsername =
                   SLQSCreateProfileTestCases[tCaseNum].pUsername;

            /* Intialize Password */
            lCreateProfileIn.curProfile.SlqsProfile3GPP.pPassword =
                   SLQSCreateProfileTestCases[tCaseNum].pPassword;

            lCreateProfileIn.curProfile.SlqsProfile3GPP.pAuthenticationPref
                   = SLQSCreateProfileTestCases[tCaseNum].pAuthenticationPref;

            /* Intialize IPv4AddrPref */
            lCreateProfileIn.curProfile.SlqsProfile3GPP.pIPv4AddrPref =
                   SLQSCreateProfileTestCases[tCaseNum].pIPv4AddrPref;

            /* Intialize pcscfAddrUsingPCO */
            lCreateProfileIn.curProfile.SlqsProfile3GPP.pPcscfAddrUsingPCO =
                   SLQSCreateProfileTestCases[tCaseNum].pPcscfAddrUsingPCO;

            /* Intialize pdpAccessConFlag */
            lCreateProfileIn.curProfile.SlqsProfile3GPP.pPdpAccessConFlag =
                   SLQSCreateProfileTestCases[tCaseNum].pPdpAccessConFlag;

            /* Intialize pcscfAddrUsingDhcp */
            lCreateProfileIn.curProfile.SlqsProfile3GPP.pPcscfAddrUsingDhcp =
                   SLQSCreateProfileTestCases[tCaseNum].pPcscfAddrUsingDhcp;

            /* Intialize imCnFlag */
            lCreateProfileIn.curProfile.SlqsProfile3GPP.pImCnFlag =
                   SLQSCreateProfileTestCases[tCaseNum].pImCnFlag;

            /* Intialize imCnFlag */
            lCreateProfileIn.curProfile.SlqsProfile3GPP.pTFTID1Params =
                   SLQSCreateProfileTestCases[tCaseNum].pTFTID1Params;

            /* Intialize imCnFlag */
            lCreateProfileIn.curProfile.SlqsProfile3GPP.pTFTID2Params=
                   SLQSCreateProfileTestCases[tCaseNum].pTFTID2Params;

            /* Intialize pdpContext */
            lCreateProfileIn.curProfile.SlqsProfile3GPP.pPdpContext =
                   SLQSCreateProfileTestCases[tCaseNum].pPdpContext;

            /* Intialize secondaryFlag */
            lCreateProfileIn.curProfile.SlqsProfile3GPP.pSecondaryFlag =
                   SLQSCreateProfileTestCases[tCaseNum].pSecondaryFlag;

            /* Intialize primaryID */
            lCreateProfileIn.curProfile.SlqsProfile3GPP.pPrimaryID =
                   SLQSCreateProfileTestCases[tCaseNum].pPrimaryID;

            /* Intialize pIPv6AddPref */
            lCreateProfileIn.curProfile.SlqsProfile3GPP.pIPv6AddPref =
                   SLQSCreateProfileTestCases[tCaseNum].pIPv6AddPref;

            /* Intialize pUMTSReqQoSSigInd */
            lCreateProfileIn.curProfile.SlqsProfile3GPP.pUMTSReqQoSSigInd =
                   SLQSCreateProfileTestCases[tCaseNum].pUMTSReqQoSSigInd;

            /* Intialize pUMTSMinQoSSigInd */
            lCreateProfileIn.curProfile.SlqsProfile3GPP.pUMTSMinQosSigInd =
                   SLQSCreateProfileTestCases[tCaseNum].pUMTSMinQosSigInd;

            /* Intialize pPriDNSIPv6addpref */
            lCreateProfileIn.curProfile.SlqsProfile3GPP.pPriDNSIPv6addpref =
                   SLQSCreateProfileTestCases[tCaseNum].pPriDNSIPv6addpref;

            /* Intialize pSecDNSIPv6addpref */
            lCreateProfileIn.curProfile.SlqsProfile3GPP.pSecDNSIPv6addpref =
                   SLQSCreateProfileTestCases[tCaseNum].pSecDNSIPv6addpref;

            /* Intialize pAddrAllocPref */
            lCreateProfileIn.curProfile.SlqsProfile3GPP.pAddrAllocPref =
                   SLQSCreateProfileTestCases[tCaseNum].pAddrAllocPref;

            /* Intialize pAddrAllocPref */
            lCreateProfileIn.curProfile.SlqsProfile3GPP.pQosClassID =
                   SLQSCreateProfileTestCases[tCaseNum].pQosClassID;

            /* Call the API, get the values */
            nRet = SLQSCreateProfile(&lCreateProfileIn,
                                     &lWdsCreateProfileOut);

            /* Print Reason */
            doprintreason (fp, nRet);
            if ( !nRet )
            {
                fprintf(fp, "SLQSCreateProfile Successful\n");

                fprintf(fp, "profileType: %d\n", profileType);

                fprintf(fp, "ProfileIndex: %d\n", profileIndex);
            }
            if( EXTENDED_ERR == nRet)
            {

                fprintf(fp, "Extended Error Code: %d\n", extErrorCode);
            }
            tCaseNum++;
        }
    }
    if (fp)
    tfclose(fp);
}

/*
 * Name:     doSLQSCreate3GPP2Profile
 *
 * Purpose:  Perform the tests that call the SLQSCreateProfile() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSCreate3GPP2Profile(void)
{
    BYTE   tCaseNum  = 0;
    ULONG  nRet;
    FILE   *fp = NULL;
    BYTE   profileType  = 0;
    BYTE   profileIndex = 0;
    USHORT extErrorCode = 0;

    struct CreateProfileIn  lCreateProfileIn;
    struct CreateProfileOut lWdsCreateProfileOut;
    lWdsCreateProfileOut.pProfileType   = &profileType;
    lWdsCreateProfileOut.pProfileIndex  = &profileIndex;
    lWdsCreateProfileOut.pExtErrorCode  = &extErrorCode;

    fp = tfopen("../../wds/test/results/slqscreateprofile.txt", "w");
    if (fp)
    {
        while (tCaseNum < MAX_SLQS_CREATE_PROFILE2_TESTCASE_NUM)
        {
            fprintf(fp,
                    "\nTest Case %d / %d : %s\n",
                    (tCaseNum + 1),
                    MAX_SLQS_CREATE_PROFILE2_TESTCASE_NUM,
                    SLQSCreateProfile2TestCases[tCaseNum].desc);

            /*Intialize the structure to be sent to API */
            /*Intialize ProfileID: reserved for future use */
             lCreateProfileIn.pProfileID =
                    SLQSCreateProfile2TestCases[tCaseNum].pProfileID;;

            /* Intialize Profile Type*/
            lCreateProfileIn.pProfileType =
                   SLQSCreateProfile2TestCases[tCaseNum].pProfileType;

            /* Intialize pProfilename */
            lCreateProfileIn.curProfile.SlqsProfile3GPP2.pNegoDnsSrvrPref =
                   SLQSCreateProfile2TestCases[tCaseNum].pNegoDnsSrvrPref;

            lCreateProfileIn.curProfile.SlqsProfile3GPP2.pPppSessCloseTimerDO =
                   SLQSCreateProfile2TestCases[tCaseNum].pPppSessCloseTimerDO;

            lCreateProfileIn.curProfile.SlqsProfile3GPP2.pPppSessCloseTimer1x =
                   SLQSCreateProfile2TestCases[tCaseNum].pPppSessCloseTimer1x;

            lCreateProfileIn.curProfile.SlqsProfile3GPP2.pAllowLinger =
                   SLQSCreateProfile2TestCases[tCaseNum].pAllowLinger;

            lCreateProfileIn.curProfile. SlqsProfile3GPP2.pLcpAckTimeout =
                   SLQSCreateProfile2TestCases[tCaseNum].pLcpAckTimeout;

            lCreateProfileIn.curProfile.SlqsProfile3GPP2.pIpcpAckTimeout =
                   SLQSCreateProfile2TestCases[tCaseNum].pIpcpAckTimeout;

            lCreateProfileIn.curProfile.SlqsProfile3GPP2.pAuthTimeout =
                   SLQSCreateProfile2TestCases[tCaseNum].pAuthTimeout;


            lCreateProfileIn.curProfile.SlqsProfile3GPP2.pLcpCreqRetryCount =
                   SLQSCreateProfile2TestCases[tCaseNum].pLcpCreqRetryCount;

            lCreateProfileIn.curProfile.SlqsProfile3GPP2.pIpcpCreqRetryCount =
                   SLQSCreateProfile2TestCases[tCaseNum].pIpcpCreqRetryCount;

            lCreateProfileIn.curProfile.SlqsProfile3GPP2.pAuthRetryCount =
                   SLQSCreateProfile2TestCases[tCaseNum].pAuthRetryCount;

            lCreateProfileIn.curProfile.SlqsProfile3GPP2.pAuthProtocol =
                   SLQSCreateProfile2TestCases[tCaseNum].pAuthProtocol;

            lCreateProfileIn.curProfile.SlqsProfile3GPP2.pUserId =
                   SLQSCreateProfile2TestCases[tCaseNum].pUserId;

            lCreateProfileIn.curProfile.SlqsProfile3GPP2.pAuthPassword =
                   SLQSCreateProfile2TestCases[tCaseNum].pAuthPassword;

            lCreateProfileIn.curProfile.SlqsProfile3GPP2.pDataRate =
                   SLQSCreateProfile2TestCases[tCaseNum].pDataRate;

            lCreateProfileIn.curProfile.SlqsProfile3GPP2.pAppType =
              SLQSCreateProfile2TestCases[tCaseNum].pAppType;

            lCreateProfileIn.curProfile.SlqsProfile3GPP2.pDataMode =
              SLQSCreateProfile2TestCases[tCaseNum].pDataMode;

            lCreateProfileIn.curProfile.SlqsProfile3GPP2.pAppPriority =
                   SLQSCreateProfile2TestCases[tCaseNum].pAppPriority;

            lCreateProfileIn.curProfile.SlqsProfile3GPP2.pApnString =
                   SLQSCreateProfile2TestCases[tCaseNum].pApnString;

            lCreateProfileIn.curProfile.SlqsProfile3GPP2.pPdnType =
              SLQSCreateProfile2TestCases[tCaseNum].pPdnType;

            lCreateProfileIn.curProfile.SlqsProfile3GPP2.pIsPcscfAddressNedded =
                   SLQSCreateProfile2TestCases[tCaseNum].pIsPcscfAddressNedded;

            lCreateProfileIn.curProfile.SlqsProfile3GPP2.pPrimaryV4DnsAddress =
                   SLQSCreateProfile2TestCases[tCaseNum].pPrimaryV4DnsAddress;

            lCreateProfileIn.curProfile.SlqsProfile3GPP2.pSecondaryV4DnsAddress
                   = SLQSCreateProfile2TestCases[tCaseNum].pSecondaryV4DnsAddress;

            lCreateProfileIn.curProfile.SlqsProfile3GPP2.pPriV6DnsAddress =
                   SLQSCreateProfile2TestCases[tCaseNum].pPriV6DnsAddress;

            lCreateProfileIn.curProfile.SlqsProfile3GPP2.pSecV6DnsAddress =
                   SLQSCreateProfile2TestCases[tCaseNum].pSecV6DnsAddress;

            /* call the API, get the values */
            nRet = SLQSCreateProfile(&lCreateProfileIn,
                                     &lWdsCreateProfileOut);
            doprintreason (fp, nRet);
            if ( !nRet )
            {
                fprintf(fp, "SLQSCreateProfile Successful\n");

                fprintf(fp, "profileType: %d\n", profileType);

                fprintf(fp, "ProfileIndex: %d\n", profileIndex);
            }
            if( EXTENDED_ERR == nRet )
            {
                fprintf(fp, "Extended Error Code: %d\n", extErrorCode);
            }
            tCaseNum++;
        }
    }
    if (fp)
    tfclose(fp);
}

/*
 * Name:     doSLQSCreateProfile
 *
 * Purpose:  Executes doSLQSCreate3GPPProfile and doSLQSCreate3GPP2Profile
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSCreateProfile( void )
{
    doSLQSCreate3GPPProfile();
    doSLQSCreate3GPP2Profile();
}

/*
 * Name:     doSLQSAutoConnect
 *
 * Purpose:  Perform the tests that call the SLQSAutoConnect() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSAutoConnect( void )
{
    BYTE   tCaseNum = 0;
    ULONG  nRet;
    FILE   *fp = NULL;
    CHAR   selOption[OPTION_LEN];
    CHAR   *pEndOfLine = NULL;
    struct slqsautoconnect lSlqsautoconnect;

    fp = tfopen("../../wds/test/results/slqsautoconnectsettings.txt", "w");
    if (fp)
    {
        while(1)
        {
            fprintf( stderr,
                     "\nPlease select one of the following options or"
                     " press <Enter> to go to main menu:\n"\
                     "1.Autoconnect Enabled \n"\
                     "2.Autoconnect Disabled\n"\
                     "3.Autoconnect Paused\n"\
                     "4.Autoconnect roaming Always allowed\n"\
                     "5.Autoconnect while in home service area only\n"\
                     "6.GetAutoconnect Settings\n"
                     "Option: " );

            /* Receive the input from the user */
            fgets( selOption, ( OPTION_LEN ), stdin );

            /* If '/n' character is not read, there are more characters in input
             * stream. Clear the input stream.
             */
            pEndOfLine = strchr( selOption, ENTER_KEY );
            if( NULL == pEndOfLine )
            {
                FlushStdinStream();
            }
            /* If user has entered an invalid input, prompt again */
            if( 2 < strlen(selOption) )
            {
                fprintf(stderr,"Please choose valid option from menu \n");
                continue;
            }
            switch( selOption[0] )
            {
                case eACENABLED:
                case eACDISABLED:
                case eACPAUSED:
                case eACRALWAYSALLOWED:
                case eACRHOMENWONLY:
                    tCaseNum = selOption[0] - 1 + 0x30 ;
                    break;

                case eEXIT_APP:
                    return;
                    break;

                default:
                    break;
            }
            tCaseNum = atoi(selOption) - 1;
            fprintf(fp,
                    "\nTest Case %d / %d : %s\n",
                    (tCaseNum + 1),
                    MAX_SLQS_AC_SETTINGS_TESTCASE_NUM,
                    SLQSAutoConnectTestCases[tCaseNum].desc);

            /* Intialize the structure to be sent to API */
            /* Intialize action */
            lSlqsautoconnect.action =
                SLQSAutoConnectTestCases[tCaseNum].action;

            /* Intialize acsetting */
            lSlqsautoconnect.acsetting =
                SLQSAutoConnectTestCases[tCaseNum].acsetting;

            /* Intialize acroamsetting */
            lSlqsautoconnect.acroamsetting =
                SLQSAutoConnectTestCases[tCaseNum].acroamsetting;

            /* call the API, get the values */
            nRet = SLQSAutoConnect (&lSlqsautoconnect);

            doprintreason (fp, nRet);
            if ( !nRet )
            {
                fprintf(fp, "SLQSAutoConnect Successful\n");

                if ( 0 == lSlqsautoconnect.action )
                {
                    fprintf(fp, "acsetting: %d\n",
                                lSlqsautoconnect.acsetting);

                    fprintf(fp, "acroamsetting: %d\n",
                                lSlqsautoconnect.acroamsetting);
                }
            }
            tCaseNum++;
        }
    }
    if (fp)
    tfclose(fp);
}

/*
 * Name:     doSLQSModify3GPPProfile
 *
 * Purpose:  Perform the tests that call the SLQSModifyProfile() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSModify3GPPProfile( void )
{
    BYTE   tCaseNum  = 0;
    ULONG  nRet;
    FILE   *fp = NULL;
    USHORT extErrorCode = 0;

    struct ModifyProfileIn  lModifyProfileIn;
    struct ModifyProfileOut lModifyProfileOut;
    lModifyProfileOut.pExtErrorCode = &extErrorCode;

    fp = tfopen("../../wds/test/results/slqsModifyprofile.txt", "w");
    if (fp)
    {
        while (tCaseNum < MAX_SLQS_MODIFY_PROFILE_TESTCASE_NUM)
        {
            fprintf(fp,
                    "\nTest Case %d / %d : %s\n",
                    (tCaseNum + 1),
                    MAX_SLQS_MODIFY_PROFILE_TESTCASE_NUM,
                    SLQSModifyProfileTestCases[tCaseNum].desc);

            /*Intialize the structure to be sent to API */
            /*Intialize ProfileID: reserved for future use */
            lModifyProfileIn.pProfileID =
                   SLQSModifyProfileTestCases[tCaseNum].pProfileID;;

            /* Intialize Profile Type*/
            lModifyProfileIn.pProfileType =
                   SLQSModifyProfileTestCases[tCaseNum].pProfileType;

            /* Intialize pProfilename */
            lModifyProfileIn.curProfile.SlqsProfile3GPP.pProfilename =
                   SLQSModifyProfileTestCases[tCaseNum].pProfilename;

            /* Intialize PDPType*/
            lModifyProfileIn.curProfile.SlqsProfile3GPP.pPDPtype =
                   SLQSModifyProfileTestCases[tCaseNum].pPDPtype;

            /* Intialize PdpHdrCompType */
            lModifyProfileIn.curProfile.SlqsProfile3GPP.pPdpHdrCompType =
                   SLQSModifyProfileTestCases[tCaseNum].pPdpHdrCompType;

            lModifyProfileIn.curProfile.SlqsProfile3GPP.pPdpDataCompType =
                   SLQSModifyProfileTestCases[tCaseNum].pPdpDataCompType;

            /* Intialize APNName */
            lModifyProfileIn.curProfile.SlqsProfile3GPP.pAPNName =
                   SLQSModifyProfileTestCases[tCaseNum].pAPNName;

            /* Intialize priDNSIPv4AddPref */
            lModifyProfileIn.curProfile.SlqsProfile3GPP.pPriDNSIPv4AddPref =
                   SLQSModifyProfileTestCases[tCaseNum].pPriDNSIPv4AddPref;

            /* Intialize secDNSIPv4AddPref */
            lModifyProfileIn.curProfile.SlqsProfile3GPP.pSecDNSIPv4AddPref =
                   SLQSModifyProfileTestCases[tCaseNum].pSecDNSIPv4AddPref;

            /* Intialize UMTSReqQoS */
            lModifyProfileIn.curProfile.SlqsProfile3GPP.pUMTSReqQoS =
                   SLQSModifyProfileTestCases[tCaseNum].pUMTSReqQoS;

            /* Intialize UMTSMinQoS */
            lModifyProfileIn.curProfile.SlqsProfile3GPP.pUMTSMinQoS =
                   SLQSModifyProfileTestCases[tCaseNum].pUMTSMinQoS;

            /* Intialize GPRSRequestedQos */
            lModifyProfileIn.curProfile.SlqsProfile3GPP.pGPRSRequestedQos =
                   SLQSModifyProfileTestCases[tCaseNum].pGPRSRequestedQos;

            /* Intialize GPRSMinimumQoS */
            lModifyProfileIn.curProfile.SlqsProfile3GPP.pGPRSMinimumQoS =
                   SLQSModifyProfileTestCases[tCaseNum].pGPRSMinimumQoS;

            /* Intialize Username */
            lModifyProfileIn.curProfile.SlqsProfile3GPP.pUsername =
                   SLQSModifyProfileTestCases[tCaseNum].pUsername;

            /* Intialize Password */
            lModifyProfileIn.curProfile.SlqsProfile3GPP.pPassword =
                   SLQSModifyProfileTestCases[tCaseNum].pPassword;

            lModifyProfileIn.curProfile.SlqsProfile3GPP.pAuthenticationPref
                   = SLQSModifyProfileTestCases[tCaseNum].pAuthenticationPref;

            /* Intialize IPv4AddrPref */
            lModifyProfileIn.curProfile.SlqsProfile3GPP.pIPv4AddrPref =
                   SLQSModifyProfileTestCases[tCaseNum].pIPv4AddrPref;

            /* Intialize pcscfAddrUsingPCO */
            lModifyProfileIn.curProfile.SlqsProfile3GPP.pPcscfAddrUsingPCO =
                   SLQSModifyProfileTestCases[tCaseNum].pPcscfAddrUsingPCO;

            /* Intialize pdpAccessConFlag */
            lModifyProfileIn.curProfile.SlqsProfile3GPP.pPdpAccessConFlag =
                   SLQSModifyProfileTestCases[tCaseNum].pPdpAccessConFlag;

            /* Intialize pcscfAddrUsingDhcp */
            lModifyProfileIn.curProfile.SlqsProfile3GPP.pPcscfAddrUsingDhcp =
                   SLQSModifyProfileTestCases[tCaseNum].pPcscfAddrUsingDhcp;

            /* Intialize imCnFlag */
            lModifyProfileIn.curProfile.SlqsProfile3GPP.pImCnFlag =
                   SLQSModifyProfileTestCases[tCaseNum].pImCnFlag;

            /* Intialize imCnFlag */
            lModifyProfileIn.curProfile.SlqsProfile3GPP.pTFTID1Params =
                   SLQSModifyProfileTestCases[tCaseNum].pTFTID1Params;

            /* Intialize imCnFlag */
            lModifyProfileIn.curProfile.SlqsProfile3GPP.pTFTID2Params=
                   SLQSModifyProfileTestCases[tCaseNum].pTFTID2Params;

            /* Intialize pdpContext */
            lModifyProfileIn.curProfile.SlqsProfile3GPP.pPdpContext =
                   SLQSModifyProfileTestCases[tCaseNum].pPdpContext;

            /* Intialize secondaryFlag */
            lModifyProfileIn.curProfile.SlqsProfile3GPP.pSecondaryFlag =
                   SLQSModifyProfileTestCases[tCaseNum].pSecondaryFlag;

            /* Intialize primaryID */
            lModifyProfileIn.curProfile.SlqsProfile3GPP.pPrimaryID =
                   SLQSModifyProfileTestCases[tCaseNum].pPrimaryID;

            /* Intialize pIPv6AddPref */
            lModifyProfileIn.curProfile.SlqsProfile3GPP.pIPv6AddPref =
                   SLQSModifyProfileTestCases[tCaseNum].pIPv6AddPref;

            /* Intialize pUMTSReqQoSSigInd */
            lModifyProfileIn.curProfile.SlqsProfile3GPP.pUMTSReqQoSSigInd =
                   SLQSModifyProfileTestCases[tCaseNum].pUMTSReqQoSSigInd;

            /* Intialize pUMTSMinQoSSigInd */
            lModifyProfileIn.curProfile.SlqsProfile3GPP.pUMTSMinQosSigInd =
                   SLQSModifyProfileTestCases[tCaseNum].pUMTSMinQosSigInd;

            /* Intialize pPriDNSIPv6addpref */
            lModifyProfileIn.curProfile.SlqsProfile3GPP.pPriDNSIPv6addpref =
                   SLQSModifyProfileTestCases[tCaseNum].pPriDNSIPv6addpref;

            /* Intialize pSecDNSIPv6addpref */
            lModifyProfileIn.curProfile.SlqsProfile3GPP.pSecDNSIPv6addpref =
                   SLQSModifyProfileTestCases[tCaseNum].pSecDNSIPv6addpref;

            /* Intialize pAddrAllocPref */
            lModifyProfileIn.curProfile.SlqsProfile3GPP.pAddrAllocPref =
                   SLQSModifyProfileTestCases[tCaseNum].pAddrAllocPref;

            /* Intialize pAddrAllocPref */
            lModifyProfileIn.curProfile.SlqsProfile3GPP.pQosClassID =
                   SLQSModifyProfileTestCases[tCaseNum].pQosClassID;

            /* Call the API, get the values */
            nRet = SLQSModifyProfile( &lModifyProfileIn,
                                      &lModifyProfileOut );

            /* Print Reason */
            doprintreason (fp, nRet);
            if ( !nRet )
            {
                fprintf(fp, "SLQSModifyProfile Successful\n");
            }
            if( EXTENDED_ERR == nRet )
            {
                fprintf(fp, "Extended Error Code: %d\n", extErrorCode);
            }
            tCaseNum++;
        }
    }
    if (fp)
    tfclose(fp);
}

/*
 * Name:     doSLQSModify3GPP2Profile
 *
 * Purpose:  Perform the tests that call the SLQSModifyProfile() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSModify3GPP2Profile(void)
{
    BYTE   tCaseNum  = 0;
    ULONG  nRet;
    FILE   *fp = NULL;
    USHORT extErrorCode = 0;

    struct ModifyProfileIn  lModifyProfileIn;
    struct ModifyProfileOut lModifyProfileOut;
    lModifyProfileOut.pExtErrorCode = &extErrorCode;

    fp = tfopen("../../wds/test/results/slqsModifyprofile.txt", "w");
    if (fp)
    {
        while (tCaseNum < MAX_SLQS_MODIFY_PROFILE2_TESTCASE_NUM)
        {
            fprintf(fp,
                    "\nTest Case %d / %d : %s\n",
                    (tCaseNum + 1),
                    MAX_SLQS_MODIFY_PROFILE2_TESTCASE_NUM,
                    SLQSModifyProfile2TestCases[tCaseNum].desc);

            /*Intialize the structure to be sent to API */
            /*Intialize ProfileID: reserved for future use */
             lModifyProfileIn.pProfileID =
                    SLQSModifyProfile2TestCases[tCaseNum].pProfileID;;

            /* Intialize Profile Type*/
            lModifyProfileIn.pProfileType =
                   SLQSModifyProfile2TestCases[tCaseNum].pProfileType;

            /* Intialize pProfilename */
            lModifyProfileIn.curProfile.SlqsProfile3GPP2.pNegoDnsSrvrPref =
                   SLQSModifyProfile2TestCases[tCaseNum].pNegoDnsSrvrPref;

            lModifyProfileIn.curProfile.SlqsProfile3GPP2.pPppSessCloseTimerDO =
                   SLQSModifyProfile2TestCases[tCaseNum].pPppSessCloseTimerDO;

            lModifyProfileIn.curProfile.SlqsProfile3GPP2.pPppSessCloseTimer1x =
                   SLQSModifyProfile2TestCases[tCaseNum].pPppSessCloseTimer1x;

            lModifyProfileIn.curProfile.SlqsProfile3GPP2.pAllowLinger =
                   SLQSModifyProfile2TestCases[tCaseNum].pAllowLinger;

            lModifyProfileIn.curProfile. SlqsProfile3GPP2.pLcpAckTimeout =
                   SLQSModifyProfile2TestCases[tCaseNum].pLcpAckTimeout;

            lModifyProfileIn.curProfile.SlqsProfile3GPP2.pIpcpAckTimeout =
                   SLQSModifyProfile2TestCases[tCaseNum].pIpcpAckTimeout;

            lModifyProfileIn.curProfile.SlqsProfile3GPP2.pAuthTimeout =
                   SLQSModifyProfile2TestCases[tCaseNum].pAuthTimeout;


            lModifyProfileIn.curProfile.SlqsProfile3GPP2.pLcpCreqRetryCount =
                   SLQSModifyProfile2TestCases[tCaseNum].pLcpCreqRetryCount;

            lModifyProfileIn.curProfile.SlqsProfile3GPP2.pIpcpCreqRetryCount =
                   SLQSModifyProfile2TestCases[tCaseNum].pIpcpCreqRetryCount;

            lModifyProfileIn.curProfile.SlqsProfile3GPP2.pAuthRetryCount =
                   SLQSModifyProfile2TestCases[tCaseNum].pAuthRetryCount;

            lModifyProfileIn.curProfile.SlqsProfile3GPP2.pAuthProtocol =
                   SLQSModifyProfile2TestCases[tCaseNum].pAuthProtocol;

            lModifyProfileIn.curProfile.SlqsProfile3GPP2.pUserId =
                   SLQSModifyProfile2TestCases[tCaseNum].pUserId;

            lModifyProfileIn.curProfile.SlqsProfile3GPP2.pAuthPassword =
                   SLQSModifyProfile2TestCases[tCaseNum].pAuthPassword;

            lModifyProfileIn.curProfile.SlqsProfile3GPP2.pDataRate =
                   SLQSModifyProfile2TestCases[tCaseNum].pDataRate;

            lModifyProfileIn.curProfile.SlqsProfile3GPP2.pAppType =
              SLQSModifyProfile2TestCases[tCaseNum].pAppType;

            lModifyProfileIn.curProfile.SlqsProfile3GPP2.pDataMode =
              SLQSModifyProfile2TestCases[tCaseNum].pDataMode;

            lModifyProfileIn.curProfile.SlqsProfile3GPP2.pAppPriority =
                   SLQSModifyProfile2TestCases[tCaseNum].pAppPriority;

            lModifyProfileIn.curProfile.SlqsProfile3GPP2.pApnString =
                   SLQSModifyProfile2TestCases[tCaseNum].pApnString;

            lModifyProfileIn.curProfile.SlqsProfile3GPP2.pPdnType =
              SLQSModifyProfile2TestCases[tCaseNum].pPdnType;

            lModifyProfileIn.curProfile.SlqsProfile3GPP2.pIsPcscfAddressNedded =
                   SLQSModifyProfile2TestCases[tCaseNum].pIsPcscfAddressNedded;

            lModifyProfileIn.curProfile.SlqsProfile3GPP2.pPrimaryV4DnsAddress =
                   SLQSModifyProfile2TestCases[tCaseNum].pPrimaryV4DnsAddress;

            lModifyProfileIn.curProfile.SlqsProfile3GPP2.pSecondaryV4DnsAddress
                   = SLQSModifyProfile2TestCases[tCaseNum].pSecondaryV4DnsAddress;

            lModifyProfileIn.curProfile.SlqsProfile3GPP2.pPriV6DnsAddress =
                   SLQSModifyProfile2TestCases[tCaseNum].pPriV6DnsAddress;

            lModifyProfileIn.curProfile.SlqsProfile3GPP2.pSecV6DnsAddress =
                   SLQSModifyProfile2TestCases[tCaseNum].pSecV6DnsAddress;

            /* call the API, get the values */
             nRet = SLQSModifyProfile(&lModifyProfileIn,
                                      &lModifyProfileOut );
            doprintreason (fp, nRet);
            if ( !nRet )
            {
                fprintf(fp, "SLQSModifyProfile Successful\n");
            }
            if( EXTENDED_ERR == nRet )
            {
                fprintf(fp, "Extended Error Code: %d\n", extErrorCode);
            }
            tCaseNum++;
        }
    }
    if (fp)
    tfclose(fp);
}

/*
 * Name:     doSLQSModifyProfile
 *
 * Purpose:  Executes doSLQSModify3GPPProfile and doSLQSModify3GPP2Profile
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSModifyProfile( void )
{
    doSLQSModify3GPPProfile();
    doSLQSModify3GPP2Profile();
}

/*
 * Name:     doSLQSSet3GPPConfigItem
 *
 * Purpose:  Perform the tests that call the SLQSSet3GPPConfigItem() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSSet3GPPConfigItem( void )
{
    ULONG              nRet;
    FILE               *fp = NULL;
    slqs3GPPConfigItem swi3gppConfigItem;
    BYTE               counter;

    /* Define the test cases to be executed */
    SLQSSetGet3gppConfigItemTestCase_t SLQSSet3gppConfigTestCases[] =
    {
        { eQCWWAN_ERR_INVALID_ARG, 1, NULL, NULL, NULL, NULL,
          "Invalid Test Case - All params NULL"},

        { eQCWWAN_ERR_NONE, 2, &SetLTEAttachProfile, NULL, NULL, NULL,
           "Valid test Case - LTEAttachProfile set" },

        { eQCWWAN_ERR_NONE, 3, NULL, SetProfileList, NULL, NULL,
           "Valid test Case - ProfileList set" },

        { eQCWWAN_ERR_NONE, 4, NULL, NULL, &SetDefaultPDNEnabled, NULL,
           "Valid test Case - Always Connect Default PDN set" },

        { eQCWWAN_ERR_NONE, 5, NULL, NULL, NULL, &SetRelease3GPP,
           "Valid test Case - 3GPP Release Set" },

        { eQCWWAN_ERR_NONE, 6, &SetLTEAttachProfile, SetProfileList,
           &SetDefaultPDNEnabled, &SetRelease3GPP,
           "Valid test Case - All parameters set" },
    };

    fp = tfopen( "../../wds/test/results/slqsset3gppconfigitem.txt", "w" );
    if ( fp )
    {
        BYTE tc     = 0;
        BYTE ncases = sizeof( SLQSSet3gppConfigTestCases ) /
                      sizeof( SLQSSet3gppConfigTestCases[0] );

        while( tc < ncases )
        {
            SLQSSetGet3gppConfigItemTestCase_t *pd = &SLQSSet3gppConfigTestCases[tc];

            fprintf( fp, "\n\nTest Case %d / %d : %s\n",
                     (tc + 1),
                     ncases,
                     pd->desc );

            fprintf( fp, "Parameter Set:\n" );
            IFPRINTF( fp, "LTEAttachProfile : %d\n", pd->pLTEAttachProfile );
            if( pd->pProfileList )
            {
                for( counter = 0; counter <= 4; counter++ )
                {
                    fprintf( fp, "pProfileList[%d] : %d\n",
                             counter,
                             pd->pProfileList[counter] );
                }
            }
            else
            {
                fprintf( fp, "pProfileList : %d\n", 0x0);
            }
            IFPRINTF( fp, "Default PDN enabled : %d\n", pd->pDefaultPDNEnabled );
            IFPRINTF( fp, "3gppRelease : %d\n",         pd->p3gppRelease );

            swi3gppConfigItem.pLTEAttachProfile  = pd->pLTEAttachProfile;
            swi3gppConfigItem.pProfileList       = pd->pProfileList;
            swi3gppConfigItem.pDefaultPDNEnabled = pd->pDefaultPDNEnabled;
            swi3gppConfigItem.p3gppRelease       = pd->p3gppRelease;

            nRet = SLQSSet3GPPConfigItem( &swi3gppConfigItem );

            /* Display result code and text */
            doprintreason( fp, nRet );
            tc++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doSLQSGet3GPPConfigItem
 *
 * Purpose:  Perform the tests that call the SLQSGet3GPPConfigItem() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSGet3GPPConfigItem( void )
{
    ULONG              nRet;
    FILE               *fp = NULL;
    slqs3GPPConfigItem swi3gppConfigItem;
    BYTE               Counter;

    /* Define the test cases to be executed */
    SLQSSetGet3gppConfigItemTestCase_t SLQSGet3gppConfigTestCases[] =
    {
        { eQCWWAN_ERR_INVALID_ARG, 1, NULL, NULL, NULL, NULL,
          "Invalid Test Case - All params NULL"},

        { eQCWWAN_ERR_NONE, 2, &LTEAttachProfile, ProfileList, NULL,
          &Release3GPP, "Valid test Case - One Param NULL" },

        { eQCWWAN_ERR_NONE, 3, &LTEAttachProfile, ProfileList,
          &defaultPDNEnabled, &Release3GPP, "Valid test Case" },
    };

    fp = tfopen( "../../wds/test/results/slqsget3gppconfigitem.txt", "w" );
    if ( fp )
    {
        BYTE tCaseNum     = 0;
        BYTE ncases = sizeof( SLQSGet3gppConfigTestCases ) /
                      sizeof( SLQSGet3gppConfigTestCases[0] );

        while( tCaseNum < ncases )
        {
            SLQSSetGet3gppConfigItemTestCase_t *pd =
                &SLQSGet3gppConfigTestCases[tCaseNum];

            swi3gppConfigItem.pLTEAttachProfile  = pd->pLTEAttachProfile;
            swi3gppConfigItem.pProfileList       = pd->pProfileList;
            swi3gppConfigItem.pDefaultPDNEnabled = pd->pDefaultPDNEnabled;
            swi3gppConfigItem.p3gppRelease       = pd->p3gppRelease;

            fprintf( fp,"\n\nTest Case %d / %d : %s\n",
                     (tCaseNum + 1),
                     ncases,
                     pd->desc );

            nRet = SLQSGet3GPPConfigItem( &swi3gppConfigItem );

            /* Display result code and text */
            doprintreason( fp, nRet );

            if (!nRet)
            {
                fprintf( fp,"Details for ConfigItem \n" );

                IFPRINTF( fp, "\nLTEAttachProfile : %d",
                          swi3gppConfigItem.pLTEAttachProfile );
                if( swi3gppConfigItem.pProfileList )
                {
                    for( Counter = 0; Counter <= 4; Counter++ )
                    {
                        fprintf( fp,"\npProfileList[%d] : %d",
                                 Counter,
                                 swi3gppConfigItem.pProfileList[Counter] );
                    }
                }
                else
                {
                    fprintf( fp,"\npProfileList : %d", 0x0 );
                }
                IFPRINTF( fp, "\nDefault PDN enabled : %d",
                          swi3gppConfigItem.pDefaultPDNEnabled );
                IFPRINTF( fp, "\n3gppRelease : %d",
                          swi3gppConfigItem.p3gppRelease );
            }
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doSLQSGetProfileSettings3GPP
 *
 * Purpose:  Perform the tests that call the SLQSGetProfileSettings() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSGetProfileSettings3GPP( void )
{
    ULONG                nRet;
    FILE                 *fp = NULL;
    GetProfileSettingIn  ProfileSettingReq;
    GetProfileSettingOut ProfileSetting;
    struct Profile3GPP   *pProfile;
    WORD                 SourceIPTFTID1[8];
    WORD                 SourceIPTFTID2[8];
    BYTE                 idx                = 0;

    /* Define the test cases to be executed */
    SLQSGetProfileSettings3GPPTestCase_t SLQSGetProfileSettings3GPPTestCases[] =
    {
        { eQCWWAN_ERR_INVALID_ARG, 1, -1, -1, NULL, NULL, NULL, NULL, NULL, NULL,
          NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
          NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
          NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, "Invalid Test Case - Invalid params" },

        { eQCWWAN_ERR_NONE, 2, profType3GPP, profIndex, NULL, NULL, NULL, NULL, NULL, NULL,
          NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
          NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
          NULL, NULL, NULL, NULL, NULL, NULL, NULL, "Valid test Case - No out parameter" },

        { eQCWWAN_ERR_NONE, 3, profType3GPP, profIndex, profName, &InValprofNameSize, &PdpType, &PDPHDRCompType,
          &PDPDataCompType, ApnName, &ValApnNameSize, &PriDNSIPv4AddPref, &SecDNSIPv4AddPref, &UMTSReqQoS, &UMTSMinQos,
          &GPRSReqQoS, &GPRSminQoS, UserName, &ValUserNameSize, Passwd, &ValPasswdSize, &AuthPref, &IPv4AddPref, &PCSCFAddPCO,
          &PDPAccCtrlFlag, &PCSCFAddDHCP, &IMCNFlag, &TFTID1, &TFTID2, &PDPCon, &SecFlag, &priID,
          IPv6AddrPref, &UMTSReqQoSsigInd, &UMTSMinQoSsigInd, priDNSIPv6AddPref, secDNSIPv6AddPref,
          &addrAllocPref, &QoSCLSID, &ApnDisFlag, &PDNInactivTim, &APNClass, &extErrCode,
          "InValid test Case - All params specified with invalid profile name size" },

        { eQCWWAN_ERR_NONE, 4, profType3GPP, profIndex, profName, &ValprofNameSize, &PdpType, &PDPHDRCompType,
          &PDPDataCompType, ApnName, &InValApnNameSize, &PriDNSIPv4AddPref, &SecDNSIPv4AddPref, &UMTSReqQoS, &UMTSMinQos,
          &GPRSReqQoS, &GPRSminQoS, UserName, &ValUserNameSize, Passwd, &ValPasswdSize, &AuthPref, &IPv4AddPref, &PCSCFAddPCO,
          &PDPAccCtrlFlag, &PCSCFAddDHCP, &IMCNFlag, &TFTID1, &TFTID2, &PDPCon, &SecFlag, &priID,
          IPv6AddrPref, &UMTSReqQoSsigInd, &UMTSMinQoSsigInd, priDNSIPv6AddPref, secDNSIPv6AddPref,
          &addrAllocPref, &QoSCLSID, &ApnDisFlag, &PDNInactivTim, &APNClass, &extErrCode,
          "InValid test Case - All params specified with invalid APN name size" },

        { eQCWWAN_ERR_NONE, 5, profType3GPP, profIndex, profName, &ValprofNameSize, &PdpType, &PDPHDRCompType,
          &PDPDataCompType, ApnName, &ValApnNameSize, &PriDNSIPv4AddPref, &SecDNSIPv4AddPref, &UMTSReqQoS, &UMTSMinQos,
          &GPRSReqQoS, &GPRSminQoS, UserName, &InValUserNameSize, Passwd, &ValPasswdSize, &AuthPref, &IPv4AddPref, &PCSCFAddPCO,
          &PDPAccCtrlFlag, &PCSCFAddDHCP, &IMCNFlag, &TFTID1, &TFTID2, &PDPCon, &SecFlag, &priID,
          IPv6AddrPref, &UMTSReqQoSsigInd, &UMTSMinQoSsigInd, priDNSIPv6AddPref, secDNSIPv6AddPref,
          &addrAllocPref, &QoSCLSID, &ApnDisFlag, &PDNInactivTim, &APNClass, &extErrCode,
          "InValid test Case - All params specified with invalid user name size" },

        { eQCWWAN_ERR_NONE, 6, profType3GPP, profIndex, profName, &ValprofNameSize, &PdpType, &PDPHDRCompType,
          &PDPDataCompType, ApnName, &ValApnNameSize, &PriDNSIPv4AddPref, &SecDNSIPv4AddPref, &UMTSReqQoS, &UMTSMinQos,
          &GPRSReqQoS, &GPRSminQoS, UserName, &ValUserNameSize, Passwd, &InValPasswdSize, &AuthPref, &IPv4AddPref, &PCSCFAddPCO,
          &PDPAccCtrlFlag, &PCSCFAddDHCP, &IMCNFlag, &TFTID1, &TFTID2, &PDPCon, &SecFlag, &priID,
          IPv6AddrPref, &UMTSReqQoSsigInd, &UMTSMinQoSsigInd, priDNSIPv6AddPref, secDNSIPv6AddPref,
          &addrAllocPref, &QoSCLSID, &ApnDisFlag, &PDNInactivTim, &APNClass, &extErrCode,
          "InValid test Case - All params specified with invalid APN name size" },

          { eQCWWAN_ERR_NONE, 7, profType3GPP, profIndex, profName, &ValprofNameSize, &PdpType, &PDPHDRCompType,
          &PDPDataCompType, ApnName, &ValApnNameSize, &PriDNSIPv4AddPref, &SecDNSIPv4AddPref, &UMTSReqQoS, &UMTSMinQos,
          &GPRSReqQoS, &GPRSminQoS, UserName, &ValUserNameSize, Passwd, &ValPasswdSize, &AuthPref, &IPv4AddPref, &PCSCFAddPCO,
          &PDPAccCtrlFlag, &PCSCFAddDHCP, &IMCNFlag, &TFTID1, &TFTID2, &PDPCon, &SecFlag, &priID,
          IPv6AddrPref, &UMTSReqQoSsigInd, &UMTSMinQoSsigInd, priDNSIPv6AddPref, secDNSIPv6AddPref,
          &addrAllocPref, &QoSCLSID, &ApnDisFlag, &PDNInactivTim, &APNClass, &extErrCode,
          "Valid test Case - All params specified with valid size" },
    };

    fp = tfopen( "../../wds/test/results/slqsgetprofilesettings3gpp.txt", "w" );
    if ( fp )
    {
        BYTE tCaseNum = 0;
        BYTE ncases   = sizeof( SLQSGetProfileSettings3GPPTestCases ) /
                        sizeof( SLQSGetProfileSettings3GPPTestCases[0] );

        while( tCaseNum < ncases )
        {
            SLQSGetProfileSettings3GPPTestCase_t *pd =
                &SLQSGetProfileSettings3GPPTestCases[tCaseNum];

            pProfile = &(ProfileSetting.curProfile.SlqsProfile3GPP);

            ProfileSettingReq.ProfileType = pd->ProfileType;
            ProfileSettingReq.ProfileID   = pd->ProfileID;

            pProfile->pProfilename        = pd->pProfilename;
            pProfile->pProfilenameSize    = pd->pProfileNameSize;
            pProfile->pPDPtype            = pd->pPDPtype;
            pProfile->pPdpHdrCompType     = pd->pPdpHdrCompType;
            pProfile->pPdpDataCompType    = pd->pPdpDataCompType;
            pProfile->pAPNName            = pd->pAPNName;
            pProfile->pAPNnameSize        = pd->pAPNNameSize;
            pProfile->pPriDNSIPv4AddPref  = pd->pPriDNSIPv4AddPref;
            pProfile->pSecDNSIPv4AddPref  = pd->pSecDNSIPv4AddPref;
            pProfile->pUMTSReqQoS         = pd->pUMTSReqQoS;
            pProfile->pUMTSMinQoS         = pd->pUMTSMinQoS;
            pProfile->pGPRSRequestedQos   = pd->pGPRSRequestedQos;
            pProfile->pGPRSMinimumQoS     = pd->pGPRSMinimumQoS;
            pProfile->pUsername           = pd->pUsername;
            pProfile->pUsernameSize       = pd->pUserNameSize;
            pProfile->pPassword           = pd->pPassword;
            pProfile->pPasswordSize       = pd->pPasswordSize;
            pProfile->pAuthenticationPref = pd->pAuthenticationPref;
            pProfile->pIPv4AddrPref       = pd->pIPv4AddrPref;
            pProfile->pPcscfAddrUsingPCO  = pd->pPcscfAddrUsingPCO;
            pProfile->pPdpAccessConFlag   = pd->pPdpAccessConFlag;
            pProfile->pPcscfAddrUsingDhcp = pd->pPcscfAddrUsingDhcp;
            pProfile->pImCnFlag           = pd->pImCnFlag;
            pProfile->pTFTID1Params       = pd->pTFTID1Params;
            pProfile->pTFTID2Params       = pd->pTFTID2Params;
            pProfile->pPdpContext         = pd->pPdpContext;
            pProfile->pSecondaryFlag      = pd->pSecondaryFlag;
            pProfile->pPrimaryID          = pd->pPrimaryID;
            pProfile->pIPv6AddPref        = pd->pIPv6AddPref;
            pProfile->pUMTSReqQoSSigInd   = pd->pUMTSReqQoSSigInd;
            pProfile->pUMTSMinQosSigInd   = pd->pUMTSMinQosSigInd;
            pProfile->pPriDNSIPv6addpref  = pd->pPriDNSIPv6addpref;
            pProfile->pSecDNSIPv6addpref  = pd->pSecDNSIPv6addpref;
            pProfile->pAddrAllocPref      = pd->pAddrAllocPref;
            pProfile->pQosClassID         = pd->pQosClassID;
            pProfile->pAPNDisabledFlag    = pd->pAPNDisabledFlag;
            pProfile->pPDNInactivTimeout  = pd->pPDNInactivTimeout;
            pProfile->pAPNClass           = pd->pAPNClass;

            ProfileSetting.pExtErrCode        = pd->pExtErrCode;
            if( pProfile->pTFTID1Params )
            {
                pProfile->pTFTID1Params->pSourceIP = SourceIPTFTID1;
            }
            if( pProfile->pTFTID2Params )
            {
                pProfile->pTFTID2Params->pSourceIP = SourceIPTFTID2;
            }

            fprintf( fp,"\n\nTest Case %d / %d : %s\n",
                     (tCaseNum + 1),
                     ncases,
                     pd->desc );

            fprintf( fp, "Parameter Set:\n" );
            fprintf( fp, "Profile Type : %d\n", pd->ProfileType );
            fprintf( fp, "Profile ID : %d\n",   pd->ProfileID );
            nRet = SLQSGetProfileSettings( &ProfileSettingReq,
                                           &ProfileSetting );

            /* Display result code and text */
            doprintreason( fp, nRet );
            if (!nRet)
            {
                fprintf( fp,"Details for Profile \n" );

                if( pProfile->pProfilename )
                {
                    fprintf( fp, "\nProfile Name                 : %s",      pProfile->pProfilename );
                }
                IFPRINTF( fp, "\nProfile Name Size               : 0x%x",    pProfile->pProfilenameSize );
                IFPRINTF( fp, "\nPDP Type                        : 0x%x",    pProfile->pPDPtype );
                IFPRINTF( fp, "\nPDP Header Compression Type     : 0x%X",    pProfile->pPdpHdrCompType );
                IFPRINTF( fp, "\nPDP data Compression Type       : 0x%X",    pProfile->pPdpDataCompType );
                if( pProfile->pAPNName )
                {
                    fprintf( fp, "\nAPN Name                     : %s",      pProfile->pAPNName );
                }
                IFPRINTF( fp, "\nAPN Name Size                   : 0x%x",    pProfile->pAPNnameSize );
                IFPRINTF( fp, "\nPrimary DNS IPv4 address Pref.  : 0x%lX",   pProfile->pPriDNSIPv4AddPref );
                IFPRINTF( fp, "\nSecondary DNS IPv4 address Pref.: 0x%lX",   pProfile->pSecDNSIPv4AddPref );

                if( pProfile->pUMTSReqQoS )
                {
                    struct UMTSQoS *pUMTSQoS = pProfile->pUMTSReqQoS;

                    fprintf( fp,"\n\n UMTS Requested QoS" );
                    fprintf( fp, "\nTraffic Class                : 0x%X",    pUMTSQoS->trafficClass );
                    fprintf( fp, "\nMaximum Uplink Bit Rate      : 0x%lX",   pUMTSQoS->maxUplinkBitrate );
                    fprintf( fp, "\nMaximum Downlink Bit Rate    : 0x%lX",   pUMTSQoS->maxDownlinkBitrate );
                    fprintf( fp, "\nGuaranteed Uplink Bit Rate   : 0x%lX",   pUMTSQoS->grntUplinkBitrate );
                    fprintf( fp, "\nGuaranteed Downlink Bit Rate : 0x%lX",   pUMTSQoS->grntDownlinkBitrate );
                    fprintf( fp, "\nQoS Delivery Order           : 0x%X",    pUMTSQoS->qosDeliveryOrder );
                    fprintf( fp, "\nMaximum SDU Size             : 0x%lX",   pUMTSQoS->maxSDUSize );
                    fprintf( fp, "\nSDU Error Ratio              : 0x%X",    pUMTSQoS->sduErrorRatio );
                    fprintf( fp, "\nResidual Bit Error Ratio     : 0x%X",    pUMTSQoS->resBerRatio );
                    fprintf( fp, "\nDelivery Erroneous SDU's     : 0x%X",    pUMTSQoS->deliveryErrSDU );
                    fprintf( fp, "\nTransfer Delay               : 0x%lX",   pUMTSQoS->transferDelay );
                    fprintf( fp, "\nTraffic Handling Priority    : 0x%lX\n", pUMTSQoS->trafficPriority );
                }

                if( pProfile->pUMTSMinQoS )
                {
                    struct UMTSQoS *pUMTSQoS = pProfile->pUMTSMinQoS;

                    fprintf( fp,"\n\n UMTS Minimum QoS" );
                    fprintf( fp, "\nTraffic Class                : 0x%X",    pUMTSQoS->trafficClass );
                    fprintf( fp, "\nMaximum Uplink Bit Rate      : 0x%lX",   pUMTSQoS->maxUplinkBitrate );
                    fprintf( fp, "\nMaximum Downlink Bit Rate    : 0x%lX",   pUMTSQoS->maxDownlinkBitrate );
                    fprintf( fp, "\nGuaranteed Uplink Bit Rate   : 0x%lX",   pUMTSQoS->grntUplinkBitrate );
                    fprintf( fp, "\nGuaranteed Downlink Bit Rate : 0x%lX",   pUMTSQoS->grntDownlinkBitrate );
                    fprintf( fp, "\nQoS Delivery Order           : 0x%X",    pUMTSQoS->qosDeliveryOrder );
                    fprintf( fp, "\nMaximum SDU Size             : 0x%lX",   pUMTSQoS->maxSDUSize );
                    fprintf( fp, "\nSDU Error Ratio              : 0x%X",    pUMTSQoS->sduErrorRatio );
                    fprintf( fp, "\nResidual Bit Error Ratio     : 0x%X",    pUMTSQoS->resBerRatio );
                    fprintf( fp, "\nDelivery Erroneous SDU's     : 0x%X",    pUMTSQoS->deliveryErrSDU );
                    fprintf( fp, "\nTransfer Delay               : 0x%lX",   pUMTSQoS->transferDelay );
                    fprintf( fp, "\nTraffic Handling Priority    : 0x%lX\n", pUMTSQoS->trafficPriority );
                }

                if( pProfile->pGPRSRequestedQos )
                {
                    struct GPRSRequestedQoS *pGPRSQoS = pProfile->pGPRSRequestedQos;

                    fprintf( fp,"\n\n GPRS Requested QoS" );
                    fprintf( fp, "\nPrecedence Class             : 0x%lX",   pGPRSQoS->precedenceClass );
                    fprintf( fp, "\nDelay Class                  : 0x%lX",   pGPRSQoS->delayClass );
                    fprintf( fp, "\nReliability Class            : 0x%lX",   pGPRSQoS->reliabilityClass );
                    fprintf( fp, "\nPeak Throughput Class        : 0x%lX",   pGPRSQoS->peakThroughputClass );
                    fprintf( fp, "\nMean Throughput Class        : 0x%lX\n", pGPRSQoS->meanThroughputClass );
                }

                if( pProfile->pGPRSMinimumQoS )
                {
                    struct GPRSRequestedQoS *pGPRSQoS = pProfile->pGPRSMinimumQoS;

                    fprintf( fp,"\n\n GPRS Minimum QoS" );
                    fprintf( fp, "\nPrecedence Class             : 0x%lX",   pGPRSQoS->precedenceClass );
                    fprintf( fp, "\nDelay Class                  : 0x%lX",   pGPRSQoS->delayClass );
                    fprintf( fp, "\nReliability Class            : 0x%lX",   pGPRSQoS->reliabilityClass );
                    fprintf( fp, "\nPeak Throughput Class        : 0x%lX",   pGPRSQoS->peakThroughputClass );
                    fprintf( fp, "\nMean Throughput Class        : 0x%lX\n", pGPRSQoS->meanThroughputClass );
                }

                if( pProfile->pUsername )
                {
                    fprintf( fp, "\nUser Name                    : %s",      pProfile->pUsername );
                }
                IFPRINTF( fp, "\nUser Name Size                  : 0x%x",    pProfile->pUsernameSize );
                if( pProfile->pPassword )
                {
                    VFPRINTF( fp, "\nPassword                    : %s",      pProfile->pPassword );
                }
                IFPRINTF( fp, "\nPassword Size                   : 0x%x",    pProfile->pPasswordSize );
                IFPRINTF( fp, "\nAuthentication Preference       : 0x%X",    pProfile->pAuthenticationPref );
                IFPRINTF( fp, "\nIPv4 Address Preference         : 0x%lX",   pProfile->pIPv4AddrPref );
                IFPRINTF( fp, "\nPCSCF Address using PCO         : 0x%X",    pProfile->pPcscfAddrUsingPCO );
                IFPRINTF( fp, "\nPDP Access Control Flag         : 0x%X",    pProfile->pPdpAccessConFlag );
                IFPRINTF( fp, "\nPCSCF Address using DHCP        : 0x%X",    pProfile->pPcscfAddrUsingDhcp );
                IFPRINTF( fp, "\nIM CN Flag                      : 0x%X",    pProfile->pImCnFlag );

                if( pProfile->pTFTID1Params )
                {
                    struct TFTIDParams *pTFTIDParams = pProfile->pTFTID1Params;

                    fprintf( fp, "\n\n Traffic Flow Template ID1 Parameters" );
                    fprintf( fp, "\nFilter ID                    : 0x%X",    pTFTIDParams->filterId );
                    fprintf( fp, "\nEvaluation Precedence ID     : 0x%X",    pTFTIDParams->eValid );
                    fprintf( fp, "\nIP Version                   : 0x%X",    pTFTIDParams->ipVersion );

                    fprintf( fp, "\nSource IP                   : ");
                    if( IPv6 == pTFTIDParams->ipVersion )
                    {
                        for( idx = 0; idx < 8; idx++ )
                        {
                            fprintf( fp, "%X", pTFTIDParams->pSourceIP[idx] );
                            if (7 != idx)
                            {
                                fprintf( fp, ":" );
                            }
                        }
                    }
                    else
                    {
                        fprintf( fp, "0x");
                        for( idx = 0; idx < 2; idx++ )
                        {
                            fprintf( fp, "%X", pTFTIDParams->pSourceIP[idx] );
                        }
                    }
                    fprintf( fp, "\nSource IP mask               : 0x%X",    pTFTIDParams->sourceIPMask );
                    fprintf( fp, "\nNext Header                  : 0x%X",    pTFTIDParams->nextHeader );
                    fprintf( fp, "\nDestination Port Range Start : 0x%X",    pTFTIDParams->destPortRangeStart );
                    fprintf( fp, "\nDestination Port Range End   : 0x%X",    pTFTIDParams->destPortRangeEnd );
                    fprintf( fp, "\nSource Port Range Start      : 0x%X",    pTFTIDParams->srcPortRangeStart );
                    fprintf( fp, "\nSource Port Range End        : 0x%X",    pTFTIDParams->srcPortRangeEnd );
                    fprintf( fp, "\nIPSec SPI                    : 0x%lX",   pTFTIDParams->IPSECSPI );
                    fprintf( fp, "\nTOS Mask                     : 0x%X",    pTFTIDParams->tosMask );
                    fprintf( fp, "\nFlow Label                   : 0x%lX\n", pTFTIDParams->flowLabel );
                }

                if( pProfile->pTFTID2Params )
                {
                    struct TFTIDParams *pTFTIDParams = pProfile->pTFTID2Params;

                    fprintf( fp, "\n\n Traffic Flow Template ID2 Parameters" );
                    fprintf( fp, "\nFilter ID                    : 0x%X",    pTFTIDParams->filterId );
                    fprintf( fp, "\nEvaluation Precedence ID     : 0x%X",    pTFTIDParams->eValid );
                    fprintf( fp, "\nIP Version                   : 0x%X",    pTFTIDParams->ipVersion );

                    fprintf( fp, "\nSource IP                   : ");
                    if( IPv6 == pTFTIDParams->ipVersion )
                    {
                        for( idx = 0; idx < 8; idx++ )
                        {
                            fprintf( fp, "%X", pTFTIDParams->pSourceIP[idx] );
                            if (7 != idx)
                            {
                                fprintf( fp, ":" );
                            }
                        }
                    }
                    else
                    {
                        fprintf( fp, "0x");
                        for( idx = 0; idx < 2; idx++ )
                        {
                            fprintf( fp, "%X", pTFTIDParams->pSourceIP[idx] );
                        }
                    }

                    fprintf( fp, "\nSource IP mask               : 0x%X",    pTFTIDParams->sourceIPMask );
                    fprintf( fp, "\nNext Header                  : 0x%X",    pTFTIDParams->nextHeader );
                    fprintf( fp, "\nDestination Port Range Start : 0x%X",    pTFTIDParams->destPortRangeStart );
                    fprintf( fp, "\nDestination Port Range End   : 0x%X",    pTFTIDParams->destPortRangeEnd );
                    fprintf( fp, "\nSource Port Range Start      : 0x%X",    pTFTIDParams->srcPortRangeStart );
                    fprintf( fp, "\nSource Port Range End        : 0x%X",    pTFTIDParams->srcPortRangeEnd );
                    fprintf( fp, "\nIPSec SPI                    : 0x%lX",   pTFTIDParams->IPSECSPI );
                    fprintf( fp, "\nTOS Mask                     : 0x%X",    pTFTIDParams->tosMask );
                    fprintf( fp, "\nFlow Label                   : 0x%lX\n",   pTFTIDParams->flowLabel );
                }

                IFPRINTF( fp, "\nPDP Context Number              : 0x%X",    pProfile->pPdpContext );
                IFPRINTF( fp, "\nSecondary Flag                  : 0x%X",    pProfile->pSecondaryFlag );
                IFPRINTF( fp, "\nPDP Context Primary ID          : 0x%X",  pProfile->pPrimaryID );
                if( pProfile->pIPv6AddPref )
                {
                    fprintf( fp, "\nIPV6 address Preference: ");
                    for( idx = 0; idx < 8; idx++ )
                    {
                        fprintf( fp, "%X", pProfile->pIPv6AddPref[idx] );
                        if (7 != idx)
                        {
                            fprintf( fp, ":" );
                        }
                    }
                }

                if( pProfile->pUMTSReqQoSSigInd )
                {
                    struct UMTSQoS *pUMTSQoS = &(pProfile->pUMTSReqQoSSigInd->UMTSReqQoS);

                    fprintf( fp, "\n\n UMTS Requested QoS with Signalling Indication" );
                    fprintf( fp, "\nTraffic Class                : 0x%X",    pUMTSQoS->trafficClass );
                    fprintf( fp, "\nMaximum Uplink Bit Rate      : 0x%lX",   pUMTSQoS->maxUplinkBitrate );
                    fprintf( fp, "\nMaximum Downlink Bit Rate    : 0x%lX",   pUMTSQoS->maxDownlinkBitrate );
                    fprintf( fp, "\nGuaranteed Uplink Bit Rate   : 0x%lX",   pUMTSQoS->grntUplinkBitrate );
                    fprintf( fp, "\nGuaranteed Downlink Bit Rate : 0x%lX",   pUMTSQoS->grntDownlinkBitrate );
                    fprintf( fp, "\nQoS Delivery Order           : 0x%X",    pUMTSQoS->qosDeliveryOrder );
                    fprintf( fp, "\nMaximum SDU Size             : 0x%lX",   pUMTSQoS->maxSDUSize );
                    fprintf( fp, "\nSDU Error Ratio              : 0x%X",    pUMTSQoS->sduErrorRatio );
                    fprintf( fp, "\nResidual Bit Error Ratio     : 0x%X",    pUMTSQoS->resBerRatio );
                    fprintf( fp, "\nDelivery Erroneous SDU's     : 0x%X",    pUMTSQoS->deliveryErrSDU );
                    fprintf( fp, "\nTransfer Delay               : 0x%lX",   pUMTSQoS->transferDelay );
                    fprintf( fp, "\nTraffic Handling Priority    : 0x%lX",   pUMTSQoS->trafficPriority );
                    fprintf( fp, "\nSignalling Indication Flag   : 0x%X\n",  pProfile->pUMTSReqQoSSigInd->SigInd );
                }

                if( pProfile->pUMTSMinQosSigInd )
                {
                    struct UMTSQoS *pUMTSQoS = &(pProfile->pUMTSMinQosSigInd->UMTSReqQoS);

                    fprintf( fp,"\n\n UMTS Minimum QoS with Signalling Indication" );
                    fprintf( fp, "\nTraffic Class                : 0x%X",    pUMTSQoS->trafficClass );
                    fprintf( fp, "\nMaximum Uplink Bit Rate      : 0x%lX",   pUMTSQoS->maxUplinkBitrate );
                    fprintf( fp, "\nMaximum Downlink Bit Rate    : 0x%lX",   pUMTSQoS->maxDownlinkBitrate );
                    fprintf( fp, "\nGuaranteed Uplink Bit Rate   : 0x%lX",   pUMTSQoS->grntUplinkBitrate );
                    fprintf( fp, "\nGuaranteed Downlink Bit Rate : 0x%lX",   pUMTSQoS->grntDownlinkBitrate );
                    fprintf( fp, "\nQoS Delivery Order           : 0x%X",    pUMTSQoS->qosDeliveryOrder );
                    fprintf( fp, "\nMaximum SDU Size             : 0x%lX",   pUMTSQoS->maxSDUSize );
                    fprintf( fp, "\nSDU Error Ratio              : 0x%X",    pUMTSQoS->sduErrorRatio );
                    fprintf( fp, "\nResidual Bit Error Ratio     : 0x%X",    pUMTSQoS->resBerRatio );
                    fprintf( fp, "\nDelivery Erroneous SDU's     : 0x%X",    pUMTSQoS->deliveryErrSDU );
                    fprintf( fp, "\nTransfer Delay               : 0x%lX",   pUMTSQoS->transferDelay );
                    fprintf( fp, "\nTraffic Handling Priority    : 0x%lX",   pUMTSQoS->trafficPriority );
                    fprintf( fp, "\nSignalling Indication Flag   : 0x%X\n",  pProfile->pUMTSMinQosSigInd->SigInd );
                }

                if( pProfile->pPriDNSIPv6addpref )
                {
                    fprintf( fp, "\nPrimary DNS IPV6 address Preference: ");
                    for( idx = 0; idx < 8; idx++ )
                    {
                       fprintf( fp, "%X", pProfile->pPriDNSIPv6addpref[idx] );
                       if (7 != idx)
                       {
                           fprintf( fp, ":" );
                       }
                    }
                }

                if( pProfile->pSecDNSIPv6addpref )
                {
                    fprintf( fp, "\nSecondary DNS IPV6 address Preference: ");
                    for( idx = 0; idx < 8; idx++ )
                    {
                        fprintf( fp, "%X", pProfile->pSecDNSIPv6addpref[idx] );
                        if (7 != idx)
                        {
                            fprintf( fp, ":" );
                        }
                    }
                }

                IFPRINTF( fp, "\nAddress Allocation Preference   : 0x%X",    pProfile->pAddrAllocPref );

                if( pProfile->pQosClassID )
                {
                    struct QosClassID *pQosClassID = pProfile->pQosClassID;

                    fprintf( fp, "\n\n 3GPP LTE QoS Parameters" );
                    fprintf( fp, "\nQoS Class identifier         : 0x%X",    pQosClassID->QCI );
                    fprintf( fp, "\nGuaranteed DL Bit Rate       : 0x%lX",   pQosClassID->gDlBitRate );
                    fprintf( fp, "\nMaximum DL Bit Rate          : 0x%lX",   pQosClassID->maxDlBitRate );
                    fprintf( fp, "\nGuaranteed UL Bit Rate       : 0x%lX",   pQosClassID->gUlBitRate );
                    fprintf( fp, "\nMaximum UL Bit Rate          : 0x%lX\n", pQosClassID->maxUlBitRate );
                }
                IFPRINTF( fp, "\nAPN disabled flag               : 0x%X",    pProfile->pAPNDisabledFlag );
                IFPRINTF( fp, "\nPDN Inactivity Timeout          : 0x%lX",   pProfile->pPDNInactivTimeout );
                IFPRINTF( fp, "\nAPN class                       : 0x%X",    pProfile->pAPNClass );
            }
            if( eQCWWAN_ERR_QMI_EXTENDED_INTERNAL == nRet)
            {
                IFPRINTF(fp, "Extended Error Code                : %d\n",    ProfileSetting.pExtErrCode);
            }
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doSLQSGetProfileSettings3GPP2
 *
 * Purpose:  Perform the tests that call the SLQSGetProfileSettings() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSGetProfileSettings3GPP2( void )
{
    ULONG                nRet;
    FILE                 *fp = NULL;
    GetProfileSettingIn  ProfileSettingReq;
    GetProfileSettingOut ProfileSetting;
    struct Profile3GPP2  *pProfile;
    BYTE                 idx                = 0;

    /* Define the test cases to be executed */
    SLQSGetProfileSettings3GPP2TestCase_t SLQSGetProfileSettings3GPP2TestCases[] =
    {
        { eQCWWAN_ERR_INVALID_ARG, 1, -1, -1, NULL, NULL, NULL, NULL, NULL, NULL,
          NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
          NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
          NULL, NULL, "Invalid Test Case - Invalid params" },

        { eQCWWAN_ERR_NONE, 2, profType3GPP2, profIndex3GPP2, NULL, NULL, NULL, NULL,
          NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
          NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
          NULL, NULL, "Valid test Case - No out parameter" },

        { eQCWWAN_ERR_NONE, 3, profType3GPP2, profIndex3GPP2, &NegoDNSServPref, &PPPSessCloTimD0,
          &PPPSessCloTim1X, &AllowLinger, &LcpAckTimeout, &IpcpAckTimeout, &AuthTimeout, &LcpCreqRetCt,
          &IpCpCReqRetCt, &authRetryCt, &authProt, userID, &InValuserIDSize, authPwd, &ValauthPwdSize, &DataRate,
          &AppType, &DataMode, &appPrio, ApnStr, &ValApnStrSize, &PdnType, &isPcscfAdd, &PriDNSIPv4Add, &SecDNSIPv4Add,
          PriDNSIPv6Add, SecDNSIPv6Add, &RatType, &ApnEnable3GPP2, &PDNInactivTim, &APNClass, &extErrCode,
          "InValid test Case - All params specified - Invalid User ID size" },

        { eQCWWAN_ERR_NONE, 4, profType3GPP2, profIndex3GPP2, &NegoDNSServPref, &PPPSessCloTimD0,
          &PPPSessCloTim1X, &AllowLinger, &LcpAckTimeout, &IpcpAckTimeout, &AuthTimeout, &LcpCreqRetCt,
          &IpCpCReqRetCt, &authRetryCt, &authProt, userID, &ValuserIDSize, authPwd, &InValauthPwdSize, &DataRate,
          &AppType, &DataMode, &appPrio, ApnStr, &ValApnStrSize, &PdnType, &isPcscfAdd, &PriDNSIPv4Add, &SecDNSIPv4Add,
          PriDNSIPv6Add, SecDNSIPv6Add, &RatType, &ApnEnable3GPP2, &PDNInactivTim, &APNClass, &extErrCode,
          "InValid test Case - All params specified - Invalid Auth Password size" },

        { eQCWWAN_ERR_NONE, 5, profType3GPP2, profIndex3GPP2, &NegoDNSServPref, &PPPSessCloTimD0,
          &PPPSessCloTim1X, &AllowLinger, &LcpAckTimeout, &IpcpAckTimeout, &AuthTimeout, &LcpCreqRetCt,
          &IpCpCReqRetCt, &authRetryCt, &authProt, userID, &ValuserIDSize, authPwd, &ValauthPwdSize, &DataRate,
          &AppType, &DataMode, &appPrio, ApnStr, &InValApnStrSize, &PdnType, &isPcscfAdd, &PriDNSIPv4Add, &SecDNSIPv4Add,
          PriDNSIPv6Add, SecDNSIPv6Add, &RatType, &ApnEnable3GPP2, &PDNInactivTim, &APNClass, &extErrCode,
          "InValid test Case - All params specified - Invalid APN string size" },

        { eQCWWAN_ERR_NONE, 6, profType3GPP2, profIndex3GPP2, &NegoDNSServPref, &PPPSessCloTimD0,
          &PPPSessCloTim1X, &AllowLinger, &LcpAckTimeout, &IpcpAckTimeout, &AuthTimeout, &LcpCreqRetCt,
          &IpCpCReqRetCt, &authRetryCt, &authProt, userID, &ValuserIDSize, authPwd, &ValauthPwdSize, &DataRate,
          &AppType, &DataMode, &appPrio, ApnStr, &ValApnStrSize, &PdnType, &isPcscfAdd, &PriDNSIPv4Add, &SecDNSIPv4Add,
          PriDNSIPv6Add, SecDNSIPv6Add, &RatType, &ApnEnable3GPP2, &PDNInactivTim, &APNClass, &extErrCode,
          "Valid test Case - All params specified - valid sizes" },

    };

    fp = tfopen( "../../wds/test/results/slqsgetprofilesettings3gpp2.txt", "w" );
    if ( fp )
    {
        BYTE tCaseNum = 0;
        BYTE ncases   = sizeof( SLQSGetProfileSettings3GPP2TestCases ) /
                        sizeof( SLQSGetProfileSettings3GPP2TestCases[0] );

        while( tCaseNum < ncases )
        {
            SLQSGetProfileSettings3GPP2TestCase_t *pd =
                &SLQSGetProfileSettings3GPP2TestCases[tCaseNum];

            pProfile = &(ProfileSetting.curProfile.SlqsProfile3GPP2);

            ProfileSettingReq.ProfileType  = pd->ProfileType;
            ProfileSettingReq.ProfileID    = pd->ProfileID;

            pProfile->pNegoDnsSrvrPref        = pd->pNegoDnsSrvrPref;
            pProfile->pPppSessCloseTimerDO    = pd->pPppSessCloseTimerDO;
            pProfile->pPppSessCloseTimer1x    = pd->pPppSessCloseTimer1x;
            pProfile->pAllowLinger            = pd->pAllowLinger;
            pProfile->pLcpAckTimeout          = pd->pLcpAckTimeout;
            pProfile->pIpcpAckTimeout         = pd->pIpcpAckTimeout;
            pProfile->pAuthTimeout            = pd->pAuthTimeout;
            pProfile->pLcpCreqRetryCount      = pd->pLcpCreqRetryCount;
            pProfile->pIpcpCreqRetryCount     = pd->pIpcpCreqRetryCount;
            pProfile->pAuthRetryCount         = pd->pAuthRetryCount;
            pProfile->pAuthProtocol           = pd->pAuthProtocol;
            pProfile->pUserId                 = pd->pUserId;
            pProfile->pUserIdSize             = pd->pUserIdSize;
            pProfile->pAuthPassword           = pd->pAuthPassword;
            pProfile->pAuthPasswordSize       = pd->pAuthPwdSize;
            pProfile->pDataRate               = pd->pDataRate;
            pProfile->pAppType                = pd->pAppType;
            pProfile->pDataMode               = pd->pDataMode;
            pProfile->pAppPriority            = pd->pAppPriority;
            pProfile->pApnString              = pd->pApnString;
            pProfile->pApnStringSize          = pd->pApnStrSize;
            pProfile->pPdnType                = pd->pPdnType;
            pProfile->pIsPcscfAddressNedded   = pd->pIsPcscfAddressNedded;
            pProfile->pPrimaryV4DnsAddress    = pd->pPrimaryV4DnsAddress;
            pProfile->pSecondaryV4DnsAddress  = pd->pSecondaryV4DnsAddress;
            pProfile->pPriV6DnsAddress        = pd->pPriV6DnsAddress;
            pProfile->pSecV6DnsAddress        = pd->pSecV6DnsAddress;
            pProfile->pRATType                = pd->pRATType;
            pProfile->pAPNEnabled3GPP2        = pd->pAPNEnabled3GPP2;
            pProfile->pPDNInactivTimeout3GPP2 = pd->pPDNInactivTimeout3GPP2;
            pProfile->pAPNClass3GPP2          = pd->pAPNClass3GPP2;

            ProfileSetting.pExtErrCode        = pd->pExtErrCode;

            fprintf( fp,"\n\nTest Case %d / %d : %s\n",
                     (tCaseNum + 1),
                     ncases,
                     pd->desc );

            fprintf( fp, "Parameter Set:\n" );
            fprintf( fp, "Profile Type : %d\n", pd->ProfileType );
            fprintf( fp, "Profile ID : %d\n",   pd->ProfileID );
            nRet = SLQSGetProfileSettings( &ProfileSettingReq,
                                           &ProfileSetting );

            /* Display result code and text */
            doprintreason( fp, nRet );
            if (!nRet)
            {
                fprintf( fp,"Details for Profile \n" );

                IFPRINTF( fp, "\nNegotiate DNS Server Preference : 0x%X",  pProfile->pNegoDnsSrvrPref );
                IFPRINTF( fp, "\nPPP Session close Timer for DO  : 0x%lX", pProfile->pPppSessCloseTimerDO );
                IFPRINTF( fp, "\nPPP Session close Timer for 1X  : 0x%lX", pProfile->pPppSessCloseTimer1x );
                IFPRINTF( fp, "\nAllow Linger                    : 0x%X",  pProfile->pAllowLinger );
                IFPRINTF( fp, "\nLCP ACK Timeout                 : 0x%X",  pProfile->pLcpAckTimeout );
                IFPRINTF( fp, "\nIPCP ACK Timeout                : 0x%X",  pProfile->pIpcpAckTimeout );
                IFPRINTF( fp, "\nAuth Timeout                    : 0x%X",  pProfile->pAuthTimeout );
                IFPRINTF( fp, "\nLCP Config. Request Retry Count : 0x%X",  pProfile->pLcpCreqRetryCount );
                IFPRINTF( fp, "\nIPCP Config. Request Retry Count: 0x%X",  pProfile->pIpcpCreqRetryCount );
                IFPRINTF( fp, "\nAuthentication Retry Count      : 0x%X",  pProfile->pAuthRetryCount );
                IFPRINTF( fp, "\nAuthentication Protocol         : 0x%X",  pProfile->pAuthProtocol );
                if( pProfile->pUserId )
                {
                    fprintf( fp, "\nUser ID                      : %s",    pProfile->pUserId );
                }
                IFPRINTF( fp, "\nUser ID Size                    : 0x%X",  pProfile->pUserIdSize );
                if( pProfile->pAuthPassword )
                {
                    fprintf( fp, "\nAuthentication Password      : %s",    pProfile->pAuthPassword );
                }
                IFPRINTF( fp, "\nAuthentication Password Size    : 0x%X",  pProfile->pAuthPasswordSize );
                IFPRINTF( fp, "\nData Rate                       : 0x%X",  pProfile->pDataRate );
                IFPRINTF( fp, "\nApplication Type                : 0x%lX", pProfile->pAppType );
                IFPRINTF( fp, "\nData Mode                       : 0x%X",  pProfile->pDataMode );
                IFPRINTF( fp, "\nApplication Priority            : 0x%X",  pProfile->pAppPriority );
                if( pProfile->pApnString )
                {
                    fprintf( fp, "\nAPN String                   : %s",    pProfile->pApnString );
                }
                IFPRINTF( fp, "\nAPN String Size                 : 0x%X",  pProfile->pApnStringSize );
                IFPRINTF( fp, "\nPDN Type                        : 0x%X",  pProfile->pPdnType );
                IFPRINTF( fp, "\nIs Pcscf Address Needed         : 0x%X",  pProfile->pIsPcscfAddressNedded );
                IFPRINTF( fp, "\nIPv4 Primary DNS Address        : 0x%lX", pProfile->pPrimaryV4DnsAddress );
                IFPRINTF( fp, "\nIPv4 Secondary DNS Address      : 0x%lX", pProfile->pSecondaryV4DnsAddress );
                if( pProfile->pPriV6DnsAddress )
                {
                    fprintf( fp, "\nPrimary DNS IPV6 address     : ");
                    for( idx = 0; idx < 8; idx++ )
                    {
                       fprintf( fp, "%X", pProfile->pPriV6DnsAddress[idx] );
                       if (7 != idx)
                       {
                           fprintf( fp, ":" );
                       }
                    }
                }

                if( pProfile->pSecV6DnsAddress )
                {
                    fprintf( fp, "\nSecondary DNS IPV6 address   : ");
                    for( idx = 0; idx < 8; idx++ )
                    {
                        fprintf( fp, "%X", pProfile->pSecV6DnsAddress[idx] );
                        if (7 != idx)
                        {
                            fprintf( fp, ":" );
                        }
                    }
                }
                IFPRINTF( fp, "\nRAT Type                        : 0x%X",  pProfile->pRATType );
                IFPRINTF( fp, "\nAPN Enabled                     : 0x%X",  pProfile->pAPNEnabled3GPP2 );
                IFPRINTF( fp, "\nPDN Inactivity Timeout          : 0x%lX", pProfile->pPDNInactivTimeout3GPP2 );
                IFPRINTF( fp, "\nAPN class                       : 0x%X",  pProfile->pAPNClass3GPP2 );

            }
            if( eQCWWAN_ERR_QMI_EXTENDED_INTERNAL == nRet)
            {
                IFPRINTF(fp, "Extended Error Code: %d\n", ProfileSetting.pExtErrCode);
            }
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doSLQSGetProfileSettings
 *
 * Purpose:  Executes doSLQSGetProfileSettings3GPP and doSLQSGetProfileSettings3GPP2
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSGetProfileSettings( void )
{
    doSLQSGetProfileSettings3GPP();
    doSLQSGetProfileSettings3GPP2();
}

/*
 * Name:     doSetMobileIPProfile
 *
 * Purpose:  Perform the tests that call the SetMobileIPProfile() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSetMobileIPProfile( void )
{
    ULONG nRet;
    FILE  *fp = NULL;
    WORD  lCount;
    CHAR  *pSPC = "577551";
    BYTE  index = 1;
    BYTE  Enabled = 1;
    ULONG Address = 0xFFFFFFFF;
    ULONG PrimaryHA  = 0xFFFFFFFF;
    ULONG SecondaryHA = 0xFFFFFFFF;
    BYTE  RevTunneling = 1;
    CHAR  *pNAI = "www.sierrawireless.com";
    ULONG HASPI = 0xFFFFFFFF;
    ULONG AAASPI = 0xFFFFFFFF;;
    CHAR  *pMNHA = "test";
    CHAR  *pMNAAA = "test";

    /* Define the test cases to be executed */
    SetMobileIPProfileTestCaseStep_t SetMobileIPProfileTestCases[] =
    {
        { eQCWWAN_ERR_INVALID_ARG, 1, NULL, index, NULL, NULL, NULL, NULL, NULL,
          NULL, NULL, NULL, NULL, NULL, "Invalid Test Case - All params NULL"},

        { eQCWWAN_ERR_INVALID_ARG, 1, pSPC, index, &Enabled, &Address, &PrimaryHA, &SecondaryHA, &RevTunneling,
           pNAI, &HASPI, &AAASPI, pMNHA, pMNAAA, "Valid Test Case"},
    };

    fp = tfopen( "TestReport/setmobileIPprofile.txt", "w" );
    if ( fp )
    {
        BYTE tCaseNum     = 0;
        BYTE ncases = sizeof( SetMobileIPProfileTestCases ) /
                      sizeof( SetMobileIPProfileTestCases[0] );

        while( tCaseNum < ncases )
        {
            SetMobileIPProfileTestCaseStep_t *pd =
                &SetMobileIPProfileTestCases[tCaseNum];

            fprintf( fp,"\n\nTest Case %d / %d : %s\n",
                     (tCaseNum + 1),
                     ncases,
                     pd->desc );
            fprintf( fp, "SPC : ");
            if (pd->pSPC)
            {
                for ( lCount = 0; pd->pSPC[lCount]!=EOS ; lCount++ )
                {
                    fprintf(fp, "%c",pd->pSPC[lCount]);
                }
            }
            fprintf( fp, "\nIndex : %d", pd->index );
            IFPRINTF( fp, "\nAddress : %lx", pd->pAddress );
            IFPRINTF( fp, "\nPrimary HA : %lx", pd->pPrimaryHA );
            IFPRINTF( fp, "\nSecondary HA : %lx", pd->pSecondaryHA );
            IFPRINTF( fp, "\nRev Tunneling : %d", pd->pRevTunneling );
            fprintf( fp, "\nNAI : ");
            if (pd->pNAI)
            {
                for ( lCount = 0; pd->pNAI[lCount]!=EOS ; lCount++ )
                {
                    fprintf(fp, "%c",pd->pNAI[lCount]);
                }
            }
            IFPRINTF( fp, "\nHASPI : %lx", pd->pHASPI );
            IFPRINTF( fp, "\nAAASPI : %lx", pd->pAAASPI );
            fprintf( fp, "\nMNHA : ");
            if (pd->pMNHA)
            {
                for ( lCount = 0; pd->pMNHA[lCount]!=EOS ; lCount++ )
                {
                    fprintf(fp, "%c",pd->pMNHA[lCount]);
                }
            }
            fprintf( fp, "\nMNAA : ");
            if (pd->pMNAAA)
            {
                for ( lCount = 0; pd->pMNAAA[lCount]!=EOS ; lCount++ )
                {
                    fprintf(fp, "%c",pd->pMNAAA[lCount]);
                }
            }
            fprintf( fp,"\n");

            nRet = SetMobileIPProfile( pd->pSPC,
                                       pd->index,
                                       pd->pEnabled,
                                       pd->pAddress,
                                       pd->pPrimaryHA,
                                       pd->pSecondaryHA,
                                       pd->pRevTunneling,
                                       pd->pNAI,
                                       pd->pHASPI,
                                       pd->pAAASPI,
                                       pd->pMNHA,
                                       pd->pMNAAA );

            /* Display result code and text */
            doprintreason( fp, nRet );
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSWdsSetEventReport
 *
 * Purpose: Perform the tests that call the SLQSWdsSetEventReport() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSWdsSetEventReport( void )
{
    /* Test Cases */
    SLQSWdsSetEventReportTestCase_t SLQSWdsSetEventReportTestCases[]=
    {
        { eQCWWAN_ERR_NONE, 1, &unSetInd, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, "All Params NULL" },

        { eQCWWAN_ERR_NONE, 2, &setInd, &transferStatInd, &setInd, &setInd,
        &setInd, &setInd, &setInd, &setInd, &setInd, &setInd,
        "All Params Set" },
    };

    FILE *fp = tfopen("../../wds/test/results/slqswdsseteventreport.txt", "w");
    wdsSetEventReportReq req;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSWdsSetEventReportTestCases )/
                      sizeof( SLQSWdsSetEventReportTestCases[0] );
        while ( tc < ncases )
        {
            SLQSWdsSetEventReportTestCase_t *pd = &SLQSWdsSetEventReportTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            req.pCurrChannelRateInd        = pd->pCurrChannelRateInd;
            req.pTransferStatInd           = pd->pTransferStatInd;
            req.pDataBearerTechInd         = pd->pDataBearerTechInd;
            req.pDormancyStatusInd         = pd->pDormancyStatusInd;
            req.pMIPStatusInd              = pd->pMIPStatusInd;
            req.pCurrDataBearerTechInd     = pd->pCurrDataBearerTechInd;
            req.pDataCallStatusChangeInd   = pd->pDataCallStatusChangeInd;
            req.pCurrPrefDataSysInd        = pd->pCurrPrefDataSysInd;
            req.pEVDOPageMonPerChangeInd   = pd->pEVDOPageMonPerChangeInd;
            req.pDataSystemStatusChangeInd = pd->pDataSystemStatusChangeInd;

            IFPRINTF( fp, "Curr Channel Rate Ind    : %x\n",req.pCurrChannelRateInd )
            if (req.pTransferStatInd)
            {
                fprintf( fp, "Stats Period : %x\n",req.pTransferStatInd->statsPeriod );
                fprintf( fp, "Stats Mask   : %lx\n",req.pTransferStatInd->statsMask );
            }
            IFPRINTF( fp, "Data Bearer Ind          : %x\n",req.pDataBearerTechInd )
            IFPRINTF( fp, "Dormancy Status Ind      : %x\n",req.pDormancyStatusInd )
            IFPRINTF( fp, "MIP Status Ind           : %x\n",req.pMIPStatusInd )
            IFPRINTF( fp, "Curr Dormancy Status Ind : %x\n",req.pCurrDataBearerTechInd )
            IFPRINTF( fp, "Data Call Status Ind     : %x\n",req.pDataCallStatusChangeInd )
            IFPRINTF( fp, "Curr Pref Data Sys Ind   : %x\n",req.pCurrPrefDataSysInd )
            IFPRINTF( fp, "EVDO Page Monito Ind     : %x\n",req.pEVDOPageMonPerChangeInd )
            IFPRINTF( fp, "Data Sys Status Ind      : %x\n",req.pDataSystemStatusChangeInd )

            ULONG nRet = SLQSWdsSetEventReport( &req );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf( fp, "SLQSWdsSetEventReport Successful\n");
                continue;
            }

            fprintf( fp, "SLQSWdsSetEventReport Unsuccessful\n");
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:     doSLQSWdsSwiPDPRuntimeSettings
 *
 * Purpose:  Perform the tests that call the SLQSWdsSwiPDPRuntimeSettings() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSWdsSwiPDPRuntimeSettings( void )
{
    local BYTE                   contextId;
    local BYTE                   bearerId;
    local CHAR                   APNName[nMaxStrLen];
    local ULONG                  IPv4Address;
    local ULONG                  IPv4GWAddress;
    local ULONG                  PrDNSIPv4Address;
    local ULONG                  SeDNSIPv4Address;
    local struct IPV6AddressInfo IPv6Address;
    local struct IPV6AddressInfo IPv6GWAddress;
    local WORD                   PrDNSIPv6Address[IPV6_ADDRESS_ARRAY_SIZE];
    local WORD                   SeDNSIPv6Address[IPV6_ADDRESS_ARRAY_SIZE];
    local ULONG                  PrPCSCFIPv4Address;
    local ULONG                  SePCSCFIPv4Address;
    local WORD                   PrPCSCFIPv6Address[IPV6_ADDRESS_ARRAY_SIZE];
    local WORD                   SePCSCFIPv6Address[IPV6_ADDRESS_ARRAY_SIZE];

    /* Define the test cases to be executed */
    SLQSWdsSwiPDPRuntimeSettingsTestCase_t SLQSWdsSwiPDPRuntimeSettingsTestCases[] =
    {
        { eQCWWAN_ERR_NONE, 1, 0x01, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        "Valid Test Case - Passing NULL"},

        { eQCWWAN_ERR_NONE, 2, 0x01,
        &contextId,
        &bearerId,
        APNName,
        &IPv4Address,
        &IPv4GWAddress,
        &PrDNSIPv4Address,
        &SeDNSIPv4Address,
        &IPv6Address,
        &IPv6GWAddress,
        PrDNSIPv6Address,
        SeDNSIPv6Address,
        &PrPCSCFIPv4Address,
        &SePCSCFIPv4Address,
        PrPCSCFIPv6Address,
        SePCSCFIPv6Address,
        "Valid Test Case - Fetching All parameters "},

    };

    FILE *fp = tfopen( "../../nas/test/results/slqswdsswipdpruntimesettings.txt", "w" );
    swiPDPRuntimeSettingsReq  req;
    swiPDPRuntimeSettingsResp resp;
    WORD                      iC;

    if ( fp )
    {
        BYTE tC     = 0;
        BYTE ncases = sizeof( SLQSWdsSwiPDPRuntimeSettingsTestCases ) /
                      sizeof( SLQSWdsSwiPDPRuntimeSettingsTestCases[0] );

        while( tC < ncases )
        {
            SLQSWdsSwiPDPRuntimeSettingsTestCase_t *pd =
                       &SLQSWdsSwiPDPRuntimeSettingsTestCases[tC++];

            /* print test data */
            fprintf( fp,"\n\nTest Case %d / %d : %s\n",
                     tC,
                     ncases,
                     pd->desc );

            req.contextId = pd->contextId;
            fprintf( fp,"Context Id : %x \n\n",req.contextId );

            resp.pContextId          = pd->pContextId;
            resp.pBearerId           = pd->pBearerId;
            resp.pAPNName            = pd->pAPNName;
            resp.pIPv4Address        = pd->pIPv4Address;
            resp.pIPv4GWAddress      = pd->pIPv4GWAddress;
            resp.pPrDNSIPv4Address   = pd->pPrDNSIPv4Address;
            resp.pSeDNSIPv4Address   = pd->pSeDNSIPv4Address;
            resp.pIPv6Address        = pd->pIPv6Address;
            resp.pIPv6GWAddress      = pd->pIPv6GWAddress;
            resp.pPrDNSIPv6Address   = pd->pPrDNSIPv6Address;
            resp.pSeDNSIPv6Address   = pd->pSeDNSIPv6Address;
            resp.pPrPCSCFIPv4Address = pd->pPrPCSCFIPv4Address;
            resp.pSePCSCFIPv4Address = pd->pSePCSCFIPv4Address;
            resp.pPrPCSCFIPv6Address = pd->pPrPCSCFIPv6Address;
            resp.pSePCSCFIPv6Address = pd->pSePCSCFIPv6Address;

            ULONG nRet = SLQSWdsSwiPDPRuntimeSettings( &req, &resp );

            /* Display result code and text */
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf( fp,"SLQSWdsSwiPDPRuntimeSettings Successful \n" );

                IFPRINTF( fp, "Context Id      : %x \n",resp.pContextId);
                IFPRINTF( fp, "Bearer Id       : %x \n",resp.pBearerId);
                if ( resp.pAPNName )
                {
                    fprintf( fp, "APN Name        : %s \n",resp.pAPNName);
                }
                IFPRINTF( fp, "IPv4 Address    : %lx \n",resp.pIPv4Address);
                IFPRINTF( fp, "IPv4 GW Address : %lx \n",resp.pIPv4GWAddress);
                IFPRINTF( fp, "Primary DNS IPv4 Address   : %lx \n",resp.pPrDNSIPv4Address);
                IFPRINTF( fp, "Secondary DNS IPv4 Address : %lx \n",resp.pSeDNSIPv4Address);

                if ( resp.pIPv6Address )
                {
                    fprintf( fp,"IPv6Address : ");
                    for ( iC = 0 ; iC < IPV6_ADDRESS_ARRAY_SIZE ; iC++ )
                    {
                        fprintf( fp,"%x ",resp.pIPv6Address->IPAddressV6[iC]);
                    }
                    fprintf( fp,"\n");
                    fprintf( fp,"IPV6PrefixLen : %d \n",resp.pIPv6Address->IPV6PrefixLen );
                }
                if ( resp.pIPv6GWAddress )
                {
                    fprintf( fp,"IPv6GWAddress : ");
                    for ( iC = 0 ; iC < IPV6_ADDRESS_ARRAY_SIZE ; iC++ )
                    {
                        fprintf( fp,"%x ",resp.pIPv6GWAddress->IPAddressV6[iC]);
                    }
                    fprintf( fp,"\n");
                    fprintf( fp,"IPV6GWPrefixLen : %d \n",resp.pIPv6GWAddress->IPV6PrefixLen );
                }
                if ( resp.pPrDNSIPv6Address )
                {
                    fprintf( fp,"Primary DNS IPv6 Address   : ");
                    for ( iC = 0 ; iC < IPV6_ADDRESS_ARRAY_SIZE ; iC++ )
                    {
                        fprintf( fp,"%x ",resp.pPrDNSIPv6Address[iC]);
                    }
                    fprintf( fp,"\n");
                }
                if ( resp.pSeDNSIPv6Address )
                {
                    fprintf( fp,"Secondary DNS IPv6 Address : ");
                    for ( iC = 0 ; iC < IPV6_ADDRESS_ARRAY_SIZE ; iC++ )
                    {
                        fprintf( fp,"%x ",resp.pSeDNSIPv6Address[iC]);
                    }
                    fprintf( fp,"\n");
                }
                IFPRINTF( fp, "Primary PCSCF IPv4 Address   : %lx \n",resp.pPrPCSCFIPv4Address);
                IFPRINTF( fp, "Secondary PCSCF IPv4 Address : %lx \n",resp.pSePCSCFIPv4Address);
                if ( resp.pPrPCSCFIPv6Address )
                {
                    fprintf( fp,"Primary PCSCF IPv6 Address   : ");
                    for ( iC = 0 ; iC < IPV6_ADDRESS_ARRAY_SIZE ; iC++ )
                    {
                        fprintf( fp,"%x ",resp.pPrPCSCFIPv6Address[iC]);
                    }
                    fprintf( fp,"\n");
                }
                if ( resp.pSePCSCFIPv6Address )
                {
                    fprintf( fp,"Secondary PCSCF IPv6 Address : ");
                    for ( iC = 0 ; iC < IPV6_ADDRESS_ARRAY_SIZE ; iC++ )
                    {
                        fprintf( fp,"%x ",resp.pSePCSCFIPv6Address[iC]);
                    }
                    fprintf( fp,"\n");
                }
            }
        }
    }
    if (fp)
        tfclose(fp);
}
