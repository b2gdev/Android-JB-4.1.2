/**
 * \ingroup dms
 *
 * \file qaDmsRestoreFactoryDefaults.c
 *
 * \brief  Contains Packing and UnPacking routines for the
 *   QMI_DMS_RESTORE_FACTORY_DEFAULTS message.
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

#include "qaDmsRestoreFactoryDefaults.h"

/******************************************************************************
 * Local Variables
 ******************************************************************************/

/******************************************************************************
 * Request handling
 ******************************************************************************/

/**
 * This function packs the ResetToFactoryDefaults SPC field to the
 * QMI message SDU
 *
 * \param    pBuf   - Pointer to storage into which the packed
 *                    data will be placed by this function.
 *
 * \param    pParam - Pointer to structure containing data for this TLV.
 *
 * \return   eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvSPCode(BYTE *pBuf, BYTE *pParam)
{
    struct QmiDmsRestoreFactoryDefaultsReq *pReq =
                     (struct QmiDmsRestoreFactoryDefaultsReq *)pParam;
    BYTE              count = 0;;
    enum eQCWWANError eRCode;
    /* Add TLV data */
    while ((pReq->pSPC) && (count < 6))
    {
        eRCode = PutByte(pBuf, *(pReq->pSPC));
        if( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;
        (pReq->pSPC)++;
        count++;
    }
    return eRCode;
}
/**
 * This function packs the Get Model ID parameters
 * to the QMI message SDU
 *
 * \param  pParamField [OUT]      - Pointer to storage into which the packed
 *                                  data will be placed by this function.
 *
 * \param  pMlength    [OUT]      - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE       - Increment successful
 * \return eQCWWAN_ERR_NO_MEMORY  - Accessed beyond allowed size attempted
 *
 * \sa     qaGobiApiDms.h for remaining parameter descriptions.
 *
 */
enum eQCWWANError PkQmiDmsRestoreFactoryDefaults (
    WORD *pMlength,
    BYTE *pParamField,
    CHAR *pSPC )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SPC,          &BuildTlvSPCode },
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    struct QmiDmsRestoreFactoryDefaultsReq req;
    enum eQCWWANError                      eRCode;

    slmemset((char *)&req, 0, sizeof(struct QmiDmsRestoreFactoryDefaultsReq));
    req.pSPC = pSPC;

    eRCode = qmbuild( pParamField,
                      (BYTE* )&req,
                      map,
                      eQMI_DMS_FACTORY_DEFAULTS,
                      pMlength );

    return eRCode;
}


/******************************************************************************
 * Response handling
 ******************************************************************************/
/**
 * This function unpacks the ResetToFactoryDefaults response message  to a
 * user-provided response structure.
 *
 * \param   pMdmResp - Pointer to packed response from the modem.
 *
 * \param   pApiResp - Pointer to storage to unpack into.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UpkQmiDmsRestoreFactoryDefaults(
    BYTE                                    *pMdmResp,
    struct QmiDmsRestoreFactoryDefaultsResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE, &qmUnpackTlvResultCode },
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_DMS_FACTORY_DEFAULTS );
    return eRCode;
}
