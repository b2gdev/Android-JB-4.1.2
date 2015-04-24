/* 
** This source code is "Not a Contribution" under Apache license
**
** Sierra Wireless RIL
**
** Based on reference-ril by The Android Open Source Project
** and U300 RIL by ST-Ericsson.
** Modified by Sierra Wireless, Inc.
**
** Copyright (C) 2010 Sierra Wireless, Inc.
** Copyright (C) ST-Ericsson AB 2008-2009
** Copyright 2006, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
**
** Based on reference-ril by The Android Open Source Project.
**
** Modified for ST-Ericsson U300 modems.
** Author: Christian Bejram <christian.bejram@stericsson.com>
*/

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "at_misc.h"

/**
 *
 * strStartsWith. Determine whether the specified string starts with the
 * specified prefix (a shorter string), informing the caller of the result
 * 
 * @param[in] line
 *     Pointer to the string to be searched for the specified prefix
 * @param [in] prefix
 *     The substring to search for at the start of the string pointed to
 *     by "line". Must be NULL terminated
 *
 * @return
 *     1 - The prefix was found
 *     0 - The prefix was not found
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *      none
 */ 
int strStartsWith(const char *line, const char *prefix)
{
    for ( ; *line != '\0' && *prefix != '\0' ; line++, prefix++) {
        if (*line != *prefix) {
            return 0;
        }
    }

    return *prefix == '\0';
}

/**
  * getFirstElementValue: Very simple function that extracts and returns 
  * whats between ElementBeginTag and ElementEndTag. 
  *
  * This function is used to extract the parameters from the XML result
  * returned by U3xx during a PDP Context setup, and used to parse the
  * tuples of operators returned from AT+COPS.
  *
  * @param[in] document
  *      Document to be scanned
  * @param[in] elementBeginTag 
  *      Begin tag to scan for, return whats between begin/end tags
  * @param[in] elementEndTag   
  *      End tag to scan for, return whats between begin/end tags
  * @param[out] remainingDocument   
  *      Can return the a pointer to the remaining of pszDocument, this 
  *      parameter is optional
  *
  * @return
  *      Returns a pointer to allocated memory (which will need to be
  *      freed when done with) containing the elements between the start
  *      and end tag, if they are found. 
  *      returns NULL if any of the specified arguments are NULL or the
  *      start or end tags can't be found in the document
  */
char *getFirstElementValue(const char* document,
                           const char* elementBeginTag,
                           const char* elementEndTag,
                           char** remainingDocument)
{
    char* value = NULL;
    char* start = NULL;
    char* end = NULL;

    if (document != NULL && elementBeginTag != NULL && elementEndTag != NULL)
    {
        start = strstr(document, elementBeginTag);
        if (start != NULL)      
        {
            end = strstr(start, elementEndTag);
            if (end != NULL)
            {
                int n = strlen(elementBeginTag);
                int m = end - (start + n);
                value = (char*) malloc((m + 1) * sizeof(char));
                strncpy(value, (start + n), m);
                value[m] = (char) 0;
                
                /* Optional, return a pointer to the remaining document,
                   to be used when document contains many tags with same name. */
                if (remainingDocument != NULL)
                {
                    *remainingDocument = end + strlen(elementEndTag);
                }
            }
        }
    }
    return value;
}

/**
 *
 * char2nib - Converts a hexadecimal or decimal value in character form
 * into its equivalent 4-bit binary value 
 * 
 * @param[in] c
 *     The character to be converted to a 4-bit binary value
 *
 * @return
 *     The converted value as a binary digit or 0 if the character was
 *     not a decimal or hexadecimal digit
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *      none
 */ 
char char2nib(char c)
{
    if (c >= 0x30 && c <= 0x39)
        return c - 0x30;

    if (c >= 0x41 && c <= 0x46)
        return c - 0x41 + 0xA;

    if (c >= 0x61 && c <= 0x66)
        return c - 0x61 + 0xA;

    return 0;
}

/**
 *
 * hexToUpper - Accepts a pointer to a string containing printable decimal 
 * and hexadecimal digits and converts any lower case hexadecimal entries
 * to upper case.
 * 
 * @param[in/out] strp
 *     Pointer to a string of digits to be analyzed and converted
 * @param [in] len
 *     The length of the string (bytes), not including the NULL terminator
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *      none
 */ 
void hexToUpper( char *strp, int len )
{
    int i;

    for(i=0; i<len; i++) {
        if( strp[i] >= 'a' && strp[i] <= 'f' )
            strp[i] &= ~0x20;
    }
}

int stringToBinary(/*in*/ const char *string,
                   /*in*/ size_t len,
                   /*out*/ unsigned char *binary)
{
    int pos;
    const char *it;
    const char *end = &string[len];

    if (end < string)
        return -EINVAL;

    if (len & 1)
        return -EINVAL;

    for (pos = 0, it = string; it != end; ++pos, it += 2) {
        binary[pos] = char2nib(it[0]) << 4 | char2nib(it[1]);
    }
    return 0;
}

int binaryToString(/*in*/ const unsigned char *binary,
                   /*in*/ size_t len,
                   /*out*/ char *string)
{
    int pos;
    const unsigned char *it;
    const unsigned char *end = &binary[len];
    static const char nibbles[] =
        {'0', '1', '2', '3', '4', '5', '6', '7',
         '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    if (end < binary)
        return -EINVAL;

    for (pos = 0, it = binary; it != end; ++it, pos += 2) {
        string[pos + 0] = nibbles[*it >> 4];
        string[pos + 1] = nibbles[*it & 0x0f];
    }
    string[pos] = 0;
    return 0;
}

/* 
 * Quick test to determine whether the attached SIM is 
 * 2G or 3G. A 3G SIM will provide an FCP Template in
 * response to a SELECT command. The FCP Template 
 * always begins with 0x62. If that character is not
 * present, then we don't parse the SIM's file header
 * instead leaving it to the Java layer to figure it out 
 */
bool is3GSim( /* in */ const char *stream )
{
    unsigned int i;
    char buf[3];

    for(i=0; i<sizeof(buf); i++) {
        buf[i] = stream[i];
    }

    /* NULL terminate */
    buf[2] = 0;

    if( atoi(buf) == FCP_TEMPLATE_BYTE )
        return true;

    return false;
}

int parseTlv(/*in*/ const char *stream,
             /*in*/ const char *end,
             /*out*/ struct tlv *tlv)
{
#define TLV_STREAM_GET(stream, end, p)  \
    do {                                \
        if (stream + 1 >= end)          \
            goto underflow;             \
        p = ((unsigned)char2nib(stream[0]) << 4)  \
          | ((unsigned)char2nib(stream[1]) << 0); \
        stream += 2;                    \
    } while (0)

    size_t size;

    TLV_STREAM_GET(stream, end, tlv->tag);
    TLV_STREAM_GET(stream, end, size);
    if (stream + size * 2 > end)
        goto underflow;
    tlv->data = &stream[0];
    tlv->end  = &stream[size * 2];
    return 0;

underflow:
    return -EINVAL;
#undef TLV_STREAM_GET
}

