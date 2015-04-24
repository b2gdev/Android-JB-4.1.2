/*************
 *
 * Filename: amsdk.c
 *
 * Purpose:  Contains routines related to handling SDK packets defined by the
 *           AM package.
 *
 * NOTE:     The functions within this file handle SDK transactions
 *           that are typically not related to QMI services and device
 *           connectivity. For example, these could be shutting down the SDK,
 *           getting SDK version string, getting SDK process ID, etc.
 *
 * Copyright: © 2012 Sierra Wireless Inc., all rights reserved
 *
 **************/

/* include files */
#include "qm/qmudefs.h"
#include "qm/qmerrno.h"
#include "qm/qmqmisvc.h"
#include "qm/qmdcs.h"
#include "pi/piudefs.h"
#include "ci/ciudefs.h"
#include "am/amidefs.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>


/* Local defines */

/* Local storage */
local swi_uint8  respbuf[256];      /* local buffer for sending response */

/*************
 *
 * Name:    am_sdk_terminated_pack_tlv
 *
 * Purpose: Pack sdk terminated notification TLV Value
 *
 * Parms:   (OUT)    pdest   - destination buffer
 *          (IN)     psrc    - source data
 *
 * Return:  success: eQCWWAN_ERR_NONE
 *          failure: eQCWWAN_ERR_MEMORY
 *
 * Abort:   none
 *
 **************/
local enum eQCWWANError
am_sdk_terminated_pack_tlv(
    swi_uint8 *pdest,
    swi_uint8 *psrc )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_dcs_sdk_terminated_tlv_values *pval;

    pin  = (struct qm_qmi_response_tlvs_values *)psrc;
    pval = (struct qm_dcs_sdk_terminated_tlv_values *)
                            &pin->tlvvalues.qmdcstlvs.sdktermination;

    return PutStream( pdest,
                      pval->reason,
                      (swi_uint32)strlen((const char *)pval->reason));
}

/*************
 *
 * Name:    am_sdk_terminated_notify
 *
 * Purpose: Sends SDK kill notification to the API side
 *
 * Parms:   (IN)    sdkstate    - SDK state
 *
 * Return:  none
 *
 * Abort:   none
 *
 **************/
global void am_sdk_terminated_notify(
    swi_uint8 *psReason )
{
    struct qm_qmi_response_tlvs_values rsptlvs;

    struct qmTlvBuilderItem map[] =
    {
        {   eTLV_DCS_IND_SDK_TERMINATED, am_sdk_terminated_pack_tlv },
        {   eTLV_TYPE_INVALID, NULL }
    };

    slmemset ( (char*)&rsptlvs.tlvvalues.qmdcstlvs.sdktermination.reason[0],
               EOS,
               sizeof (rsptlvs.tlvvalues.qmdcstlvs.sdktermination.reason));

    slmemcpy (rsptlvs.tlvvalues.qmdcstlvs.sdktermination.reason,
              psReason,
              strlen ((const char *)psReason));

    qm_dcs_event_notification_send( map, &rsptlvs );
}

/* Packet Building Routines */

/*************
 *
 * Name:    ambuildsdkresponse
 *
 * Purpose: Build an sdk response packet.
 *
 * Parms:   amsgpp         - Pointer to pointer to storage into which the
 *                           message will be written
 *
 * Return:  Number of bytes occupied by the AM packet
 *
 * Abort:   none
 *
 * Notes:   This function builds the SDK response packets, including the
 *          common AM header
 *
 *          Response packets travel from SDK side to the API-side
 *
 **************/
package swi_uint16 ambuildsdkresponse(
    swi_uint8   **ampacketpp,
    struct      amsdkrrparms *amsdkp,
    enum        ammsgtypes   amtype )
{
    swi_uint8  *packetp;        /* Local copy of incoming packet pointer */
    swi_uint16 byteswritten;    /* After parsing, contains #bytes just read */
    swi_uint16 ampacketlength;  /* For the Common part of the AM header */

    /* Duplicate the pointer into our scratch location */
    packetp = *ampacketpp;

    switch (amsdkp->amsdkrrtype)
    {
        case AMSDKRESPVERSION:
            ampacketlength = AMSDKVERSIONRESPTOTAL;
            break;
        case AMSDKRESPPID:
            ampacketlength = AMSDKPIDRESPTOTAL;
            break;
        case AMSDKRESPKILL:
            ampacketlength = AMKILLSDKRESPTOTAL;
            break;
       default:
           {
               char errmsg[100];
               snprintf(errmsg, sizeof(errmsg),
                         "%s:%d Invalid SDK request type",
                         (char *)__func__, __LINE__);
                erAbort(errmsg, amsdkp->amsdkrrtype );
           }
            break;
    }

    /* Build AM packet header */
    ambuildamheader(
        amtype,
        AMHDRVERSION,
        0,              // temp - result code
        ampacketlength,
        &packetp);

    /* Proceed with field parsing */
    *packetp++ = 1;

    /* Update the bytes read variable here */
    byteswritten = packetp - *ampacketpp;

    /* Ensure the response size matches the constant */
    if( byteswritten != ampacketlength )
    {
        /* Combine both sizes into one variable and abort */
        byteswritten = byteswritten << 16 | ampacketlength;
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d Update AMSDK***RESPTOTAL", (char *)__func__, __LINE__);
        erAbort(errmsg, byteswritten );
    }
    /* advance the caller's packet pointer to the
     * start of where the errorstring might be.
     */
    *ampacketpp += byteswritten;

    return byteswritten;
}

/* Parsing Routines */


/*************
 *
 * Name:    amkillsdk
 *
 * Purpose: Handles SDK kill request.
 *
 * Parms:   None
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local void amkillsdk()
{
    struct amrrparms respparms;   /* For collecting up the caller's args */
    swi_uint8 *respbufp = &respbuf[0];

    /* Fill in the AM structure with the required values for this request */
    respparms.amparmtype = SWI_PARM_SDK;
    respparms.amtimeout = 2;
    respparms.amparm.amsdk.amsdkrrtype = (swi_uint8)AMSDKRESPKILL;
    respparms.amresultcode = 0; /* always */
    respparms.amparm.amsdk.amsdkrrtype = AMSDKRESPKILL;
    respparms.amparm.amsdk.amsdkreqtimeout = 2;
    respparms.amparm.amsdk.amsdkxactionlen = sizeof (struct amrrparms);

    /* Build and Send response to the host */
    swi_uint16 byteswritten =  ambuildsdkresponse ( &respbufp,
                                                    &respparms.amparm.amsdk,
                                                    AMTYPESDKRESPONSE );

    swi_uint16 ipcchnum;
    ipcchnum = cigetchannelnum(CIIPCQMRR1);

    amsdk_send( respbufp,
                byteswritten,
                ipcchnum );

    /* Send notification to the host */
    am_sdk_terminated_notify ((swi_uint8*)"SDK Terminated by user");

    /* Quit the SDK */
/* RILSTART */
#if 0
SWI_TBD:
Not sure if sending SIGQUIT to the process will actually work, since it
did not work in previous testing, so instead using exit(0) which is what
has been used in the past, and is known to work.

    kill (getpid(), SIGQUIT);
#else
    exit(0);
#endif
/* RILSTOP */
}

/* RILSTART */
/*************
 *
 * Name:    amsdkcheckandhandlekill
 *
 * Purpose: Checks for and handles SDK kill requests
 *
 * Parms:   None
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes: Does not return if there is an SDK kill request
 *
 **************/
package void amsdkcheckandhandlekill( swi_uint8 *inipcmsgp )
{
    swi_uint8 *inbufp = inipcmsgp;
    struct amsdkrrparms sdkparm;

    amparsesdkheader( &inbufp,
                      &sdkparm );

    /* Check for and handle kill request */
    if (sdkparm.amsdkrrtype == AMSDKREQKILL)
    {
        /* SDK will be gone after this */
        amkillsdk ();
    }
}
/* RILSTOP */

/*************
 *
 * Name:    amsdkhandler
 *
 * Purpose: Handles SDK requests
 *
 * Parms:   None
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
package void amsdkhandler(
    swi_uint8 *inipcmsgp,
    swi_uint8 *memfreep,
    swi_uint8 ipcchannel )
{
    UNUSEDPARAM ( memfreep );
    UNUSEDPARAM ( ipcchannel );

    swi_uint8 *inbufp = inipcmsgp;
    struct amsdkrrparms sdkparm;

    amparsesdkheader( &inbufp,
                      &sdkparm );

    /* Dispatch to the proper request handler */
    switch (sdkparm.amsdkrrtype)
    {
        case AMSDKREQVERSION:
        case AMSDKREQPID:
            /* coming soon */
            break;
        case AMSDKREQKILL:
            /* SDK will be gone after this */
            amkillsdk ();
            break;
        default:
            {
                char errmsg[100];
                snprintf(errmsg, sizeof(errmsg),
                         "%s:%d invalid request type", (char *)__func__, __LINE__);
                erAbort(errmsg, sdkparm.amsdkrrtype);
            }
            break;
    }
}
