/*************
 *
 * Filename:    icipc_sdk.c
 *
 * Purpose:     The IC package provides facilities for shared-memory 
 *              tasks to communicate with one another. For a full 
 *              description of this package, refer to the file ic.txt. 
 *              
 *              The SDK user-mode task may have one or more user-mode
 *              applications that it needs to exchange information with. 
 *              The contents of this file provide the SDK task with a 
 *              means of receiving information to/from user-mode apps.
 *              This file's peer is icipc_app.c
 *                
 * Copyright:   © 2008 Sierra Wireless, Inc., all rights reserved
 *
 **************/

/* include files */
#include <stdio.h>
#include <errno.h>

#include "aa/aaglobal.h"
#include "dl/dludefs.h"
#include "ic/icidefs.h"

/* Defines */

/* Logging masks */
#define ICLOG_CLASSA  0x0001


/* Local data */
local struct dlusercb icdlcb;

/*************
 *
 * Name:    icsdk_peeravail
 *
 * Purpose: To check if the API side channel exists.
 * 
 * Parms:   ipccbp - pointer to IPC user control block
 *
 * Return:  TRUE  - API side is accessible
 *          FALSE - API side is unavailable
 *
 * Abort:   none
 *
 * Note:    Calls os package to attempt a connect() call to the specified 
 *          user channel.
 *
 **************/
local swi_bool icsdk_peeravail(struct icipccb *ipccbp)
{
    swi_uint16 errcode = 0;
    swi_bool rstat = FALSE; /* Assume not OK */
           
    /* check the channel requested */
    rstat = swi_ossdkpeeravail(&ipccbp->icipcip, &errcode);
   
    /* API side is not there, update dropped sdk to api counter and log
     * occasionally, so user knows it is happening
     */
    if (rstat == FALSE)
    {
        dlLog(  &icdlcb, ICLOG_CLASSA, 
                "IPC channel %d, API peer absent, %d", 
                (swi_uint32)ipccbp->icipcindex,
                (swi_uint32)errcode,
                (swi_uint32)NULL,
                (swi_uint32)NULL); 
         
        ipccbp->icipcsdktoapidropped++;
        if( ipccbp->icipcsdktoapidropped%20 == 0 )
        {  
            dlLog(  &icdlcb, ICLOG_CLASSA, 
                   "IPC: %d SDK to API messages dropped, API chan %d absent\n", 
                   (swi_uint32)ipccbp->icipcsdktoapidropped,
                   (swi_uint32)ipccbp->icipcindex,
                   (swi_uint32)NULL,
                   (swi_uint32)NULL);
        }
    }
    return rstat;
}

/*************
 *
 * Name: icsdk_rcv
 *
 * Purpose: Allows callers to wait for an incoming IPC message
 *             
 * Parms:   ipccbp     - Pointer to a previously-initialized IPC 
 *                       control block
 *          ipcbufp    - (IN/OUT) Pointer to a caller-supplied 
 *                       buffer into which an incoming IPC message 
 *                       is placed
 *          ipcbuflenp - (IN)  Storage available in the incoming 
 *                             buffer
 *                       (OUT) Length, in bytes, of the incoming
 *                             message
 *          timeout    - Length of time (ms) to wait for the receive
 *                       to complete. 0 to wait forever
 *
 * Return:  TRUE  - message received and stored into buffer space provided
 *          FALSE - Timeout occurred. No change to message storage space
 *
 * Abort:   none
 *
 * Notes:   The IPC channel must have been previously opened before 
 *          using this call.
 *
 **************/
global swi_bool icsdk_rcv( struct icipccb *ipccbp, 
                           swi_uint8 *ipcbufp, 
                           swi_uint32 *ipcbuflenp,
                           swi_uint32 timeout  )
{
    swi_bool result;

    /* Make sure this IPC has been previously opened */
    if( !ipccbp->icipcopen )
        erAbort("ICIPCRCV: Channel not open", ipccbp->icipcindex );
    
    /* Pass the call along to the os layer and 
     * block until the timeout or a message is Rx'd
     */
    result = swi_ossdkipcread( &ipccbp->icipcip, 
                               ipcbufp, 
                               ipcbuflenp, 
                               timeout );
    return result;
}

/*************
 *
 * Name: icsdk_send
 *
 * Purpose: Allows callers to send a message over their IPC channel
 *             
 * Parms:   ipccbp     - Pointer to a previously-initialized IPC 
 *                       control block
 *          ipcbufp    - (IN/OUT) Pointer to a caller-supplied 
 *                       buffer containing the message to be 
 *                       transmitted 
 *          ipcbuflenp - (IN)  Size of data (in bytes) in the 
 *                             buffer
 *
 * Return:  TRUE  - message sent successfully
 *          FALSE - Channel has disappeared - application side is gone. 
 *                  Discard the message in this case
 *
 * Abort:   none
 *
 * Notes:   The IPC channel must have been previously opened before 
 *          using this call.
 *
 **************/
global swi_bool icsdk_send( struct icipccb *ipccbp, 
                            swi_uint8 *ipcbufp, 
                            swi_uint32 ipcbuflen  )
{
    swi_bool result;

    /* Make sure this IPC has been previously opened */
    if( !ipccbp->icipcopen )
        erAbort("ICIPCSND: Channel not open", ipccbp->icipcindex );

    /* If API sides (Notifications, DIAG or NMEA) are not available */
    if (!icsdk_peeravail( ipccbp ) )
    {
        /* dlLog is called in above function */
        return FALSE;    
    }
 
    /* Pass the call along to the os layer */
    result = swi_ossdkipcwrite( &ipccbp->icipcip, ipcbufp, ipcbuflen );

    /* See what happened */
    if( !result )
    {
        /* Log the error and continue */
    }

    return result;
}

/*************
 *
 * Name: icinitsdkhndl
 *
 * Purpose: Initialize a user's IC IPC handle. This must be done before
 *          the caller can use the IPC channel. A by-product of this 
 *          call is that the IPC channel is opened.
 *             
 * Parms:   ipccbp   - Pointer to user-supplied memory which is the IPC
 *                     handle structure to be initialized
 *          ipcindex - The IPC channel number to be initialized
 *          chnamep  Pointer to the IPC channel name
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Note:    The memory occupied by the handle must be caller-supplied
 *          and callers must always provide a pointer to it whenever 
 *          they make use of this facility
 *
 **************/
global void icinitsdkhndl( struct icipccb *ipccbp, 
                           swi_size_t ipcindex,
                           swi_uint8 * chnamep)
{
    /* Initialize the ipc channel first */
    swi_ossdkipcopen( &ipccbp->icipcip, ipcindex, chnamep );

    dlLog(  &icdlcb, ICLOG_CLASSA, "IPC Opened, index %d, %s\n", 
            (swi_uint32)ipcindex,
            (swi_uint32)chnamep,
            (swi_uint32)NULL,
            (swi_uint32)NULL);

    /* Save the caller's index */
    ipccbp->icipcindex = ipcindex;

    /* Initialize dropped sdk to api messages counter */
    ipccbp->icipcsdktoapidropped = 0;

    /* The channel is now initialized */
    ipccbp->icipcopen = TRUE;
}

/*************
 *
 * Name: icipcinit
 *
 * Purpose: Package entry point to initialize the IPC part of the package
 *          on the SDK side.
 *             
 * Parms:   none
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Note:    Must be called after multitasking has started. Initializes
 *          the IC package's logging facility
 *
 **************/
global void icinitipc( void )
{
    /* Register for logging */
    dlregister( "IC", &icdlcb, FALSE ); 

}
