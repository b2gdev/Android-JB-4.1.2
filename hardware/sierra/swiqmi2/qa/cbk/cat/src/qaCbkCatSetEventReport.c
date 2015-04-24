/*
 *
 * \ingroup : cbk
 *
 * \file    : qaCbkCatSetEventReport.c
 *
 * \brief   : Contains Packing and UnPacking routines for the
 *            CAT_SET_EVENT_REPORT message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCbkCatSetEventReport.h"

/* Functions */

/* Request handlers */

/*
 * This function packs the Event Report Request field to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
local enum eQCWWANError BuildTlvEventReportReq(
    BYTE    *pBuf,
    BYTE    *pParam )
{
    struct QmiCbkCatSetEventReportReq *pReq =
        (struct QmiCbkCatSetEventReportReq *)pParam;

    return PutLong ( pBuf, pReq->eventMask );
}


/*
 *
 * This function packs the CAT Set Event Report parameters to the
 * QMI message SDU
 *
 * \param     pParamField [OUT] - Pointer to storage into which the packed
 *                                    data will be placed by this function.
 *
 * \param     pMlength    [OUT] - Total length of built message.
 *
 * \param     eNewMTMessage - enumeration to Set the New MT Message TLV
 *
 * \return    eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
package enum eQCWWANError PkQmiCbkCatSetEventReport(
    WORD  *pMlength,
    ULONG eventMask,
    BYTE  *pParamField )
{
    static struct qmTlvBuilderItem map[] =
    {
         { eTLV_EVENT_REPORT_REQ, BuildTlvEventReportReq },
         { eTLV_TYPE_INVALID,     NULL }  /* Important. Sentinel.
                                         * Signifies last item in map.
                                         */
    };

    struct QmiCbkCatSetEventReportReq req;
    enum eQCWWANError eRCode;

    slmemset( (char *)&req,
              0,
              sizeof (struct QmiCbkCatSetEventReportReq) );
    req.eventMask = eventMask;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_CAT_SET_EVENT,
                      pMlength);
    return eRCode;
}

/* Response handlers */

/*
 * This function unpacks the Report Registration Status from the
 * QMI response message to a user provided response structure
 *
 * \param   pTlvData - Pointer to TLV data from which to unpack.
 *
 * \param   pResp    - Pointer to structure containing storage
 *                     to return data for this TLV.
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 */
enum eQCWWANError qmUnpackTlvReportRegStatus(
    BYTE *pTlvData,
    BYTE *pResp )
{
    struct QmiCbkCatSetEventReportResp *pQmiResp =
                        (struct QmiCbkCatSetEventReportResp *)pResp;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Verify whether the Length of the TLV is according to the Standard */
    eRCode = qmQmiVerifyResponseTLVlength( (struct qmTBuffer *)pTlvData,
                                           eTLV_EVENT_REPORT_REG_STATUS_LENGTH);

    if ( (pQmiResp->pErrorMask) &&
         (eRCode == eQCWWAN_ERR_NONE) )
        eRCode = GetLong(pTlvData, pQmiResp->pErrorMask );

    return eRCode;
}

/*
 * This function unpacks the CAT_SET_EVENT_REPORT response
 * message to a user-provided response structure.
 *
 * \param     pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return    eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
package enum eQCWWANError UpkQmiCbkCatSetEventReport(
    BYTE    *pMdmResp,
    struct  QmiCbkCatSetEventReportResp *pApiResp)
{
    enum eQCWWANError eRCode;

    /* The unpack of the Callback contains only result code */
    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE,             &qmUnpackTlvResultCode },
        { eTLV_EVENT_REPORT_REG_STATUS, &qmUnpackTlvReportRegStatus },
        { eTLV_TYPE_INVALID,            NULL }  /* Important. Sentinel.
                                                 * Signifies last item in map.
                                                 */
    };

    eRCode =  qmunpackresp( pMdmResp,
                           (BYTE*)pApiResp,
                           map,
                           eQMI_CAT_SET_EVENT);
    return eRCode;
}

