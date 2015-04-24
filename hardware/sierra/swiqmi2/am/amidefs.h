/*************
 *
 * Filename: amidefs.h
 *
 * Purpose:  This file contains internal structures and constants for the
 *           Application Messaging package
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/
/* Includes */
#include "aa/aaglobal.h"
#include "dl/dludefs.h"
#include "us/usudefs.h"
#include "ds/dsudefs.h"
#include "er/erudefs.h"
#include "ic/icudefs.h"
#include "ci/ciudefs.h"
#include "mm/mmudefs.h"
#include "pi/piudefs.h"
#include "sl/sludefs.h"
#include "am/amudefs.h"

/* Timeout guard interval. This value is the basic
 * timeout applied by the api side whenever it calls
 * icapi_rcv(). It is added to the application's
 * specified timeout on API calls, while the caller's
 * timeout is used directly by the SDK when it is
 * awaiting a response from the modem.  This ensure that
 * the modem * timeout will always happen first. If the
 * icapi_rcv() * times out, then this is some kind of
 * system failure, * like the SDK process has died.
 */
#define AMTIMEGUARD 1000

/* DL package logging classes */
#define AMLOG_CLASSA 0x0001     /* INFO - Verbose */
#define AMLOG_CLASSB 0x0002     /* Less Verbose */
#define AMLOG_CLASSC 0x0004     /* Least Verbose */

#define AMSMTRCVTOUT          0 /* Timeout duration in ms (0 to wait forever) */

/* MM buffer pool definitions */
#define AMEVTPOOLSZ           20 /* # event blocks in the pool */
#define AMMSGDEPTH            20 /* Max # - inter-task SMT request msgs */

/* Specify our in-use flag bits */
#define AMMAXNOTIFBITSINUSE   0x1F /* lower 5 bits for notification flags */

/* Specify the maximum number of notifications available - Note that there is
 * no way to figure this out programmatically. This is gotten by using
 * "grep -c SWI_NOTIFY SwiApiNotify.h". If the enum SWI_TYPE_Notify changes,
 * this define must be updated.
 */
#define AMMAXNOTIFY         250

/*************
 *
 * Name:     ameventtype
 *
 * Purpose:  AM Task event types
 *
 * Members:  AM_APP_REQUEST     - Request from the Application
 *           AM_APP_RESP_SENT   - Response sent to application.
 *           AMINVALID          - Invalid event type indication
 *
 * Notes:    None
 *
 **************/
enum ameventtype
{
    AM_APP_REQUEST,
    AM_APP_RESP_SENT,
    AMAPPINVALID  /* should be always last */
};

/*************
 *
 * Name:    amevtblock
 *
 * Purpose: Structure used to pass information into the AM task.
 *
 * Members: amevent      - AM task event type
 *          ammemfreep   - Pointer to location to use when releasing caller-
 *                         supplied memory, if applicable. NULL if nothing to
 *                         release
 *          amdatalen    - Length of data being passed around
 *          amdatap      - pointer to incoming data
 *          amipcchannel - IPC channel of incoming message
 *
 * Notes:
 *
 **************/
struct amevtblock
{
    enum ameventtype amevent;
    swi_uint8        *ammemfreep;
    swi_uint16      amdatalen;
    swi_uint8       *amdatap;
    swi_size_t      amipcchannel;
    struct qllink   ameqlnk;
};

/*************
 *
 * Name:    amapicb
 *
 * Purpose: Control block used by the API-side AM package
 *
 * Members: amapiipcrr-  Storage for IPC control block used for communications
 *                       between the API and AM Request/Response task
 *          amapiipcnot- Storage for IPC control block used for communications
 *                       between the API and AM Notification task
 *
 * Note:    This structure is for the API side and is not required
 *          by the SDK side at all
 *
 **************/
struct amapicb {
    struct icipccb  amapiipcrr;
    struct icipccb  amapiipcnot;
};

/*************
 *
 * Name:    amipcnotifq
 *
 * Purpose: Notification queue structure
 *
 * Members: amchannotifflg   - bit-mapped flag for notification registered
 *                             per IPC channel
 *          amnqlnk          - queue link to next block of structure.
 *
 * Note:    This structure is for the AM package to register return channels
 *          for notifications.
 *
 **************/
struct amipcnotifq
{
    swi_uint16 amchannotifflg;
    struct qllink amnqlnk;
};

/*************
 *
 * Name:    amipccb
 *
 * Purpose: AM IPC Task control block structure
 *
 * Members:
 *          amdlcb     - DL package control block for logging
 *          amicmsgque - AMIPC task's event queue for requests
 *          amevtpool  - MM pool for events coming in from hosts.
 *          amipcwaiter- Waiter structure for blocking AM IPC task until a
 *                       response is sent to the host(s).
 *          amnotifpool - MM pool blocks for our Notification queue.This is
 *                        guaranteed to be enough for the total number of
 *                        notifications defined on the API side.
 *          amnotifheadq- Notification queue head
 *          ameventheadq- Event queue head of local queue (off of SMT queue)
 *          amreqbusy   - Busy flag for when response has not been sent
 *          amipcshell  - Array of DS shell info needed for starting IPC shells
 *                        (thread creation needs storage space, can't use stack)
 *
 * Notes:   The AM IPC task runs on the SDK side only.
 *          Incoming messages are buffered by the DS task which we eventually
 *          release after sending the message.
 *          Note that IPC for Request/Response traffic is handled by DS. We only
 *          need to initialize DS shell using IC defines.
 *
 **************/
struct amipccb
{
    struct dlusercb     amdlcb;
    struct icsmtcb      amicmsgque;
    struct mmpool       amevtpool;
    struct swi_oswaiter amipcwaiter;
#if 0 
    struct mmpool       amnotifpool;
    struct amipcnotifq  amnotifheadq;
#endif
    struct qllink       ameventheadq;
    swi_bool            amreqbusy;
    struct dsipcshell   amipcsh[CIMAXRRPAIR];
};

#include "am/amiproto.h"
