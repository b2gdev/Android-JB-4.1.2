/*
 * \ingroup nas
 *
 * \file    qaNasInitiateAttach.c
 *
 * \brief   Contains Packing and UnPacking routines for the
 *          QMI_NAS_INITIATE_ATTACH message.
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

#include "qaNasInitiateAttach.h"

/*****************************************************************************
 * Request handling
 ******************************************************************************/
/*
 * This function packs the InitiateAttach BuildTlvPSAttachAction
 * field to the QMI message SDU
 *
 * \param   pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param   pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE, on success
 *
 */
enum eQCWWANError BuildTlvPSAttachAction( BYTE *pBuf, BYTE *pParam )
{
      struct QmiNasInitiateAttachReq *pReq =
                    (struct QmiNasInitiateAttachReq *)pParam;

    /* Add TLV data */
    return PutByte( pBuf, pReq->action );
}

/*
 * This function packs the InitiateAttach parameters
 * to the QMI message SDU
 *
 * \param  pParamField [OUT] - Pointer to storage into which the packed
 *                             data will be placed by this function.
 *
 * \param  pMlength    [OUT] - Total length of built message.
 *
 * \return eQCWWAN_ERR_NONE, on success
 * \return eQCWWAN_ERR_NO_MEMORY, Accessed beyond allowed size attempted
 *
 * \sa     qaGobiApiNas.h for remaining parameter descriptions.
 */
enum eQCWWANError PkQmiNasInitiateAttach(
    WORD  *pMlength,
    BYTE  *pParamField,
    ULONG action )
{
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_PS_ATTACH_ACTION, &BuildTlvPSAttachAction },
        { eTLV_TYPE_INVALID,     NULL }  /* Important. Sentinel.
                                          * Signifies last item in map.
                                          */
    };
    enum eQCWWANError              eRCode;
    struct QmiNasInitiateAttachReq req;

    req.action = action;
    eRCode     = qmbuild( pParamField,
                          (BYTE *)&req,
                          map,
                          eQMI_NAS_ATTACH_DETACH,
                          pMlength );
    return eRCode;
}


/*****************************************************************************
 * Response handling
 ******************************************************************************/
/*
 * This function unpacks the InitiateAttach response message to a
 * user-provided response structure.
 *
 * \param     MdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp  [OUT] - Pointer to storage to unpack into.
 *
 * \return    eQCWWAN_ERR_NONE, on success
 * \return    eQCWWAN_ERR_NO_MEMORY, Accessed beyond allowed size attempted
 */
enum eQCWWANError UpkQmiNasInitiateAttach(
    BYTE                            *pMdmResp,
    struct QmiNasInitiateAttachResp *pApiResp)
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
                           eQMI_NAS_ATTACH_DETACH );
    return eRCode;
}
