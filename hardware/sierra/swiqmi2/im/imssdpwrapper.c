/*************
 *
 * Filename:    imssdpwrapper.c
 *
 * Purpose:     This file contains function common to Image Management task for
 *              for GOBI and MC7750.
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/

/*-------------
  include files
 --------------*/
#include "sm/smudefs.h"
#include "ci/ciudefs.h"
#include "sl/sludefs.h"
#include "hd/hdudefs.h"
#include "pi/piudefs.h"

#include "imssdp.h"
#include "imcommon.h"
#include "imssdp_hello.h"
#include "imssdp_unframed_write.h"
#include "imidefs.h"
#include "imudefs.h"
#include <stdio.h>

/*************
 *
 * Name:    imdssend
 *
 * Purpose: Send SDP message to the modem
 *
 * Parms:   (IN)txbufp   - pointer to buffer containing SDP message
 *          pktsize      - length of SDP message
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
void imdssend (
    swi_uint8 *txbufp,
    swi_uint32 pktsize)
{
    swi_uint16 chanid;
    swi_uint32 vid,pid;

    if( usvidpid(&vid, &pid) )
    {
        switch(pid)
        {
            case SWI_QMIPID_BOOT:
                chanid = cigetchannelnum( CIIPCUIMRR1 );
                break;

            case GOBI3KGENERIC_BOOT0: /* Gobi devices */
            case GOBI3KGENERIC_BOOT1:
            case GOBI3KGENERIC_BOOT2:
            case GOBI3KGENERIC_BOOT3:
            case GOBI3KHP_BOOT:
                chanid = cigetchannelnum( CIIPCUIMRR2 );
                break;

            default:
                {
                    char errmsg[100];
                    snprintf(errmsg, sizeof(errmsg),
                             "%s:%d Invalid Product ID",
                             (char *)__func__, __LINE__);
                    erAbort(errmsg, (vid << 16) | pid );
                }
        }
    }

    dssend( txbufp,
            pktsize,
            chanid,
            0 );
    return;
}

/*************
 *
 * Name:    imsendframed
 *
 * Purpose: Send Framed SDP message to the modem
 *
 * Parms:   (IN)txbufp   - pointer to buffer containing SSDP message
 *          pktsize      - length of SSDP message
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   Caller is responsible for obtaining and releasing buffer
 *
 **************/
void imsendframed (
    swi_uint8 *txbufp,
    swi_uint32 pktsize)
{
    swi_uint16 fcs;

    /* FCS is calculated before framing */
    fcs = imCalculate16CRC( txbufp, 8*pktsize );

    /* Perform async HDLC encapsulation and provide a send function */
    hdTxPacket( txbufp,
                pktsize,
                NULL,
                NULL,
                imdssend,
                fcs );
}

/*************
 *
 * Name:    imbuild_hello_req
 *
 * Purpose: builds the hello request command
 *
 * Parms:   psHelloReq - pointer to Hello storage
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
void imbuild_hello_req (
    ssdp_hello_req_s *psHelloReq )
{
	/* Build Hello request */
    psHelloReq->command = SSDP_HELLO_REQ_PKT_COMMAND;
    slmemset ( (char *)&psHelloReq->magic_num[0],
               0,
               sizeof (psHelloReq->magic_num) );

    slmemcpy ( (char *)&psHelloReq->magic_num[0],
               SSDP_HELLO_REQ_PKT_MAGIC_NUMBER,
               strlen (SSDP_HELLO_REQ_PKT_MAGIC_NUMBER));

    psHelloReq->version_num        = SSDP_HELLO_REQ_PKT_VERSION_NUMBER;
    psHelloReq->compatible_version = SSDP_HELLO_REQ_PKT_COMPATIBLE_VERSION;
    /* Doc recommends 0x30, so OR-ing the following */
    psHelloReq->featurebits = SSDP_HELLO_PKT_UNFRAMED_STREAMING_DATA_FEATURE_BIT |
                      SSDP_HELLO_PKT_QDL_HIGH_SPEED_FEATURE_BIT;
    return;
}

/*************
 *
 * Name:    imbuild_unframed_write_req
 *
 * Purpose: builds the Unframed write request command
 *
 * Parms:   (IN/OUT)psUnframedWriteReq - pointer to storage
 *          sequenceNumber             - sequence number of the packet
 *          blockSize                  - payload length
 *          pImageBlock                - pointer to the data
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
void imbuild_unframed_write_req (
    ssdp_unframed_write_req_s *psUnframedWriteReq,
    USHORT  sequenceNumber,
    ULONG   blockSize,
    BYTE    *pImageBlock )
{
    psUnframedWriteReq->command         = SSDP_UNFRAMED_WRITE_COMMAND;
    psUnframedWriteReq->sequence_number = sequenceNumber;
    psUnframedWriteReq->address         = 0x00000000;
    psUnframedWriteReq->length          = blockSize; /* payload len */
    psUnframedWriteReq->payload         = (swi_char *)pImageBlock;

    return;
}

/*************
 *
 * Name:    imhandleunframed
 *
 * Purpose: Remove HDLC framing
 *
 * Parms:   (IN)memrelp   - pointer to memory to be freed (not necessarily the
 *                          same as rxbufp
 *          (IN)rxbufp    - pointer to buffer containing SSDP message
 *          pktsize       - length of SSDP message
 *          errorflag     - errors flagged in HD package
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   This is called from HD package after it finishes unframing
 *          the received buffer
 *
 **************/
package void imhandleunframed (
    swi_uint8  *memrelp,
    swi_uint8  *rxbufp,
    swi_uint16 pktsize,
    swi_bool   errorflag )
{
    struct imtcb *imtcbp = imgetcbp(); /* Get the task control block ptr */
    swi_uint8    *fcsp = rxbufp+pktsize-2; /* Offset into FCS */
    swi_uint16   localfcs, fcs;

    UNUSEDPARAM( errorflag );

    /* Figure out our FCS */
    localfcs = imCalculate16CRC( rxbufp, 8*(pktsize-2) );
    /* get the provided FCS from the end of the incoming message */
    fcs = piget16 ( &fcsp );

    if (localfcs != fcs)
    {
        /* handle error */
        dlLog2( &imtcbp->imdlcb,
                IMLOG_CLASSC,
                "imhandleunframed FCS error: ours:0x%04x, theirs:0x%04x\n",
                (swi_uint32)localfcs,
                (swi_uint32)fcs );
    }

    /* store pointer to unframed data */
    imtcbp->imsmp->udata  = rxbufp;

    /* Need to release buffer passed back from hdRxPacket */
#if 0
    imtcbp->imhdmemrelp = memrelp;
#endif
    /* Need to investigate why there is a need to release the HD buffer here
     * in order not to exhaust HD buffer pool which can happen when downloading
     * a large number of images to the device within the lifetime of a single
     * SDK process. Ideally, the buffer should be released when the processing
     * of the SSDP packet is complete. Note that since "releasing" the buffer
     * doesn't actually free the memory but rather marks it as available.
     * Luckily,we are in the boot loader so chances are no other client will request
     * that memory and therefore, it can still be processed. This, however,
     * must be addressed in the future such that the memory is freed only once
     * it is no longer being processed.
     */
    mmbufrel(memrelp);
}

/*************
 *
 * Name:    imunframe
 *
 * Purpose: Remove HDLC framing
 *
 * Parms:   (IN)rxbufp   - pointer to buffer containing SDP message
 *          pktsize      - length of SDP message
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   Caller is responsible for obtaining and releasing buffer
 *
 **************/
package void imunframe (
    swi_uint8 *rxbufp,
    swi_uint32 pktsize )
{
    /* Remove framing */
    hdRxPacket( rxbufp,
                pktsize,
                imhandleunframed );
}
