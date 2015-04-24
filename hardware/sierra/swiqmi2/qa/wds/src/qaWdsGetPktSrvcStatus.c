/*
 * \ingroup wds
 *
 * \file qaWdsGetPktSrvcStatus.c
 *
 * \brief  Contains Packing and UnPacking routines for the
 *         QMI_WDS_GET_PKT_STATUS message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"

#include "qaWdsGetPktSrvcStatus.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the GetSessionState parameters
 * to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError PkQmiWdsGetPktSrvcStatus(
    WORD    *pMlength,
    BYTE    *pParamField)
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_TYPE_INVALID, NULL }  /* Important. Sentinel.
                                      * Signifies last item in map.
                                      */
    };

    enum eQCWWANError eRCode;

    eRCode = qmbuild (pParamField,
                      NULL,
                      map,
                      eQMI_WDS_GET_PKT_STATUS,
                      pMlength);

    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the GetSessionState ConnectionStatus from the
 * QMI response message to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                           to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvConnectionStatus(BYTE *pTlvData, BYTE *pResp)
{
    struct QmiWdsGetPktSrvcStatusResp *lResp =
                (struct QmiWdsGetPktSrvcStatusResp *)pResp;
    BYTE lTemp;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    if (!lResp->pConnectionStatus)
        return eRCode;

    /* pConnectionStatus is a ULONG * but its size asper the document is 1 byte */
    eRCode = GetByte(pTlvData, &lTemp);
    *(lResp->pConnectionStatus) = lTemp;

    return eRCode;
}

/*
 * This function unpacks the GetSessionState response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp - Pointer to packed response from the modem.
 *
 * \param  pApiResp - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UpkQmiWdsGetPktSrvcStatus(
    BYTE   *pMdmResp,
    struct QmiWdsGetPktSrvcStatusResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,          &qmUnpackTlvResultCode },
        { eTLV_CONNECTION_STATUS,    &UnpackTlvConnectionStatus },
        { eTLV_TYPE_INVALID,         NULL }  /* Important. Sentinel.
                                              * Signifies last item in map.
                                              */
    };

    eRCode = qmunpackresp( pMdmResp,
                          (BYTE *)pApiResp,
                          map,
                          eQMI_WDS_GET_PKT_STATUS );
    return eRCode;
}
