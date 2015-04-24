/*
 *
 * \ingroup : dms 
 *
 * \file    : qaDmsGetMSISDN.c
 *
 * \brief   : Contains Packing and UnPacking routines for the 
 *            QMI_DMS_GET_MSISDN message.
 *
 * Copyright: © 2010 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaQmiBasic.h"
#include "qaDmsGetMSISDN.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/

/*
 *
 * This function packs the GetMSISDN parameters to the QMI message SDU
 *
 * \param     pParamField [OUT] - Pointer to storage into which the packed
 *                                    data will be placed by this function.
 *
 * \param     pMlength    [OUT] - Total length of built message.
 *
 * \return    eQCWWAN_ERR_NONE     - Increment successful
 * \return    eQCWWAN_ERR_MEMORY    - Increment beyond allowed size attempted
 *
 */
enum eQCWWANError PkQmiDmsGetMSISDN( 
    WORD    *pMlength,
    BYTE    *pParamField )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel. 
                                      * Signifies last item in map.
                                      */
    };

    enum eQCWWANError eRCode;

    eRCode = qmbuild( pParamField,
                      NULL,
                      map,
                      eQMI_DMS_GET_MSISDN,
                      pMlength );

    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/

/*
 *
 * This function unpacks the voice number from the QMI response
 * message to a user provided response structure
 *
 * \param    pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param    pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return   eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UnpackTlvVoiceNumber(BYTE* pTlvData, BYTE *pResp)
{
    struct QmiDmsGetMSISDNResp *lResp = (struct QmiDmsGetMSISDNResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( (lResp->pMIN == NULL) ||
         (lResp->voiceNumberSize == 0) )
        return eQCWWAN_ERR_NONE;

    /* Extract the String Parameter */
    /* The size of the string is stored to check for buffer size in API */
    eRCode = qmQmiExtractString( pTlvData,
                                 lResp->pVoiceNumber,
                                 lResp->voiceNumberSize);
    return eRCode;
}

/*
 *
 * This function unpacks the MIN from the MSISDN response message 
 * to a user provided response structure
 *
 * \param    pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param    pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return   eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvMIN(BYTE* pTlvData, BYTE *pResp)
{
    struct QmiDmsGetMSISDNResp *lResp = (struct QmiDmsGetMSISDNResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( (lResp->pMIN == NULL) ||
         (lResp->minSize == 0) )
        return eQCWWAN_ERR_NONE;

    /* Extract the String Parameter */
    /* The size of the string is stored to check for buffer size in API */
    eRCode = qmQmiExtractString( pTlvData,
                                 lResp->pMIN,
                                 lResp->minSize );
    return eRCode;
}

/*
 *
 * This function unpacks the GetMSISDN response message to a
 * user-provided response structure.
 *
 * \param    pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param    pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return   eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UpkQmiDmsGetMSISDN(
    BYTE    *pMdmResp,
    struct  QmiDmsGetMSISDNResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE       ,&qmUnpackTlvResultCode },
        { eTLV_VOICE_NUMBER      ,&UnpackTlvVoiceNumber },
        { eTLV_MOBILE_ID         ,&UnpackTlvMIN },
        { eTLV_TYPE_INVALID      ,NULL }  /* Important. Sentinel. 
                                           * Signifies last item in map.
                                           */
    };

    eRCode = qmunpackresp( pMdmResp,
                          (BYTE*)pApiResp,
                          map,
                          eQMI_DMS_GET_MSISDN);
    return eRCode;
}

