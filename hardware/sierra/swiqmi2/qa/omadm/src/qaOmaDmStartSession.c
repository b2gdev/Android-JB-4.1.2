/*
 * \ingroup oma
 *
 * \file    qaOmaDmStartSession.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_OMA_DM_START_SESSION message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaOmaDmStartSession.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the OMADMStartSession Session Type field to the
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
enum eQCWWANError BuildTlvOmaDmSessionType( BYTE *pBuf, BYTE *pParam )
{
    struct QmiOmaDmStartSessionReq *pReq =
        (struct QmiOmaDmStartSessionReq *)pParam;

    /* Add TLV data */
    return PutByte( pBuf, pReq->sessionType );
}

/*
 * This function packs the OMADMStartSession parameters to the QMI
 * message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaGobiApiOmadms.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiOmaDmStartSession(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG sessionType )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SESSION_TYPE, &BuildTlvOmaDmSessionType },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map
                                     */
    };

    struct QmiOmaDmStartSessionReq req;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiOmaDmStartSessionReq));
    req.sessionType = sessionType;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_OMA_START_SESSION,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the OMADMSessionStart response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiOmaDmStartSession(
    BYTE                            *pMdmResp,
    struct QmiOmaDmStartSessionResp *pApiResp )
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
                           eQMI_OMA_START_SESSION );
    return eRCode;
}
