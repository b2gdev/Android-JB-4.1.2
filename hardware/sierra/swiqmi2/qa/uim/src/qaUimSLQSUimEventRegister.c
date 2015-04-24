/*
 * \ingroup uim
 *
 * \file    qaUimSLQSUimEventRegister.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_UIM_EVENT_REG message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaUimSLQSUimEventRegister.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the Event mask field to the QMI message SDU
 *
 * \param  pBuf         [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \param  pParam       [IN]  - Pointer to structure containing data for
 *                              this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvEventMask( BYTE *pBuf, BYTE *pParam )
{
    UIMEventRegisterReqResp *pReq = (UIMEventRegisterReqResp *)pParam;

    /* Add TLV data */
    return PutLong( pBuf, pReq->eventMask );
}

/*
 * This function packs the SLQS Event Register request parameters to the
 * QMI message SDU.
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaUimSLQSUimEventRegister.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiUimSlqsEventRegister(
    WORD                    *pMlength,
    BYTE                    *pParamField,
    UIMEventRegisterReqResp *pUIMEventRegisterReqResp )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_EVENT_MASK,   &BuildTlvEventMask },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map
                                     */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pUIMEventRegisterReqResp,
                      map,
                      eQMI_UIM_EVENT_REG,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the EventMask from the  QMI response message to a user
 * provided response structure
 *
 * \param  pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    [OUT]     - Pointer to structure containing storage
 *                              to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success; eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvEventMask( BYTE* pTlvData, BYTE *pResp )
{
    struct QmiUimSLQSEventRegisterResp *lResp =
        (struct QmiUimSLQSEventRegisterResp *)pResp;

    return GetLong( pTlvData, &lResp->eventMask );
}

/*
 * This function unpacks the SLQS Event Register response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiUimSlqsEventRegister(
    BYTE                               *pMdmResp,
    struct QmiUimSLQSEventRegisterResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,     &qmUnpackTlvResultCode },
        { eTLV_EVENT_MASK_RESP, &UnpackTlvEventMask },
        { eTLV_TYPE_INVALID,     NULL } /* Important. Sentinel.
                                         * Signifies last item in map.
                                         */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_UIM_EVENT_REG );
    return eRCode;
}
