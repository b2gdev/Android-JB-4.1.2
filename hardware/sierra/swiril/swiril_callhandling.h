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

#ifndef SWIRIL_CALLHANDLING_H
#define SWIRIL_CALLHANDLING_H 1

void registerVoiceCallbackQMI(void);
void requestLastCallFailCause(void *data, size_t datalen, RIL_Token t);
void requestGetCurrentCalls(void *data, size_t datalen, RIL_Token t);
void requestDial(void *data, size_t datalen, RIL_Token t);
void requestAnswer(void *data, size_t datalen, RIL_Token t);
void requestHangup(void *data, size_t datalen, RIL_Token t);
void requestHangupWaitingOrBackground(void *data, size_t datalen,
                                      RIL_Token t);
void requestHangupForegroundResumeBackground(void *data, size_t datalen,
                                             RIL_Token t);
void requestSwitchWaitingOrHoldingAndActive(void *data, size_t datalen,
                                            RIL_Token t);
void requestConference(void *data, size_t datalen, RIL_Token t);
void requestSeparateConnection(void *data, size_t datalen, RIL_Token t);
void requestExplicitCallTransfer(void *data, size_t datalen, RIL_Token t);
void requestUDUB(void *data, size_t datalen, RIL_Token t);

#endif
