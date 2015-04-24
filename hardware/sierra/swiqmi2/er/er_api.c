/*
 *  Filename: er_api.c
 *
 *  Purpose:  This file contains functions for ER package
 *            routines that are required for other 
 *            packages. 
 *
 * Copyright: © 2008 Sierra Wireless, Inc., all rights reserved
 *
 */

/* Linux definitions */
#include <stdio.h>
#include <stdlib.h>

/* Sierra Wireless Includes */
#include "aa/aaglobal.h"
#include "os/swi_osapi.h"
#include "sl/sludefs.h"
#include "eridefs.h"

/*************
 *    
 * Name: erstrlen
 *
 * Purpose: Returns the length of the string pointed to by sp.
 *             
 * Parms:   sp - pointer to string
 *
 * Return:  The number of characters that precede the terminating 
 *          null character 
 *
 * Abort:   None (No fatal errors)
 *
 * Notes:   This function is copied from slstrlen due to circular 
 *          dependencies between the ER and SL packages
 *          This should be the only place where we do this.
 *
 **************/
local swi_uint32 erstrlen(const char *sp)
{
    swi_uint32 length;  /* length of string not including the null character */

    /* initialize length */
    length = 0;

    /* cycle through string until a null character is detected */
    while(*sp++ != EOS)
        length++;

    return(length);
}


/*************
 *
 * Name:  erAbort
 *
 * Purpose: Function to log erAbort calls to the system
 *          log before terminating the caller's process
 *
 * Parms:   stringp - pointer to a string to display
 *          data    - Arbitrary value related to the error
 *
 * Return:  None
 *
 * Note:    The function should never return
 *          The formatted string will be truncated to ERMAXMSGLEN if
 *          this length is exceeded
 *
 */
global void erAbort( char *stringp, swi_uint32 data )
{
    char message[100];

    snprintf(message, sizeof(message),
             "erAbort: API Fatal error -> %s, Data: %d\n", 
             stringp, (int)data);

    /* Truncate the message to our fixed length if exceeded */
    if (erstrlen(message) > ERMAXMSGLEN)
        message[ERMAXMSGLEN] = '\0';
        
    swi_osapilog(message);
    exit(1);  /* Terminate the caller's process */
}
