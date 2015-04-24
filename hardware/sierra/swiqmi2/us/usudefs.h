/*
 * Filename: usudefs.h
 *
 * Purpose:  User definitions for the US package
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 */

#ifndef USUDEFS_H
#define USUDEFS_H

/* Includes */
#include "aa/aaglobal.h"

/* Defines */

/* Invalid EP numbers */
#define USEPINVALID 0

/* Direction Definitions */
#define USWRITE       FALSE
#define USREAD        TRUE

/*
 * Name: swi_usvendors
 *
 * Purpose: Enumerated list of USB Vendor IDs known by this SDK.
 *
 * Members: SWI_VIDSIERRA   - Sierra Wireless Vendor ID
 *          SWI_VIDHP       - Sierra Wireless/HP products
 *          QCT_VID         - QCT VID (useful for testing reference devices)
 * Notes: none
 *
 */
enum swi_usvendors
{
    SWI_VIDSIERRA = 0x1199,
    SWI_VIDHP     = 0x03F0,
    QCT_VID       = 0x05C6
};

/*
 * Name: swi_usproducts
 *
 * Purpose: Enumerated list of product IDs known by this SDK
 *
 * Members:
 *      SWI_QMIPID_APP      Sierra Wireless QMI BOOT device
 *      SWI_QMIPID_APP      Sierra Wireless QMI device
 *      GOBI3KGENERIC_BOOTx Sierra Gobi3k BOOT devices
 *      GOBI3KGENERIC_APPx  Sierra Gobi3k devices
 *      GOBI3KHP_BOOT       HP Gobi3k BOOT device
 *      GOBI3KHP_APP        HP Gobi3k device
 *      QCTG3KBOOT          QCT BOOT device (useful for testing reference devices)
 *      QCTG3K              QCT device (useful for testing reference devices)
 *
 * Notes: In the list above, "Generic" indicates the modem has
 *        the Sierra Wireless Vendor ID in its USB descriptor
  */
enum swi_usproducts
{
    SWI_QMIPID_BOOT     = 0x68A2,
    SWI_QMIPID_APP      = SWI_QMIPID_BOOT,
    GOBI3KGENERIC_BOOT0 = 0x9010,
    GOBI3KGENERIC_BOOT1 = 0x9012,
    GOBI3KGENERIC_BOOT2 = 0x9014,
    GOBI3KGENERIC_BOOT3 = 0x9018,
    GOBI3KGENERIC_APP0  = 0x9011,
    GOBI3KGENERIC_APP1  = 0x9013,
    GOBI3KGENERIC_APP2  = 0x9015,
    GOBI3KGENERIC_APP3  = 0x9019,
    GOBI3KHP_BOOT       = 0x361D,
    GOBI3KHP_APP        = 0x371D,
};

/*
 * Name: usdevicemode
 *
 * Purpose: Enumerated list of modem operating modes, used during
 *          device detection to identify what mode the modem has
 *          been detected in
 *
 * Members: USMODE_MIN      - For range checking
 *          USMODE_BOOTHOLD - For boot-and-hold operation
 *          USMODE_NORMAL   - For normal modem operation
 *          USMODE_MAX      - Maximum value for range checking
 *
 * Notes: DO NOT CHANGE THIS ENUMERATION UNLESS YOU KNOW WHAT YOU
 *        ARE DOING. YOU WILL BREAK THE LOGIC IN "usdoloopback()"
 *        IF YOU DO.
 */
enum usdevicemodes{
    USMODE_MIN = 0,
    USMODE_BOOTHOLD = USMODE_MIN,
    USMODE_APP,
    USMODE_MAX
};

/*************
 *
 * Name:    usb_device_event
 *
 * Purpose: Enumerated list of usb device events
 *
 * Members: US_BOOT_DEVICE_READY_EVT    - device ready in boot & hold mode
 *          US_APP_DEVICE_READY_EVT     - device ready in application mode
 *          US_DEVICE_DISCONNECTED_EVT  - device disconnected
 *
 * Notes:   None
 *
 **************/
enum usb_device_event{
    US_BOOT_DEVICE_READY_EVT,
    US_APP_DEVICE_READY_EVT,
    US_DEVICE_DISCONNECTED_EVT
};

/*
 * Name:    useptype
 *
 * Purpose: Enumerated list of end points
 *
 * Members:     USEP_DIAG           - DIAG
 *              USEP_QDL            - Gobi QDL (boot) interface
 *              USEP_NMEA           - NMEA
 *              USEP_ATCMD          - AT Command
 *              USEP_QMI            - QMI
 *              USEP_MASS_STORAGE   - mass storage
 *              USEP_INVALID        - Invalid end point
 *
 *  Notes:
 */
enum useptype {
    USEP_DIAG,
    USEP_QDL,
    USEP_NMEA,
    USEP_ATCMD,
    USEP_QMI,
    USEP_MASS_STORAGE, /* not implemented */
    USEP_INVALID
};

/*
 * Name:    usbep
 *
 * Purpose: US endpoints
 *
 * Members: usreadep    - read endpoint
 *          uswriteep   - write endpoint
 *
 * Notes: none
 *
 */
struct usbep {
    swi_uint32 usreadep;
    swi_uint32 uswriteep;
};

#include "usuproto.h"
#endif /* USUDEFS_H */
