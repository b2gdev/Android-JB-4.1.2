/* 
 * Wrapper functions for control and operation of modems using the
 * QMI-RIL
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

/*********************************************************
 *         EXECUTABLES SPECIFIC TO THE QMI-RIL
 *********************************************************/

/**
 *
 * QMI-Specific command to initialize the NMEA port information
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
    swigps_getnmeaport( GPS_RILTYPE_QMI, statep->rilgpsfd );
}

/**
 *
 * QMI-Specific command to be called when a tracking session started
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
 *     Nothing to do for QMI devices
 */
void session_started( void )
{
    /* Do nothing */
    return;
}

/**
 *
 * QMI-Specific command to be called when CMD_START is 
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
 *     On QMI devices, a tracking session is started as soon as the
 *     NMEA port is opened and that should already be done by the 
 *     time this function is called
 */
void start_session( void )
{
    GpsState *statep = swigps_getstatep();

    /* Acknowledge to the GPS Service */
    swigps_report_state(&statep->callbacks, GPS_STATUS_SESSION_BEGIN);
    statep->init = STATE_RUNNING;
}

/**
 *
 * QMI-Specific command to be called when a tracking session stopped
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
 *     Nothing to do for QMI devices
 */
void session_stopped( void )
{
    /* Do nothing */
    return;
}

/**
 *
 * QMI-Specific command to be called when CMD_STOP is 
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
 *     On QMI devices, closing the NMEA port causes a tracking 
 *     session to be stopped so this function does nothing
 */
void stop_session( void )
{
    /* Do nothing */
    return;
}

/**
 *
 * QMI-Specific command to be called when a time injection
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
 *     Does nothing, no followup action required for QMI modems
 *     when time injection is complete
 */
void time_injected( void )
{
    /* Do nothing */
    return;
}

/**
 *
 * QMI-Specific command to be called when CMD_INJECTTIME is 
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
 *     None
 */
void inject_time( void )
{
    GpsState *statep = swigps_getstatep();
    
    swigps_injtime( statep->rilgpsfd,
                    statep->injectTime.time,
                    statep->injectTime.uncertainty);
}

/**
 *
 * QMI-Specific command to be called when a response to get
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
 * QMI-Specific command to call when asking a QMI modem for
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
 * QMI-Specific command to be called when a response to set
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
 * QMI-Specific command to call when setting the default tracking
 * values on a QMI modem 
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
void set_defaults( void )
{
    GpsState *statep = swigps_getstatep();

    swigps_setdef( statep->rilgpsfd,
                   statep->pdsDefs.reqmode,
                   statep->pdsDefs.timeout,
                   statep->pdsDefs.interval,
                   statep->pdsDefs.accuracy, 
                   statep->tracking
                 );
    return;
}

/**
 *
 * QMI-Specific command to be called when a response to get
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
 * QMI-Specific command to call when asking QMI modem for
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
 * QMI-Specific command to be called when a response to set
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
 * QMI-Specific command to call when setting AGPS values on
 * an QMI modem
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
void set_agps( void )
{
    GpsState *statep = swigps_getstatep();

    /* Format and send the request to the modem */
    swigps_setagps( statep->rilgpsfd,
                    statep->agpsInfo.reqipaddr,
                    statep->agpsInfo.reqport,
                    statep->agpsInfo.requrl);
}

