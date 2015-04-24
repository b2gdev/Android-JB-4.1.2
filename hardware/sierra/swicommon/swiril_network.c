/* 
** This source code is "Not a Contribution" under Apache license
**
** Sierra Wireless RIL
**
** Based on reference-ril by The Android Open Source Project
** and U300 RIL by ST-Ericsson.
** Modified by Sierra Wireless, Inc.
**
** Copyright (C) 2011 Sierra Wireless, Inc.
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
** Heavily modified for ST-Ericsson U300 modems.
** Author: Christian Bejram <christian.bejram@stericsson.com>
*/

#include <stdio.h>
#include <string.h>
#include <telephony/ril.h>
#include <assert.h>
#include "at_channel.h"
#include "at_tok.h"
#include "at_misc.h"
#include "swiril_main.h"
#include "swiril_misc.h"
#include "swiril_cache.h"

#define LOG_TAG "RIL"
#include "swiril_log.h"

extern void setPowerOff(void);

/**
 * RIL_REQUEST_ENTER_NETWORK_DEPERSONALIZATION
 *
 * Requests that network personlization be deactivated.
 */
void requestEnterNetworkDepersonalization(void *data, size_t datalen,
                                          RIL_Token t)
{
    /*
     * AT+CLCK=<fac>,<mode>[,<passwd>[,<class>]]
     *     <fac>    = "PN" = Network Personalization (refer 3GPP TS 22.022)
     *     <mode>   = 0 = Unlock 
     *     <passwd> = inparam from upper layer
     */

    int err = 0;
    char *cmd = NULL;
    ATResponse *atresponse = NULL;
    const char *passwd = ((const char **) data)[0];
    RIL_Errno rilerr = RIL_E_GENERIC_FAILURE;
    int num_retries = -1;

    /* Check inparameter. */
    if (passwd == NULL) {
        goto error;
    }
    /* Build and send command. */
    asprintf(&cmd, "AT+CLCK=\"PN\",0,\"%s\"", passwd);
    err = at_send_command(cmd, &atresponse);

    free(cmd);

    if (err < 0 || atresponse->success == 0)
        goto error;

    /* TODO: Return number of retries left. */
    RIL_onRequestComplete(t, RIL_E_SUCCESS, &num_retries, sizeof(int *));
    /* power cycle modem */
    setPowerOff();

finally:
    at_response_free(atresponse);
    return;

error:
    if (atresponse && at_get_cme_error(atresponse) == 16)
            rilerr = RIL_E_PASSWORD_INCORRECT;
    
    RIL_onRequestComplete(t, rilerr, NULL, 0);
    goto finally;
}
/**
 * RIL_REQUEST_OPERATOR
 *
 * Request current operator ONS or EONS.
 */
void requestOperator(void *data, size_t datalen, RIL_Token t)
{
    int err;
    int i;
    int skip;
    ATLine *cursor;
    char *response[3];
    ATResponse *atresponse = NULL;

    memset(response, 0, sizeof(response));

    err = at_send_command_multiline
        ("AT+COPS=3,0;+COPS?;+COPS=3,1;+COPS?;+COPS=3,2;+COPS?", "+COPS:",
         &atresponse);

    /* We expect 3 lines here:
     * +COPS: 0,0,"T - Mobile"
     * +COPS: 0,1,"TMO"
     * +COPS: 0,2,"310170"
     */

    if (err < 0)
        goto error;

    for (i = 0, cursor = atresponse->p_intermediates; cursor != NULL;
         cursor = cursor->p_next, i++) {
        char *line = cursor->line;

        err = at_tok_start(&line);

        if (err < 0)
            goto error;

        err = at_tok_nextint(&line, &skip);

        if (err < 0)
            goto error;

        /* If we're unregistered, we may just get
           a "+COPS: 0" response. */
        if (!at_tok_hasmore(&line)) {
            response[i] = NULL;
            continue;
        }

        err = at_tok_nextint(&line, &skip);

        if (err < 0)
            goto error;

        /* A "+COPS: 0, n" response is also possible. */
        if (!at_tok_hasmore(&line)) {
            response[i] = NULL;
            continue;
        }

        err = at_tok_nextstr(&line, &(response[i]));

        if (err < 0)
            goto error;
    }

    if (i != 3)
        goto error;

    /* 
     * Check if modem returned an empty string, and fill it with MNC/MMC 
     * if that's the case.
     */
    if (response[0] && strlen(response[0]) == 0) {
        response[0] = alloca(strlen(response[2]) + 1);
        strcpy(response[0], response[2]);
    }

    if (response[1] && strlen(response[1]) == 0) {
        response[1] = alloca(strlen(response[2]) + 1);
        strcpy(response[1], response[2]);
    }

    /* Return the successful response and cache the value for next time */
    swiril_cache_RIL_onRequestComplete(t, RIL_E_SUCCESS, response, sizeof(response),
                                       RIL_REQUEST_OPERATOR);

finally:
    at_response_free(atresponse);
    return;

error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    goto finally;
}

/**
 *
 * Query and parse registration state from +CREG or +CGREG
 *
 * @param use_cgreg
 *     use +CGREG or not
 * @param [out] response
 *     pointer to the response data
 * @param [out] cellID
 *     pointer to the response cellID
 * @param [out] hasAct
 *     pointer to whether response including a valid "AcT" info
 *
 * @return
 *     true  - finished successfully and response data valid
 *     false - otherwise
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     "ci" of "+CREG/+CGREG" response changed from 2 bytes to 4 bytes from Release 8.
 *     This function work for both 2 bytes and 4 bytes "ci".
 *
 */
bool queryRegState(bool use_cgreg, int *response, unsigned long *cellID, bool *hasAct)
{
    int err = 0;
    ATResponse *atresponse = NULL;
    const char *cmd;
    const char *prefix;
    char *line;
    char *p;
    int commas;
    int skip;
    bool bRet = true;
    
    if ( use_cgreg ) {
        cmd = "AT+CGREG?";
        prefix = "+CGREG:";
    } else { 
        cmd = "AT+CREG?";
        prefix = "+CREG:";
    }
        
    /* Query the registration state */
    err = at_send_command_singleline(cmd, prefix, &atresponse);

    if (err < 0 ||
        atresponse->success == 0 ||
        atresponse->p_intermediates == NULL) {
        goto error;
    }

    line = atresponse->p_intermediates->line;
    err = at_tok_start(&line);
    if (err < 0) {
        goto error;
    }
    
    /* 
     * The solicited version of the CREG response is
     * +CREG: <n>,<stat>[,<lac>,<ci>[,<AcT>]]
     * and the unsolicited version is
     * +CREG: <stat>[,<lac>,<ci>[,<AcT>]]
     * The <n> parameter is basically "is unsolicited creg on?"
     * which it should always be.
     *
     * Now we should normally get the solicited version here,
     * but the unsolicited version could have snuck in
     * so we have to handle both.
     *
     * Also since the LAC and CID are only reported when registered,
     * we can have 1, 2, 3, or 4 arguments here.
     *
     * finally, a +CGREG: answer may have a fifth value that corresponds
     * to the network type, as in;
     *
     * The solicited version of the CGREG response is
     * +CGREG: <n>,<stat>[,<lac>,<ci>[,<AcT>]]
     * and the unsolicited version is
     * +CGREG: <stat>[,<lac>,<ci>[,<AcT>]]
     * The <n> parameter is basically "is unsolicited cgreg on?"
     * which it should always be.
     */

    /* Count number of commas */
    commas = 0;
    for (p = line; *p != '\0'; p++) {
        if (*p == ',')
            commas++;
    }

    switch (commas) {
    case 0:                    /* +CREG: <stat> */
        err = at_tok_nextint(&line, &response[0]);
        if (err < 0) {
            goto error;
        }
        response[1] = -1;
        *cellID = 0;
        break;

    case 1:                    /* +CREG: <n>, <stat> */
        err = at_tok_nextint(&line, &skip);
        if (err < 0) {
            goto error;
        }
        err = at_tok_nextint(&line, &response[0]);
        if (err < 0) {
            goto error;
        }
        response[1] = -1;
        *cellID = 0;
        if (err < 0) {
            goto error;
        }
        break;

    case 2:                    /* +CREG: <stat>, <lac>, <ci> */
        err = at_tok_nextint(&line, &response[0]);
        if (err < 0) {
            goto error;
        }
        err = at_tok_nexthexint(&line, &response[1]);
        if (err < 0) {
            goto error;
        }
        err = at_tok_next_unsignedlong(&line, cellID, 16);
        if (err < 0) {
            goto error;
        }
        break;
    case 3:                    /* +CREG: <n>, <stat>, <lac>, <ci> */
        err = at_tok_nextint(&line, &skip);
        if (err < 0) {
            goto error;
        }
        /* solicited response received, 
         * unsolicited response contains <lac> and <ci> only after modem registered */
        if (skip == 2) {
            err = at_tok_nextint(&line, &response[0]);
            if (err < 0) {
                goto error;
            }
            err = at_tok_nexthexint(&line, &response[1]);
            if (err < 0) {
                goto error;
            }
            err = at_tok_next_unsignedlong(&line, cellID, 16);
            if (err < 0) {
                goto error;
            }
        }
        /* unsolicited response */
        else {
                               /* +CGREG: <stat>, <lac>, <ci> ,<AcT> */
            response[0] = skip;
            err = at_tok_nextint(&line, &response[1]);
            if (err < 0) {
                goto error;
            }
            err = at_tok_next_unsignedlong(&line, cellID, 16);
            if (err < 0) {
                goto error;
            }
            err = at_tok_nexthexint(&line, &response[3]);
            if (err < 0) {
                goto error;
            }
            *hasAct = true;
        }
        break;
        /* Special case for +CREG/+CGREG, there is a fourth parameter
         * that is the AcT.
         */
    case 4:                    /* +CGREG: <n>, <stat>, <lac>, <cid>, <AcT> */
        err = at_tok_nextint(&line, &skip);
        if (err < 0) {
            goto error;
        }
        err = at_tok_nextint(&line, &response[0]);
        if (err < 0) {
            goto error;
        }
        err = at_tok_nexthexint(&line, &response[1]);
        if (err < 0) {
            goto error;
        }
        err = at_tok_next_unsignedlong(&line, cellID, 16);
        if (err < 0) {
            goto error;
        }
        err = at_tok_nexthexint(&line, &response[3]);
        if (err < 0) {
            goto error;
        }
        *hasAct = true;
        break;
    default:
        goto error;
    }
    
finally:
    at_response_free(atresponse);
    return bRet;
    
error:
    bRet = false;
    LOGE("%s err %d", __func__, err);
    goto finally;
}
