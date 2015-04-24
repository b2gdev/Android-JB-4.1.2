/*
 * \ingroup sms
 *
 * \file    qaSmsSLQSSetBroadcastConfig.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          eQMI_SMS_SET_BROADCAST_CONFIG message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaGobiApiSms.h"
#include "qaSmsSLQSSetBroadcastConfig.h"

#define MSG_MOD_CDMA 0x00
#define MSG_MOD_GW   0x01

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * Packs the SetBroadcastConfig Mode field to the QMI message SDU
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvSetMode( BYTE *pBuf, BYTE *pParam )
{
    struct QmiSmsSetBroadcastConfigReq *pReq =
        (struct QmiSmsSetBroadcastConfigReq *)pParam;

    /* Add TLV data */
    return PutByte( pBuf, pReq->mode );
}

/*
 * Packs the SetBroadcastConfig 3GPP Broadcast Configuration Information field
 * to the QMI message SDU
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvBroadcastConfigInfo( BYTE *pBuf, BYTE *pParam )
{
    struct QmiSmsSetBroadcastConfigReq *pReq =
        (struct QmiSmsSetBroadcastConfigReq *)pParam;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD lWord, numSize = 0, index = 0;
    BYTE lByte;

    if(pReq->pBroadcastConfig == NULL )
        return eRCode;

    /* Add TLV data */
    numSize = pReq->pBroadcastConfig->num_instances;
    eRCode = PutWord( pBuf, numSize);
    if ( eQCWWAN_ERR_NONE == eRCode )
    {
        while ( numSize-- )
        {
            if ( eQCWWAN_ERR_NONE == eRCode )
            {
                for ( index = 0;
                      index < pReq->pBroadcastConfig->num_instances;
                      index++ )
                {
                    lWord  = pReq->pBroadcastConfig->
                                    broadcastConfig[index].fromServiceId;
                    eRCode = PutWord( pBuf, lWord );
                    if ( eQCWWAN_ERR_NONE != eRCode )
                        return eRCode;
                    lWord  = pReq->pBroadcastConfig->
                                    broadcastConfig[index].toServiceId;
                    eRCode = PutWord( pBuf, lWord );
                    if ( eQCWWAN_ERR_NONE != eRCode )
                        return eRCode;
                    lByte  = pReq->pBroadcastConfig->
                                    broadcastConfig[index].selected;
                    eRCode = PutByte( pBuf, lByte );
                    if ( eQCWWAN_ERR_NONE != eRCode )
                        return eRCode;
                }
            }
        }
    }
    return eRCode;
}

/*
 * Packs the SetBroadcastConfig 3GPP2 Broadcast Configuration Information field
 * to the QMI message SDU
 *
 * \param  pBuf   [OUT] - Pointer to storage into which the packed
 *                        data will be placed by this function.
 *
 * \param  pParam [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildTlvCDMABroadcastConfigInfo( BYTE *pBuf, BYTE *pParam )
{
    struct QmiSmsSetBroadcastConfigReq *pReq =
        (struct QmiSmsSetBroadcastConfigReq *)pParam;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD lWord, numSize = 0, index = 0;
    BYTE lByte;

    if(pReq->pCDMABroadcastConfig == NULL )
        return eRCode;

    /* Add TLV data */
    numSize = pReq->pCDMABroadcastConfig->num_instances;
    eRCode = PutWord( pBuf, numSize);
    if ( eQCWWAN_ERR_NONE == eRCode )
    {
        while ( numSize-- )
        {
            if ( eQCWWAN_ERR_NONE == eRCode )
            {
                for ( index = 0;
                      index < pReq->pCDMABroadcastConfig->num_instances;
                      index++ )
                {
                    lWord  = pReq->pCDMABroadcastConfig->
                                    CDMABroadcastConfig[index].serviceCategory;
                    eRCode = PutWord( pBuf, lWord );
                    if ( eQCWWAN_ERR_NONE != eRCode )
                        return eRCode;
                    lWord  = pReq->pCDMABroadcastConfig->
                                    CDMABroadcastConfig[index].language;
                    eRCode = PutWord( pBuf, lWord );
                    if ( eQCWWAN_ERR_NONE != eRCode )
                        return eRCode;
                    lByte  = pReq->pCDMABroadcastConfig->
                                    CDMABroadcastConfig[index].selected;
                    eRCode = PutByte( pBuf, lByte );
                    if ( eQCWWAN_ERR_NONE != eRCode )
                        return eRCode;
                }
            }
        }
    }
    return eRCode;
}

/*
 * Packs the SetBroadcastConfig parameters to the QMI message SDU
 *
 * \param  pMlength     [OUT] - Total length of built message.
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 * \sa qaGobiApiSms.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiSmsSetBroadcastConfig(
    WORD                       *pMlength,
    BYTE                       *pBuffer,
    BYTE                       mode,
    qaQmi3GPPBroadcastCfgInfo  *pBroadcastConfig,
    qaQmi3GPP2BroadcastCfgInfo *pCDMABroadcastConfig )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SET_MODE,                       &BuildTlvSetMode },
        { eTLV_SET_BROADCAST_CONFIG_INFO,      &BuildTlvBroadcastConfigInfo },
        { eTLV_SET_CDMA_BROADCAST_CONFIG_INFO,
                                           &BuildTlvCDMABroadcastConfigInfo },
        { eTLV_TYPE_INVALID,                    NULL } /* Important. Sentinel.
                                                        * Signifies last item
                                                        * in map
                                                        */
    };

    struct QmiSmsSetBroadcastConfigReq req;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiSmsSetBroadcastConfigReq));
    req.mode = mode;
    if( MSG_MOD_GW == req.mode )
    {
        req.pBroadcastConfig       = pBroadcastConfig;
        req.pCDMABroadcastConfig   = NULL;
    }
    if( MSG_MOD_CDMA == req.mode )
    {
        req.pCDMABroadcastConfig   = pCDMABroadcastConfig;
        req.pBroadcastConfig       = NULL;
    }

    eRCode = qmbuild( pBuffer,
                      (BYTE *)&req,
                      map,
                      eQMI_WMS_SET_BC_CONFIG,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * Unpacks the SetBroadcastConfig response message to a user-provided response
 * structure.
 *
 * \param  pMdmResp           - Pointer to packed response from the modem.
 *
 * \param  pApiResp           - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiSmsSetBroadcastConfig(
    BYTE                                *pMdmResp,
    struct QmiSmsSetBroadcastConfigResp *pApiResp )
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
                           eQMI_WMS_SET_BC_CONFIG );
    return eRCode;
}
