/*
 *    Filename: mm.c
 *
 *    Purpose:  This package implements the memory manager, a package which
 *              allows other packages to create pools of fixed-length buffers
 *              and allocate and free these buffers during runtime.
 *
 *    Copyright Sierra Wireless Inc., 2008 All rights reserved
 *
 *    Notes:    The user of the queue is required to provide its own concurrent
 *              control in the way of locks and such.
 */
#include "aa/aaglobal.h"
#include "mm/mmidefs.h"
#include "mm/mmudefs.h"
#include <stdio.h>

/* Global storage location for the chain of defined pool blocks
 * Entries are filled into this pointer in first-come-first served
 * order. This list is maintained for debugging purposes only
 */
struct mmpool *mmpoolchain;

/* Logging user block */
struct dlusercb    mmdlcb;

/*************
 *
 * Name:    mmplogging -  enable/disable logging
 *
 * Purpose: To enable/disable logging for a given pool
 *
 * Parms:   poolp      - pointer to a user-supplied pool control block
 *          logging    - logging action, TRUE = enable, FALSE = disable
 *
 * Return:  None
 *
 * Abort:   NA
 *
 * Notes:   None
 *
 **************/
global void mmplogging( struct mmpool *poolp, swi_bool logging )
{
    /* Turn logging on or OFF */
    poolp->mmblog = logging;
}
/*************
 *
 * Name: mmlink
 *
 * Purpose: To add (link) a free buffer back into the specified list
 *
 * Parms:   qheadp      - Pointer to the location of the pointer to the
 *                        last entry in the queue
 *          bufferp     - Pointer to the buffer to be linked to the queue
 *
 * Return:  None
 *
 * Abort:   none
 *
 * Notes:   The queue being manipulated is singly-linked, so the queue header
 *          always points to the last block in the free list, or NULL if the
 *          list is empty
 *
 **************/
local void mmlink( struct mmbufhdr **lastpp, struct mmbufhdr *bufferp )
{
    /* Check the empty queue case first */
    if( *lastpp == NULL )
    {
        /* The queue header now points to the newcomer */
        *lastpp = bufferp;

        /* The incoming buffer is both the last and the first */
        bufferp->mmnextfreep = bufferp;
    }
    else
    {
        /* The incoming buffer pointer becomes the last one so
         * make it point to the first buffer in the list
         */
        bufferp->mmnextfreep = (*lastpp)->mmnextfreep;

        /* The old last buffer needs to point to this new one as
         * the next in the chain
         */
        (*lastpp)->mmnextfreep = bufferp;

        /* Then update the queue header to point to the incoming buffer */
        *lastpp = bufferp;
    }
}

/*************
 *
 * Name: mmunlink
 *
 * Purpose: To remove (unlink) a buffer from the specified list
 *
 * Parms:   qheadp      - Pointer to the location of the pointer to the
 *                        last entry in the queue
 *
 * Return:  Returns a pointer to the free buffer just removed, or NULL if
 *          the queue is empty
 *
 * Abort:   none
 *
 * Notes:   The queue being manipulated is singly-linked, so the queue header
 *          always points to the last block in the free list, or NULL if the
 *          list is empty
 *
 **************/
local struct mmbufhdr *mmunlink( struct mmbufhdr **lastpp )
{
    struct mmbufhdr *tempp;  /* Temporary storage for link manipulation */

    /* If the queue is empty, nothing to do. Return NULL */
    if( *lastpp == NULL )
    {
        tempp = NULL;
    }
    else
    {
        /* The queue is not empty, so remove the first
         * buffer in the list. Pointed to by the last one
         */
        tempp = (*lastpp)->mmnextfreep;

        /* If the last buffer points to itself then it is the
         * only buffer in the queue which will become empty
         * after this operation
         */
        if( (*lastpp) == tempp )
        {
            /* Empty queues contain NULL pointers */
            *lastpp = NULL;
        }
        else
        {
            /* Queue had more than 1 buffer in it */
            (*lastpp)->mmnextfreep = tempp->mmnextfreep;
        }
    }
    return tempp;
}

/*************
 *
 * Name:    mmpcreate -  create a fixed size memory pool
 *
 * Purpose: To create a pool of fixed size memory blocks from the parameters
 *          specified
 *
 * Parms:   poolp      - pointer to a user-supplied pool control block
 *          pnamep     - Pointer to a string containing the pool name
 *          numblocks  - Number of blocks the pool must contain
 *          reqblocksz - Requested size of each of the fixed blocks in the pool
 *
 * Return:  None
 *
 * Abort:   Bad calling parameter, no heap memory left
 *
 * Notes:   "pnamep" must be stored in persistent memory, the
 *          string it points to cannot disappear after this function
 *          returns.
 *
 **************/
global void mmpcreate( struct mmpool *poolp,
                                 char *pnamep,
                                 swi_uint32 numblocks,
                                 swi_uint32 reqblocksz )
{
    swi_uint32 index, blocksize;
    swi_uint32 poolmax;
    struct mmbufhdr *hdrp;
    swi_uint8 *memp;

    /***** Validate the arguments *****/

    if( poolp == NULL || pnamep == NULL || numblocks == 0 || reqblocksz == 0 )
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d bad parameter",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, 0 );
    }

    /***** Compute how much memory to allocate *****/

    /* Need to round up the requested size to the next 4-byte boundary */
    blocksize = (reqblocksz + ALIGN4) & ~(ALIGN4);

    /* Add in the invisible header. Remember, the mmbufhdr
     * structure must always be a multiple of 4 bytes in length
     */
    blocksize += sizeof( struct mmbufhdr );

    /* Blocksize now contains the amount of memory per block.
     * Calculate the total amount of memory needed for this
     * pool
     */
    poolmax = blocksize * numblocks;

    /* Ready to allocate the requested memory */
    if( (memp = (swi_uint8 *) swi_ossdkmemory( (swi_size_t)poolmax )) == NULL )
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d no free memory",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, 0 );
    }

    /* Initialize the header structure */
    poolp->mmnextp = NULL;        /* No next pool of buffers yet */
    poolp->mmnamep = pnamep;
    poolp->mmfreep = (struct mmbufhdr *) memp; /* below, this loc'n is u/d'ed */
    poolp->mmvalid = MMPOOLVALID;
    poolp->mmfirstp = memp;         /* Save address of the first free block */
    poolp->mmnumblocks = numblocks; /* Save caller's info for debug purposes */
    poolp->mmblocksize = reqblocksz;
    poolp->mmblog = FALSE;          /* Logging is OFF by default */

    /* Initialize the semaphore which is used to guarantee
     * buffer additions and removals are atomic operations
     */
    swi_ossdksemaphoreinit( &poolp->mmsemaph );

    /* Update the pool chain pointer */
    if( mmpoolchain == NULL )
        mmpoolchain = poolp;

    /* Insert this new pool pointer before the current one. The
     * effect of this is that pool pointers appear in this list
     * in the reverse order to which they were defined
     */
    else
    {
        poolp->mmnextp = mmpoolchain;
        mmpoolchain = poolp;
    }

    /***** Go through the raw memory and format each block *****/

    /* For each requested block, format the header and link the
     * block onto the free chain. We'll initialize the pool
     * starting with the first block and advancing through the
     * list toward the last one
     */
    hdrp = poolp->mmfreep;
    for( index = 0; index < numblocks; index++ )
    {
        /* Fill in the invisible header section */
        hdrp->mmsignature = MMSIGNATURE;

        /* Install a pointer back to the parent pool structure */
        hdrp->mmpoolp = poolp;

        /* Move memp to the next block in the list. NOTE: The
         * code calculating blocksize makes sure the size is
         * always on a 4-byte boundary. If that changes, then
         * this algorithm will be broken
         */
        memp += blocksize;

        /* Now point the current block to the next one */
        hdrp->mmnextfreep = (struct mmbufhdr *) memp;

        /* Now set the header to point to the next block
         * and repeat this once for each block in the freelist
         */
        hdrp = hdrp->mmnextfreep;
    }

    /* After the for loop, we need to complete the iniitalization
     * of the singly-linked list, which means both the queue head and
     * last block need to point to the first block in the list. We
     * are now one past the last block, so backup to it and then
     * install the pointer to the first block which we know from
     * the header. Then update the header to point at the last block
     */
    hdrp = (struct mmbufhdr *) (memp - blocksize);
    /* Update the last block's "next" pointer to point
     * to the last block in the free list.
     */
    hdrp->mmnextfreep = poolp->mmfreep;
    /* Set the address of the last block in the pool */
    poolp->mmlastp = (swi_uint8 *) hdrp;
    /* Set the corrected pointer to the free block linked list */
    poolp->mmfreep = hdrp;

    return;
}


/*
 *    Name: mmbufget
 *
 *    Purpose:    To obtain a memory block from the free list and return
 *                a pointer to it to the caller
 *
 *    Parameters: poolp   - Pointer to the pool to be used to allocate the
 *                          block
 *
 *    Returns:    NULL    - No blocks available
 *                        - Otherwise a pointer to the allocated buffer
 *
 *    Abort:      Invalid pool handle
 *
 *    Notes:      This call is an atomic operation and the calling task
 *                will be blocked if another is currently accessing this
 *                pool.
 *
 */
global swi_uint8 *mmbufget( struct mmpool *poolp )
{
    swi_uint8 *retval=NULL;  /* Assumes the pool will be empty */
    struct mmbufhdr *bufhdrp;

    /* Obtain exclusive access to the pool */
    swi_ossdksemaphorelock( &poolp->mmsemaph );

    if( poolp->mmvalid != MMPOOLVALID )
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d Invalid pool pointer",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, (swi_uint32) poolp );

        /* Not sure whether this call is needed. Shouldn't
         * be, but putting it here can't hurt anything
         */
        swi_ossdksemaphoreunlock( &poolp->mmsemaph );
    }

    /* This test is part of the critical section */
    else if ( poolp->mmfreep != NULL )
    {
        /* There is at least one block in the list */
        bufhdrp = mmunlink( &poolp->mmfreep );

        /* Skip past the header to the flat area */
        retval = (swi_uint8 *) (bufhdrp += 1);
    }
    /* Unlock, we're done with accessing the critical section */
    swi_ossdksemaphoreunlock( &poolp->mmsemaph );

    /* Log action if logging enabled */
    if (poolp->mmblog)
    {
        dlLog(  &mmdlcb, DLCLASSALL, "mmbufget() %s, 0x%09x\n",
                (swi_uint32)poolp->mmnamep,
                (swi_uint32)retval,
                (swi_uint32)NULL,
                (swi_uint32)NULL);
    }
    return retval;
}

/*
 *    Name: mmbufrel
 *
 *    Purpose:    To release an allocated buffer to the free pool when
 *                the caller is done with it
 *
 *    Parameters: bufferp  - Pointer to a previously-allocated memory buffer
 *
 *    Abort:      Pool invalid
 *                Invalid buffer
 *
 *    Returns:    None
 *
 *    Notes:      This call will block if the pool is already in use
 *                by another thread
 *
 */
global void mmbufrel( swi_uint8 *bufferp )
{
    struct mmbufhdr *bufhdrp;

    /* Back up the caller's buffer to the hidden area */
    bufhdrp = ( struct mmbufhdr *)  (((struct mmbufhdr *) bufferp) - 1);

    /* Make sure the block is a properly-initialized buffer */
    if( bufhdrp->mmsignature != MMSIGNATURE )
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d invalid buffer",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, (swi_uint32) bufferp );
    }

    /* Make sure the pool structure is also valid.
     * Note, no need to lock access yet because we're just reading
     * whether the pool has been initialized and this won't ever
     * change after SDK initialization is complete
     */
    if( bufhdrp->mmpoolp->mmvalid != MMPOOLVALID )
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d invalid pool",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, (swi_uint32) bufhdrp->mmpoolp );
    }

    /* Obtain exclusive access to the pool */
    swi_ossdksemaphorelock( &bufhdrp->mmpoolp->mmsemaph );

    /* Put this buffer back onto the free list */
    mmlink( &bufhdrp->mmpoolp->mmfreep, bufhdrp );

    /* Unlock, we're done with accessing the critical section */
    swi_ossdksemaphoreunlock( &bufhdrp->mmpoolp->mmsemaph );

    /* Log action if logging enabled */
    if (bufhdrp->mmpoolp->mmblog)
    {
        dlLog(  &mmdlcb, DLCLASSALL, "mmbufrel() %s, 0x%09x\n",
                (swi_uint32)bufhdrp->mmpoolp->mmnamep,
                (swi_uint32)bufferp,
                (swi_uint32)NULL,
                (swi_uint32)NULL);
    }

}

/*************
 *
 * Name: mminit
 *
 * Purpose: To initialize the MM package at system startup
 *
 * Parms:   None
 *
 * Return:  None
 *
 * Abort:   none
 *
 * Notes:   Called once during system startup to initialize the MM package
 *
 **************/
global void mminit( void )
{
    /* Initialize the pointer to the pool chain */
    mmpoolchain = NULL;

    /* Register for logging, initially enabled */
    dlregister( "MM", &mmdlcb, TRUE );

}
