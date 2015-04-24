/*
 * \ingroup cat
 *
 * \file    qaCatSendEnvelopeCmd.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_CAT_SEND_ENVELOPE_CMD message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCatSendEnvelopeCmd.h"


/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * Packs the CATSendEnvelopeCommand Envelope Command information
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
enum eQCWWANError BuildTlvenvelopeCommand( BYTE *pBuf, BYTE *pParam )
{
    struct QmiCatSendEnvelopeCmdReq *pReq =
                     (struct QmiCatSendEnvelopeCmdReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    if ( !pReq->pData )
        return eRCode;

    /* Add TLV data */
    eRCode = PutWord( pBuf, pReq->cmdID );
    if ( eQCWWAN_ERR_NONE == eRCode )
    {
        eRCode = PutWord( pBuf, pReq->dataLen);
        if ( eQCWWAN_ERR_NONE == eRCode )
            while ( (pReq->dataLen)-- && ( eQCWWAN_ERR_NONE == eRCode ))
            eRCode = PutByte(pBuf, *(pReq->pData)++);
    }
    return eRCode;
}

/*
 * Packs the CATSendEnvelopeCommand parameters to the QMI message SDU
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
enum eQCWWANError PkQmiCatSendEnvelopeCmd(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG cmdID,
    ULONG dataLen,
    BYTE  *pData )

{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_ENVELOPE_COMMAND, &BuildTlvenvelopeCommand },
        { eTLV_TYPE_INVALID,     NULL } /* Important. Sentinel.
                                         * Signifies last item in map.
                                         */
    };

    struct QmiCatSendEnvelopeCmdReq req;

    enum eQCWWANError eRCode;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiCatSendEnvelopeCmdReq));
    req.cmdID   = cmdID;
    req.dataLen = dataLen;
    req.pData   = pData;

    eRCode = qmbuild ( pParamField,
                       (BYTE *)&req,
                       map,
                       eQMI_CAT_SEND_ENVELOPE,
                       pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the QMI_CAT_SEND_ENVELOPE_CMD response
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
enum eQCWWANError UpkQmiCatSendEnvelopeCmd(
    BYTE                             *pMdmResp,
    struct QmiCatSendEnvelopeCmdResp *pApiResp )
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
                           eQMI_CAT_SEND_ENVELOPE );
    return eRCode;
}
