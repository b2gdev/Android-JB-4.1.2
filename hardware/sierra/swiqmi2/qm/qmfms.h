/*************
 *
 * Filename: qmfms.h
 *
 * Purpose:  QMI Firmware Management Service user types
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/

#ifndef QMFMS_H
#define QMFMS_H

#include "im/imudefs.h"
#include "os/swi_ossdk.h"
#include "qm/qmudefs.h"

/* Image Path defines */
#define QM_FMS_IMAGE_PATH_MAX_SZ    512
#define QM_FMS_SPKGS_STR_MAX_SZ     84
#define QM_FMS_SPKGS_STR_OFFSET     0x11C

/* CWE Sierra Package String parameter defines */
#define SLQSIMINFO_SKU_SZ           10
#define SLQSIMINFO_PARTNO_SZ        10
#define SLQSIMINFO_MODELID_SZ       10
#define SLQSIMINFO_FWVERSION_SZ     15
#define SLQSIMINFO_BOOTBLK_SZ       2
#define SLQSIMINFO_CARRIER_SZ       15
#define SLQSIMINFO_PRIVERSION_SZ    7
#define SLQSIMINFO_PACKAGEID_SZ     3

enum cwe_spkgs_param_indices{
    SPKGS_SKU_IDX,
    SPKGS_PARTNO_IDX,
    SPKGS_MODELID_IDX,
    SPKGS_FWVERSION_IDX,
    SPKGS_BOOTBLK_IDX,
    SPKGS_CARRIER_IDX,
    SPKGS_PRIVER_IDX,
    SPKGS_PKGID_IDX,
    SPKGS_LAST_IDX,
};

/* User defined types */
/*************
 *
 * Name:    qm_fms_fwdld_completion_status_notif_tlv_values
 *
 * Purpose: QMI FMS notification structure used for building the QMI
 *          notification being sent to the user application on a firmware
 *          download completion event.
 *
 * Members: fwdldstatus - firmware download completion status
 *
 * Notes:
 *
 **************/
struct qm_fms_fwdld_completion_status_notif_tlv_values
{
/* RILSTART */
    swi_uint32 fwdldstatus;
/* RILSTOP */
};

/*************
 *
 * Name:    qm_fms_set_firmware_preference_request_args
 *
 * Purpose: firmware preference request structure
 *
 * Members: none
 *
 * Notes:   none
 *
 **************/
struct qm_fms_set_firmware_preference_request_args{
};

/*************
 *
 * Name:    qm_fms_set_image_path_request_args
 *
 * Purpose: firmware download image path request structure
 *
 * Members: (IN) path   - path to directory containing firmware image
 *
 * Notes:   none
 *
 **************/
struct qm_fms_set_image_path_request_args{
    swi_char *path;
};

/*************
 *
 * Name:    qm_fms_set_image_path_response_tlv_values
 *
 * Purpose: QMI FMS set image path response structure used for building
 *          the QMI response returned to the user application making the
 *          request.
 *
 * Members: none
 *
 * Notes:
 *
 **************/
struct qm_fms_set_image_path_response_tlv_values{
};

/*************
 *
 * Name:    qm_fms_get_cwe_image_info_request_args
 *
 * Purpose: firmware preference request structure
 *
 * Members: none
 *
 * Notes:   none
 *
 **************/
struct qm_fms_get_cwe_image_info_request_args{
    swi_char *path;
};

/*************
 *
 * Name:    qm_fms_set_image_path_request_tlv_values
 *
 * Purpose: QMI FMS set image path request structure used for unpacking
 *          the QMI request sent by the user application.
 *
 * Members: path   - fully qualified path to directory containing firmware image
 *
 * Notes:
 *
 **************/
struct qm_fms_set_image_path_request_tlv_values{
    swi_char path[QM_FMS_IMAGE_PATH_MAX_SZ];
};

/*************
 *
 * Name:    qm_fms_set_image_path_request_tlv_values
 *
 * Purpose: QMI FMS get cwe image spkgs response structure used for building
 *          the QMI response returned to the user application making the
 *          request.
 *
 * Members: modelid_str     - device model id
 *          fwversion_str   - firmware boot/app image version
 *          sku_str         - SKU ID
 *          packageid_str   - CWE Sierra Package ID
 *          carrier_str     - Carrier ID
 *          priversion_str  - PRI version
 *
 * Notes:
 *
 **************/
struct qm_fms_cwe_spkgs_response_tlv_values{
    swi_char modelid_str    [SLQSIMINFO_MODELID_SZ + 1];
    swi_char fwversion_str  [SLQSIMINFO_FWVERSION_SZ + 1];
    swi_char sku_str        [SLQSIMINFO_SKU_SZ + 1];
    swi_char packageid_str  [SLQSIMINFO_PACKAGEID_SZ + 1];
    swi_char carrier_str    [SLQSIMINFO_CARRIER_SZ + 1];
    swi_char priversion_str [SLQSIMINFO_PRIVERSION_SZ + 1];
};

/*************
 *
 * Name:    qm_fms_mbn_response_tlv_values
 *
 * Purpose: QMI FMS get mbn image info response structure used for building
 *          the QMI response returned to the user application making the
 *          request.
 *
 * Members: firmwareID
 *          fwversionstr
 *
 * Notes:
 *
 **************/
struct qm_fms_mbn_response_tlv_values
{
    swi_uint32  firmwareid;
    swi_char    fwidstr[MBN_IMG_ID_STR_LEN];
    swi_char    fwversionstr[MBN_BUILD_ID_STR_LEN];
};

/*************
 *
 * Name:    qm_fms_set_image_path_request_tlv_values
 *
 * Purpose: QMI FMS get cwe image spkgs structure used for unpacking
 *          the QMI request sent by the user application.
 *
 * Members: none
 *
 * Notes:
 *
 **************/
struct qm_fms_cwe_spkgs_request_tlv_values{
    swi_char path[QM_FMS_IMAGE_PATH_MAX_SZ];
};

/*************
 *
 * Name:   qm_fms_mbn_request_tlv_values
 *
 * Purpose: QMI FMS get mbn image structure used for unpacking
 *          the QMI request sent by the user application.
 *
 * Members:
 *
 * Notes:
 *
 **************/
struct qm_fms_mbn_request_tlv_values{
    swi_char path[QM_FMS_IMAGE_PATH_MAX_SZ];
    swi_uint8 imgtype;
};

/*************
 *
 * Name:    qm_qmi_fms_tlv_values
 *
 * Purpose: Union of QMI TLV Value Structures to generalize the TLV
 *          construction of QMI FMS service responses.
 *
 * Members: set_image_path_req  - set imgae path request structure
 *          set_image_path_rsp  - set imgae path response structure
 *          cwe_spkgs_req       - get cwe spkgs info request structure
 *          cwe_spkgs_rsp       - get cwe spkgs info response structure
 *
 * Notes:
 *
 **************/
union qm_qmi_fms_tlv_values{
    struct qm_fms_set_image_path_request_tlv_values     set_image_path_req;
    struct qm_fms_set_image_path_response_tlv_values    set_image_path_rsp;
    struct qm_fms_cwe_spkgs_request_tlv_values          cwe_spkgs_req;
    struct qm_fms_cwe_spkgs_response_tlv_values         cwe_spkgs_rsp;
    struct qm_fms_mbn_request_tlv_values                mbn_req;
    struct qm_fms_mbn_response_tlv_values               mbn_rsp;
    struct qm_fms_fwdld_completion_status_notif_tlv_values  fwdldnotif;
};

#endif /* QMFMS_H */
