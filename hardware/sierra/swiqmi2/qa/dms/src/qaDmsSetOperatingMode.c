/*
 * \ingroup dms
 *
 * \file qaDmsSetOperatingMode.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        eQMI_DMS_SET_OPERATING_MODE message.
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

#include "qaDmsSetOperatingMode.h"

/******************************************************************************
 * Request handling
 ******************************************************************************/

/*
 * This function packs the SetPower OperatingMode field to the
 * QMI message SDU
 *
 * \param   pBuf    [OUT] - Pointer to storage into which the packed
 *                          data will be placed by this function.
 *
 * \param   pParam  [IN]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 */
enum eQCWWANError BuildTlvOperatingMode( BYTE *pBuf, BYTE *pParam )
{
    struct QmiDmsSetOperatingModeReq *pReq =
        (struct QmiDmsSetOperatingModeReq *)pParam;

    /* Add TLV data */
    return PutByte(pBuf, pReq->operatingMode);
}

/*
 * This function packs the SetOperatingMode parameters
 * to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                                  data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 * \sa     qaGobiApiDms.h for remaining parameter descriptions.
 *
 */
enum eQCWWANError PkQmiDmsSetOperatingMode(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG operatingMode )
{
    /* There is no mandatory TLV and hence the map is empty */
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_OPERATING_MODE, &BuildTlvOperatingMode },
        { eTLV_TYPE_INVALID,   NULL } /* Important. Sentinel.
                                       * Signifies last item in map
                                       */
    };

    struct QmiDmsSetOperatingModeReq req;

    enum eQCWWANError eRCode;

    slmemset((CHAR *)&req, 0, sizeof(struct QmiDmsSetOperatingModeReq));
    req.operatingMode = operatingMode;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_DMS_SET_OPERATING_MODE,
                      pMlength );
    return eRCode;
}


/******************************************************************************
 * Response handling
 ******************************************************************************/

/*
 * This function unpacks the SetPower response message to a
 * user-provided response structure.
 *
 * \param  pMdmResp - Pointer to packed response from the modem.
 *
 * \param  pApiResp - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
enum eQCWWANError UpkQmiDmsSetOperatingMode(
    BYTE                              *pMdmResp,
    struct QmiDmsSetOperatingModeResp *pApiResp )
{
    enum eQCWWANError eRCode;

    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,  &qmUnpackTlvResultCode },
        { eTLV_TYPE_INVALID, NULL } /* Important. Sentinel.
                                     * Signifies last item in map.
                                     */
    };

    eRCode = qmunpackresp( pMdmResp,
                           (BYTE *)pApiResp,
                           map,
                           eQMI_DMS_SET_OPERATING_MODE );
    return eRCode;
}
