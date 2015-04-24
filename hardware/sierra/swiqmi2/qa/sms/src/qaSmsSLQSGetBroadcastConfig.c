/*
 * \ingroup sms
 *
 * \file  qaSmsSLQSGetBroadcastConfig.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        QMI_WMS_GET_BROADCAST_CONFIG message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

/* include files */
#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaSmsRawRead.h"
#include "qaSmsSLQSGetBroadcastConfig.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the GetBroadcastConfig mode field
 * to the QMI message SDU
 *
 * \param pBuf   - Pointer to storage into which the packed
 *                 data will be placed by this function.
 *
 * \param pParam - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError BuildTlvBroadcastMode( BYTE *pBuf, BYTE *pParam )
{
    struct QmiSmsGetBroadcastConfigReq *pReq =
                     (struct QmiSmsGetBroadcastConfigReq *)pParam;

    /* Insert mode */
    return PutByte( pBuf, pReq->mode );
}

/*
 * Packs the GetBroadcastConfig parameters to the QMI message SDU
 *
 * \param  pParamField[OUT]   - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \param  pMlength[OUT]      - Total length of built message.
 *
 * \param  mode               - Mode
 *                                - 0x00 - CDMA
 *                                - 0x01 - GW
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError PkQmiSmsGetBroadcastConfig(
    WORD *pMlength,
    BYTE *pParamField,
    BYTE mode )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_GET_BROADCAST_CONFIG_INFO, &BuildTlvBroadcastMode },
        { eTLV_TYPE_INVALID,              NULL } /* Important. Sentinel.
                                                 * Signifies last item in map.
                                                 */
    };

    struct QmiSmsGetBroadcastConfigReq req;
    enum   eQCWWANError                eRCode;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiSmsGetBroadcastConfigReq));

    req.mode  = mode ;

    eRCode = qmbuild ( pParamField,
                       (BYTE *)&req,
                       map,
                       eQMI_WMS_GET_BC_CONFIG,
                       pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 *****************************************************************************/

/*
 * This function unpacks the GetBroadcastConfig 3GPPBroadcastCfgInfo from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError UnpackTlv3GPPBroadcastCfgInfo( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiSmsGetBroadcastConfigResp *lResp =
                 (struct QmiSmsGetBroadcastConfigResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD              lTemp  = 0;

    if ( lResp->pBroadcastConfig == NULL )
    {
        return eRCode;
    }

    /* Check for the Buffer Undersize Error */
    lTemp = qmQmiGetResponseTLVlength((struct qmTBuffer *)pTlvData);
    if ( lTemp > sizeof(qaQmi3GPPBroadcastCfgInfo) )
    {
        return eQCWWAN_ERR_BUFFER_SZ;
    }

    /* Extract the parameters into the structure */
    /* Activated index : BYTE */
    eRCode = GetByte( pTlvData,&(lResp->pBroadcastConfig->activated_ind));
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* Number of sets : WORD */
    eRCode = GetWord( pTlvData,&(lResp->pBroadcastConfig->num_instances));
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* Extracting each set of parameters based on num_instances */
    for (lTemp=0 ; lTemp < lResp->pBroadcastConfig->num_instances ;lTemp++)
    {
        /* Starting point of service : WORD */
        eRCode = GetWord( pTlvData,
             &(lResp->pBroadcastConfig->broadcastConfig[lTemp].fromServiceId));
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        /* Ending point of service : WORD */
        eRCode = GetWord( pTlvData,
               &(lResp->pBroadcastConfig->broadcastConfig[lTemp].toServiceId));
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        /* Selected : BYTE */
        eRCode = GetByte( pTlvData,
                 &(lResp->pBroadcastConfig->broadcastConfig[lTemp].selected));
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/*
 * This function unpacks the GetBroadcastConfig 3GPPBroadcastCfgInfo from the
 * QMI response message to a user provided response structure
 *
 * \param  pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param  pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError UnpackTlv3GPP2BroadcastCfgInfo( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiSmsGetBroadcastConfigResp *lResp =
                 (struct QmiSmsGetBroadcastConfigResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD              lTemp  = 0;

    if ( lResp->pCDMABroadcastConfig == NULL )
    {
        return eRCode;
    }

    /* Check for the Buffer Undersize Error */
    lTemp = qmQmiGetResponseTLVlength((struct qmTBuffer *)pTlvData );
    if ( lTemp > sizeof(qaQmi3GPP2BroadcastCfgInfo) )
    {
        return eQCWWAN_ERR_BUFFER_SZ;
    }

    /* Extract the parameters into the structure */
    /* Activated index : BYTE */
    eRCode = GetByte( pTlvData,&(lResp->pCDMABroadcastConfig->activated_ind) );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* Number of sets : WORD */
    eRCode = GetWord( pTlvData,&(lResp->pCDMABroadcastConfig->num_instances) );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    for ( lTemp=0;lTemp < lResp->pCDMABroadcastConfig->num_instances;lTemp++ )
    {
        /* Service Category : WORD */
        eRCode = GetWord( pTlvData,
   &(lResp->pCDMABroadcastConfig->CDMABroadcastConfig[lTemp].serviceCategory));

        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        /* Language : WORD */
        eRCode = GetWord( pTlvData,
         &(lResp->pCDMABroadcastConfig->CDMABroadcastConfig[lTemp].language) );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }

        /* Selected : BYTE */
        eRCode = GetByte( pTlvData,
         &(lResp->pCDMABroadcastConfig->CDMABroadcastConfig[lTemp].selected) );
        if ( eQCWWAN_ERR_NONE != eRCode )
        {
            return eRCode;
        }
    }

    return eRCode;
}

/* This function unpacks the eQMI_WMS_GET_BROADCAST_CONFIG response
 * message to a user-provided response structure.
 *
 * \param  pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp  [OUT]  - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE      - Increment successful
 * \return eQCWWAN_ERR_NO_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError UpkQmiSmsGetBroadcastConfig(
    BYTE                                *pMdmResp,
    struct QmiSmsGetBroadcastConfigResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,                 &qmUnpackTlvResultCode },
        { eTLV_3GPP_BROADCAST_CONFIG_INFO,  &UnpackTlv3GPPBroadcastCfgInfo},
        { eTLV_3GPP2_BROADCAST_CONFIG_INFO, &UnpackTlv3GPP2BroadcastCfgInfo},
        { eTLV_TYPE_INVALID,                NULL }/* Important. Sentinel.
                                                  * Signifies last item in map.
                                                  */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_WMS_GET_BC_CONFIG );
    return eRCode;
}
