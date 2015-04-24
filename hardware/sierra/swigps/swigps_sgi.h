/**
 *
 * @ingroup swiril TODO
 *
 * @file 
 *    GPS definitions unique to the Interface handler
 *
 * @author
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 * @note
 *    See also swigps_common.h for a description of the naming convention
 *    in use in this package
 *
 */
#include <semaphore.h>
#include <gps.h>
#include <stdbool.h>
#include "swigps_common.h"

/* Definitions */

/* Maximum size for NMEA serial port name */
#define MAXPORT SWIGPS_MSGSIZE_RESP_MAX_GETNMEAPORT

/* Nmea Parser stuff */
#define  NMEA_MAX_SIZE  128

/* Little endian and Big Endian conversion requests */
#define SWIGPS_LE 0
#define SWIGPS_BE 1

/**
 * GPS Main State Thread state definitions 
 *
 */
enum {
  STATE_QUIT  = 0,
  STATE_INIT  = 1,
  STATE_RUNNING = 2
};

/** 
 * 
 * NMEA Port Name storage structure
 *
 * Structure definition for storage of the GPS NMEA port string
 *
 */
typedef struct {
    int initialized;
    char name[MAXPORT+1];
} sgiNmeaPort;

/** 
 * 
 * Time Injection 
 *
 * Structure definition for use in injecting the absolute time into
 * the modem at the request of the GPS Location Service. 
 *
 */
typedef struct {
    GpsUtcTime time;         /* The time to be injected into the modem */
    int uncertainty;         /* Indication of the quality of the time */
} sgiInjTime;

/** 
 * 
 * AGPS settings received from the modem
 *
 * Structure definition for storage of AGPS-specific settings reported 
 * by the modem Requested values are stored in the requested value 
 * section in the structure below. The actual value is updated in the 
 * main part of the structure when the modem sends its response
 *
 */
typedef struct {
    AGpsType type;           /* We only support SUPL */
	unsigned int ipaddr;     /* Current IP address setting */
    int port;                /* Current port number setting */
    char url[MAXURL+1];      /* One added for NULL terminator */
    /* Requested values */
    AGpsType reqtype;        /* Requested type */
    unsigned int reqipaddr;  /* Requested ip address */
    int reqport;             /* Requested port number */
    char requrl[MAXURL+1];   /* One added for NULL terminator */
} sgiAGPS;

/** 
 * 
 * PDS Default settings structure
 *
 * Structure definition for storage of the default settings received
 * in response to a GetPdsDefs request. Requested values are stored
 * in the requested value section in the structure below. The actual
 * value is updated in the main part of the structure.
 *
 */
typedef struct {
    int initialized;
    unsigned int recurrence;
    unsigned int interval;
    unsigned int timeout;
    unsigned int accuracy;
    /* Requested values */
    GpsPositionMode reqmode;
} sgiPdsDefs;

/**
 * 
 * Vendor-specific NMEA Reader structure definition
 *
 * This structure contains control information and parsed field
 * values for NMEA sentence parsing.
 *
 */
typedef struct {
    int     pos;
    int     overflow;
    int     utc_year;
    int     utc_mon;
    int     utc_day;
    int     utc_diff;
    GpsLocation  fix;
    GpsSvStatus  sv_status;
    int     sv_status_changed;
    char    in[ NMEA_MAX_SIZE+1 ];
} NmeaReader;

/**
 * 
 * Main gps state thread state structure
 *
 * This structure is used by the main gps state thread and its 
 * associates to store various settings related to the state of
 * the GPS engine. 
 *
 */
typedef struct {
    int                     init;
    int                     fd;
    bool                    tracking;
    GpsCallbacks            callbacks;
    pthread_t               swigps_thread;
    pthread_t               swigps_tmr_thread;
    int                     rilgpsfd;
    int                     control[2];
    int                     fix_freq;
    sem_t                   fix_sem;
    int                     first_fix;
    sgiNmeaPort             gpsnmeaport;
    sgiAGPS                 agpsInfo;
    sgiPdsDefs              pdsDefs;
    sgiInjTime              injectTime;
    NmeaReader              reader;
} GpsState;


/* External Definitions */

/* swigps_outb.c */
extern void handle_rilgps( GpsState *statep, int fd );
extern int swigps_getnmeaport( GPS_RILTYPE riltype, int rilgpsfd );
extern int swigps_getdef( int rilgpsfd );
extern int swigps_setdef( int rilgpsfd,
                   int operation,
                   int timeout,
                   int interval,
                   int accuracy,
                   bool tracking );
extern int swigps_setagps( int rilgpsfd,
                    unsigned int ipaddr,
                    int port, 
                    char *hostnamep );
extern int swigps_getagps( int rilgpsfd );
extern int swigps_injtime( int rilgpsfd,
                    GpsUtcTime time,
                    int uncertainty );
extern int swigps_StartSession( int rilgpsfd,
                                int operation,
                                int timeout,
                                int interval,
                                int accuracy );
extern int swigps_StopSession( int rilgpsfd );

/* Swigps.c */
extern GpsState *swigps_getstatep(void);
extern void gps_state_setmode( GpsState*  s );

extern int swigps_init(GpsCallbacks* callbacks);
extern int swigps_start(void);
extern int swigps_stop(void);
extern void swigps_cleanup(void);
extern int swigps_inject_time(GpsUtcTime time, 
                              int64_t timeReference, 
                              int uncertainty );
extern int swigps_inject_location(double latitude, 
                                  double longitute, 
                                  float accuracy );
extern void swigps_delete_aiding_data(GpsAidingData flags);
extern void swigps_init_state( GpsState *pstate );
extern void swigps_init_state_thread( GpsState *pstate );
extern void swiagps_init( AGpsCallbacks* callbacks );
extern int swiagps_open( const char *apn );
extern int swiagps_closed( void );
extern int swiagps_failed( void );
extern int swiagps_set_server( AGpsType type, const char* hostname, int port );
extern const void* swigps_get_extension(const char* name);
extern void *gps_timer_thread( void*  arg );
extern void *gps_state_thread( void*  arg );

/* swigps_<android version>.c */
extern const GpsInterface swiGpsInterface;
extern const AGpsInterface swiAgpsInterface;
extern void swigps_report_state( GpsCallbacks *cbklistp, GpsStatusValue state );
extern void swigps_init_pdsDefs( GpsState *pstate );
extern int swigps_checkGpsCallbacks( GpsCallbacks *Callbackp );
extern int swigps_create_timer_thread( GpsState *statep );
extern int swigps_create_state_thread( GpsState *statep );

/* swigps_<at,qmi>.c */
extern void init_nmea( void );
extern void session_started( void );
extern void start_session( void );
extern void session_stopped( void );
extern void stop_session( void );
extern void time_injected( void );
extern void inject_time( void );
extern void defaults_received( void );
extern void get_defaults( void );
extern void defaults_set( void );
extern void set_defaults( void );
extern void agps_received( void );
extern void get_agps( void );
extern void agps_set( void );
extern void set_agps( void );
