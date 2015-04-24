/**
 *
 * @ingroup swiril
 *
 * @file 
 * Prototypes for QMI based OEM related Sierra functions
 *
 * @author
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */

#include <stdio.h>
#include <string.h>
#include <telephony/ril.h>
#include "swiril_main.h"
#include "at_channel.h"
#include "at_tok.h"
#include "swiril_misc.h"
#include "swiril_misc_qmi.h"
#include "swiril_device_qmi.h"
#include "swiril_oem_qmi.h"

#define LOG_TAG "RIL"
#include "swiril_log.h"

#include "swiril_oem.h"

/**
 *  Initiate CDMA activation on the attached device
 *
 *  @param  data [in]
 *          - Pointer to the string pointer array, unused 
 *  @param  datalen [in]
 *          - Count of pointers in the array, unused 
 *  @param  t [in]
 *          - A RIL token structure pointer
 *
 *  @return None
 *
 *  @note   Support: CDMA
 *
 */
void requestOEMHookStringsCMDAActQMI(void *data, size_t datalen, RIL_Token t)
{
    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
    activateServiceCDMA();
}


/**
 *  Initiate a SIM PB read request
 *
 *  @param  data [in]
 *          - Pointer to the string pointer array, unused
 *  @param  count [in]
 *          - count of the number of strings pointed to by the data array, unused
 *  @param  t [in]
 *          - A RIL token structure pointer
 *
 *  @return None
 *
 *  @note   Support: UMTS
 * 
 */
void requestOEMHookStringsReadSIMPB(const char **data, size_t count, RIL_Token t)
{
    char **responsepp = NULL;
    int responsesize=0;
    ATResponse *atresponse = NULL;
    ATLine *atlinep = NULL;
    int err = 0;
    char *cmd;
    char *line;
    char *tmpstr;
    int pbused;
    int pbsize;
    int index;
    

    /* Ensure the correct character set is used */
    err = at_send_command("AT+CSCS=\"IRA\"", &atresponse);
    if (err < 0 || atresponse->success == 0)
        goto error;
    at_response_free(atresponse);
    atresponse=NULL;


    /* Ensure all reads are from the SIM phonebook */
    err = at_send_command("AT+CPBS=\"SM\"", &atresponse);
    if (err < 0 || atresponse->success == 0)
        goto error;
    at_response_free(atresponse);
    atresponse=NULL;


    /* Determine how many records are in the SIM phonebook */
    err = at_send_command_singleline("AT+CPBS?", "+CPBS:", &atresponse);
    if (err < 0 || atresponse->success == 0)
        goto error;

    line = atresponse->p_intermediates->line;

    err = at_tok_start(&line);
    if (err < 0)
        goto error;

    err = at_tok_nextstr(&line, &tmpstr);
    if (err < 0)
        goto error;

    err = at_tok_nextint(&line, &pbused);
    if (err < 0)
        goto error;

    err = at_tok_nextint(&line, &pbsize);
    if (err < 0)
        goto error;

    at_response_free(atresponse);
    atresponse=NULL;
    LOGI("%s: pbused=%d, pbsize=%d", __func__, pbused, pbsize);


    /* Make sure there are phone book entries to read */
    if (pbused > 0) {

        /* Allocate memory for response */
        responsepp = swimalloc(pbused * sizeof(char *),
            "requestOEMHookStringsReadSIMPB: no memory");


        /* Read the phonebook, and fill in the response */
        asprintf(&cmd, "AT+CPBR=1,%d", pbsize);
        err = at_send_command_multiline(cmd, "+CPBR:", &atresponse);
        free(cmd);
        if (err < 0 || atresponse->success == 0)
            goto error;

        atlinep = atresponse->p_intermediates;

        for (index=0; (index < pbused) && (atlinep != NULL); index++) {
            line = atlinep->line;

            err = at_tok_start(&line);
            if (err < 0)
                goto error;
            
            responsepp[index] = line;

            atlinep = atlinep->p_next;
        }

        /* LOG a message if we don't get as many as we expect */
        if ( index != pbused ) {
            LOGE("%s: mismatch pbused=%d, index=%d", __func__, pbused, index);
        }

        responsesize = index * sizeof(char *);
        
    } else {
        responsepp = NULL;
        responsesize = 0;
    }

    /* Return the PB records or empty list */
    RIL_onRequestComplete(t, RIL_E_SUCCESS, responsepp, responsesize);


finally:
    /* Free memory for response */
    free(responsepp);

    at_response_free(atresponse);
    return;

error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    goto finally;
}


/**
 *
 * QMI-Specific Hook Strings Handler. This is the main entry point
 * for handling OEM hook string requests for the QMI RIL. Functions
 * called from in this handler must be QMI-specific. 
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
 *
 *      This function has a counterpart function in the AT RIL
 *      and any additions to this may also need to be considered
 *      in that RIL. 
 *
 *      This function calls the common OEM Hookstring handler 
 *      in file swiril_oem.c located in swicommon directory
 *      if the incoming subcommand doesn't match any of the 
 *      QMI-specific cases
 */

void requestOEMHookStrings(void *data, size_t datalen, RIL_Token t)
{
    int i;
    const char **cur;
    SWI_OEMHookType subcommand;

    cur = (const char **) data;
    subcommand = atoi(*cur);

    switch(subcommand) {
            
        case SWI_CDMA_ACTIVATION:
            requestOEMHookStringsCMDAActQMI(cur, (datalen / sizeof(char *)), t);
            break;

        case SWI_DO_FACTORY_RESET:
            requestOEMHookStringsFactoryReset(cur, (datalen/sizeof(char *)), t);
            break;
            
        case SWI_PRL_UPDATE:
            requestOEMHookStringsPRLUpdate(cur, (datalen / sizeof(char *)), t);
            break;

        case SWI_PRL_CANCEL:
            requestOEMHookStringsCancelOmaDm(cur, (datalen / sizeof(char *)), t);
            break;

        case SWI_GET_SAR_STATE:
            requestOEMHookStringsGetRfSarState(cur, (datalen / sizeof(char *)), t);
            break;

        case SWI_SET_SAR_STATE:
            requestOEMHookStringsSetRfSarState(cur, (datalen / sizeof(char *)), t);
            break;

        case SWI_SET_SMS_WAKE:
            requestOEMHookStringsSetSMSWake(cur, (datalen / sizeof(char *)), t);
            break;

        case SWI_GET_ppDATAp:
            requestOEMHookStringsGetppData(cur, (datalen / sizeof(char *)), t);
            break;

        case SWI_SET_ppDATAp:
            requestOEMHookStringsSetppData(cur, (datalen / sizeof(char *)), t);
            break;

        case SWI_VALIDATE_SPC:
            requestOEMHookStringsValidateSPC(cur, (datalen / sizeof(char *)), t);
            break;

        case SWI_READ_SIM_PB:
            requestOEMHookStringsReadSIMPB(cur, (datalen / sizeof(char *)), t);
            break;
            
        default:
            /* Not a QMI-specific call, check the common possibilities */
            swicommon_requestOEMHookStrings( data, datalen, t );
            break;
    }
}
