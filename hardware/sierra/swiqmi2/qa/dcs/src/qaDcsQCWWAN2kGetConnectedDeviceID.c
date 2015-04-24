/*
 * \ingroup dcs
 *
 * \file    qaDcsQCWWAN2kGetConnectedDeviceID.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          eQMI_DCS_GET_CONNECTED_DEVICE_ID message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
/* include files */

#include "SwiDataTypes.h"
#include "qaQmiBasic.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"

#include "qaDcsQCWWAN2kGetConnectedDeviceID.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the QCWWAN2kGetConnectedDeviceID parameters
 * to the QMI message SDU
 *
 * \param   pParamField [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \param   pMlength    [OUT] - Total length of built message.
 *
 * \return  eQCWWAN_ERR_NONE
 * \return  eQCWWAN_ERR_MEMORY
 *
 * \sa      qaGobiApiDcs.h for remaining parameter descriptions.
 *
 */
enum eQCWWANError PkQmiDcsQCWWAN2kGetConnectedDeviceID(
    WORD    *pMlength,
    BYTE    *pParamField)
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    enum eQCWWANError eRCode;

    eRCode = qmbuild(pParamField,
                     NULL,
                     map,
                     eQMI_DCS_GET_CONNECTED_DEVICE_ID,
                     pMlength);

    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the QCWWAN2kGetConnectedDeviceID response message to to
 * a user-provided response structure.
 *
 * \param     pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return  eQCWWAN_ERR_NONE
 * \return  eQCWWAN_ERR_MEMORY
 */
enum eQCWWANError UpkQmiDcsQCWWAN2kGetConnectedDeviceID(
    BYTE   *pMdmResp,
    struct qm_qmi_response_tlvs_values *pApiResp )
{
    return qm_dcs_get_connected_device_id_response_unpack( pMdmResp,
                                                           pApiResp );
}

