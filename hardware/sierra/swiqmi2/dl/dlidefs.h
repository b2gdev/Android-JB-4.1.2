/*
 * Filename: dlidefs.h
 *
 * Purpose: Internal definitions for the Debug Logger package (DL)
 *
 * NOTES:
 *
 * Copyright Sierra Wireless Inc., 2008. All rights reserved.
 */

#ifndef dlidefs_h
#define dlidefs_h

#include "aa/aaglobal.h"
#include "dludefs.h"

/* Defines, enumerated types, etc */

#define DLLOGBUFFSZ 100           /* Log string buffer size */
#define DLMEMDUMPMARKER 0x444C    /* mem dump marker "DLOG" */
#define DLLOGMAXARGS    4         /* Maximum number of arguments in formatting
                                   * string
                                   */
/*
 * Name:    dlgblctrlblk
 *
 * Purpose: Global Control Block.  
 *
 * Members: dlpidcount  - keep track of DLPID given out
 *          dllogenablestate - global logging enable state
 *          dlusercbtbl - Array of user block pointers
 *          dllogbuf - Buffer for the complete formatted log string
 * 
 * Note:    Assumed dlusercbtbl to be zero initialized.
 *
 */
struct dlgblctrlblk 
{
    swi_uint16          dlpidcount;
    swi_bool            dllogenablestate;
    struct dlusercb     *dlusercbtbl[DLPID_MAX];
    struct dlusercb     dllogself;
    char                dllogbuf[DLLOGBUFFSZ];
};

/*
 * Name:     dllogqblk
 *
 * Purpose: Logging Queue Block.  
 *
 * Members:
 *          dlstringp   - Pointer to message string
 *          dlparam1    - Parameter 1 of message string
 *          dlparam2    - Parameter 2 of message string
 *          dlparam3    - Parameter 3 of message string
 *          dlparam4    - Parameter 4 of message string
 *          dlpid       - Package from which the message came from
 *
 * Note:    None.
 *
 */
struct dllogqblk 
{
    const char * dlstringp;
    swi_uint32 dlparam1;
    swi_uint32 dlparam2;
    swi_uint32 dlparam3;
    swi_uint32 dlparam4;
    swi_uint16 dlpid;
};

#include "dliproto.h"

#endif /* dlidefs */
