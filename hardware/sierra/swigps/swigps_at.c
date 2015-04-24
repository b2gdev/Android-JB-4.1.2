/* 
 * Wrapper functions for control and operation of modems using the
 * AT-RIL
 *
 * This source code is "Not a Contribution" under Apache license
 *
 * Swigps Message definitions. This file contains only data and 
 * definitions related to IPC message traffic and simple executables
 * which are strictly not allowed to generate any logging traffic.
 * This file is included in executables on the swigps and swiril_gps
 * sides.
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


#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include "swigps_sgi.h"

/*** Enumerations specific to the AT command handling ***/

/** 
 *
 * Enumerations for GPS modes for AT commands differ from those
 * defined by Android. A translation is required between android
 * modes and AT modes and this list of enumerations assists with
 * this translation
 */
typedef enum gpsModes {
    SWIGPS_AT_STANDALONE = 1,
    SWIGPS_AT_MSBASED,
    SWIGPS_AT_MSASSISTED
} gps_Modes;

/*********************************************************
 *         EXECUTABLES SPECIFIC TO THE AT-RIL
 *********************************************************/

/** 
 * 
 * Convert from Android GPS mode into Sierra AT GPS Mode
 *
 * @param androidmode
 *    - The mode requested by the Android location service
 *
 * @return
 *    - The mode required when sending the AT command
 *
 * @note
 *    If the input mode is none of the standard Android ones
 *    the default returned is for standalone operation. Should
 *    never happen.
 *
 */
static gps_Modes swigps_and_to_at( int androidmode )
{
    /* First, convert between Android GPS mode constants and AT ones */
    switch(androidmode) 
    {
        case GPS_POSITION_MODE_STANDALONE:
            return SWIGPS_AT_STANDALONE;
            break;
        case GPS_POSITION_MODE_MS_BASED:
            return SWIGPS_AT_MSBASED;
            break;
        case GPS_POSITION_MODE_MS_ASSISTED:
            return SWIGPS_AT_MSASSISTED;
            break;
    }
    return SWIGPS_AT_STANDALONE;
}

/**
 *
 * AT-Specific command to initialize the NMEA port information
 * by requesting that port name from the RIL-side. Also sends
 * the GPS's expected RIL type for validation by the RIL daemon
 *
 * @param none
 *
 * @return
 *     None
 *
 * @par Abort:<br> 
 *     None
 *
 * @note
 *     None
 */
void init_nmea( void )
{
    GpsState *statep = swigps_getstatep();

    /* Initiate the request */
    swigps_getnmeaport( GPS_RILTYPE_AT, statep->rilgpsfd );
}
    
/**
 *
 * AT-Specific command to be called when a tracking session started
 * acknowledgement from the modem has been received
 *
 * @param none
 *
 * @return
 *     None
 *
 * @par Abort:<br> 
 *     None
 *
 * @note
 *     None
 */
void session_started( void )
{
    GpsState *statep = swigps_getstatep();

    swigps_report_state(&statep->callbacks, GPS_STATUS_SESSION_BEGIN);
    statep->init = STATE_RUNNING;
}

/**
 *
 * AT-Specific command to be called when CMD_START is 
 * received by the GPS Main State thread
 *
 * @param none
 *
 * @return
 *     None
 *
 * @par Abort:<br> 
 *     None
 *
 * @note
 *     AT-RIL modems operate differently from Gobi-based
 *     ones. In a Gobi modem, simply opening the NMEA 
 *     port has the effect of starting a tracking session,
 *     but in a Sierra modem using the AT-RIL, a specific
 *     command is needed to start tracking. This function
 *     is responsible for issuing that command
 */
void start_session( void )
{
    gps_Modes atmode;
    GpsState *statep = swigps_getstatep();

    /* Convert the requested mode into an AT equivalent */
    atmode = swigps_and_to_at( statep->pdsDefs.reqmode );

    swigps_StartSession( statep->rilgpsfd,
                         atmode,
                         statep->pdsDefs.timeout,
                         statep->pdsDefs.interval, 
                         statep->pdsDefs.accuracy );
}

/**
 *
 * AT-Specific command to be called when a tracking session stopped
 * acknowledgement from the modem has been received
 *
 * @param none
 *
 * @return
 *     None
 *
 * @par Abort:<br> 
 *     None
 *
 * @note
 *     Does nothing, Session Stopping requires no followup action
 *     for AT modems
 */
void session_stopped( void )
{
    /* Do nothing */
    return;
}

/**
 *
 * AT-Specific command to be called when CMD_STOP is 
 * received by the GPS Main State thread
 *
 * @param none
 *
 * @return
 *     None
 *
 * @par Abort:<br> 
 *     None
 *
 * @note
 *     AT-RIL modems operate differently from Gobi-based
 *     ones. In a Gobi modem, simply closing the NMEA 
 *     port has the effect of stopping a tracking session,
 *     but in a Sierra modem using the AT-RIL, a specific
 *     command is needed to stop tracking. This function
 *     is responsible for issuing that command
 */
void stop_session( void )
{
    GpsState *statep = swigps_getstatep();

    swigps_StopSession( statep->rilgpsfd );
}

/**
 *
 * AT-Specific command to be called when a time injection
 * request acknowledgement from the modem has been received
 *
 * @param none
 *
 * @return
 *     None
 *
 * @par Abort:<br> 
 *     None
 *
 * @note
 *     Does nothing, time injection is not implemented for
 *     AT modems
 */
void time_injected( void )
{
    /* Do nothing */
    return;
}

/**
 *
 * AT-Specific command to be called when CMD_INJECTTIME is 
 * received by the GPS Main State thread
 *
 * @param none
 *
 * @return
 *     None
 *
 * @par Abort:<br> 
 *     None
 *
 * @note
 *     Time injection is not implemented by AT modems
 */
void inject_time( void )
{
    /* do nothing */
    return;
}

/**
 *
 * AT-Specific command to be called when a response to get
 * the default tracking settings has been received from the
 * modem
 *
 * @param none
 *
 * @return
 *     None
 *
 * @par Abort:<br> 
 *     None
 *
 * @note
 *     None
 */
void defaults_received( void )
{
    /* Do nothing */
    return;
}

/**
 *
 * AT-Specific command to call when asking an AT modem for
 * its default tracking settings
 *
 * @param none
 *
 * @return
 *     None
 *
 * @par Abort:<br> 
 *     None
 *
 * @note
 *     None
 */
void get_defaults( void )
{
    /* Do nothing */
    return;
}

/**
 *
 * AT-Specific command to be called when a response to set
 * the default tracking settings has been received from the
 * modem
 *
 * @param none
 *
 * @return
 *     None
 *
 * @par Abort:<br> 
 *     None
 *
 * @note
 *     None
 */
void defaults_set( void )
{
    /* Do nothing */
    return;
}

/**
 *
 * AT-Specific command to call when setting the default tracking
 * values on an AT modem 
 *
 * @param none
 *
 * @return
 *     None
 *
 * @par Abort:<br> 
 *     None
 *
 * @note
 *     The Sierra Wireless AT command-based modem uses the same
 *     command to start a tracking session as to set the default
 *     tracking values. If no session is currently running (easy
 *     case) then the defaults are stored locally in the state
 *     structure and nothing needs to be done
 *
 *     If tracking is enabled when the defaults are changed then
 *     the session must be stopped and restarted with the new 
 *     settings
 */
void set_defaults( void )
{
    gps_Modes atmode;
    GpsState *statep = swigps_getstatep();

    /* Convert the requested mode into an AT equivalent */
    atmode = swigps_and_to_at( statep->pdsDefs.reqmode );

    /* Only need to call the RIL side if a
     * tracking session already in progress
     */
    if( statep->tracking )
    {
        /* Need to reconfigure the active tracking session */
        swigps_setdef( statep->rilgpsfd,
                       atmode,
                       statep->pdsDefs.timeout,
                       statep->pdsDefs.interval,
                       statep->pdsDefs.accuracy, 
                       statep->tracking);
    }
    return;
}

/**
 *
 * AT-Specific command to be called when a response to get
 * the AGPS settings from the modem have been received
 *
 * @param none
 *
 * @return
 *     None
 *
 * @par Abort:<br> 
 *     None
 *
 * @note
 *     None
 */
void agps_received( void )
{
    /* Do nothing */
    return;
}

/**
 *
 * AT-Specific command to call when asking an AT modem for
 * its AGPS settings
 *
 * @param none
 *
 * @return
 *     None
 *
 * @par Abort:<br> 
 *     None
 *
 * @note
 *     None
 */
void get_agps( void )
{
    /* Do nothing */
    return;
}

/**
 *
 * AT-Specific command to be called when a response to set
 * the AGPS settings on the modem have been received
 *
 * @param none
 *
 * @return
 *     None
 *
 * @par Abort:<br> 
 *     None
 *
 * @note
 *     None
 */
void agps_set( void )
{
    /* Do nothing */
    return;
}

/**
 *
 * AT-Specific command to call when setting AGPS values on
 * an AT modem
 *
 * @param none
 *
 * @return
 *     None
 *
 * @par Abort:<br> 
 *     None
 *
 * @note
 *     This function does nothing to configure the SUPL URL.
 *     When Sierra modules leave the factory the OEM customer
 *     has usually requested they be pre-configured for a 
 *     variety of settings including the SUPL URL. This value
 *     tends to be carrier-specific too. Since Sierra modems
 *     hold their values in NV RAM, there is no need to 
 *     reconfigure the setting every time this GPS module is
 *     started. 
 * 
 *     NOTE: In order for this function to work, OEM customers
 *     would need to specify an unlock password for CnD commands
 *     as a PRI option instead of/or in addition to the SUPL URL,
 *     and the code in swirilat_gps_at.c would need to be 
 *     modified to add the password. If those conditions are 
 *     met then this function could be altered to call
 *     swigps_setagps().
 */
void set_agps( void )
{
    /* Do nothing */
    return;
}

