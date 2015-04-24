/*************
 *
 * Filename: qmfms.c
 *
 * Purpose:  QMI Firmware Management Service
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/

/*-------------
  include files
 --------------*/
#include "aa/aaglobal.h"
#include "qm/qmerrno.h"
#include "am/amudefs.h"
#include "ci/ciudefs.h"
#include "er/erudefs.h"
#include "im/imudefs.h"
#include "mm/mmudefs.h"
#include "os/swi_ossdk.h"
#include "qm/qmidefs.h"
#include "qm/qmfms.h"
#include "qm/qmqmisvc.h"
#include "pi/piudefs.h"
#include "sl/sludefs.h"
#include <stdio.h>

/*-------
  Defines
 --------*/

/*--------------------
  Forward declarations
 ---------------------*/
local void qm_fms_set_image_path(swi_uint8 *);
local void qm_fms_get_cwe_spkgs_info(swi_uint8 *);
local void qm_fms_get_mbn_info(swi_uint8 *);

/*-------------
  Local storage
 --------------*/

/* QMI FMS service response structure */
local struct qmqmisvcresponse qmifmsresp;

/* Firmware Management Service request handler table */
local void(*fmshandlertbl[])(swi_uint8 *preq) =
{
    qm_fms_set_image_path,
    qm_fms_get_cwe_spkgs_info,
    qm_fms_get_mbn_info
};

/*---------
  Functions
 ----------*/

/*************
 *
 * Name:    qm_fms_set_image_path_unpack_tlv
 *
 * Purpose: Unpack firmware download image path TLV Value
 *
 * Parms:   pdest   - destination buffer
 *          pparm   - source data
 *
 * Return:  eQCWWAN_ERR_NONE
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_fms_set_image_path_unpack_tlv(
    swi_uint8 *psource,
    swi_uint8 *pdest )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_fms_set_image_path_request_tlv_values *pval;

    pin =   (struct qm_qmi_response_tlvs_values *)pdest;
    pval =  (struct qm_fms_set_image_path_request_tlv_values *)
                &pin->tlvvalues.qmfmstlvs.set_image_path_req;

    swi_uint32 len = QM_FMS_IMAGE_PATH_MAX_SZ;
    return qmQmiExtractString( psource,
                               pval->path,
                               len );
}

/*************
 *
 * Name:    qm_fms_set_image_path_send_response
 *
 * Purpose: Pack and send a FMS set image path response
 *
 * Parms:   msgid               - QMI DCS service message id
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
qm_fms_set_image_path_send_response(
    enum eQMIMessageFMS msgid,
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
    qm_qmisvc_send_response( &qmifmsresp,
                             prsptlvs,
                             map,
                             eQMI_SVC_FMS,
                             msgid,
                             eQMIRES );
}

/*************
 *
 * Name:    qm_fms_set_image_path_request_unpack
 *
 * Purpose: Unpack FMS set image path request
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
qm_fms_set_image_path_request_unpack(
    swi_uint8 *preq,
    struct qm_qmi_response_tlvs_values *preqtlvs )
{
    struct qmTlvUnpackerItem map[] =
    {
        { eTLV_FMS_IMAGE_PATH,
          qm_fms_set_image_path_unpack_tlv },

        { eTLV_TYPE_INVALID, NULL }
    };

    return qmunpack( preq,
                     (swi_uint8 *)preqtlvs,
                     map,
                     eQMI_FMS_SET_IMAGE_PATH );
}

/*************
 *
 * Name:    qm_fms_set_image_path
 *
 * Purpose: Service Gobi API UpgradeFirmware2k request
 *
 * Parms:   preq - request packet pointer
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   FMS is a pseudo-QMI service managed by the SDK in order to
 *          provide required support for Firmware Management Service APIs.
 *          Refer to QCT document 80-VF219-N.
 *
 **************/
local void
qm_fms_set_image_path(
    swi_uint8 *preq )
{
    struct qm_fms_upgrade_firmware_info;
    struct qm_qmi_response_tlvs_values reqtlvs; /* for unpacking request */
    struct qm_qmi_response_tlvs_values rsptlvs; /* for packing response */

    /* initialize result tlv */
    rsptlvs.qmiresult.result = QMI_RC_SUCCESS;
    rsptlvs.qmiresult.error  = eQCWWAN_ERR_NONE;

    enum eQCWWANError rc = qm_fms_set_image_path_request_unpack(preq, &reqtlvs);
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

    struct qmtcb *pcb = qmgetcbp();
    slstrncpy(  pcb->fms_data.path,
                reqtlvs.tlvvalues.qmfmstlvs.set_image_path_req.path,
                QM_FMS_IMAGE_PATH_MAX_SZ );

    pcb->fms_data.fw_download_requested = TRUE;

    dlLog0( &pcb->qmdlcb, QMLOG_CLASSA,
            pcb->fms_data.path );

    qm_fms_set_image_path_send_response( (enum eQMIMessageFMS)*preq,
                                         &rsptlvs );

    /* If the modem is in boot mode, there are no QMI clients available,
     * just fake a boot notification to kick-off the download.
     */
    if (pcb->qmidevicestate == DS_SIO_BOOT_READY )
    {
/* RILSTART */
        /* Allocate an event block */
        struct qmtcb *qmcbp = qmgetcbp();
        struct qmevtblock *eventbkp =
                    (struct qmevtblock *)mmbufget( &qmcbp->qmevtpool );

        if( eventbkp )
        {
            /* Stuff the fields with the required information */
            eventbkp->qmeventtype = QM_DS_DEVICE_EVT;

            /* The QMI information comes next */
            eventbkp->qmevtdatap = (swi_uint8*)DS_SIO_BOOT_READY;

            /* zero out memory release pointer */
            eventbkp->qmevtmemfreep = NULL;

            /* invalidate IPC channel from which the request arrived */
            eventbkp->qmevtipcchan = 0xFF;

            /* Send the message on its way */
            icsmt_send( &qmcbp->qmicmsgque,
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
/* RILSTOP */
    }
}

/*************
 *
 * Name:    qm_fms_get_cwe_spkgs_info_priversion_tlv_pack
 *
 * Purpose: pack PRI version TLV Value
 *
 * Parms:   (IN)    psrc    - src data
 *          (OUT)   pdest   - destination buffer
 *
 * Return:  success: eQCWWAN_ERR_NONE
 *          failure: eQCWWAN_ERR_MEMORY
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_fms_get_cwe_spkgs_info_priversion_tlv_pack(
    swi_uint8 *pdest,
    swi_uint8 *psrc )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_fms_cwe_spkgs_response_tlv_values *ptlv;

    pin = (struct qm_qmi_response_tlvs_values *)psrc;

    ptlv = (struct qm_fms_cwe_spkgs_response_tlv_values *)
                            &pin->tlvvalues.qmfmstlvs.cwe_spkgs_rsp;

    return PutStream( pdest,
                      (swi_uint8 *)ptlv->priversion_str,
                      (swi_uint32)slstrlen(ptlv->priversion_str) );
}

/*************
 *
 * Name:    qm_fms_get_cwe_spkgs_info_carrier_tlv_pack
 *
 * Purpose: pack carrier TLV Value
 *
 * Parms:   (IN)    psrc    - src data
 *          (OUT)   pdest   - destination buffer
 *
 * Return:  success: eQCWWAN_ERR_NONE
 *          failure: eQCWWAN_ERR_MEMORY
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_fms_get_cwe_spkgs_info_carrier_tlv_pack(
    swi_uint8 *pdest,
    swi_uint8 *psrc )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_fms_cwe_spkgs_response_tlv_values *ptlv;

    pin = (struct qm_qmi_response_tlvs_values *)psrc;

    ptlv = (struct qm_fms_cwe_spkgs_response_tlv_values *)
                            &pin->tlvvalues.qmfmstlvs.cwe_spkgs_rsp;

    return PutStream( pdest,
                      (swi_uint8 *)ptlv->carrier_str,
                      (swi_uint32)slstrlen(ptlv->carrier_str ) );
}

/*************
 *
 * Name:    qm_fms_get_cwe_spkgs_info_pkgversion_tlv_pack
 *
 * Purpose: pack sierra package version TLV Value
 *
 * Parms:   (IN)    psrc    - src data
 *          (OUT)   pdest   - destination buffer
 *
 * Return:  success: eQCWWAN_ERR_NONE
 *          failure: eQCWWAN_ERR_MEMORY
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_fms_get_cwe_spkgs_info_pkgversion_tlv_pack(
    swi_uint8 *pdest,
    swi_uint8 *psrc )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_fms_cwe_spkgs_response_tlv_values *ptlv;

    pin = (struct qm_qmi_response_tlvs_values *)psrc;

    ptlv = (struct qm_fms_cwe_spkgs_response_tlv_values *)
                            &pin->tlvvalues.qmfmstlvs.cwe_spkgs_rsp;

    return PutStream( pdest,
                      (swi_uint8 *)ptlv->packageid_str,
                      (swi_uint32)slstrlen(ptlv->packageid_str ) );
}

/*************
 *
 * Name:    qm_fms_get_cwe_spkgs_info_skuid_tlv_pack
 *
 * Purpose: pack SKU ID TLV Value
 *
 * Parms:   (IN)    psrc    - src data
 *          (OUT)   pdest   - destination buffer
 *
 * Return:  success: eQCWWAN_ERR_NONE
 *          failure: eQCWWAN_ERR_MEMORY
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_fms_get_cwe_spkgs_info_skuid_tlv_pack(
    swi_uint8 *pdest,
    swi_uint8 *psrc )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_fms_cwe_spkgs_response_tlv_values *ptlv;

    pin = (struct qm_qmi_response_tlvs_values *)psrc;

    ptlv = (struct qm_fms_cwe_spkgs_response_tlv_values *)
                            &pin->tlvvalues.qmfmstlvs.cwe_spkgs_rsp;

    return PutStream( pdest,
                      (swi_uint8 *)ptlv->sku_str,
                      (swi_uint32)slstrlen(ptlv->sku_str) );
}

/*************
 *
 * Name:    qm_fms_get_cwe_spkgs_info_fwversion_tlv_pack
 *
 * Purpose: pack firmware version TLV Value
 *
 * Parms:   (IN)    psrc    - src data
 *          (OUT)   pdest   - destination buffer
 *
 * Return:  success: eQCWWAN_ERR_NONE
 *          failure: eQCWWAN_ERR_MEMORY
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_fms_get_cwe_spkgs_info_fwversion_tlv_pack(
    swi_uint8 *pdest,
    swi_uint8 *psrc )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_fms_cwe_spkgs_response_tlv_values *ptlv;

    pin = (struct qm_qmi_response_tlvs_values *)psrc;

    ptlv = (struct qm_fms_cwe_spkgs_response_tlv_values *)
                            &pin->tlvvalues.qmfmstlvs.cwe_spkgs_rsp;

    return PutStream( pdest,
                      (swi_uint8 *)ptlv->fwversion_str,
                      (swi_uint32)slstrlen(ptlv->fwversion_str) );
}

/*************
 *
 * Name:    qm_fms_get_cwe_spkgs_info_modelid_tlv_pack
 *
 * Purpose: pack modelid TLV Value
 *
 * Parms:   (IN)    psrc    - src data
 *          (OUT)   pdest   - destination buffer
 *
 * Return:  success: eQCWWAN_ERR_NONE
 *          failure: eQCWWAN_ERR_MEMORY
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_fms_get_cwe_spkgs_info_modelid_tlv_pack(
    swi_uint8 *pdest,
    swi_uint8 *psrc )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_fms_cwe_spkgs_response_tlv_values *ptlv;

    pin = (struct qm_qmi_response_tlvs_values *)psrc;

    ptlv = (struct qm_fms_cwe_spkgs_response_tlv_values *)
                            &pin->tlvvalues.qmfmstlvs.cwe_spkgs_rsp;

    return PutStream( pdest,
                      (swi_uint8 *)ptlv->modelid_str,
                      (swi_uint32)slstrlen(ptlv->modelid_str ) );
}

/*************
 *
 * Name:    qm_fms_get_cwe_spkgs_info_send_response
 *
 * Purpose: Pack and send a FMS set image path response
 *
 * Parms:           msgid       - QMI DCS service message id
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
qm_fms_get_cwe_spkgs_info_send_response(
    enum eQMIMessageFMS msgid,
    struct qm_qmi_response_tlvs_values *prsptlvs )
{
    struct qmTlvBuilderItem map[] =
    {
        { eTLV_RESULT_CODE,
          &qm_result_code_tlv_pack },

        { eTLV_FMS_GET_CWE_SPKGS_INFO_SKUID_STR,
            &qm_fms_get_cwe_spkgs_info_skuid_tlv_pack },

        { eTLV_FMS_GET_CWE_SPKGS_INFO_MODELID_STR,
          &qm_fms_get_cwe_spkgs_info_modelid_tlv_pack },

        { eTLV_FMS_GET_CWE_SPKGS_INFO_FWVERSION_STR,
           &qm_fms_get_cwe_spkgs_info_fwversion_tlv_pack },

        { eTLV_FMS_GET_CWE_SPKGS_INFO_CARRIER_STR,
          &qm_fms_get_cwe_spkgs_info_carrier_tlv_pack },

        { eTLV_FMS_GET_CWE_SPKGS_INFO_PRIVERSION_STR,
          &qm_fms_get_cwe_spkgs_info_priversion_tlv_pack },

        { eTLV_FMS_GET_CWE_SPKGS_INFO_PKGVERSION_STR,
          &qm_fms_get_cwe_spkgs_info_pkgversion_tlv_pack },

        { eTLV_TYPE_INVALID,
          NULL } /* sentinal signifies last item in map */
    };

    /* construct and send response to the application */
    qm_qmisvc_send_response( &qmifmsresp,
                             prsptlvs,
                             map,
                             eQMI_SVC_FMS,
                             msgid,
                             eQMIRES );
}

/*************
 *
 * Name:    qm_fms_get_cwe_spkgs_info_imgpath_tlv_unpack
 *
 * Purpose: Unpack firmware download image path TLV Value
 *
 * Parms:   pdest   - destination buffer
 *          pparm   - source data
 *
 * Return:  eQCWWAN_ERR_NONE
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_fms_get_cwe_spkgs_info_imgpath_tlv_unpack(
    swi_uint8 *psource,
    swi_uint8 *pdest )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_fms_set_image_path_request_tlv_values *pval;

    pin =   (struct qm_qmi_response_tlvs_values *)pdest;
    pval =  (struct qm_fms_set_image_path_request_tlv_values *)
                            &pin->tlvvalues.qmfmstlvs.cwe_spkgs_req;

    return qmQmiExtractString( psource,
                               pval->path,
                               QM_FMS_IMAGE_PATH_MAX_SZ );
}

/*************
 *
 * Name:    qm_fms_get_cwe_spkgs_info_request_unpack
 *
 * Purpose: unpack FMS get cwe spkgs info request
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
qm_fms_get_cwe_spkgs_info_request_unpack(
    swi_uint8 *preq,
    struct qm_qmi_response_tlvs_values *preqtlvs )
{
    struct qmTlvUnpackerItem map[] =
    {
        {   eTLV_FMS_GET_CWE_SPKGS_INFO_IMGDIR_PATH,
            qm_fms_get_cwe_spkgs_info_imgpath_tlv_unpack },

        {   eTLV_TYPE_INVALID, NULL }
    };

    return qmunpack( preq,
                     (swi_uint8 *)preqtlvs,
                     map,
                     eQMI_FMS_GET_CWE_SPKGS_INFO );
}

/*************
 *
 * Name:    qm_fms_extract_spkgsparameters
 *
 * Purpose: extract the spkgs parameters from string into structure
 *
 * Parms:   (IN)  pspkgsstring - string containing spkgs parameters
 *          (OUT) pcwespkgsrsp - structure in which spkgs parameters
 *                               will be extracted
 *
 * Return:  TRUE  - if parameters from SPKGS string are extracted successfully
 *          FALSE - if SPKGS string is corrupt
 *
 * Abort:   none
 *
 * Notes:   FMS is a pseudo-QMI service managed by the SDK in order to
 *          provide required support for Firmware Management Service Gobi APIs.
 *          Refer to QCT document 80-VF219-N.
 *
 **************/
local swi_bool
qm_fms_extract_spkgsparameters(
    swi_char                                     *pspkgsstring,
    struct qm_fms_cwe_spkgs_response_tlv_values  *pcwespkgsrsp )
{
    enum cwe_spkgs_param_indices idx = SPKGS_SKU_IDX;
    swi_int32 paramlen;
    swi_char  *pparam;

    /* search the starting of the SPKGS string */
    pparam = strtok (pspkgsstring, "_");
    while (pparam != NULL)
    {
        switch(idx)
        {
            case SPKGS_SKU_IDX:
                paramlen = slstrlen(pparam);
                if ( SLQSIMINFO_SKU_SZ < paramlen  )
                    return FALSE;
                else
                {
                    slstrncpy( pcwespkgsrsp->sku_str,
                               pparam,
                               paramlen + 1 );
                    idx = SPKGS_PARTNO_IDX;
                    break;
                }

            case SPKGS_PARTNO_IDX:
                paramlen = slstrlen(pparam);
                if ( SLQSIMINFO_PARTNO_SZ < paramlen  )
                    return FALSE;
                else
                {
                    idx = SPKGS_MODELID_IDX;
                    break;
                }

            case SPKGS_MODELID_IDX:
                paramlen = slstrlen(pparam);
                if ( SLQSIMINFO_MODELID_SZ < paramlen  )
                    return FALSE;
                else
                {
                    slstrncpy( pcwespkgsrsp->modelid_str,
                               pparam,
                               paramlen + 1 );
                    idx = SPKGS_FWVERSION_IDX;
                    break;
                }

            case SPKGS_FWVERSION_IDX:
                paramlen = slstrlen(pparam);
                if ( SLQSIMINFO_FWVERSION_SZ < paramlen  )
                    return FALSE;
                else
                {
                    slstrncpy( pcwespkgsrsp->fwversion_str,
                               pparam,
                               paramlen + 1);
                    idx = SPKGS_BOOTBLK_IDX;
                    break;
                }

            case SPKGS_BOOTBLK_IDX:
                paramlen = slstrlen(pparam);
                if ( SLQSIMINFO_BOOTBLK_SZ < paramlen  )
                    return FALSE;
                else
                {
                    idx = SPKGS_CARRIER_IDX;
                    break;
                }

            case SPKGS_CARRIER_IDX:
                paramlen = slstrlen(pparam);
                if ( SLQSIMINFO_CARRIER_SZ < paramlen  )
                    return FALSE;
                else
                {
                    slstrncpy( pcwespkgsrsp->carrier_str,
                               pparam,
                               paramlen + 1);
                    idx = SPKGS_PRIVER_IDX;
                    break;
                }

            case SPKGS_PRIVER_IDX:
                paramlen = slstrlen(pparam);
                if ( SLQSIMINFO_PRIVERSION_SZ < paramlen  )
                    return FALSE;
                else
                {
                    slstrncpy( pcwespkgsrsp->priversion_str,
                               pparam,
                               paramlen + 1);
                    idx = SPKGS_PKGID_IDX;
                    break;
                }

            case SPKGS_PKGID_IDX:
                paramlen = slstrlen(pparam);
                if ( SLQSIMINFO_PACKAGEID_SZ < paramlen  )
                    return FALSE;
                else
                {
                    slstrncpy( pcwespkgsrsp->packageid_str,
                               pparam,
                               paramlen + 1 );
                    idx = SPKGS_LAST_IDX;
                    break;
                }

            default:
                break;
        }
        /* look for the next token */
        pparam = strtok (NULL, "_");
    }

    /* if we are unable to extract any parameter from the SPKGS string,
       SPGS string is corrupted, return with error */
    if( SPKGS_LAST_IDX != idx )
        return FALSE;

    return TRUE;
}

/*************
 *
 * Name:    qm_fms_get_valid_image
 *
 * Purpose: open the firmware image and check the valid image wih greater pkg
 *          version
 *
 * Parms:   (IN)  pimagepath     - path including the file name
 *          (OUT) pvalidfileinfo - structure containing the info of valid file
 *
 * Return:  eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 * Abort:   none
 *
 * Notes:   FMS is a pseudo-QMI service managed by the SDK in order to
 *          provide required support for Firmware Management Service Gobi APIs.
 *          Refer to QCT document 80-VF219-N.
 *
 **************/
local enum eQCWWANError
qm_fms_get_valid_image(
    swi_char               *pimagepath,
    struct swi_osfilestats *pvalidfileinfo)
{
    swi_char *pcurPkgVrsn;

    /* open the file and memory map it for reading */
    if( TRUE == swi_ossdk_mmapro(pimagepath, pvalidfileinfo) )
    {
        if( QM_FMS_SPKGS_STR_OFFSET > pvalidfileinfo->filesize)
        {
            /*  unmap the opened file and return with error */
            swi_ossdk_umapfile( pvalidfileinfo );
            return eQCWWAN_ERR_SWIIM_CORRUPTED_FW_IMAGE;
        }

        /* locate the starting of the version string */
        pcurPkgVrsn = strrchr((char *)(pvalidfileinfo->pvmap +\
                               QM_FMS_SPKGS_STR_OFFSET ), '_' );

        /* If '_' is not found or SPKGS string or pkg version string
           length is greater than limit size - its a corrupted firmware image */
        if( ( NULL == pcurPkgVrsn ) ||
            ( SLQSIMINFO_PACKAGEID_SZ < ( slstrlen(pcurPkgVrsn) - 1 ) ) ||
            ( QM_FMS_SPKGS_STR_MAX_SZ < slstrlen( (char *)\
                    (pvalidfileinfo->pvmap + QM_FMS_SPKGS_STR_OFFSET ) ) ) )
        {
            /*  unmap the opened file and return with error */
            swi_ossdk_umapfile( pvalidfileinfo );
            return eQCWWAN_ERR_SWIIM_CORRUPTED_FW_IMAGE;
        }
    }
    else
        return eQCWWAN_ERR_SWIIM_OPENING_FILE;

    return eQCWWAN_ERR_NONE;
}

/*************
 *
 * Name:    qm_fms_get_cwe_spkgs_info
 *
 * Purpose: Service Gobi API SLQSGetImageInfo request
 *
 * Parms:   preq - request packet pointer
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   FMS is a pseudo-QMI service managed by the SDK in order to
 *          provide required support for Firmware Management Service Gobi APIs.
 *          Refer to QCT document 80-VF219-N.
 *
 **************/
local void
qm_fms_get_cwe_spkgs_info(
    swi_uint8 *preq )
{
    swi_char *pimagepath;
    static   swi_char completeimpath[QM_FMS_IMAGE_PATH_MAX_SZ];
    struct   qm_qmi_response_tlvs_values reqtlvs; /* for unpacking request */
    struct   qm_qmi_response_tlvs_values rsptlvs; /* for packing response */
    enum     eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* initialize result tlv */
    rsptlvs.qmiresult.result = QMI_RC_SUCCESS;
    rsptlvs.qmiresult.error  = eQCWWAN_ERR_NONE;

    /* get the fully qualified image directory path */
    eRCode = qm_fms_get_cwe_spkgs_info_request_unpack(preq, &reqtlvs);

    if( eRCode != eQCWWAN_ERR_NONE )
    {
        /* request TLV extraction failed */
        rsptlvs.qmiresult.result = QMI_RC_FAILURE;
        rsptlvs.qmiresult.error  = eRCode;

        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d request TLV extraction failed",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, (swi_uint32)eRCode );
    }

    /* image path obtained from the API */
    pimagepath = reqtlvs.tlvvalues.qmfmstlvs.cwe_spkgs_req.path;

    /* validate path */
    if( ( 0 != swi_ossdk_stat( pimagepath ) ) ||
        ( QM_FMS_IMAGE_PATH_MAX_SZ < slstrlen(pimagepath) ) )
    {
        eRCode  = eQCWWAN_ERR_SWIIM_INVALID_PATH;
    }
    else /* valid path */
    {
        /* extract Sierra Package String information from the CWE image with
           the highest Package Version number */
        static swi_char spkgsstring[QM_FMS_SPKGS_STR_MAX_SZ];
        struct swi_osfilestats validfileinfo;
        struct swi_osdir swidir;
        validfileinfo.pvmap = NULL;

        validfileinfo.filesize = 4096;
        validfileinfo.vmapfileoffset = 0;

        /* open the directory */
        if( TRUE == swi_ossdk_opendir( pimagepath, &swidir ) )
        {
            do{
                /* traverse through the files in directory */
                if( TRUE == swi_ossdk_readdir( &swidir ) )
                {
                    /* complete path of the image including name should be less
                       than the maximum allowed limit */
                    if( QM_FMS_IMAGE_PATH_MAX_SZ < ( slstrlen(pimagepath) +
                                           slstrlen(swidir.pentry->d_name) ) )
                    {
                        eRCode  = eQCWWAN_ERR_SWIIM_INVALID_PATH;
                        continue;
                    }

                    /* get the complete path of the image with name in buffer */
                    memset( completeimpath, 0, sizeof(completeimpath) );
                    slstrncpy( completeimpath,
                               pimagepath,
                               sizeof(completeimpath));

                    /* path is valid , process it */
                    strcat(completeimpath, swidir.pentry->d_name);

                    /* check if its a CWE image type */
                    if( FW_IMG_TYPE_CWE == qmGetFileType( completeimpath ) )
                    {
                         /* check if it is a valid image */
                         eRCode = qm_fms_get_valid_image( completeimpath,
                                                          &validfileinfo ) ;
                         break;
                    }
                    else /* check the type of next file */
                        continue;
                }
                else
                    break;
            }while(1);

            /* checks if a valid image found at the given path */
            if( eRCode == eQCWWAN_ERR_NONE )
            {
                if( NULL == validfileinfo.pvmap ) /* no valid image found */
                    eRCode = eQCWWAN_ERR_SWIIM_FILE_NOT_FOUND;
                else
                {
                    memset( spkgsstring, 0, sizeof(spkgsstring) );

                    /* read the SPKGS string of the valid firmware image */
                    strcpy(spkgsstring, (validfileinfo.pvmap +
                                         QM_FMS_SPKGS_STR_OFFSET) );

                    /* unmap the file */
                    swi_ossdk_umapfile( &validfileinfo );

                    /* extract the spkgs parameters from ths string */
                    if( TRUE != qm_fms_extract_spkgsparameters( spkgsstring,
                                &(rsptlvs.tlvvalues.qmfmstlvs.cwe_spkgs_rsp) ) )
                        eRCode = eQCWWAN_ERR_SWIIM_CORRUPTED_FW_IMAGE;
                }
            }
            swi_ossdk_closedir(&swidir);
        }
        else /* directory opening failed */
            eRCode = eQCWWAN_ERR_SWIIM_OPENING_DIR;
    }

    /* If there is an error */
    if( eQCWWAN_ERR_NONE != eRCode)
    {
        rsptlvs.qmiresult.result = QMI_RC_FAILURE;
        rsptlvs.qmiresult.error  = eRCode;
        slmemset( (char *)&rsptlvs.tlvvalues.qmfmstlvs.cwe_spkgs_rsp,
                  0,
                  sizeof(rsptlvs.tlvvalues.qmfmstlvs.cwe_spkgs_rsp) );
    }

    /* send the response */
    qm_fms_get_cwe_spkgs_info_send_response( (enum eQMIMessageFMS)*preq,
                                             &rsptlvs );
}

/*************
 *
 * Name:    qm_fms_get_mbn_info_imgtype_tlv_unpack
 *
 * Purpose: Unpack firmware download image path TLV Value
 *
 * Parms:   pdest   - destination buffer
 *          pparm   - source data
 *
 * Return:  eQCWWAN_ERR_NONE
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_fms_get_mbn_info_imgtype_tlv_unpack(
    swi_uint8 *psource,
    swi_uint8 *pdest )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_fms_mbn_request_tlv_values *pval;

    pin  = (struct qm_qmi_response_tlvs_values *)pdest;
    pval = (struct qm_fms_mbn_request_tlv_values *)
                            &pin->tlvvalues.qmfmstlvs.mbn_req;

    return GetByte( psource,
                    &pval->imgtype );
}

/*************
 *
 * Name:    qm_fms_get_mbn_info_imgpath_tlv_unpack
 *
 * Purpose: Unpack firmware download image path TLV Value
 *
 * Parms:   pdest   - destination buffer
 *          pparm   - source data
 *
 * Return:  eQCWWAN_ERR_NONE
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_fms_get_mbn_info_imgpath_tlv_unpack(
    swi_uint8 *psource,
    swi_uint8 *pdest )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_fms_mbn_request_tlv_values *pval;

    pin  = (struct qm_qmi_response_tlvs_values *)pdest;
    pval = (struct qm_fms_mbn_request_tlv_values *)
                            &pin->tlvvalues.qmfmstlvs.mbn_req;

    return qmQmiExtractString( psource,
                               pval->path,
                               QM_FMS_IMAGE_PATH_MAX_SZ );
}

/*************
 *
 * Name:    qm_fms_get_mbn_info_request_unpack
 *
 * Purpose: unpack FMS get mbn info request
 *
 * Parms:   (IN)  preq       - incoming request packet
 *          (OUT) preqtlvs   - destination data containing request
 *                             parameters.
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
qm_fms_get_mbn_info_request_unpack(
    swi_uint8 *preq,
    struct qm_qmi_response_tlvs_values *preqtlvs )
{
    struct qmTlvUnpackerItem map[] =
    {
        {   eTLV_FMS_GET_MBN_INFO_IMGDIR_PATH,
            qm_fms_get_mbn_info_imgpath_tlv_unpack },

        {   eTLV_FMS_GET_MBN_INFO_IMGTYPE,
            qm_fms_get_mbn_info_imgtype_tlv_unpack },

        {   eTLV_TYPE_INVALID, NULL }
    };

    return qmunpack( preq,
                     (swi_uint8 *)preqtlvs,
                     map,
                     eQMI_FMS_GET_MBN_INFO );
}

/*************
 *
 * Name:    qm_fms_get_mbn_info_fwidstr_tlv_pack
 *
 * Purpose: pack firmwareID Value
 *
 * Parms:   (IN)    psrc    - src data
 *          (OUT)   pdest   - destination buffer
 *
 * Return:  success: eQCWWAN_ERR_NONE
 *          failure: eQCWWAN_ERR_MEMORY
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_fms_get_mbn_info_fwidstr_tlv_pack(
    swi_uint8 *pdest,
    swi_uint8 *psrc )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_fms_mbn_response_tlv_values *ptlv;

    pin = (struct qm_qmi_response_tlvs_values *)psrc;

    ptlv = (struct qm_fms_mbn_response_tlv_values *)
                            &pin->tlvvalues.qmfmstlvs.mbn_rsp;

    return PutStream( pdest,
                      (swi_uint8 *)ptlv->fwidstr,
                      sizeof(ptlv->fwidstr) );
}

/*************
 *
 * Name:    qm_fms_get_mbn_info_versionstr_tlv_pack
 *
 * Purpose: pack firmwareID Value
 *
 * Parms:   (IN)    psrc    - src data
 *          (OUT)   pdest   - destination buffer
 *
 * Return:  success: eQCWWAN_ERR_NONE
 *          failure: eQCWWAN_ERR_MEMORY
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_fms_get_mbn_info_versionstr_tlv_pack(
    swi_uint8 *pdest,
    swi_uint8 *psrc )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_fms_mbn_response_tlv_values *ptlv;

    pin = (struct qm_qmi_response_tlvs_values *)psrc;

    ptlv = (struct qm_fms_mbn_response_tlv_values *)
                            &pin->tlvvalues.qmfmstlvs.mbn_rsp;

    return PutStream( pdest,
                      (swi_uint8 *)ptlv->fwversionstr,
                      sizeof(ptlv->fwversionstr) );
}

/*************
 *
 * Name:    qm_fms_get_mbn_info_firmwareid_tlv_pack
 *
 * Purpose: pack firmwareID Value
 *
 * Parms:   (IN)    psrc    - src data
 *          (OUT)   pdest   - destination buffer
 *
 * Return:  success: eQCWWAN_ERR_NONE
 *          failure: eQCWWAN_ERR_MEMORY
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local enum eQCWWANError
qm_fms_get_mbn_info_firmwareid_tlv_pack(
    swi_uint8 *pdest,
    swi_uint8 *psrc )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_fms_mbn_response_tlv_values *ptlv;

    pin = (struct qm_qmi_response_tlvs_values *)psrc;

    ptlv = (struct qm_fms_mbn_response_tlv_values *)
                            &pin->tlvvalues.qmfmstlvs.mbn_rsp;

    return PutLong( pdest, ptlv->firmwareid );
}

/*************
 *
 * Name:    qm_fms_get_mbn_info_send_response
 *
 * Purpose: Pack and send a FMS set image path response
 *
 * Parms:           msgid     - QMI DCS service message id
 *          (OUT)   prsptlvs  - response structure
 *
 * Return:  eQCWWAN_ERR_NONE
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
local void
qm_fms_get_mbn_info_send_response(
    enum eQMIMessageFMS msgid,
    struct qm_qmi_response_tlvs_values *prsptlvs )
{
    struct qmTlvBuilderItem map[] =
    {
        { eTLV_RESULT_CODE,
          &qm_result_code_tlv_pack },

        { eTLV_FMS_GET_MBN_INFO_FIRMWAREID,
          &qm_fms_get_mbn_info_firmwareid_tlv_pack },

        { eTLV_FMS_GET_MBN_INFO_FWVERSION_STR,
          &qm_fms_get_mbn_info_versionstr_tlv_pack },

        { eTLV_FMS_GET_MBN_INFO_FIRMWAREID_STR,
          &qm_fms_get_mbn_info_fwidstr_tlv_pack },

        { eTLV_TYPE_INVALID,
          NULL } /* sentinal signifies last item in map */
    };

    /* construct and send response to the application */
    qm_qmisvc_send_response( &qmifmsresp,
                             prsptlvs,
                             map,
                             eQMI_SVC_FMS,
                             msgid,
                             eQMIRES );
}

/*************
 *
 * Name:    qm_fms_get_mbn_info
 *
 * Purpose: Service Gobi API GetImageInfo request
 *
 * \parms:  preq - request packet pointer
 *
 * \return: none
 *
 * Abort:   none
 *
 * Notes:   FMS is a pseudo-QMI service managed by the SDK in order to
 *          provide required support for Firmware Management Service Gobi APIs.
 *          Refer to QCT document 80-VF219-N.
 *
 **************/
local void
qm_fms_get_mbn_info(
    swi_uint8 *preq )
{
    struct qm_qmi_response_tlvs_values reqtlvs; /* for unpacking request */
    struct qm_qmi_response_tlvs_values rsptlvs; /* for packing response */

    /* initialize result tlv */
    rsptlvs.qmiresult.result = QMI_RC_SUCCESS;
    rsptlvs.qmiresult.error  = eQCWWAN_ERR_NONE;

    /* get the fully qualified image directory path */
    rsptlvs.qmiresult.error = qm_fms_get_mbn_info_request_unpack(preq, &reqtlvs);

    if( eQCWWAN_ERR_NONE != rsptlvs.qmiresult.error )
    {
        /* request TLV extraction failed */
        rsptlvs.qmiresult.result = QMI_RC_FAILURE;

        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d request TLV extraction failed",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, (swi_uint32)rsptlvs.qmiresult.error );
    }

    /* image path and image type obtained from the API */
    swi_char *pimgdir = reqtlvs.tlvvalues.qmfmstlvs.mbn_req.path;
    enum mbn_image_type_e imgtype = reqtlvs.tlvvalues.qmfmstlvs.mbn_req.imgtype;

    /* open the directory */
    struct swi_osdir dir;
    if( !swi_ossdk_opendir( pimgdir, &dir ) )
    {
        /* failed to open directory */
        rsptlvs.qmiresult.result = QMI_RC_FAILURE;
        rsptlvs.qmiresult.error  = eQCWWAN_ERR_SWIIM_OPENING_DIR;
    }

    else
    {
        swi_char path[QM_FMS_IMAGE_PATH_MAX_SZ];
        struct im_image_info_s imginfo;
        swi_bool traverse = TRUE; /* parse directory flag */
        do{
            /* traverse through the files in directory */
            if( swi_ossdk_readdir( &dir ) )
            {
                switch( imgtype )
                {
                    case MBN_IMG_TYPE_UQCN:
                        if( 0 != slstrcmp(dir.pentry->d_name, "uqcn.mbn") )
                        {
                            rsptlvs.qmiresult.error =
                                               eQCWWAN_ERR_SWIIM_FILE_NOT_FOUND;
                            continue;
                        }
                        break;
                    case MBN_IMG_TYPE_AMSS:
                        if( 0 != slstrcmp(dir.pentry->d_name, "amss.mbn") )
                        {
                            rsptlvs.qmiresult.error =
                                               eQCWWAN_ERR_SWIIM_FILE_NOT_FOUND;
                            continue;
                        }
                        break;
                    default:
                        rsptlvs.qmiresult.error = eQCWWAN_ERR_SWIIM_FILE_NOT_FOUND;
                        continue;
                }

                /* .mbn file found */
                swi_uint16 len = slstrlen(pimgdir);
                slstrncpy( path, pimgdir, len );

                /* account for path separator */
                if( path[len-1] != '/' )
                {
                    path[len++] = '/';
                }

                slstrncpy(  path + len,
                            dir.pentry->d_name,
                            slstrlen(dir.pentry->d_name ) + 1 );
                                        /* +1 for null terminator */

                /* initialize image path member */
                imginfo.pimgpath = path;
                if( IMERRNONE ==
                    ( rsptlvs.qmiresult.error = imuser_image_info_get(&imginfo) ) )
                {
                    struct gobi_mbn_img_info_s *pinfo =
                        (struct gobi_mbn_img_info_s *)&imginfo.imginfo.gobiinfo;

                    if( imgtype == pinfo->type )
                    {
                        /* found correct mbn image type:
                         * copy version id, version string, firmware id string
                         */
                        rsptlvs.tlvvalues.qmfmstlvs.mbn_rsp.firmwareid = pinfo->versionid;

                        slmemcpy( rsptlvs.tlvvalues.qmfmstlvs.mbn_rsp.fwversionstr,
                                  pinfo->imgvrsnstr,
                                  sizeof(rsptlvs.tlvvalues.qmfmstlvs.mbn_rsp.fwversionstr) );

                        slmemcpy( rsptlvs.tlvvalues.qmfmstlvs.mbn_rsp.fwidstr,
                                  pinfo->imgidstr,
                                  sizeof(rsptlvs.tlvvalues.qmfmstlvs.mbn_rsp.fwidstr) );

                        traverse = FALSE;
                    }
                    else
                    {
                        rsptlvs.qmiresult.error =
                                          eQCWWAN_ERR_SWIIM_CORRUPTED_FW_IMAGE;
                    }
                }
                else
                {
                    /* map IM error codes to QM error codes */
                    switch(rsptlvs.qmiresult.error)
                    {
                        case IMERRINVLDPATH:
                            rsptlvs.qmiresult.error = eQCWWAN_ERR_SWIIM_INVALID_PATH;
                            break;
                        case IMERRCORRUPTIMAGE:
                            rsptlvs.qmiresult.error = eQCWWAN_ERR_SWIIM_CORRUPTED_FW_IMAGE;
                            break;
                        default:
                            rsptlvs.qmiresult.error = eQCWWAN_ERR_GENERAL;
                    }
                    traverse = FALSE;
                }
            }
            else
            {
                traverse = FALSE;
            }
        }while(traverse);

        swi_ossdk_closedir(&dir);
    }

    /* If there is an error */
    if( eQCWWAN_ERR_NONE != rsptlvs.qmiresult.error )
    {
        rsptlvs.qmiresult.result = QMI_RC_FAILURE;
        slmemset( (swi_char *)&rsptlvs.tlvvalues.qmfmstlvs.mbn_rsp,
                  0,
                  sizeof(rsptlvs.tlvvalues.qmfmstlvs.mbn_rsp) );
    }

    /* send the response */
    qm_fms_get_mbn_info_send_response( (enum eQMIMessageFMS)*preq,
                                       &rsptlvs );
}

/*************
 *
 * Name:    qm_fms_fw_dwld_status_pack_tlv
 *
 * Purpose: Pack firmware download completion status notification TLV Value
 *
 * Parms:   pdest   - destination buffer
 *          psrc    - source data
 *
 * Return:  success: eQCWWAN_ERR_NONE
 *          failure: eQCWWAN_ERR_MEMORY
 *
 * Abort:   none
 *
 **************/
local enum eQCWWANError
qm_fms_fw_dwld_status_pack_tlv(
    swi_uint8 *pdest,
    swi_uint8 *psrc )
{
    struct qm_qmi_response_tlvs_values *pin;
    struct qm_fms_fwdld_completion_status_notif_tlv_values *pval;

    pin  = (struct qm_qmi_response_tlvs_values *)psrc;
    pval = (struct qm_fms_fwdld_completion_status_notif_tlv_values *)
                            &pin->tlvvalues.qmfmstlvs.fwdldnotif;

/* RILSTART */
    return PutLong( pdest, pval->fwdldstatus );
/* RILSTOP */
}

/*************
 *
 * Name:    qm_fms_event_notification_send
 *
 * Purpose: Pack and send a FMS event indication
 *
 * Parms:   msgid    - QMI FMS message ID
 *          prsptlvs - pointer to response TLV structure
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   FMS is a pseudo-QMI service managed by the SDK in order to
 *          support the Device Connectivity APIs. Refer to QCT document
 *          80-VF219-N.
 *
 **************/
local void
qm_fms_event_notification_send(
    struct qmTlvBuilderItem *pmap,
    struct qm_qmi_response_tlvs_values *prsptlvs )
{
    /* construct and send response to the application */
    qm_qmisvc_send_response( &qmifmsresp,
                             prsptlvs,
                             pmap,
                             eQMI_SVC_FMS,
                             eQMI_FMS_EVENT_IND,
                             eQMINOT );
}

/*************
 *
 * Name:    qm_fms_fw_dwld_complete_notify
 *
 * Purpose: Firmware Download Services (FMS) download completion event handler
 *
 * Parms:   fwdldstatus - firmware download status
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   FMS is a pseudo-QMI service managed by the SDK in order
 *          to support the Firmware Management Service APIs. Refer to
 *          QCT document 80-VF219-N.
 *
 **************/
/* RILSTART */
package void
qm_fms_fw_dwld_complete_notify(swi_int32 fwdldstatus)
{
/* RILSTOP */
    struct qm_qmi_response_tlvs_values rsptlvs;

    struct qmTlvBuilderItem map[] =
    {
        {   eTLV_FMS_IND_FW_DWLD_COMPLETE, qm_fms_fw_dwld_status_pack_tlv },
        {   eTLV_TYPE_INVALID, NULL } /* sentinal */
    };

    rsptlvs.tlvvalues.qmfmstlvs.fwdldnotif.fwdldstatus = fwdldstatus;
    qm_fms_event_notification_send( map, &rsptlvs );
}

/*************
 *
 * Name:    qm_fms_handler
 *
 * Purpose: Firmware Management Service (FMS) request handler
 *
 * Parms:   preq - request packet pointer
 *          prsp - response packet pointer
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   FMS is a pseudo-QMI service managed by the SDK in order to provide
 *          additional support to the Firmware Management Service APIs.
 *
 **************/
package void
qm_fms_handler(
    swi_uint8 *preq )
{
    enum eQMIMessageFMS request = *preq;
    qmifmsresp.ctlflgs = eQMIRES;
    struct qmtcb *tcbp = qmgetcbp();

    if( (swi_uint8)request < sizeof(fmshandlertbl)/sizeof(fmshandlertbl[0]) )
    {
        fmshandlertbl[*preq](preq);
    }
    else
    {
        qmshortresp( eQCWWAN_ERR_INTERNAL, tcbp->qmwdata.qmipcchannel );
    }
}

/*************
 *
 * Name:    qmfmsinit
 *
 * Purpose: Initialize FMS QMI service
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
package void qmfmsinit()
{
    /* Initialize QMI response structure */
    qmifmsresp.svc = eQMI_SVC_FMS;
}
