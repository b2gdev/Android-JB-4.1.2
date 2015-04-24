/*
 *
 * \ingroup : dms 
 *
 * \file    : qaDmsUIMGetIMSI.c
 *
 * \brief   : Contains Packing and UnPacking routines for the 
 *            DMS_UIM_GET_IMSI message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaQmiBasic.h"

#include "qaDmsUIMGetIMSI.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/

/*
 *
 * This function packs the GetDmsUIMGetIMSI parameters to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE, on success
 * \return eQCWWAN_ERR_MEMORY, on error
 *
 */
enum eQCWWANError PkQmiDmsUIMGetIMSI( 
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
                      eQMI_DMS_GET_IMSI,
                      pMlength );

    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/

/*
 *
 * This function unpacks the IMSI from the MSISDN response message 
 * to a user provided response structure
 *
 * \param    pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param    pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return   eQCWWAN_ERR_NONE, on success
 * \return   eQCWWAN_ERR_MEMORY, on error
 */
enum eQCWWANError UnpackTlvIMSI(BYTE* pTlvData, BYTE *pResp)
{
    struct QmiDmsUIMGetIMSIResp *lResp = (struct QmiDmsUIMGetIMSIResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( lResp->IMSISize == 0 )
        return eQCWWAN_ERR_NONE;

    /* Extract the String Parameter */
    /* The size of the string is stored to check for buffer size in API */
    eRCode = qmQmiExtractString( pTlvData,
                                 lResp->pIMSI,
                                 lResp->IMSISize );
    return eRCode;
}

/*
 *
 * This function unpacks the GetIMSI response message to a
 * user-provided response structure.
 *
 * \param    pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param    pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return   eQCWWAN_ERR_NONE, on success
 * \return   eQCWWAN_ERR_MEMORY, on error
 *
 */
enum eQCWWANError UpkQmiDmsUIMGetIMSI(
    BYTE    *pMdmResp,
    struct  QmiDmsUIMGetIMSIResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE              ,&qmUnpackTlvResultCode },
        { eTLV_MOBILE_SUBSCRIBER_ID     ,&UnpackTlvIMSI },
        { eTLV_TYPE_INVALID             ,NULL }  /* Important. Sentinel. 
                                                  * Signifies last item in map.
                                                  */
    };

    eRCode = qmunpackresp( pMdmResp,
                          (BYTE*)pApiResp,
                          map,
                          eQMI_DMS_GET_IMSI);

    return eRCode;
}
