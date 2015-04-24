/*************
 *
 * Filename: qmfmscommon.c
 *
 * Purpose:  QMI Firmware Management Service utilities common to application and SDK
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
#include "os/swi_osapi.h"

/*---------
  Functions
 ----------*/

/*************
 *
 * Name:    qm_fms_set_image_path_pack_tlv
 *
 * Purpose: Pack fully qualified image path
 *
 * Parms:   (IN)    psource - source data
 *          (OUT)   pdest   - destination buffer
 *
 * Return:  eQCWWAN_ERR_NONE
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_fms_set_image_path_pack_tlv(
    swi_uint8 *pdest,
    swi_uint8 *psource )
{
    struct qm_fms_set_image_path_request_args *pin;
    pin = (struct qm_fms_set_image_path_request_args *)psource;
    return PutStream( pdest,
                      (swi_uint8 *)pin->path,
                      slstrlen(pin->path) + 1 );
}

/*************
 *
 * Name:    qm_fms_set_firmware_preference_request_pack
 *
 * Purpose: Pack FMS set firmware preference request
 *
 * Parms:   (IN)  path  - fully qualified path
 *          (OUT) preq  - outgoing request buffer
 *          (OUT) plen  - length of packed request
 *
 * Return:  eQCWWAN_ERR_NONE on success
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
global enum eQCWWANError
qm_fms_set_firmware_preference_request_pack(
    swi_uint8   *preq,
    swi_uint16  *plen )
{
    struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID, NULL }
    };

    /* QMI request packing input arguments */
    struct qm_fms_set_firmware_preference_request_args arg;

    /* pack QMI request message */
    enum eQCWWANError rc;
    rc = qmbuild( preq,
                  (swi_uint8 *)&arg,
                  map,
                  eQMI_DMS_SET_FIRMWARE_ID,
                  plen );

    return rc;
}

/*************
 *
 * Name:    qm_fms_set_image_path_request_pack
 *
 * Purpose: Pack FMS set image path request
 *
 * Parms:   (IN)  path  - fully qualified path
 *          (OUT) preq  - outgoing request buffer
 *          (OUT) plen  - length of packed request
 *
 * Return:  eQCWWAN_ERR_NONE on success
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
global enum eQCWWANError
qm_fms_set_image_path_request_pack(
    swi_char    *path,
    swi_uint8   *preq,
    swi_uint16  *plen )
{
    struct qmTlvBuilderItem map[] =
    {
        { eTLV_FMS_IMAGE_PATH,
          qm_fms_set_image_path_pack_tlv },

        { eTLV_TYPE_INVALID, NULL }
    };

    /* QMI request packing input arguments */
    struct qm_fms_set_image_path_request_args arg;
    arg.path = path;

    /* pack QMI request message */
    enum eQCWWANError rc;
    rc = qmbuild( preq,
                  (swi_uint8 *)&arg,
                  map,
                  eQMI_FMS_SET_IMAGE_PATH,
                  plen );

    return rc;
}

/*************
 *
 * Name:    qm_fms_get_cwe_spkgs_info_priversion_tlv_unpack
 *
 * Purpose: unpack PRI version TLV Value
 *
 * Parms:   (IN)    psrc    - src data
 *          (OUT)   pdest   - destination buffer
 *
 * Return:  success: eQCWWAN_ERR_NONE
 *          failure: eQCWWAN_ERR_BUFFER_SZ or eQCWWAN_ERR_MEMORY
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_fms_get_cwe_spkgs_info_priversion_tlv_unpack(
    swi_uint8 *psrc,
    swi_uint8 *pdest )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_fms_cwe_spkgs_response_tlv_values *ptlv;

    pin = (struct qm_qmi_response_tlvs_values *)pdest;

    ptlv = (struct qm_fms_cwe_spkgs_response_tlv_values *)
                        &pin->tlvvalues.qmfmstlvs.cwe_spkgs_rsp;

    return qmQmiExtractString( psrc,
                        ptlv->priversion_str,
                        sizeof(ptlv->priversion_str) );
}

/*************
 *
 * Name:    qm_fms_get_cwe_spkgs_info_carrier_tlv_unpack
 *
 * Purpose: unpack carrier TLV Value
 *
 * Parms:   (IN)    psrc    - src data
 *          (OUT)   pdest   - destination buffer
 *
 * Return:  success: eQCWWAN_ERR_NONE
 *          failure: eQCWWAN_ERR_BUFFER_SZ or eQCWWAN_ERR_MEMORY
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_fms_get_cwe_spkgs_info_carrier_tlv_unpack(
    swi_uint8 *psrc,
    swi_uint8 *pdest )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_fms_cwe_spkgs_response_tlv_values *ptlv;

    pin = (struct qm_qmi_response_tlvs_values *)pdest;
    ptlv = (struct qm_fms_cwe_spkgs_response_tlv_values *)
                        &pin->tlvvalues.qmfmstlvs.cwe_spkgs_rsp;

    return qmQmiExtractString( psrc,
                        ptlv->carrier_str,
                        sizeof(ptlv->carrier_str) );
}

/*************
 *
 * Name:    qm_fms_get_cwe_spkgs_info_pkgversion_tlv_unpack
 *
 * Purpose: unpack sierra package version TLV Value
 *
 * Parms:   (IN)    psrc    - src data
 *          (OUT)   pdest   - destination buffer
 *
 * Return:  success: eQCWWAN_ERR_NONE
 *          failure: eQCWWAN_ERR_BUFFER_SZ or eQCWWAN_ERR_MEMORY
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_fms_get_cwe_spkgs_info_pkgversion_tlv_unpack(
    swi_uint8 *psrc,
    swi_uint8 *pdest )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_fms_cwe_spkgs_response_tlv_values *ptlv;

    pin = (struct qm_qmi_response_tlvs_values *)pdest;
    ptlv = (struct qm_fms_cwe_spkgs_response_tlv_values *)
                        &pin->tlvvalues.qmfmstlvs.cwe_spkgs_rsp;

    return qmQmiExtractString( psrc,
                               ptlv->packageid_str,
                               sizeof(ptlv->packageid_str) );
}

/*************
 *
 * Name:    qm_fms_get_cwe_spkgs_info_skuid_tlv_unpack
 *
 * Purpose: unpack SKU ID TLV Value
 *
 * Parms:   (IN)    psrc    - src data
 *          (OUT)   pdest   - destination buffer
 *
 * Return:  success: eQCWWAN_ERR_NONE
 *          failure: eQCWWAN_ERR_BUFFER_SZ or eQCWWAN_ERR_MEMORY
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_fms_get_cwe_spkgs_info_skuid_tlv_unpack(
    swi_uint8 *psrc,
    swi_uint8 *pdest )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_fms_cwe_spkgs_response_tlv_values *ptlv;

    pin = (struct qm_qmi_response_tlvs_values *)pdest;
    ptlv = (struct qm_fms_cwe_spkgs_response_tlv_values *)
                        &pin->tlvvalues.qmfmstlvs.cwe_spkgs_rsp;

    return qmQmiExtractString( psrc,
                        ptlv->sku_str,
                        sizeof(ptlv->sku_str) );
}

/*************
 *
 * Name:    qm_fms_get_cwe_spkgs_info_fwversion_tlv_unpack
 *
 * Purpose: unpack firmware version TLV Value
 *
 * Parms:   (IN)    psrc    - src data
 *          (OUT)   pdest   - destination buffer
 *
 * Return:  success: eQCWWAN_ERR_NONE
 *          failure: eQCWWAN_ERR_BUFFER_SZ or eQCWWAN_ERR_MEMORY
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_fms_get_cwe_spkgs_info_fwversion_tlv_unpack(
    swi_uint8 *psrc,
    swi_uint8 *pdest )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_fms_cwe_spkgs_response_tlv_values *ptlv;

    pin = (struct qm_qmi_response_tlvs_values *)pdest;
    ptlv = (struct qm_fms_cwe_spkgs_response_tlv_values *)
                        &pin->tlvvalues.qmfmstlvs.cwe_spkgs_rsp;

    return qmQmiExtractString( psrc,
                               ptlv->fwversion_str,
                               sizeof(ptlv->fwversion_str) );
}

/*************
 *
 * Name:    qm_fms_get_cwe_spkgs_info_modelid_tlv_unpack
 *
 * Purpose: unpack modelid TLV Value
 *
 * Parms:   (IN)    psrc    - src data
 *          (OUT)   pdest   - destination buffer
 *
 * Return:  success: eQCWWAN_ERR_NONE
 *          failure: eQCWWAN_ERR_BUFFER_SZ or eQCWWAN_ERR_MEMORY
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_fms_get_cwe_spkgs_info_modelid_tlv_unpack(
    swi_uint8 *psrc,
    swi_uint8 *pdest )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_fms_cwe_spkgs_response_tlv_values *ptlv;

    pin = (struct qm_qmi_response_tlvs_values *)pdest;

    ptlv = (struct qm_fms_cwe_spkgs_response_tlv_values *)
                        &pin->tlvvalues.qmfmstlvs.cwe_spkgs_rsp;

    return qmQmiExtractString( psrc,
                               ptlv->modelid_str,
                               sizeof(ptlv->modelid_str) );
}

/*************
 *
 * Name:    qm_fms_get_cwe_spkgs_info_response_unpack
 *
 * Purpose: Unpack FMS get cwe Sierra Package String (spkgs) info response
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
qm_fms_get_cwe_spkgs_info_response_unpack(
    swi_uint8 *prsp,
    struct qm_qmi_response_tlvs_values *prsptlvs )
{
    static struct qmTlvUnpackerItem map[] =
    {
        {   eTLV_RESULT_CODE,
            &qm_result_code_tlv_unpack },

        {   eTLV_FMS_GET_CWE_SPKGS_INFO_SKUID_STR,
            &qm_fms_get_cwe_spkgs_info_skuid_tlv_unpack },
        {
            eTLV_FMS_GET_CWE_SPKGS_INFO_MODELID_STR,
            &qm_fms_get_cwe_spkgs_info_modelid_tlv_unpack },

        {   eTLV_FMS_GET_CWE_SPKGS_INFO_FWVERSION_STR,
            &qm_fms_get_cwe_spkgs_info_fwversion_tlv_unpack },

        {   eTLV_FMS_GET_CWE_SPKGS_INFO_CARRIER_STR,
            &qm_fms_get_cwe_spkgs_info_carrier_tlv_unpack },

        {   eTLV_FMS_GET_CWE_SPKGS_INFO_PRIVERSION_STR,
            &qm_fms_get_cwe_spkgs_info_priversion_tlv_unpack },

        {   eTLV_FMS_GET_CWE_SPKGS_INFO_PKGVERSION_STR,
            &qm_fms_get_cwe_spkgs_info_pkgversion_tlv_unpack },

        {   eTLV_TYPE_INVALID,
                NULL } /* sentinal signifies last item in map */
    };

    return qmunpack( prsp,
                     (swi_uint8 *)prsptlvs,
                     map,
                     eQMI_FMS_GET_CWE_SPKGS_INFO );
}

/*************
 *
 * Name:    qm_fms_get_cwe_image_info_imgpath_tlv_pack
 *
 * Purpose: Pack fully qualified image path
 *
 * Parms:   (IN)    psource - source data
 *          (OUT)   pdest   - destination buffer
 *
 * Return:  eQCWWAN_ERR_NONE
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_fms_get_cwe_image_info_imgpath_tlv_pack(
    swi_uint8 *pdest,
    swi_uint8 *psource )
{
    struct qm_fms_cwe_spkgs_request_tlv_values *pin;
    pin = (struct qm_fms_cwe_spkgs_request_tlv_values *)psource;

    return PutStream( pdest,
                      (swi_uint8 *)pin->path,
                      slstrlen(pin->path) + 1 );
}

/*************
 *
 * Name:    qm_fms_get_cwe_spkgs_info_request_pack
 *
 * Purpose: Pack FMS get cwe Sierra Package String (spkgs) info request
 *
 * Parms:   (OUT) preq  - outgoing request buffer
 *          (OUT) plen  - length of packed request
 *
 * Return:  eQCWWAN_ERR_NONE on success
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
global enum eQCWWANError
qm_fms_get_cwe_spkgs_info_request_pack(
    const swi_char  *path,
    swi_uint8       *preq,
    swi_uint16      *plen )
{
    struct qmTlvBuilderItem map[] =
    {
        { eTLV_FMS_IMAGE_PATH,
          qm_fms_get_cwe_image_info_imgpath_tlv_pack },

        { eTLV_TYPE_INVALID, NULL }
    };

    /* QMI request packing input arguments */
    struct qm_fms_cwe_spkgs_request_tlv_values arg;
    slstrncpy( arg.path, path, sizeof(arg.path) );

    /* pack QMI request message */
    enum eQCWWANError rc;
    rc = qmbuild( preq,
                  (swi_uint8 *)&arg,
                  map,
                  eQMI_FMS_GET_CWE_SPKGS_INFO,
                  plen );

    return rc;
}

/*************
 *
 * Name:    qm_fms_get_mbn_info_fwidstr_tlv_unpack
 *
 * Purpose: unpack firmware version string TLV Value
 *
 * Parms:   (IN)    psrc    - src data
 *          (OUT)   pdest   - destination buffer
 *
 * Return:  success: eQCWWAN_ERR_NONE
 *          failure: eQCWWAN_ERR_BUFFER_SZ or eQCWWAN_ERR_MEMORY
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_fms_get_mbn_info_fwidstr_tlv_unpack(
    swi_uint8 *psrc,
    swi_uint8 *pdest )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_fms_mbn_response_tlv_values *ptlv;

    pin = (struct qm_qmi_response_tlvs_values *)pdest;
    ptlv = (struct qm_fms_mbn_response_tlv_values *)
                        &pin->tlvvalues.qmfmstlvs.mbn_rsp;

    return GetStream( psrc,
                      (swi_uint8 *)ptlv->fwidstr,
                      sizeof(ptlv->fwidstr) );
}

/*************
 *
 * Name:    qm_fms_get_mbn_info_fwversionstr_tlv_unpack
 *
 * Purpose: unpack firmware version string TLV Value
 *
 * Parms:   (IN)    psrc    - src data
 *          (OUT)   pdest   - destination buffer
 *
 * Return:  success: eQCWWAN_ERR_NONE
 *          failure: eQCWWAN_ERR_BUFFER_SZ or eQCWWAN_ERR_MEMORY
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_fms_get_mbn_info_fwversionstr_tlv_unpack(
    swi_uint8 *psrc,
    swi_uint8 *pdest )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_fms_mbn_response_tlv_values *ptlv;

    pin = (struct qm_qmi_response_tlvs_values *)pdest;
    ptlv = (struct qm_fms_mbn_response_tlv_values *)
                        &pin->tlvvalues.qmfmstlvs.mbn_rsp;

    return GetStream( psrc,
                      (swi_uint8 *)ptlv->fwversionstr,
                      sizeof(ptlv->fwversionstr) );
}

/*************
 *
 * Name:    qm_fms_get_mbn_info_firmwareid_tlv_unpack
 *
 * Purpose: unpack firmware ID TLV Value
 *
 * Parms:   (IN)    psrc    - src data
 *          (OUT)   pdest   - destination buffer
 *
 * Return:  success: eQCWWAN_ERR_NONE
 *          failure: eQCWWAN_ERR_BUFFER_SZ or eQCWWAN_ERR_MEMORY
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_fms_get_mbn_info_firmwareid_tlv_unpack(
    swi_uint8 *psrc,
    swi_uint8 *pdest )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_fms_mbn_response_tlv_values *ptlv;

    pin = (struct qm_qmi_response_tlvs_values *)pdest;
    ptlv = (struct qm_fms_mbn_response_tlv_values *)
                        &pin->tlvvalues.qmfmstlvs.mbn_rsp;

    return GetLong(psrc, &(ptlv->firmwareid) );
}

/*************
 *
 * Name:    qm_fms_get_mbn_info_response_unpack
 *
 * Purpose: Unpack FMS get mbn Image Package info response
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
qm_fms_get_mbn_info_response_unpack(
    swi_uint8 *prsp,
    struct qm_qmi_response_tlvs_values *prsptlvs )
{
    static struct qmTlvUnpackerItem map[] =
    {
        {   eTLV_RESULT_CODE,
            &qm_result_code_tlv_unpack },

        {   eTLV_FMS_GET_MBN_INFO_FIRMWAREID,
            &qm_fms_get_mbn_info_firmwareid_tlv_unpack },

        {   eTLV_FMS_GET_MBN_INFO_FWVERSION_STR,
            &qm_fms_get_mbn_info_fwversionstr_tlv_unpack },

        {   eTLV_FMS_GET_MBN_INFO_FIRMWAREID_STR,
            &qm_fms_get_mbn_info_fwidstr_tlv_unpack },

        {   eTLV_TYPE_INVALID,
            NULL }  /*sentinal signifies last item in map*/
    };

    return qmunpack( prsp,
                     (swi_uint8 *)prsptlvs,
                     map,
                     eQMI_FMS_GET_MBN_INFO );
}

/*************
 *
 * Name:    qm_fms_get_mbn_image_info_imgtype_tlv_pack
 *
 * Purpose: Pack the image type
 *
 * Parms:   (IN)    psource - source data
 *          (OUT)   pdest   - destination buffer
 *
 * Return:  eQCWWAN_ERR_NONE
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_fms_get_mbn_info_imgtype_tlv_pack(
    swi_uint8 *pdest,
    swi_uint8 *psource )
{
    struct qm_fms_mbn_request_tlv_values *pin;
    pin = (struct qm_fms_mbn_request_tlv_values *)psource;

    return PutByte( pdest,
                    pin->imgtype );
}

/*************
 *
 * Name:    qm_fms_get_mbn_image_info_imgpath_tlv_pack
 *
 * Purpose: Pack fully qualified image path
 *
 * Parms:   (IN)    psource - source data
 *          (OUT)   pdest   - destination buffer
 *
 * Return:  eQCWWAN_ERR_NONE
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_fms_get_mbn_info_imgpath_tlv_pack(
    swi_uint8 *pdest,
    swi_uint8 *psource )
{
    struct qm_fms_mbn_request_tlv_values *pin;
    pin = (struct qm_fms_mbn_request_tlv_values *)psource;

    return PutStream( pdest,
                      (swi_uint8 *)pin->path,
                      slstrlen(pin->path) + 1 );
}

/*************
 *
 * Name:    qm_fms_get_mbn_info_request_pack
 *
 * Purpose: Pack FMS get cwe Sierra Package String (spkgs) info request
 *
 * Parms:   (OUT) preq  - outgoing request buffer
 *          (OUT) plen  - length of packed request
 *
 * Return:  eQCWWAN_ERR_NONE on success
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
global enum eQCWWANError
qm_fms_get_mbn_info_request_pack(
    const swi_char  *path,
    const swi_uint8 imgtype,
    swi_uint8       *preq,
    swi_uint16      *plen )
{
    struct qmTlvBuilderItem map[] =
    {
        { eTLV_FMS_GET_MBN_INFO_IMGDIR_PATH,
          qm_fms_get_mbn_info_imgpath_tlv_pack },

        { eTLV_FMS_GET_MBN_INFO_IMGTYPE,
          qm_fms_get_mbn_info_imgtype_tlv_pack },

        { eTLV_TYPE_INVALID, NULL }
    };

    /* QMI request packing input arguments */
    struct qm_fms_mbn_request_tlv_values arg;
    slstrncpy( arg.path, path, sizeof(arg.path) );
    arg.imgtype = imgtype;

    /* pack QMI request message */
    enum eQCWWANError rc;
    rc = qmbuild( preq,
                  (swi_uint8 *)&arg,
                  map,
                  eQMI_FMS_GET_MBN_INFO,
                  plen );

    return rc;
}
