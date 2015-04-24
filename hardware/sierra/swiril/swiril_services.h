/**
 *
 * @ingroup swiril
 *
 * @file 
 * Prototypes for voice RIL QMI Sierra functions
 *
 * @author
 * Copyright: © 2012 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */

#ifndef SWIRIL_SERVICES_H
#define SWIRIL_SERVICES_H 1

void requestQueryClip(void *data, size_t datalen, RIL_Token t);
void requestGetCLIR(void *data, size_t datalen, RIL_Token t);
void requestSetCLIR(void *data, size_t datalen, RIL_Token t);
void requestSetCallForward(void *data, size_t datalen, RIL_Token t);
void requestQueryCallForwardStatus(void *data, size_t datalen, RIL_Token t);
void requestQueryCallWaiting(void *data, size_t datalen, RIL_Token t);
void requestSetCallWaiting(void *data, size_t datalen, RIL_Token t);

#endif
