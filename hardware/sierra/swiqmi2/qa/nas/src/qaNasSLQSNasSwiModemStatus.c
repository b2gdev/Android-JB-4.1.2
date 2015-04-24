/*
 * \ingroup nas
 *
 * \file qaNasSLQSNasSwiModemStatus.c
 *
 * \brief  Contains Packing and UnPacking routines for the
 *         eQMI_NAS_SWI_MODEM_STATUS message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaNasSLQSNasSwiModemStatus.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the SLQSNasSwiModemStatus parameters to the QMI message
 * SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError PkQmiNasSlqsSwiModemStatus(
    WORD *pMlength,
    BYTE *pParamField )
{
    /* There is no mandatory TLV and hence the map is empty */
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
                      eQMI_NAS_SWI_MODEM_STATUS,
                      pMlength);
    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the Common Information about the modem from the QMI
 * response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvModemCommInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    CommInfo *lResp =  &((struct QmiNasSLQSSwiModemStatusResp *)
                                          pResp)->pModemStatus->commonInfo;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the modem Information */
    eRCode = GetByte( pTlvData, &lResp->temperature );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->modemMode );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->systemMode );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->imsRegState );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    return GetByte( pTlvData, &lResp->psState );
}

/*
 * This function unpacks the LTE info about the modem from the QMI response
 * message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError UnpackTlvModemLTEInfo(
    BYTE *pTlvData,
    BYTE *pResp )
{
    LTEInfo *lResp =  ((struct QmiNasSLQSSwiModemStatusResp *)
                                        pResp)->pModemStatus->pLTEInfo;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Check For NULL */
    if ( NULL == lResp )
    {
        return eQCWWAN_ERR_NONE;
    }

    /* Extract the LTE Info */
    eRCode = GetByte( pTlvData, &lResp->band );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
       return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->bandwidth );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
       return eRCode;
    }

    eRCode = GetWord( pTlvData, &lResp->RXChan );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
       return eRCode;
    }

    eRCode = GetWord( pTlvData, &lResp->TXChan );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
       return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->emmState );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
       return eRCode;
    }

    eRCode = GetByte( pTlvData, &lResp->emmSubState );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
       return eRCode;
    }

    return GetByte( pTlvData, &lResp->emmConnState );
}

/*
 * This function unpacks the SLQSNasSwiModemStatus response message to a
 * user-provided response structure.
 *
 * \param     pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp  [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiNasSlqsSwiModemStatus(
    BYTE                                *pMdmResp,
    struct QmiNasSLQSSwiModemStatusResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,       &qmUnpackTlvResultCode },
        { eTLV_MODEM_COMMOM_INFO, &UnpackTlvModemCommInfo },
        { eTLV_MODEM_LTE_INFO,    &UnpackTlvModemLTEInfo },
        { eTLV_TYPE_INVALID,      NULL }  /* Important. Sentinel.
                                           * Signifies last item in map.
                                           */
    };
    eRCode = qmunpackresp( pMdmResp,
                          (BYTE*)pApiResp,
                          map,
                          eQMI_NAS_SWI_MODEM_STATUS );
    return eRCode;
}
