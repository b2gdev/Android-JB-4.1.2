/**************
 *
 *  Filename: qaGobiApiDmsTest.c
 *
 *  Purpose:  Contains functions implementing specific tests for
 *            Device Management Service (DMS)
 *            called by the main() routine in qatest.c
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
#include "qaGobiApiDms.h"
#include "qaGobiApiDmsTest.h"
#include "qatesthelper.h"
#include "qatestproto.h"

/*******************
    TEST DATA
 *******************/
/* Constants */
#define QMI_DMS_PM_ONLINE         0x00 /* Online */
#define QMI_DMS_PM_LOW            0x01 /* Low Power */
#define QMI_DMS_PM_FACTORY        0x02 /* Factory Test Mode */
#define QMI_DMS_PM_OFFLINE        0x03 /* Offline */
#define QMI_DMS_PM_RESET          0x04 /* Reset */
#define QMI_DMS_PM_SHUT_DOWN      0x05 /* Shut Down */
#define QMI_DMS_PM_PERSISTENT_LOW 0x06 /* Persistent Low Power */
/* Global Variables */

/* Get Custom Features parameters */

local BYTE disableIMSI;
local WORD IPFamSupport;
local BYTE RMAutoConn;
local BYTE GPSSel;

local BYTE *pDisableIMSI  = &disableIMSI;
local WORD *pIPFamSupport = &IPFamSupport;
local BYTE *pRMAutoConn   = &RMAutoConn;
local BYTE *pGPSSel       = &GPSSel;
local custFeaturesInfo CustFeatures;

/* Set Custom Features parameters*/
local BYTE GPSSelSetting = 0x00;
local ULONG GPSEnable[3] = {0, 1, 9};
local BYTE *pGPSSelect   = &GPSSelSetting;
local custFeaturesSetting CustFeaturesSetting;

/*******************
  TEST FUNCTIONS
 *******************/

/*
 * Name:     doGetManufacturer
 *
 * Purpose:  Perform the tests that call the GetManufacturer() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetManufacturer( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    BYTE  stringSize = nMaxStrLen;
    CHAR  mfrString[stringSize];
    FILE  *fp = NULL;

    fp = tfopen("../../dms/test/results/getmanufacturer.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_GET_MANUFACTURER_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_GET_MANUFACTURER_TESTCASE_NUM,
                       GetManufacturerTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Stringsize :%d\n", stringSize);

            nRet = GetManufacturer(stringSize,
                                   &mfrString[0]);

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "Manufacturer : %s\n", mfrString);
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetModelID
 *
 * Purpose:  Perform the tests that call the GetModelID() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetModelID( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    BYTE  stringSize = nMaxStrLen;
    CHAR  modelString[stringSize];
    FILE  *fp = NULL;

    fp = tfopen("../../dms/test/results/getmodelid.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_GET_MODEL_ID_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_GET_MODEL_ID_TESTCASE_NUM,
                       GetModelIDTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Stringsize :%d\n", stringSize);

            nRet = GetModelID(stringSize,
                              &modelString[0]);

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "Model : %s\n", modelString);
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetFirmwareRevision
 *
 * Purpose:  Perform the tests that call the GetFirmwareRevision() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetFirmwareRevision( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    BYTE stringSize = nMaxStrLen;
    CHAR fmrString[stringSize];
    FILE  *fp = NULL;

    fp = tfopen("../../dms/test/results/getfirmwarerevision.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_GET_FIRMWARE_REVISION_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_GET_FIRMWARE_REVISION_TESTCASE_NUM,
                       GetFirmwareRevisionTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Stringsize :%d\n", stringSize);

            nRet = GetFirmwareRevision(stringSize,
                                       &fmrString[0]);

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "Firmware Revision : %s\n", fmrString);
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetFirmwareRevisions
 *
 * Purpose:  Perform the tests that call the GetFirmwareRevisions() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetFirmwareRevisions( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    BYTE  amssSize = nMaxStrLen;
    CHAR  AMSString[nMaxStrLen];
    BYTE  bootSize = nMaxStrLen;
    CHAR  bootString[nMaxStrLen];
    BYTE  priSize = nMaxStrLen;
    CHAR  PRIString[nMaxStrLen];
    FILE  *fp = NULL;

    fp = tfopen("../../dms/test/results/getfirmwarerevisions.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_GET_FIRMWARE_REVISIONS_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_GET_FIRMWARE_REVISIONS_TESTCASE_NUM,
                       GetFirmwareRevisionsTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "amssSize :%d\n", amssSize);
            fprintf(fp, "bootSize :%d\n", bootSize);
            fprintf(fp, "priSize :%d\n", priSize);

            nRet = GetFirmwareRevisions(amssSize,
                                        &AMSString[0],
                                        bootSize,
                                        &bootString[0],
                                        priSize,
                                        &PRIString[0]);

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "AMS : %s\n", AMSString);
                fprintf(fp, "Boot : %s\n", bootString);
                fprintf(fp, "PRI : %s\n", PRIString);
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetPRLVersion
 *
 * Purpose:  Perform the tests that call the GetPRLVersion() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetPRLVersion( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;
    WORD  prlVersion;

    fp = tfopen("../../dms/test/results/getprlversion.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_GET_PRL_VERSION_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_GET_PRL_VERSION_TESTCASE_NUM,
                       GetPRLVersionTestCases[tCaseNum].desc);

            nRet = GetPRLVersion(&prlVersion);

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "PRL Version : %d\n", prlVersion);
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetIMSI
 *
 * Purpose:  Perform the tests that call the GetIMSI() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetIMSI( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    BYTE  stringSize = nMaxStrLen;
    CHAR  imsiString[stringSize];
    FILE  *fp = NULL;

    fp = tfopen("../../dms/test/results/getimsi.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_GET_IMSI_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_GET_IMSI_TESTCASE_NUM,
                       GetIMSITestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Stringsize :%d\n", stringSize);

            nRet = GetIMSI(stringSize, &imsiString[0]);

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "IMSI : %s\n", imsiString);
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetSerialNumbers
 *
 * Purpose:  Perform the tests that call the GetSerialNumbers() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetSerialNumbers( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    BYTE  esnSize = nMaxStrLen;
    CHAR  ESNString[nMaxStrLen];
    BYTE  imeiSize = nMaxStrLen;
    CHAR  IMEIString[nMaxStrLen];
    BYTE  meidSize = nMaxStrLen;
    CHAR  MEIDString[nMaxStrLen];
    FILE  *fp = NULL;

    fp = tfopen("../../dms/test/results/getserialnumbers.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_GET_SERIAL_NUMBERS_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_GET_SERIAL_NUMBERS_TESTCASE_NUM,
                       GetSerialNumbersTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "esnSize :%d\n", esnSize);
            fprintf(fp, "imeiSize :%d\n", imeiSize);
            fprintf(fp, "meidSize :%d\n", meidSize);

            nRet = GetSerialNumbers(esnSize,
                                    &ESNString[0],
                                    imeiSize,
                                    &IMEIString[0],
                                    meidSize,
                                    &MEIDString[0]);

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "ESNString : %s\n", ESNString);
                fprintf(fp, "IMEIString : %s\n", IMEIString);
                fprintf(fp, "MEIDString : %s\n", MEIDString);
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetHardwareRevision
 *
 * Purpose:  Perform the tests that call the GetHardwareRevision() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetHardwareRevision( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    BYTE  stringSize = nMaxStrLen;
    CHAR  hwString[stringSize];
    FILE  *fp = NULL;

    fp = tfopen("../../dms/test/results/gethardwarerevision.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_GET_HARDWARE_REVISION_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_GET_HARDWARE_REVISION_TESTCASE_NUM,
                       GetHardwareRevisionTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Stringsize :%d\n", stringSize);

            nRet = GetHardwareRevision(stringSize,
                                       &hwString[0]);
            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "Hardware Revision : %s\n", hwString);
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetNetworkTimeTime
 *
 * Purpose:  Perform the tests that call the GetNetworkTime() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetNetworkTime( void )
{
    BYTE       tCaseNum = 0;
    ULONG      nRet;
    FILE       *fp = NULL;
    ULONGLONG  *timeStamp;
    ULONG      *timeSource;

    fp = tfopen("../../dms/test/results/getnetworktime.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_GET_NETWORK_TIME_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       (tCaseNum + 1),
                       MAX_GET_NETWORK_TIME_TESTCASE_NUM,
                       GetNetworkTimeTestCases[tCaseNum].desc);

            timeStamp  = GetNetworkTimeTestCases[tCaseNum].timeStamp;
            timeSource = GetNetworkTimeTestCases[tCaseNum].timeSource;
            fprintf(fp, "There are no IN paramenters\n");

            nRet = GetNetworkTime( timeStamp, timeSource );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "TimeStamp is : %llx\n", *timeStamp);
                fprintf(fp, "TimeSource is : %lx\n", *timeSource);
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doUIMSetPINProtection
 *
 * Purpose:  Perform the tests that call the UIMSetPINProtection() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doUIMSetPINProtection( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../dms/test/results/uimsetpinprotection.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_UIM_SET_PIN_PROTECTION_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_UIM_SET_PIN_PROTECTION_TESTCASE_NUM,
                        UIMSetPINProtectionTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Id : %lu\n", UIMSetPINProtectionTestCases[tCaseNum].id);
            fprintf(fp, "bEnable : %lu\n", UIMSetPINProtectionTestCases[tCaseNum].bEnable);
            fprintf(fp, "Value : %s\n", UIMSetPINProtectionTestCases[tCaseNum].value);

            nRet = UIMSetPINProtection( UIMSetPINProtectionTestCases[tCaseNum].id,
                       UIMSetPINProtectionTestCases[tCaseNum].bEnable,
                       (CHAR *)&UIMSetPINProtectionTestCases[tCaseNum].value[0],
                       UIMSetPINProtectionTestCases[tCaseNum].pVerifyRetries,
                       UIMSetPINProtectionTestCases[tCaseNum].pUnblockRetries );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
                fprintf(fp, "Set PIN Protection Successful\n");

            fprintf(fp, "Verify Retries Left: %lx\n", verifyRetries);
            fprintf(fp, "Unblock Retries Left: %lx\n", unblockRetries);

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doUIMUnblockPIN
 *
 * Purpose:  Perform the tests that call the UIMUnblockPIN() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doUIMUnblockPIN( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;
    ULONG verifyRetriesLeft = 0;
    ULONG unblockRetriesLeft = 0;

    fp = tfopen("../../dms/test/results/uimunblockpin.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_UIM_UNBLOCK_PIN_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_UIM_UNBLOCK_PIN_TESTCASE_NUM,
                        UIMUnblockPINTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Id : %lu\n", UIMUnblockPINTestCases[tCaseNum].id);
            fprintf(fp, "PUK Value : %s\n", UIMUnblockPINTestCases[tCaseNum].pukValue);
            fprintf(fp, "New Value : %s\n", UIMUnblockPINTestCases[tCaseNum].newValue);

            nRet = UIMUnblockPIN( UIMUnblockPINTestCases[tCaseNum].id,
                                  (CHAR *)&UIMUnblockPINTestCases[tCaseNum].pukValue[0],
                                  (CHAR *)&UIMUnblockPINTestCases[tCaseNum].newValue[0],
                                  &verifyRetriesLeft,
                                  &unblockRetriesLeft );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
                fprintf(fp, "Unblock PIN Successful\n");

            fprintf(fp, "Verify Retries Left: %lu\n", verifyRetriesLeft);
            fprintf(fp, "Unblock Retries Left: %lu\n", unblockRetriesLeft);

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}


/*
 * Name:     doUIMVerifyPIN
 *
 * Purpose:  Perform the tests that call the UIMVerifyPIN() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doUIMVerifyPIN( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;
    ULONG verifyRetriesLeft = 0;
    ULONG unblockRetriesLeft = 0;

    fp = tfopen("../../dms/test/results/uimverifypin.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_UIM_VERIFY_PIN_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_UIM_VERIFY_PIN_TESTCASE_NUM,
                        UIMVerifyPINTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Id : %lu\n", UIMVerifyPINTestCases[tCaseNum].id);
            fprintf(fp, "Value : %s\n", UIMVerifyPINTestCases[tCaseNum].value);

            nRet = UIMVerifyPIN( UIMVerifyPINTestCases[tCaseNum].id,
                                 (CHAR *)&UIMVerifyPINTestCases[tCaseNum].value[0],
                                 &verifyRetriesLeft,
                                 &unblockRetriesLeft );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
                fprintf(fp, "Verify PIN Successful\n");

            fprintf(fp, "Verify Retries Left: %lu\n", verifyRetriesLeft);
            fprintf(fp, "Unblock Retries Left: %lu\n", unblockRetriesLeft);

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doUIMChangePIN
 *
 * Purpose:  Perform the tests that call the UIMChangePIN() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doUIMChangePIN( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;
    ULONG verifyRetriesLeft = 0;
    ULONG unblockRetriesLeft = 0;

    fp = tfopen("../../dms/test/results/uimchangepin.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_UIM_CHANGE_PIN_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_UIM_CHANGE_PIN_TESTCASE_NUM,
                        UIMChangePINTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Id : %lu\n", UIMChangePINTestCases[tCaseNum].id);
            fprintf(fp, "Old Value : %s\n", UIMChangePINTestCases[tCaseNum].oldValue);
            fprintf(fp, "New Value : %s\n", UIMChangePINTestCases[tCaseNum].newValue);

            nRet = UIMChangePIN( UIMChangePINTestCases[tCaseNum].id,
                                 (CHAR *)&UIMChangePINTestCases[tCaseNum].oldValue[0],
                                 (CHAR *)&UIMChangePINTestCases[tCaseNum].newValue[0],
                                 &verifyRetriesLeft,
                                 &unblockRetriesLeft );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
                fprintf(fp, "Change PIN Successful\n");

            fprintf(fp, "Verify Retries Left: %lu\n", verifyRetriesLeft);
            fprintf(fp, "Unblock Retries Left: %lu\n", unblockRetriesLeft);

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetVoiceNumber
 *
 * Purpose:  Perform the tests that call the GetVoiceNumber() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetVoiceNumber( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../dms/test/results/GetVoiceNumber.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_GET_VOICE_NUMBER_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_GET_VOICE_NUMBER_TESTCASE_NUM,
                        GetVoiceNumberTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf( fp,
                     "Voice Numer Size : %d\n",
                     GetVoiceNumberTestCases[tCaseNum].voiceNumberSize );
            fprintf( fp,
                     "MIN Size : %d\n",
                     GetVoiceNumberTestCases[tCaseNum].minSize );

            nRet = GetVoiceNumber( GetVoiceNumberTestCases[tCaseNum].voiceNumberSize,
                                   GetVoiceNumberTestCases[tCaseNum].pVoiceNumber,
                                   GetVoiceNumberTestCases[tCaseNum].minSize,
                                   GetVoiceNumberTestCases[tCaseNum].pMIN );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if ( !nRet )
            {
                fprintf(fp, "Voice Number Retrieved\n");
                fprintf( fp,
                         "Voice Number : %s\n",
                         GetVoiceNumberTestCases[tCaseNum].pVoiceNumber );
                fprintf( fp,
                         "MIN: %s\n",
                         GetVoiceNumberTestCases[tCaseNum].pMIN );
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}


/*
 * Name:     doSetPower
 *
 * Purpose:  Perform the tests that call the SetPower() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSetPower( void )
{
    ULONG nRet;
    SetPowerTestCase_t SetPowerTestCases[] =
    {
        {eQCWWAN_ERR_NONE, 1, QMI_DMS_PM_ONLINE-1,
         "Invalid Power Mode : Lower Bounday Check"},
        {eQCWWAN_ERR_NONE, 1, QMI_DMS_PM_LOW,
         "Valid : QMI_DMS_PM_LOW"},
        {eQCWWAN_ERR_NONE, 1, QMI_DMS_PM_FACTORY,
         "Valid : QMI_DMS_PM_FACTORY"},
        {eQCWWAN_ERR_NONE, 1, QMI_DMS_PM_OFFLINE,
         "Valid : QMI_DMS_PM_OFFLINE"},
        {eQCWWAN_ERR_NONE, 1, QMI_DMS_PM_SHUT_DOWN,
         "Valid : QMI_DMS_PM_SHUT_DOWN"},
        {eQCWWAN_ERR_NONE, 1, QMI_DMS_PM_PERSISTENT_LOW,
         "Valid : QMI_DMS_PM_PERSISTENT_LOW"},
        {eQCWWAN_ERR_NONE, 1, QMI_DMS_PM_PERSISTENT_LOW + 1,
         "Invalid Power Mode : Upper Bounday Check"},
        {eQCWWAN_ERR_NONE, 1, QMI_DMS_PM_ONLINE,
         "Valid : QMI_DMS_PM_ONLINE"},
    };

    FILE *fp = tfopen("../../dms/test/results/setpower.txt", "w");

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SetPowerTestCases ) /
                      sizeof( SetPowerTestCases[0] );
        while( tc < ncases )
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                        (tc + 1),
                        ncases,
                        SetPowerTestCases[tc].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Power Mode : %lu\n", SetPowerTestCases[tc].powerMode);

            nRet = SetPower( SetPowerTestCases[tc].powerMode );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "Set Power Successful \n");
            }

            tc++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetPower
 *
 * Purpose:  Perform the tests that call the GetPower() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetPower( void )
{
    ULONG nRet;

    GetPowerTestCase_t GetPowerTestCases[] =
    {
        {eQCWWAN_ERR_NONE, 1, "ALL Valid"}
    };

    FILE *fp = tfopen("../../dms/test/results/getpower.txt", "w");

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( GetPowerTestCases ) /
                      sizeof( GetPowerTestCases[0] );
        while (tc < ncases)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                        (tc + 1),
                        ncases,
                        GetPowerTestCases[tc].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "There are no IN parameters\n");

            ULONG powerMode = 0;
            nRet = GetPower( &powerMode );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "Get Power Successful \n");
                fprintf(fp, "Power Mode is : %lu\n", powerMode);
            }

            tc++;
        }
    }

    if(fp)
        tfclose(fp);
}

/*
 * Name:     doUIMGetControlKeyStatus
 *
 * Purpose:  Perform the tests that call the UIMGetControlKeyStatus() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doUIMGetControlKeyStatus( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    ULONG status = 0;
    ULONG verifyRetriesLeft = 0;
    ULONG unblockRetriesLeft = 0;
    FILE  *fp = NULL;

    fp = tfopen("../../dms/test/results/uimgetcontrolkeystatus.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_UIM_GET_CK_STATUS_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_UIM_GET_CK_STATUS_TESTCASE_NUM,
                        UIMGetCKStatusTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Id : %lu\n", UIMGetCKStatusTestCases[tCaseNum].id);

            nRet = UIMGetControlKeyStatus( UIMGetCKStatusTestCases[tCaseNum].id,
                                           &status,
                                           &verifyRetriesLeft,
                                           &unblockRetriesLeft );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "Required Control Key Status is as follows: \n");
                fprintf(fp, "Status : %lu\n",status);
                fprintf(fp, "Verify Retries Left : %lu\n",verifyRetriesLeft);
                fprintf(fp, "Unblock Retries Left : %lu\n",unblockRetriesLeft);
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doUIMGetICCID
 *
 * Purpose:  Perform the tests that call the UIMGetICCID() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doUIMGetICCID( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    CHAR  *string;
    FILE  *fp = NULL;

    fp = tfopen("../../dms/test/results/uimgeticcid.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_UIM_GET_ICCID_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_UIM_GET_ICCID_TESTCASE_NUM,
                        UIMGetICCIDTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Size : %d\n", UIMGetICCIDTestCases[tCaseNum].strSize);
            string = UIMGetICCIDTestCases[tCaseNum].pString;

            nRet = UIMGetICCID( UIMGetICCIDTestCases[tCaseNum].strSize,
                                &string[0] );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "UIMGetICCID Successful \n");
                fprintf(fp, "ICCID is : %s\n", string);
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doUIMGetPINStatus
 *
 * Purpose:  Perform the tests that call the UIMGetPINStatus() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doUIMGetPINStatus( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    ULONG *status;
    ULONG verifyRetriesLeft;
    ULONG unblockRetriesLeft;
    FILE  *fp = NULL;

    fp = tfopen("../../dms/test/results/uimgetpinstatus.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_UIM_GET_PIN_STATUS_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_UIM_GET_PIN_STATUS_TESTCASE_NUM,
                        UIMGetPINStatusTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Id : %lu\n", UIMGetPINStatusTestCases[tCaseNum].id);
            status = UIMGetPINStatusTestCases[tCaseNum].pStatus;

            nRet = UIMGetPINStatus( UIMGetPINStatusTestCases[tCaseNum].id,
                                    status,
                                    &verifyRetriesLeft,
                                    &unblockRetriesLeft );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "Pin Status is as follows\n");
                fprintf(fp, "Status : %lu\n", *status);
            }

            fprintf(fp, "Verify Retries Left : %lu\n", verifyRetriesLeft);
            fprintf(fp, "Unblock Retries Left : %lu\n", unblockRetriesLeft);

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doGetOfflineReason
 *
 * Purpose:  Perform the tests that call the GetOfflineReason() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetOfflineReason( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    ULONG *ReasonMask;
    ULONG *Platform;
    FILE  *fp = NULL;

    fp = tfopen("../../dms/test/results/getofflinereason.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_GET_OFFLINE_REASON_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_GET_OFFLINE_REASON_TESTCASE_NUM,
                        GetOfflineReasonTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "There are no IN parameters\n");
            ReasonMask = GetOfflineReasonTestCases[tCaseNum].ReasonMask;
            Platform   = GetOfflineReasonTestCases[tCaseNum].Platform;

            nRet =GetOfflineReason( ReasonMask, Platform );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "Get Offline Reason Successful \n");
                fprintf(fp, "ReasonMask is : %lx\n", *ReasonMask);
                fprintf(fp, "Platform is : %lx\n", *ReasonMask);
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doUIMSetCkProtection
 *
 * Purpose:  Perform the tests that call the UIMSetControlKeyProtection() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doUIMSetCkProtection( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    ULONG verifyRetriesLeft = 0;
    FILE  *fp = NULL;

    fp = tfopen("../../dms/test/results/uimsetcontrolkeyprotection.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_UIM_SET_CK_PROTECTION_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_UIM_SET_CK_PROTECTION_TESTCASE_NUM,
                        UIMSetCkProtectionTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Id : %lu\n", UIMSetCkProtectionTestCases[tCaseNum].id);
            fprintf(fp, "Status : %lu\n", UIMSetCkProtectionTestCases[tCaseNum].status);
            fprintf(fp, "Value : %s\n", UIMSetCkProtectionTestCases[tCaseNum].value);

            nRet = UIMSetControlKeyProtection(
                       UIMSetCkProtectionTestCases[tCaseNum].id,
                       UIMSetCkProtectionTestCases[tCaseNum].status,
                       (CHAR *)UIMSetCkProtectionTestCases[tCaseNum].value,
                       &verifyRetriesLeft );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
                fprintf(fp, "Control Key Protection Successful \n");

            fprintf(fp, "Verify Retries Left : %lu\n",verifyRetriesLeft);

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doUIMUnblockCk
 *
 * Purpose:  Perform the tests that call the UIMUnblockControlKey() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doUIMUnblockCk( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    ULONG unblockRetriesLeft = 0;
    FILE  *fp = NULL;

    fp = tfopen("../../dms/test/results/uimunblockcontrolkey.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_UIM_UNBLOCK_CK_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_UIM_UNBLOCK_CK_TESTCASE_NUM,
                        UIMUnblockCkTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Id : %lu\n", UIMUnblockCkTestCases[tCaseNum].id);
            fprintf(fp, "Value : %s\n", UIMUnblockCkTestCases[tCaseNum].value);

            nRet = UIMUnblockControlKey( UIMUnblockCkTestCases[tCaseNum].id,
                                         (CHAR *)UIMUnblockCkTestCases[tCaseNum].value,
                                         &unblockRetriesLeft );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
                fprintf(fp, "Control Key Unblock Successful \n");

            fprintf(fp, "Unblock Retries Left : %lu\n",unblockRetriesLeft);
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     DoGetDeviceCapabilities
 *
 * Purpose:  Perform the tests that call the GetDeviceCapabilities() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doGetDeviceCapabilities ( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    BYTE  count;
    ULONG maxTXChannelRate ;
    ULONG maxRXChannelRate ;
    ULONG dataServiceCapability ;
    ULONG simCapability ;
    ULONG radioIfacesSize;
    BYTE  radioIfaces [nMaxStrLen];
    FILE  *fp = NULL;

    fp = tfopen("../../dms/test/results/devicecapabilties.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_DEVICE_CAP_TESTCASE_NUM)
        {
            radioIfacesSize = GetDeviceCapabilitiesTestCases[tCaseNum]
                                                  .radioIfacesSize;

            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_DEVICE_CAP_TESTCASE_NUM,
                       GetDeviceCapabilitiesTestCases[tCaseNum].desc);
            fprintf(fp, "RadioIfacesSize: %lu\n", radioIfacesSize);
            nRet = GetDeviceCapabilities( &maxTXChannelRate,
                                          &maxRXChannelRate,
                                          &dataServiceCapability,
                                          &simCapability,
                                          &radioIfacesSize,
                                          (BYTE *)&radioIfaces[0] );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "MaxTXChannelRate : %lx\n", maxTXChannelRate );
                fprintf(fp, "MaxRXChannelRate : %lx\n", maxRXChannelRate );
                fprintf(fp, "DataServiceCapability : %lx\n",
                                                   dataServiceCapability );
                fprintf(fp, "SimCapability : %lx\n", simCapability );
                fprintf(fp, "RadioIfacesSize: %lx\n", radioIfacesSize);
                for(count = 0; count < radioIfacesSize; count++)
                {
                    fprintf(fp, "radioInterface[%d] : %x\n",
                            count, radioIfaces[count]);
                }

            }
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doResetToFactoryDefaults
 *
 * Purpose:  Perform the tests that call the ResetToFactoryDefaults() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doResetToFactoryDefaults( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../dms/test/results/resettofactorydefaults.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_RESET_FACTORY_DEFAULTS_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_RESET_FACTORY_DEFAULTS_TESTCASE_NUM,
                       ResetFactoryDefaultsTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "pSPC :%s\n",
                    ResetFactoryDefaultsTestCases[tCaseNum].pSPC);


            nRet = ResetToFactoryDefaults(
                     (CHAR *)&ResetFactoryDefaultsTestCases[tCaseNum].pSPC[0] );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "Factory Defaults Restored\n");
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:     doValidateSPC
 *
 * Purpose:  Perform the tests that call the ValidateSPC() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doValidateSPC( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../dms/test/results/validatespc.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_VALIDATE_SPC_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_VALIDATE_SPC_TESTCASE_NUM,
                       ValidateSpcTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "pSPC :%s\n",
                    ValidateSpcTestCases[tCaseNum].pSPC);


            nRet = ValidateSPC( (CHAR *)ValidateSpcTestCases[tCaseNum].pSPC );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "SPC Validated\n");
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

/**
 * Name:     doActivateAutomatic
 *
 * Purpose:  Perform the tests that call the ActivateAutomatic() API
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doActivateAutomatic( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../dms/test/results/activateautomatic.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_ACTIVATE_AUTOMATIC_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                        (tCaseNum + 1),
                        MAX_ACTIVATE_AUTOMATIC_TESTCASE_NUM,
                        ActivateAutomaticTestCases[tCaseNum].desc);

            fprintf(fp, "Parameter Set:\n");
            fprintf(fp, "Length of Activation Code : %c\n",\
                ActivateAutomaticTestCases[tCaseNum].activationCode[0]);
            fprintf(fp, "Activation Code : %s\n",\
                &(ActivateAutomaticTestCases[tCaseNum].activationCode[1]));

            nRet = ActivateAutomatic( (CHAR *) (ActivateAutomaticTestCases[tCaseNum].\
                                                activationCode) );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "Activate Automatic Successful");
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

void doGetActivationState( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    ULONG pActivationState;
    FILE  *fp = NULL;

    fp = tfopen("../../dms/test/results/getactivationstate.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_GET_ACTIVATION_STATE_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_GET_ACTIVATION_STATE_TESTCASE_NUM,
                       GetActivationStateTestCases[tCaseNum].desc);

            nRet = GetActivationState( &pActivationState );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "pActivationState : %lu\n", pActivationState);
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

void doUIMGetState( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    ULONG pUIMState;
    FILE  *fp = NULL;

    fp = tfopen("../../dms/test/results/uimgetstate.txt", "w");

    if (fp)
    {
        while (tCaseNum < MAX_UIM_GET_STATE_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_UIM_GET_STATE_TESTCASE_NUM,
                       UIMGetStateTestCases[tCaseNum].desc);

            nRet = SLQSUIMGetState( &pUIMState );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "pUIMState : %lu\n", pUIMState);
            }

            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

void doSLQSgetBandCapability( void )
{
    BYTE  tCaseNum = 0;
    ULONG nRet;
    FILE  *fp = NULL;

    fp = tfopen("../../dms/test/results/SLQSgetBandCapability.txt", "a");

    if (fp)
    {
        while (tCaseNum < MAX_GET_BANDCAPABLTY_TESTCASE_NUM)
        {
            fprintf(fp, "\n\nTest Case %d / %d : %s\n",
                       tCaseNum,
                       MAX_GET_BANDCAPABLTY_TESTCASE_NUM,
                       GetBandCapabilityTestCases[tCaseNum].desc);

            nRet = SLQSGetBandCapability(
                    GetBandCapabilityTestCases[tCaseNum].pBandCapability );

            /* Display result code and text */
            doprintreason (fp, nRet);

            if (!nRet)
            {
                fprintf(fp, "pBandCapability : %llx\n",
                    *(GetBandCapabilityTestCases[tCaseNum].pBandCapability));
            }
            tCaseNum++;
        }
    }

    if (fp)
        tfclose(fp);
}

void doSLQSGetCustFeatures( void )
{
    /* Test Cases */
    SLQSGetCustFeaturesTestCase_t SLQSGetCustFeaturesTestCases[] =
    {
        { eQCWWAN_ERR_INVALID_ARG, 1, NULL,          "Invalid Argument" },
        { eQCWWAN_ERR_NONE,        2, &CustFeatures, "Valid Test Case" }
    };

    FILE *fp = tfopen("../../dms/test/results/SLQSGetCustFeatures.txt", "w");
    custFeaturesInfo *pResp;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSGetCustFeaturesTestCases )/
                      sizeof( SLQSGetCustFeaturesTestCases[0] );
        while ( tc < ncases )
        {
            SLQSGetCustFeaturesTestCase_t *pd =
                &SLQSGetCustFeaturesTestCases[tc];

            pResp = pd->pCustFeaturesInfo;
            if( pResp )
            {
                pResp->pDisableIMSI   = pDisableIMSI;
                pResp->pIPFamSupport  = pIPFamSupport;
                pResp->pRMAutoConnect = pRMAutoConn;
                pResp->pGPSSel        = pGPSSel;
            }

            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tc + 1),
                        ncases,
                        pd->desc);

            ULONG nRet = SLQSGetCustFeatures( pResp );

            doprintreason( fp, nRet );
            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSGetCustFeatures Successful\n");
                fprintf(fp, "GPS Enable       : 0x%lX\n", pResp->GpsEnable);
                IFPRINTF(fp,"Disable IMSI     : 0x%X\n",  pResp->pDisableIMSI);
                IFPRINTF(fp,"IP Family support: 0x%X\n",  pResp->pIPFamSupport);
                IFPRINTF(fp,"RM Auto Connect  : 0x%X\n",  pResp->pRMAutoConnect);
                IFPRINTF(fp,"GPS Select       : 0x%X\n",  pResp->pGPSSel);
            }
            tc++;
        }
    }

    if (fp)
        tfclose(fp);
}

void doSLQSSetCustFeatures( void )
{
    /* Test Cases */
    SLQSSetCustFeaturesTestCase_t SLQSSetCustFeaturesTestCases[] =
    {
        {eQCWWAN_ERR_INVALID_ARG, 1, NULL,                 "Invalid Argument" },
        {eQCWWAN_ERR_NONE,        2, &CustFeaturesSetting, "Valid Test Case, GPS enable = 0" },
        {eQCWWAN_ERR_NONE,        3, &CustFeaturesSetting, "Valid Test Case, GPS enable = 1" },
        {eQCWWAN_ERR_QMI_INVALID_ARG, 4, &CustFeaturesSetting, "Invalid Argument Value, GPS Enable = 9" }
    };

    FILE *fp = tfopen("../../dms/test/results/SLQSSetCustFeatures.txt", "w");
    custFeaturesSetting *pResp;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSSetCustFeaturesTestCases )/
                      sizeof( SLQSSetCustFeaturesTestCases[0] );
        while ( tc < ncases )
        {
            SLQSSetCustFeaturesTestCase_t *pd =
                &SLQSSetCustFeaturesTestCases[tc];

            pResp = pd->pCustFeaturesSetting;
            if( pResp )
            {
                pResp->pGPSSel = pGPSSelect;
                pResp->pGPSEnable = &GPSEnable[tc-1];
            }

            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        (tc + 1),
                        ncases,
                        pd->desc);
            fprintf( fp, "SLQSSetCustFeatures parameters set:\n" );
            if( pResp )
            {
                IFPRINTF( fp, "GPS Antenna Select: 0x%x \n",
                          pResp->pGPSSel );
                IFPRINTF( fp, "GPS Enable/Disable: %lu \n",
                          pResp->pGPSEnable );
            }

            ULONG nRet = SLQSSetCustFeatures( pResp );
            doprintreason( fp, nRet );
            tc++;
        }
        fprintf( fp, "\nReset the module to refresh the GPS status\n");
    }

    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQGetCurrentPRLInfo
 *
 * Purpose: Perform the tests that call the SLQGetCurrentPRLInfo() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQGetCurrentPRLInfo( void )
{
    WORD prlVersion;
    BYTE prlPref;

    /* Test Cases */
    SLQSGetCurrentPRLInfo_t SLQSGetCurrentPRLInformation[] =
    {
        { eQCWWAN_ERR_NONE, 1, NULL, NULL, "Fetching None" },
        { eQCWWAN_ERR_NONE, 2, &prlVersion, &prlPref, "Fetching All Values" }
    };

    FILE  *fp = tfopen("../../dms/test/results/dmsgetcurrentprlinfo.txt", "w");
    dmsCurrentPRLInfo resp;

    if (fp)
    {
        BYTE tc = 0;
        BYTE ncases = sizeof( SLQSGetCurrentPRLInformation )/
                      sizeof( SLQSGetCurrentPRLInformation[0] );
        while ( tc < ncases )
        {
            SLQSGetCurrentPRLInfo_t *pd = &SLQSGetCurrentPRLInformation[tc++];
            fprintf(fp, "\nTest Case %d / %d : %s\n",
                        tc,
                        ncases,
                        pd->desc);

            resp.pPRLVersion    = pd->pPRLVersion;
            resp.pPRLPreference = pd->pPRLPreference;

            ULONG nRet = SLQSGetCurrentPRLInfo( &resp );
            doprintreason( fp, nRet );

            if ( eQCWWAN_ERR_NONE == nRet )
            {
                fprintf(fp, "SLQSDmsGetCurrentProtocol Successful\n");
                IFPRINTF(fp, "PRL Version: %x\n", resp.pPRLVersion);
                IFPRINTF(fp, "PRL Preference: %d\n", resp.pPRLPreference);
            }
        }
    }
    if (fp)
        tfclose(fp);
}
