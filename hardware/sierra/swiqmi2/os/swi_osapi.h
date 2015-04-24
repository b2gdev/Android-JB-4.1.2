/*************
 *
 * Filename: swi_osapi.h
 *
 * Purpose:  OS wrapper package API-side (application process) definitions
 *
 * Copyright: © 2011 Sierra Wireless Inc., all rights reserved
 *
 **************/
#ifndef swi_osapi_h
#define swi_osapi_h

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
/* include files */
#include "aa/aaglobal.h"
#include <limits.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <dirent.h>
#include <signal.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>
#include <sys/select.h>
#include <errno.h>
#include <stdio.h>
#include <sys/resource.h>
#include <sys/prctl.h>
#include <pthread.h>

/*--------------
  Defines
 --------------*/

/*--------------
  Structures
 --------------*/
/* mutex */
struct swi_osmutex{
    pthread_mutex_t     osmutex;
    pthread_mutexattr_t osmutex_attr;
};

struct swi_osipcapi{
    int osipcsocket;
    struct sockaddr_un osipcdest;
};

/* directory structure - must be indentical to swi_osdir in swi_ossdk.h */
struct swi_osdirapi{
    DIR           *pdir;
    struct dirent *pentry;
};

/* RILSTART */
/* process information - only PID required now */
struct swi_ossdkprocessInfo{
    pid_t pid;
};
/* RILSTOP */

/*--------------
  Prototypes
 --------------*/
void swi_osapierrorhandlerset( void (*handlerp)(const char *, int) );

swi_bool swi_osapisdkprocesscreate(const char *pathp);

void swi_osapiipcsetdest( struct swi_osipcapi *ip,
                          swi_size_t ipcindex,
                          swi_uint8 * ipcnamep );

swi_bool swi_osapiipcopen( struct swi_osipcapi *ip,
                           size_t ipcindex,
                           swi_uint8* ipcnamep );

swi_bool swi_osapiipcclose(struct swi_osipcapi *picp);

void swi_osapilog(const char *errorp);

swi_bool swi_osapiipcwrite( struct swi_osipcapi *ip,
                            const void *bufp,
                            size_t length );

swi_bool swi_osapiipcread( struct swi_osipcapi *ip,
                           void *bufp,
                           size_t *lengthp,
                           swi_uint32 timeout );

void swi_osapithreadcreate( void (*fnp)(void) );

int swi_osapi_stat(const char *path);

swi_bool swi_osapi_opendir( const swi_char      *path,
                            struct swi_osdirapi *pswidir );

swi_bool swi_osapi_readdir( struct swi_osdirapi *pswidir );

swi_bool swi_osapi_closedir(struct swi_osdirapi *pswidir );

swi_int32 swi_osapi_system(const char *str );

void swi_osapi_mutexinit( struct swi_osmutex *mp );

void swi_osapi_mutexlock( struct swi_osmutex *mp );

void swi_osapi_mutexunlock( struct swi_osmutex *mp );

void swi_osapi_mutexdestroy( pthread_mutex_t *mp );

/* RILSTART */
swi_bool swi_osapiverifySDKstatus( void );

swi_bool swi_osapigetprocessID( pid_t *pid );
/* RILSTOP */

#endif /* swi_osapi_h */
