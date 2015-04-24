/**
 *
 * @ingroup swiqmitool
 *
 * @file
 * common functions for swi qmi tools 
 *
 * @author
 * Copyright: © 2012 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */

/* include files */
#define  LOG_TAG  "swiqmi_tool"
#include "swiril_log.h"
#include "qmerrno.h"
#include "SWIWWANCMAPI.h"

typedef struct device_info_param{
  CHAR deviceNode[256];
  CHAR deviceKey[16];
} device_info_t;

static device_info_t devices[1] = { { {'\0'}, {'\0'} } };
static device_info_t *pdev = &devices[0];


/**
 *
 * Connects the CM API library to the specified QC WWAN device.
 *
 * @param 
 *     none
 *
 * @return
 *     none
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 *
 */
void qmiDeviceConnect( )
{
    LOGD("qmiDeviceConnect Entered. \n");
    BYTE DevicesSize = 1;
    ULONG nRet = 0;


    /* set absolute path */
    SetSDKImagePath("/system/bin/slqssdk");

    /* SLQSStart() could return an error if the SLQS daemon is restarted, so
       try again in this case.
     */
    while ( (nRet = SLQSStart()) != eQCWWAN_ERR_NONE) {
        LOGI ("SLQSStart failed.. %lx\n", nRet);
        sleep(1);
    }

    nRet = QCWWANEnumerateDevices(&DevicesSize, (unsigned char *)pdev);
    while (nRet != 0) 
    {
        LOGE("QCWWAN2kEnumerateDevices unable to find device..: %lu", nRet);
        sleep(1);
        DevicesSize = 1;
        nRet = QCWWANEnumerateDevices(&DevicesSize, (unsigned char *)pdev);
    }

    LOGD("\n#devices: %d\ndeviceNode: %s\ndeviceKey: %s\n",
        DevicesSize,
        pdev->deviceNode,
        pdev->deviceKey);

    /* connect to QMI device */
    nRet = QCWWANConnect(pdev->deviceNode, pdev->deviceKey);
    while ( nRet != 0)
    {
        LOGE("QCWWAN2kConnect unable to find device..: %lu", nRet);
        sleep(1);
        nRet = QCWWANConnect(pdev->deviceNode, pdev->deviceKey);
    }

    LOGD("\nQCWWAN2kConnect found device. \n");
    LOGD("qmiDeviceConnect Exit. \n");
}

/**
 *
 * Disonnects the CM API library to the specified QC WWAN device.
 *
 * @param 
 *     none
 *
 * @return
 *     none
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 *
 */
void qmiDeviceDisconnect(void)
{
    LOGD("qmiDeviceDisconnect Entered. \n");
    ULONG nRet = 0;
    
    nRet = QCWWANDisconnect();
    if (nRet != 0 ) 
    {
        LOGE("QCWWANDisconnect Unable to Disconnect from SDK..: %lu", nRet);
    }
    LOGD("qmiDeviceDisconnect Exit. \n");
}

/**
 *
 * To get a 16-bit value from a packet that is in network byte order and 
 * increment the packet pointer passed the read field.
 *                         
 * @param[in/out] packetpp
 *                memory location of the pointer to the buffer where the 16-bit 
 *                value will be read from. The content of this pointer is
 *                incremented to point to the next 16-bit value to be read
 *
 * @return
 *      16-bit value
 *
 * @abort
 *      none
 *
 * @note
 *      This function performs no pointer validation.
 */
unsigned short piget16(unsigned char **packetpp)
{
    unsigned short field;
    unsigned char *packetp;

    packetp = *packetpp;

    field = *(packetp + 1);
    field <<= 8;
    field += *packetp;
    packetp += 2;

    *packetpp = packetp;

    return(field);
}

/**
 *
 * To get a 32-bit value from a packet that is in network byte order and 
 * increment the packet pointer passed the read field.
 *                         
 * @param[in/out] packetpp
 *                memory location of the pointer to the packet where the 32-bit
 *                value will be read from. The content of this pointer is
 *                incremented to point to the next 32-bit value to be read
 *
 * @return
 *      32-bit value
 *
 * @abort
 *      none
 *
 * @note
 *      This function performs no pointer validation.
 */
unsigned long piget32(unsigned char **packetpp)
{
    unsigned int  field;
    unsigned char *packetp;

    packetp = *packetpp;

    field =  ((unsigned int)*(packetp + 3)) << 24;
    field += ((unsigned int)*(packetp + 2)) << 16;
    field += ((unsigned int)*(packetp + 1)) << 8;
    field += *packetp;
    packetp += 4;

    *packetpp = packetp;

    return(field);
}
