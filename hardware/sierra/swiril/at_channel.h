/*
** This source code is "Not a Contribution" under Apache license
**
** Sierra Wireless RIL
**
** Based on reference-ril by The Android Open Source Project
** and U300 RIL by ST-Ericsson.
** Modified by Sierra Wireless, Inc.
**
** Copyright (C) 2010 Sierra Wireless, Inc.
** Copyright (C) ST-Ericsson AB 2008-2009
** Copyright 2006, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
**
** Based on reference-ril by The Android Open Source Project.
**
** Modified for ST-Ericsson U300 modems.
** Author: Christian Bejram <christian.bejram@stericsson.com>
*/

#ifndef ATCHANNEL_H
#define ATCHANNEL_H 1

#ifdef __cplusplus
extern "C" {
#endif

/* Define AT_DEBUG to send AT traffic to "/tmp/radio-at.log" */
#define AT_DEBUG  0

#if AT_DEBUG
extern void  AT_DUMP(const char* prefix, const char*  buff, int  len);
#else
#define  AT_DUMP(prefix,buff,len)  do{}while(0)
#endif

#define AT_ERROR_GENERIC -1
#define AT_ERROR_COMMAND_PENDING -2
#define AT_ERROR_CHANNEL_CLOSED -3
#define AT_ERROR_TIMEOUT -4
#define AT_ERROR_INVALID_THREAD -5    /* AT commands may not be issued from
                                         reader thread (or unsolicited response
                                         callback */
#define AT_ERROR_INVALID_RESPONSE -6  /* Eg an at_send_command_singleline that
                                         did not get back an intermediate
                                         response */


typedef enum {
    NO_RESULT,      /* No intermediate response expected. */
    NUMERIC,        /* A single intermediate response starting with a 0-9. */
    SINGLELINE,     /* A single intermediate response starting with a prefix. */
    MULTILINE,      /* Multiple line intermediate response
                       starting with a prefix. */

    /* Multiple value intermediate response. Only the first line starts with a
       prefix, but all the remaining intermediate lines are part of the command
       output.
     */
    MULTIVALUE
    
} ATCommandType;

/** A singly-linked list of intermediate responses. */
typedef struct ATLine  {
    struct ATLine *p_next;
    char *line;
} ATLine;

/** Free this with at_response_free(). */
typedef struct {
    int success;              /* True if final response indicates
                                 success (eg "OK"). */
    char *finalResponse;      /* Eg OK, ERROR */
    ATLine  *p_intermediates; /* Any intermediate responses. */
} ATResponse;

/**
 * A user-provided unsolicited response handler function.
 * This will be called from the reader thread, so do not block.
 * "s" is the line, and "sms_pdu" is either NULL or the PDU response
 * for multi-line TS 27.005 SMS PDU responses (eg +CMT:).
 */
typedef void (*ATUnsolHandler)(const char *s, const char *sms_pdu);

int at_open(char *ttynamep, ATUnsolHandler h, int atopendelay, int closetime);
void at_close();

/*
 * Set default timeout for at commands. Let it be reasonable high
 * since some commands take their time. Default is 10 minutes.
 */
void at_set_timeout_msec(int timeout);

/* 
 * This callback is invoked on the command thread.
 * You should reset or handshake here to avoid getting out of sync.
 */
void at_set_on_timeout(void (*onTimeout)(void));

/*
 * This callback is invoked on the reader thread (like ATUnsolHandler), when the
 * input stream closes before you call at_close (not when you call at_close()).
 * You should still call at_close(). It may also be invoked immediately from the
 * current thread if the read channel is already closed.
 */
void at_set_on_reader_closed(void (*onClose)(void));

void at_send_escape(void);

int at_send_command_singleline (const char *command,
                                const char *responsePrefix,
                                 ATResponse **pp_outResponse);

int at_send_command_singleline_with_timeout (const char *command,
                                             const char *responsePrefix,
                                             ATResponse **pp_outResponse,
                                             int timeoutMsec);

int at_send_command_numeric (const char *command,
                                 ATResponse **pp_outResponse);

int at_send_command_multiline (const char *command,
                                const char *responsePrefix,
                                 ATResponse **pp_outResponse);

int at_send_command_multivalue (const char *command,
                                const char *responsePrefix,
                                ATResponse **pp_outResponse);

int at_handshake();

int at_send_command (const char *command, ATResponse **pp_outResponse);

int at_send_command_sms (const char *command, const char *pdu,
                            const char *responsePrefix,
                            ATResponse **pp_outResponse);

void at_response_free(ATResponse *p_response);

void at_make_default_channel(void);

typedef enum {
    CME_ERROR_NON_CME = -1,
    CME_SUCCESS = 0,
    CME_SIM_NOT_INSERTED = 10,
    CME_SIM_PIN_REQUIRED = 11,
    CME_SIM_FAILURE = 13,
    CME_INCORRECT_PASSWORD = 16,
    CME_NO_NETWORK_SERVICE = 30
} AT_CME_Error;

typedef enum {
    CMS_ERROR_NON_CMS           = -1,
    CMS_SUCCESS                 = 0,
    CMS_FC_NETWORK_PROBLEM_END  = 31, 
    CMS_RI_PROBLEM_BEGIN        = 64,
    CMS_RI_PROBLEM_END          = 95,
    CMS_NO_NETWORK_SERVICE      = 331,
    CMS_NETWORK_TIMEOUT      = 332,
} AT_CMS_Error;

AT_CME_Error at_get_cme_error(const ATResponse *p_response);

#ifdef __cplusplus
}
#endif

#endif
