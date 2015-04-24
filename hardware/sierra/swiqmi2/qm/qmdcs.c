/*************
 *
 * Filename: qmdcs.c
 *
 * Purpose:  QMI Device Connectivity Service
 *
 * Copyright: © 2011-12 Sierra Wireless Inc., all rights reserved
 *
 **************/

/*---------------
  include files
 ----------------*/
#include "aa/aaglobal.h"
#include "qm/qmerrno.h"
#include "am/amudefs.h"
#include "ci/ciudefs.h"
#include "er/erudefs.h"
#include "mm/mmudefs.h"
#include "os/swi_ossdk.h"
#include "pi/piudefs.h"
#include "qm/qmidefs.h"
#include "qm/qmdcs.h"
#include "qm/qmqmisvc.h"
#include "sl/sludefs.h"
#include "us/usudefs.h"
#include "qm/qmdcs_sdk.h"

/*---------------
  Defines
 ---------------*/

/*--------------------
  Forward declarations
 ---------------------*/

/* DCS request handler table */
local void qm_dcs_enumerate_devices(swi_uint8 *);
local void qm_dcs_connect(swi_uint8 *);
local void qm_dcs_disconnect(swi_uint8 *);
local void qm_dcs_getconnected_device_id(swi_uint8 *);
local void qm_dcs_cancel(swi_uint8 *);
local void qm_dcs_get_usb_port_names(swi_uint8 *);
local void qm_dcs_get_device_mode(swi_uint8 *);

/*-------------
  Local storage
 --------------*/

local struct qmdcstcb qmdcstaskblk;

/* QMI DCS response structure */
local struct qmqmisvcresponse qmidcsresp;

/* Device Connectivity Service (DCS) request handler table */
local void(*dcshandlertbl[])(swi_uint8 *preq) =
{
    qm_dcs_enumerate_devices,
    qm_dcs_connect,
    qm_dcs_disconnect,
    qm_dcs_getconnected_device_id,
    qm_dcs_cancel,
    qm_dcs_get_usb_port_names,
    qm_dcs_get_device_mode
};

/*---------
  Functions
 ----------*/

/*************
 *
 * Name:    qmdcsrelevtbk
 *
 * Purpose: To release the resources associated with the QMI DCS event block,
 *
 * Parms:   eventbkp - Pointer to the QMI DCS Event Block structure
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   This function releases the resources associated with the
 *          QMI event block as follows: first the memory buffer containing
 *          the newly received QMI packet is released. Next the event buffer
 *          itself is released. Both are allocated from pools created at
 *          system startup time by the DCS task.
 *
 **************/
local void
qmdcsrelevtbk( struct qmdcsevtblock *eventbkp )
{
    /* If the caller's memory can be freed... */
    if( eventbkp->qmdcsevtmemfreep!= NULL )
    {
        /* Free the QMI message buffer first */
        mmbufrel( eventbkp->qmdcsevtmemfreep);
    }

    /* Free the event block itself */
    mmbufrel( (swi_uint8 *)eventbkp );
}

/*************
 *
 * Name:    qmgetcbp
 *
 * Purpose: Return a pointer to the DCS Request/Response task control block.
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
package struct qmdcstcb *qmdcsgetcbp(void)
{
    return &qmdcstaskblk;
}


/*************
 *
 * Name:    qm_dcs_devkey_get
 *
 * Purpose: Retrieve device key (MEID) from the modem
 *
 * Parms:   (OUT)   pdevkey   - storage location for device ID.
 *
 * Return:  TRUE if device is present, FALSE otherwise
 *
 * Abort:   none
 *
 * Notes:   allocated device key storage must be at least QMI_MEID_BUF_SIZE bytes.
 *
 **************/
local swi_bool
qm_dcs_devkey_get(
    struct qm_dcs_deviceinfo *pdev )
{
    /* clear the device key buffer */
    slmemset( (char *)pdev->devkey, EOS, sizeof(pdev->devkey) );
    pdev->devkeystrsize = sizeof(pdev->devkey) - 1;

    if( !dsioctl( cigetchannelnum(CIIPCUQMRR1),
                  QMIDCSCLNT,
                  QMI_GET_MEID_IOCTL,
                  pdev->devkey,
                  QMI_MEID_BUF_SIZE ) )
    {
        struct qmtcb *tcbp = qmgetcbp();
        dlLog( &tcbp->qmdlcb, QMLOG_CLASSA,
               "QMI_GET_MEID_IOCTL failed",
               (swi_uint32)NULL,
               (swi_uint32)NULL,
               (swi_uint32)NULL,
               (swi_uint32)NULL );

        return FALSE;
    }

    return TRUE;
}

/*************
 *
 * Name:    qm_dcs_devnode_get
 *
 * Purpose: Retrieve the qmi device path
 *
 * Parms:   (OUT)   pdevnode   - allocated device path storage
 *
 * Return:  NULL on failure, pdev on success
 *
 * Abort:   none
 *
 * Notes:   allocated device path storage must be at least SWI_USB_MAX_PATH bytes.
 *
 **************/
local swi_bool
qm_dcs_devnode_get(
    struct qm_dcs_deviceinfo *pdev)
{
    /* Get the fully qualified device file name */
    struct usbep qmiep;

    /* invalid input */
    if( pdev ==  NULL )
    {
        return FALSE;
    }

    /* clear the device node buffer */
    slmemset( (char *)pdev->devnode, EOS, SWI_USB_MAX_PATH );

    if( !usgetep(USEP_QMI, &qmiep) )
    {
        return FALSE;
    }

    if( !usgetepname( qmiep.usreadep,
                      pdev->devnode,
                      SWI_USB_MAX_PATH,
                      USREAD ) )
    {
        return FALSE;
    }

    pdev->devnodestrsize = (swi_uint16)slstrlen( (const char *)pdev->devnode ) + 1;
    return TRUE;
}

/*************
 *
 * Name:    qm_dcs_enumerate_device_key_pack_tlv
 *
 * Purpose: Pack device id TLV Value
 *
 * Parms:   (OUT)   pdest   - destination buffer
 *          (IN)    psrc   - source data
 *
 * Return:  eQCWWAN_ERR_NONE
 *
 * Abort:   none
 *
 * Notes:   pdest is advanced one byte past the device id written to the buffer
 *
 **************/
local enum eQCWWANError
qm_dcs_enumerate_device_key_pack_tlv(
    swi_uint8 *pdest,
    swi_uint8 *psrc )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_dcs_enumerate_devices_response_tlv_values *pval;

    pin = (struct qm_qmi_response_tlvs_values *)psrc;
    pval = (struct qm_dcs_enumerate_devices_response_tlv_values *)&pin->tlvvalues.qmdcstlvs.enumerate;

    return  PutStream( pdest,
                       pval->dev.devkey,
                       (swi_uint32)pval->dev.devkeystrsize );
}

/*************
 *
 * Name:    qm_dcs_enumerate_device_node_pack_tlv
 *
 * Purpose: Pack device node TLV Value
 *
 * Parms:   (OUT)   pdest   - destination buffer
 *          (IN)    psrc   - source data
 *
 * Return:  eQCWWAN_ERR_NONE
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_dcs_enumerate_device_node_pack_tlv(
    swi_uint8 *pdest,
    swi_uint8 *psrc )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_dcs_enumerate_devices_response_tlv_values *pval;

    pin  = (struct qm_qmi_response_tlvs_values *)psrc;
    pval = (struct qm_dcs_enumerate_devices_response_tlv_values *)&pin->tlvvalues.qmdcstlvs.enumerate;

    return PutStream( pdest,
                      pval->dev.devnode,
                      sizeof(pval->dev.devkey) - 1);
}

/*************
 *
 * Name:    qm_dcs_enumerate_device_send_response
 *
 * Purpose: Pack and send a DCS enumeration response
 *
 * Parms:   (IN)    msgid       - QMI DCS service message id
 *          (OUT)   prsptlvs    - response structure
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local void
qm_dcs_enumerate_device_send_response(
    enum eQMIMessageDCS msgid,
    struct qm_qmi_response_tlvs_values *prsptlvs )
{
    struct qmTlvBuilderItem map[] =
    {
        { eTLV_RESULT_CODE,
          qm_result_code_tlv_pack },

        { eTLV_DCS_ENUMERATE_DEVICE_NODE,
          qm_dcs_enumerate_device_node_pack_tlv },

        { eTLV_DCS_ENUMERATE_DEVICE_KEY,
          qm_dcs_enumerate_device_key_pack_tlv },

        { eTLV_TYPE_INVALID, NULL }
    };

    /* construct and send response to the application */
    qm_qmisvc_send_response( &qmidcsresp,
                             prsptlvs,
                             map,
                             eQMI_SVC_DCS,
                             msgid,
                             eQMIRES );
}

/*************
 *
 * Name:    qm_dcs_enumerate_devices
 *
 * Purpose: Service Gobi API QCWWAN(2k)Enumerate request
 *
 * Parms:   (IN)    preq - request packet pointer
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   DCS is a pseudo-QMI service managed by the SDK in order
 *          to support the Device Connectivity APIs. Refer to
 *          QCT document 80-VF219-N.
 *
 **************/
local void
qm_dcs_enumerate_devices(
    swi_uint8 *preq )
{
    /* Get the QMI RR and DCS control block pointer */
    struct qmtcb *qmcbp = qmgetcbp();
    struct qmdcstcb *dcscbp = qmdcsgetcbp();

    /* tlv structure used to construct QMI response */
    struct qm_qmi_response_tlvs_values rsptlvs;

    rsptlvs.qmiresult.result = QMI_RC_FAILURE;
    rsptlvs.qmiresult.error  = eQCWWAN_ERR_NO_DEVICE;

    switch (qmcbp->qmidevicestate)
    {
        case DS_SIO_DISCONNECTED:
        case DS_SIO_BOOT_READY:
            qmshortresp( eQCWWAN_ERR_NO_DEVICE, dcscbp->qmdcsipcchannel );
            break;
        default:
        {
            struct qm_dcs_deviceinfo *pDev =
                &rsptlvs.tlvvalues.qmdcstlvs.enumerate.dev;
            /* get device file path and device key */
            if( !qm_dcs_devnode_get(pDev) )
            {
                rsptlvs.qmiresult.error = eQCWWAN_ERR_SWIDCS_DEVNODE_NOT_FOUND;
            }
            else if( qm_dcs_devkey_get(pDev) )
            {
                rsptlvs.qmiresult.result = QMI_RC_SUCCESS;
                rsptlvs.qmiresult.error  = eQCWWAN_ERR_NONE;
            }
            else
            {
                rsptlvs.qmiresult.error = eQCWWAN_ERR_SWIDCS_IOCTL_ERR;
            }
            qm_dcs_enumerate_device_send_response( (enum eQMIMessageDCS)*preq,
                                                    &rsptlvs );
            break;
        }
    }
}

/*************
 *
 * Name:    qm_dcs_connect_device_key_unpack_tlv
 *
 * Purpose: Unpack device key TLV Value
 *
 * Parms:   (IN)     psource    - source data
 *          (OUT)    pdest      - destination buffer
 *
 * Return:  eQCWWAN_ERR_NONE
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_dcs_connect_device_key_unpack_tlv(
    swi_uint8 *psource,
    swi_uint8 *pdest )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_dcs_connect_response_tlv_values *pval;

    pin = (struct qm_qmi_response_tlvs_values *)pdest;
    pval = (struct qm_dcs_connect_response_tlv_values *)&pin->tlvvalues.qmdcstlvs.connect;

    return GetStream( psource,
                      pval->dev.devkey,
                      sizeof(pval->dev.devkey) - 1);
}

/*************
 *
 * Name:    qm_dcs_connect_device_node_unpack_tlv
 *
 * Purpose: Unpack device node TLV Value
 *
 * Parms:   (IN)     psource    - source data
 *          (OUT)    pdest      - destination buffer
 *
 * Return:  eQCWWAN_ERR_NONE
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_dcs_connect_device_node_unpack_tlv(
    swi_uint8 *psource,
    swi_uint8 *pdest )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_dcs_connect_response_tlv_values *pval;

    pin = (struct qm_qmi_response_tlvs_values *)pdest;
    pval = (struct qm_dcs_connect_response_tlv_values *)&pin->tlvvalues.qmdcstlvs.connect;

    swi_uint32 len = SWI_USB_MAX_PATH;
    return qmQmiExtractString( psource,
                               (swi_char *)pval->dev.devnode,
                               len );
}

/*************
 *
 * Name:    qm_dcs_connect_send_response
 *
 * Purpose: Pack and send a DCS connect response
 *
 * Parms:   (IN)    msgid       - QMI DCS message id
 *          (OUT)   prsptlvs    - response structure
 *
 * Return:  eQCWWAN_ERR_NONE
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local void
qm_dcs_connect_send_response(
    enum eQMIMessageDCS msgid,
    struct qm_qmi_response_tlvs_values *prsptlvs )
{
    struct qmTlvBuilderItem map[] =
    {
        {   eTLV_RESULT_CODE,
            qm_result_code_tlv_pack },

        {   eTLV_TYPE_INVALID,
            NULL }
    };

    /* construct and send response to the application */
    qm_qmisvc_send_response( &qmidcsresp,
                             prsptlvs,
                             map,
                             eQMI_SVC_DCS,
                             msgid,
                             eQMIRES );
}

/*************
 *
 * Name:    qm_dcs_connect_request_unpack
 *
 * Purpose: Unpack DCS Connect request
 *
 * Parms:   (IN)    preq        - incoming request packet
 *          (OUT)   preqtlvs    - destination data containing request
 *                                parameters.
 *
 * Return:  eQCWWAN_ERR_NONE if successfully unpacked request
 *          eQCWWAN_ERR_MEMORY if failed to unpack request
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_dcs_connect_request_unpack(
    swi_uint8 *preq,
    struct qm_qmi_response_tlvs_values *preqtlvs )
{
    struct qmTlvUnpackerItem map[] =
    {
        { eTLV_DCS_CONNECT_DEVICE_NODE,
          qm_dcs_connect_device_node_unpack_tlv },

        { eTLV_DCS_CONNECT_DEVICE_KEY,
          qm_dcs_connect_device_key_unpack_tlv },

        { eTLV_TYPE_INVALID, NULL }
    };

    return qmunpack( preq,
                     (swi_uint8 *)preqtlvs,
                     map,
                     eQMI_DCS_CONNECT );
}

/*************
 *
 * Name:    qm_dcs_connect_find_connected_device
 *
 * Purpose: Get the device path and device key (MEID) of
 *          a connected device
 *
 * Parms:   (IN)   pdev    - device information
 *
 * Return:  TRUE if found a device, FALSE otherwise
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local swi_bool
qm_dcs_connect_find_connected_device(
    struct qm_dcs_deviceinfo *pdev )
{
    if( !qm_dcs_devnode_get(pdev) ||
        !qm_dcs_devkey_get(pdev) )
    {
        return FALSE;
    }

    return TRUE;
}

/*************
 *
 * Name:    qm_dcs_connect
 *
 * Purpose: Service Gobi API QCWWAN(2k)Connect request
 *
 * Parms:   (IN)    preq    - request packet pointer
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   1. qm_dcs_enumerate_devices should be called prior to this.
 *          2. DCS is a pseudo-QMI service managed by the SDK in order to
 *             support the Device Connectivity APIs. Refer to QCT document
 *             80-VF219-N.
 *
 **************/
local void
qm_dcs_connect(
    swi_uint8 *preq )
{
    struct qm_dcs_deviceinfo connected_device;  /* connected device path and device key */
    struct qm_qmi_response_tlvs_values reqtlvs; /* for unpacking request */
    struct qm_qmi_response_tlvs_values rsptlvs; /* for packing response */

    /* initialize result tlv */
    rsptlvs.qmiresult.result = QMI_RC_SUCCESS;
    rsptlvs.qmiresult.error  = eQCWWAN_ERR_NONE;

    /* get device node */
    if( !qm_dcs_connect_find_connected_device(&connected_device) )
    {
        /* no device found */
        rsptlvs.qmiresult.result = QMI_RC_FAILURE;
        rsptlvs.qmiresult.error  = eQCWWAN_ERR_NO_DEVICE;
    }
    /* device found */
    else
    {
        enum eQCWWANError rc = qm_dcs_connect_request_unpack(preq, &reqtlvs);
        if( rc != eQCWWAN_ERR_NONE )
        {
            /* request TLV extraction failed */
            rsptlvs.qmiresult.result = QMI_RC_FAILURE;
            rsptlvs.qmiresult.error  = rc;

            char errmsg[100];
            snprintf(errmsg, sizeof(errmsg),
                     "%s:%d request TLV extraction failed",
                     (char *)__func__, __LINE__);
            erAbort(errmsg, (swi_uint32)rc );
        }
    }

    if( rsptlvs.qmiresult.result == QMI_RC_SUCCESS )
    {
        /* compare requested and connected device paths */
        if( !slstrncmp( (char *)reqtlvs.tlvvalues.qmdcstlvs.connect.dev.devnode,
                        (char *)connected_device.devnode,
                        slstrlen( (const char *)reqtlvs.tlvvalues.qmdcstlvs.connect.dev.devnode) ) )
        {
            /* compare requested and connected device keys */
            if( !slstrncmp( (char *)reqtlvs.tlvvalues.qmdcstlvs.connect.dev.devkey,
                            (char *)connected_device.devkey,
                            (swi_uint32)QMI_MEID_BUF_SIZE - 1 ) )
            {
                /* update result tlv */
                rsptlvs.qmiresult.result = QMI_RC_SUCCESS;
                rsptlvs.qmiresult.error  = eQCWWAN_ERR_NONE;
                /* register user application with SDK */
                struct qmtcb *ptcb = qmgetcbp();
                ptcb->qmappregistered = TRUE;
            }
        }
    }

    qm_dcs_connect_send_response( (enum eQMIMessageDCS)*preq,
                                  &rsptlvs );
}

/*************
 *
 * Name:    qm_dcs_disconnect_send_response
 *
 * Purpose: Pack and send a DCS disconnect response
 *
 * Parms:   (IN)    msgid       - QMI DCS message ID
 *          (OUT)   prsptlvs    - response TLV structure
 *
 * Return:  eQCWWAN_ERR_NONE
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local void
qm_dcs_disconnect_send_response(
    enum eQMIMessageDCS msgid,
    struct qm_qmi_response_tlvs_values *prsptlvs )
{
    struct qmTlvBuilderItem map[] =
    {
        {   eTLV_RESULT_CODE,
            qm_result_code_tlv_pack },

        {   eTLV_TYPE_INVALID,
            NULL }
    };

    /* construct and send response to the application */
    qm_qmisvc_send_response( &qmidcsresp,
                             prsptlvs,
                             map,
                             eQMI_SVC_DCS,
                             msgid,
                             eQMIRES );
}

/*************
 *
 * Name:    qm_dcs_disconnect
 *
 * Purpose: Service Gobi API QCWWANDisconnect request
 *
 * Parms:   (IN)    preq - request packet pointer
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   DCS is a pseudo-QMI service managed by the SDK in order
 *          to support the Device Connectivity APIs. Refer to
 *          QCT document 80-VF219-N.
 *
 **************/
local void
qm_dcs_disconnect(
    swi_uint8 *preq )
{
    /* for packing response */
    struct qm_qmi_response_tlvs_values rsptlvs;

    /* initialize result tlv */
    rsptlvs.qmiresult.result = QMI_RC_SUCCESS;
    rsptlvs.qmiresult.error  = eQCWWAN_ERR_NONE;

/* RILSTART */
    qm_dcs_disconnect_send_response( (enum eQMIMessageDCS)*preq,
                                     &rsptlvs );

    /* shut down QMI services */
    qmdisconnect();
/* RILSTOP */
}

/*************
 *
 * Name:    qm_dcs_getconnected_device_id_send_response
 *
 * Purpose: Pack and send a DCS connected device ID response
 *
 * Parms:   (IN)    msgid       - QMI DCS message ID
 *          (OUT)   prsptlvs    - response TLV structure
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   1. qm_dcs_enumerate_devices should be called prior to this.
 *          2. DCS is a pseudo-QMI service managed by the SDK in order to
 *             support the Device Connectivity APIs. Refer to QCT document
 *             80-VF219-N.
 *
 **************/
local void
qm_dcs_getconnected_device_id_send_response(
    enum eQMIMessageDCS msgid,
    struct qm_qmi_response_tlvs_values *prsptlvs )
{
    struct qmTlvBuilderItem map[] =
    {
        {   eTLV_RESULT_CODE,
            qm_result_code_tlv_pack },

        {   eTLV_DCS_CONNECTED_DEVICEID_DEVICE_NODE,
            qm_dcs_enumerate_device_node_pack_tlv
        },

        {   eTLV_DCS_CONNECTED_DEVICEID_DEVICE_KEY,
            qm_dcs_enumerate_device_key_pack_tlv
        },

        {   eTLV_TYPE_INVALID,
            NULL }
    };

    /* construct and send response to the application */
    qm_qmisvc_send_response( &qmidcsresp,
                             prsptlvs,
                             map,
                             eQMI_SVC_DCS,
                             msgid,
                             eQMIRES );
}

/*************
 *
 * Name:    qm_dcs_getconnected_device_id
 *
 * Purpose: Service QCWWANGetConnectedDeviceID Gobi API request
 *
 * Parms:   (IN)    preq - request packet pointer
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   1. qm_dcs_enumerate_devices should be called prior to this.
 *          2. DCS is a pseudo-QMI service managed by the SDK in order to
 *             support the Device Connectivity APIs. Refer to QCT document
 *             80-VF219-N.
 *
 **************/
local void
qm_dcs_getconnected_device_id(
    swi_uint8 *preq )
{
    /* for packing response */
    struct qm_qmi_response_tlvs_values rsptlvs;

    rsptlvs.qmiresult.result = QMI_RC_FAILURE;
    rsptlvs.qmiresult.error  = eQCWWAN_ERR_NO_DEVICE;

    /* get device file path and device key */
    if( qm_dcs_devnode_get(&rsptlvs.tlvvalues.qmdcstlvs.enumerate.dev) &&
        qm_dcs_devkey_get(&rsptlvs.tlvvalues.qmdcstlvs.enumerate.dev) )
    {
       /* device found */
        rsptlvs.qmiresult.result = QMI_RC_SUCCESS;
        rsptlvs.qmiresult.error  = eQCWWAN_ERR_NONE;
    }

    qm_dcs_getconnected_device_id_send_response( (enum eQMIMessageDCS)*preq,
                                                 &rsptlvs );
}

/*************
 *
 * Name:    qm_dcs_cancel_send_response
 *
 * Purpose: Pack and send the response to a DCS cancel request
 *
 * Parms:   (IN)    msgid       - QMI DCS message ID
 *          (OUT)   prsptlvs    - response TLV structure
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local void
qm_dcs_cancel_send_response(
    enum eQMIMessageDCS msgid,
    struct qm_qmi_response_tlvs_values *prsptlvs )
{
    struct qmTlvBuilderItem map[] =
    {
        {   eTLV_RESULT_CODE,
            qm_result_code_tlv_pack },

        {   eTLV_TYPE_INVALID,
            NULL }
    };

    /* construct and send response to the application */
    qm_qmisvc_send_response( &qmidcsresp,
                             prsptlvs,
                             map,
                             eQMI_SVC_DCS,
                             msgid,
                             eQMIRES );
}

/*************
 *
 * Name:    qm_dcs_cancel
 *
 * Purpose: Service Gobi API QCWWANCancel request.
 *
 * Parms:   (IN) preq - request packet pointer
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   DCS is a pseudo-QMI service managed by the SDK in order
 *          to support the Device Connectivity APIs. Refer to
 *          QCT document 80-VF219-N.
 *
 **************/
local void
qm_dcs_cancel(
    swi_uint8 *preq)
{
    /* for packing response */
    struct qm_qmi_response_tlvs_values rsptlvs;
    rsptlvs.qmiresult.result = QMI_RC_SUCCESS;
    rsptlvs.qmiresult.error  = eQCWWAN_ERR_NONE;

    qmqmirequestcancel();
    qm_dcs_cancel_send_response( (enum eQMIMessageDCS)*preq,
                                 &rsptlvs );
}

/*************
 *
 * Name:    qm_dcs_get_usb_port_names_atcmdport_pack_tlv
 *
 * Purpose: Pack AT command port TLV Value
 *
 * Parms:   (OUT)    pdest   - destination buffer
 *          (IN)     psrc    - source data
 *
 * Return:  eQCWWAN_ERR_NONE
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_dcs_get_usb_port_names_atcmdport_pack_tlv(
    swi_uint8 *pdest,
    swi_uint8 *psrc )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_dcs_usb_port_names_response_tlv_values *pval;

    pin  = (struct qm_qmi_response_tlvs_values *)psrc;
    pval = (struct qm_dcs_usb_port_names_response_tlv_values *)&pin->tlvvalues.qmdcstlvs.portnames;

    return PutStream( pdest,
                      (swi_uint8 *)pval->usbportnames.AtCmdPort,
                      (swi_uint32)strlen(pval->usbportnames.AtCmdPort) );
}

/*************
 *
 * Name:    qm_dcs_get_usb_port_names_nmeaport_pack_tlv
 *
 * Purpose: Pack NMEA port TLV Value
 *
 * Parms:   (OUT)    pdest   - destination buffer
 *          (IN)     psrc    - source data
 *
 * Return:  eQCWWAN_ERR_NONE
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_dcs_get_usb_port_names_nmeaport_pack_tlv(
    swi_uint8 *pdest,
    swi_uint8 *psrc )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_dcs_usb_port_names_response_tlv_values *pval;

    pin  = (struct qm_qmi_response_tlvs_values *)psrc;
    pval = (struct qm_dcs_usb_port_names_response_tlv_values *)&pin->tlvvalues.qmdcstlvs.portnames;

    return PutStream( pdest,
                      (swi_uint8 *)pval->usbportnames.NmeaPort,
                      (swi_uint32)strlen(pval->usbportnames.NmeaPort) );
}

/*************
 *
 * Name:    qm_dcs_get_usb_port_names_dmport_pack_tlv
 *
 * Purpose: Pack DM port TLV Value
 *
 * Parms:   (OUT)    pdest   - destination buffer
 *          (IN)     psrc    - source data
 *
 * Return:  eQCWWAN_ERR_NONE
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_dcs_get_usb_port_names_dmport_pack_tlv(
    swi_uint8 *pdest,
    swi_uint8 *psrc )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_dcs_usb_port_names_response_tlv_values *pval;

    pin  = (struct qm_qmi_response_tlvs_values *)psrc;
    pval = (struct qm_dcs_usb_port_names_response_tlv_values *)&pin->tlvvalues.qmdcstlvs.portnames;

    return PutStream( pdest,
                      (swi_uint8 *)pval->usbportnames.DmPort,
                      (swi_uint32)strlen(pval->usbportnames.DmPort) );
}

/*************
 *
 * Name:    qm_dcs_getusbportnames
 *
 * Purpose: Retrieve the names of the services running over the
 *          /dev/ttyUSBx devices
 *
 * Parms:   (OUT)   pusbportnames   - allocated port names storage
 *
 * Return:  FALSE on failure, TRUE on success
 *
 * Abort:   none
 *
 * Notes:   allocated port names storage must be at least QMI_USB_PORT_NAME_SIZE
 *          bytes.
 *
 **************/
local swi_bool
qm_dcs_getusbportnames(
    struct qm_dcs_usb_port_names *pusbportnames)
{
    /* Get the fully qualified device file name */
    struct usbep qmiep;

    /* invalid input */
    if( pusbportnames ==  NULL )
    {
        return FALSE;
    }

    /* clear the port names buffers */
    slmemset( (char *)pusbportnames, EOS,
              sizeof (struct qm_dcs_usb_port_names) );

    /* If endpoint is available, we try to get all the friendly names */
    if( usgetep(USEP_ATCMD, &qmiep) )
    {
        usgetepname( qmiep.usreadep,
                     (swi_uint8 *)pusbportnames->AtCmdPort,
                     QMI_USB_PORT_NAME_SIZE,
                     USREAD );
        /* We ignore failure and return empty string */
    }

    if( usgetep(USEP_NMEA, &qmiep) )
    {
        usgetepname( qmiep.usreadep,
                     (swi_uint8 *)pusbportnames->NmeaPort,
                     QMI_USB_PORT_NAME_SIZE,
                     USREAD );
        /* We ignore failure and return empty string */
    }

    if( usgetep(USEP_DIAG, &qmiep) )
    {
        usgetepname( qmiep.usreadep,
                     (swi_uint8 *)pusbportnames->DmPort,
                     QMI_USB_PORT_NAME_SIZE,
                     USREAD );
        /* We ignore failure and return empty string */
    }

    return TRUE;
}

/*************
 *
 * Name:    qm_dcs_get_usb_port_names_send_response
 *
 * Purpose: Pack and send a DCS GetUsb Port Names response
 *
 * Parms:   (IN)    msgid       - QMI DCS message ID
 *          (OUT)   prsptlvs    - response TLV structure
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   1. qm_dcs_enumerate_devices should be called prior to this.
 *          2. DCS is a pseudo-QMI service managed by the SDK in order to
 *             support the Device Connectivity APIs. Refer to QCT document
 *             80-VF219-N.
 *
 **************/
local void
qm_dcs_get_usb_port_names_send_response(
    enum eQMIMessageDCS msgid,
    struct qm_qmi_response_tlvs_values *prsptlvs )
{
    struct qmTlvBuilderItem map[] =
    {
        {   eTLV_RESULT_CODE,
            qm_result_code_tlv_pack },

        {   eTLV_DCS_GET_USB_PORT_NAME_ATCMDPORT,
            qm_dcs_get_usb_port_names_atcmdport_pack_tlv
        },

        {   eTLV_DCS_GET_USB_PORT_NAME_NMEAPORT,
            qm_dcs_get_usb_port_names_nmeaport_pack_tlv
        },

        {   eTLV_DCS_GET_USB_PORT_NAME_DMPORT,
            qm_dcs_get_usb_port_names_dmport_pack_tlv
        },

        {   eTLV_TYPE_INVALID,
            NULL
        }
    };

    /* construct and send response to the application */
    qm_qmisvc_send_response( &qmidcsresp,
                             prsptlvs,
                             map,
                             eQMI_SVC_DCS,
                             msgid,
                             eQMIRES );
}

/*************
 *
 * Name:    qm_dcs_get_usb_port_names
 *
 * Purpose: Service SLQSGetUsbPortNames Gobi API request
 *
 * Parms:   (IN)    preq - request packet pointer
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   1. qm_dcs_enumerate_devices should be called prior to this.
 *          2. DCS is a pseudo-QMI service managed by the SDK in order to
 *             support the Device Connectivity APIs. Refer to QCT document
 *             80-VF219-N.
 *          3. This is an internal API that is exposed to the user.
 *
 **************/
local void
qm_dcs_get_usb_port_names(
    swi_uint8 *preq )
{
    /* for packing response */
    struct qm_qmi_response_tlvs_values rsptlvs;

    slmemset ( (char*)&rsptlvs,
               EOS,
               sizeof (struct qm_qmi_response_tlvs_values));

    rsptlvs.qmiresult.result = QMI_RC_FAILURE;
    rsptlvs.qmiresult.error  = eQCWWAN_ERR_NO_DEVICE;

    /* get port names */
    if( qm_dcs_getusbportnames( &(rsptlvs.tlvvalues.qmdcstlvs.portnames.usbportnames)))
    {
       /* Port Names retrieved */
        rsptlvs.qmiresult.result = QMI_RC_SUCCESS;
        rsptlvs.qmiresult.error  = eQCWWAN_ERR_NONE;
    }

    qm_dcs_get_usb_port_names_send_response( (enum eQMIMessageDCS)*preq,
                                              &rsptlvs );
}

/*************
 *
 * Name:    qm_dcs_device_state_pack_tlv
 *
 * Purpose: Pack device state notification TLV Value
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
qm_dcs_device_state_pack_tlv(
    swi_uint8 *pdest,
    swi_uint8 *psrc )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_dcs_device_state_change_tlv_values *pval;

    pin  = (struct qm_qmi_response_tlvs_values *)psrc;
    pval = (struct qm_dcs_device_state_change_tlv_values *)
                            &pin->tlvvalues.qmdcstlvs.devstatechgnotif;

    return PutByte( pdest, pval->devstate );
}

/*************
 *
 * Name:    qm_dcs_event_notification_send
 *
 * Purpose: Pack and send a DCS event indication
 *
 * Parms:   (IN)     pmap     - pointer to TLV action map
 *          (OUT)    prsptlvs - pointer to response TLV structure
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   DCS is a pseudo-QMI service managed by the SDK in order to
 *          support the Device Connectivity APIs. Refer to QCT document
 *          80-VF219-N.
 *
 **************/
global void
qm_dcs_event_notification_send(
    struct qmTlvBuilderItem *pmap,
    struct qm_qmi_response_tlvs_values *prsptlvs )
{
    /* construct and send response to the application */
    qm_qmisvc_send_response( &qmidcsresp,
                             prsptlvs,
                             pmap,
                             eQMI_SVC_DCS,
                             eQMI_DCS_EVENT_IND,
                             eQMINOT );
}

/*************
 *
 * Name:    qm_dcs_dev_state_change_notify
 *
 * Purpose: Data Connectivity Services (DCS) device state change event handler
 *
 * Parms:   (IN)    devstate    - device state
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   DCS is a pseudo-QMI service managed by the SDK in order
 *          to support the Device Connectivity APIs. Refer to
 *          QCT document 80-VF219-N.
 *
 **************/
package void
qm_dcs_dev_state_change_notify(
    swi_uint8 devstate )
{
    struct qm_qmi_response_tlvs_values rsptlvs;
    rsptlvs.tlvvalues.qmdcstlvs.devstatechgnotif.devstate = devstate;

    struct qmTlvBuilderItem map[] =
    {
        {   eTLV_DCS_IND_DEVICE_STATE, qm_dcs_device_state_pack_tlv },
        {   eTLV_TYPE_INVALID, NULL }
    };

    qm_dcs_event_notification_send( map, &rsptlvs );
}

/*************
 *
 * Name:    qm_dcs_get_device_mode_pack_tlv
 *
 * Purpose: Pack device state notification TLV Value
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
qm_dcs_get_device_mode_pack_tlv(
    swi_uint8 *pdest,
    swi_uint8 *psrc )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_dcs_device_state_change_tlv_values *pval;

    pin  = (struct qm_qmi_response_tlvs_values *)psrc;
    pval = (struct qm_dcs_device_state_change_tlv_values *)
                            &pin->tlvvalues.qmdcstlvs.devicestate;

    return PutByte( pdest, pval->devstate );
}

/*************
 *
 * Name:    qm_dcs_get_device_mode_send_response
 *
 * Purpose: Pack and send a DCS GetDeviceMode response
 *
 * Parms:   (IN)    msgid       - QMI DCS message ID
 *          (OUT)   prsptlvs    - response TLV structure
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   1. DCS is a pseudo-QMI service managed by the SDK in order to
 *             support the Device Connectivity APIs. Refer to QCT document
 *             80-VF219-N.
 *
 **************/
local void
qm_dcs_get_device_mode_send_response(
    enum eQMIMessageDCS msgid,
    struct qm_qmi_response_tlvs_values *prsptlvs )
{
    struct qmTlvBuilderItem map[] =
    {
        {   eTLV_RESULT_CODE,
            qm_result_code_tlv_pack },

        {   eTLV_DCS_GET_DEVICE_MODE,
            qm_dcs_get_device_mode_pack_tlv
        },

        {   eTLV_TYPE_INVALID,
            NULL
        }
    };

    /* construct and send response to the application */
    qm_qmisvc_send_response( &qmidcsresp,
                             prsptlvs,
                             map,
                             eQMI_SVC_DCS,
                             msgid,
                             eQMIRES );
}

/*************
 *
 * Name:    qm_dcs_get_device_mode
 *
 * Purpose: Service SLQSGetDeviceMode API request
 *
 * Parms:   (IN)    preq - request packet pointer
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   1. This may be called as soon as the SDK has started up. The device
 *             may not be in a state to respond to qm_dcs_enumerate_devices.
 *          2. DCS is a pseudo-QMI service managed by the SDK in order to
 *             support the Device Connectivity APIs. Refer to QCT document
 *             80-VF219-N.
 *          3. This is an internal API that is exposed to the user.
 *
 **************/
local void
qm_dcs_get_device_mode(
    swi_uint8 *preq )
{
    struct qmtcb *qmcbp = qmgetcbp();
    /* for packing response */
    struct qm_qmi_response_tlvs_values rsptlvs;

    slmemset ( (char*)&rsptlvs,
               EOS,
               sizeof (struct qm_qmi_response_tlvs_values));

    /* get device mode */
    rsptlvs.tlvvalues.qmdcstlvs.devicestate.devstate = qmcbp->qmidevicestate;

    rsptlvs.qmiresult.result = QMI_RC_SUCCESS;
    rsptlvs.qmiresult.error  = eQCWWAN_ERR_NONE;

    qm_dcs_get_device_mode_send_response( (enum eQMIMessageDCS)*preq,
                                              &rsptlvs );
}

/*************
 *
 * Name:    qm_dcs_handler
 *
 * Purpose: Data Connectivity Services (DCS) request handler
 *
 * Parms:   (IN)  preq - request packet pointer
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   DCS is a pseudo-QMI service managed by the SDK in order
 *          to support the Device Connectivity APIs. Refer to
 *          QCT document 80-VF219-N.
 *
 **************/
package void
qm_dcs_handler(
    swi_uint8 *preq )
{
    enum eQMIMessageDCS request = *preq;
    struct qmdcstcb *dcscbp = qmdcsgetcbp();

    qmidcsresp.ctlflgs = eQMIRES;
    qmidcsresp.xactionid = dcscbp->xactionid;

    if( (swi_uint8)request < sizeof(dcshandlertbl)/sizeof(dcshandlertbl[0]) )
    {
        dcshandlertbl[*preq](preq);
    }
    else
    {
        qmshortresp( eQCWWAN_ERR_INTERNAL, dcscbp->qmdcsipcchannel );
    }
}

/*************
 *
 * Name:    qmdcsinit
 *
 * Purpose: Initialize DCS QMI service
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
package void qmdcsinit()
{
    swi_bool releaseevt;    /* Some event blocks should not be released */
    swi_uint32 rmsg;        /* ptr to incoming QMI response from modem */
    struct qmdcsevtblock *eventbkp = NULL;

    /* Define and allocate the task control block */
    struct qmdcstcb *dcscbp = qmdcsgetcbp();

    /* Create the Event Pool for events received by DCS task */
    mmpcreate( &dcscbp->qmdcsevtpool,
               "DCSEVTPOOL",
               DCSEVTPOOLSZ,
               sizeof(struct qmdcsevtblock) );

    /* Prepare for logging */
    dlregister( "QM-DCS", &dcscbp->qmdcsdlcb, TRUE );

    /* Initialize the SMT Message queue structure */
    icsmt_create( &dcscbp->qmdcsicmsgque, "DCSMESSAGEQ", DCSMSGDEPTH );

    /* Initialize QMI response structure */
    qmidcsresp.svc = eQMI_SVC_DCS;

    /* Main line of processing begins next */
    for(;;)
    {
        /* Reset the release event block flag */
        releaseevt = TRUE;

        /* Wait for an incoming message then dispatch it */
        icsmt_rcv( &dcscbp->qmdcsicmsgque,
                   QMNOTIMEOUT,
                   &rmsg );
        if(rmsg)
        {
            /* Cast the data to an event block pointer */
            eventbkp = (struct qmdcsevtblock *)rmsg;
            dcscbp->qmdcsipcchannel = eventbkp->qmdcsevtipcchan;
            dcscbp->xactionid       = eventbkp->xactionid;

            /* Log the DCS request details */
            dlLog2( &dcscbp->qmdcsdlcb, QMLOG_CLASSC,
                   "SDK->DCS: Request Received : ipcch/xactionid: %d/%04x",
                   (swi_uint32)dcscbp->qmdcsipcchannel,
                   (swi_uint32)dcscbp->xactionid );

            /* Dispatch the appropriate DCS handler */
            qm_dcs_handler (eventbkp->qmdcsevtdatap);
        }
        else
        {
            releaseevt = FALSE;
        }

        /* Message has been handled, release the associated resources */
        if( releaseevt )
            qmdcsrelevtbk( eventbkp );
    }
}
