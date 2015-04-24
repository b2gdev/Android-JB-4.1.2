/*************
 *
 * Filename:    icsmt_sdk.c
 *
 * Purpose:     The IC package provides facilities for shared-memory
 *              tasks to communicate with one another. For a full
 *              description of this package, refer to the file ic.txt.
 *
 *              This file has no peer as do the icipc_xxx.c files
 *              because communications between shared memory tasks
 *              use the functions in this file exclusively. There is
 *              no need of a peer when all the tasks are threads
 *              running within a user-space process.
 *
 * Copyright:   © 2008 Sierra Wireless, Inc., all rights reserved
 *
 **************/

/* include files */
#include "aa/aaglobal.h"
#include "ic/icidefs.h"
#include <stdio.h>

/* Defines */

/*************
 *
 * Name: icsmt_create
 *
 * Purpose:   This function will allocate the message queue memory and
 *            initialize memory pointers and message queue variables.
 *
 * Parms:     icsmtbp       - pointer to an IC message queue control block
 *            smtqueuenamep - message queue name string [4 characters]
 *            smtqueuedepth - depth of the message queue
 *
 * Return:     no value
 *
 * Abort:     if failed to allocate queue message memory
 *
 * Notes:     The issmtgetp field of the message queue control block will
 *            point at the next message to be retrieved from the front of
 *            the message queue.  This pointer is incremented everytime a
 *            message is removed from the queue.
 *
 *            The icsmtputp field of the message queue control block will
 *            point at the back of the message queue where the next message
 *            should be added.  This pointer is incremented everytime a
 *            new message is added to the queue.
 *
 *            The message queue is empty when icsmtgetp == icsmtputp.
 *
 *            Because the message queue is circular in nature it is necessary
 *            for icsmtcreate() to allocate memory of size depth + 1 otherwise
 *            we would not be able to differentiate between a full and empty
 *            message queue.  There will always be one unused message space in
 *            the message queue.  A message queue will be considered full when
 *            incrementing icsmtputp would cause icsmtgetp == icsmtputp.
 *
 **************/
global void icsmt_create(
    struct icsmtcb *icsmtp,
    char *smtqueuenamep,
    swi_uint16 smtqueuedepth)
{
    /* if message queue depth is not zero the create message queue */
    if (smtqueuedepth)
    {
        /*
         * Allocate depth + 1 memory for the message queue
         */
        icsmtp->icsmtstartp =
            (swi_uint32 *) swi_ossdkmemory( sizeof(swi_uint32) *
                                            (smtqueuedepth + 1));

        /* if the allocation was not successful */
        if (icsmtp->icsmtstartp == NULL)
        {
            char errmsg[100];
            snprintf(errmsg, sizeof(errmsg),
                     "%s:%d Queue alloc failed",
                     (char *)__func__, __LINE__);
            erAbort(errmsg, smtqueuedepth + 1);
        }

        /* initialize message queue pointer */
        icsmtp->icsmtendp = icsmtp->icsmtstartp + smtqueuedepth;
        icsmtp->icsmtgetp = icsmtp->icsmtstartp;
        icsmtp->icsmtputp = icsmtp->icsmtstartp;

        /* Save the name */
        icsmtp->icsmtqnamep = (swi_uint8  *)smtqueuenamep;

        /* create message queue semaphore */
        swi_ossdksemaphoreinit(&icsmtp->icsmtsem);

        /* Create the waiter */
        swi_ossdkwaiterinit( &icsmtp->icsmtwaiter );

        /* Initialize send and receive counters */
        icsmtp->icsmtrcvcnt = 0;
        icsmtp->icsmtsendcnt = 0;
        icsmtp->icsmtmaxqueued = 0;
    }
    /* no message queue should be created */
    else
    {
        /* Clear all message queue variables */
        icsmtp->icsmtstartp = NULL;
        icsmtp->icsmtendp = NULL;
        icsmtp->icsmtgetp = NULL;
        icsmtp->icsmtputp = NULL;
    }
}
/*************
 *
 * Name:      icsmt_send
 *
 * Purpose:   To queue a message to an inter-task message queue
 *
 * Parms:     icsmtp - pointer to the message control structure of the
 *                     task that will receive this message
 *            msg    - the 32-bit message to queue
 *
 * Return:    no value
 *
 * Abort:     if failed to queue message because the queue is full
 *
 * Notes:     If this function adds to an empty queue then the receiving
 *            task will be woken up when this function wakes it.
 *
 *            The icsmtgetp field of the message queue control block will
 *            point at the next message to be retrieved from the front of
 *            the message queue.  This pointer is incremented every time a
 *            message is removed from the queue.
 *
 *            The icsmtputp field of the message queue control block will
 *            point at the back of the message queue where the next message
 *            should be added.  This pointer is incremented every time a
 *            new message is added to the message queue.
 *
 *            The message queue is empty when icsmtgetp == icsmtputp.
 *
 *            There will always be one unused message space in the message
 *            queue.  A message queue will be considered full when incrementing
 *            osputp would cause icsmtgetp == icsmtputp.
 *
 *            The build-in counter for sent messages is incremented.
 *
 **************/
global void icsmt_send( struct icsmtcb *icsmtp, swi_uint32 msg )
{
    swi_uint32 wrapped_delta;

    /* check if a message queue exists */
    if (icsmtp->icsmtstartp == NULL)
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d no msg que",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, 0 );
    }

    /* Obtain exclusivity. Note, this is a blocking call */
    swi_ossdksemaphorelock( &icsmtp->icsmtsem );

    /* put message on the back of the queue */
    *icsmtp->icsmtputp = msg;

    /* increment put message queue pointer */
    if (icsmtp->icsmtputp == icsmtp->icsmtendp)
    {
        icsmtp->icsmtputp = icsmtp->icsmtstartp;
    }
    else
        icsmtp->icsmtputp++;

    /* Increment send count */
    icsmtp->icsmtsendcnt++;

    /* record the maximum messages queued if greater than current */
    if ((icsmtp->icsmtputp > icsmtp->icsmtgetp) &&
        ( (swi_uint32)(icsmtp->icsmtputp - icsmtp->icsmtgetp) ) >
                                                       icsmtp->icsmtmaxqueued )
    {
        icsmtp->icsmtmaxqueued = icsmtp->icsmtputp - icsmtp->icsmtgetp;
    }
    else if (icsmtp->icsmtputp < icsmtp->icsmtgetp)
    {
        /* queue has wrapped, compensate for it */
        wrapped_delta = (icsmtp->icsmtputp +
                         (icsmtp->icsmtendp - icsmtp->icsmtstartp) -
                         icsmtp->icsmtgetp) + 1;   /* ahead by at least 1 */
        if ( wrapped_delta > icsmtp->icsmtmaxqueued)
        {
            icsmtp->icsmtmaxqueued = wrapped_delta;
        }
    }

    /* check if the queue was already full */
    if (icsmtp->icsmtputp == icsmtp->icsmtgetp)
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d msg que full",
                 (char *)__func__, __LINE__);
        erAbort(errmsg,
                (swi_uint32) *((swi_uint32 *) (icsmtp->icsmtqnamep) ));
    }

    /* Relinquish exclusivity */
    swi_ossdksemaphoreunlock( &icsmtp->icsmtsem );

    /* Awaken the task that will receive this message */
    swi_ossdkwaiterwake( &icsmtp->icsmtwaiter );
}

/*************
 *
 * Name:      icsmt_rcv
 *
 * Purpose:   To receive a message from an inter-task message queue
 *
 * Parms:     icsmtp  - Pointer to the receiving task's message control struct
 *            timeout - Timeout duration in ms (0 to wait forever)
 *            msgp    - Pointer to storage into which the received message
 *                      is placed. NULL if the call timed out.
 *
 * Return:    result  - TRUE if message was received
 *                    - FALSE if timeout occurred
 *
 * Abort:     if no message queue exists
 *
 * Notes:     If the queue is empty then the calling task will suspend
 *            attempting to take the semaphore. This function will resume
 *            when icsmt_send() releases the semaphore after adding a message
 *            to the queue.
 *
 *            The icsmtgetp field of the message queue control block will
 *            point at the next message to be retrieved from the front of
 *            the message queue.  This pointer is incremented everytime a
 *            message is removed from the queue.
 *
 *            The icsmtputp field of the message queue control block will
 *            point at the back of the message queue where the next message
 *            should be added.  This pointer is incremented everytime a
 *            new message is added to the queue.
 *            message queue.
 *
 *            The message queue is empty when icsmtgetp == icsmtputp.
 *
 *            There will always be one unused message space in the message
 *            queue.  A message queue will be considered full when incrementing
 *            osputp would cause osgetp == osputp.
 *
 *            The build-in counter for received messages is incremented.
 *
 **************/
global swi_bool icsmt_rcv( struct icsmtcb *icsmtp,
                           swi_uint32 timeout,
                           swi_uint32 *msgp)
{
    swi_bool result;      /* result of calling os functions */

    /* check if a message queue exists */
    if (icsmtp->icsmtstartp == NULL)
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d no msg que",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, 0 );
    }

    /* If message queue does exist check whether it is empty */
    /* Obtain exclusivity. This will block if the semaphore is in use */
    swi_ossdksemaphorelock(&icsmtp->icsmtsem);

    /* On empty queue */
    if (icsmtp->icsmtputp == icsmtp->icsmtgetp)
    {
        /* Release exclusivity */
        swi_ossdksemaphoreunlock( &icsmtp->icsmtsem );

        /* Go to sleep until a message comes in */
        result = swi_ossdkwaiterwait( &icsmtp->icsmtwaiter, timeout );

        /* Obtain exclusivity. This will block if the semaphore is in use */
        swi_ossdksemaphorelock(&icsmtp->icsmtsem);

        /* check if queue is empty */
        if ( (icsmtp->icsmtputp == icsmtp->icsmtgetp) ||
             ( FALSE == result ) )
        {
            /* Release exclusivity */
            swi_ossdksemaphoreunlock( &icsmtp->icsmtsem );
            /* The queue is empty, return NULL message pointer to caller */
            *msgp = (swi_uint32) NULL;
            return FALSE;
        }

        /* get message from the front of the queue */
        *msgp = *icsmtp->icsmtgetp;

        /* increment the "get" message queue pointer */
        if (icsmtp->icsmtgetp == icsmtp->icsmtendp)
        {
            icsmtp->icsmtgetp = icsmtp->icsmtstartp;
        }
        /* Need to wrap */
        else
        {
            icsmtp->icsmtgetp++;
        }
        /* Increment receive count */
        icsmtp->icsmtrcvcnt++;

        /* Release exclusivity */
        swi_ossdksemaphoreunlock( &icsmtp->icsmtsem );

    }
    /* On non-empty message queue */
    else
    {
        /* get message from the front of the queue */
        *msgp = *icsmtp->icsmtgetp;

        /* increment the "get" message queue pointer */
        if (icsmtp->icsmtgetp == icsmtp->icsmtendp)
        {
            icsmtp->icsmtgetp = icsmtp->icsmtstartp;
        }
        /* Need to wrap */
        else
        {
            icsmtp->icsmtgetp++;
        }
        /* Increment receive count */
        icsmtp->icsmtrcvcnt++;

        /* Release exclusivity */
        swi_ossdksemaphoreunlock( &icsmtp->icsmtsem );
    }
    return(TRUE);
}

/*************
 *
 * Name:      icsmt_getmaxqueued
 *
 * Purpose:   To return the maximum number of message in the queue
 *
 * Parms:     icsmtp  - Pointer to the receiving task's message control struct
 *
 * Return:    result  - maximum number of messages queued at any time
 *
 * Abort:     NA
 *
 * Notes:     NA
 *
 */
global swi_uint32 icsmt_getmaxqueued( struct icsmtcb *icsmtp )
{
    return (icsmtp->icsmtmaxqueued);
}

/*************
 *
 * Name:      icsmt_getqnamep
 *
 * Purpose:   To return the pointer to the queue name
 *
 * Parms:     icsmtp  - Pointer to the receiving task's message control struct
 *
 * Return:    qnamep  - pointer to the queue name
 *
 * Abort:     NA
 *
 * Notes:     NA
 *
 */
global swi_charp icsmt_getqnamep( struct icsmtcb *icsmtp )
{
    return ((swi_charp)icsmtp->icsmtqnamep);
}
