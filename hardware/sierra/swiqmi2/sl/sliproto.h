/*************
 *
 * Filename -sliproto.h  - SL internal prototype header file
 *    
 * Purpose - This file contains function prototypes for functions
 *           used internally by the string library package
 *
 * Copyright: © 2008 Sierra Wireless, all rights reserved
 *
 **************/

/* include files */

/*
 *  internal function prototypes.
 */
package int sldivision(unsigned long *nump, unsigned den);
package swi_bool slisdigit(char digit);
package swi_uint32 slstrspn(const char *str1p, 
                            const char *str2p, 
                            swi_bool any_match);
package swi_bool slishex(char hex);
