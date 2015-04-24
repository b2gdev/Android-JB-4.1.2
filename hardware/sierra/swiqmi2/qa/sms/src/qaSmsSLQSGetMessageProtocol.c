/*
 * \ingroup sms
 *
 * \file    qaSmsSLQSGetMessageProtocol.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          eQMI_WMS_GET_MESSAGE_PROTOCOL message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaSmsSLQSGetMessageProtocol.h"

/*****************************************************************************
 * Request handling
 *****************************************************************************/
/*
 * This function packs the SLQS Get Message Protocol request parameters to
 * the QMI message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed data
 *                              will be placed by this function.
 *
 * \param pSetRoutesReq [IN]  - structure with values to be packed
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError PkQmiSmsSLQSGetMessageProtocol(
    WORD *pMlength,
    BYTE *pParamField )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID, NULL }/* Important. Sentinel.
                                    * Signifies last item in map
                                    */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode =  qmbuild( pParamField,
                       NULL,
                       map,
                       eQMI_WMS_GET_MSG_PROTOCOL,
                       pMlength );
    return eRCode;
}

/*****************************************************************************
 * Response handling
 *****************************************************************************/
/*
 * Unpacks the GetMessageProtocol from the QMI response message to a user
 * provided response structure
 *
 * \param       pTlvData [IN/OUT] - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]    - Pointer to structure containing storage
 *                                  to return data for this TLV.
 *
 * \return      eQCWWAN_ERR_NONE, on success
 * \return      eQCWWAN_ERR_NO_MEMORY, Access beyond allowed size attempted
 */
enum eQCWWANError UnpackTlvMessagesProtocol( BYTE *pTlvData, BYTE *pResp )
{
    smsMsgprotocolResp *lResp =
        ( ( struct QmiSmsSLQSGetMsgProtocolResp * )pResp )->pMessageProtocol;

    /* Extract the parameters into the structure */
    return GetByte( pTlvData, &lResp->msgProtocol );
}

/*
 * This function unpacks the SLQS Get Message Protocol response to
 * a user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiSmsSLQSMessageProtocol(
    BYTE                                *pMdmResp,
    struct QmiSmsSLQSGetMsgProtocolResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,      &qmUnpackTlvResultCode },
        { eTLV_MESSAGE_PROTOCOL, &UnpackTlvMessagesProtocol },
        { eTLV_TYPE_INVALID,     NULL } /* Important. Sentinel.
                                         * Signifies last item in map.
                                         */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_WMS_GET_MSG_PROTOCOL );
    return eRCode;
}
