/*
 * \ingroup nas
 *
 * \file qaNasGetServingSystem.c
 *
 * \brief  Contains Packing and UnPacking routines for the
 *         QMI_NAS_GET_SERVING_SYSTEM message.
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

#include "qaNasGetServingSystem.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the GetServingSystem parameters
 * to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \param  pRadioIfacesSize  [IN] - Radio Interface Array Size.
 *
 * \param  nameSize          [IN] - Size of the name field.
 *
 * \return eQCWWAN_ERR_NONE, on success
 * \return eQCWWAN_ERR_NO_MEMORY, on error
 *
 */
enum eQCWWANError PkQmiNasGetServingSystem (
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
                      eQMI_NAS_GET_SS_INFO,
                      pMlength );
    return eRCode;
}


/*****************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the RoamingIndicator from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return      eQCWWAN_ERR_NONE, on success
 * \return      eQCWWAN_ERR_NO_MEMORY, on error
 */
enum eQCWWANError UnpackTlvRoamingIndicator( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetServingSystemResp *lResp =
              (struct QmiNasGetServingSystemResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE lTemp;

    if ( lResp->pRoaming == NULL )
        return eRCode;
    /* Fill in the Roaming parameter value */
    eRCode = GetByte( pTlvData, &lTemp );
    *(lResp->pRoaming) = lTemp;
    return eRCode;
}

/*
 * This function unpacks the CurrentPLMN from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return      eQCWWAN_ERR_NONE, on success
 * \return      eQCWWAN_ERR_NO_MEMORY, on error
 */
enum eQCWWANError UnpackTlvCurrentPLMN( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetServingSystemResp *lResp =
              (struct QmiNasGetServingSystemResp *)pResp;
    BYTE lnameSize = 0, lArrayIndex = 0;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( lResp->pMCC  == NULL ||
         lResp->pMNC  == NULL ||
         lResp->pName == NULL )
        return eRCode;

    /* Extract the parameters into the structure */
    eRCode = GetWord( pTlvData, lResp->pMCC );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }
    eRCode = GetWord( pTlvData, lResp->pMNC );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }
    eRCode = GetByte( pTlvData, &lnameSize );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* Check for the Buffer Undersize Error */
        if ( lnameSize > lResp->nameSize )
            return eQCWWAN_ERR_BUFFER_SZ;

    /* Fill the Network Description */
    while ( lnameSize )
    {
        BYTE lTemp;
        eRCode = GetByte( pTlvData, &lTemp );
        if ( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;

        lResp->pName[lArrayIndex] = lTemp;

        lArrayIndex++;
        lnameSize--;
    }
    /* Terminate the string with a NULL */
    lResp->pName[lArrayIndex] = EOS;

    return eRCode;
}

/*
 * This function unpacks the GetServingSystem from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return      eQCWWAN_ERR_NONE, on success
 * \return      eQCWWAN_ERR_NO_MEMORY, on error
 */
enum eQCWWANError UnpackTlvGetServingSystem( BYTE *pTlvData, BYTE *pResp )
{
   struct QmiNasGetServingSystemResp *lResp =
              (struct QmiNasGetServingSystemResp *)pResp;
    BYTE lRadioIfacesSize = 0, lArrayIndex = 0;
    BYTE lTemp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( (lResp->pRegistrationState == NULL) ||
         (lResp->pCSDomain          == NULL) ||
         (lResp->pPSDomain          == NULL) ||
         (lResp->pRAN               == NULL) ||
         (lResp->pRadioIfacesSize   == NULL) ||
         (lResp->pRadioIfaces       == NULL) )
         return eRCode;

    /* Extract the parameters into the structure */
    /* Registration State : Byte */
    eRCode = GetByte( pTlvData, &lTemp );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }
    *(lResp->pRegistrationState) = lTemp;

    /* CS Domain : Byte */
    eRCode = GetByte( pTlvData, &lTemp );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }
    *(lResp->pCSDomain) = lTemp;

    /* PS Domain : Byte */
    eRCode = GetByte( pTlvData, &lTemp );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }
    *(lResp->pPSDomain) = lTemp;

    /* Radio Access Interface : Byte */
    eRCode = GetByte( pTlvData, &lTemp );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }
    *(lResp->pRAN) = lTemp;

    /* First two bytes in the TLV value is the ArraySize */
    eRCode = GetByte( pTlvData, &lRadioIfacesSize );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* Check for the Buffer Undersize Error */
    if ( lRadioIfacesSize > *(lResp->pRadioIfacesSize) )
    {
        *(lResp->pRadioIfacesSize) = lRadioIfacesSize;
        return eQCWWAN_ERR_BUFFER_SZ;
    }

    *(lResp->pRadioIfacesSize) = lRadioIfacesSize;
    /* Fill the Radio Interfaces */
    while ( lRadioIfacesSize )
    {
        eRCode = GetByte( pTlvData, &lTemp );
        if ( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;

        lResp->pRadioIfaces[lArrayIndex] = lTemp;

        lArrayIndex++;
        lRadioIfacesSize--;
    }
    return eQCWWAN_ERR_NONE;
}

/*
 * This function unpacks the DataCapabilities from the
 * QMI response message to a user provided response structure
 *
 * \param    pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack. *
 * \param    pResp    [OUT]     - Pointer to structure containing storage
 *                                to return data for this TLV.
 *
 * \return   eQCWWAN_ERR_NONE, on success
 * \return   eQCWWAN_ERR_NO_MEMORY, on error
 */
enum eQCWWANError UnpackTlvDataCapabilities( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetServingSystemResp *lResp =
              (struct QmiNasGetServingSystemResp *)pResp;
    BYTE lDataCapsLen = 0, lArrayIndex = 0;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if ( lResp->pDataCapsLen == NULL ||
         lResp->pDataCaps    == NULL )
         return eRCode;

    /* Extract the parameters into the structure */
    eRCode = GetByte( pTlvData, &lDataCapsLen );
    if ( eQCWWAN_ERR_NONE != eRCode )
    {
        return eRCode;
    }

    /* Check for the Buffer Undersize Error */
    if ( lDataCapsLen > *(lResp->pDataCapsLen) )
    {
        *(lResp->pDataCapsLen) = lDataCapsLen;
        return eQCWWAN_ERR_BUFFER_SZ;
    }

    *(lResp->pDataCapsLen) = lDataCapsLen;
    /* Fill the DataCapabilities */
    while ( lDataCapsLen )
    {
        BYTE lTemp;
        eRCode = GetByte( pTlvData, &lTemp );
        if ( eQCWWAN_ERR_NONE != eRCode )
            return eRCode;

        lResp->pDataCaps[lArrayIndex] = lTemp;

        lArrayIndex++;
        lDataCapsLen--;
    }
    return eRCode;
}

/*
 * This function unpacks the GetServingSystem response message to to a
 * user-provided response structure.
 *
 * \param     MdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp  [OUT] - Pointer to storage to unpack into.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 * \return    eQCWWAN_ERR_NO_MEMORY, on error
 */
enum eQCWWANError UpkQmiNasGetServingSystem(
    BYTE                              *pMdmResp,
    struct QmiNasGetServingSystemResp *pApiResp)
{
	enum eQCWWANError rCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,        &qmUnpackTlvResultCode },
        { eTLV_SERVING_SYSTEM,     &UnpackTlvGetServingSystem },
        { eTLV_ROAMING_INDICATOR,  &UnpackTlvRoamingIndicator },
        { eTLV_CURRENT_PLMN,       &UnpackTlvCurrentPLMN },
        { eTLV_SERVICE_CAPABILITY, &UnpackTlvDataCapabilities },
        { eTLV_TYPE_INVALID,       NULL }  /* Important. Sentinel.
                                            * Signifies last item in map.
                                            */
    };
    rCode = qmunpackresp( pMdmResp,
                          (BYTE *)pApiResp,
                          map,
                          eQMI_NAS_GET_SS_INFO );
    return rCode;
}
