/*
 * \ingroup dcs
 *
 * \file    qaDcsSLQSGetDeviceMode
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          eQMI_DCS_GET_DEVICE_MODE message.
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

#include "qaDcsSLQSGetDeviceMode.h"
#include "qaGobiApiDcs.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the SLQSGetDeviceMode parameters
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
enum eQCWWANError PkQmiDcsSLQSGetDeviceMode(
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
                     NULL,
                     map,
                     eQMI_DCS_GET_DEVICE_MODE,
                     pMlength);

    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the SLQSGetDeviceMode response message to a
 * user-provided response structure.
 *
 * \param     pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return  eQCWWAN_ERR_NONE
 * \return  eQCWWAN_ERR_MEMORY
 */
enum eQCWWANError  UpkQmiDcsSLQSGetDeviceMode(
    BYTE *pMdmResp,
    struct qm_qmi_response_tlvs_values *pApiResp)
{
    return qm_dcs_get_device_mode_response_unpack( pMdmResp, pApiResp );
}
