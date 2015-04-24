/*************
 *
 * Filename:    imtaskgobi.c
 *
 * Purpose:     IM GOBI task
 *
 * Copyright:   © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/

/*-------------
  include files
 --------------*/
#include <stdio.h>

#include "sm/smudefs.h"
#include "os/swi_ossdk.h"
#include "ci/ciudefs.h"
#include "sl/sludefs.h"
#include "pi/piudefs.h"
#include "mm/mmudefs.h"

#include "imssdp.h"
#include "imcommon.h"
#include "imssdp_image_preference.h"
#include "imssdp_hello.h"
#include "imssdp_open_unframed.h"
#include "imssdp_unframed_write.h"
#include "imssdp_done.h"
#include "imapi.h"
#include "imidefs.h"
#include "imudefs.h"
/* RILSTART */
#include "qm/qmiproto.h"
/* RILSTOP */

/*-------
  Defines
 --------*/
#if 0
#define IMDBGFLG
#endif

/* Forward Declarations */
local void imGobiTaskShutdown(void *imrxdatap);

/*-------------
  Local storage
 --------------*/
/* IM task control block */
local struct imdatapkt  datapkt;

/* Gobi Image Management State machines */
local sm_t smt;  /* Gobi IM task state machine */
local sm_t smfd; /* Gobi Firmware Download state machine */
local sm_t smtr; /* Gobi Transaction state machine */

/*************
 *
 * Name:    imGobiRelEvtBk
 *
 * Purpose: Release the resources associated with the IM event block
 *
 * Parms:   (IN)eventbkp     - IM event block
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void
imGobiRelEvtBk(
    struct imevtblock *eventbkp )
{
    /* release memory allocated for event data */
    if( eventbkp->imevtmemfreep != NULL )
    {
        mmbufrel( eventbkp->imevtmemfreep );
    }

    /* release the event block itself */
    mmbufrel( (swi_uint8 *)eventbkp );
}

/*************
 *
 * Name:    imGobiQevt
 *
 * Purpose: Send intertask message to IM task
 *
 * Parms:   qevt            - event type
 *          (IN)imimsgp     - IM Request message
 *          (IN)memfreep    - memory to be released by IM task
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void
imGobiQevt(
    swi_uint8  qevt,
    swi_uint8  *immsgp,
    swi_uint32 immsglen,
    swi_uint8  *memfreep )
{
    struct imevtblock *eventbkp;
    struct imtcb      *imtcbp = imgetcbp();

    UNUSEDPARAM( memfreep );

    /* allocate an event block */
    eventbkp = (struct imevtblock *) mmbufget( &imtcbp->imevtpool );

    if( eventbkp )
    {
        /* populate the event block to be queued to the IM task */
        /* event received */
        eventbkp->imeventtype = qevt;
        /* data corresponding to received event */
        eventbkp->imevtdatap = immsgp;
        eventbkp->imevtdatalen = immsglen;
        /* memory to be released by the receiving task */
        eventbkp->imevtmemfreep = immsgp;

        /* send intertask message */
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
 * Name:    imGobiRxpkt
 *
 * Purpose: Entry point in Gobi IM package for receiving a packet from the modem
 *
 * Parms:   (IN)imbufp      - pointer to beginning of received message
 *          (IN)immsglen    - length of received message
 *          (IN)memrelp     - pointer to memory that needs to be released
 *          client          - not used
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   Runs in the context of a DS shell
 *
 **************/
local void imGobiRxpkt(
    swi_uint8  *imbufp,
    swi_uint32 immsglen,
    swi_uint8  *memrelp,
    swi_uint16 client )
{
    struct imtcb *imtcbp = imgetcbp();
    swi_uint8    *prxbufp;

    UNUSEDPARAM( client );

    /*
     * store ds buffer pointer to release when finished download to prevent
     * mempool exhaustion for successive downloads
     */
    imtcbp->imdsmemrelp = memrelp;

    /*Copy to our own buffer since DS re-uses it's buffer */
    prxbufp = (swi_uint8 *) mmbufget( &imtcbp->imrxpool );
    if(NULL == prxbufp)
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d no memory in RX pool",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, 0 );
    }

    slmemset( (char *)prxbufp, 0, IMRXBUFSIZE );
    slmemcpy( (char *)prxbufp, imbufp, immsglen );

    /* Intialize the transaction state machine with the recieved data */
    datapkt.datalen = immsglen;
    datapkt.pdata = prxbufp;
    imtcbp->imsmp->udata = (void *)&datapkt;

    /* Send the message to the imGobitask */
    imGobiQevt( ev_TRANS_RX_RSP,
                NULL,
                0,
                NULL);
}

/*************
 * helper functions
 *************/

/*************
 *
 * Name:    imgobibld_open_unframed_pkt_req
 *
 * Purpose: builds the Open Unframed packet request command
 *
 * Parms:   (IN)psOpenUnframedReq    - pointer to storage
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void imgobibld_open_unframed_pkt_req (
    ssdp_open_unframed_req_s *psOpenUnframedReq )
{
    struct imtcb   *imtcbp = imgetcbp();
    immbnimglist_t *pMbnImglist =
                          (immbnimglist_t *)&imtcbp->imqdl.gobimgs2dwld;

    /* Build Open Unframed packet request */
    psOpenUnframedReq->command            = SSDP_OPEN_UNFRAMED_PKT_COMMAND;
    psOpenUnframedReq->image_mode         = pMbnImglist->mbnimgs[pMbnImglist->nextimageidx].type;
    psOpenUnframedReq->image_length       = imtcbp->imqdl.fwImageSize;
    psOpenUnframedReq->window_size        = 1;
    psOpenUnframedReq->max_payload_length = imtcbp->imqdl.fwImageSize;
    psOpenUnframedReq->feature_flag       = 0x0000;

    return;
}

/*************
 *
 * Name:    imgobipreparefile
 *
 * Purpose: Get size info for the image to be downloaded
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
local void imgobipreparefile()
{
    struct imtcb   *imtcbp = imgetcbp();
    immbnimglist_t *pMbnImglist =
                          (immbnimglist_t *)&imtcbp->imqdl.gobimgs2dwld;
    swi_char              *pImagePath = NULL;

    pImagePath = pMbnImglist->mbnimgs[pMbnImglist->nextimageidx].path;

    slmemcpy( imtcbp->imqdl.fullpathfilename,
              pImagePath,
              slstrlen( pImagePath ) + 1 );

    /* open the file for reading */
    if(FALSE == swi_ossdkfileopenread( &(imtcbp->imqdl.fp),
                                       imtcbp->imqdl.fullpathfilename ) )
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d failed to open file",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, 0 );
    }

    /* get the file size */
    if(FALSE == swi_ossdkfilesize( &(imtcbp->imqdl.fp),
                                   &( imtcbp->imqdl.fwImageSize ) ) )
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d File size error",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, 0 );
    }
    swi_ossdkfileclose( &(imtcbp->imqdl.fp) );
}

/*************
 *
 * Name:    imbuild_GetImgPref_req
 *
 * Purpose: builds the SSDP Get firmware prefrence request command
 *
 * Parms:   (IN/OUT)psGetFwPrefReq - pointer to Get Firmware Prefrence storage
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
void imbuild_GetImgPref_req (
    ssdp_get_image_preference_req_t *psGetFwPrefReq )
{
    /* Build SSDP Get Firmware Prefrence request */
    psGetFwPrefReq->command = SSDP_GETFWPREF_REQ_PKT_COMMAND;

    return;
}

/*************
 *
 * Name:    imGobiImageListGet
 *
 * Purpose: Provide the file paths to the Gobi firmware images that are to
 *          be downloaded to the Gobi device. The caller provides a list of
 *          images containing the image type and firmware ID for each image
 *          that is to be written to the device. For each image in the list, if
 *          an image with a matching image type and firmware ID is found in one
 *          of the subdirectories under the path stored in the QDL control,
 *          then the image type and corresponding path (including the file name)
 *          is recorded in the output list of images to be downloaded to the device.
 *
 * Parms:   (IN)pin     - image types and firmware IDs pertaining to the images
 *                        that are to be downloaded to the device.
 *          (OUT)plist  - image types and file paths for files to be downloaded
 *                        to the device.
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void
imGobiImageListGet(
    ssdp_get_image_preference_rsp_t *pin,
    immbnimglist_t *plist )
{
    /* sub directory entries that slglob should ignore */
    slstr_t ignore[3] = { {"."},{".."},{""} };

    /* local storage for slglob image path list */
    slstr_t pathlist[11];
    struct imtcb *imtcbp = imgetcbp();

    swi_int32 npaths;
    {
        slstr_t filter[11] = {  {"0"}, {"1"}, {"2"}, {"3"}, {"4"},
                                {"5"}, {"6"}, {"7"}, {"8"}, {"9"},
                                {"10"}  };

        npaths = slglob( imtcbp->imqdl.filepath,
                         filter,
                         ignore,
                         pathlist,
                         sizeof(pathlist)/sizeof(pathlist[0]) );
    }

    if( 0 == npaths)
    {
        dlLog0( &imtcbp->imdlcb,
                IMLOG_CLASSA,
                "0 entries in path matching filter; using input path" );
        /* in the case where the user supplied the entire path to where the
         * images are located, try the input path.
         */
        npaths = 1;
        slstrncpy(  pathlist[0].str,
                    imtcbp->imqdl.filepath,
                    strlen(imtcbp->imqdl.filepath) + 1 );
                            /* +1 for null character */
    }

    /* clear the image list */
    slmemset( (char *)plist, 0, sizeof(immbnimglist_t) );

    swi_int32 pathlistidx = 0;
    do{ /* traverse directory entries */

        struct swi_osdir dir;
        if( !swi_ossdk_opendir(pathlist[pathlistidx].str, &dir) )
        {
            dlLog0( &imtcbp->imdlcb,
                    IMLOG_CLASSA,
                    "failed to open directory" );
        }

        else
        {
            slstr_t filter[3] = { {"uqcn.mbn"},{"amss.mbn"},{""} };
            slstr_t list[3];
            swi_int32 icnt; /* count of UQCN and AMSS files in subdirectory */
            /* look for UQCN and AMSS files in sub directories */
            icnt = slglob(  pathlist[pathlistidx].str,
                            filter,
                            ignore,
                            list,
                            sizeof(list)/sizeof(list[0]) - 1 );

            /* search local directory for images corresponding to those in the
             * preferred image preference list returned from the Gobi device.
             */
            while(icnt--)
            {
                /* parsed local mbn image info */
                struct im_image_info_s gobimg;

                /* extract local image information */
                gobimg.pimgpath = list[icnt].str;
                if( IMERRNONE != imuser_image_info_get(&gobimg) )
                {
                    /* next directory entry */
                    continue;
                }

                /* compare local image information to the list of images
                 * in the device preferred image preference list.
                 */
                int i;
                for( i = 0 ; i < pin->total_entries_in_list ; i++ )
                {
                    /* match the firmware image ID */
                    if( 0 == slstrncmp( (char *)pin->ssdp_img_pref[i].image_identifier,
                                        gobimg.imginfo.gobiinfo.imgidstr,
                                        SSDP_IMAGE_ID_LEN ) )
                    {
                        /* match the image type (UQCN or AMSS) */
                        if( pin->ssdp_img_pref[i].image_type !=
                            gobimg.imginfo.gobiinfo.type )
                        {
                            dlLog0( &imtcbp->imdlcb,
                                    IMLOG_CLASSA,
                                    "Image ID match for non-matching image types" );

                            /* no match - next entry in preferred image list */
                            continue;
                        }

                        /* match found - ignore duplicate images */
                        swi_bool duplicate = 0;
                        swi_uint8 j = plist->totalimages;
                        while( j-- && !duplicate )
                        {
                            if( 0 == slstrncmp( (char *)pin->ssdp_img_pref[i].image_identifier,
                                                plist->mbnimgs[j].imageid,
                                                SSDP_IMAGE_ID_LEN )             /* Image ID matches */
                                &&                                              /* AND */
                                (   pin->ssdp_img_pref[i].image_type ==
                                    plist->mbnimgs[j].type )   /* Image Type matches */
                              )
                            {
                                /* duplicate image found */
                                duplicate = 1;
                            }
                        }

                        if( duplicate )
                        {
                            /* next entry in preferred image list */
                            continue;
                        }

                        /* match found - update image list */
                        slstrncpy(  plist->mbnimgs[plist->totalimages].path,        /* path */
                                    gobimg.pimgpath,
                                    slstrlen(gobimg.pimgpath) + 1 );

                        slmemcpy(   &plist->mbnimgs[plist->totalimages].imageid,    /* image id */
                                    pin->ssdp_img_pref[i].image_identifier,
                                    MBN_IMG_ID_STR_LEN );
                                                                                    /* image type */
                        plist->mbnimgs[plist->totalimages].type =  pin->ssdp_img_pref[i].image_type;

                        /* update number of images to be downloaded to the device */
                        plist->totalimages++;
                    }
                }
            }
            if( !swi_ossdk_closedir(&dir) )
            {
                dlLog0( &imtcbp->imdlcb,
                        IMLOG_CLASSA,
                        "failed to close directory" );
            }
        }
        pathlistidx++;
    }while( pathlistidx < npaths );
}

/***************************
 * state machine functions *
 ***************************/

/*************
 *
 * Name:    imGobiTaskShutdown
 *
 * Purpose: Shuts down Gobi IM task
 *
 * Parms:   (IN)imrxdatap    - not used
 *
 * Return:  Does not return, the thread terminates
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void imGobiTaskShutdown( void *pData )
{
    struct imtcb *imtcbp = imgetcbp();

    UNUSEDPARAM( pData );

#ifdef IMDBGFLG
    dlLog1( &imtcbp->imdlcb,
            IMLOG_CLASSC,
            "imGobiShutdown :State - %lu\n",
            (swi_uint32)imtcbp->imsmp->state );
#endif /* IMDBGFLG */

    /* clear firmware download path */
    slmemset( imtcbp->imqdl.filepath,
              0,
              sizeof(imtcbp->imqdl.filepath) );

    swi_ossdkthreadterminate();
}

/*************
 *
 * Name:    imerror
 *
 * Purpose: Log error and terminate download task if ???
 *
 * Parms:   (IN)imrxdatap
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void imerror( void *imrxdatap )
{
    struct imtcb      *imtcbp = imgetcbp();
    struct sdp_packet *psdp = (struct sdp_packet *)imrxdatap;
    swi_uint32        err;
    swi_uint8         *errp = (swi_uint8 *)imrxdatap+1;

    if (psdp->phdr.command == 0x0d)
    {
        err = piget32(&errp);

        dlLog3( &imtcbp->imdlcb,
                IMLOG_CLASSC,
               "error, st:%d, error:0x%08x, %s\n",
               (swi_uint32)imtcbp->imsmp->state,
               (swi_uint32)err,
               (swi_uint32)(psdp+5) );

        /* Shut down the GOBI IM task */
        imGobiTaskShutdown( NULL );
    }
    return;
}

/*************
 *
 * Name:    imGobiTransSendReq
 *
 * Purpose: Send a request to the transaction state machine (SMTR).
 *
 * Parms:   (IN)pData   - data to be sent to the device
 *          dataLen     - length of data
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void imGobiTransSendReq(
    swi_uint8  *pData,
    swi_uint32 dataLen,
    void (*sendpktfn)( swi_uint8 *txbufp,
                       swi_uint32 pktsize ) )
{
    struct imtcb *imtcbp = imgetcbp();

    /*
     * Intialize Task control block state machine with the transaction
     * state machine.
     */
    imtcbp->imsmp = &smtr;

    /*
     * Release the memory allocated at the time of recieving the packet from
     * HD. Memory allocated to recieve packet is released here as we are
     * sure now that the received packet is processed
     */
    if (imtcbp->imhdmemrelp != NULL)
    {
       mmbufrel(imtcbp->imsmp->udata);
    }

    /* Intialize the transaction state machine with the recieved data */
    datapkt.datalen = dataLen;
    datapkt.pdata = pData;
    datapkt.sendpktfn = sendpktfn;
    imtcbp->imsmp->udata = (void *)&datapkt;

#ifdef IMDBGFLG
    dlLog1( &imtcbp->imdlcb,
            IMLOG_CLASSC,
            "imGobiTransSendReq :State - %lu\n",
            (swi_uint32)imtcbp->imsmp->state );
#endif /* IMDBGFLG */

    /* Send the messsage to the imgobitask */
    imGobiQevt( ev_TRANS_TX_REQ,
                NULL,
                0,
                NULL );
}

/*************
 *
 * Name:    imGobiFwDldSendReq
 *
 * Purpose: Send a request to the firmware download state machine (SMFD)
 *
 * Parms:   (IN)pData   - request data
 *          dataLen     - length of request data
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void imGobiFwDldSendReq(
    swi_uint8  *pData,
    swi_uint32 dataLen )
{
    struct imtcb *imtcbp = imgetcbp();

    UNUSEDPARAM( pData );
    UNUSEDPARAM( dataLen );

    /* configure the task control block to use SMFD */
    imtcbp->imsmp = &smfd;

#ifdef IMDBGFLG
    dlLog1( &imtcbp->imdlcb,
            IMLOG_CLASSC,
            "imGobiFwDldSendReq :State - %lu\n",
            (swi_uint32)imtcbp->imsmp->state );
#endif /* IMDBGFLG */

    /* send intertask messsage */
    imGobiQevt( ev_FWDLD_START,
                NULL,
                0,
                NULL );
}

/*************
 *
 * Name:    imGobiFwDldRspCbk
 *
 * Purpose: Send a response to the firmware download state machine (SMT).
 *
 * Parms:   ev          - state machine event
 *          (IN)pData   - data to be sent for the event
 *          dataLen     - length of the data to be send
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void imGobiFwDldRspCbk(
    enum imfdevent_e ev,
    void             *pData )
{
    struct imtcb      *imtcbp = imgetcbp();
    struct sdp_packet *psdp = (struct sdp_packet *)pData;
    swi_uint8         event;

    /* configure the task control block to use SMFD */
    imtcbp->imsmp = &smfd;
    imtcbp->imsmp->udata = pData;

#ifdef IMDBGFLG
    dlLog2( &imtcbp->imdlcb,
            IMLOG_CLASSC,
            "imGobiFwDldRspCbk :State - %lu,WP %d\n",
            (swi_uint32)imtcbp->imsmp->state,
            (swi_uint32)imtcbp->imqdl.numwrites );
#endif /* IMDBGFLG */

    if (psdp->phdr.command == 0x0d)
    {
        /* error command received from the modem, inform FwDld SM */
        event =  ev_FWDLD_ERROR;
    }
    else if( imtcbp->imsmp->state == st_SMFD_WAIT_UWRTRSP )
    {
        /* firmware downloading is started, send next unframed pkt */
        event = ev_FWDLD_UWRTNXT;
    }
    else
    {
        event = ev;
    }

    /* send intertask message */
    imGobiQevt( event,
                NULL,
                0,
                NULL );
}

/*************
 *
 * Name:    imGobiTaskRspCbk
 *
 * Purpose: Send a response to the task state machine (SMT)
 *
 * Parms:   ev          - state machine event
 *          (IN)pData   - data to be sent for the event
 *          dataLen     - length of the data to be send
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void imGobiTaskRspCbk(
    enum imtevent_e ev,
    swi_uint8       *pData,
    swi_uint32      dataLen )
{
    struct imtcb *imtcbp = imgetcbp();

    /* configure the task control block to use SMT */
    imtcbp->imsmp = &smt;

#ifdef IMDBGFLG
    dlLog1( &imtcbp->imdlcb,
            IMLOG_CLASSC,
            "imGobiTaskRspCbk :State - %lu\n",
            (swi_uint32)imtcbp->imsmp->state );
#endif /* IMDBGFLG */

    /* send intertask messsage */
    imGobiQevt( ev,
                pData,
                dataLen,
                NULL);
}

/*************
 *
 * Name:    imGobiFwDldError
 *
 * Purpose: Handle firmware download state machine sequencing errors
 *
 * Parms:   (IN)imrxdatap
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void imGobiFwDldError( void *imrxdatap )
{
    struct imtcb *imtcbp = imgetcbp();

    UNUSEDPARAM( imrxdatap );

#ifdef IMDBGFLG
    dlLog1( &imtcbp->imdlcb,
            IMLOG_CLASSC,
            "imGobiFwDldError :State - %lu\n",
            (swi_uint32)imtcbp->imsmp->state );
#endif /* IMDBGFLG */

    /* !0 = Inform device that QDL port is being closed, ie, reset */
    CloseQDLPort( !0 );

    /* release DS buffer used for receiving responses from modem */
    mmbufrel(imtcbp->imdsmemrelp);
    dlLog1( &imtcbp->imdlcb,
            IMLOG_CLASSC,
            "Releasing DS buffer used for receiving modem response 0x%08x",
            (swi_uint32)imtcbp->imdsmemrelp );

    imGobiTaskRspCbk( ev_TASK_DLD_FAIL,
                      NULL,
                      0 );
}

/*************
 *
 * Name:    imGobiTransRxRsp
 *
 * Purpose: Transaction state machine callback function
 *
 * Parms:   (IN)pData
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void imGobiTransRxRsp( void *imrxdatap )
{
    struct imtcb *imtcbp = imgetcbp();
    struct imdatapkt *pdatapkt =  (struct imdatapkt *)imrxdatap;

#ifdef IMDBGFLG
    dlLog2( &imtcbp->imdlcb,
            IMLOG_CLASSC,
            "imGobiTransRxRsp :State - %lu,addr = %xH\n",
            (swi_uint32)imtcbp->imsmp->state,
            (swi_uint32)pdatapkt->pdata );
#endif /* IMDBGFLG */

    /* unframe the received response */
    imunframe ( pdatapkt->pdata, pdatapkt->datalen );

    /* Release the im rx pool buffer containing the framed data as the
     * unframed data has now been copied to the hd buffer
     */
    mmbufrel( pdatapkt->pdata );

    /* inform the FwDld SM about the received response */
    imGobiFwDldRspCbk( ev_FWDLD_RSPRX,
                       imtcbp->imsmp->udata );
}

/*************
 *
 * Name:    imGobiTransTxReq
 *
 * Purpose: Send a request to the transaction state machine
 *
 * Parms:   (IN)imrxdatap    - pointer to the message to be sent to modem
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void imGobiTransTxReq( void *imrxdatap )
{
    struct imdatapkt *pdatapkt =  (struct imdatapkt *)imrxdatap;

#ifdef IMDBGFLG
    struct imtcb *imtcbp = imgetcbp();
    dlLog1( &imtcbp->imdlcb,
            IMLOG_CLASSC,
            "imGobiTransTxReq :State - %lu\n",
            (swi_uint32)imtcbp->imsmp->state );
#endif /* IMDBFLG */

    /* Send the message to the modem */
    pdatapkt->sendpktfn ( pdatapkt->pdata,
                          pdatapkt->datalen );

    /* free memory allocated for IM request */
    mmbufrel( pdatapkt->pdata );
}

/*************
 *
 * Name:    wrtGobiQdlImgBlck
 *
 * Purpose: Writes the specified image block to the device.
 *
 * Parms:   (IN)sequenceNumber    - Squence no. of the block to be send
 *          (IN)blockSize         - Length of the block
 *          (IN)pImageBlock       - Pointer to buffer containing data to be sent
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void wrtGobiQdlImgBlck(
    USHORT  sequenceNumber,
    ULONG   blockSize,
    BYTE    *pImageBlock )
{
    struct imtcb              *imtcbp = imgetcbp();
    ssdp_unframed_write_req_s sUnframedWriteReq;
    swi_uint32                packetsize;
    swi_uint8                 *txbufp;

   /* build unframed wrire request */
   imbuild_unframed_write_req ( &sUnframedWriteReq,
                                sequenceNumber,
                                blockSize,
                                pImageBlock );

    /* Get buffer from the dl pool */
    txbufp = (swi_uint8 *) mmbufget( &imtcbp->imdlpool );
    packetsize = imssdp_pack_request ( (struct sdp_packet *)&sUnframedWriteReq,
                                       txbufp );
#ifdef IMDBGFLG
    dlLog2( &imtcbp->imdlcb,
            IMLOG_CLASSC,
            "wrtGobiQdlImgBlck :blockSize - %d :packetsize - %d\n",
            (swi_uint32)blockSize,
            (swi_uint32)packetsize );
#endif /* IMDBGFLG */

    /* Start Transaction state machine to send data to the modem */
    imGobiTransSendReq( txbufp,
                        packetsize,
                        imdssend );

    return;
}

/*************
 *
 * Name:    imGobiSendHello
 *
 * Purpose: Send Hello request packet to the device
 *
 * Parms:   (IN)pdata     - Pointer to response message received from modem
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void imGobiSendHello( void *imrxdatap )
{
    struct imtcb     *imtcbp = imgetcbp();
    swi_uint8        *txbufp;
    ssdp_hello_req_s sHelloReq;
    swi_uint32       packetsize;

    UNUSEDPARAM( imrxdatap );

    /* Build hello request */
    imbuild_hello_req( &sHelloReq );

    /* Get buffer from the tx pool */
    txbufp = (swi_uint8 *) mmbufget( &imtcbp->imtxpool );
    packetsize = imssdp_hello_pack( (void *)&sHelloReq,
                                    txbufp );

#ifdef IMDBGFLG
    dlLog1( &imtcbp->imdlcb,
            IMLOG_CLASSC,
            "imGobiSendHello :State - %lu\n",
            (swi_uint32)imtcbp->imsmp->state );
#endif /* IMDBGFLG */

    /* Start Transaction state machine */
    imGobiTransSendReq( txbufp,
                        packetsize,
                        imsendframed );
}

/*************
 *
 * Name:    imGobiSendGetImgPref
 *
 * Purpose: Send Get Image Prefrence request packet to the device
 *
 * Parms:   (IN)pdata     - Pointer to response message received from modem
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void imGobiSendGetImgPref( void *imrxdatap )
{
    struct imtcb                    *imtcbp = imgetcbp();
    swi_uint8                       *txbufp;
    struct sdp_packet               *psdp = (struct sdp_packet *)imrxdatap;
    ssdp_get_image_preference_req_t sGetImgPrefReq;
    swi_uint32                      packetsize;

#ifdef IMDBGFLG
    dlLog1( &imtcbp->imdlcb,
            IMLOG_CLASSC,
            "imGobiSendGetImgPref :State - %lu\n",
            (swi_uint32)imtcbp->imsmp->state );
#endif /* IMDBGFLG */

    /*
     * if we got here, it means a Hello response came back, extract info from
     * the response
     */
    imtcbp->imrxdatap = (swi_uint8*)psdp;
    OpenQDLPort ( imtcbp->imqdl.portID,
                  imtcbp->imqdl.bBARMode,
                  &(imtcbp->imqdl.MajorVersion),
                  &(imtcbp->imqdl.MinorVersion) );

    /* Build Get Image Prefrence request */
    imbuild_GetImgPref_req( &sGetImgPrefReq );

    /* Get buffer from the tx pool */
    txbufp = (swi_uint8 *) mmbufget( &imtcbp->imtxpool );
    packetsize = imssdp_get_image_preference_pack( (void *)&sGetImgPrefReq,
                                                   txbufp );

    /* Start Transaction state machine */
    imGobiTransSendReq( txbufp,
                        packetsize,
                        imsendframed );
}

/*************
 *
 * Name:    imGobiSendFwImg
 *
 * Purpose: Get Image Prefrence respose and search for the path of the images
 *          to be downloaded.
 *
 * Parms:   (IN)pdata     - Pointer to response message received from modem
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void imGobiSendFwImg( void *imrxdatap )
{
    ssdp_get_image_preference_rsp_t sGetImgPrefRsp;
    struct imtcb                    *imtcbp = imgetcbp();
    swi_uint8                       event;

    /* Get the results of the image prefrence request */
    slmemset( (char *)&sGetImgPrefRsp, 0, sizeof(sGetImgPrefRsp) );
    imssdp_get_image_preference_unpack( (swi_uint8 *)imrxdatap,
                                        &sGetImgPrefRsp );

    /* Get the complete for each image requested by modem */
    imGobiImageListGet( &sGetImgPrefRsp, &imtcbp->imqdl.gobimgs2dwld );

    /* check if we have image to download ,if no inform the firmware dwl SM */
    if( 0 == imtcbp->imqdl.gobimgs2dwld.totalimages )
    {
        event = ev_FWDLD_ERROR;
        dlLog0( &imtcbp->imdlcb,
                IMLOG_CLASSC,
                "imGobiSendFwImg : No Matching IMAGE found\n" );
    }
    else
    {
        /* call the firmware state machine to start download */
        event = ev_FWDLD_RSPRX;
    }

#ifdef IMDBGFLG
    dlLog2( &imtcbp->imdlcb,
            IMLOG_CLASSC,
            "imGobiSendFwImg :State - %lu, event - %d\n",
            (swi_uint32)imtcbp->imsmp->state,
            (swi_uint32)event );
#endif /* IMDBGFLG */

    /* send intertask message */
    imGobiQevt( event,
                NULL,
                0,
                NULL );
}

/*************
 *
 * Name:    imGobiSendOUfPkt
 *
 * Purpose: Send Open Unframed request packet to the device
 *
 * Parms:   (IN)pdata     - Pointer to response message received from modem
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void imGobiSendOUfPkt( void *imrxdatap )
{
    struct imtcb             *imtcbp = imgetcbp();
    ssdp_open_unframed_req_s sOpenUnframedReq;
    swi_uint32               packetsize;
    swi_uint8                *txbufp;

    UNUSEDPARAM( imrxdatap );

    /* Get the download file into memory */
    imgobipreparefile();

    /* Build Open Unframed Request */
    imgobibld_open_unframed_pkt_req ( &sOpenUnframedReq );

    /* Get buffer from the tx pool */
    txbufp = (swi_uint8 *) mmbufget( &imtcbp->imtxpool );
    if ( NULL == txbufp )
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d No memory in TX pool",
                 (char *)__func__, __LINE__);
        erAbort (errmsg,0);
    }
    packetsize = imssdp_pack_request ( (struct sdp_packet *)&sOpenUnframedReq,
                                       txbufp );

#ifdef IMDBGFLG
    dlLog1( &imtcbp->imdlcb,
            IMLOG_CLASSC,
            "imGobiSendOUfPkt :State - %lu\n",
            (swi_uint32)imtcbp->imsmp->state );
#endif /* IMDBGFLG */

    /* Start Transaction state machine to send data to the modem */
    imGobiTransSendReq( txbufp,
                        packetsize,
                        imsendframed );
}

/*************
 *
 * Name:    imGobiSendUfWrite
 *
 * Purpose: Send Unframed Write packet to the device
 *
 * Parms:   (IN)pdata     - Pointer to response message received from modem
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   This function writes the first block of the firmware image to the
 *          modem.
 *
 **************/
local void imGobiSendUfWrite( void *imrxdatap )
{
    struct imtcb           *imtcbp = imgetcbp();
    struct swi_osfilestats fileinfo;
    immbnimglist_t         *pMbnImglist =
                                 (immbnimglist_t *)&imtcbp->imqdl.gobimgs2dwld;
    ULONG                  rCode;

#ifdef IMDBGFLG
    dlLog1( &imtcbp->imdlcb,
            IMLOG_CLASSC,
            "imGobiSendUfWrite :State - %lu\n",
            (swi_uint32)imtcbp->imsmp->state );
#endif /* IMDBGFLG */

    imtcbp->imrxdatap = (swi_uint8 *)imrxdatap;
    imtcbp->imqdl.ImageType =
        pMbnImglist->mbnimgs[pMbnImglist->nextimageidx].type;

    rCode = PrepareQDLImageWrite( imtcbp->imqdl.ImageType,
                                  imtcbp->imqdl.fwImageSize,
                                  &(imtcbp->imqdl.BlockSize) );

    if ( eQCWWAN_ERR_NONE != rCode )
    {
        swi_uint8 event;
        if( SSDP_OPEN_UNFRAMED_IMAGE_NT_REQUIRED == rCode )
        {
            /* If 0x07 is received for open unframed write,image is already
             * there in the modem, continue downloading the next image.
             */
            pMbnImglist->nextimageidx++;
            event = ev_FWDLD_RSPRX;
            if( pMbnImglist->totalimages > pMbnImglist->nextimageidx )
            {
                /* Send the open unframed write request for the next image */
                smfd.state = st_SMFD_SEND_FWIMG;
            }
            else
            {
                /* No image to download, shut down the GOBI IM task */
                smfd.state = st_SMFD_WAIT_UWRTRSP;
            }

        }
        else
        {
             event = ev_FWDLD_ERROR;
        }

        dlLog2( &imtcbp->imdlcb,
               IMLOG_CLASSC,
               "Send UF write Resp, st:%i, Status:%i\n",
               (swi_uint32)imtcbp->imsmp->state,
               (swi_uint32)rCode );

        /* Send the messsage to the imGobitask : switch to error state */
        imGobiQevt( event,
                    NULL,
                    0,
                    NULL);
        return;
    }

    /* TBD - Check block size */
    imtcbp->imqdl.numwrites =
                        (imtcbp->imqdl.fwImageSize)/(imtcbp->imqdl.chunkSize);
    imtcbp->imqdl.remainder =
                        (imtcbp->imqdl.fwImageSize)%(imtcbp->imqdl.chunkSize);

    if (imtcbp->imqdl.remainder != 0)
    {
        imtcbp->imqdl.numwrites++;
    }

    ULONG blockSize = IM_CHUNK_SIZE;
    if (imtcbp->imqdl.numwrites == 1)
    {
        blockSize = imtcbp->imqdl.remainder;
    }

#ifdef IMDBGFLG
    dlLog( &imtcbp->imdlcb,
           IMLOG_CLASSC,
           "imGobiSendUfWrite: SUFW Rm %d,NW %d,BSZ %d\n",
           (swi_uint32)imtcbp->imqdl.remainder,
           (swi_uint32)imtcbp->imqdl.numwrites,
           (swi_uint32)blockSize,
           (swi_uint32)NULL );
#endif /* IMDBGFLG */

    /* Map the first block and write it to modem */
    imtcbp->imqdl.nextchunkoffset = 0; /* start from beginning */
    fileinfo.filesize             = IM_CHUNK_SIZE;
    fileinfo.vmapfileoffset       = 0;
    if ( FALSE == swi_ossdk_mmapro(imtcbp->imqdl.fullpathfilename, &fileinfo) )
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d File memory mapping failed",
                 (char *)__func__, __LINE__);
        erAbort (errmsg,0);
    }

    imtcbp->imqdl.pImageBlock = (swi_char *)fileinfo.pvmap;

    imtcbp->imqdl.sequenceNumber = 0;
    wrtGobiQdlImgBlck( imtcbp->imqdl.sequenceNumber++,
                       blockSize,
                       (BYTE*)imtcbp->imqdl.pImageBlock );

    if( FALSE == swi_ossdk_umapfile( &fileinfo ) )
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d File memory unmapping failed",
                 (char *)__func__, __LINE__);
        erAbort (errmsg,0);
    }

    imtcbp->imqdl.numwrites--;
    imtcbp->imqdl.nextchunkoffset += blockSize;
    imtcbp->imqdl.byteswritten = blockSize;

    return;
}

/*************
 *
 * Name:    imGobiSendDone
 *
 * Purpose: Send Done packet to the device
 *
 * Parms:   (IN)imrxdatap    - pointer to response message from modem
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void imGobiSendDone( void *pData )
{
    struct imtcb    *imtcbp = imgetcbp();
    ssdp_done_req_s sDoneReq;
    swi_uint32      packetsize;
    swi_uint8       *txbufp;

    UNUSEDPARAM( pData );

#ifdef IMDBGFLG
    dlLog1( &imtcbp->imdlcb,
            IMLOG_CLASSC,
            "imGobiSendDone :State - %lu\n",
            (swi_uint32)imtcbp->imsmp->state );
#endif /* IMDBGFLG */

    /* Create the Done Request */
    sDoneReq.command = SSDP_DONE_REQ_COMMAND;

    /* Get buffer from the tx pool */
    txbufp = (swi_uint8 *) mmbufget( &imtcbp->imtxpool );
    packetsize = imssdp_pack_request ( (struct sdp_packet *)&sDoneReq,
                                       txbufp );

    /* Start Transaction state machine to send data to the modem */
    imGobiTransSendReq( txbufp,
                        packetsize,
                        imsendframed );
}

/*************
 *
 * Name:    imGobiSendDoneFinal
 *
 * Purpose: Send Done Final packet to the device
 *
 * Parms:   (IN)pdata     - Pointer to response message received from modem
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void imGobiSendDoneFinal( void *imrxdatap )
{
    struct imtcb      *imtcbp = imgetcbp();
    enum eQCWWANError rCode;
    swi_uint8         event;
/* RILSTART */
    swi_uint8         status;
/* RILSTOP */

#ifdef IMDBGFLG
    dlLog1( &imtcbp->imdlcb,
            IMLOG_CLASSC,
            "imGobiSendDoneFinal :State - %lu\n",
            (swi_uint32)imtcbp->imsmp->state );
#endif /* IMDBGFLG */

    imtcbp->imrxdatap = (swi_uint8 *)imrxdatap;
    rCode = ValidateQDLImages( &(imtcbp->imqdl.ImageType) );

    if( eQCWWAN_ERR_NONE != rCode )
    {
        dlLog0( &imtcbp->imdlcb,
                IMLOG_CLASSC,
                "imGobiSendDoneFinal :IMAGE DLD FAILED!!!\n" );
        event = ev_TASK_DLD_FAIL;
    }
    else
    {
        dlLog0( &imtcbp->imdlcb,
                IMLOG_CLASSC,
                "imGobiSendDoneFinal :IMAGE DLD SUCCESS!!!\n" );
        event = ev_TASK_DLD_COMP;
    }

    /* !0 = Inform device that QDL port is being closed, ie, reset */
    CloseQDLPort( !0 );

    /* release DS buffer used for receiving responses from modem */
    mmbufrel(imtcbp->imdsmemrelp);
    dlLog1( &imtcbp->imdlcb,
            IMLOG_CLASSC,
            "Releasing DS buffer used for receiving modem response 0x%08x",
            (swi_uint32)imtcbp->imdsmemrelp );
    /*
     * Being here means firmware download is completed. Do not call transaction
     * state machine. Send the done response final command directly to the
     * modem. Initiate main task state machine to inform about firmware
     * download status.
     */
    imGobiTaskRspCbk( event,
                      NULL,
                      0 );

/* RILSTART */
    if (ev_TASK_DLD_FAIL == event)
    {
        /* set to non-zero value if download failed */
        status = 1;
    }
    else if (ev_TASK_DLD_COMP == event)
    {
        /* set to zero indicate download success */
        status = 0;        
    }
    /* set firmware download status */
    qmSetFwDownloadStatus(status);
/* RILSTOP */

}

/*************
 *
 * Name:    imGobiSendNextUfWrite
 *
 * Purpose: This function intiates the Transaction SM to send Next Unframed
 *          Write packet to the modem.
 *
 * Parms:   (IN)pdata     - Pointer to response message received from modem
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void imGobiSendNextUfWrite( void *imrxdatap )
{
    struct imtcb              *imtcbp = imgetcbp();
    ssdp_unframed_write_rsp_s sUnframedWriteRsp;
    struct swi_osfilestats    fileinfo;
    immbnimglist_t            *pMbnImglist =
                                 (immbnimglist_t *)&imtcbp->imqdl.gobimgs2dwld;

#ifdef IMDBGFLG
    dlLog1( &imtcbp->imdlcb,
            IMLOG_CLASSC,
            "imGobiSendNextUfWrite :State - %lu\n",
            (swi_uint32)imtcbp->imsmp->state );
#endif /* IMDBGFLG */

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

    /* Get Unframed Write response info */
    imssdp_unframed_write_unpack( (swi_uint8*)imrxdatap, &sUnframedWriteRsp);

    if (sUnframedWriteRsp.status != SSDP_UNFRAMED_WRITE_DATA_RECEIVED_OK)
    {
        dlLog2( &imtcbp->imdlcb,
                IMLOG_CLASSC,
                "Unframed Write response error, st:%i, Status:0x%02x\n",
                (swi_uint32)imtcbp->imsmp->state,
                (swi_uint32)sUnframedWriteRsp.status );

        /* Send the messsage to the imGobitask : switch to error state */
        imGobiQevt( ev_FWDLD_ERROR,
                    NULL,
                    0,
                    NULL );
        return;
    }

    /* If download is completed, move to the next step */
    if ( 0 == imtcbp->imqdl.numwrites )
    {
        pMbnImglist->nextimageidx++;
        if( pMbnImglist->totalimages > pMbnImglist->nextimageidx )
        {
            /* Send the open unframed write request for the next image */
            smfd.state = st_SMFD_SEND_FWIMG;
        }

        /* Send the messsage to the imGobitask : switch to next state */
        imGobiQevt( ev_FWDLD_RSPRX,
                    NULL,
                    0,
                    NULL );
        return;
    }

    ULONG blockSize = IM_CHUNK_SIZE;
    fileinfo.filesize = IM_CHUNK_SIZE;
    if ( 1 == imtcbp->imqdl.numwrites )
    {
        /* Only map remainder, not  beyond file size boundary */
        blockSize         = imtcbp->imqdl.remainder;
        fileinfo.filesize = blockSize;
    }

    fileinfo.vmapfileoffset = imtcbp->imqdl.nextchunkoffset;
    if ( FALSE == swi_ossdk_mmapro(imtcbp->imqdl.fullpathfilename, &fileinfo) )
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d File memory mapping failed",
                 (char *)__func__, __LINE__);
        erAbort (errmsg,0);
    }
    imtcbp->imqdl.pImageBlock = (swi_char *)fileinfo.pvmap;

    /* write the next block */
    wrtGobiQdlImgBlck( imtcbp->imqdl.sequenceNumber++,
                       blockSize,
                       (BYTE*)imtcbp->imqdl.pImageBlock );

    if( FALSE == swi_ossdk_umapfile( &fileinfo ) )
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d File memory unmapping failed",
                 (char *)__func__, __LINE__);
        erAbort (errmsg,0);
    }

    imtcbp->imqdl.nextchunkoffset += blockSize;
    (imtcbp->imqdl.numwrites)--;
    imtcbp->imqdl.byteswritten += blockSize;

    return;
}

/*************
 *
 * Name:    imGobiFwDldStart
 *
 * Purpose: This function starts the Firmware Download state machine.
 *
 * Parms:   (IN)imrxdatap    - Not used
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void imGobiFwDldStart( void *imrxdatap )
{
#ifdef IMDBGFLG
    struct imtcb *imtcbp = imgetcbp();
    dlLog1( &imtcbp->imdlcb,
            IMLOG_CLASSC,
            "imGobiFwDldStart :State - %lu\n",
            (swi_uint32)imtcbp->imsmp->state );
#endif /* IMDBGFLG */

    UNUSEDPARAM( imrxdatap );

    /* Start Firmware download state machine */
    imGobiFwDldSendReq( NULL, 0);
}

/*************
 *
 * Name:    imstttr
 *
 * Purpose: State transition table for Transaction state machine.
 *
 * Members: one for each state
 *
 * Notes:   none
 *
 **************/
local swi_uint8 imstttr[st_SMTR_NUM_ACTIVE_STATES][ev_TRANS_MAX_EVENT] =
{
           /* ev_TRANS_TX_REQ   ev_TRANS_RX_RSP */
/* w4req */ { st_SMTR_WAIT_RSP, st_SMT_IGNR },
/* w4rsp */ { st_SMT_IGNR,      st_SMTR_WAIT_REQ },
};

/*************
 *
 * Name:    imsattr
 *
 * Purpose: State Action table for Transaction state machine.
 *
 * Members: one for each state
 *
 * Notes:   none
 *
 **************/
local void(*imsattr[st_SMTR_NUM_ACTIVE_STATES][ev_TRANS_MAX_EVENT])(void *) =
{
           /* ev_TRANS_TX_REQ   ev_TRANS_RX_RSP */
/* w4req */ { imGobiTransTxReq, imerror },
/* w4rsp */ { imerror,          imGobiTransRxRsp },
};

/*************
 *
 * Name:    imsttfd
 *
 * Purpose: State transition table for Firmware Download state machine.
 *
 * Members: one for each state
 *
 * Notes:   none
 *
 **************/
local swi_uint8 imsttfd[st_SMFD_NUM_ACTIVE_STATES][ev_FWDLD_MAX_EVENT] =
{
               /* ev_FWDLD_STRT          ev_FWDLD_RSPRX          ev_FWDLD_UWRTNXT      ev_FWDLD_ERROR */
/* idle */      { st_SMFD_WAIT_HELLORSP, st_SMFD_IGNR,           st_SMFD_IGNR,         st_SMFD_ERROR },
/* w4hrsp */    { st_SMFD_IGNR,          st_SMFD_WAIT_FWPREFRSP, st_SMFD_IGNR,         st_SMFD_ERROR },
/* w4imgprsp */ { st_SMFD_IGNR,          st_SMFD_SEND_FWIMG,     st_SMFD_IGNR,         st_SMFD_ERROR },
/* w4sendimg */ { st_SMFD_IGNR,          st_SMFD_WAIT_OPENUNF,   st_SMFD_IGNR,         st_SMFD_ERROR },
/* w4oursp */   { st_SMFD_IGNR,          st_SMFD_WAIT_UWRTRSP,   st_SMFD_IGNR,         st_SMFD_ERROR },
/* w4uwrsp */   { st_SMFD_IGNR,          st_SMFD_WAIT_DONERSP,   st_SMFD_WAIT_UWRTRSP, st_SMFD_ERROR },
/* w4dnrsp */   { st_SMFD_IGNR,          st_SMFD_IDLE,           st_SMFD_IGNR,         st_SMFD_ERROR },
/* dlderror */  { st_SMFD_IGNR,          st_SMFD_IGNR,           st_SMFD_IGNR,         st_SMFD_ERROR }
};

/*************
 *
 * Name:    imsatfd
 *
 * Purpose: State Action table for Firmware Download state machine.
 *
 * Members:
 *
 * Notes:   none
 *
 **************/
local void(*imsatfd[st_SMFD_NUM_ACTIVE_STATES][ev_FWDLD_MAX_EVENT])(void *) =
{
               /* ev_FWDLD_STRT     ev_FWDLD_RSPRX        ev_FWDLD_UWRTNXT       ev_FWDLD_ERROR */
/* idle */      { imGobiSendHello,  imGobiFwDldError,     imGobiFwDldError,      imGobiFwDldError },
/* w4hrsp */    { imGobiFwDldError, imGobiSendGetImgPref, imGobiFwDldError,      imGobiFwDldError },
/* w4imgprsp */ { imGobiFwDldError, imGobiSendFwImg,      imGobiFwDldError,      imGobiFwDldError },
/* w4sendimg */ { imGobiFwDldError, imGobiSendOUfPkt,     imGobiFwDldError,      imGobiFwDldError },
/* w4oursp */   { imGobiFwDldError, imGobiSendUfWrite,    imGobiFwDldError,      imGobiFwDldError },
/* w4uwrsp */   { imGobiFwDldError, imGobiSendDone,       imGobiSendNextUfWrite, imGobiFwDldError },
/* w4dnrsp */   { imGobiFwDldError, imGobiSendDoneFinal,  imGobiFwDldError,      imGobiFwDldError },
/* dlderror */  { imGobiFwDldError, imGobiFwDldError,     imGobiFwDldError,      imGobiFwDldError }
};

/*************
 *
 * Name:    imsttt
 *
 * Purpose: State transition table for the Task state machine
 *
 * Members:
 *
 * Notes:   none
 *
 **************/
local swi_uint8 imsttt[st_SMT_NUM_ACTIVE_STATES][ev_TASK_MAX_EVENT] =
{
               /* ev_TASK_DLD_STRT  ev_TASK_DLD_COMP    ev_TASK_DLD_FAIL */
/* idle */      { st_SMT_FWDLDNG,   st_SMTR_IGNR,       st_SMTR_IGNR },
/* fwdld */     { st_SMTR_IGNR,     st_SMT_SHUTDWN,     st_SMT_SHUTDWN },
/* shutdwn */   { st_SMTR_IGNR,     st_SMTR_IGNR,       st_SMTR_IGNR },
};

/*************
 *
 * Name:    imsatt
 *
 * Purpose: State Action table for the Task state machine
 *
 * Members:
 *
 * Notes:   none
 *
 **************/
local void(*imsatt[st_SMT_NUM_ACTIVE_STATES][ev_TASK_MAX_EVENT])(void *) =
{
               /* ev_TASK_DLD_START ev_TASK_DLD_COMP    ev_TASK_DLD_FAIL */
/* idle */      { imGobiFwDldStart, imerror,            imerror },
/* fwdld */     { imerror,          imGobiTaskShutdown, imGobiTaskShutdown },
/* shutdwn */   { imerror,          imerror,            imerror },
};

/*************
 *
 * Name:    imgobitask
 *
 * Purpose: GOBI Image Management task. This task coordinates the downloading of
 *          Firmware to the modem
 *
 * Parms:   paramp - pointer to data that may be passed in when this task
 *                   is started
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   exits when download is complete or a download error occurs
 *
 **************/
package void imgobitask(
    void *paramp )
{
    swi_uint32        rmsg;
    struct imevtblock *eventbkp;
    struct imtcb      *imtcbp = imgetcbp();
    swi_char          defaultPath[] = "/opt/Qualcomm/Gobi/Images/3000/Generic/";

    UNUSEDPARAM( paramp );

    /* Initialize the SMT Message queue structure */
    icsmt_create( &imtcbp->imicmsgque, "IMGOBIMESSAGEQ", IMMSGDEPTH );

    dlLog0( &imtcbp->imdlcb,
            IMLOG_CLASSC,
            "IMGOBI Task started\n" );

    if( 0 == slstrlen(imtcbp->imqdl.filepath) )
    {
        slmemcpy( imtcbp->imqdl.filepath,
                  (swi_uint8*)defaultPath,
                  slstrlen (defaultPath ) );
    }

    /* Initialize all state machine tables for Gobi IM */
    /* Task state machine */
    smt.sat      = *imsatt;
    smt.stt      = *imsttt;
    smt.state    = st_SMT_IDLE;
    smt.evnum    = ev_TASK_MAX_EVENT;
    smt.stIgnore = st_SMT_IGNR;
    smt.stInvld  = st_SMT_INVLD;
    smt.udata    = NULL;

    /* Firmware Download state machine */
    smfd.sat      = *imsatfd;
    smfd.stt      = *imsttfd;
    smfd.state    = st_SMFD_IDLE;
    smfd.evnum    = ev_FWDLD_MAX_EVENT;
    smfd.stIgnore = st_SMFD_IGNR;
    smfd.stInvld  = st_SMFD_INVLD;
    smfd.udata    = NULL;

    /* Transaction state machine */
    smtr.sat      = *imsattr;
    smtr.stt      = *imstttr;
    smtr.state    = st_SMTR_WAIT_REQ;
    smtr.evnum    = ev_TRANS_MAX_EVENT;
    smtr.stIgnore = st_SMTR_IGNR;
    smtr.stInvld  = st_SMTR_INVLD;
    smtr.udata    = NULL;

    /* Initialize state tables in task control block */
    imtcbp->imsmp = &smt;
    imtcbp->imhdmemrelp          = NULL;
    imtcbp->imqdl.chunkSize      = IM_CHUNK_SIZE;
    imtcbp->imqdl.remainder      = 0;
    imtcbp->imqdl.numwrites      = 0;
    imtcbp->imqdl.sequenceNumber = 0;

    slmemset(imtcbp->imqdl.filename, 0, sizeof(imtcbp->imqdl.filename) );
    slmemset( imtcbp->imqdl.fullpathfilename,
              0,
              sizeof(imtcbp->imqdl.fullpathfilename) );

    smvalidate( imtcbp->imsmp );

    /* Kickstart the firmware download */
    imGobiQevt( ev_TASK_DLD_START, NULL, 0, NULL );

    /* Main line of processing begins next */
    for(;;)
    {
        /* Wait for an incoming IPC message then dispatch it */
        icsmt_rcv( &imtcbp->imicmsgque,
                   IMNOTIMEOUT,
                   &rmsg );

        if(rmsg)
        {
            /* Cast the data to an event block pointer */
            eventbkp = (struct imevtblock *)rmsg;

            /* store event block pointer for releasing upon thread termination
             * in which case the event block buffer is not released by
             * the code at the end of this control structure
             */

            /* Validate the state machine */
            smvalidate( imtcbp->imsmp );
#ifdef IMDBGFLG
            dlLog2( &imtcbp->imdlcb,
                    IMLOG_CLASSC,
                    "imgobitask :State - %lu :Rx Ev - %d\n",
                    (swi_uint32)imtcbp->imsmp->state,
                    (swi_uint32)eventbkp->imeventtype  );
#endif /* IMDBGFLG */

            enum imeventtype ev = eventbkp->imeventtype;

            /* Message has been handled, release the associated resources */
            imGobiRelEvtBk( eventbkp );

            /* run the state machine */
            smrun(imtcbp->imsmp, ev);
        }
        /* Do nothing if the message queue is empty or on timeout */
    }
}

/*************
 *
 * Name:    imdssinitcb
 *
 * Purpose: IM Gobi task DS shell ready callback
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
local void imGobidssinitcb()
{
    swi_ossdkthreadcreate( imgobitask,
                           NULL );
}

/*************
 *
 * Name:    imgobidsstart
 *
 * Purpose: Instantiate DS interface for Gobi IM
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
package void imgobidsstart()
{
    struct imtcb *imtcbp = imgetcbp();

    /* Initialize  DS shell parameters */
    imtcbp->imdsshell.dsipcshellnamep = (swi_uint8 *)CIIPCUIMRR2;
    imtcbp->imdsshell.dsipccbfcnp     = imGobiRxpkt;
    imtcbp->imdsshell.dssinitcbfp     = imGobidssinitcb;
    /* Create the DS shell for receiving responses from the device */
    swi_ossdkthreadcreate( dslaunchshell,
                           (void *)&imtcbp->imdsshell );
}
