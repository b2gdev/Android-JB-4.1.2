/*************
 *
 * Filename: qmdcs.h
 *
 * Purpose:  QMI Device Connectivity Service user types
 *
 * Copyright: © 2010 Sierra Wireless Inc., all rights reserved
 *
 **************/

#ifndef QMDCS_H
#define QMDCS_H

#include "os/swi_ossdk.h"
#include "qm/qmudefs.h"

/* User defined types */
/*************
 *
 * Name:    qm_dcs_deviceinfo
 *
 * Purpose: QMI device information
 *
 * Members: pdevnode        - QMI device path
 *          pdevkey         - connected device ID
 *          devnodestrsize  - length of QMI device path
 *          devkeystrsize   - length of device key
 *
 * Notes:   none
 *
 **************/
struct qm_dcs_deviceinfo{
    swi_uint8  devnode[QMI_DEVICE_PATH_MAX_SIZE];
    swi_uint8  devkey[QMI_MEID_BUF_SIZE];
    swi_uint16 devnodestrsize;
    swi_uint16 devkeystrsize;
};

/*************
 *
 * Name:    qm_dcs_usb_port_names
 *
 * Purpose: QMI USB port names information
 *
 * Members: AtCmdPort - AT cmd port name
 *          NmeaPort  - NMEA port name
 *          DmPort    - DM port name
 *
 * Notes:   none
 *
 **************/
struct qm_dcs_usb_port_names
{
    swi_char AtCmdPort[QMI_USB_PORT_NAME_SIZE];
    swi_char NmeaPort [QMI_USB_PORT_NAME_SIZE];
    swi_char DmPort   [QMI_USB_PORT_NAME_SIZE];
};

/*************
 *
 * Name:    qm_dcs_device_state_change_tlv_values
 *
 * Purpose: QMI DCS notification structure used for building the QMI
 *          notification being sent to the user application on a device state
 *          change or in response to the SLQSGetDeviceMode API.
 *
 * Members: devstate   - device state
 *
 * Notes:
 *
 **************/
struct qm_dcs_device_state_change_tlv_values
{
    swi_uint8   devstate;
};

/*************
 *
 * Name:    qm_dcs_sdk_terminated_tlv_values
 *
 * Purpose: QMI SDK notification structure used for building the QMI
 *          notification being sent to the user application in response to the
 *          SDK termination events.
 *
 * Members: reason   - String containing reason
 *
 * Notes:
 *
 **************/
struct qm_dcs_sdk_terminated_tlv_values
{
    swi_uint8   reason[128];;
};


/*************
 *
 * Name:    qm_dcs_cancel_response_parms
 *
 * Purpose: QMI DCS Cancel Response Structure used for building
 *          the QMI response returned to the user application making the
 *          request.
 *
 * Members: none
 *
 * Notes:
 *
 **************/
struct qm_dcs_cancel_response_tlv_values{
};

/*************
 *
 * Name:    qm_dcs_connected_deviceid_response_tlv_values
 *
 * Purpose: QMI DCS Connected Device ID Response Structure used for building
 *          the QMI response returned to the user application making the
 *          request.
 *
 * Members: (IN) pdevice    - device information
 *
 * Notes:
 *
 **************/
struct qm_dcs_connected_deviceid_response_tlv_values{
    struct qm_dcs_deviceinfo *pdevice;
};

/*************
 *
 * Name:    qm_dcs_disconnect_response_tlv_values
 *
 * Purpose: QMI DCS  Disconnect Response Structure used for building
 *          the QMI response returned to the user application making the
 *          request.
 *
 * Members: none
 *
 * Notes:
 *
 **************/
struct qm_dcs_disconnect_response_tlv_values{
};

/*************
 *
 * Name:    qm_dcs_connect_request_args
 *
 * Purpose: QMI device information
 *
 * Members: (IN) pdevnode   - QMI device path
 *          (IN) pdevkey    - connected device ID
 *
 * Notes:   none
 *
 **************/
struct qm_dcs_connect_request_args
{
    swi_char *pdevnode;
    swi_char *pdevkey;
};

/*************
 *
 * Name:    qm_dcs_connect_response_tlv_values
 *
 * Purpose: QMI DCS Connect Response Structure used for building
 *          the QMI response returned to the user application making the
 *          request.
 *
 * Members: dev - device information
 *
 * Notes:
 *
 **************/
struct qm_dcs_connect_response_tlv_values{
    struct qm_dcs_deviceinfo dev;
};

/*************
 *
 * Name:    qm_dcs_enumerate_devices_response_tlv_values
 *
 * Purpose: QMI DCS Enumerate Devices Response Structure used for building
 *          the QMI response returned to the user application making the
 *          request.
 *
 * Notes:
 *
 **************/
struct qm_dcs_enumerate_devices_response_tlv_values{
    struct qm_dcs_deviceinfo dev;
};

/*************
 *
 * Name:    qm_dcs_usb_port_names_response_tlv_values
 *
 * Purpose: QMI DCS Get Usb Port Names Response Structure used for building
 *          the QMI response returned to the user application making the
 *          request.
 *
 * Members: (IN) pusbportnames    - port names
 *
 * Notes:
 *
 **************/
struct qm_dcs_usb_port_names_response_tlv_values{
    struct qm_dcs_usb_port_names usbportnames;
};

/*************
 *
 * Name:    qm_qmi_dcs_tlv_values
 *
 * Purpose: Union of QMI TLV Value Structures to generalize the TLV
 *          construction of QMI DCS service responses.
 *
 * Members: enumerate           - enumerate response structure
 *          connect             - connect response structure
 *          disconnect          - disconnect response structure
 *          connecteddevid      - connecteddevid response structure
 *          cancel              - cancel response structure
 *          devstatechgnotif    - device state change notification
 *          portnames           - USB port names structure
 *          devicestate         - Device mode value
 * Notes:
 *
 **************/
union qm_qmi_dcs_tlv_values{
    struct qm_dcs_enumerate_devices_response_tlv_values     enumerate;
    struct qm_dcs_connect_response_tlv_values               connect;
    struct qm_dcs_disconnect_response_tlv_values            disconnect;
    struct qm_dcs_connected_deviceid_response_tlv_values    connecteddevid;
    struct qm_dcs_cancel_response_tlv_values                cancel;
    struct qm_dcs_device_state_change_tlv_values            devstatechgnotif;
    struct qm_dcs_usb_port_names_response_tlv_values        portnames;
    struct qm_dcs_device_state_change_tlv_values            devicestate;
    struct qm_dcs_sdk_terminated_tlv_values                 sdktermination;

};

#endif /* QMDCS_H */

