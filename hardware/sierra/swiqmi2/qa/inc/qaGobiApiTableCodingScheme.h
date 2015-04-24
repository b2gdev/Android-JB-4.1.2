/**
 * \ingroup voice
 *
 * \file    qaGobiApiTableCodingScheme.h
 *
 * \brief       Data Coding Scheme
 * \section     Table10 Coding Group Bits 7..4(0000)
 * \subsection  subsec1 Use of bits 3..0\n\n
 * \li          Language using the GSM 7 bit default alphabet Bits 3..0\n
 *              indicate the language:\n
 *              0000 German\n
 *              0001 English\n
 *              0010 Italian\n
 *              0011 French\n
 *              0100 Spanish\n
 *              0101 Dutch\n
 *              0110 Swedish\n
 *              0111 Danish\n
 *              1000 Portuguese\n
 *              1001 Finnish\n
 *              1010 Norwegian\n
 *              1011 Greek\n
 *              1100 Turkish\n
 *              1101 Hungarian\n
 *              1110 Polish\n
 *              1111 Language unspecifie\n
 * \section     sec2 Coding Group Bits 7..4(0001)
 * \subsection  subsec2 use of bits 3..0
 * \li          0000 GSM 7 bit default alphabet; message preceded by language indication.\n
 *              The first 3 characters of the message are a two-character representation of the\n
 *              language encoded according to ISO 639 [12], followed by a CR character. The CR\n
 *              character is then followed by 90 characters of text.\n
 * \li          0001 UCS2; message preceded by language indication\n
 *              The message starts with a two GSM 7-bit default alphabet character\n
 *              representation of the language encoded according to ISO 639. This is padded\n
 *              to the octet boundary with two bits set to 0 and then followed by 40 characters\n
 *              of UCS2-encoded message.\n
 *              An MS not supporting UCS2 coding will present the two character language\n
 *              identifier followed by improperly interpreted user data.\n
 * \section     sec3 Coding Group Bits 7..4(0010)
 * \subsection  subsec3 use of bits 3..0
 * \li          0000 Czech\n
 *              0001 Hebrew\n
 *              0010 Arabic\n
 *              0011 Russian\n
 *              0100 Icelandic\n
 *              0101..1111 Reserved for other languages using the GSM 7 bit\n
 *              default alphabet, with unspecified handling at the MS\n
 *
 * \section     sec4 Coding Group Bits 7..4(0011)
 * \subsection  subsec4 use of bits 3..0
 * \li          0000..1111 Reserved for other languages using the GSM 7 bit\n
 *              default alphabet, with unspecified handling at the MS\n
 * \section     sec5 Coding Group Bits 7..4(01xx)
 * \subsection  subsec5 use of bits 3..0
 * \li          General Data Coding indication\n
 *             - Bits 5..0 indicate the following:\n
 *             - Bit 5, if set to 0, indicates the text is uncompressed\n
 *             - Bit 5, if set to 1, indicates the text is compressed using
 *               the compression algorithm defined in 3GPP TS 23.042
 *             - Bit 4, if set to 0, indicates that bits 1 to 0 are reserved
 *               and have no message class meaning
 *             - Bit 4, if set to 1, indicates that bits 1 to 0 have a message
 *               class meaning:
 *               Bit 1 Bit 0 Message Class:\n
 *               0 0 Class 0\n
 *               0 1 Class 1 Default meaning: ME-specific.\n
 *               1 0 Class 2 (U)SIM specific message.\n
 *               1 1 Class 3 Default meaning: TE-specific (see 3GPP TS 27.005 )\n
 *               Bits 3 and 2 indicate the character set being used, as follows:\n
 *               Bit 3 Bit 2 Character set:\n
 *               0 0 GSM 7 bit default alphab\net
 *               0 1 8 bit data\n
 *               1 0 UCS2 (16 bit) [10]\n
 *               1 1 Reserved\n
 * \section    sec6 Coding Group Bits 7..4(1001)
 * \subsection subsec6 Reserved coding groups
 * \li         Message with User Data Header (UDH) structure:\n
 *             Bit 1 Bit 0 Message Class:\n
 *             0 0 Class 0\n
 *             0 1 Class 1 Default meaning: ME-specific.\n
 *             1 0 Class 2 (U)SIM specific message.\n
 *             1 1 Class 3 Default meaning: TE-specific (see 3GPP TS 27.005 [8])\n
 *             Bits 3 and 2 indicate the alphabet being used, as follows:\n
 *             Bit 3 Bit 2 Alphabet:\n
 *             0 0 GSM 7 bit default alphabet\n
 *             0 1 8 bit data\n
 *             1 0 USC2 (16 bit) [10]\n
 *             1 1 Reserved\n
 * \section    sec7 Coding Group Bits 7..4(1010..1101)
 * \subsection  subsec7 Reserved coding groups
 * \section     sec8 Coding Group Bits 7..4(1110)
 * \subsection  subsec8 Defined by the WAP Forum
 * \section     sec9 Coding Group Bits 7..4 (1111)
 * \subsection  subsec9 Data coding / message handling
 * \li          Bit 3 is reserved, set to 0.\n
 *              Bit 2 Message coding:\n
 *              0 GSM 7 bit default alphabet\n
 *              1 8 bit data\n
 *              Bit 1 Bit 0 Message Class:\n
 *              0 0 No message class.\n
 *              0 1 Class 1 user defined.\n
 *              1 0 Class 2 user defined.\n
 *              1 1 Class 3\n
 *              default meaning: TE specific(3GPP TS 27.005) \n
 *
 * Copyright: © 2012 Sierra Wireless, Inc. all rights reserved
 *
 */

#ifndef  _GOBI_API_CODING_SCHEME_H__
#define __GOBI_API_CODING_SCHEME_H__


#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* __GOBI_API_CODING_SCHEME_H__ */
