/* 
 * This source code is "Not a Contribution" under Apache license
 *
 * swigps functions for Gingerbread and later versions of Android.
 * 
 * Copyright 2011, Sierra Wireless, Inc.
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
 */

#define  LOG_TAG  "gps_ginp"

#include "swiril_log.h"
#include "swigps_sgi.h"

#include <hardware/gps.h>

#define  GPS_DEBUG  0

#define  DFR(...)   LOGD(__VA_ARGS__)

#if GPS_DEBUG
#  define  D(...)   LOGD(__VA_ARGS__)
#else
#  define  D(...)   ((void)0)
#endif


/**
 *
 * swigps state control block initialization routine for Gingerbread
 * and newer versions of Android
 * 
 * @param[in] pstate
 *     Pointer to swigps state control block
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *      none
 *
 */
void swigps_init_state( GpsState *pstate )
{
    /* structures defined in gps.h in Gingerbread and beyond
     * have a size field that must be initialized to the size
     * of the corresponding structure 
     */
    D("%s started", __FUNCTION__);

    /* Reader Structure */
    pstate->reader.fix.size = sizeof(GpsLocation);

    /* Satellite status structure */
    pstate->reader.sv_status.size = sizeof(GpsSvStatus);
}

/**
 *
 * Swigps main state thread initialization routine for Gingerbread and beyond
 * 
 * Called with the GPS State thread starts but before its infinite loop begins
 * and performs any version-specific initialization required
 *
 * @param [in] statep
 *     Pointer to the swigps state control block structure
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
void swigps_init_state_thread( GpsState *pstate ) 
{
    /* Send the capabilities list back to the upper layer */
    if( pstate->callbacks.set_capabilities_cb ) {
        pstate->callbacks.set_capabilities_cb(
                   GPS_CAPABILITY_SCHEDULING |
                   GPS_CAPABILITY_MSB        |
                   GPS_CAPABILITY_MSA        |
                   GPS_CAPABILITY_SINGLE_SHOT);
        D("%s capabilities reported", __FUNCTION__);
    }
	return;
}

/**
 * 
 * Initialize the Position Determination default values.
 *
 * @param [in] pstate
 *    pointer to the gps state thread control block
 *   
 * @return
 *    none
 *
 * @note
 *    none
 */
void swigps_init_pdsDefs( GpsState *pstate ) 
{

    D("%s started", __FUNCTION__);

    pstate->fix_freq = 1;
    pstate->pdsDefs.accuracy  = 10;
    pstate->pdsDefs.interval  = 1;
    pstate->pdsDefs.timeout  = 255;
    pstate->pdsDefs.reqmode  = GPS_POSITION_MODE_STANDALONE;
    /* Recurrence defaults to periodic. For gingerbread 
     * and later versions of Android, an interface function
     * might change this to single-shot
     */
    pstate->pdsDefs.recurrence = GPS_POSITION_RECURRENCE_PERIODIC;
}

/**
 *
 * GpsCallbacks structure size validation for Gingerbread
 * 
 * This function ensures the size field of the GpsCallbacks
 * structure passed when the interface was opened is the 
 * same size as we're expecting. If not an error is returned
 *
 * @param[in] callbackp
 *     Pointer to GpsCallback structure passed to us when
 *     the GPS module was started up
 *
 * @return
 *      0 - The sizes match
 *     -1 - There is a size mismatch
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *
 */
int swigps_checkGpsCallbacks( GpsCallbacks *Callbackp )
{
    D("%s started", __FUNCTION__ );
    if( Callbackp->size != sizeof(GpsCallbacks) ) {
        LOGE("%s, GpsCallbacks size mismatch: theirs=%d, ours=%d", 
             __FUNCTION__, Callbackp->size, sizeof(GpsCallbacks) );
        return -1;
    }
    return 0;
}

/**
 * 
 * Allows the application to configure the GPS engine in the
 * modem, specifying various parameters. NOTE, not all of the
 * calling arguments have a corresponding QMI setting so these
 * are ignored
 *
 * @param
 *    mode [in]       
 *       Standalone, MS based or MS assisted
 * @param
 *    recurrence [in] 
 *       GPS_POSITION_RECURRENCE_PERIODIC or
 *       GPS_POSITION_RECURRENCE_SINGLE
 * @param
 *    min_interval [in]
 *       Time between fixes in milliseconds
 * @param 
 *    pref_accuracy [in]
 *       Preferred fix accyracy in meters
 *                         frequency in seconds
 * @param
 *    pref_time [in]
 *       Requested time to first fix in ms (Ignored by swigps)
 * @note
 *    The min_interval argument is used to set the timer thread's
 *    fix frequency as well as being sent to the modem's Position
 *    Determination Service (PDS) as the "interval" argument. The
 *    gps_timer_thread within the swigps package acts as a filter
 *    by sleeping for the duration of the interval and this ensures
 *    reports are send at the requested rate even if the modem 
 *    sends reports more frequently. Might be able to improve this
 *    once we understand the modem's behaviour better
 *
 *    The main GPS state thread can be changed for Gingerbread+
 *    by not needing to request the default PDS settings at 
 *    startup because all the settings (except timeout) are 
 *    received by this interface function
 *
 *    There are two bandaids below, marked with TODO and commented.
 *
 */
static int swigps_set_position_mode(GpsPositionMode mode, 
                                    GpsPositionRecurrence recurrence,
                                    uint32_t min_interval, 
                                    uint32_t pref_accuracy,
                                    uint32_t pref_time )
{
    /* Converted from ms to seconds */
    uint32_t interval;

    GpsState *s = swigps_getstatep();

    D("%s with mode: %d, recurrence %d, interval %d, accuracy %d", 
       __FUNCTION__, mode, recurrence, min_interval, pref_accuracy );

    if(!s->init) {
        D("%s: called uninitialized. State: %x\n", __FUNCTION__, s->init );
        return -1;
    }

    /* Saving these to the state structure from
     * outside the main state thread won't cause 
     * problems. 
     */
    s->pdsDefs.reqmode = mode;
    s->pdsDefs.recurrence = recurrence;

    /* TODO The test here is to make sure the accuracy is non-zero.
     * On Gobi modems this is an invalid accuracy value and 
     * the mode setting request will fail with an error 1048.
     * The question is, should we return error from this func
     * or force a valid accuracy as we do below?? TODO
     */
    if(pref_accuracy)
        s->pdsDefs.accuracy = pref_accuracy;
    else
        s->pdsDefs.accuracy = 10;

    /* Convert the interval into seconds */
    interval = min_interval / 1000;
    /* TODO - The same problem with the accuracy above also 
     * applies to the interval. An interval of zero is 
     * rejected on Gobi modems so should we force it to 
     * a value of 1 or return an error? TODO
     */
    s->fix_freq = (!interval?1:interval); /* Never less than 1 second */
    s->pdsDefs.interval = s->fix_freq;

    /* Kick the GPS State thread */
    gps_state_setmode( s );

    return 0;
}

/**
 * 
 * GPS Status Callback function - called from gps state thread
 * whenever there is a change in the modem's state that needs
 * to be reported to the GPS Location Service
 *
 * @param
 *    cbkfp [in]
 *        Pointer to the callback function originally supplied by
 *        the location service when the swigps interface was opened
 * @param
 *    state [in]
 *        The new state to be reported to the gps location service
 * 
 * @return
 *    none
 * 
 * @note
 *    The GpsStatus structure differs from Froyo to the newer Android 
 *    releases
 */
void swigps_report_state( GpsCallbacks *cbklistp, GpsStatusValue state )
{
    GpsStatus gps_status;

    D("%s started", __FUNCTION__);

    gps_status.size = sizeof(GpsStatus);
    gps_status.status = state;

    if (cbklistp->status_cb) {
        cbklistp->status_cb(&gps_status);
        D("gps status callback: 0x%x", state);
    }
}

/**
 * 
 * Create the timer thread for Gingerbread+ android versions
 * 
 * @param
 *    statep [in]
 *        Pointer to this package's main runtime control block
 * 
 * @return
 *     0 - Always
 * 
 * @note
 *    This function must be called during the initial call to the 
 *    swigps_init() function, which is really the context of the 
 *    Gps Location Service
 */
int swigps_create_timer_thread( GpsState *statep )
{

    D("%s, started", __FUNCTION__);

    /* Use the callback provided for deferred handling thread creation */
    statep->swigps_tmr_thread = 
            statep->callbacks.create_thread_cb("swigps_timer",
                                                (void (*)(void*))gps_timer_thread, 
                                                (void *)statep);
    return 0;
}


/**
 * 
 * Create the main state thread for Gingerbread+ android versions
 * 
 * @param
 *    statep [in]
 *        Pointer to this package's main runtime control block
 * 
 * @return
 *     0 - Always
 * 
 * @note
 *    This function must be called during the initial call to the 
 *    swigps_init() function, which is really the context of the 
 *    Gps Location Service
 */
int swigps_create_state_thread( GpsState *statep )
{

    D("%s, started", __FUNCTION__);

    /* Use the callback provided for deferred handling thread creation */
    statep->swigps_thread = 
            statep->callbacks.create_thread_cb("swigps_state",
                                                (void (*)(void*))gps_state_thread, 
                                                (void *)statep);
    return 0;
}

/**
 * Contains pointers to interface functions as prescribed by
 * gps.h in typedef "GpsInterface". 
 *
 * @Note
 *   See individual function headers for additional information
 *
 */
const GpsInterface  swiGpsInterface = {
    sizeof( GpsInterface ),
    swigps_init,
    swigps_start,
    swigps_stop,
    swigps_cleanup,
    swigps_inject_time,
    swigps_inject_location,
    swigps_delete_aiding_data,
    swigps_set_position_mode,
    swigps_get_extension,
};

/**
 * Contains pointers to interface functions as prescribed by
 * gps.h in typedef "AGpsInterface". 
 *
 * @Note
 *   See individual function headers for additional information
 *
 */
const AGpsInterface swiAgpsInterface = {
    sizeof( AGpsInterface ),
    swiagps_init,
    swiagps_open,
    swiagps_closed,
    swiagps_failed,
    swiagps_set_server 
};

