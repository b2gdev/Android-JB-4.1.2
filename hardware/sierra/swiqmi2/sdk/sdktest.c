/*
 *  Filename: sdktest.c
 *
 *  Purpose:  This file contains routine(s) that exercise the 
 *            SDK package
 *
 * Copyright: © 2008 Sierra Wireless, Inc., all rights reserved
 *
 */

/* Linux definitions */
#include <stdio.h>
#include <stdlib.h>

/* include files */
#include "aa/aaglobal.h"
#include "mm/mmudefs.h"

/**************
 *
 * Name:     main
 *
 * Purpose:  Entry point where testing of the SDK package starts. 
 *           
 * Parms:    argc   - Not used
 *           argv[] - Not used
 * Return:   0 - Normal termination
 *           1 - Problem with the execution
 *
 * Notes:    none
 *
 **************/
int sdkmain( int argc, char *argv[] )
{
    UNUSEDPARAM ( argc );
    UNUSEDPARAM ( argv );
    printf("Currently there is no MM package test\n");
    return 0;
}
