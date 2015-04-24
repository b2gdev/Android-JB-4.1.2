/*
 *  Filename: ertestapi.c
 *
 *  Purpose:  This file contains routine(s) that exercise the 
 *            ER package from the API side
 *
 * Copyright: © 2008 Sierra Wireless, Inc., all rights reserved
 *
 */

/* Linux definitions */
#include <stdio.h>

/* include files */
#include "aa/aaglobal.h"
#include "eridefs.h"

/**************
 *
 * Name:     main
 *
 * Purpose:  Entry point where testing of the ER package starts. 
 *           
 * Parms:    argc   - Not used
 *           argv[] - Not used
 * Return:   0 - Normal termination
 *           1 - Problem with the execution
 *
 * Notes:    none
 *
 **************/
int main( int argc, char *argv[] )
{
    printf("This test logs a message in /var/log/<logfile> from the api side\n");
    erAbort ("ER package test for extra long message longer than 80 characters that will be truncated", 0);
    return 0;
}
