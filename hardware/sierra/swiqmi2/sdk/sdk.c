/*************
 *
 * Filename: swi_sdk.c
 *
 * Purpose:  Mainline code for the SDK user-mode task. This file
 *           provides the overall context for the SDK task
 *           (process). This file contains the "main" routine for
 *           the SDK.
 *
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/

/*-------------
  include files
 --------------*/

#include "sdk/sdkidefs.h"

/*-------
  Defines
 --------*/

/*-------------
  Local storage
 --------------*/
local struct swi_sdkcb sdkcbk;

/*---------
  Functions
 ----------*/

/*************
 *
 * Name:    sdkgetcbkp
 *
 * Purpose: return a pointer to the SDK task's control block
 *
 * Params:  No values
 *
 * Return:  pointer to the SDK task's control block
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local struct swi_sdkcb *swi_sdkgetcbkp( void )
{
    /* Return a pointer to the control block */
    return &sdkcbk;
}

/*************
 *
 * Name:    main
 *
 * Purpose: main entry point for the SDK daemon.
 *
 * Params:  none
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   don't write to stdout or stderr from anywhere inside the SDK task.
 *          The file descriptors have been closed and this process is a
 *          detached daemon; use swi_ossdklog instead look in /var/log/<logfile>
 *          where <logfile> is distro-dependent.
 *
 **************/
int main(int argc, const char **argv)
{

    struct swi_sdkcb *sdkcbkp;

    UNUSEDPARAM( argc );
    UNUSEDPARAM( argv );
    /* initialize the underlying process - MUST be done before anything else */
    swi_ossdkprocessinit();

    /* initialize SDK packages and spawn all startup tasks required by the SDK
     * process.
     */
    suinit();

    /* Register for logging */
    sdkcbkp = swi_sdkgetcbkp();
    dlregister ("SDK", &sdkcbkp->sdkdlcb, TRUE);

    /* This daemon never ends it just keeps on going */
    swi_ossdkwaiterinit( &sdkcbkp->sdkwaiter );

    while(1)
    {
        /* thread to sleep permanently to prevent wasteful share of CPU time */
        swi_ossdkwaiterwait( &sdkcbkp->sdkwaiter, 0 );
    }
}
