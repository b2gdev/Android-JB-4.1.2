/**************
 *
 *  Filename: qaGobiApiNasTest.c
 *
 *  Purpose:  Contains functions implementing specific tests for
 *            Network Access Service (NAS)
 *            called by the main() routine in qatest.c
 *
 * Copyright: © 2011-2012 Sierra Wireless, Inc., all rights reserved
 *
 **************/

/* Linux definitions */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* include files */
#include "SwiDataTypes.h"
#include "qmerrno.h"

#include "qaGobiApiNas.h"
#include "qaGobiApiNasTest.h"
#include "qatesthelper.h"
#include "qatestproto.h"

/*******************
    TEST DATA
 *******************/

/* Constants */
#define NETREG       0x00
#define NETSELMCC    123
#define NETSELMNC    456

/* Global Variables */
local BYTE      EmerMode    = 0x00;
local WORD      ModePref    = 0x08; //UMTS
local ULONGLONG bandPref    = 33554432; //WCDMA US 1700 Band
local WORD      PRLPref     = 0x3FFF;
local WORD      RoamPref    = 0xFF;
local ULONGLONG LTEBandPref = 0x08;
local BYTE      chgDuration = 0x01;
local BYTE      MNCIncPCS   = 0x01;
local ULONG     srvDomPref  = 0x00;
local ULONG     GWAcqOrder  = 0x00;

local struct netSelectionPref NetSelPref = { NETREG, NETSELMCC, NETSELMNC };

local BYTE      GetEmerMode;
local WORD      GetModePref;
local ULONGLONG GetBandPref;
local WORD      GetPRLPref;
local WORD      GetRoamPref;
local ULONGLONG GetLTEBandPref;
local BYTE      GetNetSelPref;
local ULONG     GetSrvDomPref;
local ULONG     GetGWAcqOrder;

/*  Parameters for SLQSNasIndicationRegisterExt() API */
local BYTE setInd   = 0x01;
local BYTE unsetInd = 0x00;

/*******************
  TEST FUNCTIONS
 *******************/
/*
 * Name:     doprintsysInfoCommon
 *
 * Purpose:  This is a wrapper for printing System Information that is common
 *
 * Parms:    fp          - pointer to FILE
 *           pSys        - pointer to sysInfoCommon
 *
 * Return:   None
 *
 * Notes:    None.
 *
 */
void doprintsysInfoCommon( FILE *fp, sysInfoCommon *pSys )
{
    FILE *fpTmp;
    fpTmp = fp;
    fprintf( fpTmp,"Service Domain Valid   : %x\n",pSys->srvDomainValid );
    fprintf( fpTmp,"Service Domain         : %x\n",pSys->srvDomain );
    fprintf( fpTmp,"Service Capability Valid: %x\n",pSys->srvCapabilityValid );
    fprintf( fpTmp,"Service Capability     : %x\n",pSys->srvCapability );
    fprintf( fpTmp,"Roam Status Valid      : %x\n",pSys->roamStatusValid );
    fprintf( fpTmp,"Roam Status            : %x\n",pSys->roamStatus );
    fprintf( fpTmp,"Forbidden System Valid : %x\n",pSys->isSysForbiddenValid );
    fprintf( fpTmp,"Is Forbidden System    : %x\n",pSys->isSysForbiddenValid );
}

/*
 * Name:     doGetHomeNetwork
 *
 * Purpose:  Perform the tests that call the GetHomeNetwork() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */

void doGetHomeNetwork( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    WORD  pMCC;
    WORD  pMNC;
    BYTE  nameSize;
    CHAR  pName[nMaxStrLen];
    WORD  pSID;
    WORD  pNID;
    FILE  *fp = NULL;

    fp = tfopen("../../nas/test/results/gethomenetwork.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_GET_HOME_NETWORK_TESTCASE_NUM)
        {
            nameSize    = GetHomeNetworkTestCases[tCaseNum].nameSize;

            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_GET_HOME_NETWORK_TESTCASE_NUM,
                       GetHomeNetworkTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "nameSize :%d\n", nameSize);

            nRet = GetHomeNetwork(&pMCC,
                                  &pMNC,
                                  nameSize,
                                  &pName[0],
                                  &pSID,
                                  &pNID);

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "pMCC  : %d\n", pMCC);
                fprintf(fp, "pMNC  : %d\n", pMNC);
                fprintf(fp, "pName : %s\n", pName);
                fprintf(fp, "pSID  : %d\n", pSID);
                fprintf(fp, "pNID  : %d\n", pNID);
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetServingSystem
 *
 * Purpose:  Perform the tests that call the GetServingSystem() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetServingSystem( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    ULONG pRegistrationState;
    ULONG pCSDomain;
    ULONG pPSDomain;
    ULONG pRAN;
    BYTE  pRadioIfacesSize;
    BYTE  pRadioIfaces[nMaxStrLen];
    ULONG pRoaming;
    WORD  pMCC;
    WORD  pMNC;
    BYTE  nameSize;
    CHAR  pName[nMaxStrLen];
    BYTE  count;
    FILE  *fp = NULL;

    fp = tfopen("../../nas/test/results/getservingsystem.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_GET_SERVING_SYSTEM_TESTCASE_NUM)
        {
            pRadioIfacesSize    = GetServingSystemTestCases[tCaseNum].pRadioIfacesSize;
            nameSize            = GetServingSystemTestCases[tCaseNum].nameSize;

            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_GET_SERVING_SYSTEM_TESTCASE_NUM,
                       GetServingSystemTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "pRadioIfacesSize :%d\n", pRadioIfacesSize);
            fprintf(fp, "nameSize :%d\n", nameSize);

            nRet = GetServingNetwork(&pRegistrationState,
                                     &pCSDomain,
                                     &pPSDomain,
                                     &pRAN,
                                     &pRadioIfacesSize,
                                     &pRadioIfaces[0],
                                     &pRoaming,
                                     &pMCC,
                                     &pMNC,
                                     nameSize,
                                     &pName[0]);

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "pRegistrationState  : %lu\n", pRegistrationState);
                fprintf(fp, "pCSDomain  : %lu\n", pCSDomain);
                fprintf(fp, "pPSDomain : %lu\n", pPSDomain);
                fprintf(fp, "pRAN  : %lu\n", pRAN);
                fprintf(fp, "pRadioIfacesSize  : %d\n", pRadioIfacesSize);
                for(count = 0; count < pRadioIfacesSize; count++)
                    fprintf(fp, "pRadioIfaces  : %d\n", pRadioIfaces[count]);
                fprintf(fp, "pRoaming  : %lu\n", pRoaming);
                fprintf(fp, "pMCC  : %d\n", pMCC);
                fprintf(fp, "pMNC  : %d\n", pMNC);
                fprintf(fp, "pName  : %s\n", pName);
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetSignalStrength
 *
 * Purpose:  Perform the tests that call the GetSignalStrength() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetSignalStrengths( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    ULONG pArraySizes;
    INT8  pSignalStrengths[nMaxStrLen];
    ULONG pRadioInterfaces[nMaxStrLen];
    BYTE  count;
    FILE  *fp = NULL;

    fp = tfopen("../../nas/test/results/getsignalstrength.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_GET_SIGNAL_STRENGTHS_TESTCASE_NUM)
        {
            pArraySizes    = GetSignalStrengthsTestCases[tCaseNum].pArraySizes;

            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_GET_SIGNAL_STRENGTHS_TESTCASE_NUM,
                       GetSignalStrengthsTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "pArraySizes :%lu\n", pArraySizes);

            nRet = GetSignalStrengths(&pArraySizes,
                                      &pSignalStrengths[0],
                                      &pRadioInterfaces[0]);

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                for(count = 0; count < pArraySizes; count++)
                {
                    fprintf(fp, "pSignalStrengths[%d] : %d\n",
                            count, pSignalStrengths[count]);
                    fprintf(fp, "pRadioInterfaces[%d] : %lu\n",
                            count, pRadioInterfaces[count]);
                }
            }
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doInitNetworkReg
 *
 * Purpose:  Perform the tests that call the InitiateNetworkRegistration() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doInitNetworkReg( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    ULONG regType;
    WORD  mcc;
    WORD  mnc;
    ULONG rat;
    FILE  *fp = NULL;

    fp = tfopen("../../nas/test/results/initnetworkreg.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_INIT_NETWORK_REG_NUM)
        {
            regType = InitNetworkRegTestCases[tCaseNum].regType;
            mcc     = InitNetworkRegTestCases[tCaseNum].mcc;
            mnc     = InitNetworkRegTestCases[tCaseNum].mnc;
            rat     = InitNetworkRegTestCases[tCaseNum].rat;

            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_INIT_NETWORK_REG_NUM,
                       InitNetworkRegTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "regType :%lu\n", regType);
            fprintf(fp, "mcc :%d\n", mcc);
            fprintf(fp, "mnc :%d\n", mnc);
            fprintf(fp, "rat :%lu\n", rat);

            nRet = InitiateNetworkRegistration(regType,
                                               mcc,
                                               mnc,
                                               rat);

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "Network Registration Initiated\n");
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doPerformNetworkRegistration
 *
 * Purpose:  Perform the tests that call the PerformNetworkRegistration() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doPerformNetworkScan( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    BYTE  pInstanceSize;
    struct pInstance
    {
        WORD     pMCC;
        WORD     pMNC;
        ULONG    pInUse;
        ULONG    pRoaming;
        ULONG    pForbidden;
        ULONG    pPreferred;
        CHAR     pDesription[255];
    } pInstances[nMaxStrLen];
    //BYTE  pInstances[nMaxStrLen];
    BYTE  count;
    FILE  *fp = NULL;

    fp = tfopen("../../nas/test/results/performnetworkscan.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_PERFORM_NETWORK_SCAN_TESTCASE_NUM)
        {
            pInstanceSize = PerformNetworkScanTestCases[tCaseNum].pInstanceSize;

            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_PERFORM_NETWORK_SCAN_TESTCASE_NUM,
                       PerformNetworkScanTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "pInstanceSize :%d\n", pInstanceSize);

            nRet = PerformNetworkScan(&pInstanceSize,
                                      (BYTE *)&pInstances[0]);

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                for(count = 0; count < pInstanceSize; count++)
                {
                    fprintf(fp, "pMCC[%d] : %d\n",
                            count, pInstances[count].pMCC);
                    fprintf(fp, "pMNC[%d] : %d\n",
                            count, pInstances[count].pMNC);
                    fprintf(fp, "pInUse[%d] : %lu\n",
                            count, pInstances[count].pInUse);
                    fprintf(fp, "pRoaming[%d] : %lu\n",
                            count, pInstances[count].pRoaming);
                    fprintf(fp, "pForbidden[%d] : %lu\n",
                            count, pInstances[count].pForbidden);
                    fprintf(fp, "pPreferred[%d] : %lu\n",
                            count, pInstances[count].pPreferred);
                    fprintf(fp, "pDesription[%d] : %s\n",
                            count, pInstances[count].pDesription);
                }
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetServingNetworkCaps
 *
 * Purpose:  Perform the tests that call the GetServingNetworkCaps() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetServingNetworkCaps( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    BYTE  pDataCapsSize;
    ULONG pDataCaps[nMaxStrLen];
    FILE  *fp = NULL;
    BYTE  count;

    fp = tfopen("../../nas/test/results/getservingnetworkcaps.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_GET_SERVING_NETWORK_CAPS_TESTCASE_NUM)
        {
            pDataCapsSize = GetServingNetworkCapsTestCases[tCaseNum].DataCapsSize;

            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_GET_SERVING_NETWORK_CAPS_TESTCASE_NUM,
                       GetServingNetworkCapsTestCases[tCaseNum].desc);

            nRet = GetServingNetworkCapabilities( &pDataCapsSize,
                                                  (BYTE *)&pDataCaps[0] );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "pDataCapsSize  : %d\n", pDataCapsSize);
                for(count = 0; count < pDataCapsSize; count++)
                    fprintf(fp, "pDataCaps  : %lu\n", pDataCaps[count]);
            }
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doSetNetworkPreference
 *
 * Purpose:  Perform the tests that call the SetNetworkPreference() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSetNetworkPreference( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../nas/test/results/setnetworkpreference.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_SET_NETWORK_PREFERENCE_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_SET_NETWORK_PREFERENCE_TESTCASE_NUM,
                       SetNetworkPreferenceTestCases[tCaseNum].desc );

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "techpref :%lu\n",
                    SetNetworkPreferenceTestCases[tCaseNum].techpref);
            fprintf(fp, "duration :%lu\n",
                    SetNetworkPreferenceTestCases[tCaseNum].duration);

            nRet = SetNetworkPreference(
                              SetNetworkPreferenceTestCases[tCaseNum].techpref,
                              SetNetworkPreferenceTestCases[tCaseNum].duration );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "Network Preference Set\n");
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetNetworkPreference
 *
 * Purpose:  Perform the tests that call the GetNetworkPreference() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetNetworkPreference( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    ULONG pTechnologyPref;
    ULONG pDuration;
    ULONG pPersistentTechnologyPref;
    FILE  *fp = NULL;

    fp = tfopen("../../nas/test/results/getnetworkpreference.txt", "w");

    if (fp)
    {
        while ( tCaseNum < MAX_GET_NETWORK_PREFERENCE_TESTCASE_NUM )
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_GET_NETWORK_PREFERENCE_TESTCASE_NUM,
                       GetNetworkPreferenceTestCases[tCaseNum].desc);

            nRet = GetNetworkPreference( &pTechnologyPref,
                                         &pDuration,
                                         &pPersistentTechnologyPref );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "pTechnologyPref            : %lu\n",
                            pTechnologyPref);
                fprintf(fp, "pDuration                  : %lu\n", pDuration);
                fprintf(fp, "pPersistentTechnologyPref  : %lu \n",
                            pPersistentTechnologyPref);
            }
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetRFInfo
 *
 * Purpose:  Perform the tests that call the GetRFInfo() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetRFInfo( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    BYTE  count;
    BYTE  pInstancesSize;
    struct RFBandInfoElements pInstances[nMaxStrLen];
    FILE  *fp = NULL;

    fp = tfopen("../../nas/test/results/getrfinfo.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_GET_RF_INFO_TESTCASE_NUM)
        {
            pInstancesSize = GetRFInfoTestCases[tCaseNum].InstanceSize;

            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_GET_RF_INFO_TESTCASE_NUM,
                       GetRFInfoTestCases[tCaseNum].desc);

            nRet = GetRFInfo( &pInstancesSize,
                              (BYTE *)&pInstances[0] );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "pDataCapsSize  : %d\n", pInstancesSize);
                for(count = 0; count < pInstancesSize; count++)
                {
                    fprintf(fp, "radioInterface[%d] : %lx\n",
                            count, pInstances[count].radioInterface);
                    fprintf(fp, "activeBandClass[%d] : %lx\n",
                            count, pInstances[count].activeBandClass);
                    fprintf(fp, "activeChannel[%d] : %lx\n",
                            count, pInstances[count].activeChannel);
                }
            }
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doInitiateDomainAttach
 *
 * Purpose:  Perform the tests that call the InitiateDomainAttach() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doInitiateDomainAttach( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../nas/test/results/initdomainattach.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_INITIATE_DOMAIN_ATTACH_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_INITIATE_DOMAIN_ATTACH_TESTCASE_NUM,
                       InitiateDomainAttachTestCases[tCaseNum].desc );

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "action :%lu\n",
                    InitiateDomainAttachTestCases[tCaseNum].action);

            nRet = InitiateDomainAttach(
                              InitiateDomainAttachTestCases[tCaseNum].action );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "Domain Attach Initiated\n");
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetACCOLC
 *
 * Purpose:  Perform the tests that call the GetACCOLC() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetACCOLC( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    BYTE  ACCOLC;
    FILE  *fp = NULL;

    fp = tfopen("../../nas/test/results/getaccolc.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_GET_ACCOLC_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_GET_ACCOLC_TESTCASE_NUM,
                       GetACCOLCTestCases[tCaseNum].desc);

            nRet = GetACCOLC(&ACCOLC);

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "ACCOLC  : %d\n", ACCOLC);
            }
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doSetACCOLC
 *
 * Purpose:  Perform the tests that call the SetACCOLC() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSetACCOLC( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    CHAR  SPC[6];
    FILE  *fp = NULL;

    fp = tfopen("../../nas/test/results/setaccolc.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_SET_ACCOLC_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_SET_ACCOLC_TESTCASE_NUM,
                       SetACCOLCTestCases[tCaseNum].desc );

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "spc :%s\n",
                    SetACCOLCTestCases[tCaseNum].SPC);
            fprintf(fp, "accolc :%d\n",
                    SetACCOLCTestCases[tCaseNum].accolc);
            strcpy(SPC, SetACCOLCTestCases[tCaseNum].SPC);

            nRet = SetACCOLC( &SPC[0],
                              SetACCOLCTestCases[tCaseNum].accolc );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "ACCOLC Set\n");
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doSetCDMANetworkParameters
 *
 * Purpose:  Perform the tests that call the SetCDMANetworkParameters() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSetCDMANetworkParameters( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;
    BYTE  forcerev0;
    BYTE  customSCP;
    ULONG protocol;
    ULONG broadcast;
    ULONG application;
    ULONG roaming;
    fp = tfopen("../../nas/test/results/setCDMAnetworkparameters.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_SET_CDMA_NETWORK_PARAMETERS_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_SET_CDMA_NETWORK_PARAMETERS_TESTCASE_NUM,
                       SetCDMANetworkParametersTestCases[tCaseNum].desc );

            forcerev0   = SetCDMANetworkParametersTestCases[tCaseNum].forcerev0;
            customSCP   = SetCDMANetworkParametersTestCases[tCaseNum].customSCP;
            protocol    = SetCDMANetworkParametersTestCases[tCaseNum].protocol;
            broadcast   = SetCDMANetworkParametersTestCases[tCaseNum].broadcast;
            application = SetCDMANetworkParametersTestCases[tCaseNum].application;
            roaming     = SetCDMANetworkParametersTestCases[tCaseNum].roaming;

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "spc :%s\n",
                    SetCDMANetworkParametersTestCases[tCaseNum].spc);
            fprintf(fp, "forcerev0 :%d\n",
                    SetCDMANetworkParametersTestCases[tCaseNum].forcerev0);
            fprintf(fp, "customSCP :%d\n",
                    SetCDMANetworkParametersTestCases[tCaseNum].customSCP);
            fprintf(fp, "protocol :%lu\n",
                    SetCDMANetworkParametersTestCases[tCaseNum].protocol);
            fprintf(fp, "broadcast :%lu\n",
                    SetCDMANetworkParametersTestCases[tCaseNum].broadcast);
            fprintf(fp, "application :%lu\n",
                    SetCDMANetworkParametersTestCases[tCaseNum].application);
            fprintf(fp, "roaming :%lu\n",
                    SetCDMANetworkParametersTestCases[tCaseNum].roaming);

            nRet = SetCDMANetworkParameters(
                   (CHAR *)&SetCDMANetworkParametersTestCases[tCaseNum].spc[0],
                         &forcerev0,
                         &customSCP,
                         &protocol,
                         &broadcast,
                         &application,
                         &roaming );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "CDMA Network Parameters Set\n");
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetCDMANetworkParameters
 *
 * Purpose:  Perform the tests that call the GetCDMANetworkParameters() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetCDMANetworkParameters( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    BYTE  pSCI;
    BYTE  pSCM;
    BYTE  pRegHomeSID;
    BYTE  pRegForeignSID;
    BYTE  pRegForeignNID;
    BYTE  pForceRev0;
    BYTE  pCustomSCP;
    ULONG pProtocol;
    ULONG pBroadcast;
    ULONG pApplication;
    ULONG pRoaming;
    FILE  *fp = NULL;

    fp = tfopen("../../nas/test/results/getcdmanetworkparameters.txt", "w");

    if (fp)
    {
        while ( tCaseNum < MAX_GET_CDMA_NETWORK_PARAMETERS_TESTCASE_NUM )
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_GET_CDMA_NETWORK_PARAMETERS_TESTCASE_NUM,
                       GetCDMANetworkParametersTestCases[tCaseNum].desc);

            nRet = GetCDMANetworkParameters( &pSCI,
                                             &pSCM,
                                             &pRegHomeSID,
                                             &pRegForeignSID,
                                             &pRegForeignNID,
                                             &pForceRev0,
                                             &pCustomSCP,
                                             &pProtocol,
                                             &pBroadcast,
                                             &pApplication,
                                             &pRoaming );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "pSCI           : %d\n", pSCI);
                fprintf(fp, "pSCM           : %d\n", pSCM);
                fprintf(fp, "pRegHomeSID    : %d\n", pRegHomeSID);
                fprintf(fp, "pRegForeignSID : %d\n", pRegForeignSID);
                fprintf(fp, "pRegForeignNID : %d\n", pRegForeignNID);
                fprintf(fp, "pCustomSCP     : %d\n", pCustomSCP);
                fprintf(fp, "pProtocol      : %lu\n", pProtocol);
                fprintf(fp, "pBroadcast     : %lu\n", pBroadcast);
                fprintf(fp, "pApplication   : %lu\n", pApplication);
                fprintf(fp, "pRoaming       : %lu\n", pRoaming);
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetANAAAAuthStatus
 *
 * Purpose:  Perform the tests that call the GetANAAAAuthenticationStatus() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetANAAAAuthStatus( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    ULONG pStatus;
    FILE  *fp = NULL;

    fp = tfopen("../../nas/test/results/getanaaaauthstatus.txt", "w");

    if (fp)
    {
        while ( tCaseNum < MAX_GET_ANAAA_AUTH_TESTCASE_NUM )
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_GET_ANAAA_AUTH_TESTCASE_NUM,
                       GetANAAAAuthStatusTestCases[tCaseNum].desc);

            nRet = GetANAAAAuthenticationStatus( &pStatus );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "pStatus : %lu\n", pStatus);
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetSLQSServingSystem
 *
 * Purpose:  Perform the tests that call the SLQSGetServingSystem() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSGetServingSystem( void )
{
    ULONG                   nRet;
    FILE                    *fp = NULL;
    qaQmiServingSystemParam ServingSystemInfo;
    int                     ctr   = 0;
    int                     index = 0;

    /* Define the test cases to be executed */
    SLQSGetServingSystemTestCase_t SLQSGetServingSystemTestCases[] =
    {
        { eQCWWAN_ERR_INVALID_ARG, 1, NULL, "Invalid Test Case - Response structure NULL"},

        { eQCWWAN_ERR_NONE, 2, &ServingSystemInfo, "Valid test Case "},
    };

    fp = tfopen( "../../nas/test/results/slqsgetservingsysteminfo.txt", "w" );
    if ( fp )
    {
        BYTE tCaseNum     = 0;
        BYTE ncases = sizeof( SLQSGetServingSystemTestCases ) /
                      sizeof( SLQSGetServingSystemTestCases[0] );

        while( tCaseNum < ncases )
        {
            SLQSGetServingSystemTestCase_t *pd =
                &SLQSGetServingSystemTestCases[tCaseNum];

            /* print test data */
            fprintf( fp,"\n\nTest Case %d / %d : %s\n",
                     (tCaseNum + 1),
                     ncases,
                     pd->desc );

            nRet = SLQSGetServingSystem( pd->pServingSystemInfo );

            /* Display result code and text */
            doprintreason( fp, nRet );
            if (!nRet)
            {
                fprintf( fp,"Serving System Information \n" );
                fprintf( fp, "Registration state: 0x%x\n",         ServingSystemInfo.ServingSystem.regState );
                fprintf( fp, "CS Attach State: 0x%x\n",            ServingSystemInfo.ServingSystem.csAttachState );
                fprintf( fp, "PS Attach State: 0x%x\n",            ServingSystemInfo.ServingSystem.psAttachState );
                fprintf( fp, "Selected Network: 0x%x\n",           ServingSystemInfo.ServingSystem.selNetwork );
                fprintf( fp, "Number of Radio Interfaces: 0x%x\n", ServingSystemInfo.ServingSystem.numRadioInterfaces );
                ctr = ServingSystemInfo.ServingSystem.numRadioInterfaces;
                for( index = 0; index < ctr; index++ )
                {
                    fprintf( fp, "Radio Interface in use : 0x%x\n", ServingSystemInfo.ServingSystem.radioInterface[index] );
                }
                fprintf( fp,"\n" );
                fprintf( fp, "Roaming Indicator value: 0x%x\n",     ServingSystemInfo.roamIndicatorVal );

                fprintf( fp, "\n Data Service capability \n" );
                fprintf( fp, "Data Capability List Length: 0x%x\n", ServingSystemInfo.DataSrvCapabilities.dataCapabilitiesLen );
                ctr = ServingSystemInfo.DataSrvCapabilities.dataCapabilitiesLen;
                for( index = 0; index < ctr; index++ )
                {
                    fprintf( fp, "Data Capability : 0x%x\n", ServingSystemInfo.DataSrvCapabilities.dataCapabilities[index] );
                }

                fprintf( fp, "\n Current PLMN Information \n" );
                fprintf( fp, "Mobile Country Code: 0x%x\n",        ServingSystemInfo.CurrentPLMN.MCC );
                fprintf( fp, "Mobile Country Code: 0x%x\n",        ServingSystemInfo.CurrentPLMN.MNC );
                fprintf( fp, "Network Description Length: 0x%x\n", ServingSystemInfo.CurrentPLMN.netDescrLength );
                ctr = ServingSystemInfo.CurrentPLMN.netDescrLength;
                fprintf( fp, "Network Description : ");
                for( index = 0; index < ctr; index++ )
                {
                    fprintf( fp, "%c",  (char) ServingSystemInfo.CurrentPLMN.netDescr[index] );
                }
                fprintf( fp, "\n");

                fprintf( fp, "\n CDMA System ID \n" );
                fprintf( fp, "System ID: 0x%x\n",                  ServingSystemInfo.SystemID );
                fprintf( fp, "Network ID: 0x%x\n",                 ServingSystemInfo.NetworkID );

                fprintf( fp, "\n CDMA Base Station Information \n" );
                fprintf( fp, "Base ID: 0x%x\n",                    ServingSystemInfo.BasestationID );
                fprintf( fp, "Base Station latitude: 0x%lx\n",     ServingSystemInfo.BasestationLatitude );
                fprintf( fp, "Base Station longitude: 0x%lx\n",    ServingSystemInfo.BasestationLongitude );

                fprintf( fp, "\n Roaming Indicator List \n" );
                fprintf( fp, "Number of Instances: 0x%x\n",        ServingSystemInfo.RoamingIndicatorList.numInstances );
                ctr = ServingSystemInfo.RoamingIndicatorList.numInstances;
                for( index = 0; index < ctr; index++ )
                {
                    fprintf( fp, "Radio Interface : 0x%x\n",   ServingSystemInfo.RoamingIndicatorList.radioInterface[index] );
                    fprintf( fp, "Roaming Indicator : 0x%x\n", ServingSystemInfo.RoamingIndicatorList.roamIndicator[index] );
                }

                fprintf( fp,"\n" );
                fprintf( fp, "Default Roaming Indicator: 0x%x\n",  ServingSystemInfo.defaultRoamInd );

                fprintf( fp, "\n 3GPP2 Time Zone \n" );
                fprintf( fp, "Leap Seconds: 0x%x\n",               ServingSystemInfo.Gpp2TimeZone.leapSeconds );
                fprintf( fp, "Local Time Offset: 0x%x\n",          ServingSystemInfo.Gpp2TimeZone.localTimeOffset );
                fprintf( fp, "Daylight Saving indicator: 0x%x\n",  ServingSystemInfo.Gpp2TimeZone.daylightSavings );

                fprintf( fp,"\n" );
                fprintf( fp, "CDMA P_Rev in use : 0x%x\n",         ServingSystemInfo.CDMA_P_Rev );

                fprintf( fp, "3GPP Time Zone : 0x%x\n",            ServingSystemInfo.GppTimeZone );

                fprintf( fp, "3GPP NW Daylight Saving Adjustment : 0x%x\n", ServingSystemInfo.GppNetworkDSTAdjustment );

                fprintf( fp, "3GPP Location Area Code : 0x%x\n",    ServingSystemInfo.Lac );

                fprintf( fp, "Cell ID : 0x%lx\n",                   ServingSystemInfo.CellID );

                fprintf( fp, "Concurrent Service Info : 0x%x\n",    ServingSystemInfo.concSvcInfo );

                fprintf( fp, "PRL Indicator : 0x%x\n",              ServingSystemInfo.PRLInd );

                fprintf( fp, "Dual Transfer Mode Ind : 0x%x\n",    ServingSystemInfo.DTMInd );

                fprintf( fp, "\n Detailed Service Information \n" );
                fprintf( fp, "Service Status: 0x%x\n",              ServingSystemInfo.DetailedSvcInfo.srvStatus );
                fprintf( fp, "Service Capability: 0x%x\n",          ServingSystemInfo.DetailedSvcInfo.srvCapability );
                fprintf( fp, "HDR service status: 0x%x\n",         ServingSystemInfo.DetailedSvcInfo.hdrSrvStatus );
                fprintf( fp, "HDR hybrid information: 0x%x\n",     ServingSystemInfo.DetailedSvcInfo.hdrHybrid );
                fprintf( fp, "Forbidden system information: 0x%x\n", ServingSystemInfo.DetailedSvcInfo.isSysForbidden );

                fprintf( fp, "\n CDMA System Info Ext \n" );
                fprintf( fp, "MCC: 0x%x\n",                         ServingSystemInfo.CDMASystemInfoExt.MCC );
                fprintf( fp, "IMSI_11_12: 0x%x\n",                  ServingSystemInfo.CDMASystemInfoExt.imsi_11_12 );

                fprintf( fp,"\n" );
                fprintf( fp, "HDR Personality: 0x%x\n",             ServingSystemInfo.hdrPersonality );
                fprintf( fp, "TAC info for LTE: 0x%x\n",            ServingSystemInfo.trackAreaCode );

                fprintf( fp, "\n Call Barring Status \n" );
                fprintf( fp, "CS Bar Status: 0x%lx\n",               ServingSystemInfo.CallBarStatus.csBarStatus );
                fprintf( fp, "PS Bar Status: 0x%lx\n",               ServingSystemInfo.CallBarStatus.psBarStatus );
            }
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doSLQSSetBandPreference
 *
 * Purpose:  Perform the tests that call the SLQSSetBandPreference() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSSetBandPreference( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp      = NULL;

    fp = tfopen("../../nas/test/results/slqssetbandpreference.txt", "w");

    if (fp)
    {
        while ( tCaseNum < MAX_SLQS_SET_BAND_PREFERENCE_TESTCASE_NUM )
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_SLQS_SET_BAND_PREFERENCE_TESTCASE_NUM,
                       SLQSSetBandPreferenceTestCases[tCaseNum].desc );

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "band preference :%llx\n",
                    SLQSSetBandPreferenceTestCases[tCaseNum].bandpreference);

            nRet = SLQSSetBandPreference(
                    SLQSSetBandPreferenceTestCases[tCaseNum].bandpreference );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "System Band Preference Set\n");
            }

            tCaseNum++;
        }
    }

    if (fp)
    {
        tfclose(fp);
    }
}

/*
 * Name:     SLQSNasIndicationRegister
 *
 * Purpose:  Perform the tests that call the SLQSNasIndicationRegister() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSNasIndicationRegister( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp      = NULL;

    fp = tfopen("TestReport/slqsNasIndicationRegister.txt", "a");

    if (fp)
    {
        while ( tCaseNum < MAX_SLQS_INDICATION_REGISTER_TESTCASE_NUM )
        {
            const SLQSIndicationRegisterTestCase_t *pParams =
                                &(SLQSIndicationRegisterTestCases[tCaseNum]);

            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_SLQS_INDICATION_REGISTER_TESTCASE_NUM,
                       SLQSIndicationRegisterTestCases[tCaseNum].desc );

            nRet = SLQSNasIndicationRegister( pParams->systemSelectionInd,
                                              pParams->DDTMInd,
                                              pParams->servingSystemInd );

            fprintf(fp, "Return Code: %lu\n", nRet);
            doprintreason( fp, nRet );

            if (!nRet)
            {
                fprintf(fp, "NAS Indication Register Success\n");
            }
            tCaseNum++;
        }
    }
    if (fp)
    {
        tfclose(fp);
    }
}

/*
 * Name:     doSLQSGetSignalStrength
 *
 * Purpose:  Perform the tests that call the SLQSGetSignalStrength() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSGetSignalStrength( void )
{
    BYTE   tCaseNum = 0;
    ULONG  nRet;
    USHORT idx = 0, listLen = 0;
    FILE   *fp = NULL;

    fp = tfopen( "../../nas/test/results/slqsgetsignalstrength.txt", "w" );

    if (fp)
    {
        while ( tCaseNum < MAX_SLQS_GET_SIGNAL_STRENGTH_TESTCASE_NUM )
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                        ( tCaseNum + 1 ),
                        MAX_SLQS_GET_SIGNAL_STRENGTH_TESTCASE_NUM,
                        SLQSGetSignalStrengthTestCases[tCaseNum].desc );

            /* Mask all the bits of "Request Mask" to retrieve complete info */
            signalInfo.signalStrengthReqMask = 0x1F;
            fprintf(fp, "Parameter Set:\n" );
            fprintf(fp, "ReqMask : 0x%x\n", signalInfo.signalStrengthReqMask );

            nRet = SLQSGetSignalStrength( SLQSGetSignalStrengthTestCases
                                          [tCaseNum].pSignalInfo );

            /* Display result code and text */
            doprintreason ( fp, nRet );

            if (!nRet)
            {
                fprintf( fp, "\nSignal Strength Information:\n" );

                /* Display the current signal strength and radio inteface */
                fprintf( fp, "Current Signal Strength : %d\n",
                          signalInfo.rxSignalStrengthList[0].rxSignalStrength );
                fprintf( fp, "Current Radio Interface : %d\n\n",
                             signalInfo.rxSignalStrengthList[0].radioIf );

                /* Display Receive Signal Strength list */
                fprintf( fp, "Receive Signal Strength List:\n" );
                listLen = signalInfo.rxSignalStrengthListLen;
                fprintf( fp, "List's Length : %d\n", listLen );
                if( 0 != listLen )
                {
                    for( idx = 0; idx < listLen; idx++ )
                    {
                        fprintf( fp, "Receive Signal Strength[%d]: %d\n",
                                      idx,
                                      signalInfo.rxSignalStrengthList[idx].
                                      rxSignalStrength );
                        fprintf( fp, "Radio Interface[%d]        : %d\n",
                                      idx,
                                      signalInfo.rxSignalStrengthList[idx].
                                      radioIf );
                    }
                }
                fprintf( fp, "\n" );
                listLen = 0;
                idx = 0;

                /* Display ECIO list */
                fprintf( fp, "ECIO List:\n" );
                listLen = signalInfo.ecioListLen;
                fprintf( fp, "List's Length : %d\n", listLen );
                if( 0 != listLen )
                {
                    for( idx = 0; idx < listLen; idx++ )
                    {
                        fprintf( fp, "ECIO[%d]            : %d\n",
                                      idx,
                                      signalInfo.ecioList[idx].ecio );
                        fprintf( fp, "Radio Interface[%d] : %d\n",
                                      idx,
                                      signalInfo.ecioList[idx].radioIf );
                    }
                }
                fprintf( fp, "\n" );
                listLen = 0;
                idx = 0;

                fprintf( fp, "IO   : %lu\n", signalInfo.Io );
                fprintf( fp, "SINR : %d\n\n", signalInfo.sinr );

                /* Display Error Rate list */
                fprintf( fp, "Error Rate List:\n" );
                listLen = signalInfo.errorRateListLen;
                fprintf( fp, "List's Length : %d\n", listLen );
                if( 0 != listLen )
                {
                    for( idx = 0; idx < listLen; idx++ )
                    {
                        fprintf( fp, "Error Rate[%d]      : %d\n",
                                      idx,
                                      signalInfo.errorRateList[idx].errorRate );
                        fprintf( fp, "Radio Interface[%d] : %d\n",
                                      idx,
                                      signalInfo.errorRateList[idx].radioIf );
                    }
                }
                fprintf( fp, "\n" );

                /* Display RSRQ Information */
                fprintf( fp, "RSRQ Information\n" );
                fprintf( fp, "RSRQ            : %d\n",
                              signalInfo.rsrqInfo.rsrq );
                fprintf( fp, "Radio Interface : %d\n",
                              signalInfo.rsrqInfo.radioIf );
            }
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doSLQSPerformNetworkScan
 *
 * Purpose:  Perform the tests that call the SLQSPerformNetworkScan() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSPerformNetworkScan( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    BYTE  count;
    FILE  *fp = NULL;
    slqsNetworkScanInfo lnetworkInfo;

    fp = tfopen("../../nas/test/results/slqsperformnetworkscan.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_SLQS_PERFORM_NETWORK_SCAN_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       ( tCaseNum + 1 ),
                       MAX_SLQS_PERFORM_NETWORK_SCAN_TESTCASE_NUM,
                       SLQSPerformNetworkScanTestCases[tCaseNum].desc);

            /* Initialize the structure to be sent to API */
            /* Initialize the Network Instances */
            lnetworkInfo.pNetworkInfoInstances =
               SLQSPerformNetworkScanTestCases[tCaseNum].pNetworkInfoInstances;

            /* Initialize the Network Info Structure */
            lnetworkInfo.pNetworkInfo =
               SLQSPerformNetworkScanTestCases[tCaseNum].pNetworkInfo;

            /* Initialize the RAT Instances */
            lnetworkInfo.pRATInstances =
               SLQSPerformNetworkScanTestCases[tCaseNum].pRATInstances;

            /* Initialize the RAT Info Structure */
            lnetworkInfo.pRATInfo =
               SLQSPerformNetworkScanTestCases[tCaseNum].pRATInfo;

            fprintf(fp, "Parameter Set:\n");

            nRet = SLQSPerformNetworkScan( &lnetworkInfo );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet && lnetworkInfo.pNetworkInfoInstances
                      && lnetworkInfo.pNetworkInfo )
            {
                fprintf(fp, "SLQSPerformNetworkScan() Successful \n" );
                fprintf(fp, "NetworkInfoInstances from network : %d\n",
                        *( lnetworkInfo.pNetworkInfoInstances ));
                for(count = 0; count < *( lnetworkInfo.pNetworkInfoInstances ); count++)
                {
                    fprintf(fp, "MCC[%d]        : %d\n",
                          count, lnetworkInfo.pNetworkInfo[count].MCC);
                    fprintf(fp, "MNC[%d]        : %d\n",
                          count, lnetworkInfo.pNetworkInfo[count].MNC);
                    fprintf(fp, "InUse[%d]      : %lu\n",
                          count, lnetworkInfo.pNetworkInfo[count].InUse);
                    fprintf(fp, "Roaming[%d]    : %lu\n",
                          count, lnetworkInfo.pNetworkInfo[count].Roaming);
                    fprintf(fp, "Forbidden[%d]  : %lu\n",
                          count, lnetworkInfo.pNetworkInfo[count].Forbidden);
                    fprintf(fp, "Preferred[%d]  : %lu\n",
                          count, lnetworkInfo.pNetworkInfo[count].Preferred);
                    fprintf(fp, "Desription[%d] : %s\n",
                          count, lnetworkInfo.pNetworkInfo[count].Description);
                }
            }
            if (!nRet && lnetworkInfo.pRATInstances
                      && lnetworkInfo.pRATInfo )
            {
                fprintf(fp, "RATInstances from network : %d\n",
                                            *( lnetworkInfo.pRATInstances ));
                for (count = 0; count < *(lnetworkInfo.pRATInstances); count++)
                {
                    fprintf(fp, "RAT-MCC[%d]         : %d\n",
                             count, lnetworkInfo.pRATInfo[count].MCC);
                    fprintf(fp, "RAT-MNC[%d]         : %d\n",
                             count, lnetworkInfo.pRATInfo[count].MNC);
                    fprintf(fp, "RAT-Technology[%d]  : %x\n",
                             count, lnetworkInfo.pRATInfo[count].RAT);
                }
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doSLQSSetSysSelectionPref
 *
 * Purpose:  Perform the tests that call the SLQSSetSysSelectionPref() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSSetSysSelectionPref( void )
{
    ULONG               nRet;
    FILE                *fp = NULL;
    sysSelectPrefParams SysSelectPrefParams;

    /* Define the test cases to be executed */
    SLQSSetSysSelectionPrefTestCase_t SLQSSetSysSelPrefTestCases[] =
    {
        { eQCWWAN_ERR_INVALID_ARG, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
          NULL, NULL, NULL, NULL, "Invalid Test Case - All params NULL"},

        { eQCWWAN_ERR_NONE, 2, &EmerMode, NULL, NULL, NULL, NULL, NULL, NULL,
          NULL, NULL, NULL, NULL, "Valid test Case - Emergency Mode set"},

        { eQCWWAN_ERR_NONE, 3, NULL, &ModePref, NULL, NULL, NULL, NULL, NULL,
          NULL, NULL, NULL, NULL, "Valid test Case - Mode Preference set"},

        { eQCWWAN_ERR_NONE, 4, NULL, NULL, &bandPref, NULL, NULL, NULL, NULL,
          NULL, NULL, NULL, NULL, "Valid test Case - Band Preference set"},

        { eQCWWAN_ERR_NONE, 5, NULL, NULL, NULL, &PRLPref, NULL, NULL, NULL,
          NULL, NULL, NULL, NULL, "Valid test Case - CDMA PRL Preference set"},

        { eQCWWAN_ERR_NONE, 6, NULL, NULL, NULL, NULL, &RoamPref, NULL, NULL,
          NULL, NULL, NULL, NULL, "Valid Test Case - Roaming Preference set"},

        { eQCWWAN_ERR_NONE, 7, NULL, NULL, NULL, NULL, NULL, &LTEBandPref, NULL,
          NULL, NULL, NULL, NULL, "Valid Test Case - LTE Band Preference set"},

        { eQCWWAN_ERR_NONE, 8, NULL, NULL, NULL, NULL, NULL, NULL, &NetSelPref,
          NULL, NULL, NULL, NULL, "Valid Test Case - Network Selection Preference set"},

        { eQCWWAN_ERR_NONE, 9, &EmerMode, NULL, NULL, NULL, NULL, NULL, NULL,
          &chgDuration, NULL, NULL, NULL, "Valid Test Case - Change duration set and 1 parameter set"},

        { eQCWWAN_ERR_NONE, 10, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
          &chgDuration, NULL, NULL, NULL, "Valid Test Case - Only Change duration set"},

        { eQCWWAN_ERR_NONE, 11, NULL, NULL, NULL, NULL, NULL, NULL, &NetSelPref,
          NULL, &MNCIncPCS, NULL, NULL, "Valid Test Case - MNC Includes PCS Digit Status and Net sel Pref set"},

        { eQCWWAN_ERR_NONE, 12, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
          NULL, &MNCIncPCS, NULL, NULL, "Valid Test Case - Only MNC Includes PCS Digit Status set"},

        { eQCWWAN_ERR_NONE, 13, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
          NULL, NULL, &srvDomPref, NULL, "Valid Test Case - Service Domain Preference set"},

        { eQCWWAN_ERR_NONE, 14, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
          NULL, NULL, NULL, &GWAcqOrder, "Valid Test Case - GSM/WCDMA Acquisition Order Pref. set"},

        { eQCWWAN_ERR_NONE, 15, &EmerMode, &ModePref, &bandPref, &PRLPref, &RoamPref, &LTEBandPref,
          &NetSelPref, &chgDuration, &MNCIncPCS, &srvDomPref, &GWAcqOrder,
          "Valid Test Case - All parameters set"},

        { eQCWWAN_ERR_NONE, 16, &EmerMode, &ModePref, NULL, &PRLPref, &RoamPref, &LTEBandPref,
          &NetSelPref, &chgDuration, &MNCIncPCS, &srvDomPref, &GWAcqOrder,
          "Valid Test Case - All parameters except bandPref set"},

        { eQCWWAN_ERR_NONE, 17, &EmerMode, &ModePref, &bandPref, &PRLPref, &RoamPref, NULL,
          &NetSelPref, &chgDuration, &MNCIncPCS, &srvDomPref, &GWAcqOrder,
          "Valid Test Case - All parameters except LTE Band Pref set"},
    };

    fp = tfopen( "../../nas/test/results/slqssetsysselectionpref.txt", "w" );
    if ( fp )
    {
        BYTE tCaseNum     = 0;
        BYTE ncases = sizeof( SLQSSetSysSelPrefTestCases ) /
                      sizeof( SLQSSetSysSelPrefTestCases[0] );

        while( tCaseNum < ncases )
        {
            SLQSSetSysSelectionPrefTestCase_t *pd =
                &SLQSSetSysSelPrefTestCases[tCaseNum];

            /* print test data */
            fprintf( fp,"\n\nTest Case %d / %d : %s\n",
                     (tCaseNum + 1),
                     ncases,
                     pd->desc );
            fprintf( fp,"\nParameters set for System Selection Preference:\n" );
            IFPRINTF( fp, "Emergency Mode: 0x%x\n", pd->pEmerMode );
            IFPRINTF( fp, "Mode Preference: 0x%x\n", pd->pModePref );
            IFPRINTF( fp, "Band Preference: 0x%llx\n", pd->pBandPref );
            IFPRINTF( fp, "CDMA PRL Preference : 0x%x\n", pd->pPRLPref );
            IFPRINTF( fp, "Roaming Preference : 0x%x\n", pd->pRoamPref );
            IFPRINTF( fp, "LTE Band Preference : 0x%llx\n", pd->pLTEBandPref );
            if( pd->pNetSelPref)
            {
                fprintf( fp, "\nNetwork Selection Preference :\n");

                fprintf( fp, "Network Registration : 0x%x\n",
                          pd->pNetSelPref->netReg);
                fprintf( fp, "MCC : 0x%d\n", pd->pNetSelPref->mcc );
                fprintf( fp, "MNC : 0x%d\n", pd->pNetSelPref->mnc );
            }
            IFPRINTF( fp, "\nChange Duration : 0x%x\n", pd->pChgDuration );
            IFPRINTF( fp, "MNC Includes PCS Digit : 0x%x\n", pd->pMNCIncPCSDigStat );
            IFPRINTF( fp, "Service Domain Preference : 0x%lx\n", pd->pSrvDomainPref );
            IFPRINTF( fp, "GSM - WCDMA Acquisition Order Preference : 0x%lx\n",
                                                              pd->pGWAcqOrderPref );

            SysSelectPrefParams.pEmerMode    = pd->pEmerMode;
            SysSelectPrefParams.pModePref    = pd->pModePref;
            SysSelectPrefParams.pBandPref    = pd->pBandPref;
            SysSelectPrefParams.pPRLPref     = pd->pPRLPref;
            SysSelectPrefParams.pRoamPref    = pd->pRoamPref;
            SysSelectPrefParams.pLTEBandPref = pd->pLTEBandPref;
            SysSelectPrefParams.pNetSelPref  = pd->pNetSelPref;
            if( SysSelectPrefParams.pNetSelPref )
            {
                SysSelectPrefParams.pNetSelPref->netReg =
                                               pd->pNetSelPref->netReg;
                SysSelectPrefParams.pNetSelPref->mcc =
                                               pd->pNetSelPref->mcc;
                SysSelectPrefParams.pNetSelPref->mnc =
                                               pd->pNetSelPref->mnc;
            }
            SysSelectPrefParams.pChgDuration      = pd->pChgDuration;
            SysSelectPrefParams.pMNCIncPCSDigStat = pd->pMNCIncPCSDigStat;
            SysSelectPrefParams.pSrvDomainPref    = pd->pSrvDomainPref;
            SysSelectPrefParams.pGWAcqOrderPref   = pd->pGWAcqOrderPref;

            nRet = SLQSSetSysSelectionPref( &SysSelectPrefParams );

            /* Display result code and text */
            doprintreason( fp, nRet );
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doSLQSGetSysSelectionPref
 *
 * Purpose:  Perform the tests that call the SLQSGetSysSelectionPref() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSGetSysSelectionPref( void )
{
    ULONG             nRet;
    FILE              *fp = NULL;
    sysSelectPrefInfo SysSelectPrefInfo;

    /* Define the test cases to be executed */
    SLQSGetSysSelectionPrefTestCase_t SLQSGetSysSelPrefTestCases[] =
    {
        { eQCWWAN_ERR_INVALID_ARG, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
          NULL, NULL, "Invalid Test Case - All params NULL"},

        { eQCWWAN_ERR_NONE, 2, &GetEmerMode, NULL, NULL, NULL, NULL, NULL, NULL,
          NULL, NULL, "Valid test Case - Emergency Mode set"},

        { eQCWWAN_ERR_NONE, 3, NULL, &GetModePref, NULL, NULL, NULL, NULL, NULL,
          NULL, NULL, "Valid test Case - Mode Preference set"},

        { eQCWWAN_ERR_NONE, 4, NULL, NULL, &GetBandPref, NULL, NULL, NULL, NULL,
          NULL, NULL, "Valid test Case - Band Preference set"},

        { eQCWWAN_ERR_NONE, 5, NULL, NULL, NULL, &GetPRLPref, NULL, NULL, NULL,
          NULL, NULL, "Valid test Case - CDMA PRL Preference set"},

        { eQCWWAN_ERR_NONE, 6, NULL, NULL, NULL, NULL, &GetRoamPref, NULL, NULL,
          NULL, NULL, "Valid Test Case - Roaming Preference set"},

        { eQCWWAN_ERR_NONE, 7, NULL, NULL, NULL, NULL, NULL, &GetLTEBandPref, NULL,
          NULL, NULL, "Valid Test Case - LTE Band Preference set"},

        { eQCWWAN_ERR_NONE, 8, NULL, NULL, NULL, NULL, NULL, NULL, &GetNetSelPref,
          NULL, NULL, "Valid Test Case - Network Selection Preference set"},

        { eQCWWAN_ERR_NONE, 9, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &GetSrvDomPref,
          NULL, "Valid Test Case - Service Domain Preference set"},

        { eQCWWAN_ERR_NONE, 10, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &GetGWAcqOrder,
          "Valid Test Case - GSM/WCDMA Acquisition Order Pref. set"},

        { eQCWWAN_ERR_NONE, 11, &GetEmerMode, &GetModePref, &GetBandPref, &GetPRLPref,
          &GetRoamPref, &GetLTEBandPref, &GetNetSelPref, &GetSrvDomPref, &GetGWAcqOrder,
          "Valid Test Case - All parameters set"},

    };

    fp = tfopen( "../../nas/test/results/slqsgetsysselectionpref.txt", "w" );
    if ( fp )
    {
        BYTE tCaseNum     = 0;
        BYTE ncases = sizeof( SLQSGetSysSelPrefTestCases ) /
                      sizeof( SLQSGetSysSelPrefTestCases[0] );

        while( tCaseNum < ncases )
        {
            SLQSGetSysSelectionPrefTestCase_t *pd =
                &SLQSGetSysSelPrefTestCases[tCaseNum];

            /* print test data */
            fprintf( fp,"\n\nTest Case %d / %d : %s\n",
                     (tCaseNum + 1),
                     ncases,
                     pd->desc );

            SysSelectPrefInfo.pEmerMode       = pd->pEmerMode;
            SysSelectPrefInfo.pModePref       = pd->pModePref;
            SysSelectPrefInfo.pBandPref       = pd->pBandPref;
            SysSelectPrefInfo.pPRLPref        = pd->pPRLPref;
            SysSelectPrefInfo.pRoamPref       = pd->pRoamPref;
            SysSelectPrefInfo.pLTEBandPref    = pd->pLTEBandPref;
            SysSelectPrefInfo.pNetSelPref     = pd->pNetSelPref;
            SysSelectPrefInfo.pSrvDomainPref  = pd->pSrvDomainPref;
            SysSelectPrefInfo.pGWAcqOrderPref = pd->pGWAcqOrderPref;

            nRet = SLQSGetSysSelectionPref( &SysSelectPrefInfo );

            /* Display result code and text */
            doprintreason( fp, nRet );
            if (!nRet)
            {
                fprintf( fp,"System Selection Preferences \n" );
                IFPRINTF( fp, "Emergency Mode: 0x%x\n",               pd->pEmerMode );
                IFPRINTF( fp, "Mode Preference: 0x%x\n",              pd->pModePref );
                IFPRINTF( fp, "Band Preference: 0x%llx\n",            pd->pBandPref );
                IFPRINTF( fp, "CDMA PRL Preference : 0x%x\n",         pd->pPRLPref );
                IFPRINTF( fp, "Roaming Preference : 0x%x\n",          pd->pRoamPref );
                IFPRINTF( fp, "LTE Band Preference : 0x%llx\n",       pd->pLTEBandPref );
                IFPRINTF( fp, "Network Selection Preference: 0x%x\n", pd->pNetSelPref );
                IFPRINTF( fp, "Service Domain Preference : 0x%lx\n",  pd->pSrvDomainPref );
                IFPRINTF( fp, "GSM - WCDMA Acquisition Order Preference : 0x%lx\n",
                                                                      pd->pGWAcqOrderPref );
            }
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doSLQSNasGetSysInfo
 *
 * Purpose:  Perform the tests that call the SLQSNasGetSysInfo() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSNasGetSysInfo( void )
{
    local SrvStatusInfo      cdmassi;
    local SrvStatusInfo      hdrssi;
    local GSMSrvStatusInfo   gsmssi;
    local GSMSrvStatusInfo   wcdmassi;
    local GSMSrvStatusInfo   ltessi;
    local CDMASysInfo        cdmasi;
    local HDRSysInfo         hdrsi;
    local GSMSysInfo         gsmsi;
    local WCDMASysInfo       wcdmasi;
    local LTESysInfo         ltesi;
    local AddCDMASysInfo     addcdmasi;
    local WORD               addhdrsi;
    local AddSysInfo         addgsmsi;
    local AddSysInfo         addwcdmasi;
    local WORD               addltesi;
    local CallBarringSysInfo gsmcbsi;
    local CallBarringSysInfo wcdmacbsi;
    local BYTE               ltevssi;
    local BYTE               gsmcdsi;
    local BYTE               wcdmacdsi;

    /* Define the test cases to be executed */
    SLQSNasGetSysInfoTestCase_t SLQSNasGetSysInfoTestCases[] =
    {
        { eQCWWAN_ERR_NONE, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, "Valid Test Case - All params NULL"},

        { eQCWWAN_ERR_NONE, 2, &cdmassi, &hdrssi, &gsmssi, &wcdmassi, &ltessi,
        &cdmasi, &hdrsi, &gsmsi, &wcdmasi, &ltesi, &addcdmasi, &addhdrsi,
        &addgsmsi, &addwcdmasi, &addltesi, &gsmcbsi, &wcdmacbsi, &ltevssi,
        &gsmcdsi, &wcdmacdsi, "Valid Test Case - All params Provided "},
    };

    nasGetSysInfoResp resp;
    WORD              lC;
    FILE *fp = tfopen( "../../nas/test/results/slqsnasgetsysinfo.txt", "w" );
    if ( fp )
    {
        BYTE tCaseNum = 0;
        BYTE ncases   = sizeof( SLQSNasGetSysInfoTestCases ) /
                        sizeof( SLQSNasGetSysInfoTestCases[0] );

        while( tCaseNum < ncases )
        {
            SLQSNasGetSysInfoTestCase_t *pd =
                &SLQSNasGetSysInfoTestCases[tCaseNum++];

            /* print test data */
            fprintf( fp,"\n\nTest Case %d / %d : %s\n",
                     tCaseNum,
                     ncases,
                     pd->desc );

            resp.pCDMASrvStatusInfo        = pd->pCDMASrvStatusInfo;
            resp.pHDRSrvStatusInfo         = pd->pHDRSrvStatusInfo;
            resp.pGSMSrvStatusInfo         = pd->pGSMSrvStatusInfo;
            resp.pWCDMASrvStatusInfo       = pd->pWCDMASrvStatusInfo;
            resp.pLTESrvStatusInfo         = pd->pLTESrvStatusInfo;
            resp.pCDMASysInfo              = pd->pCDMASysInfo;
            resp.pHDRSysInfo               = pd->pHDRSysInfo;
            resp.pGSMSysInfo               = pd->pGSMSysInfo;
            resp.pWCDMASysInfo             = pd->pWCDMASysInfo;
            resp.pLTESysInfo               = pd->pLTESysInfo;
            resp.pAddCDMASysInfo           = pd->pAddCDMASysInfo;
            resp.pAddHDRSysInfo            = pd->pAddHDRSysInfo;
            resp.pAddGSMSysInfo            = pd->pAddGSMSysInfo;
            resp.pAddWCDMASysInfo          = pd->pAddWCDMASysInfo;
            resp.pAddLTESysInfo            = pd->pAddLTESysInfo;
            resp.pGSMCallBarringSysInfo    = pd->pGSMCallBarringSysInfo;
            resp.pWCDMACallBarringSysInfo  = pd->pWCDMACallBarringSysInfo;
            resp.pLTEVoiceSupportSysInfo   = pd->pLTEVoiceSupportSysInfo;
            resp.pGSMCipherDomainSysInfo   = pd->pGSMCipherDomainSysInfo;
            resp.pWCDMACipherDomainSysInfo = pd->pWCDMACipherDomainSysInfo;

            ULONG nRet = SLQSNasGetSysInfo( &resp );

            /* Display result code and text */
            doprintreason( fp, nRet );
            if ( nRet != eQCWWAN_ERR_NONE )
            {
                fprintf( fp,"SLQS NAS Get Sys Information Unsuccessful \n" );
                continue;
            }

            fprintf( fp,"SLQS NAS Get Sys Information Successful \n" );
            if ( resp.pCDMASrvStatusInfo )
            {
                fprintf( fp,"Service Status(CDMA) : %x\n",resp.pCDMASrvStatusInfo->srvStatus );
                fprintf( fp,"Is Preferred(CDMA)   : %x\n",resp.pCDMASrvStatusInfo->isPrefDataPath );
            }
            if ( resp.pHDRSrvStatusInfo )
            {
                fprintf( fp,"Service Status(HDR) : %x\n",resp.pHDRSrvStatusInfo->srvStatus );
                fprintf( fp,"Is Preferred(HDR)   : %x\n",resp.pHDRSrvStatusInfo->isPrefDataPath );
            }
            if ( resp.pGSMSrvStatusInfo )
            {
                fprintf( fp,"Service Status(GSM)      : %x\n",resp.pGSMSrvStatusInfo->srvStatus );
                fprintf( fp,"True Service Status(GSM) : %x\n",resp.pGSMSrvStatusInfo->trueSrvStatus );
                fprintf( fp,"Is Preferred(GSM)        : %x\n",resp.pGSMSrvStatusInfo->isPrefDataPath );
            }
            if ( resp.pWCDMASrvStatusInfo )
            {
                fprintf( fp,"Service Status(WCDMA)     : %x\n",resp.pWCDMASrvStatusInfo->srvStatus );
                fprintf( fp,"True Service Status(WCDMA): %x\n",resp.pWCDMASrvStatusInfo->trueSrvStatus );
                fprintf( fp,"Is Preferred(WCDMA)       : %x\n",resp.pWCDMASrvStatusInfo->isPrefDataPath );
            }
            if ( resp.pLTESrvStatusInfo )
            {
                fprintf( fp,"Service Status(LTE)      : %x\n",resp.pLTESrvStatusInfo->srvStatus );
                fprintf( fp,"True Service Status(LTE) : %x\n",resp.pLTESrvStatusInfo->trueSrvStatus );
                fprintf( fp,"Is Preferred(LTE)        : %x\n",resp.pLTESrvStatusInfo->isPrefDataPath );
            }
            if ( resp.pCDMASysInfo )
            {
                fprintf( fp,"\nCDMA SYSTEM INFORMATION ----\n");
                CDMASysInfo *t = resp.pCDMASysInfo;
                doprintsysInfoCommon( fp, &t->sysInfoCDMA );
                fprintf( fp,"System PRL Valid    : %x\n",t->isSysPrlMatchValid );
                fprintf( fp,"System PRL          : %x\n",t->isSysPrlMatch );
                fprintf( fp,"P_Rev Valid         : %x\n",t->pRevInUseValid );
                fprintf( fp,"P_Rev In Use        : %x\n",t->pRevInUse );
                fprintf( fp,"BS P_Rev Valid      : %x\n",t->bsPRevValid );
                fprintf( fp,"P_Rev In Use        : %x\n",t->bsPRev );
                fprintf( fp,"CCS_supp Valid      : %x\n",t->ccsSupportedValid );
                fprintf( fp,"CCS_supp            : %x\n",t->ccsSupported );
                fprintf( fp,"System Id           : %x\n",t->systemID );
                fprintf( fp,"Network Id          : %x\n",t->networkID );
                fprintf( fp,"BS Info Valid       : %x\n",t->bsInfoValid );
                fprintf( fp,"Base ID             : %x\n",t->baseId );
                fprintf( fp,"Base Latitude       : %lx\n",t->baseLat );
                fprintf( fp,"Base Longitude      : %lx\n",t->baseLong );
                fprintf( fp,"Packet Zone Valid   : %x\n",t->packetZoneValid );
                fprintf( fp,"Packet Zone         : %x\n",t->packetZone );
                fprintf( fp,"Network ID Valid    : %x\n",t->networkIdValid );
                fprintf( fp,"MCC info            : " );
                for ( lC = 0 ; lC < PLMN_LENGTH ; lC++ )
                {
                    fprintf( fp,"%x ",t->MCC[lC] );
                }
                fprintf( fp,"\n" );
                fprintf( fp,"MNC info            : " );
                for ( lC = 0 ; lC < PLMN_LENGTH ; lC++ )
                {
                    fprintf( fp,"%x ",t->MNC[lC] );
                }
                fprintf( fp,"\n" );
            }

            if ( resp.pHDRSysInfo )
            {
                fprintf( fp,"\nHDR SYSTEM INFORMATION ----\n");
                HDRSysInfo *t = resp.pHDRSysInfo;
                doprintsysInfoCommon( fp, &t->sysInfoHDR );
                fprintf( fp,"System PRL Valid    : %x\n",t->isSysPrlMatchValid );
                fprintf( fp,"System PRL          : %x\n",t->isSysPrlMatch );
                fprintf( fp,"Personality Valid   : %x\n",t->hdrPersonalityValid );
                fprintf( fp,"Personality         : %x\n",t->hdrPersonality );
                fprintf( fp,"Active Prot Valid   : %x\n",t->hdrActiveProtValid );
                fprintf( fp,"Active Protocol     : %x\n",t->hdrActiveProt );
                fprintf( fp,"IS-856 Sys Valid    : %x\n",t->is856SysIdValid );
                fprintf( fp,"IS-856 system ID    : " );
                for ( lC = 0 ; lC < SLQS_SYSTEM_ID_SIZE ; lC++ )
                {
                    fprintf( fp,"%x ",t->is856SysId[lC] );
                }
                fprintf( fp,"\n" );
            }
            if ( resp.pGSMSysInfo )
            {
                fprintf( fp,"\nGSM SYSTEM INFORMATION ----\n");
                GSMSysInfo *t = resp.pGSMSysInfo;
                doprintsysInfoCommon( fp, &t->sysInfoGSM );
                fprintf( fp,"LAC Valid           : %x\n",t->lacValid );
                fprintf( fp,"LAC                 : %x\n",t->lac );
                fprintf( fp,"Cell ID Valid       : %x\n",t->cellIdValid );
                fprintf( fp,"Cell ID             : %lx\n",t->cellId );
                fprintf( fp,"Reg Rej Info Valid  : %x\n",t->regRejectInfoValid );
                fprintf( fp,"Reject Srvc Domain  : %x\n",t->rejectSrvDomain );
                fprintf( fp,"Reject Cause        : %x\n",t->rejCause );
                fprintf( fp,"Network Id Valid    : %x\n",t->networkIdValid );
                fprintf( fp,"MCC info            : " );
                for ( lC = 0 ; lC < PLMN_LENGTH ; lC++ )
                {
                    fprintf( fp,"%x ",t->MCC[lC] );
                }
                fprintf( fp,"\n" );
                fprintf( fp,"MNC info            : " );
                for ( lC = 0 ; lC < PLMN_LENGTH ; lC++ )
                {
                    fprintf( fp,"%x ",t->MNC[lC] );
                }
                fprintf( fp,"\n" );
                fprintf( fp,"EGPRS Support Valid : %x\n",t->egprsSuppValid );
                fprintf( fp,"EGPRS Support       : %x\n",t->egprsSupp );
                fprintf( fp,"DTM Support Valid   : %x\n",t->dtmSuppValid );
                fprintf( fp,"DTM Support         : %x\n",t->dtmSupp );
            }
            if ( resp.pWCDMASysInfo )
            {
                fprintf( fp,"\nWCDMA SYSTEM INFORMATION ----\n");
                WCDMASysInfo *t = resp.pWCDMASysInfo;
                doprintsysInfoCommon( fp, &t->sysInfoWCDMA );
                fprintf( fp,"LAC Valid           : %x\n",t->lacValid );
                fprintf( fp,"LAC                 : %x\n",t->lac );
                fprintf( fp,"Cell ID Valid       : %x\n",t->cellIdValid );
                fprintf( fp,"Cell ID             : %lx\n",t->cellId );
                fprintf( fp,"Reg Rej Info Valid  : %x\n",t->regRejectInfoValid );
                fprintf( fp,"Reject Srvc Domain  : %x\n",t->rejectSrvDomain );
                fprintf( fp,"Reject Cause        : %x\n",t->rejCause );
                fprintf( fp,"Network Id Valid    : %x\n",t->networkIdValid );
                fprintf( fp,"MCC info            : " );
                for ( lC = 0 ; lC < PLMN_LENGTH ; lC++ )
                {
                    fprintf( fp,"%x ",t->MCC[lC] );
                }
                fprintf( fp,"\n" );
                fprintf( fp,"MNC info            : " );
                for ( lC = 0 ; lC < PLMN_LENGTH ; lC++ )
                {
                    fprintf( fp,"%x ",t->MNC[lC] );
                }
                fprintf( fp,"\n" );
                fprintf( fp,"HS Call Status Valid: %x\n",t->hsCallStatusValid );
                fprintf( fp,"HS Call Status      : %x\n",t->hsCallStatus );
                fprintf( fp,"HS Ind Valid        : %x\n",t->hsIndValid );
                fprintf( fp,"HS Indication       : %x\n",t->hsInd );
                fprintf( fp,"PSC Valid           : %x\n",t->pscValid );
                fprintf( fp,"Primary Scrambling Code : %x\n",t->psc );
            }
            if ( resp.pLTESysInfo )
            {
                fprintf( fp,"\nLTE SYSTEM INFORMATION ----\n");
                LTESysInfo *t = resp.pLTESysInfo;
                doprintsysInfoCommon( fp, &t->sysInfoLTE );
                fprintf( fp,"LAC Valid           : %x\n",t->lacValid );
                fprintf( fp,"LAC                 : %x\n",t->lac );
                fprintf( fp,"Cell ID Valid       : %x\n",t->cellIdValid );
                fprintf( fp,"Cell ID             : %lx\n",t->cellId );
                fprintf( fp,"Reg Rej Info Valid  : %x\n",t->regRejectInfoValid );
                fprintf( fp,"Reject Srvc Domain  : %x\n",t->rejectSrvDomain );
                fprintf( fp,"Reject Cause        : %x\n",t->rejCause );
                fprintf( fp,"Network Id Valid    : %x\n",t->networkIdValid );
                fprintf( fp,"MCC info            : " );
                for ( lC = 0 ; lC < PLMN_LENGTH ; lC++ )
                {
                    fprintf( fp,"%x ",t->MCC[lC] );
                }
                fprintf( fp,"\n" );
                fprintf( fp,"MNC info            : " );
                for ( lC = 0 ; lC < PLMN_LENGTH ; lC++ )
                {
                    fprintf( fp,"%x ",t->MNC[lC] );
                }
                fprintf( fp,"\n" );
                fprintf( fp,"TAC Valid           : %x\n",t->tacValid );
                fprintf( fp,"Tracking Area Code  : %x\n",t->tac );
            }
            if ( resp.pAddCDMASysInfo )
            {
                fprintf( fp, "Geo Sys Idx(CDMA) : %x\n", resp.pAddCDMASysInfo->geoSysIdx );
                fprintf( fp, "Reg Period (CDMA) : %x\n", resp.pAddCDMASysInfo->regPrd );
            }
            IFPRINTF( fp, "Geo Sys Idx(HDR) : %x\n", resp.pAddHDRSysInfo );
            if ( resp.pAddGSMSysInfo )
            {
                fprintf( fp, "Geo Sys Idx(GSM) : %x\n", resp.pAddGSMSysInfo->geoSysIdx );
                fprintf( fp, "Cell Br Cap(GSM) : %lx\n", resp.pAddGSMSysInfo->cellBroadcastCap );
            }
            if ( resp.pAddWCDMASysInfo )
            {
                fprintf( fp, "Geo Sys Idx(WCDMA) : %x\n", resp.pAddWCDMASysInfo->geoSysIdx );
                fprintf( fp, "Cell Br Cap(WCDMA) : %lx\n", resp.pAddWCDMASysInfo->cellBroadcastCap );
            }
            IFPRINTF( fp, "Geo Sys Idx(LTE) : %x\n", resp.pAddLTESysInfo );
            if ( resp.pGSMCallBarringSysInfo )
            {
                fprintf( fp, "CS Bar Status(GSM): %lx\n", resp.pGSMCallBarringSysInfo->csBarStatus );
                fprintf( fp, "PS Bar Status(GSM): %lx\n", resp.pGSMCallBarringSysInfo->psBarStatus );
            }
            if ( resp.pWCDMACallBarringSysInfo )
            {
                fprintf( fp, "CS Bar Status(WCDMA): %lx\n", resp.pWCDMACallBarringSysInfo->csBarStatus );
                fprintf( fp, "PS Bar Status(WCDMA): %lx\n", resp.pWCDMACallBarringSysInfo->psBarStatus );
            }
            IFPRINTF( fp, "Voice Supp on LTE: %x\n", resp.pLTEVoiceSupportSysInfo );
            IFPRINTF( fp, "GSM Cipher Domain: %x\n", resp.pGSMCipherDomainSysInfo );
            IFPRINTF( fp, "WCDMA Cipher Domain: %x\n", resp.pWCDMACipherDomainSysInfo );
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doSLQSNasSwiModemStatus
 *
 * Purpose:  Perform the tests that call the SLQSNasSwiModemStatus() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSNasSwiModemStatus( void )
{
    local LTEInfo  lteinfo;

    /* Define the test cases to be executed */
    SLQSNasSwiModemStatusTestCase_t SLQSNasSwiModemStatusTestCases[] =
    {
        { eQCWWAN_ERR_NONE, 1, NULL, "Valid Test Case - Passing NULL"},

        { eQCWWAN_ERR_NONE, 2, &lteinfo, "Valid Test Case - Fetching LTE info"},

    };

    FILE *fp = tfopen( "../../nas/test/results/slqsnasswimodemstatus.txt", "w" );
    swiModemStatusResp resp;

    if ( fp )
    {
        BYTE tC     = 0;
        BYTE ncases = sizeof( SLQSNasSwiModemStatusTestCases ) /
                      sizeof( SLQSNasSwiModemStatusTestCases[0] );

        while( tC < ncases )
        {
            SLQSNasSwiModemStatusTestCase_t *pd =
                &SLQSNasSwiModemStatusTestCases[tC++];

            /* print test data */
            fprintf( fp,"\n\nTest Case %d / %d : %s\n",
                     tC,
                     ncases,
                     pd->desc );

            resp.pLTEInfo = pd->pLTEInfo;

            ULONG nRet = SLQSNasSwiModemStatus( &resp );

            /* Display result code and text */
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf( fp,"SLQSNasSwiModemStatus Successful \n" );
                fprintf( fp,"Modem Temperature : %x \n",resp.commonInfo.temperature );
                fprintf( fp,"Modem Mode        : %x \n",resp.commonInfo.modemMode );
                fprintf( fp,"System Mode       : %x \n",resp.commonInfo.systemMode );
                fprintf( fp,"IMS Reg State     : %x \n",resp.commonInfo.imsRegState );
                fprintf( fp,"PS Attach State   : %x \n",resp.commonInfo.psState );

                if ( resp.pLTEInfo )
                {
                    LTEInfo *t = resp.pLTEInfo;
                    fprintf( fp,"Band            : %x \n",t->band );
                    fprintf( fp,"BandWidth       : %x \n",t->bandwidth );
                    fprintf( fp,"RX Channel      : %x \n",t->RXChan );
                    fprintf( fp,"TX Channel      : %x \n",t->TXChan );
                    fprintf( fp,"EMM State       : %x \n",t->emmState );
                    fprintf( fp,"EMM Sub State   : %x \n",t->emmSubState );
                    fprintf( fp,"EMM Conn State  : %x \n",t->emmConnState );
                }
            }
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSNasGetHDRColorCode
 *
 * Purpose: Perform the tests that call the SLQSNasGetHDRColorCode() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSNasGetHDRColorCode( void )
{
    local BYTE colorcode;

    /* Test Cases */
    SLQSGetHDRColorCodeTestCase_t SLQSGetHDRColorCodeTestCases[] =
    {
        {eQCWWAN_ERR_NONE, 1, NULL, " Valid case: Passing out parameter as NULL" },
        {eQCWWAN_ERR_NONE, 2, &colorcode, "All Valid" }
    };

    nasGetHDRColorCodeResp resp;

    FILE *fp = tfopen("../../voice/test/results/SLQSNasGetHDRColorCode.txt", "w");

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSGetHDRColorCodeTestCases )/
                      sizeof( SLQSGetHDRColorCodeTestCases[0] );

        while ( tc < ncases )
        {
            SLQSGetHDRColorCodeTestCase_t *pd = &SLQSGetHDRColorCodeTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            resp.pColorCode = pd->pColorCode;

            ULONG nRet = SLQSNasGetHDRColorCode( &resp );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSGetHDRColorCode Successful\n");
                IFPRINTF(fp,"Color Code is : %d \n",  pd->pColorCode);
            }
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSNasGetTxRxInfo
 *
 * Purpose: Perform the tests that call the SLQSNasGetTxRxInfo() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSNasGetTxRxInfo( void )
{
    local rxInfo RXChain0Info;
    local rxInfo RXChain1Info;
    local txInfo TXInfo;

    /* Test Cases */
    SLQSGetTxRxInfoTestCase_t SLQSGetTxRxInfoTestCases[] =
    {
        {eQCWWAN_ERR_NONE, 1, 0x01, &RXChain0Info, &RXChain1Info, NULL,
                                    "Valid Case: One optional params is NULL" },
        {eQCWWAN_ERR_NONE, 2, 0x02, &RXChain0Info, NULL, &TXInfo,
                                    "Valid Case: One optional params is NULL" },
        {eQCWWAN_ERR_NONE, 3, 0x03, NULL, &RXChain1Info, &TXInfo,
                                    "Valid Case: One optional params is NULL" },
        {eQCWWAN_ERR_NONE, 4, 0x04, NULL, NULL, NULL,
                                    "Valid Case: All optional params is NULL" },
        {eQCWWAN_ERR_NONE, 5, 0x05, &RXChain0Info, &RXChain1Info, &TXInfo,
                                                                  "All Valid" }
    };

    nasGetTxRxInfoReq  req;
    nasGetTxRxInfoResp resp;

    FILE *fp = tfopen("../../voice/test/results/slqsnasgettxrxinfo.txt", "w");

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSGetTxRxInfoTestCases )/
                      sizeof( SLQSGetTxRxInfoTestCases[0] );

        while ( tc < ncases )
        {
            SLQSGetTxRxInfoTestCase_t *pd = &SLQSGetTxRxInfoTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            req.radio_if = pd->radio_if;
            fprintf( fp, "Radio Interference : %x \n\n",req.radio_if );

            resp.pRXChain0Info = pd->pRXChain0Info;
            resp.pRXChain1Info = pd->pRXChain1Info;
            resp.pTXInfo       = pd->pTXInfo;

            ULONG nRet = SLQSNasGetTxRxInfo( &req, &resp );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE != nRet )
            {
                fprintf(fp, "SLQSNasGetTxRxInfo Unsuccessful\n");
                continue;
            }

            fprintf(fp, "SLQSNasGetTxRxInfo Successful\n");
            if ( NULL != resp.pRXChain0Info )
            {
                fprintf(fp, "Rx Chain 0 Info Response \n");
                fprintf(fp, "Is radio Tuned  : %x\n",
                                resp.pRXChain0Info->isRadioTuned);
                fprintf(fp, "Rx Power Value  : %lx\n",
                                resp.pRXChain0Info->rxPower);
                fprintf(fp, "ECIO            : %lx\n",
                                resp.pRXChain0Info->ecio);
                fprintf(fp, "RSCP            : %lx\n",
                                resp.pRXChain0Info->rscp);
                fprintf(fp, "RSRP            : %lx\n",
                                resp.pRXChain0Info->rsrp);
                fprintf(fp, "Phase           : %lx\n",
                                resp.pRXChain0Info->phase);
            }

            if ( NULL != resp.pRXChain1Info )
            {
                fprintf(fp, "Rx Chain 1 Info Response \n");
                fprintf(fp, "Is radio Tuned  : %x\n",
                                resp.pRXChain1Info->isRadioTuned);
                fprintf(fp, "Rx Power Value  : %lx\n",
                                resp.pRXChain1Info->rxPower);
                fprintf(fp, "ECIO            : %lx\n",
                                resp.pRXChain1Info->ecio);
                fprintf(fp, "RSCP            : %lx\n",
                                resp.pRXChain1Info->rscp);
                fprintf(fp, "RSRP            : %lx\n",
                                resp.pRXChain1Info->rsrp);
                fprintf(fp, "Phase           : %lx\n",
                                resp.pRXChain1Info->phase);
            }

            if ( NULL != resp.pTXInfo )
            {
                fprintf(fp, "Tx Info Response \n");
                fprintf(fp, "Is in traffic   : %x\n",
                                resp.pTXInfo->isInTraffic);
                fprintf(fp, "Tx Power Value  : %lx\n",
                                resp.pTXInfo->txPower);
            }
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSNasGetSigInfo
 *
 * Purpose: Perform the tests that call the SLQSNasGetSigInfo() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSNasGetSigInfo( void )
{
    local CDMASSInfo  cdmaSSInfo;
    local HDRSSInfo   hdrSSInfo;
    local BYTE        gsmSSInfo;
    local CDMASSInfo  wcdmaSSInfo;
    local LTESSInfo   lteSSInfo;

    /* Test Cases */
    SLQSGetSigInfoTestCase_t SLQSGetSigInfoTestCases[] =
    {
        {eQCWWAN_ERR_NONE, 1, NULL, NULL, NULL, NULL,
                           NULL,"Valid case - All optional params are NULL " },
        {eQCWWAN_ERR_NONE, 2, &cdmaSSInfo, &hdrSSInfo, &gsmSSInfo, &wcdmaSSInfo,
                                   &lteSSInfo,"All Valid - Fetching for All" },
    };

    nasGetSigInfoResp resp;

    FILE *fp = tfopen("../../voice/test/results/slqsnasgetsiginfo.txt", "w");

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSGetSigInfoTestCases )/
                      sizeof( SLQSGetSigInfoTestCases[0] );
        while ( tc < ncases )
        {
            SLQSGetSigInfoTestCase_t *pd = &SLQSGetSigInfoTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            resp.pCDMASSInfo  = pd->pCDMASSInfo;
            resp.pHDRSSInfo   = pd->pHDRSSInfo;
            resp.pGSMSSInfo   = pd->pGSMSSInfo;
            resp.pWCDMASSInfo = pd->pWCDMASSInfo;
            resp.pLTESSInfo   = pd->pLTESSInfo;

            ULONG nRet = SLQSNasGetSigInfo( &resp );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE != nRet )
            {
                fprintf(fp, "SLQSNasGetSigInfo Unsuccessful\n");
                continue;
            }

            fprintf(fp, "SLQSNasGetSigInfo Successful\n");
            if ( NULL != resp.pCDMASSInfo )
            {
                fprintf(fp, "CDMA SS Information \n");
                fprintf(fp, "RSSI  : %x\n", resp.pCDMASSInfo->rssi);
                fprintf(fp, "ECIO  : %x\n", resp.pCDMASSInfo->ecio);
            }
            if ( NULL != resp.pHDRSSInfo )
            {
                fprintf(fp, "HDR SS Information \n");
                fprintf(fp, "RSSI  : %x\n", resp.pHDRSSInfo->rssi);
                fprintf(fp, "ECIO  : %x\n", resp.pHDRSSInfo->ecio);
                fprintf(fp, "SINR  : %x\n", resp.pHDRSSInfo->sinr);
                fprintf(fp, "IO    : %lx\n", resp.pHDRSSInfo->io);
            }
            IFPRINTF(fp, "GSM SIG INFO : %x\n", resp.pGSMSSInfo);
            if ( NULL != resp.pWCDMASSInfo )
            {
                fprintf(fp, "WCDMA SS Information \n");
                fprintf(fp, "RSSI  : %x\n", resp.pWCDMASSInfo->rssi);
                fprintf(fp, "ECIO  : %x\n", resp.pWCDMASSInfo->ecio);
            }
            if ( NULL != resp.pLTESSInfo )
            {
                fprintf(fp, "LTE SS Information \n");
                fprintf(fp, "RSSI  : %x\n", resp.pLTESSInfo->rssi);
                fprintf(fp, "RSRQ  : %x\n", resp.pLTESSInfo->rsrq);
                fprintf(fp, "RSRP  : %x\n", resp.pLTESSInfo->rsrp);
                fprintf(fp, "SNR   : %x\n", resp.pLTESSInfo->snr);
            }
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSNasIndicationRegisterExt
 *
 * Purpose: Perform the tests that call the SLQSNasIndicationRegisterExt() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSNasIndicationRegisterExt( void )
{
    /* Test Cases */
    SLQSNasIndicationRegisterExtTestCase_t SLQSNasIndicationRegisterExtTestCases[] =
    {
        { eQCWWAN_ERR_NONE, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, "Invalid case - AtLeast one parameter must be set " },

        { eQCWWAN_ERR_NONE, 2, &setInd, &setInd, &setInd, &setInd, &setInd,
        &setInd, &setInd, &setInd, &setInd, &setInd, &setInd, &setInd,
        "Valid case - All params are set " },

        { eQCWWAN_ERR_NONE, 3, &unsetInd, &unsetInd, &unsetInd, &unsetInd, &unsetInd,
        &unsetInd, &unsetInd, &unsetInd, &unsetInd, &unsetInd, &unsetInd, &unsetInd,
        "Valid case - All params are unset " }
    };

    nasIndicationRegisterReq req;

    FILE *fp = tfopen("../../voice/test/results/slqsnasindicationregisterext.txt", "w");

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSNasIndicationRegisterExtTestCases )/
                      sizeof( SLQSNasIndicationRegisterExtTestCases[0] );
        while ( tc < ncases )
        {
            SLQSNasIndicationRegisterExtTestCase_t *pd =
                            &SLQSNasIndicationRegisterExtTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            req.pSystemSelectionInd  = pd->pSystemSelectionInd;
            req.pDDTMInd             = pd->pDDTMInd;
            req.pServingSystemInd    = pd->pServingSystemInd;
            req.pDualStandByPrefInd  = pd->pDualStandByPrefInd;
            req.pSubscriptionInfoInd = pd->pSubscriptionInfoInd;
            req.pNetworkTimeInd      = pd->pNetworkTimeInd;
            req.pSysInfoInd          = pd->pSysInfoInd;
            req.pSignalStrengthInd   = pd->pSignalStrengthInd;
            req.pErrorRateInd        = pd->pErrorRateInd;
            req.pHDRNewUATIAssInd    = pd->pHDRNewUATIAssInd;
            req.pHDRSessionCloseInd  = pd->pHDRSessionCloseInd;
            req.pManagedRoamingInd   = pd->pManagedRoamingInd;

            IFPRINTF(fp, "System Selection Ind  : %x\n", req.pSystemSelectionInd);
            IFPRINTF(fp, "DDTM Ind              : %x\n", req.pDDTMInd);
            IFPRINTF(fp, "Serving System Ind    : %x\n", req.pServingSystemInd);
            IFPRINTF(fp, "Dual StandBy Pref Ind : %x\n", req.pDualStandByPrefInd);
            IFPRINTF(fp, "Subscription Info Ind : %x\n", req.pSubscriptionInfoInd);
            IFPRINTF(fp, "Network Time Ind      : %x\n", req.pNetworkTimeInd);
            IFPRINTF(fp, "System Info Ind       : %x\n", req.pSysInfoInd);
            IFPRINTF(fp, "Signal Strength Ind   : %x\n", req.pSignalStrengthInd);
            IFPRINTF(fp, "Error Rate Ind        : %x\n", req.pErrorRateInd);
            IFPRINTF(fp, "HDR New UATI Assn Ind : %x\n", req.pHDRNewUATIAssInd);
            IFPRINTF(fp, "HDR Session Close Ind : %x\n", req.pHDRSessionCloseInd);
            IFPRINTF(fp, "managed Roaming Ind   : %x\n", req.pManagedRoamingInd);

            ULONG nRet = SLQSNasIndicationRegisterExt( &req );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE != nRet )
            {
                fprintf(fp, "SLQSNasIndicationRegisterExt Unsuccessful\n");
                continue;
            }

            fprintf(fp, "SLQSNasIndicationRegisterExt Successful\n");
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:     doSLQSGetPLMNName
 *
 * Purpose:  Perform the tests that call the SLQSGetPLMNName() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSGetPLMNName( void )
{
    ULONG           nRet;
    FILE            *fp = NULL;
    BYTE            index = 0;
    nasPLMNNameReq  PLMNNameReq;
    nasPLMNNameResp PLMNNameResp;

    PLMNNameReq.mcc = 302;
    PLMNNameReq.mnc = 720;

    /* Define the test cases to be executed */
    SLQSGetPLMNNameTestCase_t SLQSGetPLMNNameTestCases[] =
    {
        { eQCWWAN_ERR_INVALID_ARG, 1, NULL, NULL,
                          "Invalid Test Case - All params NULL"},
        { eQCWWAN_ERR_NONE,        2, &PLMNNameReq, &PLMNNameResp,
                          "Valid Test Case - All parameters set"},
    };

    fp = tfopen( "TestReport/slqsgetplmnname.txt", "w" );
    if ( fp )
    {
        BYTE tCaseNum     = 0;
        BYTE ncases = sizeof( SLQSGetPLMNNameTestCases ) /
                      sizeof( SLQSGetPLMNNameTestCases[0] );

        while( tCaseNum < ncases )
        {
            SLQSGetPLMNNameTestCase_t *pd =
                &SLQSGetPLMNNameTestCases[tCaseNum];

            /* print test data */
            fprintf( fp,"\n\nTest Case %d / %d : %s\n",
                     (tCaseNum + 1),
                     ncases,
                     pd->desc );

            nRet = SLQSGetPLMNName( pd->pPLMNNameReq, pd->pPLMNNameResp );

            /* Display result code and text */
            doprintreason( fp, nRet );
            if ( (!nRet) && (pd->pPLMNNameResp) )
            {
                nasPLMNNameResp *lresp = pd->pPLMNNameResp;
                fprintf( fp,"SPN Encoding :: %xH\n", lresp->spnEncoding);
                fprintf( fp,"SPN Length :: %xH\n", lresp->spnLength);
                if ((lresp->spnEncoding == 1) && (lresp->spnLength > 0))
                {
                    fprintf( fp, "SPN(unicode) :: ");
                    for (index = 0; index < lresp->spnLength; index++)
                        fprintf( fp,"%x ", lresp->spn[index]);
                    fprintf( fp,"\n");
                }
                else if ((lresp->spnEncoding == 0) && (lresp->spnLength > 0))
                {
                    fprintf( fp,"SPN(ascii) :: ");
                    for (index = 0; index < lresp->spnLength; index++)
                        fprintf( fp,"%c", lresp->spn[index]);
                    fprintf( fp,"\n");
                }

                fprintf( fp,"shortname Encoding :: %xH\n", lresp->shortNameEn);
                fprintf( fp,"shortname Country Initial :: %xH\n", lresp->shortNameCI);
                fprintf( fp,"shortname Spare Bits :: %xH\n", lresp->shortNameSB);
                fprintf( fp,"shortname Length :: %xH\n", lresp->shortNameLen);
                if ((lresp->shortNameEn == 1) && (lresp->shortNameLen > 0))
                {
                    fprintf( fp, "Short Name(unicode) :: ");
                    for (index = 0; index < lresp->shortNameLen; index++)
                        fprintf( fp,"%x ", lresp->shortName[index]);
                    fprintf( fp,"\n");
                }
                else if (lresp->shortNameLen > 0)
                {
                    fprintf( fp, "Short Name(ascii) :: ");
                    for (index = 0; index < lresp->shortNameLen; index++)
                        fprintf( fp,"%c", lresp->shortName[index]);
                    fprintf( fp,"\n");
                }

                fprintf( fp,"longname Encoding :: %xH\n", lresp->longNameEn);
                fprintf( fp,"longname Country Initial :: %xH\n", lresp->longNameCI);
                fprintf( fp,"longname Spare Bits :: %xH\n", lresp->longNameSB);
                fprintf( fp,"longname Length :: %xH\n", lresp->longNameLen);
                if ((lresp->longNameEn == 1) && (lresp->longNameLen > 0))
                {
                    fprintf( fp, "Long Name(unicode) :: ");
                    for (index = 0; index < lresp->longNameLen; index++)
                        fprintf( fp,"%x ", lresp->longName[index]);
                    fprintf( fp,"\n");
                }
                else if (lresp->longNameLen > 0)
                {
                    fprintf( fp, "Long Name(ascii) :: ");
                    for (index = 0; index < lresp->longNameLen; index++)
                        fprintf( fp,"%c", lresp->longName[index]);
                    fprintf( fp,"\n");
                }
            }
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doSLQSGetOperatorNameData
 *
 * Purpose:  Perform the tests that call the SLQSGetOperatorNameData() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSGetOperatorNameData( void )
{
    ULONG               nRet;
    FILE                *fp = NULL;
    BYTE                index = 0,index_1 = 0;
    nasOperatorNameResp OperatorNameData;
    serviceProviderName srvcProviderName;
    operatorPLMNList    operatorPLMNLists;
    PLMNNetworkName     PLMNNetworkNames;
    operatorNameString  operatorNameStrings;
    PLMNNetworkNameData NITZInformation;

    OperatorNameData.pSrvcProviderName   = &srvcProviderName;
    OperatorNameData.pOperatorPLMNList   = &operatorPLMNLists;
    OperatorNameData.pPLMNNetworkName    = &PLMNNetworkNames;
    OperatorNameData.pOperatorNameString = &operatorNameStrings;
    OperatorNameData.pNITZInformation    = &NITZInformation;

    /* Define the test cases to be executed */
    SLQSGetOperatorNameDataTestCase_t SLQSGetOperatorNameTestCases[] =
    {
        { eQCWWAN_ERR_INVALID_ARG, 1, NULL,
          "Invalid Test Case - All params NULL"},
        { eQCWWAN_ERR_NONE,        2, &OperatorNameData,
          "Valid Test Case - All parameters set"},
    };

    fp = tfopen( "TestReport/slqsgetgetoperatorname.txt", "w" );
    if ( fp )
    {
        BYTE tCaseNum     = 0;
        BYTE ncases = sizeof( SLQSGetOperatorNameTestCases ) /
                      sizeof( SLQSGetOperatorNameTestCases[0] );

        while( tCaseNum < ncases )
        {
            SLQSGetOperatorNameDataTestCase_t *pd =
                &SLQSGetOperatorNameTestCases[tCaseNum];

            /* print test data */
            fprintf( fp,"\n\nTest Case %d / %d : %s\n",
                     (tCaseNum + 1),
                     ncases,
                     pd->desc );

            nRet = SLQSGetOperatorNameData( pd->pOperatorNameData );

            /* Display result code and text */
            doprintreason( fp, nRet );

            if ( (pd->pOperatorNameData != NULL) && (!nRet) )
            {
                if( pd->pOperatorNameData->pSrvcProviderName )
                {
                    serviceProviderName *lresp =
                           pd->pOperatorNameData->pSrvcProviderName;
                    fprintf( fp, "\nService Provider Name Information\n" );
                    fprintf( fp, "Display Condition %x\n",
                                        lresp->displayCondition);
                    fprintf( fp, "SPN Length :: %d \n",
                                        lresp->spnLength);
                    fprintf( fp, "SPN :: ");
                    for(index = 0; index < lresp->spnLength; index++)
                    {
                        fprintf( fp, "%c", lresp->spn[index]);
                    }
                    fprintf( fp, "\n" );
                }

                if( pd->pOperatorNameData->pOperatorPLMNList )
                {
                    operatorPLMNList *lresp =
                                 pd->pOperatorNameData->pOperatorPLMNList;
                    fprintf( fp, "\nOperator PLMN List Information\n" );
                    fprintf( fp, "Num Instance :: %d\n", lresp->numInstance );

                    for(index = 0; index < lresp->numInstance; index++)
                    {
                        fprintf( fp, "MCC :: %c %c %c\n",
                                 lresp->PLMNData[index].mcc[1],
                                 lresp->PLMNData[index].mcc[2],
                                 lresp->PLMNData[index].mcc[3] );
                        fprintf( fp, "MNC :: %c %c %c",
                                 lresp->PLMNData[index].mnc[1],
                                 lresp->PLMNData[index].mnc[2],
                                 lresp->PLMNData[index].mnc[3] );
                        fprintf( fp, "LAC1 :: %x\n",
                                 lresp->PLMNData[index].lac1 );
                        fprintf( fp, "LAC2 :: %x\n",
                                 lresp->PLMNData[index].lac2 );
                        fprintf( fp, "PLMNRecID :: %x\n",
                                 lresp->PLMNData[index].PLMNRecID );
                    }
                }

                if( pd->pOperatorNameData->pPLMNNetworkName )
                {
                    PLMNNetworkName *qresp =
                            pd->pOperatorNameData->pPLMNNetworkName;
                    fprintf( fp, "\nPLMN Network Name\n" );
                    fprintf( fp, "Num Instance :: %d\n", qresp->numInstance );
                    for(index = 0; index < qresp->numInstance; index++)
                    {
                        PLMNNetworkNameData *lresp =
                            &pd->pOperatorNameData->pPLMNNetworkName->\
                            PLMNNetName[index];
                        fprintf( fp, "Config Scheme :: %x\n",
                                       lresp->codingScheme );
                        fprintf( fp, "CountryInitails :: %x\n",
                                      lresp->countryInitials );
                        fprintf( fp, "Long Name Length :: %x\n",
                                           lresp->longNameLen );
                        fprintf( fp, "Long Name :: ");
                        for(index_1 = 0; index_1 < lresp->longNameLen;
                                                            index_1++)
                        {
                            fprintf( fp, "%x ", lresp->longName[index_1] );
                        }
                        fprintf( fp, "\n" );
                        fprintf( fp, "Long Name Spare Bits :: %x\n",
                                         lresp->longNameSpareBits );
                        fprintf( fp, "Short Name Length :: %x\n",
                                           lresp->shortNameLen );
                        fprintf( fp, "Short Name :: ");
                        for(index_1 = 0; index_1 < lresp->shortNameLen;
                                                             index_1++)
                        {
                            fprintf( fp, "%x ", lresp->shortName[index_1] );
                        }
                        fprintf( fp, "\n" );
                        fprintf( fp, "Short Name Spare Bits :: %x\n",
                                         lresp->shortNameSpareBits );
                    }
                }

                if( pd->pOperatorNameData->pOperatorNameString )
                {
                    operatorNameString *lresp =
                                 pd->pOperatorNameData->pOperatorNameString;
                    fprintf( fp, "\nOperator Name String\n" );
                    fprintf( fp, "PLMN Name :: ");
                    for(index = 0; lresp->PLMNName[index] != EOS; index++)
                    {
                        fprintf( fp, "%c ", lresp->PLMNName[index] );
                    }
                    fprintf( fp, "\n" );
                }

                if( pd->pOperatorNameData->pNITZInformation )
                {
                    PLMNNetworkNameData *lresp =
                            pd->pOperatorNameData->pNITZInformation;
                    fprintf( fp, "\nNITZ Information\n" );
                    fprintf( fp, "Config Scheme :: %x\n",
                                   lresp->codingScheme );
                    fprintf( fp, "CountryInitails :: %x\n",
                                  lresp->countryInitials );
                    fprintf( fp, "Long Name Spare Bits :: %x\n",
                                     lresp->longNameSpareBits );
                    fprintf( fp, "Short Name Spare Bits :: %x\n",
                                     lresp->shortNameSpareBits );
                    fprintf( fp, "Long Name Length :: %d\n",
                                       lresp->longNameLen );
                    fprintf( fp, "Long Name :: ");
                    for(index = 0; index < lresp->longNameLen; index++)
                    {
                        fprintf( fp, "%c", lresp->longName[index] );
                    }
                    fprintf( fp, "\n" );
                    fprintf( fp, "Short Name Length :: %d\n",
                                        lresp->shortNameLen );
                    fprintf( fp, "Short Name :: ");
                    for(index = 0; index < lresp->shortNameLen; index++)
                    {
                        fprintf( fp, "%c", lresp->shortName[index] );
                    }
                    fprintf( fp, "\n" );
                }
           }
           tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSGet3GPP2Subscription
 *
 * Purpose: Perform the tests that call the SLQSNasGet3GPP2Subscription() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSGet3GPP2Subscription( void )
{
    local namName      NAMNameInfo;
    local dirNum       DirNum;
    local homeSIDNID   HomeSIDNID;
    local minBasedIMSI MinBasedIMSI;
    local trueIMSI     TrueIMSI;
    local CDMAChannel  CDMAChannel;

    /* Test Cases */
    SLQSGet3GPP2SubscriptionTestCase_t SLQSGet3GPP2SubscriptionTestCases[] =
    {
        {eQCWWAN_ERR_NONE, 1, 0x01, &NAMNameInfo, &DirNum, &HomeSIDNID,
                                    &MinBasedIMSI, &TrueIMSI, NULL,
                                    "Valid Case: One optional params is NULL" },
        {eQCWWAN_ERR_NONE, 2, 0x01, &NAMNameInfo, &DirNum, &HomeSIDNID,
                                    &MinBasedIMSI, NULL, &CDMAChannel,
                                    "Valid Case: One optional params is NULL" },
        {eQCWWAN_ERR_NONE, 3, 0x01, &NAMNameInfo, &DirNum, &HomeSIDNID,
                                    NULL, &TrueIMSI, &CDMAChannel,
                                    "Valid Case: One optional params is NULL" },
        {eQCWWAN_ERR_NONE, 4, 0x01, &NAMNameInfo, &DirNum, NULL,
                                    &MinBasedIMSI, &TrueIMSI, &CDMAChannel,
                                    "Valid Case: One optional params is NULL" },
        {eQCWWAN_ERR_NONE, 5, 0x01, &NAMNameInfo, NULL, &HomeSIDNID,
                                    &MinBasedIMSI, &TrueIMSI, &CDMAChannel,
                                    "Valid Case: One optional params is NULL" },
        {eQCWWAN_ERR_NONE, 6, 0x01, NULL, &DirNum, &HomeSIDNID,
                                    &MinBasedIMSI, &TrueIMSI, &CDMAChannel,
                                    "Valid Case: One optional params is NULL" },
        {eQCWWAN_ERR_NONE, 7, 0x01, NULL, NULL, NULL,
                                    NULL, NULL, NULL,
                                    "Valid Case: All optional params is NULL" },
        {eQCWWAN_ERR_NONE, 8, 0x01, &NAMNameInfo, &DirNum, &HomeSIDNID,
                                    &MinBasedIMSI, &TrueIMSI, &CDMAChannel,
                                    "All Valid" },
        {eQCWWAN_ERR_NONE, 8, 0xFF, &NAMNameInfo, &DirNum, &HomeSIDNID,
                                    &MinBasedIMSI, &TrueIMSI, &CDMAChannel,
                                    "All Valid - For Current NAM" }
    };

    nasGet3GPP2SubscriptionInfoReq  req;
    nasGet3GPP2SubscriptionInfoResp resp;
    WORD                            lC;

    FILE *fp = tfopen("../../nas/test/results/slqsget3gpp2subscription.txt", "w");

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSGet3GPP2SubscriptionTestCases )/
                      sizeof( SLQSGet3GPP2SubscriptionTestCases[0] );

        while ( tc < ncases )
        {
            SLQSGet3GPP2SubscriptionTestCase_t *pd =
                    &SLQSGet3GPP2SubscriptionTestCases[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            req.namID = pd->namID;
            fprintf( fp, "Nam Id : %x \n\n",req.namID );

            resp.pNAMNameInfo  = pd->pNAMNameInfo;
            resp.pDirNum       = pd->pDirNum;
            resp.pHomeSIDNID   = pd->pHomeSIDNID;
            resp.pMinBasedIMSI = pd->pMinBasedIMSI;
            resp.pTrueIMSI     = pd->pTrueIMSI;
            resp.pCDMAChannel  = pd->pCDMAChannel;

            ULONG nRet = SLQSNasGet3GPP2Subscription( &req, &resp );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE != nRet )
            {
                fprintf(fp, "SLQSNasGet3GPP2Subscription Unsuccessful\n");
                continue;
            }

            fprintf(fp, "SLQSNasGet3GPP2Subscription Successful\n");
            if ( NULL != resp.pNAMNameInfo )
            {
                fprintf(fp, "Nam Name Response \n");
                fprintf(fp, "NAM name length  : %x\n",
                                                resp.pNAMNameInfo->namNameLen);
                fprintf(fp, "Nam Name Info:");
                for ( lC = 0 ; lC < resp.pNAMNameInfo->namNameLen ; lC++ )
                {
                    fprintf(fp, " %x", resp.pNAMNameInfo->namName[lC]);
                }
                fprintf(fp, "\n");
            }

            if ( NULL != resp.pDirNum )
            {
                fprintf(fp, "Directory Number Response \n");
                fprintf(fp, "Directory number length  : %x\n",
                                                resp.pDirNum->dirNumLen);
                fprintf(fp, "Directory Number Info:");
                for ( lC = 0 ; lC < resp.pDirNum->dirNumLen ; lC++ )
                {
                    fprintf(fp, " %x", resp.pDirNum->dirNum[lC]);
                }
                fprintf(fp, "\n");
            }

            if ( NULL != resp.pHomeSIDNID )
            {
                fprintf(fp, "SID/NID Response \n");
                fprintf(fp, "Number of sets : %x\n",
                                                resp.pHomeSIDNID->numInstances);
                for ( lC = 0 ; lC < resp.pHomeSIDNID->numInstances ; lC++ )
                {
                    sidNid *t = &resp.pHomeSIDNID->SidNid[lC];
                    fprintf(fp, "SID NID Info %d\n",  lC);
                    fprintf(fp, "System ID  :%x ",   t->sid);
                    fprintf(fp, "Network ID :%x\n", t->nid);
                }
            }

            if ( NULL != resp.pMinBasedIMSI )
            {
                fprintf(fp, "Min Based IMSI Response \n");
                fprintf(fp, "MCCM :");
                for ( lC = 0; lC < PLMN_LENGTH ; lC++)
                {
                    fprintf(fp, " %x ", resp.pMinBasedIMSI->mccM[lC]);
                }
                fprintf(fp, "\n");

                fprintf(fp, "IMSI_M_11_12 : %x\n", resp.pMinBasedIMSI->imsiM1112);

                fprintf(fp, "IMSI_M 1 :");
                for ( lC = 0; lC < IMSI_M_S1_LENGTH ; lC++)
                {
                    fprintf(fp, " %x ", resp.pMinBasedIMSI->imsiMS1[lC]);
                }
                fprintf(fp, "\n");

                fprintf(fp, "IMSI_M 2 :");
                for ( lC = 0; lC < IMSI_M_S2_LENGTH ; lC++)
                {
                    fprintf(fp, " %x", resp.pMinBasedIMSI->imsiMS2[lC]);
                }
                fprintf(fp, "\n");
            }

            if ( NULL != resp.pTrueIMSI )
            {
                fprintf(fp, "True IMSI Response \n");
                fprintf(fp, "MCCT :");
                for ( lC = 0; lC < PLMN_LENGTH ; lC++)
                {
                    fprintf(fp, " %x ", resp.pTrueIMSI->mccT[lC]);
                }
                fprintf(fp, "\n");

                fprintf(fp, "IMSI_T_11_12 : %x\n", resp.pTrueIMSI->imsiT1112);

                fprintf(fp, "IMSI_T 1 :");
                for ( lC = 0; lC < IMSI_M_S1_LENGTH ; lC++)
                {
                    fprintf(fp, " %x ", resp.pTrueIMSI->imsiTS1[lC]);
                }
                fprintf(fp, "\n");

                fprintf(fp, "IMSI_T 2 :");
                for ( lC = 0; lC < IMSI_M_S2_LENGTH ; lC++)
                {
                    fprintf(fp, " %x ", resp.pTrueIMSI->imsiTS2[lC]);
                }
                fprintf(fp, "\n");

                fprintf(fp, "IMSI_T_ADDR_NUM : %x\n", resp.pTrueIMSI->imsiTaddrNum);
            }
            if ( NULL != resp.pCDMAChannel )
            {
                fprintf(fp, "CDMA Channel Information \n");
                fprintf(fp, "Primary Carrier A   : %x\n", resp.pCDMAChannel->priChA);
                fprintf(fp, "Primary Carrier B   : %x\n", resp.pCDMAChannel->priChB);
                fprintf(fp, "Secondary Carrier A : %x\n", resp.pCDMAChannel->secChA);
                fprintf(fp, "Secondary Carrier A : %x\n", resp.pCDMAChannel->secChB);
            }
        }
    }
    if (fp)
        tfclose(fp);
}


/*
 * Name:     doSLQSNasGetCellLocationInfo
 *
 * Purpose:  Perform the tests that call the SLQSNasGetCellLocationInfo() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSNasGetCellLocationInfo( void )
{
    local GERANInfo               geranInfo;
    local UMTSInfo                umtsInfo;
    local CDMAInfo                cdmaInfo;
    local LTEInfoIntrafreq        lteInfoIntrafreq;
    local LTEInfoInterfreq        lteInfoInterfreq;
    local LTEInfoNeighboringGSM   lteInfoNeighboringGSM;
    local LTEInfoNeighboringWCDMA lteInfoNeighboringWCDMA;

    /* Define the test cases to be executed */
    SLQSNasGetCellLocationInfoTestCase_t SLQSNasGetCellLocationInfoTestCases[] =
    {
        { eQCWWAN_ERR_NONE, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
          "Valid Test Case - All params NULL"},

        { eQCWWAN_ERR_NONE, 1, &geranInfo, &umtsInfo, &cdmaInfo,
        &lteInfoIntrafreq, &lteInfoInterfreq, &lteInfoNeighboringGSM,
        &lteInfoNeighboringWCDMA, "Valid Test Case - Fetching All"},

    };

    FILE *fp = tfopen( "../../nas/test/results/slqsgetcelllocationinfo.txt", "w" );
    nasCellLocationInfoResp resp;
    WORD                    lC, lC1;

    if ( fp )
    {
        BYTE tC     = 0;
        BYTE ncases = sizeof( SLQSNasGetCellLocationInfoTestCases ) /
                      sizeof( SLQSNasGetCellLocationInfoTestCases[0] );

        while( tC < ncases )
        {
            SLQSNasGetCellLocationInfoTestCase_t *pd =
                &SLQSNasGetCellLocationInfoTestCases[tC++];

            /* print test data */
            fprintf( fp,"\n\nTest Case %d / %d : %s\n",
                     tC,
                     ncases,
                     pd->desc );

            resp.pGERANInfo               = pd->pGERANInfo;
            resp.pUMTSInfo                = pd->pUMTSInfo;
            resp.pCDMAInfo                = pd->pCDMAInfo;
            resp.pLTEInfoIntrafreq        = pd->pLTEInfoIntrafreq;
            resp.pLTEInfoInterfreq        = pd->pLTEInfoInterfreq;
            resp.pLTEInfoNeighboringGSM   = pd->pLTEInfoNeighboringGSM;
            resp.pLTEInfoNeighboringWCDMA = pd->pLTEInfoNeighboringWCDMA;

            ULONG nRet = SLQSNasGetCellLocationInfo( &resp );

            /* Display result code and text */
            doprintreason( fp, nRet );

            if (!nRet)
            {
                fprintf( fp,"SLQS NAS Get Cell Location Info Successful \n" );
                if ( resp.pGERANInfo )
                {
                    fprintf( fp,"\nGERAN Information \n" );
                    GERANInfo *temp = resp.pGERANInfo;
                    fprintf( fp,"Cell Id   : %lx \n",temp->cellID );
                    fprintf( fp,"PLMN info : " );
                    for (lC=0;lC<PLMN_LENGTH;lC++)
                    {
                        fprintf( fp,"%x ",temp->plmn[lC] );
                    }
                    fprintf( fp,"\n" );
                    fprintf( fp,"LAC       : %x \n",temp->lac );
                    fprintf( fp,"ARFCN     : %x \n",temp->arfcn );
                    fprintf( fp,"BSIC      : %x \n",temp->bsic );
                    fprintf( fp,"Timing Adv: %lx \n",temp->timingAdvance );
                    fprintf( fp,"RX-lev    : %x \n",temp->rxLev );
                    fprintf( fp,"No. of sets of following info : %x \n",temp->nmrInst );
                    for (lC=0;lC<temp->nmrInst;lC++)
                    {
                        nmrCellInfo *temp1 = &temp->insNmrCellInfo[lC];
                        fprintf( fp,"\tNMR Cell %d Information \n",lC+1 );
                        fprintf( fp,"\tNMR Cell Id   : %lx \n",temp1->nmrCellID );
                        fprintf( fp,"\tNMR PLMN info : " );
                        for (lC1=0;lC1<PLMN_LENGTH;lC1++)
                        {
                            fprintf( fp,"%x ",temp1->nmrPlmn[lC1] );
                        }
                        fprintf( fp,"\n" );
                        fprintf( fp,"\tNMR LAC       : %x \n",temp1->nmrLac );
                        fprintf( fp,"\tNMR ARFCN     : %x \n",temp1->nmrArfcn );
                        fprintf( fp,"\tNMR BSIC      : %x \n",temp1->nmrBsic );
                        fprintf( fp,"\tNMR RX-lev    : %x \n",temp1->nmrRxLev );
                    }
                }
                if ( resp.pUMTSInfo )
                {
                    fprintf( fp,"\nUMTS Information \n" );
                    UMTSInfo *temp = resp.pUMTSInfo;
                    fprintf( fp,"Cell Id   : %lx \n",temp->cellID );
                    fprintf( fp,"PLMN info : " );
                    for (lC=0;lC<PLMN_LENGTH;lC++)
                    {
                        fprintf( fp,"%x ",temp->plmn[lC] );
                    }
                    fprintf( fp,"\n" );
                    fprintf( fp,"LAC       : %x \n",temp->lac );
                    fprintf( fp,"UARFCN    : %x \n",temp->uarfcn );
                    fprintf( fp,"PSC       : %x \n",temp->psc );
                    fprintf( fp,"RSCP      : %x \n",temp->rscp );
                    fprintf( fp,"ECIO      : %x \n",temp->ecio );
                    fprintf( fp,"No. of sets of umts instance : %x \n",temp->umtsInst );
                    for (lC=0;lC<temp->umtsInst;lC++)
                    {
                        UMTSinstInfo *temp1 = &temp->UMTSInstInfo[lC];
                        fprintf( fp,"\tUMTS Instance %d Information \n",lC+1 );
                        fprintf( fp,"\tUMTS UARFCN  : %x \n",temp1->umtsUarfcn );
                        fprintf( fp,"\tUMTS PSC     : %x \n",temp1->umtsPsc);
                        fprintf( fp,"\tUMTS RSCP    : %x \n",temp1->umtsRscp );
                        fprintf( fp,"\tUMTS ECIO    : %x \n",temp1->umtsEcio );
                    }
                    fprintf( fp,"No. of sets of geran instance : %x \n",temp->geranInst );
                    for (lC=0;lC<temp->geranInst;lC++)
                    {
                        geranInstInfo *temp1 = &temp->GeranInstInfo[lC];
                        fprintf( fp,"\tGERAN Instance %d Information \n",lC+1 );
                        fprintf( fp,"\tGERAN ARFCN   : %x \n",temp1->geranArfcn );
                        fprintf( fp,"\tGERAN BSIC NCC: %x \n",temp1->geranBsicNcc);
                        fprintf( fp,"\tGERAN BSIC BCC: %x \n",temp1->geranBsicBcc );
                        fprintf( fp,"\tGERAN RSSI    : %x \n",temp1->geranRssi );
                    }
                }
                if ( resp.pCDMAInfo )
                 {
                     fprintf( fp,"\nCDMA Information \n" );
                     CDMAInfo *temp = resp.pCDMAInfo;
                     fprintf( fp,"System Id  : %x \n",temp->sid );
                     fprintf( fp,"Network Id : %x \n",temp->nid );
                     fprintf( fp,"Base Id    : %x \n",temp->baseId );
                     fprintf( fp,"Reference PN: %x \n",temp->refpn );
                     fprintf( fp,"Base Lat   : %lx \n",temp->baseLat );
                     fprintf( fp,"Base Long  : %lx \n",temp->baseLong );
                 }
                if ( resp.pLTEInfoIntrafreq )
                {
                    fprintf( fp,"\nLTE Information - INtra Frequency \n" );
                    LTEInfoIntrafreq *temp = resp.pLTEInfoIntrafreq;
                    fprintf( fp,"UE in Idle    : %x \n",temp->ueInIdle );
                    fprintf( fp,"PLMN info     : " );
                    for (lC=0;lC<PLMN_LENGTH;lC++)
                    {
                        fprintf( fp,"%x ",temp->plmn[lC] );
                    }
                    fprintf( fp,"\n" );
                    fprintf( fp,"TAC                   : %x \n",temp->tac );
                    fprintf( fp,"Global Cell Id        : %lx \n",temp->globalCellId );
                    fprintf( fp,"EARFCN                : %x \n",temp->earfcn );
                    fprintf( fp,"Serving Cell Id       : %x \n",temp->servingCellId );
                    fprintf( fp,"Cell Resel Priority   : %x \n",temp->cellReselPriority );
                    fprintf( fp,"Non-Intra Search      : %x \n",temp->sNonIntraSearch );
                    fprintf( fp,"Serving Cell Threshold: %x \n",temp->threshServingLow );
                    fprintf( fp,"Intra Search          : %x \n",temp->sIntraSearch );
                    fprintf( fp,"No. of sets of following info : %x \n",temp->cellsLen );
                    for (lC=0;lC<temp->cellsLen;lC++)
                    {
                        cellParams *temp1 = &temp->CellParams[lC];
                        fprintf( fp,"\tCell Number %d Information \n",lC+1 );
                        fprintf( fp,"\tPhysical Cell Id: %x \n",temp1->pci );
                        fprintf( fp,"\tRSRQ            : %x \n",temp1->rsrq );
                        fprintf( fp,"\tRSRP            : %x \n",temp1->rsrp );
                        fprintf( fp,"\tRSSI            : %x \n",temp1->rssi );
                        fprintf( fp,"\tSRX-lev         : %x \n",temp1->srxlev );
                    }
                }
                if ( resp.pLTEInfoInterfreq )
                {
                    fprintf( fp,"\nLTE Information - Inter Frequency \n" );
                    LTEInfoInterfreq *temp = resp.pLTEInfoInterfreq;
                    fprintf( fp,"UE in Idle    : %x \n",temp->ueInIdle );
                    fprintf( fp,"Frequency Len : %d \n",temp->freqsLen );
                    for (lC=0;lC<temp->freqsLen;lC++)
                    {
                        infoInterFreq *t = &(temp->InfoInterfreq[lC]);
                        fprintf( fp,"Frequency Number %d Information \n",lC+1 );
                        fprintf( fp,"EARFCN            : %d \n",t->earfcn );
                        fprintf( fp,"Thresh X Low      : %d \n",t->threshXLow );
                        fprintf( fp,"Thresh X High     : %d \n",t->threshXHigh );
                        fprintf( fp,"Cell Resel Pty    : %d \n",t->cell_resel_priority );
                        fprintf( fp,"Cells Length      : %d \n",t->cells_len );
                        for (lC1=0;lC1<t->cells_len;lC1++)
                        {
                            cellParams *temp1 = &t->cellInterFreqParams[lC1];
                            fprintf( fp,"Cell Number %d Information \n",lC1+1 );
                            fprintf( fp,"\tPhysical Cell Id: %x \n",temp1->pci );
                            fprintf( fp,"\tRSRQ            : %x \n",temp1->rsrq );
                            fprintf( fp,"\tRSRP            : %x \n",temp1->rsrp );
                            fprintf( fp,"\tRSSI            : %x \n",temp1->rssi );
                            fprintf( fp,"\tSRX-lev         : %x \n",temp1->srxlev );
                        }
                    }
                }
                if ( resp.pLTEInfoNeighboringGSM )
                {
                    fprintf( fp,"\nLTE Information - Neighboring GSM \n" );
                    LTEInfoNeighboringGSM *temp = resp.pLTEInfoNeighboringGSM;
                    fprintf( fp,"UE in Idle    : %x \n",temp->ueInIdle );
                    fprintf( fp,"Frequency Len : %d \n",temp->freqsLen );
                    for (lC=0;lC<temp->freqsLen;lC++)
                    {
                        lteGsmCellInfo *t = &(temp->LteGsmCellInfo[lC]);
                        fprintf( fp,"\tFrequency Number %d Information \n",lC+1 );
                        fprintf( fp,"\tCell Resel Pty    : %d \n",t->cellReselPriority );
                        fprintf( fp,"\tThresh GSM High   : %d \n",t->threshGsmHigh );
                        fprintf( fp,"\tThresh GSM Low    : %d \n",t->threshGsmLow );
                        fprintf( fp,"\tNCC Permitted     : %x \n",t->nccPermitted );
                        fprintf( fp,"\tCells Length      : %d \n",t->cells_len );
                        for (lC1=0;lC1<t->cells_len;lC1++)
                        {
                            gsmCellInfo *temp1 = &t->GsmCellInfo[lC1];
                            fprintf( fp,"\tGSM Cell Number %d Information \n",lC1+1 );
                            fprintf( fp,"\t\tARFCN         : %d \n",temp1->arfcn );
                            fprintf( fp,"\t\tBand_1900     : %x \n",temp1->band1900 );
                            fprintf( fp,"\t\tCell ID Valid : %x \n",temp1->cellIdValid );
                            fprintf( fp,"\t\tBSIC ID       : %x \n",temp1->bsicId );
                            fprintf( fp,"\t\tRSSI          : %x \n",temp1->rssi );
                            fprintf( fp,"\t\tSRX-lev       : %x \n",temp1->srxlev );
                        }
                    }
                }
                if ( resp.pLTEInfoNeighboringWCDMA )
                {
                    fprintf( fp,"\nLTE Information - Neighboring WCDMA \n" );
                    LTEInfoNeighboringWCDMA *temp = resp.pLTEInfoNeighboringWCDMA;
                    fprintf( fp,"UE in Idle    : %x \n",temp->ueInIdle );
                    fprintf( fp,"Frequency Len : %d \n",temp->freqsLen );
                    for (lC=0;lC<temp->freqsLen;lC++)
                    {
                        lteWcdmaCellInfo *t = &(temp->LTEWCDMACellInfo[lC]);
                        fprintf( fp,"\tFrequency Number %d Information \n",lC+1 );
                        fprintf( fp,"\tUARFCN          : %d \n",t->uarfcn );
                        fprintf( fp,"\tCell Resel Pty  : %d \n",t->cellReselPriority );
                        fprintf( fp,"\tThresh X High   : %d \n",t->threshXhigh );
                        fprintf( fp,"\tThresh X Low    : %d \n",t->threshXlow );
                        fprintf( fp,"\tCells Length    : %d \n",t->cellsLen );
                        for (lC1=0;lC1<t->cellsLen;lC1++)
                        {
                            wcdmaCellInfo *temp1 = &t->WCDMACellInfo[lC1];
                            fprintf( fp,"\tWCDMA Cell Number %d Information \n",lC1+1 );
                            fprintf( fp,"\t\tPSC        : %d \n",temp1->psc );
                            fprintf( fp,"\t\tCPICH_RSCP : %x \n",temp1->cpich_rscp );
                            fprintf( fp,"\t\tCPICH_ECNO : %x \n",temp1->cpich_ecno );
                            fprintf( fp,"\t\tSRX-lev    : %x \n",temp1->srxlev );
                        }
                    }
                }
            }
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:     doSLQSInitNetworkReg
 *
 * Purpose:  Perform the tests that call the SLQSInitiateNetworkRegistration() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSInitNetworkReg( void )
{
    MNRInfo          temp;
    ULONG            changeDurationPC = 1;
    ULONG            changeDurationPer = 0;
    BOOL             mncPcsDigitT = TRUE;
    BOOL             mncPcsDigitF = FALSE;
    struct MNRegInfo MNRInfo3Digit, MNRInfo2Digit;

    MNRInfo3Digit.mcc = 302;
    MNRInfo2Digit.mcc = 302;
    MNRInfo3Digit.mnc = 220;
    MNRInfo2Digit.mnc = 0x022;
    MNRInfo3Digit.rat = 5;
    MNRInfo2Digit.rat = 5;

    SLQSNasInitNetworkRegTestCase_t SLQSNasInitNetworkRegTestCases[] =
    {
        {eQCWWAN_ERR_NONE, 1, QMI_NAS_MANUAL_REGISTRATION, &MNRInfo3Digit,
                &changeDurationPer, &mncPcsDigitT,"Valid Registration"},
        {eQCWWAN_ERR_NONE, 2, QMI_NAS_MANUAL_REGISTRATION, &MNRInfo3Digit,
                &changeDurationPC, &mncPcsDigitT, "Valid Registration"},
        {eQCWWAN_ERR_NONE, 3, QMI_NAS_MANUAL_REGISTRATION, &MNRInfo2Digit,
                &changeDurationPer, &mncPcsDigitF, "Valid Registration"},
        {eQCWWAN_ERR_NONE, 4, QMI_NAS_MANUAL_REGISTRATION, &MNRInfo2Digit,
                &changeDurationPC, &mncPcsDigitT, "Valid Registration"},
        {eQCWWAN_ERR_NONE, 5, QMI_NAS_AUTOMATIC_REGISTRATION, &MNRInfo3Digit,
                &changeDurationPer, &mncPcsDigitT, "Valid Registration"},
        {eQCWWAN_ERR_NONE, 6, QMI_NAS_AUTOMATIC_REGISTRATION, &MNRInfo3Digit,
                &changeDurationPC, &mncPcsDigitT, "Valid Registration"},
        {eQCWWAN_ERR_NONE, 7, QMI_NAS_AUTOMATIC_REGISTRATION, &MNRInfo3Digit,
                &changeDurationPer, &mncPcsDigitF, "Valid Registration"},
        {eQCWWAN_ERR_NONE, 8, QMI_NAS_AUTOMATIC_REGISTRATION, &MNRInfo3Digit,
                &changeDurationPC, &mncPcsDigitF, "Valid Registration"},
        {eQCWWAN_ERR_NONE, 9, QMI_NAS_MANUAL_REGISTRATION, &MNRInfo3Digit,
                NULL, NULL, "All Optional Parameter NULL"},
        {eQCWWAN_ERR_NONE, 10, QMI_NAS_AUTOMATIC_REGISTRATION, NULL, NULL,
                NULL, "All Optional Parameter NULL"}
    };

    ULONG nRet;
    nasInitNetworkReg req;
    FILE  *fp = NULL;

    fp = tfopen( "../../nas/test/results/slqsinitnetworkreg.txt", "w" );

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSNasInitNetworkRegTestCases )/
                      sizeof( SLQSNasInitNetworkRegTestCases[0] );
        while ( tc < ncases )
        {
            SLQSNasInitNetworkRegTestCase_t *pd =
                                     &SLQSNasInitNetworkRegTestCases[tc];
            req.regAction = pd->regAction;
            if( pd->pMNRegInfo )
            {
                req.pMNRInfo = &temp;
                req.pMNRInfo->mcc = pd->pMNRegInfo->mcc;
                req.pMNRInfo->mnc = pd->pMNRegInfo->mnc;
                req.pMNRInfo->rat = pd->pMNRegInfo->rat;
            }
            else
            {
                req.pMNRInfo = NULL;
            }
            req.pChangeDuration = pd->pChangeDuration;
            req.pMncPcsDigitStatus = pd->pMncPcsDigitStatus;

            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tc + 1),
                        ncases,
                        pd->desc);

            fprintf( fp, "Parameter Set:\n" );
            fprintf( fp, "regType :%lu\n", req.regAction );
            if( req.pMNRInfo )
            {
                fprintf( fp, "mcc :%d\n", req.pMNRInfo->mcc );
                fprintf( fp, "mnc :%x\n", req.pMNRInfo->mnc );
                fprintf( fp, "rat :%lu\n", req.pMNRInfo->rat );
            }
            if( req.pChangeDuration )
            {
                fprintf( fp, "changeDuration :%lu\n", *(req.pChangeDuration) );
            }
            if( req.pMncPcsDigitStatus )
            {
                fprintf( fp, "mncpcsdigitstatus:%d\n",
                                                   *(req.pMncPcsDigitStatus) );
            }

            nRet = SLQSInitiateNetworkRegistration( &req );

            /* Display result code and text */
            doprintreason ( fp, nRet );

            if ( !nRet )
            {
                fprintf( fp, "Network Registration Initiated\n" );
            }
            tc++;
        }
    }
    if ( fp )
        tfclose( fp );
}
