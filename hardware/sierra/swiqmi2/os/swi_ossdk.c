/*************
 *
 * Filename: swi_ossdk.c
 *
 * Purpose: Wrap OS functions for SDK-side (SDK process)
 *
 * Copyright: © 2011-2012 Sierra Wireless Inc., all rights reserved
 *
 **************/

/* include files */
#include "aa/aaglobal.h"        /* system wide typedefs */
#include "swi_ossdk.h"          /* package user include file */
#include "qm/qmudefs.h"
#include "qm/qmerrno.h"
#include "us/usudefs.h"
#include "am/amudefs.h"
#include <sys/ioctl.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <dirent.h>
#ifndef ANDROID_LOGGING
#include <syslog.h>
#endif
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
#ifndef ANDROID_NO_STACKDUMP
#include <ucontext.h>
#endif
#include <sys/mman.h>

#define SWI_OSWAITSTATE_SLEEP 0
#define SWI_OSWAITSTATE_AWAKE 1
#define SWI_OSWAITSTATE_ALERT 2

#define SWI_SOCKET_REAP_MS 500          /* socket reap time after exit */
#define SWI_RETRY_MS 100                /* milliseconds */
#define SWI_MESSAGE_BYTES 5000          /* max IPC message */
#define SWI_PROCESS_STARTUP_MS 100      /* delay when application down */

#define SWI_MAX_SIGNAL_NAME 16

#define SWI_STACK_SIZE    (1024UL * 64UL)    /* stack size for threads */

#ifdef ANDROID_LOGGING
#define LOG_TAG "RILSLQS"
/* RILSTART */
/* #include <utils/Log.h> */
#include "swiril_log.h"
/* RILSTOP */
#endif

local volatile swi_bool swi_sigdepth = 0;   /* signal nest level */
local volatile pthread_t swi_sigpid = 0;    /* signal thread id */

local pthread_mutex_t swi_sigmutex = PTHREAD_MUTEX_INITIALIZER;
                                    /* signal mutex to prevent multi-threads */
#ifndef ANDROID_NO_STACKDUMP
local ucontext_t *swi_sigcontextp;  /* first signal context for debugger */
#endif
local siginfo_t swi_siginfo;        /* first signal info */

local char *swi_sigtable[] = {      /* names for crash log entries */
    "0", "SIGHUP", "SIGINT", "SIGQUIT", "SIGILL", "SIGTRAP",
    "SIGABRT", "SIGBUS", "SIGFPE", "SIGKILL", "SIGUSR1", "SIGSEGV",
    "SIGUSR2", "SIGPIPE", "SIGALRM", "SIGTERM"
};

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
#ifndef ANDROID_LOGGING
    if (llerrno != 0)
    {
        errno = llerrno;
        syslog(LOG_USER | LOG_ERR, "%s: %m", errorp);
    }
    else
        syslog(LOG_USER | LOG_ERR, "%s", errorp);
#else
    if (llerrno != 0)
        LOGE("%5i : %s", llerrno, errorp);
    else
        LOGI("%s", errorp);
#endif
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
 * Name: swi_sdkonesignalinstall
 *
 * Purpose: Install a signal handler for one signal
 *
 * Params:  signo - signal number
 *          functionp - signal handler function of type
 *                      void (*functionp)(int, siginfo_t *, void *)
 *
 * Return:  No value
 *
 * Abort:   Install failed
 *
 * Notes:   none
 *
 **************/
local void swi_sdkonesignalinstall(swi_uint32 signo,
                                void (*functionp)(int, siginfo_t *, void *))
{
    struct sigaction sa;

    if (functionp == NULL)
        swi_ossdkabort("signal handler install failed: NULL signal handler");

    sa.sa_sigaction = functionp;
    sigemptyset(&sa.sa_mask);

    sa.sa_flags = SA_NODEFER | SA_SIGINFO;

    if (sigaction(signo, &sa, NULL) < 0)
        swi_ossdkaborterror("signal handler install failed", errno);

}

/*************
 *
 * Name: swi_sdkonesignaldeinstall
 *
 * Purpose: Deinstall a signal handler for one signal
 *
 * Params:  signo - signal number
 *
 * Return:  No value
 *
 * Abort:   Install failed
 *
 * Notes:
 *
 **************/
static void swi_sdkonesignaldeinstall(swi_uint32 signo)
{
    struct sigaction sa;

    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(signo, &sa, NULL);
}

/*************
 *
 * Name: swi_SIGQUIThandler
 *
 * Purpose: Signal handler for SIGQUIT
 *
 * Params:  signo - signal number
 *          siginfop - pointer to signal info
 *          contextp - pointer to interrupted user context
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void swi_SIGQUIThandler(int signo, siginfo_t *siginfop, void *contextp)
{
    UNUSEDPARAM( (void *)&signo );
    UNUSEDPARAM( (void *)siginfop );
    UNUSEDPARAM( contextp );

    swi_ossdklog("process terminated by user------------");
    exit(EXIT_SUCCESS);  /* simple exit with no core dump */
}

/*************
 *
 * Name: swi_sdksignalinit
 *
 * Purpose: Install a signal handler for all standard signals.
 *
 * Params:  functionp - signal handler function of type
 *                      void (*functionp)(int, siginfo_t *, void *)
 *
 * Return:   No value
 *
 * Abort:    Install failed
 *
 * Notes:    none
 *
 **************/
local void swi_sdksignalinit(void (*functionp)(int, siginfo_t *, void *))
{
    swi_sdkonesignalinstall(SIGINT, functionp);
    swi_sdkonesignalinstall(SIGQUIT, swi_SIGQUIThandler);
    swi_sdkonesignalinstall(SIGILL, functionp);
    swi_sdkonesignalinstall(SIGABRT, functionp);
    swi_sdkonesignalinstall(SIGFPE, functionp);
    swi_sdkonesignalinstall(SIGSEGV, functionp);
    swi_sdkonesignalinstall(SIGPIPE, functionp);
    swi_sdkonesignalinstall(SIGBUS, functionp);
    swi_sdkonesignalinstall(SIGSYS, functionp);
    swi_sdkonesignalinstall(SIGTRAP, functionp);
    swi_sdkonesignalinstall(SIGTERM, functionp);
}

/*************
 *
 * Name: swi_sdksignalhandler
 *
 * Purpose: Signal handler for all signals.
 *
 * Params:  signo - signal number
 *          siginfop - pointer to signal info
 *          contextp - pointer to interrupted user context
 *
 * Return:  No value
 *
 * Abort:   Install failed
 *
 * Notes:   none
 *
 **************/
local void swi_sdksignalhandler(int signo, siginfo_t *siginfop, void *contextp)
{
    char signalmsg[128];
    char signame[SWI_MAX_SIGNAL_NAME];
    void **regp;
    pthread_t sigpid;

    sigpid = pthread_self();

    if (sigpid != swi_sigpid)
    {
        /* hang any other threads that signal during signal handling */
        pthread_mutex_lock(&swi_sigmutex);
    }

    swi_sigpid = sigpid;        /* save the signal process ID */
    swi_sigdepth += 1;            /* signal nest depth */

    if (swi_sigdepth == 1)    /* if non-nested signal */
    {
        /* save context for nested signals*/
        swi_siginfo = *siginfop;
#ifndef ANDROID_NO_STACKDUMP
        swi_sigcontextp = (ucontext_t *)contextp;
#endif

        /* put info in /var/log/user.log in processor-independent way */

        if (signo < (int) (sizeof(swi_sigtable) / sizeof(swi_sigtable[0])))
            snprintf(signame, sizeof(signame), "%s", swi_sigtable[signo]);
        else
            snprintf(signame, sizeof(signame), "%d", signo);

        snprintf(signalmsg, sizeof(signalmsg),
            "unexpected signal %s, address %p", signame, siginfop->si_addr);

        swi_ossdklog(signalmsg);
        /* Send notification to the host */
        am_sdk_terminated_notify ((swi_uint8*)signalmsg);

#ifndef ANDROID_NO_STACKDUMP
        /* print out the machine context (the registers) */
        /* look in Linux structure mcontext_t for details */
        /* on i86 program counter probably is at offset 0x38 and */
        /* on ARM program counter probably is at offset 0x3c */

        for (regp = (void **)&swi_sigcontextp->uc_mcontext;
                        regp < (void **)(&swi_sigcontextp->uc_mcontext + 1);
                        regp += 1 )
        {
            snprintf(signalmsg, sizeof(signalmsg),
                        "context offset 0x%03x: %p",
                        (char *)regp - (char *)&swi_sigcontextp->uc_mcontext,
                        *regp);

            swi_ossdklog(signalmsg);
        }
#endif

        if (signo == SIGINT || signo == SIGTERM)  /* if no core dump */
        {
            swi_ossdklog("exiting ----------------------");
            exit(1);  /* simple exit with no core dump */
        }

        swi_ossdklog("attempting to dump core ------");

        if (signo != SIGILL && signo != SIGFPE && signo != SIGSEGV)
                                /* if no location to return to for core dump */
        {
            swi_sdkonesignaldeinstall(SIGABRT);
            for ( ; ; )
                raise(SIGABRT);
        }

        /* else return to location for core dump (debug with GDB) */
        swi_sdkonesignaldeinstall(signo);
    }
#ifndef ANDROID_NO_STACKDUMP
    else if (swi_sigdepth == 2)  /* nested signal, not good, try to exit */
    {
        /* restore original, non-nested context */
        *(ucontext_t *)contextp = *swi_sigcontextp;

        if (signo != SIGILL && signo != SIGFPE && signo != SIGSEGV)
                                /* if no location to return to for core dump */
        {
            swi_sdkonesignaldeinstall(SIGABRT);
            for ( ; ; )
                raise(SIGABRT);        /* dump core */
        }

        /* else try to return for core dump */
        swi_sdkonesignaldeinstall(swi_siginfo.si_signo);
        swi_sdkonesignaldeinstall(signo);
    }
#endif
    else /* this handler seriously isn't working -- kill ourselves */
    {
        for ( ; ; )
            raise(SIGKILL);
    }
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
 * Name:    swi_ossdkthreadterminate
 *
 * Purpose: terminate a thread in the current process
 *
 * Params:  none
 *
 * Return:  none
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
global void swi_ossdkthreadterminate(void)
{
    pthread_exit(NULL);
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

    if ( pthread_attr_init(&attr) < 0)
        swi_ossdkaborterror("thread create (i) failed", errno);

    /* PowerPC's PTHREAD_STACK_MIN defaults to 131072 (128K),
     * double our minimum.
     */
#if defined PPC || defined MIPS
    if ( pthread_attr_setstacksize (&attr, 2*SWI_STACK_SIZE) !=0)
#else
    if ( pthread_attr_setstacksize (&attr, SWI_STACK_SIZE) !=0)
#endif
        swi_ossdkabort("thread create (m) failed: No memory");

    if ( pthread_attr_setdetachstate(&attr,
                        PTHREAD_CREATE_DETACHED) < 0)
        swi_ossdkaborterror("thread create (d) failed", errno);

    /* Not directly related to stack dump, but this feature is
     * not available on Android platforms either
     */
#ifndef ANDROID_NO_STACKDUMP
    if ( pthread_attr_setinheritsched(&attr,
                        PTHREAD_INHERIT_SCHED) < 0)
        swi_ossdkaborterror("thread create (v) failed", errno);
#endif


    if (pthread_create(&tid, &attr, (void *(*)(void *))fnp, paramp) < 0)
        swi_ossdkaborterror("thread create (c) failed", errno);
}

/*************
 *
 * Name: swi_ossdkmemory
 *
 * Purpose: Permanently allocate memory
 *
 * Params:  size    - size of memory to allocate
 *
 * Return:  Pointer to memory, aligned to sizeof(void *) or better
 *
 * Abort:   Out of memory, can't allocate
 *
 * Notes:   none
 *
 **************/
void *swi_ossdkmemory(swi_size_t size)
{
    void *memp;

    memp = (void *)malloc(size);

    if (memp)
        return memp;

    swi_ossdkabort("out of memory");            /* doesn't return */
    return (void *)NULL;                    /* keep compiler happy */
}

/*************
 *
 * Name: swi_ossdksemaphoreinit
 *
 * Purpose: Initialize the semaphore structure before it is used
 *
 * Params:  sp      - pointer to semaphore structure
 *
 * Return:  No value
 *
 * Abort:   Semaphore is already in use, out of memory
 *
 * Notes:   Must be called before any threads use the structure.
 *
 **************/
void swi_ossdksemaphoreinit(struct swi_ossemaphore *sp)
{
    if (pthread_mutex_init(&sp->ossemmutex, NULL) < 0)
        swi_ossdkaborterror("semaphore initialize failed", errno);
}

/*************
 *
 * Name: swi_ossdksemaphorelock
 *
 * Purpose: Obtain exclusive access to the semaphore, waiting if necessary
 *
 * Params:  sp      - pointer to semaphore structure
 *
 * Return:  No value
 *
 * Abort:   Semaphore not initialized or locked by same thread
 *
 * Notes:   swi_ossdksemaphoreinit must have been called first
 *
 **************/
void swi_ossdksemaphorelock(struct swi_ossemaphore *sp)
{
    if (pthread_mutex_lock(&sp->ossemmutex) < 0)
        swi_ossdkaborterror("semaphore lock failed", errno);
}

/*************
 *
 * Name: swi_ossdksemaphoreunlock
 *
 * Purpose: Release exclusive access to the semaphore
 *
 * Params:  sp      - pointer to semaphore structure
 *
 * Return:  No value
 *
 * Abort:   semaphore not initialized or not locked by same thread
 *
 * Notes:   swi_ossdksemaphoreinit must have been called first
 *
 *           Semaphore must have been locked by this thread
 *
 **************/
void swi_ossdksemaphoreunlock(struct swi_ossemaphore *sp)
{
    if (pthread_mutex_unlock(&sp->ossemmutex) < 0)
        swi_ossdkaborterror("semaphore unlock failed", errno);
}

/*************
 *
 * Name: swi_ossdkwaiterinit
 *
 * Purpose: Initalize the waiter structure before it is used
 *
 * Params:  wp      - pointer to waiter structure
 *
 * Return:  No value
 *
 * Abort:   Wait structure already in use, out of memory
 *
 * Notes:    Must be called before any threads use the structure.
 *
 **************/
void swi_ossdkwaiterinit(struct swi_oswaiter *wp)
{
    if (pthread_mutex_init(&wp->swi_oswaitmutex, NULL) < 0)
        swi_ossdkaborterror("waiter initialze (m) failed", errno);

    if (pthread_cond_init(&wp->swi_oswaitcondition, NULL) < 0)
        swi_ossdkaborterror("waitier initialize (c) failed", errno);

    wp->swi_oswaitsleepstate = SWI_OSWAITSTATE_AWAKE;
}

/*************
 *
 * Name: swi_ossdkwaiterwait
 *
 * Purpose:  Wait for another thread to wake us up
 *
 * Params:   wp      - pointer to waiter structure
 *           timeout - timeout in ms (0 to wait forever)
 *
 * Return:   TRUE if swi_ossdkwaiterwake may have been called, FALSE if timed
 *           out and swi_ossdkwaiterwake was not called
 *
 * Abort:    Corrupted structure, not initialized
 *
 * Notes:    Spurious wakeups are expected; the thread is guaranteed to wake
 *           up at least once after a swi_ossdkwaiterwake call.
 *
 *           swi_ossdkwaiterinit must have been called first
 *
 **************/
swi_bool swi_ossdkwaiterwait(struct swi_oswaiter *wp, swi_uint32 timeout)
{
    swi_bool waswoken;
    struct timespec times;

    /* clock_gettime requires the -lrt compiler option (real-time library) */
    clock_gettime(CLOCK_REALTIME, &times);

    /* Only do this if a non-zero timeout was specified */
    if( timeout )
    {
        times.tv_sec += timeout / 1000;
        times.tv_nsec += (timeout % 1000) * 1000000UL; /* milliseconds to ns */
        times.tv_sec += times.tv_nsec / 1000000000UL;
        times.tv_nsec %= 1000000000UL; /* nsec must be less than 1E9 */
    }

    if (pthread_mutex_lock(&wp->swi_oswaitmutex) < 0)
        swi_ossdkaborterror("waiter wait (m) failed", errno);

    if (wp->swi_oswaitsleepstate != SWI_OSWAITSTATE_ALERT)
                                /* if nobody woke us up */
    {
        wp->swi_oswaitsleepstate = SWI_OSWAITSTATE_SLEEP;
        if( timeout )
        {
            if (pthread_cond_timedwait(&wp->swi_oswaitcondition,
                        &wp->swi_oswaitmutex, &times) < 0 &&
                        errno != ETIMEDOUT)
                swi_ossdkaborterror("waiter wait (tc) failed", errno);
        }
        /* No timeout specified so use another pthread call */
        else
        {
            if (pthread_cond_wait(&wp->swi_oswaitcondition,
                        &wp->swi_oswaitmutex ) < 0 )
                swi_ossdkaborterror("waiter wait (c) failed", errno);
        }
    }

    waswoken = wp->swi_oswaitsleepstate != SWI_OSWAITSTATE_SLEEP;

    wp->swi_oswaitsleepstate = SWI_OSWAITSTATE_AWAKE;

    if (pthread_mutex_unlock(&wp->swi_oswaitmutex) < 0)
        swi_ossdkaborterror("waiter wait (u) failed", errno);

    return waswoken;
}

/*************
 *
 * Name: swi_ossdkwaiterwake
 *
 * Purpose: Wake up a waiting thread
 *
 * Params:  wp      - pointer to waiter structure
 *
 * Return:  No value
 *
 * Abort:   Corrupted structure, not initialized
 *
 * Notes:   Spurious wakeups are expected; the waiting thread is guaranteed
 *          to wake up at least once after a swi_ossdkwaiterwake call. Wakes
 *          are not stacked; multiple wakes in quick succession may only
 *          result in the waiting thread waking up once.
 *
 *          swi_ossdkwaiterinit must have been called first
 *
 **************/
void swi_ossdkwaiterwake(struct swi_oswaiter *wp)
{
    if (pthread_mutex_lock(&wp->swi_oswaitmutex) < 0)
        swi_ossdkaborterror("waiter wake (m) failed", errno);

    if (wp->swi_oswaitsleepstate == SWI_OSWAITSTATE_SLEEP) /* if asleep */
    {
        if (pthread_cond_signal(&wp->swi_oswaitcondition) < 0)
            swi_ossdkaborterror("waiter wake (c) failed", errno);
    }

    wp->swi_oswaitsleepstate = SWI_OSWAITSTATE_ALERT;

    if (pthread_mutex_unlock(&wp->swi_oswaitmutex) < 0)
        swi_ossdkaborterror("waiter wake (u) failed", errno);
}

/*************
 *
 * Name:     swi_ossdkfileopenread
 *
 * Purpose:  Open a disk file for read only of binary bytes
 *
 * Params:   fp      - pointer to file control structure
 *           pathp   - pointer to string containing file path
 *
 * Return:   TRUE if OK, false if file doesn't exist
 *
 * Abort:    none
 *
 * Notes:    none
 *
 **************/
swi_bool swi_ossdkfileopenread(struct swi_osfile *fp, const char *pathp)
{
    int rv;

    rv = open(pathp, O_RDONLY);

    fp->osfiledescrip = rv;

    return rv >= 0;
}

/*************
 *
 * Name: swi_ossdkfileclose
 *
 * Purpose: Close a disk file opened with swi_ossdkfileopenread or
 *          swi_ossdkfileopenwrite
 *
 * Params:  fp      - pointer to file control structure
 *
 * Return:  No value
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
void swi_ossdkfileclose(struct swi_osfile *fp)
{
    if (fp->osfiledescrip < 0)
        return;

    swi_sdkfdclose(fp->osfiledescrip);

    fp->osfiledescrip = -1;
}

/*************
 *
 * Name: swi_ossdkfileread
 *
 * Purpose: Read binary bytes from a disk file
 *
 * Params:  fp      - pointer to file control structure
 *          bufp    - pointer to buffer in which to read
 *          lengthp - pointer to size of read buffer
 *
 * Return:  TRUE if OK, FALSE if file system error
 *
 * Abort:   none
 *
 * Notes:   The number of bytes read is returned in lengthp, which is zero
 *          if the end of file was reached.
 *
 *          swi_ossdkfileopenread must have been called first
 *
 **************/
swi_bool swi_ossdkfileread(struct swi_osfile *fp,
                           void *bufp,
                           swi_size_t *lengthp)
{
    swi_ssize_t rv;

    if (fp->osfiledescrip < 0)
    {
        *lengthp = 0;
        return FALSE;
    }

    do
    {
        rv = read(fp->osfiledescrip, bufp, *lengthp);

    } while (rv < 0 && errno == EINTR);

    if (rv < 0)
    {
        *lengthp = 0;
        return FALSE;
    }

    *lengthp = (swi_size_t)rv;
    return TRUE;
}
/*************
 *
 * Name: swi_ossdkfilesize
 *
 * Purpose: Returns the total size of the file in bytes
 *
 * Params:  fp    - pointer to file control structure
 *          sizep - memory to return the file size (in bytes)
 *
 * Return:  TRUE if OK, FALSE if file system error
 *
 * Abort:   none
 *
 * Notes:   swi_ossdkfileopenread must have been called first
 *
 **************/
swi_bool swi_ossdkfilesize(struct swi_osfile *fp, swi_size_t *sizep)
{
    struct stat finfo;

    *sizep = 0;

    if (fp->osfiledescrip < 0)
    {
        return FALSE;
    }

    if(fstat(fp->osfiledescrip, &finfo) < 0)
    {
        return FALSE;
    }

    *sizep = (swi_size_t)finfo.st_size;

    return TRUE;
}

/*************
 *
 * Name: swi_ossdkipcformlocalsocketname
 *
 * Purpose: Form local socket name
 *
 * Params:  str         - Point to buffer that save socket name.
 *          size        - Size of the buffer pointer by str
 *          ipcindex    - IPC channel No.
 *          ipcnamep    - Pointer to channel name.
 *
 * Return:  None.
 *
 * Abort:   None.
 *
 * Notes:   The implementation of this function must match the implementation
 *          of function swi_osapiipcformremotesocketname().
 *
 **************/
local void swi_ossdkipcformlocalsocketname(char *str,
                                      size_t size,
                                      size_t ipcindex,
                                      swi_uint8 * ipcnamep)
{
    memset(str, 0, size); /* must be zeroed */

    /* use abstract namespace */
    snprintf(str+1, size - 1, "SWI_SDK_%s_SOCKET_%u", ipcnamep, ipcindex);
}

/*************
 *
 * Name: swi_ossdkipcformremotesocketname
 *
 * Purpose: Form remote socket name
 *
 * Params:  str         - Point to buffer that save socket name.
 *          size        - Size of the buffer pointer by str
 *          ipcindex    - IPC channel No.
 *          ipcnamep    - Pointer to channel name.
 *
 * Return:  None.
 *
 * Abort:   None.
 *
 * Notes:   The implementation of this function must match the implementation
 *          of function swi_osapiipcformlocalesocketname().
 *
 **************/
local void swi_ossdkipcformremotesocketname(char *str,
                                      size_t size,
                                      size_t ipcindex,
                                      swi_uint8 * ipcnamep)
{
    memset(str, 0, size); /* must be zeroed */

    /* use abstract namespace */
    snprintf(str+1, size - 1, "SWI_API_%s_SOCKET_%u", ipcnamep, ipcindex);
}

/*************
 *
 * Name: swi_ossdkipcformlocalsocketaddr
 *
 * Purpose: Form local socket address
 *
 * Params:  sap         - Pointer to structure that save socket address.
 *          ipcindex    - IPC channel No.
 *          ipcnamep    - Pointer to channel name.
 *
 * Return:  None.
 *
 * Abort:   None.
 *
 **************/
local void swi_ossdkipcformlocalsocketaddr(struct sockaddr_un *sap,
                                         size_t ipcindex,
                                         swi_uint8 * ipcnamep)
{
    sap->sun_family = AF_UNIX;
    swi_ossdkipcformlocalsocketname(sap->sun_path, sizeof(sap->sun_path),
            ipcindex, ipcnamep);
}

/*************
 *
 * Name: swi_ossdkipcformremotesocketaddr
 *
 * Purpose: Form remote socket address
 *
 * Params:  sap         - Pointer to structure that save socket address.
 *          ipcindex    - IPC channel No.
 *          ipcnamep    - Pointer to channel name.
 *
 * Return:  None.
 *
 * Abort:   None.
 *
 **************/
local void swi_ossdkipcformremotesocketaddr(struct sockaddr_un *sap,
                                         size_t ipcindex,
                                         swi_uint8 * ipcnamep)
{
    sap->sun_family = AF_UNIX;
    swi_ossdkipcformremotesocketname(sap->sun_path, sizeof(sap->sun_path),
            ipcindex, ipcnamep);
}

/*************
 *
 * Name: swi_ossdkipcsetdest
 *
 * Purpose: Set destination socket
 *
 * Params:  ip          - Pointer to structure that saves destination socket
 *                        address to be set.
 *          ipcindex    - IPC channel No. to set.
 *          ipcnamep    - Pointer to channel name.
 *
 * Return:  None.
 *
 * Abort:   None.
 *
 **************/
void swi_ossdkipcsetdest(struct swi_osipc *ip,
                         swi_size_t ipcindex,
                         swi_uint8 * ipcnamep)
{
    swi_ossdkipcformremotesocketaddr(&ip->osipcdest, ipcindex, ipcnamep);
}

/*************
 *
 * Name: swi_ossdkipccleardest
 *
 * Purpose: Clear destination socket
 *
 * Params:  ip          - Pointer to structure that saves destination socket
 *                        address to be cleared.
 *
 * Return:  None.
 *
 * Abort:   None.
 *
 **************/
void swi_ossdkipccleardest(struct swi_osipc *ip)
{
    ip->osipcdest.sun_family = AF_UNSPEC;
}

/*************
 *
 * Name: swi_ossdkisdestset
 *
 * Purpose: Identify whether destination socket is set.
 *
 * Params:  ip          - Pointer to structure that saves destination socket
 *                        address to be set.
 *
 * Return:  TRUE if destination is set; otherwise return FALSE;
 *
 * Abort:   None.
 *
 **************/
local swi_bool swi_ossdkisdestset(struct swi_osipc *ip)
{
    return (AF_UNSPEC != ip->osipcdest.sun_family);
}

/*************
 *
 * Name: swi_ossdkipcopen
 *
 * Purpose: Open an inter-process communication channel on the SDK side to
 *          communicate with the application process or processes
 *
 * Params:  ip       - pointer to IPC structure
 *          ipcindex - channel number
 *          ipcnamep - pointer to channel name
 *
 * Return:  No value
 *
 * Abort:   No permission, no kernel memory, called twice with same channel
 *          number
 *
 * Notes:   none
 *
 **************/
void swi_ossdkipcopen(  struct swi_osipc *ip,
                        swi_size_t ipcindex,
                        swi_uint8 * ipcnamep)
{
    struct sockaddr_un loc_sa;
    int socketsize;
    int delay;

    /* build our local address */
    swi_ossdkipcformlocalsocketaddr(&loc_sa, ipcindex, ipcnamep);

    /* build remote address in IPC structure */
    swi_ossdkipcsetdest(ip, ipcindex, ipcnamep);

    ip->osipcsocket = socket(PF_LOCAL, SOCK_DGRAM, 0);

    if (ip->osipcsocket < 0)
    {
        swi_ossdkaborterror("IPC open (s) failed", errno);
    }

    delay = 0;
    for ( ; ; )
    {
        /* bind local address */
        if (bind(ip->osipcsocket, (struct sockaddr *)&loc_sa,
                                            sizeof(loc_sa)) >= 0)
            break;

        if (delay >= SWI_SOCKET_REAP_MS ||
                            errno != EADDRINUSE)
        {
            swi_ossdkaborterror("IPC open (b) failed", errno);
        }

        /* linux takes a while to reap sockets so retry after SWI_RETRY_MS */
        swi_ossdkdelay(SWI_RETRY_MS);
        delay += SWI_RETRY_MS;
    }

    socketsize = SWI_MESSAGE_BYTES;
    if (setsockopt(ip->osipcsocket, SOL_SOCKET, SO_SNDBUF, &socketsize,
                                sizeof(socketsize)) < 0)
    {
        swi_ossdkaborterror("IPC open (o) failed", errno);
    }
}

/*************
 *
 * Name: swi_ossdkipcclose
 *
 * Purpose: Close socket
 *
 * Params:  picp        - Pointer to structure that saves destination socket
 *                        address to be closed.
 *
 * Return:  TRUE if socket is closed succefully; otherwise return FALSE;
 *
 * Abort:   None.
 *
 **************/
swi_bool swi_ossdkipcclose(struct swi_osipc *picp)
{
    /* Close socket */
    return (close(picp->osipcsocket) == 0);
}

/*************
 *
 * Name: swi_ossdkipcwrite
 *
 * Purpose:  Write a message to an IPC channel
 *
 * Params:   ip      - pointer to IPC structure
 *           bufp    - pointer to buffer to write
 *           length  - size of buffer to write (in bytes)
 *
 * Return:   TRUE if OK, false if channel doesn't exist (e.g., if application
 *           process no longer exists); discard msg and continue
 *
 * Abort:    Unexpected Linux error
 *
 * Notes:    swi_ossdkipcapiopen or swi_ossdkipcsdkopen must have been
 *           called first
 *
 **************/
swi_bool swi_ossdkipcwrite(struct swi_osipc *ip,
                           const void *bufp,
                           swi_size_t length)
{
    int delay;

    delay = 0;
    for ( ; ; )
    {
        if (sendto(ip->osipcsocket, bufp, length, MSG_EOR | MSG_NOSIGNAL,
                        (struct sockaddr *)&ip->osipcdest,
                        sizeof(ip->osipcdest)) >= 0)
            break;

        if (errno == EINTR)
        {
            /* signal - just retry */
        }
        else if (errno != ECONNREFUSED)
        {
            swi_ossdkaborterror("IPC write failed", errno);
        }
        else if (delay > 0)    /* if tried once before */
        {
            return FALSE;
        }
        else
        {
            swi_ossdkdelay(SWI_RETRY_MS);
            delay += SWI_RETRY_MS;
        }
    }

    return TRUE;
}

/*************
 *
 * Name: swi_ossdkipcread
 *
 * Purpose: Read a message from an IPC channel
 *
 * Params:  ip      - pointer to IPC structure
 *          bufp    - pointer to buffer in which to read
 *          lengthp - pointer to size of read buffer
 *          timeout - timeout in ms (0 to wait forever)
 *
 * Return:  TRUE if OK, false if timeout occurs.
 *
 * Abort:   Unexpected Linux error
 *
 * Notes:   The number of bytes read is returned in lengthp, which is zero
 *          if the timeout occurred before a message arrived
 *
 *          There is no way to determine if the application process is running
 *          with this function. Just waits if not.
 *
 *          swi_ossdkipcapiopen or swi_ossdkipcsdkopen must have been called
 *          first
 *
 **************/
swi_bool swi_ossdkipcread(struct swi_osipc *ip,
                          void *bufp,
                          swi_size_t *lengthp,
                          swi_uint32 timeout)
{
    swi_ssize_t rv;
    struct timeval times;
    struct timeval *timesp;
    fd_set fdmask;

    if (timeout != 0)
    {
        timesp = &times;
        times.tv_sec = timeout / 1000UL;
        times.tv_usec = (timeout * 1000UL) % 1000000UL;
    }
    else
        timesp = NULL;

    /* wait for message */

    FD_ZERO(&fdmask);
    FD_SET(ip->osipcsocket, &fdmask); /* set file descriptor mask bit */
    do {
        rv = select(ip->osipcsocket + 1, &fdmask, NULL, &fdmask, timesp);
    } while (rv < 0 && errno == EINTR);

    if (rv < 0)
    {
        swi_ossdkaborterror("IPC read (s) failed", errno);
    }
    else if (rv == 0 && timeout > 0)    /* timeout */
    {
        *lengthp = 0;
        return FALSE;
    }
    /* else rv is 1 */

    /* read message */
    if(swi_ossdkisdestset(ip))
    {
        /* In case IPC destination is set, use recv() to receive data */
        do
        {
            rv = recv(ip->osipcsocket, bufp, *lengthp, MSG_NOSIGNAL);
        } while (rv < 0 && errno == EINTR);
    }
    else
    {
        socklen_t fromlen;

        /* In case IPC destination is not set, use recvfrom() to receive data,
           destination is set after returning from recvfrom() successfully. */
        do
        {
            fromlen = sizeof(ip->osipcdest);
            rv = recvfrom(ip->osipcsocket, bufp, *lengthp, MSG_NOSIGNAL,
                    (struct sockaddr *)&ip->osipcdest, &fromlen);
        } while (rv < 0 && errno == EINTR);
    }

    if (rv < 0)
    {
        swi_ossdkaborterror("IPC read (r) failed", errno);
    }

    *lengthp = (swi_size_t)rv;
    return TRUE;
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
    for( i = 0; i < SWI_OS_MAXENDPOINTS; i++)
    {
        int j;
        for( j = 0 ; j < QMI_SUPPORTED_CLNT_MAX ; j ++ )
        {
            fd = up->osusbreaddescriptqmi[i][j];

            if (fd >= 0)    /* if open */
            {
                up->osusbreaddescriptqmi[i][j] = -1;
                swi_sdkfdclose(fd);
            }

            fd = up->osusbwritedescriptqmi[i][j];

            if (fd >= 0)    /* if open */
            {
                up->osusbwritedescriptqmi[i][j] = -1;
                swi_sdkfdclose(fd);
            }
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

    /* trigger device notification callbacks for registered clients */
    us_trigger_device_notification_callbacks( US_DEVICE_DISCONNECTED_EVT );
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

    up->osusbstate = SWI_OS_USB_STATE_CHECK;

    /* Initially, no device detected */
    up->osusbvendorid = 0;
    up->osusbproductid = 0;

    /* Initialize the per-endpoint items */
    int count = sizeof(up->osusbreaddescriptqmi) / sizeof(up->osusbreaddescriptqmi[0]);
    for (i = 0; i < count; i++)
    {
        /* tty */
        up->osusbreadendpointtty[i].osusbttynum     = SWI_USB_INVALID_DEVICE_NUM;
        up->osusbreadendpointtty[i].osusbifnum      = SWI_USB_INVALID_DEVICE_NUM;
        up->osusbwriteendpointtty[i].osusbttynum    = SWI_USB_INVALID_DEVICE_NUM;
        up->osusbwriteendpointtty[i].osusbifnum     = SWI_USB_INVALID_DEVICE_NUM;

        up->osusbreaddescript[i]    = -1;   /* invalid file descriptor */
        up->osusbwritedescript[i]   = -1;   /* invalid file descriptor */

        /* qcqmi */
        up->osusbreadendpointqcqmi[i].osusbqcqminum     = SWI_USB_INVALID_DEVICE_NUM;
        up->osusbreadendpointqcqmi[i].osusbifnum        = SWI_USB_INVALID_DEVICE_NUM;
        up->osusbwriteendpointqcqmi[i].osusbqcqminum    = SWI_USB_INVALID_DEVICE_NUM;
        up->osusbwriteendpointqcqmi[i].osusbifnum       = SWI_USB_INVALID_DEVICE_NUM;

        int j;
        int clientCount = sizeof(up->osusbreaddescriptqmi[0]) / sizeof(up->osusbreaddescriptqmi[0][0]);
        for( j = 0 ; j < clientCount ; j++ )
        {
            up->osusbreaddescriptqmi[i][j]  = -1;
            up->osusbwritedescriptqmi[i][j] = -1;
        }

        /* Set the read termination parameters to their default values */
        up->osusbreadterminator[i] = SWI_NOREAD_TERMINATION;
        up->osusbreadthreshold[i] = SWI_NOREAD_THRESHOLD;
    }
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
swi_bool swi_ossdkusbproduct( struct swi_osusb *up,
                              swi_uint32 *vendoridp,
                              swi_uint32 *productidp )
{
    swi_uint32 state;

    if (pthread_mutex_lock(&up->osusbmutex) < 0)
        swi_ossdkaborterror("USB semaphore lock failed", errno);

    *vendoridp = up->osusbvendorid;
    *productidp = up->osusbproductid;

    state = up->osusbstate; /* read current state */

    if (pthread_mutex_unlock(&up->osusbmutex) < 0)
        swi_ossdkaborterror("USB semaphore unlock failed", errno);

    return SWI_OS_USB_STATE_OK == state;
}

/*************
 *
 * Name:    swi_ossdkusbnuminterfaces
 *
 * Purpose: Get the number of USB interfaces of the connected device
 *
 * Params:  up         - pointer to usb structure
 *          pnumif     - pointer to location in which to store number of USB interfaces
 *
 * Return:  TRUE if product exists, false if not
 *
 * Abort:   Linux problem - corrupt or no memory, etc.
 *
 * Notes:   swi_ossdkusbinit must have been called first.
 *          May return false at startup until modem recognized;
 *          calling swi_ossdkusbwaitmodem first is recommended
 *
 **************/
global swi_uint32 swi_ossdkusbnuminterfaces(struct swi_osusb *up)
{
    return up->osusbNumInterfaces;
}

/*************
 *
 * Name:    swi_endpointfindbyif
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
 * Name: swi_qcqmiendpointcompare
 *
 * Purpose: Compare two endpointty structures to determine equivalence
 *
 * Params:  e1 - qcqmi endpoint 1
 *          e2 - qcqmi endpoint 2
 *
 * Return:  FALSE when not equivalent, TRUE otherwise
 *
 * Abort:   None
 *
 * Notes:   None
 *
 **************/
package swi_bool
swi_qcqmiendpointcompare(
    const struct swi_endpointqcqmi *e1,
    const struct swi_endpointqcqmi *e2 )
{
    if( (e1->osusbqcqminum == e2->osusbqcqminum) &&
        (e1->osusbifnum  == e2->osusbifnum) )
    {
        return TRUE;
    }
    return FALSE;
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
    swi_bool ttyep = TRUE;

    if (endpoint >= SWI_OS_MAXENDPOINTS)
        swi_ossdkabort("getting name of illegal endpoint number");

    if (pthread_mutex_lock(&up->osusbmutex) < 0)
        swi_ossdkaborterror("USB semaphore lock failed", errno);

    state = up->osusbstate;

    if (readnotwrite)
        ttyn = up->osusbreadendpointtty[endpoint].osusbttynum;
    else
        ttyn = up->osusbwriteendpointtty[endpoint].osusbttynum;

    /* if not a USBtty endpoint, check if it's a qcqmi endpoint */
    if( ttyn >= SWI_OS_MAXUSBTTYS )
    {
        ttyep = FALSE;

        if (readnotwrite)
            ttyn = up->osusbreadendpointqcqmi[endpoint].osusbqcqminum;
        else
            ttyn = up->osusbwriteendpointqcqmi[endpoint].osusbqcqminum;
    }

    if (pthread_mutex_unlock(&up->osusbmutex) < 0)
        swi_ossdkaborterror("USB semaphore unlock failed", errno);

    if (state != SWI_OS_USB_STATE_OK || ttyn >= SWI_OS_MAXUSBTTYS)
        return FALSE;

    if(ttyep)
    {
        /* unlike all the other ttys, ttyUSBn contains a decimal number */
        snprintf(namep, size, "/dev/ttyUSB%u", (unsigned int)ttyn);
    }
    else
    {
        snprintf(namep, size, "/dev/qcqmi%u", (unsigned int)ttyn);
    }

    rv = stat(namep, &ss);    /* see if it exists */
    if (rv >= 0 && S_ISCHR(ss.st_mode))    /* is character device ? */
        return TRUE;

    swi_sdkusbioerrorunlocked(up);    /* error recovery */

    return FALSE;
}

/*************
 *
 * Name: swi_ossdkusmapifacetoep_qmi
 *
 * Purpose: Map iface number to a qmi device endpoint
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
global int
swi_ossdkusmapifacetoep_qmi(
    struct swi_osusb *up,
    swi_uint32 iface,
    enum swi_endpointdirection direction )
{
    struct swi_endpointqcqmi *ep;
    int ret_val;
    int itemcnt;

    if( direction == SWI_EP_IN )
    {
        ep = up->osusbreadendpointqcqmi;
        itemcnt = sizeof(up->osusbreadendpointqcqmi)
                        / sizeof(up->osusbreadendpointqcqmi[0]);
    }
    else
    {
        ep = up->osusbwriteendpointqcqmi;
        itemcnt = sizeof(up->osusbwriteendpointqcqmi)
                        / sizeof(up->osusbwriteendpointqcqmi[0]);
    }

    if (pthread_mutex_lock(&up->osusbmutex) < 0)
        swi_ossdkaborterror("USB semaphore lock failed", errno);

    ret_val = swi_endpointfindbyif( iface,
                                    (struct swi_endpointtty *)ep,
                                    itemcnt );

    if (pthread_mutex_unlock(&up->osusbmutex) < 0)
        swi_ossdkaborterror("USB semaphore unlock failed", errno);

    return ret_val;
}

/*************
 *
 * Name:    swi_ossdkusbendpoint_qcqmi
 *
 * Purpose: Get the OS text name of a qcqmi endpoint (for applications doing I/O)
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
swi_bool
swi_ossdkusbendpoint_qcqmi(
    struct swi_osusb *up,
    swi_uint32 endpoint,
    swi_bool readnotwrite,
    char *namep,
    swi_size_t size )
{
    swi_uint32 qcqmin;
    swi_uint32 state;
    struct stat ss;
    int rv;

    if (endpoint >= SWI_OS_MAXENDPOINTS)
        swi_ossdkabort("getting name of illegal endpoint number");

    if (pthread_mutex_lock(&up->osusbmutex) < 0)
        swi_ossdkaborterror("USB semaphore lock failed", errno);

    state = up->osusbstate;

    if (readnotwrite)
    {
        qcqmin = up->osusbreadendpointqcqmi[endpoint].osusbqcqminum;
    }
    else
    {
        qcqmin = up->osusbwriteendpointqcqmi[endpoint].osusbqcqminum;
    }
    if (pthread_mutex_unlock(&up->osusbmutex) < 0)
        swi_ossdkaborterror("USB semaphore unlock failed", errno);

    if (state != SWI_OS_USB_STATE_OK || qcqmin >= SWI_OS_MAXQCQMIS)
        return FALSE;

    snprintf(namep, size, "/dev/qcqmi%u", (unsigned int)qcqmin);
    rv = stat(namep, &ss); /* see if it exists */
    if( rv >= 0 && S_ISCHR(ss.st_mode) ) /* is character device ? */
        return TRUE;

    swi_sdkusbioerrorunlocked(up); /* error recovery */

    return FALSE;
}

/*************
 *
 * Name:    swi_ossdkusbioctl
 *
 * Purpose: Issue an IOCTL command to a USB endpoint
 *
 * Params:  up       - pointer to usb structure
 *          endpoint - endpoint ID
 *          ioctlcmd - IOCTL command to issue
 *          parg     - pointer to ioctl argument
 *
 * Return:  TRUE if OK, FALSE if device no longer exists
 *
 * Abort:   Huge endpoint number
 *
 * Notes:   swi_ossdkusbinit must have been called first
 *          Only one thread may write to each endpoint.
 *
 **************/
global swi_bool
swi_ossdkusbioctl(
    struct swi_osusb         *up,
    enum qmisupportedclients client,
    struct usbep             endpoint,
    const swi_uint32         ioctlcmd,
    void                     *parg )
{
    int fd;
    int rv;
    swi_uint32 state;
    char pathp[SWI_USB_MAX_PATH];

    if( endpoint.usreadep >= SWI_OS_MAXENDPOINTS )
    {
        swi_ossdkabort("USB ioctl: illegal read endpoint number");
    }

    if( endpoint.uswriteep >= SWI_OS_MAXENDPOINTS )
    {
        swi_ossdkabort("USB ioctl: illegal write endpoint number");
    }

    if (up->osusbstate != SWI_OS_USB_STATE_OK)
    {
        swi_ossdklog("USB state not OK");
        return FALSE;
    }

    fd = up->osusbwritedescriptqmi[endpoint.uswriteep][client];

    if (fd < 0)        /* if file descriptor not open */
    {
        if ( !swi_ossdkusbendpoint_qcqmi(up,
                   endpoint.uswriteep,
                   FALSE,
                   pathp,
                   sizeof(pathp)) )    /* if no write endpoint */
        {
            return FALSE;
        }

        /* Under Linux, the O_NONBLOCK flag indicates that one wants to open but
         * does not necessarily have the intention to read or write.  This is typ‐
         * ically  used  to open devices in order to get a file descriptor for use
         * with ioctl(2).
         */
        fd = open(pathp, O_RDWR | O_NOCTTY | O_NONBLOCK);

        if (fd < 0)
        {
            swi_ossdklogerror("USB ioctl open error", errno);
#if 0
            swi_sdkusbioerrorunlocked(up);    /* error recovery */
#endif
            return FALSE;
        }

        /* Opened correctly, save the descriptor */
        if (pthread_mutex_lock(&up->osusbmutex) < 0)
            swi_ossdkaborterror("USB semaphore lock failed", errno);

        state = up->osusbstate;
        if (state == SWI_OS_USB_STATE_OK)
        {
            up->osusbwritedescriptqmi[endpoint.uswriteep][client] = fd;
            up->osusbreaddescriptqmi[endpoint.usreadep][client] = fd;
        }

        if (pthread_mutex_unlock(&up->osusbmutex) < 0)
            swi_ossdkaborterror("USB semaphore unlock failed", errno);

        if (state != SWI_OS_USB_STATE_OK)
        {
            swi_sdkfdclose(fd);
            return FALSE;
        }
    }

    /* filter ioctl commands */
    switch(ioctlcmd)
    {
        case QMI_GET_SERVICE_FILE_IOCTL:
        {
            unsigned long ioctlarg = *((unsigned long *)parg);
            rv = ioctl(fd, ioctlcmd, (void *)ioctlarg);
            break;
        }
        case QMI_GET_MEID_IOCTL:
        {
            rv = ioctl(fd, ioctlcmd, parg);
            break;
        }
        case QMI_RELEASE_SERVICE_FILE_IOCTL:
        case QMI_GET_VIDPID_IOCTL:
        {
            swi_ossdklog("IOCTL not implemented");
            return FALSE;
        }
        default:
        {
            swi_ossdklog("IOCTL not supported");
            return FALSE;
        }
    }

    if (rv < 0)    /* if error */
    {
        /* A QMI ioctl error is returned when the SDK attempts to register for
         * a QMI service which the firmware does not support.
         */
        swi_ossdklogerror("USB ioctl error", errno);
        return FALSE;
    }

    return TRUE;
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
swi_bool
swi_ossdkusbwrite(
    struct swi_osusb *up,
    enum useptype epdev,
    enum qmisupportedclients client,
    swi_uint32 endpoint,
    const void *bufp,
    swi_size_t length )
{
    int fd;
    swi_ssize_t rv;

    if( 0 > (fd = swi_ossdkusbopen(up, epdev, client, endpoint, WRMODE)) )
    {
        return FALSE;
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
 * Name: swi_ossdkreadterms
 *
 * Purpose: Set the read termination character and threshold count so
 *          that the USB read operation terminates as soon as this character
 *          is encountered or the minimum number of characters has been read
 *
 * Params:  up        - pointer to usb structure
 *          endpoint  - Which endpoint to apply these settings to
 *          readterm  - stop read when this character is received. Any
 *                      8 bit value may be used except 0xFF. Use
 *
 *                      SWI_NOREAD_TERMINATION
 *
 *                      to disable this termination setting (USB
 *                      reads do not try to match characters)
 *
 *          threshold - Minimum number of characters to read. Return
 *                      if this many or more has been read. Use
 *
 *                      SWI_NOREAD_THRESHOLD
 *
 *                      to disable this (usb reads ignore the threshold)
 *
 * Return:  None
 *
 * Abort:   Endpoint beyond maximum value
 *
 * Notes:   swi_ossdkusbinit must have been called first
 *
 *          If both termination parameters are configured then processing
 *          favours the read termination character over the threshold. If
 *          the read termination character is encountered, the read will
 *          return regardless of the count of bytes received
 *
 **************/
global void swi_ossdkreadterms( struct swi_osusb *up, swi_uint32 endpoint,
                                swi_uint8 readterm, swi_uint32 threshold )
{
    /* Ensure the endpoint is within allowable range */
    if (endpoint >= SWI_OS_MAXENDPOINTS)
            swi_ossdkabort("USB illegal endpoint number");

    /* configure the read terminator parameters */
    up->osusbreadterminator[endpoint] = readterm;
    up->osusbreadthreshold[endpoint] = threshold;
}

/*************
 *
 * Name: swi_ossdkusbread
 *
 * Purpose: Read characters from a USB enpoint
 *
 * Params: up        - pointer to usb structure
 *         edev      - endpoint type (i.e tty or qmi device endpoint)
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
global swi_bool
swi_ossdkusbread(
    struct swi_osusb *up,
    enum useptype epdev,
    enum qmisupportedclients client,
    swi_uint16  endpoint,
    void        *bufp,
    swi_size_t  *lengthp,
    swi_uint32  timeout )
{
    int fd;
    swi_ssize_t rv;

    if( 0 > (fd = swi_ossdkusbopen(up, epdev, client, endpoint, RDMODE)) )
    {
        *lengthp = 0;
        return FALSE;
    }

    swi_uint32 delayed = 0;
    char *cp = (char *)bufp;
    char *cstartp = NULL;
    char *cendp = cp + *lengthp;
    swi_int16 bytes2read = -1;

    /* Readdonep == NULL means don't exit yet */
    char *readdonep = NULL;
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
                    //swi_ossdkdelay(10);
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

            if( epdev == USEP_QMI )
            {
                if(  bytes2read == -1 )
                {
                    /* - Length of QMI message is a 5 byte offset from cstartp.
                     * - bytes2read must account for QMI service header control
                     *   flags (1 byte), transaction id (2 bytes), message id
                     *   (2 bytes), message length field (2 bytes) = 7 bytes
                     */
                    int qmisvcmsglenoffset = QMUXSDU_HDR_SZ + QMISVC_MSGLEN_OFFSET;
                    bytes2read = QMUXSDU_CTRLFLGS_SZ    +
                                 QMUXSDU_XACTIONID_SZ   +
                                 QMISVC_MSGID_SZ        +
                                 QMISVC_MSGLENSZ        +
                                 /* architecture independent QMI message
                                    length parameter extraction */
                                 (*((swi_uint8 *)(cstartp + qmisvcmsglenoffset + 1)) << 8 )
                                 + *((swi_uint8 *)(cstartp + qmisvcmsglenoffset)) ;
                }

                if( bytes2read != -1 &&
                    ( cp - (char *)bufp ) >= bytes2read )
                {
#ifndef ANDROID_LOGGING
                    syslog( LOG_USER | LOG_DEBUG,
                            "USB read: bytes2read = %d, read %d bytes",
                            bytes2read, (size_t) cp - (size_t)bufp );
                    return TRUE;
#else
                    LOGV( "USB Read: bytes2read = %d, read %d bytes",
                          bytes2read, (size_t)cp - (size_t)bufp );
                    return TRUE;
#endif
                }
            }
            else
            {
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
            }

            /* Only do the next check if the previous one found no match */
            if( !readdonep )
            {
                /* Can return if we've read a minimum number of characters */
                if( ( (size_t)cp - (size_t)bufp ) >= up->osusbreadthreshold[endpoint] )
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

/*************
 *
 * Name:    swi_ossdkusbopen
 *
 * Purpose: Open USB device file
 *
 * Params: up        - pointer to usb structure
 *         epdev     - endpoint type (i.e tty or qmi device endpoint)
 *         client    - Relevant for a QMI endpoint - QMI client to be associated
 *                     with the file descriptor.
 *         endpoint  - endpoint number
 *         mode      - open the file in the given mode
 *
 * Return: TRUE if OK, FALSE if device no longer exists
 *
 * Abort:  none
 *
 * Notes:  none
 *
 **************/
global swi_int32
swi_ossdkusbopen(
    struct swi_osusb *up,
    enum useptype epdev,
    enum qmisupportedclients client,
    swi_uint16 endpoint,
    filemode_t mode )
{
    char pathp[SWI_USB_MAX_PATH];
    swi_int32 fd = -1; /* start off with invalid file descriptor */

    if( SWI_OS_MAXENDPOINTS <= endpoint )
    {
        swi_ossdkabort("USB read from illegal endpoint number");
    }

    if( mode > FMODEMAX )
    {
        swi_ossdkabort("invalid file mode specified");
    }

    if( SWI_OS_USB_STATE_OK != up->osusbstate )
    {
/* RILSTART */
#ifndef ANDROID_LOGGING
        syslog( LOG_DEBUG,
                "%d/%s: osusbstate: %ld",
                __LINE__, __func__, up->osusbstate );
#else
        LOGD( "%d/%s: osusbstate: %ld",
              __LINE__, __func__, up->osusbstate );
#endif
/* RILSTART */

        return fd;
    }

    if( USEP_QMI != epdev )
    {
        fd = RDMODE == mode ? up->osusbreaddescript[endpoint]
                            : up->osusbwritedescript[endpoint] ;
    }
    else
    {
        fd = RDMODE == mode ? up->osusbreaddescriptqmi[endpoint][client]
                            : up->osusbwritedescriptqmi[endpoint][client] ;
    }

    if (fd < 0) /* if file descriptor not open */
    {
        if( USEP_QMI == epdev )
        {
            /* if no corresponding read or write endpoint */
            if (!swi_ossdkusbendpoint_qcqmi( up,
                                             endpoint,
                                             RDMODE == mode ? TRUE : FALSE,
                                             pathp,
                                             sizeof(pathp)) )
            {
                return fd;
            }
        }
        else
        {
            /* if no corresponding read or write endpoint */
            if (!swi_ossdkusbendpoint( up,
                                       endpoint,
                                       RDMODE == mode ? TRUE : FALSE,
                                       pathp,
                                       sizeof(pathp)) )
            {
                return fd;
            }
        }
        /* QMI files are opened for read/write once swi_ossdkusbioctl
         * is executed. Hence, if the file is not already open for writing
         * something fundemental has gone wrong such as device reset/removal
         * and we should inform the registrant of the QMI clients
         */
        if( USEP_QMI != epdev )
        {
            fd = RDMODE == mode
                 ? open(pathp, O_RDONLY | O_NOCTTY| O_NONBLOCK )
                 : open(pathp, O_WRONLY | O_NOCTTY ) ;
        }

        if (fd < 0)
        {
            const char rdstr[] = "read";
            const char wrstr[] = "write";
/* RILSTART */
#ifndef ANDROID_LOGGING
            syslog( LOG_DEBUG,
                    "%d/%s: USB %s open error: %m", __LINE__, __func__,
                    RDMODE == mode ? rdstr : wrstr );
#else
            LOGD( "%d/%s: USB %s open error: %m", __LINE__, __func__,
                  RDMODE == mode ? rdstr : wrstr );
#endif
/* RILSTART */

            swi_sdkusbioerrorunlocked(up);  /* error recovery */
            return fd;
        }

        /* Opened correctly, save the descriptor */
        if (pthread_mutex_lock(&up->osusbmutex) < 0)
        {
            swi_ossdkaborterror("USB semaphore lock failed", errno);
        }

        swi_uint32 state = up->osusbstate;
        if( SWI_OS_USB_STATE_OK == state )
        {
            if( USEP_QMI != epdev )
            {
                if( RDMODE == mode )
                {
                    up->osusbreaddescript[endpoint] = fd;
                }
                else
                {
                    up->osusbwritedescript[endpoint] = fd;
                }

            }
            else
            {
                if( RDMODE == mode )
                {
                    up->osusbreaddescriptqmi[endpoint][client] = fd;
                }
                else
                {
                    up->osusbwritedescriptqmi[endpoint][client] = fd;
                }
            }
        }

        if( pthread_mutex_unlock(&up->osusbmutex) < 0 )
        {
            swi_ossdkaborterror("USB semaphore unlock failed", errno);
        }

        if( SWI_OS_USB_STATE_OK != state )
        {
            swi_sdkfdclose(fd);
            return fd;
        }

        if( USEP_QMI != epdev && RDMODE == mode )
        {
            /* Reconfigure the serial port */
            struct termios newtio;
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

            if( tcsetattr(fd, TCSANOW, &newtio) < 0 )
            {
                swi_ossdklogerror("USB tcsetattr error", errno);
            }
        }
    }
    return fd;
}

/*************
 *
 * Name: swi_ossdkusbclose
 *
 * Purpose: Close the specified USB port
 *
 * Params: up        - pointer to usb structure
 *         endpoint  - endpoint ID
 *         readnotwrite - TRUE, endpoint is a read endpoint
 *                        FALSE, endpoint is a write endpoint
 *
 * Return: none
 *
 * Abort:  none
 *
 * Notes:  Closes the specified USB read and write ports if they
 *         were open.
 *
 **************/
void swi_ossdkusbclose(
    struct swi_osusb *up,
    enum useptype epdev,
    enum qmisupportedclients client,
   swi_uint16 endpoint,
   swi_bool readnotwrite )
{
    int fd;

    /* get the descriptor for the specified endpoint */
    if( readnotwrite )
    {
        if( epdev != USEP_QMI )
        {
            fd = up->osusbreaddescript[endpoint]; /* Read EP */
        }
        else
        {
            fd = up->osusbreaddescriptqmi[endpoint][client]; /* Read EP */
        }
    }
    else
    {
        if( epdev != USEP_QMI )
        {
           fd = up->osusbwritedescript[endpoint];  /* Write EP */
        }
        else
        {
            fd = up->osusbwritedescriptqmi[endpoint][client]; /* Read EP */
        }
    }

    /* Close it if a valid descriptor */
    if (!(fd < 0))
        swi_sdkfdclose(fd);
}

/*************
 *
 * Name: swi_allfilesclose
 *
 * Purpose: Closes all files.
 *
 * Params:  None
 *
 * Return:  No value
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/
local void swi_allfilesclose(void)
{
    struct rlimit rl;

    /* reasonable number in case getrlimit fails */
    rl.rlim_cur = 10 + QMI_SUPPORTED_CLNT_MAX;
    getrlimit(RLIMIT_NOFILE, &rl);    /* get one past last file descriptor */

    swi_uint32 fd;
    for ( fd = 0; fd < rl.rlim_cur; fd++ )    /* for each file descriptor */
    {
        swi_sdkfdclose(fd);
    }
}

/*************
 *
 * Name: swi_ossdkprocessinit
 *
 * Purpose: Initialize the SDK process, right after it starts, turning
 *          it into a daemon and setting up crash handlers.
 *
 * Params:  None
 *
 * Return:  No value
 *
 * Abort:   none
 *
 * Notes:   Must be called first in main() program
 *          Quietly exits if daemon is already running
 *
 **************/
void swi_ossdkprocessinit(void)
{

#ifndef ANDROID_LOGGING
    /* open the system log for writing */
    openlog("SWI SDK Process", 0, LOG_USER);
#endif
    /* create a new terminal session so we can proceed when
     * our parent process exits.
     */
    setsid();    /* don't care if this fails */

    /* RILSTART - remove the signal handling */
#ifndef ANDROID_NO_STACKDUMP
    swi_sdksignalinit(swi_sdksignalhandler);    /* handle signals */
#endif
    /* RILSTOP */

    swi_allfilesclose();    /* close inherited files/ttys */

    if (swi_sdkrunning())        /* if already running SDK PROCESS */
        exit(0);                /* quietly exit */
}

/*************
 *
 * Name: swi_ossdkpeeravail
 *
 * Purpose: Determine if API peer process is already running on a specific
 *          channel
 *
 * Params:  ip             - Pointer to structure of socket being verified.
 *          errcode        - pointer to return variable to hold the error from
 *                           connect() call execution.
 *
 * Return:  TRUE  - API peer side is accessible
 *          FALSE - API peer side is unavailable
 *
 * Abort:   no memory
 *
 * Notes:   The caller assures the channel validity.
 *
 **************/
swi_bool swi_ossdkpeeravail(struct swi_osipc *ip, swi_uint16 *errcodep)
{
    int peer_sd;
    swi_bool peeravail = TRUE; /* assume OK */

    peer_sd = socket(PF_LOCAL, SOCK_DGRAM, 0);    /* must be SOCK_DGRAM */

    /* Simply attempt to connect */
    if (connect(peer_sd,
                (struct sockaddr *)&ip->osipcdest,
                sizeof(struct sockaddr_un)) < 0) /* probably not running */
    {
        peeravail = FALSE;
    }

    /* Set the return variable with the error code from the last call */
    *errcodep = errno;

    /* We only wanted to know if it was there, so just discard the socket */
    close (peer_sd);

    return peeravail;
}

/*************
 *
 * Name:    swi_ossdk_stat
 *
 * Purpose: wrapper for unix stat system call
 *
 * Params:  (IN)    path   - path to stat
 *
 * Return:  return value of stat system call
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
global int swi_ossdk_stat(
    const char *path )
{
    struct stat buf;
    int rv;

    rv = stat(path, &buf);
    if(rv)
    {
        swi_ossdklog(__func__);
    }

    return rv;
}

/*************
 *
 * Name:    swi_ossdk_closedir
 *
 * Purpose: wrapper for unix opendir system call
 *
 * Params:  (IN)    path    - path
 *          (OUT)   pswidir - swi directory structure
 *
 * Return:  success: TRUE
 *          failure: FALSE
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
global swi_bool swi_ossdk_closedir(
    struct swi_osdir *pswidir )
{
    if( NULL != pswidir )
    {
        if( 0 != closedir(pswidir->pdir) )
        {
            swi_ossdklog(__func__);
            return FALSE;
        }
    }

    return TRUE;
}

/*************
 *
 * Name:    swi_ossdk_opendir
 *
 * Purpose: wrapper for unix opendir system call
 *
 * Params:  (IN)    path    - path
 *          (OUT)   pswidir - swi directory structure
 *
 * Return:  success: TRUE
 *          failure: FALSE
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
global swi_bool swi_ossdk_opendir(
    const swi_char   *path,
    struct swi_osdir *pswidir )
{
    pswidir->pdir = opendir(path);

    /* process all files at the specified path */
    if( pswidir->pdir == NULL )
    {
        swi_ossdklog(__func__);
        return FALSE;
    }

    return TRUE;
}

/*************
 *
 * Name:    swi_ossdk_readdir
 *
 * Purpose: wrapper for unix readdir system call
 *
 * Params:  (IN/OUT)    pswidir - swi directory structure
 *
 * Return:  success: TRUE
 *          failure: FALSE
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
global swi_bool swi_ossdk_readdir(
    struct swi_osdir *pswidir )
{
    pswidir->pentry = readdir( pswidir->pdir );
    if( NULL == pswidir->pentry ) /* end of stream */
    {
        swi_ossdklog(__func__);
        return FALSE;
    }
    return TRUE;
}

/*************
 *
 * Name:    swi_ossdk_mmapro
 *
 * Purpose: memory map a file in read only mode
 *
 * Params:  (IN)    path        - path at which the file is placed
 *          (IN/OUT)pfileinfo   - swi file attributes structure
 *
 * Return:  success: TRUE
 *          failure: FALSE
 *
 * Abort:   none
 *
 * Notes:   1.  if pfileinfo->filesize is zero or larger than the file size,
 *              the size of the file is mapped. Otherwise, the size provided by
 *              pfileinfo->filesize is mapped.
 *
 *          2.  failure can be due to lack of memory or if the pfileinfo->fsize
 *              + pfileinfo->vmapfileoffset exceed the file's size.
 *
 **************/
global swi_bool swi_ossdk_mmapro(
    const swi_char         *path,
    struct swi_osfilestats *pfileinfo )
{
    struct swi_osfile fp;
    swi_size_t fsize;

    /* open the file for reading */
    if( !swi_ossdkfileopenread( &fp, path ) )
    {
        char msg[256] = "failed to open file: ";
        memcpy( msg + strlen(msg), path, strlen(path) + 1);
        swi_ossdklog(msg);

        return FALSE;
    }

    if(FALSE == swi_ossdkfilesize( &fp, &(fsize) ) )
    {
        /* close the file */
        swi_ossdkfileclose( &fp);

        char msg[256] = "failed to get file size: ";
        memcpy( msg + strlen(msg), path, strlen(path) + 1);
        swi_ossdklog(msg);

        return FALSE;
    }

    /* fail if the sum of the requested size to map and the requested file
     * offset exceed the file's actual size.
     */
    if( pfileinfo->filesize + pfileinfo->vmapfileoffset > fsize  )
    {
        /* close the file */
        swi_ossdkfileclose( &fp);

        return FALSE;
    }

    if( pfileinfo->filesize == 0 || pfileinfo->filesize > fsize  )
    {
        pfileinfo->filesize = fsize;
    }

    /* calculate page aligned offset */
    ssize_t pgaligned_offset, diff;

    diff = pfileinfo->vmapfileoffset & (sysconf(_SC_PAGE_SIZE) - 1);
    pgaligned_offset = pfileinfo->vmapfileoffset & ~(sysconf(_SC_PAGE_SIZE) - 1);

    /* map file to virtual address space of calling process
     * - let kernel choose address at which to create the mapping
     * - offset based on image type and must be a multiple of sysconf(_SC_PAGE_SIZE)
     * - Protection: PROT_READ, Flags: MAP_PRIVATE | MAP_POPULATE
     */
    pfileinfo->pvunmap = mmap(  NULL,
                                pfileinfo->filesize + diff,
                                PROT_READ,
                                MAP_PRIVATE | MAP_POPULATE,
                                fp.osfiledescrip,
                                pgaligned_offset );

    if ( MAP_FAILED == pfileinfo->pvunmap )
    {
        /* close the file */
        swi_ossdkfileclose( &fp);

        swi_ossdklog("swi_ossdk_mmapro: File mapping failed");
        return FALSE;
    }

    /* Adjust client's pointer to the mapped file as per the requested offset */
    pfileinfo->pvmap = pfileinfo->pvunmap + diff;

    /* close the file */
    swi_ossdkfileclose( &fp);

    return TRUE;
}

/*************
 *
 * Name:    swi_ossdk_umapfile
 *
 * Purpose: wraper for munmap
 *
 * Params:  (IN)   pfileinfo - swi file attributes structure
 *
 * Return:  success: TRUE
 *          failure: FALSE
 *
 * Abort:   none
 *
 * Notes:
 *
 **************/
global swi_bool swi_ossdk_umapfile(
    struct swi_osfilestats *pfileinfo )
{
    if( 0 != munmap( pfileinfo->pvunmap, pfileinfo->filesize ) )
    {
        swi_ossdklog(__func__);
        return FALSE;
    }

    return TRUE;
}
