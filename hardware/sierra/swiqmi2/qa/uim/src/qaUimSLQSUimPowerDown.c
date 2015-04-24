/*
 * \ingroup uim
 *
 * \file    qaUimSLQSUimPowerDown.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_UIM_POWER_DOWN message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaUimSLQSUimPowerDown.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the Power Down Slot parameter to the QMI message SDU.
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvPowerDownSlot( BYTE *pBuf, BYTE *pParam )
{
    UIMPowerDownReq *pReq = ( UIMPowerDownReq *)pParam;

    if ( NULL == pReq )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Insert Slot param */
    return PutByte( pBuf, pReq->slot );
}

/*
 * This function packs the SLQS PowerDown request parameters to the QMI message SDU.
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaUimSLQSUimPowerDown.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiUimSlqsPowerDown(
    WORD            *pMlength,
    BYTE            *pParamField,
    UIMPowerDownReq *pUIMPowerDownReq )
{
    /* There is no mandatory TLV and hence the map is empty */
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_POWER_DOWN_SLOT, &BuildTlvPowerDownSlot },
        { eTLV_TYPE_INVALID,    NULL } /* Important. Sentinel.
                                        * Signifies last item in map
                                        */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pUIMPowerDownReq,
                      map,
                      eQMI_UIM_POWER_DOWN,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the SLQS PowerDown response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiUimSlqsPowerDown(
    BYTE                           *pMdmResp,
    struct QmiUimSLQSPowerDownResp *pApiResp )
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
                           eQMI_UIM_POWER_DOWN );
    return eRCode;
}
