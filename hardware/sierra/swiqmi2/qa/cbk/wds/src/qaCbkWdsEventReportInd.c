/*
 * \ingroup cbk
 *
 * \file    qaCbkWdsEventReportInd.c
 *
 * \brief   Contains UnPacking routines for the
 *          QMI_WDS_EVENT_REPORT_IND message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCbkWdsEventReportInd.h"

/* Functions */

/*
 * This function unpacks the Total Rx Bytes from the
 * QMI indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvRxTotalBytes(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    struct ByteTotalsTlv *lResp =
            &((struct QmiCbkWdsEventStatusReportInd *)pResp)->BTTlv;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the received Byte Totals */
    eRCode = GetLongLong( pTlvData, &(lResp->RxTotalBytes) );

    if ( eRCode == eQCWWAN_ERR_NONE )
        lResp->TlvPresent = TRUE;

    return eRCode;
}

/*
 * This function unpacks the total Tx Bytes from the
 * QMI indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvTxTotalBytes(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    struct ByteTotalsTlv *lResp =
                &((struct QmiCbkWdsEventStatusReportInd *)pResp)->BTTlv;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the Transmitted Byte Totals */
    eRCode = GetLongLong( pTlvData, &(lResp->TxTotalBytes) );

    if ( eRCode == eQCWWAN_ERR_NONE )
        lResp->TlvPresent = TRUE;

    return eRCode;
}

/*
 * This function unpacks the Mip Status from the
 * QMI indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvMIPStatus(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    struct MobileIPStatusTlv *lResp =
                &((struct QmiCbkWdsEventStatusReportInd *)pResp)->MSTlv;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    BYTE lByte = 0;

    /* Extract the Mobile IP Status */
    eRCode = GetByte( pTlvData, &lByte );
    if ( eRCode == eQCWWAN_ERR_NONE )
    {
        lResp->MipStatus = lByte;
        lResp->TlvPresent = TRUE;
    }

    return eRCode;
}

/*
 * This function unpacks the Dormancy Status from the
 * QMI indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvDormancyStatus(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */ 
    struct DormancyStatusTlv *lResp = 
        &((struct QmiCbkWdsEventStatusReportInd *)pResp)->DSTlv;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    BYTE lByte = 0;

    if ( (ULONG)NULL == (ULONG)&lResp->DormancyStatus )
         return eRCode;

    /* Extract the Dormancy Status */
    eRCode = GetByte( pTlvData, &lByte );
    if ( eRCode == eQCWWAN_ERR_NONE )
    {
        lResp->DormancyStatus = lByte;
        lResp->TlvPresent = TRUE;
    }
    return eRCode;
}

/*
 * This function unpacks the Data Bearer Technology from the
 * QMI indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvDataBearerTechnology(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */ 
    struct DataBearerTechTlv *lResp = 
        &((struct QmiCbkWdsEventStatusReportInd *)pResp)->DBTechTlv;

    enum eQCWWANError eRCode;

    /* Set Default to Unknown Data bearer Technology */
    BYTE lMask = 0xFF;

    if ( (ULONG)NULL == (ULONG)&lResp->DataBearerTechnology )
        return eQCWWAN_ERR_NONE;

    /* Extract the Data Bearer Technology */
    eRCode = GetByte( pTlvData,
                      &lMask );

    if ( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    lResp->DataBearerTechnology = (ULONG)lMask;
    lResp->TlvPresent = TRUE;

    return eRCode;
}

/*
 * This function unpacks the Current Data Bearer Technology from the
 * QMI indication message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 */
package enum eQCWWANError UnpackCbkTlvCurrentDataBearerTechnology(
    BYTE *pTlvData,
    BYTE *pResp )
{
    /* Get the required TLV structure to the local pointer */
    struct CurrentDataBearerTechTlv *lResp =
        &((struct QmiCbkWdsEventStatusReportInd *)pResp)->CDBTechTlv;

    enum eQCWWANError eRCode;

    BYTE  lByte;
    ULONG lMask = 0xFF;
    ULONG lSOMask;

    if ( (ULONG)NULL == (ULONG)&lResp->RATMask )
        return eQCWWAN_ERR_NONE;

    /* Set the default mask to unknown */
    lResp->RATMask = (ULONG) lMask;

    /* Extract the current network type */
    eRCode  = GetByte( pTlvData,
                       &lByte );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the rat mask */
    eRCode = GetLong( pTlvData,
                      &lMask );
    if( eRCode != eQCWWAN_ERR_NONE )
        return eRCode;

    /* Extract the service option mask */
    eRCode = GetLong( pTlvData,
                      &lSOMask );

    if( eRCode == eQCWWAN_ERR_NONE )
       lResp->TlvPresent = TRUE;

    lResp->SOMask = lSOMask;

    /* Set value of DataBearer technology based on the current network type
     * and rat mask */
    if( MASKDONOTCARE == lMask || MASKNULLBEARER == lMask )
    {
        /* Set RAT mask to Do Not Care */
        lResp->RATMask = lMask;
    }

    /* Check if network type is 3GPP */
    if( NETWORK3GPP == lByte )
    {
        /* Convert in RAT MASK values to the required output values */
        if( (MASKGPRS & lMask) )
        {
            lResp->RATMask = 0x03;        /* GPRS */
        }

        if( (MASKWCDMA & lMask) )
        {
            lResp->RATMask = 0x04;        /* WCDMA */
        }

        if( (MASKEDGE & lMask) )
        {
            lResp->RATMask = 0x06;        /* EDGE */
        }

        if( (MASKLTE & lMask) )
        {
            lResp->RATMask = 0x0A;        /* LTE */
        }

        if( (MASKHSDPA & lMask) && (MASKWCDMA & lMask) )
        {
            lResp->RATMask = 0x07;        /* HSDPA and WCDMA */
        }

        if( (MASKWCDMA & lMask) && (MASKHSUPA & lMask) )
        {
            lResp->RATMask = 0x08;        /* WCDMA and HSUPA */
        }

        if( (MASKHSDPA & lMask) && (MASKHSUPA & lMask) )
        {
            lResp->RATMask = 0x09;        /* HSDPA and HSUPA */
        }

        if( (MASKHSDPAPLUS & lMask) && (MASKWCDMA & lMask) )
        {
            lResp->RATMask = 0x0C;        /* HSDPA+ and WCDMA */
        }

        if( (MASKHSDPAPLUS & lMask) && (MASKHSUPA & lMask) )
        {
            lResp->RATMask = 0x0D;        /* HSDPA+ and HSUPA */
        }

        if( (MASKDCHSDPAPLUS & lMask) && (MASKWCDMA & lMask) )
        {
            lResp->RATMask = 0x0E;        /* DC_HSDPA+ and WCDMA */
        }

        if( (MASKDCHSDPAPLUS & lMask) && (MASKHSUPA & lMask) )
        {
            lResp->RATMask = 0x0F;        /* DC_HSDPA+ and HSUPA */
        }
    }

    /* Check if network type is 3GPP2*/
    if( NETWORK3GPP2 == lByte )
    {
       if( (MASKCDMA1X & lMask) )
       {
           lResp->RATMask = 0x01;       /* CDMA 1x */
       }

       if( (MASKEVDOREV0 & lMask) )
       {
           lResp->RATMask = 0x02;       /* EV-DO Rev0 */
       }

       /* EV-DO RevA */
       if( (MASKEVDOREVA & lMask) )
       {
           if( (SOMASKEHRPD & lSOMask) )
           {
               lResp->RATMask = 0x0B;  /* EHRPD */
           }
           else
           {
               lResp->RATMask = 0x05;  /* HRPD */
           }
       }
    }

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
package enum eQCWWANError UpkQmiCbkWdsEventReportInd(
    BYTE   *pMdmResp,
    struct QmiCbkWdsEventStatusReportInd *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_DATA_BEARER_TECHNOLOGY,
            &UnpackCbkTlvDataBearerTechnology },
        { eTLV_DORMANCY_STATUS,
            &UnpackCbkTlvDormancyStatus },
        { eTLV_MIP_STATUS,
            &UnpackCbkTlvMIPStatus },
        { eTLV_TX_TOTAL_BYTES,
            &UnpackCbkTlvTxTotalBytes },
        { eTLV_RX_TOTAL_BYTES,
            &UnpackCbkTlvRxTotalBytes },
        { eTLV_CURRENT_DATA_BEARER_TECHNOLOGY,
            &UnpackCbkTlvCurrentDataBearerTechnology },
        { eTLV_TYPE_INVALID,
            NULL }  /* Important. Sentinel.
                     * Signifies last item in map.
                     */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE*)pApiResp,
                           map,
                           eQMI_WDS_EVENT_IND );
    return eRCode;
}
