/*
 * \ingroup dms
 *
 * \file    qaDmsValSerProgramCode.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_DMS_VALIDATE_SERVICE_PROGRAMMING_CODE message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaDmsValSerProgramCode.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the ValidateSPC SPC field to the
 * QMI message SDU
 *
 * \param   pBuf   - Pointer to storage into which the packed
 *                   data will be placed by this function.
 *
 * \param   pParam - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError BuildTlvSPCVal(BYTE *pBuf, BYTE *pParam)
{
    struct QmiDmsValSerProgramCodeReq *pReq =
                     (struct QmiDmsValSerProgramCodeReq* )pParam;
    BYTE              count = 0;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

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
/*
 * This function packs the Get Model ID parameters
 * to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 * \sa     qaGobiApiDms.h for remaining parameter descriptions.
 *
 */
enum eQCWWANError PkQmiDmsValSerProgramCode (
    WORD *pMlength,
    BYTE *pParamField,
    CHAR *pSPC )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_SPC_VALID,    &BuildTlvSPCVal},
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    struct QmiDmsValSerProgramCodeReq req;
    enum eQCWWANError eRCode;

    slmemset((char *)&req, 0, sizeof(struct QmiDmsValSerProgramCodeReq));
    req.pSPC = pSPC;

    eRCode = qmbuild( pParamField,
                      (BYTE* )&req,
                      map,
                      eQMI_DMS_VALIDATE_SPC,
                      pMlength );
    return eRCode;
}


/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the ValidateSPC response message to a
 * user-provided response structure.
 *
 * \param   pMdmResp - Pointer to packed response from the modem.
 *
 * \param   pApiResp - Pointer to the structure where TLVs are unpacked
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError UpkQmiDmsValSerProgramCode(
    BYTE                               *pMdmResp,
    struct QmiDmsValSerProgramCodeResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE, &qmUnpackTlvResultCode },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };
    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_DMS_VALIDATE_SPC );
    return eRCode;
}
