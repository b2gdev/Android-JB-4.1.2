/*
 * \ingroup wds
 *
 * \file qaWdsGetCurDataBearerTechnology.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        QMI_WDS_GET_CUR_DATA_BEARER message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaWdsGetCurDataBearerTechnology.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the GetDataBearerTechnology parameters
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
enum eQCWWANError PkQmiWdsGetCurDataBearerTechnology(
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
                      eQMI_WDS_GET_CUR_DATA_BEARER,
                      pMlength);

    return eRCode;
}

/*****************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the last call data bearer from the QMI response message
 * to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                           to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvLastCallDataBearer(BYTE *pTlvData, BYTE *pResp)
{
    QmiWDSDataBearers *pDataBearers =
                    ((struct QmiWdsGetCurDataBearerResp *)pResp)->pDataBearers;
    QmiWDSDataBearerTechnology *pLastCallDB =
                    pDataBearers->pLastCallDataBearerTechnology;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the last call data bearer technology TLV items */    
    if (pLastCallDB != NULL)
    {
        eRCode = GetByte(pTlvData, &pLastCallDB->currentNetwork);
        if (eRCode == eQCWWAN_ERR_NONE)
        {
            eRCode = GetLong(pTlvData, &pLastCallDB->ratMask);
            if (eRCode == eQCWWAN_ERR_NONE)
            {
                eRCode = GetLong(pTlvData, &pLastCallDB->soMask);

                /* Set the last call data bearer bit-mask */
                if (eRCode == eQCWWAN_ERR_NONE)
                    pDataBearers->dataBearerMask |= 
                                        QMI_WDS_LAST_CALL_DB_MASK;
            }
        }
    }
    return eRCode;
}

/*
 * This function unpacks the current data bearer from the QMI response message
 * to a user provided response structure
 *
 * \param pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param pResp    - Pointer to structure containing storage
 *                           to return data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError UnpackTlvCurrentDataBearer(BYTE *pTlvData, BYTE *pResp)
{
    QmiWDSDataBearers *pDataBearers = 
                    ((struct QmiWdsGetCurDataBearerResp *)pResp)->pDataBearers;
    QmiWDSDataBearerTechnology *pCurDB =
                    pDataBearers->pCurDataBearerTechnology;

    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Extract the current data bearer technology TLV items */    
    if (pCurDB != NULL)
    {
        eRCode = GetByte(pTlvData, &pCurDB->currentNetwork);
        if (eRCode == eQCWWAN_ERR_NONE)
        {
            eRCode = GetLong(pTlvData, &pCurDB->ratMask);
            if (eRCode == eQCWWAN_ERR_NONE)
            {
                eRCode = GetLong(pTlvData, &pCurDB->soMask);

                /* Set the current data bearer bit-mask */
                if (eRCode == eQCWWAN_ERR_NONE)
                   pDataBearers->dataBearerMask |= QMI_WDS_CURRENT_CALL_DB_MASK;
            }
        }
    }
    return eRCode;
}

/*
 * This function unpacks the GetDataBearerTechnology response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp - Pointer to packed response from the modem.
 *
 * \param  pApiResp - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError UpkQmiWdsGetCurDataBearerTechnology(
    BYTE   *pMdmResp,
    struct QmiWdsGetCurDataBearerResp *pApiResp)
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,         &qmUnpackTlvResultCode },
        { eTLV_CURRENT_DATABEARER,  &UnpackTlvCurrentDataBearer },
        { eTLV_LASTCALL_DATABEARER, &UnpackTlvLastCallDataBearer },
        { eTLV_TYPE_INVALID,        NULL }  /* Important. Sentinel.
                                             * Signifies last item in map.
                                             */
    };

    eRCode = qmunpackresp( pMdmResp,
                          (BYTE *)pApiResp,
                          map,
                          eQMI_WDS_GET_CUR_DATA_BEARER );
    return eRCode;
}
