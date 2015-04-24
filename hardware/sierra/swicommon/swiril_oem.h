/**
 *
 * @ingroup swiril
 *
 * @file 
 * Definitions and function prototype for common OEM Hookstrings 
 * handler functions. 
 *
 * @author
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */

/**
 * list of supported RIL_REQUEST_OEM_HOOK_STRINGS subcommand
 * Note: the negative value subcommand(s) reserved for Sierra 
 * internal test purpose only 
 */
typedef enum {
    SWI_AT_COMMAND_OPT      = -2, /**< Test support for AT cmd w/ sending option */
    SWI_CURRENT_STATE       = 1,  /**< invoke RIL currentState() */
    SWI_ON_SUPPORTS         = 2,  /**< invoke RIL onSupports() */
    SWI_ON_CANCEL           = 3,  /**< invoke RIL onCancel() */
    SWI_GET_VERSION         = 4,  /**< invoke RIL getVersion() */
    SWI_CDMA_ACTIVATION     = 5,  /**< CDMA Activation procedure */
    SWI_DO_FACTORY_RESET    = 6,  /**< Reset modem to factory defaults */
    SWI_PRL_UPDATE          = 7,  /**< Client-initiated PRL update request */
    SWI_GET_SAR_STATE       = 8,  /**< Client-initiated Get RF SAR status */
    SWI_SET_SAR_STATE       = 9,  /**< Client-initiated Set RF SAR status */
    SWI_SET_SMS_WAKE        = 10, /**< Call SetSMSWake() */
    SWI_GET_ppDATAp         = 11, /**< Query ##DATA# */
    SWI_SET_ppDATAp         = 12, /**< Set ##DATA# */
    SWI_VALIDATE_SPC        = 13,  /**< Validate SPC */
    SWI_READ_SIM_PB         = 14,  /**< Read the complete PB from SIM */
    SWI_PRL_CANCEL          = 15,  /**< Cancel the current OMADM session */
} SWI_OEMHookType;

/**
 * list of supported AT command sending options
 */
typedef enum {
    SWI_AT_CMD_OPT_1   = 1, /**< AT command with sending option single line */
    SWI_AT_CMD_OPT_2   = 2, /**< AT command with sending option multi line */
    SWI_AT_CMD_OPT_3   = 3, /**< AT command with sending option multi value */
} SWI_ATSendOption;

/* External Definitions */
extern void swicommon_requestOEMHookStrings(void *data,size_t datalen,RIL_Token t);
