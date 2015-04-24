/**
 *
 * @ingroup swiril
 *
 * @file 
 * Provides support for QMI Position Determination Service (PDS) functions.
 *
 * @author
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */

#include <telephony/ril.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <cutils/sockets.h>

#include "swiril_main.h"
#include "swiril_cache.h"
#include "swiril_misc_qmi.h"
#include "swiril_network_qmi.h"

#include "SWIWWANCMAPI.h"
#include "qmerrno.h"
#include "qmudefs.h"
#include "swiril_gps.h"

/* To capture logs from this facility use:
 *     'adb logcat -v time | grep gps_'
 */
#define LOG_TAG "gps_qmi"
#include "swiril_log.h"

#define  GPS_DEBUG  0

#define  DFR(...)   LOGD(__VA_ARGS__)

#if GPS_DEBUG
#  define  D(...)   LOGD(__VA_ARGS__)
#else
#  define  D(...)   ((void)0)
#endif

/* Local definitions */

#define DISABLE_TRACKING 0
#define ENABLE_TRACKING 1

#define UTC 1
#define FORCE 1


/**
 *
 * GPS QMI agent which handles the request to get PDS default settings
 * from a QMI modem. 
 * 
 * @param [in] paramp
 *     Indicates which file handle to use when replying to the sgi side
 *     once the default PDS settings have been received from the modem
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *      Runs in the context of the RIL daemon
 *
 */
void rilgpsGetDef( void *paramp )
{
    int written, fd;
    ULONG operation;
    ULONG interval;
    ULONG accuracy;
    BYTE timeout;
    ULONG nret;
    char *errp;
    BYTE *bufp;
    BYTE respbuf[SWIGPS_MSGSIZE_HEADER+SWIGPS_MSGSIZE_RESP_GETDEF];
    char errmsgbuf[ERRMSGSIZE];

    /* Get the fd from paramp */
    fd = (int) paramp;

    D("getPds: using filedesc: %d", fd);

    /* Clear the response buffer prior to use */
    memset( respbuf, 0, sizeof(respbuf) );

    /* Initiate the QMI request */
    nret = GetPDSDefaults( &operation, &timeout, &interval, &accuracy );

    LOGI("getPds: got PdsDefaults with result %d", (int)nret );

    LOGI("   operation: %d, timeout %d\n\
   accuracy: %d, interval %d", 
   (int)operation, (int)timeout, (int)accuracy, (int)interval );

    /* Just send nret back along with the values received. If there
     * was an error the recipient can deal with it
     */

    /* Copy of the pointer to the buffer */
    bufp = respbuf;

    /* Make the header */
    errp = swigps_makhdr( SWIGPS_MSGTYPE_RESP_GETDEF, 
                   SWIGPS_MSGSIZE_RESP_GETDEF, 
                   nret,
                   bufp, sizeof(respbuf), &written );

    /* Ensure the header got built */
    if(errp) {
        LOGE("%s makhdr error: %s",__func__, errp);
        LOGE("Called with type %d, buflength %d", 
             SWIGPS_MSGTYPE_RESP_GETDEF, sizeof(respbuf) );
		return;
	}

    /* Adjust bufp to the first byte past the header */
    bufp = respbuf + SWIGPS_MSGSIZE_HEADER;

    /* Mode - 1 byte */
    *bufp++ = (BYTE) operation;

    /* Timeout - 1 byte */
    *bufp++ = timeout;

    /* Interval - 4 bytes */
    piput32( interval, &bufp );

    /* Accuracy - 4 bytes */
    piput32( accuracy, &bufp );

    /* Send it to the swigps daemon */
    if ( socksend( fd, respbuf, (bufp - respbuf), errmsgbuf ) ) {
        LOGE("%s", errmsgbuf);
        return;
    }

    D("Sent %d bytes to swigps", (bufp-respbuf));
}

/**
 *
 * GPS QMI Agent to process a request to set the PDS default values
 * in a QMI modem. 
 * 
 * @param [in] paramp
 *     A pointer to the arguments sent by the swigps daemon. NOTE:
 *     This points to a block of allocated memory and must be freed
 *     before returning from this handler
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *      Runs in the context of the RIL daemon
 * 
 *      The incoming arguments are stored in network byte order in
 *      a buffer pointed to by paramp. The order of arguments is:
 * 
 *      fd        - File descriptor for socket connection to swigps
 *      args      - Packet bytes which follow the format of the 
 *                  PDS Defaults Request packet, documented in the
 *                  "Software Design Specification for Android GPS
 *                  Vendor Specific Location"
 *
 *      This function ignores the "tracking" status byte at the 
 *      end of the incoming parameters packet
 *
 */
void rilgpsSetDef( void *paramp )
{
    int written, fd;
    char *errp;
    ULONG operation;
    ULONG interval;
    ULONG accuracy;
    ULONG EnabledStatus;
    ULONG TrackingStatus;
    BYTE timeout;
    ULONG nret, ret;
    BYTE *tmpbufp, *bufp;
    char errmsgbuf[ERRMSGSIZE];

    /* No variable part, just the header is returned */
    BYTE respbuf[SWIGPS_MSGSIZE_HEADER];

    /* First, unpack the incoming arguments 
     * and free the incoming buffer pointer
     * before we make any decisions about 
     * whether to return early or not
     */
    bufp = (unsigned char *) paramp;
    tmpbufp = bufp;

    /* Get the fd from paramp */
    fd = piget32( &bufp );

    /* Mode - 1 byte */
    operation = (ULONG) *bufp++;

    /* Timeout - 1 byte */
    timeout = *bufp++;

    /* Interval - 4 bytes */
    interval = piget32( &bufp );

    /* Accuracy - 4 bytes */
    accuracy = piget32( &bufp );

    D("%s: op'n: %d, timeout: %d, interval: %d, accuracy: %d",
      __func__, (int) operation, (int) timeout, (int) interval, (int) accuracy);

    /* Done with the input buffer, free the memory */
    free( tmpbufp );

    D("%s: filedesc: %d",__func__, fd);

    /* Clear the response buffer prior to use */
    memset( respbuf, 0, sizeof(respbuf) );

    /* Get the tracking state first so 
     * we know what to do next
     */
    ret = GetPDSState( &EnabledStatus, &TrackingStatus );
    D("%s: Got PDS State: ret = %d, enabled=%d, tracking=%d",  
       __func__,(int) ret, (int) EnabledStatus, (int) TrackingStatus);

    /* Force the session to be stopped */
    ret = SetPDSState( DISABLE_TRACKING );

    D("%s Stopped session with ret = %d",__func__, (int) ret );

    /* Set the defaults now the session's stopped */
    nret = SetPDSDefaults( operation, timeout, interval, accuracy );

    LOGI("%s result %d",__func__, (int)nret );

	/* Re-enable the session if it was previously enabled */
    if( EnabledStatus ) {
        ret = SetPDSState( 1 );
        D("%s: tracking resumed, result %d",__func__, (int) ret);
    }

    /* Just send nret back along with the values received. 
     * If there was an error swigps can deal with it
     */

    /* Make the header */
    errp = swigps_makhdr( SWIGPS_MSGTYPE_RESP_SETDEF, 
                          SWIGPS_MSGSIZE_RESP_SETDEF, 
                          nret,
                          respbuf, sizeof(respbuf), &written );

    /* Ensure the header got built */
    if(errp) {
        LOGE("%s makhdr error: %s",__func__, errp);
        LOGE("Called with type %d, buflength %d", 
             SWIGPS_MSGTYPE_RESP_SETDEF, sizeof(respbuf) );
		return;
	}

    /* Send it to the swigps daemon */
    if ( socksend( fd, respbuf, written, errmsgbuf ) ) {
        LOGE("%s", errmsgbuf);
        return;
    }

    D("%s: sent %d bytes to swigps", __func__,written);
}

/**
 *
 * GPS QMI agent for getting AGPS information from the modem and
 * sending it back to the swigps daemon
 * 
 * @param [in] paramp
 *     Indicates which file handle to use when replying to the sgi side
 *     once the default PDS settings have been received from the modem
 *
 * @return
 *     none
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *      Runs in the context of the RIL daemon
 * 
 *      Not used at the moment. Placeholder in case it is needed 
 * 
 *      TODO This function does not support returning the AGPS Server's URL
 *      yet. Once the SLQS API supports returning the URL, this function
 *      should be updated to include it.
 *
 */
void rilgpsGetAgps( void *paramp )
{
    static BYTE respbuf[SWIGPS_MSGSIZE_HEADER+SWIGPS_MSGSIZE_MAX_AGPS];
    static BYTE urlstring[MAXURL];
    BYTE urlStringLen = MAXURL;
    int written, fd;
    char *errp;
    ULONG ipaddr;
    ULONG port;
    ULONG nret;
    BYTE *bufp;
    char errmsgbuf[ERRMSGSIZE];

    /* Get the fd from paramp */
    fd = (int) paramp;

    D("rilgpsGetAgps: using filedesc: %d", fd);

    /* Clear the response buffer prior to use */
    memset( respbuf, 0, sizeof(respbuf) );

    /* Initiate the QMI request - TODO, need to return urlstring in
	 * the response below.
     */
    nret = SLQSGetAGPSConfig( &ipaddr, &port, urlstring, &urlStringLen );

    LOGI("rilgpsGetAgps: got AGPS info with result %d", (int)nret );
    LOGI("   IP Addr: %x, Port: %d", (int) ipaddr, (int) port );

    /* Send nret back along with the values received. If there
     * was an error the recipient can deal with it
     */

    /* Copy of the pointer to the buffer */
    bufp = respbuf;

    /* Make the header */
    errp = swigps_makhdr( SWIGPS_MSGTYPE_RESP_GETAGPS, 
                   SWIGPS_MSGSIZE_MIN_AGPS, 
                   nret,
                   bufp, sizeof(respbuf), &written );

    /* Ensure the header got built */
    if(errp) {
        LOGE("%s makhdr error: %s",__func__, errp);
        LOGE("rilgpsGetAgps: Called with type %d, buflength %d", 
             SWIGPS_MSGTYPE_RESP_GETAGPS, sizeof(respbuf) );
		return;
	}

    /* Adjust bufp to the first byte past the header */
    bufp = respbuf + SWIGPS_MSGSIZE_HEADER;

    /* IPAddress */
    piput32( ipaddr, &bufp );

    /* Accuracy - 4 bytes */
    piput32( port, &bufp );

    /* Send it to the swigps daemon */
    if ( socksend( fd, respbuf, (bufp - respbuf), errmsgbuf ) ) {
        LOGE("%s", errmsgbuf);
        return;
    }

    D("rilgpsGetAgps: sent %d bytes to swigps", (bufp-respbuf));
}

/**
 *
 * GPS QMI agent for setting AGPS information on the modem and
 * sending it back to the swigps daemon
 * 
 * @param [in] 
 *     Pointer to an array of memory containing the values passed
 *     from the swigps package. Needs to be unpacked into local
 *     variables
 * @return
 *     none
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *      Runs in the context of the RIL daemon
 *
 *      This function is not re-entrant because the urlbuf is statically
 *      declared to save putting too much storage on the stack
 * 
 *      This function configures either the IP address and port number
 *      or the hostname URL and it's associated length. For this reason
 *      the AGPS interface function in swigps.c accepts an IP address in
 *      ASCII form and converts it into its binary equivalent. 
 *      
 *      The question of whether the QMI device would accept a host URL 
 *      and port number encoded as:
 *
 *                           www.hostname.com:<port>
 *
 *      is not known. Although the API accepts this combination it is 
 *      unclear whether it actually works as expected within a GOBI
 *      device
 *
 *      To use: 
 *      - If the IP Address and port number fields contain a value 
 *        then the IP address and port number are configured
 * 
 *      - If the IP address or port number are NULL and the host URL
 *        is a string longer than MINURL bytes then it is configured
 *
 */
void rilgpsSetAgps( void *paramp )
{
    int written, fd;
    char *errp;
    ULONG ipaddr;
    ULONG port;
    BYTE urllength;
    ULONG EnabledStatus;
    ULONG TrackingStatus;
    ULONG ret=0, qret=eQCWWAN_ERR_NONE, qret1, qret2;
    BYTE *tmpbufp, *bufp;
    char errmsgbuf[ERRMSGSIZE];

    /* Temporary storage for the URL */
    static BYTE urlbuf[MAXURL];

    /* Response buffer - sent back to swigps after modem responds */
    BYTE respbuf[SWIGPS_MSGSIZE_HEADER];

    /* First, unpack the incoming arguments 
     * and free the incoming buffer pointer
     * before we make any decisions about 
     * whether to return early or not
     */
    bufp = (unsigned char *) paramp;
    tmpbufp = bufp;

    /* Get the fd from paramp */
    fd = piget32( &bufp );

    /* ip address - 4 bytes */
    ipaddr = piget32( &bufp );

    /* server port - 4 bytes */
    port = piget32( &bufp );

    /* Length of the URL, not including NULL termination */
    urllength = *bufp++;

    /* Server URL - length variable */
    pigetmulti( &bufp, urlbuf, (unsigned short) urllength );

    /* NULL Terminate the URL */
    urlbuf[urllength] = 0;

    /* Done with the input buffer, free the memory */
    free( tmpbufp );

    D("rilgpsSetAGPS: filedesc: %d", fd);

    /* Summarize the changes to be made */
    D("rilgpsSetAgps: ipaddr: 0x%x, port: %d\n                urllength: %d, url: '%s'", (unsigned int)ipaddr, (int) port, (int) urllength, urlbuf );

    /* Get the tracking state first so 
     * we know what to do next. If enabled
     * need to disable tracking to change 
     * the AGPS settings
     */
    ret = GetPDSState( &EnabledStatus, &TrackingStatus );
    D("rilgpsSetAGPS PDS State: ret = %d, enabled=%d, tracking=%d",
       (int) ret, (int) EnabledStatus, (int) TrackingStatus);

    /* Force the session to be stopped */
    ret = SetPDSState( DISABLE_TRACKING );

    D("rilgpsSetAgps: Halted tracking: ret = %d", (int) ret );

    /* Call SLQSSetAGPSConfig() with the first form 
     * only if both the IP address and port are non-
     * NULL
     */
    if( ipaddr && port ) {
        qret1 = SLQSSetAGPSConfig( &ipaddr, &port, NULL, NULL);
        D("qret1 = %d", (int) qret1 );
        if( qret1 != eQCWWAN_ERR_NONE ) {
            LOGE("rilgpsSetAgps: qret1=%d", (int)qret1);
            qret = qret1;
        }
    }
    /* Call SLQSSetAGPSConfig() with the second form 
     * only if the URL length is not 0 and is reasonable
     */
	if( urllength > MINURL ) {
		qret2 = SLQSSetAGPSConfig( NULL, NULL, urlbuf, &urllength);
        D("qret2 = %d", (int)qret2 );
        if( qret2 != eQCWWAN_ERR_NONE ) {
            LOGE("rilgpsSetAgps: qret2=%d", (int)qret2);
            qret = qret2;
        }
    }

    /* Re-enable the session if it was previously enabled */
    if( EnabledStatus ) {
        ret = SetPDSState( ENABLE_TRACKING );
        D("rilgpsSetAgps: Tracking restarted, result %d", (int) ret);
    }

    /* Just send qretx back along with the values received. 
     * If there was an error swigps can deal with it
     */

    /* Make the header */
    errp = swigps_makhdr( SWIGPS_MSGTYPE_RESP_SETAGPS, 
                          SWIGPS_MSGSIZE_RESP_SETAGPS, 
                          qret,
                          respbuf, sizeof(respbuf), &written );

    /* Ensure the header got built */
    if(errp) {
        LOGE("%s makhdr error: %s",__func__, errp);
        LOGE("rilgpsSetAgps: type %d, buflength %d", 
             SWIGPS_MSGTYPE_RESP_SETAGPS, sizeof(respbuf) );
		return;
	}

    /* Send it to the swigps daemon */
    if ( socksend( fd, respbuf, written, errmsgbuf ) ) {
        LOGE("%s", errmsgbuf);
        return;
    }

    D("rilgpsSetAgps: sent %d bytes to swigps", written);
}

/**
 *
 * GPS QMI agent for handling requests to inject time into the 
 * modem
 * 
 * @param [in] 
 *     Pointer to an array of memory containing the values passed
 *     from the swigps package. Needs to be unpacked into local
 *     variables
 * @return
 *     none
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *      Runs in the context of the RIL daemon
 *
 *      This function is not re-entrant because the urlbuf is statically
 *      declared to save putting too much storage on the stack
 * 
 */
void rilgpsInjTime( void *paramp )
{
    int written, fd;
    char *errp;
    ULONGLONG time;
    ULONG uncertainty;
    ULONG EnabledStatus;
    ULONG TrackingStatus;
    ULONG ret=0, qret=eQCWWAN_ERR_NONE;
    BYTE *tmpbufp, *bufp;
    char errmsgbuf[ERRMSGSIZE];

    /* Temporary storage for the URL */
    static BYTE urlbuf[MAXURL];

    /* Response buffer - sent back to swigps after modem responds */
    BYTE respbuf[SWIGPS_MSGSIZE_HEADER];

    /* First, unpack the incoming arguments 
     * and free the incoming buffer pointer
     * before we make any decisions about 
     * whether to return early or not
     */
    bufp = (unsigned char *) paramp;
    tmpbufp = bufp;

    /* Get the fd from paramp */
    fd = piget32( &bufp );

    D("rilgpsInjTime: filedesc: %d", fd);

    /* 64 bit Time value */
    time = piget64( &bufp );

    /* Uncertainty = 1/2 round trip delay time to NTP server */
    uncertainty = piget32( &bufp );

    D("rilgpsInjTime: Got time=%llu, uncertainty=%d", time, (int)uncertainty);

    /* Done with the input buffer, free the memory */
    free( tmpbufp );

    /* Monitor the tracking state */
    ret = GetPDSState( &EnabledStatus, &TrackingStatus );
    D("rilgpsInjTime PDS State: ret = %d, enabled=%d, tracking=%d",
       (int) ret, (int) EnabledStatus, (int) TrackingStatus);

    /* Inject the time */
    qret = SLQSPDSInjectAbsoluteTimeReference( time, uncertainty, UTC, FORCE );
    D("qret = %d", (int)qret );
    if( qret != eQCWWAN_ERR_NONE ) {
        LOGE("rilgpsInjTime: qret=%d", (int)qret);
    }

    /* Just send qret back along with the values received. 
     * If there was an error swigps can deal with it
     */

    /* Make the header */
    errp = swigps_makhdr( SWIGPS_MSGTYPE_RESP_INJTIME, 
                          SWIGPS_MSGSIZE_RESP_INJTIME, 
                          qret,
                          respbuf, sizeof(respbuf), &written );

    /* Ensure the header got built */
    if(errp) {
        LOGE("%s makhdr error: %s",__func__, errp);
        LOGE("rilgpsInjTime: type %d, buflength %d", 
             SWIGPS_MSGTYPE_RESP_INJTIME, sizeof(respbuf) );
		return;
	}

    /* Send it to the swigps daemon */
    if ( socksend( fd, respbuf, written, errmsgbuf ) ) {
        LOGE("%s", errmsgbuf);
        return;
    }

    D("rilgpsInjTime: sent %d bytes to swigps", written);
}

/**
 *
 * GPS QMI agent for handling stop session requests 
 * 
 * @param [in] 
 *     Pointer to an array of memory containing the values passed
 *     from the swigps package. Unused
 * @return
 *     none
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *      Runs in the context of the RIL daemon
 *
 *      Does nothing, the Stop Session request is an AT modem-
 *      only requirement
 * 
 */
void rilgpsStopSession( void *paramp )
{
    /* Free incoming buffer */
    free( paramp );

    /* Do nothing */
    return;
}

/**
 *
 * GPS QMI agent for handling start session requests 
 * 
 * @param [in] 
 *     Pointer to an array of memory containing the values passed
 *     from the swigps package. Unused
 *     variables
 * @return
 *     none
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *      Runs in the context of the RIL daemon
 *
 *      Does nothing, the Start Session request is an AT modem-
 *      only requirement
 * 
 */
void rilgpsStartSession( void *paramp )
{
    /* Free incoming buffer */
    free( paramp );

    /* Do nothing */
    return;
}

