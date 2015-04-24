/*************
 *
 * Filename:    hdrx
 *
 * Purpose:     This file contains code that implements the "HDLC" deframing
 *              mechanism, incoming packet processing.
 *
 * Note:
 * Copyright:   � 2008 Sierra Wireless, Inc., all rights reserved
 *
 **************/
/* include files */
#include "aa/aaglobal.h"
#include "ql/qludefs.h"
#include "sl/sludefs.h"
#include "sl/sludefs.h"

#include "hdidefs.h"
#include <stdio.h>

/* Defines */

/* Allocate local storage for Receive Control block */
local struct hdRxCtlBk hdRxpcb;


/*************
 *
 * Name:     hdGetRxCtlBk
 *
 * Purpose:  get the port control block
 *
 * Parms:    none
 *
 * Return:   pointer to HD package receive port control block
 *
 * Abort:    none
 *
 * Notes:    None
 *
 **************/
package struct hdRxCtlBk *hdGetRxCtlBk()
{
        return &hdRxpcb;
}

/*************
 *
 * Name:     hdRxGetStatus
 *
 * Purpose:  get the Receive ready status
 *
 * Parms:    None
 *
 * Return:   YES - port is ready
 *           NO - port is not ready
 *
 * Abort:    None
 *
 * Notes:    For now we always return 1 until we know what to do about
 *           the Rx/Tx port status
 *
 **************/
package swi_bool hdRxGetStatus ()
{
        return (1);
}

/*************
 *
 * Name:    hdrxreset - initialize serial receive variables
 *
 * Purpose: To reset the serial Rx state variables so HD can start
 *          deframing another received packet
 *
 * Parms:   hdRxpcbp pointer to port control block
 *          HDALLOCATE   - Allocate a fresh buffer from the pool
 *          HDNOALLOCATE - Do not allocate a buffer from the pool
 *
 * Return:  no value
 *
 * Abort:   none
 *
 * Notes:   This function allocates a buffer. It is assumed that
 *          someone downstream will release it
 *
 **************/
package void hdrxreset(struct hdRxCtlBk * hdRxpcbp, swi_bool doallocate )
{
    /* initialize read variables */
    hdRxpcbp->hdrdllstate   = WAIT_FOR_SYNC;
    hdRxpcbp->hdrerror      = NO;
    hdRxpcbp->hdrdatavalid  = NO;
    hdRxpcbp->hdrendframe   = NO;

    hdRxpcbp->hdrdatacount  = 0;

    /* Sometimes the state machine is reset without sending the
     * deframed packet. In this case, we can re-use the buffer
     * that was already allocated. The caller decides whether to
     * allocate or not
     */
    if( doallocate )
    {
        /* Allocate a buffer for the next receive operation */
        hdRxpcbp->hdrMemrelp = (swi_uint8 *) mmbufget(&hdRxpcbp->hdRxPool);

        /* Ensure we got the storage */
        if( hdRxpcbp->hdrMemrelp == NULL )
        {
            char errmsg[100];
            snprintf(errmsg, sizeof(errmsg),
                     "%s:%d no memory",
                     (char *)__func__, __LINE__);
            erAbort(errmsg, 0 );
        }

        /* Assign the location to start placing the deframed packet */
        hdRxpcbp->hdrHoldBufferp = (hdRxpcbp->hdrMemrelp+hdRxpcbp->hdOutoffset);
    }

    /* Always reset the current location pointer to the start of the buffer */
    hdRxpcbp->hdrHoldBufferEndp = hdRxpcbp->hdrHoldBufferp;
}

/*************
 *
 * Name:    hdrxdllstate - data link layer state machine
 *
 * Purpose: To drive data link layer state machine
 *
 * Parms:   rxchrp - pointer to character to be processed
 *
 * Return:  none
 *
 * Abort:   N/A
 *
 * Notes:   Flags are used in the control block
 *          rxdatavalid - data is valid
 *          rxendframe  - end of frame found
 *
 **************/
package swi_uint8 hdrxdllstate (swi_uint8 *rxchrp)
{
    struct hdRxCtlBk *hdRxpcbp = hdGetRxCtlBk();
    swi_uint8 rxbyte;

    /* always fetch a byte to process */
    rxbyte = *rxchrp;
    hdRxpcbp->hdrdatavalid = NO;
    hdRxpcbp->hdrendframe  = NO;
    switch(hdRxpcbp->hdrdllstate)
    {
       case WAIT_FOR_SYNC:
           /* We're looking for a flag ... */
           if (rxbyte == DLL_FLAG)
           {  /* and we got one, so go on to the next step. */
              hdRxpcbp->hdrdllstate = SYNCED_NO_DATA;
           }
           /* discard anything else */
           break;
       case SYNCED_NO_DATA:
           /* looking for real data */
           switch (rxbyte)
           {
               case DLL_FLAG: /* Another flag. No change. */
                   break;
                case DLL_CTRL_ESC:
                   hdRxpcbp->hdrdllstate = ESCAPED;
                   break;
               default:          /* we can begin a packet */
                   hdRxpcbp->hdrdllstate = COLLECT_DATA;
                   hdRxpcbp->hdrdatavalid = YES;
                   hdRxpcbp->hdrdatacount++;
                   break;
           }
           break;
       case COLLECT_DATA:     /* We're collecting a packet */
           switch (rxbyte)
           {
               case DLL_FLAG: /* End of frame encountered  */
                   hdRxpcbp->hdrdllstate = SYNCED_NO_DATA;
                   hdRxpcbp->hdrendframe = YES;
                   break;
               case DLL_CTRL_ESC:
                   hdRxpcbp->hdrdllstate = ESCAPED;
                   break;
               default:         /* got data, tell caller to use it */
                   hdRxpcbp->hdrdatavalid = YES;
                   hdRxpcbp->hdrdatacount++;
                   break;
           }
           break;
       case ESCAPED:
           switch (rxbyte)
           {
               case DLL_FLAG: /* ESCAPE/flag found, error, reset */
               case DLL_CTRL_ESC:
                   hdRxpcbp->hdrerror = YES;
                   hdRxpcbp->hdrdllstate = WAIT_FOR_SYNC;
                   break;
               default:      /* Otherwise, mask out the CTRL ESC to
                              * retrieve the original data.  */
                   rxbyte ^= DLL_ESC_MASK;
                   hdRxpcbp->hdrdllstate = COLLECT_DATA;
                   hdRxpcbp->hdrdatavalid = YES;
                   hdRxpcbp->hdrdatacount++;
                   break;
           }
           break;
       default:
           /* Shouldn't happen with an enum, start over */
           hdRxpcbp->hdrdllstate = WAIT_FOR_SYNC;
           hdRxpcbp->hdrdatacount = 0;
           break;
    }/* switch on state */
    return (rxbyte);
} /* rxchar */

/*
 * Name:    hdRxPkgInit
 *
 * Purpose: Initialize the Rx processing control block
 *
 * Params:  outoffset - The amount of space to skip from the front of the
 *                      receive buffer before starting to deframe a packet.
 *                      Stored permanently within the package control block.
 *
 * Return:  None
 *
 * Note:    None
 *
 * Abort:   n/a
 *
 */
package void hdRxInit ( swi_uint32 outoffset )
{
    struct hdRxCtlBk *hdRxpcbp = hdGetRxCtlBk();

    hdRxpcbp->hdOutoffset = outoffset;

    /* Receive memory pool is used to store inbound
     * packet data as it is being deframed. These
     * packets are forwarded up the stack, so no
     * more copying is required
     */
    mmpcreate( &hdRxpcbp->hdRxPool, "hdRxPool", HDRXPOOLSZ, HDRXBUFSIZE );

    /* Call the common Rx control block initialization routine */
    hdrxreset ( hdRxpcbp, HDALLOCATE );
}
