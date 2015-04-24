/*************
 *
 * Filename:    piBe - Big Endian put and get functions.
 *
 * Purpose:     This file contains all functions for reading and 
 *              writing packets in big Endian byte order.
 *              ( QMI protocol is packed in little Endian format).
 *
 * Copyright:    © 2011 Sierra Wireless, All rights reserved
 *
 **************/
#include "aa/aaglobal.h"
#include "pi/piudefs.h"

/*************
 *
 * Name:     piput16Be - put a 16-bit value into a packet
 *
 * Purpose:  To put a 16-bit value into a packet that is in big endian order
 *           and increment the packet pointer passed the written field
 *                         
 * Parms:    field - 16-bit field value
 *           packetpp - memory location of the pointer to the packet 
 *           where the 16-bit field will be written to
 *
 * Return:   no value
 *
 * Abort:    none
 *
 * Notes:    This function performs no pointer validation.
 *
 **************/
global void piput16Be(swi_uint16 field, swi_uint8 **packetpp)
{
    swi_uint8 *packetp;

    packetp = *packetpp;
    *packetp++ = field >> 8;
    *packetp++ = field;

    *packetpp = packetp;
}

/*************
 *
 * Name:     piput32Be - put a 32-bit value into a packet
 *
 * Purpose:  To put a 32-bit value into a packet that is in big endian order
 *           and increment the packet pointer passed the written field
 *                         
 * Parms:    field     - 32-bit field value
 *           bufferpp  - memory location of the pointer to the packet 
 *                          where the 32-bit value will be put.  
 *
 * Return:   no value
 *
 * Abort:    none
 *
 * Notes:    This function performs no pointer validation.
 *
 **************/
global void piput32Be(swi_uint32 field, swi_uint8 **packetpp)
{
    swi_uint8 *packetp;

    packetp = *packetpp;
    *packetp++ = field >> 24;
    *packetp++ = field >> 16;
    *packetp++ = field >> 8;
    *packetp++ = field;
    *packetpp = packetp;
}

/*************
 *
 * Name:     piput64Be - put a 64 bit value into a packet
 *
 * Purpose:  To put a 64-bit value into a packet that is in big endian order
 *           and increment the packet pointer passed the written field
 *                         
 * Parms:    field     - 64 bit field value
 *           bufferpp  - memory location of the pointer to the packet 
 *                       where the 64 bit value will be put.  
 *
 * Return:   no value
 *
 * Abort:    none
 *
 * Notes:    This function performs no pointer validation.
 *
 **************/
global void piput64Be(swi_uint64 field, swi_uint8 **packetpp)
{
    swi_uint8 *packetp;

    packetp = *packetpp;

    *packetp++ = field >> 56;
    *packetp++ = field >> 48;
    *packetp++ = field >> 40;
    *packetp++ = field >> 32;
    *packetp++ = field >> 24;
    *packetp++ = field >> 16;
    *packetp++ = field >> 8;
    *packetp++ = field;

    *packetpp = packetp;
}

/*************
 *
 * Name:     piget16Be - get a 16-bit value from a packet
 *
 * Purpose:  To get a 16-bit value from a packet that is in big endian 
 *           order and increment the packet pointer passed the read field.
 *                         
 * Parms:    packetpp  - memory location of the pointer to the buffer where 
 *                       the 16-bit value will be read from.  
 *
 * Return:   16-bit value
 *
 * Abort:    none
 *
 * Notes:    This function performs no pointer validation.
 *
 **************/
global swi_uint16 piget16Be(swi_uint8 **packetpp)
{
    swi_uint16 field;
    swi_uint8 *packetp;

    packetp = *packetpp;

    field = *packetp++;
    field <<= 8;
    field += *packetp++;

    *packetpp = packetp;

    return(field);

}

/*************
*
* Name:     piget32Be - get a 32-bit value from a packet
*
* Purpose:  To get a 32-bit value from a packet that is in big endian 
*           order and increment the packet pointer passed the read field.
*                         
* Parms:    packetpp - memory location of the pointer to the packet 
*                      where the 32-bit value will be read from.  
*
* Return:   32-bit value
*
* Abort:    none
*
* Notes:    This function performs no pointer validation.
*
**************/
global swi_uint32 piget32Be(swi_uint8 **packetpp)
{
    swi_uint32 field;
    swi_uint8 *packetp;

    packetp = *packetpp;
    field = *packetp++;
    field <<= 8;
    field += *packetp++;
    field <<= 8;
    field += *packetp++;
    field <<= 8;
    field += *packetp++;
    *packetpp = packetp;

    return(field);

}

/*************
*
* Name:     piget64Be - get a 64 bit value from a packet
*
* Purpose:  To get a 64-bit value from a packet that is in bid endian 
*           order and increment the packet pointer passed the read field.
*                         
* Parms:    packetpp - memory location of the pointer to the packet 
*                      where the 64 bit value will be read from.  
*
* Return:   64 bit value
*
* Abort:    none
*
* Notes:    This function performs no pointer validation.
*           Due to a problem we have noted with the gcc compiler this function
*           should not be called. Instead one should duplicate the code inline
*           where ever the function needs to be called as shown in 
*           SwiIntUpkCmBasic.c, function SwiUpkRadioBandCfg().
*
**************/
global swi_uint64 piget64Be(swi_uint8 **packetpp)
{
    swi_uint64 field;

    swi_uint8 *packetp;

    packetp = *packetpp;

    field = *packetp++;
    field <<= 8;
    field += *packetp++;
    field <<= 8;
    field += *packetp++;
    field <<= 8;
    field += *packetp++;
    field <<= 8;
    field += *packetp++;
    field <<= 8;
    field += *packetp++;
    field <<= 8;
    field += *packetp++;
    field <<= 8;
    field += *packetp++;
    *packetpp = packetp;

    return(field);

}
