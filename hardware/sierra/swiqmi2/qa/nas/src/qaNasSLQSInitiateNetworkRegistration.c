/*
 * \ingroup nas
 *
 * \file   qaNasSLQSInitateNetworkRegistration.c
 *
 * \brief  Contains Packing and UnPacking routines for the
 *         QMI_NAS_INITIATE_NETWORK_REGISTRATION message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaGobiApiNas.h"
#include "qaNasSLQSInitiateNetworkRegistration.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the Register Action parametrs to the QMI message SDU
 *
 * \param   pBuf   [OUT]  - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildNasTlvRegisterAction(
    BYTE *pBuf,
    BYTE *pParam)
{
    nasInitNetworkReg *pReq =
    ((struct QmiNasSLQSInitiateNetworkRegistrationReq *)pParam)->
                                              pNasInitNetworkReg;

    /* Insert the Registration Action */
    return PutByte(pBuf, pReq->regAction );
}

/*
 * This function packs the MCC, MNC and Radio Access Technology parameters to
 * the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildNasTlvManualRegisterInfo(
    BYTE *pBuf,
    BYTE *pParam )
{
    nasInitNetworkReg *pReq =
    ((struct QmiNasSLQSInitiateNetworkRegistrationReq *)pParam)->
                                              pNasInitNetworkReg;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* If the registration is automatic there is nothig to fill here */
    if ( AUTOMATIC_REGISTRATION == pReq->regAction )
    {  
        return eRCode;
    }

    if( NULL == pReq->pMNRInfo )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert the MCC MNC and Radio Access Technology */
    /* QMI Request :: QMI_NAS_INITIATE_NETWORK_REGISTER_REQ */
    eRCode = PutWord(pBuf, pReq->pMNRInfo->mcc );
    if( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = PutWord(pBuf, pReq->pMNRInfo->mnc );
    if( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return PutByte(pBuf, pReq->pMNRInfo->rat );
}

/*
 * This function packs the Change Action parameters to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildNasTlvChangeDuration(
    BYTE *pBuf,
    BYTE *pParam )
{
    nasInitNetworkReg *pReq =
    ((struct QmiNasSLQSInitiateNetworkRegistrationReq *)pParam)->
                                              pNasInitNetworkReg;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* If the registration is automatic there is nothing to fill here */
    if ( AUTOMATIC_REGISTRATION == pReq->regAction )
    {
        return eRCode;
    }

    if( NULL == pReq->pChangeDuration )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert the Change Duration */
    /* QMI Request :: QMI_NAS_INITIATE_NETWORK_REGISTER_REQ */
    return PutByte(pBuf, *pReq->pChangeDuration );
}

/*
 * This function packs the MNCPCSDigitInclude Status parameter to the
 * QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError BuildNasTlvMNCPCSDigitStatus(
    BYTE *pBuf,
    BYTE *pParam )
{
    nasInitNetworkReg *pReq =
    ((struct QmiNasSLQSInitiateNetworkRegistrationReq *)pParam)->
                                              pNasInitNetworkReg;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* If the registration is automatic there is nothing to fill here */
    if ( AUTOMATIC_REGISTRATION == pReq->regAction )
    { 
        return eRCode;
    }

    if( NULL == pReq->pMncPcsDigitStatus )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Insert the Change Duration */
    /* QMI Request :: QMI_NAS_INITIATE_NETWORK_REGISTER_REQ */
    return PutByte(pBuf, *pReq->pMncPcsDigitStatus );
}

/*
 * This function packs the SLQSIntiateNetworkRegistration parameters
 * to the QMI message SDU
 *
 * \param  pParamField      [OUT] - Pointer to storage into which the packed
 *                                   data will be placed by this function.
 *
 * \param  pMlength         [OUT] - Total length of built message.
 *
 * \param  pPdsInitNetRegExt [IN] - Pointer to structure of
 *                                  nasInitNetworkReg
 *                                    - See \ref nasInitNetworkReg for more
 *                                      information
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError PkQmiNasSLQSInitiateNetworkRegistration(
    WORD              *pMlength,
    BYTE              *pParamField,
    nasInitNetworkReg *pNasInitNetReg )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_NAS_REGISTER_ACTION,        &BuildNasTlvRegisterAction },
        { eTLV_NAS_MANUAL_REGISTER_ACTION, &BuildNasTlvManualRegisterInfo },
        { eTLV_NAS_CHANGE_DURATION,        &BuildNasTlvChangeDuration },
        { eTLV_NAS_MNC_PCS_DIGIT_STATUS,   &BuildNasTlvMNCPCSDigitStatus },
        { eTLV_TYPE_INVALID,               NULL }  /* Important. Sentinel.
                                                    * Signifies last item in map.
                                                    */
    };

    struct QmiNasSLQSInitiateNetworkRegistrationReq req;
    slmemset( (char *)&req,
               0,
               sizeof (struct QmiNasSLQSInitiateNetworkRegistrationReq) );

    req.pNasInitNetworkReg = pNasInitNetReg;

    enum eQCWWANError eRCode;
    eRCode = qmbuild( pParamField,
                      (BYTE*)&req,
                      map,
                      eQMI_NAS_REGISTER_NET,
                      pMlength );
    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the SLQSIntiateNetworkRegistration response message to a
 * user-provided response structure.
 *
 * \param    pMdmResp  [IN]  - Pointer to packed response from the modem.
 *
 * \param    pApiResp  [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiNasSLQSInitiateNetworkRegistration(
    BYTE *pMdmResp,
    struct QmiNasSLQSInitiateNetworkRegistrationResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };
    eRCode = qmunpackresp( pMdmResp,
                          (BYTE*)pApiResp,
                          map,
                          eQMI_NAS_REGISTER_NET );
    return eRCode;
}
