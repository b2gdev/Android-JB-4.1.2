/**
 * \ingroup voice
 *
 * \file    qaGobiApiTableServiceOptions.h
 *
 * \brief   Voice Service Options
 *
 * \section Table8 Service Option codes (Code - Reason)
 * \subsection Description
 * \li 0x0001 - Basic variable rate voice service (8 kbps)
 * \li 0x0002 - Mobile station loopback (8 kbps)
 * \li 0x0003 - Enhanced variable rate voice service (8 kbps)
 * \li 0x0004 - Asynchronous data service (9.6 kbps)
 * \li 0x0005 - Group 3 facsimile (9.6 kbps)
 * \li 0x0006 - Short message service (rate set 1)
 * \li 0x0007 - Packet data service: Internet or ISO Protocol stack (9.6 kbps)
 * \li 0x0008 - Packet data service: CDPD Protocol stack (9.6 kbps)
 * \li 0x0009 - Mobile station loopback (13 kbps)
 * \li 0x000A - transparent service
 * \li 0x000B - III nontransparent service
 * \li 0x000C - Asynchronous data service (14.4 or 9.6 kbps)
 * \li 0x000D - Group 3 facsimile (14.4 or 9.6 kbps)
 * \li 0x000E - Short message service (rate set 2)
 * \li 0x000F - Packet data service: Internet or ISO Protocol
 *              stack (14.4 kbps)
 * \li 0x0010 - Packet data service: CDPD Protocol stack (14.4 kbps)
 * \li 0x0011 - High-rate voice service (13 kbps)
 * \li 0x0012 - Over-the-air parameter administration (rate set 1)
 * \li 0x0013 - Over-the-air parameter administration (rate set 2)
 * \li 0x0014 - Group 3 analog facsimile (rate set 1)
 * \li 0x0015 - Group 3 analog facsimile (rate set 2)
 * \li 0x0016 - High-speed packet data service:
 *              Internet or ISO Protocol stack (RS1 forward, RS1 reverse)
 * \li 0x0017 - High-speed packet data service:
 *              Internet or ISO Protocol stack (RS1 forward, RS2 reverse)
 * \li 0x0018 - High-speed packet data service:
 *              Internet or ISO Protocol stack (RS2 forward, RS1 reverse)
 * \li 0x0019 - High-speed packet data service:
 *              Internet or ISO Protocol stack (RS2 forward, RS2 reverse)
 * \li 0x001A - High-speed packet data service:
 *              CDPD Protocol stack (RS1 forward, RS1 reverse)
 * \li 0x001B - High-speed packet data service:
 *              CDPD Protocol stack (RS1 forward, RS2 reverse)
 * \li 0x001C - High-speed packet data service:
 *              CDPD Protocol stack (RS2 forward, RS1 reverse)
 * \li 0x001D - High-speed packet data service:
 *              CDPD Protocol stack (RS2 forward, RS2 reverse)
 * \li 0x001E - RATE_SET_1 Supplemental channel loopback test for rate set 1
 * \li 0x001F - RATE_SET_2 Supplemental channel loopback test for rate set 2
 * \li 0x0020 - Test Data Service Option (TDSO)
 * \li 0x0021 - cdma2000 high-speed packet data service,
 *              Internet or ISO Protocol stack
 * \li 0x0022 - cdma2000 high-speed packet data service, CDPD Protocol
 * \li 0x0023 - Location services, rate set 1 (9.6 kbps)
 * \li 0x0024 - Location services, rate set 2 (14.4 kbps)
 * \li 0x0025 - ISDN interworking service (64 kbps)
 * \li 0x0026 - GSM voice
 * \li 0x0027 - GSM circuit data
 * \li 0x0028 - GSM packet data
 * \li 0x0029 - GSM short message service
 * \li 0x0036 - Markov Service Option (MSO)
 * \li 0x0037 - Loopback Service Option (LSO)
 * \li 0x0038 - Selectable mode vocoder
 * \li 0x0039 - 32 kbps circuit video conferencing
 * \li 0x003A - CONFERENCING 64 kbps circuit video conferencing
 * \li 0x003B - HRPD packet data service, which when used in paging over the 1X
 *              air interface, a page response is not required
 * \li 0x003C - Link Layer Assisted Robust Header Compression (LLA ROHC) -
 *               header removal
 * \li 0x003D - LLA ROHC - Header Compression
 * \li 0x003E - Source-controlled Variable-Rate Multimode Wideband (VMR-WB)
 *              speech codec rate set 2
 * \li 0x003F - Source-controlled VMR-WB speech codec rate set 1
 * \li 0x0040 - HRPD auxiliary packet data service instance
 * \li 0x0041 - cdma2000/GPRS interworking
 * \li 0x0042 - ISO_PROTOCOL_SO_66 cdma2000 high-speed packet data service,
 *              Internet or ISO Protocol stack
 * \li 0x0043 - HRPD packet data IP service where higher layer protocol
 *              is IP or ROHC
 * \li 0x0044 - Enhanced variable rate voice service (EVRC-B)
 * \li 0x0045 - HRPD packet data service, which when used in paging over
 *              the 1X air interface, a page response is required
 * \li 0x0046 - Enhanced variable rate voice service (EVRC-WB)
 * \li 0x1004 - Asynchronous data service, Revision 1 (9.6 or 14.4 kbps)
 * \li 0x1005 - Group 3 facsimile, Revision 1 (9.6 or 14.4 kbps)
 * \li 0x1007 - Packet data service: Internet or ISO Protocol stack,
 *              Revision 1 (9.6 or 14.4 kbps)
 * \li 0x1008 - Packet data service: CDPD Protocol stack, Revision 1
 *              (9.6 or 14.4 kbps)
 * \li 0x7FF8 - Identifies service reference identifier 0
 * \li 0x7FF9 - Identifies service reference identifier 1
 * \li 0x7FFA - Identifies service reference identifier 2
 * \li 0x7FFB - Identifies service reference identifier 3
 * \li 0x7FFC - Identifies service reference identifier 4
 * \li 0x7FFD - Identifies service reference identifier 5
 * \li 0x7FFE - Identifies service reference identifier 6
 * \li 0x7FFF - Identifies service reference identifier 7
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef __GOBI_API_SERVICE_OPTION_H__
#define __GOBI_API_SERVICE_OPTION_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __GOBI_API_SERVICE_OPTION_H__ */
