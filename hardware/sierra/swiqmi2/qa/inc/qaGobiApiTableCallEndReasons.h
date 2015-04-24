/**
 * \ingroup wds
 *
 * \file    qaGobiApiTableCallEndReasons.h
 *
 * \brief   Wireless Data Service Call End Reasons
 *
 * \section Table1 Call end reason codes (Code - Reason)
 * \subsection gen General
 * \li 1 - Reason unspecified
 * \li 2 - Client ended the call
 * \li 3 - Device has no service
 * \li 4 - Call has ended abnormally
 * \li 5 - Received release from base station; no reason given
 * \li 6 - Access attempt already in progress
 * \li 7 - Access attempt failure for reason other than the above
 * \li 8 - Call rejected because of redirection or handoff
 * \li 9 - Call failed because close is in progress
 * \li 10 - Authentication failed
 * \li 11 - Call ended because of internal error
 * \subsection _3GPP 3GPP specification-defined call-end reasons (Type=6)
 * \li 8 – OPERATOR_DETERMINED_BARRING
 * \li 25 – LLC_SNDCP_FAILURE
 * \li 26 – INSUFFICIENT_RESOURCES
 * \li 27 – UNKNOWN_APN
 * \li 28 – UNKNOWN_PDP
 * \li 29 – AUTH_FAILED
 * \li 30 – GGSN_REJECT
 * \li 31 – ACTIVATION_REJECT
 * \li 32 – OPTION_NOT_SUPPORTED
 * \li 33 – OPTION_UNSUBSCRIBED
 * \li 34 – OPTION_TEMP_OOO
 * \li 35 – NSAPI_ALREADY_USED
 * \li 36 – REGULAR_DEACTIVATION
 * \li 37 – QOS_NOT_ACCEPTED
 * \li 38 – NETWORK_FAILURE
 * \li 39 – UMTS_REACTIVATION_REQ
 * \li 40 – FEATURE_NOT_SUPPORTED
 * \li 41 – TFT_SEMANTIC_ERROR
 * \li 42 – TFT_SYNTAX_ERROR
 * \li 43 – UNKNOWN_PDP_CONTEXT
 * \li 44 – FILTER_SEMANTIC_ERROR
 * \li 45 – FILTER_SYNTAX_ERROR
 * \li 46 – PDP_WITHOUT_ACTIVE_TFT
 * \li 81 – INVALID_TRANSACTION_ID
 * \li 95 – MESSAGE_INCORRECT_SEMANTIC
 * \li 96 – INVALID_MANDATORY_INFO
 * \li 97 – MESSAGE_TYPE_UNSUPPORTED
 * \li 98 – MSG_TYPE_NONCOMPATIBLE_STATE
 * \li 99 – UNKNOWN_INFO_ELEMENT
 * \li 100 – CONDITIONAL_IE_ERROR
 * \li 101 – MSG_AND_PROTOCOL_STATE_UNCOMPATIBLE
 * \li 111 – PROTOCOL_ERROR
 * \li 112 – APN_TYPE_CONFLICT
 * \subsection MobileIP Mobile IP call-end reasons (Type=1)
 * \li 64  – MIP_FA_ERR_REASON_UNSPECIFIED
 * \li 65  – MIP_FA_ERR_ADMINISTRATIVELY_PROHIBITED
 * \li 66  – MIP_FA_ERR_INSUFFICIENT_RESOURCES
 * \li 67  – MIP_FA_ERR_MOBILE_NODE_AUTHENTICATION_FAILURE
 * \li 68  – MIP_FA_ERR_HA_AUTHENTICATION_FAILURE
 * \li 69  – MIP_FA_ERR_REQUESTED_LIFETIME_TOO_LONG
 * \li 70  – MIP_FA_ERR_MALFORMED_REQUEST
 * \li 71  – MIP_FA_ERR_MALFORMED_REPLY
 * \li 72  – MIP_FA_ERR_ENCAPSULATION_UNAVAILABLE
 * \li 73  – MIP_FA_ERR_VJHC_UNAVAILABLE
 * \li 74  – MIP_FA_ERR_REVERSE_TUNNEL_UNAVAILABLE
 * \li 75  – MIP_FA_ERR_REVERSE_TUNNEL_IS_MANDATORY_AND_T_BIT_NOT_SET
 * \li 79  – MIP_FA_ERR_DELIVERY_STYLE_NOT_SUPPORTED
 * \li 97  – MIP_FA_ERR_MISSING_NAI
 * \li 98  – MIP_FA_ERR_MISSING_HA
 * \li 99  – MIP_FA_ERR_MISSING_HOME_ADDR
 * \li 104 – MIP_FA_ERR_UNKNOWN_CHALLENGE
 * \li 105 – MIP_FA_ERR_MISSING_CHALLENGE
 * \li 106 – MIP_FA_ERR_STALE_CHALLENGE
 * \li 128 – MIP_HA_ERR_REASON_UNSPECIFIED
 * \li 129 – MIP_HA_ERR_ADMINISTRATIVELY_PROHIBITED
 * \li 130 – MIP_HA_ERR_INSUFFICIENT_RESOURCES
 * \li 131 – MIP_HA_ERR_MOBILE_NODE_AUTHENTICATION_FAILURE
 * \li 132 – MIP_HA_ERR_FA_AUTHENTICATION_FAILURE
 * \li 133 – MIP_HA_ERR_REGISTRATION_ID_MISMATCH
 * \li 134 – MIP_HA_ERR_MALFORMED_REQUEST
 * \li 136 – MIP_HA_ERR_UNKNOWN_HA_ADDR
 * \li 137 – MIP_HA_ERR_REVERSE_TUNNEL_UNAVAILABLE
 * \li 138 – MIP_HA_ERR_REVERSE_TUNNEL_IS_MANDATORY_AND_T_BIT_NOT_SET
 * \li 139 – MIP_HA_ERR_ENCAPSULATION_UNAVAILABLE
 * \li 65536 – MIP_ERR_REASON_UNKNOWN
 * \subsection Internal Internal call-end reasons (Type=2)
 * \li 202 – CALL_ENDED
 * \li 201 – INTERNAL_ERROR
 * \li 203 – INTERNAL_UNKNOWN_CAUSE_CODE
 * \li 204 – UNKNOWN_CAUSE_CODE
 * \li 205 – CLOSE_IN_PROGRESS
 * \li 206 – NW_INITIATED_TERMINATION
 * \li 207 – APP_PREEMPTED
 * \subsection CDMA CDMA
 * \li 500 - Device is CDMA locked until power cycle
 * \li 501 - Received intercept from base station; origination only
 * \li 502 - Received reorder from base station; origination only
 * \li 503 - Received release from base station; service option reject
 * \li 504 - Received incoming call from base station
 * \li 505 - Received alert stop from base station; incoming only
 * \li 506 - Received end activation; OTASP call only
 * \li 507 - Max access probes transmitted
 * \li 508 - Concurrent service is not supported by base station
 * \li 509 - No response received from base station
 * \li 510 - Call rejected by the base station;
 * \li 511 - Concurrent services requested were not compatible;
 * \li 512 - Call manager subsystem already in TC
 * \li 513 - Call manager subsystem is ending a GPS call in favor of a user call
 * \li 514 - Call manager subsystem is ending a SMS call in favor of a user call
 * \li 515 - Device has no CDMA service
 * \subsection UMTS UMTS
 * \li 1000 - Call origination request failed
 * \li 1001 - Client rejected the incoming call
 * \li 1002 - Device has no UMTS service
 * \li 1003 - Network ended the call
 * \li 1004 - LLC or SNDCP failure
 * \li 1005 - Insufficient resources
 * \li 1006 - Service option temporarily out of order
 * \li 1007 - NSAPI already used
 * \li 1008 - Regular PDP context deactivation
 * \li 1009 - Network failure
 * \li 1010 - Reactivation requested
 * \li 1011 - Protocol error, unspecified
 * \li 1012 - Operator-determined barring (exclusion)
 * \li 1013 - Unknown or missing Access Point Name (APN)
 * \li 1014 - Unknown PDP address or PDP type
 * \li 1015 - Activation rejected by GGSN
 * \li 1016 - Activation rejected, unspecified
 * \li 1017 - Service option not supported
 * \li 1018 - Requested service option not subscribed
 * \li 1019 - Quality of Service (QoS) not accepted
 * \li 1020 - Semantic error in the TFT operation
 * \li 1021 - Syntactical error in the TFT operation
 * \li 1022 - Unknown PDP context
 * \li 1023 - Semantic errors in packet filter(s)
 * \li 1024 - Syntactical error in packet filter(s)
 * \li 1025 - PDP context without TFT already activated
 * \li 1026 - Invalid transaction identifier value
 * \li 1027 - Semantically incorrect message
 * \li 1028 - Invalid mandatory information
 * \li 1029 - Message type non-existent or not implemented
 * \li 1030 - Message not compatible with state
 * \li 1031 - Information element nonexistent or not implemented
 * \li 1032 - Conditional information element error
 * \li 1033 - Message not compatible with protocol state
 * \li 1034 - APN restriction value incompatible with active PDP context
 * \li 1035 - No GPRS context present
 * \li 1036 - Requested feature not supported
 * \subsection CDMA_EVDO CDMA 1xEV-DO
 * \li 1500 - Abort connection setup due to the reception of a ConnectionDeny message with deny code set
 *            to either general or network busy.
 * \li 1501 - Abort connection setup due to the reception of a ConnectionDeny message with deny code set
 *            to either billing or authentication failure.
 * \li 1502 - Change EV-DO system due to redirection or PRL not preferred
 * \li 1503 - Exit EV-DO due to redirection or PRL not preferred
 * \li 1504 - No EV-DO session
 * \li 1505 - Call manager is ending a EV-DO call origination in favor of a GPS call
 * \li 1506 - Connection setup timeout
 * \li 1507 - Call manager released EV-DO call
 * \li 2000 – CLIENT_END
 * \li 2001 – NO_SRV
 * \li 2002 – FADE
 * \li 2003 – REL_NORMAL
 * \li 2004 – ACC_IN_PROG
 * \li 2005 – ACC_FAIL
 * \li 2006 – REDIR_OR_HANDOFF
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __GOBI_API_CALL_END_REASONS_H__
#define __GOBI_API_CALL_END_REASONS_H__


#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __GOBI_API_CALL_END_REASONS_H__ */

