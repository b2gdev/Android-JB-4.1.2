/*
 * \ingroup nas
 *
 * \file    qaCbkNasNetworkTimeInd.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          eQMI_NAS_NETWORK_TIME_IND message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCbkNasNetworkTimeInd.h"

/* Locals */
static BYTE ntitimezone;
static BYTE dayltsavadj;

/* Functions */
/*
 * This function unpacks the Universal Time Information from the QMI indication
 * message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvIndUniversalTime(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    UniversalTime *lResp = &(( nasNetworkTime *)pResp)->universalTime;
    enum eQCWWANError eRCode;

    /* Extract the Universal Time Info */
    eRCode = GetWord( pTlvData, &lResp->year );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->month );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->day );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->hour );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->minute );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->second );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetByte( pTlvData, &lResp->dayOfWeek );
}

/*
 * This function unpacks the Time Zone Information from the QMI indication
 * message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvIndTimeZone(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    nasNetworkTime *lResp = ( nasNetworkTime *)pResp;
    lResp->pTimeZone = &ntitimezone;

    /* Extract the Time Zone Info */
    return GetByte( pTlvData, lResp->pTimeZone );
}

/*
 * This function unpacks the Daylight Saving Adjustment Information from the
 * QMI indication message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UnpackTlvIndDaylightSavingAdjustment(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    nasNetworkTime *lResp = ( nasNetworkTime *)pResp;
    lResp->pDayltSavAdj = &dayltsavadj;

    /* Extract the Daylight Saving Adjustment Info */
    return GetByte( pTlvData, lResp->pDayltSavAdj );
}

/*
 * This function unpacks the Network Time indication message to to a
 * user-provided response structure.
 *
 * \param     pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp  [OUT]  - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiCbkNasNetworkTimeInd(
    BYTE           *pMdmResp,
    nasNetworkTime *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_IND_UNIVERSAL_TIME, &UnpackTlvIndUniversalTime },
        { eTLV_IND_TIME_ZONE,      &UnpackTlvIndTimeZone },
        { eTLV_IND_DAYLT_SAV_ADJ,  &UnpackTlvIndDaylightSavingAdjustment },
        { eTLV_TYPE_INVALID,       NULL }  /* Important. Sentinel.
                                                * Signifies last item in map.
                                                */
    };
    eRCode = qmunpackresp( pMdmResp,
                           (BYTE*)pApiResp,
                           map,
                           eQMI_NAS_NETWORK_TIME_IND );
    return eRCode;
}
