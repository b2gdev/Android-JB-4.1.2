/*
 * \ingroup wds
 *
 * \file qaWdsGetCallDuration.c
 *
 * \brief  Contains Packing and UnPacking routines for the
 *         QMI_WDS_GET_DURATION message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"

#include "qaWdsGetCallDuration.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the GetSessionDuration parameters
 * to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError PkQmiWdsGetCallDuration(
    WORD    *pMlength,
    BYTE    *pParamField)
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    enum eQCWWANError eRCode;

    eRCode = qmbuild (pParamField,
                      NULL,
                      map,
                      eQMI_WDS_GET_DURATION,
                      pMlength);

    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the GetSessionDuration CallDuration from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                           to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvCallDuration(BYTE* pTlvData, BYTE *pResp)
{
    struct QmiWdsGetDurationResp *lResp = (struct QmiWdsGetDurationResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if(!lResp->pCallDuration)
       return eRCode;

    eRCode = GetLongLong(pTlvData, lResp->pCallDuration);

    return eRCode;
}

/*
 * This function unpacks the GetSessionDuration response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp - Pointer to packed response from the modem.
 *
 * \param  pApiResp - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UpkQmiWdsGetCallDuration(
    BYTE   *pMdmResp,
    struct QmiWdsGetDurationResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,     &qmUnpackTlvResultCode },
        { eTLV_CALL_DURATION,   &UnpackTlvCallDuration },
        { eTLV_TYPE_INVALID,    NULL }  /* Important. Sentinel.
                                         * Signifies last item in map.
                                         */
    };

    eRCode = qmunpackresp(pMdmResp,
            (BYTE *)pApiResp,
            map,
            eQMI_WDS_GET_DURATION);

    return eRCode;
}
