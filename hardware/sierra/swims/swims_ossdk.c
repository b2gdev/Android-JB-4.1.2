/**
 *
 * @ingroup swims
 *
 * @file
 * Wrap OS functions for SDK-side (SDK process)
 *
 * @author
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */

/* include files */
#include "SwiDataTypes.h"      /* data types typedefs */
#include "swims_ossdk.h"       /* package user include file  */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <dirent.h>
#define LOG_TAG "RILSCAN"
#include "swiril_log.h"
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <sys/resource.h>
#include <sys/prctl.h>


#define SWI_OSWAITSTATE_SLEEP 0
#define SWI_OSWAITSTATE_AWAKE 1
#define SWI_OSWAITSTATE_ALERT 2

#define SWI_SOCKET_REAP_MS 500          /* socket reap time after exit */
#define SWI_RETRY_MS 100                /* milliseconds */
#define SWI_MESSAGE_BYTES 5000          /* max IPC message */
#define SWI_PROCESS_STARTUP_MS 100      /* delay when application down */

#define SWI_USB_MAX_PATH 256
#define SWI_USB_MAX_FILENAME 32
#define SWI_MAX_SIGNAL_NAME 16

#define SWI_STACK_SIZE    (1024UL * 1024UL)    /* stack size for threads */

/* Forward references */
extern void swi_sdkfdclose(int fd);


/*************
 *
 * Name: swi_ossdklogerror
 *
 * Purpose:  Write to log, show low-level error
 *           
 * Params:   errorp  - string describing problem
 *           llerrno - low-level error number (from errno)
 *
 * Return:   No value
 *
 * Abort:    none
 *
 * Notes:    Don't overuse or repeatedly call this function for the same 
 *           error; i.e., for every retry
 *
 **************/
local void swi_ossdklogerror(const char *errorp, int llerrno)
{
    if (llerrno != 0)
    {
        errno = llerrno;
        LOGE("Modem scan -- %s %d", errorp, llerrno);
    }
    else
        LOGI("%s", errorp);
}

/*************
 *
 * Name: swi_ossdklog
 *
 * Purpose:  Write to the system log /var/log/user.log
 *           
 * Params:   errorp  - string describing problem
 *
 * Return:   No value
 *
 * Abort:    none
 *
 * Notes:    Don't overuse or repeatedly call this function for the same error;
 *           i.e., for every retry
 *
 **************/
void swi_ossdklog(const char *errorp)
{
    swi_ossdklogerror(errorp, 0);
}

/*************
 *
 * Name: swi_ossdkaborterror
 *
 * Purpose:  Abort the process, write to log, show low-level error
 *           
 * Params:   reasonp - string describing problem
 *           llerrno - low-level error number (from errno)
 *
 * Return:   No value
 *
 * Abort:    none
 *
 * Notes:    none
 *
 **************/

void swi_ossdkaborterror(const char *errorp, int llerrno)
{
    swi_ossdklogerror(errorp, llerrno);
    
    abort();
}

/*************
 *
 * Name: swi_ossdkabort
 *
 * Purpose:  Abort the process, write to log /var/log/user.log
 *           
 * Params:   reasonp - string describing problem
 *
 * Return:   No value
 *
 * Abort:    none
 *
 * Notes:    none
 *
 **************/

void swi_ossdkabort(const char *errorp)
{
    swi_ossdkaborterror(errorp, 0);
}

/*************
 *
 * Name: swi_ossdkdelay
 *
 * Purpose:  Block for a specified time.
 *           
 * Params:   timeout - timeout in ms
 *
 * Return:   No value
 *
 * Abort:    none
 *
 * Notes:    none
 *
 **************/
local void swi_ossdkdelay(swi_uint32 timeout)
{
    struct timespec timev;
    struct timespec timer;
    int rv;

    timer.tv_sec = timeout / 1000UL;
    timer.tv_nsec = (timeout * 1000000UL) % 1000000000UL;
    
    do
    {
        timev = timer;        /* remaining time */
        rv = nanosleep(&timev, &timer);
        
    } while (rv < 0 && errno == EINTR);
}

/*************
 *
 * Name: swi_sdkrunning
 *
 * Purpose: Determine if SDK process is already running Leaving 
 *          SWI_SDK_SOCKET_MUTEX socket open
 *           
 * Params:  none
 *
 * Return:  TRUE if already running
 *
 * Abort:   no memory, called many times
 *
 * Notes:   Intented to be called once, at startup
 *
 **************/
local swi_bool swi_sdkrunning(void)
{
    struct sockaddr_un run_sa;
    int delay;
    int sock;
    
    run_sa.sun_family = AF_UNIX;
    memset(run_sa.sun_path, 0, sizeof(run_sa.sun_path)); /* must be zeroed */
    snprintf(&run_sa.sun_path[1], sizeof(run_sa.sun_path) - 1,
        "SWI_SDK_SOCKET_MUTEX");  /* use abstract namespace */
    
    sock = socket(PF_LOCAL, SOCK_DGRAM, 0);    /* must be SOCK_DGRAM */
    
    if (sock < 0)
        swi_ossdkaborterror("SDK run check (s) failed", errno);
    
    delay = 0;
    for ( ; ; )
    {
        if (bind(sock, (struct sockaddr *)&run_sa, 
                                            sizeof(run_sa)) >= 0)
            break;    /* not running */
        
        if (errno != EADDRINUSE)    /* maybe called too many times? */
            swi_ossdkaborterror("SDK run check (b) failed", errno);
        
        if (delay >= SWI_SOCKET_REAP_MS)
            return TRUE;    /* already running */
            
        /* linux takes a while to reap sockets so retry after SWI_RETRY_MS */

        swi_ossdkdelay(SWI_RETRY_MS);
        delay += SWI_RETRY_MS;
    }

    return FALSE;
}


/*************
 *
 * Name: swi_ossdkthreadcreate
 *
 * Purpose: Create a thread in the current process
 *           
 * Params:  fnp    - pointer to function that comprises the thread
 *          paramp - pointer to a parameter or structure that can be 
 *                   passed to the new thread. Can be NULL.
 *
 * Return:  No value
 *
 * Abort:   Out of memory; can't create.
 *
 * Notes:   When paramp is used, make sure storage is allocated. Do not use 
 *          stack space for this call. 
 *
 **************/
void swi_ossdkthreadcreate(void (*fnp)(void * paramp), void *paramp)
{
    pthread_attr_t attr;
    pthread_t tid;
    void *stackloc;

    if ( pthread_attr_init(&attr) < 0)
        swi_ossdkaborterror("thread create (i) failed", errno);

    stackloc = malloc(SWI_STACK_SIZE);
    if (!stackloc)
        swi_ossdkabort("thread create (m) failed: No memory");
    
    if (pthread_attr_setstack(&attr, stackloc, SWI_STACK_SIZE) < 0)
        swi_ossdkaborterror("thread create (s) failed", errno);
        
    if ( pthread_attr_setdetachstate(&attr,
                        PTHREAD_CREATE_DETACHED) < 0)
        swi_ossdkaborterror("thread create (d) failed", errno);

#if ANDROID_NOT_PORTED
    if ( pthread_attr_setinheritsched(&attr,
                        PTHREAD_INHERIT_SCHED) < 0)
        swi_ossdkaborterror("thread create (v) failed", errno);
#endif

    if (pthread_create(&tid, &attr, (void *(*)(void *))fnp, paramp) < 0)
        swi_ossdkaborterror("thread create (c) failed", errno);
}

/*************
 *
 * Name: swi_sdkusbioerrorlocked
 *
 * Purpose: Closes all usb file descriptors. Marks usb in BAD state.
 *           
 * Params:  up  - pointer to usb structure
 *
 * Return:  No value
 *
 * Abort:   none
 *
 * Notes:   Mutex in swi_osusb structure must be previously locked
 *
 **************/
package void swi_sdkusbioerrorlocked(struct swi_osusb *up)
{
    int fd;
    int i;
    
    up->osusbstate = SWI_OS_USB_STATE_BAD;
    
    for (i = 0; i < SWI_OS_MAXENDPOINTS; i++)
    {
        fd = up->osusbreaddescript[i];
        
        if (fd >= 0)    /* if open */
        {
            up->osusbreaddescript[i] = -1;
            swi_sdkfdclose(fd);
        }
        
        fd = up->osusbwritedescript[i];

        if (fd >= 0)    /* if open */
        {
            up->osusbwritedescript[i] = -1;
            swi_sdkfdclose(fd);
        }
    }
}


/*************
 *
 * Name: swi_sdkusbioerrorunlocked
 *
 * Purpose: Closes all usb file descriptors. Marks usb in BAD state.
 *           
 * Params:  up  - pointer to usb structure
 *
 * Return:  No value
 *
 * Abort:   none
 *
 * Notes:   Mutex in swi_osusb structure must be unlocked
 *
 **************/
package void swi_sdkusbioerrorunlocked(struct swi_osusb *up)
{
    if (pthread_mutex_lock(&up->osusbmutex) < 0)
        swi_ossdkaborterror("USB semaphore lock failed", errno);

    swi_sdkusbioerrorlocked(up);    /* error recovery */
    
    if (pthread_mutex_unlock(&up->osusbmutex) < 0)
        swi_ossdkaborterror("USB semaphore unlock failed", errno);

}

/*************
 *
 * Name: swi_ossdkusbinit
 *
 * Purpose: Initialize the USB structure before it is used
 *           
 * Params:  up      - pointer to USB structure
 *
 * Return:  No value
 *
 * Abort:   Linux problem - corrupt or no memory, etc.
 *
 * Notes:   Must be called before any threads use the structure.
 *
 **************/
void swi_ossdkusbinit(struct swi_osusb *up)
{
    int i;
    
    if (pthread_mutex_init(&up->osusbmutex, NULL) < 0)
        swi_ossdkaborterror("USB semaphore initialize failed", errno);

    up->osusbstate = SWI_OS_USB_STATE_BAD;

    /* Initially, no device detected */
    up->osusbvendorid = 0;
    up->osusbproductid = 0;
    
    /* Initialize the per-endpoint items */
    for (i = 0; i < SWI_OS_MAXENDPOINTS; i++)
    {
        up->osusbreadendpointtty[i].osusbttynum = ~0U;
        up->osusbreadendpointtty[i].osusbifnum = ~0U;
        up->osusbwriteendpointtty[i].osusbttynum = ~0U;
        up->osusbwriteendpointtty[i].osusbifnum = ~0U;
        
        up->osusbreaddescript[i] = -1;        /* invalid file descriptor */
        up->osusbwritedescript[i] = -1;        /* invalid file descriptor */

        /* Set the read termination parameters to their default values */
        up->osusbreadterminator[i] = SWI_NOREAD_TERMINATION;
        up->osusbreadthreshold[i] = SWI_NOREAD_THRESHOLD;
    }

    swi_ossdkusbscan(up);    /* see if modem is already present */
                 /* if not, this call will be made later, anyway */
}

/*************
 *
 * Name: swi_ossdkusbproduct
 *
 * Purpose: Get the product IDs of the USB device, if it exists
 *           
 * Params:  up         - pointer to usb structure
 *          vendoridp  - pointer to location in which to store vendor ID
 *          productidp - pointer to location in which to store product ID
 *
 * Return:  TRUE if product exists, false if not
 *
 * Abort:   Linux problem - corrupt or no memory, etc.
 *
 * Notes:   swi_ossdkusbinit must have been called first
 *            May return false at startup until modem recognized;
 *            calling swi_ossdkusbwaitmodem first is recommended
 *
 **************/
swi_bool swi_ossdkusbproduct(struct swi_osusb *up, 
                             swi_uint32 *vendoridp, 
                             swi_uint32 *productidp)
{
    swi_uint32 state;

    if (pthread_mutex_lock(&up->osusbmutex) < 0)
        swi_ossdkaborterror("USB semaphore lock failed", errno);

    state = up->osusbstate;
    *vendoridp = up->osusbvendorid;
    *productidp = up->osusbproductid;
    
    if (pthread_mutex_unlock(&up->osusbmutex) < 0)
        swi_ossdkaborterror("USB semaphore unlock failed", errno);
    
    return state == SWI_OS_USB_STATE_OK;
}

/*************
 *
 * Name: swi_endpointfindbyif
 *
 * Purpose: Find entry in endpoint array with specified ifnum
 *           
 * Params:  ifnum - interface num to find
 *          ep    - array of endpoints to search in
 *          epcnt - array size (in items) 
 *
 * Return:  index of found entry, or -1 if not found
 *
 * Abort:   None
 *
 * Notes:   None 
 *
 **************/
local int swi_endpointfindbyif( 
        const swi_uint32 ifnum, 
        const struct swi_endpointtty * ep, 
        const int epcnt )
{
    int i;
    
    for( i=0; i<epcnt; i++ )
    {
        if( ep[i].osusbifnum == ifnum )
            return i;
    }
    return -1;
}

/*************
 *
 * Name: swi_endpointcompare
 *
 * Purpose: Compare two endpointty structures to determine equivalence
 *           
 * Params:  e1 - endpointty1
 *          e2 - endpointty2 
 *
 * Return:  FALSE when not equivalent, TRUE otherwise
 *
 * Abort:   None
 *
 * Notes:   None 
 *
 **************/
package swi_bool swi_endpointcompare( 
        const struct swi_endpointtty * e1, 
        const struct swi_endpointtty * e2 )
{
    if(e1->osusbttynum == e2->osusbttynum && e1->osusbifnum  == e2->osusbifnum)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*************
 *
 * Name: swi_ossdkusmapifacetoep
 *
 * Purpose: Map iface number to an endpoint
 *           
 * Params:  up - pointer to usb structure
 *          iface - usb interface number to look for
 *          direction - direction of the sought endpoint   
 *
 * Return:  endpoint number from specified interface or -1 if not found
 *
 * Abort:   None
 *
 * Notes:   None 
 *
 **************/
global int swi_ossdkusmapifacetoep( struct swi_osusb *up,
                                    swi_uint32 iface,
                                    enum swi_endpointdirection direction )
{
    struct swi_endpointtty * ep;
    swi_uint32 ret_val;
    int        itemcnt;
    
    if( direction == SWI_EP_IN )
    {
        ep = up->osusbreadendpointtty;
        itemcnt = sizeof(up->osusbreadendpointtty)
                        / sizeof(up->osusbreadendpointtty[0]);
    }
    else
    {
        ep = up->osusbwriteendpointtty;
        itemcnt = sizeof(up->osusbwriteendpointtty)
                        / sizeof(up->osusbwriteendpointtty[0]);
    }
    
    if (pthread_mutex_lock(&up->osusbmutex) < 0)
        swi_ossdkaborterror("USB semaphore lock failed", errno);

    ret_val = swi_endpointfindbyif( iface, ep, itemcnt );

    if (pthread_mutex_unlock(&up->osusbmutex) < 0)
        swi_ossdkaborterror("USB semaphore unlock failed", errno);
    
    return ret_val;
}

/*************
 *
 * Name: swi_ossdkusbendpoint
 *
 * Purpose: Get the OS text name of an endpoint (for applications doing I/O)
 *           
 * Params:  up           - pointer to usb structure
 *          endpoint     - endpoint ID
 *          readnotwrite - TRUE if read endpoint, FALSE if write outpoint
 *          namep        - pointer to location in which to store the name
 *          size         - size of name storage (should be PATH_MAX)
 *
 * Return:  TRUE if endpoint exist and name was copied, FALSE if not
 *
 * Abort:   Huge endpoint number
 *          Linux problem - corrupt or no memory, etc.
 *
 * Notes:   swi_ossdkusbinit must have been called first
 *          May return false at startup until modem recognized;
 *          calling swi_ossdkusbwaitmodem first is recommended
 *          Must be called from a thread that can access the usb structure
 *
 **************/
swi_bool swi_ossdkusbendpoint(struct swi_osusb *up, 
                              swi_uint32 endpoint,
                              swi_bool readnotwrite,
                              char *namep, 
                              swi_size_t size)
{
    swi_uint32 ttyn;
    swi_uint32 state;
    struct stat ss;
    int rv;
    
    if (endpoint >= SWI_OS_MAXENDPOINTS)
        swi_ossdkabort("getting name of illegal endpoint number");
        
    if (pthread_mutex_lock(&up->osusbmutex) < 0)
        swi_ossdkaborterror("USB semaphore lock failed", errno);

    state = up->osusbstate;
    
    if (readnotwrite)
        ttyn = up->osusbreadendpointtty[endpoint].osusbttynum;
    else
        ttyn = up->osusbwriteendpointtty[endpoint].osusbttynum;
    
    if (pthread_mutex_unlock(&up->osusbmutex) < 0)
        swi_ossdkaborterror("USB semaphore unlock failed", errno);
    
    if (state != SWI_OS_USB_STATE_OK || ttyn >= SWI_OS_MAXUSBTTYS)
        return FALSE;
    
    /* unlike all the other ttys, ttyUSBn contains a decimal number */
    snprintf(namep, size, "/dev/ttyUSB%u", (unsigned int)ttyn);
    
    rv = stat(namep, &ss);    /* see if it exists */
    if (rv >= 0 && S_ISCHR(ss.st_mode))    /* is character device ? */
        return TRUE;

    swi_sdkusbioerrorunlocked(up);    /* error recovery */
     
    return FALSE;
}

/*************
 *
 * Name: swi_ossdkusbwrite
 *
 * Purpose: Write characters to a USB endpoint
 *           
 * Params:  up       - pointer to usb structure
 *          endpoint - endpoint ID
 *          bufp     - pointer to buffer to write
 *          length   - size of buffer to write (in bytes)
 *
 * Return:  TRUE if OK, FALSE if device no longer exists
 *
 * Abort:   Huge endpoint number
 *
 * Notes:   swi_ossdkusbinit must have been called first
 *          Only one thread may write to each endpoint.
 *
 **************/
swi_bool swi_ossdkusbwrite(struct swi_osusb *up, 
                           swi_uint32 endpoint, 
                           const void *bufp, 
                           swi_size_t length)
{
    int fd;
    swi_ssize_t rv;
    swi_uint32 state;
    char pathp[SWI_USB_MAX_PATH];
    
    if (endpoint >= SWI_OS_MAXENDPOINTS)
            swi_ossdkabort("USB write to illegal endpoint number");

    if (up->osusbstate != SWI_OS_USB_STATE_OK)
        return FALSE;
        
    fd = up->osusbwritedescript[endpoint];
    
    if (fd < 0)        /* if file descriptor not open */
    {
        if (!swi_ossdkusbendpoint(up, endpoint, FALSE,
                        pathp, sizeof(pathp)))    /* if no write endpoint */
            return FALSE;

        fd = open(pathp, O_WRONLY | O_NOCTTY);

        if (fd < 0)
        {
            swi_ossdklogerror("USB write open error", errno);
            swi_sdkusbioerrorunlocked(up);    /* error recovery */
            return FALSE;
        }
        
        /* Opened correctly, save the descriptor */
        if (pthread_mutex_lock(&up->osusbmutex) < 0)
            swi_ossdkaborterror("USB semaphore lock failed", errno);
    
        state = up->osusbstate;
        if (state == SWI_OS_USB_STATE_OK)
            up->osusbwritedescript[endpoint] = fd;
        
        if (pthread_mutex_unlock(&up->osusbmutex) < 0)
            swi_ossdkaborterror("USB semaphore unlock failed", errno);
        
        if (state != SWI_OS_USB_STATE_OK)
        {
            swi_sdkfdclose(fd);
            return FALSE;
        }
    }
    
    do
    {
        rv = write(fd, bufp, length);
    } while (rv < 0 && errno == EINTR);
    
    if (rv < 0)    /* if error */
    {
        swi_ossdklogerror("USB write error", errno);
        swi_sdkusbioerrorunlocked(up);    /* error recovery */
        return FALSE;
    }

    return TRUE;
}

/*************
 *
 * Name: swi_ossdkusbread
 *
 * Purpose: Read characters from a USB enpoint
 *           
 * Params: up        - pointer to usb structure
 *         endpoint  - endpoint ID
 *         bufp      - pointer to buffer in which to read
 *         lengthp   - pointer to size of read buffer
 *         timeout   - timeout in ms (0 to wait forever)
 *
 * Return: TRUE if OK, FALSE if device no longer exists
 *
 * Abort:  none
 *
 * Notes:  The number of bytes read is returned in lengthp, which is zero
 *         if the timeout occurred before any bytes arrived
 *
 *         swi_ossdkusbinit must have been called first
 *         Only one thread may read from each endpoint.
 *
 **************/
swi_bool swi_ossdkusbread(struct swi_osusb *up, 
                          swi_uint16 endpoint, 
                          void *bufp, 
                          swi_size_t *lengthp, 
                          swi_uint32 timeout)
{
    int fd;
    char *readdonep;
    swi_uint32 state;
    swi_ssize_t rv;
    char pathp[SWI_USB_MAX_PATH];
    swi_uint32 delayed;
    char *cp;
    char *cstartp;
    char *cendp;
    struct termios newtio;
    
    if (endpoint >= SWI_OS_MAXENDPOINTS)
            swi_ossdkabort("USB read from illegal endpoint number");
        
    if (up->osusbstate != SWI_OS_USB_STATE_OK)
    {
        *lengthp = 0;
        return FALSE;
    }
    
    fd = up->osusbreaddescript[endpoint];
    
    if (fd < 0)        /* if file descriptor not open */
    {
        if (!swi_ossdkusbendpoint(up, endpoint, TRUE,
                        pathp, sizeof(pathp)))    /* if no read endpoint */
        {
            *lengthp = 0;
            return FALSE;
        }
        
        fd = open(pathp, O_RDONLY | O_NOCTTY | O_NONBLOCK );
        
        if (fd < 0)
        {
            swi_ossdklogerror("USB read open error", errno);
            swi_sdkusbioerrorunlocked(up);    /* error recovery */
            *lengthp = 0;
            return FALSE;
        }

        /* Opened correctly, save the descriptor */
        if (pthread_mutex_lock(&up->osusbmutex) < 0)
            swi_ossdkaborterror("USB semaphore lock failed", errno);
    
        state = up->osusbstate;
        if (state == SWI_OS_USB_STATE_OK)
            up->osusbreaddescript[endpoint] = fd;
        
        if (pthread_mutex_unlock(&up->osusbmutex) < 0)
            swi_ossdkaborterror("USB semaphore unlock failed", errno);
        
        if (state != SWI_OS_USB_STATE_OK)
        {
            swi_sdkfdclose(fd);
            *lengthp = 0;
            return FALSE;
        }
        
        /* Reconfigure the serial port */
        memset(&newtio, 0, sizeof(newtio));
        newtio.c_cflag = CS8 | CLOCAL | CREAD;
        newtio.c_iflag = IGNPAR;
             
        /* Select the read() termination parameters as follows:
         * Intercharacter timeout = 1/10th of a second.
         * Minimum number of characters read = 1 
         *
         * This is to be interpreted as follows:
         * The intercharacter timer is not started until the
         * first character is received. Then after VTIME 1/10ths
         * of a second, if no additional characters have been 
         * received, return what we've got so far. The read()
         * call will also return when a minimum of VMIN chars
         * have been received.
         */
        newtio.c_cc[VTIME]    = 1;  /* inter-character timer */
        newtio.c_cc[VMIN]     = 1; /* blocking read until n chars received */
        
        tcflush(fd, TCIFLUSH);
        rv = tcsetattr(fd, TCSANOW, &newtio);
        if (rv < 0)
            swi_ossdklogerror("USB tcsetattr error", errno);
            
    }

    delayed = 0;
    cp = (char *)bufp;
    cendp = cp + *lengthp;
    
    /* Readdonep = NULL means don't exit yet */
    readdonep = NULL;
    while (cp < cendp && !readdonep)
    {
        rv = read(fd, cp, cendp - cp);
        
        if (rv < 0)
        {
            if (errno == EAGAIN)
            {
                if (delayed > timeout && timeout > 0)    /* if timeout */
                {
                    /* Uncomment the following line for debug logging */
                    //swi_ossdklog("USB read timeout exit");
                    *lengthp = cp - (char *)bufp;
                    return TRUE;
                }
                else    /* else continue to read */
                {
                    /* Uncomment the following line for debug logging */
                    //swi_ossdklog("USB read delay");
                    /* Hard coded to 10 for now. may be tune further */   
                    swi_ossdkdelay(10);
                    delayed += SWI_RETRY_MS;
                }
            }
            else if (errno != EINTR)        /* else error */
            {
                swi_ossdklogerror("USB read error", errno);
                swi_sdkusbioerrorunlocked(up);    /* error recovery */
                *lengthp = 0;
                return FALSE;
            }
        }
        else if (rv == 0)    /* end of file is also an error */
        {
            swi_ossdklog("USB read unexpected EOF error");
            swi_sdkusbioerrorunlocked(up);    /* error recovery */
            *lengthp = 0;
            return FALSE;
        }
        else        /* rv > 0 means characters read */
        {
            cstartp = cp;
            cp += rv;
 
            /* Try to match the incoming character with the defined
             * threshold character, if one has been defined. If the
             * character appears anywhere in the newly received 
             * data, then we will return.
             */
            if( up->osusbreadterminator[endpoint] != SWI_NOREAD_TERMINATION )
            {
                /* Scan through the just-received characters for 
                 * the termination character
                 */
                while(cstartp < cp)
                {
                    /* If at least 1 char matches the configured char 
                     * then that's a good enough reason to exit
                     */
                    if( *cstartp == (char) up->osusbreadterminator[endpoint] )
                    {
                        /* indicate char match return */
                        readdonep = "USB Read: Char matched";
                        break;
                    }
                    cstartp++;
                }
            }

            /* Only do the next check if the previous one found no match */
            if( !readdonep )
            {
                /* Can return if we've read a minimum number of characters */
                if( ( cp - (char *) bufp ) >= (int)up->osusbreadthreshold[endpoint] )
                {
                    /* indicate length threshold return */
                    readdonep = "USB Read: Rcvd large pkt";
                } 
            }
        }
    }

    /* Uncomment the following two lines for debug logging */
    // if( readdonep )
        //swi_ossdklog( readdonep );

    *lengthp = cp - (char *)bufp;
    return TRUE;
}

