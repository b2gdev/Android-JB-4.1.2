/* 
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
#include <stdio.h>
#include <unistd.h>

#include <cutils/sockets.h>

#include "swigps_common.h"


/*********************************************************
 *                  MESSAGE DEFINITIONS
 *********************************************************/

/**
 * 
 * Message Type Definitions
 *
 * Indexed by swigpsMsgType enumeration and contains a 
 * range of allowable packet lengths for the variable-
 * sized portion of an IPC message. The header size is
 * fixed and not included in these values 
 *
 */
gpsMsgProps msgProperties[SWIGPS_MSGTYPE_MAXVAL] = 
{
    {SWIGPS_MSGSIZE_REQ_GETNMEAPORT, SWIGPS_MSGSIZE_REQ_GETNMEAPORT},
    {SWIGPS_MSGSIZE_RESP_MIN_GETNMEAPORT, SWIGPS_MSGSIZE_RESP_MAX_GETNMEAPORT},

    {SWIGPS_MSGSIZE_REQ_GETDEF,  SWIGPS_MSGSIZE_REQ_GETDEF},
    {SWIGPS_MSGSIZE_RESP_GETDEF, SWIGPS_MSGSIZE_RESP_GETDEF},

    {SWIGPS_MSGSIZE_REQ_SETDEF,  SWIGPS_MSGSIZE_REQ_SETDEF},
    {SWIGPS_MSGSIZE_RESP_SETDEF, SWIGPS_MSGSIZE_RESP_SETDEF},

    {SWIGPS_MSGSIZE_REQ_GETAGPS, SWIGPS_MSGSIZE_REQ_GETAGPS},
    {SWIGPS_MSGSIZE_MIN_AGPS,    SWIGPS_MSGSIZE_MAX_AGPS},

    {SWIGPS_MSGSIZE_MIN_AGPS,    SWIGPS_MSGSIZE_MAX_AGPS},
    {SWIGPS_MSGSIZE_RESP_SETAGPS,SWIGPS_MSGSIZE_RESP_SETAGPS},

    {SWIGPS_MSGSIZE_REQ_INJTIME, SWIGPS_MSGSIZE_REQ_INJTIME},
    {SWIGPS_MSGSIZE_RESP_INJTIME,SWIGPS_MSGSIZE_RESP_INJTIME},

    {SWIGPS_MSGSIZE_REQ_STOP, SWIGPS_MSGSIZE_REQ_STOP},
    {SWIGPS_MSGSIZE_RESP_STOP,SWIGPS_MSGSIZE_RESP_STOP},

    {SWIGPS_MSGSIZE_REQ_START, SWIGPS_MSGSIZE_REQ_START},
    {SWIGPS_MSGSIZE_RESP_START,SWIGPS_MSGSIZE_RESP_START}
};

/*********************************************************
 *    EXECUTABLES COMMON TO SWIGPS AND SWIRIL_GPS
 *********************************************************/

/**
 *
 * Get a 16-bit value, in network byte order, from a packet and
 * increment the packet pointer
 *
 * @param[in] packetpp 
 *     pointer to the packet where the 16-bit value will be read
 * @param[out] packetpp 
 *     location where the packet pointer will be incremented
 *
 * @return
 *      the 16-bit value
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *      This function performs no pointer validation
 */
unsigned short piget16(unsigned char **packetpp)
{
    unsigned short field;
    unsigned char *packetp;

    packetp = *packetpp;

    field = *packetp++;
    field <<= 8;
    field += *packetp++;

    *packetpp = packetp;

    return field;
}

/**
 *
 * Get a 32-bit value, in network byte order, from a packet and
 * increment the packet pointer
 *
 * @param[in] packetpp 
 *     pointer to the packet where the 32-bit value will be read
 * @param[out] packetpp 
 *     location where the packet pointer will be incremented
 *
 * @return
 *      the 32-bit value
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *      This function performs no pointer validation
 */
unsigned int piget32(unsigned char **packetpp)
{
    unsigned int field;
    unsigned char *packetp;

    packetp = *packetpp;

    field = *packetp++;
    field <<= 8;
    field += *packetp++;
    field <<= 8;
    field += *packetp++;
    field <<= 8;
    field += *packetp++;

    *packetpp = packetp;

    return field;
}

/**
 *
 * Get a 64-bit value, in network byte order, from a packet and
 * increment the packet pointer
 *
 * @param[in] packetpp 
 *     pointer to the packet where the 64-bit value will be read
 * @param[out] packetpp 
 *     location where the packet pointer will be incremented
 *
 * @return
 *      the 64-bit value
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *      This function performs no pointer validation
 */
unsigned long long piget64(unsigned char **packetpp)
{
    unsigned long long field;
    unsigned char *packetp;

    packetp = *packetpp;

    field = *packetp++;
    field <<= 8;
    field += *packetp++;
    field <<= 8;
    field += *packetp++;
    field <<= 8;
    field += *packetp++;
    field <<= 8;
    field += *packetp++;
    field <<= 8;
    field += *packetp++;
    field <<= 8;
    field += *packetp++;
    field <<= 8;
    field += *packetp++;

    *packetpp = packetp;

    return field;
}


/**
 *
 * Get an array of 8-bit fields from a packet and
 * increment the packet pointer
 *
 * @param[in] packetpp 
 *     pointer to pointer to the packet where the array will be read from
 * @param[out] packetpp 
 *     location where the packet pointer will be incremented
 * @param[out] bufferp 
 *     location where the array of 8-bit fields will be copied
 * @param numfields 
 *     number of 8-bit fields
 *
 * @return
 *      none
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *      This function performs no pointer validation
 */
void pigetmulti(unsigned char **packetpp, 
                unsigned char *bufferp, 
                unsigned short numfields)
{
    unsigned char *packetp;
    int i;

    packetp = *packetpp;

    for(i = numfields - 1; i >= 0; i--)
    {
        *bufferp++ = *packetp++;
    }

    *packetpp = packetp;
}

/**
 *
 * Put a 16-bit value, in network byte order, into a packet and
 * increment the packet pointer
 *
 * @param field 
 *     16-bit field value to insert
 * @param[in] packetpp 
 *     pointer to the packet where the 16-bit field will be written
 * @param[out] packetpp 
 *     location where the packet pointer will be incremented
 *
 * @return
 *      none
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *      This function performs no pointer validation
 */
void piput16(unsigned short field, unsigned char **packetpp)
{
    unsigned char *packetp;

    packetp = *packetpp;

    *packetp++ = (unsigned char)(field >> 8);
    *packetp++ = (unsigned char)field;

    *packetpp = packetp;
}

/**
 *
 * Put a 32-bit value, in network byte order, into a packet and
 * increment the packet pointer
 *
 * @param field 
 *     32-bit field value to insert
 * @param[in] packetpp 
 *     pointer to the packet where the 32-bit field will be written
 * @param[out] packetpp 
 *     location where the packet pointer will be incremented
 *
 * @return
 *      none
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *      This function performs no pointer validation
 */
void piput32(unsigned int field, unsigned char **packetpp)
{
    unsigned char *packetp;

    packetp = *packetpp;

    *packetp++ = (unsigned char)(field >> 24);
    *packetp++ = (unsigned char)(field >> 16);
    *packetp++ = (unsigned char)(field >> 8);
    *packetp++ = (unsigned char)field;

    *packetpp = packetp;
}

/**
 *
 * Put a 64-bit value, in network byte order, into a packet and
 * increment the packet pointer
 *
 * @param field 
 *     64-bit field value to insert
 * @param[in] packetpp 
 *     pointer to the packet where the 64-bit field will be written
 * @param[out] packetpp 
 *     location where the packet pointer will be incremented
 *
 * @return
 *      none
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *      This function performs no pointer validation
 */
void piput64(unsigned long long field, unsigned char **packetpp)
{
    unsigned char *packetp;

    packetp = *packetpp;

    *packetp++ = (unsigned char)(field >> 56);
    *packetp++ = (unsigned char)(field >> 48);
    *packetp++ = (unsigned char)(field >> 40);
    *packetp++ = (unsigned char)(field >> 32);
    *packetp++ = (unsigned char)(field >> 24);
    *packetp++ = (unsigned char)(field >> 16);
    *packetp++ = (unsigned char)(field >> 8);
    *packetp++ = (unsigned char)field;

    *packetpp = packetp;
}

/**
 *
 * Put an array of 8-bit fields into a packet and
 * increment the packet pointer
 *
 * @param fieldsp 
 *     array of 8-bit fields
 * @param[in] packetpp 
 *     pointer to the packet where the array of 8-bit fields will be written
 * @param[out] packetpp 
 *     location where the packet pointer will be incremented
 *
 * @return
 *      none
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *      This function performs no pointer validation
 */
void piputmulti(unsigned char *fieldsp, 
                unsigned char **packetpp, 
                unsigned short numfields)
{
    unsigned char *packetp;
    int i;

    packetp = *packetpp;

    for(i = numfields - 1; i >= 0; i--)
    {
        *packetp++ = *fieldsp++;
    }

    *packetpp = packetp;
}

/**
 *
 * Build a message header
 * 
 * The message header consists of 2-byte fields, the type, result and 
 * body length, respectively. This function builds a header into the 
 * storage area passed in by the caller.
 *
 * @param [in] type:
 *     The type to be assigned to the header
 * @param[in] length
 *     The length to put into the length field of the header. Range-checked
 *     by this function as part of validation
 * @param[in] result
 *     For Outbound requests this is zero. For inbound requests, contains the
 *     QMI API result code
 * @param[in] bufp
 *     Pointer to storage into which the header will be written, provided
 *     by the caller
 * @param[in] buflength
 *     The length available in bufp. Used for validation
 * @param[out] writtenp
 *     Pointer to storage into which the number of bytes written into 
 *     the buffer will be returned.
 *
 * @return
 *     NULL - No errors
 *          - Pointer to a string indicating the error that occurred otherwise
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *      none
 *
 */
char *swigps_makhdr( swigpsMsgType type, int length, 
                   unsigned short result, unsigned char *bufp, 
                   int buflength, int *writtenp )
{
    unsigned char *localbufp;   /* Local copy of bufp */
    gpsMsgProps *msgProps;

    /* Validate incoming arguments */
    if( buflength < SWIGPS_MSGSIZE_HEADER ) {
        return "Buflength too small for header";
    }

    if( type >= SWIGPS_MSGTYPE_MAXVAL ) {
        return "Invalid message type";
    }

    msgProps = msgProperties + type;

    /* Length falls between specified min and max values, inclusive */
    if( length >= msgProps->minlength && length <= msgProps->maxlength ) {
        localbufp = bufp;
        piput16( type, &localbufp );
        piput16( result, &localbufp );
        piput16( length, &localbufp );
        *writtenp = localbufp - bufp;
        return NULL;
    }

    return "Invalid length for msg type";
}


/**
 *
 * Receive a single packet on the socket using the packet's header 
 * information as a guide to knowing when a complete packet has been
 * received.
 * 
 * The length of the received packet is returned in numbytes using similar
 * return values as the standard recv() function.
 *
 * @param fd 
 *     the fd for the socket
 * @param[out] datap 
 *     buffer for the packet
 * @param[n/out] numbytes 
 *     - on input, the amount of space available in datap's array
 *     - on output:
 *       if numbytes>0     : number of bytes written to the buffer
 *       if numbytes==0    : socket was closed by other side
 *       if numbytes==(-1) : nothing to read on the socket; try again later
 * @param [out] errmsgbuf
 *     Pointer to storage to contain an error string if one is generated 
 *     by this function. Caller must ensure the buffer is sufficient size
 *     for errors generated by this function
 * @param [in] errmsgsize
 *     The number of bytes available in the error message buffer
 *
 * @return
 *     0: If no errors were found. In this case, the values in numbytes
 *        and, if appropriate, the return buffer are valid.
 *    -1: 
 *        If an error is found. None of the other return values are valid.
 *        This indicates that there is some error with the socket being 
 *        read.  It is up to the caller to decide what to do but further 
 *        reads of the socket should not be attempted as they will
 *        probably return garbage.
 *
 * @par Abort:<br> 
 *      none
 *
 * @note
 *      - there is no buffer overflow checking, since the buffer size is not
 *        currently passed into the function.  The buffer size is assumed to
 *        be at least MAXDATASIZE.
 */
int receivepacket(int fd, unsigned char *datap, int *numbytes, char *errmsgbuf, int errmsgsize)
{
    unsigned char *bufp;
    int count;
    int totalbytes;
    int nbytes;
    int length;

    bufp = datap;
    /* Only accept enough bytes to receive the header for now */
    nbytes = recv(fd, bufp, SWIGPS_MSGSIZE_HEADER, 0);

    if (nbytes == -1)
    {
        /* An error occured in the recv() call.  Determine whether this is due
         * to no more data in the non-blocking socket, or something else.
         */
        if (errno == EAGAIN)
        {
            /* timed out on a non-blocking socket; try again later */
            *numbytes=-1;
            return 0;
        }
        else
        {
            /* some other (non-valid) error occured */
            snprintf(errmsgbuf, errmsgsize, "recv %s", strerror(errno));
            return -1;
        }
    }
    else if (nbytes == 0)
    {
        /* the socket connection was closed by the other side */
        *numbytes=0;
        return 0;
    }
    else if (nbytes != SWIGPS_MSGSIZE_HEADER)
    {
        snprintf(errmsgbuf, errmsgsize, 
                 "read length field error %d", nbytes);
        return -1;
    }
    /* Read thrice because length is the 3rd 16 bits in the header. 
     * OK to discard these reads because they will be picked up 
     * after this call. These reads also reposition bufp to the 
     * first byte of the variable part of the incoming buffer
     */
    length = piget16(&bufp);
    length = piget16(&bufp);
    length = piget16(&bufp);
    if (length > MAXGPS - SWIGPS_MSGSIZE_HEADER)
    {
        snprintf(errmsgbuf, errmsgsize, 
                 "response too big error %d", length);
        return -1;
    }

    /* From this point on, the header has been received and
     * now it's time to read the remainder of the buffer as
     * denoted by the "length" field from the header. Note
     * From our piget16() calls, above, bufp points to the 
     * next byte after the header, so it can be used below.
     */
    nbytes = 0;
    count = 0;
    totalbytes = 0;

    /* Proceed to receive all advertised bytes before returning to caller */
    while (totalbytes < length)
    {
        if ((nbytes = recv(fd, 
                           (bufp + totalbytes), 
                           (length - totalbytes), 
                           0)) == -1) {

            snprintf(errmsgbuf, errmsgsize, "recv %s", strerror(errno));
            return -1;
        }
        totalbytes += nbytes;
        /* to prevent deadlock */
        if (count ++ > MAX_READ_ITERATIONS)
        {
            snprintf(errmsgbuf, errmsgsize, 
                     "can't get enough data in %d loops", count);
            return -1;
        }
    }

    /* Always read a minimum of the file header */
    *numbytes = SWIGPS_MSGSIZE_HEADER + length;
    return 0;
}


/**
 * 
 * Send a message to the specified socket
 *
 * @param [in] fd
 *    File descriptor representing the socket to send to
 * @param [in] bufp
 *    Pointer to the buffer containing the message to send
 * @param [[in] length
 *    Length of the packet to be transmitted (bytes)
 * @param [out] errmsgbuf
 *     Pointer to storage to contain an error string if one is generated 
 *     by this function. Caller must ensure the buffer is sufficient size
 *     for errors generated. On a normal return, code = 0, the error msg
 *     buffer contains a log message indicating how many bytes were sent
 *
 * @return
 *     0: If no errors were found. In this case, the values in numbytes
 *        and, if appropriate, the return buffer are valid.
 *    -1: 
 *        If an error is found. None of the other return values are valid.
 *        This indicates that there is some error with the socket being 
 *        read.  It is up to the caller to decide what to do but further 
 *        reads of the socket should not be attempted as they will
 *        probably return garbage.
 * 
 * @par abort:<br>
 *    none
 * 
 * @note
 *    none
 *
 */

int socksend( int fd, void *bufp, size_t length, char *errmsgbuf)
{
    int ret;

    /* Send the message */
    ret = send( fd, bufp, length, 0 );

    /* Validate the attempt */
    if( ret > 0 ) {
        sprintf(errmsgbuf, "sent %d bytes", ret );
        return 0;
    }
    
    /* There was an error sending */
    sprintf(errmsgbuf, "Error %d sending packet", ret );
    
    return -1;
}

