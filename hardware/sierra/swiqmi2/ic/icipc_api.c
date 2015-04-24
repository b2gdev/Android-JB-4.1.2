/*************
 *
 * Filename:    icipc_app.c
 *
 * Purpose:     The IC package provides facilities for tasks to 
 *              communicate with one another. For a full description
 *              of this package, refer to the file ic.txt. 
 *              
 *              There may be multiple user-mode processes needing to
 *              exchange information with the SDK. The contents of this 
 *              file provides them with a means of sending/receiving 
 *              information to/from the SDK. This file's peer is
 *              icipc_sdk.c
 *                
 * Copyright:   Â© 2008 Sierra Wireless, Inc., all rights reserved
 *
 **************/

/* include files */
#include "aa/aaglobal.h"
#include "os/swi_osapi.h"

/* Package Includes */
#include "ic/icidefs.h"

/* Defines */


/*************
 *
 * Name: icapi_rcv
 *
 * Purpose: Allows callers in the API-side to wait for an 
 *          incoming IPC message
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
 *          When swi_osapiipcread() returns TRUE with a buffer length 
 *          of ZERO, it indicates that the operation has timed out.
 *
 **************/
global swi_bool icapi_rcv( struct icipccb *ipccbp, 
                           swi_uint8 *ipcbufp, 
                           swi_uint32 *ipcbuflenp,
                           swi_uint32 timeout  )
{
    swi_bool result;

    /* Pass the call along to the os layer and 
     * block until the timeout or a message is Rx'd
     */
    result = swi_osapiipcread( (struct swi_osipcapi *)&ipccbp->icipcip, 
                               ipcbufp, 
                               (size_t *)ipcbuflenp, 
                               timeout );
    return result;
}

/*************
 *
 * Name: icapi_send
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
global swi_bool icapi_send( struct icipccb *ipccbp, 
                            swi_uint8 *ipcbufp, 
                            swi_uint32 ipcbuflen  )
{
    swi_bool result;

    /* Pass the call along to the os layer */
    result = swi_osapiipcwrite( (struct swi_osipcapi *)&ipccbp->icipcip,
                                 ipcbufp, ipcbuflen );

    /* See what happened */
    if( !result )
    {
        /* Log the error and continue */
    }

    return result;
}
/*************
 *
 * Name:     icinitapihndl
 *
 * Purpose:  Initializes the specified IPC channel for the caller
 *           
 * Parms:    ipccbp   - Pointer to the caller-supplied IPC control 
 *                      block that will be used whenever this channel
 *                      is accessed
 *           ipcindex - The IPC channel to open
 *           ipcnamep - The IPC channel name
 *
 * Return:   TRUE     - Channel opened successfully
 *           FALSE    - Channel already opened by another process
 *
 * Abort:    None
 *
 * Notes:    None
 *
 **************/
package swi_bool icinitapihndl( struct icipccb *ipccbp, 
    swi_size_t ipcindex,
    swi_uint8 *ipcnamep)
{
    /* Save the index */
    ipccbp->icipcindex = ipcindex;

    /* Call the OS layer to initialize the IPC handle */
    return swi_osapiipcopen( (struct swi_osipcapi *)&ipccbp->icipcip, ipcindex,
                              ipcnamep);
}
