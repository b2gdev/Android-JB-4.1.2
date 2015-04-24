/*
 * \ingroup nas
 *
 * \file qaNasGetHomeNetwork.c
 *
 * \brief  Contains Packing and UnPacking routines for the 
 *         QMI_NAS_GET_HOME_NETWORK message.
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

#include "qaNasGetHomeNetwork.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the GetHomeNetwork parameters
 * to the QMI message SDU
 *
 * \param       pParamField [OUT] - Pointer to storage into which the packed
 *                                    data will be placed by this function.
 *
 * \param       pMlength    [OUT] - Total length of built message.
 *
 * \param       nameSize    [IN]  - Size of the Network Descriptor Array.
 *
 * \return      eQCWWAN_ERR_NONE, on success
 * \return      eQCWWAN_ERR_MEMORY, on error
 *
 */
enum eQCWWANError PkQmiNasGetHomeNetwork( 
    WORD     *pMlength, 
    BYTE     *pParamField )
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
                      eQMI_NAS_GET_HOME_INFO,
                      pMlength );
    return eRCode;
}


/*****************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the GetHomeNetwork from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]  - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]     - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return      eQCWWAN_ERR_NONE, on success
 * \return      eQCWWAN_ERR_MEMORY, on error
 */
enum eQCWWANError UnpackTlvGetHomeNetwork(
    BYTE    *pTlvData,
    BYTE    *pResp )
{ 
    struct QmiNasGetHomeNetworkResp *lResp =
        (struct QmiNasGetHomeNetworkResp *)pResp;
    BYTE lnameSize = 0, lNameIndex = 0;
    enum eQCWWANError eRCode;

    /* Extract the parameters into the structure */
    /* Extract the parameters into the structure */
    eRCode  = GetWord (pTlvData, lResp->pMCC);
    if (eQCWWAN_ERR_NONE != eRCode)
    {
        return eRCode;
    }
    eRCode  = GetWord (pTlvData, lResp->pMNC);
    if (eQCWWAN_ERR_NONE != eRCode)
    {
        return eRCode;
    }
    eRCode  = GetByte (pTlvData, &lnameSize);
    if (eQCWWAN_ERR_NONE != eRCode)
    {
        return eRCode;
    }

    /* Check for the Buffer Undersize Error */
    if ( lnameSize > lResp->nameSize )
        return eQCWWAN_ERR_BUFFER_SZ;

    /* Fill the Network Description */
    while ( lnameSize-- )
    {
        eRCode = GetByte ( pTlvData,
                           (BYTE *)&(lResp->pName[lNameIndex++]) );
    }
    /* Terminate the string with a NULL */ 
    lResp->pName[lNameIndex] = 0;
    return eRCode;
}

/*
 * This function unpacks the HomeSystemID from the
 * QMI response message to a user provided response structure
 *
 * \param       pTlvData [IN/OUT]   - Pointer to TLV data from which to unpack.
 *
 * \param       pResp    [OUT]      - Pointer to structure containing storage
 *                                     to return data for this TLV.
 *
 * \return      eQCWWAN_ERR_NONE, on success
 * \return      eQCWWAN_ERR_MEMORY, on error
 */
enum eQCWWANError UnpackTlvHomeSystemID(
    BYTE    *pTlvData,
    BYTE    *pResp )
{ 
    struct QmiNasGetHomeNetworkResp *lResp = 
        (struct QmiNasGetHomeNetworkResp *)pResp;
    enum eQCWWANError eRCode;
   
    if ( !lResp->pSID ||
         !lResp->pNID )
        return eQCWWAN_ERR_NONE;

    /* Extract the parameters into the structure */
    /* Two Byte System ID */
    eRCode  = GetWord (pTlvData, lResp->pSID);
    if (eQCWWAN_ERR_NONE != eRCode)
    {
        return eRCode;
    }
    /* Two Byte Network ID */
    eRCode  = GetWord (pTlvData, lResp->pNID);
    return eRCode;
}

/*
 * This function unpacks the GetHomeNetwork response message to to a
 * user-provided response structure.
 *
 * \param     MdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp  [OUT] - Pointer to storage to unpack into.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 * \return    eQCWWAN_ERR_MEMORY, on error
 */
enum eQCWWANError UpkQmiNasGetHomeNetwork(
    BYTE   *pMdmResp,
    struct QmiNasGetHomeNetworkResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,             &qmUnpackTlvResultCode },
        { eTLV_HOME_NETWORK,            &UnpackTlvGetHomeNetwork },
        { eTLV_HOME_SYSTEM_ID,          &UnpackTlvHomeSystemID },
        { eTLV_TYPE_INVALID,            NULL }  /* Important. Sentinel. 
                                                 * Signifies last item in map.
                                                 */
    };
    eRCode = qmunpackresp( pMdmResp, 
                          (BYTE*)pApiResp,
                          map,
                          eQMI_NAS_GET_HOME_INFO );
    return eRCode;
}
