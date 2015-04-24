/*
 * \ingroup fms
 *
 * \file    qaFmsSlqsGetImageInfo.c
 *
 * \brief   Pack and Unpack routines for eQMI_FMS_GET_CWE_SPKGS_INFO
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaFmsSlqsGetImageInfo.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the SLQSGetImageInfo parameters to the QMI message SDU
 *
 * \param   pMlength    [OUT] - Total length of built message.
 *
 * \param   pParamField [OUT] - Pointer to storage into which the packed
 *
 * \param   path        [IN]  - Pointer to file's path parameter to be packed
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 */
enum eQCWWANError PkQmiGetCweSpkgsInfo(
    WORD    *pMlength,
    BYTE    *pParamField,
    LPCSTR  path )
{
    return  qm_fms_get_cwe_spkgs_info_request_pack( path,
                                                    pParamField,
                                                    pMlength );
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the SLQSGetImageInfo response message to a
 * user-provided response structure.
 *
 * \param     pMdmResp   [IN]  - packed response from the modem.
 *
 * \param     pApiResp   [OUT] - storage to unpack into.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 */
enum eQCWWANError UpkQmiGetCweSpkgsInfo(
    BYTE                               *pMdmResp,
    struct qm_qmi_response_tlvs_values *pApiResp)
{
    return qm_fms_get_cwe_spkgs_info_response_unpack( pMdmResp,
                                                      pApiResp );
}
