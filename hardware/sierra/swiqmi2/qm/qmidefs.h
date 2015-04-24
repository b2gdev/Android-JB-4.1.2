/*************
 *
 * Filename: qmidefs.h
 *
 * Purpose:  QM package private constants and structures
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/

#ifndef QMIDEFS_H
#define QMIDEFS_H

/* Includes */
#include "aa/aaglobal.h"
#include "qm/qmerrno.h"
#include "am/amudefs.h"
#include "dl/dludefs.h"
#include "mm/mmudefs.h"
#include "ic/icudefs.h"
#include "os/swi_ossdk.h"
#include "qm/qmudefs.h"
#include "ds/dsudefs.h"

/* Transmit Buffer Pool Definitions */
#define QMTXPOOLSZ      5
#define QMTXBUFSIZE     QMUXSDU_HDR_SZ + QMPARAMMAX

/* Transmit Small Buffer Pool Definitions */
#define QMTXSMALLPLSZ   20
#define QMTXSMLBUFSIZE  QMUXSDU_HDR_SZ + 256

/* Timeout guard interval. This value is the basic timeout applied by the API
 * side whenever it calls icapi_rcv(). It is added to the application's
 * specified timeout on API calls, while the caller's timeout is used directly
 * by the SDK when it is awaiting a response from the modem for a QMI response.
 * This ensure that the modem timeout will always happen first. If the
 * icapi_rcv() times out, then this is some kind of system failure like the SDK
 * process has died.
 */
#define QMTIMEGUARD         1000

/* Timeout value to use in QM package when calling icsmt_rcv() */
#define QMNOTIMEOUT         0

/* QMI Request/Response task Event IDs */
#define QMRESPONSE          1    /* Event ID for QMI Response packet */

/* Request/Response (RR) Task */
#define QMMRESPTIMEOUT    5000 /* Modem response timeout in ms */
#define QMEVTPOOLSZ       30   /* # of buffers in event pool */
#define QMMSGDEPTH        (QMEVTPOOLSZ + 1) /* QM RR Task msg queue depth */

/* Logging masks */
#define QMLOG_CLASSA        0x0001
#define QMLOG_CLASSB        0x0002
#define QMLOG_CLASSC        0x0004

/*************
 *
 * Name:    qmeventtypes
 *
 * Purpose: Enumerated list of event block types. These are the types
 *          that get written into the QMI Event Block whenever an
 *          inter-task message is to be sent to the QM Task.
 *
 * Members: QM_QMIREQ_EVT   - QMI service request event
 *          QM_QMIRSP_EVT   - QMI service response event
 *          QM_TMRTICK_EVT  - Timer Tick event
 *          QM_APPDEV_EVT
 * Notes:   None
 *
 **************/
enum qmeventtype {
    QM_MIN_EVT,
    QM_QMIREQ_EVT = QM_MIN_EVT,
    QM_QMIRSP_EVT,
    QM_TMRTICK_EVT,
    QM_DS_DEVICE_EVT,
};

/*************
 *
 * Name:    qmevtblock
 *
 * Purpose: Structure used to pass information into the QMI
 *          Request/Response task.
 *
 * Members: qmeventtype     - Indicates the remaining content of this block
 *          qmevtdatap      - Pointer to a buffer containing data to be handled
 *                            by the QM RR Task.
 *          qmevtmemfreep   - Pointer to the address to use when releasing the
 *                            memory containing the data. NULL if nothing needs
 *                            to be released.
 *          qmevtipcchan    - IPC channel pertaining to a QMI request event
 *
 * Notes:   None
 *
 **************/
struct qmevtblock
{
    enum qmeventtype qmeventtype;
    swi_uint8        *qmevtdatap;
    swi_uint8        *qmevtmemfreep;
    swi_uint8        qmevtipcchan;
    enum qmisupportedclients client;
};

/*************
 *
 * Name:    qmwdata
 *
 * Purpose: Structure to hold key information about the outstanding packet
 *          being sent to the modem. When the response is received it must
 *          correlate directly with this information.
 *
 * Members: qmsbusy         - Whether the QM RR task is already waiting
 *                            for a QMI response from the modem.
 *          qmtimeoutflag   - Indicates whether the thread timed out
 *                            waiting for a response from the modem.
 *          qmoperation     - The value we expect to see in the operation
 *                            type field of the response packet.
 *          qmipcchannel    - The IPC channel that the QMI request came through
 *
 * Notes:
 *
 **************/
struct qmwdata
{
    swi_bool   qmbusy;
    swi_bool   qmtimeoutflag;
    swi_uint8  qmoperation;
    swi_uint8  qmipcchannel;
};

/*************
 *
 * Name:    qm_fms_info
 *
 * Purpose: Information required by the Image Management task for performing
 *          firmware upgrades.
 *
 * Members: path    - fully qualified path to directory containing candidate
 *                    firmware image(s).
 *
 * Notes:   none
 *
 **************/
struct qm_fms_info{
/* RILSTART */
    swi_char  path[QM_FMS_IMAGE_PATH_MAX_SZ];
    swi_bool  fw_download_requested;
    swi_bool  fw_download_started;
    swi_int32 fw_download_status;
/* RILSTOP */
};

/*************
 *
 * Name:    qmtcb
 *
 * Purpose: QM RR Task control block structure
 *
 * Members: qmdlcb      -   DL package control block for logging
 *          qmicmsgque      IC SMT Message queue
 *          qmevtpool   -   MM pool structure. Contains buffers used
 *                          to store information about QMI Reponses.
 *                          Groups several arguments that can be referenced
 *                          by a single handle
 *          qmwdata     -   Structure to hold the information about
 *                          the current transaction awaiting a response
 *                          from the modem
 *          qmictthndl  -   IC package timer handle used by this task to
 *                          use the periodic timer tick facility of the IC
 *                          package.
 *          qmmdmwaittmr -  Downcounting timer used to detect timeouts when
 *                          waiting for modem responses. Tick interval is
 *                          given by:
 *                                  <timeout(ms)>/ICTICKINTERVAL
 *          qmtxpool        - qm task request large buffer pool
 *          qmtxsmlpool     - qm task request small buffer pool
 *          qmdsshell       - qm task ds shell
 *          qmideviceready  - qcqmi device ready flag
 *          qmappregistered - user application registered with SDK flag
 *          fmsdata         - firmware download metadata
 *
 * Notes:   none
 *
 **************/
struct qmtcb
{
    struct dlusercb     qmdlcb;
    struct icsmtcb      qmicmsgque;
    struct mmpool       qmevtpool;
    struct qmwdata      qmwdata;
    struct icttregqblk  qmictthndl;
    swi_uint32          qmmdmwaittmr;

    struct mmpool       qmtxpool;
    struct mmpool       qmtxsmlpool;

    struct dsipcshell   qmdsshell;
    swi_uint8           qmidevicestate;
    swi_bool            qmappregistered;
    struct qm_fms_info  fms_data;
};

/*************
 *
 * Name:    qmgcb
 *
 * Purpose: QM RR Globl Control Block
 *
 * Members:
 *
 * Notes:   none
 *
 **************/
struct qmgcb
{
    struct qmtcb *qmtcbp;
    struct dlusercb qmdlcb;
    struct qmiclientmanager *pclntmgr;
};

/*************
 *
 * Name:    amqmimsgtype
 *
 * Purpose: QMI message type
 *
 * Members: eQMIREQ - QMI request
 *          eQMIRES - QMI response
 *          eQMINOT - QMI notification
 *          eQMIINVALID - invalid QMI message type
 * Notes:
 *
 **************/
enum amqmimsgtype{
    eQMIREQ,
    /* NB: QMI documentation and Firmware do not agree. The doc states that
     * Response = 1 and Notification = 2, but they are coded with values of
     * 2 and 4, respectively.
     */
    eQMIRES = 2,
    eQMINOT = 4,
    eQMIINVALID
};

/*************
 *
 * Name:    qmicontrolpoint
 *
 * Purpose: QMI Service Client record. Contains the necessary fields for matching
 *          up a QMI client request with a QMI service response
 *
 * Members: qmisvcname  - QMI service name
 *          svctype     - QMI Service Type for which the client has issued
 *                        a request.
 *          registered  - QMI client registered with service?
 *          notifchan   - Incoming Notification IPC channel
 *                        (SDK to APP notifications).
 *          xactionid   - Transaction ID of the last issued QMI service request.
 *          clientbusy  - awaiting a QMI response from the device
 *          msgid       - QMI Message ID of the last issued QMI service request
 *          unotifications_supported - Specifies if unsolicited notifications
 *                                     are supported by the service
 *
 * Notes:
 *
 **************/
struct qmicontrolpoint{
    char        qmisvcname[8];
    swi_int16   svctype;
    swi_bool    registered;
    swi_int32   notifchan;
    swi_uint16  xactionid;
    swi_bool    clientbusy;
    swi_int16   msgid;
    swi_bool    unotifications_supported;
};

#include "qm/qmiproto.h"

#endif /* QMIDEFS_H */
