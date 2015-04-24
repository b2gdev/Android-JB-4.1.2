/*************
 *
 * Filename:    slmanage.c - String Library Manager
 *
 * Purpose:     This is the main file for the string library.
 *
 * Copyright:   © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/

/* include files */
#include "aa/aaglobal.h"
#include "os/swi_osapi.h"
#include "slidefs.h"
#include "sludefs.h"

/* Local constants and enumerated types */
#define SLUINT32MAX 0xFFFFFFFF

/* This constant is bitwise AND with an address
 * to determine whether the address is word
 * aligned or divisible by four
 */
#define SLALIGNED (swi_uint32)3

/* local structures */

/* functions */
/*************
 *
 * Name:    slahtol
 *
 * Purpose: Converts the initial part of the string
 *          pointed to by "sp" from ASCII hex to swi_uint32.
 *          This function performs an equivalent duty
 *          as slatol() except that slahtol() is for
 *          converting ASCII hexadecimal strings rather
 *          than for converting ASCII decimal strings.
 *
 * Parms:   sp - pointer to string to convert
 *
 * Return:  A swi_uint32 conversion of the parameter "sp"
 *
 * Abort:   None (No fatal errors)
 *
 * Notes:   If sp does not contain a valid hexadecimal number,
 *          zero is returned. If the result cannot be represented
 *          as a swi_uint32 due to overflow problems, SLUINT32MAX is
 *          returned.  Non-hex input (e.g. "G") is ignored (no
 *          error is returned) if this does not occur at where sp
 *          is pointed to.
 *
 **************/
global swi_uint32 slahtol(const char *sp)
{
    char *slastp;  /* ptr to last valid hex rep'n in the string to convert */
    char *sfirstp; /* ptr to the 1st valid hex rep'n in the string to convert */
    int i;         /* loop counter */
    swi_uint32 result; /* converted swi_uint32 value from the string */

    /* initialize result */
    result = 0;

    sfirstp = (char *)sp;

    /* removes any whitespace pointed by sp */
    while(*sfirstp == ' ')
        sfirstp++;

    /* points slastp at the first possible
     * valid hex representation in
     * the string
     */
    slastp = sfirstp;

    /* points slastp past the last valid hex representation in the string */
    while(slishex(*slastp))
        slastp++;

    /* return a zero if no valid hex number exist */
    if (slastp == sfirstp)
    {
        return(0);
    }

    /* return SLUINT32MAX if the swi_uint32 data type
     * cannot represent the result without overflowing.
     * Each byte is equivalent to 2 hex
     */
    else if ( ( (swi_uint32)(slastp - sfirstp) ) > ( sizeof(swi_uint32) * 2 ) )
    {
        return(SLUINT32MAX);
    }

    /* points slastp at the last valid hex
     * representation in the string
     */
    slastp--;

    /* This process is repeated until slastp and
     * sfirstp points to the same char and the result
     * is equal the long representation in the string.
     */
    for(i = 0; slastp >= sfirstp; i++)
    {
        /* convert the character into a hex value and
         * multiply the hex with its hex placeholder
         */
        if (slisdigit(*slastp))
            result += ((*slastp - '0') << (4 * i));
        else if ((*slastp >= 'A') && (*slastp <= 'F'))
            result += ((*slastp - 'A' + 10) << (4 * i));
        else
            result += ((*slastp - 'a' + 10) << (4 * i));

        /* slastp is decremented so that it points to the
         * preceding valid hex character.
         */
        slastp--;
    }

    return (result);

}

/*************
 *
 * Name:    slatol64
 *
 * Purpose: Converts the initial part of the string pointed to by
 *          "sp" to swi_uint64
 *
 * Parms:   sp - pointer to string to convert
 *
 * Return:  A swi_uint64 conversion of the parameter "sp"
 *
 * Abort:   None (No fatal errors)
 *
 * Notes:   If a string does not contain a valid integer number,
 *         a zero value is returned. Return is unspecified if the swi_uint64
 *          data type cannot represent the result without overflowing.
 *
 **************/
global swi_uint64 slatol64(const char *sp)
{
    char *slastp;  /* ptr to the last valid int rep'n in string to convert */
    char *sfirstp; /* ptr to the 1st valid int rep'n in string to convert */
    int i;         /* loop counter */
    swi_uint64 result; /* converted swi_uint32 value from the string */
    swi_uint64 placeholder;  /* decimal placeholder */

    result = 0;
    placeholder = 1;

    sfirstp = (char *)sp;

    /* removes any whitespace pointed by sp */
    while(*sfirstp == ' ')
        sfirstp++;

    /* points slastp at the first possible valid
     * decimal representation in the string
     */
    slastp = sfirstp;

    /* points slastp past the last valid integer
     * representation in the string
     */
    while(slisdigit(*slastp))
        slastp++;

    /* return a zero if no valid decimal number exist */
    if (slastp == sfirstp)
    {
        return(0);
    }

    /* points slastp at the last valid integer representation
     * in the string
     */
    slastp--;

    /* This process is repeated until slastp and sfirstp
     * point to the same char, and result is equal the
     * integer representation in the string
     */
    for(i = 0; slastp >= sfirstp; i++)
    {
        /* convert the character into an integer
         * value and multiply the integer with its
         * decimal placeholder
         */
        result += ((*slastp - '0') * placeholder);

        /* equivalent to 10 raise to the power of i */
        placeholder *= 10;

        /* slastp is decremented so that it points to
         * the preceding valid integer character
         */
        slastp--;
    }

    return (result);
}

/*************
 *
 * Name:    slatol
 *
 * Purpose: Converts the initial part of the string pointed to by
 *          "sp" to swi_uint32
 *
 * Parms:   sp - pointer to string to convert
 *
 * Return:  A swi_uint32 conversion of the parameter "sp"
 *
 * Abort:   None (No fatal errors)
 *
 * Notes:   If a string does not contain a valid integer number,
 *          a zero value is returned.
 *
 **************/
global swi_uint32 slatol(const char *sp)
{
    char *slastp;  /* ptr to the last valid int rep'n in string to convert */
    char *sfirstp; /* ptr to the 1st valid int rep'n in string to convert */
    int i;         /* loop counter */
    swi_uint32 result; /* converted swi_uint32 value from the string */
    swi_uint32 placeholder;  /* decimal placeholder */

    result = 0;
    placeholder = 1;

    sfirstp = (char *)sp;

    /* removes any whitespace pointed by sp */
    while(*sfirstp == ' ')
        sfirstp++;

    /* points slastp at the first possible valid
     * decimal representation in the string
     */
    slastp = sfirstp;

    /* points slastp past the last valid integer
     * representation in the string
     */
    while(slisdigit(*slastp))
        slastp++;

    /* return a zero if no valid decimal number exist */
    if (slastp == sfirstp)
    {
        return(0);
    }

    /* points slastp at the last valid integer representation
     * in the string
     */
    slastp--;

    /* This process is repeated until slastp and sfirstp
     * point to the same char, and result is equal the
     * integer representation in the string
     */
    for(i = 0; slastp >= sfirstp; i++)
    {
        /* convert the character into an integer
         * value and multiply the integer with its
         * decimal placeholder
         */
        result += ((*slastp - '0') * placeholder);

        /* equivalent to 10 raise to the power of i */
        placeholder *= 10;

        /* slastp is decremented so that it points to
         * the preceding valid integer character
         */
        slastp--;
    }

    return (result);
}

/*************
 *
 * Name:    slatoi - ASCII to Integer
 *
 * Purpose: Converts the initial part of the string
 *          pointed to by "sp" to swi_uint16
 *
 * Parms:   sp - pointer to string to convert
 *
 * Return:  A swi_uint16 conversion of the parameter "sp"
 *
 * Abort:   None (No fatal errors)
 *
 * Notes:   If a string does not contain a valid integer
 *          number, the returned value will be 0.
 *
 **************/
global swi_uint16 slatoi(const char *sp)
{
    /* converts the swi_uint32 result from the ASCII to
     * long, slatol(), function to a swi_uint16 data type
     */
    return (swi_uint16)slatol(sp);
}

/*************
 *
 * Name:    slmemcmp
 *
 * Purpose: Compares the first "length" bytes of the
 *          memory pointed to by s1p to the first
 *          "length" characters of the object pointed
 *          to by s2p
 *
 * Parms:   s1p     - pointer to first memory location
 *                    to compare
 *          s2p     - pointer to second memory location
 *                    to compare
 *          length  - maximum quantity of bytes to
 *                    compare
 *
 * Return:  An int greater than, equal to, or less
 *          than zero, according as the memory pointed
 *          to by s1p is greater than, equal to, or
 *          less than the memory pointed to by s2p
 *
 * Abort:   None (No fatal errors)
 *
 * Notes:   A zero value is returned iF the "length" is zero.
 *
 **************/
global int slmemcmp(
    const void *s1p,
    const void *s2p,
    swi_uint32 length )
{
    int result;        /* result of comparison*/
    swi_uint8 *s1cpyp;    /* copy of s1p */
    swi_uint8 *s2cpyp;    /* copy of s2p */

    s1cpyp = (swi_uint8 *)s1p;
    s2cpyp = (swi_uint8 *)s2p;

    /* stop comparison after "length" bytes */
    while(length--)
    {
        /* compare bytes then increment pointers to point to next byte */
        result = *s1cpyp++ - *s2cpyp++;

        /* return result if the object pointed by s1p is greater
         * or less than the object pointed by s2p
         */
        if (result)
            return result;
    }

    /* the object pointed by s1p is equal to the object pointed by s2p */
    return 0;
}

/*************
 *
 * Name: slmemcpy
 *
 * Purpose: Copies "length" characters from the object
 *          pointed to by srcp into the object pointed
 *          to by destp.
 *
 * Parms:   destp - pointer to location of memory
 *                  to which memory will be copied
 *           srcp - pointer to location of memory
 *                  from where memory will be copied
 *         length - quantity of bytes to copy
 *
 * Return: The value of destp
 *
 * Abort:  None (No fatal errors)
 *
 * Notes:  Unlike C libraries implementation of memcpy(), if
 *         copying takes place between objects that overlap, the
 *         behaviour is defined.
 *
 **************/
global void *slmemcpy(
    void *destp,
    void *srcp,
    swi_uint32 length )
{
    swi_uint32 uint32size;  /* number of bytes of a uint32 data type */
    swi_uint32 *dest32p;    /* points to destp as a 32 bit block */
    swi_uint32 *src32p;     /* points to srcp as a 32 bit block */
    swi_uint8 *dest8p;      /* points to destp as a 8 bit block */
    swi_uint8 *src8p;       /* points to srcp as a 8 bit block */
    swi_uint32 remainder;   /* # of bytes that can't be xferrd in swi_uint32 format */
    swi_uint32 numword;     /* number of words */

    dest8p = (swi_uint8 *)destp;
    src8p = (swi_uint8 *)srcp;

    /* memory bytes copy only if any of the address
     * is not word aligned, where bits[1:0] != 0b00
     */
    if (((swi_uint32)dest8p & SLALIGNED) || ((swi_uint32)src8p & SLALIGNED))
    {
        /* copy bytes from the object pointed to by
         * src8p starting from the most significant address
         * to prevent the object pointed to by src8p from
         * corrupting if objects overlap
         */
        if (src8p < dest8p)
        {
            /* points src8p "length" bytes past srcp */
            src8p += (length - 1);

            /* points dest8p "length" bytes past destp */
            dest8p += (length - 1);

            /* copy byte by byte from msb to lsb */
            while(length--)
                *dest8p-- = *src8p--;
        }
        /* copy bytes from the object pointed to by
         * src8p starting from the least significant
         * address to prevent the object pointed to by
         * src8p from corrupting if objects overlap
         */
        else
        {
            /* copy byte by byte from lsb to msb */
            while(length--)
                *dest8p++ = *src8p++;
        }
    }
    else
    {
        uint32size = sizeof(swi_uint32);   /* # of bytes of a swi_uint32 data type */
        numword = length / uint32size; /* # of words */
        remainder = length % uint32size; /* # of bytes that can't be xferred in
                                          * swi_uint32 format
                                          */

        /* point dest32p & src32p at the start
         * of objects to be copied to and from
         */
        dest32p = (swi_uint32 *)destp;
        src32p = (swi_uint32 *)srcp;

        /* copy words from the object pointed to by
         * src32p starting from the most significant
         * address to prevent the object pointed to by
         * src32p from corrupting if objects overlap
         */
        if (src32p < dest32p)
        {
            /* points dest8p 'length' bytes past destp */
            dest8p += (length - 1);

            /* points src8p 'length' bytes past srcp */
            src8p += (length - 1);

            /* copy byte by byte from msb to lsb */
            while(remainder--)
                *dest8p-- = *src8p--;

            /* points src32p 'numwords' past srcp */
            src32p += (numword - 1);

            /* points dest32p 'numwords' past destp */
            dest32p += (numword - 1);

            /* copy word by word from remaining msw to lsw */
            while(numword--)
                *dest32p-- = *src32p--;
        }
        /* copy bytes from the object pointed to by src32p
         * starting from the least significant address to
         * prevent the object pointed to by src32p from
         * corrupting if objects overlap
         */
        else
        {
            /* copy word by word from lsw to msw */
            while(numword--)
            {
                *dest32p++ = *src32p;
                src32p++; /* do this separately -  MP750 compiler bug */
            }

            /* points dest8p at the remaining bytes to be copied to */
            dest8p = (swi_uint8 *)dest32p;

            /* points src8p at the remaining bytes to be copied from */
            src8p = (swi_uint8 *)src32p;

            /* copy byte by byte from remaining lsb to msb */
            while(remainder--)
                *dest8p++ = *src8p++;
        }
    }

    return destp;
}

/*************
 *
 * Name:    slmemset
 *
 * Purpose: This function fills memory with a particular data value
 *
 * Parms:   srcp    - pointer to location of memory to
 *                    begin fill operation
 *          value   - byte value to write to memory
 *          length  - quantity of bytes to fill
 *
 * Return:  None
 *
 * Abort:   None (No fatal errors)
 *
 * Notes:   None
 *
 **************/
global void slmemset(
    char *srcp,
    swi_uint8 value,
    swi_uint32 length )
{
    while (length--)
    {
           *srcp++ = value;
    }
}


/*************
 *
 * Name: slstrcmp
 *
 * Purpose: Compares the string pointed to by s1p
 *          to the string pointed to by s2p.
 *
 * Parms:   s1p - pointer to first string to compare
 *          s2p - pointer to second string to compare
 *
 * Return:  An int greater than, equal to, or less
 *          than zero, according as the string pointed
 *          to by s1p is greater than, equal to, or
 *          less than the string pointed to by s2p
 *
 * Abort:   None (No fatal errors)
 *
 * Notes:   None
 *
 **************/
global int slstrcmp(
    const char *s1p,
    const char *s2p )
{
    int result;        /* result of comparison*/
    char *s1cpyp;    /* copy of s1p */
    char *s2cpyp;    /* copy of s2p */

    s1cpyp = (char *)s1p;
    s2cpyp = (char *)s2p;

    /* comparison before null character(s) */
    while(*s1cpyp != EOS && *s2cpyp != EOS)
    {
        result = *s1cpyp++ - *s2cpyp++;

        /* return result if the string pointed
         * by s1p is greater or less than the
         * string pointed by s2p
         */
        if (result)
            return result;
    }

    /* comparison at null character(s) */
    result = *s1cpyp - *s2cpyp;

    return result;
}

/*************
 *
 * Name: slstrlen
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
 * Notes:   None
 *
 **************/
global swi_uint32 slstrlen(const char *sp)
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
 * Name: slstrncmp
 *
 * Purpose: Compares not more than "length" characters
 *          (characters that follow a null character
 *          are not compared) from the string pointed
 *          to by s1p to the string    pointed to by s2p.
 *
 * Parms:   s1p    - pointer to first string to
 *                   compare
 *          s2p    - pointer to second string to
 *                   compare
 *          length - maximum quantity of characters
 *                   to compare
 *
 * Return: An int greater than, equal to, or less
 *         than zero, according as the string pointed
 *         to by s1p is greater than, equal to, or
 *         less than the string pointed to by s2p
 *
 * Abort:  None (No fatal errors)
 *
 * Notes:  None
 *
 **************/
global int slstrncmp(
    const char *s1p,
    const char *s2p,
    swi_uint32 length )
{
    int result;        /* result of comparison*/
    char *s1cpyp;    /* copy of s1p */
    char *s2cpyp;    /* copy of s2p */

    /* initially assume strings are equal */
    result = 0;

    s1cpyp = (char *)s1p;
    s2cpyp = (char *)s2p;

    /* stop comparison after "length" characters
     * or before the null character(s)
     */
    while(length && (*s1cpyp != EOS && *s2cpyp != EOS))
    {
        result = *s1cpyp++ - *s2cpyp++;

        /* return result if the string pointed by
         * s1cpyp is greater or less than the string
         * pointed by s2cpyp
         */
        if (result > 0  || result < 0)
            return result;

        /* decrement number of characters to compare */
        length--;
    }

    /* comparison at null character(s)
     * if "length" characters not compared yet
     */
    if (length)
        result = *s1cpyp - *s2cpyp;

    return result;
}

/*************
 *
 * Name:  slstrncpy
 *
 * Purpose: Copies not more than "length" characters
 *          (characters that follow a null character
 *          are not copied) from the array pointed to
 *          by srcp into the array pointed to by destp.
 *          If copying takes place between objects
 *          that overlap, the behaviour is undefined.
 *
 * Parms:   destp  - pointer to location of memory
 *                   to where string will be copied
 *          srcp   - pointer to location of memory
 *                   from where string will be copied
 *          length - maximum characters to copy
 *
 * Return:  The value of destp
 *
 * Abort:   None (No fatal errors)
 *
 * Notes:   If the string pointed to by srcp is longer than
 *          length characters, the resultant array pointed to
 *          by destp will not be null terminated.
 *          If the string pointed to by srcp is shorter than
 *          length characters, null characters are continually
 *          copied to the string pointed by destp until "length"
 *          characters have been copied.
 *
 **************/
global char *slstrncpy(
    char        *destp,
    const char  *srcp,
    swi_uint32  length )
{
    char *destcpyp;        /* copy of destp */
    char *srccpyp;        /* copy of srcp */

    destcpyp = destp;
    srccpyp = (char *)srcp;

    /* copy characters in the array pointed by
     * srcp into the array pointed to by destp
     */
    while(length && (*srccpyp != EOS))
    {
        *destcpyp++ = *srccpyp++;
        length--;
    }

    /* null characters are continually copied to
     * the string pointed by destp until "length"
     * characters have been copied.
     */
    while(length--)
        *destcpyp++ = EOS;

    return destp;
}

/*************
 *
 * Name: sltoupper
 *
 * Purpose: If the passed-in character is a lower- case character then
 *          return the corresponding upper-case letter. Otherwise, return
 *          the passed-in character unchanged.
 *
 * Parms:   c - lower-case character to convert to
 *              upper-case
 *
 * Return:  Upper-case converted character
 *
 * Abort:   None (No fatal errors)
 *
 * Notes:   None
 *
 **************/
global char sltoupper(char c)
{
    char retval;    /* return value */

    /* initialize return value */
    retval = c;

    /* If we have a lower case character */
    if (c >= 'a' && c <= 'z')
    {
        /* Convert lower case character to upper case */
        retval = c - 'a' + 'A';
    }

    return (retval);
}

/*************
 *
 * Name: slisprint
 *
 * Purpose: Checks if the character is a printable ASCII character.
 *
 * Parms:   c - ASCII character
 *
 * Return:  TRUE  - if printable
 *          FALSE - otherwise
 *
 * Abort:     None (No fatal errors)
 *
 * Notes:     None
 *
 **************/
global swi_bool slisprint(char c)
{
    /* check if character is printable */
    if (c >= 0x20 && c <= 0x7e)
    {
        return (TRUE);
    }

    return (FALSE);
}

/**********
 * Name:    slstrtok - string tokenizer
 *
 * Purpose: Breaks a string into a series of tokens
 *
 * Parms:   tokp     - points to string to be tokenized
 *          delimitp - points to delimiter
 *          tokblkp  - points to user defined storage for
 *                     a particular slstrtok() instance
 *
 * Returns: pointer to the first character of a token, or a null
 *          pointer if there is no token.
 *
 * Abort:   None
 *
 * Notes:   If tokp == NULL, slstrtok() continues tokenizing the string.
 *
 **********/
global char *slstrtok(
    char                *tokp,
    const char          *sldelimitp,
    struct slstrtokblk  *tokblkp )
{
    char *resultp;     /* points to the token */
    char *tokevalp;    /* points to the char after the
                        * delimiter to be evaluated for token conditions
                        */

    /* ensures that subsequent call with a null
     * pointer will start a search from the saved pointer.
     */
    if (tokp)
    {
        tokevalp = tokp;
    }
    else
        tokevalp = tokblkp->slstrtokevalp;

    /* Skip over the delimitor characters and detect end of string.*/
    if (*(tokevalp += slstrspn(tokevalp, sldelimitp, YES)) == 0)
        return (NULL);

    /* Skip over non-separator characters and write a null
     * to where the  delimiter is located.  Increment tokevalp
     * so that the pointer points after the delimitor
     */
    resultp = tokevalp;
    if (*(tokevalp += slstrspn(tokevalp, sldelimitp, NO)) != 0)
        *tokevalp++ = 0;

    /* Update tokblkp->tokevalp so that it points
     * correctly after the delimiter
     */
    tokblkp->slstrtokevalp = tokevalp;

    return (resultp);
}

/*************
 *
 * Name:    slasciihextouint8
 *
 * Purpose: Converts a two-character array representation of a
 *          hexadecimal value into a swi_uint8 value
 *
 * Parms:   sp - pointer to ascii string
 *
 * Return:  converted value
 *
 * Abort:   None (No fatal errors)
 *
 * Notes:   Character array is assumed to be {msn, lsn}
 *
 **************/
global swi_uint8 slasciihextouint8(char *sp)
{
    swi_uint8 result;  /* converted swi_uint32 value from the string */

    /* initialize result */
    result = 0;

    /* convert the character into a hex value
     * and multiply the hex with its hex placeholder
     */
    if (slisdigit(*sp))
        result += (*sp - '0');
    else if ((*sp >= 'A') && (*sp <= 'F'))
        result += (*sp - 'A' + 10);
    else
        result += (*sp - 'a' + 10);

    sp++;
    result <<= 4;

    /* convert the character into a hex value
     * and multiply the hex with its hex placeholder
     */
    if (slisdigit(*sp))
        result += (*sp - '0');
    else if ((*sp >= 'A') && (*sp <= 'F'))
        result += (*sp - 'A' + 10);
    else
        result += (*sp - 'a' + 10);

    return (result);

}

/*************
 *
 * Name:    sluint64toasciidec
 *
 * Purpose: Converts a swi_uint64 value to a 0 padded
 *          character array representation of the
 *          value in ASCII in decimal with no null
 *          termination.
 *
 * Parms:   destp  - pointer to memory location
 *                   where the string conversion
 *                   should be placed.
 *          valuehigh  - high 4 byte of the value to be converted
 *          valuelow   - low 4 byte of the value to be converted
 *
 * Return:  pointer past the
 *
 * Abort:   None (No fatal errors)
 *
 * Notes:   Does not include null termination, so
 *          calling function must ensure destp buffer
 *          is at least 20 bytes big.
 *
 **************/
global char *sluint64toasciidec(
    char        *destp,
    swi_uint32  valuehigh,
    swi_uint32  valuelow )
{
    char *indexp;   /* index pointer into destp */
    swi_uint64 divident;
    swi_uint64 value;
    swi_uint8 singledigit;

    indexp = destp;

    value = ((((swi_uint64)valuehigh) << 32) | valuelow);

    for(divident = 10000000000000000000ULL; divident != 0; divident /= 10)
    {
        if(0 != value/divident)
        {
            break;
        }
    }

    for(; divident != 0; divident /= 10)
    {
        singledigit = (swi_uint8)(value/divident);
        *indexp++ = singledigit + '0';
        value -= singledigit * divident;
    }

    return indexp;

}

/*************
 *
 * Name:    sluint8toasciidec
 *
 * Purpose: Converts a swi_uint8 value to a 0 padded
 *          character array representation of the
 *          value in ASCII in decimal with no null
 *          termination.
 *
 * Parms:   destp  - pointer to memory location
 *                   where the string conversion
 *                   should be placed.
 *          value  - value to be converted
 *
 * Return:  pointer past the
 *
 * Abort:   None (No fatal errors)
 *
 * Notes:   Does not include null termination, so
 *          calling function must ensure destp buffer
 *          is at least 2 bytes big.
 *
 **************/
global char *sluint8toasciidec(
    char *destp,
    swi_uint8 value )
{
    swi_uint8 hundreds;        /* hundreds digit */
    swi_uint8 tens;            /* tens digit */
    swi_uint8 ones;            /* ones digit */
    char *indexp;   /* index pointer into destp */

    indexp = destp;
    hundreds = value/100;
    tens = (value - hundreds*100)/10;
    ones = value - hundreds*100 - tens*10;

    if(hundreds)
    {
        *indexp++ = hundreds + '0';
        *indexp++ = tens + '0';
        *indexp++ = ones + '0';
    }
    else if (tens)
    {
        *indexp++ = tens + '0';
        *indexp++ = ones + '0';
    }
    else
    {
        *indexp++ = ones + '0';
    }

    return indexp;
}
/*************
 *
 * Name:    sluint8toasciihex
 *
 * Purpose: Converts a swi_uint8 value to a 0 padded
 *          character array representation of the
 *          value in ASCII in hex with no null
 *          termination.
 *
 * Parms:   destp - pointer to memory location
 *                  where the string conversion
 *                  should be placed.
 *          value - value to be converted
 *
 * Return:  destp
 *
 * Abort:   None (No fatal errors)
 *
 * Notes:   Does not include null termination, so
 *          calling function must ensure destp buffer
 *          is at least 2 bytes big.
 *
 **************/
global char *sluint8toasciihex(
    char *destp,
    swi_uint8 value )
{
    swi_uint8 lsn;         /* least significant nibble */
    swi_uint8 msn;         /* most significant nible */
    char *indexp;   /* index pointer into destp */

    indexp = destp;
    msn = (value & 0xF0) >> 4; /* mask and shift MSB */
    if (msn <= 9)
    {
        *indexp++ = msn + '0';  /* '0'-'9' value */
    }
    else
    {
        *indexp++ = msn + 'A' - 10;  /* 'A'-'F' value */
    }

    lsn = value & 0x0F; /* mask off LSB */

    if (lsn <= 9)
    {
        *indexp++ = lsn + '0';  /* '0'-'9' value */
    }
    else
    {
        *indexp++ = lsn + 'A' - 10;  /* 'A'-'F' value */
    }


    return destp;
}

/*************
 *
 * Name:    sluint16toasciihex
 *
 * Purpose: Converts a swi_uint16 value to a 0 padded
 *          string representation of the value in
 *          ASCII in hex and then Null terminate the
 *          string.
 *
 * Parms:   destp - pointer to memory location
 *                  where the string conversion
 *                  should be placed.
 *          value - value to be converted
 *
 * Return:  destp
 *
 * Abort:   None (No fatal errors)
 *
 * Notes:   Includes null termination, so calling
 *          function must ensure destp buffer is at
 *          least 5 bytes big.
 *
 **************/
global char *sluint16toasciihex(
    char *destp,
    swi_uint16 value )
{
    swi_uint8 lsb; /* least significant byte */
    swi_uint8 msb; /* most significant byte */

    /* calculate MSB & LSB */
    msb = (value & 0xFF00) >> 8;
    lsb = (value & 0x00FF);

    /* convert MSB & LSB */
    sluint8toasciihex(destp, msb);
    sluint8toasciihex(destp+2, lsb);

    *(destp+4) = 0; /* null-terminate string */

    return destp;
}

/*************
 *
 * Name:    sluint32toasciihex
 *
 * Purpose: Converts a swi_uint32 value to a 0 padded
 *          string representation of the value in
 *          ASCII in hex and then Null terminate the
 *          string.
 *
 * Parms:   destp - pointer to memory location
 *                  where the string conversion
 *                  should be placed.
 *          value - value to be converted
 *
 * Return:  destp
 *
 * Abort:   None (No fatal errors)
 *
 * Notes:   Includes null termination, so calling
 *          function must ensure destp buffer is at
 *          least 7 bytes big.
 *
 **************/
global char *sluint32toasciihex(
    char *destp,
    swi_uint32 value )
{
    swi_uint16 lsw; /* least significant word */
    swi_uint16 msw; /* most significant word */

    /* calculate MSW & LSW */
    msw = (value & 0xFFFF0000) >> 16;
    lsw = (value & 0x0000FFFF);

    /* convert MSW & LSW */
    sluint16toasciihex(destp, msw);
    sluint16toasciihex(destp+4, lsw);

    return destp;
}

/*************
 *
 * Name:    slstrstr
 *
 * Purpose: determine if a substring is contained within a string
 *
 * Parms:   haystack    - string
 *          needle      - substring
 *
 * Return:  pointer to beginning of substring or NULL if substring not found
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
global char *slstrstr(
    const swi_char *haystack,
    const swi_char *needle )
{
    return strstr(haystack, needle);
}

/*************
 *
 * Name:    slglob
 *
 * Purpose: Creates a list of pathnames using the given path and directory
 *          entries under that "path". If "filter" is not NULL, the only
 *          directory entries considered are those matching the "filter"
 *          entries. Otherwise, all entries are considered. If "ignore" is not
 *          is not NULL, the directory entries matching those in the ignore
 *          list are not considered. Otherwise, all entries are considered.
 *
 * Parms:   (IN)path    - path to search
 *
 *          (IN)filter  - directory entries to search for. All entries
 *                        considered if set to NULL. Last entry must contain
 *                        a zero length string.
 *
 *          (IN)ignore  - directory entries to ignore. Not entries ignored if
 *                        set to NULL. Last entry must contain a zero length
 *                        string.
 *
 *          (IN)list    - list of directory entries under "path" (filter and
 *                        ignore lists accounted for).
 *
 *          listsz      - maximum number of directory entries the list can hold

 * Return:  number of directory entries found (filter and ignore lists
 *          accounted for).
 *
 * Abort:   none
 *
 * Notes:   if the number of directory entries exceeds listsz, only the first
 *          listsz entries are stored in list.
 *
 **************/
global swi_int32 slglob(
    const char      *path,
    const slstr_t   *filter,
    const slstr_t   *ignore,
    slstr_t         *list,
    const int       listsz )
{
    int listcnt = 0; /* count of relevant directory entries */
    struct swi_osdirapi dir;

    if( !swi_osapi_opendir(path, &dir) )
    {
        return 0;
    }

    do{ /* traverse directory entries */

        if( swi_osapi_readdir(&dir) )
        {
            /* don't skip current directory entry unless filter is enabled */
            int skip = 0;

            /* check ignore list */
            if( NULL != ignore )
            {
                int j = 0;
                while( 0 < strlen(ignore[j].str) )
                {
                    int offset =
                    slstrlen(dir.pentry->d_name) > slstrlen(ignore[j].str)
                    ? slstrlen(dir.pentry->d_name) - slstrlen(ignore[j].str)
                    : 0;

                    if( 0 ==
                        slstrncmp(  &dir.pentry->d_name[offset],
                                    ignore[j].str,
                                    slstrlen(ignore[j].str) ) )
                    {
                        skip = 1;
                    }
                    j++;
                }
            }

            if( !skip && (NULL != filter ) )
            {
                skip = 1;
                /* filter directory entries */
                int i = 0;
                while( 0 < strlen(filter[i].str) ) /* end of list NULL entry */
                {
                    int offset =
                    slstrlen(dir.pentry->d_name) > slstrlen(filter[i].str)
                    ? slstrlen(dir.pentry->d_name) - slstrlen(filter[i].str)
                    : 0;

                    if( 0 ==
                        slstrncmp(  &dir.pentry->d_name[offset],
                                    filter[i].str,
                                    slstrlen(filter[i].str) ) )
                    {
                        skip = 0;
                        break;
                    }
                    i++;
                }
            }

            /* store directory entry if there's room in user supplied storage */
            if( !skip && (listsz > listcnt ) )
            {
                int len = slstrlen(path);
                slstrncpy( list[listcnt].str, path, slstrlen(path));

                /* account for path separator */
                if( list[listcnt].str[len-1] != '/' )
                {
                    list[listcnt].str[len++] = '/';
                }

                slstrncpy(  &list[listcnt].str[len],
                            dir.pentry->d_name,
                            strlen(dir.pentry->d_name ) + 1 );
                                        /* +1 for null terminator */
                listcnt++;
            }
        }
    }while(dir.pentry);

    swi_osapi_closedir(&dir);

    /* return number of matches found in directory */
    return listcnt;
}
