/**
 *
 * @ingroup swiril
 *
 * @file 
 * Prototypes for SIM related Sierra functions
 *
 * @author
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */

#ifndef RIL_SIM_QMI_H
#define RIL_SIM_QMI_H 1

#define IMEI_STRING_LENGTH 16       /**< IMEI string length including NULL */
#define ESN_STRING_LENGTH 9         /**< ESN string length including NULL */
#define MEID_STRING_LENGTH 15       /**< MEID string length including NULL */

void requestGetIMSIQMI(void *data, size_t datalen, RIL_Token t);
void requestGetIMEIQMI(void *data, size_t datalen, RIL_Token t);
void requestGetIMEISVQMI(void *data, size_t datalen, RIL_Token t);
void requestGetDeviceIdentityQMI(void *data, size_t datalen, RIL_Token t);
void requestEnterSIMPINQMI(void *data, size_t datalen, RIL_Token t);
void requestEnterSIMPIN2QMI(void *data, size_t datalen, RIL_Token t);
void requestEnterSIMPUKQMI(void *data, size_t datalen, RIL_Token t);
void requestEnterSIMPUK2QMI(void *data, size_t datalen, RIL_Token t);
void requestChangeSIMPINQMI(void *data, size_t datalen, RIL_Token t);
void requestChangeSIMPIN2QMI(void *data, size_t datalen, RIL_Token t);
void pollSIMState(void *param);
void resetpollSIMRetry(void);
void sendPendingSIMREADY(void);
void ResetPendingSIMReady(void);

#endif
