/*************
 *
 * Filename:  qmiproto.h
 *
 * Purpose:   This file contains internal prototypes for the qm package
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/

/* include files */
#include "qm/qmdcs.h"
#include "qm/qmfms.h"

/* qmtask_sdk.c */
struct qmtcb *qmgetcbp(void);

void qmclearrespinfo(void);

void qmshortresp(
    enum eQCWWANError resultcode,
    swi_uint32 ipcchannel );

const struct qmicontrolpoint *qmiclientinfoget(
    enum qmisupportedclients client );

void qmdisconnect(void);

void qmqmirequestcancel(void);

enum qmisupportedclients qmisvc2qmiclnt_map(
    enum eQMIService svc );

void qm_ds_dev_notification_cb(
    enum ds_sio_event event );

void qmwdsclientset(
    enum qmisupportedclients client );

enum qmisupportedclients qmwdsclientget();
/* RILSTART */
swi_int8 qmGetFwDownloadStatus( void );

void qmSetFwDownloadStatus( swi_int32 fwdldstatus );
/* RILSTOP */

/* qmcommon.c */
enum eQCWWANError qmunpack(
    swi_uint8 *bufp,
    swi_uint8 *resp,
    struct qmTlvUnpackerItem *pItem,
    const swi_uint16 messageId );

/* qmdcscommon.c */
enum eQCWWANError
qm_dcs_enumerate_devices_response_unpack(
    swi_uint8 *preq,
    struct qm_qmi_response_tlvs_values *preqtlvs );

/* qmdcs.c */
void qm_dcs_handler(
    swi_uint8 *preq );

void qm_dcs_dev_state_change_notify(
     swi_uint8 devstate );

void qmdcsinit();

/* qmfms.c */
void qm_fms_handler(
    swi_uint8 *preq );

/* RILSTART */
void qm_fms_fw_dwld_complete_notify(
    swi_int32 fwdldstatus );
/* RILSTOP */

void qmfmsinit();

/* qmparser.c */
void qmparser(  enum eQMIService svc,
                swi_uint8* pmsg );
