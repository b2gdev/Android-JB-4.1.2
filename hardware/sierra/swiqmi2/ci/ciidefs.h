/*************
 *
 * Filename: ciidefs
 *
 * Purpose:  Internal constants and data structures for the CI package.
 *
 * Note:     None
 *
 * Copyright: © 2009 Sierra Wireless Inc., all rights reserved
 *
 **************/

/*
 *  Local constants and enumerated types.
 */
#ifndef ciidefs_h
#define ciidefs_h

#include "aa/aaglobal.h"
#include "er/erudefs.h"
#include "sl/sludefs.h"
#include "ic/icudefs.h"
#include "ciudefs.h"

/*
* Name:     cigblctrlblk
 *
 * Purpose: Global Control Block.  
 *
 * Members: ciipcchanlist  - Array of channel structure 
 * 
 * Note:    None.
 */
struct cigblctrlblk 
{
    struct ciipcchan    ciipcchanlist[CIMAXIPCCHAN];
};

#include "ciiproto.h"

#endif
