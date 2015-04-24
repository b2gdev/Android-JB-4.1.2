/*************
 *
 * Filename: qmtask_sdk.c
 *
 * Purpose:  qm task
 *
 * Copyright: © 2011-12 Sierra Wireless Inc., all rights reserved
 *
 **************/

/*---------------
  include files
 ---------------*/
#include "qm/qmerrno.h"
#include "am/amudefs.h"
#include "ci/ciudefs.h"
#include "er/erudefs.h"
#include "im/imudefs.h"
#include "mm/mmudefs.h"
#include "os/swi_ossdk.h"
#include "pi/piudefs.h"
#include "qm/qmidefs.h"
#include "qmqmisvc.h"
#include "sdk/sdkudefs.h"
#include "sl/sludefs.h"
#include "su/suudefs.h"
#include "us/usudefs.h"
#include "qm/qmdcs_sdk.h"
#include <stdio.h>

/*---------------
  Defines
 ---------------*/
#define QMTICKMS 1000

/*---------------
  Local storage
 ---------------*/
/* QM task control block */
local struct qmtcb qmtaskblk;
local struct swi_ossemaphore qmtaskblkmutex;

/* A dummy event block sent from the IC package timer task service to the QM
 * task at a fixed interval.
 */
local struct qmevtblock qmtimerevt =
{
    QM_TMRTICK_EVT,
    NULL,
    NULL,
    0,
    QMIUNSUPPORTEDCLNT
};

/*************
 *
 * Name:    qmiclientmanager
 *
 * Purpose: QMI  client database
 *
 * Members: qmiclients      - QMI clients
 *          activeclient    - QMI client waiting for a response from the device
 *
 * Notes:
 *
 **************/
local struct qmiclientmanager{
    struct qmicontrolpoint qmiclients[QMI_SUPPORTED_CLNT_MAX];
    enum qmisupportedclients activeclient;
    enum qmisupportedclients activewdsclient;
}qmiclientmgr;

/* Structure used to send QMI responses from the device to the application */
local struct qmqmisvcresponse qmiresp;
/* RILSTART */
/* This variable is used in qmqmisvc.c */
struct swi_ossemaphore qmqmirspmutex;
/* RILSTOP */

/*---------------
  Functions
 ---------------*/
/*************
 *
 * Name:    qmisvcxactionidassign
 *
 * Purpose: assign the next transaction id for the given service
 *
 * Parms:   xactionid - client's last issued transaction id
 *
 * Return:  transaction id client is to use for issuing its next QMI request
 *
 * Abort:   none
 *
 * Notes:   None
 *
 **************/
local swi_uint16
qmisvcxactionidassign(
    swi_uint16 xactionid)
{
    /* handle overrun as zero is not a valid transaction id */
    if( ++xactionid == QMI_TRANSACTION_ID_INVALID )
        ++xactionid;

    return xactionid;
}

/*************
 *
 * Name:    qmixactionidvalidate
 *
 * Purpose: validate transaction id of incoming QMI response
 *
 * Parms:   xactionid - client's last issued transaction id
 *
 * Return:  transaction id client is to use for issuing its next QMI request
 *
 * Abort:   none
 *
 * Notes:   None
 *
 **************/
package swi_bool
qmixactionidvalidate(
   swi_uint16 xactionid)
{
    enum qmisupportedclients client;

    client = qmiclientmgr.activeclient;

    return  xactionid == qmiclientmgr.qmiclients[client].xactionid
            ? TRUE
            : FALSE ;
}

package const struct qmicontrolpoint *
qmiclientinfoget(
        enum qmisupportedclients client )
{
    return (const struct qmicontrolpoint *)&qmiclientmgr.qmiclients[client];
}

/*************
 *
 * Name:    qmisvc2qmiclnt_map
 *
 * Purpose: To get the QMI service client associated with a QMI service
 *
 * Parms:   svc - QMI service type
 *
 * Return:  QMI Service client
 *
 * Abort:   none
 *
 * Notes:   None
 *
 **************/
package enum qmisupportedclients
qmisvc2qmiclnt_map(
    enum eQMIService svc )
{
    switch(svc)
    {
        case eQMI_SVC_DMS:
            return QMIDMSCLNT;
            break;
        case eQMI_SVC_WDS:
            return QMIWDSCLNT;
            break;
        case eQMI_SVC_NAS:
            return QMINASCLNT;
            break;
        case eQMI_SVC_QOS:
            return QMIQOSCLNT;
            break;
        case eQMI_SVC_WMS:
            return QMIWMSCLNT;
            break;
        case eQMI_SVC_PDS:
            return QMIPDSCLNT;
            break;
        case eQMI_SVC_AUTH:
            return QMIAUTCLNT;
            break;
        case eQMI_SVC_CAT:
            return QMICATCLNT;
            break;
        case eQMI_SVC_RMS:
            return QMIRMSCLNT;
            break;
        case eQMI_SVC_OMA:
            return QMIOMACLNT;
            break;
        case eQMI_SVC_SWIOMA:
            return QMISWIOMACLNT;
            break;
        case eQMI_SVC_DCS:
            return QMIDCSCLNT;
            break;
        case eQMI_SVC_FMS:
            return QMIFMSCLNT;
            break;
        case eQMI_SVC_SAR:
            return QMISARCLNT;
            break;
        case eQMI_SVC_VOICE:
            return QMIVOICECLNT;
            break;
        case eQMI_SVC_UIM:
            return QMIUIMCLNT;
            break;
        default:
            return QMIUNSUPPORTEDCLNT;
            break;
    }
}

/*************
 *
 * Name:    qmgetcbp
 *
 * Purpose: Return a pointer to the QMI Request/Response task control block.
 *
 * Parms:   none
 *
 * Return:  pointer to task control block
 *
 * Abort:   none
 *
 * Notes:   None
 *
 **************/
package struct qmtcb *
qmgetcbp(void)
{
    return &qmtaskblk;
}

/*************
 *
 * Name:    qmrelevtbk
 *
 * Purpose: To release the resources associated with the QMI event block,
 *          passed to this task from the HIP Receive task
 *
 * Parms:   eventbkp - Pointer to the QMI Event Block structure, received
 *                     from the HIP Receive task
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   This function releases the resources associated with the
 *          QMI event block as follows: first the memory buffer containing
 *          the newly received QMI packet is released. Next the event buffer
 *          itself is released. Both are allocated from pools created at
 *          system startup time by the QM task.
 *
 **************/
local void
qmrelevtbk(
    struct qmevtblock *eventbkp )
{
    /* If the caller's memory can be freed... */
    if( eventbkp->qmevtmemfreep != NULL )
    {
        /* Free the QMI message buffer first */
        mmbufrel( eventbkp->qmevtmemfreep );
    }

    /* Free the event block itself */
    mmbufrel( (swi_uint8 *)eventbkp );
}

/*************
 *
 * Name:    qmclearrespinfo
 *
 * Purpose: To clear the information about the current outbound request.
 *
 * Parms:   none
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   Calling this function causes the qmmdmwaittmr to be stopped
 *          if it isn't already
 *
 **************/
package void
qmclearrespinfo(void)
{
    /* Get the QMI RR control block pointer */
    struct qmtcb *qmcbp = qmgetcbp();

    /* Clear out active request response status fields */
    struct qmwdata *swinfop;
    swinfop = &qmcbp->qmwdata;
    swinfop->qmbusy         = FALSE;
    swinfop->qmoperation    = eQMIINVALID;
    swinfop->qmipcchannel   = 0xFF;

    /* Stop the timer */
    qmcbp->qmmdmwaittmr = 0;

    /* Reset QMI client fields */
    enum qmisupportedclients client;
    client = qmiclientmgr.activeclient;
    qmiclientmgr.qmiclients[client].clientbusy = FALSE;
    qmiclientmgr.qmiclients[client].msgid = QMI_MSG_ID_INVALID;
    qmiclientmgr.activeclient = QMIUNSUPPORTEDCLNT;
}

/*************
 *
 * Name:    qmsaverespinfo
 *
 * Purpose: To store the information about the current outbound request
 *          so that when the response comes in we can validate that its
 *          the correct one for the stop-and-wait protocol. This function
 *          converts the outgoing operation type into the expected
 *          incoming operation response
 *
 * Parms:   pqmparms    - pointer to record containing information about the
 *                        outgoing QMI request.
 *          ipcchan     - IPC channel over which the request originated
 *
 * Return:  none
 *
 * Abort:   Stop and wait violation
 *
 * Notes:   This function starts the internal modem response timer using
 *          the timeout value specified by the caller in the API
 *
 **************/
local void
qmsaverespinfo(
    struct amqmirrparms *pqmparms,
    swi_uint8 ipcchan )
{
    struct qmtcb *qmcbp = qmgetcbp();
    struct qmwdata *swinfop = &qmcbp->qmwdata;

    /* Check for an existing stop and wait operation */
    if( swinfop->qmbusy )
    {
        /* Stop and wait already busy - Fatal Error */
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d Stop-and-wait violation",
                 (char *)__func__, __LINE__);
        erAbort( errmsg,
                 pqmparms->amqmisvctype );
    }

    /* store the active QMI client */
    enum qmisupportedclients client = qmisvc2qmiclnt_map(pqmparms->amqmisvctype);

    /* establish that we are dealing with a WDS control point */
    if( QMIWDSCLNT ==  client )
    {
        /* WDS control point can be of an IPv4 or an IPv6 type */
        if( QMIWDSV6CLNT == qmiclientmgr.activewdsclient )
        {
            client = QMIWDSV6CLNT; /* v6 */
        }
    }

    qmiclientmgr.activeclient = client;

    /* assign transaction ID */
    qmiclientmgr.qmiclients[client].xactionid =
        qmisvcxactionidassign( qmiclientmgr.qmiclients[client].xactionid );

    /* Record that we are busy servicing a QMI request and the IPC channel
     * on which the request arrived and to which the response will be sent.
     */
    swinfop->qmbusy = TRUE;
    swinfop->qmoperation = eQMIRES;
    swinfop->qmipcchannel = ipcchan;

    /* number of ticks to use for the given timeout (one tick minimum) */
    swi_uint32 ticks = pqmparms->amqmireqtimeout/QMTICKMS;
    if( 0 == ticks )
    {
        ticks = 1;
    }
    
    
/* RILSTART */ 
    /* To ensure that atleast the timeout is met, we increment the ticks by
     * 2 secs. This is to cover scenarios when the device is taking more time
     * to respond during initialization.
     */
    ticks += 2;
/* RILSTOP */ 
    

    /* Start the timer */
    qmcbp->qmmdmwaittmr = ticks;
}

/*************
 *
 * Name:    qmshortresp
 *
 * Purpose: Build and send a short response to the API side. This
 *          function is used to send a reply to a request received
 *          from the API side when the request cannot be complied
 *          with in the normal manner
 *
 * Parms:   resultcode - An AM packet result code to be stuffed into
 *                       the returned packet
 *          ipcchannel - IPC channel to send short response to.
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
package void
qmshortresp(
    enum eQCWWANError resultcode,
    swi_uint32 ipcchannel )
{

    /* Fill the QM argument structure with NULLS.
     * Set the result code to indicate the timeout occurred.
     * The reader needs to realize that none of the fields are
     * valid if it receives a timeout response.
     */
    struct amqmirrparms qmargs; /* QMI Arguments from the QMI REQUEST packet */
    slmemset( (char *)&qmargs, 0x0, sizeof( struct amqmirrparms ) );

    /* Now set the result code to indicate a timeout occurred */
    qmargs.amresultcode = resultcode;

    /* Build the response packet, empty except for the timeout indication.
     * This call modifies the address pointed to by outbufp.
     */
    swi_uint8 outbuf[AMQMIRESPTOTAL];   /* temp storage for response */
    swi_uint8 *outbufp = outbuf;        /* working pointer */
    swi_uint16 outipcsize; /* Length (bytes) of the outgoing IPC msg */
    outipcsize = ambuildqmixactionheader( &outbufp,
                                          &qmargs,
                                          AMTYPEQMIRESPONSE );

    /* Log this particular short response transaction */
    struct qmtcb *qmcbp;
    qmcbp = qmgetcbp();
    dlLog2( &qmcbp->qmdlcb, QMLOG_CLASSC,
            "qmshortresp( %d, ch: %d)",
            (swi_uint32)resultcode,
            (swi_uint32)ipcchannel );

    /* Clear the expected response information */
    qmclearrespinfo();

    /* Send response to the host */
    amsdk_send( outbuf, outipcsize, (swi_uint8) ipcchannel);
}

/*************
 *
 * Name:    qmvalidateresponse
 *
 * Purpose: Validate a QMI response
 *
 * Parms:   none
 *
 * Return:  TRUE if response valid, FALSE otherwise
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local swi_bool
qmvalidateresponse(void)
{
    struct qmwdata *swinfop;
    swi_bool validated = FALSE; /* Assume the packets don't match */

    /* Get the QMI RR control block pointer */
    struct qmtcb *qmcbp = qmgetcbp();

    /* Get a pointer to the stored expected values */
    swinfop = &qmcbp->qmwdata;

    /* If busy not set, then the rest of the fields are meaningless.
     * Likely cause is that incoming response arrived after a timeout.
     */
    if( swinfop->qmbusy )
    {
        /* Compare the incoming values with the stored ones */
        if( swinfop->qmoperation == qmiresp.ctlflgs &&
            qmixactionidvalidate(qmiresp.xactionid) )
        {
            /* The incoming packet is the one we were waiting for */
            validated = TRUE;

/* RILSTART */
            /* No longer busy so clear the transaction */
            //qmclearrespinfo();
/* RILSTOP */
        }
        /* no further action required since either a valid response will arrive,
         * or a timeout will occur. Either way, a response will be sent back
         * as a result of an issued request.
         */
    }
    return validated;
}

/*************
 *
 * Name:    qmvalidaterequest
 *
 * Purpose: Validate a QMI request
 *
 * Parms:   none
 *
 * Return:  eQCWWAN_ERR_NONE if response valid, eQCWWAN_ERR_xxx error otherwise
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local enum eQCWWANError
qmvalidaterequest(
    struct amqmirrparms *pqmparms )
{
    struct qmtcb *qmcbp = qmgetcbp();
    enum qmisupportedclients client;
    client = qmisvc2qmiclnt_map( pqmparms->amqmisvctype );

    /* if the client is not supported the request is invalid */
    if( client == QMIUNSUPPORTEDCLNT )
    {
        dlLog0( &qmtaskblk.qmdlcb,
                QMLOG_CLASSA,
                "qmvalidaterequest: Unsupported Client" );

        return eQCWWAN_ERR_SWICM_QMI_CLNT_NOT_SUPPORTED;
    }

    /* check if QMI device is available */
    if( qmcbp->qmidevicestate != DS_SIO_READY )
    {
        dlLog0( &qmtaskblk.qmdlcb,
                QMLOG_CLASSA,
                "qmvalidaterequest: qmidevicestate != DS_SIO_READY" );

        return eQCWWAN_ERR_NO_DEVICE;
    }

    /* If the application has not issued a DCS Connect request it is not
     * allowed to call any SLQS APIs with the exception of DCS APIs.
     */
    if( !qmcbp->qmappregistered )
    {
        dlLog0( &qmtaskblk.qmdlcb,
                QMLOG_CLASSA,
                "qmvalidaterequest: application not registered" );

        return eQCWWAN_ERR_SWIDCS_APP_DISCONNECTED;
    }

    return eQCWWAN_ERR_NONE;
}

/*************
 *
 * Name: qmqmirsp
 *
 * Purpose: Handle incoming QMI responses from the modem
 *
 * Parms:   qmpacketp  - Pointer to the QMI response packet
 *          client     - QMI client receiving the response
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   On return from this call it is safe to release the resources
 *          associated with the event block.
 *
 *       QMI SERVICE MESSAGE                       QMI SVC MSG HDR
 *       --------------------------------------    ------------------------
 *       | QMI SVC MSG HDR | QMI SVC MSG TLVs |    | MSG ID  | MSG LENGTH |
 *       --------------------------------------    ------------------------
 * bytes:          4               n                      2            2
 *
 *       QMI SERVICE MESSAGE RESPONSE
 *       ------------------------------------------------------------------------
 *       | QMI SVC MSG HDR | QMI RESPONSE MESSAGE RESULT TLV | QMI SVC MSG TLVs |
 *       ------------------------------------------------------------------------
 * bytes:          4               7                                   m
 *
 *       QMI RESPONSE MESSAGE RESULT TLV (not present in Indications! )
 *       ---------------------------------------------------------
 *       | TYPE  = 0x02 | LENGTH = 0x04 | QMI RESULT | QMI ERROR |
 *       ---------------------------------------------------------
 * bytes:          1               2           2           2
 *
 **************/
local void
qmqmirsp(
    swi_uint8 *qmpacketp,
    enum qmisupportedclients client )
{
    /* Get a working pointer to the QMI response Packet header */
    swi_uint8 *qmresp = qmpacketp;

    /* lock semaphore - exclusive access to QMI response structure shared by
     * QM thread and all DS notification threads
     */
    swi_ossdksemaphorelock(&qmqmirspmutex);

    /* populate QMI response structure */
    qmiresp.ctlflgs   = *qmresp++;
    qmiresp.xactionid = piget16(&qmresp);
    swi_uint8 *localp = qmresp;
    qmiresp.msgid     = piget16(&localp);
    /* NB: qmresp now points to the QMI service message ID in
     * the QMI service message header.
     */

    /* validate QMI response - true by default for notifications */
    swi_bool validated = TRUE;
    if( qmiresp.ctlflgs == eQMIRES )
    {
        validated = qmvalidateresponse();
    }

    if( qmtaskblk.qmappregistered && validated )
    {
        /* Build the AM response header */
        struct amqmirrparms qmparms;
        qmparms.amresultcode = eQCWWAN_ERR_NONE;

        /* QMI service type */
        qmparms.amqmisvctype = (swi_uint8)qmiclientmgr.qmiclients[client].svctype;

        /* timeout does not get used in response */
        qmparms.amqmireqtimeout = 0;

        /* determine AM response payload length */
        swi_uint8 *plen = qmresp + QMISVC_MSGLEN_OFFSET; /* pointer to QMI response message length */
        swi_uint16 xlen = piget16(&plen) + QMISVC_MSGHDR_SZ;

        /* truncate response if it exceeds allocated response buffer size
         * total response size = AM header +
         *                       QMUX SDU Preamble +
         *                       QMI service message (QMUX SDU)
         */
        int overrun = (xlen + AMQMIRESPTOTAL) - sizeof(qmiresp.amqmimsg);
        if(overrun > 0)
        {
           dlLog1(  &qmtaskblk.qmdlcb,
                    QMLOG_CLASSA,
                    "qmqmirsp: QMI response exceeds buffer size by %lu bytes,"\
                    " concatenating response!!!",
                    (swi_uint32)overrun );

           qmparms.amqmixactionlen = xlen - overrun;
        }
        else
        {
           qmparms.amqmixactionlen = xlen;
        }

        swi_uint8 *plocal  = (swi_uint8 *)qmiresp.amqmimsg;
        swi_uint16 outipcsize;  /* Length (bytes) of the outgoing IPC msg */

        /* build AMQM header */
        enum ammsgtypes amtype;
        amtype = qmiresp.ctlflgs == eQMIRES ?  AMTYPEQMIRESPONSE : AMTYPEQMINOTIFICATION ;
        outipcsize = ambuildqmixactionheader( &plocal,
                                              &qmparms,
                                              amtype );

        /* AM payload */
        slmemcpy( &qmiresp.amqmimsg[outipcsize],
                  qmresp,
                  qmparms.amqmixactionlen );

        /* Add the length of the AM payload */
        outipcsize += qmparms.amqmixactionlen;

        /* Determine IPC channel to send QMI message to the application */
        swi_uint16 ipcchnum;
        if( qmiresp.ctlflgs == eQMIRES )
        {   /* Request/Response Channel */
            ipcchnum = cigetchannelnum(CIIPCQMRR1);
        }
        else
        {   /* Notification Channel */
            ipcchnum = qmiclientmgr.qmiclients[client].notifchan;
        }

        dlLog( &qmtaskblk.qmdlcb, QMLOG_CLASSA,
               "SDK<-Mdm: ch/QMImsgid/QMImsglen/IPCmsglen: %d/%04x/%d/%d",
               (swi_uint32)ipcchnum,
               (swi_uint32)qmiresp.msgid,
               (swi_uint32)qmparms.amqmixactionlen,
               (swi_uint32)qmparms.amqmixactionlen + AMQMIRESPTOTAL );

/* RILSTART */
        if( qmiresp.ctlflgs == eQMIRES )
        {
            /* No longer busy so clear the transaction */
            qmclearrespinfo();
        }
/* RILSTOP */

        /* Send this packet to the host */
        amsdk_send( qmiresp.amqmimsg,
                    outipcsize,
                    ipcchnum );

        /* Unlock semaphore - exclusive access to QMI response structure shared by
         * QM execution thread and all DS notification threads
         */
        swi_ossdksemaphoreunlock(&qmqmirspmutex);

    }
    /* Didn't validate, probably because this packet arrived after
     * the timeout expired, the application had issued a disconnect request,
     * or the request was canceled. Log the stray and release the resources
     * without bothering the application
     */
    else
    {
        /* Unlock semaphore - exclusive access to QMI response structure shared by
         * QM execution thread and all DS notification threads
         */
        swi_ossdksemaphoreunlock(&qmqmirspmutex);

        /* Log a stray packet */
        dlLog2( &qmtaskblk.qmdlcb, QMLOG_CLASSA,
                "Rcv'd stale modem response: SDK<-Mdm: ch/msgid: %d/%04x",
                (swi_uint32)qmtaskblk.qmwdata.qmipcchannel,
                (swi_uint32)qmiresp.msgid );
    }
}

/*************
 *
 * Name: qmrxpkt
 *
 * Purpose: Entry point in QM package for receiving a QMI packet from the
 *          modem.
 *
 * Parms:   qmhdrp      - pointer to beginning of received message
 *          qmmsglen    - length of received message
 *          percode     - pointer to SIO error status value
 *          client      - QMI client receiving the response
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   Runs in the context of a DS shell
 *
 **************/
local void qmrxpkt(
    swi_uint8  *qmhdrp,
    swi_uint32 qmmsglen,
    swi_uint8  *memrelp,
    swi_uint16 client )
{
    /* handle based on message type */
    swi_uint8 msgtype = *qmhdrp;

    UNUSEDPARAM( qmmsglen );
    UNUSEDPARAM( memrelp );

    switch(msgtype)
    {
        case eQMIRES:
        case eQMINOT:
             qmqmirsp( qmhdrp,
                       (enum qmisupportedclients)client );
             break;
        default:
            {
                char errmsg[100];
                snprintf(errmsg, sizeof(errmsg),
                         "%s:%d should only receive QMI Responses,"\
                         "received message type",
                         (char *)__func__, __LINE__);
                erAbort( errmsg,
                         (swi_uint32)msgtype );
            }
            break;
    }
}

/*************
 *
 * Name:    qmgetsizedbuf
 *
 * Purpose: To allocate a buffer payload based on the size of the message.
 *
 * Parms:   msglen  - length of the message in number of bytes
 *
 * Return:  Pointer to allocated buffer
 *
 * Abort:   out of memory
 *
 * Notes:   There are 2 pools for data transmission with buffer sizes of
 *          QMTXBUFSIZE and QMTXSMLBUFSIZE. The msglen parameter is used to
 *          determine what size of pool buffer to allocate. If msglen is set
 *          to 0 by the caller buffer of size QMTXBUFSIZE is allocated. If
 *          msglen is non-zero then msglen is examined and the buffer of either
 *          QMTXSMLBUFSIZE or QMTXBUFSIZE is allocated.
 *
 **************/
local swi_uint8 *qmgetsizedbuf(
    swi_uint32 msglen )
{
    struct qmtcb *qmtcbp = qmgetcbp();
    swi_uint8 *bufp;

    /* Decide on which pool to use based on message length */
    if ((msglen != 0) &&
        (msglen < QMTXSMLBUFSIZE))
    {
        /* Get buffer from small pool */
        bufp = (swi_uint8 *) mmbufget( &qmtcbp->qmtxsmlpool );
    }
    else
    {
        /* Get buffer from the large pool */
        bufp = (swi_uint8 *) mmbufget( &qmtcbp->qmtxpool );
    }

    /* Should never run out of memory */
    if( bufp == NULL )
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d no memory",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, 0);
    }

    return bufp;
}

/*************
 *
 * Name:    qmiclientreset
 *
 * Purpose: To restore default settings for a managed QMI client
 *
 * Parms:   none
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local void qmiclientreset(
    enum qmisupportedclients client )
{
    struct qmicontrolpoint *pc = qmiclientmgr.qmiclients;

    if( (pc + client)->registered )
    {
        dsclose( cigetchannelnum(CIIPCUQMRR1),
                 client);
    }

    (pc + client)->registered = FALSE;
    (pc + client)->xactionid  = QMI_TRANSACTION_ID_INVALID;
    (pc + client)->clientbusy = FALSE;
    (pc + client)->msgid      = QMI_MSG_ID_INVALID;
    (pc + client)->notifchan  = cigetchannelnum(CIIPCQMNOTIF1);
}

/*************
 *
 * Name:    qmregister4qmisvc
 *
 * Purpose: To register a client for a QMI service on the device
 *
 * Parms:   client  - QMI client to register
 *
 * Return:  TRUE if successful, FALSE otherwise
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local swi_bool
qmregister4qmisvc(
    enum qmisupportedclients client )
{
    swi_uint16 chanid;
    enum eQMIService svctype;

    chanid  = cigetchannelnum(CIIPCUQMRR1);
    svctype = qmiclientmgr.qmiclients[client].svctype;

    /* register client with QMI service */
    if( dsioctl( chanid,
                 client,
                 QMI_GET_SERVICE_FILE_IOCTL,
                 &svctype,
                 sizeof(svctype) ) )
    {
        /* registration with QMI service successful */
        qmiclientmgr.qmiclients[client].registered = TRUE;
        qmiclientmgr.qmiclients[client].clientbusy = TRUE;

        struct qmtcb *qmcbp = qmgetcbp();
        dlLog2( &qmcbp->qmdlcb, QMLOG_CLASSA,
                "Launching QMI DS shell: service %d(%s)",
                (swi_uint32)svctype,
                (swi_uint32)qmiclientmgr.qmiclients[client].qmisvcname );

        /* initialize QMI DS shell parameters */
        qmcbp->qmdsshell.dsipcshellnamep = (swi_uint8 *)CIIPCUQMRR1;
        qmcbp->qmdsshell.dsipccbfcnp = qmrxpkt;
        qmcbp->qmdsshell.client = client;

        /* Activate QMI DS shell for reading QMI response */
        qmcbp->qmdsshell.dssinitcbfp = NULL;
        swi_ossdkthreadcreate(  dslaunchshell,
                                &qmtaskblk.qmdsshell);
    }
    else
    {
        struct qmtcb *qmcbp = qmgetcbp();
        dlLog1( &qmcbp->qmdlcb, QMLOG_CLASSA,
                "Unable to register for QMI service %s",
                (swi_uint32)qmiclientmgr.qmiclients[client].qmisvcname );

        /* client registration failed */
        return FALSE;
    }

    /* client registration successul */
    return TRUE;
}

package void qmqmirequestcancel(void)
{
    qmclearrespinfo();
}


package void qmwdsclientset(
    enum qmisupportedclients client )
{
    qmiclientmgr.activewdsclient = client;
}

package enum qmisupportedclients qmwdsclientget(void)
{
    return  qmiclientmgr.activewdsclient;
}

/*************
 *
 * Name:    qmsenddcsreq
 *
 * Purpose: Send a new QMI Request message to the DCS thread
 *
 * Parms:   amqmimsgp   - Pointer to the QMI Request message
 *          memfreep    - Pointer to the memory release address for the inbound
 *                        message. QM RR task will manage the release of this
 *                        packet when done processing it.
 *          ipcchannel  - The IPC channel to send the response to.
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
global void qmsenddcsreq( swi_uint8 *qmidcsmsgp, swi_uint8 ipcchannel )
{
    struct qmdcsevtblock *dcseventbkp;
    struct qmdcstcb *qmdcscbp = qmdcsgetcbp();

    /* Allocate an event block */
    dcseventbkp = (struct qmdcsevtblock *) mmbufget (&qmdcscbp->qmdcsevtpool);

    if( dcseventbkp )
    {
        /* Get the QMI RR control block pointer */
        struct qmtcb *qmcbp = qmgetcbp();

        /* The QMI information comes next */
        dcseventbkp->qmdcsevtdatap = qmidcsmsgp;

        /* Save the memory release pointer */
        dcseventbkp->qmdcsevtmemfreep = NULL;

        /* Save the IPC channel from which the request arrived */
        dcseventbkp->qmdcsevtipcchan = ipcchannel;

        /* Save the transaction ID for the request */
        dcseventbkp->xactionid = qmiclientmgr.qmiclients[QMIDCSCLNT].xactionid;

        /* Log this new request now that we know some details */
        dlLog2( &qmcbp->qmdlcb, QMLOG_CLASSC,
               "SDK->DCS: Request Sent : ipcch/xactionid: %d/%04x",
               (swi_uint32)ipcchannel,
               (swi_uint32)dcseventbkp->xactionid );

        /* Send the message on its way */
        icsmt_send( &qmdcscbp->qmdcsicmsgque,
                    (swi_uint32) dcseventbkp );
    }
    else
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d no event blocks",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, 0 );
    }
}

/*************
 *
 * Name:    qmqmireq
 *
 * Purpose: Entry point in QMI package for sending a packet to the
 *          modem. First checks to ensure Stop-and-Wait protocol is
 *          being obeyed. Saves some information and then sends the
 *          packet on its way
 *
 * Parms:   packetp - Pointer to a byte-field containing the QMI service Request
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   On return from this function, the memory containing the
 *          parameter section can be deleted, if applicable.
 *
 *          Runs in the context of the QMI Request/Response thread.
 *
 **************/
local void
qmqmireq(
    swi_uint8 *packetp,
    swi_uint8 ipcchan )
{
    /* Get the QMI RR control block pointer */
    struct qmtcb *qmcbp = qmgetcbp();

    /* Store IPC channel */
    qmcbp->qmwdata.qmipcchannel = ipcchan;

    /* Parse the contents of the QMI request packet,
     * advancing the packet pointer in the process
     */
    swi_uint8 *inbufp = packetp;
    struct amqmirrparms qmparm;
    amparseqmixactionheader( &inbufp,
                             &qmparm );

    /* validate the QMI request */
    enum qmisupportedclients client;
    client = qmisvc2qmiclnt_map(qmparm.amqmisvctype);

#ifdef QM_DBG_QMRR
    dlLog( &qmcbp->qmdlcb, QMLOG_CLASSA,
           "SDK->Mdm: request received : ipcch/svctype/xactionlen/clientnum: "\
           "%d/%04x/%d/%d",
           (swi_uint32)ipcchan,
           (swi_uint32)qmparm.amqmisvctype,
           (swi_uint32)qmparm.amqmixactionlen,
           (swi_uint32)client );
#endif
    int rc = eQCWWAN_ERR_NO_DEVICE;
    switch (qmcbp->qmidevicestate)
    {
        case DS_SIO_DISCONNECTED:
        case DS_SIO_BOOT_READY:
            /* Allow DCS & FMS handling when device is in boot & hold mode
             * or disconnected.
             */
            if( client != QMIFMSCLNT && client != QMIDCSCLNT &&
                eQCWWAN_ERR_NONE != (rc = qmvalidaterequest(&qmparm)) )
            {
/* RILSTART */
                qmsaverespinfo(&qmparm, ipcchan);
/* RILSTOP*/
                qmshortresp( rc,
                             ipcchan );
                return;
            }
            break;
        case DS_SIO_READY:
            if( client != QMIDCSCLNT &&
                eQCWWAN_ERR_NONE != (rc = qmvalidaterequest(&qmparm)) )
            {
/* RILSTART */
                qmsaverespinfo(&qmparm, ipcchan);
/* RILSTOP */
                qmshortresp( rc,
                             ipcchan );
                return;
            }
            break;
        default:
/* RILSTART */
            qmsaverespinfo(&qmparm, ipcchan);
/* RILSTOP */
            /* Invalid state */
            qmshortresp( eQCWWAN_ERR_INTERNAL,
                         ipcchan );
            break;
    }

    /* Log this new request now that we know some details */
    dlLog( &qmcbp->qmdlcb, QMLOG_CLASSA,
           "SDK->Mdm: request validated : ipcch/svctype/xactionlen/clientnum: %d/%04x/%d/%d",
           (swi_uint32)ipcchan,
           (swi_uint32)qmparm.amqmisvctype,
           (swi_uint32)qmparm.amqmixactionlen,
           (swi_uint32)client );

    /* Some QMI requests are serviced by local QMI services rather than by the
     * device.
     */
    switch(client)
    {
        case QMIDCSCLNT:
            qmsaverespinfo(&qmparm, ipcchan);
            qmsenddcsreq(inbufp, ipcchan);
            return;
            break;
        case QMIFMSCLNT:
            qmsaverespinfo(&qmparm, ipcchan);
            qm_fms_handler(inbufp);
            return;
        default:
            break;
    }

    /* invoke the QMI message parser */
    qmparser(qmparm.amqmisvctype, inbufp);

    switch( qmparm.amqmisvctype )
    {
        /* WDS - determine whether to use v4 or v6 client for QMI request */
        case eQMI_SVC_WDS:
            client = qmwdsclientget();
            break;
        default:
            break;
    }

    /* get a QMI client for the requested service */
    if( qmiclientmgr.qmiclients[client].registered == FALSE )
    {
        if( !qmregister4qmisvc(client) )
        {
            /* report failure to application */
            dlLog0( &qmtaskblk.qmdlcb,
                    QMLOG_CLASSA,
                    "qmqmireq: could not get a QMI client for requested service" );

            qmshortresp( eQCWWAN_ERR_SWICM_QMI_SVC_NOT_SUPPORTED,
                         qmcbp->qmwdata.qmipcchannel );
            return;
        }
    }

    /* Client is registered with QMI service */
    qmsaverespinfo(&qmparm, ipcchan);

    /* Allocate a buffer for the outgoing QMI request */
    swi_uint8 *sendbufp;
    sendbufp = (swi_uint8 *)qmgetsizedbuf(qmparm.amqmixactionlen);

    /* Build the outgoing QMI request */
    swi_uint8 *localp = sendbufp; /* working pointer */

    /* Control Flag */
    *localp++ = (swi_uint8)eQMIREQ;

    /* transaction id */
    piput16( qmiclientmgr.qmiclients[client].xactionid,
             &localp);

    /* Size of the packet to be sent to the modem */
    swi_uint32 packetsize;
    packetsize = localp - sendbufp;

    /* QMI service message */
    slmemcpy( localp,
              inbufp,
              qmparm.amqmixactionlen );

    /* update outgoing message packet size */
    packetsize += qmparm.amqmixactionlen;

    /* send the QMI service request */
    swi_uint16 chanid = cigetchannelnum(CIIPCUQMRR1);

    if( !dssend( sendbufp, packetsize, chanid, client ) )
    {
        qmshortresp(eQCWWAN_ERR_SWIDCS_FILEIO_ERR, (swi_uint32)cigetchannelnum(CIIPCQMRR1));
    }

    /* free memory allocated for QMI request */
    mmbufrel(sendbufp);
}

/*************
 *
 * Name:    qmsendrr
 *
 * Purpose: Send a new QMI Request message to the QM task
 *
 * Parms:   amqmimsgp     - Pointer to the QMI Request message
 *          memfreep    - Pointer to the memory release address for the inbound
 *                        message. QM RR task will manage the release of this
 *                        packet when done processing it.
 *          ipcchannel  - The IPC channel to send the response to.
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
global void
qmsendrr(
    swi_uint8 *amqmimsgp,
    swi_uint8 *memfreep,
    swi_uint8 ipcchannel )
{
    struct qmevtblock *eventbkp;
    struct qmtcb *qmcbp;

    qmcbp = qmgetcbp();

    /* Allocate an event block */
    eventbkp = (struct qmevtblock *) mmbufget( &qmcbp->qmevtpool );

    if( eventbkp )
    {
        /* Stuff the fields with the required information */
        eventbkp->qmeventtype = QM_QMIREQ_EVT;

        /* The QMI information comes next */
        eventbkp->qmevtdatap = amqmimsgp;

        /* Save the memory release pointer */
        eventbkp->qmevtmemfreep = memfreep;

        /* Save the IPC channel from which the request arrived */
        eventbkp->qmevtipcchan = ipcchannel;

        /* Send the message on its way */
        icsmt_send( &qmcbp->qmicmsgque,
                    (swi_uint32) eventbkp );
    }
    else
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d no event blocks",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, 0 );
    }
}

/*************
 *
 * Name: qmmdmtimeout
 *
 * Purpose: Called if the qmmdmtmrwait timer expires, meaning that the
 *          modem has not responded to an outstanding QMI request
 *          in the time specified by the caller of the API. Send back
 *          a timeout indication to the API side
 *
 * Parms:   ipcchannel  - IPC channel to send response to
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void
qmmdmtimeout(
    swi_uint32 ipcchannel )
{
    /* Send the timeout reply to the api side */
    qmshortresp( eQCWWAN_ERR_QMI_RSP_TO, ipcchannel);
}

/*************
 *
 * Name:    qmtick
 *
 * Purpose: Called once each time a timer tick message is received
 *          by the RR task. This function takes care of managing
 *          internal timers, decrementing them for each call. When
 *          the timer expires (reaches zero) the expiry function
 *          is called
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
local void
qmtick(void)
{
    struct qmtcb *qmcbp = qmgetcbp();

    if( qmcbp->qmmdmwaittmr && (0 == --qmcbp->qmmdmwaittmr) )
    {
        /* the timer is active and has just timed out */
        qmmdmtimeout(qmcbp->qmwdata.qmipcchannel);
    }
}

/*************
 *
 * Name:    qmderegisterclients
 *
 * Purpose: deregister all currently registered QMI clients
 *
 * Parms:   none
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local void
qmderegisterclients(void)
{
    enum qmisupportedclients c;

    for( c = 0; c < QMI_SUPPORTED_CLNT_MAX ; c++ )
    {
        if( qmiclientmgr.qmiclients[c].registered )
        {
            qmiclientreset(c);
        }
    }
}

/*************
 *
 * Name:    qm_register_startup_clients
 *
 * Purpose: Register for QMI services that send unsolicited notifications
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
local void qm_register_startup_clients()
{
    enum qmisupportedclients client;
    swi_uint8 cltcnt =
        sizeof(qmiclientmgr.qmiclients) / sizeof(qmiclientmgr.qmiclients[0]);
    struct qmicontrolpoint *pc = qmiclientmgr.qmiclients;

    while( cltcnt-- )
    {
        if( TRUE == (pc+cltcnt)->unotifications_supported &&
            FALSE == (pc+cltcnt)->registered )
        {
            /* Retrieve the client for the particular service */
            client = qmisvc2qmiclnt_map((pc+cltcnt)->svctype);

            /* Register Client */
            if( !qmregister4qmisvc(client) )
            {
                /* Log failure to startup client */
                struct qmtcb *qmcbp = qmgetcbp();
                dlLog1( &qmcbp->qmdlcb, QMLOG_CLASSA,
                        "could not initialise QMI client for %s service",
                        (swi_uint32)(pc+cltcnt)->qmisvcname );
            }
        }
    }
}

/*************
 *
 * Name:    qm_ds_handle_dev_disconnected
 *
 * Purpose: Handle DS device disconnected event notifications
 *
 * Parms:   none
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local void
qm_ds_handle_dev_disconnected()
{
    struct qmtcb *pcb = qmgetcbp();

    /* enter critical section */
    swi_ossdksemaphorelock(&qmtaskblkmutex);

    if( pcb->qmidevicestate != DS_SIO_DISCONNECTED )
    {
        /* remember that device is disconnected */
        pcb->qmidevicestate = DS_SIO_DISCONNECTED;

        /* deregister all clients */
        qmderegisterclients();

        /* if there is an outstanding QMI request, send a response */
        if( pcb->qmwdata.qmbusy )
        {
            qmshortresp(eQCWWAN_ERR_NO_DEVICE, pcb->qmwdata.qmipcchannel);
        }

        /* send device status change notification to application.
         * peer availability is handled by lower layers.
         */
        dlLog1(  &pcb->qmdlcb, QMLOG_CLASSC,
                 "qm_ds_handle_dev_disconnected: devstate %d", (int)DS_SIO_DISCONNECTED );

        qm_dcs_dev_state_change_notify((swi_uint8)DS_SIO_DISCONNECTED);

        /* exit critical section */
        swi_ossdksemaphoreunlock(&qmtaskblkmutex);
    }
    else
    {
        /* exit critical section */
        swi_ossdksemaphoreunlock(&qmtaskblkmutex);
    }
}

/* RILSTART */
swi_int8 qmGetFwDownloadStatus( void )
{
    struct qmtcb *pcb = qmgetcbp();
    return pcb->fms_data.fw_download_status;
}

void qmSetFwDownloadStatus( swi_int32 fwdldstatus )
{
    struct qmtcb *pcb = qmgetcbp();
    pcb->fms_data.fw_download_status = fwdldstatus;
}
/* RILSTOP */

/*************
 *
 * Name:    qm_ds_handle_boot_dev_ready
 *
 * Purpose: Handle DS boot device event notifications
 *
 * Parms:   none
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local void
qm_ds_handle_boot_dev_ready()
{
    struct qmtcb *pcb = qmgetcbp();

    pcb->qmidevicestate = DS_SIO_BOOT_READY;

/* RILSTART */
    if (pcb->fms_data.fw_download_requested &&
        pcb->fms_data.fw_download_started)
    {
        /* send firmware download completion notification to application */
        qm_fms_fw_dwld_complete_notify(qmGetFwDownloadStatus());

        /* clear firmware download request flag */
        pcb->fms_data.fw_download_requested = FALSE;
        pcb->fms_data.fw_download_started   = FALSE;

        /* clear firmware download path */
        slmemset( pcb->fms_data.path, 0, sizeof(pcb->fms_data.path) );
    } 
    else if (pcb->fms_data.fw_download_requested &&
             !pcb->fms_data.fw_download_started)
    {
        /* create image management task */
        imtaskinit(pcb->fms_data.path);

        pcb->fms_data.fw_download_started = TRUE;

        /* clear the firmware download path */
        slmemset( pcb->fms_data.path, 0, sizeof(pcb->fms_data.path) );
    }
/* RILSTOP */
}

/*************
 *
 * Name:    qm_ds_handle_app_dev_ready
 *
 * Purpose: Handle DS application device event notifications
 *
 * Parms:   none
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local void
qm_ds_handle_app_dev_ready()
{
    struct qmtcb *pcb = qmgetcbp();

    /* enter critical section */
    swi_ossdksemaphorelock(&qmtaskblkmutex);

    qm_register_startup_clients();

/* RILSTART */
    if( pcb->fms_data.fw_download_requested &&
        pcb->fms_data.fw_download_started )
    {
        /* send firmware download completion notification to application */
        qm_fms_fw_dwld_complete_notify(qmGetFwDownloadStatus());

        /* clear firmware download request flag */
        pcb->fms_data.fw_download_requested = FALSE;
        pcb->fms_data.fw_download_started   = FALSE;

        /* clear firmware download path */
        slmemset( pcb->fms_data.path, 0, sizeof(pcb->fms_data.path) );
    }

    else if( pcb->fms_data.fw_download_requested &&
            !pcb->fms_data.fw_download_started )
    {
        /* send firmware not downloaded notification to application */
        qm_fms_fw_dwld_complete_notify(eQCWWAN_ERR_SWIIM_FIRMWARE_NOT_DOWNLOADED);

        /* clear firmware download request flag */
        pcb->fms_data.fw_download_requested = FALSE;
        pcb->fms_data.fw_download_started   = FALSE;
/* RILSTOP */

        /* clear firmware download path */
        slmemset( pcb->fms_data.path, 0, sizeof(pcb->fms_data.path) );
    }

    /* remember that device is connected */
    swi_uint8 prevdevstate = pcb->qmidevicestate;
    pcb->qmidevicestate = DS_SIO_READY;

    /* send device status change notification to application
     * peer availability is handled by lower layers
     */
    if( prevdevstate != DS_SIO_READY )
    {
        dlLog1(  &pcb->qmdlcb, QMLOG_CLASSC,
                "qm_ds_handle_app_dev_ready: devstate %d", (int)DS_SIO_READY );

        qm_dcs_dev_state_change_notify((swi_uint8)DS_SIO_READY);
    }

    /* exit critical section */
    swi_ossdksemaphoreunlock(&qmtaskblkmutex);
}

/*************
 *
 * Name:    qm_ds_dev_event_handler
 *
 * Purpose: Handle DS application device event notifications
 *
 * Parms:   event   - DS SIO device event being reported
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local void
qm_ds_dev_event_handler(
    enum ds_sio_event event )
{
    switch(event)
    {
        case DS_SIO_BOOT_READY:
            qm_ds_handle_boot_dev_ready();
            break;

        case DS_SIO_READY:
            qm_ds_handle_app_dev_ready();
            break;

        case DS_SIO_DISCONNECTED:
            qm_ds_handle_dev_disconnected();
            break;

        default:
            break;
    }
}

/*************
 *
 * Name:    qm_ds_dev_notification_cb
 *
 * Purpose: Callback registered with Device Services to receive
 *          device notifications.
 *
 * Parms:   event   - DS event to report
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
package void
qm_ds_dev_notification_cb(
    enum ds_sio_event event )
{
    local swi_int32 lastevent = -1;

    /* filter repeat events */
    if( (swi_int32)event == lastevent )
    {
        return;
    }

    /* update the last event received */
    lastevent = event;

    /* Allocate an event block */
    struct qmtcb *qmcbp = qmgetcbp();
    struct qmevtblock *eventbkp =
                (struct qmevtblock *)mmbufget( &qmcbp->qmevtpool );

    if( eventbkp )
    {
        /* Stuff the fields with the required information */
        eventbkp->qmeventtype = QM_DS_DEVICE_EVT;

        /* The QMI information comes next */
        eventbkp->qmevtdatap = (swi_uint8*)event;

        /* zero out memory release pointer */
        eventbkp->qmevtmemfreep = NULL;

        /* invalidate IPC channel from which the request arrived */
        eventbkp->qmevtipcchan = 0xFF;

        /* Send the message on its way */
        icsmt_send( &qmcbp->qmicmsgque,
                    (swi_uint32) eventbkp );
    }
    else
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d no event blocks",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, 0 );
    }
}

/*************
 *
 * Name:    qmiclientmanagerinit
 *
 * Purpose: QMI clients initialization
 *
 * Parms:   none
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   None
 *
 **************/
local void
qmiclientmanagerinit(void)
{
    /* no active client on startup */
    qmiclientmgr.activeclient = QMIUNSUPPORTEDCLNT;

    /* default wds to v4 client */
    qmiclientmgr.activewdsclient = QMIWDSCLNT;

    /* initialize QMI clients (Control Points) */
    int i;
    for( i = 0 ; i < QMI_SUPPORTED_CLNT_MAX ; i++ )
    {
        qmiclientreset(i);
    }

    i = 0;
    struct qmicontrolpoint *pc = qmiclientmgr.qmiclients;

    /* add new clients to the end of the list as the index must match
     * the enumerated value of "enum qmisupportedclients" in qmudefs.h
     */

    /* WDS */
    (pc + i)->svctype = eQMI_SVC_WDS;
/* RILSTART */
    (pc + i)->unotifications_supported = FALSE;
/* RILSTOP */
    slstrncpy(  (pc + i++)->qmisvcname,
                QMIWDSSVCNAM,
                strlen(QMIWDSSVCNAM) );
    /* DMS */
    (pc + i)->svctype = eQMI_SVC_DMS;
    slstrncpy(  (pc + i++)->qmisvcname,
                QMIDMSSVCNAM,
                strlen(QMIDMSSVCNAM) );
    /* NAS */
    (pc + i)->svctype = eQMI_SVC_NAS;
/* RILSTART */
    (pc + i)->unotifications_supported = FALSE;
/* RILSTOP */
    slstrncpy(  (pc + i++)->qmisvcname,
                QMINASSVCNAM,
                strlen(QMINASSVCNAM) );
    /* QOS */
    (pc + i)->svctype = eQMI_SVC_QOS;
    slstrncpy(  (pc + i++)->qmisvcname,
                QMIQOSSVCNAM,
                strlen(QMIQOSSVCNAM) );
    /* WMS */
    (pc + i)->svctype = eQMI_SVC_WMS;
    slstrncpy(  (pc + i++)->qmisvcname,
                QMIWMSSVCNAM,
                strlen(QMIWMSSVCNAM) );
    /* PDS */
    (pc + i)->svctype = eQMI_SVC_PDS;
/* RILSTART */
    (pc + i)->unotifications_supported = FALSE;
/* RILSTOP */
    slstrncpy(  (pc + i++)->qmisvcname,
                QMIPDSSVCNAM,
                strlen(QMIPDSSVCNAM) );
    /* AUT */
    (pc + i)->svctype = eQMI_SVC_AUTH;
    slstrncpy(  (pc + i++)->qmisvcname,
                QMIAUTSVCNAM,
                strlen(QMIAUTSVCNAM) );
    /* UIM */
    (pc + i)->svctype = eQMI_SVC_UIM;
    slstrncpy(  (pc + i++)->qmisvcname,
                QMIUIMSVCNAM,
                strlen(QMIUIMSVCNAM) );
    /* CAT */
    (pc + i)->svctype = eQMI_SVC_CAT;
    slstrncpy(  (pc + i++)->qmisvcname,
                 QMICATSVCNAM,
                 strlen(QMICATSVCNAM) );
    /* RMS */
    (pc + i)->svctype = eQMI_SVC_RMS;
    slstrncpy(  (pc + i++)->qmisvcname,
                QMIRMSSVCNAM,
                strlen(QMIRMSSVCNAM) );
    /* OMA */
    (pc + i)->svctype = eQMI_SVC_OMA;
    slstrncpy(  (pc + i++)->qmisvcname,
                QMIOMASVCNAM,
                strlen(QMIOMASVCNAM) );
    /* PBM */
    (pc + i)->svctype = eQMI_SVC_ENUM_BEGIN;
    slstrncpy(  (pc + i++)->qmisvcname,
                QMIPBMSVCNAM,
                strlen(QMIPBMSVCNAM) );
    /* DCS */
    (pc + i)->svctype = eQMI_SVC_DCS;
    slstrncpy(  (pc + i++)->qmisvcname,
                QMIDCSSVCNAM,
                strlen(QMIDCSSVCNAM) );

    /* SWIOMA */
    (pc + i)->svctype = eQMI_SVC_SWIOMA;
    slstrncpy(  (pc + i++)->qmisvcname,
                QMISWIOMASVCNAM,
                strlen(QMISWIOMASVCNAM) );

    /* FMS */
    (pc + i)->svctype = eQMI_SVC_FMS;
    slstrncpy(  (pc + i++)->qmisvcname,
                QMIFMSSVCNAM,
                strlen(QMIFMSSVCNAM) );
    /* SAR */
    (pc + i)->svctype = eQMI_SVC_SAR;
    slstrncpy(  (pc + i++)->qmisvcname,
                QMISARSVCNAM,
                strlen(QMISARSVCNAM) );
    /* VOICE */
    (pc + i)->svctype = eQMI_SVC_VOICE;
/* RILSTART */
    (pc + i)->unotifications_supported = FALSE;
/* RILSTOP */
    slstrncpy(  (pc + i++)->qmisvcname,
                QMIVOICESVCNAM,
                strlen(QMIVOICESVCNAM) );
    /* WDSV6*/
    (pc + i)->svctype = eQMI_SVC_WDS;
    slstrncpy(  (pc + i++)->qmisvcname,
                QMIWDSSVCNAM,
                strlen(QMIWDSSVCNAM) );
}

/*************
 *
 * Name:    qmdisconnect
 *
 * Purpose: Service request to dissociate SDK from currently connected application
 *
 * Parms:   none
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
package void
qmdisconnect(void)
{
    struct qmtcb *tcbp = qmgetcbp();

    /* Dissociate SDK from Application */
    tcbp->qmappregistered = FALSE;

    /* Tear down communication with device QMI server */
    qmderegisterclients();

    /* default wds to v4 client */
    qmiclientmgr.activewdsclient = QMIWDSCLNT;
}

/*************
 *
 * Name:    qmtask
 *
 * Purpose: The QMI Request/Response task. This task coordinates the forwarding
 *          of messages from a client (i.e user application) to the device and
 *          from the device back to the client.
 *
 * Parms:   paramp - pointer to data that may be passed in when this task
 *                   is started.
 *
 * Return:  never returns
 *
 * Abort:   none
 *
 * Notes:   As the QMI protocol is "stop and wait", this task blocks in two places,
 *          first where it waits for an IPC message from the application and second,
 *          awaiting a response from the device. A timeout ensures this task doesn't
 *          wait for the modem's response forever.
 *
 **************/
local void
qmtask(
    void *paramp )
{
    swi_bool releaseevt;    /* Some event blocks should not be released */
    swi_uint32 rmsg;        /* ptr to incoming QMI response from modem */
    struct qmevtblock *eventbkp;

    UNUSEDPARAM( paramp );

    /* Define and allocate the task control block */
    struct qmtcb *qmcbp = qmgetcbp();

    /* Prepare for logging */
    dlregister( "QM", &qmcbp->qmdlcb, TRUE );

    /* Initialize the SMT Message queue structure */
    icsmt_create( &qmcbp->qmicmsgque, "QMMESSAGEQ", QMMSGDEPTH );

    /* Create the Event Pool for receiving QMI requests and notifications  */
    mmpcreate( &qmcbp->qmevtpool,
               "QMEVTPOOL",
               QMEVTPOOLSZ,
               sizeof(struct qmevtblock) );

    dlLog0( &qmcbp->qmdlcb, QMLOG_CLASSC, "Task started\n" );

    /* Register with the IC package to receive periodic timer ticks */
    ictttickreg( &qmcbp->qmicmsgque,
                 (swi_uint32)&qmtimerevt,
                 &qmcbp->qmictthndl,
                 QMTICKMS );

    /* register with DS for device notifications */
    ds_register_device_notification_callback(qm_ds_dev_notification_cb);

    /* report that task has been created and is now running */
    sutaskinitcb(QMSDKPKG);

    /* Main line of processing begins next */
    for(;;)
    {
        /* Reset the release event block flag */
        releaseevt = TRUE;

        /* Wait for an incoming IPC message then dispatch it.
         * Timeout value is NULL to wait forever. We get our
         * ticks from the IC timer task.
         */
        icsmt_rcv( &qmcbp->qmicmsgque,
                   QMNOTIMEOUT,
                   &rmsg );

        if(rmsg)
        {
            /* Cast the data to an event block pointer */
            eventbkp = (struct qmevtblock *)rmsg;

            /* Dispatch to the handler for this type of message */
            switch( eventbkp->qmeventtype )
            {
                /* API-side to SDK-side QMI Request */
                case QM_QMIREQ_EVT:
                    qmqmireq( eventbkp->qmevtdatap,
                              eventbkp->qmevtipcchan );
                    break;

                /* Modem's response to QMI Request */
                case QM_QMIRSP_EVT:
                    qmqmirsp( eventbkp->qmevtdatap,
                              eventbkp->client );
                    break;

                /* Periodic timer tick message */
                case QM_TMRTICK_EVT:
                    qmtick();
                    releaseevt = FALSE;
                    break;

                /* Device notification event */
                case QM_DS_DEVICE_EVT:
                     qm_ds_dev_event_handler(
                            (enum ds_sio_event)(eventbkp->qmevtdatap) );
                    break;

                default:
                    {
                        char errmsg[100];
                        snprintf(errmsg, sizeof(errmsg),
                                 "%s:%d Invalid Event message",
                                 (char *)__func__, __LINE__);
                        erAbort(errmsg, rmsg);
                    }
                    break;
            }

            /* Message has been handled, release the associated resources */
            if( releaseevt )
                qmrelevtbk( eventbkp );
        }
        /* Do nothing if the message queue is empty or on timeout */
    }
}

/*************
 *
 * Name:    qmtaskinit
 *
 * Purpose: Creates the task by making it known to the os
 *          package.
 *
 * Parms:   none
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   qminit() must have been called
 *
 **************/
global void
qmtaskinit( void )
{
    /* Create the QM task */
    swi_ossdkthreadcreate(  qmtask,
                            NULL );
}

/*************
 *
 * Name:    qminit
 *
 * Purpose: Package initialization routine
 *
 * Parms:   none
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   None
 *
 **************/
global void qminit(void)
{

    /* define and allocate the task control block */
    struct qmtcb *qmcbp = qmgetcbp();

    /* ensure the modem wait timer is disabled initially */
    qmcbp->qmmdmwaittmr = 0;

    /* initial qmi device state */
    qmcbp->qmidevicestate = DS_SIO_DISCONNECTED;

    /* no application is registered with the SDK */
    qmcbp->qmappregistered = FALSE;

    /* initialize firmware download data member */
    slmemset( qmcbp->fms_data.path, 0, sizeof(qmcbp->fms_data.path) );
    qmcbp->fms_data.fw_download_requested = FALSE;
    qmcbp->fms_data.fw_download_started   = FALSE;
    qmSetFwDownloadStatus (eQCWWAN_ERR_SWIIM_CORRUPTED_FW_IMAGE);

    /* initialize mutexes */
    swi_ossdksemaphoreinit(&qmtaskblkmutex);
    swi_ossdksemaphoreinit(&qmqmirspmutex);

    /* initialize local QMI services */
    qmqmisvcinit();

    /* initialize QMI clients */
    qmiclientmanagerinit();

    /* create a memory pool for transmit buffer allocation */
    mmpcreate( &qmcbp->qmtxpool,
               "qmtxpool",
               QMTXPOOLSZ,
               QMTXBUFSIZE );

    /* create a memory pool for transmit small buffer allocation */
    mmpcreate( &qmcbp->qmtxsmlpool,
               "qmtxsmlpool",
               QMTXSMALLPLSZ,
               QMTXSMLBUFSIZE );
}
