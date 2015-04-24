/* 
 * This source code is "Not a Contribution" under Apache license
 *
 * Swigps message builder and parser. Request messages (sgi -> sgr)
 * are created in this file and response packets (sgr->sgi) are 
 * parsed in this file. Complimentary functions are located in 
 * swigps_inb.c
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

#include <telephony/ril.h>
#include <errno.h>
#include <pthread.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <math.h>
#include <time.h>
#include <semaphore.h>
#include <signal.h>
#include <unistd.h>
#include "swiril_main.h"
#include "swiril_misc.h"
#include "swiril_gps.h"
#include "swims_ossdkuproto.h"

#define  LOG_TAG  "gps_inb"

#include "swiril_log.h"
#include <cutils/sockets.h>

#define  GPS_DEBUG  0

#define  DFR(...)   LOGD(__VA_ARGS__)

#if GPS_DEBUG
#  define  D(...)   LOGD(__VA_ARGS__)
#else
#  define  D(...)   ((void)0)
#endif


/* Records the RIL type, AT or QMI, this code  is running under */
GPS_RILTYPE s_riltype;

/* Forward Declaration */
swigpsTypeHandler gpsTypeHandler[SWIGPS_MSGTYPE_MAXVAL];


/*****                      *****
 *****   Packet Parsers     *****
 *****                      *****/


/**
 *
 * Initialize the RIL type for future comparison against what the GPS
 * vendor specific code expects to be running. Could be either the 
 * AT or the QMI RIL and the GPS vendor specific module's dynamic 
 * library must match. The remedy is to ensure the GPS and RIL library
 * are of the same type
 * 
 * @param[in] riltype
 *     The type of RIL the caller is
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *      No validation is performed, it is expected the caller will
 *      get it right. An error will be logged if there's a mismatch
 *      This function must be called before any others in this file
 */
void initializeRILType( GPS_RILTYPE riltype ) 
{
    LOGI("%s with riltype: %d", __func__, riltype );
    s_riltype = riltype;
}

/**
 *
 * Handle incoming request to fetch the NMEA tty port name for the swigps
 * package.
 * 
 * @param[in] bufp
 *     Pointer to the variable part of the buffer in which the incoming 
 *     packet was stored. The header has already been parsed prior to this
 *     function being called. 
 * @param [in] type:
 *     The type value that has been parsed out of the incoming buffers' 
 *     header
 * @param[in] length
 *     The length already parsed from the length field in the header. 
 *     Range-checked by this function as part of validation
 * @param[in] uparm
 *     arbitrary void * value. User defined variable - the file descriptor
 *     for the socket to send responses back to the sgi side. Passed to
 *     the enqueued function for when it needs to send the response back
 *     to the swigps daemon
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *      This request handler is unique in this file because it handles the
 *      request and sends a response all in one. Other functions in this file
 *      dispatch control to the code in swiril_gps_qmi.c by enqueing a request
 *      to the RIL daemon.
 *
 */
void reqGetNmeaPort( unsigned char* bufp, 
                     swigpsMsgType type,
                     size_t length, 
                     void *uparm)
{
    char *errp = NULL;
    char *outbufp;
    char respbuf[SWIGPS_MSGSIZE_HEADER+SWIGPS_MSGSIZE_RESP_MAX_GETNMEAPORT];
    char errmsgbuf[ERRMSGSIZE];
    char *gpstty_name;
    int i, namesize=0, written;
    int ret;
    int fd = (int) uparm;
    GPS_RILTYPE riltype;

    D("reqGetNmeaPort: using filedesc: %d", fd);

    /* Validate that the length is correct */
    if( length >= msgProperties[type].minlength && 
        length <= msgProperties[type].maxlength    ) {

        outbufp = respbuf;

        /* Validate the incoming RIL Type */
        riltype = piget16( &bufp );

        LOGI("%s: Got riltype %d", __func__, riltype);

        /* Validate the caller's RIL type against ours */
        if( riltype == s_riltype ) {
            LOGI("%s: GPS/RIL Types match", __func__ );
        }
        else {
            LOGE("%s: ERROR - WRONG GPS VENDOR LIBRARY IN USE", __func__);
        }

        /* Fetch the gps device name if a modem with NMEA 
         * support has been detected. Build a response and
         * check for validity before responding to swigps 
         */
        gpstty_name = swims_ossdkgetgpsifname();
        if(gpstty_name != NULL ) {
            /* Define the GPS ttyUSB number */
            LOGI("Using %s for NMEA", gpstty_name);

            /* Note - the length does not include a NULL terminator */
            namesize = strlen(gpstty_name);
            
            /* For consistency with all other packets to swigps, 
             * use a QM error code to report the result
             */
            errp = swigps_makhdr( SWIGPS_MSGTYPE_RESP_GETNMEAPORT, 
                                  namesize, SWIGPS_SUCCESS,
                                  (unsigned char *) outbufp, 
                                  sizeof(respbuf), &written );

            /* Advance the buffer pointer to the variable-length part */
            outbufp = respbuf + written;

            /* Put the serial port name into the buffer */
            for(i=0; i<namesize; i++)
                *outbufp++ = *gpstty_name++;
        }
        else {
            /* Not sending an additional error string for now. 
             * Logging one on this side should be sufficient
             */
            LOGE("No GPS device name available");
            errp = swigps_makhdr( SWIGPS_MSGTYPE_RESP_GETNMEAPORT, 
                                  0, SWIGPS_ERR_NO_NMEA_TTY,
                                  (unsigned char *)outbufp, 
                                  sizeof(respbuf), &written );
        }

        /* Ensure the header got built */
        if(errp) {
            LOGE("getNmeaPort error: %s", errp);
            return;
        }

        D("getNmeaPort: port name has %d bytes", namesize );

        /* Send the reply to the swigps side */
        if ( socksend( fd, respbuf, (outbufp - respbuf), errmsgbuf ) ) {
            LOGE("%s", errmsgbuf);
        }

        return;
    }
    /* Do nothing, log an error and send no response */
    LOGE("reqGetNmeaPort: Length error on type: %d. Recv'd length: %d",type,length);
}

/**
 *
 * Common routine to process incoming requests for service from swigps.c
 * 
 * @param[in] funcp
 *     Pointer to function to be passed to the RIL daemon via call to 
 *     enqueueRILEvent()
 * @param[in] bufp
 *     Pointer to the variable part of the buffer in which the incoming 
 *     packet was stored. The header has already been parsed prior to this
 *     function being called.
 * @param [in] type:
 *     The type value that has been parsed out of the incoming buffers' 
 *     header
 * @param[in] length
 *     The length already parsed from the length field in the header. 
 *     Range-checked by this function as part of validation
 * @param[in] uparm
 *     arbitrary void * value. User defined variable - the file descriptor
 *     for the socket to send responses back to the sgi side. Passed to
 *     the enqueued function for when it needs to send the response back
 *     to the swigps daemon
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
void packnSend( void (*rilfuncp)( void *),
                  unsigned char* bufp, 
                  swigpsMsgType type,
                  size_t length, 
                  void *uparm)
{
    unsigned char *argp, *largp;
    unsigned int ipaddr;
    unsigned int port;
    unsigned char urllength;
    int fd = (int) uparm;

    D("%s filedesc: %d", __func__, fd);

    /* Validate that the length is correct */
    if( length >= msgProperties[type].minlength && 
        length <= msgProperties[type].maxlength ) {

        D("%s: type: %d and length %d from swigps",__func__, type, length );

        /* Instead of unpacking the args from the input buffer
         * copy the buffer into some allocated memory and let
         * the enqueued function unpack it
         */
        argp = swimalloc( sizeof(fd)+length,
                         "packnSend: Allocation failed");

        /* Make a copy of the arg buffer pointer */
        largp = argp;

        /* First item in is the file descriptor */
        piput32( fd, &largp );

        /* Then the variable length portion */
        memcpy( largp, bufp, length );

        /* Dispatch the request to the RIL Event handler and we're done */
        enqueueRILEvent(RIL_EVENT_QUEUE_NORMAL, 
                        rilfuncp, 
                        (void *) argp, 
                        NULL );
        return;

    }
    /* Can only log an error occurred and move on */
    LOGE("%s: length error on type: %d. Recv'd length: %d",
         __func__,type,length);
}

/**
 *
 * Handle incoming request for PDS default settings from swigps 
 * 
 * @param[in] bufp
 *     Pointer to the variable part of the buffer in which the incoming 
 *     packet was stored. The header has already been parsed prior to this
 *     function being called.
 * @param [in] type:
 *     The type value that has been parsed out of the incoming buffers' 
 *     header
 * @param[in] length
 *     The length already parsed from the length field in the header. 
 *     Range-checked by this function as part of validation
 * @param[in] uparm
 *     arbitrary void * value. User defined variable - the file descriptor
 *     for the socket to send responses back to the sgi side. Passed to
 *     the enqueued function for when it needs to send the response back
 *     to the swigps daemon
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
void reqGetDef( unsigned char* bufp, 
                swigpsMsgType type,
                size_t length, 
                void *uparm)
{
    int fd = (int) uparm;

    D("reqGetPds: using filedesc: %d", fd);

    packnSend( rilgpsGetDef, bufp, type, length, uparm );
}

/**
 *
 * Handle incoming request to change the default PDS session settings
 * 
 * @param[in] bufp
 *     Pointer to the variable part of the buffer in which the incoming 
 *     packet was stored. The header has already been parsed prior to this
 *     function being called.
 * @param [in] type:
 *     The type value that has been parsed out of the incoming buffers' 
 *     header
 * @param[in] length
 *     The length already parsed from the length field in the header. 
 *     Range-checked by this function as part of validation
 * @param[in] uparm
 *     arbitrary void * value. User defined variable - the file descriptor
 *     for the socket to send responses back to the sgi side. Passed to
 *     the enqueued function for when it needs to send the response back
 *     to the swigps daemon
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *      Instead of unpacking the incoming buffer, it and the user parm
 *      value are copied into a newly allocated buffer and sent along
 *      with the enqueued function for handling inside the RIL daemon
 *
 */
void reqSetDef( unsigned char* bufp, 
                swigpsMsgType type,
                size_t length, 
                void *uparm)
{
    unsigned char *argp, *largp;
    unsigned char mode;
    unsigned char timeout;
    int interval, accuracy;
    int fd = (int) uparm;

    D("%s: using filedesc: %d", __func__, fd);

    packnSend( rilgpsSetDef, bufp, type, length, uparm );
}

/**
 *
 * Handle incoming request to get the AGPS information from the modem
 * 
 * @param[in] bufp
 *     Pointer to incoming packet containing the get AGPS request
 * @param [in] type:
 *     The value from the incoming packet's header which triggered a 
 *     call to this function
 * @param[in] length
 *     The length already parsed from the length field in the header. 
 *     Range-checked by this function as part of validation
 * @param[in] uparm
 *     arbitrary void * value. User defined variable - the file descriptor
 *     for the socket to send responses back to the sgi side. Passed to
 *     the enqueued function for when it needs to send the response back
 *     to the swigps daemon
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
void reqGetAgps( unsigned char* bufp, 
                swigpsMsgType type,
                size_t length, 
                void *uparm)
{
    int fd = (int) uparm;

    D("%s: using filedesc: %d", __func__, fd);

    packnSend( rilgpsGetAgps, bufp, type, length, uparm );
}

/**
 *
 * Handle incoming request to change the modem's AGPS server settings
 * 
 * @param[in] bufp
 *     Pointer to storage holding the incoming message from swigps
 *     Points to the first byte of the variable length part
 * @param [in] type:
 *     The value contained in the header's type field for the packet 
 *     that triggered a call to this function
 * @param[in] length
 *     The length of the variable length portion of this message
 * @param[in] uparm
 *     arbitrary void * value. User defined variable - the file descriptor
 *     for the socket to send responses back to the sgi side. Passed to
 *     the enqueued function for when it needs to send the response back
 *     to the swigps daemn
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *      Instead of unpacking the incoming buffer, it and the user parm
 *      value are copied into a newly allocated buffer and sent along
 *      with the enqueued function for handling inside the RIL daemon
 *
 */
void reqSetAgps( unsigned char* bufp, 
                swigpsMsgType type,
                size_t length, 
                void *uparm)
{
    unsigned char *argp, *largp;
    unsigned int ipaddr;
    unsigned int port;
    unsigned char urllength;
    int fd = (int) uparm;

    D("%s: using filedesc: %d", __func__, fd);

    packnSend( rilgpsSetAgps, bufp, type, length, uparm );
}

/**
 *
 * Handle incoming time injection requests
 * 
 * @param[in] bufp
 *     Pointer to storage holding the incoming message from swigps
 *     Points to the first byte of the variable length part
 * @param [in] type:
 *     The value contained in the header's type field for the packet 
 *     that triggered a call to this function
 * @param[in] length
 *     The length of the variable length portion of this message
 * @param[in] uparm
 *     arbitrary void * value. User defined variable - the file descriptor
 *     for the socket to send responses back to the sgi side. Passed to
 *     the enqueued function for when it needs to send the response back
 *     to the swigps daemn
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *      Instead of unpacking the incoming buffer, it and the user parm
 *      value are copied into a newly allocated buffer and sent along
 *      with the enqueued function for handling inside the RIL daemon
 *
 */
void reqInjTime( unsigned char* bufp, 
                 swigpsMsgType type,
                 size_t length, 
                 void *uparm)
{
    unsigned char *argp, *largp;
    unsigned int ipaddr;
    unsigned int port;
    unsigned char urllength;
    int fd = (int) uparm;

    D("%s: using filedesc: %d", __func__, fd);

    packnSend( rilgpsInjTime, bufp, type, length, uparm );
}

/**
 *
 * Handle requests to stop a current GPS tracking session
 * 
 * @param[in] bufp
 *     Pointer to storage holding the incoming message from swigps
 *     Points to the first byte of the variable length part
 * @param [in] type:
 *     The value contained in the header's type field for the packet 
 *     that triggered a call to this function
 * @param[in] length
 *     The length of the variable length portion of this message
 * @param[in] uparm
 *     arbitrary void * value. User defined variable - the file descriptor
 *     for the socket to send responses back to the sgi side. Passed to
 *     the enqueued function for when it needs to send the response back
 *     to the swigps daemn
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *      Instead of unpacking the incoming buffer, it and the user parm
 *      value are copied into a newly allocated buffer and sent along
 *      with the enqueued function for handling inside the RIL daemon
 *
 */
void reqStop( unsigned char* bufp, 
              swigpsMsgType type,
              size_t length, 
              void *uparm)
{
    unsigned char *argp, *largp;
    unsigned int ipaddr;
    unsigned int port;
    unsigned char urllength;
    int fd = (int) uparm;

    D("%s: using filedesc: %d", __func__, fd);

    packnSend( rilgpsStopSession, bufp, type, length, uparm );
}

/**
 *
 * Handle requests to start a GPS tracking session
 * 
 * @param[in] bufp
 *     Pointer to storage holding the incoming message from swigps
 *     Points to the first byte of the variable length part
 * @param [in] type:
 *     The value contained in the header's type field for the packet 
 *     that triggered a call to this function
 * @param[in] length
 *     The length of the variable length portion of this message
 * @param[in] uparm
 *     arbitrary void * value. User defined variable - the file descriptor
 *     for the socket to send responses back to the sgi side. Passed to
 *     the enqueued function for when it needs to send the response back
 *     to the swigps daemn
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *      Instead of unpacking the incoming buffer, it and the user parm
 *      value are copied into a newly allocated buffer and sent along
 *      with the enqueued function for handling inside the RIL daemon
 *
 */
void reqStart( unsigned char* bufp, 
              swigpsMsgType type,
              size_t length, 
              void *uparm)
{
    unsigned char *argp, *largp;
    unsigned int ipaddr;
    unsigned int port;
    unsigned char urllength;
    int fd = (int) uparm;

    D("%s filedesc: %d", __func__, fd);

    packnSend( rilgpsStartSession, bufp, type, length, uparm );
}

/**
 *
 * Handle incoming swigps traffic, arranging to parse it and 
 * then dispatching the appropriate handler for it
 *
 * @param[in] fd
 *     A socket descriptor to use for communication with the 
 *     peer
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *     none
 *
 * @note
 *     This function remains in control until the socket closes and
 *     then it returns to the caller until another connection is 
 *     accepted. It also bails out if any other error condition is
 *     detected. Check the logs to find out the reason
 */
void handle_swigps( int fd )
{
    static unsigned char s_gpsbuf[MAXGPS];
    unsigned char *packetp;
    char *dbgstrp;
    int ret, buflength;
    unsigned short type, length, result;
    swigpsCbkFp cbkfp;
    char errmsgbuf[ERRMSGSIZE];

    /* Loop forever - until the connection is closed */
    for(;;) {

        /* Reset buflength each pass through the loop */
        buflength = sizeof(s_gpsbuf);

        /* Empty the receive buffer */
        memset( s_gpsbuf, 0, buflength );

        /* Going to block until the read is complete */
        LOGI("Awaiting packet from swigps");
        ret = receivepacket(fd, s_gpsbuf, 
                            &buflength, errmsgbuf, 
                            sizeof(errmsgbuf));

        LOGI("receivepacket: length %d, errstr: %s", 
           buflength, (ret?errmsgbuf:"No error") );

        /* Timed out on recv() call */
        if( buflength == -1 ) {
            LOGE("Timeout on recv(). Sleep a little");
            sleep(1);
            continue;
        }

        /* Special case first - Other end closed. 
         * Can happen if the Android GPS Location
         * service requests a close through the
         * interface function
         */
        if( !ret && buflength == 0 ) {
            LOGE("Swigps has closed");
            return;
        }

        /* Other error cases */
        if( ret ) {
            LOGE("IPC Receive error: %s", errmsgbuf );
            return;
        }

        packetp = s_gpsbuf;
        /* Received a good packet. Skip the type and 
         * result fields and get the length field 
         */
        type = piget16( &packetp );
        result = piget16( &packetp );
        length = piget16( &packetp );

        /* Validate the type - check against the allowable range */
        if( type >= SWIGPS_MSGTYPE_MAXVAL ) {
            LOGE("Rec'd unknown type from swigps: %d", type );
            return;
        }
    
        /* Type is valid, dispatch to handler, if one is defined */
        cbkfp = gpsTypeHandler[type].gpsCbkfp;
        dbgstrp = gpsTypeHandler[type].gpsCbkfNamep;
    
        D("Received type %d. Callback slot: '%s'", type, dbgstrp );

        /* Finally, call the handler for this type, if one is defined */
        if( cbkfp ) {
            cbkfp( packetp, type, length, (void *) fd );
        }
    } /* for */
}
    

/*****                              *****
 *****   Incoming Packet dispatch   *****
 *****                              *****/

/** 
 * 
 * Array of handler functions.
 *
 * This array contains slots for all defined packet type values, 
 * but only the slots for inbound responses are populated. Outbound
 * slots are placeholders and contain NULL. Indexed by type value
 *
 */ 
swigpsTypeHandler gpsTypeHandler[SWIGPS_MSGTYPE_MAXVAL] = {
    {reqGetNmeaPort, "reqGetNmeaPort"},      //!< SWIGPS_MSGTYPE_REQ_GETNMEAPORT 
    {NULL,          "Empty"},                //!< SWIGPS_MSGTYPE_RESP_GETNMEAPORT
    {reqGetDef, "reqGetDef"},                //!< SWIGPS_MSGTYPE_REQ_GETPDS 
    {NULL,          "Empty"},                //!< SWIGPS_MSGTYPE_RESP_GETPDS
    {reqSetDef, "reqSetDef"},                //!< SWIGPS_MSGTYPE_REQ_SETPDS 
    {NULL,          "Empty"},                //!< SWIGPS_MSGTYPE_RESP_SETPDS 
    {reqGetAgps, "reqGetAgps"},              //!< SWIGPS_MSGTYPE_REQ_GETAGPS 
    {NULL,          "Empty"},                //!< SWIGPS_MSGTYPE_RESP_GETAGPS 
    {reqSetAgps, "reqSetAgps"},              //!< SWIGPS_MSGTYPE_REQ_SETAGPS
    {NULL,          "Empty"},                //!< SWIGPS_MSGTYPE_RESP_SETAGPS
    {reqInjTime, "reqInjTime"},              //!< SWIGPS_MSGTYPE_REQ_INJTIME
    {NULL,          "Empty"},                //!< SWIGPS_MSGTYPE_RESP_INJTIME
    {reqStop,    "reqStop"},                 //!< SWIGPS_MSGTYPE_REQ_STOP
    {NULL,          "Empty"},                //!< SWIGPS_MSGTYPE_RESP_STOP
    {reqStart,    "reqStart"},               //!< SWIGPS_MSGTYPE_REQ_STOP
    {NULL,          "Empty"}                 //!< SWIGPS_MSGTYPE_RESP_STOP
};

