/*
 * \ingroup pds
 *
 * \file    qaPdsInjectPositionData.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_PDS_INJECT_POSITION_DATA message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaPdsInjectPositionData.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * Packs the PDS position data time stamp field to the QMI message SDU
 *
 * \param   pBuf[OUT]  - Pointer to storage into which the packed
 *                       data will be placed by this function.
 *
 * \param   pParam[IN] - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError BuildTlvTimeStamp( BYTE *pBuf, BYTE *pParam )
{
    struct PDSPositionData *pReq  = (struct PDSPositionData *)pParam;

    if( !pReq->pTimeStamp )
        return eQCWWAN_ERR_NONE;

    /* insert time stamp */
    return Put64( pBuf, *pReq->pTimeStamp );
}

/*
 * Packs the PDS position data latitude field to the QMI message SDU
 *
 * \param   pBuf[OUT]  - Pointer to storage into which the packed
 *                       data will be placed by this function.
 *
 * \param   pParam[IN] - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError BuildTlvLatitude( BYTE *pBuf, BYTE *pParam )
{
    struct PDSPositionData *pReq  = (struct PDSPositionData *)pParam;

    if( !pReq->pLatitude )
        return eQCWWAN_ERR_NONE;

    /* insert latitude information */
    return Put64( pBuf, *pReq->pLatitude );
}

/*
 * Packs the PDS position data longitude field to the QMI message SDU
 *
 * \param   pBuf[OUT]  - Pointer to storage into which the packed
 *                       data will be placed by this function.
 *
 * \param   pParam[IN] - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError BuildTlvLongitude( BYTE *pBuf, BYTE *pParam )
{
    struct PDSPositionData *pReq  = (struct PDSPositionData *)pParam;

    if( !pReq->pLongitude )
        return eQCWWAN_ERR_NONE;

    /* insert time stamp */
    return Put64( pBuf, *pReq->pLongitude );
}

/*
 * Packs the PDS position data altitude with respect to ellipsoid field to
 * the QMI message SDU
 *
 * \param   pBuf[OUT]  - Pointer to storage into which the packed
 *                       data will be placed by this function.
 *
 * \param   pParam[IN] - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError BuildTlvAltitudeFromEllipsoid( BYTE *pBuf, BYTE *pParam )
{
    struct PDSPositionData *pReq  = (struct PDSPositionData *)pParam;

    if( !pReq->pAltitudeWrtEllipsoid )
        return eQCWWAN_ERR_NONE;

    /* insert time stamp */
    return PutLong( pBuf, *pReq->pAltitudeWrtEllipsoid );
}
/*
 * Packs the PDS position data altitude from sea level field to the QMI
 * message SDU
 *
 * \param   pBuf[OUT]  - Pointer to storage into which the packed
 *                       data will be placed by this function.
 *
 * \param   pParam[IN] - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError BuildTlvAltitudeFromSealevel( BYTE *pBuf, BYTE *pParam )
{
    struct PDSPositionData *pReq  = (struct PDSPositionData *)pParam;

    if( !pReq->pAltitudeWrtSealevel )
        return eQCWWAN_ERR_NONE;

    /* insert time stamp */
    return PutLong( pBuf, *pReq->pAltitudeWrtSealevel );
}

/*
 * Packs the PDS position data horizontal uncertainty circular field to the
 * QMI message SDU
 *
 * \param   pBuf[OUT]  - Pointer to storage into which the packed
 *                       data will be placed by this function.
 *
 * \param   pParam[IN] - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError BuildTlvHorizondalUncCircular( BYTE *pBuf, BYTE *pParam )
{
    struct PDSPositionData *pReq  = (struct PDSPositionData *)pParam;

    if( !pReq->pHorizontalUncCircular )
        return eQCWWAN_ERR_NONE;

    /* insert time stamp */
    return PutLong( pBuf, *pReq->pHorizontalUncCircular );
}

/*
 * Packs the PDS position data vertical uncertainty field to the QMI message SDU
 *
 * \param   pBuf[OUT]  - Pointer to storage into which the packed
 *                       data will be placed by this function.
 *
 * \param   pParam[IN] - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError BuildTlvVerticalUnc( BYTE *pBuf, BYTE *pParam )
{
    struct PDSPositionData *pReq  = (struct PDSPositionData *)pParam;

    if( !pReq->pVerticalUnc )
        return eQCWWAN_ERR_NONE;

    /* insert time stamp */
    return PutLong( pBuf, *pReq->pVerticalUnc );
}

/*
 * Packs the PDS position data horizondal confidence field to QMI message SDU
 *
 * \param   pBuf[OUT]  - Pointer to storage into which the packed
 *                       data will be placed by this function.
 *
 * \param   pParam[IN] - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError BuildTlvHorizondalConfidence( BYTE *pBuf, BYTE *pParam )
{
    struct PDSPositionData *pReq  = (struct PDSPositionData *)pParam;

    if( !pReq->pHorizontalConfidence )
        return eQCWWAN_ERR_NONE;

    /* insert time stamp */
    return PutByte( pBuf, *pReq->pHorizontalConfidence );
}

/*
 * Packs the PDS position data vertical confidence field to the QMI message SDU
 *
 * \param   pBuf[OUT]  - Pointer to storage into which the packed
 *                       data will be placed by this function.
 *
 * \param   pParam[IN] - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError BuildTlvVerticalConfidence( BYTE *pBuf, BYTE *pParam )
{
    struct PDSPositionData *pReq  = (struct PDSPositionData *)pParam;

    if( !pReq->pVerticalConfidence )
        return eQCWWAN_ERR_NONE;

    /* insert time stamp */
    return PutByte( pBuf, *pReq->pVerticalConfidence );
}

/*
 * Packs the PDS position data position source field to the QMI message SDU
 *
 * \param   pBuf[OUT]  - Pointer to storage into which the packed
 *                       data will be placed by this function.
 *
 * \param   pParam[IN] - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError BuildTlvPositionSource( BYTE *pBuf, BYTE *pParam )
{
    struct PDSPositionData *pReq  = (struct PDSPositionData *)pParam;

    if( !pReq->pPositionSource )
        return eQCWWAN_ERR_NONE;

    /* insert time stamp */
    return PutByte( pBuf, *pReq->pPositionSource );
}

/*
 * Packs the PDS position data time type field to the QMI message SDU
 *
 * \param   pBuf[OUT]  - Pointer to storage into which the packed
 *                       data will be placed by this function.
 *
 * \param   pParam[IN] - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError BuildTlvTimeType( BYTE *pBuf, BYTE *pParam )
{
    struct PDSPositionData *pReq  = (struct PDSPositionData *)pParam;

    if( !pReq->pTimeType )
        return eQCWWAN_ERR_NONE;

    /* insert time stamp */
    return PutByte( pBuf, *pReq->pTimeType );
}
/*
 * Packs the ResetPDSData parameters to the QMI message SDU
 *
 * \param  pParamField[OUT] - Pointer to storage into which the packed
 *                            data will be placed by this function.
 *
 * \param  pMlength[OUT]    - Total length of built message.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError PkQmiPdsInjectPositionData(
    WORD                   *pMlength,
    BYTE                   *pParamField,
    struct PDSPositionData *pPositionData )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TIMESTAMP,               &BuildTlvTimeStamp },
        { eTLV_LATITUE,                 &BuildTlvLatitude },
        { eTLV_LONGITUDE,               &BuildTlvLongitude },
        { eTLV_ALTITUDE_FROM_ELLIPSOID, &BuildTlvAltitudeFromEllipsoid },
        { eTLV_ALTITUDE_FROM_SEALEVEL,  &BuildTlvAltitudeFromSealevel },
        { eTLV_HORIZONDAL_UNC_CIRCULAR, &BuildTlvHorizondalUncCircular },
        { eTLV_VERTICAL_UNC,            &BuildTlvVerticalUnc },
        { eTLV_HORIZONDAL_CONFIDENCE,   &BuildTlvHorizondalConfidence },
        { eTLV_VERTICAL_CONFIDENCE,     &BuildTlvVerticalConfidence },
        { eTLV_POSITION_SOURCE,         &BuildTlvPositionSource },
        { eTLV_TIME_TYPE,               &BuildTlvTimeType },
        { eTLV_TYPE_INVALID,            NULL } /* Important. Sentinel.
                                                * Signifies last item in map.
                                                */
    };

    enum eQCWWANError eRCode;
    eRCode = qmbuild( pParamField,
                      (BYTE *)pPositionData,
                      map,
                      eQMI_PDS_INJECT_POSITION,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * Unpacks the Inject Position Data response message to a user-provided response
 * structure.
 *
 * \param  pMdmResp[IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp[OUT] - Pointer to storage to unpack into.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError UpkQmiPdsInjectPositionData(
    BYTE                                *pMdmResp,
    struct QmiPdsInjectPositionDataResp *pApiResp )
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
                           eQMI_PDS_INJECT_POSITION );
    return eRCode;
}
