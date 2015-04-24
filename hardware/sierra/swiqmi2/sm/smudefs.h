/************
 *
 * Filename:  smudefs.h
 *
 * Purpose:   External definitions for the sm package
 *
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 ************/

#ifndef SMUDEFS_H
#define SMUDEFS_H

/* include files */
#include "aa/aaglobal.h"

/*************
 *
 * Name:     Special States
 *
 * Purpose:  The definitions are used to signal the state machine
 *           to take special actions, instead of transitioning to
 *           another state
 *
 * Members:  stINVAL - Received event is invalid for this state - ABORT
 *           stIGNOR - Ignore event in this state
 *
 * Notes:
 *
 **************/
#define stINVAL 0xFF
#define stIGNOR 0xFE

/*************
 *
 * Name:     smaction_t
 *
 * Purpose:  definition for action functions to be executed upon
 *           entering a state.  Used in the state action table.
 *
 * Notes:
 *
 **************/
typedef void (*smaction_t)(void *pdata);

/*************
 *
 * Name:    sm_t
 *
 * Purpose: State machine type.  Allows multiple state machines
 *          to be managed
 *
 * Members: state       - current state
 *          evnum       - number of possible events
 *          sat         - state action table
 *          stt         - state transition table
 *          stIgnore    - State machine value for the IGNORE state
 *          stInvld     - State machine value for the INVALID state
 *          udata       - user data, passed to action functions in the stt
 *
 * Notes:    Events MUST be numbered from 0 to evnum-1.  They are used
 *           to index into the state transition table
 *
 **************/
typedef struct
{
  swi_uint32    state;
  swi_uint8     evnum;
  smaction_t    *sat;
  swi_uint8     *stt;
  swi_uint8     stIgnore;
  swi_uint8     stInvld;
  void          *udata;
}sm_t;

/*
 * Package global entry point definitions - these come last
 */
#include "smuproto.h"

#endif /* SMUDEFS_H */

