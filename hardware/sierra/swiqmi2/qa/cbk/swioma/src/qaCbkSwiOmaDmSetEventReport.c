/*
 * \ingroup cbk
 *
 * \file qaSwiOmsDmSetEventReport.c
 *
 * \brief Contains Packing and UnPacking routines for the
 *        QMI_SWIOMA_DM_SET_EVENT_REPORT message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCbkSwiOmaDmSetEventReport.h"

/* Functions */

/* Request handlers */

/*
 * This function packs the QMI_SWIOMA_DM_SET_EVENT_REPORT request parameters
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
package enum eQCWWANError PkQmiCbkSwiOmaDmSetEventReport(
    WORD *pMlength,
    BYTE *pParamField )
{
    /* There is no mandatory TLV hence map is empty */
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
                      eQMI_SWIOMA_SET_EVENT,
                      pMlength );

    return eRCode;
}

/* Response handlers */

/*
 * This function unpacks the QMI_SWIOMA_DM_SET_EVENT_REPORT response
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
package enum eQCWWANError UpkQmiCbkSwiOmaDmSetEventReport(
    BYTE                                    *pMdmResp,
    struct QmiCbkSwiOmaDmSetEventReportResp *pApiResp )
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
                           eQMI_SWIOMA_SET_EVENT );
    return eRCode;
}
