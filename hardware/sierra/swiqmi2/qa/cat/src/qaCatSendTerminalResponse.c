/*
 * \ingroup cat
 *
 * \file    qaCatSendTerminalResponse.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_CAT_SEND_TR  message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCatSendTerminalResponse.h"


/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the CATSendTerminalResponse information
 * field to the QMI message SDU
 *
 * \param pBuf   - Pointer to storage into which the packed
 *                 data will be placed by this function.
 *
 * \param pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError BuildTlvTerminalResponse( BYTE *pBuf, BYTE *pParam )
{
    struct QmiCatSendTerminalResponseReq *pReq =
                     (struct QmiCatSendTerminalResponseReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pReq->pData )
        return eRCode;

    /* Add TLV data */
    eRCode = PutLong( pBuf, pReq->refID );
    if ( eQCWWAN_ERR_NONE == eRCode )
    {
        eRCode = PutWord( pBuf, pReq->dataLen);
        while ( (pReq->dataLen)-- && ( eQCWWAN_ERR_NONE == eRCode ))
            eRCode = PutByte(pBuf, *(pReq->pData)++);
    }
    return eRCode;
}

/*
 * Packs the CATSendTerminalResponse parameters to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError PkQmiCatSendTerminalResponse(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG refID,
    ULONG dataLen,
    BYTE  *pData )

{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TERMINAL_RESPONSE, &BuildTlvTerminalResponse },
        { eTLV_TYPE_INVALID,      NULL } /* Important. Sentinel.
                                         * Signifies last item in map.
                                         */
    };

    struct QmiCatSendTerminalResponseReq req;
    enum   eQCWWANError                  eRCode;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiCatSendTerminalResponseReq));
    req.refID   = refID;
    req.dataLen = dataLen;
    req.pData   = pData;

    eRCode = qmbuild ( pParamField,
                       (BYTE *)&req,
                       map,
                       eQMI_CAT_SEND_TERMINAL,
                       pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the QMI_CAT_SEND_TR response
 * message to a user-provided response structure.
 *
 * \param  MdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp  [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError UpkQmiCatSendTerminalResponse(
    BYTE                                 *pMdmResp,
    struct QmiCatSendTerminalResponseResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };
    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_CAT_SEND_TERMINAL );

    return eRCode;
}
