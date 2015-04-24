/*
 * \ingroup sar
 *
 * \file    qaSarSLQSSetRFSarState.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_SAR_RF_SET_STATE message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaSarSLQSSetRFSarState.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the SarSLQSSetRFSarState SAR RF State field to the
 * QMI message SDU
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvSarRFState( BYTE *pBuf, BYTE *pParam )
{
    struct QmiSarSLQSSetRFStateReq *pReq =
        (struct QmiSarSLQSSetRFStateReq *)pParam;

    /* Add TLV data */
    return PutLong( pBuf, pReq->rf_sar_state );
}

/*
 * This function packs the SarSLQSSetRFSarState parameters to the QMI
 * message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaGobiApiSar.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiSarSLQSSetRFState(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG RFSarState )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SAR_RF_STATE, &BuildTlvSarRFState },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map
                                     */
    };

    struct QmiSarSLQSSetRFStateReq req;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiSarSLQSSetRFStateReq));
    req.rf_sar_state = RFSarState;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_SAR_RF_SET_STATE,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the SarSLQSSetRFSarState response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp      [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp      [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiSarSLQSSetRFState(
    BYTE                            *pMdmResp,
    struct QmiSarSLQSSetRFStateResp *pApiResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

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
                           eQMI_SAR_RF_SET_STATE );
    return eRCode;
}
