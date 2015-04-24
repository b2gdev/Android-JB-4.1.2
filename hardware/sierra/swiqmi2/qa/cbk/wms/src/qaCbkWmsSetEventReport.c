/*
 *
 * \ingroup : cbk
 *
 * \file    : qaCbkWmsSetEventReport.c
 *
 * \brief   : Contains Packing and UnPacking routines for the
 *            WMS_SET_EVENT_REPORT message.
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

/* include files */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "qaCbkWmsSetEventReport.h"

/* Functions */

/* Request handlers */

/*
 * Packs the New MT Message TLV
 *
 * \param  pBuf   [OUT]  - Pointer to buffer into which the data will be packed
 *
 * \param  pParam [OUT]  - Pointer to structure containing data for this TLV.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
local enum eQCWWANError BuildTlvNewMTMessage(
    BYTE    *pBuf,
    BYTE    *pParam )
{
    struct QmiCbkWmsSetEventReportReq *pReq =
        (struct QmiCbkWmsSetEventReportReq *)pParam;

    /* If the parameter has not been set do not fill the TLV */
    if (QMI_CBK_PARAM_NOCHANGE == pReq->eNewMTMessage)
        return eQCWWAN_ERR_NONE;

    return PutByte ( pBuf, pReq->eNewMTMessage );
}

/*
 *
 * Packs the WMS Set Event Report TLVs
 *
 * \param  pMlength     [OUT] - Total length of the packed TLVs
 *
 * \param  eNewMTMessage - enumeration to Set the New MT Message TLV
 *
 * \param  pParamField  [OUT] - Pointer to storage into which the packed
 *                              data will be placed by this function.
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
package enum eQCWWANError PkQmiCbkWmsSetEventReport(
    WORD                  *pMlength,
    enum eQmiCbkSetStatus eNewMTMessage,
    BYTE                  *pParamField )
{
    static struct qmTlvBuilderItem map[] =
    {
         { eTLV_NEW_MT_MESSAGE, BuildTlvNewMTMessage },
         { eTLV_TYPE_INVALID,   NULL }  /* Important. Sentinel.
                                         * Signifies last item in map.
                                         */
    };

    struct QmiCbkWmsSetEventReportReq req;
    slmemset( (char *)&req,
              0,
              sizeof (struct QmiCbkWmsSetEventReportReq) );

    req.eNewMTMessage   = eNewMTMessage;
    enum eQCWWANError eRCode;

    eRCode = qmbuild( pParamField,
                      (BYTE *)&req,
                      map,
                      eQMI_WMS_SET_EVENT,
                      pMlength);
    return eRCode;
}

/* Response handlers */

/*
 * Unpacks the WMS_SET_EVENT_REPORT response
 *
 * \param   pMdmResp   [IN]  - Pointer to source (response) buffer
 *
 * \param   pApiResp   [OUT] - Pointer to storage containing destination buffer
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_ERR_XXX otherwise
 *
 */
package enum eQCWWANError UpkQmiCbkWmsSetEventReport(
    BYTE    *pMdmResp,
    struct  QmiCbkWmsSetEventReportResp *pApiResp)
{
    enum eQCWWANError eRCode;

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
                           eQMI_WMS_SET_EVENT);
    return eRCode;
}

