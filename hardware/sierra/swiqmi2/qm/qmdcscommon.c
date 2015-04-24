/*************
 *
 * Filename: qmdcscommon.c
 *
 * Purpose:  QMI Device Connectivity Service utilities common to application and SDK
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/

/*-------------
  include files
 --------------*/
#include "aa/aaglobal.h"
#include "qm/qmidefs.h"
#include "qm/qmerrno.h"
#include "sl/sludefs.h"
#include "qm/qmqmisvc.h"

/*---------
  Functions
 ----------*/

/*************
 *
 * Name:    qm_dcs_enumerate_device_key_unpack_tlv
 *
 * Purpose: Unpack device key TLV Value
 *
 * Parms:   (IN)    psource - source data
 *          (OUT)   pdest   - destination buffer
 *
 * Return:  eQCWWAN_ERR_NONE on success
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_dcs_enumerate_device_key_unpack_tlv(
    swi_uint8 *psource,
    swi_uint8 *pdest )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_dcs_enumerate_devices_response_tlv_values *pval;

    pin = (struct qm_qmi_response_tlvs_values *)pdest;
    pval = (struct qm_dcs_enumerate_devices_response_tlv_values *)&pin->tlvvalues.qmdcstlvs.enumerate;

    pval->dev.devkeystrsize = sizeof(pval->dev.devkey);
    slmemset( (char *)pval->dev.devkey,0, sizeof(pval->dev.devkey) );
    return GetStream( psource,
                      pval->dev.devkey,
                      sizeof(pval->dev.devkey) - 1 );
}

/*************
 *
 * Name:    qm_dcs_enumerate_device_node_unpack_tlv
 *
 * Purpose: Unpack device node TLV Value
 *
 * Parms:   (IN)    psource - source data
 *          (OUT)   pdest   - destination storage
 *
 * Return:  eQCWWAN_ERR_NONE on success
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_dcs_enumerate_device_node_unpack_tlv(
    swi_uint8 *psource,
    swi_uint8 *pdest )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_dcs_enumerate_devices_response_tlv_values *pval;

    pin = (struct qm_qmi_response_tlvs_values *)pdest;
    pval = (struct qm_dcs_enumerate_devices_response_tlv_values *)&pin->tlvvalues.qmdcstlvs.enumerate;

    enum eQCWWANError rc;
    rc = qmQmiExtractString( psource,
                             (swi_char *)pval->dev.devnode,
                             sizeof(pval->dev.devnode) );

    pval->dev.devnodestrsize =
    rc == eQCWWAN_ERR_NONE
    ? slstrlen((char *)pval->dev.devnode) + 1
    : 0;

    return rc;
}

/*************
 *
 * Name:    qm_dcs_enumerate_devices_response_unpack
 *
 * Purpose: Unpack DCS enumerate response
 *
 * Parms:   (IN)    prsp        - response data
 *          (OUT)   prsptlvs    - destination response parameters structure
 *
 * Return:  eQCWWAN_ERR_NONE on success
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
global enum eQCWWANError
qm_dcs_enumerate_devices_response_unpack(
    swi_uint8 *prsp,
    struct qm_qmi_response_tlvs_values *prsptlvs )
{
    struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,
          qm_result_code_tlv_unpack },

        { eTLV_DCS_ENUMERATE_DEVICE_NODE,
          qm_dcs_enumerate_device_node_unpack_tlv },

        { eTLV_DCS_ENUMERATE_DEVICE_KEY,
          qm_dcs_enumerate_device_key_unpack_tlv },

        { eTLV_TYPE_INVALID, NULL }
    };

    return qmunpack( prsp,
                     (swi_uint8 *)prsptlvs,
                     map,
                     eQMI_DCS_ENUMERATE_DEVICES );
}

/*************
 *
 * Name:    qm_dcs_connect_device_response_unpack
 *
 * Purpose: Unpack DCS connect response
 *
 * Parms:   (IN)    prsp        - response data
 *          (OUT)   prsptlvs    - destination response parameters structure
 *
 * Return:  eQCWWAN_ERR_NONE on success
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
global enum eQCWWANError
qm_dcs_connect_response_unpack(
    swi_uint8 *prsp,
    struct qm_qmi_response_tlvs_values *prsptlvs )
{
    struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,
          qm_result_code_tlv_unpack },

        { eTLV_TYPE_INVALID, NULL }
    };

    return qmunpack( prsp,
                     (swi_uint8 *)prsptlvs,
                     map,
                     eQMI_DCS_CONNECT );
}

/*************
 *
 * Name:    qm_dcs_connect_device_key_pack_tlv
 *
 * Purpose: Pack device key TLV Value
 *
 * Parms:   (OUT)   pdest   - destination buffer
 *          (IN)    pparm   - source data
 *
 * Return:  eQCWWAN_ERR_NONE
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_dcs_connect_device_key_pack_tlv(
    swi_uint8 *pdest,
    swi_uint8 *psource )
{
    struct qm_dcs_connect_request_args *pin;
    pin = (struct qm_dcs_connect_request_args *)psource;
    return PutStream( pdest,
                      (swi_uint8 *)pin->pdevkey,
                      QMI_MEID_BUF_SIZE - 1 );
}

/*************
 *
 * Name:    qm_dcs_connect_device_node_pack_tlv
 *
 * Purpose: Pack device node TLV Value
 *
 * Parms:   (OUT)   pdest   - destination buffer
 *          (IN)    psrc    - source data
 *
 * Return:  eQCWWAN_ERR_NONE
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_dcs_connect_device_node_pack_tlv(
    swi_uint8 *pdest,
    swi_uint8 *psource )
{
    struct qm_dcs_connect_request_args *pin;
    pin = (struct qm_dcs_connect_request_args *)psource;
    return PutStream( pdest,
                      (swi_uint8 *)pin->pdevnode,
                      slstrlen(pin->pdevnode) + 1 );
}

/*************
 *
 * Name:    qm_dcs_connect_request_pack
 *
 * Purpose: Pack  DCS Connect request
 *
 * Parms:   (IN)    parg      - source device path buffer
 *          (IN)    pdevkey   - source device key buffer
 *          (OUT)   preq      - outgoing request buffer
 *          (OUT)   plen      - length of packed request
 *
 * Return:  eQCWWAN_ERR_NONE on success
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
global enum eQCWWANError
qm_dcs_connect_request_pack(
    swi_char *pdevnode,
    swi_char *pdevkey,
    swi_uint8 *preqbuf,
    swi_uint16 *plen )
{
    struct qmTlvBuilderItem map[] =
    {
        { eTLV_DCS_CONNECT_DEVICE_NODE,
          qm_dcs_connect_device_node_pack_tlv },

        { eTLV_DCS_CONNECT_DEVICE_KEY,
          qm_dcs_connect_device_key_pack_tlv },

        { eTLV_TYPE_INVALID, NULL }
    };

    /* QMI request packing input arguments */
    struct qm_dcs_connect_request_args arg;
    arg.pdevnode = pdevnode;
    arg.pdevkey = pdevkey;

    /* pack QMI request message */
    enum eQCWWANError rc;
    rc = qmbuild( preqbuf,
                  (swi_uint8 *)&arg,
                  map,
                  eQMI_DCS_CONNECT,
                  plen );

    return rc;
}

/*************
 *
 * Name:    qm_dcs_get_connected_device_id_response_unpack
 *
 * Purpose: Unpack DCS connected device id response
 *
 * Parms:   (IN)    prsp        - source response data
 *          (OUT)   prsptlvs    - structure into which the response TLV data is
 *                                unpacked.
 *
 * Return:  eQCWWAN_ERR_NONE on success
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
global enum eQCWWANError
qm_dcs_get_connected_device_id_response_unpack(
    swi_uint8 *prsp,
    struct qm_qmi_response_tlvs_values *prsptlvs )
{
    struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,
          qm_result_code_tlv_unpack },

        { eTLV_DCS_CONNECTED_DEVICEID_DEVICE_NODE,
          qm_dcs_enumerate_device_node_unpack_tlv },

        { eTLV_DCS_CONNECT_DEVICE_KEY,
          qm_dcs_enumerate_device_key_unpack_tlv },

        { eTLV_TYPE_INVALID, NULL }
    };

    return qmunpack( prsp,
                     (swi_uint8 *)prsptlvs,
                     map,
                     eQMI_DCS_GET_CONNECTED_DEVICE_ID );
}

/*************
 *
 * Name:    qm_dcs_get_usb_port_names_atcmdport_unpack_tlv
 *
 * Purpose: Unpack AT command port name TLV Value
 *
 * Parms:   (IN)    psource - source data
 *          (OUT)   pdest   - destination storage
 *
 * Return:  eQCWWAN_ERR_NONE on success
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_dcs_get_usb_port_names_atcmdport_unpack_tlv(
    swi_uint8 *psource,
    swi_uint8 *pdest )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_dcs_usb_port_names_response_tlv_values *pval;

    pin = (struct qm_qmi_response_tlvs_values *)pdest;
    pval = (struct qm_dcs_usb_port_names_response_tlv_values *)&pin->tlvvalues.qmdcstlvs.portnames;

    enum eQCWWANError rc;
    rc = qmQmiExtractString( psource,
                             (swi_char *)pval->usbportnames.AtCmdPort,
                             sizeof(pval->usbportnames.AtCmdPort) );

    return rc;
}
/*************
 *
 * Name:    qm_dcs_get_usb_port_names_nmeaport_unpack_tlv
 *
 * Purpose: Unpack NMEA port name TLV Value
 *
 * Parms:   (IN)    psource - source data
 *          (OUT)   pdest   - destination storage
 *
 * Return:  eQCWWAN_ERR_NONE on success
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_dcs_get_usb_port_names_nmeaport_unpack_tlv(
    swi_uint8 *psource,
    swi_uint8 *pdest )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_dcs_usb_port_names_response_tlv_values *pval;

    pin = (struct qm_qmi_response_tlvs_values *)pdest;
    pval = (struct qm_dcs_usb_port_names_response_tlv_values *)&pin->tlvvalues.qmdcstlvs.portnames;

    enum eQCWWANError rc;
    rc = qmQmiExtractString( psource,
                             (swi_char *)pval->usbportnames.NmeaPort,
                             sizeof(pval->usbportnames.NmeaPort) );

    return rc;
}
/*************
 *
 * Name:    qm_dcs_get_usb_port_names_dmport_unpack_tlv
 *
 * Purpose: Unpack DM port name TLV Value
 *
 * Parms:   (IN)    psource - source data
 *          (OUT)   pdest   - destination storage
 *
 * Return:  eQCWWAN_ERR_NONE on success
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_dcs_get_usb_port_names_dmport_unpack_tlv(
    swi_uint8 *psource,
    swi_uint8 *pdest )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_dcs_usb_port_names_response_tlv_values *pval;

    pin = (struct qm_qmi_response_tlvs_values *)pdest;
    pval = (struct qm_dcs_usb_port_names_response_tlv_values *)&pin->tlvvalues.qmdcstlvs.portnames;

    enum eQCWWANError rc;
    rc = qmQmiExtractString( psource,
                             (swi_char *)pval->usbportnames.DmPort,
                             sizeof(pval->usbportnames.DmPort) );

    return rc;
}

/*************
 *
 * Name:    qm_dcs_get_usb_port_names_response_unpack
 *
 * Purpose: Unpack DCS SLQSGetUSBPortNames response
 *
 * Parms:   (IN)    prsp        - response data
 *          (OUT)   prsptlvs    - destination response parameters structure
 *
 * Return:  eQCWWAN_ERR_NONE on success
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
global enum eQCWWANError
qm_dcs_get_usb_port_names_response_unpack(
    swi_uint8 *prsp,
    struct qm_qmi_response_tlvs_values *prsptlvs )
{
    struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,
          qm_result_code_tlv_unpack },

        { eTLV_DCS_GET_USB_PORT_NAME_ATCMDPORT,
          qm_dcs_get_usb_port_names_atcmdport_unpack_tlv
        },

        { eTLV_DCS_GET_USB_PORT_NAME_NMEAPORT,
          qm_dcs_get_usb_port_names_nmeaport_unpack_tlv
        },

        { eTLV_DCS_GET_USB_PORT_NAME_DMPORT,
          qm_dcs_get_usb_port_names_dmport_unpack_tlv
        },

        { eTLV_TYPE_INVALID, NULL }
    };

    return qmunpack( prsp,
                     (swi_uint8 *)prsptlvs,
                     map,
                     eQMI_DCS_GET_USB_PORT_NAMES );
}

/*************
 *
 * Name:    qm_dcs_get_device_mode_unpack_tlv
 *
 * Purpose: Unpack Device Mode TLV Value
 *
 * Parms:   (IN)    psource - source data
 *          (OUT)   pdest   - destination storage
 *
 * Return:  eQCWWAN_ERR_NONE on success
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_dcs_get_device_mode_unpack_tlv(
    swi_uint8 *psource,
    swi_uint8 *pdest )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_dcs_device_state_change_tlv_values *pval;

    pin = (struct qm_qmi_response_tlvs_values *)pdest;
    pval = (struct qm_dcs_device_state_change_tlv_values *)&pin->tlvvalues.qmdcstlvs.devicestate;

    enum eQCWWANError rc;
    rc = GetByte( psource, &(pval->devstate));

    return rc;
}

/*************
 *
 * Name:    qm_dcs_get_device_mode_response_unpack
 *
 * Purpose: Unpack DCS SLQSGetDeviceMode response
 *
 * Parms:   (IN)    prsp        - response data
 *          (OUT)   prsptlvs    - destination response parameters structure
 *
 * Return:  eQCWWAN_ERR_NONE on success
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
global enum eQCWWANError
qm_dcs_get_device_mode_response_unpack(
    swi_uint8 *prsp,
    struct qm_qmi_response_tlvs_values *prsptlvs )
{
    struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,
          qm_result_code_tlv_unpack },

        { eTLV_DCS_GET_DEVICE_MODE,
          qm_dcs_get_device_mode_unpack_tlv
        },

        { eTLV_TYPE_INVALID, NULL }
    };

    return qmunpack( prsp,
                     (swi_uint8 *)prsptlvs,
                     map,
                     eQMI_DCS_GET_DEVICE_MODE);
}

