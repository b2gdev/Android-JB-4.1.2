/*
 * \ingroup voice
 *
 * \file    qaVoiceOriginateUSSD.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_VOICE_ORIGINATE_USSD message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaVoiceOriginateUSSD.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the USS Information fields to the QMI message SDU
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvUSSInfo( BYTE *pBuf, BYTE *pParam )
{
    struct QmiVoiceOriginateUSSDReq *pReq =
        (struct QmiVoiceOriginateUSSDReq *)pParam;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE  index = 0, USSLen;

    /* Add TLV data */
    eRCode = PutByte( pBuf, pReq->pUSSInfo->ussDCS );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    eRCode = PutByte( pBuf, pReq->pUSSInfo->ussLen );
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    USSLen = pReq->pUSSInfo->ussLen;
    for( index  = 0; index < USSLen; index++)
    {
        eRCode = PutByte( pBuf, pReq->pUSSInfo->ussData[index] );
        if ( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;
    }

    return eRCode;
}

/*
 * This function packs the Originate USSD request parameters to the QMI
 * message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaGobiApiUSSD.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiVoiceOriginateUSSD(
    WORD           *pMlength,
    BYTE           *pParamField,
    struct USSInfo *pUSSInfo )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_USS_Information, &BuildTlvUSSInfo },
        { eTLV_TYPE_INVALID,    NULL } /* Important. Sentinel.
                                        * Signifies last item in map
                                        */
    };

    struct QmiVoiceOriginateUSSDReq req;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiVoiceOriginateUSSDReq));
    req.pUSSInfo = pUSSInfo;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_VOICE_ORIGINATE_USSD,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the Originate USSD response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiVoiceOriginateUSSD(
    BYTE                             *pMdmResp,
    struct QmiVoiceOriginateUSSDResp *pApiResp )
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
                           eQMI_VOICE_ORIGINATE_USSD );
    return eRCode;
}
