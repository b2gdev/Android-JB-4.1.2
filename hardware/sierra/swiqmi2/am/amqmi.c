/*************
 *
 * Filename: amqmi.c
 *
 * Purpose:  Contains routines related to handling QMI packets defined by the
 *           AM package. Runs on the API-side.
 *
 * NOTE:     The functions within this file build and parse QMI transactions
 *           (a.k.a QMUX SDUs).
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/

/* include files */
#include "qm/qmudefs.h"
#include "qm/qmerrno.h"
#include "pi/piudefs.h"
#include "am/amidefs.h"
#include <stdio.h>

/* Local storage */

/* Packet Building Routines */

/*************
 *
 * Name: ambuildqmixaction
 *
 * Purpose: Build an AM QMI transaction header
 *
 * Parms:   amsgp          - Pointer to storage into which the message
 *                           will be written
 *          amqmiparams    - Pointer to a structure containing the QMI
 *                           parameters
 *          amtype         - AM packet type field
 *
 * Return:  Number of bytes occupied by the AM QMI packet header
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
global swi_uint16
ambuildqmixactionheader(
    swi_uint8 **amsgpp,
    struct amqmirrparms *amqmirrparmp,
    enum ammsgtypes amtype )
{
    swi_uint8  *packetp;     /* Local copy of incoming packet pointer */
    swi_uint32 byteswritten; /* After parsing, contains #bytes just read */

    /* Duplicate the pointer into our scratch location */
    packetp = *amsgpp;

    /* Build AM packet header */
    ambuildamheader(
        amtype,
        AMHDRVERSION,
        amqmirrparmp->amresultcode,
        AMQMIRRNSZ + amqmirrparmp->amqmixactionlen,
        &packetp);

    /* QMUX SDU PREAMBLE */
    *packetp++ = amqmirrparmp->amqmisvctype;
    piput32(amqmirrparmp->amqmireqtimeout, &packetp);
    piput16(amqmirrparmp->amqmixactionlen, &packetp);

    /* Update the bytes read variable here */
    byteswritten = packetp - *amsgpp;

    /* Ensure that bytes written matches the defined constant for size */
    if( byteswritten != AMQMIREQTOTAL )
    {
        /* Combine both sizes into one and abort */
        byteswritten = byteswritten << 16 | AMQMIREQTOTAL;
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d Update AMQMIREQTOTAL",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, byteswritten );
    }

    return byteswritten;
}

/* Parsing Routines */

/*************
 *
 * Name: amparseqmixaction
 *
 * Purpose: Parse an QMI transaction header
 *
 * Parms:   amqmipacketpp  - double pointer to the AM QMI Request packet.
 *          amqmirrparmp   - Pointer to a caller-allocated structure defined by
 *                           AM package for exchanging QMI Request/Response
 *                           parameters.
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   Parses the fields from a QMI Request, Response, or Notification
 *          AM packet and returns the values to the caller in the structure
 *          provided.
 *
 *          On return the pointer to packet pointer is advanced to point
 *          to the QMI Parameter field
 *
 **************/
global void
amparseqmixactionheader(
    swi_uint8 **amqmipacketpp,
    struct amqmirrparms *amqmirrparmp )
{
    swi_uint8  *packetp;     /* Local copy of incoming packet pointer */
    swi_uint32 bytesread;    /* After parsing, contains #bytes just read */

    /* Duplicate the pointer into our scratch location */
    packetp = *amqmipacketpp;

    amqmirrparmp->amqmisvctype = *packetp++;
    amqmirrparmp->amqmireqtimeout = piget32(&packetp);
    amqmirrparmp->amqmixactionlen = piget16(&packetp);
    bytesread = packetp - *amqmipacketpp;

    /* Make sure the bytes read matches the packet size */
    if( bytesread != AMQMIRRNSZ )
    {
        /* Combine both sizes into one and abort */
        char errmsg[100];
        bytesread = (bytesread << 16) | AMQMIRRNSZ;
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d Update AMQMIRRNSZ",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, bytesread );
    }

    /* advance the caller's packet pointer to the start of the QMI Message ID */
    *amqmipacketpp += bytesread;
}
