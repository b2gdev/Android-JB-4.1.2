/*************
 *
 * Filename:    qmqmisvc.h
 *
 * Purpose:     Local (SLQS) QMI service support
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/

#ifndef QMQMISVC_H
#define QMQMISVC_H

/*---------------
  include files
 ---------------*/

#include "qmdcs.h"
#include "qmfms.h"

enum qmi_response_result_code{
    QMI_RC_SUCCESS,
    QMI_RC_FAILURE
};

/*---------------
  User Types
 ---------------*/

/*************
 *
 * Name:    qm_qmi_result_tlv_values
 *
 * Purpose: QMI result TLV Values
 *
 * Members: (IN) pdevnode    - qcqmi file system device node
 *          (IN) pdevkey     - modem device ID
 *
 * Notes:
 *
 **************/
struct qm_qmi_result_tlv_values
{
    swi_uint32 validresponse;
    swi_uint16 result;
    swi_uint16 error;
};

union qm_qmi_tlv_values{
    union qm_qmi_dcs_tlv_values qmdcstlvs;
    union qm_qmi_fms_tlv_values qmfmstlvs;
};

/*************
 *
 * Name:    qm_qmi_response_tlvs_values
 *
 * Purpose: QMI Response Structure used for building QMI response returned to
 *          the user application making the request.
 *
 * Members: qmiresult   - qmi result tlv
 *          tlvvalues   - qmi response parameter tlvs i.e. tlvs after response tlv
 *
 * Notes:
 *
 **************/
struct qm_qmi_response_tlvs_values{
    struct qm_qmi_result_tlv_values qmiresult;
    union  qm_qmi_tlv_values tlvvalues;
};

/*************
 *
 * Name:    qmqmisvcresponse
 *
 * Purpose: QMI Service Response structure.
 *
 * Members: svc         - QMI service type
 *          ctlflgs     - Request = 0, Response = 1, Notification = 2
 *          xactionid   - Transaction ID of the last issued QMI service request
 *          msgid       - QMI Message ID of the last issued QMI service request
 *          amqmimsg    - Storage location for AM response sent from SDK to
 *                        application.
 *          qmimsglen   - QMI response message length
 *
 * Notes:
 *
 **************/
struct qmqmisvcresponse
{
    enum eQMIService    svc;
    swi_uint8           ctlflgs;
    swi_uint16          xactionid;
    swi_int8            msgid;
    swi_uint8           amqmimsg[1024]; /* should equal AMMAXRESPBKLEN */
    swi_uint16          qmimsglen;
};

/*---------------
  Prototypes
 ---------------*/
enum eQCWWANError qm_result_code_tlv_pack(
    swi_uint8 *pdest,
    swi_uint8 *psrc );

void qm_qmisvc_send_response(
    struct qmqmisvcresponse             *pqmirsp,
    struct qm_qmi_response_tlvs_values  *prsptlvs,
    struct qmTlvBuilderItem             *pmap,
    enum eQMIService                    svc,
    swi_int8                            msgid,
    swi_uint8                           msgtype );

void qmqmisvcinit();

#endif /* QMQMISVC_H */
