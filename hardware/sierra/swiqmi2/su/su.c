/*************
 *
 * Filename:  su.c
 *
 * Purpose:   SDK Startup package. SDK package initializations and task
 *            creation synchronization.
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/

/*-------------
  include files
 --------------*/

#include "aa/aaglobal.h"
#include "qm/qmerrno.h"
#include "am/amudefs.h"
#include "ci/ciudefs.h"
#include "dl/dludefs.h"
#include "ds/dsudefs.h"
#include "er/erudefs.h"
#include "hd/hdudefs.h"
#include "ic/icudefs.h"
#include "im/imudefs.h"
#include "mm/mmudefs.h"
#include "pi/piudefs.h"
#include "ql/qludefs.h"
#include "qm/qmudefs.h"
#include "sdk/sdkudefs.h"
#include "sl/sludefs.h"
#include "sm/smudefs.h"
#include "us/usudefs.h"

/*-------
  Defines
 --------*/

/*-------------
  Local storage
 --------------*/
local struct dlusercb sudlcb;

/*---------
  Functions
 ----------*/

/*************
 *
 * Name:    sutaskinitcb
 *
 * Purpose: Task creation callback function used to enforce a deterministic
 *          task startup sequence
 *
 * Parms:   sdkpkg - the package for which the required task(s) has been created
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   every package for which a task is created must call this function
 *          to report that its task(s) is(are) running
 *
 **************/
global void sutaskinitcb(enum sdkpkg_e sdkpkg)
{
    dlLog(  &sudlcb,
            DLCLASSALL,
            "%d -> task started",
            (swi_uint32)sdkpkg,
            (swi_uint32)NULL,
            (swi_uint32)NULL,
            (swi_uint32)NULL );

    switch(sdkpkg)
    {
        case ICSDKPKG:
            qmtaskinit();
            break;
        case QMSDKPKG:
            amipctaskinit();
            break;
        case AMSDKPKG:
            ustaskinit();
            break;
        case USSDKPKG:
            break;
        default:
            break;
    }
}

/*************
 *
 * Name:    suinit
 *
 * Purpose: SU package initialization
 *
 * Parms:   none
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local void suinitlocal(void)
{
    dlregister("SU", &sudlcb, TRUE);
}

/*************
 *
 * Name:    suinit
 *
 * Purpose: Call all package initialization routines for unit
 *          testing on the desktop
 *
 * Parms:   none
 *
 * Return:  no value
 *
 * Abort:   none
 *
 * Notes:   To be called during SDK startup from the "main"
 *          entry point.
 *
 *          Initialization performed within each "xxinit()" call is
 *          limited to setting up internal structures within the
 *          package itself. It is not permitted that one package
 *          call into another to use its services during this phase
 *          of system initialization. However, it is permissible to
 *          make calls into the OS package for memory allocation,
 *          logging, etc. It is assumed that the OS Wrapper is fully
 *          initialized and ready to offer its services to outside
 *          callers.
 *
 *          This function is split into two parts, individual package
 *          initialization is first, followed by creation of tasks.
 *          Task creation should only be started after all package
 *          initialization routines have been called.
 *
 **************/
global void suinit( void )
{

    /************ Part 1 - Package Initialization ***************/
    /* first call the package initialization functions */
    dlinit();
    suinitlocal();
    mminit();
    icinitipc();
    ciinit();
    usinit();
    aminit();
    dsinit();
    qminit();
    iminit();
    hdinit(0);
    sminit();

    /* Log the startup with version info.
     */
    dlLog(  &sudlcb,
            DLCLASSALL,
            "Process Started, version: %s\n",
            (swi_uint32)amgettagname(),
            (swi_uint32)NULL,
            (swi_uint32)NULL,
            (swi_uint32)NULL);

    /***************** Part 2 - Task Creation *****************/

    /* next, kick off package task creation */
    ictttaskinit();
}
