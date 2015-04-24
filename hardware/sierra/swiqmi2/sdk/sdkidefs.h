/*************
 *
 * Filename: swi_sdkidefs.h
 *
 * Purpose:  Internal constants and data structures for the sdk package.
 *
 * Note:     None
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/

#include "aa/aaglobal.h"
#include "dl/dludefs.h"
#include "os/swi_ossdk.h"
#include "su/suudefs.h"

/*
 *  Local constants and enumerated types.
 */

#define SDK_CLASSA  0x0001  /* SDK Task logging mask A */

/*************
 *
 * Name:    swi_sdkcb
 *
 * Purpose: SDK package control block structure. This structure 
 *          contains what is necessary for the SDK task's runtime
 *          operation.
 *             
 * Members: sdkwaiter    - An OS Waiter structure that the SDK 
 *                         task uses to block itself after the 
 *                         initialization is complete
 *          sdkdlcb      - Data Logging package control block
 *
 * Notes:   None
 *
 **************/
struct swi_sdkcb
{
    struct swi_oswaiter sdkwaiter;
    struct dlusercb sdkdlcb;    
};
