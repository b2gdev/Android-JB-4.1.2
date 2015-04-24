/*************
 *
 * Filename: am.c
 *
 * Purpose:  AM Package common routines
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/

/* include files */
#include "qm/qmerrno.h"
#include "am/amidefs.h"
#include <stdio.h>

/*
 * Default to no tag. In SVN, this should never be changed in the trunk. The
 * recommended procedure is:
 *  - tag a version of the repository to "SLQSxx.yy.zz" and
 *  - check-out this file from the tagged copy and
 *  - replace "(NO TAG)" with "SLQSxx.yy.zz" and
 *  - check-in back into the tagged version.
 *
 * This should be the only change that is allowed in a tagged version.
 */
/* RILSTART */
const char SLQS_VERSION[]="SLQS02.01.01";
/* RILSTOP */
/*************
 *
 * Name: amgethdrptr
 *
 * Purpose: To accept a pointer to any AM packet's Parameter field and
 *          return a pointer to where this particular  AM packet should
 *          begin.
 *
 * Parms:   parmp     - Pointer to the first byte of the parameter
 *                      field (payload)
 *          amtype    - Which AM Message type to compute the offset
 *                      for.
 *          bufstartp - Pointer to the absolute beginning of the buffer
 *                      needed to ensure we don't back past the beginning
 *                      of this allocated memory buffer
 *
 * Return:  Pointer to the first byte of where to start building the AM
 *          packet
 *
 * Abort:   If reversing the pointer goes before the beginning of the
 *          memory buffer. This means that the AM packet header is
 *          bigger than the combined sizes of all  headers
 *          which could only happen if modifications were recently made
 *          to one or more of the headers
 *
 * Notes:   Reuses memory and avoids copying of the payload.
 *
 **************/
global swi_uint8 *amgethdrptr( swi_uint8 *parmp,
                               swi_uint16 amtype,
                               swi_uint8 *bufstartp )
{
    swi_uint8 *amhdrp;

    switch( amtype )
    {
        case AMTYPEQMINOTIFICATION:
            amhdrp = parmp - AMQMINOTFTOTAL;
            break;

        case AMTYPEQMIRESPONSE:
            amhdrp = parmp - AMQMIRESPTOTAL;
            break;

        default:
            {
                char errmsg[100];
                snprintf(errmsg, sizeof(errmsg),
                         "%s:%d No amtype specified",
                         (char *)__func__, __LINE__);
                erAbort(errmsg, amtype);
            }
    }

    if( bufstartp > amhdrp )
    {
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d AM header too big",
                 (char *)__func__, __LINE__);
        erAbort(errmsg, amtype );
    }

    return amhdrp;
}

/*************
 *
 * Name: ambuildamheader
 *
 * Purpose: To build an AM packet header in the given packet buffer
 *
 * Parms:   amtype          - AM packet type to be put in header
 *          amheaderversion - AM header version to be put in header
 *          amresultcode    - Result Code to be put in header
 *          ampacketlength  - Length of AM payload
 *          packetpp        - Destination pointer for construction of AM packet header
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   The packet header will be advanced past the AM packet header
 *          by this function.
 *
 **************/
package void ambuildamheader(
    swi_uint16 amtype,
    swi_uint16 amheaderversion,
    enum eQCWWANError amresultcode,
    swi_uint16 ampacketlength,
    swi_uint8 **packetpp)
{
    swi_uint8 *packetp;  /* temporary packet pointer */

    /* initialize packet pointer to the beginning of the packet */
    packetp = *packetpp;

    /* pack the AM packet type */
    piput16( amtype, &packetp );

    /* pack the current AM packet protocol header version */
    piput16( amheaderversion, &packetp );

    /* pack the AM result code */
    piput32( amresultcode, &packetp );

    /* pack the total length of the AM packet */
    piput16( ampacketlength, &packetp );

    /*
     * return a packet pointer that has been incremented
     * past the header fields
     */
    *packetpp = packetp;
}
/*************
 *
 * Name: amparseamheader
 *
 * Purpose: To parse an AM packet header in the given packet buffer
 *
 * Parms:   amtypep          - Location to place parsed AM packet type
 *          amheaderversionp - Location to place parsed AM header version
 *          amresultcodep    - Location to place parsed result code
 *          ampacketlengthp  - Location to place parsed length of AM packet
 *          packetpp         - Location of packet buffer pointer which is to
 *                           be parsed.
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   The packet header will be advanced past the AM packet header
 *          by this function.
 *
 **************/
package void amparseamheader(
    swi_uint16 *amtypep,
    swi_uint16 *amheaderversionp,
    enum eQCWWANError *amresultcodep,
    swi_uint16 *ampacketlengthp,
    swi_uint8 **packetpp)
{
    swi_uint8 *packetp;  /* temporary packet pointer */

    /* initialize packet pointer to the beginning of the packet */
    packetp = *packetpp;

    /* unpack the AM packet type */
    *amtypep = piget16( &packetp );

    /*
     * unpack the AM packet protocol header version
     * to which this packet conforms
     */
    *amheaderversionp = piget16( &packetp );

    /* pack the total length of the AM packet */
    *amresultcodep = piget32( &packetp );

    /*
     * return a packet pointer that has been incremented
     * past the header fields
     */
    *ampacketlengthp = piget16( &packetp );

    /* return the forwarded packet pointer */
    *packetpp = packetp;
}

/*************
 *
 * Name: amgettagname
 *
 * Purpose: To return a pointer to a string indicating the tag name or
 *          (NO TAG)
 *
 * Parms:   none
 *
 * Return:  Pointer to the tag name string
 *
 * Abort:   None
 *
 * Notes:   This feature is here because this object is available to both
 *          the SDK daemon and Application program.
 *          This is useful for tracking the SDK version out in the field, which
 *          should be tagged.
 *
 **************/
global swi_uint8 *amgettagname( void )
{
    /* Return tagged name or "(NO TAG)" */
    return ((swi_uint8 *)SLQS_VERSION);
}

/*************
 *
 * Name:    amparsesdkheader
 *
 * Purpose: Parse an SDK transaction header
 *
 * Parms:   amqmipacketpp  - double pointer to the AM QMI Request packet.
 *          amsdkrrparmp   - Pointer to a caller-allocated structure defined by
 *                           AM package for exchanging SDK Request/Response
 *                           parameters.
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   Parses the fields from an SDK Request, Response, or Notification
 *          AM packet and returns the values to the caller in the structure
 *          provided.
 *
 *          On return the pointer to packet pointer is advanced to point
 *          to the SDK Parameter field
 *
 **************/
package void amparsesdkheader(
    swi_uint8           **amsdkpacketpp,
    struct amsdkrrparms *amsdkrrparmp )
{
    swi_uint8  *packetp;    /* Local copy of incoming packet pointer */
    swi_uint32 bytesread;   /* After parsing, contains #bytes just read */

    /* Duplicate the pointer into our scratch location */
    packetp = *amsdkpacketpp;

    amsdkrrparmp->amsdkrrtype = *packetp++;
    amsdkrrparmp->amsdkreqtimeout = piget32(&packetp);
    amsdkrrparmp->amsdkxactionlen = piget16(&packetp);
    bytesread = packetp - *amsdkpacketpp;

    /* Make sure the bytes read matches the packet size */
    if( bytesread != AMSDKRRNSZ )
    {
        /* Combine both sizes into one and abort */
        bytesread = (bytesread << 16) | AMSDKRRNSZ;
        char errmsg[100];
        snprintf(errmsg, sizeof(errmsg),
                 "%s:%d Update AMSDKRRNSZ", (char *)__func__, __LINE__);
        erAbort(errmsg, bytesread);
    }

    /* advance the caller's packet pointer to the start of the QMI Message ID */
    *amsdkpacketpp += bytesread;
}
