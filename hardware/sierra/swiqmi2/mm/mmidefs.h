/*
 *    Filename:    mmidefs.h
 *
 *    Purpose:     Internal definitions for the MM package
 *
 *    Copyright: C 2008 Sierra Wireless All rights reserved
 *
 */

/* Internal Definitions */

#define MMPOOLVALID 0xDEADBEEF
#define MMSIGNATURE 0xBEEFCAFE

/*
 * Name:    mmbufhdr
 *
 * Purpose: This structure is the header for individual pool buffers.
 *          It resides at the very top of each pool block, behind the
 *          pointer a user gets and it always contains valid information
 *          about the pool block once the pool has been initialized
 *
 * Members: mmsignature - A fixed amount of storage containing the same
 *                        hexadecimal signature for every block. This
 *                        field is checked every time the MM package 
 *                        is requested to perform an operation on the 
 *                        buffer. If the signature is incorrect then 
 *                        an error is declared and the processor is 
 *                        brought down
 *          mmnextfreep - Pointer to the next free block in the list
 *          mmpoolp     - Pointer to the pool structure for the pool
 *                        of which this block is a member. When this 
 *                        block is released to the free pool, this 
 *                        pointer is used to find the appropriate pool
 *
 * Notes: If the contents of this structure change, make sure the size
 *        is always a multiple of 4 bytes. 
 *
 */
struct mmbufhdr
{
    swi_uint32 mmsignature;
    struct mmbufhdr *mmnextfreep;
    struct mmpool *mmpoolp;
};

#include "mmiproto.h"
