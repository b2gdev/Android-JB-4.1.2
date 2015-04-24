/**
 *
 * @ingroup swiril
 *
 * @file 
 * Prototypes for cache related functions
 *
 * @author
 * Copyright: © 2010 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */


void swiril_cache_get(int RIL_command, void **responsepp, int *responselenp);
void swiril_cache_set(int RIL_command, void *responsedatap, int responsesize);

void swiril_cache_RIL_onRequestComplete(
    RIL_Token t,
    RIL_Errno e,
    void *response,
    size_t responselen,
    int RIL_command);

void swiril_cache_clearalldata(void);
void swiril_cache_clearnetworkdata(void);


