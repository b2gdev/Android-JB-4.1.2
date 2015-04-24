/*
 * \ingroup cbk
 *
 * \file    qaCbkNasServingSystemsInd.c
 *
 * \brief   Contains UnPacking routines for the
 *          eQMI_NAS_GET_SERVING_SYSTEM message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCbkNasServingSystemInd.h"

/* Functions */

/*
 * This function unpacks the Serving System TLV from the QMI indication
 * message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvServingSystem(
    BYTE *pTlvData,
    BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    struct ServingSystemTlv *lSSTlv=
            &((struct QmiCbkNasServingSystemInd *)pResp)->SSTlv;
    struct ServingSystemInfo *lResp =
            &(lSSTlv->ServingSystem);

    /* Get all the parameters from Serving System TLV */
    eRCode = GetByte ( pTlvData, &lResp->registrationState );
    if ( eRCode == eQCWWAN_ERR_NONE )
        eRCode = GetByte( pTlvData, &lResp->csAttachState );

    if ( eRCode == eQCWWAN_ERR_NONE )
        eRCode = GetByte( pTlvData, &lResp->psAttachState );

    if ( eRCode == eQCWWAN_ERR_NONE )
        eRCode = GetByte( pTlvData, &lResp->selectedNetwork );

    if ( eRCode == eQCWWAN_ERR_NONE )
        eRCode = GetByte( pTlvData, &lResp->radioInterfaceNo );

    if ( eRCode == eQCWWAN_ERR_NONE )
    {
        BYTE count = 0;
        BYTE len = 0;
        len = lResp->radioInterfaceNo ;
        for ( count = 0; count < len; count++ )
        {
            BYTE lTemp;
            eRCode = GetByte ( pTlvData, &lTemp);
            lResp->radioInterfaceList[count] = lTemp;
            if ( eRCode != eQCWWAN_ERR_NONE )
                return eRCode;
        }
    }
    lSSTlv->TlvPresent = TRUE;
    return eRCode;
}

/*
 * This function unpacks the Roaming Indicator TLV from the QMI indication
 * message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvRoamingIndValue(
    BYTE *pTlvData,
    BYTE *pResp )
{
    struct RoamingIndicationTlv *lResp =
        &((struct QmiCbkNasServingSystemInd *)pResp)->RITlv;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Get the parameter from Roaming Indicator Value TLV */
    eRCode = GetByte (pTlvData, &lResp->roaming_ind);
    lResp->TlvPresent = TRUE;

    return eRCode;
}

/*
 * This function unpacks the Current PLMN TLV from the QMI indication message
 * to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvCurrentPLMN(
    BYTE *pTlvData,
    BYTE *pResp )
{
    UNUSEDPARAM( pTlvData );
    UNUSEDPARAM( pResp );

    /* To be completed when implementing the related Call-back */
    return eQCWWAN_ERR_NONE;
}

/*
 * This function unpacks the Data Service Capabilities TLV from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvDataServiceCaps(
    BYTE *pTlvData,
    BYTE *pResp )
{
    struct DataCapabilitiesTlv *lResp =
        &((struct QmiCbkNasServingSystemInd *)pResp)->DCTlv;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Get all the parameters from Serving System TLV */
    eRCode = GetByte(pTlvData, &lResp->datacaps_len);
    if ( eRCode == eQCWWAN_ERR_NONE )
    {
        BYTE count = 0;
        BYTE len = 0;
        len = lResp->datacaps_len ;
        for ( count = 0; count < len; count++ )
        {
            BYTE lTemp;
            eRCode = GetByte (pTlvData, &lTemp);
            if ( eRCode != eQCWWAN_ERR_NONE )
                return eRCode;
            lResp->datacaps[count] = lTemp;
        }
        lResp->TlvPresent = TRUE;
    }
    return eRCode;
}

/*
 * This function unpacks the Current PLMN TLV from the QMI indication message
 * to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvCDMASystemId(
    BYTE *pTlvData,
    BYTE *pResp )
{
    UNUSEDPARAM( pTlvData );
    UNUSEDPARAM( pResp );

    /* To be completed when implementing the related Call-back */
    return eQCWWAN_ERR_NONE;
}

/*
 * This function unpacks the Current PLMN TLV from the QMI indication message
 * to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvCDMABaseStationInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    UNUSEDPARAM( pTlvData );
    UNUSEDPARAM( pResp );

    /* To be completed when implementing the related Call-back */
    return eQCWWAN_ERR_NONE;
}

/*
 * This function unpacks the Current PLMN TLV from the QMI indication message
 * to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvRoamingIndList(
    BYTE *pTlvData,
    BYTE *pResp )
{
    UNUSEDPARAM( pTlvData );
    UNUSEDPARAM( pResp );

    /* To be completed when implementing the related Call-back */
    return eQCWWAN_ERR_NONE;
}

/*
 * This function unpacks the Current PLMN TLV from the QMI indication message
 * to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvDefaultRoamingInd(
    BYTE *pTlvData,
    BYTE *pResp )
{
    UNUSEDPARAM( pTlvData );
    UNUSEDPARAM( pResp );

    /* To be completed when implementing the related Call-back */
    return eQCWWAN_ERR_NONE;
}

/*
 * This function unpacks the Current PLMN TLV from the QMI indication message
 * to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlv3GPP2TimeZone(
    BYTE *pTlvData,
    BYTE *pResp )
{
    UNUSEDPARAM( pTlvData );
    UNUSEDPARAM( pResp );

    /* To be completed when implementing the related Call-back */
    return eQCWWAN_ERR_NONE;
}

/*
 * This function unpacks the Current PLMN TLV from the QMI indication message
 * to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvCDMAPRevInUse(
    BYTE *pTlvData,
    BYTE *pResp )
{
    UNUSEDPARAM( pTlvData );
    UNUSEDPARAM( pResp );

    /* To be completed when implementing the related Call-back */
    return eQCWWAN_ERR_NONE;
}

/*
 * This function unpacks the Serving System Indication to a
 * user-provided response structure.
 *
 * \param   pMdmResp - Pointer to packed response from the modem.
 *
 * \param   pApiResp - Pointer to storage to unpack into.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UpkQmiCbkNasServingSystemInd(
    BYTE                             *pMdmResp,
    struct QmiCbkNasServingSystemInd *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_SERVING_SYSTEM,         &UnpackCbkTlvServingSystem },
        { eTLV_ROAMING_IND_VALUE,      &UnpackCbkTlvRoamingIndValue },
        { eTLV_CURRENT_PLMN,           &UnpackCbkTlvCurrentPLMN },
        { eTLV_DATA_SERVICE_CAPS,      &UnpackCbkTlvDataServiceCaps },
        { eTLV_CDMA_SYSTEM_ID,         &UnpackCbkTlvCDMASystemId },
        { eTLV_CDMA_BASE_STATION_INFO, &UnpackCbkTlvCDMABaseStationInfo },
        { eTLV_ROAMING_IND_LIST,       &UnpackCbkTlvRoamingIndList },
        { eTLV_DEFAULT_ROAMING_IND,    &UnpackCbkTlvDefaultRoamingInd },
        { eTLV_3GPP2_TIME_ZONE,        &UnpackCbkTlv3GPP2TimeZone },
        { eTLV_CDMA_P_REV_IN_USE,      &UnpackCbkTlvCDMAPRevInUse },
        { eTLV_TYPE_INVALID,           NULL } /* Important. Sentinel.
                                               * Signifies last item in map.
                                               */
    };
    eRCode = qmunpackresp( pMdmResp,
                           (BYTE*)pApiResp,
                           map,
                           eQMI_NAS_SS_INFO_IND );
    return eRCode;
}
