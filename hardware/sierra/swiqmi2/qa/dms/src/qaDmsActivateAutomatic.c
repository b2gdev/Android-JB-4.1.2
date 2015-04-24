/*
 * \ingroup dms
 *
 * \file    qaDmsActivateAutomatic.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_DMS_ACTIVATE_AUTOMATIC message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaQmiBasic.h"
#include "qaDmsActivateAutomatic.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the ActivateAutomatic Activation Code field to the
 * QMI message SDU
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data
 *                        for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError BuildTlvActivationCode( BYTE *pBuf, BYTE *pParam )
{
    struct QmiDmsActivateAutomaticReq *pReq =
                     (struct QmiDmsActivateAutomaticReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( !pReq->pActivationCode )
        return eRCode;

    /* Add TLV data */
    /* First Byte in the TLV is the length of Activation Code */
    eRCode = PutByte( pBuf, slstrlen(pReq->pActivationCode) );

    /* Fill the Activation Code */
    while ( *(pReq->pActivationCode) != EOS &&
            eQCWWAN_ERR_NONE == eRCode )
    {
        eRCode = PutByte( pBuf, *(pReq->pActivationCode)++ );
    }
    return eRCode;
}

/*
 * This function packs the Activate Automatic parameters to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError PkQmiDmsActivateAutomatic(
    WORD *pMlength,
    BYTE *pParamField,
    CHAR *pActivationCode )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_ACTIVATION_CODE, &BuildTlvActivationCode },
        { eTLV_TYPE_INVALID,    NULL } /* Important. Sentinel.
                                        * Signifies last item in map.
                                        */
    };

    struct QmiDmsActivateAutomaticReq req;
    enum   eQCWWANError               eRCode;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiDmsActivateAutomaticReq));
    req.pActivationCode = pActivationCode;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_DMS_ACTIVATE_AUTOMATIC,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the QMI_DMS_ACTIVATE_AUTOMATIC response
 * message to a user-provided response structure.
 *
 * \param  pMdmResp - Pointer to packed response from the modem.
 *
 * \param  pApiResp - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError UpkQmiDmsActivateAutomatic(
    BYTE                               *pMdmResp,
    struct QmiDmsActivateAutomaticResp *pApiResp )
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
                           eQMI_DMS_ACTIVATE_AUTOMATIC );
    return eRCode;
}

