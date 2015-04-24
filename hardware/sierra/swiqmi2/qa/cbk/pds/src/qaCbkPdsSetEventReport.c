/*
 *
 * \ingroup : cbk
 *
 * \file    : qaPdsSetEventReport.c
 *
 * \brief   : Contains Packing and UnPacking routines for the
 *            PDS_SET_EVENT_REPORT message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCbkPdsSetEventReport.h"

/* Functions */

/* Request handlers */

/*
 * This function packs the Position Data NMEA field to the QMI message SDU
 *
 * \param  pBuf   [OUT]  - Pointer to storage into which the packed
 *                         data will be placed by this function.
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
local enum eQCWWANError BuildTlvPositionDataNMEA(
    BYTE    *pBuf,
    BYTE    *pParam )
{
    struct QmiCbkPdsSetEventReportReq *pReq =
        (struct QmiCbkPdsSetEventReportReq *)pParam;

    /* If the parameter has not been set do not fill the TLV */
    if (QMI_CBK_PARAM_NOCHANGE == pReq->ePositionDataNMEA)
        return eQCWWAN_ERR_NONE;

    return PutByte ( pBuf, pReq->ePositionDataNMEA );
}


/*
 *
 * This function packs the PDS Set Event Report parameters to the
 * QMI message SDU
 *
 * \param     pParamField [OUT] - Pointer to storage into which the packed
 *                                    data will be placed by this function.
 *
 * \param     pMlength    [OUT] - Total length of built message.
 *
 * \param     ePositionDataNMEA - enumeration to Set the Position Data NMEA TLV
 *
 * \return    eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
package enum eQCWWANError PkQmiCbkPdsSetEventReport(
    WORD                  *pMlength,
    enum eQmiCbkSetStatus ePositionDataNMEA,
    enum eQmiCbkSetStatus eParsedPositionData,
    BYTE                  *pParamField )
{
    static struct qmTlvBuilderItem map[] =
    {
         { eTLV_POSITION_DATA_NMEA_REQ, BuildTlvPositionDataNMEA },
         { eTLV_TYPE_INVALID,           NULL }  /* Important. Sentinel.
                                                 * Signifies last item in map.
                                                 */
    };

    struct QmiCbkPdsSetEventReportReq req;
    slmemset( (char *)&req,
              0,
              sizeof (struct QmiCbkPdsSetEventReportReq) );

    req.ePositionDataNMEA   = ePositionDataNMEA;
    req.eParsedPositionData = eParsedPositionData;

    enum eQCWWANError eRCode;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_PDS_SET_EVENT,
                      pMlength);
    return eRCode;
}

/* Response handlers */

/*
 * This function unpacks the PDS_SET_EVENT_REPORT response
 * message to a user-provided response structure.
 *
 * \param     pMdmResp   [IN]  - Pointer to packed response from the modem.
 *
 * \param     pApiResp   [OUT] - Pointer to storage to unpack into.
 *
 * \return    eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
package enum eQCWWANError UpkQmiCbkPdsSetEventReport(
    BYTE    *pMdmResp,
    struct  QmiCbkPdsSetEventReportResp *pApiResp)
{
    enum eQCWWANError eRCode;

    /* The unpack of the Callback contains only result code */
    static struct qmTlvUnpackerItem map[] =
    {
        { eTLV_RESULT_CODE       ,&qmUnpackTlvResultCode },
        { eTLV_TYPE_INVALID      ,NULL }  /* Important. Sentinel.
                                           * Signifies last item in map.
                                           */
    };
    eRCode =  qmunpackresp( pMdmResp,
                           (BYTE*)pApiResp,
                           map,
                           eQMI_PDS_SET_EVENT);
    return eRCode;
}

