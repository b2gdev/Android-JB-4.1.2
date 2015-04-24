/*
 * Filename:  amudefs.h
 *
 * Purpose:   User definitions for AM package
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 */

#ifndef amudefs_h
#define amudefs_h

/* Includes */
#include "ic/icudefs.h"
#include "qm/qmudefs.h"

/* AM Package Definitions */

/* AM Packet Header Version. This value is linked into
 * the API and SDK side of the program. It should change
 * whenever there is a change to the protocol that would
 * render AM transactions between the API and SDK
 * incompatible.
 */
#define AMHDRVERSION 0x0000

/* MAC Address length (bytes) */
#define AMMACSIZE          6

/* Maximum AM packet lengths */
#define AMMAXREQBKLEN   1024
#define AMMAXRESPBKLEN  1024
#define AMMAXNOTIFBKLEN  1024

#define AMQMIRRNSZ              QMUXSDU_PREAMBLE

/* Size of the common part of the AM packet, this
 * is defined inside IDD_Appmessaging.doc
 */
#define AMHEADERSZ               10

/* QMI Total Sizes */
#define AMQMIRESPTOTAL          (AMQMIRRNSZ + AMHEADERSZ)
#define AMQMIREQTOTAL           (AMQMIRRNSZ + AMHEADERSZ)
#define AMQMINOTFTOTAL          (AMQMIRRNSZ + AMHEADERSZ)

/*
    -------------------------------------------------------------------
    | SDK Request Type | SDK Request Timeout | SDK Transaction Length |
    -------------------------------------------------------------------
bytes:      1               4                           2

 */

#define AMSDKRRNSZ              7
/* SDK Total Sizes */
#define AMSDKRESPTOTAL          (AMSDKRRNSZ + AMHEADERSZ)
#define AMSDKREQTOTAL           (AMSDKRRNSZ + AMHEADERSZ)
#define AMSDKNOTFTOTAL          (AMSDKRRNSZ + AMHEADERSZ)
#define AMSDKVERSIONRESPSZ      32

/* SDK Admin Packet Sizes (bytes) */
#define AMSDKVERSIONREQSZ       1
#define AMSDKPIDREQSZ           1
#define AMSDKPIDRESPSZ          4
#define AMKILLSDKREQSZ          1
#define AMKILLSDKRESPSZ         1
#define AMKILLSDKNOTFSZ         1

/* SDK Admin Total Sizes */
#define AMSDKVERSIONREQTOTAL    (AMHEADERSZ + AMSDKVERSIONREQSZ)
#define AMSDKVERSIONRESPTOTAL   (AMHEADERSZ + AMSDKVERSIONRESPSZ)
#define AMSDKPIDREQTOTAL        (AMHEADERSZ + AMSDKPIDREQSZ)
#define AMSDKPIDRESPTOTAL       (AMHEADERSZ + AMSDKPIDRESPSZ)
#define AMKILLSDKREQTOTAL       (AMHEADERSZ + AMKILLSDKREQSZ)
#define AMKILLSDKRESPTOTAL      (AMHEADERSZ + AMKILLSDKRESPSZ)
#define AMKILLSDKNOTFTOTAL      (AMHEADERSZ + AMKILLSDKNOTFSZ)

/* Firmware download constants */
#define AMMAXFILENAMESZ         80
#define AMMAXERRORSTRINGSZ      40

/*
 * Name:   ammsgtypes
 *
 * Purpose: Contains an enumerated list of values that identify an
 *          AM packet's "type" field.
 *
 *          Packets with these type fields are sent/received over
 *          Inter-process communication channels
 *
 * Members: AMMINTYPE         -   Absolute minimum defined message type
 *          AMTYPEQMIREQUEST      QMI Request packet from an application
 *          AMTYPEQMIRESPONSE     QMI Response packet to an application
 *          AMTYPEQMINOTIFICATION QMI Notification packet
 *          AMMAXTYPE         -   Absolute maximum defined message type
 *          AMINVALID         -   Not a valid type. Used in some returns
 *
 * Note:    None
 *
 */
enum ammsgtypes {
    /* Absolute minimum message value */
    AMMINTYPE = 0x0000,
    /* AMQMI Range: 0x00C0 to 0x00FF */
    AMTYPEQMIMIN = 0x00C0,
    AMTYPEQMIREQUEST = AMTYPEQMIMIN,
    AMTYPEQMIRESPONSE,
    AMTYPEQMINOTIFICATION,
    AMTYPEQMIMAX,
    /* AMSDK Range: 0x0100 to 0x01FF */
    AMTYPESDKMIN = 0x0100,
    AMTYPESDKREQUEST = AMTYPESDKMIN,
    AMTYPESDKRESPONSE,
    AMTYPESDKNOTIFICATION,
    AMTYPESDKMAX,
    /* Maximum currently defined message + 1 */
    AMMAXTYPE = AMTYPESDKMAX,

    /* Invalid message - Always last in this list */
    AMINVALID = 0xFFFF
};

enum amsdkrrtypes {
    AMSDKREQVERSION,
    AMSDKRESPVERSION,
    AMSDKREQPID,
    AMSDKRESPPID,
    AMSDKREQKILL,
    AMSDKRESPKILL
};

/*
 * Name:    amqmirrparms
 *
 * Purpose: QMI Request Response parameter structure
 *
 * Members: amresultcode        - Used when building QMIRESPONSE to indicate
 *                                success or a timeout occurred.
 *
 *          amqmixactionlen     - QMI transaction length. This field is here in
 *                                case we need to support multiple QMI service
 *                                requests in a single transaction. For a single
 *                                QMI service request in a transaction, this field
 *                                should be equal to the QMI message length.
 *
 *          amqmireqtimeout     - Application-specified timeout waiting for a responsea
 *
 *          amqmisvctype        - QMI Service type
 *
 * Note:    The size of the fields in this structure are coded to match the
 *          definition of the Generalized QMI transaction structure as per
 *          80-VB816-1_A_QMI_Architecture.pdf
 */
struct amqmirrparms {
    enum eQCWWANError   amresultcode;
    swi_uint32          amqmixactionlen;

    /* QMUX SDU preamble fields */
    swi_uint32          amqmireqtimeout;
    swi_uint8           amqmisvctype;
};

/*
 * Name:    amsdkrrparms
 *
 * Purpose: SDK Request Response parameter structure
 *
 * Members: amsdkxactionlen   - SDK transaction length.
 *          amsdkreqtimeout   - Application-specified timeout waiting for a response
 *          amsdkrrtype       - SDK req/resp type
 *          amsdkparms        - Parameters for the req/resp, one of the following:
 *            - amkillsdk       - KILL SDK req/resp
 *            - amsdkpid        - Get PID req/resp (future)
 *            - amsdkversion    - Get SDK version req/resp (future)
*
 * Note:    N/A
 */
struct amsdkrrparms {
    swi_uint32     amsdkxactionlen;

    /*  SDU preamble fields */
    swi_uint32     amsdkreqtimeout;
    swi_uint8      amsdkrrtype;
    union {
        swi_uint32 amkillsdk;
        swi_uint32 amsdkpid;
        swi_uint8  amsdkversion[AMSDKVERSIONRESPSZ];
    } amsdkparms;
};

/*
 * Name:   amrrparmtype
 *
 * Purpose: Enumerated list of AM packet parameter field types
 *
 * Members: SWI_PARM_QMI      - QMI AM packet
 *
 * Note:    None
 *
 */
enum amrrparmtype
{
    SWI_PARM_QMI,
    SWI_PARM_SDK
};

/*
 * Name:    amrrparms
 *
 * Purpose: Structure used to store all AM Request/Response parameters.
 *
 * Members: amparmtype      - Type of the AM parameter field
 *          amtimeout       - Name of the file containing the image to be
 *                            downloaded to the device.
 *          amresultcode    - result of the AM request/response transaction
 *          amqmi           - QMI request/response parameters
 *          amsdk           - SDK request/response parameters
 *
 * Note:    None
 */
struct amrrparms
{
    enum amrrparmtype  amparmtype;
    swi_uint32         amtimeout;
    enum eQCWWANError  amresultcode;
    union
    {
        struct amqmirrparms     amqmi;
        struct amsdkrrparms     amsdk;
    } amparm;
};

#include "am/amuproto.h"

#endif
