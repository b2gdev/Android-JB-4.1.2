/* 
** This source code is "Not a Contribution" under Apache license
**
** Sierra Wireless RIL
**
** Based on reference-ril by The Android Open Source Project
** Modified by Sierra Wireless, Inc.
**
** Copyright (C) 2010 Sierra Wireless, Inc.
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
*/

#include "at_tok.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/**
 * Starts tokenizing an AT response string.
 * Returns -1 if this is not a valid response string, 0 on success.
 * Updates *p_cur with current position.
 */
int at_tok_start(char **p_cur)
{
    if (*p_cur == NULL) {
        return -1;
    }

    /* Skip prefix,
       consume "^[^:]:". */

    *p_cur = strchr(*p_cur, ':');

    if (*p_cur == NULL) {
        return -1;
    }

    (*p_cur)++;

    return 0;
}

static void skipWhiteSpace(char **p_cur)
{
    if (*p_cur == NULL) return;

    while (**p_cur != '\0' && isspace(**p_cur)) {
        (*p_cur)++;
    }
}

static void skipNextComma(char **p_cur)
{
    if (*p_cur == NULL) return;

    while (**p_cur != '\0' && **p_cur != ',') {
        (*p_cur)++;
    }

    if (**p_cur == ',') {
        (*p_cur)++;
    }    
}

static char * nextTok(char **p_cur)
{
    char *ret = NULL;

    skipWhiteSpace(p_cur);

    if (*p_cur == NULL) {
        ret = NULL;
    } else if (**p_cur == '"') {
        (*p_cur)++;
        ret = strsep(p_cur, "\"");
        skipNextComma(p_cur);
    } else {
        ret = strsep(p_cur, ",");
    }

    return ret;
}


/**
 * Parses the next integer in the AT response line and places it in *p_out.
 * Returns 0 on success and -1 on fail.
 * Updates *p_cur.
 * "base" is the same as the base param in strtol.
 */
static int at_tok_nextint_base(char **p_cur, int *p_out, int base, int  uns)
{
    char *ret;
    
    if (*p_cur == NULL) {
        return -1;
    }

    ret = nextTok(p_cur);

    if (ret == NULL) {
        return -1;
    } else {
        long l;
        char *end;

        if (uns)
            l = strtoul(ret, &end, base);
        else
            l = strtol(ret, &end, base);

        *p_out = (int)l;

        if (end == ret) {
            return -1;
        }
    }

    return 0;
}

/**
 * Parses the next base 10 integer in the AT response line
 * and places it in *p_out.
 * Returns 0 on success and -1 on fail.
 * Updates *p_cur.
 */
int at_tok_nextint(char **p_cur, int *p_out)
{
    return at_tok_nextint_base(p_cur, p_out, 10, 0);
}

/**
 * Parses the next base 16 integer in the AT response line 
 * and places it in *p_out.
 * Returns 0 on success and -1 on fail.
 * Updates *p_cur.
 */
int at_tok_nexthexint(char **p_cur, int *p_out)
{
    return at_tok_nextint_base(p_cur, p_out, 16, 1);
}

int at_tok_nextbool(char **p_cur, char *p_out)
{
    int ret;
    int result;

    ret = at_tok_nextint(p_cur, &result);

    if (ret < 0) {
        return -1;
    }

    /* Booleans should be 0 or 1. */
    if (!(result == 0 || result == 1)) {
        return -1;
    }

    if (p_out != NULL) {
        *p_out = (char)result;
    }

    return ret;
}

int at_tok_nextstr(char **p_cur, char **p_out)
{
    if (*p_cur == NULL) {
        return -1;
    }

    *p_out = nextTok(p_cur);

    /* SWI_TBD: is this a bug, i.e. should we check if *p_out is NULL and 
       return -1 in this case?  Think about it some more.
     */

    return 0;
}


/**
 * Parses the base 10 integer at 'index' in the AT response line and places it
 * in *p_out.
 *  - all intermediate values are discarded. 
 *  - 'index' is relative to the current position in the AT response line,
 *    so that index=0 is always the next value.
 *
 * Returns 0 on success and -1 on fail.
 * Updates *p_cur.
 *
 * SWI_TBD: this function could be made generic, by taking a function pointer
 *          instead of explicitly calling at_tok_nextint() but that would make
 *          calling it rather cumbersone. If this function is needed for other
 *          values, then revisit the implementation.
 */
int at_tok_index_int(char **p_cur, int *p_out, int index)
{
    char *ret;
    int i;
    
    if (*p_cur == NULL) {
        return -1;
    }

    /* discard all the intermediate values */
    for (i=0; i<index; i++)
    {
        ret = nextTok(p_cur);

        if (ret == NULL)
        {
            return -1;
        }
    }

    /* get the desired value */
    return at_tok_nextint(p_cur, p_out);
}



/** Returns 1 on "has more tokens" and 0 if not. */
int at_tok_hasmore(char **p_cur)
{
    return ! (*p_cur == NULL || **p_cur == '\0');
}

/**
 *
 * Parses the next unsigned long integer in the AT response line and places it in *p_out.
 *
 * @param [in] p_cur
 *     pointer to the string pointer to be parsed
 * @param [out] p_out
 *     pointer to the out data to be written
 * @param base
 *     between 2 and 36 which is same as the base param in strtol/strtoul
 * 
 * @return
 *     0  - success
 *     -1 - otherwise
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 *
 */
int at_tok_next_unsignedlong(char **p_cur, unsigned long *p_out, int base)
{
    char *ret;
    
    if (*p_cur == NULL) {
        return -1;
    }

    ret = nextTok(p_cur);

    if (ret == NULL) {
        return -1;
    } else {
        unsigned long l;
        char *end;

        l = strtoul(ret, &end, base);
        *p_out = l;

        if (end == ret) {
            return -1;
        }
    }

    return 0;
}
