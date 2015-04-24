/*************
 *
 * Filename: slucs2.c
 *
 * Purpose: This file contains conversion functions for UTF8 to and from
 *          UCS2.
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
#define SL2BYTEUTF8FIRSTBYTEMSK 0x1F
#define SL3BYTEUTF8FIRSTBYTEMSK 0x0F
#define SLSECONDARYUTF8BYTEMSK  0x3F

#define SL2BYTEUTF8FIRSTBYTEMRK 0xC0
#define SL3BYTEUTF8FIRSTBYTEMRK 0xE0
#define SL4BYTEUTF8FIRSTBYTEMRK 0xF0
#define SLSECONDARYUTF8BYTEMRK  0x80

#define SLUTF8CHARSIZE1 (swi_UCS2)0x80
#define SLUTF8CHARSIZE2 (swi_UCS2)0x800

/* these constants define the UTF-16 surrogate ranges */
/* characters within this range can not be represented in UCS2 */
#define SLUTF16SURSTART  0xD800
#define SLUTF16SURSTOP   0xDFFF

#define ONEMASK ((swi_ssize_t)(-1) / 0xFF)

/*************
 *
 * Name:    slUcs2ToUtf8
 *
 * Purpose: To convert a UCS2 string to a UTF-8 string.
 *          This function will NULL-terminate the UTF-8 string.
 *
 *          An UCS2 character is converted to UTF-8 in the following way:
 *
 *              UCS2                  UTF-8
 *                                    byte1    byte2    byte3
 *              00000000 0xxxxxxx     0xxxxxxx
 *              00000yyy yyxxxxxx     110yyyyy 10xxxxxx
 *              zzzzyyyy yyxxxxxx     1110zzzz 10yyyyyy 10xxxxxx
 *
 * Parms:   ucs2stringp  - pointer to UCS2 string to be converted
 *          numucs2chars - number of characters in the UCS2 string
 *          utf8stringp  - pointer to the empty UTF-8 string buffer where
 *                         the converted string will be placed
 *          maxbytes     - maximum number of bytes supported by the passed in
 *                         UTF-8 string pointer (including NULL terminator).
 *
 * Return:  TRUE  - if the conversion was successful
 *          FALSE - if the string could not be fit in the provided buffer
 *
 * Abort:   none
 *
 * Notes:   It is hoped that the caller will ensure that the string
 *          buffer given by utf8stringp will be large enough to contain all of
 *          the converted characters plus a NULL character.  Which means
 *          the buffer must be at least of size (numucs2chars*4 + 1).  If the
 *          buffer is too small then the string will be truncated.
 *
 **************/
global swi_bool slUcs2ToUtf8(
    swi_UCS2   *ucs2stringp,
    swi_uint16 numucs2chars,
    unsigned char *utf8stringp,
    swi_uint16 maxbytes)
{
    swi_uint16 numconvchars; /* counter of the number of character converted */
    swi_UCS2   ucs2char;     /* value of the UCS2 character being converted */
    unsigned char *endstringp = utf8stringp + maxbytes - 1;

    /* clear the number of converted characters counter */
    numconvchars = 0;

    /* loop through the number of characters to convert */
    while(numconvchars != numucs2chars)
    {
        /* get the next UCS2 character for conversion */
        ucs2char = *ucs2stringp++;

        /* if this UCS2 character is represented in UTF-8 as a single byte */
        if(ucs2char < SLUTF8CHARSIZE1)
        {
            /*
             * verify that there is room in the buffer for this
             * converted character
             */
            if(utf8stringp + 1 > endstringp)
            {
                *utf8stringp = EOS;
                return(FALSE);
            }

            /* copy character over and increment UTF-8 string pointer */
            *utf8stringp++ = (char)ucs2char;
        }
        /* if this UCS2 character is represented in UTF-8 as two bytes */
        else if(ucs2char < SLUTF8CHARSIZE2)
        {
            /*
             * verify that there is room in the buffer for this
             * converted character
             */
            if(utf8stringp + 2 > endstringp)
            {
                *utf8stringp = EOS;
                return(FALSE);
            }

            /* copy the second UTF-8 character into the string buffer */
            *(utf8stringp + 1) =
                (char)(SLSECONDARYUTF8BYTEMRK +
                       (ucs2char & SLSECONDARYUTF8BYTEMSK));

            /* shift ucs2char to its first byte value */
            ucs2char >>= 6;

            /* copy the first UTF-8 character into the string buffer */
            *utf8stringp =
                (char)(SL2BYTEUTF8FIRSTBYTEMRK +
                        (ucs2char & SL2BYTEUTF8FIRSTBYTEMSK));

            /*
             * increment the UTF-8 string pointer pass the
             * representation of this character
             */
            utf8stringp +=2;
        }
        /* this USC2 character is represented in UTF-8 as three bytes */
        else
        {
            /*
             * verify that there is room in the buffer for this
             * converted character
             */
            if(utf8stringp + 3 > endstringp)
            {
                *utf8stringp = EOS;
                return(FALSE);
            }

            /* copy the third UTF-8 character into the string buffer */
            *(utf8stringp + 2) =
                (char)(SLSECONDARYUTF8BYTEMRK +
                    (ucs2char & SLSECONDARYUTF8BYTEMSK));

            /* shift ucs2char to its second byte value */
            ucs2char >>= 6;

            /* copy the second UTF-8 character into the string buffer */
            *(utf8stringp + 1) =
                (char)(SLSECONDARYUTF8BYTEMRK +
                    (ucs2char & SLSECONDARYUTF8BYTEMSK));

            /* shift ucs2char to its first byte value */
            ucs2char >>= 6;

            /* copy the first UTF-8 character into the string buffer */
            *utf8stringp =
                (char)(SL3BYTEUTF8FIRSTBYTEMRK +
                    (ucs2char & SL3BYTEUTF8FIRSTBYTEMSK));

            /*
             * increment the UTF-8 string pointer pass the representation
             * of this character
             */
            utf8stringp +=3;
        }
        /* increment number of characters converted */
        numconvchars++;
    }

    *utf8stringp = EOS;

    return(TRUE);
}
/*************
 *
 * Name:    slValidateUtf8char
 *
 * Purpose: To validate that a UTF-8 character is legal.  This function
 *          can assess multi-byte charaters.
 *          user.
 *
 *          Valid UTF-8 encodings for characters of length 1 to 3 bytes is as
 *          follows:
 *
 *          Binary UTF-8 format             Legal UTF-8 values
 *                                          byte1  byte2 byte3
 *          0xxxxxxx                        00-7F
 *          110xxxxx 10xxxxxx               C2-DF  80-BF
 *          1110xxxx 10xxxxxx 10xxxxxx      E0     A0-BF 80-BF or
 *                                          E1-EF  80-BF 80-BF
 *
 * Parms:   firstbyte   - first byte of the character
 *          secondbyte  - second byte of the character (maybe garbage if
 *                        the first byte indicates a single byte character)
 *          thirdbyte   - third byte of the character (maybe garbage if the
 *                        first byte does not indicate a character three bytes
 *                        or longer)
 *          fourthbyte  - fourth byte of the character (maybe garbage if the
 *                        first byte does not indicate a four byte character)
 *          numbytesp   - memory location to return the number of bytes used
 *                        for the given character
 *
 * Return:  TRUE  - if UTF-8 character is legal
 *          FALSE - otherwise
 *
 * Abort:   none
 *
 * Notes:   This function does not assess the validity of 4-byte UTF-8
 *          characters since 4-byte characters can not be represented
 *          in UCS2 and the only user of this function is apUtf8ToUcs2().
 *
 **************/
local swi_bool slValidateUtf8char(
    unsigned char firstbyte,
    unsigned char secondbyte,
    unsigned char thirdbyte,
    unsigned char fourthbyte,
    swi_uint8 *numbytesp)
{
    /* if the first byte indicates that this is a single byte char */
    if(firstbyte < SL2BYTEUTF8FIRSTBYTEMRK)
    {
        /* set the number of bytes to one */
        *numbytesp = 1;

        /* validate that the single byte character is within the legal range */
        if(firstbyte >= 0x80)
        {
            return(FALSE);
        }
    }
    /* if the first byte indicates that there are two bytes in this char */
    else if (firstbyte < SL3BYTEUTF8FIRSTBYTEMRK)
    {
        /* set the number of bytes to two */
        *numbytesp = 2;

        /*
         * validate that the first byte of this character is within
         * the legal range
         */
        if(firstbyte < 0xC2 || firstbyte > 0xDF)
        {
            return(FALSE);
        }

        /*
         * validate that the second byte of the two byte character is within
         * the legal range
         */
        if(secondbyte < 0x80 || secondbyte > 0xBF)
        {
            return(FALSE);
        }
    }
    /* if the first byte indicates that there are three bytes in this char */
    else if (firstbyte < SL4BYTEUTF8FIRSTBYTEMRK)
    {
        /* set the number of bytes to three */
        *numbytesp = 3;

        /* if the first byte is a valid value of 0xEO */
        if(firstbyte == 0xE0)
        {
            /* check that the second byte is in range */
            if(secondbyte < 0xA0 || secondbyte > 0xBF)
            {
                return(FALSE);
            }
        }
        /* else check if the first byte is in a legal range */
        else if(firstbyte >= 0xE1 && firstbyte <= 0xEF)
        {
            /* given the first byte range - validate the second byte */
            if(secondbyte < 0x80 || secondbyte > 0xBF)
            {
                return(FALSE);
            }
        }
        /* else the first byte is illegal */
        else
        {
            return(FALSE);
        }

        /* validate that the third byte is legal */
        if(thirdbyte < 0x80 || thirdbyte > 0xBF)
        {
            return(FALSE);
        }
    }
    /* else this must be a four byte character */
    else
    {
        /* set the number of bytes to four */
        *numbytesp = 4;

        /* if the first byte is a valid value of 0xFO */
        if(firstbyte == 0xF0)
        {
            /* check that the second byte is in range */
            if(secondbyte < 0x90 || secondbyte > 0xBF)
            {
                return(FALSE);
            }
        }
        /* else if the first byte is in a legal range */
        else if(firstbyte >= 0xF1 && firstbyte <= 0xF3)
        {
            /* check that the second byte is in range */
            if(secondbyte < 0x80 || secondbyte > 0xBF)
            {
                return(FALSE);
            }
        }
        /* else the first byte is not legal */
        else
        {
            return(FALSE);
        }

        /* validate that the third byte is legal */
        if(thirdbyte < 0x80 || thirdbyte > 0xBF)
        {
            return(FALSE);
        }
        /* validate that the fourth byte is legal */
        if(fourthbyte < 0x80 || fourthbyte > 0xBF)
        {
            return(FALSE);
        }
    }
    return(TRUE);
}
/*************
 *
 * Name:    slUtf8ToUcs2
 *
 * Purpose: To convert a UTF-8 string given by the API user to a UCS2
 *          string to be sent to the device.
 *
 *          An UTF-8 character is converted to UCS2 in the following way:
 *
 *              UTF-8                         UCS2
 *              byte1    byte2    byte3       fixed at 2 bytes
 *              0xxxxxxx                      00000000 0xxxxxxx
 *              110yyyyy 10xxxxxx             00000yyy yyxxxxxx
 *              1110zzzz 10yyyyyy 10xxxxxx    zzzzyyyy yyxxxxxx
 *
 * Parms:   utf8stringp - pointer to the UTF-8 character string to be converted
 *                        Note that this string MUST be NULL-terminated!
 *          usc2stringp - pointer to the string buffer to place the converted
 *                        UCS2 string.
 *          maxnumchars - the maximum number of UCS2 characters the above
 *                        string buffer can accommodate.
 *          numcharsp   - memory location to place the number of converted
 *                        characters
 *
 * Return:  TRUE  - conversion was successful
 *          FALSE - otherwise
 *
 * Abort:   none
 *
 * Notes:   4-byte UTF-8 characters can not be represented in UCS2 neither can
 *          UTF-16 characters that make use of surrogate pairs.
 *
 **************/
global swi_bool slUtf8ToUcs2(
    unsigned char *utf8stringp,
    swi_UCS2      *usc2stringp,
    swi_uint16    maxnumchars,
    swi_uint16    *numcharsp)
{
    swi_uint16 numconvchars = 0;
    swi_UCS2   ucs2char;
    swi_uint8  numutf8bytes;

    /* while not at the end of the UTF-8 string */
    while(*utf8stringp != EOS)
    {
        /* clear temporary UCS2 character variable */
        ucs2char = 0;

        /* if the given UCS2 string buffer is full */
        if(numconvchars == maxnumchars)
        {
            return(FALSE);
        }

        /* check that the given UTF-8 string is legal */
        if(!slValidateUtf8char( *utf8stringp,
                                *(utf8stringp + 1),
                                *(utf8stringp + 2),
                                *(utf8stringp + 3),
                                &numutf8bytes))
        {
            /* illegal UTF-8 string */
            return(FALSE);
        }

        /* decode the UTF-8 character based on its number of bytes */
        switch(numutf8bytes)
        {
            /* UTF-8 character is one byte long */
            case 1:
                ucs2char += (swi_UCS2) *utf8stringp++;
                break;

            /* UTF-8 character is two bytes long */
            case 2:
                ucs2char += (swi_UCS2 )(*utf8stringp++ & SL2BYTEUTF8FIRSTBYTEMSK);
                ucs2char <<= 6;
                ucs2char += (swi_UCS2)(*utf8stringp++ & SLSECONDARYUTF8BYTEMSK);
                break;

            /* UTF-8 character is three bytes long */
            case 3:
                ucs2char += (swi_UCS2 )(*utf8stringp++ & SL3BYTEUTF8FIRSTBYTEMSK);
                ucs2char <<= 6;
                ucs2char += (swi_UCS2)(*utf8stringp++ & SLSECONDARYUTF8BYTEMSK);
                ucs2char <<= 6;
                ucs2char += (swi_UCS2)(*utf8stringp++ & SLSECONDARYUTF8BYTEMSK);
                break;

            /* UTF-8 character is not supported byte UCS2 */
            default:
                return(FALSE);
        }

        /* check that UCS2 characters is not part of a surrogate pair */
        if(ucs2char >= SLUTF16SURSTART && ucs2char <= SLUTF16SURSTOP)
        {
            /* UCS2 doesn't support surrogate values */
            return(FALSE);
        }

        /* store converted UCS2 character into UCS2 string buffer */
        *usc2stringp++ = ucs2char;

        /* increment number of converted characters */
        numconvchars++;
    }

    /* return number of converted characters to caller */
    *numcharsp = numconvchars;

    /* convertion was succcessful */
    return(TRUE);
}

/*************
 *
 * Name:    slstrlenutf8
 *
 * Purpose: To return the string length of an UTF-8 string given by the API
 *          to be sent to the device.
 *
 *          An UTF-8 character is converted to UCS2 in the following way:
 *
 *              UTF-8                         UCS2
 *              byte1    byte2    byte3       fixed at 2 bytes
 *              0xxxxxxx                      00000000 0xxxxxxx
 *              110yyyyy 10xxxxxx             00000yyy yyxxxxxx
 *              1110zzzz 10yyyyyy 10xxxxxx    zzzzyyyy yyxxxxxx
 *
 * Parms:   sp - pointer to the UTF-8 character string to be converted
 *                        Note that this string MUST be NULL-terminated!
 *
 * Return:  number of utf8 characters (not bytes)
 *
 * Abort:   none
 *
 * Notes:   4-byte UTF-8 characters can not be represented in UCS2 neither can
 *          UTF-16 characters that make use of surrogate pairs.
 *
 **************/
global swi_ssize_t slstrlenutf8(const char * sp)
{
    const char * lsp;     /* local sp pointer */
    swi_ssize_t count = 0;
    swi_ssize_t u;
    unsigned char b;
    swi_bool not_done=TRUE;

    /* Handle any initial misaligned bytes. */
    for (lsp = sp; (unsigned long int)(lsp) & (sizeof(swi_ssize_t) - 1); lsp++) {
        b = *lsp;

        /* Exit if we hit a zero byte. */
        if (b == '\0')
            not_done = FALSE;

        /* Is this byte NOT the first byte of a character? */
        count += (b >> 7) & ((~b) >> 6);
    }

    if (not_done)
    {
        /* Handle complete blocks. */
        for (; ; lsp += sizeof(swi_ssize_t)) {
            /* Prefetch 256 bytes ahead. */
            __builtin_prefetch(&lsp[256], 0, 0);

            /* Grab 4 or 8 bytes of UTF-8 data. */
            u = *(swi_ssize_t *)(lsp);

            /* Exit the loop if there are any zero bytes. */
            if ((u - ONEMASK) & (~u) & (ONEMASK * 0x80))
                break;

            /* Count bytes which are NOT the first byte of a character. */
            u = ((u & (ONEMASK * 0x80)) >> 7) & ((~u) >> 6);
            count += (u * ONEMASK) >> ((sizeof(swi_ssize_t) - 1) * 8);
        }

        /* Take care of any left-over bytes. */
        for (; ; lsp++) {
            b = *lsp;

            /* Exit if we hit a zero byte. */
            if (b == '\0')
                break;

            /* Is this byte NOT the first byte of a character? */
            count += (b >> 7) & ((~b) >> 6);
        }
    }
    return ((lsp - sp) - count);
}
