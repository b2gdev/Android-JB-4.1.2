/*
 *    Filename: mmudefs.h
 *
 *    Purpose:  User definitions for the MM package
 *
 *    Copyright: C 2008 Sierra Wireless All rights reserved
 *
 */

#ifndef mmudefs_h
#define mmudefs_h

#include "os/swi_ossdk.h"
#include "dl/dludefs.h"
#include "er/erudefs.h"

/*
 * Name:    mmretcode
 *
 * Purpose: Enumerated list of return codes from the MM package
 *          to callers of its global entry points
 *
 * Members: MMDONE     - Successful return, requested operation complete
 *          MMNOMEM    - Unable to allocate memory from the operatin 
 *          MMBADPARM  - There is a problem with one of the calling args
 *          MMBADPOOL  - The poolp is wrong or the pool is not initialized
 *          MMBADBUF   - The buffer pointer does not point to a valid buffer
 *          MMPOOLEMPTY - All the free blocks in the pool have been allocated
 *
 * Note:    The first and last entries in the list are for range checking
 *          purposes, if required
 *
 */
enum mmretcode {
    MMRETMIN = 0,
    MMDONE = MMRETMIN,
    MMNOMEM, 
    MMBADPARM,
    MMBADPOOL,
    MMBADBUF,
    MMPOOLEMPTY,
    MMRETMAX
};

/*
 * Name:    mmpool
 *
 * Purpose: This definition is provided so that users can allocate
 *          sufficient storage for their memory pool structures. 
 *          However, users are forbidden from manipulating the
 *          contents of this structure and can only pass a pointer
 *          to it into MM package global functions when
 *          required.
 *
 * Members: mmnextp - Pointer to the pool header structure of the 
 *                    neighbouring pool (optional for debugging)
 *          mmfreep - Pointer to the free block list. 
 *          mmnamep - Pointer to a NULL-terminated character string
 *                    containing the name of this pool
 *          mmvalid - A sentinel value that indicates whether the 
 *                    pool has been initialized or not
 *          mmnumbks- Number of blocks in the pool
 *          mmblocksz - Size of the User-portion of each block
 *          mmfirstp - Address of the first block for debugging purpose
 *          mmlastp - Address of the last block in the pool
 *          mmsemaph - Semaphore used to make accessing a pool a thread-
 *                    safe operation
 *          mmblog  - flag to mark pool for logging 
 *
 * Notes:   The free list is implemented as a singly-linked list
 *          so that the "mmfreep" points to the *last* block in 
 *          the list and the last block always points to the 
 *          first block in the list. 
 *
 *          The "mmbufhdr" pointer is defined within the mmidefs.h
 *          file and external packages don't need to know about it
 *
 */
struct mmpool
{
    struct mmpool *mmnextp;
    struct mmbufhdr *mmfreep;
    char *mmnamep;
    swi_uint32 mmvalid;
    swi_uint32 mmnumblocks;
    swi_uint32 mmblocksize;
    swi_uint8 *mmfirstp;
    swi_uint8 *mmlastp;
    struct swi_ossemaphore mmsemaph;
    swi_bool    mmblog;
};

#include "mmuproto.h"

#endif
