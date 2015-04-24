/**
 *
 * @ingroup swims
 *
 * @file
 * main fuction of modem scan for swims package
 *
 * @author
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"      /* data types typedefs */
#include "swims_ossdk.h"       /* package user include file  */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>

/** 
 * Generic static interface number definition
 */
enum gen_interface_num
{
  SWI_GEN_AT,
  SWI_GEN_GPS, 
  SWI_GEN_DIAG
};

/** 
 * Sierra static interface number definition
 */
enum sin_interface_num
{
  SWI_SIN_HIP = 0,          /**< HIP interface */
  SWI_SIN_DIAG,             /**< diagnostic interface */
  SWI_SIN_NMEA,             /**< NMEA interface */
  SWI_SIN_AT,               /**< AT command interface */
  SWI_SIN_PDP1,             /**< PDP1 interface */
  SWI_SIN_PDP2,             /**< PDP1 interface */
  SWI_SIN_PDP3,             /**< PDP1 interface */
  SWI_SIN_DIP,              /**< direct IP interface */
  SWI_SIN_QMI,              /**< QMI interface */
  SWI_SIN_MASS_STORAGE,     /**< mass storage interface */
};

/** 
 * Gobi static interface number definition. Given specific 
 * interface numbers in this list to emphasize they aren't 
 * numbered from zero
 */
enum gobi_interface_num
{
  SWI_GOBI_DM = 1,
  SWI_GOBI_AT = 2,
  SWI_GOBI_GPS = 3
};

/* usb */
local struct swi_osusb usbs; /**< shared variable of struct swi_osusb */

/**
 *
 * Thread that scans usb ports
 *
 * @param[in] paramp 
 *     Pointer to a void type - not used
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     
 */
local void usbscanthread(void *paramp)
{
    for ( ; ; )
    {
        sleep(1);
        /* too many print out disable it 
         * swi_ossdklog("Scanning USB\n"); 
         */
        swi_ossdkusbscan(&usbs);    /* see if modem is present */
        
    }
    
}

/**
 *
 * initialize the modem scan and start scan process for library usage
 *
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 */
void swims_ossdkscaninit(void)
{    
    /* init the USB structure */
    swi_ossdkusbinit(&usbs);    

    /* start a scan thread */
    swi_ossdkthreadcreate(usbscanthread, NULL);
}

/**
 *
 * Get the specified command port tty name for library usage
 *
 * @param interface 
 *     The modem-agnostic interface type which is translated into
 *     a modem-specific type inside this function
 *
 * @return
 *     pointer to the requested command port tty name or 
 *     NULL if the interface doesn't exist or there was a problem
 *
 * @par Abort:<br> 
 *     when the PID of modem is invalid
 *
 * @note
 *     return value can be NULL
 */
char * swims_ossdkgetifname( swi_uint32 interface )
{    
    static char ttydevname[256];
    swi_uint32 msinterface; /* modem specific interface */
    swi_uint32 vendorid;
    swi_uint32 productid;
    swi_int32 wrendpoint = -1;
    static swi_bool newstate = FALSE;
    static swi_bool oldstate = FALSE;
    char modemstatusmsg[256];
    
    if (swi_ossdkusbproduct(&usbs, &vendorid, &productid))
    {
        /* it is sufficient to check OUT EP only */
        switch(productid)
        {
            case 0x68A2:        /* Composite MC7750 etc  */
            case 0x68A3:        /* Composite MC8791 USB305 SL8080 etc  */

                /* Map to the modem specific interface base on Product ID */
                if( interface == SWI_GEN_AT )
                {
                    msinterface = SWI_SIN_AT;
                }
                else if( interface == SWI_GEN_GPS )
                {
                    msinterface = SWI_SIN_NMEA;
                }
                else if( interface == SWI_GEN_DIAG )
                {
                    msinterface = SWI_SIN_HIP; /*MC77XX is using SWI_SIN_HIP instead of SWI_SIN_DIAG */
                }
                else
                {
                    swi_ossdklog("SWI Iface conversion not implemented");
                    return NULL;
                }
                wrendpoint = swi_ossdkusmapifacetoep(&usbs, msinterface, SWI_EP_OUT);
                break;

            case 0x9011:        /* Composite MC83xx etc  */
            case 0x9013:        /* Composite MC83xx etc  */
            case 0x9019:        /* Composite MC83xx for Lenovo */
                if( interface == SWI_GEN_AT )
                {
                    msinterface = SWI_GOBI_AT; /* AT is on GOBI USB interface 2 */
                }
                else if( interface == SWI_GEN_GPS )
                {
                    msinterface = SWI_GOBI_GPS; /* GPS is on GOBI USB interface 3 */
                }
                else if( interface == SWI_GEN_DIAG ) 
                {
                    msinterface = SWI_GOBI_DM;  /* DM is on GOBI USB interface 1 */
                }
                else
                {
                    swi_ossdklog("Gobi Iface conversion not implemented");
                    return NULL;
                }
                wrendpoint = swi_ossdkusmapifacetoep(&usbs, msinterface, SWI_EP_OUT);
                break;
                                        
            default:
                swi_ossdklog("Unrecognized product ID\n");
                wrendpoint = -1;
                break;
        }
        
        /* Look for interface tty name, but make sure the endpoint is valid.
         * Based on testing, sometimes the endpoint is not valid even though
         * the device was found.  This is normally a temporary condition.
         */
        if ( (wrendpoint >= 0) &&
             (swi_ossdkusbendpoint(&usbs, wrendpoint,
                                   FALSE, ttydevname, sizeof(ttydevname))) )
        {
            snprintf(modemstatusmsg, sizeof(modemstatusmsg),
                    "    USB write endpoint %d is %s\n", (int)wrendpoint, ttydevname);
            swi_ossdklog(modemstatusmsg);

            /* Got valid name; update status and return the name */
            newstate = TRUE;
            oldstate = newstate;
            return ttydevname;
        }
    }

    /* Valid name was not found; return NULL */
    
    newstate = FALSE;
    memset(ttydevname, 0, sizeof(ttydevname));
    if (newstate != oldstate)
    {
        oldstate = newstate;
        
        /* write to log file */
        swi_ossdklog("swims_ossdkgetifname: USB modem not found");
    }
    return NULL;
    
}

/**
 *
 * Get the Sierra AT command port tty name for library usage
 *
 * @return
 *     pointer to the AT command port tty name
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     return value can be NULL
 *
 *     The returned string from this function must be handled immediately
 *     because the internal storage for it will be overwritten if this 
 *     function's sibling function - swims_ossdkgetgpsifname() - is called
 *
 *     Can be called regardless of the type of modem connected
 */
char * swims_ossdkgetatifname( void )
{
    /* Fetch the AT device name */
    return swims_ossdkgetifname( SWI_GEN_AT );
}

/**
 *
 * Get the Sierra GPS command port tty name for library usage
 *
 * @return
 *     pointer to the GPS command port tty name
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     return value can be NULL
 *
 *     The returned string from this function must be handled immediately
 *     because the internal storage for it will be overwritten if this 
 *     function's sibling function - swims_ossdkgetatifname() - is called
 *
 *     Can be called regardless of the type of modem connected
 */
char * swims_ossdkgetgpsifname(void)
{
    /* Fetch the GPS device name */
    return swims_ossdkgetifname( SWI_GEN_GPS );
}

/**
 *
 * Get the Sierra Diag command port tty name for library usage
 *
 * @return
 *     pointer to the Diag command port tty name
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     return value can be NULL
 *
 *     The returned string from this function must be handled immediately
 *     because the internal storage for it will be overwritten if this 
 *     function's sibling function - swims_ossdkgetatifname() and
 *     swims_ossdkgetgpsifname() - are called
 *
 *     Can be called regardless of the type of modem connected
 */
char * swims_ossdkgetdmifname(void)
{
    /* Fetch the Diag device name */
    return swims_ossdkgetifname( SWI_GEN_DIAG );
}

