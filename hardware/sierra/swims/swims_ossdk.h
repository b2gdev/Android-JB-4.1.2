/**
 *
 * @ingroup swims
 *
 * @file
 * Wrap OS functions for SDK-side (SDK process) definitions.
 * Not for use by application process functions.
 *
 * @author
 * Copyright: © 2010 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */

#ifndef swi_ossdk_h
#define swi_ossdk_h

/* include files */
#include "SwiDataTypes.h"      /* data types typedefs */
#include <pthread.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

/*
 * constants
 */

#define SWI_OS_MAXIDS 16           /* number of USB vendors */
#define SWI_OS_MAXENDPOINTS 32     /* number of USB endpoints */
#define SWI_OS_MAXUSBTTYS 1024     /* number of ttyUSBs */

#define SWI_OS_USB_STATE_BAD 0     /* modem not functioning */
#define SWI_OS_USB_STATE_CHECK 1   /* modem functionality being checked */
#define SWI_OS_USB_STATE_OK 2      /* modem appears to be working */

#define SWI_NOREAD_TERMINATION 0xFF        /* Default USB read termination */
#define SWI_NOREAD_THRESHOLD   0xFFFFFFFF  /* No minimum threshold defined */

/*
 * structures
 */

/* semaphore */
struct swi_ossemaphore    {
    pthread_mutex_t ossemmutex;
};

/* thread wait/wake */
struct swi_oswaiter {
    pthread_mutex_t swi_oswaitmutex;          /* pthread mutex */
    pthread_cond_t  swi_oswaitcondition;      /* pthread condition variable */
    volatile swi_uint32 swi_oswaitsleepstate; /* SWI_OSWAITSTATE_xxx */
};

/* disk file I/O */
struct swi_osfile {
    int osfiledescrip;
};

/* inter-process communication to API-Side */
struct swi_osipc  {
    int osipcsocket;
    struct sockaddr_un osipcdest;
};

struct swi_endpointtty {
    swi_uint32    osusbttynum;    /* tty number in the system */
    swi_uint32  osusbifnum;       /* usb device interface number */ 
};

/* USB device I/O */
struct swi_osusb  {
    pthread_mutex_t osusbmutex;    /* mutex for this structure */
    swi_uint32 osusbstate;
    swi_uint32 osusbvendorid;
    swi_uint32 osusbproductid;
    
    /* ttyUSB# for read EPs */
    struct swi_endpointtty osusbreadendpointtty[SWI_OS_MAXENDPOINTS];  
    /* ttyUSB# for write */
    struct swi_endpointtty osusbwriteendpointtty[SWI_OS_MAXENDPOINTS];  
    
    int osusbreaddescript[SWI_OS_MAXENDPOINTS];  /* read file descriptor */
    int osusbwritedescript[SWI_OS_MAXENDPOINTS]; /* write file descriptor */

    /* Array of read termination characters per endpoint */
    swi_uint8  osusbreadterminator[SWI_OS_MAXENDPOINTS]; 
    /* Array of read termination length thresholds per endpoint */
    swi_uint32 osusbreadthreshold[SWI_OS_MAXENDPOINTS];  
};

/* endpoint direct from the HOST point of view */
enum swi_endpointdirection
{
    SWI_EP_IN,  /* gadget to host */
    SWI_EP_OUT  /* host to gadget */
};

/* prototypes */
void swi_ossdkprocessinit(void);
void swi_ossdkthreadcreate(void (*fnp)(void *paramp), void *paramp);
void *swi_ossdkmemory(swi_size_t size);
void swi_ossdksemaphoreinit(struct swi_ossemaphore *sp);
void swi_ossdksemaphorelock(struct swi_ossemaphore *sp);
void swi_ossdksemaphoreunlock(struct swi_ossemaphore *sp);
void swi_ossdkwaiterinit(struct swi_oswaiter *wp);
swi_bool swi_ossdkwaiterwait(struct swi_oswaiter *wp, swi_uint32 timeout);
void swi_ossdkwaiterwake(struct swi_oswaiter *wp);
void swi_ossdkabort(const char *reasonp);
void swi_ossdklog(const char *errorp);
swi_bool swi_ossdkfileopenread(struct swi_osfile *fp, const char *pathp);
void swi_ossdkfileclose(struct swi_osfile *fp);
swi_bool swi_ossdkfilesize(struct swi_osfile *fp, swi_size_t *sizep);
swi_bool swi_ossdkfileread(struct swi_osfile *fp, void *bufp, swi_size_t *lengthp);
void swi_ossdkipcsetdest(struct swi_osipc *ip, swi_size_t ipcindex, 
    swi_uint8 * ipcnamep);
void swi_ossdkipccleardest(struct swi_osipc *ip); 
void swi_ossdkipcopen(struct swi_osipc *ip, swi_size_t ipcindex, 
    swi_uint8* chnamep);
swi_bool swi_ossdkipcclose(struct swi_osipc *picp);
swi_bool swi_ossdkipcwrite(struct swi_osipc *ip, const void *bufp, 
    swi_size_t length);
swi_bool swi_ossdkipcread(struct swi_osipc *ip, void *bufp, swi_size_t *lengthp, 
    swi_uint32 timeout);
void swi_ossdkusbinit(struct swi_osusb *up );
swi_uint32 swi_ossdkusbscan(struct swi_osusb *up);
swi_bool swi_ossdkusbproduct(struct swi_osusb *up, swi_uint32 *vendoridp, 
    swi_uint32 *productidp);
swi_bool swi_ossdkusbendpoint(struct swi_osusb *up, swi_uint32 endpoint,
                swi_bool readnotwrite, char *namep, swi_size_t size);
void swi_ossdkusbwaitmodem(struct swi_osusb *up);
swi_bool swi_ossdkusbwrite(struct swi_osusb *up, swi_uint32 endpoint, 
    const void *bufp, swi_size_t length);
void swi_ossdkreadterms( struct swi_osusb *up, swi_uint32 endpoint, 
                         swi_uint8 readterm, swi_uint32 threshold );
swi_bool swi_ossdkusbread(struct swi_osusb *up, swi_uint16 endpoint, 
    void *bufp, swi_size_t *lengthp, swi_uint32 timeout);
void swi_ossdkusbclose(struct swi_osusb *up, swi_uint16 endpoint, 
        swi_bool readnotwrite);
void swi_ossdksetup( swi_uint32 *setuplistp );
int swi_ossdkusmapifacetoep( struct swi_osusb *up, 
        swi_uint32 iface, enum swi_endpointdirection direction );
swi_bool swi_ossdkpeeravail(struct swi_osipc *ip, swi_uint16 *errcodep);
swi_bool swi_endpointcompare( 
        const struct swi_endpointtty * e1, 
        const struct swi_endpointtty * e2 );
void swi_ossdkaborterror(const char *errorp, int llerrno);


#endif
