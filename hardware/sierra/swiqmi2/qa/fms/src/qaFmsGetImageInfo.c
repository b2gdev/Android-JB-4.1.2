/*
 * \ingroup fms
 *
 * \file    qaFmsGetImageInfo.c
 *
 * \brief   Pack and Unpack routines for eQMI_FMS_GET_MBN_INFO
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaFmsGetImageInfo.h"

/*******************
 * Request handling
 ******************/

/*
 * Packs the GetImageInfo parameters to the QMI message SDU
 *
 * \param   pMlength    [OUT] - length of packed message.
 *
 * \param   pParamField [OUT] - storage for packed message
 *
 * \param   path        [IN]  - path to firmware image
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 */
enum eQCWWANError PkQmiGetMbnInfo(
    WORD    *pMlength,
    BYTE    *pParamField,
    LPCSTR  path,
    BYTE    imgtype )
{
    return  qm_fms_get_mbn_info_request_pack( path,
                                              imgtype,
                                              pParamField,
                                              pMlength );
}

/********************
 * Response handling
 ********************/
/*
 *  Unpacks the GetImageInfo response message to a user-provided response
 *  structure.
 *
 *  \param     pMdmResp   [IN]  - packed response from the modem.
 *
 *  \param     pApiResp   [OUT] - storage to unpack into.
 *
 *  \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 */
enum eQCWWANError UpkQmiGetMbnInfo(
    BYTE                               *pMdmResp,
    struct qm_qmi_response_tlvs_values *pApiResp)
{
    return qm_fms_get_mbn_info_response_unpack( pMdmResp,
                                                pApiResp );
}
