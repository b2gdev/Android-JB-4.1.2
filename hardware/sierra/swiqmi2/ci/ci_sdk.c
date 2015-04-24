/*
 * Filename: ci_sdk.c
 *
 * Purpose:  Routines callable by other packages to obtain information on
 *           IPC channels used by the SDK.
 *
 * NOTES:
 *
 * Copyright: © 2009 Sierra Wireless Inc., all rights reserved
 */


/* include files */

#include "ciidefs.h"
#include <stdio.h>

/* Global Control Block */

/* local variables */
/* Functions */

/*
 * Name:    ciinitsdkipcchan
 *
 * Purpose: Opens the IPC channel given the name
 *
 * Params:  channelnamep - pointer to IPC channel name.
 *
 * Return:  None.
 *
 * Note:    None.
 *
 * Abort:   Invalid service name.
 *
 */
global void ciinitsdkipcchan( swi_uint8 *channelnamep)
{
    swi_uint16 channel;
    swi_uint16 totalnumchannels;
    struct cigblctrlblk *cigcbp = cigetgcbp();

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

    /* init the IPC channel */
    if (cigcbp->ciipcchanlist[channel].cichanfixedinfop->ciiftype ==
        CIIPC)
    {
        icinitsdkhndl( &cigcbp->ciipcchanlist[channel].ciipccb,
                       cigcbp->ciipcchanlist[channel].cichannel,
                       (swi_uint8 *)cigcbp->ciipcchanlist[channel].
                           cichanfixedinfop->cichannamep);
    }
}
