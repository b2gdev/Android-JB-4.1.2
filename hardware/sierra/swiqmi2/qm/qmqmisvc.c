/*************
 *
 * Filename: qmqmisvc.c
 *
 * Purpose:  Local (SLQS) QMI service support
 *
 * Copyright: © 2011-12 Sierra Wireless Inc., all rights reserved
 *
 **************/

/*---------------
  include files
 ---------------*/
#include "aa/aaglobal.h"
#include "qm/qmerrno.h"
#include "am/amudefs.h"
#include "ci/ciudefs.h"
#include "dl/dludefs.h"
#include "qm/qmidefs.h"
#include "qmqmisvc.h"
#include "qmidefs.h"
#include "qm/qmdcs_sdk.h"
#include <stdio.h>

/*---------------
  local storage
 ---------------*/
/* RILSTART */
/* This variable is defined in qmtasksdk.c */
extern struct swi_ossemaphore qmqmirspmutex;
/* RILSTOP */

extern swi_bool qmixactionidvalidate (swi_uint16 xactionid);

/*---------------
  Functions
 ---------------*/

/*************
 *
 * Name:    qmdcsvalidateresponse
 *
 * Purpose: Validate a QMI DCS response
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
local swi_bool qmdcsvalidateresponse(struct qmqmisvcresponse *pqmirsp)
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
        if( swinfop->qmoperation == pqmirsp->ctlflgs &&
            qmixactionidvalidate(pqmirsp->xactionid) )
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
 * Name:    qm_result_code_tlv_pack
 *
 * Purpose: Pack result code TLV Value
 *
 * Parms:   (IN)    pdest - destination buffer
 *          (OUT)   psrc  - source data
 *
 * Return:  eQCWWAN_ERR_NONE, on success
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
global enum eQCWWANError
qm_result_code_tlv_pack(
    swi_uint8 *pdest,
    swi_uint8 *psrc )
{
    enum eQCWWANError rc;
    struct qm_qmi_response_tlvs_values *pin;

    pin = (struct qm_qmi_response_tlvs_values *)psrc;

    rc = PutWord(pdest, pin->qmiresult.result);
    if( rc == eQCWWAN_ERR_NONE )
    {
        rc = PutWord(pdest, pin->qmiresult.error);
    }

    return rc;
}

/*************
 *
 * Name:    qm_qmisvc_send_response
 *
 * Purpose: Construct and send a QMI response to the application
 *
 * Parms:   (IN/OUT)    pqmirsp  - QMI service response structure
 *          (IN)        prsptlvs - QMI response tlvs
 *          (OUT)       pmap     - TLV map for building QMI resonse
 *          svc                 - QMI service type
 *          msgid               - QMI message ID
 *          msgtype             - QMI message type (response or notification)
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
package void
qm_qmisvc_send_response(
    struct qmqmisvcresponse             *pqmirsp,
    struct qm_qmi_response_tlvs_values  *prsptlvs,
    struct qmTlvBuilderItem             *pmap,
    enum eQMIService                    svc,
    swi_int8                            msgid,
    swi_uint8                           msgtype )
{
    struct amqmirrparms qmparms;
    qmparms.amresultcode = eQCWWAN_ERR_NONE;

    /* enter critical section */
    swi_ossdksemaphorelock(&qmqmirspmutex);

    /* populate QMI response structure */
    pqmirsp->ctlflgs = msgtype;
    pqmirsp->msgid   = msgid;

    /* validate DCS QMI response - true by default for notifications */
    swi_bool validated = TRUE;
    if( (svc == eQMI_SVC_DCS) && (pqmirsp->ctlflgs == eQMIRES) )
    {
        validated = qmdcsvalidateresponse(pqmirsp);
    }

    if( validated )
    {
        /* construct QMI message */
        enum eQCWWANError rc;
        swi_uint16 qmimsglen;
        rc = qmbuild( pqmirsp->amqmimsg + AMQMIRESPTOTAL,
                      (swi_uint8 *)prsptlvs,
                      pmap,
                      msgid,
                      &qmimsglen );

        qmimsglen += QMISVC_MSGHDR_SZ;

        if( rc != eQCWWAN_ERR_NONE )
        {
            /* exit critical section */
            swi_ossdksemaphoreunlock(&qmqmirspmutex);

            /* record result and error for debugging */
            prsptlvs->qmiresult.result = QMI_RC_FAILURE;
            prsptlvs->qmiresult.error  = rc;

            char errmsg[100];
            snprintf(errmsg, sizeof(errmsg),
                     "%s:%d construct QMI message failed",
                     (char *)__func__, __LINE__);
            erAbort(errmsg, rc);
        }

        /* QMI service type */
        qmparms.amqmisvctype = (swi_uint8)svc;

        /* QMI transaction length */
        qmparms.amqmixactionlen = qmimsglen;

        /* timeout does not get used in response */
        qmparms.amqmireqtimeout = 0;

        swi_uint8 *plocal  = (swi_uint8 *)pqmirsp->amqmimsg;
        swi_uint16 outipcsize;  /* Length (bytes) of the outgoing IPC msg */

        /* Build AMQM transaction header */
        enum ammsgtypes amtype;
        amtype = msgtype == eQMIRES
                 ? AMTYPEQMIRESPONSE
                 : AMTYPEQMINOTIFICATION ;

        outipcsize = ambuildqmixactionheader( &plocal,
                                              &qmparms,
                                              amtype );

        /* Add the length of the AM payload */
        outipcsize += qmimsglen;

        /* Determine IPC channel to send QMI message to the application */
        swi_uint16 ipcchnum;
        if( msgtype == eQMIRES )
        {   /* Request/Response Channel */
            ipcchnum = cigetchannelnum(CIIPCQMRR1);
        }
        else
        {   /* Notification Channel */
            const struct qmicontrolpoint *pclient =
                qmiclientinfoget( qmisvc2qmiclnt_map(svc) );

            ipcchnum = pclient->notifchan;
        }

        struct qmdcstcb *tcbp = qmdcsgetcbp();
        dlLog( &tcbp->qmdcsdlcb, QMLOG_CLASSA,
               "SDK<-DCS: ch/QMImsgid/QMImsglen/IPCmsglen: %d/%04x/%d/%d",
               (swi_uint32)ipcchnum,
               (swi_uint32)pqmirsp->msgid,
               (swi_uint32)pqmirsp->qmimsglen,
               (swi_uint32)outipcsize );

/* RILSTART */
        if( msgtype == eQMIRES )
        {    /* No longer busy so clear the transaction */
            qmclearrespinfo();
        }

        /* Send this packet to the host */
        amsdk_send( pqmirsp->amqmimsg,
                    outipcsize,
                    ipcchnum );
/* RILSTOP */

        /* exit critical section */
        swi_ossdksemaphoreunlock(&qmqmirspmutex);
    }
    /* Didn't validate, probably because this packet arrived after
     * the timeout expired, the application had issued a disconnect request,
     * or the request was canceled. Log the stray and release the resources
     * without bothering the application
     */
    else
    {
        struct qmdcstcb *tcbp = qmdcsgetcbp();
        /* Unlock semaphore - exclusive access to QMI response structure shared by
         * QM execution thread and all DS notification threads
         */
        swi_ossdksemaphoreunlock(&qmqmirspmutex);

        /* Log a stray packet */
        dlLog2( &tcbp->qmdcsdlcb, QMLOG_CLASSA,
                "Stale SDK response (DCS/FMS): Device<-SDK: Svc/msgid: %d/%04x",
                (swi_uint32)svc,
                (swi_uint32)msgid );
    }

}

/*************
 *
 * Name:    qmqmisvcinit
 *
 * Purpose: Initialize local QMI service
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
package void qmqmisvcinit()
{
/* RILSTART */
    //swi_ossdksemaphoreinit(&qmqmirspmutex);
/* RILSTOP */
    qmfmsinit();
    /* Create the DCS task */
    swi_ossdkthreadcreate(  qmdcsinit, NULL );
}
