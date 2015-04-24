/*
 * \ingroup nas
 *
 * \file PkQmiNasInitateNetworkRegistration.c
 *
 * \brief  Contains Packing and UnPacking routines for the 
 *         QMI_NAS_INITIATE_NETWORK_REGISTRATION message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"

#include "qaNasInitiateNetworkRegistration.h"


/*****************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the IntiateNetworkRegistration 
 * BuildTlvRegisterAction field to the QMI message SDU
 *
 * \param   pBuf   [OUT]  - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 * \return  eQCWWAN_ERR_MEMORY, on error
 *
 */
enum eQCWWANError BuildTlvRegisterAction(
    BYTE    *pBuf,
    BYTE    *pParam)
{
    struct QmiNasInitiateNetworkRegistrationReq *pReq =
        (struct QmiNasInitiateNetworkRegistrationReq *)pParam;

    /* Insert the Signal Strength mask into the QMI message */
    /* Additional Request for RSSI is present in the message */ 
    return PutByte(pBuf, pReq->regType );
}

/*
 * This function packs the IntiateNetworkRegistration 
 * BuildTlvManualRegisterInformation field to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 * \return eQCWWAN_ERR_MEMORY, on error
 *
 */
enum eQCWWANError BuildTlvManualRegisterInformation(
    BYTE    *pBuf,
    BYTE    *pParam )
{
    struct QmiNasInitiateNetworkRegistrationReq *pReq =
        (struct QmiNasInitiateNetworkRegistrationReq *)pParam;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* If the registration is automatic there is nothig to fill here */
    if ( pReq->regType == QMI_NAS_AUTOMATIC_REGISTRATION )
        return eRCode;
    
    /* Insert the MCC MNC and Radio Access Technology */
    /* QMI Request :: QMI_NAS_INITIATE_NETWORK_REGISTER_REQ */ 
    eRCode = PutWord(pBuf, pReq->mcc );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    eRCode = PutWord(pBuf, pReq->mnc );
    if( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;

    eRCode = PutByte(pBuf, pReq->rat );
    return eRCode;
}

/*
 * This function packs the IntiateNetworkRegistration parameters
 * to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \param  regType     [IN] - Registration Type of the TLV.
 *
 * \param  mcc         [IN] - Mobile Country Code.
 *
 * \param  mnc         [IN] - Mobile National Code.
 *
 * \param  rat         [IN] - Radio Access Technology.
 *
 * \return eQCWWAN_ERR_NONE, on success
 * \return eQCWWAN_ERR_MEMORY, on error
 *
 */
enum eQCWWANError PkQmiNasInitiateNetworkRegistration( 
    WORD      *pMlength, 
    BYTE      *pParamField, 
    ULONG     regType,
    WORD      mcc,
    WORD      mnc,
    ULONG     rat )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_REGISTER_ACTION,        &BuildTlvRegisterAction },
        { eTLV_MANUAL_REGISTER_ACTION, &BuildTlvManualRegisterInformation },
        { eTLV_TYPE_INVALID,           NULL }  /* Important. Sentinel. 
                                                * Signifies last item in map.
                                                */
    };

    struct QmiNasInitiateNetworkRegistrationReq req;
    slmemset( (char *)&req, 
              0, 
              sizeof (struct QmiNasInitiateNetworkRegistrationReq) );

    req.regType = regType;
    req.mcc = mcc;
    req.mnc = mnc;
    req.rat = rat;

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
 * This function unpacks the IntiateNetworkRegistration response message to to a
 * user-provided response structure.
 *
 * \param     pMdmResp  [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp  [OUT] - Pointer to storage to unpack into.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 * \return    eQCWWAN_ERR_MEMORY, on error
 */
enum eQCWWANError UpkQmiNasInitiateNetworkRegistration(
    BYTE    *pMdmResp,
    struct  QmiNasInitiateNetworkRegistrationResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,            &qmUnpackTlvResultCode },
        { eTLV_TYPE_INVALID,           NULL }  /* Important. Sentinel. 
                                                * Signifies last item in map.
                                                */
    };
    eRCode = qmunpackresp( pMdmResp,
                          (BYTE*)pApiResp,
                          map,
                          eQMI_NAS_REGISTER_NET );
    return eRCode;
}
