/*
 * Filename: cipackage.c
 *
 * Purpose:  Routines callable by other packages to obtain information on
 *           IPC channels common to both SDK and API.
 *
 * NOTES:
 *
 * Copyright: © 2011-2012 Sierra Wireless Inc., all rights reserved
 */

/* include files */

#include "aa/aaglobal.h"
#include "ciidefs.h"
#include <stdio.h>

/* Global Control Block */

local struct cigblctrlblk cigcb;

/* local variables */

/* Channel table - Do not re-arrange the order once this is in place */

const struct ciipcchanfixed ciipcchanfixedinfo[] =
{/* name           peername         I/F         IPC USAGE       USB I/F USAGE */
    {CIIPCQMRR1,    CIIPCQMNOTIF1,  CIIPC,      CIRR,           USEP_INVALID},
    {CIIPCQMNOTIF1, CIIPCQMRR1,     CIIPC,      CINOTIF,        USEP_INVALID},
    {CIIPCUQMRR1,   "DUMMY",        CIUIF,      CIIFINVALID,    USEP_QMI},
    {CIIPCUIMRR1,   "DUMMY",        CIUIF,      CIIFINVALID,    USEP_DIAG},
    {CIIPCUIMRR2,   "DUMMY",        CIUIF,      CIIFINVALID,    USEP_QDL},
    {CIIPCUI,       "DUMMY",        CIIPC,      CIUI,           USEP_INVALID},
    {CIIPCDIAG,     CIIPCUDIAG,     CIIPC,      CIDRSVC,        USEP_INVALID},
    {CIIPCUDIAG,    CIIPCDIAG,      CIUIF,      CIIFINVALID,    USEP_DIAG},
    {CIIPCNMEA,     CIIPCUNMEA,     CIIPC,      CIDRSVC,        USEP_INVALID}
};

/*
 * Maximum number of channels at run time. Note that CIMAXIPCCHAN must be
 * in sync with this.
 */
#define CIMAXCHANNELS sizeof(ciipcchanfixedinfo)/sizeof(struct ciipcchanfixed)

/* Functions */
/*
 * Name:    cigetgcbp
 *
 * Purpose: To get a pointer to the package's global control block.
 *
 * Params:  None.
 *
 * Return:  Pointer to the package's global control block.
 *
 * Note:    None.
 *
 * Abort:   None.
 *
 */
package struct cigblctrlblk * cigetgcbp(void)
{
    return (&cigcb);
}

/*
 * Name:    cigetnumchannels
 *
 * Purpose: To get the total number of IPC channels
 *
 * Params:  None.
 *
 * Return:  The total number of IPC channels.
 *
 * Note:    None.
 *
 * Abort:   None.
 *
 */
global swi_uint16 cigetnumchannels(void)
{
    return (CIMAXCHANNELS);
}

/*
 * Name:    cigetfixedinfop
 *
 * Purpose: To get a pointer to the IPC channel's fixed information block
 *
 * Params:  channel - channel to use as index into our table.
 *
 * Return:  Pointer to the IPC channel's fixed information block
 *
 * Note:    None.
 *
 * Abort:   None.
 *
 */
package struct ciipcchanfixed * cigetfixedinfop(swi_uint16 channel)
{
    return (cigcb.ciipcchanlist[channel].cichanfixedinfop);
}

/*
 * Name:    cigetindex
 *
 * Purpose: To get index of the main name item in the table
 *
 * Params:  channelnamep - pointer to the IPC name .
 *
 * Return:  Index to the table if found
 *          Value of the number of items, ie, non-index
 *
 * Note:    None.
 *
 * Abort:   None.
 *
 */
global swi_uint16 cigetindex( swi_uint8 *channelnamep )
{
    swi_uint16 i;
    swi_uint32 len;
    swi_uint16 totalnumchannels;

    totalnumchannels = cigetnumchannels();
    len = slstrlen ((const char *)channelnamep);
    for (i = 0; i < totalnumchannels; i++)
    {
        if (slmemcmp (channelnamep,
                      ciipcchanfixedinfo[i].cichannamep,
                      len) == 0)
        {
            return (i);
        }
    }
    return totalnumchannels;
}

/*
 * Name:    ciinit
 *
 * Purpose: Initialization for the CI package.
 *
 * Params:  None.
 *
 * Return:  None.
 *
 * Note:    Fixed information is referenced by RAM portion of the channel
 *          information blocks. Any application that needs to use the IPC
 *          channels to communicate with the SDK must call this first before
 *          using any of the CI functions.
 *
 * Abort:   None.
 *
 */
global void ciinit(void)
{
    int i;
    swi_uint16 totalnumchannels;

    totalnumchannels = cigetnumchannels();

    /* Copy the fixed info to the RAM portion */
    for (i = 0; i < totalnumchannels; i++)
    {
        cigcb.ciipcchanlist[i].cichanfixedinfop =
                                (struct ciipcchanfixed *)&ciipcchanfixedinfo[i];
        cigcb.ciipcchanlist[i].cichannel = i;
    }
}

/*
 * Name:    cigetipcp
 *
 * Purpose: To get a pointer to the IPC control block.
 *
 * Params:  ciipcchannel - IPC channel.
 *
 * Return:  Pointer to the channel's IPC control block.
 *
 * Note:    None.
 *
 * Abort:   None.
 *
 */
global struct icipccb * cigetipcp(swi_uint16 ciipcchannel)
{
    return (&cigcb.ciipcchanlist[ciipcchannel].ciipccb);
}

/*
 * Name:    cigetnextipcp
 *
 * Purpose: To get a pointer to a given 'use type' IPC control block.
 *
 * Params:  ipcnum   - channel index, starting from 0
 *          ipcifuse - use type of the channel
 *
 * Return:  Pointer to an IPC control block.
 *          NULL - if not found
 *
 * Note:   For package use to hide the 'use type' from end-user.
 *
 * Abort:  None.
 *
 */
package struct icipccb * cigetnextipcp(
    swi_uint16 ipcnum,
    enum ciipcifusetyp  ipcifuse)
{
    int i;
    int localcount = 0;
    swi_uint16 totalnumchannels;

    totalnumchannels = cigetnumchannels();

    /* Loop through our channel table */
    for (i=0;  i < totalnumchannels; i++)
    {
        if (cigcb.ciipcchanlist[i].cichanfixedinfop->ciiftype == CIIPC)
        {
            if (cigcb.ciipcchanlist[i].cichanfixedinfop->ciipcifuse == ipcifuse)
            {
                /* Return the IPC control block */
                if (ipcnum == localcount)
                {
                    return (&cigcb.ciipcchanlist[i].ciipccb);
                }
                localcount++;
            }
        }
    }
    /* no more */
    return NULL;
}

/*
 * Name:    cigetnextrripcp
 *
 * Purpose: To get a pointer to a RR IPC control block.
 *
 * Params:  rripcnum - RR channel index, starting from 0
 *
 * Return:  Pointer to a RR IPC control block.
 *          NULL - if not found
 *
 * Note:    None.
 *
 * Abort:   None.
 *
 */
global struct icipccb * cigetnextrripcp(swi_uint16 rripcnum)
{
    return ( cigetnextipcp(rripcnum, CIRR) );
}

/*
 * Name:    cigetnextnotifipcp
 *
 * Purpose: To get a pointer to a notification IPC control block.
 *
 * Params:  notifipcnum - notification channel index, starting from 0
 *
 * Return:  Pointer to a notification IPC control block.
 *          NULL - if not found
 *
 * Note:    None.
 *
 * Abort:   None.
 *
 */
global struct icipccb * cigetnextnotifipcp(swi_uint16 notifipcnum)
{
   return ( cigetnextipcp(notifipcnum, CINOTIF) );
}

/*
 * Name:    cigetnextchannel
 *
 * Purpose: To get the channel number of the next'use type'
 *
 * Params:  ipcnum   - channel index, starting from 0
 *          ipcifuse - use type of the channel
 *
 * Return:  Channel number.
 *          if not found, Total number of channels
 *
 * Note:   For package use to hide the 'use type' from end-user.
 *
 * Abort:  None.
 *
 */
package swi_uint16 cigetnextchannel(
    swi_uint16 ipcnum,
    enum ciipcifusetyp  ipcifuse)
{
    int i;
    int localcount = 0;
    swi_uint16 totalnumchannels;

    totalnumchannels = cigetnumchannels();

    /* Loop through our channel table */
    for (i=0;  i < totalnumchannels; i++)
    {
        if (cigcb.ciipcchanlist[i].cichanfixedinfop->ciiftype == CIIPC)
        {
            if (cigcb.ciipcchanlist[i].cichanfixedinfop->ciipcifuse == ipcifuse)
            {
                /* Return the IPC control block */
                if (ipcnum == localcount)
                {
                    return (i);
                }
                localcount++;
            }
        }
    }
    /* no more */
    return totalnumchannels;
}
/*
 * Name:    cigetnextrrchannum
 *
 * Purpose: To get a pointer to a RR IPC control block.
 *
 * Params:  rripcnum - RR channel index, starting from 0
 *
 * Return:  Pointer to a RR IPC control block.
 *          NULL - if not found
 *
 * Note:    None.
 *
 * Abort:   None.
 *
 */
global swi_uint16 cigetnextrrchannum(swi_uint16 rripcnum)
{
    return ( cigetnextchannel(rripcnum, CIRR) );
}

/*
 * Name:    cigetnextrrnamep
 *
 * Purpose: To get a pointer to a CIRR IPC name.
 *
 * Params:  ipcnum - RR channel index, starting from 0
 *
 * Return:  Pointer to a RR IPC name
 *          NULL - if not found
 *
 * Note:    None.
 *
 * Abort:   None.
 *
 */
global swi_uint8 * cigetnextrrnamep(swi_uint16 ipcnum)
{
    int i;
    int localcount = 0;
    swi_uint16 totalnumchannels;

    totalnumchannels = cigetnumchannels();

    /* Loop through our channel table */
    for (i=0;  i < totalnumchannels; i++)
    {
        if (cigcb.ciipcchanlist[i].cichanfixedinfop->ciiftype == CIIPC)
        {
            if (cigcb.ciipcchanlist[i].cichanfixedinfop->ciipcifuse == CIRR)
            {
                /* Return the IPC name pointer */
                if (ipcnum == localcount)
                {
                  return (swi_uint8 *)(cigcb.ciipcchanlist[i].
                          cichanfixedinfop->cichannamep);
                }
                localcount++;
            }
        }
    }
    /* no more */
    return NULL;
}

/*
 * Name:    cigetnextdrnamep
 *
 * Purpose: To get a pointer to a RR IPC name.
 *
 * Params:  ipcnum - RR channel index, starting from 0
 *
 * Return:  Pointer to a RR IPC name
 *          NULL - if not found
 *
 * Note:    None.
 *
 * Abort:   None.
 *
 */
global swi_uint8  *cigetnextdrnamep(swi_uint16 ipcnum)
{
    int i;
    int localcount = 0;
    swi_uint16 totalnumchannels;

    totalnumchannels = cigetnumchannels();

    /* Loop through our channel table */
    for (i=0;  i < totalnumchannels; i++)
    {
        if (cigcb.ciipcchanlist[i].cichanfixedinfop->ciipcifuse == CIDRSVC)
        {
            /* Return the IPC name pointer */
            if (ipcnum == localcount)
            {
                return (swi_uint8 *)(cigcb.ciipcchanlist[i].
                        cichanfixedinfop->cichannamep);
            }
            localcount++;
        }
    }
    /* no more */
    return (swi_uint8 *)NULL;
}

/*
 * Name:    cigetifusetype
 *
 * Purpose: To get the channel use type for the given channel.
 *
 * Params:  channel - IPC channel
 *
 * Return:  Channel use type of IPC channel.
 *
 * Note:    None.
 *
 * Abort:   None.
 *
 */
global int cigetifusetype( swi_uint16 channel )
{
    if (cigcb.ciipcchanlist[channel].cichanfixedinfop->ciiftype == CIIPC)
        return (cigcb.ciipcchanlist[channel].cichanfixedinfop->ciipcifuse);
    else
        return (cigcb.ciipcchanlist[channel].cichanfixedinfop->ciuifuse);
}

/*
 * Name:    cigetifacetype
 *
 * Purpose: To get the channel interface type for the given channel.
 *
 * Params:  channelnamep - pointer to name of channel.
 *
 * Return:  Channel interface type of IPC channel.
 *          CIIFINVALID - if not found
 *
 * Note:    None.
 *
 * Abort:   Invalid service name.
 */
global enum ciifacetype cigetifacetype( swi_uint8 *channelnamep )
{
    swi_uint16 channel;
    swi_uint16 totalnumchannels;

    totalnumchannels = cigetnumchannels();

    channel = cigetindex (channelnamep);

    if (channel >= totalnumchannels)
    {
        /* Invalid service name */
        /* log a fatal error */
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d Invalid service name",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, (swi_uint32) channelnamep);
    }

    return (cigcb.ciipcchanlist[channel].cichanfixedinfop->ciiftype);
}

/*
 * Name:    cigetchannelnum
 *
 * Purpose: To get the IPC channel number
 *
 * Params:  channelnamep - Pointer to IPC channel name.
 *
 * Return:  IPC channel number.
 *
 * Note:    None.
 *
 * Abort:   Invalid service name.
 */
global swi_uint16 cigetchannelnum(char *channelnamep)
{
    swi_uint16 channel;
    swi_uint16 totalnumchannels;

    totalnumchannels = cigetnumchannels();

    channel = cigetindex ((swi_uint8 *)channelnamep);

    if (channel >= totalnumchannels)
    {
        /* Invalid service name */
        /* log a fatal error */
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d Invalid service name",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, (swi_uint32) channelnamep);
    }

    return (cigcb.ciipcchanlist[channel].cichannel);
}

/*
 * Name:    cigetchannelname
 *
 * Purpose: To get the IPC channel name
 *
 * Params:  channel - IPC channel number.
 *
 * Return:  Pointer to IPC channel name .
 *
 * Note:    None.
 *
 * Abort:   Invalid channel number.
 */
global swi_uint8 * cigetchannelname(swi_uint16 channel)
{
    swi_uint16 totalnumchannels;

    totalnumchannels = cigetnumchannels();
    if (channel >= totalnumchannels)
    {
        /* log a fatal error */
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d Invalid channel number",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, (swi_uint32) channel);
    }

    return ((swi_uint8 *)cigcb.ciipcchanlist[channel].
             cichanfixedinfop->cichannamep);
}

/*
 * Name:    cigetnumappclients
 *
 * Purpose: To get the number of application clients supported
 *
 * Params:  ipcifuse - client use type.
 *
 * Return:  count - number of clients of a given usage.
 *
 * Note:    For package use to hide the 'use type' from end-user.
 *
 * Abort:   N/A.
 */
package swi_uint16 cigetnumappclients( enum ciipcifusetyp  ipcifuse )
{
    swi_uint16 i;
    swi_uint16 count = 0;
    swi_uint16 totalnumchannels;

    totalnumchannels = cigetnumchannels();

    /* Count items */
    for (i = 0; i < totalnumchannels; i++)
    {
        if (ciipcchanfixedinfo[i].ciipcifuse == ipcifuse)
        {
            count++;
        }
    }

    return count;
}

/*
 * Name:    cigetnumRRchan
 *
 * Purpose: To get the number of RR clients
 *
 * Params:  None.
 *
 * Return:  count - number of RR clients.
 *
 * Note:    None.
 *
 * Abort:   N/A.
 */
global swi_uint16 cigetnumRRchan( void )
{
    return ( cigetnumappclients (CIRR) );
}

/*
 * Name:    cigetnumnotifchan
 *
 * Purpose: To get the number of Notification channels
 *
 * Params:  None.
 *
 * Return:  count - number of Notification channels
 *
 * Note:    None.
 *
 * Abort:   N/A.
 */
global swi_uint16 cigetnumnotifchan( void )
{
   return ( cigetnumappclients (CINOTIF) );
}

/*
 * Name:    cigetnumdrservices
 *
 * Purpose: To get the number of CIDRSVC channels
 *
 * Params:  None.
 *
 * Return:  count - number of CIDRSVC channels
 *
 * Note:    None.
 *
 * Abort:   N/A.
 */
global swi_uint16 cigetnumdrservices( void )
{
   return ( cigetnumappclients ( CIDRSVC ) );
}

/*
 * Name:    cigetpeerchan
 *
 * Purpose: To get channel number of the peer
 *
 * Params:  channel    - The channel whose peer we want.
 *
 * Return:  peerchan   - The peer channel
 *
 * Note:    None.
 *
 * Abort:   N/A.
 */
global swi_uint16 cigetpeerchan( swi_uint16 channel )
{
    swi_uint8 *peernamep;

    peernamep = (swi_uint8 *)ciipcchanfixedinfo[channel].cipeerchannamep;

    return (cigetindex (peernamep));
}

/*
 * Name:    cigetpeernamep
 *
 * Purpose: To get channel name of the peer
 *
 * Params:  channel    - The channel whose peer name we want.
 *
 * Return:  peername   - The peer channel name
 *
 * Note:    None.
 *
 * Abort:   N/A.
 */
global swi_uint8 *cigetpeernamep( swi_uint16 channel )
{
    return ((swi_uint8 *)ciipcchanfixedinfo[channel].cipeerchannamep);
}

