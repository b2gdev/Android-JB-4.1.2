/**************
 *  Filename: qaGobiApiPdsTest.c
 *
 *  Purpose:  Position Determination Service (PDS) API test driver function
 *            definitions
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

#include "qaGobiApiPds.h"
#include "qaGobiApiPdsTest.h"
#include "qatesthelper.h"
#include "qatestproto.h"

/**
 * Name:    doGetPDSState
 *
 * Purpose: Perform the tests that call the GetPDSState() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doGetPDSState( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    ULONG enabledStatus;
    ULONG trackingStatus;
    FILE  *fp = NULL;

    fp = tfopen("../../pds/test/results/getpdsstate.txt", "w");
    if (fp)
    {
        while (tCaseNum < MAX_GET_PDS_STATE_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_GET_PDS_STATE_TESTCASE_NUM,
                        GetPDSStateTestCases[tCaseNum].desc);

            nRet = GetPDSState( &enabledStatus,
                                &trackingStatus );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "GetPDSState Successful\n");
                fprintf(fp, "GPS Enabled Status : %lx\n", enabledStatus);
                fprintf(fp, "GPS Tracking Status : %lx\n",trackingStatus);
            }
            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/**
 * Name:    doSetPDSState
 *
 * Purpose: Perform the tests that call the SetPDSState() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSetPDSState( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../pds/test/results/setpdsstate.txt", "w");
    if (fp)
    {
        while (tCaseNum < MAX_SET_PDS_STATE_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_SET_PDS_STATE_TESTCASE_NUM,
                        SetPDSStateTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "enable : %lu\n",\
                    SetPDSStateTestCases[tCaseNum].enable);

            nRet = SetPDSState( SetPDSStateTestCases[tCaseNum].enable );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "SetPDSState Successful\n");
            }
            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/**
 * Name:    doStartPDSTrackingSessionExt
 *
 * Purpose: Perform the tests that call the StartPDSTrackingSessionExt() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doStartPDSTrackingSessionExt( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../pds/test/results/startpdstrackingsessionext.txt", "w");
    if (fp)
    {
        while (tCaseNum < MAX_START_TRACKING_SESSION_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_START_TRACKING_SESSION_TESTCASE_NUM,
                        StartTrackingSessionExtTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "sessionControl : %xH\n",\
                   StartTrackingSessionExtTestCases[tCaseNum].sessionControl );
            fprintf(fp, "sessionType : %xH\n",\
                   StartTrackingSessionExtTestCases[tCaseNum].sessionType );
            fprintf(fp, "sessionOperation : %xH\n",\
                   StartTrackingSessionExtTestCases[tCaseNum].
                                                        sessionOperation );
            fprintf(fp, "sessionServerOption : %xH\n",\
                   StartTrackingSessionExtTestCases[tCaseNum].
                                                        sessionServerOption );
            fprintf(fp, "fixTimeout : %xH\n",\
                   StartTrackingSessionExtTestCases[tCaseNum].fixTimeout );
            fprintf(fp, "fixInterval : %lx\n",\
                   StartTrackingSessionExtTestCases[tCaseNum].fixInterval );
            fprintf(fp, "fixCount : %lx\n",\
                   StartTrackingSessionExtTestCases[tCaseNum].fixCount );
            fprintf(fp, "fixAccuracy : %lx\n",\
                   StartTrackingSessionExtTestCases[tCaseNum].fixAccuracy ) ;

            nRet = StartPDSTrackingSessionExt(
                   StartTrackingSessionExtTestCases[tCaseNum].sessionControl,
                   StartTrackingSessionExtTestCases[tCaseNum].sessionType,
                   StartTrackingSessionExtTestCases[tCaseNum].sessionOperation,
                   StartTrackingSessionExtTestCases[tCaseNum].
                                                       sessionServerOption,
                   StartTrackingSessionExtTestCases[tCaseNum].fixTimeout,
                   StartTrackingSessionExtTestCases[tCaseNum].fixInterval,
                   StartTrackingSessionExtTestCases[tCaseNum].fixCount,
                   StartTrackingSessionExtTestCases[tCaseNum].fixAccuracy );

            /* Display result code and text */
            doprintreason (fp, nRet);


            if ( !nRet )
            {
                fprintf(fp, "StartPDSTrackignSessionExt Successful\n");
            }
            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/**
 * Name:    doStopPDSTrackingSession
 *
 * Purpose: Perform the tests that call the StopPDSTrackingSession() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doStopPDSTrackingSession( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../pds/test/results/stoppdstrackingsession.txt", "w");
    if (fp)
    {
        while (tCaseNum < MAX_STOP_TRACKING_SESSION_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_STOP_TRACKING_SESSION_TESTCASE_NUM,
                        StopTrackingSessionTestCases[tCaseNum].desc);

            nRet = StopPDSTrackingSession();

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
                fprintf(fp, "StopPDSTrackingSession Successful\n");

            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/**
 * Name:    doGetPDSDefaults
 *
 * Purpose: Perform the tests that call the GetPDSDefaults() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doGetPDSDefaults( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../pds/test/results/getpdsdefaults.txt", "w");
    if (fp)
    {
        while ( tCaseNum < MAX_GET_PDS_DEFAULTS_TESTCASE_NUM )
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_GET_PDS_DEFAULTS_TESTCASE_NUM,
                        GetPDSDefaultsTestCases[tCaseNum].desc);

            nRet = GetPDSDefaults(
                             GetPDSDefaultsTestCases[tCaseNum].poperation,
                             GetPDSDefaultsTestCases[tCaseNum].pmaxtimeout,
                             GetPDSDefaultsTestCases[tCaseNum].pinterval,
                             GetPDSDefaultsTestCases[tCaseNum].paccuracy );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "GetPDSDetaults Successful\n");
                fprintf(fp, "Operating Mode : %lu\n", operation);
                fprintf(fp, "Max. Time for each Fix : %d\n", maxtimeout);
                fprintf(fp, "Interval between Fix requests : %lu\n", interval);
                fprintf(fp, "Current Accuracy Threshold : %lu\n", accuracy);
            }
            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/**
 * Name:    doSetPDSDefaults
 *
 * Purpose: Perform the tests that call the SetPDSDefaults() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSetPDSDefaults( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../pds/test/results/setpdsdefaults.txt", "w");
    if (fp)
    {
        while ( tCaseNum < MAX_SET_PDS_DEFAULTS_TESTCASE_NUM )
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_SET_PDS_DEFAULTS_TESTCASE_NUM,
                        SetPDSDefaultsTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Operating Mode : %lu\n",\
                    SetPDSDefaultsTestCases[tCaseNum].operation);
            fprintf(fp, "Max. Time for each Fix : %d\n",\
                    SetPDSDefaultsTestCases[tCaseNum].timeout);
            fprintf(fp, "Interval between Fix requests : %lu\n",\
                    SetPDSDefaultsTestCases[tCaseNum].interval);
            fprintf(fp, "Current Accuracy Threshold : %lu\n",\
                    SetPDSDefaultsTestCases[tCaseNum].accuracy);

            nRet = SetPDSDefaults( SetPDSDefaultsTestCases[tCaseNum].operation,
                                   SetPDSDefaultsTestCases[tCaseNum].timeout,
                                   SetPDSDefaultsTestCases[tCaseNum].interval,
                                   SetPDSDefaultsTestCases[tCaseNum].accuracy );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "SetPDSDefaults Successful\n");
            }
            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/**
 * Name:    doGetXTRAAutomaticDownload
 *
 * Purpose: Perform the tests that call the GetXTRAAutomaticDownload() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doGetXTRAAutomaticDownload( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../pds/test/results/getxtraautomaticdownload.txt", "w");
    if (fp)
    {
        while ( tCaseNum < MAX_GET_XTRA_AUTO_DOWNLOAD_TESTCASE_NUM )
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_GET_XTRA_AUTO_DOWNLOAD_TESTCASE_NUM,
                        GetXtraAutoDownloadTestCases[tCaseNum].desc);

            nRet = GetXTRAAutomaticDownload(
                              GetXtraAutoDownloadTestCases[tCaseNum].pEnabled,
                              GetXtraAutoDownloadTestCases[tCaseNum].pInterval );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "GetXTRAAutomaticDownload Successful\n");
                fprintf(fp, "Automatic XTRA download status: %lu\n",
                        Enabled);
                fprintf(fp, "Interval between XTRA downloads: %d\n",
                        Interval);
            }
            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/**
 * Name:    doSetXTRAAutomaticDownload
 *
 * Purpose: Perform the tests that call the SetXTRAAutomaticDownload() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSetXTRAAutomaticDownload( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../pds/test/results/setxtraautomaticdownload.txt", "w");
    if (fp)
    {
        while ( tCaseNum < MAX_SET_XTRA_AUTO_DOWNLOAD_TESTCASE_NUM )
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_SET_XTRA_AUTO_DOWNLOAD_TESTCASE_NUM,
                        SetXtraAutoDownloadTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Automatic XTRA download status: %lu\n",\
                    SetXtraAutoDownloadTestCases[tCaseNum].bEnabled);
            fprintf(fp, "Interval between XTRA downloads : %d\n",\
                    SetXtraAutoDownloadTestCases[tCaseNum].interval);

            nRet = SetXTRAAutomaticDownload(
                           SetXtraAutoDownloadTestCases[tCaseNum].bEnabled,
                           SetXtraAutoDownloadTestCases[tCaseNum].interval );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "SetXTRAAutomaticDownload Successful\n");
            }
            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/**
 * Name:    doGetXTRANetwork
 *
 * Purpose: Perform the tests that call the GetXTRANetwork() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doGetXTRANetwork( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../pds/test/results/getxtranetwork.txt", "w");
    if (fp)
    {
        while ( tCaseNum < MAX_GET_XTRA_NETWORK_TESTCASE_NUM )
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_GET_XTRA_NETWORK_TESTCASE_NUM,
                        GetXtraNetworkTestCases[tCaseNum].desc);

            nRet = GetXTRANetwork(
                          GetXtraNetworkTestCases[tCaseNum].pPreference );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "GetXTRANetwork Successful\n");
                fprintf(fp, "XTRA WWAN network preference: %lu\n", preference);
            }
            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/**
 * Name:    doSetXTRANetwork
 *
 * Purpose: Perform the tests that call the SetXTRANetwork() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSetXTRANetwork( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../pds/test/results/setxtranetwork.txt", "w");
    if (fp)
    {
        while ( tCaseNum < MAX_SET_XTRA_NETWORK_TESTCASE_NUM )
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_SET_XTRA_NETWORK_TESTCASE_NUM,
                        SetXtraNetworkTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Automatic XTRA download status: %lu\n",\
                    SetXtraNetworkTestCases[tCaseNum].preference);


            nRet = SetXTRANetwork(
                     SetXtraNetworkTestCases[tCaseNum].preference );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "SetXTRANetwork Successful\n");
            }
            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/**
 * Name:    doGetServiceAutomaticTracking
 *
 * Purpose: Perform the tests that call the GetServiceAutomaticTracking() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doGetServiceAutomaticTracking( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../pds/test/results/getserviceautomatictracking.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_GET_SERVICE_AUTO_TRACK_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_GET_SERVICE_AUTO_TRACK_TESTCASE_NUM,
                        GetServiceAutomaticTrackingTestCases[tCaseNum].desc);

            nRet = GetServiceAutomaticTracking(\
                      GetServiceAutomaticTrackingTestCases[tCaseNum].pbAuto );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "GetServiceAutomaticTracking Successful\n");
                fprintf(fp, "Automatic Tracking Status : %lx\n", bAuto);
            }
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/**
 * Name:    doSetServiceAutomaticTracking
 *
 * Purpose: Perform the tests that call the SetServiceAutomaticTracking() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSetServiceAutomaticTracking( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../pds/test/results/setserviceautomatictracking.txt", "w");
    if (fp)
    {
        while (tCaseNum < MAX_SET_SERVICE_AUTO_TRACK_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_SET_SERVICE_AUTO_TRACK_TESTCASE_NUM,
                        SetServiceAutomaticTrackingTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "bAuto : %lu\n",\
                    SetServiceAutomaticTrackingTestCases[tCaseNum].bAuto);

            nRet = SetServiceAutomaticTracking(\
                       SetServiceAutomaticTrackingTestCases[tCaseNum].bAuto );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
                fprintf(fp, "SetServiceAutomaticTracking Successful\n");

            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/**
 * Name:    doGetPortAutomaticTracking
 *
 * Purpose: Perform the tests that call the GetPortAutomaticTracking() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doGetPortAutomaticTracking( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../pds/test/results/getportautomatictracking.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_GET_PORT_AUTO_TRACK_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_GET_PORT_AUTO_TRACK_TESTCASE_NUM,
                        GetPortAutomaticTrackingTestCases[tCaseNum].desc);

            nRet = GetPortAutomaticTracking(\
                      GetPortAutomaticTrackingTestCases[tCaseNum].pbAuto );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "GetPortAutomaticTracking Successful\n");
                fprintf(fp, "Automatic Tracking Status : %lx\n", config);
            }
            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/**
 * Name:    doSetPortAutomaticTracking
 *
 * Purpose: Perform the tests that call the SetPortAutomaticTracking() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSetPortAutomaticTracking( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../pds/test/results/setportautomatictracking.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_SET_PORT_AUTO_TRACK_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_SET_PORT_AUTO_TRACK_TESTCASE_NUM,
                        SetPortAutomaticTrackingTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "bAuto : %lx\n",\
                    SetPortAutomaticTrackingTestCases[tCaseNum].bAuto);

            nRet = SetPortAutomaticTracking(\
                       SetPortAutomaticTrackingTestCases[tCaseNum].bAuto );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
                fprintf(fp, "SetPortAutomaticTracking Successful\n");

            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/**
 * Name:    doForceXTRADownload
 *
 * Purpose: Perform the tests that call the ForceXTRADownload() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doForceXTRADownload( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../pds/test/results/forcextradownload.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_PDS_FORCED_XTRA_DOWNLOAD_TESTCASE_NUM )
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_PDS_FORCED_XTRA_DOWNLOAD_TESTCASE_NUM,
                        ForceXTRADownloadTestCases[tCaseNum].desc);

            nRet = ForceXTRADownload();

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "ForceXTRADownload Successful\n");
            }
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/**
 * Name:    doGetXTRAValidity
 *
 * Purpose: Perform the tests that call the GetXTRAValidity() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doGetXTRAValidity( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../pds/test/results/getxtravalidity.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_GET_XTRA_VALIDITY_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_GET_XTRA_VALIDITY_TESTCASE_NUM,
                        GetXTRAValidityTestCases[tCaseNum].desc);

            nRet = GetXTRAValidity(\
                      GetXTRAValidityTestCases[tCaseNum].pGPSWeek,
                      GetXTRAValidityTestCases[tCaseNum].pGPSWeekOffset,
                      GetXTRAValidityTestCases[tCaseNum].pDuration );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "GetXTRAValidity Successful\n");
                fprintf(fp, "GPS week of validity period: %d\n", gPSWeek);
                fprintf(fp, "GPS week offset of validity \
                                            period: %d\n", gPSWeekOffset);
                fprintf(fp, "Length of validity period: %d\n", duration);
            }
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/**
 * Name:    doResetPDSData
 *
 * Purpose: Perform the tests that call the ResetPDSData() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doResetPDSData ( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../pds/test/results/resetpdsdata.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_RESET_PDS_DATA_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_RESET_PDS_DATA_TESTCASE_NUM,
                        ResetPDSDataTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");

            if ( ResetPDSDataTestCases[tCaseNum].pGPSDataMask )
            fprintf(fp, "GPS week of validity period: %lx\n",\
                          *(ResetPDSDataTestCases[tCaseNum].pGPSDataMask) );

            if ( ResetPDSDataTestCases[tCaseNum].pCellDataMask )
            fprintf(fp, "GPS week of validity period: %lx\n",\
                          *(ResetPDSDataTestCases[tCaseNum].\
                                                  pCellDataMask) );

            nRet = ResetPDSData(\
                      ResetPDSDataTestCases[tCaseNum].pGPSDataMask,
                      ResetPDSDataTestCases[tCaseNum].pCellDataMask );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "ResetPDSData Successful\n");
            }
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/**
 * Name:    PDSInjectTimeReference
 *
 * Purpose: Perform the tests that call the PDSInjectTimeReference() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doPDSInjectTimeReference ( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../pds/test/results/pdsinjecttimereference.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_PDS_INJECT_TIME_REFERENCE_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_PDS_INJECT_TIME_REFERENCE_TESTCASE_NUM,
                        PDSInjectTimeReferenceTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "System time: %llu\n", \
                          PDSInjectTimeReferenceTestCases[tCaseNum].systemTime);
            fprintf(fp, "System time discontinuities: %d\n", \
                          PDSInjectTimeReferenceTestCases[tCaseNum]. \
                                                  systemDiscontinuities);

            nRet = PDSInjectTimeReference(\
                      PDSInjectTimeReferenceTestCases[tCaseNum].systemTime,
                      PDSInjectTimeReferenceTestCases[tCaseNum]. \
                                             systemDiscontinuities );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "PDSInjectTimeReference Successful\n");
            }
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSSetAGPSConfig
 *
 * Purpose: Perform the tests that call the SLQSSetAGPSConfig() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSSetAGPSConfig( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../pds/test/results/SLQSSetAGPSConfig.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_SLQS_SET_AGPS_CONFIG_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_SLQS_SET_AGPS_CONFIG_TESTCASE_NUM,
                        SLQSSetAGPSConfigTestCases[tCaseNum].desc);

            nRet = SLQSSetAGPSConfig(
                       SLQSSetAGPSConfigTestCases[tCaseNum].pServerAddress,
                       SLQSSetAGPSConfigTestCases[tCaseNum].pServerPort,
                       SLQSSetAGPSConfigTestCases[tCaseNum].pServerUrl,
                       SLQSSetAGPSConfigTestCases[tCaseNum].pServerUrlLength );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                if(SLQSSetAGPSConfigTestCases[tCaseNum].pServerAddress)
                    fprintf(fp, "\tServer Address : %lx\n",
                            *(SLQSSetAGPSConfigTestCases[tCaseNum].pServerAddress) );
                if(SLQSSetAGPSConfigTestCases[tCaseNum].pServerPort)
                    fprintf(fp, "\tServer Port : %lx\n",
                            *(SLQSSetAGPSConfigTestCases[tCaseNum].pServerPort) );
                if(SLQSSetAGPSConfigTestCases[tCaseNum].pServerUrl)
                    fprintf(fp, "\tServer URL : %s\n",
                            SLQSSetAGPSConfigTestCases[tCaseNum].pServerUrl );
                if(SLQSSetAGPSConfigTestCases[tCaseNum].pServerUrlLength)
                    fprintf(fp, "\tServer URL Length : %xH\n",
                            *(SLQSSetAGPSConfigTestCases[tCaseNum].pServerUrlLength) );
            }
            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSPDSInjectAbsoluteTimeReference
 *
 * Purpose: Perform the tests that call the SLQSPDSInjectAbsoluteTimeReference() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSPDSInjectAbsoluteTimeReference ( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../pds/test/results/SLQSPDSInjectAbsoluteTimeReference.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_PDS_INJECT_ABSOLUTE_TIME_REFERENCE_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_PDS_INJECT_ABSOLUTE_TIME_REFERENCE_TESTCASE_NUM,
                        PDSInjectAbsoluteTimeReferenceTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Time MSec: %llu\n", \
                          PDSInjectAbsoluteTimeReferenceTestCases[tCaseNum].timeMsec);
            fprintf(fp, "Time Uncertailty: %lu\n", \
                          PDSInjectAbsoluteTimeReferenceTestCases[tCaseNum].timeUncMsec);
            fprintf(fp, "Time Base: %d\n", \
                          PDSInjectAbsoluteTimeReferenceTestCases[tCaseNum].timeBase);
            fprintf(fp, "Force Flag: %d\n", \
                          PDSInjectAbsoluteTimeReferenceTestCases[tCaseNum].forceFlag);

            nRet = SLQSPDSInjectAbsoluteTimeReference(\
                      PDSInjectAbsoluteTimeReferenceTestCases[tCaseNum].timeMsec,
                      PDSInjectAbsoluteTimeReferenceTestCases[tCaseNum].timeUncMsec,
                      PDSInjectAbsoluteTimeReferenceTestCases[tCaseNum].timeBase,
                      PDSInjectAbsoluteTimeReferenceTestCases[tCaseNum].forceFlag );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "SLQSPDSInjectAbsoluteTimeReference Successful\n");
            }
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSGetAGPSConfig
 *
 * Purpose: Perform the tests that call the SLQSGetAGPSConfig() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSGetAGPSConfig( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../pds/test/results/SLQSGetAGPSConfig.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_SLQS_GET_AGPS_CONFIG_TESTCASE_NUM)
        {
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_SLQS_GET_AGPS_CONFIG_TESTCASE_NUM,
                        SLQSGetAGPSConfigTestCases[tCaseNum].desc);

            nRet = SLQSGetAGPSConfig(
                       SLQSGetAGPSConfigTestCases[tCaseNum].pServerAddress,
                       SLQSGetAGPSConfigTestCases[tCaseNum].pServerPort,
                       SLQSGetAGPSConfigTestCases[tCaseNum].pServerUrl,
                       SLQSGetAGPSConfigTestCases[tCaseNum].pServerUrlLength );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                if(SLQSGetAGPSConfigTestCases[tCaseNum].pServerAddress)
                    fprintf(fp, "\tServer Address : %lx\n",
                            *(SLQSGetAGPSConfigTestCases[tCaseNum].pServerAddress) );
                if(SLQSGetAGPSConfigTestCases[tCaseNum].pServerPort)
                    fprintf(fp, "\tServer Port : %lx\n",
                            *(SLQSGetAGPSConfigTestCases[tCaseNum].pServerPort) );
                if(SLQSGetAGPSConfigTestCases[tCaseNum].pServerUrl)
                    fprintf(fp, "\tServer URL : %s\n",
                            SLQSGetAGPSConfigTestCases[tCaseNum].pServerUrl );
                if(SLQSGetAGPSConfigTestCases[tCaseNum].pServerUrlLength)
                    fprintf(fp, "\tServer URL Length : %xH\n",
                            *(SLQSGetAGPSConfigTestCases[tCaseNum].pServerUrlLength) );
            }
            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSPDSInjectPositionData
 *
 * Purpose: Perform the tests that call the SLQSPDSInjectPositionData() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSPDSInjectPositionData( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;
    struct PDSPositionData curPositionData;

    fp = tfopen("../../pds/test/results/SLQSPDSInjectPositionData.txt", "w");

    if( fp )
    {
        while( tCaseNum < MAX_SLQS_PDS_INJECT_POSITION_DATA_TESTCASE_NUM )
        {
            const SLQSPDSInjectPositionData_t *curTestCase =
                &(SLQSPDSInjectPositionDataTestCases[tCaseNum]);
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_SLQS_PDS_INJECT_POSITION_DATA_TESTCASE_NUM,
                        SLQSPDSInjectPositionDataTestCases[tCaseNum].desc);

            curPositionData.pTimeStamp             = curTestCase->pTimeStamp;
            curPositionData.pLatitude              = curTestCase->pLatitude;
            curPositionData.pLongitude             = curTestCase->pLongitude;
            curPositionData.pAltitudeWrtEllipsoid  = curTestCase->pAltitudeWrtEllipsoid;
            curPositionData.pAltitudeWrtSealevel   = curTestCase->pAltitudeWrtSealevel;
            curPositionData.pHorizontalUncCircular = curTestCase->pHorizontalUncCircular;
            curPositionData.pVerticalUnc           = curTestCase->pVerticalUnc;
            curPositionData.pHorizontalConfidence  = curTestCase->pHorizontalConfidence;
            curPositionData.pVerticalConfidence    = curTestCase->pVerticalConfidence;
            curPositionData.pPositionSource        = curTestCase->pPositionSource;
            curPositionData.pTimeType              = curTestCase->pTimeType;

            nRet = SLQSPDSInjectPositionData( &curPositionData );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                if(curTestCase->pTimeStamp)
                    fprintf(fp, "\tTimeStamp : %llx\n", *curTestCase->pTimeStamp);
                if(curTestCase->pLatitude)
                    fprintf(fp, "\tLatitude : %llx\n", *curTestCase->pLatitude);
                if(curTestCase->pLongitude)
                    fprintf(fp, "\tLongitude : %llx\n", *curTestCase->pLongitude);
                if(curTestCase->pAltitudeWrtEllipsoid)
                    fprintf(fp, "\tAltitudeWrtEllipsoid : %lx\n", *curTestCase->pAltitudeWrtEllipsoid);
            }
            tCaseNum++;
        }
    }
    if (fp)
        tfclose(fp);
}

