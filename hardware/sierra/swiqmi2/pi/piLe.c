/*************
 *
 * Filename:    piLe - network byte order put and get functions.
 *
 * Purpose:     This file contains all functions for reading and 
 *              writing packets in network byte order, or in little
 *              Endian byte order( QMI protocol is packed in little
 *              Endian format). If SLQS is defined the packing is 
 *              assumed to be little Endian.
 *
 * Copyright:    © 2011 Sierra Wireless, All rights reserved
 *
 **************/
#include "aa/aaglobal.h"
#include "pi/piudefs.h"

/*************
 *
 * Name:        piput16 - put a 16-bit value into a packet
 *
 * Purpose:     To put a 16-bit value into a packet that is in network 
 *              byte order and increment the packet pointer passed the 
 *              written field.
 *                         
 * Parms:       field - 16-bit field value
 *              packetpp - memory location of the pointer to the packet 
 *              where the 16-bit field will be written to
 *
 * Return:      no value
 *
 * Abort:       none
 *
 * Notes:       This function performs no pointer validation.
 *
 **************/
global void piput16(swi_uint16 field, swi_uint8 **packetpp)
{
    swi_uint8 *packetp;

    packetp = *packetpp;
#ifndef SLQS
    *packetp++ = field >> 8;
    *packetp++ = field;
#else
    *packetp++ = field;
    *packetp++ = field >> 8;
#endif /* SLQS */
    *packetpp = packetp;
}

/*************
 *
 * Name:     piput32 - put a 32-bit value into a packet
 *
 * Purpose:  To put a 32-bit value into a packet that is in network 
 *           byte order and increment the packet pointer passed the 
 *           written field.
 *                         
 * Parms:    field     - 32-bit field value
 *           bufferpp  - memory location of the pointer to the packet 
 *                       where the 32-bit value will be put.  
 *
 * Return:   no value
 *
 * Abort:    none
 *
 * Notes:    This function performs no pointer validation.
 *
 **************/
global void piput32(swi_uint32 field, swi_uint8 **packetpp)
{
    swi_uint8 *packetp;

    packetp = *packetpp;
#ifndef SLQS
    *packetp++ = field >> 24;
    *packetp++ = field >> 16;
    *packetp++ = field >> 8;
    *packetp++ = field;
#else
    *packetp++ = field;
    *packetp++ = field >> 8;
    *packetp++ = field >> 16;
    *packetp++ = field >> 24;
#endif /* SLQS */
    *packetpp = packetp;
}
/*************
 *
 * Name:     piput64 - put a 64 bit value into a packet
 *
 * Purpose:  To put a 64 bit value into a packet that is in network 
 *           byte order and increment the packet pointer passed the 
 *           written field.
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
global void piput64(swi_uint64 field, swi_uint8 **packetpp)
{
    swi_uint8 *packetp;

    packetp = *packetpp;

#ifndef SLQS
    *packetp++ = field >> 56;
    *packetp++ = field >> 48;
    *packetp++ = field >> 40;
    *packetp++ = field >> 32;
    *packetp++ = field >> 24;
    *packetp++ = field >> 16;
    *packetp++ = field >> 8;
    *packetp++ = field;
#else
    *packetp++ = field;
    *packetp++ = field >> 8;
    *packetp++ = field >> 16;
    *packetp++ = field >> 24;
    *packetp++ = field >> 32;
    *packetp++ = field >> 40;
    *packetp++ = field >> 48;
    *packetp++ = field >> 56;
#endif /* SLQS */

    *packetpp = packetp;
}

/*************
 *
 * Name:        piput16s - put a 16-bit value array into a packet
 *
 * Purpose:     To put a 16-bit value array into a packet that is in network 
 *              byte order and increment the packet pointer passed the 
 *              written field.
 *                         
 * Parms:       field - point for 16-bit field value array
 *              num - number of 16-bit fields to be copied
 *              packetpp - memory location of the pointer to the packet 
 *              where the 16-bit array field will be written to
 *
 * Return:      no value
 *
 * Abort:       none
 *
 * Notes:       This function performs no pointer validation.
 *
 **************/
global void piput16s(const swi_uint16 *field, swi_uint16 num, swi_uint8 **packetpp)
{
    swi_uint16 count;
    swi_uint8 *packetp;

    packetp = *packetpp;

    for(count = 0; count < num; count++)
    {
        piput16(*field++, &packetp);
    }

    *packetpp = packetp;
}

/*************
 *
 * Name:        piput32s - put a 32-bit value array into a packet
 *
 * Purpose:     To put a 32-bit value array into a packet that is in network 
 *              byte order and increment the packet pointer passed the 
 *              written field.
 *                         
 * Parms:       field - point for 32-bit field value array
 *              num - number of 32-bit fields to be copied
 *              packetpp - memory location of the pointer to the packet 
 *              where the 32-bit array field will be written to
 *
 * Return:      no value
 *
 * Abort:       none
 *
 * Notes:       This function performs no pointer validation.
 *
 **************/
global void piput32s(const swi_uint32 *field, swi_uint16 num, swi_uint8 **packetpp)
{
    swi_uint16 count;
    swi_uint8 *packetp;

    packetp = *packetpp;

    for(count = 0; count < num; count++)
    {
        piput32(*field++, &packetp);
    }

    *packetpp = packetp;
}

/*************
*
* Name:     piputmulti - put a string of 8-bit fields into a packet
*
* Purpose:  To put a string of 8-bit fields into a packet and increment
*           the packet pointer passed the last written 8-bit field
*                         
* Parms:    fieldsp  - pointer to string of 8-bit fields
*           packetpp - memory location of the pointer to the packet 
*                      where the 8-bit fields should be copied to.  
*           numfields- number of 8-bit fields to be copied
*
* Return:   no value
*
* Abort:    none
*
* Notes:    This function performs no pointer validation.
*
*************/
global void piputmulti(const swi_uint8 *fieldsp, 
                       swi_uint8 **packetpp, 
                       swi_uint16 numfields)
{
    swi_uint8 *packetp;
    int i;

    packetp = *packetpp;

    for(i = numfields - 1; i >= 0; i--)
    {
        *packetp++ = *fieldsp++;
    }

    *packetpp = packetp;
}

/*************
 *
 * Name:     piget16 - get a 16-bit value from a packet
 *
 * Purpose:  To get a 16-bit value from a packet that is in network byte 
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
global swi_uint16 piget16(swi_uint8 **packetpp)
{
    swi_uint16 field;
    swi_uint8 *packetp;

    packetp = *packetpp;

#ifndef SLQS
    field = *packetp++;
    field <<= 8;
    field += *packetp++;
#else
    field = *(packetp + 1);
    field <<= 8;
    field += *packetp;
    packetp += 2;
#endif /* SLQS */

    *packetpp = packetp;

    return(field);

}

/*************
*
* Name:     piget32 - get a 32-bit value from a packet
*
* Purpose:  To get a 32-bit value from a packet that is in network 
*           byte order and increment the packet pointer passed the 
*           read field.
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
global swi_uint32 piget32(swi_uint8 **packetpp)
{
    swi_uint32 field;
    swi_uint8 *packetp;

    packetp = *packetpp;
#ifndef SLQS
    field = *packetp++;
    field <<= 8;
    field += *packetp++;
    field <<= 8;
    field += *packetp++;
    field <<= 8;
    field += *packetp++;
#else
    field =  ((swi_uint32)*(packetp + 3)) << 24;
    field += ((swi_uint32)*(packetp + 2)) << 16;
    field += ((swi_uint32)*(packetp + 1)) << 8;
    field += *packetp;
    packetp += 4;
#endif /* SLQS */
    *packetpp = packetp;

    return(field);

}
/*************
*
* Name:     piget64 - get a 64 bit value from a packet
*
* Purpose:  To get a 64 bit value from a packet that is in network 
*           byte order and increment the packet pointer passed the 
*           read field.
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
global swi_uint64 piget64(swi_uint8 **packetpp)
{
    swi_uint64 field;

    swi_uint8 *packetp;

    packetp = *packetpp;

#ifndef SLQS
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
#else
    field =  ((swi_uint64)*(packetp + 7)) << 8*7;
    field += ((swi_uint64)*(packetp + 6)) << 8*6;
    field += ((swi_uint64)*(packetp + 5)) << 8*5;
    field += ((swi_uint64)*(packetp + 4)) << 8*4;
    field += ((swi_uint64)*(packetp + 3)) << 8*3;
    field += ((swi_uint64)*(packetp + 2)) << 8*2;
    field += ((swi_uint64)*(packetp + 1)) << 8*1;
    field += *packetp;
    packetp += 8;
#endif /* SLQS */

    *packetpp = packetp;

    return(field);

}

/*************
 *
 * Name:     piget16s - get a 16-bit value array from a packet
 *
 * Purpose:  To get a 16-bit array value from a packet that is in network byte 
 *           order and increment the packet pointer passed the read field.
 *                         
 * Parms:    packetpp  - memory location of the pointer to the buffer where 
 *                       the 16-bit array value will be read from.  
 *           field     - pointer to a buffer to which the 16-bit fields 
 *                       should be copied
 *           num       - number of 16-bit fields to be copied
 *
 * Return:   no value
 *
 * Abort:    none
 *
 * Notes:    This function performs no pointer validation.
 *
 **************/
global void piget16s(swi_uint16 *field, swi_uint16 num, swi_uint8 **packetpp)
{
    swi_uint16 count;
    swi_uint8 *packetp;

    packetp = *packetpp;

    for(count = 0; count < num; count++)
    {
        *field++ = piget16(&packetp);
    }
}

/*************
 *
 * Name:     piget32s - get a 32-bit value array from a packet
 *
 * Purpose:  To get a 32-bit array value from a packet that is in network byte 
 *           order and increment the packet pointer passed the read field.
 *                         
 * Parms:    packetpp  - memory location of the pointer to the buffer where 
 *                       the 32-bit array value will be read from.  
 *           field     - pointer to a buffer to which the 32-bit fields 
 *                       should be copied
 *           num       - number of 32-bit fields to be copied
 *
 * Return:   no value
 *
 * Abort:    none
 *
 * Notes:    This function performs no pointer validation.
 *
 **************/
global void piget32s(swi_uint32 *field, swi_uint16 num, swi_uint8 **packetpp)
{
    swi_uint16 count;
    swi_uint8 *packetp;

    packetp = *packetpp;

    for(count = 0; count < num; count++)
    {
        *field++ = piget32(&packetp);
    }
}

/*************
 *
 * Name:    pigetmulti - get a string of 8-bit fields from a packet
 *
 * Purpose: To get a string of 8-bit fields from a packet and increment 
 *          the packet pointer pass the last read 8-bit field.
 *                         
 * Parms:   packetpp  - memory location of a pointer to a packet where 
 *                      a string of 8-bit fields should be read from.  
 *          bufferp   - pointer to a buffer to which the 8-bit fields 
 *                      should be copied
 *          numfields - number of 8-bit fields to be copied
 *
 * Return:  no value
 *
 * Abort:   none
 *
 * Notes:   This function performs no pointer validation.
 *
 **************/
global void pigetmulti(swi_uint8 **packetpp, swi_uint8 *bufferp, swi_uint16 numfields)
{
    swi_uint8 *packetp;
    int i;

    packetp = *packetpp;

    for(i = numfields - 1; i >= 0; i--)
    {
            *bufferp++ = *packetp++;
    }

    *packetpp = packetp;
}

/*************
 *
 * Name:     piget16_noinc - get a 16-bit value from a packet
 *
 * Purpose:  To get a 16-bit value from a packet that is in network 
 *           byte order and not increment the packet pointer
 *        
 * Parms:    packetpp - memory location of the pointer to the buffer 
 *                      where the 16-bit value will be read from.  
 *
 * Return:   16-bit value
 *
 * Abort:    none
 *
 * Notes:    This function performs no pointer validation. This function is 
 *           declared inline for speed. This may not work with other compilers.
 *
 **************/
global swi_uint16 piget16_noinc ( swi_uint8 **packetpp )
{
   swi_uint16 field;
   swi_uint8 *packetp;

   packetp = *packetpp;

   field = *packetp++;
   field <<= 8;
   field += *packetp++;

   return field;
}

/*************
*
* Name:     piget32_noinc - get a 32-bit value from a packet
*
* Purpose:  To get a 32-bit value from a packet that is in network byte order
*           and not increment the packet pointer
*        
* Parms:    packetpp - memory location of the pointer to the packet where the 32
*                      bit value will be read from.  
*
* Return:   32-bit value
*
* Abort:    none
*
* Notes:    This function performs no pointer validation. This function is 
*           declared inline for speed. This may not work with other compilers.
*
**************/
global swi_uint32 piget32_noinc ( swi_uint8 **packetpp )
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

   return field;
}

/*************
 *
 * Name:     piput16_host
 *
 * Purpose:  put a 16-bit value into a packet in host byte order
 *           and increment the packet pointer past the last value written
 *        
 * Parms:    field    - 16-bit field value
 *           packetpp - memory location of the pointer to the packet where 
 *                      the 16-bit field will be written to
 *
 * Return:   no value
 *
 * Abort:    none
 *
 * Notes:    This function performs no pointer validation. This function is 
 *           declared inline for speed. This may not work with other compilers.
 *
 **************/
global void piput16_host ( swi_uint16 field, swi_uint8 **packetpp )
{
   swi_uint8 *packetp;
   
   packetp = *packetpp;

   *packetp++ = (swi_uint8)field;
   *packetp++ = (swi_uint8)(field >> 8);

   *packetpp = packetp;
}

/*************
 *
 * Name:     piput16_noinc - put a 16-bit value into a packet
 *
 * Purpose:  To put a 16-bit value into a packet that is in network byte 
 *           order and not increment the packet pointer
 *        
 * Parms:    field    - 16-bit field value
 *           packetpp - memory location of the pointer to the packet where 
 *                      the 16-bit field will be written to
 *           inc      - the amount for the function to temporarily increment 
 *                      packetpp
 *
 * Return:   no value
 *
 * Abort:    none
 *
 * Notes:    This function performs no pointer validation. This function is 
 *           declared inline for speed. This may not work with other compilers.
 *
 **************/
global void piput16_noinc ( swi_uint16 field, swi_uint8 **packetpp, swi_uint32 inc )
{
   swi_uint8 *packetp;
   
   packetp = *packetpp + inc;

   *packetp++ = (swi_uint8)(field >> 8);
   *packetp++ = (swi_uint8)field;
}

/*************
 *
 * Name:     piget16 - get a 16-bit value from a packet
 *
 * Purpose:  To get a 16-bit value from a packet that is in host byte order and
 *           increment the packet pointer past the last value written
 *        
 * Parms:    packetpp - memory location of the pointer to the buffer where 
 *                      the 16-bit value will be read from.  
 *
 * Return:   16-bit value
 *
 * Abort:    none
 *
 * Notes:    This function performs no pointer validation. This function is 
 *           declared inline for speed. This may not work with other compilers.
 *
 **************/
global swi_uint16 piget16_host ( swi_uint8 **packetpp )
{
   swi_uint16 field;
   swi_uint8 *packetp;

   packetp = *packetpp;

   field = *packetp++;
   field |= *packetp++ << 8;

   *packetpp = packetp;

   return field;
}
