/*************
 *
 * Filename: hdidefs
 *
 * Purpose:  Internal constants and data structures for the hd package.
 *
 * Note:     None
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/
/*
 *  Local constants and enumerated types.
 */
#ifndef hdidefs_h
#define hdidefs_h

//#include "hp/hpudefs.h"
#include "mm/mmudefs.h"

/* Async HDLC achieves data transparency at the byte level by using
 * two special values. The first is a flag value which begins and ends
 * every packet: 
 */
#define  DLL_FLAG      0x7e

/* The flag value might appear in the data.  If it does, it is sent as
 * a two-byte sequence consisting of a special escape value followed by
 * the flag value XORed with 0x20.  This gives a special meaning to the
 * escape character, so if it appears in the data it is itself escaped
 * in the same way. 
 */
#define  DLL_CTRL_ESC  0x7d
#define  DLL_ESC_MASK  0x20

/* The number of buffers in each HD pool is defined here */
#define HDDATABUFCNT  4   

/* Memory Pool Definitions */
#define HDMAXMESSAGE  1024          /* Max message size */
#define HDRXPOOLSZ    20            /* # blocks in the pool */
#define HDRXBUFSIZE   HDMAXMESSAGE  /* Size of each block in the pool */
#define HDALLOCATE    TRUE          /* Allocate from the free pool */
#define HDNOALLOCATE  FALSE         /* No need to allocate a buffer */

/*************
 *
 * Name:    hdtxchrstate
 *
 * Purpose: These are the per character state machine states.
 *             
 * Members: HDWAITPKT - Wait for the start of a packet 
 *          HDDATA    - handle regular data
 *          HDESCAPED - handle escaped data
 *
 * Notes:   This state machine provides all the data link layer transmit 
 *          capabilities - start and end flags, escaping of data
 *
 **************/
enum hdtxchrstate
{
    HDWAITTXPKT = 0,
    HDDATA,
    HDESCAPED
};

/*************
 *
 * Name:    hdrxdllstate - Receive task character state machine      
 *
 * Purpose: These are the state machine states.
 *             
 * Members: WAIT_FOR_SYNC  - Waiting for a flag to start a packet 
 *          SYNCED_NO_DATA - Have a flag, expecting the packet to start
 *          COLLECT_DATA   - In the middle of a packet 
 *          ESCAPED        - ESCAPED, char needed
 *
 * Notes:   none
 *
 **************/
enum hdrxdllstate 
{
    WAIT_FOR_SYNC = 0, /* Waiting for a flag to start a packet       */
    SYNCED_NO_DATA,    /* Have a flag, expecting the packet to start */
    COLLECT_DATA,      /* In the middle of a packet                  */
    ESCAPED            /* ESCAPED ,char needed,                      */
};

/*************
 * 
 * Name:      hdSendblk
 *  
 * Purpose:   Provides a structure for framing outbound data requests
 *
 * Members:   hddata  - The data portion of the structure, an array of 
 *                      bytes large enough to hold the longest framed
 *                      HIP message.
 *
 * Notes:     None
 *
 *************/
struct hdSendblk {
    swi_uint8 hddata[2*HDMAXMESSAGE];
};

/*************
 * 
 * Name:      hdreq
 *  
 * Purpose:   Provides a structure for hd request blocks.
 *
 * Members:   hdsendblkp - Pointer to a send block structure 
 *            hdpacketp  - Pointer to a data packet.
 *            hdpacketsz - Size of data packet 
 *            hdsendcfp  - send completion call back function 
 *            hduserp    - user parameter for call back function
 *            hdusersendfp - user-provided send function
 *            hdfcs      - FCS passed in by user
 *                      
 * Notes:     none
 *
 **************/
struct hdreq {
    struct hdSendblk *hdsendblkp;
    swi_uint8 *hdpacketp;
    swi_uint16 hdpacketsz;
    void (*hdsendcfp)(void *userp, swi_uint8 *bufp, swi_bool errorflg);
    void *hduserp;    
    void (*hdusersendfp)(swi_uint8 *bufp, swi_uint32 pktsize);
    swi_uint16 hdfcs;
};


/*************
 *
 * Name:    hdtxCtlBk
 *
 * Purpose: Control structure for transmitting packets through the HD facility
 *             
 * Members: hdtxdllstate  - data link state
 *          hdtbufpool    - OS memory pool structure for allocating memory for
 *                          packets to be Tx through the HD package
 *          hdtreqpool    - OS memory pool structure for allocating memory for
 *                          request blocks to be Tx through the HD package
 *          hdttxHoldBufferp - pointer to allocated buffer space for output data
 *          hdttxHoldBufferPosp - pointer to current location in the HoldBuffer
 *          hdtxpktsdropped - number of tx packets dropped 
 *
 * Notes:   none
 *
 **************/
struct hdTxCtlBk
{
    enum hdtxchrstate hdtxdllstate;
    struct mmpool hdtbufpool;
    struct mmpool hdtreqpool;
    swi_uint8 *hdtHoldBufferp;
    swi_uint8 *hdtHoldBufferPosp;
    swi_uint32  hdtxpktsdropped;
};

/*************
 *
 * Name:    hdRxCtlBk - Serial receive task Port control block 
 *
 * Purpose: This structure contains Port related data.
 *             
 * Members: hdrdllstate      - data link engine state
 *          hdRxPool         - Memory pool control structure for Rx Pool
 *          hdOutoffset      - Offset into hdRxPool blk
 *          hdrMemrelp       - ptr to addr. to use when releasing hdRxPool buf
 *          hdrHoldBufferp   - ptr to allocated space for storing received data
 *          hdrHoldBufferEndp - ptr to current copy location in buffer
 *          hdrerror         - state engine error
 *          hdrdatavalid     - flag valid character returned from state engine
 *          hdrendframe      - flag end of frame encountered
 *          hdrdatacount     - state engine data counter
 *          hdrxready        - flag for port ready checks
 *
 * Notes:   none
 *
 **************/
struct hdRxCtlBk
{
    enum hdrxdllstate hdrdllstate;
    struct mmpool hdRxPool;
    swi_uint32 hdOutoffset;
    swi_uint8 *hdrMemrelp;
    swi_uint8 *hdrHoldBufferp;
    swi_uint8 *hdrHoldBufferEndp;
    swi_bool  hdrerror;
    swi_bool  hdrdatavalid;
    swi_bool  hdrendframe;
    swi_uint16 hdrdatacount;
    void     *hdrnuserp;
    swi_bool hdrxready;
};

#include "hdiproto.h"

#endif
