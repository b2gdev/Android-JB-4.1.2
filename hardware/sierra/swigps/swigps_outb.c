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

#include <errno.h>
#include <unistd.h>

#define  LOG_TAG  "gps_outb"

#include "swiril_log.h"
#include "swigps_sgi.h"

#define  GPS_DEBUG  0

#define  DFR(...)   LOGD(__VA_ARGS__)

#if GPS_DEBUG
#  define  D(...)   LOGD(__VA_ARGS__)
#else
#  define  D(...)   ((void)0)
#endif

/* Forward declarations */
swigpsTypeHandler gpsTypeHandler[];

/*****                                    *****
 ***** Message Building/Parsing Functions *****
 *****                                    *****/

/**
 *
 * Response handler for receiving the NMEA serial port handle from the
 * swiril side
 * 
 * @param[in] bufp
 *     Pointer to storage containing the response packet
 * @param [in] type:
 *     The value from the incoming packet header which triggered a
 *     call to this function
 * @param[in] length
 *     The total length of the variable portion of the response packet
 * @param[in] uparm
 *     arbitrary void * value. Contains a pointer to the state variable for
 *     the main state thread in swigps.c 
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *      This function is only called if there was no error code
 *      contained in the incoming packet header. 
 * 
 *      Other than storing the GPS port name into the state structure
 *      and marking the name as initialized, this function has no other
 *      effect. 
 *
 *      The NMEA port handle is a key component in whether this Vendor-
 *      specific GPS code will allow its "start()" interface function to
 *      be called. If the handle is unknown at the time of the start() 
 *      call then an error is returned to the interface's owner. 
 *
 *      Furthermore, the ENGINE ON status is not sent to the interface's
 *      owner until the GPS handle is returned. A properly designed 
 *      GPS Location Service ought not to call any more interface functions
 *      until the ENGINE ON status is received.
 *
 */
void respGetNmeaPort(unsigned char* bufp, 
                     swigpsMsgType type,
                     size_t length, 
                     void *uparm)
{
    unsigned int i;
    GpsState *statep = (GpsState *) uparm;

    D("%s started", __func__);

    /* Validate that the length is correct */
    if( length >= msgProperties[type].minlength && 
        length <= msgProperties[type].maxlength    ) {

        D("%s, type is %d, length is %d", __func__, type, length );

        /* Copy the port name into the state structure */
        for(i=0; i<length; i++)
            statep->gpsnmeaport.name[i] = *bufp++;

        /* NULL terminate the string */
        statep->gpsnmeaport.name[i] = 0;

        /* Done with initializing the string */
        statep->gpsnmeaport.initialized = 1;

        /* log the result */
        LOGI("respGetNmeaPort Got NMEA Port name %s",statep->gpsnmeaport.name);
   
        /* Send the engine on status to the GPS Location Service */
        swigps_report_state(&statep->callbacks, GPS_STATUS_ENGINE_ON);

        return;
    }

    /* Can only log an error occurred and move on */
    LOGE("Length mismatch for type: %d. Recv'd length: %d", type, length );
}

/**
 *
 * Ask for the NMEA USB port name from the swiril side
 * 
 * This function builds and sends a request to the swiril_gps side
 * (Outbound direction).
 *
 * @param[in]  riltype
 *     The RIL type expected to be in use by the GPS vendor specific
 *     code. Used by the ril-daemon for validation purposes 
 * @param[in]  rilgpsfd 
 *     File descriptor for the socket connecting swigps to swiril_gps
 *
 * @return
 *       0 - The operation was successful
 *     < 0 - An error occurred:
 *         -ENOMEM - Insufficient storage (returned from subroutine call)
 *         -EINVAL - Internal failure (Abort would be appropriate)
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *      This function is a key component in this swigps vendor-specific
 *      startup. The interface's "start()" function will return with an 
 *      error until the NMEA port handle is known and the ENGINE ON status
 *      will only be returned when this function's counterpart, 
 *      respGetNmeaPort() has been called with a valid handle. Therefore, 
 *      calling this function is mandatory and should happen very early 
 *      on in the main gps state thread's startup.
 *
 */
int swigps_getnmeaport( GPS_RILTYPE riltype, int rilgpsfd )
{
    int bufsize, ret, written;
    char *errp;
    unsigned char *bufp;
    unsigned char pktbuf[ SWIGPS_MSGSIZE_HEADER+SWIGPS_MSGSIZE_REQ_GETNMEAPORT];
    char errmsgbuf[ERRMSGSIZE];

    D("%s started with riltype %d", __func__, riltype);

    /* Request the header be built. Note, no 
     * variable length portion in this request
     */
    errp = swigps_makhdr( SWIGPS_MSGTYPE_REQ_GETNMEAPORT, 
                          SWIGPS_MSGSIZE_REQ_GETNMEAPORT, 0,
                          pktbuf, sizeof(pktbuf), &written );

    /* Ensure the header got built */
    if(errp) {
        LOGE("%s", errp);
        LOGE("getnmeaport: Called with type %d, buflength %d", 
             SWIGPS_MSGTYPE_REQ_GETDEF, sizeof(pktbuf) );
		return -EINVAL;
	}

    /* Make a copy of the pointer to the packet array */
    bufp = pktbuf + written;

    /* Pack the buffer with the ril type value */

    /* riltype - 2 bytes */
    piput16( riltype, &bufp );

    /* Update the total number of bytes written */
    written = (bufp - pktbuf);

    /* Send the request over the IPC channel */
    if( !socksend( rilgpsfd, pktbuf, (size_t) written, errmsgbuf ) ) {
        D("getnmeaport sent %d bytes to swiril_gps", written );

        /* Log that a response is expected. Handled via a call
         * to handle_rilgps(), below, called from the main state
         * thread when the IPC socket event is triggered
         */
        LOGI("getnmeaport() response pending");

        return 0;
    }

    /* Else some error occurred */
    LOGE( "%s", errmsgbuf );
    return -1;
}

/**
 *
 * Response handler for packets containing inbound default responses
 * 
 * @param[in] bufp
 *     Pointer to storage containing the response packet
 * @param [in] type:
 *     The value from the incoming packet header which triggered a
 *     call to this function
 * @param[in] length
 *     The total length of the variable portion of the response packet
 * @param[in] uparm
 *     arbitrary void * value. Contains a pointer to the state variable for
 *     the main state thread in swigps.c 
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
void respGetDef(unsigned char* bufp, 
                swigpsMsgType type,
                size_t length, 
                void *uparm)
{
    unsigned int accuracy;
    unsigned int interval;
    unsigned char timeout;
    unsigned char mode;

    GpsState *statep = (GpsState *) uparm;

    D("%s started", __func__);

    /* Validate that the length is correct */
    if( length >= msgProperties[type].minlength && 
        length <= msgProperties[type].maxlength    ) {

        /* Parse out the Default settings reported by the modem */
        mode = *bufp++;
        timeout = *bufp++;
    
        interval = piget32( &bufp );
        accuracy = piget32( &bufp );

        statep->pdsDefs.accuracy = accuracy;
        statep->pdsDefs.interval = interval;
        statep->pdsDefs.timeout = (unsigned int) timeout;
        statep->pdsDefs.reqmode = mode;
 
        /* Handle any modem-technology specific followup actions */
        defaults_received();

        /* for now just log the result */
		LOGI("Rec'd PdsDefs. type: %d\n\
               timeout: %d\n\
               interval: %d\n\
               accuracy: %d",
             mode, timeout, interval, accuracy);

        return;
    }

    /* Can only log an error occurred and move on */
    LOGE("Length mismatch for type: %d. Recv'd length: %d", type, length );
}


/**
 *
 * Request default tracking settings from the modem
 * 
 * This function builds and sends a request to the swiril_gps side
 * (Outbound direction) blocking the caller until a response is 
 * received.
 *
 * @param[in]  rilgpsfd 
 *     File descriptor for the socket connecting swigps to swiril_gps
 *
 * @return
 *       0 - The operation was successful
 *     < 0 - An error occurred:
 *         -ENOMEM - Insufficient storage (returned from subroutine call)
 *         -EINVAL - Internal failure (Abort would be appropriate)
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *
 */
int swigps_getdef( int rilgpsfd )
{
    int bufsize, ret, written;
    char *errp;
    unsigned char pktbuf[ SWIGPS_MSGSIZE_HEADER+SWIGPS_MSGSIZE_REQ_GETDEF ];
    char errmsgbuf[ERRMSGSIZE];

    D("%s started", __func__);

    /* Request the header be built. Note, no 
     * variable length portion in this request
     */
    errp = swigps_makhdr( SWIGPS_MSGTYPE_REQ_GETDEF,
                          SWIGPS_MSGSIZE_REQ_GETDEF, 0,
                          pktbuf, sizeof(pktbuf), &written );

    /* Ensure the header got built */
    if(errp) {
        LOGE("%s makhdr error: %s",__func__, errp);
        LOGE("Called with type %d, buflength %d", 
             SWIGPS_MSGTYPE_REQ_GETDEF, sizeof(pktbuf) );
		return -EINVAL;
	}

    /* Ensure the header got built */
    if( written < 0 ) {
        LOGE("getdef: Failed to build header. %d", -written );
        return -EINVAL;
    }

    /* Send the request over the IPC channel */
    if( !socksend( rilgpsfd, pktbuf, (size_t) written, errmsgbuf ) ) {
        D("%s sent %d bytes to swiril_gps",__func__, written );

        /* Log that a response is expected. Handled via a call
         * to handle_rilgps(), below, called from the main state
         * thread when the IPC socket event is triggered
         */
        LOGI("%s response pending", __func__);

        return 0;
    }

    /* Else some error occurred */
    LOGE("%s", errmsgbuf );
    return -1;
}

/**
 *
 * Response handler for request to set/change the default tracking
 * values. 
 * 
 * @param[in] bufp
 *     Pointer to storage containing the response packet
 *     The pointer is positioned at the start of the variable area 
 *     of the message
 * @param [in] type:
 *     The value from the incoming packet header which triggered a
 *     call to this function
 * @param[in] length
 *     The total length of the variable portion of the response packet
 * @param[in] uparm
 *     arbitrary void * value. Contains a pointer to the state variable for
 *     the main state thread in swigps.c 
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
void respSetDef( unsigned char* bufp, 
                 swigpsMsgType type, 
                 size_t length, 
                 void *uparm )
{
    D("%s started", __func__);

    /* Validate that the length is correct */
    if( length >= msgProperties[type].minlength && 
        length <= msgProperties[type].maxlength ) {

        /* There is no variable portion for this message
         * so nothing further to unpack 
         */
    
        /* Convert uparm back into a pointer to the state variable */
        GpsState *statep = (GpsState *) uparm;
 
        /* Handle any modem-technology specific followup actions */
        defaults_set();

        LOGI("%s: mode change to %d complete", __func__, statep->pdsDefs.reqmode);
    }
}

/**
 *
 * Request a change to the GPS operating mode
 * 
 * This function builds and sends a request to the sgr side
 * (Outbound direction) to set the operating mode and other 
 * default GPS session values
 *
 * @param[in]  rilgpsfd 
 *     File descriptor for the socket connecting swigps to swiril_gps
 * @param[in]  operation 
 *     The requested operating mode: 0: Standalone, 1: MS-based,
 *     2: MS-assisted
 * @param[in]  timeout 
 *     The maximum time (sec) to work on each fix
 * @param[in]  interval 
 *     Interval (sec) between fix segments
 * @param[in]  accuracy 
 *     Requested accuracy of the fix (metres)
 * @param[in]  tracking 
 *     bool: 0, no tracking session in progress
 *           1, tracking session underway in modem
 *
 * @return
 *       0 - The operation was successful
 *     < 0 - An error occurred:
 *         -ENOMEM - Insufficient storage (returned from subroutine call)
 *         -EINVAL - Internal failure (Abort would be appropriate)
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *      The response packet will contain an indication of the outcome
 *      of this request within the modem. A successful response must
 *      be sure to update the state structure with the values changed
 *      by this request
 *
 */
int swigps_setdef( int rilgpsfd,
                   int operation,
                   int timeout,
                   int interval,
                   int accuracy,
                   bool tracking )
{
    int bufsize, ret, written;
    char *errp;
    unsigned char *bufp;
	unsigned char pktbuf[ SWIGPS_MSGSIZE_HEADER+SWIGPS_MSGSIZE_REQ_SETDEF ];
    char errmsgbuf[ERRMSGSIZE];

    D("%s started", __func__);

    /* Request the header be built. Note, no 
     * variable length portion in this request
     */
    errp = swigps_makhdr( SWIGPS_MSGTYPE_REQ_SETDEF, 
                          SWIGPS_MSGSIZE_REQ_SETDEF, 0,
                          pktbuf, sizeof(pktbuf), &written );

    /* Ensure the header got built */
    if(errp) {
        LOGE("%s makhdr error: %s",__func__, errp);
        LOGE("Called with type %d, buflength %d", 
             SWIGPS_MSGTYPE_REQ_SETDEF, sizeof(pktbuf) );
		return -EINVAL;
	}
 
    /* Make a copy of the pointer to the packet array */
    bufp = pktbuf + written;

    /* Pack the buffer with the remaining values */

    /* Mode - 1 byte */
    *bufp++ = (unsigned char) operation;

    /* Timeout - 1 byte */
    *bufp++ = (unsigned char) timeout;

    /* Interval - 4 bytes */
    piput32( interval, &bufp );

    /* Accuracy - 4 bytes */
    piput32( accuracy, &bufp );

    /* Tracking status */
    *bufp++ = tracking;

    /* Update the total number of bytes written */
    written = (bufp - pktbuf);

    /* Send the request over the IPC channel */
    if( !socksend( rilgpsfd, pktbuf, (size_t) written, errmsgbuf ) ) {
        D("%s sent %d bytes to swiril_gps", __func__,written );

        /* Log that a response is expected. Handled via a call
         * to handle_rilgps(), below, called from the main state
         * thread when the IPC socket event is triggered
         */
        LOGI("%s response pending",__func__);

        return 0;
    }

    /* Else some error occurred */
    LOGE("%s", errmsgbuf);
    return -1;
}

/**
 *
 * Response handler for AGPS information sent from the swiril_gps
 * side
 * 
 * @param[in] bufp
 *     Pointer to storage containing the response packet. Points to
 *     the variable-length portion of the packet
 * @param [in] type:
 *     The value from the incoming packet header which triggered a
 *     call to this function
 * @param[in] length
 *     The total length of the variable portion of the response packet
 * @param[in] uparm
 *     arbitrary void * value. Contains a pointer to the state variable for
 *     the main state thread in swigps.c 
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
void respGetAgps(unsigned char* bufp, 
                 swigpsMsgType type,
                 size_t length, 
                 void *uparm)
{
    unsigned char *tmpbufp;
    size_t urlsize;

    GpsState *statep = (GpsState *) uparm;

    D("%s started", __func__);

    /* Validate that the length is correct */
    if( length >= msgProperties[type].minlength && 
        length <= msgProperties[type].maxlength    ) {

        /* Copy the packet pointer */
        tmpbufp = bufp;

        /* Parse out the Ipv4 value and the port number */
        statep->agpsInfo.ipaddr = piget32( &tmpbufp );
        statep->agpsInfo.port = piget32( &tmpbufp );
        
        /* Reduce the length by the amount just
         * read. If there is anything remaining
         * then there must be a URL included
         */
        urlsize = length - (tmpbufp - bufp);

        D("respGetAgps: urlsize is %d", urlsize );

        if( urlsize > MAXURL ) {
            LOGE("respgetAgps: url too big %d, truncating", urlsize );
            urlsize = MAXURL;
        }

        /* Fetch the URL */
        pigetmulti( &tmpbufp, (unsigned char *) statep->agpsInfo.url, urlsize );

        /* Make sure the URL is null terminated */
        statep->agpsInfo.url[urlsize] = 0;

        /* Handle any modem-technology specific followup actions */
        agps_received();

        /* for now just log the result */
        LOGI("Rec'd AGPS Info: ipaddr: %d port: %d\nUrl: %s",
               statep->agpsInfo.ipaddr, 
               statep->agpsInfo.port, 
               statep->agpsInfo.url);

        return;
    }

    /* Can only log an error occurred and move on */
    LOGE("Length mismatch for type: %d. Recv'd length: %d", type, length );
}

/**
 *
 * Request AGPS settings from the modem
 * 
 * This function builds and sends a request to the swiril_gps side
 * (Outbound direction) blocking the caller until a response is 
 * received.
 *
 * @param[in]  rilgpsfd 
 *     File descriptor for the socket connecting swigps to swiril_gps
 *
 * @return
 *       0 - The operation was successful
 *     < 0 - An error occurred:
 *         -ENOMEM - Insufficient storage (returned from subroutine call)
 *         -EINVAL - Internal failure (Abort would be appropriate)
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *
 */
int swigps_getagps( int rilgpsfd )
{
    int bufsize, ret, written;
    char *errp;
    unsigned char pktbuf[ SWIGPS_MSGSIZE_HEADER+SWIGPS_MSGSIZE_REQ_GETAGPS ];
	char errmsgbuf[ERRMSGSIZE];

    D("%s started", __func__);

    /* Request the header be built */
    errp = swigps_makhdr( SWIGPS_MSGTYPE_REQ_GETAGPS,
                          SWIGPS_MSGSIZE_REQ_GETAGPS, 0,
                          pktbuf, sizeof(pktbuf), &written );

    /* Ensure the header got built */
    if(errp) {
        LOGE("%s makhdr error: %s",__func__, errp);
        LOGE("Called with type %d, buflength %d", 
             SWIGPS_MSGTYPE_REQ_GETAGPS, sizeof(pktbuf) );
		return -EINVAL;
	}

    /* Send the request over the IPC channel */
    if( !socksend( rilgpsfd, pktbuf, (size_t) written, errmsgbuf ) ) {
        D("%s sent %d bytes to swiril_gps", __func__, written );

        /* Log that a response is expected. Handled via a call
         * to handle_rilgps(), below, called from the main state
         * thread when the IPC socket event is triggered
         */
        LOGI("%s response pending", __func__);

        return 0;
    }

    /* Else some error occurred */
    LOGE("%s", errmsgbuf );
    return -1;
}

/**
 *
 * Response handler for request to set/change the default PDS
 * values. 
 * 
 * @param[in] bufp
 *     Pointer to storage containing the response packet
 *     The pointer is positioned at the start of the variable 
 *     area of the message
 * @param [in] type:
 *     The value from the incoming packet header which triggered a
 *     call to this function
 * @param[in] length
 *     The total length of the variable portion of the response packet
 * @param[in] uparm
 *     arbitrary void * value. Contains a pointer to the state variable for
 *     the main state thread in swigps.c 
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
void respSetAgps( unsigned char* bufp, 
                 swigpsMsgType type, 
                 size_t length, 
                 void *uparm )
{
    unsigned int urllength;
    GpsState *statep = (GpsState *) uparm;

    D("%s started", __func__);

    /* Validate that the length is correct */
    if( length >= msgProperties[type].minlength && 
        length <= msgProperties[type].maxlength ) {

        /* If we get this far, there were no QMI errors
         * from setting the AGPS values on the modem. 
         * Update the current values with the requested 
         * ones
         */
        statep->agpsInfo.type = statep->agpsInfo.reqtype;
        statep->agpsInfo.ipaddr = statep->agpsInfo.reqipaddr;
        statep->agpsInfo.port = statep->agpsInfo.reqport;
        strncpy( statep->agpsInfo.url, statep->agpsInfo.requrl, MAXURL);

        /* Handle any modem-technology specific followup actions */
        agps_set();

        /* Update the default setting with the requested mode */
        LOGI("%s: complete", __func__);
    }
}

/**
 *
 * Change AGPS settings As Specified
 * 
 * This function builds and sends a request to the modem to change
 * its AGPS settings to the ones specified by the caller.
 *
 * @param[in]  rilgpsfd 
 *     File descriptor for the socket connecting swigps to swiril_gps
 * @param[in]  ipaddr
 *     The IP address of the host server. NOTE: this is not provided by
 *     android and must be computed within swigps.c prior to calling this
 *     function.
 * @param[in] port
 *     Port number on the host computer which provides access to the AGPS
 *     server
 * @param[in] hostnamep
 *     Pointer to the string (URL) for the AGPS server
 *
 * @return
 *       0 - The operation was successful
 *     < 0 - An error occurred:
 *         -ENOMEM - Insufficient storage (returned from subroutine call)
 *         -EINVAL - Internal failure (Abort would be appropriate)
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *
 */
int swigps_setagps( int rilgpsfd,
                    unsigned int ipaddr,
                    int port, 
                    char *hostnamep )
{
    int bufsize, ret, written;
    char *errp;
    unsigned char *bufp;
    unsigned int urllength;
    static unsigned char pktbuf[SWIGPS_MSGSIZE_HEADER+SWIGPS_MSGSIZE_MAX_AGPS];
	char errmsgbuf[ERRMSGSIZE];

    D("%s started", __func__);

    /* Compute the pktlength from the 
     * string pointed to by hostnamep 
     */
    urllength = strlen( hostnamep );

    /* Request the header be built. Note, no 
     * variable length portion in this request
     */
    errp = swigps_makhdr( SWIGPS_MSGTYPE_REQ_SETAGPS, 
                          urllength + SWIGPS_MSGSIZE_MIN_AGPS, 0,
                          pktbuf, sizeof(pktbuf), &written );

    /* Ensure the header got built */
    if(errp) {
        LOGE("%s makhdr error: %s",__func__, errp);
        LOGE("Called with type %d, buflength %d", 
             SWIGPS_MSGTYPE_REQ_SETAGPS, sizeof(pktbuf) );
		return -EINVAL;
	}
 
    /* Make a copy of the pointer to the packet array */
    bufp = pktbuf + written;

    /* Pack the buffer with the remaining values */

    /* ipaddress */
    piput32( ipaddr, &bufp );

    /* port */
    piput32( port, &bufp );

    /* Store it into the packet */
    *bufp++ = (unsigned char) urllength;
   
    /* Finally, copy the URL into the destination */
    piputmulti( (unsigned char *) hostnamep, &bufp, urllength );

    D("setagps: ipaddr: 0x%x, port %d, urllength %u, hostnamep: %s",
      ipaddr, port, urllength, hostnamep );

    /* Update the total number of bytes written */
    written = (bufp - pktbuf);

    /* Send the request over the IPC channel */
    if( !socksend( rilgpsfd, pktbuf, (size_t) written, errmsgbuf ) ) {
        D("setagps sent %d bytes to swiril_gps", written );

        /* Log that a response is expected. Handled via a call
         * to handle_rilgps(), below, called from the main state
         * thread when the IPC socket event is triggered
         */
        LOGI("setagps() response pending");

        return 0;
    }

    /* Else some error occurred */
    LOGE("%s", errmsgbuf );
    return -EINVAL;
}

/**
 *
 * Response handler for time injection request
 * 
 * @param[in] bufp
 *     Pointer to storage containing the response packet
 *     The pointer is positioned at the start of the
 *     variable area of the message
 * @param [in] type:
 *     The value from the incoming packet header which triggered a
 *     call to this function
 * @param[in] length
 *     The total length of the variable portion of the response packet
 * @param[in] uparm
 *     arbitrary void * value. Contains a pointer to the state variable for
 *     the main state thread in swigps.c 
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
void respInjTime( unsigned char* bufp, 
                 swigpsMsgType type, 
                 size_t length, 
                 void *uparm )
{
    unsigned int urllength;
    GpsState *statep = (GpsState *) uparm;

    D("%s started", __func__);

    /* If we get this far, there were no QMI errors
     * from the time injection request to the modem
     */

    /* Validate that the length is correct */
    if( length >= msgProperties[type].minlength && 
        length <= msgProperties[type].maxlength ) {

        /* Handle any modem-technology specific followup actions */
        time_injected();

        /* Update the default setting with the requested mode */
        LOGI("%s: complete", __func__);
    }
}

/**
 *
 * Send a request to the modem to inject the current time
 * 
 * @param[in]  rilgpsfd 
 *     File descriptor for the socket connecting swigps to swiril_gps
 * @param[in] time
 *     The current time to be injected
 * @param[in] uncertainty
 *     A measure of the quality of the time and its reference
 *
 * @return
 *       0 - The operation was successful
 *     < 0 - An error occurred:
 *         -ENOMEM - Insufficient storage (returned from subroutine call)
 *         -EINVAL - Internal failure (Abort would be appropriate)
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *
 */
int swigps_injtime( int rilgpsfd,
                    GpsUtcTime time,
                    int uncertainty )
{
    int bufsize, ret, written;
    char *errp;
    unsigned char *bufp;
    static unsigned char pktbuf[SWIGPS_MSGSIZE_HEADER+SWIGPS_MSGSIZE_REQ_INJTIME];
	char errmsgbuf[ERRMSGSIZE];

    D("%s started", __func__);

	/* Build the header */
    errp = swigps_makhdr( SWIGPS_MSGTYPE_REQ_INJTIME, 
                          SWIGPS_MSGSIZE_REQ_INJTIME, 0,
                          pktbuf, sizeof(pktbuf), &written );

    /* Ensure the header got built */
    if(errp) {
        LOGE("%s makhdr error: %s",__func__, errp);
        LOGE("Called with type %d, buflength %d", 
             SWIGPS_MSGTYPE_REQ_INJTIME, sizeof(pktbuf) );
		return -EINVAL;
	}
 
    /* Make a copy of the pointer to the packet array */
    bufp = pktbuf + written;

    /* time - ms since 1970, 64 bits */
    piput64( time, &bufp );

    /* Uncertainty, 1/2 round trip delay to NTP server */
    piput32( uncertainty, &bufp );

    D("%s: time %llu, uncertainty %d", __func__, time, uncertainty );

    /* Update the total number of bytes written */
    written = (bufp - pktbuf);

    /* Send the request over the IPC channel */
    if( !socksend( rilgpsfd, pktbuf, (size_t) written, errmsgbuf ) ) {
        D("%s sent %d bytes", __func__, written );

        /* Log that a response is expected. Handled via a call
         * to handle_rilgps(), below, called from the main state
         * thread when the IPC socket event is triggered
         */
        LOGI("%s response pending",__func__);

        return 0;
    }

    /* Else some error occurred */
    LOGE("%s", errmsgbuf );
    return -EINVAL;
}

/**
 *
 * Response handler for GPS Stop Tracking request
 * 
 * @param[in] bufp
 *     Pointer to storage containing the response packet
 *     The pointer is positioned at the start of the
 *     variable area of the message
 * @param [in] type:
 *     The value from the incoming packet header which triggered a
 *     call to this function
 * @param[in] length
 *     The total length of the variable portion of the response packet
 * @param[in] uparm
 *     arbitrary void * value. Contains a pointer to the state variable for
 *     the main state thread in swigps.c 
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
void respStopSession( unsigned char* bufp, 
                      swigpsMsgType type, 
                      size_t length, 
                      void *uparm )
{
    unsigned int urllength;
    GpsState *statep = (GpsState *) uparm;

    D("%s started", __func__);

    /* If we get this far, there were no errors
     * from the Stop Session request to the modem
     */

    /* Validate that the length is correct */
    if( length >= msgProperties[type].minlength && 
        length <= msgProperties[type].maxlength ) {

        /* Handle any modem-technology specific followup actions */
        session_stopped();

        /* Update the default setting with the requested mode */
        LOGI("%s: complete",__func__);

        return;
    }
    LOGE("%s: Length error in response, %d", __func__, length );
}

/**
 *
 * Send a request to the modem to Stop any active GPS tracking session
 * 
 * @param[in]  rilgpsfd 
 *     File descriptor for the socket connecting swigps to swiril_gps
 *
 * @return
 *       0 - The operation was successful
 *     < 0 - An error occurred:
 *         -ENOMEM - Insufficient storage (returned from subroutine call)
 *         -EINVAL - Internal failure (Abort would be appropriate)
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *
 */
int swigps_StopSession( int rilgpsfd )
{
    int bufsize, ret, written;
    char *errp;
    unsigned char *bufp;
    static unsigned char pktbuf[SWIGPS_MSGSIZE_HEADER+SWIGPS_MSGSIZE_REQ_STOP];
	char errmsgbuf[ERRMSGSIZE];

    D("%s started", __func__);

	/* Build the header */
    errp = swigps_makhdr( SWIGPS_MSGTYPE_REQ_STOP, 
                          SWIGPS_MSGSIZE_REQ_STOP, 0,
                          pktbuf, sizeof(pktbuf), &written );

    /* Ensure the header got built */
    if(errp) {
        LOGE("%s makhdr error: %s",__func__, errp);
        LOGE("Called with type %d, buflength %d", 
             SWIGPS_MSGTYPE_REQ_STOP, sizeof(pktbuf) );
		return -EINVAL;
	}
 
    /* Send the request over the IPC channel */
    if( !socksend( rilgpsfd, pktbuf, (size_t) written, errmsgbuf ) ) {
        D("%s sent %d bytes", __func__,written );

        /* Log that a response is expected. Handled via a call
         * to handle_rilgps(), below, called from the main state
         * thread when the IPC socket event is triggered
         */
        LOGI("%s response pending",__func__);

        return 0;
    }

    /* Else some error occurred */
    LOGE("%s", errmsgbuf );
    return -EINVAL;
}

/**
 *
 * Response handler for GPS Start Tracking request
 * 
 * @param[in] bufp
 *     Pointer to storage containing the response packet
 *     The pointer is positioned at the start of the
 *     variable area of the message
 * @param [in] type:
 *     The value from the incoming packet header which triggered a
 *     call to this function
 * @param[in] length
 *     The total length of the variable portion of the response packet
 * @param[in] uparm
 *     arbitrary void * value. Contains a pointer to the state variable for
 *     the main state thread in swigps.c 
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
void respStartSession( unsigned char* bufp, 
                      swigpsMsgType type, 
                      size_t length, 
                      void *uparm )
{
    unsigned int urllength;
    GpsState *statep = (GpsState *) uparm;

    D("%s started", __func__);

    /* If we get this far, there were no errors
     * from the Start Session request to the modem
     */

    /* Validate that the length is correct */
    if( length >= msgProperties[type].minlength && 
        length <= msgProperties[type].maxlength ) {

        /* Handle any modem-technology specific followup actions */
        session_started();

        /* Update the default setting with the requested mode */
        LOGI("%s: complete",__func__);

        return;
    }
    LOGE("%s: Length error in response, %d", __func__, length );
}

/**
 *
 * Send a request to the modem to Start a GPS tracking session
 * 
 * @param[in]  rilgpsfd 
 *     File descriptor for the socket connecting swigps to swiril_gps
 * @param[in]  operation 
 *     The requested operating mode: 0: Standalone, 1: MS-based,
 *     2: MS-assisted
 * @param[in]  timeout 
 *     The maximum time (sec) to work on each fix
 * @param[in]  interval 
 *     Interval (sec) between fix segments
 * @param[in]  accuracy 
 *     Requested accuracy of the fix (metres)
 *
 *
 * @return
 *       0 - The operation was successful
 *     < 0 - An error occurred:
 *         -ENOMEM - Insufficient storage (returned from subroutine call)
 *         -EINVAL - Internal failure (Abort would be appropriate)
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *
 */
int swigps_StartSession( int rilgpsfd,
                         int operation,
                         int timeout,
                         int interval,
                         int accuracy )
{
    int bufsize, ret, written;
    char *errp;
    unsigned char *bufp;
    static unsigned char pktbuf[SWIGPS_MSGSIZE_HEADER+SWIGPS_MSGSIZE_REQ_START];
	char errmsgbuf[ERRMSGSIZE];

    D("%s started", __func__);

	/* Build the header */
    errp = swigps_makhdr( SWIGPS_MSGTYPE_REQ_START, 
                          SWIGPS_MSGSIZE_REQ_START, 0,
                          pktbuf, sizeof(pktbuf), &written );

    /* Ensure the header got built */
    if(errp) {
        LOGE("%s makhdr error: %s",__func__, errp);
        LOGE("Called with type %d, buflength %d", 
             SWIGPS_MSGTYPE_REQ_START, sizeof(pktbuf) );
		return -EINVAL;
	}

    /* Make a copy of the pointer to the packet array */
    bufp = pktbuf + written;

    /* Pack the buffer with the remaining values */

    /* Mode - 1 byte */
    *bufp++ = (unsigned char) operation;

    /* Timeout - 1 byte */
    *bufp++ = (unsigned char) timeout;

    /* Interval - 4 bytes */
    piput32( interval, &bufp );

    /* Accuracy - 4 bytes */
    piput32( accuracy, &bufp );

    /* Update the total number of bytes written */
    written = (bufp - pktbuf);
 
    /* Send the request over the IPC channel */
    if( !socksend( rilgpsfd, pktbuf, (size_t) written, errmsgbuf ) ) {
        D("%s sent %d bytes", __func__,written );

        /* Log that a response is expected. Handled via a call
         * to handle_rilgps(), below, called from the main state
         * thread when the IPC socket event is triggered
         */
        LOGI("%s response pending",__func__);

        return 0;
    }

    /* Else some error occurred */
    LOGE("%s", errmsgbuf );
    return -EINVAL;
}

/**
 * 
 * Handle an incoming message from the RILGPS daemon
 *
 * @param [in] statep
 *    Pointer to the state structure stored in the main GPS
 *    state thread
 * @param [in] fd
 *    File descriptor representing the socket holding the
 *    message to be processed
 * 
 * @return
 *    none
 * 
 * @par abort:<br>
 *    none
 * 
 * @note
 *    This function and any invoked by it are executed in the 
 *    context of the main gps thread in swigps.c
 *
 *    The scope of the s_gpsbuf array is limited to this function
 *    and it's a statically-declared array. Care must be take to 
 *    have completely emptied its contents before returning from 
 *    this function.
 */
void handle_rilgps( GpsState *statep, int fd )
{
    static unsigned char s_gpsbuf[MAXGPS];
	unsigned char *packetp;
    char *dbgstrp;
    int ret, buflength;
    unsigned short type, result, length;
    swigpsCbkFp cbkfp;
	char errmsgbuf[ERRMSGSIZE];

    D("%s started", __func__);

    buflength = sizeof(s_gpsbuf);

    /* Empty the receive buffer */
    memset( s_gpsbuf, 0, buflength );

    /* Going to block until the read is complete */
    D("Reading traffic from RIL side");
    ret = receivepacket(fd, s_gpsbuf, &buflength, errmsgbuf, sizeof(errmsgbuf));

    /* Special case first - Other end closed. Should never happen */
    if( !ret && buflength == 0 ) {
        LOGE("Swiril_gps closed socket");
        return;
    }

    /* Other error cases */
    if( ret ) {
        LOGE("IPC Receive error: %s", errmsgbuf );
        return;
    }

    packetp = s_gpsbuf;
    /* Else we've received a good packet. Dispatch to its handler */
    type = piget16( &packetp );
    result = piget16( &packetp );
    length = piget16( &packetp );

    /* Validate the type - check against the allowable range */
    if( type >= SWIGPS_MSGTYPE_MAXVAL ) {
        LOGE("Rec'd unknown type from swiril_gps: %d", type );
        return;
    }

    /* Check the result code here and handle all error cases
     * TODO Might need to push this down into the individual 
     * callback function for finer-grained error case handling 
     * but for now it'll be filtered right here
     */
    if( result ) {
        LOGE("Received QMI Error %d for msg type %d", result, type );
        return;
    }
    
    /* Type is valid, dispatch to handler, if one is defined */
    cbkfp = gpsTypeHandler[type].gpsCbkfp;
    dbgstrp = gpsTypeHandler[type].gpsCbkfNamep;

    D("Received type %d. Callback slot: '%s'", type, dbgstrp );

    /* Finally, call the handler for this type, if one is defined */
    if( cbkfp ) {
        cbkfp( packetp, type, length, (void *) statep );
    }
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
 * slots are placeholders and contain NULL. Indexed by type value, 
 * so entries in this list must correspond to the message type 
 * definitions in swigps_common.h
 *
 */ 
swigpsTypeHandler gpsTypeHandler[SWIGPS_MSGTYPE_MAXVAL] = {
    {NULL,       "empty"},                //!< SWIGPS_MSGTYPE_REQ_GETNMEAPORT
    {respGetNmeaPort, "respGetNmeaPort"}, //!< SWIGPS_MSGTYPE_RESP_GETNMEAPORT
    {NULL,       "empty"},                //!< SWIGPS_MSGTYPE_REQ_GETDEF
    {respGetDef, "respGetDef"},           //!< SWIGPS_MSGTYPE_RESP_GETDEF
    {NULL,       "empty"},                //!< SWIGPS_MSGTYPE_REQ_SETDEF
    {respSetDef, "respSetDef"},           //!< SWIGPS_MSGTYPE_RESP_SETDEF
    {NULL,       "empty"},                //!< SWIGPS_MSGTYPE_REQ_GETAGPS
    {respGetAgps,"respGetAgps"},          //!< SWIGPS_MSGTYPE_RESP_GETAGPS
    {NULL,       "empty"},                //!< SWIGPS_MSGTYPE_REQ_SETAGPS
    {respSetAgps,"respSetAgps"},          //!< SWIGPS_MSGTYPE_RESP_SETAGPS
    {NULL,       "empty"},                //!< SWIGPS_MSGTYPE_REQ_INJTIME
    {respInjTime,"respInjTime"},          //!< SWIGPS_MSGTYPE_RESP_INJTIME
    {NULL,       "empty"},                //!< SWIGPS_MSGTYPE_REQ_STOP
    {respStopSession,"respStopSession"},  //!< SWIGPS_MSGTYPE_RESP_STOP
    {NULL,       "empty"},                //!< SWIGPS_MSGTYPE_REQ_START
    {respStartSession,"respStartSession"} //!< SWIGPS_MSGTYPE_RESP_START
};

