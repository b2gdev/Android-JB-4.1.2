/*************
 *
 * Filename:    amapi.c
 *
 * Purpose:     Image management API support
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/

/*-------------
  include files
 --------------*/
#include "qa/inc/SwiDataTypes.h"
#include "ci/ciudefs.h"
#include "sl/sludefs.h"
#include "imssdp_hello.h"
#include "imssdp_open_unframed.h"
#include "imssdp_unframed_write.h"
#include "imssdp_done.h"
#include "imssdp_donefinal.h"
#include "imssdp.h"
#include "imcommon.h"
#include "imapi.h"
#include "imidefs.h"

/*-------
  Defines
 --------*/

/*-------------
  Local storage
 --------------*/

/*---------
  Functions
 ----------*/

/*
 *  Enumerates the QDL port IDs that are currently attached to the system
 *
 *  \param  pPortSize[IN/OUT]
 *          - Upon input, maximum number of elements that the device array can
 *          contain.
 *          - Upon successful output, actual number of elements in the
 *          device array.
 *
 *  \param  pPorts[IN/OUT]
 *          - Device array; array elements are port ID valuess:\n
 *            BYTE portID - /dev/ttyUSB port number\n
 *
 *  \return eQCWWAN_ERR_NONE on success\n
 *          <error code(s)> otherwise
 *
 *  \note   Timeout: n/a. This function interfaces directly to the API
 *
 */
ULONG GobiEnumerateQDLPorts(
    BYTE    *pPortSize,
    BYTE    *pPorts )
{
    UNUSEDPARAM( pPortSize );
    UNUSEDPARAM( pPorts );

    return 0;
}

/*
 *  Sets the timeout for all subsequent QDL transactions. The minimum allowed
 *  is 2 seconds.
 *
 *  \param  to
 *          - <description>
 *
 *  \return eQCWWAN_ERR_NONE on success\n
 *          <error code(s)> otherwise
 *
 *  \note   Timeout: n/a. This function interfaces directly to the API
 *
 */
ULONG SetQDLTimeout(
    ULONG   to )
{
    UNUSEDPARAM( to );
    return 0;
}

/*
 *  Opens the specified QDL port of the device and queries the boot downloader
 *  for version information
 *
 *  \param  portID
 *          - <description>
 *
 *  \param  bBARMode
 *          - <description>
 *
 *  \param  pMajorVersion[OUT]
 *          - <description>
 *
 *  \param  pMinorVersion[OUT]
 *          - <description>
 *
 *  \return eQCWWAN_ERR_NONE on success\n
 *          <error code(s)> otherwise
 *
 *  \note   Timeout: Configurable via SetQDLTimeout\n
 *
 */
ULONG OpenQDLPort( 
    BYTE    portID,
    ULONG   bBARMode,
    ULONG   *pMajorVersion,
    ULONG   *pMinorVersion )
{
    /* Define and allocate the task control block */
    struct imtcb *imtcbp = imgetcbp();
    ssdp_hello_rsp_s sHelloRsp;

    UNUSEDPARAM( portID );
    UNUSEDPARAM( bBARMode );

    /* Get the results of the Hello request */
    imssdp_hello_unpack (imtcbp->imrxdatap, &sHelloRsp);
    /* TBD
     * PortID and bBARMode
     */
    *pMajorVersion = sHelloRsp.major_boot_flash_version;
    *pMinorVersion = sHelloRsp.minor_boot_flash_version;

    return eQCWWAN_ERR_NONE;
}
/*
 *  Closes the currently open QDL port of the device and optionally informs the
 *  device that the download session is complete (triggering a device reset)
 *
 *  \param  bInformDevice
 *          - <description>
 *
 *  \return eQCWWAN_ERR_NONE on success\n
 *          <error code(s)> otherwise
 *
 *  \note   Timeout: Configurable via SetQDLTimeout\n
 *
 */
ULONG CloseQDLPort(
    ULONG   bInformDevice)
{
    struct imtcb *imtcbp = imgetcbp();
    swi_uint8 * txbufp;
    ssdp_donefinal_req_s sDoneFinalReq;
    swi_uint32 packetsize;

    if (bInformDevice)
    {
        /* Send the Done Final Request */
        sDoneFinalReq.command = SSDP_DONEFINAL_REQ_COMMAND;

        /* Get buffer from the tx pool */
        txbufp = (swi_uint8 *) mmbufget( &imtcbp->imtxpool );

        packetsize = imssdp_pack_request ( (struct sdp_packet *)&sDoneFinalReq,
                                           txbufp );

        imsendframed (txbufp, packetsize);

        /* free memory allocated for IM request */
        mmbufrel(txbufp);
    }
    return 0;
}

/*  Gets the current images preference, as reported by the device boot
 *  downloader.
 *
 *  \param  pImageListSize[IN/OUT]
 *          - <description>
 *
 *  \param  pImageList[OUT]
 *          - <description>
 *
 *  \return eQCWWAN_ERR_NONE on success\n
 *          <error code(s)> otherwise
 *
 *  \note   Timeout: Configurable via SetQDLTimeout\n
 *
 *          The returned image list will contain the entire images
 *          preference, i.e., all image types required by the current images
 *          preference, including those images that are already stored on the
 *          device.
 *
 */
ULONG GetQDLImagePreference(
    ULONG   *pImageListSize,
    BYTE    *pImageList )
{
    UNUSEDPARAM( pImageListSize );
    UNUSEDPARAM( pImageList );

    return 0;
}

/*
 *  Prepares the device boot downloader for an image write
 *
 *  \param  imageType
 *          - <description>
 *
 *  \param  imageSize
 *          - <description>
 *
 *  \param  pBlockSize[IN/OUT]
 *          - <description>
 *
 *  \return eQCWWAN_ERR_NONE on success\n
 *          <error code(s)> otherwise
 *
 *  \note   Timeout: Configurable via SetQDLTimeout\n
 *
 *          The image file that is being written is broken up into blocks whose
 *          size is subject to negotiation with the boot downloader. The maximum
 *          supported by the API is 64 MB. The maximum supported by the boot
 *          downloader is returned to the caller upon successful completion. The
 *          actual block size used in subsequent writes must be the smaller of
 *          the two values.\n
 *
 *          A return code of 41 indicates that the specified image is not required
 *          to be downloaded, as it already exists in device storage. As such, the
 *          image write must be skipped.
 *
 */
ULONG PrepareQDLImageWrite(
    BYTE    imageType,
    ULONG   imageSize,
    ULONG   *pBlockSize )
{
    struct imtcb *imtcbp = imgetcbp();
    ssdp_open_unframed_rsp_s sOpenUnframedRsp;

    UNUSEDPARAM( imageType );
    UNUSEDPARAM( imageSize );

    /* Get the results of the Open Unframed request */
    imssdp_unpack_response (imtcbp->imrxdatap,
                            (struct sdp_packet *)&sOpenUnframedRsp);

    if ( sOpenUnframedRsp.status != SSDP_OPEN_UNFRAMED_STATUS_OK)
    {
        return sOpenUnframedRsp.status;
    }

    *pBlockSize = sOpenUnframedRsp.max_payload_length;

    return eQCWWAN_ERR_NONE;
}

/*
 *  Writes the specified image block to the device
 *
 *  \param  sequenceNumber
 *          - <description>
 *
 *  \param  blockSize
 *          - <description>
 *
 *  \param  pImageBlock
 *          - <description>
 *
 *  \return eQCWWAN_ERR_NONE on success\n
 *          <error code(s)> otherwise
 *
 *  \note   Timeout: Configurable via SetQDLTimeout\n
 *
 */
ULONG WriteQDLImageBlock( 
    USHORT  sequenceNumber,
    ULONG   blockSize,
    BYTE    *pImageBlock )
{
    struct imtcb              *imtcbp = imgetcbp();
    ssdp_unframed_write_req_s sUnframedWriteReq;
    swi_uint32                packetsize;
    swi_uint8                 *dlbufp;

   imbuild_unframed_write_req ( &sUnframedWriteReq,
                                sequenceNumber,
                                blockSize,
                                pImageBlock );

   /* CRC is calculated in the pack function */

    /* Get buffer from the dl pool */
    dlbufp = (swi_uint8 *) mmbufget( &imtcbp->imdlpool );

    packetsize = imssdp_pack_request ( (struct sdp_packet *)&sUnframedWriteReq,
                                       dlbufp );

    /* send the Unframed Write Request - note: no HDLC framing */
    imdssend (dlbufp, packetsize);

    /* free memory allocated for IM request */
    mmbufrel(dlbufp);

    return 0;
}

/*
 *  Request the device to validate the content of downloaded images. This
 *  function is called proceeding the completion of image downloading.
 *
 *  \param  pImageType[OUT]
 *          - <description>
 *
 *  \return eQCWWAN_ERR_NONE on success\n
 *          <error code(s)> otherwise
 *
 *  \note   Timeout: Configurable via SetQDLTimeout\n
 *
 */
ULONG ValidateQDLImages(
    BYTE    *pImageType )
{
    struct imtcb *imtcbp = imgetcbp();
    ssdp_done_rsp_s sDoneRsp;

    /* Get the results of the done request */
    imssdp_unpack_response (imtcbp->imrxdatap,
                            (struct sdp_packet *)&sDoneRsp);

    switch (sDoneRsp.session_status)
    {
        case SSDP_DONE_SUCCESS:
            *pImageType = 0xFF; /* ok? */
            break;
        case SSDP_DONE_AUTH_FAILED:
        case SSDP_DONE_FLASH_WRITE_ERROR:
        default:
            *pImageType = sDoneRsp.failed_image_type;
            dlLog( &imtcbp->imdlcb,
                   IMLOG_CLASSC,
                   "Done Resp failed, s:%i, type: %i, error:%s\n",
                   (swi_uint32)imtcbp->imsmp->state,
                   (swi_uint32)sDoneRsp.failed_image_type,
                   (swi_uint32)sDoneRsp.error_description,
                   (swi_uint32)NULL );
            return eQCWWAN_ERR_GENERAL;
    }

    return eQCWWAN_ERR_NONE;
}
