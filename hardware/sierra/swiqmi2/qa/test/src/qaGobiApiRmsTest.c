/**************
 *
 *  Filename: qaGobiApiRmsTest.c
 *
 *  Purpose:  Remote Management Service (RMS) API test driver function definitions
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

#include "qaGobiApiRms.h"
#include "qaGobiApiRmsTest.h"
#include "qatesthelper.h"
#include "qatestproto.h"

/*
 * Name:     doGetSMSWake
 *
 * Purpose:  Perform the tests that call the GetSMSWake() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetSMSWake( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../rms/test/results/getsmswake.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_RMS_GET_SMS_WAKE_TESTCASE_NUM )
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_RMS_GET_SMS_WAKE_TESTCASE_NUM,
                       GetSMSWakeTestCases[tCaseNum].desc);

            nRet = GetSMSWake( GetSMSWakeTestCases[tCaseNum].pEnabled,
                               GetSMSWakeTestCases[tCaseNum].pWakeMask );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "GetSMSWake Successful\n");
                fprintf(fp, "SMSwake functionality enabled: %lx\n",enabled );
                fprintf(fp, "SMS wake mask to search for in incoming\
                                                 messages:%lx\n", wakeMask);
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doSetSMSWake
 *
 * Purpose:  Perform the tests that call the SetSMSWake() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSetSMSWake( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../rms/test/results/setsmswake.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_RMS_SET_SMS_WAKE_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_RMS_SET_SMS_WAKE_TESTCASE_NUM,
                       SetSMSWakeTestCases[tCaseNum].desc);

            fprintf(fp, "SMS wake functionality enabled: %lx\n",\
                                SetSMSWakeTestCases[tCaseNum].bEnable );
            fprintf(fp, "SMS wakemask to search for incoming messages: %lx\n",\
                               SetSMSWakeTestCases[tCaseNum].wakeMask );

            nRet = SetSMSWake( SetSMSWakeTestCases[tCaseNum].bEnable,
                               SetSMSWakeTestCases[tCaseNum].wakeMask );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "SetSMSWake Successful\n");
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}
