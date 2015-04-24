/*
 *  Filename: icudefs.h
 *
 *  Purpose:  User definitions for the IC package
 *
 *  Copyright: © 2008-2012 Sierra Wireless Inc., all rights reserved
 *
 */

#ifndef icudefs_h
#define icudefs_h

#include "os/swi_ossdk.h"
#include "ql/qludefs.h"

/* Maximum IPC reading timeout */
#define ICIPC_MAXRDTIMEOUT     300000

/* Tick Task - tick sending interval value in milliseconds */
#define ICTTTICK   100

/*************
 *
 * Name:    icsmtcb
 *
 * Purpose: Structure that implements a shared-memory an inter-task
 *          messaging facility.
 *
 * Members: icsmtputp    - Location to put the next message to
 *          icsmtgetp    - Location to get the next message from
 *          icsmtstartp  - Pointer to the start of the message area
 *          icsmtendp    - Pointer to the last entry in the msg area
 *          icsmtqnamep  - Pointer to a caller-supplied string
 *          icsmtrcvcnt  - counts received messages since reset
 *          icsmtsendcnt - counts messages that has been sent since reset
 *          icsmtmaxqueued - maximum messages that were on  a queue
 *          icsmtwaiter  - Waiter structure. Allows the thread that
 *                         owns this structure to block waiting for
 *                         incoming messages
 *          issmtsem     - Semaphore structure used by callers to
 *                         obtain exclusive access to this structure
 *          mstimeout      signal the registered client periodically every
 *                         mstimeout seconds.
 *          msruntime      running count-down of mstimeout
 *
 * Notes:   For use by threads within the SDK process only
 *
 *          Later when this SDK needs to support 64 bit architectures,
 *          another declaration for the pointers below will be needed.
 *          This can wait until the next release when we have time to
 *          fix this properly
 *
 **************/
struct icsmtcb
{
    swi_uint32 *icsmtputp;
    swi_uint32 *icsmtgetp;
    swi_uint32 *icsmtstartp;
    swi_uint32 *icsmtendp;
    swi_uint8  *icsmtqnamep;
    swi_uint32 icsmtrcvcnt;
    swi_uint32 icsmtsendcnt;
    swi_uint32 icsmtmaxqueued;
    swi_uint32 mstimeout;
    swi_uint32 msrunttime;
    /* Structures from other packages */
    struct swi_oswaiter icsmtwaiter;
    struct swi_ossemaphore icsmtsem;
};

/*************
 *
 * Name:    icipccb
 *
 * Purpose: Structure that implements a control block for
 *          packages needing to use IPC facilities
 *
 * Members: icipcip      - OS package ipc control block structure
 *          icipcopen    - Flag to indicate whether the channel has
 *                         been opened yet. Must be open before send
 *                         or receive can be done
 *          icipcindex   - The internal representation of the IPC name
 *                         being used by the caller
 *          icipcsdktoapidropped
 *                       - The number of sdk to api messages dropped due to API
 *                         side not there for this channel.
 * Notes:   none
 *
 **************/
struct icipccb
{
    struct swi_osipc icipcip;
    swi_bool         icipcopen;
    swi_size_t       icipcindex;
    swi_uint32       icipcsdktoapidropped;
};

/*
 * Name:    icttregqblk
 *
 * Purpose: Registration Queue Block.
 *
 * Members:
 *          ictticsmtp   - pointer to where to send the tick message
 *          ictttickmsg  - tick message to send
 *          icttqlink    - queue link
 *
 * Note:    Caller identification string is in ictticsmt.
 *
 */
struct icttregqblk
{
    struct icsmtcb  *ictticsmtp;
    swi_uint32      ictttickmsg;
    struct qllink   icttqlink;
};

/* User prototypes */
#include "ic/icuproto.h"

#endif
