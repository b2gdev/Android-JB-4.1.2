/*************
 *
 * Filename:  icticktask_sdk
 *
 * Purpose:   Contains Time Tick task and associated functions
 *
 * Copyright: © 2011-2012 Sierra Wireless Inc., all rights reserved
 *
 **************/

/*-------------
  include files
 --------------*/

#include "ic/icidefs.h"
#include "mm/mmudefs.h"
#include "ql/qludefs.h"
#include "sdk/sdkudefs.h"
#include "su/suudefs.h"

/*-------
  Defines
 --------*/

/*-------------
  Local storage
 --------------*/

/* IC Time Tick Task  Control block storage */
local struct icttcb icttcbk;

/*---------
  Functions
 ----------*/

/*************
 *
 * Name:    icgetttcbp
 *
 * Purpose: Returns a pointer to the IC Time Tick Task control block
 *          structure.
 *
 * Parms:   None
 *
 * Return:  Pointer to locally defined control block
 *
 * Abort:   None
 *
 * Notes:   None
 *
 **************/
package struct icttcb *icgetttcbp( void )
{
    return &icttcbk;
}

/*************
 *
 * Name:    icttinit
 *
 * Purpose: Initializes the Time Tick Task of the IC package.
 *
 * Parms:   None
 *
 * Return:  None
 *
 * Abort:   None
 *
 * Notes:   Must be called once at initialization time before the
 *          other tasks start registering for time ticks.
 *
 **************/
local void icttinit( void )
{
    struct icttcb *icttcbp = icgetttcbp();

    /* Initialize waiter structure */
    swi_ossdkwaiterinit( &(icttcbp->icttwaiter) );

    /* Initialize semaphore */
    swi_ossdksemaphoreinit( &(icttcbp->icttsem) );

    /* Initialize the head of registration queue */
    qlinitlnk(&(icttcbp->icttregqhp.icttqlink), NULL);

    /* Register with the logger process */
    dlregister ("ICTT", &icttcbp->icttdluser, FALSE);

}

/*************
 *
 * Name:    ictttask
 *
 * Purpose: The IC Time Tick task main entry point
 *
 * Parms:   paramp - pointer to data that may be passed in when this task
 *                   is started.
 *
 * Return:  never returns
 *
 * Abort:   None
 *
 * Notes:   None
 *
 *
 **************/
local void ictttask( void *paramp )
{
    UNUSEDPARAM( paramp );

    /* Define and allocate the task control block */
    struct icttcb *icttcbp = icgetttcbp();

    /* Initialize task structures */
    icttinit();

    /* Log this task's startup */
    dlLog0( &icttcbp->icttdluser, ICLOG_CLASSA, "IC Tick Task Started" );

    /* Now that the startup is logged, disable logging for IC */
    dlmasterenable( &icttcbp->icttdluser, FALSE );

    /* report that task has been created and is now running */
    sutaskinitcb(ICSDKPKG);

    /* Main line of processing begins next */
    swi_bool result;
    struct icttregqblk *regqblkp = NULL;
    for( ; ; )
    {
        /* Wait for tick */
        result = swi_ossdkwaiterwait( &(icttcbp->icttwaiter), ICTTTICK );

        /* on time-out, send the registered tick message to all registered users*/
        if( !result )
        {
            /* point to the first link in the queue */
            regqblkp = (struct icttregqblk *)qlgetnext(
                        &(icttcbp->icttregqhp.icttqlink));

            /* loop through queue */
            struct icsmtcb *pt;
            while( NULL != regqblkp)
            {
                pt = regqblkp->ictticsmtp;

                /* decrement time-out duration b/n notifcations */
                if( 0 != pt->mstimeout )
                {
                    pt->msrunttime -= ICTTTICK;
                }

                if( 0 == pt->msrunttime )
                {
                    /* send registered tick */
                    icsmt_send( (struct icsmtcb *)(regqblkp->ictticsmtp),
                                (swi_uint32)(regqblkp->ictttickmsg) );

                    /* reset client's count down to next tick transmission */
                    pt->msrunttime = pt->mstimeout;
                }

                regqblkp = (struct icttregqblk *)qlgetnext( &regqblkp->icttqlink );
            }
        }
    }
}

/*************
 *
 * Name:    ictttaskinit
 *
 * Purpose: Creates the IC Time Tick task by making it known to the os
 *          package.
 *
 * Parms:   None
 *
 * Return:  None
 *
 * Abort:   None
 *
 * Notes:   Must be called *after* icinit() has been called
 *
 **************/
global void ictttaskinit( void )
{
    /* Create the IC Time Tick task */
    swi_ossdkthreadcreate( ictttask, NULL );
}

/*************
 *
 * Name:    ictttickreg
 *
 * Purpose: Entry point to Time Tick Task of the IC package.
 *          Register callers for time tick notifications.
 *
 * Parms:   icsmtp          - pointer to intertask message queue
 *          tickmsg         - the message to be send with each tick
 *          callerregblk    - caller supplied memory - registration block
 *          mstimeout       - tick notification interval (0 for default)
 *
 * Return:  None
 *
 * Abort:   None
 *
 * Notes:   Callers task must have been initialized.
 *          Caller must assure memory for callerregblk is permanently
 *          allocated
 *
 **************/
global void ictttickreg(
    struct icsmtcb      *icsmtp,
    swi_uint32          tickmsg,
    struct icttregqblk  *callerregblk,
    swi_uint32          mstimeout )
{
    struct icttregqblk *regblk; /* local copy */
    struct icttcb *icttcbp = icgetttcbp();

    regblk = callerregblk;

    /* Register the caller for time ticks */
    /* Lock  semaphore */
    swi_ossdksemaphorelock( &(icttcbp->icttsem) );

    /* Fill the registration queue block supplied by the caller */
    regblk->ictticsmtp = icsmtp;
    regblk->ictttickmsg = tickmsg;
    icsmtp->mstimeout = mstimeout == 0 ? ICTTTICK : mstimeout;
    icsmtp->msrunttime = mstimeout;

    /* Initialize queue link */
    qlinitlnk(&regblk->icttqlink, regblk);

    /* Add the registration to head of queue */
    qladdprev(&(icttcbp->icttregqhp.icttqlink), &regblk->icttqlink);

    /* Unlock  semaphore */
    swi_ossdksemaphoreunlock( &(icttcbp->icttsem) );
}
