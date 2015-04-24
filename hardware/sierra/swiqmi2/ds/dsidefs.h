/*************
 *
 * Filename: dsidefs.c
 *
 * Purpose:  This file contains internal structures and constants for the
 *           Device Service package
 *
 * Copyright: © 2008 Sierra Wireless Inc., all rights reserved
 *
 **************/
/* Includes */
#include "aa/aaglobal.h"
#include "dl/dludefs.h"
#include "ic/icudefs.h"
#include "mm/mmudefs.h"
#include "pi/piudefs.h"
#include "sl/sludefs.h"
#include "us/usudefs.h"
#include "ci/ciudefs.h"
#include "ds/dsudefs.h"

/* Definitions */

/* DL package logging classes */
#define DSLOG_CLASSA   0x0001
#define DSLOG_CLASSB   0x0002
#define DSLOG_CLASSC   0x0004

/* Timeout to use when waiting for incoming IPC traffic from API side */
#define DSIPCTIMEOUT   0    /* wait forever */

/* MM buffer pool definitions */
#define DSIPCBUFSZ     2000 /* Size of an IPC Buffer - TBD tune later */
#define DSIPCPOOLSZ    45   /* # of buffers in pool  - TBD tune later */

/* Receive buffer size */
#define DSRXPOOLSZ      5     /* # blocks in the pool */
#define DSQQMIRXPOOLSZ  (QMI_SUPPORTED_CLNT_MAX - QMI_SUPPORTED_CLNT_MIN)

/* EP buffer definitions */
#define DSEPBUFSZ      4096 /* Size of an EP input Buffer */

/* Transmit Small Buffer Pool Definitions */
#define DSEPSMLBUFSIZE 300     /* payload of each block in bytes */

/* DS US task pause delay. Used when the modem disappears
 * and a call to usrcvtty() returns FALSE
 */
#define DSRXSCANDELAY 1000  /* Delay in ms */

/*************
 *
 * Name:    dsipccb
 *
 * Purpose: DS IPC Task control block structure
 *
 * Members:
 *          dsdlcb     - DL package control block for logging
 *          dsbufpool  - MM pool structure. Blocks in this pool are used
 *                       to store incoming data packets from the API
 *                       side
 *          dsrcvcbckp - pointer to IPC receive handler
 *                       IPC receive handler is installed when the
 *                       channel is opened with the following prototype:
 *              inipcmsgp - pointer to the beginning of the received message
 *              inmsglen  - length of the received message
 *              memfreep  - pointer to memory to be released by the callback
 *              ipcchannel  -IPC channel used for this service
 * Notes:   The DS IPC task runs on the SDK side only.
 *          This is a 'generic' DS IPC Task control block structure
 *          Several DS IPC Tasks may exist concurrently; each such task
 *          serves one IPC channel.
 *
 *
 **************/
struct dsipccb
{
    struct dlusercb  dsdlcb;
    struct mmpool    dsbufpool;
    void (*dsrcvcbckp)(swi_uint8 *inipcmsgp, swi_uint32 inmsglen,
        swi_uint8 *memfreep, swi_uint16 ipcchannel );
};

/*************
 * Name:    dsappipccb
 *
 * Purpose: DS Application-side IPC channel control block structure
 *
 * Members: dsipccbhdl   - IC package IPC channel control block
 *          dsnamep      - Pointer to a user-supplied name useful
 *                         for troubleshooting
 *          dschannel    - IPC channel number this instance of the
 *                         application side DS package is interacting
 *                         with
 *
 * Note:    The application side of the DS package interacts with
 *          IPC channels only, other device types are not currently
 *          supported
 *
 **************/
struct dsappipccb {
    struct icipccb dsipccbhdl;
    swi_uint8 *dsnamep;
    swi_uint16 dschannel;
};

/*************
 * Name:    dsappcb
 *
 * Purpose: DS Application-side task control block structure
 *
 * Members: dsdxchancb   - array of CIMAXIPCCHAN IPC channel control blocks
 *
 * Note:    The application side of the DS package interacts with
 *          IPC channels only, other device types are not currently
 *          supported
 *
 **************/
struct dsappcb
{
    struct dsappipccb dsdxchancb[CIMAXIPCCHAN];
};

/**
 * Name: dsuscb
 *
 * Purpose: DS Endpoint-side Task control block
 *
 * Members: dsusdldcb       - DL package control block for logging
 *          dsusdwaiter     - Waiter structure used by the DS Endpoint
 *                            task for timing during startup and for
 *                            blocking during waiting for messages from the
 *                            modem
 *          dsbufpool       - MM pool structure. Blocks in this pool are used
 *                            to store incoming data packets from the modem for
 *                            EP side
 *          dsrxpktsdropped - number of rx packets dropped
 *
 * Notes: none
 *
 */
struct dsuscb {
    struct dlusercb     dsusdldcb;
    struct swi_oswaiter dsusdwaiter;
    struct mmpool       dsbufpool;
    swi_uint32          dsrxpktsdropped;
};

#include "ds/dsiproto.h"
