/*
 * Filename: usscan_sdk.c
 *
 * Purpose:  USB Serial Interface package
 *
 * Copyright: © 2011-2012 Sierra Wireless Inc., all rights reserved
 *
 * Notes: none
 */

/*-------------
  include files
 --------------*/

#include "aa/aaglobal.h"
#include "sdk/sdkudefs.h"
#include "su/suudefs.h"
#include "us/usidefs.h"

/*-------
  Defines
 --------*/
#define US_EVT_NOTIF_CLIENTS_MAX 10 /* maximum number of clients that can
                                     * register for device mode change
                                     * notifications.
                                     */
/*-------------
  Local storage
 --------------*/
/* Array of clients registered to receive USB device state change notifications */
local void(*useventcallbacklist[US_EVT_NOTIF_CLIENTS_MAX])(enum usb_device_event ev);

/* USB scan control block */
local struct usscb usscbk;

/*
 * Name:    usgetscbkp
 *
 * Purpose: Returns a pointer to the US task's control block structure
 *
 * Param:   None
 *
 * Returns: Pointer to usscbk
 *
 * Abort:   None
 *
 * Notes:   none
 */
package struct usscb *usgetscbkp(void)
{
    return &usscbk;
}

/*
 * Name:    usgetepname
 *
 * Purpose: Return name of the connected device given the endpoint number
 *
 * Param:   endpoint  - endpoint we are interested in (read or write)
 *          namep     - pointer to storage for endpoint name
 *          namlen    - maximum length of storage for name
 *          inout     - direction, TRUE = IN , FALSE = OUT
 *
 * Returns: TRUE if get name successful, FALSE otherwise
 *
 * Abort:   None
 *
 * Notes:   None
 *
 */
global swi_bool usgetepname( swi_uint32 endpoint,
                             swi_uint8  *namep,
                             swi_uint32 namelen,
                             swi_bool   inout )
{
    struct usscb *usscbkp;  /* US package control block pointer */

    /* Get a pointer to the US package control block */
    usscbkp = usgetscbkp();

    /* Get the name, given the direction */
    if (!swi_ossdkusbendpoint( &(usscbkp->uscusbctl),
                               endpoint,
                               inout,
                               (char *)namep,
                               namelen) )
    {
        return FALSE;
    }

    return TRUE;
}

/*
 * Name:    usgetinfo
 *
 * Purpose: Return device Vendor ID and Product ID
 *
 * Param:   vendidp   - Pointer to storage for the vendor ID
 *          prodidp   - Pointer to storage for the product ID
 *
 * Returns: TRUE if product exists, FALSE otherwise
 *
 * Abort:   None
 *
 * Notes:   None
 *
 */
global swi_bool usvidpid( swi_uint32 *vendidp,
                          swi_uint32 *prodidp )
{
    struct usscb *usscbkp;  /* US package control block pointer */

    /* Get a pointer to the US package control block */
    usscbkp = usgetscbkp();

    /* Get the information */
    return swi_ossdkusbproduct( &usscbkp->uscusbctl, vendidp, prodidp );
}

/*
 * Name:     usgetnuminterfaces
 *
 * Purpose:  Return the number of USB interfaces for the connected device
 *
 * Param:    pnumif   - Pointer to storage for the number of USB interfaces
 *
 * Returns:  TRUE if product exists, FALSE otherwise
 *
 * Abort:    None
 *
 * Notes:    None
 *
 */
global swi_bool usgetnuminterfaces(swi_uint32 *pnumif)
{
    struct usscb *usscbkp = usgetscbkp();
    *pnumif = swi_ossdkusbnuminterfaces(&usscbkp->uscusbctl);
    return 0 != *pnumif;
}

/*
 * Name:    usrcvtty
 *
 * Purpose: Receive some data from a specified endpoint
 *
 * Param:   endpoint - The endpoint to receive from
 *          epdev    - device type endpoint (i.e. tty or qcqmi)
 *          datap    - Pointer to a buffer to store the received data
 *          dlengthp - Pointer to the number of bytes available in datap
 *                     On return, contains the number of bytes read or 0
 *                     if timed out
 *          timeout  - The length of time (ms) to wait for a
 *                     response. 0 means wait forever
 *
 * Returns: TRUE  - Data received
 *          FALSE - The device has disappeared (cable unplugged, modem
 *                  reset, etc)
 *
 * Abort:   None
 *
 * Notes:   This function runs in a different thread from the main
 *          US scan task.
 *
 */
global swi_bool usrcvtty( enum useptype epdev,
                          enum qmisupportedclients client,
                          swi_uint16  endpoint,
                          swi_uint8   *datap,
                          swi_size_t  *dlengthp,
                          swi_uint32  timeout )
{
    swi_bool retval;          /* Returned by this function */
    swi_size_t readlength;
    struct usscb *usscbkp;  /* US package control block pointer */

    /* Get a pointer to the US package control block */
    usscbkp = usgetscbkp();

    /* Make sure the caller isn't using a bad endpoint */
    if( endpoint == USEPINVALID )
        return FALSE;

    /* Initialize the return value to show a successful read */
    retval = TRUE;

    /* Need to reset this every time through the loop */
    readlength = *dlengthp;

    dlLog2( &usscbkp->usdlscb, USLOG_CLASSB,
            "reading ep: %d, timeout %d\n",
            (swi_uint32)endpoint,
            (swi_uint32)timeout );

    /* Read the data from the endpoint */
    retval = swi_ossdkusbread( &usscbkp->uscusbctl,
                               epdev,
                               client,
                               endpoint,
                               datap,
                               &readlength,
                               timeout );

    /* Update the caller's length */
    *dlengthp = readlength;

    return retval;
}

/*
 * Name:     usioctltty
 *
 * Purpose:  Issue an ioctl command to a special (char device) file
 *
 * Param:    endpoint    - The destination endpoint
 *           ioctlcmd    - ioctl command to issue
 *           pparm       - ioctl command parameter pointer
 *           parmlen     - ioctl command parameter length
 *
 * Returns:  TRUE if succeeded,
 *           FALSE otherwise
 *
 * Abort:    None
 *
 * Notes:    This function still requires testing before it is to be used
 *
 *           If the modem disappears the caller must take steps to
 *           determine when it has returned and what endpoint to
 *           use for the type of traffic desired.
 *
 *           This function runs in a different thread from the main
 *           US scan task.
 *
 */
global swi_bool usioctltty( enum useptype epdev,
                            enum qmisupportedclients client,
                            struct usbep endpoint,
                            swi_uint16   ioctlcmd,
                            void         *pparm,
                            size_t       parmlen )
{
    swi_bool rv;
    struct usscb *usscbkp;

    UNUSEDPARAM( epdev );
    UNUSEDPARAM( parmlen );

    /* Get a pointer to the US package control block */
    usscbkp = usgetscbkp();

    /* Make sure the caller isn't using a bad endpoint */
    if( endpoint.uswriteep == USEPINVALID ||
        endpoint.usreadep == USEPINVALID )
    {
        return FALSE;
    }

    dlLog3(  &usscbkp->usdlscb, USLOG_CLASSC,
            "IOCTL writeep/readep/ioctl: %d/%d/0x%06x\n",
            (swi_uint32)endpoint.uswriteep,
            (swi_uint32)endpoint.usreadep,
            (swi_uint32)ioctlcmd );

    /* Issue the ioctl to the endpoint */
    rv = swi_ossdkusbioctl( &usscbkp->uscusbctl,
                            client,
                            endpoint,
                            ioctlcmd,
                            pparm );

    return rv;
}

/*
 * Name:    ussendtty
 *
 * Purpose: Send some data to a tty. The caller specifies the
 *          endpoint and this entry determines which tty to use
 *          and sends the traffic to it.
 *
 * Param:   endpoint - The destination endpoint
 *          epdev    - device type endpoint (i.e. tty or qcqmi)
 *          datap    - Pointer to a buffer of data to send
 *          dlength  - The number of bytes pointed-to by datap
 *
 * Returns: TRUE if write succeeded,
 *          FALSE if the modem has disappeared (crashed, unplugged
 *          reset, etc.)
 *
 * Abort:   None
 *
 * Notes:   If the modem disappears the caller must take steps to
 *          determine when it has returned and what endpoint to
 *          use for the type of traffic desired.
 *
 *          This function runs in a different thread from the main
 *          US scan task.
 *
 */
global swi_bool ussendtty(  enum useptype epdev,
                            enum qmisupportedclients client,
                            swi_uint16  endpoint,
                            swi_uint8   *datap,
                            swi_size_t  dlength )
{
    swi_bool retval;
    struct usscb *usscbkp;

    /* Get a pointer to the US package control block */
    usscbkp = usgetscbkp();

    /* Make sure the caller isn't using a bad endpoint */
    if( endpoint == USEPINVALID )
        return FALSE;

    /* Log the write, if enabled */
    dlLog1( &usscbkp->usdlscb, USLOG_CLASSC,
            "writing ep: %d\n",
            (swi_uint32)endpoint );

    /* Send the data to the endpoint. Watch the return
     * code to determine if there's been a problem
     */
    retval = swi_ossdkusbwrite( &usscbkp->uscusbctl,
                                epdev,
                                client,
                                endpoint,
                                datap,
                                dlength );

    return retval;
}

/*
 * Name:    usopentty
 *
 * Purpose: open the tty associated with the specified endpoint
 *
 * Param:   epdev   - device associated with the endpoint e.g. tty, qcqmi
 *          client  - Relevant for a QMI endpoint - QMI client to be associated
 *                    with the file descriptor.
 *          ep      - Pointer to a US package endpoint type containing
 *                    valid numbers for the read and write endpoint
 *          mode    - open the file in the given mode (refer to filemode_t)
 *
 * Returns: SUCCESS - file descriptor
 *          FAILURE - -1
 *
 * Abort:   None
 *
 * Notes:
 *
 */
global swi_int32 usopentty( enum useptype epdev,
                            enum qmisupportedclients client,
                            struct usbep *ep,
                            int mode )
{
    struct usscb *usscbkp;

    /* Get a pointer to the US package control block */
    usscbkp = usgetscbkp();

    if( RDMODE ==  mode )
    {
        /* open the specified read endpoint */
        return swi_ossdkusbopen( &usscbkp->uscusbctl,
                                 epdev,
                                 client,
                                 ep->usreadep,
                                 mode );
    }

    else if( WRMODE ==  mode )
    {
        /* open the specified write endpoint */
        return swi_ossdkusbopen( &usscbkp->uscusbctl,
                                 epdev,
                                 client,
                                 ep->uswriteep,
                                 mode );
    }

    return -1;
}

/*
 * Name:    usclosetty
 *
 * Purpose: Close the tty associated with the specified endpoint
 *
 * Param:   ep      - Pointer to a US package endpoint type containing
 *                    valid numbers for the read and write endpoint
 *
 *          client  - Relevant for a QMI endpoint - QMI client to be associated
 *                    with the file descriptor.
 *
 * Returns: None
 *
 * Abort:   None
 *
 * Notes:   The endpoints may or may not be open when this function
 *          is called.
 *
 *          Package use only for now
 *
 */
package void usclosetty( enum useptype epdev,
                         enum qmisupportedclients client,
                         struct usbep *ep )
{
    struct usscb *usscbkp;

    /* Get a pointer to the US package control block */
    usscbkp = usgetscbkp();

    /* Close the specified write endpoint */
    swi_ossdkusbclose( &usscbkp->uscusbctl,
                       epdev,
                       client,
                       ep->uswriteep,
                       FALSE );

    /* Close the specified read endpoint */
    swi_ossdkusbclose( &usscbkp->uscusbctl,
                       epdev,
                       client,
                       ep->usreadep,
                       TRUE );
}

/*
 * Name:    usifacetoep
 *
 * Purpose: Find usb endpoint number based on direction & usb interface number
 *
 * Param:   iface - usb interface number
 *          dir   - endpoint direction
 *
 * Returns: endpoint number or USEPINVALID
 *
 * Abort:   None
 *
 * Notes:   None
 *
 */
package int usifacetoep( swi_uint32 iface,
                         enum swi_endpointdirection dir )
{
    int epnum;

    /* Get the information */
    epnum = swi_ossdkusmapifacetoep( &usgetscbkp()->uscusbctl, iface, dir );
    if( epnum < 0 )
    {
        /* QMI (qcqmi) and non-QMI (tty) interface numbers are always unique,
         * therefore no situation should occur where epnum is valid for a
         * non-QMI interface due to the call below.
         */
        epnum = swi_ossdkusmapifacetoep_qmi( &usgetscbkp()->uscusbctl, iface, dir );
        if( epnum < 0 )
        {
            return USEPINVALID;
        }
    }
    return epnum;
}

/*************
 *
 * Name:    usmapepbyifnum
 *
 * Purpose: populate struct usbep based on interface number
 *
 * Param:   ifnum   - interface number
 *          ep      - usbep structure to populate
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void usmapepbyifnum( swi_uint32 ifnum,
                           struct usbep* ep )
{
    ep->usreadep  = usifacetoep( ifnum, SWI_EP_IN );
    ep->uswriteep = usifacetoep( ifnum, SWI_EP_OUT );
}

/*************
 *
 * Name:    usgetep
 *
 * Purpose: Return the endpoints to the caller This function should
 *          only be called once, otherwise if the modem resets before
 *          the caller uses the endpoint information, the results could
 *          be invalid.
 *
 * Param:   eptype  - enum useptype
 *          ep      - pointer to endpoint structure to return the read &
 *                    write endpoints.
 *
 * Return:  TRUE    - endpoint available
 *          FALSE   - endpoint is not available
 *
 * Abort:   none
 *
 * Notes:   This function should be called only just before the
 *          call to the read or write function, otherwise the
 *          EP information could be invalid. The function
 *          incorporates a call to usvidpid() which forces a
 *          check of the OS package's internal USB state. If the
 *          modem is active, then the state is fine and the EP
 *          returned will be the specified endpoint. But if the modem
 *          is inactive, this function will return FALSE
 *          and the caller should not try to read or write to the
 *          modem.
 *
 *          There is still a small possibility that the modem can
 *          crash or be reset between the time this function is
 *          called and the call to the US read or write entry.
 *          But if this happens, the call to the USB read or write
 *          function would return an error.
 *
 **************/
global swi_bool usgetep( enum useptype eptype,
                         struct usbep  *ep )
{
    /* Start with invalid endpoint */
    ep->usreadep  = USEPINVALID;
    ep->uswriteep = USEPINVALID ;

    /* First determine if a modem has been detected */
    swi_uint32 vendid, prodid;
    if( !usvidpid( &vendid, &prodid ) )
    {
        return FALSE;
    }

    /* Get a pointer to the US package control block */
    struct usscb *usscbkp = usgetscbkp();

    switch (eptype)
    {
        case USEP_DIAG:
            ep->usreadep  = usscbkp->useptab.usdiagep.usreadep;
            ep->uswriteep = usscbkp->useptab.usdiagep.uswriteep;
            break;
        case USEP_NMEA:
            ep->usreadep  = usscbkp->useptab.usnmeaep.usreadep;
            ep->uswriteep = usscbkp->useptab.usnmeaep.uswriteep;
        break;
        case USEP_ATCMD:
            ep->usreadep  = usscbkp->useptab.usatcmdep.usreadep;
            ep->uswriteep = usscbkp->useptab.usatcmdep.uswriteep;
        break;
        case USEP_MASS_STORAGE:
            ep->usreadep  = usscbkp->useptab.usmassstorageep.usreadep;
            ep->uswriteep = usscbkp->useptab.usmassstorageep.uswriteep;
        break;
        case USEP_QMI:
            ep->usreadep  = usscbkp->useptab.usqmiep.usreadep;
            ep->uswriteep = usscbkp->useptab.usqmiep.uswriteep;
        break;
        case USEP_QDL:
            ep->usreadep  = usscbkp->useptab.usqdlep.usreadep;
            ep->uswriteep = usscbkp->useptab.usqdlep.uswriteep;
            /* In case the end points are invalid, check the end points
             * corresponding to interface 0. This scenario occurs when the
             * device used is a Sierra Gobi Device */
            if(ep->usreadep == USEPINVALID || ep->uswriteep == USEPINVALID)
            {
                ep->usreadep  = usscbkp->useptab.usqdlep2.usreadep;
                ep->uswriteep = usscbkp->useptab.usqdlep2.uswriteep;
            }
        break;
        default:
            return FALSE;
    }

    if (ep->usreadep == USEPINVALID || ep->uswriteep == USEPINVALID)
    {
        return FALSE;
    }

    return TRUE;
}

/*************
 *
 * Name:    uslearneps
 *
 * Purpose: Get all Endpoints for the currently enumerated modem
 *
 * Param:   devicemode  - a supported device mode
 *
 * Return:  TRUE    - endpoints filled
 *          FALSE   - device not recognized or unsupported modem mode specified
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local swi_bool uslearneps(enum usdevicemodes devicemode)
{
    swi_bool retval = FALSE;
    swi_uint32 pid, vid;

    /* Endpoint mapping depends on Product ID */
    if( !usvidpid( &vid, &pid ) )
    {
        return retval;
    }

    /* Get a pointer to the US package control block */
    struct usscb *usscbkp = usgetscbkp();

    switch(pid)
    {
        case SWI_QMIPID_APP: /* Sierra QMI devices */
            if( devicemode == USMODE_BOOTHOLD )
            {
                usmapepbyifnum( 0, &usscbkp->useptab.usdiagep ); /* DIAG */
                retval = TRUE;
            }
            else if( devicemode == USMODE_APP )
            {
                usmapepbyifnum( 0, &usscbkp->useptab.usdiagep ); /* DIAG */
                usmapepbyifnum( 2, &usscbkp->useptab.usnmeaep ); /* NMEA */
                usmapepbyifnum( 3, &usscbkp->useptab.usatcmdep); /* MDM  */
                usmapepbyifnum( 8, &usscbkp->useptab.usqmiep  ); /* NET  */
                retval = TRUE;
            }
            break;

        case GOBI3KGENERIC_BOOT0: /* Gobi devices */
        case GOBI3KGENERIC_BOOT1:
        case GOBI3KGENERIC_BOOT2:
        case GOBI3KGENERIC_BOOT3:
        case GOBI3KHP_BOOT:
            usmapepbyifnum( 1, &usscbkp->useptab.usqdlep   ); /* QDL  */
            /* QDL End points map differently for SL9090 devices
             * wherein the interface number is 0 */
            usmapepbyifnum( 0, &usscbkp->useptab.usqdlep2   ); /* QDL for GobiSierra */
            retval = TRUE;
            break;

        case GOBI3KGENERIC_APP0: /* Gobi3k devices */
        case GOBI3KGENERIC_APP1:
        case GOBI3KGENERIC_APP2:
        case GOBI3KGENERIC_APP3:
        case GOBI3KHP_APP:
            usmapepbyifnum( 0, &usscbkp->useptab.usqmiep   ); /* NET  */
            usmapepbyifnum( 1, &usscbkp->useptab.usdiagep  ); /* DIAG */
            usmapepbyifnum( 2, &usscbkp->useptab.usatcmdep ); /* MDM  */
            usmapepbyifnum( 3, &usscbkp->useptab.usnmeaep  ); /* NMEA */
            retval = TRUE;
            break;

        default:
            break;
    }

    return retval;
}

/************
 *
 * Name:    us_trigger_device_notification_callbacks
 *
 * Purpose: Device notification callback execution for registered clients
 *
 * Param:   ev  - USB device event
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 *************/
global void
us_trigger_device_notification_callbacks(enum usb_device_event ev)
{
    int i;
    for( i = 0 ; i < US_EVT_NOTIF_CLIENTS_MAX ; i++ )
    {
        if( useventcallbacklist[i] != NULL )
        {
            useventcallbacklist[i](ev);
        }
    }
}

/*************
 *
 * Name:    us_register_device_notification_callback
 *
 * Purpose: Device event notification registration API
 *
 * Parms:   ev  - SIO device event
 *
 * Return:  TRUE if successful, FALSE if registration list is full
 *
 * Abort:   none
 *
 * Notes:   USB clients use this API in order to register a callback
 *          function that will be executed when an event corresponding
 *          to the underlying device occurs. The registered callback
 *          must not block.
 *
 **************/
global swi_bool
us_register_device_notification_callback(void(*pcb)(enum usb_device_event ev))
{
    int i;
    for( i = 0 ; i < US_EVT_NOTIF_CLIENTS_MAX ; i++ )
    {
        if( useventcallbacklist[i] == NULL )
        {
            useventcallbacklist[i] = pcb;
            /* callback registered */
            return TRUE;
        }
    }

    /* registration list full */
    return FALSE;
}

/*
 * Name:    ustask
 *
 * Purpose: The USB task's main loop.
 *
 * Parms:   paramp - pointer to data that may be passed in when this task
 *                   is started.
 *
 * Returns: None
 *
 * Abort:   None
 *
 * Notes:   This task periodically scans sysfs to detect any changes in the
 *          state of the connected device and reports the corresponding events
 *          to those clients which have registered for device state changes.
 *
 */
global void ustask(void *paramp)
{
    UNUSEDPARAM(paramp);

    /* get a pointer to the US package control block */
    struct usscb *usscbkp = usgetscbkp();

    /* register with DL for logging, initially disabled */
    dlregister( "USST", &usscbkp->usdlscb, FALSE);

    /* initialize the task's waiter structure */
    swi_ossdkwaiterinit( &usscbkp->uscwaiterscan );

    /* Initialize the task's USB structure */
    swi_ossdkusbinit( &usscbkp->uscusbctl );

    /* report that task has been created and is now running */
    sutaskinitcb(USSDKPKG);

    for( ; ; )
    {
        /* run a scan for USB devices */
        swi_ossdkusbscan( &usscbkp->uscusbctl );

        /* update usb control block */
        swi_uint32 numif;
        if( usgetnuminterfaces(&numif) )
        {
            uslearneps( numif == 1 ? USMODE_BOOTHOLD : USMODE_APP );
        }

        /* send notifications if the device is either disconnected or ready */
        if( SWI_OS_USB_STATE_CHECK != usscbkp->uscusbctl.osusbstate )
        {
            if( SWI_OS_USB_STATE_OK == usscbkp->uscusbctl.osusbstate )
            {
                us_trigger_device_notification_callbacks( numif == 1
                                                          ? US_BOOT_DEVICE_READY_EVT
                                                          : US_APP_DEVICE_READY_EVT );
            }
            else
            {
                us_trigger_device_notification_callbacks( US_DEVICE_DISCONNECTED_EVT );
            }
        }

        /* sleep before rescanning sysfs */
        swi_ossdkwaiterwait( &usscbkp->uscwaiterscan, USSLEEPSCAN );
    }
}


/*
 * Name:    ustaskinit
 *
 * Purpose: spawn the task that scans sysfs in order to determine if a
 *          supported device is present, and if so, to ensure correct
 *          endpoint to /dev node mapping
 *
 * Param:   None
 *
 * Returns: None
 *
 * Abort:   None
 *
 * Notes:   None
 */
global void ustaskinit(void)
{
    swi_ossdkthreadcreate( ustask, NULL );
}

/*
 * Name:    usinit
 *
 * Purpose: To initialize the USB serial interface package
 *
 * Param:   None
 *
 * Returns: None
 *
 * Abort:   None
 *
 * Notes:   None
 */
global void usinit(void)
{
    struct usscb *usscbkp;

    /* Get a pointer to the US package control block */
    usscbkp = usgetscbkp();

    /* Initially no valid read or write endpoints */
    usscbkp->useptab.usdiagep.usreadep    = USEPINVALID;
    usscbkp->useptab.usdiagep.uswriteep   = USEPINVALID;
    usscbkp->useptab.usnmeaep.usreadep    = USEPINVALID;
    usscbkp->useptab.usnmeaep.uswriteep   = USEPINVALID;
    usscbkp->useptab.usatcmdep.usreadep   = USEPINVALID;
    usscbkp->useptab.usatcmdep.uswriteep  = USEPINVALID;
    usscbkp->useptab.usqmiep.usreadep     = USEPINVALID;
    usscbkp->useptab.usqmiep.uswriteep    = USEPINVALID;
    usscbkp->useptab.usmassstorageep.usreadep   = USEPINVALID;
    usscbkp->useptab.usmassstorageep.uswriteep  = USEPINVALID;
}

