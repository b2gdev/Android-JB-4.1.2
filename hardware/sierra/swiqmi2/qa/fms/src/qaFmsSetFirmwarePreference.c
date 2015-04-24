/*
 * \ingroup fms
 *
 * \file    qaFmsSetFirmwarePreferece.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          eQMI_DMS_SET_FIRMWARE_PREF message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaFmsSetFirmwarePreference.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the SetFirmwarePreference parameters to the QMI message SDU
 *
 * \param   pParamField [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \param   pMlength    [OUT] - Total length of built message.
 *
 * \return  eQCWWAN_ERR_NONE
 * \return  eQCWWAN_ERR_MEMORY
 *
 * \sa      qaGobiApiFms.h for remaining parameter descriptions.
 *
 */
enum eQCWWANError PkQmiFmsSetFirmwarePreference(
    WORD    *pMlength,
    BYTE    *pParamField )
{
    return qm_fms_set_firmware_preference_request_pack(  pParamField,
                                                         pMlength );
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the SetFirmwarePreference response message to a
 * user-provided response structure.
 *
 * \param     pMdmResp   [IN]  - packed response from the modem.
 *
 * \param     pApiResp   [OUT] - storage to unpack into.
 *
 * \return    eQCWWAN_ERR_NONE
 * \return    eQCWWAN_ERR_MEMORY
 */
enum eQCWWANError UpkQmiFmsSetFirmwarePreference(
    BYTE   *pMdmResp,
    struct qm_qmi_response_tlvs_values *pApiResp)
{
    enum eQCWWANError rCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qm_result_code_tlv_unpack },
        { eTLV_TYPE_INVALID, NULL } /* sentinal signifies last item in map */
    };

    rCode = qmunpackresp ( pMdmResp,
                           (BYTE*)pApiResp,
                           map,
                           eQMI_DMS_SET_FIRMWARE_PREF );

    return rCode;
}
