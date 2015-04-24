/**
 * \ingroup voice
 *
 * \file    qaGobiApiTableVoiceCallEndReasons.h
 *
 * \brief   Voice Service Call and supplementary services end reasons
 *
 * \section Table9 Voice Call and supplementary services end reason codes (Code - Reason)
 * \subsection General
 * \li 0   - Phone is offline
 * \li 20  - Phone is CDMA locked until a power cycle; CDMA only
 * \li 21  - Phone has no service
 * \li 22  - Call has ended abnormally; CDMA only
 * \li 23  - Received intercept from the base station; originating only; CDMA only
 * \li 24  - Received reorder from the base station; originating only; CDMA only
 * \li 25  - Received release from the base station; no reason was given
 * \li 26  - Received release from the base station; SO reject; CDMA only
 * \li 27  - Received incoming call from the base station
 * \li 28  - Received alert stop from the base station; incoming only; CDMA only
 * \li 29  - Client ended the call
 * \li 30  - Received end activation; OTASP call only; CDMA only
 * \li 31  - MC aborted the origination/conversation; CDMA only
 * \li 32  - Maximum access probes were transmitted; CDMA only
 * \li 33  - Persistence test failure; FEATURE_JCDMA only; CDMA only
 * \li 34  - R-UIM is not present
 * \li 35  - Access attempt is already in progress
 * \li 36  - Access failure for a reason other than the above
 * \li 37  - Received retry order; originating only; IS 2000; CDMA only
 * \li 38  - BYBS Concurrent service is not supported by the base station
 * \li 39  - No response was received from the base station
 * \li 40  - Call was rejected by the base station; CDMA only
 * \li 41  - Concurrent services requested were not compatible; CDMA only
 * \li 42  - Access is blocked by the base station; CDMA only
 * \li 43  - Corresponds to CM_CALL_ORIG_ ERR_ALREADY_IN_TC
 * \li 44  - Call is ended because an emergency call is flashed over this call; CDMA only
 * \li 45  - Used if CM is ending a GPS call in preference of a user call
 * \li 46  - Used if CM is ending an SMS call in preference of a user call
 * \li 47  - Used if CM is ending a data call in preference of an emergency call
 * \li 48  - Call was rejected because of a redirection or handoff
 * \li 49  - Access is blocked by the base station for all mobiles; KDDI-specific; CDMA only
 * \li 50  - To support OTASP SPC Error indication
 * \li 51  - Maximum access probes for an IS-707B call; CDMA only
 * \li 102 - WCDMA/GSM only; client rejected an incoming call
 * \li 103 - WCDMA/GSM only; client rejected a setup indication
 * \li 104 - WCDMA/GSM only; network ended the call
 * \li 105 - WCDMA/GSM only
 * \li 106 - GWM/WCDMA only; phone has no service
 * \li 107 - 1X only; phone has no service
 * \li 108 - Full service is unavailable
 * \li 109 - Indicates resources are not available to handle a new MO/MT PS call
 * \subsection Supplementary service Errors
 * \li 110 - Unknown subscriber
 * \li 111 - Illegal subscriber
 * \li 112 - Bearer service not provisioned
 * \li 113 - Tele service not provisioned
 * \li 114 - Illegal equipment
 * \li 115 - Call barred
 * \li 116 - Illegal ss operation
 * \li 117 - Ss error status
 * \li 118 - Ss not available
 * \li 119 - Ss subscriptiON VIOLATION
 * \li 120 - Ss incompatibility
 * \li 121 - Facility not supported
 * \li 122 - Absent subscriber
 * \li 123 - Short term denial
 * \li 124 - Long term denial
 * \li 125 - System failure
 * \li 126 - Data missing
 * \li 127 - Unexpected data value
 * \li 128 - Pwd registration failure
 * \li 129 - Negative pwd check
 * \li 130 - Num of pwd attempts violation
 * \li 131 - Position method failure
 * \li 132 - Unknown alphabet
 * \li 133 - Ussd busy
 * \li 134 - Rejected by user
 * \li 135 - Rejected by network
 * \li 136 - Deflection to served subscriber
 * \li 137 - Special service code
 * \li 138 - Invalid deflected to number
 * \li 139 - Mpty participants exceeded
 * \li 140 - Resources not available
 * \subsection Call control cause values
 * \li 141 - Unassigned number
 * \li 142 - No route to destination
 * \li 143 - Channel unacceptable
 * \li 144 - Operator determined barring
 * \li 145 - Normal call clearing
 * \li 146 - User busy sEE [s3, aNNEX h]
 * \li 147 - No user responding sEE [s3, aNNEX h]
 * \li 148 - User alerting no  answer
 * \li 149 - Call rejected sEE [s3, aNNEX h]
 * \li 150 - Number changed sEE [s3, aNNEX h]
 * \li 151 - Preemption sEE [s3, aNNEX h]
 * \li 152 - Destination out of order
 * \li 153 - Invalid number format
 * \li 154 - Facility rejected
 * \li 155 - Resp to status enquiry
 * \li 156 - Normal unspecified
 * \li 157 - No circuit or channel available
 * \li 158 - Network out of order
 * \li 159 - Temporary failure
 * \li 160 - Switching equipment congestion
 * \li 161 - Access information discarded
 * \li 162 - Requested circuit  or channel not available
 * \li 163 - Resources unavailable or unspecified
 * \li 164 - Qos unavailable
 * \li 165 - Requested facility not subscribed
 * \li 166 - Incoming calls barred within cug
 * \li 167 - Bearer capability not auth
 * \li 168 - Bearer capability unavailable
 * \li 169 - Service option not available
 * \li 170 - Acm limit exceeded
 * \li 171 - Bearer service not implemented
 * \li 172 - Requested facility not implemented
 * \li 173 - Only digital information bearer available
 * \li 174 - Service or option not implemented
 * \li 175 - Invalid transaction identifier
 * \li 176 - USER NOT MEMBER OF CUG
 * \li 177 - Incompatible destination
 * \li 178 - Invalid transit nw selection
 * \li 179 - Semantically  incorrect message
 * \li 180 - Invalid mandatory information
 * \li 181 - Message type non implemented
 * \li 182 - Message type not compatible with protocol statE
 * \li 183 - Information element non existent
 * \li 184 - Conditonal ie error
 * \li 185 - Message not compatible with protocol state
 * \li 186 - Recovery on timer expired
 * \li 187 - Protocol error unspecified
 * \li 188 - Interworking  unspecified
 * \li 189 - Outgoing calls barred within cug
 * \li 190 - No cug selection
 * \li 191 - Unknown cug index
 * \li 192 - Cug index  incompatible
 * \li 193 - Cug call failure unspecified
 * \li 194 - Clir not subscribed
 * \li 195 - Ccbs possible sEE
 * \li 196 - Ccbs not possible
 * \subsection MM-GMM reject causes
 * \li 197 - Imsi unknown in hlr
 * \li 198 - Illegal ms
 * \li 199 - Imsi unknown in vlr
 * \li 200 - Imei not accepted
 * \li 201 - Illegal me sEE
 * \li 202 - Plmn not allowed
 * \li 203 - Location area not allowed
 * \li 204 - Roaming not allowed in this location area
 * \li 205 - No suitable cells in location area
 * \li 206 - Network failure sEE
 * \li 207 - mac failure sEE
 * \li 208 - Synch failure
 * \li 209 - Network congestion
 * \li 210 - GSM authentication  unacceptable
 * \li 211 - Service not subscribed
 * \li 212 - Service temporarily  out of order
 * \li 213 - Call cannot be  identified
 * \li 214 - Incorrect semantics  in message
 * \li 215 - Mandatory information invalid
 * \li 216 - Call failed due to other access stratum failures
 * \li 217 - SIM is invalid
 * \li 218 - Invalid call state
 * \li 229 - Access class is blocked
 * \li 220 - No resources are in the protocol stack to allow the call
 * \li 221 - Invalid user data was received
 * \subsection MM reject causes
 * \li 222 - Timer T3230 is expired
 * \li 223 - No cell is available
 * \li 225 - Radio link was lost due to other lower layer causes
 * \subsection CNM reject causes
 * \li 226 - Timer T303 is expired
 * \li 227 - CNM MM release is pending
 * \subsection Access stratum reject causes
 * \li 228 - Access stratum RR release indication
 * \li 229 - Access stratum random access failure
 * \li 230 - RRC_REL_IND Access stratum RRC release indication
 * \li 231 - Access stratum close session indication
 * \li 232 - Access stratum open session failure
 * \li 233 - Access stratum low level failure
 * \li 234 - Access stratum low level failure redial is not allowed
 * \li 235 - Access stratum low level immediate retry
 * \li 236 - Access stratum abort radio is unavailable
 * \subsection OTA reject causes
 * \li 237 - Service option is not supported
 * \subsection Additional IP end reasons
 * \li 300 - Received SIP 400 bad request;waiting for INVITE response
 * \li 301 - Received SIP 400 bad request;waiting for INVITE response
 * \li 302 - Received SIP 404 not found; call failed; called party does not exist
 * \li 303 - Received SIP 415 unsupported media type; call failed; called party does not support media
 * \li 304 - Received SIP 480 temporarily unavailable; call failed; called party is not in the LTE area
 * \li 305 - No network response; call failed
 * \li 306 - No network response; unable to put call on hold
 * \li 307 - Moved to eHRPD; call failed or dropped; not in the LTE area
 * \li 308 - Upgrade/downgrade rejected (200 OK with the current call SDP)
 * \li 309 - Received 403 call forbidden;waiting for INVITE response
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __GOBI_API_VOICE_CALL_END_REASONS_H__
#define __GOBI_API_VOICE_CALL_END_REASONS_H__


#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __GOBI_API_VOICE_CALL_END_REASONS_H__ */
