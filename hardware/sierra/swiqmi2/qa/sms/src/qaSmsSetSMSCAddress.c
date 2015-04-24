/*
 * \ingroup sms
 *
 * \file    qaSmsSetSMSCAddress.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_WMS_SET_SMSC_ADDRESS message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */
#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaSmsSetSMSCAddress.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the SetSMSCAddress Address field
 * to the QMI message SDU
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
enum eQCWWANError BuildTlvSMSCAddress( BYTE *pBuf, BYTE *pParam )
{
    struct QmiSmsSetSMSCAddressReq *pReq =
                     (struct QmiSmsSetSMSCAddressReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Add TLV data */
    while ( (eRCode == eQCWWAN_ERR_NONE) &&
            (*(pReq->pSMSCAddress) != EOS) )
    {
        eRCode = PutByte( pBuf, *(pReq->pSMSCAddress)++ );
    }
    return eRCode;
}

/*
 * This function packs the SetSMSCAddress Type field
 * to the QMI message SDU
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
enum eQCWWANError BuildTlvSMSCType( BYTE *pBuf, BYTE *pParam )
{
    struct QmiSmsSetSMSCAddressReq *pReq =
                     (struct QmiSmsSetSMSCAddressReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Add TLV data */
    while ( (eRCode == eQCWWAN_ERR_NONE) &&
            (*(pReq->pSMSCType) != EOS) )
    {
        eRCode = PutByte( pBuf, *(pReq->pSMSCType)++ );
    }
    return eRCode;
}

/*
 * This function packs the SetSMSCAddressSMS parameters to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                                    data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 * \sa     qaGobiApiSms.h for remaining parameter descriptions.
 *
 */
enum eQCWWANError PkQmiSmsSetSMSCAddress(
    WORD *pMlength,
    BYTE *pParamField,
    CHAR *pSMSCAddress,
    CHAR *pSMSCType )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_ADDRESS_SMSC, &BuildTlvSMSCAddress },
        { eTLV_TYPE_SMSC,    &BuildTlvSMSCType },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };

    enum   eQCWWANError            eRCode;
    struct QmiSmsSetSMSCAddressReq req;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiSmsSetSMSCAddressReq));
    req.pSMSCAddress = pSMSCAddress;
    req.pSMSCType    = pSMSCType;

    eRCode = qmbuild ( pParamField,
                       (BYTE *)&req,
                       map,
                       eQMI_WMS_SET_SMSC_ADDR,
                       pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the SET_SMSC_ADDRESS response
 * message to a user-provided response structure.
 *
 * \param  pMdmResp  [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp  [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError UpkQmiSmsSetSMSCAddress(
    BYTE                            *pMdmResp,
    struct QmiSmsSetSMSCAddressResp *pApiResp )
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
                           eQMI_WMS_SET_SMSC_ADDR );
    return eRCode;
}

