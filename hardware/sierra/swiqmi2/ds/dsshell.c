/*************
 *
 * Filename: dsshell.c
 *
 * Purpose:  DS package shell task and associated functions
 *
 * Copyright: © 2011-2012 Sierra Wireless Inc., all rights reserved
 *
 **************/

/*-------------
  include files
 --------------*/
#include "ds/dsidefs.h"
#include "sl/sludefs.h"
#include "ci/ciudefs.h"
#include "mm/mmudefs.h"
#include <stdio.h>

/*-------
  Defines
 --------*/

/*-------------
  Local storage
 --------------*/

/* Array of task control blocks one for each channel */
local struct dsipccb dsshellcb[CIMAXIPCCHAN];
local struct dsuscb dsepcbk[CIMAXIPCCHAN];

/* DS event notification callback table */
#define DS_EVT_NOTIF_CLIENTS_MAX 10
local void(*dseventcallbacklist[DS_EVT_NOTIF_CLIENTS_MAX])(enum ds_sio_event ev) = {NULL};

/*---------
  Functions
 ----------*/

/*************
 *
 * Name:    dsgetepcbp
 *
 * Purpose: Function to get a pointer to the Endpoint
 *          control block structure.
 *
 * Parms:   index   - entry in the configuration structure
 *
 * Return:  pointer to struct dsuscb
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local struct dsuscb*
dsgetepcbp(
    swi_uint16  index )
{
    return &(dsepcbk[index]);
}

/*************
 *
 * Name:    dsgetipccbp
 *
 * Purpose: Function to get a pointer to  IPC channel control block structure.
 *
 * Parms:   chan - channel index
 *
 * Return:  pointer to struct dsipctcb
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local struct dsipccb*
dsgetipccbp(
    swi_uint16 chan )
{
    return &(dsshellcb[chan]);
}

/*************
 *
 * Name:    ds_trigger_device_notification_callbacks
 *
 * Purpose: Device notification callback execution for registered clients
 *
 * Parms:   ev  - SIO device event
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void
ds_trigger_device_notification_callbacks(
    enum ds_sio_event ev )
{
    int i;
    for( i = 0 ; i < DS_EVT_NOTIF_CLIENTS_MAX ; i++ )
    {
        if( dseventcallbacklist[i] != NULL )
        {
            dseventcallbacklist[i](ev);
        }
    }
}

/*************
 *
 * Name:    ds_usdev_notification_cb
 *
 * Purpose: Callback registered with USB device event notifications service
 *
 * Parms:   ev  - SIO device event
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void
ds_usdev_notification_cb(
    enum usb_device_event event)
{
    switch(event)
    {
        case US_BOOT_DEVICE_READY_EVT:
            /* notify registered DS clients */
            ds_trigger_device_notification_callbacks(DS_SIO_BOOT_READY);
            break;

        case US_APP_DEVICE_READY_EVT:
            /* notify registered DS clients */
            ds_trigger_device_notification_callbacks(DS_SIO_READY);
            break;

        case US_DEVICE_DISCONNECTED_EVT:
            /* notify registered DS clients */
            ds_trigger_device_notification_callbacks(DS_SIO_DISCONNECTED);
            break;


        default:
            /* use any valid channel id for log */
            dlLog1(  &dsepcbk[0].dsusdldcb,
                     DSLOG_CLASSC,
                     "Unhandled USB device event %d\n",
                     (swi_uint32)event );
            break;
    }
}

/*************
 *
 * Name:    ds_register_device_notification_callback
 *
 * Purpose: Device event notification registration API
 *
 * Parms:   ev  - SIO device event
 *
 * Return:  TRUE if successful, FALSE if registration list is full
 *
 * Abort:   none
 *
 * Notes:   DS clients use this API in order to register a callback
 *          function that will be executed when an event corresponding
 *          to the underlying SIO device occurs.
 *
 **************/
global swi_bool
ds_register_device_notification_callback(
    void(*pcb)(enum ds_sio_event ev) )
{
    int i;
    for( i = 0 ; i < DS_EVT_NOTIF_CLIENTS_MAX ; i++ )
    {
        if( dseventcallbacklist[i] == NULL )
        {
            dseventcallbacklist[i] = pcb;
            /* callback registered */
            return TRUE;
        }
    }

    /* registration list full */
    return FALSE;
}

/*************
 *
 * Name:    dsinit
 *
 * Purpose: Initialize DS control structures
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
global void dsinit(void)
{
    /* Initialize DS clients memory pools */
    int i;
    for( i = 0 ; i < cigetnumchannels() ; i++ )
    {
        swi_bool qmichan;
        qmichan = ( 0 == slstrncmp( (char *)cigetchannelname(i),
                    CIIPCUQMRR1,
                    sizeof(CIIPCUQMRR1) ) )
        ? TRUE
        : FALSE;

        if(qmichan)
        {
            mmpcreate( &(dsepcbk[i].dsbufpool), "dsQmiRxPool", DSQQMIRXPOOLSZ, DSEPBUFSZ );
        }
        else
        {
            mmpcreate( &(dsepcbk[i].dsbufpool), "dsRxPool", DSRXPOOLSZ, DSEPBUFSZ );
        }

        /* Register with the logger process */
        dlregister((char *)cigetchannelname(i) , &dsepcbk[i].dsusdldcb, TRUE);
        dsepcbk[i].dsrxpktsdropped = 0;
    }

    /* register for USB device notifications */
    if( !us_register_device_notification_callback(ds_usdev_notification_cb) )
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d Could not register for USB device events",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, 0);
    }
}

/*************
 *
 * Name:    dsshell_ipc
 *
 * Purpose: DS-Shell execution context for receiving data from a local IPC socket.
 *
 * Parms:   shnamep         - string with the device service name
 *          dsrcvcbckp(IN)  - callback on message receive function with the
 *                            following input parameters:
 *                            inipcmsgp(IN) - beginning of received message
 *                            inmsglen      - length of received message
 *                            memfreep      - memory to be released by callback
 *                            ipcchannel    - IPC channel used for this service
 *
 * Return:  never returns
 *
 * Abort:   none
 *
 * Notes:   This task blocks waiting for inbound packets from the specified
 *          IPC channel.
 *
 **************/
local void
dsshell_ipc(
    swi_uint8 *shnamep,
    void *dsrcvcbckp( swi_uint8 *inipcmsgp,
                      swi_uint32 inmsglen,
                      swi_uint8 *memfreep,
                      swi_uint16 ipcchannel ),
    void (*dssinitcbfp)(swi_bool) )
{
    swi_uint16 chanid;

    /* initialize the IPC Handle needed by this DS shell instance */
    chanid = cigetchannelnum((char *)shnamep);

    /* define and allocate the task control block */
    struct dsipccb *dsipccbp;
    dsipccbp = dsgetipccbp(chanid);

    /* register for logging */
    dlregister( (char *)shnamep, &dsipccbp->dsdlcb, FALSE );

    /* install receive handler */
    dsipccbp->dsrcvcbckp = (void *)dsrcvcbckp;

    /* set the IPC channel pointer */
    struct icipccb *ipccbp;
    ipccbp =  cigetipcp(chanid);

    /* create the IPC Pool used for exchanging data with the IPC layer */
    mmpcreate( &dsipccbp->dsbufpool,
               dsipccbp->dsdlcb.dlpkgnamep,
               DSIPCPOOLSZ,
               DSIPCBUFSZ );

    /* tell the world we've started up */
    dlLog0( &dsipccbp->dsdlcb,
            DSLOG_CLASSC,
            "IPC DS shell started\n" );

    swi_uint32 ipcbuflen;   /* received data length */
    swi_bool   success;     /* read status */
    swi_uint8  *inipcmsgp;  /* incoming IPC packet buffer pointer */

    /* inform client that DS shell has been created */
    if( NULL != dssinitcbfp )
    {
        dssinitcbfp(TRUE);
    }

    for(;;)
    {
        /* reset the buffer length */
        ipcbuflen = DSIPCBUFSZ;

        /* allocate a buffer to store the next incoming IPC messages */
        inipcmsgp = (swi_uint8 *) mmbufget( &dsipccbp->dsbufpool );

        if( inipcmsgp == NULL )
        {
            /* Abort the SDK - this should never happen */
            char errmsg[100];
            snprintf(errmsg, sizeof(errmsg),
                     "%s:%d No event blk memory",
                     (char *)__func__, __LINE__);
            erAbort(errmsg, 0 );
        }

        /* Wait for incoming requests from the application via the IPC channel */
        success = icsdk_rcv( ipccbp,
                             inipcmsgp,
                             &ipcbuflen,
                             DSIPCTIMEOUT );

        /* Invoke the installed receive handler  on succesful receive */
        if(success)
        {
            /* Note: The buffer is released by the callback */
            dsipccbp->dsrcvcbckp(inipcmsgp, ipcbuflen, inipcmsgp, chanid);
        }
        else
        {
            /* Release the buffer and return to the top */
            mmbufrel(inipcmsgp);
        }
    }
}

/*************
 *
 * Name:    dsshell_exit
 *
 * Purpose: Terminates the DS-Shell
 *
 * Parms:   pbuf2release - buffer to release
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local void dsshell_exit(swi_uint8 *pbuf2release)
{
    /* release the buffer the shell used to receive data from the
     * device.
     */
    mmbufrel(pbuf2release);

    /* notify clients registered with DS */
    ds_trigger_device_notification_callbacks(DS_SIO_DISCONNECTED);

    /* device no longer valid, terminate this shell */
    swi_ossdkthreadterminate();
}

/*************
 *
 * Name:    dsshell_ep
 *
 * Purpose: DS-Shell execution context for receiving data from a device endpoint.
 *
 * Parms:   shnamep(IN)     - shell name
 *          client          - For a QMI endpoint, the QMI client requesting a shell;
 *                            dummy value for any non-QMI endpoint.
 *          dsrcvcbckp(IN)  - callback function with the following input
 *                            parameters:
 *                            inipcmsgp(IN) - beginning of received message
 *                            inmsglen      - length of received message
 *                            memfreep(IN)  - memory to be released by callback
 *                            ipcchannel    - IPC channel used for this service
 *
 * Return:  never returns
 *
 * Abort:   none
 *
 * Notes:   This task blocks waiting for inbound packets from the specified
 *          endpoint. Upon invokation, the callback function must read the
 *          contents of the received packet within the execution context of
 *          the DS shell.
 *
 **************/
local void dsshell_ep(
    swi_uint8 *shnamep,
    enum qmisupportedclients client,
    void *dsrcvcbckp( swi_uint8 *inipcmsgp,
                      swi_uint32 inmsglen,
                      swi_uint8 *memfreep,
                      swi_uint16 ipcchannel),
    void (*dssinitcbfp)(swi_bool) )
{
    /* default to any tty device type endpoint */
    enum useptype epdev = USEP_DIAG;

    swi_uint16 chanid;
    chanid = cigetchannelnum((char *)shnamep);
    epdev = cigetifusetype( chanid );

    /* define and allocate the task control block */
    struct dsuscb *dsuscbp = dsgetepcbp(chanid);

    /* initialize this task's waiter structure */
    swi_ossdkwaiterinit( &dsuscbp->dsusdwaiter );

    /* get an input buffer from the pool */
    swi_uint8 *inbufp;
    inbufp = (swi_uint8 *)mmbufget( &dsuscbp->dsbufpool );

    /* should never run out of memory */
    if( !inbufp )
    {
        erAbort("dsshell_ep: no memory", 0 );
    }

    /* tell the world we've started up */
    dlLog0(  &dsuscbp->dsusdldcb,
            DSLOG_CLASSC,
            "Endpoint DS shell instance created\n" );

    struct usbep endpoint;  /* endpoint to receive data from */
    swi_size_t resplength;  /* receive buffer length */
    swi_bool success;       /* read status */
    swi_uint32 timeout = 300;   /* Periodically wake up */

    for(;;)
    {
        /* if the USB system is not alive yet */
        if( !usgetep( cigetifusetype( chanid ), &endpoint ) )
        {
            /* invoke our waiter */
            swi_ossdkwaiterwait( &dsuscbp->dsusdwaiter, DSRXSCANDELAY );

            /* restart from the top of the loop */
            continue;
        }

        /* reset this every time through the loop */
        resplength = DSEPBUFSZ;
        slmemset( (char *) inbufp, 0, DSEPBUFSZ );

        /* inform client that DS shell has been created */
        if( NULL != dssinitcbfp )
        {
            /* Ensure that there's a valid file descriptor associated
             * with the device file prior to calling dssinit callback.
             * Otherwise, if the invoked callback sends data to the
             * same device file, we run the risk of missing incoming data.
             */
            if( 0 > usopentty( epdev, client, &endpoint, RDMODE ) )
            {
                dssinitcbfp(FALSE); /* could not start DS shell */
                dsshell_exit(inbufp);
            }
            else
            {
                dssinitcbfp(TRUE); /* DS shell started successfully */
                dssinitcbfp = NULL;
            }

        }

        /* wait for incoming messages from the device */
        success = usrcvtty( epdev,
                            client,
                            endpoint.usreadep,
                            inbufp,
                            &resplength,
                            timeout );

        if(success)
        {
            /* if not a timeout */
            if( resplength != 0 )
            {
                /* The same buffer is reused for a subsequent read.
                 * Therefore, the contents must be read by the callback
                 * function within the context of this DS shell.
                 */
                dsrcvcbckp( inbufp, (swi_uint32)resplength, inbufp, client );
            }
        }
        else
        {
            dsshell_exit(inbufp);
        }
    } /* end for loop */
}

/*************
 *
 * Name:    dsshell
 *
 * Purpose: Invokes the correct DS shell creation function according to the type
 *          of channel the DS shell will be receiving data from.
 *
 * Parms:   shnamep(IN)     - shell name
 *          client          - For a QMI endpoint, the QMI client requesting a shell;
 *                            dummy value for any non-QMI endpoint.
 *          dsrcvcbckp(IN)  - callback function with the following input
 *                            parameters:
 *                            inipcmsgp(IN) - beginning of received message
 *                            inmsglen      - length of received message
 *                            memfreep(IN)  - memory to be released by callback
 *                            ipcchannel    - IPC channel used for this service
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
package void dsshell(
    swi_uint8 *shnamep,
    enum qmisupportedclients client,
    void *dsrcvcbckp( swi_uint8 *inipcmsgp,
                      swi_uint32 inmsglen,
                      swi_uint8 *memfreep,
                      swi_uint16 ipcchannel),
   void (*dsinitcbfp)(swi_bool) )
{
    swi_uint16 devicetype;

    /* determine the type of channel to create a DS shell for */
    ciinitsdkipcchan(shnamep);
    devicetype = cigetifacetype(shnamep);

    switch(devicetype)
    {
        case CIIPC:
            dsshell_ipc(shnamep, dsrcvcbckp, dsinitcbfp);
            break;
        case CIUIF:
            dsshell_ep(shnamep, client, dsrcvcbckp, dsinitcbfp);
            break;
        default:
            {
                char errmsg[100];

                snprintf(errmsg, sizeof(errmsg),
                         "%s:%d invalid device type",
                         (char *)__func__, __LINE__);
                erAbort(errmsg, 0 );
            }
            break;
    }
}

 /*************
 *
 * Name:    dslaunchshell
 *
 * Purpose: Creates an instance of a DS shell
 *
 * Parms:   shellparmp(IN) - parameters for starting the DS shell instance
 *
 * Return:  never returns
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
global void
dslaunchshell(
    void *shellparmp)
{
    struct dsipcshell *ipcshellp = (struct dsipcshell *)shellparmp;

    /* use any valid channel ID for log */
    struct dsuscb *dsuscbp = dsgetepcbp(cigetchannelnum(CIIPCUDIAG));
    dsuscbp = dsgetepcbp(cigetchannelnum(CIIPCUDIAG));

    dlLog0( &dsuscbp->dsusdldcb,
            DSLOG_CLASSC,
            "DS Shell launched\n" );

    /* Invoke instance of a DS shell */
    dsshell( ipcshellp->dsipcshellnamep,
             ipcshellp->client,
             (void *)ipcshellp->dsipccbfcnp,
             ipcshellp->dssinitcbfp );
}

/*************
 *
 * Name:    dsep_send
 *
 * Purpose: Sends a packet to the device via an endpoint
 *
 * Parms:   epid         - endpoint to send packet through
 *          packetp(IN)  - packet to send to the device
 *          paylength    - length of packet
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
package swi_bool dsep_send(
    enum useptype epid,
    enum qmisupportedclients client,
    swi_uint8 *packetp,
    swi_uint32 paylength)
{
    struct usbep endpoint;

    if( usgetep(epid, &endpoint) )
    {
        /* Send data to the endpoint*/
        return ussendtty( epid,
                          client,
                          endpoint.uswriteep,
                          packetp,
                          paylength );
    }

    /* use any valid channel id for log */
    dlLog2( &dsepcbk[0].dsusdldcb,
            DSLOG_CLASSC,
            "invalid endpoint: client %d, epid %d",
            (swi_uint32)client,
            (swi_uint32)epid );

    return FALSE;
}

/*************
 *
 * Name:    dsep_close
 *
 * Purpose: close endpoint file descriptors
 *
 * Parms:   epid    - endpoint
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
package void
dsep_close(
    enum useptype epid,
    enum qmisupportedclients client )
{
    struct usbep endpoint;

    if( usgetep(epid, &endpoint) )
    {
        usclosetty(epid, client, &endpoint);
        return;
    }

    /* use any valid channel id for log */
    dlLog2( &dsepcbk[0].dsusdldcb,
            DSLOG_CLASSC,
            "invalid endpoint: client %d, epid %d",
            (swi_uint32)client,
            (swi_uint32)epid );
}

/*************
 *
 * Name:    dsep_ioctl
 *
 * Purpose: Send an ioctl command to the modem through an endpoint
 *
 * Parms:   epid         - Endpoint to send packet through
 *          ioctlcmd     - ioctl command
 *          paramp(IN)   - ioctl parameter
 *          parmlen      - ioctl parameter length
 *
 * Return:  TRUE on success, FALSE otherwise
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
package swi_bool
dsep_ioctl(
    enum useptype epid,
    enum qmisupportedclients client,
    swi_uint16    ioctlcmd,
    void          *paramp,
    size_t        parmlen )
{
    struct usbep endpoint;

    if( usgetep(epid, &endpoint) )
    {
       return usioctltty( epid,
                          client,
                          endpoint,
                          ioctlcmd,
                          paramp,
                          parmlen );
    }

    /* use any valid channel id for log */
    dlLog2( &dsepcbk[0].dsusdldcb,
            DSLOG_CLASSC,
            "invalid endpoint: client %d, epid %d",
            (swi_uint32)client,
            (swi_uint32)epid );

    return FALSE; /* rid of compiler warning */
}

