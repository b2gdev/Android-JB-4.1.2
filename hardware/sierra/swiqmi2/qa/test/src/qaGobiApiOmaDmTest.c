/**************
 *  Filename: qaGobiApiOmaDmTest.c
 *
 *  Purpose:  Open Mobile Alliance Device Management Service (OMADMS) API test
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

#include "qaGobiApiOmadm.h"
#include "qaGobiApiOmaDmTest.h"
#include "qatesthelper.h"
#include "qatestproto.h"

/*
 * Name:    doOMADMStartSession
 *
 * Purpose: Perform the tests that call the OMADMStartSession() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doOMADMSStartSession( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../omadm/test/results/omadmstartsession.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_OMADMS_START_SESSION_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_OMADMS_START_SESSION_TESTCASE_NUM,
                        OMADMSStartSessionTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Session Type : %lx\n",\
                    OMADMSStartSessionTestCases[tCaseNum].sessiontype);

            nRet = OMADMStartSession(
                       OMADMSStartSessionTestCases[tCaseNum].sessiontype );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
                fprintf(fp, "OMADMStartSession Successful\n");

            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doOMADMCancelSession
 *
 * Purpose: Perform the tests that call the OMADMCancelSession() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doOMADMSCancelSession( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../omadm/test/results/omadmcancelsession.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_OMADMS_CANCEL_SESSION_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_OMADMS_CANCEL_SESSION_TESTCASE_NUM,
                        OMADMSCancelSessionTestCases[tCaseNum].desc);

            nRet = OMADMCancelSession();

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
                fprintf(fp, "OMADMCancelSession Successful\n");

            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doOMADMGetSessionInfo
 *
 * Purpose: Perform the tests that call the OMADMGetSessionInfo() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doOMADMSGetSessionInfo( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../omadm/test/results/omadmgetsessioninfo.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_OMADMS_GET_SESSION_INFO_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_OMADMS_GET_SESSION_INFO_TESTCASE_NUM,
                        OMADMSGetSessionInfoTestCases[tCaseNum].desc);

            nRet = OMADMGetSessionInfo(
                       OMADMSGetSessionInfoTestCases[tCaseNum].pSessionState,
                       OMADMSGetSessionInfoTestCases[tCaseNum].pSessionType,
                       OMADMSGetSessionInfoTestCases[tCaseNum].pFailureReason,
                       OMADMSGetSessionInfoTestCases[tCaseNum].pRetryCount,
                       OMADMSGetSessionInfoTestCases[tCaseNum].pSessionPause,
                       OMADMSGetSessionInfoTestCases[tCaseNum].pTimeRemaining);

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "SLQSOMADMGetSessionInfo Successful\n");
                fprintf(fp, "OMADM SessionState : %lx\n", \
                   *(OMADMSGetSessionInfoTestCases[tCaseNum].pSessionState) );
                fprintf(fp, "OMADM SessionType : %lx\n",  \
                   *(OMADMSGetSessionInfoTestCases[tCaseNum].pSessionType) );
                fprintf(fp, "OMADM pFailureReason : %lx\n",  \
                   *(OMADMSGetSessionInfoTestCases[tCaseNum].pFailureReason) );
                fprintf(fp, "OMADM pRetryCount : %d\n",   \
                   *(OMADMSGetSessionInfoTestCases[tCaseNum].pRetryCount) );
                fprintf(fp, "OMADM pSessionPause : %x\n",   \
                   *(OMADMSGetSessionInfoTestCases[tCaseNum].pSessionPause) );
                fprintf(fp, "OMADM pTimeRemaining : %x\n",   \
                   *(OMADMSGetSessionInfoTestCases[tCaseNum].pTimeRemaining) );
            }
            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doOMADMGetPendingNIA
 *
 * Purpose: Perform the tests that call the OMADMGetPendingNIA() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doOMADMSGetPendingNIA( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../omadm/test/results/omadmgetpendingNIA.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_OMADMS_GET_PENDING_NIA_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_OMADMS_GET_PENDING_NIA_TESTCASE_NUM,
                        OMADMSGetPendingNIATestCases[tCaseNum].desc);

            nRet = OMADMGetPendingNIA(
                           OMADMSGetPendingNIATestCases[tCaseNum].pSessionType,
                           OMADMSGetPendingNIATestCases[tCaseNum].pSessionID );

            fprintf(fp, "Return Code: %lu\n", nRet);
            doprintreason( fp, nRet );

            if ( !nRet )
            {
                fprintf(fp, "SLQSOMADMGetPendingNIA Successful\n");
                fprintf(fp, "OMADM NIA SessionType : %lu\n",  \
                   *(OMADMSGetPendingNIATestCases[tCaseNum].pSessionType) );
                fprintf(fp, "OMADM NIA SessionID   : %x\n",  \
                   *(OMADMSGetPendingNIATestCases[tCaseNum].pSessionID) );
            }
            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}
