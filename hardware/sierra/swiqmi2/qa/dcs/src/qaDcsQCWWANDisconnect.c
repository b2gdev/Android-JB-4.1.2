/*
 * \ingroup dcs
 *
 * \file    qaDcsQCWWANDisConnect.c
 *
 * \brief   Contains Packing and UnPacking routines for the eQMI_DCS_DISCONNECT
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

#include "qaDcsQCWWANDisconnect.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the QCWWANDisConnect parameters
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
 */
enum eQCWWANError PkQmiDcsQCWWANDisconnect(
    WORD    *pMlength,
    BYTE    *pParamField )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel. 
                                      * Signifies last item in map.
                                      */
    };

    enum eQCWWANError eRCode;

    eRCode = qmbuild( pParamField,
                      NULL, //(BYTE*)&req,
                      map,
                      eQMI_DCS_DISCONNECT,
                      pMlength );

    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the QCWWANDisconnect response message to to a
 * user-provided response structure.
 *
 * \param     pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return  eQCWWAN_ERR_NONE
 * \return  eQCWWAN_ERR_MEMORY
 */
enum eQCWWANError UpkQmiDcsQCWWANDisconnect(
    BYTE   *pMdmResp,
    struct  QmiDcsQCWWANDisconnectResp *pApiResp)
{
    enum eQCWWANError  rCode;

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
                           eQMI_DCS_DISCONNECT );

    return rCode;
}
