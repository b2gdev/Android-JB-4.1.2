/*************
 *
 * Filename:    imidefs.h
 *
 * Purpose:     IM package private constants and structures
 *
 * Copyright:   © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/

#ifndef IMIDEFS_H
#define IMIDEFS_H

/* Includes */
#include "aa/aaglobal.h"
#include "qm/qmerrno.h"
#include "dl/dludefs.h"
#include "mm/mmudefs.h"
#include "ic/icudefs.h"
#include "os/swi_ossdk.h"
#include "qm/qmudefs.h"
#include "sm/smudefs.h"
#include "ds/dsudefs.h"
#include "imssdp_image_preference.h"

/* event buffer pool definitions */
#define IMEVTPOOLSZ         10 /* # event blocks in the pool */
#define IMMSGDEPTH          10 /* Max # - inter-task SMT request msgs */

/* Download Buffer Pool Definitions */
#define IMDLPOOLSZ          2
#define IMDLBUFSIZE         1024*16

/* Transmit Buffer Pool Definitions */
#define IMTXPOOLSZ          10
#define IMTXBUFSIZE         1024

/* Receive Buffer Pool Definitions */
#define IMRXPOOLSZ          10
#define IMRXBUFSIZE         512


/* Timeout value to use in IM package when calling icsmt_rcv() */
#define IMNOTIMEOUT         0
#define IMDEFAULTIMEOUT     2000 /* milliseconds */

/* Request/Response (RR) Task */
#define IMMRESPTIMEOUT      5000 /* Modem response timeout in ms */

/* Logging masks */
#define IMLOG_CLASSA        0x0001
#define IMLOG_CLASSB        0x0002
#define IMLOG_CLASSC        0x0004

/* FW download defines */
#define    IM_CHUNK_SIZE            1024*8
#define    IM_SIERRA_CWE_IMAGE      0x80
#define    IM_MAX_BUF_SIZE          256

/*************
 *
 * Name:    imeventtypes
 *
 * Purpose: Enumerated list of event block types. These are the types
 *          that get written into the QMI Event Block whenever an
 *          inter-task message is to be sent to the IM Task.
 *
 * Members:
 *
 * Notes:   Mapped to SSDP responses where relevant
 *
 **************/
enum imeventtype {
    eIM_MIN,
    eIM_REQ,
    eIM_DL2SDP_REQ,
    eIM_FRAMED_RSP,
    eIM_UNFRAMED_RSP,
    eIM_TMRTICK,
    eIM_MAX
};

/*************
 *
 * Name:     imstate_t
 *
 * Purpose:  Enumerated states, managed by state machine
 *
 * Members:  stZERO     -
 *           stONE      -
 *           stTWO      -
 *           stMAX      - Number of states
 *
 * Notes:    STATES MUST BE ENUMERATED FROM 0 (zero)
 *
 **************/
enum imstate_t
{
    stIM_IDLE,
    stIM_WAITSDP,
    stIM_HELLO,
    stIM_OF_PKT,
    stIM_UF_WRITE,
    stIM_DONE,
    stIM_DONE_FINAL,
    stIM_NUM_ACTIVE_STATES, /* used for table sizing */
    stIM_IGNORE,
    stIM_INVAL,
    stIM_MAX = stIM_INVAL
};

/*************
 *
 * Name:    imsteventtypes
 *
 * Purpose: Enumerated list of event block types. These are the types
 *          that get written into the QMI Event Block whenever an
 *          inter-task message is to be sent to the IM Task.
 *
 * Members:
 *
 * Notes:   Mapped to SSDP responses where relevant
 *
 **************/
enum imsteventtype {
    eIM_MIN_EVT,
    eIM_SDP_ACK = eIM_MIN_EVT,
    eIM_HELLO_RSP,
    eIM_OFpkt_RSP,
    eIM_UFwrite_RSP,
    eIM_DONE_RSP,
    eIM_DLOAD_RSP,
    eIM_GET_IMAGE_PREF_RSP,
    eIM_ERROR_RSP,
    eIM_SHUTDOWN,
    eIM_BADELF,
    eIM_HOSTIOERR,
    eIM_MAX_EVENT
};

/*************
 *
 * Name:    imsmtstates_e
 *
 * Purpose: Image Manamgement Transaction State Machine (smt) states.
 *
 * Members:
 *
 * Notes:   Enumeration must start at zero.
 *
 **************/
enum imsmtstates_e {
    st_SMTR_WAIT_REQ,
    st_SMTR_WAIT_RSP,
    st_SMTR_NUM_ACTIVE_STATES, /* used for table sizing */
    st_SMTR_IGNR,
    st_SMTR_INVLD
};

/*************
 *
 * Name:    imsmtevents_e
 *
 * Purpose: Image Manamgement Transaction State Machine (smt) events.
 *
 * Members:
 *
 * Notes:
 *
 **************/
enum imsmtevents_e {
    ev_TRANS_TX_REQ,
    ev_TRANS_RX_RSP,
    ev_TRANS_MAX_EVENT
};

/*************
 *
 * Name:    imsmfdstates_e
 *
 * Purpose: Image Management Firmware Download State Machine (smfd) states
 *
 * Members:
 *
 * Notes:   Enumeration must start at zero.
 *
 **************/
enum imsmfdstates_e {
    st_SMFD_IDLE,
    st_SMFD_WAIT_HELLORSP,
    st_SMFD_WAIT_FWPREFRSP,
    st_SMFD_SEND_FWIMG,
    st_SMFD_WAIT_OPENUNF,
    st_SMFD_WAIT_UWRTRSP,
    st_SMFD_WAIT_DONERSP,
    st_SMFD_ERROR,
    st_SMFD_NUM_ACTIVE_STATES, /* used for table sizing */
    st_SMFD_IGNR,
    st_SMFD_INVLD
};

/*************
 *
 * Name:    imfdevent_e
 *
 * Purpose: Image Management Firmware Download State Machine (smfd) events.
 *
 * Members:
 *
 * Notes:
 *
 **************/
enum imfdevent_e {
    ev_FWDLD_START,
    ev_FWDLD_RSPRX,
    ev_FWDLD_UWRTNXT,
    ev_FWDLD_ERROR,
    ev_FWDLD_MAX_EVENT
};

/*************
 *
 * Name:    imtstate_e
 *
 * Purpose: Image Management Task State Machine (smt) states.
 *
 * Members:
 *
 * Notes:   Enumeration must start at zero.
 *
 **************/
enum imtstate_e {
  st_SMT_IDLE,
  st_SMT_FWDLDNG,
  st_SMT_SHUTDWN,
  st_SMT_NUM_ACTIVE_STATES, /* used for table sizing */
  st_SMT_IGNR,
  st_SMT_INVLD
};

/*************
 *
 * Name:    imGobiStEventType
 *
 * Purpose: Image Management Task State Machine (smt) events
 *
 * Members:
 *
 * Notes:
 *
 **************/
enum imtevent_e {
    ev_TASK_DLD_START,
    ev_TASK_DLD_COMP,
    ev_TASK_DLD_FAIL,
    ev_TASK_MAX_EVENT
};

/*************
 *
 * Name:    imdataptr
 *
 * Purpose: contains information about the data to be shared among different
 *          state machines.
 *
 * Members: pdatalen - Length of the data
 *          pdata    - pointer to the data buffer
 *
 * Notes:   none
 *
 **************/
struct imdatapkt
{
    swi_uint32 datalen;
    swi_uint8  *pdata;
    void (*sendpktfn)( swi_uint8 *txbufp,
                       swi_uint32 pktsize );
};

/*************
 *
 * Name:    imevtblock
 *
 * Purpose: Structure used to pass information into the IM task.
 *
 * Members: imeventtype     - Indicates the remaining content of this block
 *          imevtdatap      - Pointer to a buffer containing data to be handled
 *                            by the IM Task.
 *          imevtdatalen    - Length of data pointed to by imevtdatap
 *          imevtmemfreep   - Pointer to the address to use when releasing the
 *                            memory containing the data. NULL if nothing needs
 *                            to be released.
 *          imevtblkp       - Points to this event block
 *
 * Notes:   None
 *
 **************/
struct imevtblock
{
    enum imeventtype imeventtype;
    swi_uint8        *imevtdatap;
    swi_uint32       imevtdatalen;
    swi_uint8        *imevtmemfreep;
    swi_uint8        *imevtblkp;
};

/*************
 *
 * Name:    immbnimglistelement_s
 *
 * Purpose: mbn image list element.
 *
 * Members: type    - mbn image type
 *          path    - path to the image to download
 *
 * Notes:   None
 *
 **************/
typedef struct immbnimglistelement_s{
    enum mbn_image_type_e type;
    swi_char path[IM_IMAGE_PATH_MAX_SZ];
    swi_char imageid[MBN_IMG_ID_STR_LEN];
}immbnimglistelement_t;

/*************
 *
 * Name:    immbnimglist_s
 *
 * Purpose: mbn image list.
 *
 * Members: totalimages     - number of images to download
 *          nextimageidx    - index of the next image to download
 *          mbnimgs         - information about images to download
 *
 * Notes:   None
 *
 **************/
 typedef struct immbnimglist_s{
    swi_uint32 totalimages;
    swi_uint32 nextimageidx;
    immbnimglistelement_t mbnimgs[SSDP_MAX_IMAGE_PREF];
}immbnimglist_t;

/*************
 *
 * Name:    imqdl
 *
 * Purpose: Structure used to store information for the FW download APIs.
 *
 * Members: pPortSize       -
 *
 * Notes:   None
 *
 **************/
struct imqdl
{
    /* QDL parameters */
    swi_uint8   *pPortSize;
    swi_uint8   *pPorts;
    swi_uint8   portID;
    swi_uint32  bBARMode;
    swi_uint32  MajorVersion;
    swi_uint32  MinorVersion;
    swi_uint8   ImageType;
    swi_uint32  fwImageSize;
    swi_uint32  cweImageSize;
    swi_uint32  BlockSize;
    swi_uint16  sequenceNumber;
    swi_char    *pImageBlock;
    swi_char    filepath[IM_MAX_BUF_SIZE];
    swi_char    filename[IM_MAX_BUF_SIZE];
    swi_char    fullpathfilename[IM_MAX_BUF_SIZE];
    swi_uint32  chunkSize;
    swi_uint32  remainder;
    swi_uint32  numwrites;
    struct      swi_osfilestats fileinfo;
    swi_uint32  nextchunkoffset;
    struct      swi_osfile fp;
    swi_uint32  byteswritten;
    immbnimglist_t  gobimgs2dwld;
};

/*************
 *
 * Name:    imtcb
 *
 * Purpose: QM RR Task control block structure
 *
 * Members: imdlcb     - DL package control block for logging
 *          imicmsgque - IC SMT Message queue
 *          imevtpool  - MM pool structure. Contains buffers used
 *                       to store information about IM Reponses.
 *                       Groups several arguments that can be referenced
 *                       by a single handle
 *          imdlpool   - MM pool structure. Contains buffers used
 *                       for reading download file and packing download file
 *                       for sending to the modem
 *          imdtxpool  - MM pool structure. Contains buffers used for packing
 *                       SDP requests to the modem
 *          imrxpool   - MM pool structure. Contains buffers used
 *                       to for receiving responses from the modem
 *          imrxdatap  - pointer to received data from the modem
 *          imdsshell  - dsshell structure for communication to/from the modem
 *          imsmp      - pointer to state machine storage
 *          imqdl      - storage for download info
 *          imhdmemrelp- pointer to memory to be released
 *                       (from the HD receive package)
 *
 * Notes:   none
 *
 **************/
struct imtcb
{
    struct dlusercb    imdlcb;
    struct icsmtcb     imicmsgque;
    struct mmpool      imevtpool;
    struct imevtblock   *imevtblkp;
    struct mmpool      imdlpool;
    struct mmpool      imtxpool;
    struct mmpool      imrxpool;
    swi_uint8          *imrxdatap;
    struct dsipcshell   imdsshell;
    swi_uint8          *imdsmemrelp;
    sm_t               *imsmp;
    struct imqdl       imqdl;
    swi_uint8          *imhdmemrelp;

    /* not used yet */
    struct icttregqblk imictthndl;
    swi_uint32         immdmwaittmr;
    swi_bool           imideviceready;
};

/*************
 *
 * Name:    imgcb
 *
 * Purpose: im Global Control Block
 *
 * Members:
 *
 * Notes:   none
 *
 **************/
struct imgcb
{
    struct imtcb *imtcbp;
    struct dlusercb imdlcb;
};

#include "im/imiproto.h"

#endif /* IMIDEFS_H */
