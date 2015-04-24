/*************
 *
 * Filename: slcommon.c 
 *
 * Purpose: This file contains String Library helper functions 
 *          common to slvsprintf.c, slsscanf.c and slmanage.c.
 *
 * Copyright: � 2008 Sierra Wireless, all rights reserved
 *
 **************/

/* include files */
#include "aa/aaglobal.h" /* Standard data types */
#include "slidefs.h"     /* Internal SL definitions/definitions */
#include "sludefs.h"     /* Public SL declarations/definitions */

/*
 *  Local constants and enumerated types.
 */

/*
 * local structures
 */

/*************
 *    
 * Name:    sldivision - Do division.
 *
 * Purpose: Divide the numerator by the denominator and compute 
 *          their remainder.     
 *             
 * Parms:   nump - pointer to the numerator
 *          den - denominator
 *
 * Return:  The remainder of the division operation.
 *
 * Abort:   None (No fatal errors)
 *
 * Notes:   This function modifies the orginal numerator.
 *
 **************/
package int sldivision(unsigned long *nump, unsigned den)
{
    int remainder;    

    remainder = *nump % den;
    *nump = *nump / den;
    
    return remainder;
}

/*************
 *    
 * Name:    slisdigit 
 *
 * Purpose: Determines whether an ASCII character is a decimal digit 
 *             
 * Parms:   digit - character to be evaluated
 *
 * Return:  YES if 'digit' is a ASCII decimal character. 
 *          NO otherwise.
 *
 * Abort:   None (No fatal errors)
 *
 * Notes:     
 *
 **************/
package swi_bool slisdigit(char digit)
{
    return ((digit >= '0') && (digit <= '9'));
}

/*************
 *    
 * Name:    slishex 
 *
 * Purpose: Determines whether an ASCII character is a hexidecimal
 *             
 * Parms:   hexdigit - character to be evaluated
 *
 * Return:  YES if 'hex' is a ASCII hexidecimal character. 
 *          NO otherwise.
 *
 * Abort:   None (No fatal errors)
 *
 * Notes:     
 *
 **************/
package swi_bool slishex(char hex)
{
    return (((hex >= '0') && (hex <= '9')) || 
            ((hex >= 'A') && (hex <= 'F')) || 
            ((hex >= 'a') && (hex <= 'f')));
}

/**********
 * Name:    slstrspn - string span
 *
 * Purpose: Finds the quantity of characters at the beginning of the 
 *          string str1p that match or do not match with any character 
 *          in the string str2p. 
 *
 * Parms:   str1p     - points to string 1 
 *          str2p     - points to string 2
 *          any_match - YES to find the quantity of characters at the 
 *                      beginning of the string str1p that match with 
 *                      any character in the string str2p.
 *                      NO to find the quantity of characters at the 
 *                      beginning of the string str1p that do not match 
 *                      with any characters in the str2p.
 *
 * Returns: Returns the number of characters at the beginning of the 
 *          string str1p that match or do not match with any character 
 *          in the string str2p. 
 *
 * Abort:   None
 *
 * Notes:   None
 * 
 **********/ 
package swi_uint32 slstrspn(const char *str1p, 
                            const char *str2p, 
                            swi_bool any_match)
{    
    /* length of the first sequence of characters found in string str1p 
     * that match or don't match with any char in string str2p 
     */
    swi_uint32 counter = 0;    
    int i;        /* loop counter */
    int j;        /* loop counter */
    swi_bool equal; /* true if a char from string str1p matches 
                     * with any of the char from string str2p 
                     */

    /* cycle through all the char in string str1p until NULL char is reached */
    for(i = 0; *(str1p + i) != 0; i++)
    {    
        /* assume char from string str1p does not 
         * match with any of the char from string str2p 
         */
        equal = NO;        
        
        /* Set equal to YES & stop cycling through the char in 
         * string str2p if any char from str2p match with the 
         * char from str1p 
         */
        for(j = 0; *(str2p + j) != 0; j++)
        {
              if (*(str1p + i) == *(str2p + j))
            {                     
                equal = YES;
                break;
            }            
        }
    
        /* stop cycling through the char in string str1p 
         * if the condition below is true
         */
        if ((any_match && !equal) || (!any_match && equal))
        {    
            break;
        }

        /* counter is incremented if a char from string str1p 
         * matches (if any_match = YES) or don't match(if 
         * any_match = NO) with any of the char from string str2p 
         */
        counter++;
    }

    return counter;
}

/*************
 *    
 * Name:    slva_end - Variable Argument End
 *
 * Purpose: Clean up when all the arguments have 
 *          been retrieved.
 *             
 * Parms:   argpp - pointer to argument pointer
 *
 * Return:  None
 *
 * Abort:   None (No fatal errors)
 *
 * Notes:   To be used with slvsprintf() and slsscanf() only.
 *          Users should only use slva_end to clean up the 
 *          variable argument list after calling slvsprintf().  
 *          slsscanf() calls this function internally.  Any 
 *          other usage can lead to unpredictable behavior.
 *          In addition, char * replaces va_list macro in 
 *          stdarg.h.
 *
 **************/
global void slva_end(char **argpp)
{
    /* argument pointer becomes a null pointer */
    *argpp = NULL;
}

/*************
 *    
 * Name:    slva_start - Variable Argument Start
 *
 * Purpose: Initialize the variable argument list.
 *             
 * Parms:   argpp - pointer to argument pointer
 *          fixpp - pointer to a fixed pointer argument 
 *                  preceeding the variable arguments
 *
 * Return:  None 
 *
 * Abort:   None (No fatal errors)
 *
 * Notes:   To be used with slvsprintf() and slsscanf() only.
 *          Users should only use slva_start to initialize the 
 *          variable argument list prior to passing 
 *          the argument pointer to slvsprintf().  
 *          slsscanf() calls this function internally.  Any 
 *          other usage can lead to unpredictable behavior.
 *          In addition, char * replaces va_list macro in stdarg.h.
 *
 **************/
global void slva_start(char **argpp, const char **fixpp)
{
    /* initialize argument list */
    *argpp = (char *)&(*fixpp) + sizeof(*fixpp);
}
