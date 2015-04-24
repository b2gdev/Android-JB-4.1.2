/*************
 *
 * Filename: slvsprintf.c - Format output to a string.
 *
 * Purpose:  This file contains the slvsprintf() function for the
 *           String Library
 *
 * Copyright: � 2008 Sierra Wireless, Inc. all rights reserved
 *
 **************/

/* include files */

#include "aa/aaglobal.h"    /* Standard data types */
#include "er/erudefs.h"
#include "sl/slidefs.h"     /* Internal SL definitions/definitions */
#include "sl/sludefs.h"     /* Public SL declarations/definitions */
#include <stdio.h>
/*
 *  Local constants and enumerated types.
 */

/* These bitfield values below are used
 * by vsprintf() as part of the conversion type mask
 */
#define ZEROPAD 1               /* pad with zero */
#define SIGN    2               /* unsigned/signed long */
#define PLUS    4               /* show plus */
#define SPACE   8               /* space if plus */
#define LEFT    16              /* left justified */
#define SPECIAL 32              /* 0x or 0X */
#define LARGE   64              /* use 'ABCDEF' instead of 'abcdef' */

/*
 * local structures
 */

/*************
 *
 * Name:    slcountfmtargs
 *
 * Purpose: This function counts the number of format specifiers given a format
 *          string
 *
 * Parms:   sp       - pointer to format string
 *
 * Return:  numargs  - number of format specifiers
 *
 * Abort:   None
 *
 * Notes:   Internal/local users of slvsprintflen() must call this function
 *          first and verify it against their maximum number of arguments.
 *          All other must call the wrapper function slvsprintflenc() and
 *          provide the maximum number of arguments allowed.
 *
 **************/
global swi_uint32 slcountfmtargs(const char * sp)
{
    const char *scp    = sp; /* local pointer */
    swi_uint32 numargs = 0;

    /* Loop through the format string */
    while (*scp)
    {
        if (*scp == '%')
        {
            numargs++;
            scp++;
            /* If user wants to print % symbol */
            if (*scp == '%')
            {
                numargs--; /* do not count it */
                scp++;
            }
            continue;      /* skip the next increment */
        }
        scp++;             /* non % increment, point to nect character */
    }

    return numargs;
}

/*************
 *
 * Name: slstrnlen
 *
 * Purpose: Finds the length of not more than the number of characters
 *          in a string.
 *
 * Parms:   sp    - pointer to string
 *          count - number of characters to include in the string length
 *
 * Return:  'count' characters or all characters in a string up to but
 *          not including the null character if 'count' is larger than
 *          the total number of characters in the string.
 *
 * Abort:   None (No fatal errors)
 *
 * Notes:   None
 *
 **************/
global int slstrnlen(const char * sp, int count)
{
    char *scp;

    /* leave loop if end of string is reached or
     * if 'count' characters have been included
     * in the string length to be returned
     */
    for (scp = (char *)sp; count-- && *scp != (char)EOS; ++scp)
    {
    }

    /* returns the length */
    return scp - sp;
}

/*************
 *
 * Name: slnumberlen
 *
 * Purpose:   Output the formatted number argument into a string
 *            according to the precision, field width, base, type,
 *            and maximum string length.
 *
 * Parms:     strp - points to the string where formatted output
 *                   is received.
 *            num  - number argument from variable list
 *            base - base of the number - octal, decimal, hexidecimal
 *            size - field width to be printed
 *            precision - precision to be printed
 *            type - control the conversion type mask
 *            maxlen - maximum length of string where formatted output
 *                     is received.
 *
 * Return:    Pointer past the last formatted character in the string
 *            pointed to by 'strp'.
 *
 * Abort:     None (No fatal errors)
 *
 * Notes:     This function is used exclusively with slvsprintflen().
 *
 **************/
local char *slnumberlen (
    char *strp,
    long num,
    int base,
    int size,
    int precision,
    int type,
    swi_uint32 maxlen)
{
    char *origstrp;
    char pad;        /* to pad the width field with zeros or space */
    char sign;       /* sign of the number argument */
    char tmp[10];    /* temporary buffer for conversion of numbers to ASCII */
    const char *digitsp = "0123456789abcdefghijklmnopqrstuvwxyz";
    int i;           /* number of ASCII characters converted from numbers */

    origstrp = strp;
    /* value printed in hexadecimal using A, B, C, D, E ,F */
    if (type & LARGE)
        digitsp = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    /* overides 0 flag */
    if (type & LEFT)
        type &= ~ZEROPAD;

    if (base < 2 || base > 36)
        return NULL;

    /* select type of pad */
    if (type & ZEROPAD)
        pad = '0';
    else
        pad = ' ';

    /* default */
    sign = EOS;

    /* enter this loop if conversion specifier includes d or i*/
    if (type & SIGN)
    {
        if (num < 0)
        {
            sign = '-';
            num = -num;
            size--;
        }
        else if (type & PLUS)
        {
            sign = '+';
            size--;
        }
        else if (type & SPACE)
        {
            sign = ' ';
            size--;
        }
    }

    /* enter this loop if the flag contains '#' */
    if (type & SPECIAL)
    {
        if (base == 16)
            size -= 2;
        else if (base == 8)
            size--;
    }

    /* convert numbers into decimal numbers in ASCII representation */
    i = 0;
    if (num == 0)
        tmp[i++] = '0';
    else
        while (num != 0)
            tmp[i++] = digitsp[sldivision((unsigned long *)&num, base)];

    /* buffer overflow */
    if (i > 10)
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d Buffer overflow",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, 0);
    }

    /* default precision value: minimum number of digits to appear */
    if (i > precision)
        precision = i;

    /* reduce the field width to be padded with space or zeros */
    size -= precision;

    /* enter if statement to print in right justified format padded w/ space */
    if (!(type & (ZEROPAD + LEFT)))
        while(size-- > 0)
        {
            *strp++ = ' ';
            /* Validate the formatted string does not exceed the maximum
             * length.
             */
            if (strp == (origstrp+maxlen-1))
            {
                return (strp);
            }

        }

    if (sign)
        *strp++ = sign;

    /* Validate the formatted string does not exceed the maximum
     * length.
     */
    if (strp == (origstrp+maxlen-1))
    {
        return (strp);
    }

    if (type & SPECIAL)
    {
        /* make 1st digit a zero */
        if (base == 8)
        {
            *strp++ = '0';
            /* Validate the formatted string does not exceed the maximum
             * length.
             */
            if (strp == (origstrp+maxlen-1))
            {
                return (strp);
            }
        }
        /* prefix 0x or 0X*/
        else if (base == 16)
        {
            *strp++ = '0';
            /* Validate the formatted string does not exceed the maximum
             * length.
             */
            if (strp == (origstrp+maxlen-1))
            {
                return (strp);
            }
            *strp++ = digitsp[33];
            /* Validate the formatted string does not exceed the maximum
             * length.
             */
            if (strp == (origstrp+maxlen-1))
            {
                return (strp);
            }
        }
    }

    /* print in right justified format  w/ 'pads' */
    if (!(type & LEFT))
        while (size-- > 0)
        {
            *strp++ = pad;
            /* Validate the formatted string does not exceed the maximum
             * length.
             */
            if (strp == (origstrp+maxlen-1))
            {
                return (strp);
            }
        }

    while (i < precision--)
    {
        *strp++ = '0';
        /* Validate the formatted string does not exceed the maximum
         * length.
         */
        if (strp == (origstrp+maxlen-1))
        {
            return (strp);
        }
    }

    while (i-- > 0)
    {
        *strp++ = tmp[i];
        /* Validate the formatted string does not exceed the maximum
         * length.
         */
        if (strp == (origstrp+maxlen-1))
        {
            return (strp);
        }
    }

    /* print in left justified format with space */
    while (size-- > 0)
    {
        *strp++ = ' ';
        /* Validate the formatted string does not exceed the maximum
         * length.
         */
        if (strp == (origstrp+maxlen-1))
        {
            return (strp);
        }

    }

    return strp;
}


/*************
*
* Name:      slvsprintflen - Format output to a string of a given length
*
* Purpose:   Sends formatted output to a string using an
*            argument list.  A null char is appended to the
*            end of the string.  A length variable will limit
*            the amount of formatted output send to the string.
*
* Parms:     bufp   - points to the string where formatted output
*                     is received.
*            fmtp   - points to the format string with the format specifiers
*            args   - points to a variable list argument
*            maxlen - maximum length bufp can be received of formatted output
*                     including null char
*
* Return:    The number of characters written in the array, not counting
*            the terminating null character
*
* Abort:     Buffer Overflow
*
* Notes:     This function does not call slva_end().
*            Conversion specifiers supported are d, i, o, u, x, X, s, c, p, n
*            and %.
*            Modifiers supported are h and l.
*            The flags -, +, ' ', # and 0 are supported
*            Precision, width and negative values are supported.
*
**************/
local swi_uint32 slvsprintflen (
    char *bufp,
    const char *fmtp,
    char *argp,
    swi_uint32 maxlen)
{
    char *strp;            /* points at the location where the formatted
                            * character is written to
                            */
    const char *sp;        /* points to the string argument from variable list */

    int len;               /* number of characters of a string to be printed */
    int i;                 /* loop counter */
    int base;              /* base of the number */
    int flags;             /* controls the conversion */
    int field_width;       /* width of output field */
    int precision;         /* min. number of digits for integers; max
                            * number of chars for string
                            */
    int qualifier;         /* 'h', 'l' for integer fields */

    long *addresslongp;    /* generic pointer used to store address from
                            * variable argument
                            */
    int *addressintp;      /* generic pointer used to store address from
                            * variable argument
                            */
    unsigned long num;     /* number argument from variable list */
    unsigned long skipnum; /* used to point 'fmtp' 'skipnum' times pass a number
                            * specified in a conversion specifier
                            */

    for (strp = bufp ; *fmtp ; ++fmtp)
    {
        /* stores all characters into the string pointed to by 'bufp' until
         * the start of a conversion specifier beginning with a % character */
        if (*fmtp != '%')
        {
            *strp++ = *fmtp;
            /* Validate the formatted string does not exceed the maximum length
             * taking into account a Null character needs to be added as well.
             */
            if (strp == (maxlen+bufp-1))
            {
                /* Append Null character and return. */
                *strp = EOS;
                return (strp-bufp);
            }
            continue;
        }

        /* process flags */
        flags = 0;

        /* skips '%' */
        fmtp++;

        if (*fmtp == '-')
        {
            flags |= LEFT;
            fmtp++;
        }
        else if(*fmtp == '+')
        {
            flags |= PLUS;
            fmtp++;
        }
        else if(*fmtp == ' ')
        {
            flags |= SPACE;
            fmtp++;
        }
        else if(*fmtp == '#')
        {
            flags |= SPECIAL;
            fmtp++;
        }
        else if(*fmtp == '0')
        {
            flags |= ZEROPAD;
            fmtp++;
        }

        /* get field width */
        field_width = -1;

        if (*fmtp >= '0' && *fmtp <= '9')
        {
            field_width = slatoi(fmtp);

            /* points 'fmtp' at the character after the ascii number
             * representation
             */
            skipnum = field_width;
            while(skipnum)
            {
                sldivision(&skipnum, 10);
                fmtp++;
            }
        }
        else if (*fmtp == '*')
        {
            ++fmtp;

            /* it's the next argument */
            field_width = (*(int *)((argp += sizeof(int)) - sizeof(int)));
            /* make it left justified */
            if (field_width < 0)
            {
                field_width = -field_width;
                flags |= LEFT;
            }
        }

        /* get the precision */
        precision = -1;
        if (*fmtp == '.')
        {
            ++fmtp;
            if (*fmtp >= '0' && *fmtp <= '9')
            {
                precision = slatoi(fmtp);

                /* points 'fmtp' at the character after the ascii number
                 * representation
                 */
                skipnum = precision;
                while(skipnum)
                {
                    sldivision(&skipnum, 10);
                    fmtp++;
                }
            }
            else if (*fmtp == '*')
            {
                ++fmtp;

                /* it's the next argument */
                precision = (*(int *)((argp += sizeof(int)) - sizeof(int)));
            }

            if (precision < 0)
                precision = 0;
        }

        /* get the conversion qualifier */
        qualifier = -1;
        if (*fmtp == 'h' || *fmtp == 'l')
        {
            qualifier = *fmtp;
            ++fmtp;
        }

        /* default base */
        base = 10;

        /* get the conversion specifier */
        switch (*fmtp)
        {
            case 'c':
                /* right-justified format */
                if (!(flags & LEFT))
                    while (--field_width > 0)
                    {
                        *strp++ = ' ';
                        /* Validate the formatted string does not exceed
                         * the maximum length taking into account a Null
                         * character needs to be added as well.
                         */
                        if (strp == (maxlen+bufp-1))
                        {
                            /* Append Null character and return. */
                            *strp = EOS;
                            return (strp-bufp);
                        }
                    }

                /* get character argument */
                *strp++ = (unsigned char)(*(int *)
                           ((argp += sizeof(int)) - sizeof(int)));
                /* Validate the formatted string does not exceed
                 * the maximum length taking into account a Null
                 * character needs to be added as well.
                 */
                if (strp == (maxlen+bufp-1))
                {
                    /* Append Null character and return. */
                    *strp = EOS;
                    return (strp-bufp);
                }

                /* left-justified format */
                while (--field_width > 0)
                {
                    *strp++ = ' ';
                    /* Validate the formatted string does not exceed
                     * the maximum length taking into account a Null
                     * character needs to be added as well.
                     */
                    if (strp == (maxlen+bufp-1))
                    {
                        /* Append Null character and return. */
                        *strp = EOS;
                        return (strp-bufp);
                    }
                }
                continue;

            case 's':
                /* get string argument */
                sp = (*(char **)((argp += sizeof(char *)) - sizeof(char *)));

                if (!sp)
                    sp = "<NULL>";

                /* get the number of characters of a string to be printed */
                len = slstrnlen(sp, precision);

                /* right-justified format */
                if (!(flags & LEFT))
                    while (len < field_width--)
                    {
                        *strp++ = ' ';
                        /* Validate the formatted string does not exceed
                         * the maximum length taking into account a Null
                         * character needs to be added as well.
                         */
                        if (strp == (maxlen+bufp-1))
                        {
                            /* Append Null character and return. */
                            *strp = EOS;
                            return (strp-bufp);
                        }
                    }

                /* print formatted characters to string pointed to by 'bufp' */
                for (i = 0; i < len; ++i)
                {
                    *strp++ = *sp++;
                    /* Validate the formatted string does not exceed
                     * the maximum length taking into account a Null
                     * character needs to be added as well.
                     */
                    if (strp == (maxlen+bufp-1))
                    {
                        /* Append Null character and return. */
                        *strp = EOS;
                        return (strp-bufp);
                    }
                }

                /* left-justified format */
                while (len < field_width--)
                {
                    *strp++ = ' ';
                    /* Validate the formatted string does not exceed
                     * the maximum length taking into account a Null
                     * character needs to be added as well.
                     */
                    if (strp == (maxlen+bufp-1))
                    {
                        /* Append Null character and return. */
                        *strp = EOS;
                        return (strp-bufp);
                    }
                }

                continue;

            case 'p':
                /* enter if statement if field width not specified in the
                 * conversion specifier
                 */
                if (field_width == -1)
                {
                    field_width = 2 * sizeof(void *);
                    flags |= ZEROPAD;
                }

                /* strp receives the value of a formatted pointer argument */
                strp = slnumberlen(strp,
                (unsigned long)(*(void **)
                                 ((argp += sizeof(void *)) - sizeof(void *))),
                16, field_width, precision, flags, (maxlen+bufp-strp));

                /* Validate the formatted string does not exceed
                 * the maximum length taking into account a Null
                 * character needs to be added as well.
                 */
                if (strp == (maxlen+bufp-1))
                {
                    /* Append Null character and return. */
                    *strp = EOS;
                    return (strp-bufp);
                }

                continue;

            case 'n':
                /* specifies that the pointer points to a long int */
                if (qualifier == 'l')
                {
                    /* points 'addressp' at the object pointed to by the
                     * pointer argument
                     */
                    addresslongp = (*(long **)
                                    ((argp += sizeof(long *)) - sizeof(long *)));

                    /* number of characters printed thus far */
                    *addresslongp = (strp - bufp);
                }
                /* pointer points to an int */
                else
                {
                    /* points ip at the object pointed by the pointer argument */
                    addressintp = (*(int **)
                                    ((argp += sizeof(int *)) - sizeof(int *)));

                    /* number of characters printed thus far */
                    *addressintp = (strp - bufp);
                }

                continue;

            /* integer number formats - set up the flags and "break" */
            /* octal */
            case 'o':
                base = 8;
                break;

            /* large hexidecimal */
            case 'X':
                flags |= LARGE;

            /* hexidecimal */
            case 'x':
                base = 16;
                break;

            /* signed int */
            case 'd':
            case 'i':
                flags |= SIGN;

            /* unsigned int */
            case 'u':
                break;

            default:
                /* prints out the incorrect conversion specifiers except
                 * for %% where only % is printed
                 */
                if (*fmtp != '%')
                {
                    *strp++ = '%';
                    /* Validate the formatted string does not exceed
                     * the maximum length taking into account a Null
                     * character needs to be added as well.
                     */
                    if (strp == (maxlen+bufp-1))
                    {
                        /* Append Null character and return. */
                        *strp = EOS;
                        return (strp-bufp);
                    }

                }

                if (*fmtp)
                {
                    *strp++ = *fmtp;
                    /* Validate the formatted string does not exceed
                     * the maximum length taking into account a Null
                     * character needs to be added as well.
                     */
                    if (strp == (maxlen+bufp-1))
                    {
                        /* Append Null character and return. */
                        *strp = EOS;
                        return (strp-bufp);
                    }

                }
                else
                    --fmtp;

                continue;
        }

        /* value is converted to a long or unsigned long int */
        if (qualifier == 'l')
        {
            num = (*(unsigned long *)
                     ((argp += sizeof(unsigned long)) - sizeof(unsigned long)));
            if (flags & SIGN)
                num = (long)num;
        }

        /* value is converted to a short int or unsigned short int */
        else if (qualifier == 'h')
        {
            num = (unsigned short)(*(int *)((argp += sizeof(int)) - sizeof(int)));
            if (flags & SIGN)
                num = (short)num;
        }
        else if (flags & SIGN)
            num = (*(int *)((argp += sizeof(int)) - sizeof(int)));
        else
            num = (*(unsigned int *)
            ((argp += sizeof(unsigned int)) - sizeof(unsigned int)));

        /* formatted number argument is output into the string pointed to
         * by 'strp'. 'strp' points to the last formatted character in that
         * string after calling slnumber()
         */
        strp = slnumberlen(strp, num, base, field_width, precision, flags,
                             (maxlen+bufp-strp));

        /* Validate the formatted string does not exceed
         * the maximum length taking into account a Null
         * character needs to be added as well.
         */
        if (strp == (maxlen+bufp-1))
        {
            /* Append Null character and return. */
            *strp = EOS;
            return (strp-bufp);
        }
    }

    /* appends a null character to the string */
    *strp = EOS;

    /* The number of characters printed to the string, not counting
     * the terminating null character
     */
    return strp - bufp;
}

/*************
*
* Name:      slvsprintflenc - Format output to a string of a given length
*
* Purpose:   This is a wrapper function to slvsprintflen() to provide an
*            additional parameter for the maximum number of arguments.
*
* Parms:     bufp   - points to the string where formatted output
*                     is received.
*            fmtp   - points to the format string with the format specifiers
*            args   - points to a variable list argument
*            maxlen - maximum length bufp can be received of formatted output
*                     including null char
*            maxargs- maximum number of arguments allowed by caller
*
* Return:    The number of characters written in the array, not counting
*            the terminating null character
*
* Abort:     Number of format specifiers greater than the number of
*            maximum arguments
*
* Notes:     This calls the newly created slcountfmtargs() to isolate the caller
*            from having to call that function.
*
**************/
global swi_uint32 slvsprintflenc (
    char *bufp,
    const char *fmtp,
    char *argp,
    swi_uint32 maxlen,
    swi_uint32 maxargs)
{
    swi_uint32 numargs;

    /* Get the number of arguments specified in the format string */
    numargs = slcountfmtargs (fmtp);

    /*Abort if it exceeds given maximum */
    if (numargs > maxargs)
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d too many format specifiers",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, numargs);
    }

    /* Number of arguments OK, do the actual formatting */
    return (slvsprintflen (bufp, fmtp, argp, maxlen ));
}
