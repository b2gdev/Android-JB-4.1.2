/*
 * \ingroup basic
 *
 * \file    qmcommon.c.c
 *
 * \brief   Common functionality used by application and SDK
 *
 * Copyright: © 2011 Sierra Wireless, Inc. all rights reserved
 *
 */
#include "qm/qmerrno.h"
#include "am/amudefs.h"
#include "pi/piudefs.h"
#include "qm/qmidefs.h"
#include "sl/sludefs.h"
#include "qmqmisvc.h"

/*************
 *
 * Increments Index in TBuffer by given increment value
 *
 * \param   bufp        pointer to qmTBuffer structure
 * \param   increment   Amount to increment the index in the TBuffer
 *
 * \return  eQCWWAN_ERR_NONE      - Increment successful
 * \return  eQCWWAN_ERR_MEMORY    - Increment beyond allowed size attempted
 *
 **************/
local enum eQCWWANError IncrementIndex(
    struct qmTBuffer *tbufp,
    swi_uint16 increment )
{
    if (tbufp->Index + increment <= tbufp->Size)
    {
        tbufp->Index += increment;
    }
    else
    {
        return eQCWWAN_ERR_MEMORY;
    }
    return eQCWWAN_ERR_NONE;
}

/*************
 *
 * Sets Index in TBuffer to given value
 *
 * \param   bufp    pointer to qmTBuffer structure
 * \param   index   Value to set the index to
 *
 * \return  eQCWWAN_ERR_NONE      - Set Index successful
 * \return  eQCWWAN_ERR_MEMORY    - Set Index beyond allowed size attempted
 *
 **************/
local enum eQCWWANError IndexB(
    swi_uint8 *bufp,
    swi_uint16 index )
{
    struct qmTBuffer *tbufp = (struct qmTBuffer*)bufp;

    if (index < tbufp->Size)
    {
        tbufp->Index = index;
    }
    else
    {
        return eQCWWAN_ERR_MEMORY;
    }
    return eQCWWAN_ERR_NONE;
}

/*************
 *
 * Poke a Byte value in TBuffer at given index, with no Index change
 *
 * \param   bufp    pointer to qmTBuffer structure
 * \param   index   index to buffer where value is to be set
 * \param   value   Value to set in the buffer
 *
 * \return  eQCWWAN_ERR_NONE      - Poke Byte successful
 * \return  eQCWWAN_ERR_MEMORY    - Poke Byte beyond allowed size attempted
 *
 **************/
package enum eQCWWANError PokeByte(
    swi_uint8 *bufp,
    swi_uint16 index,
    swi_uint8 value )
{
    struct qmTBuffer *tbufp = (struct qmTBuffer*)bufp;

    if (index + sizeof(value) <= tbufp->Size)
    {
        *(swi_uint8*)&(tbufp->Datap[index]) = value;
    }
    else
    {
        return (eQCWWAN_ERR_MEMORY);
    }
    return eQCWWAN_ERR_NONE;
}

/*************
 *
 * Poke a WORD value in TBuffer at given index, with no Index change
 *
 * \param   bufp    pointer to qmTBuffer structure
 * \param   index   index to buffer where value is to be set
 * \param   value   Value to set in the buffer
 *
 * \return  eQCWWAN_ERR_NONE       - Poke WORD successful
 * \return  eQCWWAN_ERR_MEMORY     - Poke WORD beyond allowed size attempted
 *
 **************/
package enum eQCWWANError PokeWord(
    swi_uint8  *bufp,
    swi_uint16 index,
    swi_uint16 value )
{
    struct qmTBuffer *tbufp = (struct qmTBuffer*)bufp;

    if (index + sizeof(value) <= tbufp->Size)
    {
        swi_uint8 *plocal = tbufp->Datap + index;
        piput16( value, &plocal );
    }
    else
    {
        return (eQCWWAN_ERR_MEMORY);
    }
    return eQCWWAN_ERR_NONE;
}

/*************
 *
 * Puts a Byte value in TBuffer at given index and increment Index
 *
 * \param   bufp    pointer to qmTBuffer structure
 * \param   value   Value to set in the buffer
 *
 * \return  eQCWWAN_ERR_NONE      - Put Byte successful
 * \return  eQCWWAN_ERR_MEMORY    - Put Byte beyond allowed size attempted
 *
 **************/
global enum eQCWWANError PutByte(
    swi_uint8 *bufp,
    swi_uint8 value )
{
    struct qmTBuffer *tbufp = (struct qmTBuffer*)bufp;

    if (tbufp->Index + sizeof(value) <= tbufp->Size)
    {
        *(swi_uint8*)&tbufp->Datap[tbufp->Index] = value;
        tbufp->Index += sizeof(value);
    }
    else
    {
        return eQCWWAN_ERR_MEMORY;
    }
    return eQCWWAN_ERR_NONE;
}

/*************
 *
 *  Get a stream of octets in TBuffer starting at Index and increment Index.
 *
 * \param   bufp    pointer to qmTBuffer structure
 * \param   value   octet stream
 *
 * \return  eQCWWAN_ERR_NONE      - Successful
 * \return  eQCWWAN_ERR_MEMORY    - A read beyond allowed size attempted
 *
 * \notes
 *
 **************/
global enum eQCWWANError GetStream(
    swi_uint8 *bufp,
    swi_uint8 *value,
    swi_uint32 streamlen )
{
    struct qmTBuffer *tbufp = (struct qmTBuffer*)bufp;
    swi_uint32 count = 0;

    while( (tbufp->Index <= tbufp->Size) &&
           count < streamlen )
    {
        *(value + count++) = *(swi_uint8*)&tbufp->Datap[tbufp->Index++];
    }

    if( tbufp->Index > tbufp->Size )
    {
        return eQCWWAN_ERR_MEMORY;
    }

    return eQCWWAN_ERR_NONE;
}

/*************
 *
 * Puts a stream of octets in TBuffer at given index and increments Index
 *
 * \param   bufp    pointer to qmTBuffer structure
 * \param   value   octet stream
 *
 * \return  eQCWWAN_ERR_NONE      - Successful
 * \return  eQCWWAN_ERR_MEMORY    - A write beyond allowed size attempted
 *
 * \notes
 *
 **************/
global enum eQCWWANError PutStream(
    swi_uint8 *bufp,
    swi_uint8 *value,
    swi_uint32 streamlen )
{
    struct qmTBuffer *tbufp = (struct qmTBuffer*)bufp;
    swi_uint32 count = 0;

    while( (tbufp->Index <= tbufp->Size) &&
           count < streamlen )
    {
        *(swi_uint8*)&tbufp->Datap[tbufp->Index++] = *(value + count++);
    }

    return eQCWWAN_ERR_NONE;
}

/*************
 *
 * Puts a Word value in TBuffer at given index and increment Index
 *
 * \param   bufp    pointer to qmTBuffer structure
 * \param   value   Value to set in the buffer
 *
 * \return  eQCWWAN_ERR_NONE      - Put Word successful
 * \return  eQCWWAN_ERR_MEMORY    - Put Word beyond allowed size attempted
 *
 **************/
global enum eQCWWANError PutWord(
    swi_uint8 *bufp,
    swi_uint16 value )
{
    struct qmTBuffer *tbufp = (struct qmTBuffer*)bufp;

    if (tbufp->Index + sizeof(value) <= tbufp->Size)
    {
        swi_uint8 *plocal = tbufp->Datap + tbufp->Index;
        piput16( value, &plocal );
        tbufp->Index += sizeof(value);
    }
    else
    {
        return eQCWWAN_ERR_MEMORY;
    }
    return eQCWWAN_ERR_NONE;
}

/*************
 *
 * Puts a LONG value in TBuffer at given index and increment Index
 *
 * \param   bufp    pointer to qmTBuffer structure
 * \param   value   Value to set in the buffer
 *
 * \return  eQCWWAN_ERR_NONE      - Put LONG successful
 * \return  eQCWWAN_ERR_MEMORY    - Put LONG beyond allowed size attempted
 *
 **************/
global enum eQCWWANError PutLong(
    swi_uint8 *bufp,
    swi_uint32 value )
{
    struct qmTBuffer *tbufp = (struct qmTBuffer*)bufp;

    if (tbufp->Index + sizeof(value) <= tbufp->Size)
    {
        swi_uint8 *plocal = tbufp->Datap + tbufp->Index;
        piput32( value, &plocal );
        tbufp->Index += sizeof(value);
    }
    else
    {
        return eQCWWAN_ERR_MEMORY;
    }
    return eQCWWAN_ERR_NONE;
}

/*************
 *
 * Puts a 64 bit value in TBuffer at given index and increment Index
 *
 * \param   bufp    pointer to qmTBuffer structure
 * \param   value   Value to set in the buffer
 *
 * \return  eQCWWAN_ERR_NONE   - Put LONG successful
 * \return  eQCWWAN_ERR_MEMORY - Put LONG beyond allowed size attempted
 *
 **************/
global enum eQCWWANError Put64(
    swi_uint8 *bufp,
    swi_uint64 value )
{
    struct qmTBuffer *tbufp = (struct qmTBuffer*)bufp;

    if (tbufp->Index + sizeof(value) <= tbufp->Size)
    {
        swi_uint8 *plocal = tbufp->Datap + tbufp->Index;
        piput64( value, &plocal );
        tbufp->Index += sizeof(value);
    }
    else
    {
        return eQCWWAN_ERR_MEMORY;
    }
    return eQCWWAN_ERR_NONE;
}

/*************
 *
 * Puts a Word value in TBuffer in big endian format at given index
 * and increment Index
 *
 * \param   bufp    pointer to qmTBuffer structure
 * \param   value   Value to set in the buffer
 *
 * \return  eQCWWAN_ERR_NONE      - Put Word successful
 * \return  eQCWWAN_ERR_MEMORY    - Put Word beyond allowed size attempted
 *
 **************/
global enum eQCWWANError PutWordBe(
    swi_uint8 *bufp,
    swi_uint16 value )
{
    struct qmTBuffer *tbufp = (struct qmTBuffer*)bufp;

    if (tbufp->Index + sizeof(value) <= tbufp->Size)
    {
        swi_uint8 *plocal = tbufp->Datap + tbufp->Index;
        piput16Be( value, &plocal );
        tbufp->Index += sizeof(value);
    }
    else
    {
        return eQCWWAN_ERR_MEMORY;
    }
    return eQCWWAN_ERR_NONE;
}

/*************
 *
 * Puts a LONG value in TBuffer in big endian format at given index
 * and increment Index
 *
 * \param   bufp    pointer to qmTBuffer structure
 * \param   value   Value to set in the buffer
 *
 * \return  eQCWWAN_ERR_NONE      - Put LONG successful
 * \return  eQCWWAN_ERR_MEMORY    - Put LONG beyond allowed size attempted
 *
 **************/
global enum eQCWWANError PutLongBe(
    swi_uint8 *bufp,
    swi_uint32 value )
{
    struct qmTBuffer *tbufp = (struct qmTBuffer*)bufp;

    if (tbufp->Index + sizeof(value) <= tbufp->Size)
    {
        swi_uint8 *plocal = tbufp->Datap + tbufp->Index;
        piput32Be( value, &plocal );
        tbufp->Index += sizeof(value);
    }
    else
    {
        return eQCWWAN_ERR_MEMORY;
    }
    return eQCWWAN_ERR_NONE;
}

/*************
 *
 * Puts a 64 bit value in TBuffer in big endian format at given index
 * and increment Index
 *
 * \param   bufp    pointer to qmTBuffer structure
 * \param   value   Value to set in the buffer
 *
 * \return  eQCWWAN_ERR_NONE   - Put LONG successful
 * \return  eQCWWAN_ERR_MEMORY - Put LONG beyond allowed size attempted
 *
 **************/
global enum eQCWWANError Put64Be(
    swi_uint8 *bufp,
    swi_uint64 value )
{
    struct qmTBuffer *tbufp = (struct qmTBuffer*)bufp;

    if (tbufp->Index + sizeof(value) <= tbufp->Size)
    {
        swi_uint8 *plocal = tbufp->Datap + tbufp->Index;
        piput64Be( value, &plocal );
        tbufp->Index += sizeof(value);
    }
    else
    {
        return eQCWWAN_ERR_MEMORY;
    }
    return eQCWWAN_ERR_NONE;
}

/*************
 *
 * Gets a swi_uint8 value in TBuffer at given index and increment Index to next
 *
 * \param   bufp    pointer to qmTBuffer structure
 * \param   valuep  Pointer to location to return value gotten.
 *
 * \return  eQCWWAN_ERR_NONE      - Get Byte successful
 * \return  eQCWWAN_ERR_MEMORY    - Get Byte beyond allowed size attempted
 *
 **************/
global enum eQCWWANError GetByte(
    swi_uint8 *bufp,
    swi_uint8 *valuep )
{
    struct qmTBuffer *tbufp = (struct qmTBuffer *)bufp;

    if (tbufp->Index + sizeof(swi_uint8) <= tbufp->Size)
    {
        *valuep = *(swi_uint8*)&tbufp->Datap[tbufp->Index];
        tbufp->Index += sizeof(swi_uint8);
    }
    else
    {
        return eQCWWAN_ERR_MEMORY;
    }

    return eQCWWAN_ERR_NONE;
}

/*************
 *
 * Gets a swi_uint16 value in TBuffer at given index and increment Index to next
 *
 * \param   bufp    pointer to qmTBuffer structure
 * \param   valuep  Pointer to location to return value gotten.
 *
 * \return  eQCWWAN_ERR_NONE      - Get Word successful
 * \return  eQCWWAN_ERR_MEMORY    - Get Word beyond allowed size attempted
 *
 **************/
global enum eQCWWANError GetWord(
    swi_uint8 *bufp,
    swi_uint16 *valuep )
{
    struct qmTBuffer *tbufp = (struct qmTBuffer*)bufp;

    if (tbufp->Index + sizeof(swi_uint16) <= tbufp->Size)
    {
        swi_uint8 *plocal = tbufp->Datap + tbufp->Index;
        *valuep = piget16( &plocal );
        tbufp->Index += sizeof(swi_uint16);
    }
    else
    {
        return eQCWWAN_ERR_MEMORY;
    }

    return eQCWWAN_ERR_NONE;
}

/*************
 *
 * Gets a LONG value in TBuffer at given index and increment Index to next
 *
 * \param   bufp    pointer to qmTBuffer structure
 * \param   valuep  Pointer to location to return value gotten.
 *
 * \return  eQCWWAN_ERR_NONE      - Get LONG successful
 * \return  eQCWWAN_ERR_MEMORY    - Get LONG beyond allowed size attempted
 *
 **************/
global enum eQCWWANError GetLong(
    swi_uint8 *bufp,
    swi_uint32 *valuep )
{
    struct qmTBuffer *tbufp = (struct qmTBuffer*)bufp;

    if (tbufp->Index + sizeof(swi_uint32) <= tbufp->Size)
    {
        swi_uint8 *plocal = tbufp->Datap + tbufp->Index;
        *valuep = piget32( &plocal );
        tbufp->Index += sizeof(swi_uint32);
    }
    else
    {
        return eQCWWAN_ERR_MEMORY;
    }

    return eQCWWAN_ERR_NONE;
}

/*************
 *
 * Gets a LONGLONG value in TBuffer at given index and increment Index to next
 *
 * \param   bufp    pointer to qmTBuffer structure
 * \param   valuep  Pointer to location to return value gotten.
 *
 * \return  eQCWWAN_ERR_NONE      - Get LONGLONG successful
 * \return  eQCWWAN_ERR_MEMORY    - Get LONGLONG beyond allowed size attempted
 *
 **************/
global enum eQCWWANError GetLongLong(
    swi_uint8 *bufp,
    swi_uint64 *valuep )
{
    struct qmTBuffer *tbufp = (struct qmTBuffer*)bufp;

    if (tbufp->Index + sizeof(swi_uint64) <= tbufp->Size)
    {
        swi_uint8 *plocal = tbufp->Datap + tbufp->Index;
        *valuep = piget64( &plocal );
        tbufp->Index += sizeof(swi_uint64);
    }
    else
    {
        return eQCWWAN_ERR_MEMORY;
    }

    return eQCWWAN_ERR_NONE;
}

/*************
 *
 * Gets a swi_uint16 value in TBuffer in big endian format at given index and
 * increment Index to next
 *
 * \param   bufp    pointer to qmTBuffer structure
 * \param   valuep  Pointer to location to return value gotten.
 *
 * \return  eQCWWAN_ERR_NONE      - Get Word successful
 * \return  eQCWWAN_ERR_MEMORY    - Get Word beyond allowed size attempted
 *
 **************/
global enum eQCWWANError GetWordBe(
    swi_uint8 *bufp,
    swi_uint16 *valuep )
{
    struct qmTBuffer *tbufp = (struct qmTBuffer*)bufp;

    if (tbufp->Index + sizeof(swi_uint16) <= tbufp->Size)
    {
        swi_uint8 *plocal = tbufp->Datap + tbufp->Index;
        *valuep = piget16Be( &plocal );
        tbufp->Index += sizeof(swi_uint16);
    }
    else
    {
        return eQCWWAN_ERR_MEMORY;
    }

    return eQCWWAN_ERR_NONE;
}

/*************
 *
 * Gets a Long value in TBuffer in big endian format at given index and
 * increment Index to next
 *
 * \param   bufp    pointer to qmTBuffer structure
 * \param   valuep  Pointer to location to return value gotten.
 *
 * \return  eQCWWAN_ERR_NONE      - Get LONG successful
 * \return  eQCWWAN_ERR_MEMORY    - Get LONG beyond allowed size attempted
 *
 **************/
global enum eQCWWANError GetLongBe(
    swi_uint8 *bufp,
    swi_uint32 *valuep )
{
    struct qmTBuffer *tbufp = (struct qmTBuffer*)bufp;

    if (tbufp->Index + sizeof(swi_uint32) <= tbufp->Size)
    {
        swi_uint8 *plocal = tbufp->Datap + tbufp->Index;
        *valuep = piget32Be( &plocal );
        tbufp->Index += sizeof(swi_uint32);
    }
    else
    {
        return eQCWWAN_ERR_MEMORY;
    }

    return eQCWWAN_ERR_NONE;
}

/*************
 *
 * Gets a LONGLONG value in TBuffer in big endian format at given index and
 * increment Index to next
 *
 * \param   bufp    pointer to qmTBuffer structure
 * \param   valuep  Pointer to location to return value gotten.
 *
 * \return  eQCWWAN_ERR_NONE      - Get LONGLONG successful
 * \return  eQCWWAN_ERR_MEMORY    - Get LONGLONG beyond allowed size attempted
 *
 **************/
global enum eQCWWANError GetLongLongBe(
    swi_uint8 *bufp,
    swi_uint64 *valuep )
{
    struct qmTBuffer *tbufp = (struct qmTBuffer*)bufp;

    if (tbufp->Index + sizeof(swi_uint64) <= tbufp->Size)
    {
        swi_uint8 *plocal = tbufp->Datap + tbufp->Index;
        *valuep = piget64Be( &plocal );
        tbufp->Index += sizeof(swi_uint64);
    }
    else
    {
        return eQCWWAN_ERR_MEMORY;
    }

    return eQCWWAN_ERR_NONE;
}

/*************
 *
 * Gets the Tlv Type from a qmTBuffer structure
 *
 * \param   bufp(IN)    pointer to qmTBuffer structure
 * \param   valuep(OUT) pointer to TLV Type storage location
 *
 * \return  eQCWWAN_ERR_NONE    - TLV Type successfully extracted
 * \return  eQCWWAN_ERR_MEMORY  - TLV Type extraction failed
 *
 * \note    This function is used by QMI message unpacking routines
 * *
 **************/
package enum eQCWWANError GetTlvType(
    swi_uint8 *bufp,
    swi_uint8 *valuep )
{
    struct qmTBuffer *tbufp = (struct qmTBuffer*)bufp;
    swi_uint16 lIndex = 0;

    /* rewind index to point to the TLV Type */
    lIndex = tbufp->Index - QMITLV_HDR_SZ;
    if (lIndex + sizeof(swi_uint8) <= tbufp->Size)
        *valuep = tbufp->Datap[lIndex];
    else
        return eQCWWAN_ERR_MEMORY;

    return eQCWWAN_ERR_NONE;
}

/*************
 *
 * Gets a swi_uint16 value in TBuffer to know the length of the string which is
 * not NULL terminated. The length is obtained by peeking two indexes behind.
 *
 * This function will be used by routines which unpack the TLVs which
 * have string as the value and are not NULL terminated.
 *
 * \param   bufp    pointer to qmTBuffer structure
 * \param   valuep  Pointer to location to return value gotten.
 *
 * \return  eQCWWAN_ERR_NONE      - Get StringLen successful
 * \return  eQCWWAN_ERR_MEMORY    - Get StringLen allowed size attempted
 *
 **************/
package enum eQCWWANError GetStringLen(
    swi_uint8 *bufp,
    swi_uint16 *valuep )
{
    struct qmTBuffer *tbufp = (struct qmTBuffer*)bufp;
    swi_uint16 lIndex;

    /* Intialise the Index to two indexes before */
    /* Now we are in the current index where the length of the TLV is defined */
    lIndex = tbufp->Index - QMITLV_LENGTH_SZ;
    if (lIndex + sizeof(swi_uint16) <= tbufp->Size)
    {
        swi_uint8 *plocal = tbufp->Datap + lIndex;
        *valuep = piget16( &plocal );
    }
    else
    {
        return eQCWWAN_ERR_MEMORY;
    }
    return eQCWWAN_ERR_NONE;
}

/*************
 *
 * This function gives the type of the file from the known file types.
 *
 * \param   path    path where the file is placed with file name
 *
 * \return  File type          - known file type
 * \return  FW_IMG_TYPE_INVLD  - If its a unnown file type
 *
 **************/
global enum fw_image_type_e qmGetFileType(
    const swi_char *path )
{
    swi_int16 len = strlen(path);

    if( 0 == slstrncmp( ( path + (len-4) ), ".cwe", 4) )
        return FW_IMG_TYPE_CWE;

    if( 0 == slstrncmp( ( path + (len-4) ), ".mbn", 4) )
        return FW_IMG_TYPE_MBN;

    return FW_IMG_TYPE_INVLD;
}

/*************
 *
 * This function performs a cross check to ensure that the received TLV
 * parameter length is correct as per the QMI specification
 *
 * \param   bufp        pointer to qmTBuffer structure
 * \param   TlvLength   lenth of the TLV according to spec .
 *
 * \return  eQCWWAN_ERR_NONE               - Get StringLen successful
 * \return  eQCWWAN_ERR_MALFORMED_QMI_RSP  - The TLV is malformed
 *
 **************/
global enum eQCWWANError qmQmiVerifyResponseTLVlength(
    struct qmTBuffer *bufp,
    swi_uint16 TlvLength )
{
    swi_uint16 lTlvLength;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = GetStringLen ((swi_uint8 *)bufp, &lTlvLength);
    if (eRCode != eQCWWAN_ERR_NONE)
        return eRCode;

    if ( lTlvLength != TlvLength )
        eRCode = eQCWWAN_ERR_MALFORMED_QMI_RSP;

    return eRCode;
}

/*************
 *
 * This function performs returns the received TLV parameter
 * length from the response buffer
 *
 * \param   bufp        pointer to qmTBuffer structure
 *
 * \return
 *          - actual TlvLength from the response buffer
 *          - 0 - Otherwise
 *
 **************/
global swi_uint16 qmQmiGetResponseTLVlength(
    struct qmTBuffer *bufp )
{
    swi_uint16 lTlvLength;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    eRCode = GetStringLen ((swi_uint8 *)bufp, &lTlvLength);
    if (eRCode != eQCWWAN_ERR_NONE)
        return 0;
    else
        return lTlvLength;
}

/*
 * Copies a string of bytes from a QMI TLV into a user supplied buffer. The
 * number of bytes copied from the QMI TLV is equal to the TLV's Length
 * parameter plus one additional byte for the NULL terminator appended to the
 * end of the string.
 *
 * \param   pTlvData      - [IN]  Pointer to the buffer which has the TLV Value
 *
 * \param   pInBuffer     - [IN]  Buffer where the parameter has to be extracted
 *
 * \param   lengthBuffer  - [IN]  Maximum Length of the buffer
 *
 * \return: Length of Bytes written into the buffer
 *
 */
global enum eQCWWANError qmQmiExtractString(
    swi_uint8 *pTlvData,
    swi_char  *pInBuffer,
    swi_uint32 lengthBuffer )
{
    swi_uint16 lBufferSize = 0;
    swi_uint16 TlvLength;
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;

    /* Get the Length of the string as the string will not be NULL terminated */
    eRCode = GetStringLen( pTlvData, &TlvLength );

    /* Check for the Buffer Undersize Error */
    if( (eRCode == eQCWWAN_ERR_NONE) &&
        (TlvLength >= lengthBuffer) )
    {
        eRCode = eQCWWAN_ERR_BUFFER_SZ;
    }

    /* Null String, No Memory allocation or Buffer Undersize Error */
    if ( (TlvLength == 0) ||
         (pInBuffer == NULL) ||
         (eRCode != eQCWWAN_ERR_NONE) )
    {
        return eRCode;
    }

    /* Copy till End of Tlv */
    while( eQCWWAN_ERR_NONE ==
           ( eRCode = GetByte( pTlvData,
                               (swi_uint8 *)&pInBuffer[lBufferSize++])) )
    {
        /* Break Condition */
        if ( lBufferSize == TlvLength )
            break;
    }

    /* null terminated string */
    pInBuffer[lBufferSize] = EOS;

    return eRCode;
}

/*************
 *
 * Name:    qm_result_code_tlv_unpack
 *
 * Purpose: Unpack result code TLV Value
 *
 * Parms:   (IN)    psource - source data
 *          (OUT)   pdest   - destination buffer
 *
 * Return:  eQCWWAN_ERR_NONE on success
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
global enum eQCWWANError
qm_result_code_tlv_unpack(
    swi_uint8 *psrc,
    swi_uint8 *pdest )
{
    enum eQCWWANError rc;
    struct qm_qmi_response_tlvs_values *ptlv =
        (struct qm_qmi_response_tlvs_values *)pdest;

    ptlv->qmiresult.validresponse = FALSE;

    rc = GetWord((swi_uint8 *)psrc, &ptlv->qmiresult.result);
    if( rc != eQCWWAN_ERR_NONE )
    {
       return rc;
    }

    if( ptlv->qmiresult.result != eQCWWAN_ERR_NONE &&
        ptlv->qmiresult.result != eQCWWAN_ERR_GENERAL )
    {
        return eQCWWAN_ERR_INVALID_QMI_RSP;
    }

    rc = GetWord((swi_uint8 *)psrc, &ptlv->qmiresult.error);
    if( rc != eQCWWAN_ERR_NONE )
    {
       return rc;
    }

    ptlv->qmiresult.validresponse = TRUE;
    return rc;
}

/*
 * Unpack response TLV into given buffer
 *
 * \param   bufp        Pointer to source TLV data to be unpacked.
 *
 * \param   resp        Pointer to storage provided by the application for
 *                      unpacked TLV data.
 *
 * \param   pItem       Pointer to map of (TLV item, callback) pairs that are used to
 *                      unpack the QMI response message's TLV parameters.
 *
 * \param   messageId   QMI message ID - unused for now. May be used to check
 *                      that we are getting the correct results from the modem.
 *
 * \return  eQCWWAN_ERR_NONE
 * \return  eQCWWAN_ERR_MEMORY
 *
 */
package enum eQCWWANError qmunpack(
    swi_uint8 *bufp,
    swi_uint8 *resp,
    struct qmTlvUnpackerItem *pItem,
    const swi_uint16 messageId )
{
    enum eQCWWANError eRCode = eQCWWAN_ERR_NONE;
    struct qmTBuffer tbuf;
    struct qmTBuffer *tbufp = &tbuf;

    /* Initialize our local buffer */
    tbuf.Index = 0;

    /* extract QMI Service message header fields */
    swi_int16 msgid = piget16(&bufp);
    tbuf.Size = piget16(&bufp);

    /* To Do - decide action to take on invalid message id */
    if( msgid != messageId )
    {
    }

    /* Update our buffer to point to QMI message TLVs */
    tbuf.Datap = bufp;

    /* Index is now at QMI SDU. */
    while (tbufp->Index != tbufp->Size)
    {
        swi_uint8 tlvType;
        swi_uint16 tlvLen;
        struct qmTlvUnpackerItem *pUnpackerItem = pItem;

        eRCode = GetByte((swi_uint8*)tbufp, &tlvType);
        if (eRCode)
        {
            return eRCode;
        }

        eRCode= GetWord((swi_uint8*)tbufp, &tlvLen);
        if (eRCode)
        {
            return eRCode;
        }

        swi_uint16 lIndex = tbufp->Index;
        while (pUnpackerItem->tlvType != eTLV_TYPE_INVALID)
        {
            if (pUnpackerItem->tlvType == tlvType)
            {
                /*  Recognized TLV so let derived class unpack it. */
                /* To compensate, skip our main buffer over this tlv so we are
                 * ready for next loop iteration.
                 */
                eRCode = (*pUnpackerItem->Unpacker)((swi_uint8*)tbufp, resp);
                if (eRCode != eQCWWAN_ERR_NONE)
                {
                     return eRCode;
                }
                /* The Derived Class has not unpacked the TLV */
                /* We need to increment the index to the next TLV */
                else if ( lIndex == tbufp->Index )
                {
                    IncrementIndex(tbufp, tlvLen);
                }
                break;
             }
            pUnpackerItem++;
        }

        if (pUnpackerItem->tlvType == eTLV_TYPE_INVALID)
        {
            /* Unrecognized TLV. According to specification we should
             * just ignore it and continue.
             */
            IncrementIndex(tbufp, tlvLen);
        }
    } /* while */

    return eRCode;
}

/*
 * Repeatedly build TLV into given buffer
 *
 * \param   bufp        Pointer to storage into which the QMI service message
 *                      will be packed.
 *
 * \param   reqp        Pointer to parameter list structure passed in from an
 *                      SLQS API. These parameters are used by the TLV packing
 *                      functions for populating the TLV Value fields.
 *
 * \param   pItem       Pointer to map of (TLV item, callback) pairs that are used to
 *                      construct the QMI message TLV parameters.
 *
 * \param   messageId   QMI message ID to insert into the header of the buffer
 *
 * \param   mLengthp    Storage location for the length of the QMI service message VALUE field.
 *
 * \return  eQCWWAN_ERR_NONE
 * \return  eQCWWAN_ERR_MEMORY
 *
 */
global enum eQCWWANError qmbuild(
    swi_uint8 *bufp,
    swi_uint8 *reqp,
    struct qmTlvBuilderItem *pItem,
    const swi_uint16 messageId,
    swi_uint16 *mLengthp )
{
    enum eQCWWANError eRCode;
    struct qmTBuffer tbuf;
    struct qmTBuffer *tbufp = &tbuf;

    /* Initialize our local buffer */
    tbuf.Size = (AMMAXREQBKLEN - AMQMIREQTOTAL);
    tbuf.Index = 0;
    tbuf.Datap = bufp;

    /* Set return length to zero */
    *mLengthp = 0;

    /* Save header location, we cannot fill it until QMI SDU has been built. */
    swi_uint16 headerIndex = tbufp->Index;
    /* Skip to QMI SDU. */
    eRCode = IncrementIndex (tbufp, QMISVC_MSGHDR_SZ);

    /* Add each TLV to the SDU.*/
    while (pItem->tlvType != eTLV_TYPE_INVALID)
    {
        /* point to start of TLV header */
        swi_uint16 tlvHeaderIndex = tbufp->Index;

        /* Skip over TLV header. We'll fix it up when we know TLV length. */
        eRCode = IncrementIndex (tbufp, QMITLV_HDR_SZ);

        /* Call individual build function */
        eRCode = (*pItem->Builder)((swi_uint8*)tbufp, reqp);
        if (eRCode != eQCWWAN_ERR_NONE)
            return eRCode;

        swi_uint16 tlvLength = tbufp->Index - (tlvHeaderIndex + QMITLV_HDR_SZ);

        if (tlvLength > 0)
        {
            // Fix TLV header.
            eRCode = PokeByte((swi_uint8*)tbufp, tlvHeaderIndex, pItem->tlvType);
            eRCode = PokeWord((swi_uint8*)tbufp, tlvHeaderIndex + QMITLV_TYPE_SZ, tlvLength);
        }
        else
        {
            /*
             * NB: To the best of our knowledge a valid TLV should never
             * have a length of 0 (refer to QCT CR#00417479). Unfortunately,
             * QCT QMI documentation does not explicitly state a rule here.
             */

             /* We added nothing so just undo the previous IncrementIndexB() */
            eRCode = IndexB((swi_uint8*)tbufp, tlvHeaderIndex);
        }
        pItem++;
    }

    /* Calculate SDU length.*/
    *mLengthp = (tbufp->Index - QMISVC_MSGHDR_SZ);

    /* Now fill in the QMI header. */
    eRCode = PokeWord((swi_uint8*)tbufp, headerIndex, messageId);
    eRCode = PokeWord((swi_uint8*)tbufp, headerIndex + QMISVC_MSGID_SZ, *mLengthp);
    return eRCode;
}
