/* 
 * This source code is "Not a Contribution" under Apache license
 *
 * Sierra Wireless RIL
 *
 * Based on reference-ril by The Android Open Source Project
 * and U300 RIL by ST-Ericsson.
 * Modified by Sierra Wireless, Inc.
 *
 * Copyright (C) 2011 Sierra Wireless, Inc.
 * Copyright (C) ST-Ericsson AB 2008-2009
 * Copyright 2006, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Based on reference-ril by The Android Open Source Project.
 *
 * Heavily modified for ST-Ericsson U300 modems.
 * Author: Christian Bejram <christian.bejram@stericsson.com>
 */

#include <stdio.h>
#include <string.h>
#include <telephony/ril.h>
#include "swiril_main.h"
#include "at_channel.h"
#include "swiril_misc.h"

#define LOG_TAG "RIL"
#include "swiril_log.h"

#include "swiril_oem.h"


/**
 *
 * OEM-specific subcommand to invoke AT command request from client handler
 *
 * @param[in] data 
 *     pointer to the string pointer array
 * @param count 
 *     string pointer count
 * @param t 
 *     RIL token
 *
 * @return
 *     None
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 */
static void swiAtCommandHandler(const char **data, int count, RIL_Token t)
{
    int err = -1;
    int response = 0;
    ATLine *atlinep = NULL;
    ATResponse *atresponse = NULL;
    char *cmd = NULL;
    char *waitstr = NULL;
    char **respp = NULL;
    int linecount;
    int i;
    const char ** cur = data;
    char *subcmd = NULL;
    int opt;

    /* skip subcommand */
    cur ++;
    count --;
        
    /* grab AT command sending option and move pointer*/
    opt = **cur - '0';
    cur++;


    /* change the AT command strings to upper case */
    cmd = swimalloc(strlen(*cur) + 1, "swiAtCommandHandler: can not allocate AT command");
    memset(cmd, 0, strlen(*cur) + 1);
    
    /* no need to upper, because some AT parameters are case sensitive e.g. apn name */
    /* cmd = sltoupperstrn(cmd, *cur, strlen(*cur)); */
    slstrncpy(cmd, *cur, strlen(*cur));
   
    /* make sure 2nd parameter contains "AT" */
    if ((strstr(cmd, "AT") == NULL) && (strstr(cmd, "at") == NULL)) {
        LOGE("swiAtCommandHandler wrong AT command '%s' parameter", *cur);
        goto error;
    }
    
    /* send AT command */
    if (count == 2) {
        /* only contains subcommand and AT command */
        switch (opt) {
            case SWI_AT_CMD_OPT_1:
                err = at_send_command_singleline(cmd, "", &atresponse);
                break;
                
            case SWI_AT_CMD_OPT_2:
                err = at_send_command_multiline(cmd, "", &atresponse);
                break;
                
            case SWI_AT_CMD_OPT_3:
                err = at_send_command_multivalue(cmd, "", &atresponse);
                break;
                
            default:
                LOGE("swiAtCommandHandler invalid sending option");
                goto error;
        }
    }
    else if (count == 3) {
        /* contains subcommand, AT command, and wait string */
        cur++;
        waitstr = swimalloc(strlen(*cur) + 1, "swiAtCommandHandler: can not allocate AT response");
        memset(waitstr, 0, strlen(*cur) + 1);
        waitstr = sltoupperstrn(waitstr, *cur, strlen(*cur));

        switch (opt) {
            case SWI_AT_CMD_OPT_1:
                err = at_send_command_singleline(cmd, waitstr, &atresponse);
                break;
                
            case SWI_AT_CMD_OPT_2:
                err = at_send_command_multiline(cmd, waitstr, &atresponse);
                break;
                
            case SWI_AT_CMD_OPT_3:
                err = at_send_command_multivalue(cmd, waitstr, &atresponse);
                break;
                
            default:
                LOGE("swiAtCommandHandler invalid sending option");
                goto error;
        }
    }
    else {
        LOGE("swiAtCommandHandler %d parameters invalid", count);
        goto error;
    }
    
    if (err < 0 || atresponse->success == 0)
        goto error;

    /* count how many lines of AT command response */
    for (linecount=0, atlinep=atresponse->p_intermediates;
            atlinep!=NULL; linecount++, atlinep=atlinep->p_next);

    /* allocate memory for responses, first string is always reserved subcommand */
    respp = (char **)swimalloc((linecount + 1) * sizeof(char *), 
                               "swiAtCommandHandler allocate char** memory failed");

    /* put back subcommand */
    asprintf(&subcmd, "%s", *data);
    respp[0] = subcmd;

    /* add AT command response */
    atlinep=atresponse->p_intermediates;
    for (i = 1; i <= linecount; i++) {
        
        if (atlinep != NULL) {
            respp[i] = atlinep->line;
        }
        atlinep=atlinep->p_next;
    }
    RIL_onRequestComplete(t, RIL_E_SUCCESS, respp, ((linecount + 1) * sizeof(char *)));
    
finally:
    at_response_free(atresponse);

    if (cmd != NULL)
        free(cmd);
        
    if (waitstr != NULL)
        free(waitstr);

    if (subcmd != NULL)
        free(subcmd);
                
    if (respp != NULL)
        free(respp);
    return;

error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    goto finally;
}
    
/**
 *
 * OEM-specific subcommand RIL currentState() handler
 *
 * @param[in] data 
 *     pointer to the string pointer array
 * @param count 
 *     string pointer count
 * @param t 
 *     RIL token
 *
 * @return
 *     None
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 */
static void swiCurStateHandler(const char **data, int count, RIL_Token t)
{
    char *response[2];
    RIL_RadioState state;
    char *strp = NULL;
    char *subcmd = NULL;
    
    asprintf(&subcmd, "%s", *data);
    response[0] = subcmd;
    state = currentState();
    asprintf(&strp, "%d", state);
    response[1] = strp;
    
    RIL_onRequestComplete(t, RIL_E_SUCCESS, response, 2 * sizeof(char *));
    
    if (subcmd != NULL)
        free(subcmd);
        
    if (strp != NULL)
        free(strp);
}    

/**
 *
 * OEM-specific subcommand RIL onSupports() handler
 *
 * @param[in] data 
 *     pointer to the string pointer array
 * @param count 
 *     string pointer count
 * @param t 
 *     RIL token
 *
 * @return
 *     None
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 */
static void swiOnSupportsHandler(const char **data, int count, RIL_Token t)
{
    char *response[2];
    int state;
    char *strp = NULL;
    int rilrequest;
    char *subcmd = NULL;
    
    if (count == 2) {
        asprintf(&subcmd, "%s", *data++);
        response[0] = subcmd;
        rilrequest = atoi(*data);
        state = onSupports(rilrequest);
        asprintf(&strp, "%d", state);
        response[1] = strp;
        
        RIL_onRequestComplete(t, RIL_E_SUCCESS, response, 2 * sizeof(char *));
        
        if (subcmd != NULL)
            free(subcmd);
            
        if (strp != NULL)
            free(strp);
    }
    else {
        RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    }
}
 
/**
 *
 * OEM-specific subcommand RIL onCancel() handler
 *
 * @param[in] data 
 *     pointer to the string pointer array
 * @param count 
 *     string pointer count
 * @param t 
 *     RIL token
 *
 * @return
 *     None
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     Because RIL onCancel() doesn't do anything, just pass back the current RIL token t 
 *     for test purpose
 */
static void swiOnCancelHandler(const char **data, int count, RIL_Token t)
{
    char *response[1];
    char *subcmd = NULL;
    
    asprintf(&subcmd, "%s", *data);
    response[0] = subcmd;
    /* SWI_TBD: it is no way for client side to pass in a vaild RIL_Token, so pass t to make onCancel() happy */
    onCancel(t);
    
    RIL_onRequestComplete(t, RIL_E_SUCCESS, response, sizeof(char *));
    
    if (subcmd != NULL)
        free(subcmd);
}

/**
 *
 * OEM-specific subcommand RIL getVersion() handler
 *
 * @param[in] data 
 *     pointer to the string pointer array
 * @param count 
 *     string pointer count
 * @param t 
 *     RIL token
 *
 * @return
 *     None
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 */
static void swiGetVersionHandler(const char **data, int count, RIL_Token t)
{
    char *response[2];
    RIL_RadioState state;
    char *strp = NULL;
    char *subcmd = NULL;
    
    asprintf(&subcmd, "%s", *data);
    response[0] = subcmd;
    asprintf(&strp, "%s", getVersion());
    response[1] = strp;
    
    RIL_onRequestComplete(t, RIL_E_SUCCESS, response, 2 * sizeof(char *));
    
    if (subcmd != NULL)
        free(subcmd);
        
    if (strp != NULL)
        free(strp);
} 

/**
 *
 * Common OEM Hookstrings handler function. This function can respond
 * to OEM Hookstring requests with subcommands identified in the case
 * statement below. It is called by either of the two RIL-specific 
 * hookstring handler functions if the incoming subcommand doesn't 
 * match one of the RIL-specific ones.
 *
 * @param[in] data 
 *     pointer to the packet data 
 * @param datalen 
 *     packet data length
 * @param t 
 *     RIL token
 *
 * @return
 *     None
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     This command defined as a RIL test command.
 *     The idea is treating the string array sent from 
 *     client side as a command structure
 *         data[0] - subcommand
 *         data[1] - parameter 1 of subcommand
 *         data[2] - parameter 2 of subcommand
 *         ... ...
 * 
 *     The response packet will be like following
 *         responses[0] - subcommand
 *         responses[1] - raw string of subcommand response line 1
 *         responses[2] - raw string of subcommand response line 2
 *         ... ... 
 */
void swicommon_requestOEMHookStrings(void *data, size_t datalen, RIL_Token t)
{
    int i;
    const char **cur;
    SWI_OEMHookType subcommand;

    cur = (const char **) data;
    subcommand = atoi(*cur);

    switch(subcommand) {
        case SWI_AT_COMMAND_OPT:
            swiAtCommandHandler(cur, (datalen / sizeof(char *)), t);
            break;
            
        case SWI_CURRENT_STATE:
            swiCurStateHandler(cur, (datalen / sizeof(char *)), t);
            break;
            
        case SWI_ON_SUPPORTS:
            swiOnSupportsHandler(cur, (datalen / sizeof(char *)), t);
            break;
            
        case SWI_ON_CANCEL:
            swiOnCancelHandler(cur, (datalen / sizeof(char *)), t);
            break;
            
        case SWI_GET_VERSION:
            swiGetVersionHandler(cur, (datalen / sizeof(char *)), t);
            break;

        default:
            LOGE("requestOEMHookStrings unsupported subcommand %d", subcommand);
            RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
            break;
    }
}
