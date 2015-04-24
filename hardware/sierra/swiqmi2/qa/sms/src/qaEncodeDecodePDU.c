/*
 * \file    qaMakePdu.c
 *
 * \brief   Contains encoding funtions for text to PDU and decoding
 *          funtion for PDU to text
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 */

#include "SwiDataTypes.h"
#include "sludefs.h"
#include "qmudefs.h"
#include "qmerrno.h"
#include "stdio.h"
#include "qaEncodeDecodePDU.h"
#include <wchar.h>

/******************************************************************************
 * Support Funtion Definition
 ******************************************************************************/
/*
 * Converts Ascii format of data to Hex format
 *
 * \param  pHexa[OUT] - Converted hex data
 *         pStr[IN]   - String which need to be converted
 *
 * \return None
 *
 * \notes  None
 *
 */
void SwiAsc2Hex( BYTE *pHexa, CHAR *pStr )
{
   USHORT asciiCount;

   for ( asciiCount = 0 ; *pStr ; pStr++, asciiCount++ )
   {
       pHexa[asciiCount/2] = ((asciiCount & 1) ? pHexa[asciiCount/2] : 0)
       | ( ASC2HEX(*pStr) << ((asciiCount & 1) ? 0 : 4));
   }
}

/*
 * Converts Hex format of data to ASCII format
 *
 * \params  pStr[OUT] - Hold final converted ASCII string
 *          pHexa[IN] - Hex data to convert in ASCII
 *          len[IN]   - length of hex data
 *
 * \return  None
 *
 * \notes   None
 *
 */
void SwiHex2Asc( BYTE *pStr, BYTE *pHexa, USHORT len )
{
   USHORT idx;

   for ( idx = 0 ; idx < len ; idx ++ )
   {
       /* 0..9 + '0'   or   10..15 + '0' + 7 */
       pStr[idx] = HEX( pHexa, idx ) + 0x30 +\
                      ( (HEX( pHexa, idx) <= 9) ? 0 : 7 );
   }
   pStr[len] = 0;
}

/*
 * Swaps charater of sting passed
 *
 * \param  pStr[IN] - Pointer to string which need to be swapped
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 * \notes   none
 */
local ULONG Swapchars( CHAR* pStr )
{
    INT8 Length;
    INT8 position;
    CHAR c;
    Length = strlen(pStr);

    /* Length is 0,there are not enough character for swapping */
    if( Length <= 1 )
    {
        return eQCWWAN_ERR_SWISMS_SMSC_NUM_CORRUPTED;
    }

    for ( position = 0; position < Length-1; position+=2 )
    {
        c                = pStr[position];
        pStr[position]   = pStr[position+1];
        pStr[position+1] = c;
    }
    return eQCWWAN_ERR_NONE;
}

/******************************************************************************
 * GSM PDU Encoding Funtion Definition
 *****************************************************************************/
/*
 * Converts ascii text to pdu string
 *
 * \param   pText[IN]   - SMS to send
 *          length[IN] - SMS message length
 *          pPdu[OUT]  - Dynamically allocated memory pointer
 *                        which will hold final pdu message hex dump
 *
 * \return  pdu string length
 *
 * \notes   none
 */
local USHORT Text2Pdu(
    CHAR   *pText,
    USHORT length,
    CHAR   *pPdu )
{
    CHAR   tmp[500];
    CHAR   octett[10];
    USHORT pdubitposition;
    USHORT pdubyteposition = 0;
    USHORT character;
    USHORT bit;
    USHORT pdubitnr;
    USHORT counted_characters = 0;

    /* Fill the pdu memory buffer with zero */
    memset( pPdu, 0, MAXIMUM_PDU );

    /* Clear the tmp buffer */
    for ( character = 0; character < sizeof(tmp); character++ )
    {
        tmp[character] = 0;
    }

    /* Convert 8bit text stream to 7bit stream */
    for ( character = 0; character < length; character++ )
    {
       counted_characters++;
        for ( bit = 0; bit < 7; bit++ )
        {
            pdubitnr        = 7*character+bit;
            pdubyteposition = pdubitnr/8;
            pdubitposition  = pdubitnr%8;
            if ( pText[character] & (1<<bit) )
            {
                tmp[pdubyteposition] =
                   tmp[pdubyteposition] | (1<<pdubitposition);
            }
            else
            {
                tmp[pdubyteposition]=
                   tmp[pdubyteposition] & ~(1<<pdubitposition);
            }
        }
    }
    tmp[pdubyteposition+1] = 0;

    /* Convert 7bit stream to hex-dump */
    for ( character=0; character <= pdubyteposition; character++ )
    {
        sprintf(octett,"%02X",(BYTE)tmp[character]);
        strcat(pPdu,octett);
    }

    return counted_characters;
}

/*
 * Make PDU string with input mobile number and message
 *
 * \param  number[IN]     - Mobile number entered by user
 *         pMessage[IN]   - Sms to send
 *         messagelen[IN] - Length of sms
 *         pPdu[OUT]      - Dynamically allocated memory pointer
 *                          which will hold final pdu message hex dump
 *         validity [IN]  - PDU protocol described field
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 * \notes  none
 */

ULONG SwiMakePdu(
    CHAR   *number,
    CHAR   *pMessage,
    USHORT messagelen,
    CHAR   *pPdu,
    USHORT validity )
{
    USHORT coding = 0; /* for 7bit coding */
    USHORT flags;
    CHAR   tmp[50];
    CHAR   tmp2[500];
    CHAR   *pEndOfLine = NULL;
    USHORT numberformat;
    USHORT numberlength;
    ULONG nRet;

    if ( !number  ||
         !pMessage ||
         !pPdu )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    strcpy(tmp,number);
    numberlength = strlen(tmp);
    if( numberlength <= 20 )
    {
        numberformat = NATIONALFORMAT;
    }
    else
    {
        numberformat = NUMBERFORMAT;
    }

    if( NULL != (pEndOfLine = strchr( tmp, ENTER_KEY )) )
    {
        numberlength = numberlength -1;
        tmp[numberlength] = 0;
    }

    /* terminate the number with F if the length is odd */
    if ( numberlength%2 )
    {
        strcat(tmp,"F");
    }

    /* Swap every second character to make PDU Message */
    nRet = Swapchars(tmp);

    /* SMS-Sumbit MS to SMSC */
    flags = 1;

    /* Validity field */
    flags += 16;
    messagelen = Text2Pdu( pMessage,messagelen,tmp2 );
    sprintf( pPdu,
             "00%02X00%02X%02X%s00%02X%02X%02X",
             flags,
             numberlength,
             numberformat,
             tmp,
             coding,
             validity,
             messagelen );

    /* concatenate the text to the PDU string */
    strcat(pPdu,tmp2);
    return nRet;
}

/******************************************************************************
 * GSM PDU Decoding Funtion Definition
 *****************************************************************************/
/*
 * Converts an octet(ASCII) to a 8-Bit value
 *
 * \params  octet[IN] - octet which user wants to converts
 *
 * \return  8 bit value
 *
 * \notes   none
 */
local ULONG Octet2bin(CHAR* octet)
{
    ULONG result = 0;
    if ( octet[0]>57 )
       result += octet[0]-55;
    else
       result += octet[0]-48;
       result = result<<4;
    if ( octet[1]>57 )
       result += octet[1]-55;
    else
       result += octet[1]-48;
    return result;
}

/*
 * Converts PDU to text
 *
 * \params  pPdu[IN]  - Pdu message which user wants to converts in text
 *          text[OUT] - Text message
 *          with_udh  - If UDH want to fectch UDH header
 *
 * \return  Length of text
 *
 * \notes   None
 */
local ULONG Pdu2text(
    CHAR *pPdu,
    BYTE *pTextMsgLength,
    CHAR *text,
    ULONG with_udh )
{
     ULONG bitposition=0;
     ULONG byteposition;
     ULONG byteoffset;
     ULONG charcounter;
     ULONG bitcounter;
     ULONG septets;
     ULONG octets;
     ULONG udhsize;
     ULONG octetcounter;
     ULONG skip_characters;
     CHAR  c;
     CHAR  binary[500]={'\0'};

     septets = Octet2bin(pPdu);
     /* Validate the PDU message */
     if ( septets > 184 )
     {
         return eQCWWAN_ERR_SWISMS_MSG_CORRUPTED;
     }

     if( *pTextMsgLength < septets )
     {
         *pTextMsgLength = septets;
         return eQCWWAN_ERR_BUFFER_SZ;
     }

     if ( with_udh )
     {
         /* copy the data header to udh and convert to hex dump */
         udhsize = Octet2bin(pPdu+2);

         /* Calculate how many text characters include the UDH.*/
         /* After the UDH there may follow filling bits to reach a 7bit boundary.*/
         skip_characters=(((udhsize+1)*8)+6)/7;
     }
     else
     {
         skip_characters = 0;
     }

     /* Copy user data of pPdu to a buffer */
     octets = (septets*7+7)/8;
     for (octetcounter=0; octetcounter<octets; octetcounter++)
          binary[octetcounter]=Octet2bin(pPdu+(octetcounter<<1)+2);

     /* Then convert from 8-Bit to 7-Bit encapsulated in 8 bit */
     /* skipping storing of some characters used by UDH. */
     for (charcounter=0; charcounter<septets; charcounter++)
     {
          c=0;
          for (bitcounter=0; bitcounter<7; bitcounter++)
     {
          byteposition = bitposition/8;
          byteoffset   = bitposition%8;
     if (binary[byteposition]&(1<<byteoffset))
          c = c|128;
     bitposition++;
     c=(c>>1)&127; /* The shift fills with 1, but I want 0 */
    }
    if (charcounter>=skip_characters)
        text[charcounter-skip_characters]=c;
    }
    text[charcounter-skip_characters]=0;
    return charcounter - skip_characters;
}

/*
 * Extract PDU for messages type 0 (SMS-Deliver)
 *
 * \param   pSrcPointer[IN] - Pointer to source string
 *          pSendr[OUT]     - sender address
 *          pDate[OUT]      - date field
 *          pTime[OUT]      - time field
 *          pmessage[OUT]   - Pointer to message fetched
 *          with_udh[OUT]   - If UDH header present in message
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 * \notes  None
 */
local ULONG ExtractSMSDeliver(
    CHAR  *pSrcPointer,
    BYTE  *pSenderAddrLength,
    CHAR  *pSendr,
    CHAR  *pDate,
    CHAR  *pTime,
    BYTE  *pTextMsgLength,
    CHAR  *pMessage,
    ULONG with_udh )
{
    ULONG Length;
    ULONG padding;
    ULONG nRet;
    if ( NULL == pSrcPointer && 0 == strlen(pSrcPointer) )
    {
        return eQCWWAN_ERR_SWISMS_MSG_CORRUPTED;
    }
    Length      = Octet2bin(pSrcPointer);

    /* Check If size of buffer is sufficient to hold message */
    if( *pSenderAddrLength < Length )
    {
        *pSenderAddrLength = Length;
        return eQCWWAN_ERR_BUFFER_SZ;
    }
    if ( Length >= 160 )
    {
        return eQCWWAN_ERR_SWISMS_MSG_LEN_TOO_LONG;
    }
    padding     = Length % 2;
    pSrcPointer += 4;

    strncpy(pSendr,pSrcPointer,Length+padding);
    nRet = Swapchars(pSendr);
    pSendr[Length]=0;
    if (pSendr[Length-1]=='F')
        pSendr[Length-1]=0;

    pSrcPointer = pSrcPointer + Length + padding + 3;

    /* Increment pointer to read date */
    pSrcPointer += 1;

    /* prepare date format to print */
    sprintf(pDate,"%c%c-%c%c-%c%c",pSrcPointer[5],pSrcPointer[4],\
                  pSrcPointer[3],pSrcPointer[2],pSrcPointer[1],pSrcPointer[0]);

    /* Increment pointer to read time stamp */
    pSrcPointer = pSrcPointer + 6;

    /* Prepare time format to print */
    sprintf(pTime,"%c%c:%c%c:%c%c",pSrcPointer[1],pSrcPointer[0],\
             pSrcPointer[3],pSrcPointer[2],pSrcPointer[5],pSrcPointer[4]);

    /* Increment pointer to point at message */
    pSrcPointer = pSrcPointer + 8;
    Pdu2text(pSrcPointer,pTextMsgLength,pMessage,with_udh);
    return nRet;
}

/*
 * Extract PDU for messages type 1 (Sent SMS)
 * \param  pSrcPointer[IN] - Pointer to source string
 *         pSendr[OUT]     - date field
 *         pTime[OUT]      - time field
 *         pDate[OUT]      - date field
 *         pMessage[OUT]   - Pointer to message fetched
 *         with_udh        - If UDH header present in message
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 * \notes   none
 */
local ULONG ExtractSMSSent(
    CHAR  *pSrcPointer,
    BYTE  *pSenderAddrLength,
    CHAR  *pSendr,
    CHAR  *pDate,
    CHAR  *pTime,
    BYTE  *pTextMsgLength,
    CHAR  *pMessage,
    ULONG with_udh )
{
    ULONG length;
    ULONG padding;
    ULONG nRet;

    UNUSEDPARAM( pDate );
    UNUSEDPARAM( pTime );

    if( NULL == pSrcPointer )
    {
        return eQCWWAN_ERR_SWISMS_MSG_CORRUPTED;
    }
    length  = Octet2bin(pSrcPointer);

    /* Check If size of buffer is sufficient to hold Sender Address */
    if( *pSenderAddrLength < length )
    {
        *pSenderAddrLength = length;
        return eQCWWAN_ERR_BUFFER_SZ;
    }
    padding = length%2;
    pSrcPointer += 4;
    strncpy(pSendr,pSrcPointer,length+padding);
    nRet = Swapchars(pSendr);
    pSendr[length]=0;
    if (pSendr[length-1]=='F')
        pSendr[length-1]=0;

    pSrcPointer = pSrcPointer + length + padding+3;
    pSrcPointer += 3;
    Pdu2text(pSrcPointer,pTextMsgLength,pMessage,with_udh);
    return nRet;
}

/*
 * Extract PDU read from device to readable form of message
 *
 * \params  pPdu[IN]       - Pointer to GSM PDU string
 *          pSendr[OUT]   - sender address
 *          pDate[OUT]    - date field
 *          pTime[OUT]    - time field
 *          pMessage[OUT] - Pointer to message fetched
 *          pSmsc[OUT]     - SMSC address
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 * \notes   None
 */
ULONG SwiExtractpdu(
    CHAR *pPdu,
    BYTE *pSenderAddrLength,
    CHAR *pSendr,
    BYTE *pTextMsgLength,
    BYTE *pScAddrLength,
    CHAR *pDate,
    CHAR *pTime,
    CHAR *pMessage,
    CHAR *pSmsc )
{
    ULONG Length    = 0;
    ULONG Type      = 0 ;
    CHAR  *Pointer  = NULL;
    pSendr[0]       = 0;
    pDate[0]        = 0;
    pTime[0]        = 0;
    pMessage[0]     = 0;
    pSmsc[0]         = 0;
    *pMessage       = 0;
    ULONG with_udh  = 0;

    if( NULL == pPdu && 0 == strlen(pPdu) )
    {
        return eQCWWAN_ERR_SWISMS_MSG_CORRUPTED;
    }
    Pointer = pPdu;

    /* Get senders pSmsc */
    Length = Octet2bin(Pointer)*2-2;

    if ( *pScAddrLength < Length )
    {
        *pScAddrLength = Length;
        return eQCWWAN_ERR_BUFFER_SZ;
    }
    if ( Length > 0 && Length < 13 )
    {
        Pointer = Pointer+4;
        strncpy(pSmsc,Pointer,Length);
        Swapchars(pSmsc);
        pSmsc[Length]=0;
        if (pSmsc[Length-1]=='F')
            pSmsc[Length-1]=0;
        Pointer = Pointer + Length;
    }
    /* Is UDH bit set?*/
    if ( Octet2bin(Pointer) & 64 )
        with_udh = 1;

    Type = Octet2bin(Pointer) & 3;
    Pointer += 2;

    /* SMS Deliver */
    if ( Type == 0 )
    {
        return ExtractSMSDeliver( Pointer,pSenderAddrLength,\
                                  pSendr,pDate,pTime,pTextMsgLength,\
                                  pMessage,with_udh );

    }
    /*  Sent message */
    else if ( Type == 1 )
    {
        return ExtractSMSSent( Pointer+2,pSenderAddrLength,\
                               pSendr,pDate,pTime,pTextMsgLength,\
                               pMessage,with_udh );
    }
    else
    /* Unsupported type */
    {
        sprintf(pMessage,"Message format (%lx) is not supported."
                        " Cannot decode.\n",Type);
        return eQCWWAN_ERR_NONE;
    }
}

/****************************************************************************
*                       CDMA PDU Encoding Functions
*****************************************************************************/
/*
 * Make PDU string with input mobile number and message
 *
 * \param  pPackedBuffer[OUT] - Pointer to dynamically allocated buffer
 *                              which holds packed header
 *         relValue[IN]       - pRelValidity (optional pointer)
 *                              Gives the relative validity period
 *                              of the outgoing message
 *                              per per 4.5.6-1 of spec
 *                              0 - Set Relative validity to 11
 *                              1 - Set Relative validity to 71
 *                              2 - Set Relative validity to 167
 *                              3 - Set Relative validity to 169
 *                              4 - Set Relative validity to 171
 *                              Values have the following meanings:
 *                              0 to 143: validity period = (value + 1)*
 *                              5 minutes
 *                              144 to 167: validity period = 12 hours
 *                              + (value - 143)*30 minutes
 *                              168 to 196: validity period =
 *                              (value - 166) * 1 day
 *                              197 to 244: validity period =
 *                              (value - 192) * 1 week
 *                              245: validity period = indefinite
 *         Len[IN]            - Pointer to Length of Validity header buffer
 *
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 * \note    Use of this function is optional in CDMA message encoding
 */
local ULONG EncodeRelValidityHdr(
    BYTE  *pPackedBuffer,
    BYTE  *relValue,
    ULONG *Len )
{
    BYTE* pBuf;
    ULONG bytesPacked = 0;
    BYTE  RelValidity = 0;

    if( pPackedBuffer == NULL || Len == NULL )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    if( *Len < 3 )
    {
        return eQCWWAN_ERR_BUFFER_SZ;
    }

    pBuf = pPackedBuffer;
    *pBuf++ = REL_VAL_PERIOD_SUB_IDENTIFIER;
    bytesPacked++;
    *pBuf++ = REL_VAL_PERIOD_SUB_IDENTIFIER_HDR_LEN;
    bytesPacked++;

    if( relValue )
    {
       RelValidity = *relValue;
    }

    switch(RelValidity)
    {
        case 0:
            *pBuf = 11;  /* decimal value as per 4.5.6-1 of spec */
            break;
        case 1:
            *pBuf = 71;  /* decimal value as per 4.5.6-1 of spec */
            break;
        case 2:
            *pBuf = 167; /* decimal value as per 4.5.6-1 of spec */
            break;
        case 3:
            *pBuf = 169; /* decimal value as per 4.5.6-1 of spec */
            break;
        case 4:
            *pBuf = 171; /* decimal value as per 4.5.6-1 of spec */
            break;
    }
    bytesPacked++;

    /* Update Len to indicate output buffer length */
    *Len = bytesPacked;
    pBuf = pPackedBuffer;

    return eQCWWAN_ERR_NONE;
}

/* Helper Packing Functions */

/*
 * Make PDU string with input mobile number and message
 *
 * \param  pPackedBuffer[OUT] - Pointer to dynamically allocated memory,
 *                              hold encoded priority
 *         Priority [IN]      - Gives the priority of the outgoing message:
 *                              0   - normal (default if pointer is NULL)
 *                              1   - interactive
 *                              2   - urgent
 *                              3   - emergency
 *                              64  - 64 is decoded value for URGENT VZAM,
 *                                    Support interactive
 *                              128 - 128 is decoded value for URGENT VZAM,
 *                                    Support urgent
 *                              192 - 128 is decoded value for URGENT VZAM
 *                                    Support emergency
 *         Len [IN] - Pointer to Length of Priority header buffer
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 * \notes  Default value 3 for length parameter is used,
 *         Use of this function is optional in CDMA message encoding
 */
local ULONG EncodePriorityHdr(
    BYTE  *pPackedBuffer,
    BYTE  Priority,
    ULONG *Len )
{
    BYTE *pBuf;
    ULONG bytesPacked = 0;

    if( pPackedBuffer == NULL || Len == NULL )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    if( *Len < 3 )
    {
        return eQCWWAN_ERR_BUFFER_SZ;
    }

    if( Priority > 2 )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    pBuf = pPackedBuffer;
    *pBuf++ = PRIORIY_SUB_IDENTIFIER;
    bytesPacked++;
    *pBuf++ = PRIORIY_SUB_IDENTIFIER_HDR_LEN;
    bytesPacked++;

    switch(Priority)
    {
        case 0:
            *pBuf = 0;
            break;
        case 1:
            *pBuf = 64;  /* Binary format 01-000000 */
            break;
        case 2:
            *pBuf = 128; /* Binary format 10-000000 */
            break;
        case 3:
            *pBuf = 192; /* Binary format 11-000000 */
            break;
    }
    bytesPacked++;

    /* Update Len to indicate output buffer len;*/
    *Len = bytesPacked;
    pBuf = pPackedBuffer;

    return eQCWWAN_ERR_NONE;
}

/*
 * Packs the given byte into the destination at the given offset
 * for the given number of length bits
 *
 * \param  src[IN]  - Source string
 *         dst[OUT] - Address of buffer, hold final value
 *         pos[IN]  - Offset in trms of numer of bits
 *         len[IN]  - Number of length bits to be packed
 *
 * \return None
 *
 * \notes  none
 */
local void PackByteToBits(BYTE src, BYTE dst[], WORD pos, WORD len )
{
    WORD   t_pos = pos % 8;
    WORD   bits  = 8 - t_pos;

    dst += (pos+len-1)/8;

    if ( bits >= len )
    {
        *dst &= (BYTE) ~MASK_B(t_pos, len);
        *dst |= (BYTE) (MASK_B(t_pos, len) & (src << (bits - len)));
    }
    else /* len > bits */
    {
        dst--;
        *dst &= (BYTE) ~MASK_B(t_pos, bits);
        *dst |= (BYTE) (MASK_B(t_pos, bits) & (src >> (len - bits)));

        dst++;
        *dst &= (BYTE) ~MASK_B(0, (len - bits));
        *dst |= (BYTE) (MASK_B(0, (len - bits)) & (src << (8 - (len - bits))));
    }
}

/*
 * Convert ascii char to digits
 *
 * \param  symbol[IN] - Character which need to be converted in number
 *
 * \return converted number
 *
 * \note   none
 */
local BYTE Char2Num( CHAR symbol )
{
    BYTE Num;
    Num = (BYTE)symbol;

    Num = Num - 48;

    return Num;
}

/*
 * Convert digits to ascii char
 *
 * \params  Num [IN] - Number which need to be converted in character
 *
 * \return  Converted character
 *
 * \notes   none
 */
local CHAR Num2Char( BYTE Num )
{
    CHAR c;

    c = (CHAR)Num + 48;

    return c;
}

/*
 * Packs destination address to CDMA PDU format header
 *
 * \param  pPackedBuffer[OUT] - Pointer to dynamically allocated buffer,
 *                              holds final value
 *         pszDestAddr[IN]    - Pointer to Destination number(Mobile number)
 *         DestAddrLen        - Length of destination number(Mobile number)
 *         Len[IN/OUT]        - On input pointer to the length of allocated
 *                              memory, on output indicate buffer length
 *         ReserveBits        - Indicates reserve bits
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 * \notes  none
 */
local ULONG EncodeDestAddrHdr(
    BYTE  *pPackedBuffer,
    CHAR  *pszDestAddr,
    BYTE  DestAddrLen,
    ULONG *Len,
    BYTE  ReserveBits )
{
    BYTE DestAddrId, HdrLen, NumAndDigitMode, NumsOfDigits, Reserved;
    WORD   bit_pos = 0;
    BYTE  *numbers;
    CHAR  *pszNum;
    ULONG bytesPacked = 0;
    BYTE  RetNum = 0;

    if( pPackedBuffer == NULL ||
        Len == NULL || pszDestAddr == NULL )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    NumsOfDigits = DestAddrLen;
    pszNum = pszDestAddr;
    numbers = (BYTE*)malloc(NumsOfDigits);

    /* Convert ascii char to digits */
    int i;
    for(i = 0; i < NumsOfDigits; i++ )
    {
         RetNum = Char2Num( *(pszNum+i) );

        /* We are defaulting to 4 bit DTMF coding for
        Destination phone number. In that coding
        0 is represented as 1010 in binary format */

        if( RetNum == 0 )
        {
            *(numbers+i) = 10;
        }
        else
        {
            *(numbers+i) = RetNum;
        }
    }

    DestAddrId = DESTADDR_IDENTIFIER;  /* For MO-SMS the id = 4; */
    PackByteToBits( DestAddrId,pPackedBuffer, bit_pos,8 );
    bit_pos += 8;
    bytesPacked++;

    /*Len is total memory allocated for Destination Addr buffer */
    HdrLen = (BYTE)*Len - 2;
    PackByteToBits( HdrLen,pPackedBuffer, bit_pos,8 );
    bit_pos += 8;
    bytesPacked++;

   /* Num Mode and Digit Mode are 1 bit in size and
      set to zero for regular cell phone numbers
      hence copy 2 bits */

    NumAndDigitMode = 0;
    PackByteToBits( NumAndDigitMode,pPackedBuffer, bit_pos,2 );
    bit_pos += 2;

    PackByteToBits( NumsOfDigits,pPackedBuffer, bit_pos,8 );
    bit_pos += 8;

    /* Pack each digit in 4 bit DTMF format */
    int ii;
    for(ii = 0; ii < NumsOfDigits ; ii++ )
    {
        PackByteToBits( *(numbers+ii),pPackedBuffer, bit_pos,4 );
        bit_pos += 4;
    }

    if( ReserveBits )
    {
        Reserved = 0;
        PackByteToBits( Reserved,pPackedBuffer, bit_pos,ReserveBits );
        bit_pos += ReserveBits;
    }

    bytesPacked = (bit_pos/8);

    /* Update Len to indicate output buffer length */
    *Len = bytesPacked;

    if( numbers )
    {
        free(numbers);
    }

    return eQCWWAN_ERR_NONE;
}

/*
 * Packs callback number to CDMA PDU format
 *
 * \param  pPackedBuffer[OUT] - Pointer to dynamically allocated buffer,
 *                              holds final value
 *         pszCallbackAddr[IN]- Pointer to callback address
 *         CallbackLen        - Length of callback number
 *         Len[IN/OUT]        - On input pointer to the length of allocated
 *                              memory, on output indicate buffer length
 *         ReserveBits        - Indicates reserve bits
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 * \notes  none
 */
local ULONG EncodeCallbackNumber(
    BYTE  *pPackedBuffer,
    CHAR  *pszCallbackAddr,
    BYTE  CallbackLen,
    ULONG *Len,
    BYTE  ReserveBits )
{
    BYTE CallbackId, HdrLen, DigitMode,NumsOfDigits,Reserved;
    WORD  bit_pos = 0;
    BYTE  *numbers;
    CHAR  *pszNum;
    ULONG bytesPacked = 0;
    BYTE  RetNum = 0;

    if( pPackedBuffer == NULL || Len == NULL || pszCallbackAddr == NULL )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    NumsOfDigits = CallbackLen;
    pszNum = pszCallbackAddr;
    numbers = (BYTE*)malloc(NumsOfDigits);

    /* Convert ascii char to digits */
    int i;
    for( i = 0; i < NumsOfDigits; i++ )
    {
         RetNum = Char2Num( *(pszNum+i) );

        /*We are defaulting to 4 bit DTMF coding for
        callback phone number. In that coding
        0 is represented as 1010 in binary format.*/

        if( RetNum == 0 )
        {
            *(numbers+i) = 10;
        }
        else
        {
            *(numbers+i) = RetNum;
        }
    }

    CallbackId = CALLBACK_NUM_SUB_IDENTIFIER;
    PackByteToBits( CallbackId,pPackedBuffer, bit_pos,8 );
    bit_pos += 8;
    bytesPacked++;

    HdrLen = (BYTE)*Len - 2;
    PackByteToBits( HdrLen,pPackedBuffer, bit_pos,8 );
    bit_pos += 8;
    bytesPacked++;

    DigitMode = 0;
    PackByteToBits( DigitMode,pPackedBuffer, bit_pos,1 );
    bit_pos += 1;

    PackByteToBits( NumsOfDigits,pPackedBuffer, bit_pos,8 );
    bit_pos += 8;

    /* Pack each digit in 4 bit DTMF format */
    int ii;
    for( ii = 0; ii < NumsOfDigits ; ii++ )
    {
        PackByteToBits( *(numbers+ii),pPackedBuffer, bit_pos,4 );
        bit_pos += 4;
    }

    if( ReserveBits )
    {
        Reserved = 0;
        PackByteToBits( Reserved,pPackedBuffer, bit_pos,ReserveBits );
        bit_pos += ReserveBits;
    }

    bytesPacked = (bit_pos/8);

    /* Update Len to indicate output buffer length */
    *Len = bytesPacked;

    if( numbers )
    {
        free(numbers);
    }

    return eQCWWAN_ERR_NONE;
}

/*
 * Packs userdata to CDMA PDU format
 *
 * \param  pPackedBuffer[OUT] - Pointer to dynamically allocated buffer,
 *                              holds final value
 *         pszSmsMsg[IN]      - Pointer to sms message
 *         MsgLen[IN]         - Length of sms message
 *         Len[IN/OUT]        - On input pointer to the length of allocated
 *                              memory, on output indicate buffer length
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 * \notes  none
 */
local ULONG EncodeUserDataHdr(
    BYTE *pPackedBuffer,
    CHAR *pszSmsMsg,
    BYTE  MsgLen,
    ULONG *Len )
{
    ULONG totalbits = 0, temp = 0;
    ULONG totalbytes = 0;
    BYTE  NumofReserveBits = 0;

    CHAR *pszTempMsg;
    BYTE UserDataId, HdrLen, MsgEncoding, Reserved;
    WORD bit_pos = 0;

    if( pPackedBuffer == NULL ||
        Len == NULL ||
        pszSmsMsg == NULL ||
        MsgLen > 160 )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* First calculate the number of BYTES need to pack complete information */
    totalbits = MsgLen*7;
    totalbits += 8;
    totalbits += 5;

    temp =  totalbits % 8;

    if( temp == 0)
    {
        /* No padding needed */
        totalbytes = (totalbits/8)+2;
        NumofReserveBits = 0;
    }
    else
    {
        NumofReserveBits = (BYTE)(8 - (totalbits % 8) );
        totalbits = totalbits + NumofReserveBits;
        /* 1 Byte identifier + 1 Byte Length + Rest of the bytes
           + Reserved bits */
        totalbytes = (totalbits/8)+2;
    }

    if( *Len < totalbytes )
    {
        return eQCWWAN_ERR_BUFFER_SZ;
    }

    UserDataId = USER_DATA_SUB_IDENTIFIER;
    PackByteToBits( UserDataId,pPackedBuffer, bit_pos,8 );
    bit_pos += 8;

    HdrLen = (BYTE)totalbytes - 2;
    PackByteToBits( HdrLen,pPackedBuffer, bit_pos,8 );
    bit_pos += 8;

    MsgEncoding = USER_DATA_MSG_ENCODING_7BIT_ASCII;
    PackByteToBits( MsgEncoding,pPackedBuffer, bit_pos,5);
    bit_pos += 5;

    PackByteToBits( MsgLen,pPackedBuffer, bit_pos,8);
    bit_pos += 8;

    pszTempMsg = pszSmsMsg;
    int ii;
    for( ii = 0; ii < MsgLen ; ii++ )
    {
        PackByteToBits( *(pszTempMsg+ii),pPackedBuffer, bit_pos,7 );
        bit_pos += 7;
    }

    if( NumofReserveBits )
    {
        Reserved = 0;
        PackByteToBits( Reserved,pPackedBuffer, bit_pos,NumofReserveBits );
        bit_pos += NumofReserveBits;
    }

    /* Update Len to indicate output buffer length */
    *Len = totalbytes;

    return eQCWWAN_ERR_NONE;
}

/*
 * Packs message identifier to CDMA PDU format
 *
 * \params  pPackedBuffer[OUT] - Pointer to dynamically allocated buffer,
 *                               holds final value
 *          MsgType            - MsgType
 *          MsgId              - The message reference number for this
 *                               message.This value should be incremented
 *                               for every message the host application sends
 *          Len[IN/OUT]        - On input pointer to the length of allocated
 *                               memory, on output indicate buffer length
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 * \notes  none
 */
local ULONG EncodeMsgIdenfier(
    BYTE   *pPackedBuffer,
    BYTE   MsgType,
    USHORT MsgId,
    ULONG  *Len )
{
    BYTE  MsgIdentifier, HdrLen, temp;
    WORD  bit_pos = 0;
    ULONG bytesPacked = 0;

    if( pPackedBuffer == NULL || Len == NULL || MsgType > 6  )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    if( *Len < 5 )
    {
        return eQCWWAN_ERR_BUFFER_SZ;
    }

    MsgIdentifier = MSG_SUB_IDENTIFIER_ID;
    PackByteToBits( MsgIdentifier,pPackedBuffer, bit_pos,8 );
    bit_pos += 8;
    bytesPacked++;

    HdrLen = MSG_SUB_IDENTIFIER_HDR_LEN;
    PackByteToBits( HdrLen,pPackedBuffer, bit_pos,8 );
    bit_pos += 8;
    bytesPacked++;

    /* MsgType = 2 is MO-SUBMIT */
    PackByteToBits( MsgType,pPackedBuffer, bit_pos,4 );
    bit_pos += 4;

    temp  = MsgId >> 8;
    PackByteToBits( temp,pPackedBuffer, bit_pos,8 );
    bit_pos += 8;

    temp = (BYTE)MsgId;
    PackByteToBits( temp,pPackedBuffer, bit_pos,8 );
    bit_pos += 8;

    /* Last 4 bits zero -- reserved */
    PackByteToBits( 0,pPackedBuffer, bit_pos,4 );
    bit_pos += 4;

    bytesPacked = bytesPacked + 3;

    /* Update Len to indicate output buffer length */
    *Len = bytesPacked;

    return eQCWWAN_ERR_NONE;
}
/*
 * Packs bearerdata to CDMA PDU format
 *
 * \params  pPackedBuffer[OUT] - Pointer to dynamically allocated buffer,
 *                               holds final value
 *          DataLen            - length of bearer data
 *          Len[IN/OUT]        - On input pointer to the length of allocated
 *                               memory, on output indicate buffer length
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 * \notes  none
 */
local ULONG EncodeBearerData(
    BYTE  *pPackedBuffer,
    BYTE  DataLen,
    ULONG *Len )
{
    BYTE* pBuf;
    ULONG bytesPacked = 0;

    if( pPackedBuffer == NULL || Len == NULL  )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    if( *Len < 2 )
    {
        return eQCWWAN_ERR_BUFFER_SZ;
    }

    pBuf = pPackedBuffer;
    *pBuf++ = BEARER_DATA_IDENTIFIER;
    bytesPacked++;

    *pBuf++ = DataLen;
    bytesPacked++;

    /* Update Len to indicate output buffer length */
    *Len = bytesPacked;
    pBuf = pPackedBuffer;

    return eQCWWAN_ERR_NONE;
}

/*
 * Packs bearerReplyOptionHdr to CDMA PDU format
 *
 * \param  pPackedBuffer[OUT] - Pointer to dynamically allocated buffer,
 *                              holds final value
 *         Len[IN/OUT]        - On input pointer to the length of allocated
 *                              memory, on output indicate buffer length
 *
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 * \note   None
 */
local ULONG EncodeBearerReplyOptionHdr( BYTE  *pPackedBuffer,
                                        ULONG *Len )
{
    BYTE* pBuf;
    ULONG bytesPacked = 0;

    if( pPackedBuffer == NULL || Len == NULL )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    if( *Len < 3 )
    {
        return eQCWWAN_ERR_BUFFER_SZ;
    }

    pBuf = pPackedBuffer;
    *pBuf++ = BEARER_REPLY_IDENTIFIER;
    bytesPacked++;
    *pBuf++ = BEARER_REPLY_HDR_LEN;
    bytesPacked++;

    *pBuf = 0;
    bytesPacked++;

    /* Update Len to indicate output buffer length */
    *Len = bytesPacked;
    pBuf = pPackedBuffer;

    return eQCWWAN_ERR_NONE;
}

/*
 * Packs TeleServiceHdr to CDMA PDU format
 *
 * \param  pPackedBuffer[OUT] - Pointer to dynamically allocated buffer,
 *                              holds final value
 *         pLen[IN/OUT]       - On input pointer to the length of allocated
 *                              memory, on output indicate buffer length
 *
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 * \note   None
 */
local ULONG EncodeTeleServiceHdr( BYTE  *pPackedBuffer,
                                  ULONG *Len)
{
    USHORT teleidvalue;
    BYTE* pBuf;
    ULONG bytesPacked = 0;
    BYTE temp = 0;

    if( pPackedBuffer == NULL || Len == NULL )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    if( *Len < 5 )
    {
        return eQCWWAN_ERR_BUFFER_SZ;
    }

    teleidvalue = TELESERVICE_ID_VALUE;
    pBuf = pPackedBuffer;

    *pBuf++ = POINT_TO_POINT_MSG;
    bytesPacked++;
    *pBuf++ = TELESERVICE_IDENTIFIER;
    bytesPacked++;
    *pBuf++ = TELESERVICE_HDR_LEN;
    bytesPacked++;

    temp = teleidvalue >> 8;
    *pBuf++ = temp;
    bytesPacked++;

    temp = (BYTE)teleidvalue;
    *pBuf++ = temp;
    bytesPacked++;

    /* Update Len to indicate output buffer length */
    *Len = bytesPacked;
    pBuf = pPackedBuffer;

    return eQCWWAN_ERR_NONE;
}

/*
 * Function to convert char string to WORD string
 *
 * \param  pDest[OUT] - Pointer to WORD output string caller allocates
 *         pSrc[IN]   - Pointer to CHAR string
 *         srcLen[IN] - Length of the CHAR string
 *
 *
 * \return Pointer to converted string
 *
 * \note   None
 *
 */
local wchar_t *SW_AtoT ( wchar_t *pDest, char *pSrc, ULONG srcLen)
{
    wcsncpy((wchar_t*)pDest, (wchar_t*)pSrc, srcLen);
    return (pDest);
}

/*
 * Convenient function to convert WORD string to char string
 *
 * \param  pDest[OUT] - Pointer to char output string caller allocates
 *         pSrc[IN]   - Pointer to WORD string
 *         srcLen[IN] - Length of the WORD string
 *
 * \return Pointer to converted ACSII string
 *
 * \note   None
 *
 */
local CHAR *SW_TtoA (char* pDest, wchar_t *pSrc, ULONG srcLen)
{
    strncpy((char*)pDest, (char*)pSrc, srcLen );
    return (pDest);
}

/*
 * Encode text message to CDMA PDU fromat
 *
 * \param  pOut[OUT]
 *         - The constructed raw message
 * \param  pOutLen [OUT]
 *         - Upon input, specifies the total number of bytes
 *           that the given pOut buffer can hold
 *           (a buffer of length 240 is recommended)Upon
 *           successful output, specifies the length of the
 *           constructed
 * \param  pDestNum [IN]
 *         - NULL-terminated ASCII String containing a
 *           destination address( mobile number )
 *
 * \param  DestNumLen [OUT]
 *         - destination address length
 *
 * \param  pCallbackNum[IN]
 *         - NULL-terminated ASCII String containing a
 *           callback address International number
 *
 * \param  CallbackNumLen[IN]
 *         - callback address length
 *
 * \param  pMsgBody[IN]
 *         - Pointer to text sms message
 *
 * \param  MsgLen[IN]
 *         - Text sms message length
 *
 * \param  MsgType[IN]
 *         - Indicate type of message
 *
 * \param  MsgId[IN]
 *         - The message reference number for this message
 *           This value should be incremented for every
 *           message the host application sends
 *
 * \param  Priority[IN]
 *         - Gives the priority of the outgoing message:
 *           0 - normal (default if NULL pointer is given)
 *           1 - interactive
 *           2 - urgent
 *           3 - emergency
 *           64 - 64 is decoded value for URGENT VZAM Support interactive
 *           128 - 128 is decoded value for URGENT VZAM Support urgent
 *           192 - 128 is decoded value for URGENT VZAM Support emergency
 * \param  RelVal
 *         - Gives the relative validity period of the
 *           outgoing message per 4.5.6-1 of spec
 *           0 - Set Relative validity to 11
 *           1 - Set Relative validity to 71
 *           2 - Set Relative validity to 167
 *           3 - Set Relative validity to 169
 *           4 - Set Relative validity to 171
 *               Values have the following meanings:
 *           0 to 143: validity period = (value + 1)* 5
 *                     minutes
 *           144 to 167: validity period = 12 hours +
 *                      (value - 143)*30 minutes
 *                              168 to 196: validity period = (value - 166) *
 *                               1 day
 *                              197 to 244: validity period = (value - 192) *
 *                              1 week
 *                              245: validity period = indefinite
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 * \note    None
 *
 */
local ULONG EncodeCompleteMoSms(
    BYTE*  pOut,
    BYTE* pOutLen,
    char* pDestNum,
    BYTE DestNumLen,
    char *pCallbackNum,
    BYTE CallbackNumLen,
    char* pMsgBody,
    BYTE MsgLen,
    BYTE MsgType,
    WORD MsgId,
    BYTE Priority,
    BYTE* RelVal )
{
    ULONG TeleserviceLen = 0, DestAddrLen = 0;
    ULONG CallbackAddrLen = 0, BearerDataLen = 0;
    ULONG MsgIDLen = 0, PriorityLen = 0;
    ULONG UserDataLen = 0, RelValidityLen = 0;
    ULONG BearerReplyOptionLen = 0, TotalLen = 0;
    BYTE  BearerMsgLen = 0;

    WORD bits = 0, remainder = 0;
    BYTE reserved = 0;

    BYTE* pbTemp;

    eQCWWAN_APP_ERR rCode = eQCWWAN_APP_ERR_FAILED;

    if( pDestNum == NULL ||
        pMsgBody == NULL ||
        MsgLen > MAX_SMS_MSG_LEN ||
        Priority > 6 )
    {
        return eQCWWAN_ERR_INVALID_ARG;
    }

    /* Teleservice Id  default LEN = 5 */
    TeleserviceLen = 5;
    BYTE* TeleServiceBuf = (BYTE*)malloc(TeleserviceLen);
    if( !TeleServiceBuf )
    {
        return eQCWWAN_APP_ERR_FAILED;
    }
    memset( TeleServiceBuf, 0, TeleserviceLen );

    rCode = EncodeTeleServiceHdr(TeleServiceBuf, &TeleserviceLen );
    if( (ULONG)rCode != (ULONG)eQCWWAN_ERR_NONE )
    {
        return rCode;
    }

    /* Bearer Reply Option default LEN = 3 */
    BearerReplyOptionLen = 3;
    BYTE* BearerReplyOptionBuf = (BYTE*)malloc(BearerReplyOptionLen);
    if( !BearerReplyOptionBuf )
    {
        return eQCWWAN_APP_ERR_FAILED;
    }
    memset(BearerReplyOptionBuf,0,BearerReplyOptionLen);

    rCode = EncodeBearerReplyOptionHdr( BearerReplyOptionBuf,
                                        &BearerReplyOptionLen );
    if( (ULONG)rCode != (ULONG)eQCWWAN_ERR_NONE )
    {
        return rCode;
    }
    /* Calculate Destination Addr Len */
    /* ParamID(8bits)+ParamLen(8bits)+NumMode(1bit)+DigitMode(1bit)
       +NumofDigits(8bits) */

    bits = 8+8+1+1+8+(4*DestNumLen);
    remainder = bits%8;
    if( remainder )
    {
        reserved =  8 - remainder;
    }

    bits = bits + reserved;
    DestAddrLen = bits/8;
    BYTE* DestAddrBuf = (BYTE*)malloc(DestAddrLen);

    if( !DestAddrBuf )
    {
        return eQCWWAN_APP_ERR_FAILED;
    }

    memset( DestAddrBuf, 0, DestAddrLen );

    rCode = EncodeDestAddrHdr( DestAddrBuf,
                               pDestNum,
                               DestNumLen,
                               &DestAddrLen,
                               reserved);
    if( (ULONG)rCode != (ULONG)eQCWWAN_ERR_NONE )
    {
        return rCode;
    }
    /* Calculate Callback Addr Len */
    BYTE* CallbackAddrBuf = NULL;
    if( CallbackNumLen > 0 )
    {
        /* ParamID(8bits)+ParamLen(8bits)+DigitMode(1bit)+NumofDigits(8bits)
           +(NumOfDigits*DTMF format len(4 bits) + reserve bits(if needed)*/
        bits = 8+8+1+8+(4*CallbackNumLen);
        remainder = bits%8;
        if( remainder )
        {
            reserved =  8 - remainder;
        }

        bits = bits + reserved;

        CallbackAddrLen = bits/8;
        CallbackAddrBuf = (BYTE*)malloc(CallbackAddrLen);

        if( !CallbackAddrBuf )
        {
            return eQCWWAN_APP_ERR_FAILED;
        }

        memset(CallbackAddrBuf,0,CallbackAddrLen);

        rCode = EncodeCallbackNumber(CallbackAddrBuf,
                                     pCallbackNum,
                                     CallbackNumLen,
                                     &CallbackAddrLen,
                                     reserved);
        if( (ULONG)rCode != (ULONG)eQCWWAN_ERR_NONE )
        {
            return rCode;
        }
    }
    /* MSG IDENTIFIER default LEN = 5 */
    MsgIDLen = 5;
    BYTE* MsgIDBuf = (BYTE*)malloc(MsgIDLen);

    if( !MsgIDBuf )
    {
        return eQCWWAN_APP_ERR_FAILED;
    }

    memset(MsgIDBuf,0,MsgIDLen);

    rCode = EncodeMsgIdenfier(MsgIDBuf,MsgType,MsgId,&MsgIDLen );
    if( (ULONG)rCode != (ULONG)eQCWWAN_ERR_NONE )
    {
        return rCode;
    }

    UserDataLen = 200;
    BYTE* UserDataBuf = (BYTE*)malloc(UserDataLen);

    if( !UserDataBuf )
    {
        return eQCWWAN_APP_ERR_FAILED;
    }

    memset( UserDataBuf, 0, UserDataLen );
    rCode = EncodeUserDataHdr(UserDataBuf,pMsgBody,MsgLen, &UserDataLen);
    if( (ULONG)rCode != (ULONG)eQCWWAN_ERR_NONE )
    {
        return rCode;
    }

    /* Priority default LEN = 3 */
    PriorityLen = 3;
    BYTE* PriorityBuf = (BYTE*)malloc(PriorityLen);

    if( !PriorityBuf )
    {
        /* LogCritical(_T("Failed to allocate memory") ); */
        return eQCWWAN_APP_ERR_FAILED;
    }

    memset(PriorityBuf,0,PriorityLen);

    rCode = EncodePriorityHdr(PriorityBuf,Priority,&PriorityLen);
    if( (ULONG)rCode != (ULONG)eQCWWAN_ERR_NONE )
    {
        return rCode;
    }

    /* Relative validity default LEN = 3 */

    RelValidityLen = 3;
    BYTE* RelValidityBuf = (BYTE*)malloc(RelValidityLen);

    if( !RelValidityBuf )
    {
        return eQCWWAN_APP_ERR_FAILED;
    }

    memset(RelValidityBuf,0,RelValidityLen);

    rCode = EncodeRelValidityHdr(RelValidityBuf,RelVal,&RelValidityLen);
    if( (ULONG)rCode != (ULONG)eQCWWAN_ERR_NONE )
    {
        return rCode;
    }
    /* BearerData default LEN = 2 */
    BearerDataLen = 2;
    BYTE* BearerDataBuf = (BYTE*)malloc(BearerDataLen);

    if( !BearerDataBuf )
    {
        return eQCWWAN_APP_ERR_FAILED;
    }

    memset(BearerDataBuf,0,BearerDataLen);

    BearerMsgLen = (BYTE)(MsgIDLen + UserDataLen + RelValidityLen +
                                            PriorityLen + CallbackAddrLen);
    rCode = EncodeBearerData(BearerDataBuf,BearerMsgLen,&BearerDataLen);
    if( (ULONG)rCode != (ULONG)eQCWWAN_ERR_NONE )
    {
        return rCode;
    }

    TotalLen = TeleserviceLen + DestAddrLen + BearerReplyOptionLen + MsgIDLen +
               UserDataLen + RelValidityLen + PriorityLen + BearerDataLen +
               CallbackAddrLen;

    if( pOut == NULL )
    {
        return eQCWWAN_APP_ERR_FAILED;
    }

    memset(pOut,0,TotalLen);

    /* Copy layer by layer */
    pbTemp = pOut;

    memcpy(pbTemp, TeleServiceBuf, TeleserviceLen );
    pbTemp += TeleserviceLen;
    free(TeleServiceBuf);
    memcpy(pbTemp, BearerReplyOptionBuf, BearerReplyOptionLen );
    pbTemp += BearerReplyOptionLen;
    free(BearerReplyOptionBuf);


    memcpy(pbTemp, DestAddrBuf, DestAddrLen );
    pbTemp += DestAddrLen;
    free(DestAddrBuf);

    memcpy(pbTemp, BearerDataBuf, BearerDataLen );
    pbTemp += BearerDataLen;
    free(BearerDataBuf);

    memcpy(pbTemp, MsgIDBuf, MsgIDLen );
    pbTemp += MsgIDLen;
    free(MsgIDBuf);

    memcpy(pbTemp, UserDataBuf, UserDataLen );
    pbTemp += UserDataLen;
    free(UserDataBuf);

    memcpy(pbTemp, RelValidityBuf, RelValidityLen );
    pbTemp += RelValidityLen;
    free(RelValidityBuf);
    memcpy(pbTemp, PriorityBuf, PriorityLen );
    pbTemp += PriorityLen;
    free(PriorityBuf);

    if(( CallbackAddrLen > 0 )&&( NULL != CallbackAddrBuf))
    {
        memcpy(pbTemp,CallbackAddrBuf,CallbackAddrLen);
        pbTemp += CallbackAddrLen;
        free(CallbackAddrBuf);
    }

    *pOutLen = TotalLen;
    pbTemp = pOut ;
    return rCode;
}

/*
 * This function can be used to encode an outgoing CDMA message.
 * the resulting message can be either stored on the device using
 * SaveSMS() or sent to the service center using SendSMS().
 * The caller must provide an text message to be encoded, a message
 * ID, a callback address and a destination address.  All other
 * information is optional for the caller to specify.
 * Default values will be used in the encoded when option parameter
 * are not given.
 *
 * \param  pMessageSize[IN/OUT]
 *         -Upon input, specifies the total number of bytes that the
 *          given pMessage buffer can hold (a buffer of length 240
 *          is recommended)Upon successful output, specifies the
 *          length of the constructed message placed in the pMessage
 *          buffer (in bytes)
 *
 * \param  pMessage[OUT]
 *         - The constructed raw message
 *
 * \param  messageId[IN]
 *         - The message reference number for this message.
 *           This value should be incremented for every message the host
 *           application sends
 *
 * \param  pDestAddr[IN]
 *         - Gives NULL-terminated ASCII String containing a destination
 *           address International number will be prepended with a '+'
 *           character
 *
 * \param  pCallbackAddr[IN]
 *         - Gives NULL-terminated ASCII String containing a callback
 *           address International number will be prepended with a '+'
 *           character
 *
 * \param  textMsgLength
 *         - Number of characters in the text message(excluding NULL)
 *
 * \param  pTextMsg[IN]
 *         - text message to be encoded
 *
 * \param  pPriority[IN](optional pointer)
 *         - Gives the priority of the outgoing message:
 *           0 - normal (default if NULL pointer is given)
 *           1 - interactive
 *           2 - urgent
 *           3 - emergency
 *           64 - 64 is decoded value for URGENT VZAM Support interactive
 *           128 - 128 is decoded value for URGENT VZAM Support urgent
 *           192 - 128 is decoded value for URGENT VZAM Support emergency
 *
 * \param  pEncodingAlphabet[IN/OUT](optional pointer)
 *         - Upon input, specifies the alphabet the text message
 *           should be encoded in
 *           0 - 8bit ASCII (not supported at this time)
 *           1 - IS91EP (not supported at this time)
 *           2 - 7bit ASCII (default if NULL pointer is given)
 *           3 - IA5 (not supported at this time)
 *           4 - unicode (not supported at this time)
 *           5 - shift JIS (not supported at this time)
 *           6 - korean (not supported at this time)
 *           7 - latin hebrew (not supported at this time)
 *           8 - latin (not supported at this time)
 *           9 - GSM 7 bit default
 *           Upon successful output, specifies the alphabet
 *           used to encode the message.
 *
 * \param  pRelValidity[IN](optional pointer)
 *         - Gives the relative validity period of the outgoing message
 *           per per 4.5.6-1 of spec
 *           0 - Set Relative validity to 11
 *           1 - Set Relative validity to 71
 *           2 - Set Relative validity to 167
 *           3 - Set Relative validity to 169
 *           4 - Set Relative validity to 171
 *               Values have the following meanings:
 *           0 to 143: validity period = (value + 1)* 5 minutes
 *           144 to 167: validity period = 12 hours + (value - 143)*30
 *                       minutes
 *           168 to 196: validity period = (value - 166) * 1 day
 *           197 to 244: validity period = (value - 192) * 1 week
 *           245: validity period = indefinite
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 * \note   Currently this API can only encode 7bit ASCII messages.
 *
 */
ULONG SwiCDMAEncodeMOTextMsg(
    BYTE    *pMessageSize,
    BYTE    *pMessage,
    BYTE    messageId,
    CHAR    *pDestAddr,
    CHAR    *pCallbackAddr,
    ULONG   textMsgLength,
    wchar_t *pTextMsg,
    BYTE    *pPriority,
    BYTE    *pEncodingAlphabet,
    BYTE    *pRelValidity )
{
    BYTE destAddrLen = 0;
    BYTE callbackAddrLen = 0;
    BYTE priority = 0;
    CHAR asciiTextMessage[240];

    if(pEncodingAlphabet)
    {
        if( *pEncodingAlphabet != QCWWAN_SMS_DATAENCODE_7bitAscii )
        {
            return eQCWWAN_APP_ERR_IMPOSSIBLE_ENCODING;
        }
    }

    if(pDestAddr)
    {
        destAddrLen = (BYTE)strlen(pDestAddr);
    }
    if(pCallbackAddr)
    {
        callbackAddrLen = (BYTE)strlen(pCallbackAddr);
    }
    if(pPriority)
    {
        priority = *pPriority;
    }

    SW_TtoA (asciiTextMessage, pTextMsg, textMsgLength);
    switch( EncodeCompleteMoSms( pMessage,
                                 pMessageSize,
                                 pDestAddr,
                                 destAddrLen,
                                 pCallbackAddr,
                                 callbackAddrLen,
                                 asciiTextMessage,
                                 textMsgLength,
                                 2, /* submit */
                                 messageId,
                                 (BYTE)priority,
                                 pRelValidity))

    {
        case eQCWWAN_ERR_NONE:
            return (eQCWWAN_ERR_NONE);
            break;

        case eQCWWAN_APP_ERR_FAILED:
            return (eQCWWAN_ERR_GENERAL);
            break;

        case eQCWWAN_ERR_INVALID_ARG:
            return (eQCWWAN_ERR_INVALID_ARG);
            break;

        case eQCWWAN_APP_ERR_BUFFER_SZ:
            return (eQCWWAN_ERR_BUFFER_SZ);
            break;

        default:
            return (eQCWWAN_ERR_GENERAL);
    }
}

/****************************************************************************
*                       CDMA PDU Decoding  Functions
*****************************************************************************/
/*
 * Unpacks bits to Byte
 *
 * \param  src[OUT] - Pointer to source string
 *         pos[IN]   - Bit position
 *         len[IN]   - Length in which bits will be grouped
 *
 *
 * \return Result of unpacking
 *
 * \note  None
 *
 */
local BYTE UnpackBitsToAByte( BYTE *src,  int pos, int len)
{
    BYTE result = 0;
    int rshift = 0;

    src += pos/8;
    pos %= 8;

    rshift = MAX( 8 - (pos + len), 0);

    if ( rshift > 0 )
    {
        result = (BYTE)MASK_AND_SHIFT(len, pos, rshift, *src);
    }
    else
    {
        result = (BYTE)MASK(8-pos, pos, *src);
        src++;
        len -= 8 - pos;

        if ( len > 0 )
        {
          result = ( result<<len ) | (*src >> (8-len));
        }
    }

   return result;
}

/*
 * Unpacks BearerData
 *
 * \param  pBearerDataBuf[IN]
 *         - Pointer bearer Data bufer
 *
 * \param  BearerDataLen[IN]
 *         - BearerDataLen
 *
 * \param  pMsgId[OUT]
 *         - Pointer to Fecthed message ID
 *
 * \param  pYr[OUT]
 *         - Pointer to year field in fetched timestamp
 *
 * \param  pMon[OUT]
 *         - Pointer to month field in fetched timestamp
 *
 * \param  pDay[OUT]
 *         - Pointer to day field in fetched timestamp
 *
 * \param  pHr[OUT]
 *         - Pointer to hours field in fetched timestamp
 *
 * \param  pMin[OUT]
 *         - Pointer to minute field in fetched timestampa
 *
 * \param  pSec[OUT]
 *         - Pointer to second field in fetched timestam
 *
 * \param  pUserDataBuf[OUT]
 *         - Pointer user Data bufer
 *
 * \param  pUserDataLen[OUT]
 *         - Pointer to length of user data buffera
 *
 * \param  pPriority[OUT]
 *         - Pointer to unpacked priority of message
 *
 * \param  pCallbackBuf[OUT]
 *         - pointer to callback buffer
 *
 * \param  pCallbackLen[OUT]
 *         - Pointer to callback length
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 * \note  None
 *
 */
local ULONG DecodeBearerData(
    BYTE *pBearerDataBuf,
    BYTE BearerDataLen,
    WORD *pMsgId,
    BYTE *pYr,
    BYTE *pMon,
    BYTE *pDay,
    BYTE *pHr,
    BYTE *pMin,
    BYTE *pSec,
    BYTE **pUserDataBuf,
    BYTE *pUserDataLen,
    BYTE *pPriority,
    BYTE **pCallbackBuf,
    BYTE *pCallbackLen )
{
    BYTE* pMsg = pBearerDataBuf;
    BYTE* pEndMsg = pMsg + BearerDataLen;
    eQCWWAN_APP_ERR rc = eQCWWAN_ERR_NONE;

    BYTE b1, b11, b2, b21, b22, b3, b31;
    WORD SkipLen = 0;

    while ( pMsg < pEndMsg )
    {
        switch(*pMsg)
        {
        case MSG_SUB_IDENTIFIER_ID:
            if( *(++pMsg) != MSG_SUB_IDENTIFIER_HDR_LEN )
            {
                rc = eQCWWAN_APP_ERR_FAILED;
            }
            BYTE MsgType, temp;
            b1 = *(++pMsg);
            b2 = *(++pMsg);
            b3 = *(++pMsg);

            /* Mask first 4 bits of b1 variable,first 4 bits is Message type */
            temp = b1 & 0xF0;

            /* Right shift to convert 4 MSBs to LSBs ==>
               11110000 becomes 00001111 */
            MsgType = temp >> 4;

            if( MsgType != 1 &&
                MsgType != 2 &&
                MsgType != 4 &&
                MsgType != 5 &&
                MsgType != 6 )
            {
                rc = eQCWWAN_APP_ERR_FAILED;
            }
            b11 = b1 & 0x0F;  /* Mask Last 4 bits of first byte */
            b11 = b11<<4;     /* Left shift to convert 4 LSB to MSBs
                                 ==> 00001111 becomes 11110000 */

            b31 = b3 & 0xF0;  /* Mask first 4 bits of 3rd byte */
            b31 = b31 >> 4;   /* Right shift to convert 4 MSBs to LSBs
                                 ==> 33330000 becomes 00003333 */

            b21 = b2 & 0xF0;  /* Mask first 4 bits of 2nd byte */
            b21 = b21 >> 4;   /* Right shift to convert 4 MSBs to LSBs
                                 ==> 22220000 becomes 00002222 */

            b22 = b2 & 0x0F;  /* Mask Last 4 bits of 2nd byte */
            b22 = b22 << 4;   /* Left shift to convert 4 LSB to MSBs
                                 ==> 00002222 becomes 22220000 */

            *pMsgId = b11 + b21;
            *pMsgId <<= 8;
            *pMsgId += (b22 + b31);
            break;

        case PRIORIY_SUB_IDENTIFIER:
            if( *(++pMsg) != PRIORIY_SUB_IDENTIFIER_HDR_LEN )
            {
                rc = eQCWWAN_APP_ERR_FAILED;
            }
            *pPriority = *(++pMsg);

            break;

        case USER_DATA_SUB_IDENTIFIER:
            *pUserDataLen = *(++pMsg);
            *pUserDataBuf = (BYTE*) malloc(*pUserDataLen);
            if(*pUserDataBuf == NULL )
            {
                rc = eQCWWAN_APP_ERR_FAILED;
            }
            /* increment ptr to point to data */
            ++pMsg;
            memcpy((void*)*pUserDataBuf,(void*)pMsg,(*pUserDataLen));
            pMsg = pMsg + ((*pUserDataLen) - 1);
            break;

        case MSG_TIMESTAMP_SUB_INDETIFIER:
            if( *(++pMsg) != MSG_TIMESTAMP_SUB_INDETIFIER_HDR_LEN )
            {
                rc = eQCWWAN_APP_ERR_FAILED;
            }
            /* Upper layer expect year to be in decimal format */
            *pYr  = *(++pMsg);
            *pMon = *(++pMsg);
            *pDay = *(++pMsg);
            *pHr  = *(++pMsg);
            *pMin = *(++pMsg);
            *pSec = *(++pMsg);
            break;

        case LANGUAGE_INDICATOR_SUB_INDETIFIER:
            if( *(++pMsg) != LANGUAGE_INDICATOR_SUB_INDETIFIER_HDR_LEN )
            {
                rc = eQCWWAN_APP_ERR_FAILED;
            }
            pMsg = pMsg + LANGUAGE_INDICATOR_SUB_INDETIFIER_HDR_LEN;
            break;

        case CALLBACK_NUM_SUB_IDENTIFIER:
            *pCallbackLen = *(++pMsg);
            *pCallbackBuf = (BYTE*) malloc(*pCallbackLen);
            if(*pCallbackBuf == NULL )
            {
                rc = eQCWWAN_APP_ERR_FAILED;
            }
            /* increment ptr to point to data */
            ++pMsg;
            memcpy((void*)*pCallbackBuf,(void*)pMsg,*pCallbackLen);
            pMsg = pMsg + (*pCallbackLen - 1);
            break;

        case USER_RESP_CODE_SUB_IDENTIFIER:
        case ABS_VAL_PERIOD_SUB_IDENTIFIER:

            break;


        case REL_VAL_PERIOD_SUB_IDENTIFIER:
        case DEF_DELIVERY_TIME_ABS_SUB_IDENTIFIER:
        case DEF_DELIVERY_TIME_REL_SUB_IDENTIFIER:
        case PRIVACY_SUB_IDENTIFIER:
        case REPLY_OPTION_SUB_IDENTIFIER:
        case NUM_OF_MSGS_SUB_IDENTIFIER:
        case ALERT_MSG_DELIVERY_SUB_IDENTIFIER:
        case MSG_DISP_MODE_SUB_IDENTIFIER:
        case MULTI_ENCODING_USER_DATA_SUB_IDENTIFIER:
        case MSG_DEPOSIT_INDEX_SUB_IDENTIFIER:
        case SERVICE_CAT_PROGRAM_DATA_SUB_IDENTIFIER:
        case SERVICE_CAT_PROGRAM_RESULT_SUB_IDENTIFIER:
        case MSG_STATUS_SUB_IDENTIFIER:
            SkipLen = *(++pMsg);
            pMsg = pMsg + SkipLen;
            break;
        default:
            rc = eQCWWAN_APP_ERR_FAILED;
            break;
        }

        if( (ULONG)rc != (ULONG)eQCWWAN_ERR_NONE )
        {
            break;
        }
        else
        {
            pMsg++;
        }
    }
    return rc;
}

/*
 * Splits CDMA PDU message
 *
 * \param  pMTSms[IN]              - Pointer bearer Data bufer
 *
 * \param  MsgLen[IN]              - BearerDataLen
 *
 * \param  TeleServiceIdValue[OUT] - Indicates teleservice ID
 *
 * \param  pOrigAddrBuf[OUT]       - Points to origin address buffer
 *
 * \param  pDestAddrBuf[OUT]       - Points to Destination address buffer
 *
 * \param  BearerReplySeq[OUT]     - Points to bearer reply sequence
 *
 * \param  pBearerDataBuf[OUT]     - Points to bearer data buffer
 *
 * \param  BearerDataLen[OUT]      - Points to bearer data length
 *
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 * \note   None
 *
 */
local ULONG SplitParamMTSms(
    BYTE *pMTSms,
    ULONG MsgLen,
    WORD *TeleServiceIdValue,
    BYTE **pOrigAddrBuf,
    BYTE **pDestAddrBuf,
    BYTE *BearerReplySeq,
    BYTE **pBearerDataBuf,
    BYTE *BearerDataLen )
{
    BYTE* pMsg = pMTSms;
    BYTE* pEndMsg = pMsg + MsgLen;
    eQCWWAN_APP_ERR rc = eQCWWAN_ERR_NONE;

    *pDestAddrBuf = NULL;
    *pOrigAddrBuf = NULL;
    *pBearerDataBuf = NULL;

    BYTE OriginAddrLen;
    BYTE DestAddrLen;
    WORD SkipLen = 0;

    if( *pMsg != POINT_TO_POINT_MSG )
    {
        return eQCWWAN_APP_ERR_NOT_SUP;
    }

    pMsg++;

    while ( pMsg < pEndMsg )
    {
        switch(*pMsg)
        {
        case TELESERVICE_IDENTIFIER:
            if( *(++pMsg) != TELESERVICE_HDR_LEN )
            {
                rc = eQCWWAN_APP_ERR_FAILED;
            }
            *TeleServiceIdValue = *(++pMsg);
            *TeleServiceIdValue <<= 8;
            *TeleServiceIdValue = *(++pMsg);
            break;

        case BEARER_REPLY_IDENTIFIER:
            if( *(++pMsg) != BEARER_REPLY_HDR_LEN )
            {
                rc = eQCWWAN_APP_ERR_FAILED;
            }
            *BearerReplySeq = *(++pMsg);
            break;

        case ORIGADDR_IDENTIFIER:
            OriginAddrLen = *(++pMsg);
            if( OriginAddrLen != ORIGADDR_HDR_LEN )
            {
            }
            *pOrigAddrBuf = (BYTE*) malloc(OriginAddrLen);
            if(*pOrigAddrBuf == NULL )
            {
                rc = eQCWWAN_APP_ERR_FAILED;
            }
            /* increment ptr to point to data */
            ++pMsg;
            memcpy((void*)*pOrigAddrBuf,(void*)pMsg,OriginAddrLen);
            pMsg = pMsg + (OriginAddrLen - 1);
            break;

        case DESTADDR_IDENTIFIER:
            DestAddrLen = *(++pMsg);
            if( DestAddrLen != DESTADDR_HDR_LEN )
            {
            }
            *pDestAddrBuf = (BYTE*) malloc(DestAddrLen);
            if(*pDestAddrBuf == NULL )
            {
                rc = eQCWWAN_APP_ERR_FAILED;
            }
            /* increment ptr to point to data */
            ++pMsg;
            memcpy((void*)*pDestAddrBuf,(void*)pMsg,DestAddrLen);
            pMsg = pMsg + (DestAddrLen - 1);
            break;

        case BEARER_DATA_IDENTIFIER:
            *BearerDataLen = *(++pMsg);
            *pBearerDataBuf = (BYTE*) malloc(*BearerDataLen);
            if(*pBearerDataBuf == NULL )
            {
                rc = eQCWWAN_APP_ERR_FAILED;
            }
            /* increment ptr to point to data */
            ++pMsg;
            memcpy((void*)*pBearerDataBuf,(void*)pMsg,(*BearerDataLen));
            pMsg = pMsg + ((*BearerDataLen) - 1);
            break;

        case SERVICE_CATEGORY_IDENIFIER:
        case ORIGSUBADDR_IDENTIFIER:
            SkipLen = *(++pMsg);
            pMsg = pMsg + SkipLen;
            break;

        default:
            rc = eQCWWAN_APP_ERR_FAILED;
            break;
        }
        if( (ULONG)rc != (ULONG)eQCWWAN_ERR_NONE )
        {
            break;
        }
        else
        {
            pMsg++;
        }
    }

    return rc;
}

/*
 * Unpacks origin Address from provided buffer
 *
 * \param  pOrigAddrBuf[IN]
 *         - Pointer to origin address buffer
 *
 * \param  pDigitMode[OUT]
 *         - Pointer to digit mode data
 *
 * \param  pNumMode[OUT]
 *         - Pointer to number mode data
 *
 * \param  szOriginAddr[OUT]
 *         - points to origin address
 *
 * \param  AddrLen[OUT]
 *         - Points to origin address length
 *
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 * \note   None
 *
 */
local ULONG DecodeOriginAddr(
    BYTE *pOrigAddrBuf,
    BYTE *pDigitMode,
    BYTE *pNumMode,
    WORD *szOriginAddr,
    BYTE *AddrLen )
{
    int bit_pos = 0;
    BYTE Numbers[50];
    CHAR szNumbers[50];
    BYTE rcvdAddrLen;

    *pDigitMode = UnpackBitsToAByte( pOrigAddrBuf,  bit_pos, 1);
    bit_pos += 1;

    *pNumMode = UnpackBitsToAByte( pOrigAddrBuf,  bit_pos, 1);
    bit_pos += 1;

    if( *pDigitMode != 0 || *pNumMode != 0 )
    {
        return eQCWWAN_APP_ERR_NOT_SUP;
    }

    rcvdAddrLen = UnpackBitsToAByte( pOrigAddrBuf,  bit_pos, 8);
    bit_pos += 8;

    *AddrLen = rcvdAddrLen;  /* Assign the length */
    int i;
    for( i = 0; i < *AddrLen; i++ )
    {
        Numbers[i] = UnpackBitsToAByte( pOrigAddrBuf,  bit_pos, 4);
        if( Numbers[i] == 10 )
        {
            Numbers[i] = 0;
        }

        szNumbers[i] = Num2Char( Numbers[i] );
        bit_pos += 4;
    }

    SW_AtoT((wchar_t*)szOriginAddr,szNumbers,*AddrLen);

    return eQCWWAN_ERR_NONE;
}

/*
 * Unpacks user data from provided buffer
 *
 * \param  pUserDataBuf[IN]
 *         - Pointer to origin address buffer
 *
 * \param  UserDataLen[IN]
 *         - Pointer to digit mode data
 *
 * \param  pMsgEncoding[OUT]
 *         - Pointer to message encoding type
 *
 * \param  pMsgLen[OUT]
 *         - points to unpacked message length
 *
 * \param  pMsgBody[OUT]
 *         - points to unpacked message
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 * \note   None
 *
 */
local ULONG DecodeUserData(
    BYTE *pUserDataBuf,
    BYTE UserDataLen,
    BYTE *pMsgEncoding,
    BYTE *pMsgLen,
    BYTE **pMsgBody )
{
    int bit_pos = 0;
    BYTE* ptr;

    UNUSEDPARAM( UserDataLen );

    *pMsgEncoding = UnpackBitsToAByte( pUserDataBuf,  bit_pos, 5);
    bit_pos += 5;

    if( *pMsgEncoding != USER_DATA_MSG_ENCODING_7BIT_ASCII )
    {
        return eQCWWAN_APP_ERR_NOT_SUP;
    }

    *pMsgLen = UnpackBitsToAByte( pUserDataBuf,  bit_pos, 8);
    bit_pos += 8;

    *pMsgBody = (BYTE*)malloc(*pMsgLen);
    if( *pMsgBody == NULL )
    {
        return eQCWWAN_APP_ERR_FAILED;
    }
    ptr = *pMsgBody;
    int i;
    for( i = 0; i < *pMsgLen; i++ )
    {
        *(ptr+i) = UnpackBitsToAByte( pUserDataBuf,  bit_pos, 7);
        bit_pos += 7;
    }

    return eQCWWAN_ERR_NONE;
}

/*
 * Unpacks CDMA PDU whcih is read using SLQSGetSMS API
 *
 * \param  pEncodedSms[IN]
 *         - Pointer Message read by SLQSGetSMSa
 *
 * \param  UserDataLen[IN]
 *         - Length of message
 *
 * \param  pSms[OUT]
 *         - Structure to be filled with unpacked data
 *
 * \return  eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 * \note   None
 *
 */
local ULONG DecodeCompleteMTSms(
    BYTE  *pEncodedSms,
    ULONG MsgLen,
    QCWWAN_STRUCT_CDMA_SMS_RetrieveSms *pSms )
{
    WORD TeleServiceIdValue = 0;
    BYTE* pDestAddrBuf = NULL;
    BYTE* pOriginAddrBuf =NULL;

    /*Bearer Reply field is 6 bits so it cannot be more than 63 */
    BYTE BearerReplySequence = 255;
    BYTE* pBearerDataBuf = NULL;
    BYTE BearerDataLen;

    BYTE DigitMode, NumMode;

    WORD MsgId;
    BYTE Yr = 200, Mon = 200, Day, Hr, Min, Sec;
    BYTE *pUserDataBuf,UserDataLen, *pCallbackBuf, CallbackLen,Priority = 200;

    BYTE MsgEncoding, DataMsgLen;
    BYTE* pMsgBody;

    eQCWWAN_APP_ERR rc = eQCWWAN_APP_ERR_FAILED;

    rc = SplitParamMTSms(pEncodedSms,
                         MsgLen,
                         &TeleServiceIdValue,
                         &pOriginAddrBuf,
                         &pDestAddrBuf,
                         &BearerReplySequence,
                         &pBearerDataBuf,
                         &BearerDataLen );


    if( (ULONG)rc != (ULONG)eQCWWAN_ERR_NONE )
    {
        if(pBearerDataBuf)
        {
            free(pBearerDataBuf);
        }
        if(pOriginAddrBuf)
        {
            free(pOriginAddrBuf);
        }
        if(pDestAddrBuf)
        {
            free(pDestAddrBuf);
        }

        return rc;
    }

    pSms->sHeader.sizeStruct = sizeof(QCWWAN_STRUCT_SMS_CdmaHeader);

    /* Lets say there are no params at the beginning */
    pSms->sHeader.eParamMask = QCWWAN_SMS_PARAMASK_NO_PARAMS;

    /* If Berer reply was received its value would be less than 255 */
    if( BearerReplySequence != 255 )
    {
        pSms->sHeader.eParamMask = (QCWWAN_TYPE_SMS_ParamMask )
               (pSms->sHeader.eParamMask | QCWWAN_SMS_PARAMASK_BEARER_REPLY );
        pSms->sHeader.tlAckReq = BearerReplySequence >> 2;
    }

    /* We only support Point to Point messages for now
       If its not P2P this function would have returned
       error before reaching here */
    pSms->sHeader.eMsgCategory = QCWWAN_SMS_MSGCAT_POINT_TO_POINT;
    pSms->sHeader.eTeleservice = QCWWAN_SMS_TELESRV_CELL_MESSAGING;

    /*Don't know anything about this fields yet
    pSms->sHeader.serviceCategory*/

    pSms->sHeader.eMsgIdType = QCWWAN_SMS_MSGIDTYPE_DELIVER;

    /* Only proceed if PTR is NOT NULL */
    if(pOriginAddrBuf)
    {

        rc = DecodeOriginAddr( pOriginAddrBuf,
                               &DigitMode,
                               &NumMode,
                               pSms->sHeader.szAddress,
                               &(pSms->sHeader.addrLen) );

        if( (ULONG)rc != (ULONG)eQCWWAN_ERR_NONE )
        {
            if(pBearerDataBuf)
            {
                free(pBearerDataBuf);
            }
            if(pOriginAddrBuf)
            {
                free(pOriginAddrBuf);
            }

            return rc;
        }

        if( pOriginAddrBuf )
        {
            pSms->sHeader.eParamMask = (QCWWAN_TYPE_SMS_ParamMask )
                  (pSms->sHeader.eParamMask | QCWWAN_SMS_PARAMASK_ADDRESS);
        }

        pSms->sHeader.addrDigitMode = DigitMode;
        pSms->sHeader.addrNumMode = NumMode;
        pSms->sHeader.addrNumType = 0;
        pSms->sHeader.addrNumPlan = 0;

        if(pOriginAddrBuf != NULL )
        {
            free(pOriginAddrBuf);
        }
    }
    else if(pDestAddrBuf)/* if we want to retrieve a 'Draft' msg */
    {

        rc = DecodeOriginAddr( pDestAddrBuf,
                              &DigitMode,
                              &NumMode,
                              pSms->sHeader.szAddress,
                              &(pSms->sHeader.addrLen) );

        if( (ULONG)rc != (ULONG)eQCWWAN_ERR_NONE )
        {
            if(pBearerDataBuf)
            {
                free(pBearerDataBuf);
            }
            if(pDestAddrBuf)
            {
                free(pDestAddrBuf);
            }

            return rc;
        }

        if( pDestAddrBuf )
        {
            pSms->sHeader.eParamMask = (QCWWAN_TYPE_SMS_ParamMask )
                 (pSms->sHeader.eParamMask | QCWWAN_SMS_PARAMASK_ADDRESS);
        }

        pSms->sHeader.addrDigitMode = DigitMode;
        pSms->sHeader.addrNumMode = NumMode;
        pSms->sHeader.addrNumType = 0;
        pSms->sHeader.addrNumPlan = 0;

        if( pDestAddrBuf != NULL )
        {
            free(pDestAddrBuf);
        }
    }
    else
    {
        return eQCWWAN_APP_ERR_UNEXPECTED_RESP;
    }

    /* Only proceed if PTR is NOT NULL */
    if( pBearerDataBuf )
    {
        pUserDataBuf = NULL;
        pCallbackBuf = NULL;

        rc = DecodeBearerData( pBearerDataBuf,
                               BearerDataLen,
                               &MsgId,
                               &Yr,
                               &Mon,
                               &Day,
                               &Hr,
                               &Min,
                               &Sec,
                               &pUserDataBuf,
                               &UserDataLen,
                               &Priority,
                               &pCallbackBuf,
                               &CallbackLen );

        if( (ULONG)rc != (ULONG)eQCWWAN_ERR_NONE )
        {
            if(pBearerDataBuf)
            {
                free(pBearerDataBuf);
            }
            if(pOriginAddrBuf)
            {
                free(pOriginAddrBuf);
            }
            if( pUserDataBuf )
            {
                free(pUserDataBuf);
            }

            return rc;
        }

        if( Yr != 200 && Mon != 200 )
        {
            pSms->sHeader.eParamMask = (QCWWAN_TYPE_SMS_ParamMask )
                (pSms->sHeader.eParamMask | QCWWAN_SMS_PARAMASK_MC_TIME_STAMP);
        }

        if( Priority != 200 )
        {
            pSms->sHeader.eParamMask = (QCWWAN_TYPE_SMS_ParamMask )
                   (pSms->sHeader.eParamMask | QCWWAN_SMS_PARAMASK_PRIORITY );
        }

        if( pUserDataBuf )
        {
            pSms->sHeader.eParamMask = (QCWWAN_TYPE_SMS_ParamMask )
                  (pSms->sHeader.eParamMask |QCWWAN_SMS_PARAMASK_USER_DATA );
        }

        pSms->sHeader.ePriority = (QCWWAN_TYPE_SMS_Priority)Priority;
        pSms->nSMSId = MsgId;
        pSms->sHeader.ts_yr = Yr;
        pSms->sHeader.ts_mon = Mon;
        pSms->sHeader.ts_day = Day;
        pSms->sHeader.ts_hrs = Hr;
        pSms->sHeader.ts_min = Min;
        pSms->sHeader.ts_sec = Sec;

        if(pBearerDataBuf != NULL )
        {
            free(pBearerDataBuf);
        }

        /* Only proceed if PTR is NOT NULL */
        if( pUserDataBuf )
        {
            rc = DecodeUserData( pUserDataBuf,
                                 UserDataLen,
                                 &MsgEncoding,
                                 &DataMsgLen,
                                 &pMsgBody );

            if( (ULONG)rc != (ULONG)eQCWWAN_ERR_NONE )
            {
                if(pBearerDataBuf)
                {
                    free(pBearerDataBuf);
                }
                if(pOriginAddrBuf)
                {
                    free(pOriginAddrBuf);
                }
                if( pUserDataBuf )
                {
                    free(pUserDataBuf);
                }
                if( pUserDataBuf )
                {
                    free(pUserDataBuf);
                }

                return rc;
            }
            pSms->sHeader.dataEncoding = QCWWAN_SMS_DATAENCODE_7bitAscii;
            pSms->sHeader.eMsgDispMode = QCWWAN_SMS_MSGDISPMODE_DEFAULT;

            /* Copy the pointer to message body, application
               will the free the memory */
            pSms->pMessage = pMsgBody;
            pSms->sizeBuffer = DataMsgLen;

            if(pUserDataBuf != NULL )
            {
                free(pUserDataBuf);
            }
        }

        if( pCallbackBuf )
        {
            if( (ULONG)rc != (ULONG)eQCWWAN_ERR_NONE )
            {
                if(pBearerDataBuf)
                {
                    free(pBearerDataBuf);
                }
                if(pOriginAddrBuf)
                {
                    free(pOriginAddrBuf);
                }
                if( pUserDataBuf )
                {
                    free(pUserDataBuf);
                }
                if( pCallbackBuf )
                {
                    free(pCallbackBuf);
                }

                return rc;
            }

            pSms->sHeader.eParamMask = (QCWWAN_TYPE_SMS_ParamMask )
                   (pSms->sHeader.eParamMask | QCWWAN_SMS_PARAMASK_CALL_BACK);
        }
    }
    else
    {
        return eQCWWAN_ERR_SWISMS_BEARER_DATA_NOT_FOUND;

    }
    return eQCWWAN_ERR_NONE;
}

/*
 * This function can be used to decode a received CDMA SMS message
 * read off the device using the GetSMS() API. The text message will
 * be returned in text along with related message information.
 *
 * \param  messageLength[IN]
 *         - Length of the message to be decoded in bytes
 *
 * \param  pMessage[IN]
 *         - Message read off the device via GetSMS
 *
 * \param  pSenderAddrLength[IN/OUT]
 *         - Upon input, indicates the maximum number of ASCII
 *           characters (including NULL termination) that the
 *           pSenderAddr buffer can accommodate.  Note that a
 *           length of 14 is reasonable.Upon successful output,
 *           returns the length of originating address string
 *          (including the NULL termination)
 *
 * \param  pSenderAddr[OUT]
 *         - Returns NULL-terminated ASCII String containing the
 *           originating address International number will be
 *           prepended with a '+'character
 *
 * \param  pTextMsgLength[IN/OUT]
 *         - Upon input, specifies the number of UCS2 characters the
 *           given text message buffer can accommodate.Upon successful
 *           output, returns the number of UCS2 characters returns in
 *           the given text messagebuffer(including NULL-terminator)
 *
 * \param  pTextMsg[OUT]
 *         - Returns the text message as  NULL-terminated UCS2 string
 *
 * \param  pPriority[OUT] (optional pointer)
 *         - Returns the priority setting of the message
 *           0x00 - normal
 *           0x01 - interactive
 *           0x02 - urgent
 *           0x03 - emergency
 *           0xFF - unavailable setting
 *
 * \param  pPrivacy[OUT](optional pointer)
 *         - Returns the privacy setting of the message
 *           0x00 - not restricted
 *           0x01 - restricted
 *           0x02 - confidential
 *           0x03 - secret
 *           0xFF - unavailable setting
 *
 * \param  pLanguage[OUT] (optional pointer)
 *         - Returns the language setting of the message
 *           0x00 - unspecified
 *           0x01 - english
 *           0x02 - french
 *           0x03 - spanish
 *           0x04 - japanese
 *           0x05 - korean
 *           0x06 - chinese
 *           0x07 - hebrew
 *           0xFF - unavailable setting
 *
 * \param  mcTimeStamp[8][OUT] (optional pointer)
 *         - Returns the message center timestamp which takes the form:
 *           YYMMDDHHMMSSTZ where
 *           YY - year
 *           MM - month
 *           DD - day
 *           HH - hour
 *           MM - minute
 *           SS - second
 *           TZ - timezone
 *           All values are in decimal. Timezone is in relation to GMT,
 *           one unit is equal to 15 minutes and MSB indicates a
 *           negative value.If this information is unavailable for
 *           message then this field will be filled with 0xFF
 *
 * \param  absoluteValidity[8[OUT] (optional pointer)
 *         - Returns the absolute validity period setting for this
 *           message.Thisfield takes the same form as mcTimeStamp
 *
 * \param  pRelativeValidity[OUT] (optional pointer)
 *         - Returns the relative validity period.Values have the
 *           following meanings:
 *           0 to 143: validity period =(value + 1)* 5 minutes
 *           144 to 167: validity period =12 hours+(value - 143)*30
 *           minutes
 *           168 to 196: validity period = (value - 166) * 1 day
 *           197 to 244: validity period = (value - 192) * 1 week
 *           245: validity period = indefinite
 *           246: validity period = immediate
 *           247: validity period = valid until mobile becomes inactive
 *           248: validity period = valid until registration area
 *                changes
 *           249 to 254: reserved
 *           255: unavailable information
 *
 * \param  pDisplayMode[OUT] (optional pointer)
 *         - Returns the display mode parameter
 *           0x00 - immediate display
 *           0x01 - mobile default setting
 *           0x02 - user invoked
 *           0x03 - reserved
 *           0xFF - unavailable parameter
 *
 * \param  pUserAcknowledgementReq[OUT] (optional pointer)
 *         - Returns the user (manual) acknowledgment request parameter
 *           TRUE means the user is requested to manually acknowledge
 *           the delivery of the message FALSE means no such user
 *           acknowledgement is requested
 *
 * \param  pReadAcknowledgementReq[OUT] (optional pointer)
 *         - Returns the read acknowledgement request parameter
 *           TRUE means acknowledgment of the message being viewed is
 *           requested FALSE means no such read acknowledgement is
 *           requested
 *
 * \param pAlertPriority[OUT] (optional pointer)
 *        - Returns the alerting parameter setting
 *          0x00 - use default alert
 *          0x01 - use low priority alert
 *          0x02 - use medium priority alert
 *          0x03 - use high priority alert
 *          0xFF - unavailable parameter
 *
 * \param  pCallbkAddrLength[OUT] (optional pointer)
 *         - returns the length of Callback
 *           address string (including the NULL termination)
 *
 * \param  pCallbkAddr[OUT]
 *         - returns NULL-terminated ASCII String containing callback
 *           address String containing the Call Back number with a 32
 *           maximum characters.
 *
 *
 * \return eQCWWAN_ERR_NONE on success, eQCWWAN_xxx error value otherwise
 *
 * \note  None
 *
 */
ULONG SwiCDMADecodeMTTextMsg(
    ULONG  messageLength,
    BYTE*  pMessage,
    ULONG* pMessageID,
    BYTE*  pSenderAddrLength,
    CHAR*  pSenderAddr,
    BYTE*  pTextMsgLength,
    WORD*  pTextMsg,
    BYTE*  pPriority,
    BYTE*  pPrivacy,
    BYTE*  pLanguage,
    BYTE   mcTimeStamp[8],
    BYTE   absoluteValidity[8],
    BYTE*  pRelativeValidity,
    BYTE*  pDisplayMode,
    BOOL*  pUserAcknowledgementReq,
    BOOL*  pReadAcknowledgementReq,
    BYTE*  pAlertPriority,
    BYTE*  pCallbkAddrLength,
    CHAR*  pCallbkAddr )
{
    ULONG ret;
    QCWWAN_STRUCT_CDMA_SMS_RetrieveSms Sms;

    ret = DecodeCompleteMTSms(pMessage, messageLength, &Sms);

    if(ret == 0)
    {
        if(pMessageID)
        {
            *pMessageID = Sms.nSMSId;
        }
        if(pSenderAddrLength && pSenderAddr)
        {
            if(Sms.sHeader.eParamMask & QCWWAN_SMS_PARAMASK_ADDRESS)
            {
                if(*pSenderAddrLength < Sms.sHeader.addrLen)
                {
                     *pSenderAddrLength = Sms.sHeader.addrLen + 1;
                     return(eQCWWAN_ERR_BUFFER_SZ);
                }
                memset(pSenderAddr, 0, *pSenderAddrLength);
                *pSenderAddrLength = Sms.sHeader.addrLen + 1;
                strncpy( pSenderAddr, (CHAR*)Sms.sHeader.szAddress,
                         Sms.sHeader.addrLen);
            }
            else
            {
                *pSenderAddrLength = 0;
                *pSenderAddr = 0;

            }
        }

        if(pCallbkAddrLength && pCallbkAddr)
        {
            if(Sms.sHeader.eParamMask & QCWWAN_SMS_PARAMASK_CALL_BACK)
            {
                if(*pCallbkAddrLength < Sms.sHeader.callBackLen)
                {
                    *pCallbkAddrLength = Sms.sHeader.callBackLen + 1;
                    return(eQCWWAN_ERR_BUFFER_SZ);
                }
/* RILSTART */
/* wctomb() is not implemented on Android, and although wcrtomb() is
 * implemented, it is just a stub function that does not do anything.
 * Ideally wctomb() should be replaced by something else, although it
 * is not essential until SLQSCDMADecodeMTTextMsg() is used by the RIL
 * since that is the only function that calls SwiCDMADecodeMTTextMsg().
 */
#if 0
                memset(pCallbkAddr, 0, *pCallbkAddrLength);
                *pCallbkAddrLength = Sms.sHeader.callBackLen + 1;
                wctomb(pCallbkAddr, (wchar_t)Sms.sHeader.szCallBack );
#else
                *pCallbkAddrLength = 0;
                *pCallbkAddr = 0;
#endif
/* RILSTOP */

            }
            else
            {
                *pCallbkAddrLength = 0;
                *pCallbkAddr = 0;

            }
        }

        if(pTextMsgLength && pTextMsg)
        {
            if(Sms.sHeader.eParamMask & QCWWAN_SMS_PARAMASK_USER_DATA)
            {
                if(*pTextMsgLength < Sms.sizeBuffer)
                {
                    *pTextMsgLength = (BYTE)Sms.sizeBuffer;
                    return(eQCWWAN_ERR_BUFFER_SZ);
                }
                memset(pTextMsg, 0, (*pTextMsgLength) * 2 );
                *pTextMsgLength = (BYTE)Sms.sizeBuffer;
                strncpy( (CHAR*)pTextMsg, (CHAR*)Sms.pMessage, Sms.sizeBuffer);
            }
            else
            {
                *pTextMsgLength = 0;
                *pTextMsg = 0;
            }
        }
        if(pPriority)
        {
            if(Sms.sHeader.eParamMask & QCWWAN_SMS_PARAMASK_PRIORITY)
            {
                 *pPriority = Sms.sHeader.ePriority;
                  /* 64 is decoded value for URGENT VZAM support
                     NORMAL - 0 or URGENT - 64 */
                  if (*pPriority == 64)
                  {
                      /* for VZAM  interactive (64), set to Normal
                         as VZAM does not use */
                      *pPriority = 0;
                  }
                  /* 128 is decoded value for URGENT VZAM
                     support NORMAL - 0 or URGENT - 128 */
                  else if (*pPriority == 128)
                  {
                      *pPriority = 1;
                  }
                  /* for WMC, this is set to urgent  as VZAM does
                     not currently support emergency (192)*/
                  else if (*pPriority == 192)
                  {
                     *pPriority = 1;
                  }
                  else
                  {
                  /* default - NORMAL, if we get here, then modem
                  must have returned a 0 or their were problems */
                      *pPriority = 0;
                  }
            }
            else
            {
                *pPriority = 0xFF;
            }
        }
        if(pPrivacy)
        {
            if(Sms.sHeader.eParamMask & QCWWAN_SMS_PARAMASK_PRIVACY)
            {
                *pPrivacy = Sms.sHeader.ePrivacy;
            }
            else
            {
                *pPrivacy = 0xFF;
            }
        }
        if(pLanguage)
        {
            if(Sms.sHeader.eParamMask & QCWWAN_SMS_PARAMASK_LANGUAGE)
            {
                *pLanguage = Sms.sHeader.eLanguage;
            }
            else
            {
                *pLanguage = 0xFF;
            }
        }
        if(mcTimeStamp)
        {
            if(Sms.sHeader.eParamMask & QCWWAN_SMS_PARAMASK_MC_TIME_STAMP)
            {
                mcTimeStamp[0] =  Sms.sHeader.ts_yr;
                mcTimeStamp[1] =
                   (Sms.sHeader.ts_mon >> 4)* 10 + (Sms.sHeader.ts_mon & 0x0F);
                mcTimeStamp[2] =
                   (Sms.sHeader.ts_day >> 4)* 10 + (Sms.sHeader.ts_day & 0x0F);
                mcTimeStamp[3] =
                   (Sms.sHeader.ts_hrs >> 4)* 10 + (Sms.sHeader.ts_hrs & 0x0F);
                mcTimeStamp[4] =
                   (Sms.sHeader.ts_min >> 4)* 10 + (Sms.sHeader.ts_min & 0x0F);
                mcTimeStamp[5] =
                   (Sms.sHeader.ts_sec >> 4)* 10 + (Sms.sHeader.ts_sec & 0x0F);
                mcTimeStamp[6] = 0;
            }
            else
            {
                mcTimeStamp[0] = 0xFF;
                mcTimeStamp[1] = 0xFF;
                mcTimeStamp[2] = 0xFF;
                mcTimeStamp[3] = 0xFF;
                mcTimeStamp[4] = 0xFF;
                mcTimeStamp[5] = 0xFF;
                mcTimeStamp[6] = 0xFF;
            }
        }
        if(absoluteValidity)
        {
            if(Sms.sHeader.eParamMask & QCWWAN_SMS_PARAMASK_VALID_ABS)
            {
                absoluteValidity[0] = Sms.sHeader.abVal_yr;
                absoluteValidity[1] = (Sms.sHeader.abVal_mon >> 4)* 10
                                          + (Sms.sHeader.abVal_mon & 0x0F);
                absoluteValidity[2] = (Sms.sHeader.abVal_day >> 4)* 10
                                          + (Sms.sHeader.abVal_day & 0x0F);
                absoluteValidity[3] = (Sms.sHeader.abVal_hrs >> 4)* 10
                                          + (Sms.sHeader.abVal_hrs & 0x0F);
                absoluteValidity[4] = (Sms.sHeader.abVal_min >> 4)* 10
                                          + (Sms.sHeader.abVal_min & 0x0F);
                absoluteValidity[5] = (Sms.sHeader.abVal_sec >> 4)* 10
                                          + (Sms.sHeader.abVal_sec & 0x0F);
                absoluteValidity[6] = 0;
            }
            else
            {
                absoluteValidity[0] = 0xFF;
                absoluteValidity[1] = 0xFF;
                absoluteValidity[2] = 0xFF;
                absoluteValidity[3] = 0xFF;
                absoluteValidity[4] = 0xFF;
                absoluteValidity[5] = 0xFF;
                absoluteValidity[6] = 0xFF;
            }
        }
        if(pRelativeValidity)
        {
            if(Sms.sHeader.eParamMask & QCWWAN_SMS_PARAMASK_VALID_REL)
            {
                *pRelativeValidity = Sms.sHeader.relVal_time;
            }
            else
            {
                *pRelativeValidity = 0xFF;
            }
        }
        if(pDisplayMode)
        {
            if(Sms.sHeader.eParamMask & QCWWAN_SMS_PARAMASK_DISP_MODE)
            {
                *pDisplayMode = Sms.sHeader.eMsgDispMode;
            }
            else
            {
                *pDisplayMode = 0xFF;
            }
        }
        if(pUserAcknowledgementReq)
        {
            /* not supported by this code base yet */
            *pUserAcknowledgementReq = FALSE;
        }
        if(pReadAcknowledgementReq)
        {
            /* not supported by this code base yet */
            *pReadAcknowledgementReq = FALSE;
        }
        if(pAlertPriority)
        {
            if(Sms.sHeader.eParamMask & QCWWAN_SMS_PARAMASK_DISP_MODE)
            {
                *pAlertPriority = Sms.sHeader.deliveryAlert;
            }
            else
            {
                *pAlertPriority = 0xFF;
            }
        }
    }

    switch(ret)
    {
    case eQCWWAN_ERR_NONE:
        return(eQCWWAN_ERR_NONE);
        break;

    case eQCWWAN_APP_ERR_FAILED:
        return(eQCWWAN_ERR_GENERAL);
        break;

    case eQCWWAN_ERR_INVALID_ARG:
        return(eQCWWAN_ERR_INVALID_ARG);
        break;

    case eQCWWAN_APP_ERR_BUFFER_SZ:
        return(eQCWWAN_ERR_BUFFER_SZ);
        break;

    case eQCWWAN_APP_ERR_UNEXPECTED_RESP:
        return(eQCWWAN_ERR_INVALID_ARG);
        break;

    default:
        return(eQCWWAN_ERR_GENERAL);
    }
}
