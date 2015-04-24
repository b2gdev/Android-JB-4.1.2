/**
 *
 * @ingroup swiril
 *
 * @file 
 * Prototypes for SMS related Sierra functions
 *
 * @author
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */

#ifndef RIL_SMS_QMI_H
#define RIL_SMS_QMI_H 1

/* Init function */
void smsServiceInit(void);

/* UMTS functions*/
void requestSendSMSUMTS(void *data, size_t datalen, RIL_Token t);
void requestSMSAcknowledgeUMTS(void *data, size_t datalen, RIL_Token t);
void requestWriteSMSToSimUMTS(void *data, size_t datalen, RIL_Token t);
void requestGetSMSCAddressUMTS(void *data, size_t datalen, RIL_Token t);
void requestSetSMSCAddressUMTS(void *data, size_t datalen, RIL_Token t);
void requestDeleteSmsOnSimUMTS(void *data, size_t datalen, RIL_Token t);

/* CDMA functions*/
void requestSendSMSCDMA(void *data, size_t datalen, RIL_Token t);
void requestSMSAcknowledgeCDMA(void *data, size_t datalen, RIL_Token t);
void requestWriteSMSToRuimCDMA(void *data, size_t datalen, RIL_Token t);
void requestDeleteSmsOnRuimCDMA(void *data, size_t datalen, RIL_Token t);

/* Register functions*/
void registerNewSMSCallbackQMI(void);
void getSMSList(void *);

#endif
