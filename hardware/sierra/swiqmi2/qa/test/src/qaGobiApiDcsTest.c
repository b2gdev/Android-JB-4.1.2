/**************
 *
 *  Filename:   qaGobiApiDcsTest.c
 *
 *  Purpose:    QMI DCS service test routines
 *
 * Copyright: © 2011 Sierra Wireless, Inc., all rights reserved
 *
 **************/

/* Linux definitions */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "sludefs.h"

#include "qaGobiApiDcs.h"
#include "qatesthelper.h"
#include "qatestproto.h"

/*
 * Name:     doDCSGetConnecteDevID
 *
 * Purpose:  QCWWAN2kGetConnectedDeviceID API driver
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doDCSGetConnecteDevID( void )
{
    FILE  *fp = NULL;
    CHAR  devid[QMI_DEVICE_PATH_MAX_SIZE];
    ULONG devidsz = sizeof(devid);
    CHAR  devkey[QMI_MEID_BUF_SIZE + 1];
    ULONG devkeysz = sizeof(devkey);
    ULONG nRet;

    fp = tfopen("../../dcs/test/results/dcsgetconnectedevid.txt", "w");

    if(fp)
    {
        nRet = QCWWAN2kGetConnectedDeviceID( devidsz,
                                              devid,
                                              devkeysz,
                                              devkey );

        /* Display result code and text */
        doprintreason (fp, nRet);

        if ( !nRet )
        {
            fprintf(fp, "DCSGetConnectedDevID Successful\n");
            fprintf(fp, "device id:\t%s\ndevice key:\t%s\n", devid, devkey);
        }
        else
        {
            fprintf(fp, "DCSGetConnectedDevID Failed\n");
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:     doSLQSGetUsbPortNames
 *
 * Purpose:  SLQSGetUsbPortNames API driver
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSGetUsbPortNames( void )
{
    FILE *fp = NULL;
    ULONG nRet;
    struct DcsUsbPortNames UsbPortNames;

    slmemset ((char*)&UsbPortNames, EOS, sizeof (struct DcsUsbPortNames));

    fp = tfopen("../../dcs/test/results/slqsgetusbportnames.txt", "w");

    if(fp)
    {
        nRet = SLQSGetUsbPortNames( &UsbPortNames );

        /* Display result code and text */
        doprintreason (fp, nRet);

        if ( !nRet )
        {
            fprintf( fp, "SLQSGetUsbPortNames Successful\n");
            fprintf( fp, "AT Command Port: %s\n", UsbPortNames.AtCmdPort );
            fprintf( fp, "NMEA Port      : %s\n", UsbPortNames.NmeaPort );
            fprintf( fp, "DM Port        : %s\n", UsbPortNames.DmPort );
        }
        else
        {
            fprintf( fp, "SLQSGetUsbPortNames Failed\n" );
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:     doSLQSGetDeviceMode
 *
 * Purpose:  SLQSGetDeviceMode API driver
 *
 * Return:   none
 *
 * Notes:    none
 *
 */
void doSLQSGetDeviceMode( void )
{
    FILE *fp = NULL;
    ULONG nRet;
    BYTE DeviceMode;


    fp = tfopen("../../dcs/test/results/slqsgetdevicemode.txt", "w");

    if(fp)
    {
        nRet = SLQSGetDeviceMode( &DeviceMode );

        /* Display result code and text */
        doprintreason (fp, nRet);

        if ( !nRet )
        {
            fprintf( fp, "SLQSGetDeviceMode Successful\n");
            fprintf( fp, "Device Mode: %d\n", DeviceMode );
        }
        else
        {
            fprintf( fp, "SLQSGetDeviceMode Failed\n" );
        }
    }
    if (fp)
        tfclose(fp);
}

/*
 * Name:    doSLQSKillSDKProcess
 *
 * Purpose: Perform the tests that call the SLQSKillSDKProcess() API
 *
 * Return:  none
 *
 * Notes:   none
 *
 */
void doSLQSKillSDKProcess( void )
{
    ULONG  nRet;
    FILE   *fp = NULL;

    fp = tfopen("../../dcs/test/results/slqskillsdkprocess.txt", "w");

    if (fp)
    {
        nRet = SLQSKillSDKProcess();

        doprintreason( fp, nRet );

        if ( !nRet )
        {
            fprintf (fp, "SLQSKillSDKProcess Successful\n");
        }
        else
        {
            fprintf( fp, "SLQSKillSDKProcess Failed\n" );
        }
    }

    if (fp)
        tfclose(fp);
}

