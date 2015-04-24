/* 
 * This source code is "Not a Contribution" under Apache license
 *
 * Sierra Wireless RIL GPS Client
 *
 * Copyright (C) 2011 Sierra Wireless, Inc.
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
 * Author: Sierra Wireless, Inc.
 */

#include <cutils/sockets.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <fcntl.h>
#include <pthread.h>
#include <alloca.h>
#include <getopt.h>
#include <sys/socket.h>
#include <termios.h>
#include <cutils/properties.h>

#include "swiril_gps.h"

#define LOG_TAG "gps_ril"
#include "swiril_log.h"

#define RILGPS_VERSION_STRING  "Sierra Rilgps V0.3"

#define MAX_AT_RESPONSE 0x1000

#define timespec_cmp(a, b, op)         \
        ((a).tv_sec == (b).tv_sec    \
        ? (a).tv_nsec op (b).tv_nsec \
        : (a).tv_sec op (b).tv_sec)


/*** Declarations ***/

/*** Static Variables ***/

/* swiril_gps thread storage */
pthread_t s_tid_swiril_gps;

/* GPS socket storage */
static int s_fdGps = -1;

/**
 *
 * Initialize the GPS socket
 *
 * @param
 *     none
 *
 * @return
 *     false - Could not start the socket
 *     true  - Success
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 */
bool initsock( void )
{
    int ret;

    /* Get the predefined socket */
    s_fdGps = android_get_control_socket(SOCKET_NAME_SWIGPS);

    /* Validate the result */
    if (s_fdGps < 0) {
        LOGE("Failed to get socket '" SOCKET_NAME_SWIGPS "' errno:%d", errno);
        return false;
    }

    /* Next step - listen for a connection */
    ret = listen(s_fdGps, 4);

    /* Validate the result */
    if (ret < 0) {
        LOGE("Failed to listen on ril debug socket '%d': %s",
             s_fdGps, strerror(errno));
        return false;
    }

    LOGI("Got fd %d for ctl", s_fdGps );

    return true;
}

/**
 *
 * swiril_gps thread entry point. Implements the swiril_gps
 * agent running under the RIL daemon on behalf of swigps
 * which runs under the context of a different process
 * 
 * @param[in] arg 
 *     Not used, should be NULL from sender
 *
 * @return
 *     NULL
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     none
 */
static void *swiril_gps( void *arg )
{
    struct sockaddr_un peeraddr;
    socklen_t socklen = sizeof(peeraddr);
    int gpsfd, i;
    size_t buflength;
    
    /* Open the socket that connects this
     * thread with the swigps engine
     */
    char * tty_name = NULL;
    static char gpsbuf[512];

    LOGI("swiril_gps Thread starting!");

    /* Clear out the receive buffer */
    memset( gpsbuf, 0, sizeof(gpsbuf) ); 

    /* Initialize the socket used to receive from swigps */
    if( !initsock() )
    {
        LOGE("Swiril_gps thread terminating");
        return NULL;
    }

    /* Start of the main loop for this thread */
    for( ;; ) {

        gpsfd = accept( s_fdGps, (struct sockaddr *) &peeraddr, &socklen );
    
        /* Validate the connection */
        if( gpsfd < 0 ) {
            LOGE("Error on accept() errno: %d", errno );

            /* Go back around and wait for another connection */
            continue;
        }

        LOGE("Accepted %d for GPS IPC", gpsfd );

        /* Execute the transaction handler while 
         * the socket remains valid (no errors)
         */
        handle_swigps( gpsfd );

    }
 
    return NULL;
}

/**
 *
 * Initialize the RILGPS functions
 *
 * @param[in] riltype
 *     The type of RIL driving this instance of the GPS thread, 
 *     GPS_RILTYPE_QMI or
 *     GPS_RILTYPE_AT
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
void RILGPS_Init( GPS_RILTYPE riltype )
{
    pthread_attr_t attr;

    LOGE("%s", __func__ );

    /* Set the RIL type in the place that matters */
    initializeRILType( riltype );

    /* Create the swiril_gps thread */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&s_tid_swiril_gps, &attr, swiril_gps, NULL );
}
