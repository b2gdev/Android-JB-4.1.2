/*
 * Filename: dlpackage.c
 *
 * Purpose: Routines callable by other packages to
 *          log debug messages on the SDK.
 *
 * NOTES:
 *
 * Copyright Sierra Wireless Inc., 2008. All rights reserved.
 */


/* include files */

#include "aa/aaglobal.h"
#include "er/erudefs.h"
#include "sl/sludefs.h"
#include "os/swi_ossdk.h"
#include "dludefs.h"
#include "dlidefs.h"
#include <stdio.h>

/* Global Control Block */
local struct dlgblctrlblk dlgcb;

/* local variables */

struct dldebugtype
{
    swi_uint32 timestamp;
    char dllogbuf[DLLOGBUFFSZ];
};

/* Functions */
/*
 * Name:    dlgetgcbp
 *
 * Purpose: Package function to get a pointer to the package's global control
 *          block.
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
package struct dlgblctrlblk * dlgetgcbp(void)
{
    return (&dlgcb);
}

/*
 * Name:    dlusercbconfig
 *
 * Purpose: To configure a package's user block.
 *
 * Params:  dlusercbp  - Pointer to DL user block information.
 *          dlkeepmask - Boolean; TRUE - ignore the classmask passed in;
 *                       FALSE - store the classmask passed in.
 *
 * Return:  TRUE    - successful configuration
 *          FALSE   - package unregistered ?
 *
 * Note:    None.
 *
 * Abort:
 *
 */
global swi_bool dlusercbconfig(struct dlusercb * dlusercbp, swi_bool dlkeepmask)
{
    struct dlgblctrlblk * dlgcbp;
    swi_bool usercbpvalid = TRUE;

    /* Get pointer to global control block */
    dlgcbp = (struct dlgblctrlblk *) dlgetgcbp();

    /* Do not attempt to set unregistered packages */
    if (dlgcbp->dlusercbtbl[dlusercbp->dlpkgid])
    {
        if (!dlkeepmask)
        {
            /* Store the class mask passed in */
            dlgcbp->dlusercbtbl[dlusercbp->dlpkgid]->dlclassmask =
                                                        dlusercbp->dlclassmask;
        }
        dlgcbp->dlusercbtbl[dlusercbp->dlpkgid]->dlmasterenable =
                                                       dlusercbp->dlmasterenable;
    }
    else
    {
        /* DL can't log itself, return a FALSE so caller can log it */
        usercbpvalid = FALSE;
    }

    /* return TRUE always */
    return (usercbpvalid);
}

/*
 * Name:    dlpackidget
 *
 * Purpose: To check to see if the passed in string is a valid package id
 *          string.
 *
 * Params:  userpackidp - Pointer to a package id string host has entered.
 *
 * Return:  Package ID enumeration or the maximum package ID identifier if
 *          string did not match
 *
 * Note:    None.
 *
 * Abort:   None.
 *
 */
global swi_uint16 dlpackidget(char * userpackidp)
{
    struct dlgblctrlblk * dlgcbp = dlgetgcbp();
    swi_uint32 count;
    swi_bool found = FALSE;

    /* Validate the string pointer */
    for (count=0; (count < DLPID_MAX) && (found == FALSE); count++)
    {
        if (dlgcbp->dlusercbtbl[count])
        {
            if (slstrcmp(userpackidp,
                dlgcbp->dlusercbtbl[count]->dlpkgnamep) == 0)
            {
                found = TRUE;
                break;
            }
        }
    }
    if (found)
        return (count);

    return DLPID_MAX;
}

/*
 * Name:    dlpackidstrget
 *
 * Purpose: To return a pointer to the package string indexed by the
 *          package id enumeration.
 *
 * Params:  packid - package id enumeration.
 *
 * Return:  Pointer to a package id string.
 *          NULL if pointer is unused
 *
 * Note:    None.
 *
 * Abort:   None.
 *
 */
global char * dlpackidstrget(swi_uint16 packid)
{
    /* Get pointer to global control block */
    struct dlgblctrlblk * dlgcbp = dlgetgcbp();

    if (dlgcbp->dlusercbtbl[packid])
        return ((char *) &dlgcbp->dlusercbtbl[packid]->dlpkgnamep);

    return NULL;
}

/*
 * Name:    dllmasterenable
 *
 * Purpose: Change the master enable flag for the caller's dluser
 *          control block.
 *
 * Params:  dlcbkp - Pointer to the user's DL package control block
 *          state  - TRUE: enable logging; FALSE: disable logging
 *
 * Return:  None.
 *
 * Note:    None.
 *
 * Abort:   None.
 *
 */
global void dlmasterenable(struct dlusercb *dlcbkp, swi_bool state)
{
    /* Get pointer to global control block */
    struct dlgblctrlblk * dlgcbp = dlgetgcbp();

    dlgcbp->dlusercbtbl[dlcbkp->dlpkgid]->dlmasterenable = state;

}

/*
 * Name:    dllogenable
 *
 * Purpose: Enable/Disable debug logging.
 *
 * Params:  state - TRUE: enable logging; FALSE: disable logging
 *
 * Return:  None.
 *
 * Note:    None.
 *
 * Abort:   None.
 *
 */
global void dllogenable(swi_bool state)
{
    /* Get pointer to global control block */
    struct dlgblctrlblk * dlgcbp = dlgetgcbp();

    if (state)
    {
        /* Enable logging */
        dlgcbp->dllogenablestate = TRUE;
    }
    else
    {
        /* Disable logging */
        dlgcbp->dllogenablestate = FALSE;
    }
}

/*
 * Name:    dllogentoggle
 *
 * Purpose: Enable/Disable debug logging by toggling the bit.
 *
 * Params:  None.
 *
 * Return:  None.
 *
 * Note:    None.
 *
 * Abort:   None.
 *
 */
global void dllogentoggle(void)
{
    /* Get pointer to global control block */
    struct dlgblctrlblk * dlgcbp = dlgetgcbp();

    /* Toggle logging state */
    dlgcbp->dllogenablestate ^= TRUE;
}

 /*
 * Name:    dlregister
 *
 * Purpose: User entry point to register with DL to log messages.  Receive
 *          and store the pointer to the user block into the gcb.
 *
 * Params:  pkgnamep        - pointer to user provided package name string
 *          usercbp         - Pointer to user block
 *          masterenable    - initial state, TRUE = enabled, FALSE = disabled
 *
 * Return:  None
 *
 * Note:    None.
 *
 * Abort:   Package ID is assigned by the DL package and auto-incremented.
 *          If we run out due to maximum table size overflow for the user
 *          control block pointer array, we abort.
 *          If the user block pointer is aleready used, we abort too
 *
 */
global void dlregister(
    char * pkgnamep,
    struct dlusercb * usercbp,
    swi_bool masterenable)
{
    /* Get pointer to global control block */
    struct dlgblctrlblk * dlgcbp = dlgetgcbp();

    /* confirm this client is registering for the first time */
    swi_int32 i;
    for( i = 0 ;  i < DLPID_MAX ; i++ )
    {
        if( usercbp == dlgcbp->dlusercbtbl[i] )
        {
            /* client exists in our database */
            if( !usercbp->registered )
            {
                /* should never point to a client control block that hasn't
                 * been registered.
                 */
                char errmsg[100];
                snprintf(errmsg, sizeof(errmsg),
                         "%s:%d Client DL control block stored but not marked"\
                        " as registered!",
                         (char *)__func__, __LINE__);
                erAbort(errmsg, 0);
            }
            else
            {
                swi_char msg[128] =
                    "WARNING: Attempt to re-register a client: ";

                slmemcpy( &msg[slstrlen(msg)],
                          pkgnamep,
                          slstrlen(pkgnamep) + 1 );

                /* log an error */
                dlLog1(  &dlgcbp->dllogself,
                         DLCLASSALL,
                         msg,
                         (swi_uint32)DLPID_MAX );
                return;
            }
            break;
        }
    }

    if (dlgcbp->dlpidcount >= DLPID_MAX)
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d Invalid Package ID",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, (swi_uint32) dlgcbp->dlpidcount);
    }

    /* Store user block pointer into DL gcb */
    dlgcbp->dlusercbtbl[dlgcbp->dlpidcount] = usercbp;

    /* Initialize user block */
    usercbp->dlclassmask    = DLCLASSALL;
    usercbp->dlmasterenable = masterenable;
    usercbp->dlpkgid        = dlgcbp->dlpidcount++; /* autoincrement */
    usercbp->dlpkgnamep     = pkgnamep;
    usercbp->registered     = TRUE;

    return;
}

/*
 * Name:    dlprint
 *
 * Purpose: Print the complete formatted log string.
 *
 * Params:  strp - Pointer to the log string.
 *
 * Return:  None.
 *
 * Note:    None.
 *
 * Abort:   None.
 *
 */
local void dlprint(char * strp)
{

    /* Print out log */
    swi_ossdklog (strp);
}

/*
 * Name:    dllogprint
 *
 * Purpose: Print the log message with associated parameters.
 *
 * Params:  dllogqblkp - Pointer to the log queue block.
 *
 * Return:  None.
 *
 * Note:    None.
 *
 * Abort:   Buffer overflow.
 *
 */
package void dllogprint(struct dllogqblk * dllogqblkp)
{
    /* Get pointer to global control block */
    struct dlgblctrlblk * dlgcbp = dlgetgcbp();
    char * bufp;
    swi_uint32 strlen;

    bufp = dlgcbp->dllogbuf;

    /* Copy the package string the log is from. */
    strlen = slvsprintflenc(bufp,
                            "%s:",
                            (char *)(dlpackidstrget(dllogqblkp->dlpid)),
                            (DLLOGBUFFSZ- (bufp - dlgcbp->dllogbuf)),
                            1);

    /* Increment buffer pointer */
    bufp += strlen;

    /* Copy the log string. Just the address of the first parameter is
     * needed.  String length returned does not include the null terminator.
     * This call checks for the maximum number of arguments that can be used
     * in dlLog (DLLOGMAXARGS) and will abort if exceeded.
     */
    strlen = slvsprintflenc( bufp,
                            dllogqblkp->dlstringp,
                            (char *) &dllogqblkp->dlparam1,
                            (DLLOGBUFFSZ- (bufp - dlgcbp->dllogbuf)),
                            DLLOGMAXARGS);

    /* Increment buffer pointer */
    bufp += strlen;

    if ((bufp - dlgcbp->dllogbuf) > DLLOGBUFFSZ)
    {
        /* Buffer overflow. Should never get here as slvsprintflen should
         * ensure no buffer overflow. */
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d Log buffer overflow",
                 (char *)__func__, __LINE__);
        erAbort(errmsg,
                (swi_uint32) (bufp - dlgcbp->dllogbuf));
    }

    /* Print out log */
    dlprint(dlgcbp->dllogbuf);

    /* Clear the print buffer */
    slmemset (&(dlgcbp->dllogbuf[0]), 0, DLLOGBUFFSZ);
}

/*
 * Name:    dlinit
 *
 * Purpose: Initializes the DL package
 *
 * Params:
 *
 * Return:  None.
 *
 * Note:    None.
 *
 * Abort:   None.
 *
 */
global void dlinit()
{
    /* Get pointer to global control block */
    struct dlgblctrlblk * dlgcbp = dlgetgcbp();
    int i;

    dlgcbp->dlpidcount = 0;

    for (i=0; i < DLPID_MAX; i++)
    {
        dlgcbp->dlusercbtbl[i] = NULL;
    }

    /* Enable logging */
    dllogenable(TRUE);

    /* register ourselves for logging */
    dlregister("DL", &dlgcbp->dllogself, TRUE);
 }

/*
 * Name:    dlLog
 *
 * Purpose: User entry point to log messages.
 *
 *
 * Params:    usercbp   - pointer to user's DL control block
 *            class     - class (level) of debug information
 *            stringp   - pointer to debug message string
 *            param1    - parameter 1
 *            param2    - parameter 2
 *            param3    - parameter 3
 *            param4    - parameter 4
 *
 * Return:    None.
 *
 * Note:    None.
 *
 * Abort:    If the Package ID is invalid or has been defined already.
 *           If the string pointer is invalid.
 *           If the number of formatting specifiers exceed our limit of
 *           DLLOGMAXARGS.
 */
global void dlLog3(
    struct dlusercb *usercbp,
    swi_uint16      class,
    swi_char        *stringp,
    swi_uint32      param1,
    swi_uint32      param2,
    swi_uint32      param3 )
{
    dlLog( usercbp, class, stringp, param1, param2, param3, 0);
}

global void dlLog2(
    struct dlusercb *usercbp,
    swi_uint16      class,
    swi_char        *stringp,
    swi_uint32      param1,
    swi_uint32      param2 )
{
    dlLog( usercbp, class, stringp, param1, param2, 0, 0);
}

global void dlLog1(
    struct dlusercb *usercbp,
    swi_uint16      class,
    swi_char        *stringp,
    swi_uint32      param1 )
{
    dlLog( usercbp, class, stringp, param1, 0, 0, 0);
}

global void dlLog0(
    struct dlusercb *usercbp,
    swi_uint16      class,
    swi_char        *stringp )
{
    dlLog( usercbp, class, stringp, 0, 0, 0, 0);
}

global void dlLog(
    struct dlusercb * usercbp,
    swi_uint16 class,
    char * stringp,
    swi_uint32 param1,
    swi_uint32 param2,
    swi_uint32 param3,
    swi_uint32 param4
    )
{

    /* Get pointer to global control block */
    struct dlgblctrlblk * dlgcbp = dlgetgcbp();
    struct dllogqblk dllogqblock;
    swi_uint16 packageid = DLPID_MAX;

    if (!dlgcbp->dllogenablestate)
    {
        /* Logging is not enabled so nothing else needs to be done. */
        return;
    }

    packageid = usercbp->dlpkgid;

    /* Validate the package ID */
    if (packageid >= DLPID_MAX)
    {
        /* Package ID invalid */
        /* log a fatal error */
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d Invalid Package ID",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, (swi_uint32) packageid);
    }

    if ((dlgcbp->dlusercbtbl[packageid]->dlmasterenable)
        && (((dlgcbp->dlusercbtbl[packageid]->dlclassmask) & class)
        || (class == DLCLASSALL)))
    {
        /* Get all arguments and format them into a string */
        dllogqblock.dlpid = packageid;

        dllogqblock.dlstringp = stringp;

        dllogqblock.dlparam1 = param1;
        dllogqblock.dlparam2 = param2;
        dllogqblock.dlparam3 = param3;
        dllogqblock.dlparam4 = param4;

        /* Print out log */
        dllogprint(&dllogqblock);
    }
}
