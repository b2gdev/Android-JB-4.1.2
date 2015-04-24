/*
 * \ingroup pds
 *
 * \file    qaPdsInjectAbsoluteTimeRef.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_PDS_INJECT_ABSOLUTE_TIME_REF message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaPdsInjectAbsoluteTimeRef.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * Packs the absolute time injection field to the QMI message SDU
 *
 * \param   pBuf[OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam[IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError BuildTlvAbsoluteTimeInjection( BYTE *pBuf, BYTE *pParam )
{
    struct QmiPdsInjectAbsoluteTimeRefReq *pReq =
        (struct QmiPdsInjectAbsoluteTimeRefReq *)pParam;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* insert time in milliseconds */
    eRCode = Put64( pBuf, pReq->timeMsec );

    /* insert time uncertainty in milliseconds */
    if ( eQCWWAN_ERR_NONE == eRCode )
        eRCode = PutLong( pBuf, pReq->timeUncMsec );

    /* insert time base */
    if ( eQCWWAN_ERR_NONE == eRCode )
        eRCode = PutByte( pBuf, pReq->timeBase );

    /* insert force flag */
    if ( eQCWWAN_ERR_NONE == eRCode )
        eRCode = PutByte( pBuf, pReq->forceFlag );
    return eRCode;
}

/*
 * Packs the PdsInjectAbsoluteTimeRef parameters to the QMI message SDU
 *
 * \param  pParamField[OUT]   - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \param  pMlength[OUT]      - Total length of built message.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError PkQmiPdsInjectAbsoluteTimeRef(
    WORD      *pMlength,
    BYTE      *pParamField,
    ULONGLONG timeMsec,
    ULONG     timeUncMsec,
    BYTE      timeBase,
    BYTE      forceFlag )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_ABSOLUTE_TIME_INJECTION, &BuildTlvAbsoluteTimeInjection },
        { eTLV_TYPE_INVALID,            NULL } /* Important. Sentinel.
                                                * Signifies last item in map.
                                                */
    };

    struct QmiPdsInjectAbsoluteTimeRefReq req;

    enum eQCWWANError eRCode;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiPdsInjectAbsoluteTimeRefReq));
    req.timeMsec    = timeMsec;
    req.timeUncMsec = timeUncMsec;
    req.timeBase    = timeBase;
    req.forceFlag   = forceFlag;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_PDS_INJECT_ABS_TIME,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * Unpacks the PdsInjectAbsoluteTimeRef response message to a user-provided
 * response structure.
 *
 * \param  pMdmResp[IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp[OUT] - Pointer to storage to unpack into.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError UpkQmiPdsInjectAbsoluteTimeRef(
    BYTE                                   *pMdmResp,
    struct QmiPdsInjectAbsoluteTimeRefResp *pApiResp )
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
                           eQMI_PDS_INJECT_ABS_TIME );
    return eRCode;
}
