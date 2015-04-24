/*************
 *
 * Filename: dsipctask_sdk.c
 *
 * Purpose:  DS package IPC task associated functions
 *
 * Copyright: © 2011-2012 Sierra Wireless Inc., all rights reserved
 *
 **************/

/*-------------
  include files
 --------------*/
#include "ds/dsidefs.h"
#include "ci/ciudefs.h"
#include <stdio.h>

/*-------
  Defines
 --------*/

/*-------------
  Local storage
 --------------*/

/*---------
  Functions
 ----------*/

/*************
 *
 * Name:    dsioctl
 *
 * Purpose: Send ioctl command over a specified channel
 *
 * Parms:   chanid      - channel id
 *          ioctlcmd    - ioctl command to issue
 *          paramp       - storage for ioctl data, if required
 *          parmlen     - size, in bytes, of ioctl data storage
 *
 * Return:  TRUE on success, FALSE otherwise
 *
 * Abort:   none
 *
 * Notes:   On return it is safe for the caller to release the buffer memory if
 *          applicable.
 *
 *          This is thread safe function.
 *
 **************/
global swi_bool
dsioctl(
    swi_uint16 chanid,
    enum qmisupportedclients client,
    swi_uint16 ioctlcmd,
    void       *paramp,
    size_t     parmlen )
{
    switch( cigetifacetype(cigetchannelname(chanid)) )
    {
        case CIUIF:
            return dsep_ioctl( cigetifusetype(chanid),
                               client,
                               ioctlcmd,
                               paramp,
                               parmlen );
        default:
            {
                char errmsg[100];
                snprintf(errmsg, sizeof(errmsg),
                         "%s:%d unknown or unsupported channel type",
                         (char *)__func__, __LINE__);
                erAbort( errmsg,
                         chanid );
            }
    }
    return FALSE; /* rid of compiler warning */
}
/*************
 *
 * Name: dssend
 *
 * Purpose: DS package send interface over a specified channel
 *
 * Parms:   packetptr - pointer to AM packet
 *          packetlen - length of AP packet
 *          chanid    - channel id
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   On return it is safe for the caller to release the buffer
 *          memory if applicable.
 *          This is thread safe function.
 *
 **************/
global swi_bool
dssend(
    swi_uint8 *packetptr,
    swi_uint32 packetlen,
    swi_uint16 chanid,
    enum qmisupportedclients client )
{
    struct icipccb *ipccbp;

    switch( cigetifacetype(cigetchannelname(chanid)) )
    {
        case CIUIF:
            /* Send the message over the endpoint  */
            return dsep_send( cigetifusetype(chanid),
                              client,
                              packetptr,
                              packetlen);
            break;
        case CIIPC:
            /* Obtain pointer to the IPC channel structure */
            ipccbp = cigetipcp(chanid);
            /* Send the message over the IPC channel  */
            return icsdk_send( ipccbp,
                               packetptr,
                               packetlen );
            break;
        default:
            {
                char errmsg[100];
                snprintf(errmsg, sizeof(errmsg),
                         "%s:%d unknown channel type",
                         (char *)__func__, __LINE__);
                erAbort(errmsg, 0 );
            }
            break;
    }

    return FALSE;
}

/*************
 *
 * Name: dsclose
 *
 * Purpose: Closes specific IPC channel
 *
 * Parms:   chanid - channel id
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   This function clears channels structure and un-installs the
 *          receive callback function
 *
 **************/
global void
dsclose(
    swi_uint16 chanid,
    enum qmisupportedclients client )
{
    switch( cigetifacetype(cigetchannelname(chanid)) )
    {
        case CIUIF:
            dsep_close( cigetifusetype(chanid), client );
            break;
        default:
            {
                char errmsg[100];
                snprintf(errmsg, sizeof(errmsg),
                         "%s:%d unknown channel type",
                         (char *)__func__, __LINE__);
                erAbort(errmsg, 0 );
            }
    }
}
