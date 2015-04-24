/*************
 *
 * Filename: imtask_sdk.c
 *
 * Purpose:  IM task
 *
 * Copyright: © 2011-2012 Sierra Wireless Inc., all rights reserved
 *
 **************/

/*-------------
  include files
 --------------*/
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>

#include "qa/inc/SwiDataTypes.h"

#include "sm/smudefs.h"
#include "os/swi_ossdk.h"
#include "ci/ciudefs.h"
#include "sl/sludefs.h"
#include "hd/hdudefs.h"
#include "pi/piudefs.h"
#include "mm/mmudefs.h"

#include "imssdp.h"
#include "imcommon.h"
#include "imssdp_dload_to_sdp.h"
#include "imssdp_hello.h"
#include "imssdp_open_unframed.h"
#include "imssdp_unframed_write.h"
#include "imssdp_done.h"
#include "imssdp_donefinal.h"
#include "imapi.h"
#include "imudefs.h"
#include "imidefs.h"
/* RILSTART */
#include "qm/qmiproto.h"
/* RILSTOP */

/*-------
  Defines
 --------*/

/* Forward Declarations */
local void imtaskshutdown( void *imrxdatap );

/* Image Management State machines */
local sm_t imst;

/*-------------
  Local storage
 --------------*/

/* IM task control block */
local struct imtcb imtaskblk;
local swi_uint8    *imtaskevtmemfreep = NULL;

/*************
 *
 * Name:    imgetcbp
 *
 * Purpose: Return a pointer to the Image Management task control block.
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
package struct imtcb *
imgetcbp(void)
{
    return &imtaskblk;
}

/*************
 *
 * Name:    imrelevtbk
 *
 * Purpose: To release the resources associated with the IM event block
 *
 * Parms:   eventbkp - Pointer to the IM Event Block structure,
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   This function releases the resources associated with the
 *          IM event block as follows: first the memory buffer containing
 *          the newly received IM packet is released. Next the event buffer
 *          itself is released.
 *
 **************/
local void
imrelevtbk(
    struct imevtblock *eventbkp )
{
    /* If the caller's memory can be freed... */
    if( eventbkp->imevtmemfreep != NULL )
    {
        /* Free the IM message buffer first */
        mmbufrel( eventbkp->imevtmemfreep );
    }

    /* Free the event block itself */
    mmbufrel( (swi_uint8 *)eventbkp );
}


/*************
 *
 * Name:    imqueuevt
 *
 * Purpose: Send a new IM Request message to the IM task
 *
 * Parms:   qevt        - event type
 *          imimsgp     - Pointer to the IM Request message
 *          memfreep    - Pointer to the memory release address for the inbound
 *                        message. IM task will manage the release of this
 *                        packet when done processing it.
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
global void
imqueuevt(
    enum imeventtype qevt,
    swi_uint8 *immsgp,
    swi_uint32 immsglen,
    swi_uint8 *memfreep
)
{
    struct imevtblock *eventbkp;
    struct imtcb      *imtcbp = imgetcbp(); /* Get the task control block ptr */

    UNUSEDPARAM( memfreep );

    /* Allocate an event block */
    eventbkp = (struct imevtblock *) mmbufget( &imtcbp->imevtpool );

    if( eventbkp)
    {
        /* Stuff the fields with the required information */
        eventbkp->imeventtype = qevt;

        /* The IM information comes next */
        eventbkp->imevtdatap = immsgp;
        eventbkp->imevtdatalen = immsglen;

        /* Save the memory release pointer */
        eventbkp->imevtmemfreep = immsgp;

        /* Send the message on its way */
        icsmt_send( &imtcbp->imicmsgque,
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
 * Name:    imgetcwefname
 *
 * Purpose: Looks for a CWE file
 *
 * Parms:   pathp   - base path to search in
 *
 * Return:  fw_image_type_e
 *
 * Abort:   none
 *
 * Notes:   full file name is set in task control block field if found
 *
 **************/
local  enum fw_image_type_e  imgetcwefname (char * pathp)
{
    DIR *dp;
    struct dirent *ep;
    enum fw_image_type_e eftype = FW_IMG_TYPE_INVLD;
    struct imtcb      *imtcbp = imgetcbp(); /* Get the task control block ptr */

    dp = opendir (pathp);
    if (dp != NULL)
    {
        while ((ep = readdir (dp))) /* assignment, not comparison */
        {
            eftype = imgetfiletype (ep->d_name);
            if (eftype == FW_IMG_TYPE_CWE)
            {
                /* For now, find the first CWE file and run with it */
                slmemset ( imtcbp->imqdl.filename,
                           0,
                           sizeof (imtcbp->imqdl.filename) );
                slmemcpy ( imtcbp->imqdl.filename,
                           ep->d_name,
                          sizeof (ep->d_name) );
                slmemset ( imtcbp->imqdl.fullpathfilename,
                           0,
                           sizeof (imtcbp->imqdl.fullpathfilename) );
                strcat ( imtcbp->imqdl.fullpathfilename,
                         imtcbp->imqdl.filepath );
                strcat ( imtcbp->imqdl.fullpathfilename,
                         "/" );
                strcat ( imtcbp->imqdl.fullpathfilename,
                         imtcbp->imqdl.filename );
                break;
           }
        }
        (void) closedir (dp);
    }
    else
        dlLog0( &imtcbp->imdlcb,
                IMLOG_CLASSC,
                "Couldn't open the directory" );

    return eftype;
}
/*************
 *
 * Name:    immdmtimeout
 *
 * Purpose: Called if the immdmtmrwait timer expires, meaning that the
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
immdmtimeout(
    swi_uint32 ipcchannel )
{
    UNUSEDPARAM( ipcchannel );

    return;
}

/*************
 *
 * Name: imtick
 *
 * Purpose: Called once each time a timer tick message is received
 *          by the IM task. This function takes care of managing
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
imtick(void)
{
    struct imtcb      *imtcbp = imgetcbp(); /* Get the task control block ptr */

    /* Decrement the timer and if it reaches zero as a result,
     * invoke the timeout function
     */
    if( imtcbp->immdmwaittmr )
    {
        if( --(imtcbp->immdmwaittmr) ==  0 )
        {
            /* Invoke the timer expiry function */
            immdmtimeout(0);
        }
    }
}

/*************
 *
 * Name:    imrxpkt
 *
 * Purpose: Entry point in IM package for receiving a packet from the modem.
 *
 * Parms:   imbufp      - pointer to beginning of received message
 *          immsglen    - length of received message
 *          memrelp     - pointer to memory that needs to be released
 *          client      - not used
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   Runs in the context of a DS shell
 *
 **************/
local void imrxpkt(
    swi_uint8  *imbufp,
    swi_uint32 immsglen,
    swi_uint8  *memrelp,
    swi_uint16 client )
{
    struct imtcb      *imtcbp = imgetcbp(); /* Get the task control block ptr */
    swi_uint8* rxbufp;

    UNUSEDPARAM( client );

    /* store ds buffer pointer to release when finished download to prevent
     * mempool exhaustion for successive downloads
     */
    imtcbp->imdsmemrelp = memrelp;
#if 0
    dlLog1( &imtcbp->imdlcb,
            IMLOG_CLASSC,
            "Storing DS buffer used for receiving modem response 0x%08x",
            (swi_uint32)imtcbp->imdsmemrelp );
#endif
    /* Copy to our own buffer since DS re-uses it's buffer */
    rxbufp = (swi_uint8 *) mmbufget( &imtcbp->imrxpool );
    slmemset( (char *)rxbufp, 0, IMRXBUFSIZE );
    slmemcpy( (char *)rxbufp, imbufp, immsglen );

    /* Simply queue data on our own queue to run in our own context */
    imqueuevt( eIM_FRAMED_RSP,
               rxbufp,
               immsglen,
               NULL);
}

/*************
 *
 * Name:    impreparefile
 *
 * Purpose: Get size info from download file
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
local void impreparefile()
{
    struct imtcb *imtcbp = imgetcbp(); /* Get the task control block ptr */

    if( FW_IMG_TYPE_INVLD == imgetcwefname (imtcbp->imqdl.filepath) )
    {
        dlLog0( &imtcbp->imdlcb,
                IMLOG_CLASSC,
                "Invalid Image Type, shutting down IM task" );

        imtaskshutdown(NULL);
    }

    /* open the file for reading */
    if(FALSE == swi_ossdkfileopenread( &(imtcbp->imqdl.fp),
                                       imtcbp->imqdl.fullpathfilename ) )
    {
        dlLog0( &imtcbp->imdlcb,
                IMLOG_CLASSC,
                "File opening failed\n" );
        return;
     }

    /* get the file size */
    if(FALSE == swi_ossdkfilesize( &(imtcbp->imqdl.fp),
                                   &( imtcbp->imqdl.cweImageSize ) ) )
    {
        dlLog0( &imtcbp->imdlcb,
                IMLOG_CLASSC,
                "File size failed\n" );
        return;
     }

    /* close the file */
    swi_ossdkfileclose(&imtcbp->imqdl.fp);

    /* FW Image size excludes the header size, the header is sent separately */
    imtcbp->imqdl.fwImageSize = imtcbp->imqdl.cweImageSize -
                                SSDP_OPEN_UNFRAMED_PKT_CWE_HEADER_SZ;
}

/*************
 * helper functions
 *************/

/*************
 *
 * Name:    imbuild_dload_to_sdp_req
 *
 * Purpose: builds the DLOAD to SDP request command
 *
 * Parms:   psDloadToSdp - pointer to storage
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void imbuild_dload_to_sdp_req (
    ssdp_dload_to_sdp_req_s *psDloadToSdp )
{
    /* Build DLOAD to SDP request */
    psDloadToSdp->command = SSDP_DLOAD_TO_SDP;
    psDloadToSdp->flags   = 0x0000; /* unused, reserved for future */

    return;
}

/*************
 *
 * Name:    imbuild_open_unframed_packet_req
 *
 * Purpose: builds the Open Unframed packet request command
 *
 * Parms:   psOpenUnframedReq - pointer to storage
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void imbuild_open_unframed_packet_req (
    ssdp_open_unframed_req_s *psOpenUnframedReq )
{
    struct imtcb *imtcbp = imgetcbp(); /* Get the task control block ptr */

    /* Build Open Unframed packet request */
    psOpenUnframedReq->command            = SSDP_OPEN_UNFRAMED_PKT_COMMAND;
    psOpenUnframedReq->image_mode         = IM_SIERRA_CWE_IMAGE;
    psOpenUnframedReq->image_length       = imtcbp->imqdl.cweImageSize;
    psOpenUnframedReq->window_size        = 1;
    psOpenUnframedReq->max_payload_length = imtcbp->imqdl.fwImageSize;
    psOpenUnframedReq->feature_flag       = 0x0000;
/*    psOpenUnframedReq->feature_flag = 0x0001;   Not supported yet */

    psOpenUnframedReq->pimage_header_payload = imtcbp->imqdl.pImageBlock;

    return;
}

 /*************
  * state machine functions
  *************/
/*************
 *
 * Name:    imtaskshutdown
 *
 * Purpose: Shuts down IM task
 *
 * Parms:   imrxdatap - not used
 *
 * Return:  Does not return, the thread terminates
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void imtaskshutdown( void *imrxdatap )
{
    struct imtcb *imtcbp = imgetcbp(); /* Get the task control block ptr */

    UNUSEDPARAM( imrxdatap );
    /* close the file */
    swi_ossdkfileclose(&imtcbp->imqdl.fp);

    /* Release the last consumed event block */
    if( NULL != imtaskevtmemfreep )
    {
        imrelevtbk( ( struct imevtblock * )imtaskevtmemfreep );
        imtaskevtmemfreep = NULL;
    }

    /* clear firmware download path */
    slmemset( imtcbp->imqdl.filepath,
              0,
              sizeof(imtcbp->imqdl.filepath) );

    dlLog0( &imtcbp->imdlcb,
            IMLOG_CLASSC,
           "Task terminated\n" );

    swi_ossdkthreadterminate();
}

/*************
 *
 * Name:    im_error
 *
 * Purpose: Does nothing, just log error.
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
local void im_error( void *imrxdatap )
{
    struct imtcb *imtcbp = imgetcbp(); /* Get the task control block ptr */
    struct sdp_packet *psdp = (struct sdp_packet *)imrxdatap;
    swi_uint32 err;
    swi_uint8 *errp = (swi_uint8 *)imrxdatap+1;

    if (psdp->phdr.command == 0x0d)
    {
        err = piget32(&errp);

        dlLog( &imtcbp->imdlcb,
               IMLOG_CLASSC,
               "error, st:%d, error:0x%08x, %s\n",
               (swi_uint32)imtcbp->imsmp->state,
               (swi_uint32)err,
               (swi_uint32)(psdp+5),
               (swi_uint32)NULL );
        imtaskshutdown( NULL );
    }
    return;
}

/*************
 *
 * Name:    imdloadtosdp
 *
 * Purpose: Send switch from dload to sdp mode command to modem.
 *
 * Parms:   imrxdatap  - not used here
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void imdloadtosdp( void *imrxdatap )
{
    struct imtcb            *imtcbp = imgetcbp(); /* task control block ptr */
    swi_uint8               *txbufp;
    ssdp_dload_to_sdp_req_s sDloadToSdp;
    swi_uint32              packetsize;

    UNUSEDPARAM( imrxdatap );

    imbuild_dload_to_sdp_req ( &sDloadToSdp );

    txbufp = (swi_uint8 *) mmbufget( &imtcbp->imtxpool );

    packetsize = imssdp_pack_request ( (struct sdp_packet *)&sDloadToSdp,
                                       txbufp );
    imsendframed (txbufp, packetsize);

    dlLog( &imtcbp->imdlcb,
           IMLOG_CLASSC,
           "Send DLOAD to SDP (0x%02x), st:%i\n",
           (swi_uint32)sDloadToSdp.command,
           (swi_uint32)imtcbp->imsmp->state,
           (swi_uint32)NULL,
           (swi_uint32)NULL );

    /* free memory allocated for IM request */
    mmbufrel(txbufp);
    return;
}

/*************
 *
 * Name:    imsendhello
 *
 * Purpose: Send hello to modem .
 *
 * Parms:   imrxdatap - pointer to response message from modem
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void imsendhello( void *imrxdatap )
{
    struct imtcb      *imtcbp = imgetcbp(); /* Get the task control block ptr */
    swi_uint8         *txbufp;
    ssdp_hello_req_s  sHelloReq;
    swi_uint32        packetsize;
    struct sdp_packet *psdp = (struct sdp_packet *)imrxdatap;

    /*   ??  VerifyACK (imrxdatap) */
    ssdp_dload_to_sdp_rsp_s sDloadToSdpRsp;
    imssdp_unpack_response ((swi_uint8 *)psdp,
                            (struct sdp_packet *)&sDloadToSdpRsp);

    dlLog( &imtcbp->imdlcb,
           IMLOG_CLASSC,
           "Dload to SDP Rsp, st:%i, 0x%02x\n",
           (swi_uint32)imtcbp->imsmp->state,
           (swi_uint32)sDloadToSdpRsp.command,
           (swi_uint32)NULL,
           (swi_uint32)NULL );

    imbuild_hello_req( &sHelloReq );

    txbufp = (swi_uint8 *) mmbufget( &imtcbp->imtxpool );

    packetsize = imssdp_pack_request ( (struct sdp_packet *)&sHelloReq,
                                       txbufp );

    imsendframed (txbufp, packetsize);

    dlLog( &imtcbp->imdlcb,
           IMLOG_CLASSC,
           "Send Hello (0x%02x), st:%i\n",
           (swi_uint32)sHelloReq.command ,
           (swi_uint32)imtcbp->imsmp->state,
           (swi_uint32)NULL,
           (swi_uint32)NULL );

    /* free memory allocated for IM request */
    mmbufrel(txbufp);

    /* We handle the hello response in the next state */
    return;
}
/*************
 *
 * Name:    imsendOFpkt
 *
 * Purpose: Send Open frame message to modem
 *
 * Parms:   imrxdatap - pointer to response message from modem
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void imsendOFpkt( void *imrxdatap )
{
    struct imtcb             *imtcbp = imgetcbp(); /* task control block ptr */
    swi_uint8                *txbufp;
    ssdp_open_unframed_req_s sOpenUnframedReq;
    swi_uint32               packetsize;
    struct sdp_packet        *psdp = (struct sdp_packet *)imrxdatap;
    struct swi_osfilestats   fileinfo;

    /* if we got here, it means a Hello response came back, extract info from
     * the response */
    imtcbp->imrxdatap = (swi_uint8*)psdp;
    OpenQDLPort ( imtcbp->imqdl.portID,
                  imtcbp->imqdl.bBARMode,
                  &(imtcbp->imqdl.MajorVersion),
                  &(imtcbp->imqdl.MinorVersion) );

    /* get the download file into memory */
    impreparefile();

    /* Get buffer from the tx pool */
    txbufp = (swi_uint8 *) mmbufget( &imtcbp->imtxpool );

    /* Map the header chunk that we are interested in */
    fileinfo.filesize = IM_CHUNK_SIZE;
    fileinfo.vmapfileoffset = 0;
    if ( FALSE == swi_ossdk_mmapro(imtcbp->imqdl.fullpathfilename, &fileinfo) )
    {
        dlLog0( &imtcbp->imdlcb,
               IMLOG_CLASSC,
               "imsendOFpkt mmap error\n");
        return;
    }
    imtcbp->imqdl.pImageBlock = (swi_char *)fileinfo.pvmap;

    imbuild_open_unframed_packet_req ( &sOpenUnframedReq );

    packetsize = imssdp_pack_request ( (struct sdp_packet *)&sOpenUnframedReq,
                                       txbufp );

    imsendframed (txbufp, packetsize);

    dlLog2( &imtcbp->imdlcb,
            IMLOG_CLASSC,
            "Send OF packet (0x%02x), st:%i\n",
            (swi_uint32)sOpenUnframedReq.command,
            (swi_uint32)imtcbp->imsmp->state );

    if( FALSE == swi_ossdk_umapfile( &fileinfo ) )
    {
        dlLog0( &imtcbp->imdlcb,
               IMLOG_CLASSC,
               "imsendOFpkt munmap error\n");
        return;
    }

    /* free memory allocated for IM request */
    mmbufrel(txbufp);

    return;
}

/*************
 *
 * Name:    imsendUFwrite
 *
 * Purpose: Send Unframed Write to modem
 *
 * Parms:   imrxdatap - pointer to response message from modem
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   This function writes the first block of the firmware image to the
 *          modem. On the MC7750 cwe file, the header is not required when
 *          sending the firmware image as it has been sent in the Open Unframed
 *          command. Hence we exclude the header portion in the write.
 *
 **************/
local void imsendUFwrite( void *imrxdatap )
{
    struct imtcb      *imtcbp = imgetcbp(); /* Get the task control block ptr */
    struct swi_osfilestats   fileinfo;
    ULONG  rCode;

    dlLog1( &imtcbp->imdlcb,
            IMLOG_CLASSC,
            "Send UF write, st:%i\n",
            (swi_uint32)imtcbp->imsmp->state );

    imtcbp->imrxdatap = (swi_uint8 *)imrxdatap;
    imtcbp->imqdl.ImageType = IM_SIERRA_CWE_IMAGE;
    rCode = PrepareQDLImageWrite( imtcbp->imqdl.ImageType,
                                  imtcbp->imqdl.cweImageSize,
                                  &(imtcbp->imqdl.BlockSize) );

    if (rCode != 0)
    {
        dlLog( &imtcbp->imdlcb,
               IMLOG_CLASSC,
               "Send UF write Resp, st:%i, Status:%i\n",
               (swi_uint32)imtcbp->imsmp->state,
               (swi_uint32)rCode,
               (swi_uint32)NULL,
               (swi_uint32)NULL );
        return;
    }

    /* Calculate number of chunks required, based on whole image. The header
     * should be excluded in the first write
     */
    imtcbp->imqdl.numwrites = (imtcbp->imqdl.cweImageSize)/(imtcbp->imqdl.chunkSize);
    imtcbp->imqdl.remainder = (imtcbp->imqdl.cweImageSize)%(imtcbp->imqdl.chunkSize);

    if (imtcbp->imqdl.remainder != 0)
    {
        imtcbp->imqdl.numwrites++;
    }

    ULONG blockSize = IM_CHUNK_SIZE;
    if (imtcbp->imqdl.numwrites == 1)
    {
        blockSize = imtcbp->imqdl.remainder;
    }

    /* Map the first block and write it to modem */
    imtcbp->imqdl.nextchunkoffset = 0; /* start from beginning */
    fileinfo.filesize             = IM_CHUNK_SIZE; /* map more than required */
    fileinfo.vmapfileoffset       = 0;
    if ( FALSE == swi_ossdk_mmapro(imtcbp->imqdl.fullpathfilename, &fileinfo) )
    {
        dlLog0( &imtcbp->imdlcb,
                IMLOG_CLASSC,
                "imsendUFwrite mmap error\n");
        return;
    }

    imtcbp->imqdl.pImageBlock     = (swi_char *)fileinfo.pvmap;
    imtcbp->imqdl.sequenceNumber  = 0;

    WriteQDLImageBlock( imtcbp->imqdl.sequenceNumber++,
                        blockSize - SSDP_OPEN_UNFRAMED_PKT_CWE_HEADER_SZ,
                        (BYTE*)imtcbp->imqdl.pImageBlock +
                        SSDP_OPEN_UNFRAMED_PKT_CWE_HEADER_SZ);

    if( FALSE == swi_ossdk_umapfile( &fileinfo ) )
    {
        dlLog0( &imtcbp->imdlcb,
                IMLOG_CLASSC,
                "imsendUFwrite: Error munmapping the file\n");
    }

    imtcbp->imqdl.numwrites--;
    imtcbp->imqdl.nextchunkoffset += blockSize;
    imtcbp->imqdl.byteswritten = blockSize - SSDP_OPEN_UNFRAMED_PKT_CWE_HEADER_SZ;
    return;
}

/*************
 *
 * Name:    imsendDone
 *
 * Purpose: Send Done
 *
 * Parms:   imrxdatap - pointer to response message from modem
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void imsendDone( void *imrxdatap )
{
    struct imtcb              *imtcbp = imgetcbp(); /* Task control block ptr */
    ssdp_done_req_s           sDoneReq;
    swi_uint32                packetsize;
    swi_uint8                *txbufp;

    UNUSEDPARAM( imrxdatap );

    dlLog( &imtcbp->imdlcb,
           IMLOG_CLASSC,
           "Send Done (0x%02x), st:%i\n",
           (swi_uint32)SSDP_DONE_REQ_COMMAND,
           (swi_uint32)imtcbp->imsmp->state,
           (swi_uint32)NULL,
           (swi_uint32)NULL );

    /* send the Done Request */
    sDoneReq.command = SSDP_DONE_REQ_COMMAND;

    /* Get buffer from the tx pool */
    txbufp = (swi_uint8 *) mmbufget( &imtcbp->imtxpool );
    packetsize = imssdp_pack_request ( (struct sdp_packet *)&sDoneReq,
                                       txbufp );

    imsendframed (txbufp, packetsize);
    /* free memory allocated for IM request */
    mmbufrel(txbufp);

    return;
}

/*************
 *
 * Name:    imsendNextUFwrite
 *
 * Purpose: Send Next block of Unframed Write to modem
 *
 * Parms:   imrxdatap - pointer to response message from modem
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void imsendNextUFwrite( void *imrxdatap )
{
    struct imtcb      *imtcbp = imgetcbp(); /* Get the task control block ptr */
    ssdp_unframed_write_rsp_s sUnframedWriteRsp;
    struct swi_osfilestats    fileinfo;

    /* Cut down number of lines logged */
    if ( ((~imtcbp->imqdl.numwrites & 0x7f) == 0 ) ||
            imtcbp->imqdl.numwrites == 0 )
    {
        dlLog( &imtcbp->imdlcb,
               IMLOG_CLASSC,
               "Send Next UF write, st:%i, writesleft:%i, written:%i, tot:%i\n",
               (swi_uint32)imtcbp->imsmp->state,
               (swi_uint32)imtcbp->imqdl.numwrites,
               (swi_uint32)imtcbp->imqdl.byteswritten,
               (swi_uint32)imtcbp->imqdl.fwImageSize );
    }

    imtcbp->imrxdatap = (swi_uint8 *)imrxdatap;

    /* Get Unframed Write response  info */
    imssdp_unframed_write_unpack( (swi_uint8*)imrxdatap, &sUnframedWriteRsp);

    if (sUnframedWriteRsp.status != SSDP_UNFRAMED_WRITE_DATA_RECEIVED_OK)
    {
        dlLog2( &imtcbp->imdlcb,
                IMLOG_CLASSC,
                "Unframed Write response error, st:%i, Status:0x%02x\n",
                (swi_uint32)imtcbp->imsmp->state,
                (swi_uint32)sUnframedWriteRsp.status );
        /* Abort ? */
    }

    /* If download is completed, move to the next step */
    if (imtcbp->imqdl.numwrites == 0)
    {
        imsendDone (imrxdatap);
        imtcbp->imsmp->state = stIM_DONE;
        return;
    }

    ULONG blockSize   = IM_CHUNK_SIZE;
    fileinfo.filesize = IM_CHUNK_SIZE;
    if (imtcbp->imqdl.numwrites == 1)
    {
        /* Only map remainder, not  beyond file size boundary */
        blockSize         = imtcbp->imqdl.remainder;
        fileinfo.filesize = blockSize;
    }

    fileinfo.vmapfileoffset = imtcbp->imqdl.nextchunkoffset;
    if ( FALSE == swi_ossdk_mmapro(imtcbp->imqdl.fullpathfilename, &fileinfo) )
    {
        dlLog0( &imtcbp->imdlcb,
               IMLOG_CLASSC,
               "imsendNextUFwrite mmap error\n");
        return;
    }
    imtcbp->imqdl.pImageBlock = (swi_char *)fileinfo.pvmap;

    /* write the next block */
    WriteQDLImageBlock( imtcbp->imqdl.sequenceNumber++,
                        blockSize,
                        (BYTE*)imtcbp->imqdl.pImageBlock );

    if( FALSE == swi_ossdk_umapfile( &fileinfo ) )
    {
        dlLog0( &imtcbp->imdlcb,
                IMLOG_CLASSC,
                "imsendNextUFwrite: Error munmapping the file\n");
        return;
    }

    imtcbp->imqdl.nextchunkoffset += blockSize;
    (imtcbp->imqdl.numwrites)--;
    imtcbp->imqdl.byteswritten += blockSize;

    return;
}

/*************
 *
 * Name:    imsendDoneFinal
 *
 * Purpose: Send DoneFinal.
 *
 * Parms:   imrxdatap - pointer to response message from modem
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void imsendDoneFinal( void *imrxdatap )
{
    struct imtcb      *imtcbp = imgetcbp(); /* Get the task control block ptr */
    enum eQCWWANError rCode;

    dlLog( &imtcbp->imdlcb,
           IMLOG_CLASSC,
           "Send DoneFinal, st:%i\n",
           (swi_uint32)imtcbp->imsmp->state,
           (swi_uint32)NULL,
           (swi_uint32)NULL,
           (swi_uint32)NULL );

    imtcbp->imrxdatap = (swi_uint8 *)imrxdatap;
    rCode = ValidateQDLImages( &(imtcbp->imqdl.ImageType) );

/* RILSTART */
    /* Set the download status */
    qmSetFwDownloadStatus(rCode);
/* RILSTOP */

    /* !0 = Inform device that QDL port is being closed, ie, reset */
    CloseQDLPort( !0 );

    /* release DS buffer used for receiving responses from modem */
    mmbufrel(imtcbp->imdsmemrelp);
    dlLog1( &imtcbp->imdlcb,
            IMLOG_CLASSC,
            "Releasing DS buffer used for receiving modem response 0x%08x",
            (swi_uint32)imtcbp->imdsmemrelp );

    imtaskshutdown (imrxdatap);

    return;
}

/* These tables located here to avoid forward declarations of above functions */
/*************
 *
 * Name:     im_stt
 *
 * Purpose:  Describes state transitions used by the im state machine
 *
 * Notes:
 *
  0stIM_IDLE       = stIM_MIN,
  1stIM_WAITSDP,
  2stIM_HELLO,
  3stIM_OF_PKT,
  4stIM_UF_WRITE,
  5stIM_DONE,
  6stIM_DONE_FINAL,
    eIM_SDP_ACK = eIM_MIN_EVT,
    eIM_HELLO_RSP,
    eIM_OFpkt_RSP,
    eIM_UFwrite_RSP,
    eIM_DONE_RSP,
    eIM_DLOAD_RSP,
    eIM_GET_IMAGE_PREF_RSP,

 *
 **************/
local swi_uint8 im_stt[stIM_NUM_ACTIVE_STATES][eIM_MAX_EVENT] =
{
              /* evSDPACK     evhellorsp   evOFpktrsp   evUFwritersp   evDonersp        DLOAD        evGImgPref    error        evShutDown      evBadELFhdr  evhostIOerr */
/* state 0 */   {stIM_WAITSDP,stIM_IGNORE, stIM_IGNORE, stIM_IGNORE,   stIM_IGNORE,     stIM_IGNORE, stIM_IGNORE, stIM_IGNORE, stIM_IGNORE,    stIM_IGNORE, stIM_IGNORE},
/* state 1 */   {stIM_IGNORE, stIM_HELLO,  stIM_IGNORE, stIM_IGNORE,   stIM_IGNORE,     stIM_IGNORE, stIM_IGNORE, stIM_IGNORE, stIM_IGNORE,    stIM_IGNORE, stIM_IGNORE},
/* state 2 */   {stIM_IGNORE, stIM_IGNORE, stIM_OF_PKT, stIM_IGNORE,   stIM_IGNORE,     stIM_IGNORE, stIM_IGNORE, stIM_HELLO,  stIM_IGNORE,    stIM_IGNORE, stIM_IGNORE},
/* state 3 */   {stIM_IGNORE, stIM_IGNORE, stIM_IGNORE, stIM_UF_WRITE, stIM_IGNORE,     stIM_IGNORE, stIM_IGNORE, stIM_IGNORE, stIM_IGNORE,    stIM_IGNORE, stIM_IGNORE},
/* state 4 */   {stIM_IGNORE, stIM_IGNORE, stIM_IGNORE, stIM_UF_WRITE, stIM_IGNORE,     stIM_IGNORE, stIM_DONE,   stIM_IGNORE, stIM_IGNORE,    stIM_IGNORE, stIM_IGNORE},
/* state 5 */   {stIM_IGNORE, stIM_IGNORE, stIM_IGNORE, stIM_IGNORE,   stIM_DONE_FINAL, stIM_IGNORE, stIM_IGNORE, stIM_IGNORE, stIM_DONE_FINAL,stIM_IGNORE, stIM_IGNORE},
/* state 6 */   {stIM_IGNORE, stIM_IGNORE, stIM_IGNORE, stIM_IGNORE,   stIM_IGNORE,     stIM_IGNORE, stIM_IGNORE, stIM_IGNORE, stIM_IGNORE,    stIM_IGNORE, stIM_IGNORE}
};

/*************
 *
 * Name:    im_sat
 *
 * Purpose:  set of functions executed upon entering a state
 *
 * Members:  one for each state
 *
 * Notes:
 *
 **************/
local void(*im_sat[stIM_NUM_ACTIVE_STATES][eIM_MAX_EVENT])(void *) =
{
        /*imdloadtosdp is called when task starts */
              /* evSDKACK     evhellorsp   evOFpktrsp     evUFwritersp       evDonersp        evGImgPref   evError   evShutdown      evBadELFhdr  evhostIOerr  */
/* state 0 */   {imsendhello, im_error,    im_error,      im_error,          im_error,        im_error,    im_error, im_error,       im_error,    im_error },
/* state 1 */   {im_error,    imsendOFpkt, im_error,      im_error,          im_error,        im_error,    im_error, im_error,       im_error,    im_error },
/* state 2 */   {im_error,    im_error,    imsendUFwrite, im_error,          im_error,        im_error,    im_error, im_error,       im_error,    im_error },
/* state 3 */   {im_error,    im_error,    im_error,      imsendNextUFwrite, im_error,        im_error,    im_error, im_error,       im_error,    im_error },
/* state 4 */   {im_error,    im_error,    im_error,      imsendNextUFwrite, im_error,        im_error,    im_error, im_error,       imsendhello, im_error },
/* state 5 */   {im_error,    im_error,    im_error,      im_error,          imsendDoneFinal, im_error,    im_error, imtaskshutdown, im_error,    im_error },
/* state 6 */   {im_error,    im_error,    im_error,      im_error,          im_error,        im_error,    im_error, im_error,       im_error,    im_error }
};

/*************
 *
 * Name:    imtask
 *
 * Purpose: The Image Management task. This task coordinates the downloading of
 *          Firmware to the modem
 *
 * Parms:   paramp - pointer to data that may be passed in when this task
 *                   is started.
 *
 * Return:  exits when download is completed or errored out.
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local void
imtask(
    void *paramp )
{
    swi_bool          releaseevt; /* Some event blocks should not be released */
    swi_uint32        rmsg;       /* ptr to incoming IM response from modem */
    struct imevtblock *eventbkp;
    struct imtcb      *imtcbp = imgetcbp(); /* Get the task control block ptr */
    struct sdp_packet *psdp;
    BYTE               event;

    UNUSEDPARAM( paramp );

    /* Initialize the SMT Message queue structure */
    icsmt_create( &imtcbp->imicmsgque, "IMMESSAGEQ", IMMSGDEPTH );

    dlLog(  &imtcbp->imdlcb,
            IMLOG_CLASSC,
            "Task started\n",
           (swi_uint32)NULL,
           (swi_uint32)NULL,
           (swi_uint32)NULL,
           (swi_uint32)NULL );

    /* These two are set before we start the state machine proceesing */
    GobiEnumerateQDLPorts ( imtcbp->imqdl.pPortSize,
                            imtcbp->imqdl.pPorts );

    SetQDLTimeout ( IMDEFAULTIMEOUT );

    /* Initialize state tables */
    imtcbp->imsmp                = &imst;
    imtcbp->imsmp->sat           = *im_sat;
    imtcbp->imsmp->stt           = *im_stt;
    imtcbp->imsmp->evnum         = eIM_MAX_EVENT;
    imtcbp->imsmp->state         = stIM_IDLE;
    imtcbp->imqdl.chunkSize      = IM_CHUNK_SIZE;
    imtcbp->imsmp->stIgnore      = stIM_IGNORE;
    imtcbp->imsmp->stInvld       = stIM_INVAL;
    imtcbp->imqdl.remainder      = 0;
    imtcbp->imqdl.numwrites      = 0;;
    imtcbp->imqdl.sequenceNumber = 0;
    slmemset( imtcbp->imqdl.filename,
              0,
              sizeof(imtcbp->imqdl.filename) );
    slmemset( imtcbp->imqdl.fullpathfilename,
              0,
              sizeof(imtcbp->imqdl.fullpathfilename) );

    smvalidate( imtcbp->imsmp );

    /* Kick off the state machine by asking FW to switch to SDP mode */
    imqueuevt(eIM_DL2SDP_REQ, NULL, 0, NULL );

    /* Main line of processing begins next */
    for(;;)
    {
        /* Reset the release event block flag */
        releaseevt = TRUE;

        /* Wait for an incoming IPC message then dispatch it.
         * Timeout value is NULL to wait forever. We get our
         * ticks from the IC timer task.
         */
        icsmt_rcv( &imtcbp->imicmsgque,
                   IMNOTIMEOUT,
                   &rmsg );

        if(rmsg)
        {
            /* Cast the data to an event block pointer */
            eventbkp = (struct imevtblock *)rmsg;

            /* Dispatch to the handler for this type of message */
            switch( eventbkp->imeventtype )
            {
                case eIM_DL2SDP_REQ:
                    imdloadtosdp(NULL);
                    break;

                case eIM_FRAMED_RSP: /* Rx Messages are Framed at this point */
                     imtaskevtmemfreep = (swi_uint8 *)eventbkp;
                     imtcbp->imsmp->udata = eventbkp->imevtdatap;
                     imunframe (imtcbp->imsmp->udata, eventbkp->imevtdatalen);
                     psdp = (struct sdp_packet *)imtcbp->imsmp->udata;
                     event = imssdp_map_cmd(psdp->phdr.command);
                     if (psdp->phdr.command == 0x02)
                     {
                         /* temp workaround for FW returning same code for different commands */
                         switch (imtcbp->imsmp->state)
                         {
                             case stIM_IDLE:
                                 event = eIM_SDP_ACK;
                                 break;
                             case stIM_WAITSDP:
                                 event = eIM_HELLO_RSP;
                                 break;
                             default:
                                 break; /* no need to adjust */
                         }
                     }
#ifdef DEBUG
                    dlLog( &imtcbp->imdlcb,
                           IMLOG_CLASSC,
                           "Framed Rsp: st:%i, ev:%d, sdpcmd:0x%02x\n",
                           (swi_uint32)imtcbp->imsmp->state,
                           (swi_uint32)event,
                           (swi_uint32)psdp->phdr.command,
                           (swi_uint32)NULL );
#endif
                   smrun ( imtcbp->imsmp, event );
                   if (imtcbp->imhdmemrelp != NULL)
                   {
                       mmbufrel(imtcbp->imsmp->udata);
                   }
                   break;

                /* Periodic timer tick message */
                case eIM_TMRTICK:
                    imtick();
                    releaseevt = FALSE;
                    break;

                default:
                    {
                        char errmsg[100];
                        snprintf(errmsg, sizeof(errmsg),
                                 "%s:%d Invalid Event message",
                                 (char *)__func__, __LINE__);
                        erAbort(errmsg,
                                rmsg);
                    }
                    break;
            }

            /* Message has been handled, release the associated resources */
            if( releaseevt )
            {
                imrelevtbk( eventbkp );
                imtaskevtmemfreep = NULL;
            }
        }
        /* Do nothing if the message queue is empty or on timeout */
    }
}

/*************
 *
 * Name:    imdssinitcb
 *
 * Purpose: IM task DS shell ready callback
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
local void imdssinitcb(swi_bool dsshellready)
{
    struct imtcb *imtcbp = imgetcbp();

    if( !dsshellready )
    {
        dlLog0( &imtcbp->imdlcb,
                IMLOG_CLASSC,
                "IM task DS shell failed to start" );

        imtaskshutdown(NULL);
    }

    /* Create the IM task */
    swi_ossdkthreadcreate( imtask,
                           NULL );
}

/*************
 *
 * Name:    imdsstart
 *
 * Purpose: Instantiate DS interface for IM.
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
imdsstart()
{
    struct imtcb      *imtcbp = imgetcbp(); /* Get the task control block ptr */

    /* Initialize  DS shell parameters */
    imtcbp->imdsshell.dsipcshellnamep = (swi_uint8 *)CIIPCUIMRR1;
    imtcbp->imdsshell.dsipccbfcnp     = imrxpkt;
    imtcbp->imdsshell.dssinitcbfp     = imdssinitcb;

    /* Activate DS shell for sending SDP requests and reading responses */
    swi_ossdkthreadcreate( dslaunchshell,
                           &imtaskblk.imdsshell);
}

/*************
 *
 * Name:    imtaskinit
 *
 * Purpose: Creates the task by making it known to the os
 *          package.
 *
 * Parms:   fwfilepathp - pointer to path where download file is located at
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   iminit() must have been called
 *
 **************/
global void
imtaskinit( const char *fwfilepathp )
{
    swi_uint32 vid,pid;

    /* do nothing if path is empty */
    if( 0 == slstrlen( fwfilepathp) )
    {
        return;
    }

    if( usvidpid(&vid, &pid) )
    {
        struct imtcb *imtcbp = imgetcbp();

        /* save firmware download path */
        slmemset( imtcbp->imqdl.filepath,
                  0,
                  sizeof(imtcbp->imqdl.filepath) );

        if( 0 != slstrlen(fwfilepathp) )
        {
            slmemcpy ( imtcbp->imqdl.filepath,
                       (swi_uint8 *)fwfilepathp,
                       slstrlen(fwfilepathp) );
        }

        switch(pid)
        {
            case SWI_QMIPID_BOOT:
                /* Prepare for logging */
                if( !imtcbp->imdlcb.registered )
                {
                    dlregister( "IM", &imtcbp->imdlcb, TRUE );
                }

                /* create IM DS shell prior to starting the download*/
                imdsstart();
                break;


            case GOBI3KGENERIC_BOOT0: /* Gobi devices */
            case GOBI3KGENERIC_BOOT1:
            case GOBI3KGENERIC_BOOT2:
            case GOBI3KGENERIC_BOOT3:
            case GOBI3KHP_BOOT:
                /* Prepare for logging */
                if( !imtcbp->imdlcb.registered )
                {
                    dlregister( "IMGOBI", &imtcbp->imdlcb, TRUE );
                }
                /* create Gobi IM DS shell prior to starting the download */
                imgobidsstart();
                break;

            default:
                {
                    char errmsg[100];
                    snprintf(errmsg, sizeof(errmsg),
                             "%s:%d Invalid Product ID",
                             (char *)__func__, __LINE__);
                    erAbort(errmsg, pid);
                }
                break;
        }
    }
}

/*************
 *
 * Name:    iminit
 *
 * Purpose: Package initialization routine
 *
 * Parms:   none
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   Given a) the download task is terminated once a download completes
 *                   or fails and
 *                b) there is no mechanism for freeing the
 *                   memory reserved for a memory pool, the memory pool for the
 *                   IM task is created here to avoid a memory leak
 *                   for a session whereby multiple downloads occur during the
 *                   lifetime of a SLQS process”
 *
 **************/
global void iminit(void)
{
    struct imtcb *imtcbp = imgetcbp(); /* Get the task control block ptr */

    /* Ensure the modem wait timer is disabled initially */
    imtcbp->immdmwaittmr = 0;

    /* invalidate firmware image file descriptor */
    imtcbp->imqdl.fp.osfiledescrip = -1;

    /* Create the Event Pool for receiving IM responses */
    mmpcreate( &imtcbp->imevtpool,
               "IMEVTPOOL",
               IMEVTPOOLSZ,
               sizeof(struct imevtblock) );

    /* create a memory pool for download buffer allocation */
    mmpcreate( &imtcbp->imdlpool,
               "IMDLPOOL",
               IMDLPOOLSZ,
               IMDLBUFSIZE );

    /* create a memory pool for transmit buffer allocation */
    mmpcreate( &imtcbp->imtxpool,
               "IMTXPOOL",
               IMTXPOOLSZ,
               IMTXBUFSIZE );

    /* create a memory pool for receive buffer allocation */
    mmpcreate( &imtcbp->imrxpool,
               "IMRXPOOL",
               IMRXPOOLSZ,
               IMRXBUFSIZE );
}
