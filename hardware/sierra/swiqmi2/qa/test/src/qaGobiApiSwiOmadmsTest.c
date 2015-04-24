/**************
 *  Filename: qaGobiApiSwiOmadmsTest.c
 *
 *  Purpose:  Open Mobile Alliance Device ManagementService (OMADMS) API test
 *            driver function definitions
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

#include "qaGobiApiSwiOmadms.h"
#include "qaGobiApiSwiOmadmsTest.h"
#include "qatesthelper.h"
#include "qatestproto.h"

/*******************
    TEST DATA
 *******************/
/* Constants */

/* Global Variables */

/* data for SLQSOMADMSetSettings2 */
local BYTE FOTAdownloadInvalid = 2;
local BYTE FOTAUpdateInvalid   = 2;

local BYTE FOTAdownloadDisable = 0;
local BYTE FOTAUpdateDisable   = 0;
local BYTE AutoSdmDisable      = 0;

local BYTE FOTAdownloadEnable = 1;
local BYTE FOTAUpdateEnable   = 1;
local BYTE AutoSdmEnable      = 1;

/*******************
  TEST FUNCTIONS
 *******************/

/**
 * Name:    doSLQSOMADMStartSession
 *
 * Purpose: Perform the tests that call the SLQSOMADMStartSession() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSOMADMStartSession( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../swioma/test/results/slqsomadmstartsession.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_SLQS_OMADM_START_SESSION_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_SLQS_OMADM_START_SESSION_TESTCASE_NUM,
                        SLQSOMADMStartSessionTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Session Type : %lx\n",\
                    SLQSOMADMStartSessionTestCases[tCaseNum].sessiontype);

            nRet = SLQSOMADMStartSession(
                        SLQSOMADMStartSessionTestCases[tCaseNum].sessiontype );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
                fprintf(fp, "SLQSOMADMStartSession Successful\n");

            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/**
 * Name:    doSLQSOMADMCancelSession
 *
 * Purpose: Perform the tests that call the SLQSOMADMCancelSession() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSOMADMCancelSession( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../swioma/test/results/slqsomadmcancelsession.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_SLQS_OMADM_CANCEL_SESSION_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_SLQS_OMADM_CANCEL_SESSION_TESTCASE_NUM,
                        SLQSOMADMCancelSessionTestCases[tCaseNum].desc);

            nRet = SLQSOMADMCancelSession(
                            SLQSOMADMCancelSessionTestCases[tCaseNum].session );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
                fprintf(fp, "SLQSOMADMCancelSession Successful\n");

            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/**
 * Name:    doSLQSOMADMGetSessionInfo
 *
 * Purpose: Perform the tests that call the SLQSOMADMGetSessionInfo() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSOMADMGetSessionInfo( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../swioma/test/results/slqsomadmgetsessioninfo.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_SLQS_OMADM_GET_SESSION_INFO_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_SLQS_OMADM_GET_SESSION_INFO_TESTCASE_NUM,
                        SLQSOMADMGetSessionInfoTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Session Type : %lx\n",\
                   *(SLQSOMADMGetSessionInfoTestCases[tCaseNum].psessionType) );

            nRet = SLQSOMADMGetSessionInfo(
                    SLQSOMADMGetSessionInfoTestCases[tCaseNum].psessionType,
                    SLQSOMADMGetSessionInfoTestCases[tCaseNum].psessionState );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "SLQSOMADMGetSessionInfo Successful\n");
                fprintf(fp, "OMADM SessionType : %lx\n", \
                   *(SLQSOMADMGetSessionInfoTestCases[tCaseNum].psessionType) );
                fprintf(fp, "OMADM SessionState : %lx\n", sessionState);
            }
            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/**
 * Name:    doSLQSOMADMSendSelection
 *
 * Purpose: Perform the tests that call the SLQSOMADMSendSelection() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSOMADMSendSelection( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../swioma/test/results/slqsomadmsendselection.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_SLQS_OMADM_SEND_SELECTION_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_SLQS_OMADM_SEND_SELECTION_TESTCASE_NUM,
                        SLQSOMADMSendSelectionTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Selection : %lx\n",\
                    SLQSOMADMSendSelectionTestCases[tCaseNum].selection);

            nRet = SLQSOMADMSendSelection(\
                       SLQSOMADMSendSelectionTestCases[tCaseNum].selection );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
                fprintf(fp, "SLQSOMADMSendSelection Successful\n");

            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/**
 * Name:    doSLQSOMADMGetSettings
 *
 * Purpose: Perform the tests that call the SLQSOMADMGetSettings() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSOMADMGetSettings( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../swioma/test/results/slqsomadmgetsettings.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_SLQS_OMADM_GET_SETTINGS_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_SLQS_OMADM_GET_SETTINGS_TESTCASE_NUM,
                        SLQSOMADMGetSettingsTestCases[tCaseNum].desc);

            nRet = SLQSOMADMGetSettings(\
                       SLQSOMADMGetSettingsTestCases[tCaseNum].pbOMADMEnabled,
                       SLQSOMADMGetSettingsTestCases[tCaseNum].pbFOTADownload,
                       SLQSOMADMGetSettingsTestCases[tCaseNum].pbFOTAUpdate );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "SLQSOMADMGetSettings Successful\n");
                fprintf(fp, "OMA-DM Enabled : %lx\n", bOMADMEnabled);
                fprintf(fp, "FOTA Download : %lx\n", bFOTADownload);
                fprintf(fp, "FOTA Update: %lx\n", bFOTAUpdate);
            }
            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/**
 * Name:    doSLQSOMADMSetSettings
 *
 * Purpose: Perform the tests that call the SLQSOMADMSetSettings() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSOMADMSetSettings( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../swioma/test/results/slqsomadmsetsettings.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_SLQS_OMADM_SET_SETTINGS_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_SLQS_OMADM_SET_SETTINGS_TESTCASE_NUM,
                        SLQSOMADMSetSettingsTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "FOTA Download : %lx\n",\
                    SLQSOMADMSetSettingsTestCases[tCaseNum].bFOTADownload);
            fprintf(fp, "FOTA Update : %lx\n",\
                    SLQSOMADMSetSettingsTestCases[tCaseNum].bFOTAUpdate);

            nRet = SLQSOMADMSetSettings(\
                                SLQSOMADMSetSettingsTestCases[tCaseNum].\
                                                           bFOTADownload,
                                SLQSOMADMSetSettingsTestCases[tCaseNum].\
                                                           bFOTAUpdate  );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
                fprintf(fp, "SLQSOMADMSetSettings Successful\n");

            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:     doSLQSOMADMSetSettings2
 *
 * Purpose:  Perform the tests that call the SLQSOMADMSetSettings2() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSOMADMSetSettings2( void )
{
    ULONG                       nRet;
    FILE                        *fp = NULL;
    SLQSOMADMSettingsReqParams  slqsOMADMSettings;

    /* Define the test cases to be executed */
    SLQSOMADMSetSettings2TestCase_t SLQSOMADMSetSettings2TestCases[] =
    {
        { eQCWWAN_ERR_INVALID_ARG, 1, FOTAdownloadInvalid, FOTAUpdateInvalid, NULL,
          "Invalid Test Case - All params NULL"},

        { eQCWWAN_ERR_NONE, 2, FOTAdownloadDisable, FOTAUpdateDisable, NULL,
          "Valid test Case - One Param NULL" },

        { eQCWWAN_ERR_NONE, 3, FOTAdownloadDisable, FOTAUpdateDisable,
          &AutoSdmDisable, "Valid test Case - all disable" },

        { eQCWWAN_ERR_NONE, 4, FOTAdownloadEnable, FOTAUpdateEnable,
          &AutoSdmEnable, "Valid test Case" },
    };

    fp = tfopen( "../../swioma/test/results/slqsomadmsetsettings2.txt", "w" );
    if ( fp )
    {
        BYTE tCaseNum     = 0;
        BYTE ncases = sizeof( SLQSOMADMSetSettings2TestCases ) /
                      sizeof( SLQSOMADMSetSettings2TestCases[0] );

        while( tCaseNum < ncases )
        {
            SLQSOMADMSetSettings2TestCase_t *pd =
                &SLQSOMADMSetSettings2TestCases[tCaseNum];

            slqsOMADMSettings.FOTAdownload = pd->pFOTADownload;
            slqsOMADMSettings.FOTAUpdate   = pd->pFOTAUpdate;
            slqsOMADMSettings.pAutosdm     = pd->pAutoSdm;

            fprintf( fp,"\n\nTest Case %d / %d : %s\n",
                     (tCaseNum + 1),
                     ncases,
                     pd->desc );
            fprintf( fp, "Parameters Set:\n" );
            fprintf( fp, "FOTA Automatic Download : 0x%x\n",         pd->pFOTADownload );
            fprintf( fp, "FOTA Automatic Update : 0x%x\n",           pd->pFOTAUpdate );
            IFPRINTF( fp, "OMA Automatic UI Alert Response : 0x%x\n", pd->pAutoSdm );

            nRet = SLQSOMADMSetSettings2( &slqsOMADMSettings );

            /* Display result code and text */
            doprintreason( fp, nRet );
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doSLQSOMADMSetSettings2
 *
 * Purpose:  Perform the tests that call the SLQSOMADMSetSettings2() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSOMADMGetSettings2( void )
{
    ULONG             nRet;
    FILE              *fp = NULL;
    SLQSOMADMSettings slqsOMADMSettings;
    ULONG             GetOMADMEnabled;
    BYTE              GetFOTAdownload;
    BYTE              GetFOTAUpdate;
    BYTE              GetAutoSdm;

    /* Define the test cases to be executed */
    SLQSOMADMGetSettings2TestCase_t SLQSOMADMGetSettings2TestCases[] =
    {
        { eQCWWAN_ERR_INVALID_ARG, 1, NULL, NULL, NULL, NULL,
          "Invalid Test Case - All params NULL"},

        { eQCWWAN_ERR_NONE, 2, &GetOMADMEnabled, &GetFOTAdownload, &GetFOTAUpdate, NULL,
          "Valid test Case - One Param NULL" },

        { eQCWWAN_ERR_NONE, 3,  &GetOMADMEnabled, &GetFOTAdownload, &GetFOTAUpdate,
          &GetAutoSdm, "Valid test Case - all parameters" },

    };

    fp = tfopen( "../../swioma/test/results/slqsomadmgetsettings2.txt", "w" );
    if ( fp )
    {
        BYTE tCaseNum     = 0;
        BYTE ncases = sizeof( SLQSOMADMGetSettings2TestCases ) /
                      sizeof( SLQSOMADMGetSettings2TestCases[0] );

        while( tCaseNum < ncases )
        {
            SLQSOMADMGetSettings2TestCase_t *pd =
                &SLQSOMADMGetSettings2TestCases[tCaseNum];

            slqsOMADMSettings.pOMADMEnabled = pd->pOMADMEnabled;
            slqsOMADMSettings.pFOTAdownload = pd->pFOTADownload;
            slqsOMADMSettings.pFOTAUpdate   = pd->pFOTAUpdate;
            slqsOMADMSettings.pAutosdm      = pd->pAutoSdm;

            fprintf( fp,"\n\nTest Case %d / %d : %s\n",
                     (tCaseNum + 1),
                     ncases,
                     pd->desc );

            nRet = SLQSOMADMGetSettings2( &slqsOMADMSettings );

            /* Display result code and text */
            doprintreason( fp, nRet );
            if (!nRet)
            {
                fprintf( fp,"Details for OMADM Settings \n" );

                IFPRINTF( fp, "\nOMA-DM Enabled : 0x%lx",            slqsOMADMSettings.pOMADMEnabled );
                IFPRINTF( fp, "\nFOTA Automatic Download : 0x%x",    slqsOMADMSettings.pFOTAdownload );
                IFPRINTF( fp, "\nFOTA Automatic Update : 0x%x"  ,    slqsOMADMSettings.pFOTAUpdate );
                IFPRINTF( fp, "\nOMA Auto UI Alert Response : 0x%x", slqsOMADMSettings.pAutosdm );
            }
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}
