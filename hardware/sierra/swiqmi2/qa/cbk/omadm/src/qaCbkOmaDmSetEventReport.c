/*
 * \ingroup cbk
 *
 * \file qaCbkOmaDmSetEventReport.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        QMI_OMA_SET_EVENT_REPORT message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCbkOmaDmSetEventReport.h"

/* Functions */

/* Request handlers */
/*
 * This function packs the OMA-DM Session State field to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
local enum eQCWWANError BuildTlvSessionStateEvent(
    BYTE    *pBuf,
    BYTE    *pParam )
{
    struct QmiCbkOmaDmSetEventReportReq *pReq =
        (struct QmiCbkOmaDmSetEventReportReq *)pParam;

    /* If the parameter has not been set do not fill the TLV */
    if (QMI_CBK_PARAM_NOCHANGE == pReq->eOmaDmSessionStateEvt)
        return eQCWWAN_ERR_NONE;

    return PutByte ( pBuf, pReq->eOmaDmSessionStateEvt );
}

/*
 * This function packs the QMI_OMA_SET_EVENT_REPORT request parameters
 * Set Event Report parameters to the QMI message SDU
 *
 * \param  pMlength    [OUT]  - Total length of built message.
 *
 * \param  pParamField [OUT]  - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
package enum eQCWWANError PkQmiCbkOmaDmSetEventReport(
    WORD                  *pMlength,
    enum eQmiCbkSetStatus eOmaDmSessionStateEvt,
    BYTE                  *pParamField )
{
    /* There is no mandatory TLV hence map is empty */
    static struct qmTlvBuilderItem map[] =
    {
        { eTLV_OMA_SESSION_STATE_REPORTING_REQ, BuildTlvSessionStateEvent },
        { eTLV_TYPE_INVALID                   , NULL } /* Important. Sentinel.
                                                        * Signifies last item
                                                        * in map.
                                                        */
    };

    enum eQCWWANError eRCode;

    struct QmiCbkOmaDmSetEventReportReq req;
    slmemset( (char *)&req,
              0,
              sizeof (struct QmiCbkOmaDmSetEventReportReq) );

    req.eOmaDmSessionStateEvt   = eOmaDmSessionStateEvt;
    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_OMA_SET_EVENT,
                      pMlength );

    return eRCode;
}

/* Response handlers */

/*
 * This function unpacks the QMI_OMA_SET_EVENT_REPORT response
 * message to a user-provided response structure.
 *
 * \param  pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param  pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return eQCWWAN_ERR_NONE   - Increment successful
 * \return eQCWWAN_ERR_MEMORY - Access beyond allowed size attempted
 *
 */
package enum eQCWWANError UpkQmiCbkOmaDmSetEventReport(
    BYTE                                 *pMdmResp,
    struct QmiCbkOmaDmSetEventReportResp *pApiResp )
{
    enum eQCWWANError eRCode;

    /* The unpack of the Callback contains only result code */
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
                           eQMI_OMA_SET_EVENT );
    return eRCode;
}
