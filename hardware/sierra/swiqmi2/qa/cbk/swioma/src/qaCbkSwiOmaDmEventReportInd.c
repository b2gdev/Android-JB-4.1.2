/*
 * \ingroup cbk
 *
 * \file    qaCbkSwiOmaDmEventReportInd.c
 *
 * \brief   Contains UnPacking routines for the
 *          QMI_SWIOMA_DM_EVENT_REPORT_IND message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCbkSwiOmaDmEventReportInd.h"

/* Functions */

/*
 * This function unpacks the SwiOmaDMFota TLV parameters from the QMI
 * indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvOmaDMFota(
    BYTE *pTlvData,
    BYTE *pResp )
{
    USHORT uCount = 0;

    /* Get the required TLV structure to the local pointer */
    struct sessionInfoTlv *lResponse =
        &((struct QmiCbkSwiOmaDmEventStatusReportInd *)pResp)->SITlv;

    /* Get the SwiOmaDmFota TLV structure to the local pointer */
    struct omaDmFotaTlv *lResp = &(lResponse->sessionInfo.omaDmFota);

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the paramters from omaDmFota */
    /* Extract the download state */
    eRCode = GetByte( pTlvData, &(lResp->state) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the user input request */
    eRCode = GetByte( pTlvData, &(lResp->userInputReq) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the user input timeout */
    eRCode = GetWord( pTlvData, &(lResp->userInputTimeout) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the FW download size */
    eRCode = GetLong( pTlvData, &(lResp->fwdloadsize) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the FW download complete */
    eRCode = GetLong( pTlvData, &(lResp->fwloadComplete) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the FW update complete */
    eRCode = GetWord( pTlvData, &(lResp->updateCompleteStatus) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the severity */
    eRCode = GetByte( pTlvData, &(lResp->severity) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the version length */
    eRCode = GetWord( pTlvData, &(lResp->versionlength) );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the version name */
    while((uCount < lResp->versionlength) && (eRCode == eQCWWAN_ERR_NONE))
    {
        eRCode = GetByte( pTlvData, &lResp->version[uCount] );
        uCount++;
    }
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the name length */
    uCount  = 0;
    eRCode = GetWord( pTlvData, &(lResp->namelength) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the name */
    while((uCount <lResp->namelength) && (eRCode == eQCWWAN_ERR_NONE))
    {
        eRCode = GetByte( pTlvData, &lResp->package_name[uCount] );
        uCount++;
    }
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the description length */
    uCount  = 0;
    eRCode = GetWord( pTlvData, &(lResp->descriptionlength) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the description */
    while((uCount < lResp->descriptionlength) && (eRCode == eQCWWAN_ERR_NONE))
    {
        eRCode = GetByte( pTlvData, &lResp->description[uCount] );
        uCount++;
    }

    /* Initilize the remaining structure elements */
    lResponse->TlvPresent = TRUE;
    lResponse->sessionType = QMI_SWIOMA_DM_FOTA;

    return eRCode;
}

/*
 * This function unpacks the OmaDMconfig TLV from the QMI indication
 * message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvOmaDMconfig(
    BYTE *pTlvData,
    BYTE *pResp )
{
    USHORT uCount = 0;

    /* Get the required TLV structure to the local pointer */
    struct sessionInfoTlv *lResponse =
       &((struct QmiCbkSwiOmaDmEventStatusReportInd *)pResp)->SITlv;

    /* Get the SwiOmaDMconfig TLV structure to the local pointer */
    struct omaDmConfigTlv *lResp = &(lResponse->sessionInfo.omaDmConfig);

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the OmaDMconfig TLV parameter */
    /* Extract the state */
    eRCode = GetByte( pTlvData, &(lResp->state) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the user input request */
    eRCode = GetByte( pTlvData, &(lResp->userInputReq) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

        /* Extract the user input timeout */
    eRCode = GetWord( pTlvData, &(lResp->userInputTimeout) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the alert message length */
    eRCode = GetWord( pTlvData, &(lResp->alertmsglength) );
    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the alert message */
    while ((uCount < lResp->alertmsglength) && (eRCode == eQCWWAN_ERR_NONE))
    {
        eRCode = GetByte( pTlvData, &lResp->alertmsg[uCount] );
        uCount++;
    }

    /* Initilize the remaining structure elements */
    lResponse->TlvPresent  = TRUE;
    lResponse->sessionType = QMI_SWIOMA_DM_CONFIG;
  
    return eRCode;
}
/*
 * This function unpacks the Event Report Indication message to a
 * user-provided response structure.
 *
 * \param   pMdmResp - Pointer to packed response from the modem.
 *
 * \param   pApiResp - Pointer to storage to unpack into.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UpkQmiCbkSwiOmaDmEventReportInd(
    BYTE                                      *pMdmResp,
    struct QmiCbkSwiOmaDmEventStatusReportInd *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_IND_OMA_DM_FOTA,   &UnpackCbkTlvOmaDMFota },
        { eTLV_IND_OMA_DM_CONFIG, &UnpackCbkTlvOmaDMconfig },
        { eTLV_TYPE_INVALID,      NULL } /* Important. Sentinel.
                                          * Signifies last item in map.
                                          */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_SWIOMA_EVENT_IND );
    return eRCode;
}
