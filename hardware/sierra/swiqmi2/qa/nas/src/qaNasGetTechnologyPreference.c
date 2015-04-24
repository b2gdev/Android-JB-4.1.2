/*
 * \ingroup nas
 *
 * \file qaNasGetTechnologyPreference.c
 *
 * \brief  Contains Packing and UnPacking routines for the
 *         eQMI_NAS_GET_TECH_PREF message.
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

#include "qaNasGetTechnologyPreference.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the GetTechnologyPreference parameters
 * to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE, on success
 * \return eQCWWAN_ERR_NO_MEMORY, on error
 *
 */
enum eQCWWANError PkQmiNasGetTechPref(
    WORD *pMlength,
    BYTE *pParamField)
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };
    enum eQCWWANError eRCode;

    eRCode = qmbuild( pParamField,
                      NULL,
                      map,
                      eQMI_NAS_GET_TECH_PREF,
                      pMlength );
    return eRCode;
}

/******************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the GetTechnologyPreference ActiveTechnologyPreference
 * from the QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return      eQCWWAN_ERR_NONE, on success
 * \return      eQCWWAN_ERR_NO_MEMORY, on error
 */
enum eQCWWANError UnpackTlvActiveTechPref( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetTechPrefResp *lResp =
                (struct QmiNasGetTechPrefResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD lTemp1;
    BYTE lTemp2;

    if ( (lResp->pActiveTechPref == NULL) ||
         (lResp->pDuration       == NULL) )
         return eRCode;

    eRCode = GetWord( pTlvData, &lTemp1);
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;
    *(lResp->pActiveTechPref) = lTemp1;

    eRCode = GetByte( pTlvData, &lTemp2);
    if ( eQCWWAN_ERR_NONE != eRCode )
        return eRCode;
    *(lResp->pDuration) = lTemp2;

    return eRCode;
}

/*
 * This function unpacks the GetTechnologyPreference
 * PersistentTechnologyPreference from the QMI response
 * message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                   to return data for this TLV.
 *
 * \return      eQCWWAN_ERR_NONE, on success
 * \return      eQCWWAN_ERR_NO_MEMORY, on error
 */
enum eQCWWANError UnpackTlvPersistentTechPref( BYTE *pTlvData, BYTE *pResp )
{
    struct QmiNasGetTechPrefResp *lResp =
                (struct QmiNasGetTechPrefResp *)pResp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    WORD lTemp;
    if ( lResp->pPersistentTechPref == NULL )
         return eRCode;

    eRCode = GetWord( pTlvData, &lTemp);
    *(lResp->pPersistentTechPref) = lTemp;

    return eRCode;
}

/*
 * This function unpacks the GetTechnologyPreference response message to a
 * user-provided response structure.
 *
 * \param     MdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp  [OUT] - Pointer to storage to unpack into.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 * \return    eQCWWAN_ERR_NO_MEMORY, on error
 */
enum eQCWWANError UpkQmiNasGetTechPref(
    BYTE                         *pMdmResp,
    struct QmiNasGetTechPrefResp *pApiResp)
{
	enum eQCWWANError rCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,          &qmUnpackTlvResultCode },
        { eTLV_ACTIVE_TECH_PREF,     &UnpackTlvActiveTechPref },
        { eTLV_PERSISTENT_TECH_PREF, &UnpackTlvPersistentTechPref },
        { eTLV_TYPE_INVALID,          NULL } /* Important. Sentinel.
                                              * Signifies last item in map.
                                              */
    };
    rCode = qmunpackresp( pMdmResp,
                          (BYTE *)pApiResp,
                          map,
                          eQMI_NAS_GET_TECH_PREF );
    return rCode;
}
