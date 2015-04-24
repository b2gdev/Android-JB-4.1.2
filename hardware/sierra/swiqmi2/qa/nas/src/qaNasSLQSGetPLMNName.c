/*
 * \ingroup : nas
 *
 * \file    : qaNasSLQSGetPLMNName.c
 *
 * \brief   : Contains Packing and UnPacking routines for the
 *            eQMI_NAS_GET_PLMN_NAME message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaNasSLQSGetPLMNName.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the PLMN name request parameters to the QMI message SDU
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx otherwise
 */
package enum eQCWWANError PkTlvPLMN(
    BYTE *pTlvData,
    BYTE *pReq )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    nasPLMNNameReq *lReq =  (nasPLMNNameReq *)pReq;

    if (NULL == lReq)
        return eRCode;

    eRCode = PutWord( pTlvData, lReq->mcc );
    if (eRCode !=  eQCWWAN_ERR_NONE)
    {
        return eRCode;
    }
    return PutWord( pTlvData, lReq->mnc );
}

/*
 * Packs the PkQmiNasGetPLMNName parameters to the QMI message SDU
 *
 * \param  pParamField[OUT] - Pointer to storage into which the packed
 *                            data will be placed by this function.
 *
 * \param  pMlength[OUT]    - Total length of built message.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError PkQmiNasGetPLMNName(
    WORD            *pMlength,
    BYTE            *pParamField,
    nasPLMNNameReq  *pPLMNNameReq)
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_PLMN,         PkTlvPLMN },
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    enum eQCWWANError eRCode;

    eRCode = qmbuild( pParamField,
                      (BYTE *)pPLMNNameReq,
                      map,
                      eQMI_NAS_GET_PLMN_NAME,
                      pMlength );

    return eRCode;
}


/******************************************************************************
 * Response handling
 *****************************************************************************/


/*
 * This function unpacks the 3GPP EONS PLMN Name from the QMI response message
 * to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx otherwise
 */
package enum eQCWWANError UnpackTlv3GPPEONSPLMNName(
    BYTE *pTlvData,
    BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE index = 0;
    nasPLMNNameResp *lResp = 
                ((struct QmiNasGetPLMNNameResp *)pResp)->pPLMNNameResp;

    if (NULL == lResp)
        return eRCode;

    eRCode = GetByte( pTlvData, &(lResp->spnEncoding) );
    if (eRCode !=  eQCWWAN_ERR_NONE)
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &(lResp->spnLength) );
    if (eRCode !=  eQCWWAN_ERR_NONE)
    {
        return eRCode;
    }

    index = 0;
    while (index < lResp->spnLength)
    {
        eRCode = GetByte( pTlvData, &(lResp->spn[index++]) );
        if (eRCode !=  eQCWWAN_ERR_NONE)
        {
            return eRCode;
        }
    }

    eRCode = GetByte( pTlvData, &(lResp->shortNameEn) );
    if (eRCode !=  eQCWWAN_ERR_NONE)
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &(lResp->shortNameCI) );
    if (eRCode !=  eQCWWAN_ERR_NONE)
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &(lResp->shortNameSB) );
    if (eRCode !=  eQCWWAN_ERR_NONE)
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &(lResp->shortNameLen) );
    if (eRCode !=  eQCWWAN_ERR_NONE)
    {
        return eRCode;
    }

    index = 0;
    while (index < lResp->shortNameLen)
    {
        eRCode = GetByte( pTlvData, &(lResp->shortName[index++]) );
        if (eRCode !=  eQCWWAN_ERR_NONE)
        {
            return eRCode;
        }
    }

    eRCode = GetByte( pTlvData, &(lResp->longNameEn) );
    if (eRCode !=  eQCWWAN_ERR_NONE)
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &(lResp->longNameCI) );
    if (eRCode !=  eQCWWAN_ERR_NONE)
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &(lResp->longNameSB) );
    if (eRCode !=  eQCWWAN_ERR_NONE)
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &(lResp->longNameLen) );
    if (eRCode !=  eQCWWAN_ERR_NONE)
    {
        return eRCode;
    }

    index = 0;
    while (index < lResp->longNameLen)
    {
        eRCode = GetByte( pTlvData, &(lResp->longName[index++]) );
        if (eRCode !=  eQCWWAN_ERR_NONE)
        {
            return eRCode;
        }
    }
    return eRCode;
}

/*
 * This function unpacks the PLMN Name response message to a user-provided
 * response structure.
 *
 * \param  pMdmResp  [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp  [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiNasGetPLMNName(
    BYTE                         *pMdmResp,
    struct QmiNasGetPLMNNameResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,         &qmUnpackTlvResultCode },
        { eTLV_3GPP_EONS_PLMN_NAME, &UnpackTlv3GPPEONSPLMNName },
        { eTLV_TYPE_INVALID,        NULL } /* Important. Sentinel.
                                            * Signifies last item in map.
                                            */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_NAS_GET_PLMN_NAME );
    return eRCode;
}

