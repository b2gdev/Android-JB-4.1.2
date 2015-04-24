/*
 *  Filename: hduser.c
 *
 *  Purpose:  This file contains the interface routines routines
 *            for the Data Link Layer package
 *
 * Copyright: © 2008 Sierra Wireless, Inc., all rights reserved
 *
 */

#include <stdio.h>

/* include files */
#include "aa/aaglobal.h"
#include "mm/mmudefs.h"
#include "ql/qludefs.h"
#include "hd/hdidefs.h"
#include <stdio.h>

/**************************************************************************
 *
 * Name:     hdTxPacket
 *
 * Purpose:  Accept a packet destined for the modem, add framing characters
 *           and send it on its way
 *
 * Parms:    packetp   - pointer to a buffer containing the complete
 *                       message
 *           packetsz  - size of data in bytes
 *           sendcfp   - NULL, or pointer to function to call after
 *                       the message has been sent
 *           userp     - Arbitrary 32 bit value - will be passed as a
 *                       void* to sendcfp() when called
 * Return:   TRUE   -   send successful
 *           FALSE  - send failed
 *
 * Abort:    No send control blocks available
 *
 * Notes:    The prototype for the callback function is as follows:
 *
 *              void (*sendcfp)(void *userp, swi_uint8 *bufp, swi_bool errorflg)
 *
 *           Where:
 *             - userp is an arbitrary value of pointer size. The sendcfp()
 *               function is responsible for any validation required.
 *             - bufp is a pointer to the packetp argument passed
 *               to this function
 *             - errorflag indicates the success or failure of the
 *               send operation. FALSE if sent with no errors,
 *               TRUE if there were errors
 *
 **************/
global swi_bool hdTxPacket( swi_uint8 *packetp,
                        swi_uint16 packetsz,
                        void (*sendcfp)(void *userp,
                                        swi_uint8 *bufp,
                                        swi_bool errorflg),
                        void *userp,
                        void (*usersendfp) ( swi_uint8 *txbufp,
                                             swi_uint32 pktsize),
                        swi_uint16 fcs)
{
    struct hdTxCtlBk *hdTxpcbp = hdGetTxCtlBk();
    struct hdreq *hdreqp;

    /* get a request block from the pool */
    hdreqp = (struct hdreq *) mmbufget( &hdTxpcbp->hdtreqpool );

    if (!hdreqp)
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d No Tx buffer",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, 0);
    }

    /* save callback info into request block */

    /* Store the callback parameters */
    hdreqp->hdsendcfp = sendcfp;
    hdreqp->hduserp = userp;
    hdreqp->hdusersendfp = usersendfp;
    hdreqp->hdfcs = fcs;

    /* Store the package information */
    hdreqp->hdpacketp = packetp;
    hdreqp->hdpacketsz = packetsz;

    /* Send Data Link Layer message to Modem */
    return (hdsend (hdreqp));
}

/*************
 *
 * Name:    hdRxPacket
 *
 * Purpose: Main entry point for receiving new packets of data. These
 *          incoming packets drive the state machine
 *
 * Parms:   datap        - pointer to data received
 *          length       - Length of data received
 *
 * Return:  None
 *
 * Abort:   None
 *
 * Notes:   Once this call returns, the incoming datap storage may be
 *          released
 *
 **************/
global void hdRxPacket(swi_uint8 *datap,
                       swi_uint16 length,
                       void (*userrxfp) ( swi_uint8 *memrelp,
                                          swi_uint8 *ufbufp,
                                          swi_uint16 pktsize,
                                          swi_bool   errorflag )
)
{
    struct hdRxCtlBk *hdRxpcbp = hdGetRxCtlBk();
    swi_uint8 rxbyte;
    swi_uint16 rxlen;
    swi_uint16 packetlen;
    swi_uint8 *inbufp;
    swi_bool errorflg;

    errorflg = NO;
    rxlen = length;
    inbufp = datap;

    /* loop while Rx buffer is not empty */
    while (rxlen--)
    {   /* get a byte and run it through the Data Link Layer state machine */
        rxbyte = hdrxdllstate(inbufp++);
        if (hdRxpcbp->hdrdatavalid)
        {
            /* data is extracted, store it away */
            *hdRxpcbp->hdrHoldBufferEndp++ = rxbyte;
            if ((hdRxpcbp->hdrHoldBufferEndp - hdRxpcbp->hdrHoldBufferp) > HDRXBUFSIZE)
            {
                /* truncate message and set error flag */
                hdRxpcbp->hdrendframe = YES;
                errorflg = YES;
            }
        }
        if (hdRxpcbp->hdrendframe)
        {    /* closing flag encountered, check for dummy packet */
            if (hdRxpcbp->hdrdatacount == 0)
            {    /* Discard this frame. In this case we don't deliver
                  * anything, so there is no need to allocate another
                  * receive buffer. Just reset the existing pointers
                  */
                 hdrxreset (hdRxpcbp, HDNOALLOCATE);
                 continue; /* check if more frames available in Rx buffer */
            }
            else
            {
                packetlen = hdRxpcbp->hdrHoldBufferEndp-hdRxpcbp->hdrHoldBufferp;
                /* Send the deframed incoming buffer back to the caller */
                userrxfp ( hdRxpcbp->hdrMemrelp,
                           hdRxpcbp->hdrHoldBufferp,
                           packetlen,
                           errorflg );

                /* message sent, reset variables for next frame,
                 * request another buffer to be allocated from
                 * HD Rx pool
                 */
                hdrxreset (hdRxpcbp, HDALLOCATE);
            }
        }
    } /* while Rx buffer not done */
}
/*
 *  hdinit
 *
 *  Purpose: HD Package initialization routine. Called once during system
 *           startup.
 *
 *  Params:  outoffset - The amount of space from the front of the Rx
 *                       buffer to skip before starting to deframe a
 *                       HIP packet
 *
 *  Returns: None
 *
 *  Abort:   None
 *
 *  Note:    None
 *
 */
global void hdinit( swi_uint32 outoffset )
{
    /* Call the internal package initialization routines */
    hdTxInit();
    hdRxInit(outoffset);
}
