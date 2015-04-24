/*
 * \ingroup dcs
 *
 * \file    qaDcsQCWWAN2kEnumerateDevices.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          eQMI_DCS_ENUMERATE_DEVICES message.
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

#include "qaDcsQCWWAN2kEnumerateDevices.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the QCWWAN2kEnumerateDevices parameters
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
enum eQCWWANError PkQmiDcsQCWWAN2kEnumerateDevices(
    WORD      *pMlength,
    BYTE      *pParamField )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    enum eQCWWANError eRCode;

    eRCode = qmbuild(pParamField,
                     NULL, // (BYTE*)&req,
                     map,
                     eQMI_DCS_ENUMERATE_DEVICES,
                     pMlength);

    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the QCWWAN2kEnumerateDevices response message to a
 * user-provided response structure.
 *
 * \param     pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return  eQCWWAN_ERR_NONE
 * \return  eQCWWAN_ERR_MEMORY
 */
enum eQCWWANError  UpkQmiDcsQCWWAN2kEnumerateDevices(
    BYTE *pMdmResp,
    struct qm_qmi_response_tlvs_values *pApiResp)
{
    return qm_dcs_enumerate_devices_response_unpack( pMdmResp, pApiResp );
}
