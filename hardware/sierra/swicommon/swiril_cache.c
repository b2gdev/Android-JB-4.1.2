/**
 *
 * @ingroup swiril
 *
 * @file 
 * Provides functions for caching the response from RIL commands
 *
 * @author
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 *
 * Instructions For Adding New Cached RIL Command Item:
 *
 *  1) add to swiril_cache_table[]
 *
 *  2) add to swiril_cache_set()
 *
 *  3) add to swiril_main.c/processRequest()
 *
 *  4) call swiril_cache_RIL_onRequestComplete() or swiril_cache_set(),
 *     as appropriate, in the handler function for the RIL command
 *
 */

#include <string.h>

#define LOG_TAG "RIL"
#include "swiril_log.h"
#include <telephony/ril.h>

#include "swiril_main.h"
#include "swiril_misc.h"

/* Needed for log messages -- copied from swiril_main.c */
extern const char *requestToString(int request);


/**
 * Cache table entry
 */
struct swiril_cache_entry {
    int RIL_command;       /**< RIL command whose response is cached */
    void *responsedatap;   /**< the cached response data */
    int responsesize;      /**< the cached response length */
};


/**
 * Define the cache table.
 *
 * Any new entries should be grouped with similar commands, depending on the 
 * type of info stored, and added to the end of the respective group.  Note that
 * the first entry for the device info group is used as a marker for the end of
 * the network info group.
 */
struct swiril_cache_entry swiril_cache_table[] =  {
    /* This group of commands are network info specific, and the cache should
       be cleared whenever a network related command or event occurs, or
       whenever the device resets.
     */
    { RIL_REQUEST_REGISTRATION_STATE,             NULL, 0 },
    { RIL_REQUEST_GPRS_REGISTRATION_STATE,        NULL, 0 },
    { RIL_REQUEST_SIGNAL_STRENGTH,                NULL, 0 },
    { RIL_REQUEST_OPERATOR,                       NULL, 0 },
    { RIL_REQUEST_QUERY_NETWORK_SELECTION_MODE,   NULL, 0 },
    
    /* This group of commands are device info specific, and the cache should
       be cleared whenever the device resets.
     */
    { RIL_REQUEST_GET_IMSI,                       NULL, 0 },
    { RIL_REQUEST_DEVICE_IDENTITY,                NULL, 0 },
    { RIL_REQUEST_GET_IMEI,                       NULL, 0 },
    { RIL_REQUEST_GET_IMEISV,                     NULL, 0 },
    { RIL_REQUEST_BASEBAND_VERSION,               NULL, 0 },
};

/* Used as end marker for looping through the network info related commands
   in the cache table
 */
#define SWIRIL_CACHE_TABLE_NETWORK_INFO_END_MARKER RIL_REQUEST_GET_IMSI

/* Used for looping through the whole cache table */
#define SWIRIL_CACHE_TABLE_SIZE (sizeof(swiril_cache_table)/sizeof(struct swiril_cache_entry))


/**
 *
 * Find the entry for the RIL command in the cache table.
 *
 * @param RIL_command
 *     RIL command to search for
 *
 * @return
 *     pointer to the entry, or NULL if not found
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 *
 */
struct swiril_cache_entry * swiril_cache_findentry(int RIL_command)
{
    unsigned int idx;
    
    for (idx=0; idx<SWIRIL_CACHE_TABLE_SIZE; idx++) {
        if ( swiril_cache_table[idx].RIL_command == RIL_command ) {
            return &swiril_cache_table[idx];
        }
    }
    
    /* SWI_TBD
       Should we do something more severe than just logging the error
     */
    /* We didn't find the RIL_command in the table */
    LOGE("swiril_cache_findentry: entry for %s not found", requestToString(RIL_command));
    return NULL;
}


/**
 *
 * Get the cached response data for the RIL command
 *
 * @param RIL_command
 *     RIL command to search for
 * @param [out] responsepp
 *     pointer to the cached response data
 * @param [out] responselenp
 *     cached response length
 *
 * @return
 *     none
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 *
 */
void swiril_cache_get(int RIL_command, void **responsepp, int *responselenp)
{
    struct swiril_cache_entry *entryp;
    entryp = swiril_cache_findentry(RIL_command);
        
    if (entryp != NULL) {
        *responsepp = entryp->responsedatap;
        *responselenp = entryp->responsesize;
    } else {
        *responsepp = NULL;
        *responselenp = 0;
        LOGE("swiril_cache_get: %s is not cacheable", requestToString(RIL_command));
    }

    if (*responsepp != NULL) {
        LOGI("swiril_cache_get: valid cached data for %s", requestToString(RIL_command));
    }
}


/**
 *
 * Set the cached response for the specified cache table entry.
 *
 * The response data is a single block of bytes of size datasize.  This is
 * not necessarily the same as the response length.
 *
 * @param [out] entryp
 *     pointer to the cache table entry
 * @param [in] responsedatap
 *     pointer to the response data to write to the cache
 * @param responsesize
 *     size of the RIL command response, as stored in the cache entry
 * @param datasize
 *     size of the data to store; this may be different than responsesize
 *
 * @return
 *     none
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 *
 */
static void swiril_cache_singlevalueset(
    struct swiril_cache_entry *entryp,
    void *responsedatap,
    int responsesize,
    int datasize)
{
    /* Free the previously allocated cache memory, if any, since the new
       string could be a different size.
     */
    if ( entryp->responsedatap != NULL ) {
        free(entryp->responsedatap);
    }
    entryp->responsedatap = NULL;
    entryp->responsesize = 0;
    
    /* If there is nothing to store, nothing more to do */
    if (responsedatap == NULL) {
        return;
    }
    
    /* There is data to cache, so allocate new storage for it */
    entryp->responsedatap = 
        swimalloc(datasize, "swiril_cache_singlevalueset: can not allocate cache");
    
    /* copy the new value to the cache */
    memcpy(entryp->responsedatap, responsedatap, datasize);
    entryp->responsesize = responsesize;
}


/**
 *
 * Set the cached response for the specified RIL command.
 *
 * @param RIL_command
 *     RIL command whose response to cache
 * @param [in] responsedatap
 *     pointer to the response data to write to the cache
 * @param responsesize
 *     size of the RIL command response, as stored in the cache entry
 *
 * @return
 *     none
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 *
 */
void swiril_cache_set(int RIL_command, void *responsedatap, int responsesize)
{
    int idx;
    int datasize;
    int numelements;
    struct swiril_cache_entry *entryp;

    if (responsedatap != NULL) {
        LOGI("swiril_cache_set: updating cache for %s", requestToString(RIL_command));
    }
    else {
        LOGI("swiril_cache_set: clearing cache for %s", requestToString(RIL_command));
    }

    entryp = swiril_cache_findentry(RIL_command);
    if (entryp == NULL) {
        LOGE("swiril_cache_set: %s is not cacheable", requestToString(RIL_command));
        return;
    }

    switch (RIL_command) {

        case RIL_REQUEST_SIGNAL_STRENGTH:
        case RIL_REQUEST_QUERY_NETWORK_SELECTION_MODE:
            /* Response is just data, with responsesize giving the data size */
            swiril_cache_singlevalueset(entryp, responsedatap, responsesize, responsesize);
            break;

        case RIL_REQUEST_GET_IMSI:
        case RIL_REQUEST_GET_IMEI:
        case RIL_REQUEST_GET_IMEISV:
        case RIL_REQUEST_BASEBAND_VERSION:
            /* Response is a string pointer. Need to cache the complete string. 
               Note that the responsesize is the size of the string pointer
               and not the string itself.
             */

            /* If there is data to cache, determine the space required for the
               string. Remember to include space for the trailing '\0'
             */
            datasize=0;
            if (responsedatap != NULL) {
                datasize = strlen(responsedatap)+1;
            }

            swiril_cache_singlevalueset(entryp, responsedatap, responsesize, datasize);
            break;

        case RIL_REQUEST_DEVICE_IDENTITY:
        case RIL_REQUEST_REGISTRATION_STATE:
        case RIL_REQUEST_GPRS_REGISTRATION_STATE:
        case RIL_REQUEST_OPERATOR:
            /* Response is an array of string pointers.  Each string will need
               to be cached separately. Note that the responsesize is the size
               in bytes of the array, so need to divide by sizeof(char *) to
               get the number of elements in the array.
             */
            {
                /* local vars to avoid lots of casting below */
                char **srcpp;
                char **olddestpp;
                char **newdestpp;

                /* Free the previously allocated cache memory, if any, since the
                   new strings could be a different size.
                 */
                olddestpp = (char **)entryp->responsedatap;
                if ( olddestpp != NULL ) {
                    numelements = entryp->responsesize / sizeof(char *);
                    for (idx=0; idx<numelements; idx++) {
                        if ( olddestpp[idx] != NULL ) {
                            free(olddestpp[idx]);
                        }
                    }
                    /* entryp->responsedatap (i.e. olddestpp) is not NULL */
                    free(entryp->responsedatap);
                    entryp->responsedatap = NULL;
                    entryp->responsesize = 0;
                }
                
                /* If there is nothing to store, nothing more to do */
                if (responsedatap == NULL) {
                    return;
                }
                
                /* There is data to cache, so allocate new storage for the strings.
                   remembering to include space for the trailing '\0'. Sometimes
                   the strings are NULL, so this needs to be handled.  Once the
                   storage is allocated, copy the data over.
                 */
                srcpp = (char **)responsedatap;
                newdestpp = swimalloc(responsesize, "swiril_cache_set: can not allocate cache");

                numelements = responsesize / sizeof(char *);
                for (idx=0; idx<numelements; idx++) {
                    if (srcpp[idx] != NULL) {
                        datasize = strlen(srcpp[idx]) + 1;

                        newdestpp[idx] = 
                            swimalloc(datasize, "swiril_cache_set: can not allocate cache");

                        /* copy the new value to the cache */
                        memcpy(newdestpp[idx], srcpp[idx], datasize);
                    }
                    else {
                        newdestpp[idx] = NULL;
                    }
                }

                entryp->responsedatap = newdestpp;
                entryp->responsesize = responsesize;
                break;
            }

        default:
            LOGE("swiril_cache_set: %s is not cacheable", requestToString(RIL_command));
            break;

    }
}


/**
 *
 * Return command response, and also cache the response.
 *
 * This function is similar to the standard RIL_onRequestComplete(), and
 * is intended as an easy to use substitute that also handles caching the
 * response.  This function should only be called for RIL_E_SUCCESS, as
 * there is no result to cache otherwise.
 *
 * @param t
 *     token passed to the RIL command request
 * @param e
 *     response result code (expected to be RIL_E_SUCCESS)
 * @param [in] response
 *     pointer to the response data
 * @param responselen
 *     size of the RIL command response
 * @param RIL_command
 *     RIL command that this response is for.
 *
 * @return
 *     none
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 *
 */
void swiril_cache_RIL_onRequestComplete(
    RIL_Token t,
    RIL_Errno e,
    void *response,
    size_t responselen,
    int RIL_command)
{
    /* Cache the value for later */
    if (e == RIL_E_SUCCESS) {
        swiril_cache_set(RIL_command, response, responselen);
    } else {
        LOGE("swiril_cache_RIL_onRequestComplete: unexpected errno=%i", e);
    }

    /* Indicate the request is complete */
    RIL_onRequestComplete(t, e, response, responselen);
}


/**
 *
 * Clear ALL the cached data.
 *
 * @return
 *     none
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 *
 */
void swiril_cache_clearalldata(void)
{
    unsigned int idx;

    for (idx=0; idx<SWIRIL_CACHE_TABLE_SIZE; idx++) {
        swiril_cache_set(swiril_cache_table[idx].RIL_command, NULL, 0);
    }
}


/**
 *
 * Clear network related cached data.
 *
 * This is data that may no longer be valid after a network related RIL command
 * or network related AT unsolicited output.
 *
 * @return
 *     none
 *
 * @par Abort:<br>
 *     none
 *
 * @note
 *     none
 *
 */
void swiril_cache_clearnetworkdata(void)
{
    unsigned int idx;

    /* Loop through the table until we get the end marker, or the end of the
       table, whichever comes first
     */
    for (idx=0;
         (idx<SWIRIL_CACHE_TABLE_SIZE) && 
             (swiril_cache_table[idx].RIL_command != SWIRIL_CACHE_TABLE_NETWORK_INFO_END_MARKER);
         idx++) {
        swiril_cache_set(swiril_cache_table[idx].RIL_command, NULL, 0);
    }
}



