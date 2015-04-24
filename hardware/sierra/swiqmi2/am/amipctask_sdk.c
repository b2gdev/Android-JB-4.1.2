/*************
 *
 * Filename: amipctask_sdk.c
 *
 * Purpose:  AM package IPC Receive task and associated functions
 *
 * Copyright: © 2011-2012 Sierra Wireless Inc., all rights reserved
 *
 **************/

/* include files */
#include "ci/ciudefs.h"
#include "sdk/sdkudefs.h"
#include "su/suudefs.h"
#include "qm/qmerrno.h"
#include "am/amidefs.h"
#include <stdio.h>

/* Local storage */
struct amipccb amipcbk;

/* This next structure is a dummy event block which will
 * be sent to this AM task whenever a response is sent to the host.
 */
struct amevtblock amresponseevt = { AM_APP_RESP_SENT, NULL, 0, NULL, 0,
                                    {NULL,NULL,NULL} };


/*************
 *
 * Name: amgetipccbp
 *
 * Purpose: Function to get a pointer to the IPC receive task's
 *          control block structure.
 *
 * Parms:   none
 *
 * Return:  pointer to struct amipctcb
 *
 * Abort:   none
 *
 * Notes:   None
 *
 **************/
package struct amipccb *amgetipccbp( void )
{
    return &amipcbk;
}

/*************
 *
 * Name:    amqueipcrspevt
 *
 * Purpose: This function sends the event AM_APP_RESP_SENT to the AM task
 *
 * Parms:  ipcchannel  - ipcchannel that this response went out on
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   No pool is used. Sends a canned event.
 *
 **************/
local void amqueipcrspevt( swi_uint8 ipcchannel )
{
    struct amevtblock  *eventbkp;    /* event block pointer for AM task */

    /* Get pointer to the task control block */
    struct amipccb *amipccbp = amgetipccbp();

    /* Get canned event block to send event to AM task */
    eventbkp = &amresponseevt;
    eventbkp->amipcchannel = ipcchannel;

    /* Send the new event to the AM task for processing */
    icsmt_send( &amipccbp->amicmsgque, (swi_uint32) eventbkp );
}

/*************
 *
 * Name:    amipcrcvhandler
 *
 * Purpose: This function sends the event AM_APP_REQUEST to the AM task
 *
 * Parms:   msgp        - pointer to message being passed in
 *          datalen     - length of data passed in
 *          memfreep    - pointer to memory to be freed
 *          ipcchannel  - ipcchannel that this request came in from
 *
 * Return:  none
 *
 * Abort:   When AM is out of event blocks
 *
 * Notes:   This is a callback function whenever DS receives traffic.
 *          No pre-processing is needed on the incoming message, so we queue it
 *          to the AM task immediately.
 *          This runs in the context of the calling task, in this case, DS's
 *          shells.
 *
 **************/
local void amipcrcvhandler(
    swi_uint8   *msgp,
    swi_uint16  datalen,
    swi_uint8   *memfreep,
    swi_size_t  ipcchannel)
{
    struct amevtblock  *eventbkp;    /* event block pointer for AM task */

    /* Get pointer to the task control block */
    struct amipccb *amipccbp = amgetipccbp();

    /* Allocate event block to send event to AM task */
    eventbkp = (struct amevtblock *) mmbufget( &amipccbp->amevtpool );

    /* Don't want NULL event block pointers */
    if( eventbkp )
    {
        /* Fill the event with AM request message */
        eventbkp->amevent        = AM_APP_REQUEST;
        eventbkp->ammemfreep     = memfreep;
        eventbkp->amdatalen      = datalen;
        eventbkp->amdatap        = msgp;
        eventbkp->amipcchannel   = ipcchannel;
        qlinitlnk( &(eventbkp->ameqlnk), NULL );

        /* Send the new event to the AM task for processing */
        icsmt_send( &amipccbp->amicmsgque, (swi_uint32) eventbkp );
    }
    /* NULL Block */
    else
    {
        /* This should never happen */
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d Out of evt blk memory",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, 0 );
    }
}

/*************
 *
 * Name:    amsdk_send
 *
 * Purpose: Global function that sends a RR response/Notification to the host.
 *
 * Parms:   ipcbufp    - pointer to data which will be sent
 *          ipbuflen   - length of data to send
 *          ipcchannel - IPC channel
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   None
 *
 **************/
global void amsdk_send( swi_uint8 *ipcbufp,
                        swi_uint32 ipcbuflen,
                        swi_uint8 ipcchannel )
{
    enum   ciipcifusetyp       amifusetype;
    struct amipccb *amipccbp = amgetipccbp();

    /* Use dssend instead of icsdk_send since we are using the DS engine */
    enum qmisupportedclients client = QMIUNSUPPORTEDCLNT;
                                /* Local IPC socket dummy QMI client value */

    amifusetype = cigetifusetype( ipcchannel ) ;

    /* Only forward the following types */
    if ( (CIRR == amifusetype) || (CINOTIF == amifusetype) )
    {
        dssend( ipcbufp, ipcbuflen, ipcchannel, client );
    }
    else
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d Invalid channel type",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, amifusetype );
    }

    /* Queue 'response sent' only if Req/Resp type */
    if ( CIRR == amifusetype )
    {
        /* Send inter-task message to the response Q to flag that we're done */
        amqueipcrspevt(ipcchannel);
        dlLog0(  &amipccbp->amdlcb, AMLOG_CLASSA, "Response sent\n" );
    }
    else
    {
        dlLog0(  &amipccbp->amdlcb, AMLOG_CLASSA, "Notification sent\n" );
    }
}

/*************
 *
 * Name:    amreleventbk
 *
 * Purpose: To release the resources assocated with the AM event block.
 *
 * Parms:   eventbkp - Pointer to the AM event block structure
 *
 * Return:  None
 *
 * Abort:   None
 *
 * Notes:   This function releases the resources associated with the
 *          AM event block as follows: first the memory buffer
 *          containing the newly received AM packet is released.
 *          Next the event buffer itself is released. Both are
 *          allocated from pools created at system startup time
 *          by this AM task.
 *
 **************/
local void amreleventbk( struct amevtblock *eventbkp )
{
    /* Free the AM message buffer (if any) first */
    if ( eventbkp->ammemfreep != NULL )
    {
        mmbufrel( eventbkp->ammemfreep );
    }

    /* Finally, free the event block itself */
    mmbufrel( (swi_uint8 *)eventbkp );
 }

/*************
 *
 * Name: amShortResp
 *
 * Purpose: Build and send a short response to the API side. This
 *          function is used to send a reply to a request received
 *          from the API side when the request cannot be complied
 *          with in the normal manner
 *
 * Parms:   amtypercv   - AM packet type of the received request.
 *          resultcode  - An AM packet result code to be stuffed into
 *                        the returned packet
 *          ipcchannel   - The IPC channel which will receive the Short response
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void amShortResp(
    swi_uint16 amtypercv,
    enum eQCWWANError resultcode,
    swi_uint8 ipcchannel )
{
    swi_uint16 amtype;
    swi_uint8  outputbuf[AMHEADERSZ];
    swi_uint8  *outputbufp = &outputbuf[0];

    switch(amtypercv)
    {
        case AMTYPEQMIREQUEST:
            amtype = AMTYPEQMIRESPONSE;
            break;

        default:
            amtype = AMINVALID;
    }

    /* build an AM packet that consists only of a header */
    ambuildamheader(amtype, AMHDRVERSION, resultcode, 0, &outputbufp);

    /* Send this packet to the host */
    amsdk_send( &outputbuf[0], AMHEADERSZ, ipcchannel );
}

/*************
 *
 * Name: amdispatchrequest
 *
 * Purpose: Based on the received AM packet type the request is
 *          dispatched to the appropriate handler function.
 *
 * Parms:   amtype    - AM packet type
 *          inipcmsgp - Pointer to received AM request packet payload.
 *          memfreep  - Pointer to be used when freeing memory for this request
 *          ipcchannel- The IPC channel which message came from
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void amdispatchrequest(
    swi_uint16 amtype,
    swi_uint8 *inipcmsgp,
    swi_uint8 *memfreep,
    swi_uint8 ipcchannel)
{
    struct amipccb *amipccbp = amgetipccbp();

    switch( amtype )
    {
       case AMTYPEQMIREQUEST:
            /* dispatch this to the QMI package */
            qmsendrr( inipcmsgp,
                      memfreep,
                      ipcchannel );
            break;
       case AMTYPESDKREQUEST:
            /* dispatch this to the AM package */
            amsdkhandler( inipcmsgp,
                          memfreep,
                          ipcchannel );
            break;
        default:
            /* Log that we got an unrecognized message */
            dlLog(  &amipccbp->amdlcb, AMLOG_CLASSA,
                    "Rcv'd undefined AM type, %x\n",
                    (swi_uint32)amtype,
                    (swi_uint32)ipcchannel,
                    (swi_uint32)NULL,
                    (swi_uint32)NULL);
            break;
    }
}

/*************
 *
 * Name:    amstartipcs
 *
 * Purpose: Starts all instances of DS shells for AM IPC service for RRx
 *
 * Parms:   none
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   None
 *
 **************/
package void amstartipcs( )
{
    swi_uint16 numshells;
    swi_uint16 i;
    /* Define and allocate the task control block */
    struct amipccb *amipccbp = amgetipccbp();
    swi_uint8 *peernamep;
    swi_uint16 channel;

    /* Loop through the number of available RR/notification channel pairs */
    numshells = cigetnumRRchan();
    for (i = 0; i < numshells; i++)
    {
        /* Get the IPC Name and set callback function */
        amipccbp->amipcsh[i].dsipcshellnamep = cigetnextrrnamep (i);
        amipccbp->amipcsh[i].dsipccbfcnp = (void *)amipcrcvhandler;
        amipccbp->amipcsh[i].dssinitcbfp = NULL;
        /* Create a thread for that channel */
        swi_ossdkthreadcreate ( dslaunchshell, (void *)&amipccbp->amipcsh[i] );

        /* Open the corresponding IPC channels for Notifications
         * Note: notifications do not use DS shell */
        channel = cigetindex (amipccbp->amipcsh[i].dsipcshellnamep);
        peernamep = cigetpeernamep ( channel );
        ciinitsdkipcchan(peernamep);
    }
}

/*************
 *
 * Name: amipctask
 *
 * Purpose: Implements the main part of the IPC receive task. This task
 *          exists solely to block waiting for incoming IPC messages and
 *          then forward these messages to the appropriate recipient for
 *          further processing.
 *
 * Parms:   paramp - pointer to data that may be passed in when this task
 *                   is started.
 *
 * Return:  never returns
 *
 * Abort:   none
 *
 * Notes:   This task blocks waiting for inbound AM Packets on
 *          the multiple IPC channels from the APIs
 *
 **************/
local void amipctask( void *paramp )
{
    /* Define and allocate the task control block */
    struct amipccb *amipccbp = amgetipccbp();

    UNUSEDPARAM( paramp );

    /* Create the Event Pool for events received by AM task */
    mmpcreate( &amipccbp->amevtpool,
               "AMEVTPOOL",
               AMEVTPOOLSZ,
               sizeof(struct amevtblock) );

     /* Tell the world we've started up */
    dlLog0 ( &amipccbp->amdlcb, DLCLASSALL, "IPC Task started\n" );

    /* Initialize the SMT Requests message Q */
    icsmt_create( &amipccbp->amicmsgque, "AMMESSAGEQ", AMMSGDEPTH );

    /* Activate:
     *  - AM instances of RR channels for DS service
     *  - Notification channels
     */
    amstartipcs();

    /* Disable logging now that we've started up, can be re-enabled via apui */
    dlmasterenable( &amipccbp->amdlcb, FALSE );

    amipccbp->amreqbusy = FALSE;

    /* report that task has been created and is now running */
    sutaskinitcb(AMSDKPKG);

    /* Main AM task loop */
    swi_uint32 loop;
    for(loop=0;;loop++)
    {
        /* Wait to receive  a control message from  the Application
         * via DS IPC and AM receive handler
         */
        swi_uint32 reqmsg;
        icsmt_rcv( &amipccbp->amicmsgque, AMSMTRCVTOUT, &reqmsg );
        if(reqmsg)
        {
            struct amevtblock *eventbkp = (struct amevtblock *)reqmsg;
            struct qllink *qlp;
            switch( eventbkp->amevent )
            {
                case AM_APP_RESP_SENT:
                    /* Response from modem sent to application, flag to service
                     * next request
                     */
                    amipccbp->amreqbusy = FALSE;
                    dlLog1 ( &amipccbp->amdlcb, DLCLASSALL,
                             "Resp sent evt recvd ch:%d\n",
                             (swi_uint32)eventbkp->amipcchannel );
                        /*
                         * No need to delete event blk, this event block is
                         * a canned-message.
                        */
                    break;
                case AM_APP_REQUEST:      /* Request from the Application */
/* RILSTART */
                        /* Check for AMTYPESDKREQUEST/AMSDKREQKILL and handle
                         * immediately instead of queueing.  This will cause
                         * the SDK process to be exited.
                         */
                        {
                            /* Storage for incoming IPC packet buffer pointer */
                            swi_uint8 *inipcmsgp = eventbkp->amdatap;
                            
                            /* Parse received packet header */
                            swi_uint16 amlength, amtype, amhdrversion;
                            enum eQCWWANError amresultcode;
                            amparseamheader( &amtype,
                                             &amhdrversion,
                                             &amresultcode,
                                             &amlength,
                                             &inipcmsgp);

                            if ( amtype == AMTYPESDKREQUEST )
                            {
                                 /* If received handle the kill immediately, in
                                    which case, this function does not return
                                  */
                                 amsdkcheckandhandlekill( inipcmsgp );
                            }
                        }
/* RILSTOP */

                        /* Always put such requests on our local queue */
                        qlp = &eventbkp->ameqlnk;
                        qlinitlnk( qlp, (void *) eventbkp );

                        /* add link to end of queue */
                        qladdprev(&(amipccbp->ameventheadq), qlp);
                    break;
                default:
                    /* ignore unknown events */
                    dlLog1 ( &amipccbp->amdlcb, DLCLASSALL,
                             "Ignoring unknown event %x\n",
                             (swi_uint32)eventbkp->amevent );
                    break;
            }

            if( !amipccbp->amreqbusy ) /* no outstanding responses */
            {
                struct qllink *curlinkp = &(amipccbp->ameventheadq);
                struct amevtblock *qeventbkp = qlremnext(curlinkp);
                if (qeventbkp)
                {
                    /* Dispatch requests accordingly */
                    amipccbp->amreqbusy = TRUE;
                    /* Storage for incoming IPC packet buffer pointer */
                    swi_uint8 *inipcmsgp = qeventbkp->amdatap;

                    /* Parse received packet header */
                    swi_uint16 amlength, amtype, amhdrversion;
                    enum eQCWWANError amresultcode;
                    amparseamheader( &amtype,
                                     &amhdrversion,
                                     &amresultcode,
                                     &amlength,
                                     &inipcmsgp);

                    if( amhdrversion != AMHDRVERSION )
                    {
                        /* log an error and tell the API side of the error */
                        dlLog3 ( &amipccbp->amdlcb, AMLOG_CLASSA,
                                "[%d] AM header mismatch, API: %d, SDK %d\n",
                                (swi_uint32)loop,
                                (swi_uint32)amhdrversion,
                                (swi_uint32)AMHDRVERSION );

                        /* Send a packet back to the caller */
                        amShortResp( amtype,
                                     eQCWWAN_ERR_SWICM_AM_VERS_ERROR,
                                     qeventbkp->amipcchannel );
                    }
                    else
                    {
                        /* Dispatch received request to handler */
                        amdispatchrequest( amtype,
                                           inipcmsgp,
                                           NULL,
                                           qeventbkp->amipcchannel );
                    }
                    /* Free up incoming blocks */
                    amreleventbk (qeventbkp);
                }
                else
                {
                    dlLog0 (&amipccbp->amdlcb, DLCLASSALL,
                            "No more events\n" );
                }
             } /* not busy */
        } /* rmsg */
    } /* for */
}

/*************
 *
 * Name:    aminit
 *
 * Purpose: Initialize internal variables within the AM package
 *          at SDK startup
 *
 * Parms:   none
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
global void aminit( void )
{
    /* Define and allocate the task control block */
    struct amipccb *amipccbp = amgetipccbp();

    /* Initialize the head of local event queue */
    qlinitlnk(&(amipccbp->ameventheadq), NULL);

    /* Initialize logging facility */
    dlregister( "AM", &amipccbp->amdlcb, FALSE);
}

/*************
 *
 * Name: amipctaskinit
 *
 * Purpose: Creates the task by making it known to the os package.
 *
 * Parms:   none
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   Must be called *after* aminit() has been called
 *
 **************/
global void amipctaskinit( void )
{
    /* Create the AMIPC task */
    swi_ossdkthreadcreate( amipctask, NULL );
}

