/*
 *
 * \ingroup  dms 
 *
 * \file     qaDmsGetPRLVersion.c
 *
 * \brief    Contains Packing and UnPacking routines for the 
 *           QMI_DMS_GET_PRL_VERSION message.
 *
 * Copyright: © 2010 Sierra Wireless, Inc. all rights reserved
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaQmiBasic.h"
#include "qaDmsGetPRLVersion.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the GetPRLVersion parameters to the QMI message SDU
 *
 * \param      pParamField [OUT] - Pointer to storage into which the packed
 *                                    data will be placed by this function.
 *
 * \param      pMlength    [OUT] - Total length of built message.
 * 
 * \return     eQCWWAN_ERR_NONE     - Increment successful
 * \return     eQCWWAN_ERR_MEMORY    - Increment beyond allowed size attempted
 *
 */
enum eQCWWANError PkQmiDmsGetPRLVersion( WORD *pMlength, BYTE *pParamField )
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
                      eQMI_DMS_GET_PRL_VERSION,
                      pMlength );

    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the GetPRLVersion PRLVersion from the QMI response 
 * message to a user provided response structure
 *
 * \param     pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param     pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return    eQCWWAN_ERR_NONE
 *
 */
enum eQCWWANError UnpackTlvPRLVersion(BYTE* pTlvData, BYTE *pResp)
{
    struct QmiDmsGetPRLVersionResp *lResp =
        (struct QmiDmsGetPRLVersionResp *)pResp;
    enum eQCWWANError eRCode;

    if(lResp->pPRLVersion == NULL)
        return eQCWWAN_ERR_NONE;

    eRCode = GetWord( pTlvData, lResp->pPRLVersion );

    return eRCode;
}

/*
 * This function unpacks the GetModelId response message to a
 * user-provided response structure.
 *
 * \param     pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UpkQmiDmsGetPRLVersion(
    BYTE    *pMdmResp,
    struct  QmiDmsGetPRLVersionResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE       ,&qmUnpackTlvResultCode },
        { eTLV_PRL_VERSION       ,&UnpackTlvPRLVersion },
        { eTLV_TYPE_INVALID      ,NULL }  /* Important. Sentinel. 
                                           * Signifies last item in map.
                                           */
    };

    eRCode = qmunpackresp( pMdmResp,
                          (BYTE*)pApiResp,
                          map,
                          eQMI_DMS_GET_PRL_VERSION );

    return eRCode;
}

