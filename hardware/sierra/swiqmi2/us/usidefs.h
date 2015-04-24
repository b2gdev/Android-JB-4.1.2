/*
 * Filename: usidefs.h
 *
 * Purpose:  Internal definitions for USB Scanning task package
 *
 * Copyright: © 2011-2012 Sierra Wireless Inc., all rights reserved
 *
 */

/* Includes */
#include "aa/aaglobal.h"
#include "dl/dludefs.h"
#include "os/swi_ossdk.h"
#include "pi/piudefs.h"
#include "ic/icudefs.h"
#include "ci/ciudefs.h"
#include "us/usudefs.h"

/* Defines */

/* DL Logging Mask Definitions */
#define USLOG_CLASSA           0x0001  /* Important but not verbose logging */
#define USLOG_CLASSB           0x0002  /* read path, can be disabled */
#define USLOG_CLASSC           0x0004  /* Write path, can be disabled */
#define USLOG_CLASSD           0x0008  /* Verbose logging, not classified */

#define USSLEEPSCAN            300   /* Scan task Sleep duration (ms) */

/*
 * Name:    usbeptable
 *
 * Purpose: US endpoints table
 *
 * Members: usdiagep        - r/w endpoints for DIAG
 *          usnmeaep        - r/w endpoints for NMEA
 *          usatcmdep       - r/w endpoints for AT cmd
 *          usmassstorageep - r/w endpoints for MASS STORAGE (not implemented)
 *          usqmiep         - r/w endpoints for QMI service
 *          usqdlep         - r/w endpoints for QDL service
 *          usqdlep2        - r/w end points for QDL service on Sierra Gobi devices
 *
 * Notes: none
 *
 */
struct usbeptable {
    struct usbep   usdiagep;
    struct usbep   usnmeaep;
    struct usbep   usatcmdep;
    struct usbep   usmassstorageep;
    struct usbep   usqmiep;
    struct usbep   usqdlep;
    struct usbep   usqdlep2;
};

/*
 * Name: usscb
 *
 * Purpose: US scan task control block
 *
 * Members: uscwaiterscan
 *                     - An OS package Waiter structure used by
 *                       the US scan task to generate timed intervals
 *          uscusbctl  - The main USB control structure required
 *                       by the OS package for all interaction
 *                       with its USB subroutine calls. There is
 *                       only one of these structures for the
 *                       whole SDK
 *          usdlscb    - User control block for the DL package
 *                       so Scan task can generate log msgs
 *          useptab    - Endpoint number table for all possible endpoints;
 *                       contains read/write endpoints
 *
 * Notes: none
 *
 */
struct usscb {
    struct swi_oswaiter uscwaiterscan;
    struct swi_osusb    uscusbctl;
    struct dlusercb     usdlscb;
    struct usbeptable   useptab;
};

#include "usudefs.h"
#include "usiproto.h"
