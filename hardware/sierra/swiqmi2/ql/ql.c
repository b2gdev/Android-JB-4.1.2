/*
 *    Filename: ql.c
 *
 *    Purpose:  doubly-linked queue manager
 *
 *    Copyright Sierra Wireless Inc., 2008 All rights reserved
 *
 *    Notes: The user of the queue is required to provide its own concurrent
 *           control in the way of locks and such.
 */
#include "aa/aaglobal.h"
#include "ql/qludefs.h"

/*
 *    Name: qlinitlnk
 *
 *    Purpose: To initialize a queue link
 *
 *    Param:   linkp - pointer to a queue link
 *             userp - pointer to user data to be linked
 *
 *    Returns: Nothing
 *
 *     Abort:  None
 *
 *    Notes:   None
 */
global void qlinitlnk( struct qllink *linkp, void *userp )
{
    /* initialize queue link pointer to point as itself */
    linkp->qlnextp = linkp;
    linkp->qlprevp = linkp;

    /* set the user data pointer in the queue link */
    linkp->qluserp = userp;
}

/*
 *    Name: qladdnext 
 *
 *    Purpose: To add a new queue link "next" to the current link in the queue
 *
 *    Param: curlinkp - pointer to the current queue link
 *           newlinkp - pointer to the new queue link to be added next to 
 *                      the current one
 *    Returns: Nothing
 *
 *    Abort:   None
 *
 *    Notes:   qlinit() must be called to initialize a link before it can 
 *             be added to the queue
 */

global void qladdnext(struct qllink *curlinkp, struct qllink *newlinkp)
{
    /* add new link to queue */
    newlinkp->qlnextp = curlinkp->qlnextp;
    newlinkp->qlprevp = curlinkp;
    newlinkp->qlnextp->qlprevp = newlinkp;
    curlinkp->qlnextp = newlinkp;
} 

/*
 *    Name: qladdprev
 *
 *    Purpose: To add a new queue link previous to the current link in 
 *             the queue
 *
 *    Param:   curlinkp - pointer to the current queue link
 *             newlinkp - pointer to the new queue link to be added
 *                        next to the current one
 *    Returns: Nothing
 *
 *    Abort:   None
 *
 *    Notes:   qlinit() must be called to initialize a link before it can 
 *             be added to the queue
 */

global void qladdprev(struct qllink *curlinkp, struct qllink *newlinkp)
{
    /* add new link to queue */
    newlinkp->qlprevp = curlinkp->qlprevp;
    newlinkp->qlnextp = curlinkp;
    newlinkp->qlprevp->qlnextp = newlinkp;
    curlinkp->qlprevp = newlinkp;

}

/*
 *    Name: qlgetnext
 *
 *    Purpose: To retrieve the pointer to the next user data in the queue
 *
 *    Param:   curlinkp - pointer to the current queue link
 *
 *    Returns: the next user data pointer in the queue
 *
 *    Abort:   None
 *
 *    Notes:   None
 *                
 */

global void *qlgetnext( struct qllink *curlinkp)
{
    /* return pointer to the next user data */
    return(curlinkp->qlnextp->qluserp);

}

/*
 *    Name: qlgetprev
 *
 *    Purpose: To retrieve the pointer to the previous user 
 *             data in the queue
 *
 *    Param:   curlinkp
 *
 *    Returns: the previous user data pointer in the queue
 *
 *    Abort:   None
 *
 *    Notes:   None
 *                
 */

global void *qlgetprev( struct qllink *curlinkp)
{
    /* return pointer to the previous user data */
    return(curlinkp->qlprevp->qluserp);
}

/*
 *    Name:    qlremnext 
 *
 *    Purpose: To remove a queue link next to the current one
 *
 *    Param:   curlinkp - pointer to current queue link
 *
 *    Returns: the user data pointer of the removed queue link
 *
 *    Abort:   None
 *
 *    Notes:   None
 */
global void *qlremnext( struct qllink *curlinkp)
{
    struct qllink *remlinkp;    /* pointer to the queue link to be removed */

    /* get pointer to the queue link to be removed */
    remlinkp = curlinkp->qlnextp;

    /* remove queue link from queue */
    curlinkp->qlnextp = remlinkp->qlnextp;
    remlinkp->qlnextp->qlprevp = curlinkp;

    /* return user data pointer */
    return(remlinkp->qluserp);
}
    

/*
 *    Name: qlremprev
 *
 *    Purpose: To remove the queue link previous to the current one
 *
 *    Param:   curlinkp - pointer to current queue link
 *
 *    Returns: the user data pointer of the removed queue link
 *
 *    Abort:   None
 *
 *    Notes:   None
 */
global void *qlremprev( struct qllink *curlinkp)
{
    struct qllink *remlinkp;    /* pointer to the queue link to be removed */

    /* get pointer to the queue link to be removed */
    remlinkp = curlinkp->qlprevp;

    /* remove queue link */
    curlinkp->qlprevp = remlinkp->qlprevp;
    remlinkp->qlprevp->qlnextp = curlinkp;

    /* return user data pointer of the removed queue link */
    return( remlinkp->qluserp);
}

/*
 *    Name: qlremcur
 *
 *    Purpose: To remove the current queue link 
 *
 *    Param:   curlinkp - pointer to current queue link
 *
 *    Returns: the user data pointer of the removed queue link
 *
 *    Abort:   None
 *
 *    Notes:   None
 *
 */
global void *qlremcur( struct qllink *curlinkp)
{
    struct qllink *nextlinkp;    /* Ptr to the Q link next to current link */

    /* get a pointer to queue link next to the one to be removed */
    nextlinkp = curlinkp->qlnextp;

    /* remove the queue from the list */
    curlinkp->qlprevp->qlnextp = nextlinkp;
    nextlinkp->qlprevp = curlinkp->qlprevp;

    /* return user data pointer of the removed queue link */
    return( curlinkp->qluserp);

}
