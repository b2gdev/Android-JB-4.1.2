/**************
 *
 *  Filename: qaGobiApiCatTest.c
 *
 *  Purpose:  Card Application Toolkit (CAT) API test driver function definitions
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

#include "qaGobiApiCat.h"
#include "qaGobiApiCatTest.h"
#include "qatesthelper.h"
#include "qatestproto.h"

/*
 * Name:     doCATSendEnvelopeCommand
 *
 * Purpose:  Perform the tests that call the CATSendEnvelopeCommand() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doCATSendEnvelopeCommand( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    BYTE  *data;
    BYTE  messageSize = 0;
    FILE  *fp = NULL;

    fp = tfopen("../../cat/test/results/catsendenvelopecommand.txt", "w");
    fprintf(fp,"\n\n<CATSendEnvelopeCommand>");

    if (fp)
    {
        while (tCaseNum < MAX_CAT_SEND_ENVELOPE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_CAT_SEND_ENVELOPE_NUM,
                       CATSendEnvelopeCommandTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "cmdID:%lx\n", CATSendEnvelopeCommandTestCases \
                                                   [tCaseNum].cmdID);
            fprintf(fp, "dataLen:%lx\n", CATSendEnvelopeCommandTestCases \
                                                   [tCaseNum].dataLen);

            data = CATSendEnvelopeCommandTestCases[tCaseNum].pData;
            messageSize = CATSendEnvelopeCommandTestCases[tCaseNum].dataLen;
            while (messageSize--)
            {
                fprintf(fp, "%x ", *data);
                data++;
            }
            fprintf(fp,"\n");


            nRet = CATSendEnvelopeCommand(
                            CATSendEnvelopeCommandTestCases[tCaseNum].cmdID,
                            CATSendEnvelopeCommandTestCases[tCaseNum].dataLen,
                            (CATSendEnvelopeCommandTestCases[tCaseNum].pData));

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "CATSendEnvelopeCommand Successful\n");
            }
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doCATSendTerminalResponse
 *
 * Purpose:  Perform the tests that call the CATSendTerminalResponse() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doCATSendTerminalResponse( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../cat/test/results/catsendterminalresponse.txt", "w");
    fprintf(fp,"\n\n<CATSendTerminalResponse>");

    if (fp)
    {
        while (tCaseNum < MAX_CAT_SEND_TR_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_CAT_SEND_TR_NUM,
                       CATSendTerminalResponseTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "refID:%lx\n", CATSendTerminalResponseTestCases \
                                                  [tCaseNum].refID);
            fprintf(fp, "dataLen:%lx\n", CATSendTerminalResponseTestCases \
                                                  [tCaseNum].dataLen);
            fprintf(fp, "data:%s\n",   CATSendTerminalResponseTestCases
                                                  [tCaseNum].data);


            nRet = CATSendTerminalResponse(CATSendTerminalResponseTestCases \
                                                  [tCaseNum].refID,
                                          CATSendTerminalResponseTestCases \
                                                  [tCaseNum].dataLen,
                                          &(CATSendTerminalResponseTestCases \
                                                  [tCaseNum].data[0]) );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "CATSendTerminalResponse Successful\n");
            }
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}
