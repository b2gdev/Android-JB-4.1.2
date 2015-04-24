/**************
 *
 *  Filename: qaGobiApiPdsTest.h
 *
 *  Purpose:  Contains prototypes for functions implementing specific
 *            tests for Position Determination Service (PDS)
 *            called by the main() routine in qatest.c
 *
 * Copyright: © 2011 Sierra Wireless, Inc., all rights reserved
 *
 **************/

#ifndef __GOBI_API_PDS_TEST_H__
#define __GOBI_API_PDS_TEST_H__

/* Defines */
#define nMaxStrLen                                          0xFF
#define MAX_GET_PDS_STATE_TESTCASE_NUM                      0x01
#define MAX_SET_PDS_STATE_TESTCASE_NUM                      0x07
#define MAX_START_TRACKING_SESSION_TESTCASE_NUM             0x01
#define MAX_STOP_TRACKING_SESSION_TESTCASE_NUM              0x01
#define MAX_GET_PDS_DEFAULTS_TESTCASE_NUM                   0x05
#define MAX_SET_PDS_DEFAULTS_TESTCASE_NUM                   0x04
#define MAX_GET_XTRA_AUTO_DOWNLOAD_TESTCASE_NUM             0x03
#define MAX_SET_XTRA_AUTO_DOWNLOAD_TESTCASE_NUM             0x03
#define MAX_GET_XTRA_NETWORK_TESTCASE_NUM                   0x02
#define MAX_SET_XTRA_NETWORK_TESTCASE_NUM                   0x05
#define MAX_GET_SERVICE_AUTO_TRACK_TESTCASE_NUM             0x02
#define MAX_SET_SERVICE_AUTO_TRACK_TESTCASE_NUM             0x03
#define MAX_GET_PORT_AUTO_TRACK_TESTCASE_NUM                0x02
#define MAX_SET_PORT_AUTO_TRACK_TESTCASE_NUM                0x03
#define MAX_PDS_FORCED_XTRA_DOWNLOAD_TESTCASE_NUM           0x01
#define MAX_GET_XTRA_VALIDITY_TESTCASE_NUM                  0x08
#define MAX_RESET_PDS_DATA_TESTCASE_NUM                     0x04
#define MAX_PDS_INJECT_TIME_REFERENCE_TESTCASE_NUM          0x03
#define MAX_SLQS_SET_AGPS_CONFIG_TESTCASE_NUM               0x04
#define MAX_PDS_INJECT_ABSOLUTE_TIME_REFERENCE_TESTCASE_NUM 0x04
#define MAX_SLQS_GET_AGPS_CONFIG_TESTCASE_NUM               0x06
#define MAX_SLQS_PDS_INJECT_POSITION_DATA_TESTCASE_NUM      0x02

/* Structure for test cases step */
typedef struct _GetPDSStateTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              desc[100];
} GetPDSStateTestCase_t;

typedef struct _SetPDSStateTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             enable;
    CHAR              desc[100];
} SetPDSStateTestCase_t;

typedef struct _StartTrackingSessionExtTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    BYTE              sessionControl;
    BYTE              sessionType;
    BYTE              sessionOperation;
    BYTE              sessionServerOption;
    BYTE              fixTimeout;
    ULONG             fixInterval;
    ULONG             fixCount;
    ULONG             fixAccuracy;
    CHAR              desc[100];
} StartTrackingSessionExtTestCase_t;

typedef struct _StopTrackingSessionTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              desc[100];
} StopTrackingSessionTestCase_t;

typedef struct _GetPDSDefaultsTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             *poperation;
    BYTE              *pmaxtimeout;
    ULONG             *pinterval;
    ULONG             *paccuracy;
    CHAR              desc[100];
} GetPDSDefaultsTestCase_t;

typedef struct _SetPDSDefaultsTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             operation;
    BYTE              timeout;
    ULONG             interval;
    ULONG             accuracy;
    CHAR              desc[100];
} SetPDSDefaultsTestCase_t;

typedef struct _GetXtraAutoDownloadTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             *pEnabled;
    USHORT            *pInterval;
    CHAR              desc[100];
} GetXtraAutoDownloadTestCase_t;

typedef struct _SetXtraAutoDownloadTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             bEnabled;
    USHORT            interval;
    CHAR              desc[100];
} SetXtraAutoDownloadTestCase_t;

typedef struct _GetXtraNetworkTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             *pPreference;
    CHAR              desc[100];
} GetXtraNetworkTestCase_t;

typedef struct _SetXtraNetworkTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             preference;
    CHAR              desc[100];
} SetXtraNetworkTestCase_t;

typedef struct _SLQSSetAGPSConfigTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             *pServerAddress;
    ULONG             *pServerPort;
    BYTE              *pServerUrl;
    BYTE              *pServerUrlLength;
    CHAR              desc[100];
} SLQSSetAGPSConfigTestCase_t;

typedef struct _GetServiceAutomaticTrackingTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             *pbAuto;
    CHAR              desc[100];
} GetServiceAutomaticTrackingTestCase_t;

typedef struct _SetServiceAutomaticTrackingTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             bAuto;
    CHAR              desc[100];
} SetServiceAutomaticTrackingTestCase_t;

typedef struct _GetPortAutomaticTrackingTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             *pbAuto;
    CHAR              desc[100];
} GetPortAutomaticTrackingTestCase_t;

typedef struct _SetPortAutomaticTrackingTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             bAuto;
    CHAR              desc[100];
} SetPortAutomaticTrackingTestCase_t;

typedef struct _ForceXTRADownloadTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    CHAR              desc[100];
} ForceXTRADownloadTestCase_t;

typedef struct _GetXTRAValidityTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    USHORT            *pGPSWeek;
    USHORT            *pGPSWeekOffset;
    USHORT            *pDuration;
    CHAR              desc[100];
} GetXTRAValidityTestCase_t;

typedef struct _ResetPDSDataTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONG             *pGPSDataMask;
    ULONG             *pCellDataMask;
    CHAR              desc[100];
} ResetPDSDataTestCase_t;

typedef struct _PDSInjectTimeReferenceTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG             testCaseId;
    ULONGLONG         systemTime;
    USHORT            systemDiscontinuities;
    CHAR              desc[100];
} PDSInjectTimeReferenceTestCase_t;

typedef struct _PDSInjectAbsoluteTimeReferenceTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG     testCaseId;
    ULONGLONG timeMsec;
    ULONG     timeUncMsec;
    BYTE      timeBase;
    BYTE      forceFlag;
    CHAR      desc[100];
} PDSInjectAbsoluteTimeReferenceTestCase_t;

typedef struct _SLQSGetAGPSConfigTestCase_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG     testCaseId;
    ULONG     *pServerAddress;
    ULONG     *pServerPort;
    BYTE      *pServerUrl;
    BYTE      *pServerUrlLength;
    CHAR      desc[100];
} SLQSGetAGPSConfigTestCase_t;

typedef struct _SLQSPDSInjectPositionData_t
{
    enum eQCWWANError expectedReturnValue;
    ULONG     testCaseId;
    ULONGLONG *pTimeStamp;
    ULONGLONG *pLatitude;
    ULONGLONG *pLongitude;
    ULONG     *pAltitudeWrtEllipsoid;
    ULONG     *pAltitudeWrtSealevel;
    ULONG     *pHorizontalUncCircular;
    ULONG     *pVerticalUnc;
    BYTE      *pHorizontalConfidence;
    BYTE      *pVerticalConfidence;
    BYTE      *pPositionSource;
    BYTE      *pTimeType;
    CHAR      desc[100];
} SLQSPDSInjectPositionData_t;

/* Define the test cases to be executed */
const GetPDSStateTestCase_t \
      GetPDSStateTestCases[MAX_GET_PDS_STATE_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_NONE, 1, "All valid" }
};

const SetPDSStateTestCase_t \
      SetPDSStateTestCases[MAX_SET_PDS_STATE_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_NONE, 1, 2,   "All valid : Invalid Parameter [Upper Limit]" },
    { eQCWWAN_ERR_NONE, 2, -1,  "All valid : Invalid Parameter [Lower Limit]" },
    { eQCWWAN_ERR_NONE, 3, 1,   "All valid : Enabling State" },
    { eQCWWAN_ERR_NONE, 4, 1,   "All valid : Enabling Again [Error 1026]" },
    { eQCWWAN_ERR_NONE, 5, 0,   "All valid : Disabling State" },
    { eQCWWAN_ERR_NONE, 6, 0,   "All valid : Disabling Again [Error 1026]" },
    { eQCWWAN_ERR_NONE, 7, 1,   "All valid : Enabling State" }
};

const StartTrackingSessionExtTestCase_t \
      StartTrackingSessionExtTestCases[MAX_START_TRACKING_SESSION_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_NONE, 1, 0, 0, 0, 0, 255, 1, 10, 10,
        "Starting Tracking Session" }
};

const StopTrackingSessionTestCase_t \
      StopTrackingSessionTestCases[MAX_STOP_TRACKING_SESSION_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_NONE, 1, "Stopping Tracking Session" }
};

ULONG operation;
BYTE  maxtimeout;
ULONG interval;
ULONG accuracy;
const GetPDSDefaultsTestCase_t \
      GetPDSDefaultsTestCases[MAX_GET_PDS_DEFAULTS_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_NONE,        1, &operation, &maxtimeout, &interval,
      &accuracy, "All Valid" },
    { eQCWWAN_ERR_INVALID_ARG, 2, NULL, &maxtimeout, &interval,
      &accuracy, "Sending OUT param pointer as NULL" },
    { eQCWWAN_ERR_INVALID_ARG, 3, &operation, NULL, &interval,
      &accuracy, "Sending OUT param pointer as NULL" },
    { eQCWWAN_ERR_INVALID_ARG, 4, &operation, &maxtimeout, NULL,
      &accuracy, "Sending OUT param pointer as NULL" },
    { eQCWWAN_ERR_INVALID_ARG, 5, &operation, &maxtimeout, &interval,
      NULL, "Sending OUT param pointer as NULL" }
};

const SetPDSDefaultsTestCase_t \
      SetPDSDefaultsTestCases[MAX_SET_PDS_DEFAULTS_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_NONE, 1, 0x00, 5, 1000, 10, "All valid : Setting Defaults" },
    { eQCWWAN_ERR_NONE, 2, 0x05, 5, 1000, 10, "Invalid Operating Mode" },
    { eQCWWAN_ERR_NONE, 3, 0x01, 5, 2000, 10, "All valid : Setting Defaults" },
    { eQCWWAN_ERR_NONE, 4, 0x02, 5, 1000, 10, "All valid : Setting Defaults" }
};

ULONG  Enabled;
USHORT Interval;
const GetXtraAutoDownloadTestCase_t \
      GetXtraAutoDownloadTestCases[MAX_GET_XTRA_AUTO_DOWNLOAD_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_INVALID_ARG, 1, NULL, &Interval,
      "Sending OUT param pointer as NULL" },
    { eQCWWAN_ERR_INVALID_ARG, 2, &Enabled, NULL,
      "Sending OUT param pointer as NULL" },
    { eQCWWAN_ERR_NONE,        3, &Enabled, &Interval, "All valid" }
};

const SetXtraAutoDownloadTestCase_t \
      SetXtraAutoDownloadTestCases[MAX_SET_XTRA_AUTO_DOWNLOAD_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_NONE, 1, 1, 1, "Enabling : Setting Xtra Automatic Download" },
    { eQCWWAN_ERR_NONE, 2, 0, 1, "Disabling : Setting Xtra Automatic Download" },
    { eQCWWAN_ERR_NONE, 3, 1, 2, "Enabling : Setting Xtra Automatic Download" },
};

ULONG preference;
const GetXtraNetworkTestCase_t \
      GetXtraNetworkTestCases[MAX_GET_XTRA_NETWORK_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_INVALID_ARG, 1, NULL, "Sending OUT param pointer as NULL" },
    { eQCWWAN_ERR_NONE,        2, &preference, "All valid" }
};

const SetXtraNetworkTestCase_t \
      SetXtraNetworkTestCases[MAX_SET_XTRA_NETWORK_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_NONE,        1, 0, "All valid : Setting Xtra Network" },
    { eQCWWAN_ERR_INVALID_ARG, 2, 3, "Invalid Case" },
    { eQCWWAN_ERR_INVALID_ARG, 3, -1, "Invalid Case" },
    { eQCWWAN_ERR_NONE,        4, 1, "All valid : Setting Xtra Network" },
    { eQCWWAN_ERR_NONE,        5, 2, "All valid : Setting Xtra Network" }
};

ULONG serverAddress;
ULONG serverPort;

BYTE  serverUrl[]     = "www.abc.com";
ULONG serverAddress   = 0x12345678;
ULONG serverport      = 5001;
BYTE  serverUrlLength = 11;

const SLQSSetAGPSConfigTestCase_t \
      SLQSSetAGPSConfigTestCases[MAX_SLQS_SET_AGPS_CONFIG_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_NONE, 1, NULL, NULL, NULL, NULL, "All NULL" },
    { eQCWWAN_ERR_NONE, 2, NULL, NULL, serverUrl, &serverUrlLength, "Only URL" },
    { eQCWWAN_ERR_NONE, 3, &serverAddress, &serverport, NULL, NULL, "Only Address" },
    { eQCWWAN_ERR_NONE, 4, &serverAddress, &serverport, serverUrl, &serverUrlLength, "All Valid" },
};

ULONG bAuto;

const GetServiceAutomaticTrackingTestCase_t \
      GetServiceAutomaticTrackingTestCases[\
          MAX_GET_SERVICE_AUTO_TRACK_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_INVALID_ARG, 1, NULL, "Sending OUT param pointer as NULL" },
    { eQCWWAN_ERR_NONE,        2, &bAuto, "All valid" }
};

const SetServiceAutomaticTrackingTestCase_t \
      SetServiceAutomaticTrackingTestCases[\
          MAX_SET_SERVICE_AUTO_TRACK_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_NONE, 1, 0x02, "Invalid Value" },
    { eQCWWAN_ERR_NONE, 1, 0x01, "All Valid - Enable" },
    { eQCWWAN_ERR_NONE, 2, 0x00, "All valid - Disable" }
};

ULONG config;

const GetPortAutomaticTrackingTestCase_t \
      GetPortAutomaticTrackingTestCases[\
          MAX_GET_PORT_AUTO_TRACK_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_INVALID_ARG, 1, NULL,\
        "Sending OUT param pointer as NULL" },
    { eQCWWAN_ERR_NONE, 2, &config, "All valid" }
};

const SetPortAutomaticTrackingTestCase_t \
      SetPortAutomaticTrackingTestCases[\
          MAX_SET_PORT_AUTO_TRACK_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_NONE, 1, 0x02, "Invalid Parameter" },
    { eQCWWAN_ERR_NONE, 1, 0x01, "All Valid - Enable" },
    { eQCWWAN_ERR_NONE, 2, 0x00, "All valid - Disable" }
};

const ForceXTRADownloadTestCase_t \
      ForceXTRADownloadTestCases[\
          MAX_PDS_FORCED_XTRA_DOWNLOAD_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_NONE, 1, "All valid" }
};

USHORT gPSWeek, gPSWeekOffset, duration;

const GetXTRAValidityTestCase_t \
      GetXTRAValidityTestCases[ MAX_GET_XTRA_VALIDITY_TESTCASE_NUM] =

{
    { eQCWWAN_ERR_INVALID_ARG, 1, NULL, NULL, NULL, \
                                       "Sending OUT param pointer as NULL" },
    { eQCWWAN_ERR_NONE,        2, &gPSWeek, &gPSWeekOffset, \
                                       &duration, "All valid" },
    { eQCWWAN_ERR_INVALID_ARG, 3, NULL, &gPSWeekOffset, \
                                       &duration, "One Invalid parameter" },
    { eQCWWAN_ERR_INVALID_ARG, 4, &gPSWeek, NULL, &gPSWeekOffset, \
                                        "One Invalid parameter" },
    { eQCWWAN_ERR_INVALID_ARG, 5, &gPSWeek, &gPSWeek, NULL, \
                                       "One Invalid parameter" },
    { eQCWWAN_ERR_INVALID_ARG, 6, NULL, &gPSWeekOffset,\
                                       &duration, "Two Invalid parameter" },
    { eQCWWAN_ERR_INVALID_ARG, 7, &gPSWeek, NULL, NULL, \
                                       "Two Invalid parameter" },
    { eQCWWAN_ERR_INVALID_ARG, 8, NULL, &gPSWeek, NULL, \
                                      "Two Invalid parameter" }
};

#define GPS_DATA_CLEAR  0x000003FF
#define CELL_DATA_CLEAR 0x000000FF

ULONG gPSDataMask  = GPS_DATA_CLEAR;
ULONG cellDataMask = CELL_DATA_CLEAR;

const ResetPDSDataTestCase_t \
      ResetPDSDataTestCases[\
          MAX_RESET_PDS_DATA_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_INVALID_ARG, 1, NULL,         NULL, "All Invalid"},
    { eQCWWAN_ERR_NONE,        2, &gPSDataMask, &cellDataMask,\
                                                      "All valid" },
    { eQCWWAN_ERR_NONE,        3, NULL,         &cellDataMask,\
                                                       "Resetting Cell Data" },
    { eQCWWAN_ERR_NONE,        4, &gPSDataMask, NULL,  "Resetting GPS Data" }
};

const PDSInjectTimeReferenceTestCase_t \
      PDSInjectTimeReferenceTestCases[\
          MAX_PDS_INJECT_TIME_REFERENCE_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_NONE,  1, 0,   0, "Sending time & discontinuities 0" },
    { eQCWWAN_ERR_NONE,  2, 0,   1, "Sytem time is 0" },
    { eQCWWAN_ERR_NONE,  3, 100, 4, "All valid" }

};

const PDSInjectAbsoluteTimeReferenceTestCase_t \
      PDSInjectAbsoluteTimeReferenceTestCases[\
          MAX_PDS_INJECT_ABSOLUTE_TIME_REFERENCE_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_NONE,  1, 0,   0, 0, 0, "All 0" },
    { eQCWWAN_ERR_NONE,  2, 100, 1, 2, 0, "Invalid Time Base" },
    { eQCWWAN_ERR_NONE,  3, 100, 1, 0, 1, "All Valid : Time Base 0" },
    { eQCWWAN_ERR_NONE,  3, 100, 1, 1, 0, "All Valid : Time Base 0" }

};

/* PDS GetAGPSConfig() parameters */
ULONG ServerAddress;
ULONG ServerPort;
BYTE  ServerURL[30];
BYTE  ServerURLLength = 30;

const SLQSGetAGPSConfigTestCase_t SLQSGetAGPSConfigTestCases\
        [MAX_SLQS_GET_AGPS_CONFIG_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_NONE, 1, NULL,           NULL,        NULL,       NULL,             "Invalid Address" },
    { eQCWWAN_ERR_NONE, 1, NULL,           &ServerPort, ServerURL, &ServerURLLength,  "Invalid Address" },
    { eQCWWAN_ERR_NONE, 1, &ServerAddress, NULL,        ServerURL, &ServerURLLength,  "Invalid Address" },
    { eQCWWAN_ERR_NONE, 1, &ServerAddress, &ServerPort, NULL,       &ServerURLLength, "Invalid Address" },
    { eQCWWAN_ERR_NONE, 1, &ServerAddress, &ServerPort, ServerURL, NULL,              "Invalid Address" },
    { eQCWWAN_ERR_NONE, 1, &ServerAddress, &ServerPort, ServerURL, &ServerURLLength,  "All Valid" },
};

/* PDS SLQSPDSInjectPositionData() parameters */
ULONGLONG TimeStamp = 0x12345678;
ULONGLONG Latitude  = 0x12345678;
ULONGLONG Longitude = 0x12345678;
ULONG     AltitudeWrtEllipsoid;
ULONG     AltitudeWrtSealevel;
ULONG     HorizontalUncCircular;
ULONG     VerticalUnc;
BYTE      HorizontalConfidence = 0x12;
BYTE      VerticalConfidence;
BYTE      PositionSource;
BYTE      TimeType;

const SLQSPDSInjectPositionData_t SLQSPDSInjectPositionDataTestCases\
        [MAX_SLQS_PDS_INJECT_POSITION_DATA_TESTCASE_NUM] =
{
    { eQCWWAN_ERR_NONE, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, "Invalid Address" },
    { eQCWWAN_ERR_NONE, 1, &TimeStamp, &Latitude, &Longitude, &AltitudeWrtEllipsoid, &AltitudeWrtSealevel,
        &HorizontalUncCircular, &VerticalUnc, &VerticalConfidence, &HorizontalConfidence, &PositionSource, &TimeType, "Valid Address" },
};


#endif /* __GOBI_API_PDS_TEST_H__ */
