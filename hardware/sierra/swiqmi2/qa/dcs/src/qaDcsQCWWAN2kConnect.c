/*
 * \ingroup dcs
 *
 * \file    qaDcsQCWWAN2kConnect.c
 *
 * \brief   Contains Packing and UnPacking routines for the eQMI_DCS_CONNECT
 *          message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"

#include "qaDcsQCWWAN2kConnect.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the QCWWAN2kConnect parameters
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
enum eQCWWANError PkQmiDcsQCWWAN2kConnect(
    WORD    *pMlength,
    BYTE    *pParamField,
    CHAR    *pDeviceID,
    CHAR    *pDeviceKey )
{
    return qm_dcs_connect_request_pack( pDeviceID,
                                        pDeviceKey,
                                        pParamField,
                                        pMlength );
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the QCWWAN2kConnect response message to to a
 * user-provided response structure.
 *
 * \param     pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return    eQCWWAN_ERR_NONE
 * \return    eQCWWAN_ERR_MEMORY
 */
enum eQCWWANError UpkQmiDcsQCWWAN2kConnect(
    BYTE   *pMdmResp,
    struct  QmiDcsQCWWAN2kConnectResp *pApiResp)
{
    enum eQCWWANError rCode;

    static struct qmTlvUnpackerItem map[] =
    {
            { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
            { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    rCode = qmunpackresp ( pMdmResp,
                           (BYTE*)pApiResp,
                           map,
                           eQMI_DCS_CONNECT);

    return rCode;
}
