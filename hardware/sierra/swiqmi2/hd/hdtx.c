/*************
 *
 * Filename: hdtx
 *
 * Purpose:  This file contains code for adding framing characters to
 *           outbound data packets
 *
 * Note:     None
 *
 * Copyright: © 2008 Sierra Wireless Inc., all rights reserved
 *
 **************/
/* include files */
#include "aa/aaglobal.h"
#include "mm/mmudefs.h"
#include "er/erudefs.h"
#include "ql/qludefs.h"
#include "sl/sludefs.h"
#include "us/usudefs.h"
#include "pi/piudefs.h"
#include "hd/hdidefs.h"
#include <stdio.h>

/***********************************************************************
 * Static Variables
 ***********************************************************************/

/* Allocate Port Control block */
package struct hdTxCtlBk hdTxpcb;

/*************
 *
 * Name:      hdTxGetCtlBk
 *
 * Purpose:   get the port control block
 *
 * Parms:     none
 *
 * Return:    pointer to HD task port control block
 *
 * Abort:     none
 *
 * Notes:     none
 *
 **************/
package struct hdTxCtlBk *hdGetTxCtlBk()
{
        return &hdTxpcb;
}

/*************
 *
 * Name:      hdTxGetDroppedPktCnt
 *
 * Purpose:   get the Tx dropped-packets count
 *
 * Parms:     none
 *
 * Return:    number of dropped packets
 *
 * Abort:     none
 *
 * Notes:     none
 *
 **************/
package swi_uint32 hdTxGetDroppedPktCnt()
{
    struct hdTxCtlBk *hdTxpcbp = hdGetTxCtlBk();
    return hdTxpcbp->hdtxpktsdropped;
}

/*************
 *
 * Name:    hdTxDone
 *
 * Purpose: To reset state variables once Tx is done.
 *
 * Parms:   paramp - user parameter pointer
 *
 * Return:  no value
 *
 * Abort:   none
 *
 * Notes:
 **************/
local void hdtxDone(void *paramp)
{
    struct hdreq *hdreqp = (struct hdreq *)paramp;

    /* Release the framing buffer */
    mmbufrel( (swi_uint8 *) hdreqp->hdsendblkp );

    /* Call the caller's all-done
     * routine, if one was supplied
     */
    if( hdreqp->hdsendcfp != NULL )
    {
        /* Inform caller via callback routine */
        (hdreqp->hdsendcfp)(hdreqp->hduserp, hdreqp->hdpacketp, NO);
    }

    /* Release the hd buffer */
    mmbufrel( (swi_uint8 *) hdreqp );
}

/*************
 *
 * Name:    hdtxdllstate - process character state machine
 *
 * Purpose: Process the character based on special characters (0x7E, 0x7D)
 *          or normal data.
 *
 * Parms:   txpcbp - pointer to the transmit control block
 *          txchrp - pointer to character to be processed.  Will not
 *                   be escaped if regular data. Escaped if value is
 *                   0x7E or 0x7D
 *          last   - YES - last character has been processed, return a FLAG
 *                   NO  - still not done yet
 *
 * Return:  YES - OK for caller to fetch next character
 *          NO  - Caller should not fetch next character yet
 *
 * Abort:   None
 *
 * Notes:   When calling for the first time, ie, HDWAITTXPKT state,
 *          txchrp does not need to be initialized, a flag is
 *          returned anyway
 *
 **************/
local swi_bool hdtxdllstate(struct hdTxCtlBk *txpcbp,
                            swi_uint8 *txchrp,
                            swi_bool last)
{
    swi_bool fetch = NO;

    /*
     * Caller asked for closing flag, simply reset state and return
     */
    if (last)
    {
        *txchrp = DLL_FLAG;
        txpcbp->hdtxdllstate  = HDWAITTXPKT;
        return (fetch); /* dummy */
    }
    switch (txpcbp->hdtxdllstate)
    {
        case HDWAITTXPKT:
            *txchrp = DLL_FLAG;
            txpcbp->hdtxdllstate = HDDATA;
            fetch = NO;
            break;
        case HDDATA:
            if (*txchrp != DLL_FLAG && *txchrp != DLL_CTRL_ESC)
            {   /* regular character, no need to modify.
                 * Fetch next as well
                 */
                fetch = YES;
            }
            else
            {   /* Special characters, need to modify, don't fetch yet */
                *txchrp = DLL_CTRL_ESC;
                txpcbp->hdtxdllstate = HDESCAPED;
                fetch = NO;
            }
            break;
        case HDESCAPED:
            /* Control ESCAPE would already have been
             * sent, mask the data
             */
            *txchrp ^= DLL_ESC_MASK;
            txpcbp->hdtxdllstate = HDDATA;
            fetch = YES;
            break;
        default:
            /* BAD state - should never get here */
            txpcbp->hdtxdllstate  = HDWAITTXPKT;
            break;
    }
    return (fetch);
}

/*************
 *
 * Name: hdsend
 *
 * Purpose: Entry point to frame and send a data message
 *
 * Parms:   hppacketp  - pointer to a buffer containing the data
 *                       to be sent
 *          packetsz   - size of data in bytes
 *
 * Return:  no value
 *
 * Abort:   if parameters are invalid
 *
 * Notes:   None
 *
 **************/
package swi_bool hdsend(struct hdreq *hdreqp)
{
    struct hdTxCtlBk *hdTxpcbp = hdGetTxCtlBk();
    struct hdSendblk *sendblkp;
    register swi_uint8 *nextbytep;  /* moving message pointer */
    swi_uint16 sizetosend;
    swi_uint8 txchr;                /* byte to be run thru state machine */
    swi_bool fetch;                 /* flag whether next char to be fetched */
    swi_uint8 *hppacketp = hdreqp->hdpacketp;
    swi_uint16 packetsz = hdreqp->hdpacketsz;
    swi_bool bsendstatus = TRUE;

    /* Make sure we get a good packet size */
    if (packetsz > HDMAXMESSAGE)
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d invalid packet size",
                 (char *)__func__, __LINE__);
        erAbort (errmsg, packetsz);
    }

    /* Check if serial port is available. We assume that
     * if the Rx port is online, then the TX port is too.
     */
    if (!hdRxGetStatus())
    {
        /* We don't have the serial port
         * return to caller with error if
         * a callback has been provided
         */
        if( hdreqp->hdsendcfp )
        {
            (hdreqp->hdsendcfp)(hdreqp->hduserp, hppacketp, YES);
        }

        /* Release the hd request buffer */
        mmbufrel( (swi_uint8 *) hdreqp );
        return FALSE;
    }

    /* get pointer to next byte to be copied to the HOLD buffer */
    nextbytep = hppacketp;

    /* Allocate a buffer from the HD pool to store the incoming
     * HIP package as it is being framed
     */
    sendblkp = (struct hdSendblk *) mmbufget( &hdTxpcbp->hdtbufpool );

    /* Make sure we got a pointer, abort otherwise */
    if( sendblkp != NULL )
    {
        /* Store a pointer to the data part of the buffer */
        hdTxpcbp->hdtHoldBufferp = &sendblkp->hddata[0];

        /* Set the pointer to the hold buffer */
        hdTxpcbp->hdtHoldBufferPosp = hdTxpcbp->hdtHoldBufferp;

    }
    else
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d Null framing block ptr",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, 0 );
    }

    /* packetsz is not what we want to count. It's the
     * number of times we have to loop due to stuffing,
     * ie, some characters need two loops to process if
     * stuffed
     */
    while ((nextbytep - hppacketp) != packetsz)
    {
        /* Note, the first time through this loop,
         * the txchr variable is not initialized,
         * but the state machine will be in the
         * WAITTXPKT state, meaning it will fill
         * txchr with a flag character.
         */
        fetch = hdtxdllstate (hdTxpcbp, &txchr, NO);
        if (fetch == YES)
        {   /* regular data, OK to get next */
            *hdTxpcbp->hdtHoldBufferPosp++ = txchr;
            txchr = *++nextbytep;
        }
        else
        {   /* escaped data, re-load data for masking
             * do not move position yet
             */
            *hdTxpcbp->hdtHoldBufferPosp++ = txchr;
            txchr = *nextbytep;
        }
    }

    /* add FCS */
    piput16 ( hdreqp->hdfcs, &hdTxpcbp->hdtHoldBufferPosp );

    /* Add frame closing character  */
    fetch = hdtxdllstate (hdTxpcbp, &txchr, YES);
    *hdTxpcbp->hdtHoldBufferPosp++ = txchr;

    /* send data off */
    sizetosend = hdTxpcbp->hdtHoldBufferPosp - hdTxpcbp->hdtHoldBufferp;

    hdreqp->hdsendblkp = sendblkp;

    /* Send to destination using user-provided send function */
    hdreqp->hdusersendfp (&sendblkp->hddata[0], sizetosend );

#if 0
    /* Send the packet to the modem */
    if (usgetep(USEP_HIP, &hdep))
        ussendtty( hdep.uswriteep, &sendblkp->hddata[0], sizetosend );
    else
    {
        /* Update dropped packets counter and log it */
        hdTxpcbp->hdtxpktsdropped++;
        bsendstatus = FALSE;
    }
#endif

    /* Clean up */
    hdtxDone( (void *) hdreqp );

    /* Reset the state to its initial value */
    hdTxpcbp->hdtxdllstate  = HDWAITTXPKT;

    return bsendstatus;
}

/*************
 * Name:    hdTxInit
 *
 * Purpose: Initialize the hd Tx pkg in a single-threaded environment.
 *
 * Params:  None
 *
 * Return:  None
 *
 * Note:    None
 *
 * Abort:   n/a
 *
 *************/
package void hdTxInit ( void )
{
    struct hdTxCtlBk *hdTxpcbp = hdGetTxCtlBk();

    /* Allocate memory for the Transmit buffer block
     * With stuffing, message could be twice max
     * size of message
     */
    hdTxpcbp->hdtHoldBufferp = NULL;

    /* Create memory pool for HD data packets */
    mmpcreate( &hdTxpcbp->hdtbufpool, "hdtxbufpool",
               HDDATABUFCNT, sizeof(struct hdSendblk) );

    /* Create memory pool for HD req packets */
    mmpcreate( &hdTxpcbp->hdtreqpool, "hdtxreqpool",
               HDDATABUFCNT, sizeof(struct hdreq) );

    /* Reset the state to its initial value */
    hdTxpcbp->hdtxdllstate = HDWAITTXPKT;

    /* Number of Tx packets dropped */
    hdTxpcbp->hdtxpktsdropped = 0;
}
