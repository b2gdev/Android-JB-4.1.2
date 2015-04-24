/*************
 *
 * Filename: amapi.c
 *
 * Purpose:  Contains routines related to handling packets
 *           defined by the AM package on the API side.
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/

/* include files */
#include "qm/qmerrno.h"
#include "am/amidefs.h"
#include <stdio.h>

/* Local storage */

/* AM API control block structure */
struct amapicb amapicblk;

/* Arrays used by the API for sending requests to and receiving responses from
 * the SDK. Request/Response arrays can be global because the protocol is stop
 * and wait for AM request/response operations. Notifications arrive
 * asynchronously from the SDK.
 */
local swi_uint8 amrequestblock[AMMAXREQBKLEN];
local swi_uint8 amresponseblock[AMMAXRESPBKLEN];

/* Mutex used to Protect the simultaneous access of the QMI request and
 * response buffers
 */
local pthread_mutex_t amreqrespblock_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Array used for storing the Notification received from Modem */
local swi_uint8 amnotificationblock[AMMAXNOTIFBKLEN];

/*************
 *
 * Name: amgetapicbp
 *
 * Purpose: Return a pointer to the AM package control block for the
 *          API-side
 *
 * Parms:   none
 *
 * Return:  pointer to the AM package control block structure
 *
 * Abort:   none
 *
 * Notes:   None
 *
 **************/
local struct amapicb *amgetapicbp( void )
{
    return &amapicblk;
}

/* Request/Response Buffer routines */

/*************
 *
 * Name: amgetparamp
 *
 * Purpose: Return a pointer to the SDK/QMI parameter field, given the
 *          AM SDK/QMI packet type of interest and a pointer to the beginning
 *          of the AM packet.
 *
 * Parms:   amqmitype     - Which of the QMI-specific messages we want
 *                          the pointer to the parameter field of.
 *
 *          bufstartp     - Pointer to the start of the buffer that will/
 *                          already does contain the QMI message
 *
 * Return:  Number of bytes to the start of the parameter field
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
global swi_uint8 *amgetparamp(
    swi_uint16 amqmitype,
    swi_uint8 *bufstartp )
{
    swi_uint32 offset;

    switch(amqmitype)
    {
        case AMTYPEQMIREQUEST:
            offset = AMQMIREQTOTAL;
            break;

        case AMTYPEQMIRESPONSE:
            offset = AMQMIRESPTOTAL;
            break;
        case AMTYPESDKREQUEST:
            offset = AMSDKREQTOTAL;
            break;
        case AMTYPESDKRESPONSE:
            offset = AMSDKRESPTOTAL;
            break;

        default:
            return NULL;
    }

    return offset + bufstartp;
}

/*************
 *
 * Name: amgetNotifbufp
 *
 * Purpose: Return a pointer to sufficient storage for holding a
 *          AM Notification for transmission to the SDK side.
 *
 * Parms:   none
 *
 * Return:  Pointer to an array of swi_uint8 large enough to contain
 *          the biggest possible AM request
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
global swi_uint8 *amgetNotifbufp( void )
{
    return amnotificationblock;
}

/*************
 *
 * Name: amgetreqbufp
 *
 * Purpose: Return a pointer to sufficient storage for holding a
 *          AM request for transmission to the SDK side.
 *
 * Parms:   none
 *
 * Return:  Pointer to an array of swi_uint8 large enough to contain
 *          the biggest possible AM request
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
global swi_uint8 *amgetreqbufp( void )
{
    /* Lock the request and response buffers - this is
     * done from the beginning of every API execution
     */
    pthread_mutex_lock(&amreqrespblock_mutex);
    return(&amrequestblock[0]);
}

/*************
 *
 * Name:    amrelreqbufp
 *
 * Purpose: Releases the pointer holding an AM request for
 *          transmission to the SDK side.
 *
 * Parms:   none
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
global void amrelreqbufp( void )
{
    /* Unlock the request and response buffers - this is
     * done at the end of every API execution
     */
    pthread_mutex_unlock(&amreqrespblock_mutex);
}

/*************
 *
 * Name: amgetrespbufp
 *
 * Purpose: Return a pointer to sufficient storage for holding a
 *          AM response received from the SDK side.
 *
 * Parms:   none
 *
 * Return:  Pointer to an array of swi_uint8 large enough to contain
 *          the biggest possible AM response
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local swi_uint8 *amgetrespbufp( void )
{
    return(&amresponseblock[0]);
}
/*************
 *
 * Name: amgetrespbufsz
 *
 * Purpose: Return the size of the response block, in bytes
 *
 * Parms:   none
 *
 * Return:  size of the amresponseblock array, in bytes
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local swi_uint32 amgetrespbufsz( void )
{
    return sizeof( amresponseblock );
}

/*
 ******************* Utility Routines ********************
 */
/* Packet Building Routines */

/*************
 *
 * Name: ambuildrequest
 *
 * Purpose: Build a AM request packet in the provided storage area
 *          given the information contained in the parameter structure
 *
 * Parms:   amsgpp          -Pointer to pointer to storage into which the
 *                           message will be written
 *          amrrparmp      - Pointer to a structure containing the request
 *                           parameters
 *
 * Return:  Number of bytes occupied by the AM packet.
 *
 * Abort:   none
 *
 * Notes:   AM request packets travel from the API-side to the SDK
 *          side
 *
 **************/
local swi_uint16 ambuildrequest(
    swi_uint8        **amsgpp,
    struct amrrparms *amrrparmp )
{
    swi_uint16 amsize = 0;

    switch( amrrparmp->amparmtype )
    {
        case SWI_PARM_QMI:
            amsize = ambuildqmixactionheader( amsgpp,
                        &amrrparmp->amparm.amqmi,
                        AMTYPEQMIREQUEST );
            amsize += amrrparmp->amparm.amqmi.amqmixactionlen;
            break;

        case SWI_PARM_SDK:
            amsize = ambuildsdkrequest( amsgpp,
                                        &amrrparmp->amparm.amsdk,
                                        AMTYPESDKREQUEST );
            break;
         default:
            break;
    }
    return( amsize );
}

/*
 ******************* Parsing Routines ********************
 */

/*************
 *
 * Name: amparseresponse
 *
 * Purpose: This routine knows how to unpack the contents of AM
 *          response messages
 *
 * Parms:   ampacketpp  - pointer to a pointer to an AM response
 *                        packet.
 *          amtype        Type of AM message
 *          amrrparmp   - Pointer to a caller-allocated structure
 *                        defined by AM package for exchanging
 *                        Request/Response parameters. Saves
 *                        long lists of calling arguments
 *
 * Return:  TRUE  - the response has been parsed
 *          FALSE - the AM response type is unknown
 *
 * Abort:   none
 *
 * Notes:   Parses the fields from an AM response packet and returns
 *          the values to the caller in the structure provided
 *
 **************/
local swi_bool amparseresponse(
    swi_uint8 **ampacketpp,
    enum ammsgtypes amtype,
    struct amrrparms *amrrparmp)
{
    swi_bool retval = FALSE;

    switch(amtype)
    {
        case AMTYPEQMIRESPONSE:
            /* if the response type matches the request type */
            if(amrrparmp->amparmtype == SWI_PARM_QMI)
            {
                /* parse QMI specfic AM packet payload */
                amparseqmixactionheader( ampacketpp,
                                         &amrrparmp->amparm.amqmi );
                retval = TRUE;
            }
            break;
       case AMTYPESDKRESPONSE:
            /* if the response type matches the request type */
            if(amrrparmp->amparmtype == SWI_PARM_SDK)
            {
                /* parse SDK specfic AM packet payload */
                amparsesdkheader( ampacketpp,
                                  &amrrparmp->amparm.amsdk );
                retval = TRUE;
            }
            break;
        default:
            /* the AM response type is unsupported by the API side */
            retval = FALSE;
            break;
    }

    return(retval);
}

/*************
 *
 * Name: amparsenotify
 *
 * Purpose: This routine knows how to unpack the contents of AM
 *          response messages
 *
 * Parms:   apipcmsgp  - pointer to a pointer to an AM response
 *                        packet.
 *          amtype     - type of AM packet to be parsed
 *          amrrparmp  - Pointer to a caller-allocated structure
 *                       defined by AM package for collecting notification
 *                       information.
 *
 * Return:  TRUE  - the notification has been parsed
 *          FALSE - the AM notification type is unknown
 *
 * Abort:   none
 *
 * Notes:   Parses the fields from an AM response packet and returns
 *          the values to the caller in the structure provided
 *
 *
 **************/
local swi_bool amparsenotify(
    swi_uint8 **apipcmsgp,
    enum ammsgtypes amtype,
    struct amrrparms *parmsp)
{
    swi_bool retval = TRUE;

    /* parse data based on AM packet type */
    switch(amtype)
    {
        case AMTYPEQMINOTIFICATION:
            parmsp->amparmtype = SWI_PARM_QMI;
            amparseqmixactionheader( apipcmsgp,
                                     &parmsp->amparm.amqmi );
            break;
        default:
            retval = FALSE;
    }
    return(retval);
}

/*************
 *
 * Name: amsendnwait
 *
 * Purpose: Format an API ongoing AM packet using the calling arguments
 *          send it on its way and block until a response is received.
 *          Pass the response back to the caller
 *
 * Parms:   amhdrp     - Pointer to the beginning of the packet that
 *                       will contain an AM Message
 *          amparamsp  - [IN] Pointer to a structure containing valid AM
 *                       parameters sufficient to build an outgoing AM
 *                       request packet
 *                       [OUT] Contains the parsed fields from an incoming AM
 *                       response packet
 *          responsepp - Pointer to a pointer which will be loaded with
 *                       the address of the Parameter field received
 *                       from the modem
 *
 * Return:  Value of enum eQCWWANError
 *
 * Abort:   none
 *
 * Notes:   This is a blocking call which returns only after the modem
 *          has sent its response or the timeout (stored in amparamsp)
 *          has expired, whichever is sooner
 *
 *          This function returns the caller a pointer to the received
 *          buffer data in responsepp. The actual storage holding this
 *          data is global to this package. It cannot be overwritten
 *          until the next icapi_rcv() call is posted which can only
 *          happen when the application sends the next message. There
 *          are NO RESOURCES TO RELEASE on the return path
 *
 **************/
global enum eQCWWANError amsendnwait(
    swi_uint8 *amhdrp,
    struct amrrparms *amparamsp,
    swi_uint8 **responsepp )
{
    swi_bool result;                 /* Result of call to IPC read routine */
    swi_uint16 amsize;           /* Returned by pkt building routine */
    swi_uint16 amtype, amlength; /* Returned by amchecktype() call */
    swi_uint16 amhdrversion;     /* Returned by amchecktype() call */
    swi_uint32 buflength;        /* Used in call to icapi_rcv() */
    struct amapicb *amapicbp;    /* Pointer to control block */
    swi_uint8 *hdrp;             /* Local copy of calling argument */
    swi_uint8 *inbufp;           /* Local pointer to buffer from IPC RCV */
    enum eQCWWANError amresultcode;

    /* Initialize the control block pointer */
    amapicbp = amgetapicbp();

    /* initialize the local copy */
    hdrp = amhdrp;

    /* Build the AM packet first */
    amparamsp->amresultcode = eQCWWAN_ERR_NONE; /* result code for requests */
    amsize = ambuildrequest( &hdrp,
                             amparamsp );

    /* Send the packet to the SDK side */
    icapi_send(&amapicbp->amapiipcrr, amhdrp, amsize);

    /* Make a local copy of the buffer for storing the received IPC msg */
    inbufp = amgetrespbufp();
    slmemset((char *)inbufp, 0, AMMAXRESPBKLEN);

    /*
     * Determine the maximum size of response that can be
     * accommodated in the local buffer
     */
    buflength = amgetrespbufsz();

    /* if a non-zero timeout is specified */
    if(amparamsp->amtimeout)
    {
        /*
         * adjust the timeout value so to ensure that the API side
         * times out before the SDK side.
         */
        amparamsp->amtimeout = (AMTIMEGUARD + amparamsp->amtimeout);

        /* for relatively short timeouts provide a large cushion */
        if( amparamsp->amtimeout <= 10000 )
        {
            amparamsp->amtimeout <<= 1;
        }
        else
        {
            /* otherwise, provide a 25% margin */
            amparamsp->amtimeout += amparamsp->amtimeout >> 2;
        }
    }

    /* Now, block waiting for a response from the modem or a timeout */
    result = icapi_rcv( &amapicbp->amapiipcrr,
                        inbufp,
                        &buflength,
                        amparamsp->amtimeout);

    /* Determine if there was a timeout. Note, it's possible this
     * is a real timeout or that the SDK task has disappeared
     */
    if( ( !buflength ) || ( FALSE == result ) )
    {
        /* Timeout */
        return(eQCWWAN_ERR_SWICM_TIMEOUT);
    }

    /* Parse the header of the received AM packet */
    amparseamheader(
        &amtype,
        &amhdrversion,
        &amresultcode,
        &amlength,
        &inbufp);

    /* if this is not a short response from the SDK */
    if(amlength)
    {
        /* Parse AM response packet payload */
        if(!amparseresponse(&inbufp, amtype, amparamsp))
        {
            /* if response type is unknown */
            return(eQCWWAN_ERR_INVALID_QMI_RSP);
        }
    }

    /* return pointer to the end of the parsed fields */
    *responsepp = inbufp;

    /* convert the AM result code to a SWI_RCODE and return */
    return(amresultcode);

}
/*************
 *
 * Name:    amapiwaitnotif
 *
 * Purpose: Waits for a notification on an IPC notification channel.
 *
 * Parms:   apipcmsgp       - pointer to storage allocated for receiving
 *                            indication.
 *          apipcbuflngp    - pointer to requested number of octets to be read/
 *                            number of actually read.
 *          parmsp          - pointer to parameter structure for parsed indication
 *                            fields.
 *          payloadpp       - pointer to message payload
 *          timeout         - socket operation timeout value (milliseconds)
 *                            0 = wait forever
 *
 * Return:  TRUE            - notification message received successfully
 *          FALSE           - error in receiving notification message
 *
 * Abort:   if received AM packet header version is not supported
 *          if received AM packet type is not supported
 *
 * Notes:   When timeout occurs this function returns TRUE and sets the buffer
 *          length return parameter to zero.
 *
 *
 **************/
global swi_bool amapiwaitnotif(
    swi_uint8        *apipcmsgp,
    swi_uint32       *apipcbuflngp,
    struct amrrparms *parmsp,
    swi_uint8        **payloadpp,
    swi_uint32       timeout )
{
    /* Get Notification Channel Handle */
    struct amapicb *amapnotcbp  = amgetapicbp();

    swi_uint8 *packetp;
    packetp = apipcmsgp;
    swi_bool result;
    /* Wait for IPC message on notification channel */
    result = icapi_rcv( &amapnotcbp->amapiipcnot,
                        packetp,
                        apipcbuflngp,
                        timeout);

    /* When notification is received successfully */
    if( result && (*apipcbuflngp != 0) )
    {

        swi_uint16 amtype;
        swi_uint16 amlength;
        swi_uint16 amhdrver;
        enum eQCWWANError amresultcode;
        /* Parse the header of the received AM packet */
        amparseamheader(&amtype,
                        &amhdrver,
                        &amresultcode,
                        &amlength,
                        &packetp);

        /* First ensure the AM packet header matches */
        if( amhdrver != AMHDRVERSION )
        {
            /* Put both values into our 32 bit error data */
            amhdrver= amhdrver<< 16 | AMHDRVERSION;
            char errmsg[100];
            snprintf(errmsg, sizeof(errmsg),
                     "%s:%d API/SDK IPC hdr mismatch",
                     (char *)__func__, __LINE__);
            erAbort(errmsg, amhdrver );
        }

        /* Parse known AM packet type */
        if(!amparsenotify(&packetp, amtype, parmsp))
        {
            char errmsg[100];
            snprintf(errmsg, sizeof(errmsg),
                     "%s:%d API/SDK IPC unknown packet",
                     (char *)__func__, __LINE__);
            erAbort(errmsg, amtype );
        }

        /* update client's message payload pointer */
        *payloadpp = packetp;
    }

    return result;
}

/*************
 *
 * Name: aminitapi
 *
 * Purpose: Initialize the AM package for the API side
 *
 * Parms:   none
 *
 * Return:  TRUE  - Initialized successfully
 *          FALSE - Unable to open an IPC channel for use
 *
 * Abort:   none
 *
 * Notes:   Don't confuse this function with the SDK-side function
 *          of exactly the same name. There should never be a linker
 *          conflict because this file's object is not part of the
 *          SDK link and vice-versa
 *
 **************/
global swi_bool aminitapi(void)
{
    swi_bool retval = FALSE;
    struct amapicb *amapicbp = amgetapicbp();
    int i;
    swi_uint16  numappchan = 0;
    swi_uint8   *chnamep;
    swi_uint8   *pchnamep;
    swi_uint16  channel, pchannel;

    /* Initialize the Channel Information package */
    ciinit();

    numappchan = cigetnumRRchan();

    /*
     * Loop through all the channels to get the first available
     * (Request, Response)/Notification pair.
     */
    for (i=0; i < numappchan; i++)
    {
        /* Initialize the IC handle for Request/Response channel */
        channel = cigetnextrrchannum(i);
        chnamep = cigetchannelname(channel);
        retval = ciinitapiipcchan(&amapicbp->amapiipcrr, chnamep);
        if ( retval )
        {
            /* get the peer Notification channel as well */
            pchannel = cigetpeerchan( channel );
            pchnamep = cigetchannelname(pchannel);
            /* Initialize the IC handle for Notification channel */
            retval = ciinitapiipcchan(&amapicbp->amapiipcnot, pchnamep);
            /* if both RR/Notification channels are OK. we can proceed */
            if ( retval )
                break;
        }
    }
    return retval;
}

/*************
 *
 * Name:    ambuildsdkrequest
 *
 * Purpose: Build an sdk request packet.
 *
 * Parms:   amsgpp  - double pointer to storage into which the message will
 *                    be written
 *
 * Return:  Number of bytes occupied by the AM packet
 *
 * Abort:   none
 *
 * Notes:   This function builds the SDK request packets, including the
 *          common AM header
 *
 *          Request packets travel from API side to the SDK-side
 *
 **************/
package swi_uint16 ambuildsdkrequest(
    swi_uint8 **amsgpp,
    struct    amsdkrrparms *amsdkp,
    enum      ammsgtypes   amtype )
{
    swi_uint8  *packetp;        /* Local copy of incoming packet pointer */
    swi_uint16 ampacketlength;  /* For the Common part of the AM header */
    swi_uint32 byteswritten;    /* After parsing, contains #bytes just read */

    /* Duplicate the pointer into our scratch location */
    packetp = *amsgpp;

    switch (amsdkp->amsdkrrtype)
    {
        case AMSDKREQVERSION:
            ampacketlength = AMSDKVERSIONREQTOTAL;
            break;
        case AMSDKREQPID:
            ampacketlength = AMSDKPIDREQTOTAL;
            break;
        case AMSDKREQKILL:
            ampacketlength = AMKILLSDKREQTOTAL;
            break;
       default:
           {
               char errmsg[100];
                snprintf(errmsg, sizeof(errmsg),
                        "%s:%d invalid SDK request type",
                        (char *)__func__, __LINE__);
                erAbort(errmsg, amsdkp->amsdkrrtype );
           }
            break;
    }

    /* Build AM packet header */
    ambuildamheader(
        amtype,
        AMHDRVERSION,
        eQCWWAN_ERR_NONE,
        ampacketlength,
        &packetp);

    /* Request type comes next */
    *packetp++ = amsdkp->amsdkrrtype;

    /* Update the bytes read variable here */
    byteswritten = packetp - *amsgpp;

    if( byteswritten != ampacketlength )
    {
        /* Combine both sizes into one and abort */
        byteswritten = byteswritten << 16 | ampacketlength;
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d Update AMSDK***REQTOTAL", (char *)__func__, __LINE__);
        erAbort(errmsg, byteswritten );
    }

    /* advance the caller's packet pointer */
    *amsgpp += byteswritten;

    return byteswritten;
}

