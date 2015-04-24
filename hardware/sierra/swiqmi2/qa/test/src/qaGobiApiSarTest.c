/**************
 *  Filename: qaGobiApiSarTest.c
 *
 *  Purpose:  Specific Absorption Rate (SAR) API test
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

#include "qaGobiApiSar.h"
#include "qaGobiApiSarTest.h"
#include "qatesthelper.h"
#include "qatestproto.h"

/*
 * Name:    doSLQSGetRfSarState
 *
 * Purpose: Perform the tests that call the SLQSGetRfSarState() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */

void doSLQSGetRfSarState( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../sar/test/results/slqssarrfgetstate.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_SAR_RF_GET_STATE_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_SAR_RF_GET_STATE_TESTCASE_NUM,
                        SARRFGetStateTestCases[tCaseNum].desc);

            nRet = SLQSGetRfSarState(
                       SARRFGetStateTestCases[tCaseNum].pSarrfstate );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "SarRFGetState Successful\n");
                fprintf(fp, "sarrfstate:%lu\n",
                                *SARRFGetStateTestCases[tCaseNum].pSarrfstate);
            }

            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSSetRfSarState
 *
 * Purpose: Perform the tests that call the SLQSSetRfSarState() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSSetRfSarState( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../sar/test/results/slqssetrfsarstate.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_SAR_RF_SET_STATE_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_SAR_RF_SET_STATE_TESTCASE_NUM,
                        SARRFSetStateTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "SAR RF State : %lx\n",\
                    SARRFSetStateTestCases[tCaseNum].rfsarstate);

            nRet = SLQSSetRfSarState(
                       SARRFSetStateTestCases[tCaseNum].rfsarstate );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
                fprintf(fp, "SLQSSetRfSarState Successful\n");

            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}
