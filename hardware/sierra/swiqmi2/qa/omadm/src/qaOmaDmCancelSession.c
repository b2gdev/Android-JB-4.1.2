/*
 * \ingroup oma
 *
 * \file    qaOmaDmCancelSession.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_OMA_DM_CANCEL_SESSION message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaOmaDmCancelSession.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the OMADMCancelSession parameters to the QMI message
 * SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaGobiApiOmadms.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiOmaDmCancelSession(
    WORD *pMlength,
    BYTE *pParamField)
{
   /* map is empty as we do not have any mandatory tlv*/
   static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = qmbuild( pParamField,
                      NULL,
                      map,
                      eQMI_OMA_CANCEL_SESSION,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the SLQSOMADMCancelSession response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp     [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp     [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiOmaDmCancelSession(
    BYTE                             *pMdmResp,
    struct QmiOmaDmCancelSessionResp *pApiResp )
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
                           eQMI_OMA_CANCEL_SESSION );
    return eRCode;
}
