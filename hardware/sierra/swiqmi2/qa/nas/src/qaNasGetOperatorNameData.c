/*
 * \ingroup : nas
 *
 * \file    : qaNasGetOperatorNameData.c
 *
 * \brief   : Contains Packing and UnPacking routines for the
 *            eQMI_NAS_GET_OPERATOR_NAME_DATA message.
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaNasGetOperatorNameData.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * Packs the PkQmiNasGetOperatorNameData parameters to the QMI message SDU
 *
 * \param  pParamField[OUT] - Pointer to storage into which the packed
 *                            data will be placed by this function.
 *
 * \param  pMlength[OUT]    - Total length of built message.
 *
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX on error
 *
 */
enum eQCWWANError PkQmiNasGetOperatorNameData(
    WORD *pMlength,
    BYTE *pParamField )
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
                      eQMI_NAS_GET_OPERATOR_NAME,
                      pMlength );

    return eRCode;
}


/******************************************************************************
 * Response handling
 *****************************************************************************/

/*
 * This function unpacks the Service provide name TLV from the QMI
 * response message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx otherwise
 */
package enum eQCWWANError UnpackTlvServiceProviderName(
    BYTE *pTlvData,
    BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE index = 0;
    serviceProviderName *lResp =
        ((struct QmiNasGetOperatorNameDataResp *)pResp)->pOperatorNameData->\
                                                            pSrvcProviderName;

    if (NULL == lResp)
        return eRCode;

    /* Extract the Service provide name values */
    eRCode = GetByte( pTlvData, &(lResp->displayCondition) );
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
    return eRCode;
}

/*
 * This function unpacks the Operator PLMN list TLV from the QMI response
 * message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx otherwise
 */
package enum eQCWWANError UnpackTlvOperatorPLMNList(
    BYTE *pTlvData,
    BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE index = 0, index_1 = 0;
    operatorPLMNList *lResp =
    ((struct QmiNasGetOperatorNameDataResp *)pResp)->pOperatorNameData->\
                                                            pOperatorPLMNList;

    if (NULL == lResp)
        return eRCode;

    eRCode = GetByte( pTlvData, &(lResp->numInstance) );
    if (eRCode !=  eQCWWAN_ERR_NONE)
    {
        return eRCode;
    }

    index = 0;
    while (index < lResp->numInstance)
    {
        for( index_1 = 0; index_1 <= MAX_MCC_MNC_INDEX; index_1++)
        {
            eRCode = GetByte( pTlvData, &(lResp->PLMNData[index].mcc[index_1]));
            if (eRCode !=  eQCWWAN_ERR_NONE)
            {
                return eRCode;
            }
        }

        for( index_1 = 0; index_1 <= MAX_MCC_MNC_INDEX; index_1++)
        {
            eRCode = GetByte(pTlvData, &(lResp->PLMNData[index].mnc[index_1]));
            if (eRCode !=  eQCWWAN_ERR_NONE)
            {
                return eRCode;
            }
        }

        eRCode = GetWord( pTlvData, &(lResp->PLMNData[index].lac1) );
        if (eRCode !=  eQCWWAN_ERR_NONE)
        {
            return eRCode;
        }

        eRCode = GetWord( pTlvData, &(lResp->PLMNData[index].lac2) );
        if (eRCode !=  eQCWWAN_ERR_NONE)
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData, &(lResp->PLMNData[index].PLMNRecID) );
        if (eRCode !=  eQCWWAN_ERR_NONE)
        {
            return eRCode;
        }
        index++;
    }
    return eRCode;
}

/*
 * This function unpacks the PLMN network name TLV from the QMI response
 * message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx otherwise
 */
package enum eQCWWANError UnpackTlvPLMNNetworkName(
    BYTE *pTlvData,
    BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE index = 0, index_1 = 0;
    PLMNNetworkName *lResp =
    ((struct QmiNasGetOperatorNameDataResp *)pResp)->pOperatorNameData->\
                                                            pPLMNNetworkName;

    if (NULL == lResp)
        return eRCode;

    eRCode = GetByte( pTlvData, &(lResp->numInstance) );
    if (eRCode !=  eQCWWAN_ERR_NONE)
    {
        return eRCode;
    }

    index = 0;
    while (index < lResp->numInstance)
    {
        eRCode = GetByte( pTlvData,
                          &(lResp->PLMNNetName[index].codingScheme) );
        if (eRCode !=  eQCWWAN_ERR_NONE)
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData,
                          &(lResp->PLMNNetName[index].countryInitials) );
        if (eRCode !=  eQCWWAN_ERR_NONE)
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData,
                          &(lResp->PLMNNetName[index].longNameSpareBits) );
        if (eRCode !=  eQCWWAN_ERR_NONE)
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData,
                          &(lResp->PLMNNetName[index].shortNameSpareBits) );
        if (eRCode !=  eQCWWAN_ERR_NONE)
        {
            return eRCode;
        }

        eRCode = GetByte( pTlvData,
                          &(lResp->PLMNNetName[index].longNameLen) );
        if (eRCode !=  eQCWWAN_ERR_NONE)
        {
            return eRCode;
        }

        index_1 = 0;
        while (index_1 < lResp->PLMNNetName[index].longNameLen)
        {
            eRCode = GetByte(pTlvData,
                             &(lResp->PLMNNetName[index].longName[index_1++]));
            if (eRCode !=  eQCWWAN_ERR_NONE)
            {
                return eRCode;
            }
        }

        eRCode = GetByte( pTlvData, &(lResp->PLMNNetName[index].shortNameLen) );
        if (eRCode !=  eQCWWAN_ERR_NONE)
        {
            return eRCode;
        }

        index_1 = 0;
        while (index_1 < lResp->PLMNNetName[index].shortNameLen)
        {
            eRCode = GetByte(pTlvData,
                            &(lResp->PLMNNetName[index].shortName[index_1++]));
            if (eRCode !=  eQCWWAN_ERR_NONE)
            {
                return eRCode;
            }
        }
        index++;
    }
    return eRCode;
}

/*
 * This function unpacks the Operator name string TLV from the QMI response
 * message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx otherwise
 */
package enum eQCWWANError UnpackTlvOperatorNameString(
    BYTE *pTlvData,
    BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE index = 0;
    operatorNameString *lResp =
    ((struct QmiNasGetOperatorNameDataResp *)pResp)->pOperatorNameData->\
                                                        pOperatorNameString;

    if ( NULL == lResp )
        return eRCode;

    for( index = 0; lResp->PLMNName[index] != EOS; index++ )
    {
        eRCode = GetByte( pTlvData, &(lResp->PLMNName[index++]) );
        if (eRCode !=  eQCWWAN_ERR_NONE)
        {
            return eRCode;
        }
    }
    return eRCode;
}

/*
 * This function unpacks the NITZ information TLV from the QMI response message
 * to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return: eQCWWAN_ERR_NONE on success, eQCWWAN_xxx otherwise
 */
package enum eQCWWANError UnpackTlvNITZInformation(
    BYTE *pTlvData,
    BYTE *pResp )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    BYTE index = 0;
    PLMNNetworkNameData *lResp =
    ((struct QmiNasGetOperatorNameDataResp *)pResp)->pOperatorNameData->\
                                                            pNITZInformation;

    if(NULL == lResp)
        return eRCode;

    eRCode = GetByte( pTlvData, &(lResp->codingScheme) );
    if (eRCode !=  eQCWWAN_ERR_NONE)
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &(lResp->countryInitials) );
    if (eRCode !=  eQCWWAN_ERR_NONE)
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &(lResp->longNameSpareBits) );
    if (eRCode !=  eQCWWAN_ERR_NONE)
    {
        return eRCode;
    }

    eRCode = GetByte( pTlvData, &(lResp->shortNameSpareBits) );
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
    return eRCode;
}

/*
 * This function unpacks the Operator Name Data response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp  [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp  [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
enum eQCWWANError UpkQmiNasGetOperatorNameData(
    BYTE                                 *pMdmResp,
    struct QmiNasGetOperatorNameDataResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,           &qmUnpackTlvResultCode },
        { eTLV_SERVICE_PROVIDER_NAME, &UnpackTlvServiceProviderName },
        { eTLV_OPERATOR_PLMN_LIST,    &UnpackTlvOperatorPLMNList },
        { eTLV_PLMN_NETWORK_NAME,     &UnpackTlvPLMNNetworkName },
        { eTLV_OPERATOR_NAME_STRING,  &UnpackTlvOperatorNameString },
        { eTLV_NITZ_INFORMATION,      &UnpackTlvNITZInformation },
        { eTLV_TYPE_INVALID,          NULL } /* Important. Sentinel.
                                              * Signifies last item in map.
                                              */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_NAS_GET_OPERATOR_NAME );
    return eRCode;
}

