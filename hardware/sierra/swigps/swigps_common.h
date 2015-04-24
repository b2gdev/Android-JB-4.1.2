/**
 *
 * @ingroup swiril
 *
 * @file 
 * Definitions for vendor specific GPS handling which are common to
 * to the RIL side and the GPS Interface side. 
 * 
 * There are two components of this vendor specific code running within
 * two daemons:
 * There is a component running under the RIL daemon. The RIL daemon
 * part exists to service requests to access the modem from the main
 * GPS engine. It accepts requests from an IPC channel and interacts
 * with the modem to fulfill the request. 
 *
 * The main vendor-specific GPS handler is invoked by the GPS Location
 * service under the Android operating system and thus has a separate
 * system process (daemon) controlling it. 
 *
 * A Naming convention is needed for some definitions which may be 
 * unique to one of these components. The convention uses the following 
 * prefixes:
 *    sgi: for "SwiGps Interface". Items with this prefix are use only
 *      on the Android-to-Vendor-Specific side, i.e. most of the files
 *      contained in the swigps directory
 *    sgr: for "SwiRil Gps". Items using this prefix are used only on
 *      the swiril_gps side, i.e. gps files whose name begins with 
 *      "swiril_gps"
 *    swigps: for definitions that are agnostic to one side or the other,
 *      i.e. either can use it.
 *
 * @author
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */

/* Definitions */
#define SWIGPS_DEVICE_PROPERTY "ril.gps.device"

/* Maximum transfer size */ 
#define MAXGPS 512

#define MAX_READ_ITERATIONS 10

/* Size of mandatory buffer passed into receivepacket() and socksend() */
#define ERRMSGSIZE 40

/* Socket name */
#define SOCKET_NAME_SWIGPS "swigps"

/* IPC traffic between swigps and swiril_gps uses a packet format
 * consisting of a fixed-size message header and a variable length
 * message body. The header contains 2 2-byte fields, a type and a
 * length. The variable length part depends on the type field.
 *
 *       +-------------------------------------------------+
 *      2| type: One of swigpsMsgType                      |
 *       +-------------------------------------------------+
 *      2| result: (Inbound only) QMI result of the request|
 *       +-------------------------------------------------+
 *      2| length: size (bytes) of variable portion (0..n) |
 *       +-------------------------------------------------+
 *       |                                                 |
 *      n| type-specific content                           |
 *       |                                                 |
 *       +-------------------------------------------------+
 */
#define SWIGPS_MSGSIZE_HEADER 6
#define MAXURL 128

/* MIN URL is the smallest possible URL
 * that makes sense - in this case of
 * the form x.com. Very unlikely, but
 * possible. 
 */
#define MINURL 5

/* Type-specific content length values */
#define SWIGPS_MSGSIZE_REQ_GETNMEAPORT         2
#define SWIGPS_MSGSIZE_RESP_MIN_GETNMEAPORT    9 /* Assumes "ttyUSBx" */
#define SWIGPS_MSGSIZE_RESP_MAX_GETNMEAPORT   22

#define SWIGPS_MSGSIZE_REQ_GETDEF              0
#define SWIGPS_MSGSIZE_RESP_GETDEF            10

#define SWIGPS_MSGSIZE_REQ_SETDEF             11
#define SWIGPS_MSGSIZE_RESP_SETDEF             0

#define SWIGPS_MSGSIZE_REQ_GETAGPS             0
#define SWIGPS_MSGSIZE_RESP_SETAGPS            0

#define SWIGPS_MSGSIZE_REQ_INJTIME            12
#define SWIGPS_MSGSIZE_RESP_INJTIME            0

#define SWIGPS_MSGSIZE_REQ_STOP                0
#define SWIGPS_MSGSIZE_RESP_STOP               0

#define SWIGPS_MSGSIZE_REQ_START              10
#define SWIGPS_MSGSIZE_RESP_START              0

/* The next two are common to both the 
 * AGPS request and AGPS response
 */
#define SWIGPS_MSGSIZE_MIN_AGPS                9
#define SWIGPS_MSGSIZE_MAX_AGPS  (SWIGPS_MSGSIZE_MIN_AGPS+MAXURL)

/** 
 * 
 * GPS package error codes
 *
 * SWIGPS_SUCCESS                  Operation completed successfully
 * SWIGPS_ERR_NO_NMEA_TTY          NMEA TTY could not be located
 *
 */
#define    SWIGPS_SUCCESS          0
#define    SWIGPS_ERR_NO_NMEA_TTY  1

/** 
 * 
 * Message header type definitions. Each enumeration in this type
 * definition, except the MIN and MAX values, identifies a particular
 * message type for exchange between the swigps and swiril_gps side.
 * These enumerations are used as indeces into the msgValidation 
 * array, below.
 *
 * SWIGPS_MSGTYPE_MINVAL           Minimum valid message type number
 * SWIGPS_MSGTYPE_REQ_GETDEF       Request the default PDS settings 
 *                                 from the modem 
 * SWIGPS_MSGTYPE_RESP_GETDEF      Values reported by the modem in 
 *                                 response to a request to get defaults
 * SWIGPS_MSGTYPE_REQ_SETDEF       Request to set/change default GPS values
 *                                 within the modem
 * SWIGPS_MSGTYPE_RESP_SETDEF      Response to a request to set/change PDS 
 *                                 defaults within the modem
 * SWIGPS_MSGTYPE_REQ_GETAGPS      Request to fetch AGPS info from modem
 * SWIGPS_MSGTYPE_RESP_GETAGPS     Response containing AGPS info from modem
 * SWIGPS_MSGTYPE_REQ_SETAGPS      Request to set AGPS values to modem
 * SWIGPS_MSGTYPE_RESP_SETAGPS     Modem's response to setting AGPS values
 * SWIGPS_MSGTYPE_REQ_INJTIME      Request to inject time into GPS engine
 * SWIGPS_MSGTYPE_RESP_INJTIME     Response to inject time request
 * SWIGPS_MSGTYPE_REQ_STOP         Request to the modem to stop tracking
 * SWIGPS_MSGTYPE_RESP_STOP        Modem's response to the stop tracking req
 * SWIGPS_MSGTYPE_REQ_START        Request to the modem to start tracking
 * SWIGPS_MSGTYPE_RESP_START       Modem's response to the start tracking req
 * SWIGPS_MSGTYPE_MAXVAL           Maximum valid message type + 1
 * 
 */
typedef enum { 
    SWIGPS_MSGTYPE_MINVAL = 0,
    /* Get the NMEA Port Handle */
    SWIGPS_MSGTYPE_REQ_GETNMEAPORT = SWIGPS_MSGTYPE_MINVAL,
    SWIGPS_MSGTYPE_RESP_GETNMEAPORT,
    /* Get defs and their response */
    SWIGPS_MSGTYPE_REQ_GETDEF,
    SWIGPS_MSGTYPE_RESP_GETDEF,
    /* Set defs and their response */
    SWIGPS_MSGTYPE_REQ_SETDEF,
    SWIGPS_MSGTYPE_RESP_SETDEF,
    /* Get AGPS settings and response */
    SWIGPS_MSGTYPE_REQ_GETAGPS,
    SWIGPS_MSGTYPE_RESP_GETAGPS,
    /* Set AGPS settings and response */
    SWIGPS_MSGTYPE_REQ_SETAGPS,
    SWIGPS_MSGTYPE_RESP_SETAGPS,
    /* Inject time request and response */
    SWIGPS_MSGTYPE_REQ_INJTIME,
    SWIGPS_MSGTYPE_RESP_INJTIME,
    /* Stop a tracking session */
    SWIGPS_MSGTYPE_REQ_STOP,
    SWIGPS_MSGTYPE_RESP_STOP,
    /* Start a tracking session */
    SWIGPS_MSGTYPE_REQ_START,
    SWIGPS_MSGTYPE_RESP_START,
    /* End of list */
    SWIGPS_MSGTYPE_MAXVAL
} swigpsMsgType;

/* GPS RIL Type definitions - used to ensure the correct GPS library
 * is working with its peer RIL host. Member names self-explanatory
 */
typedef enum { 
    GPS_RILTYPE_AT,
    GPS_RILTYPE_QMI,
    GPS_RILTYPE_MAX
} GPS_RILTYPE; 

/**
 * 
 * Message header properties for validation purposes. Each message 
 * header type has an associated structure of this kind 
 */
typedef struct {
    unsigned short minlength;  /*!< Minimum length for the specified msg type */
    unsigned short maxlength;  /*!< Maximum length for the packet */
} gpsMsgProps;

/**
 *
 * Type-specific callback function definition
 *
 * Each type has a callback function associated with it and this structure
 * definition defines the prototype all such callback functions must adhere
 * to.
 *
 * The expected function prototype arguments are interpreted as follows:
 * @param [in] swigpsMsgType
 *    The type contained in the message header which triggered a call 
 *    to this function. In some cases a single function might handle
 *    multiple types and that value has already been parsed out of the
 *    incoming packet.
 * @param [in] unsigned char*
 *    This is a pointer to the packet containing the remainder of the 
 *    message just received over the IPC channel. This function will 
 *    need to parse out the remainder of the message using the type
 *    value as a guide
 * @param [in] size_t
 *    The amount of data, in bytes, contained in the packet. 
 * @param [in] void *
 *    A pointer to a user-specified parameter. No attempt is made by
 *    the caller to interpret this value
 *
 */
typedef void (*swigpsCbkFp)(unsigned char*, swigpsMsgType, size_t, void *);

/**
 * Type-specific callback array structure definition. 
 *
 * Each type related to a response message has an associated callback 
 * function defined for it. This structure defines the contents of an
 * entry in the array of callback functions. 
 *
 * @param: gpsCbkfp
 *    Pointer to the callback function associated with the 'type' value
 * @param: gpsCbkfnNamep
 *    Pointer to a string identifying the type for debug logging purposes
 *
 */
typedef struct {
    swigpsCbkFp gpsCbkfp;
    char *gpsCbkfNamep;
} swigpsTypeHandler;


/* External declarations */
extern gpsMsgProps msgProperties[];
extern unsigned short piget16(unsigned char **packetpp);
extern unsigned int piget32(unsigned char **packetpp);
extern unsigned long long piget64(unsigned char **packetpp);
extern void pigetmulti(unsigned char **packetpp, 
                unsigned char *bufferp, 
                unsigned short numfields);
extern void piput16(unsigned short field, unsigned char **packetpp);
extern void piput32(unsigned int field, unsigned char **packetpp);
extern void piput64(unsigned long long field, unsigned char **packetpp);
extern void piputmulti(unsigned char *fieldsp, 
                unsigned char **packetpp, 
                unsigned short numfields);
extern char *swigps_makhdr( swigpsMsgType type, int length,
                   unsigned short result, unsigned char *bufp, 
                   int buflength, int *writtenp );
extern  int socksend( int fd, void *bufp, size_t length, char *errmsgbuf );
extern int receivepacket(int fd, 
                         unsigned char *datap, 
                         int *numbytes, 
                         char *errmsgbuf, 
                         int ermsgsize);

