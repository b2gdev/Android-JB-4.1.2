/*
 * \ingroup pds
 *
 * \file    qaPdsInjectRelativeTimeRef.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_PDS_INJECT_RELATIVE_TIME_REF message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaPdsInjectRelativeTimeRef.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * Packs the time injection field to the PdsInjectRelativeTimeRef QMI message
 * SDU
 *
 * \param   pBuf[OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam[IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError BuildTlvTimeInjection( BYTE *pBuf, BYTE *pParam )
{
    struct QmiPdsInjectRelativeTimeRefReq *pReq =
        (struct QmiPdsInjectRelativeTimeRefReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Add TLV data */
    eRCode = Put64( pBuf, pReq->systemTime );
    if ( eQCWWAN_ERR_NONE == eRCode )
        eRCode = PutWord( pBuf, pReq->systemDiscontinuities );
    return eRCode;
}

/*
 * Packs the PdsInjectRelativeTimeRef parameters to the QMI message SDU
 *
 * \param  pParamField[OUT]   - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \param  pMlength[OUT]      - Total length of built message.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError PkQmiPdsInjectRelativeTimeRef(
    WORD      *pMlength,
    BYTE      *pParamField,
    ULONGLONG systemTime,
    USHORT    systemDiscontinuities )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TIME_INJECTION, &BuildTlvTimeInjection },
        { eTLV_TYPE_INVALID,   NULL } /* Important. Sentinel.
                                       * Signifies last item in map.
                                       */
    };

    struct QmiPdsInjectRelativeTimeRefReq req;

    enum eQCWWANError eRCode;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiPdsInjectRelativeTimeRefReq));
    req.systemTime            = systemTime;
    req.systemDiscontinuities = systemDiscontinuities;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_PDS_INJECT_TIME,
                      pMlength );

    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * Unpacks the PdsInjectRelativeTimeRef response message to a user-provided
 * response structure.
 *
 * \param  pMdmResp[IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp[OUT] - Pointer to storage to unpack into.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError UpkQmiPdsInjectRelativeTimeRef(
    BYTE                                   *pMdmResp,
    struct QmiPdsInjectRelativeTimeRefResp *pApiResp )
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
                           eQMI_PDS_INJECT_TIME );
    return eRCode;
}
