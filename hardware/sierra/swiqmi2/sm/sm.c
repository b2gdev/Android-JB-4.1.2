/************
 *
 * Filename:  sm.c
 *
 * Purpose:   State Machine implementation
 *
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 ************/

/*-------------
  include files
 --------------*/

#include "aa/aaglobal.h"
#include "dl/dludefs.h"
#include "er/erudefs.h"
#include "smudefs.h"
#include <stdio.h>

/*-------
  Defines
 --------*/

/*-------------
  Local storage
 --------------*/
local struct dlusercb smlog;

/*---------
  Functions
 ----------*/

/*************
 *
 * Name:     sm_is_ill_defined
 *
 * Purpose:  checks if the state machine is ill defined
 *
 * Parms:    sm (IN)    - state machine instance
 *
 * Return:   true - state machine ill defined, false otherwise
 *
 * Abort:    none
 *
 * Notes:    none
 *
 **************/
local swi_bool sm_is_ill_defined(
    const sm_t *sm)
{

    UNUSEDPARAM( sm );

    /* To Do - ensure no loop exists such that a valid state cannot be entered */
    /* To Do - validate states stored in table */
    return FALSE;
}

/*************
 *
 * Name:     smvalidate
 *
 * Purpose:  validates state machine definition
 *
 * Parms:    sm (IN)    - state machine instance
 *
 * Return:   none
 *
 * Abort:    none
 *
 * Notes:    must be called prior to using the state machine for the first time
 *
 **************/
global void smvalidate(
    const sm_t *sm )
{

    if (sm == NULL)
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d NULL pointer passed",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, 0);
    }

    if( sm_is_ill_defined(sm) )
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d Ill-defined State Machine",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, 0);
    }
}

/*************
 *
 * Name:     smrun
 *
 * Purpose:  advances state machine to next state.
 *
 * Parms:    sm (IN)    - state machine instance
 *           ev         - event
 *
 * Return:   none
 *
 * Abort:    none
 *
 * Notes:
 *
 *
 **************/
global void smrun(
    sm_t *sm,
    const swi_uint32 ev )
{
    swi_uint32 nextstate;
    swi_uint32 localState;

    if( ev > sm->evnum )
    {
        dlLog1( &smlog,
                DLCLASSALL,
                "Event out of range ",
                ev );

        return;
    }

    nextstate = ((sm->stt)[sm->state * sm->evnum + ev]);

    if( nextstate == sm->stInvld )
    {
        dlLog2( &smlog,
                DLCLASSALL,
                "invalid transition: current state 0x%02X; event 0x%02X",
                sm->state,
                ev );

        return;
    }

    if( nextstate == sm->stIgnore )
    {
        dlLog2( &smlog,
                DLCLASSALL,
                "ignoring event: event 0x%02X state 0x%02X",
                ev,
                sm->state );

        return;
    }

    dlLog3( &smlog,
            DLCLASSALL,
            "event: 0x%02X transition: 0x%02X->0x%02X",
            ev,
            sm->state,
            nextstate );

    /* In order for this function to be re-entrant, i.e. to properly handle
     * concurrent execution of multiple state machines whereby one state machine
     * triggers another, we save the state of the currently executing state
     * machine prior to calling the state action function.
     */
    localState = sm->state;
    sm->state = nextstate;
    if( sm->sat[localState * sm->evnum + ev] != NULL )
    {
        sm->sat[localState * sm->evnum + ev](sm->udata);
    }
}

/*************
 *
 * Name:     sminit
 *
 * Purpose:  SM package initializations
 *
 * Parms:    none
 *
 * Return:   none
 *
 * Abort:    none
 *
 * Notes:    none
 *
 **************/
global void sminit()
{
    dlregister("SM", &smlog, TRUE);
}
