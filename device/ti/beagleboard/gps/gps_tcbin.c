/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
* Author: RuvindaD
* Date: 11/07/2012
* Zone24x7
*/

/* this implements a GPS hardware library for Zone24x7 TCBIN.
 * the following code should be built as a shared library that will be
 * placed into /system/lib/hw/gps.beagleboard.so
 *
 * it will be loaded by the code in hardware/libhardware/hardware.c
 * which is itself called from android_location_GpsLocationProvider.cpp
 */


#include <errno.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <math.h>
#include <time.h>
#define  LOG_TAG  "gps_tcbin"
#include <cutils/log.h>
#include <cutils/sockets.h>
#include <hardware/gps.h>
#include <stdio.h>
#include <termios.h>
#include <semaphore.h>

#include "w2sg0006_ioctl.h"

#ifndef FNDELAY
#define FNDELAY O_NONBLOCK 
#endif

#define  TCBIN_GPS_DATA_NODE "/dev/ttyO0"
#define  TCBIN_GPS_PWR_NODE "/dev/gps"

#define  GPS_DEBUG  0
#define ZONE_DB 1

#if GPS_DEBUG
#  define  D(...)   ALOGD(__VA_ARGS__)
#else
#  define  D(...)   ((void)0)
#endif

#if ZONE_DB
#  define  ZD(...)   ALOGD(__VA_ARGS__)
#else
#  define  ZD(...)   ((void)0)
#endif

#if ZONE_DB
#  define  ZI(...)   ALOGI(__VA_ARGS__)
#else
#  define  ZI(...)   ((void)0)
#endif

#if ZONE_DB
#  define  ZE(...)   ALOGE(__VA_ARGS__)
#else
#  define  ZE(...)   ((void)0)
#endif

#define GPS_STATUS_CB(_cb, _s)    \
  if ((_cb).status_cb) {          \
    GpsStatus gps_status;         \
    gps_status.status = (_s);     \
    (_cb).status_cb(&gps_status); \
  }

#define GPS_STATE_LOCK_FIX(_s)         \
{                                      \
  int ret;                             \
  do {                                 \
    ret = sem_wait(&(_s)->fix_sem);    \
  } while (ret < 0 && errno == EINTR);   \
}

#define GPS_STATE_UNLOCK_FIX(_s)       \
  sem_post(&(_s)->fix_sem)

#define  NMEA_MAX_SIZE  83
#define  MAX_NMEA_TOKENS  32

 enum {
  STATE_QUIT  = 0,
  STATE_INIT  = 1,
  STATE_START = 2
};

/* commands sent to the gps thread */
enum {
    CMD_QUIT  = 0,
    CMD_START = 1,
    CMD_STOP  = 2
};

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
    gps_location_callback  callback;
    char    in[ NMEA_MAX_SIZE+1 ];
} NmeaReader;

typedef struct {
    int                     init;
    int                     fd;
    int						pwrfd;
    GpsCallbacks            callbacks;
    pthread_t               thread;
    pthread_t               tmr_thread;
    int                     control[2];
    int                     fix_freq;
    sem_t                   fix_sem;
    int                     first_fix;
    NmeaReader *            reader;
    int 		    timer_thread_running;
} GpsState;


static void gps_timer_thread( void*  arg );

static GpsState  _gps_state[1];
static GpsState *gps_state = _gps_state;

/*****************************************************************/
/*****************************************************************/
/*****                                                       *****/
/*****         TCBIN  DEVICE Specific functions              *****/
/*****                                                       *****/
/*****************************************************************/
/*****************************************************************/

static void gps_enable()
{
    ZI("gps_enale\n");
	ioctl(gps_state->pwrfd,W2SG0006_ENABLE);
    return;
}

static void gps_disable()
{
    ZI("gps_disable\n");
	ioctl(gps_state->pwrfd,W2SG0006_DISABLE);
    return;
}

static void gps_pwrOn()
{
    ZI("gps_pwrOn\n");
	ioctl(gps_state->pwrfd,W2SG0006_PWR_ON);
    return;
}

static void gps_pwrOff()
{
    ZI("gps_pwrOff\n");
	ioctl(gps_state->pwrfd,W2SG0006_PWR_OFF);
    return;
}

static int gps_is_enabled()
{
    ZI("gps_is_enabled\n");  
	int ret = -1;
    
    return ret;
}

/*****************************************************************/
/*****************************************************************/
/*****                                                       *****/
/*****       N M E A   T O K E N I Z E R                     *****/
/*****                                                       *****/
/*****************************************************************/
/*****************************************************************/

typedef struct {
    const char*  p;
    const char*  end;
} Token;

typedef struct {
    int     count;
    Token   tokens[ MAX_NMEA_TOKENS ];
} NmeaTokenizer;

static int
nmea_tokenizer_init( NmeaTokenizer*  t, const char*  p, const char*  end )
{
    int    count = 0;
    char*  q;

    // the initial '$' is optional
    if (p < end && p[0] == '$')
        p += 1;

    // remove trailing newline
    if (end > p && end[-1] == '\n') {
        end -= 1;
        if (end > p && end[-1] == '\r')
            end -= 1;
    }

    // get rid of checksum at the end of the sentecne
    if (end >= p+3 && end[-3] == '*') {
        end -= 3;
    }

    while (p < end) {
        const char*  q = p;

        q = memchr(p, ',', end-p);
        if (q == NULL)
            q = end;

        if (q >= p) {
            if (count < MAX_NMEA_TOKENS) {
                t->tokens[count].p   = p;
                t->tokens[count].end = q;
                count += 1;
            }
        }
        if (q < end)
            q += 1;

        p = q;
    }

    t->count = count;
    return count;
}

static Token
nmea_tokenizer_get( NmeaTokenizer*  t, int  index )
{
    Token  tok;
    static const char*  dummy = "";

    if (index < 0 || index >= t->count) {
        tok.p = tok.end = dummy;
    } else
        tok = t->tokens[index];

    return tok;
}


static int
str2int( const char*  p, const char*  end )
{
    int   result = 0;
    int   len    = end - p;
    if(len<1)
      goto Fail;

    for ( ; len > 0; len--, p++ )
    {
        int  c;

        if (p >= end)
            goto Fail;

        c = *p - '0';
        if ((unsigned)c >= 10)
            goto Fail;

        result = result*10 + c;
    }
    return  result;

Fail:
    return -1;
}

static double
str2float( const char*  p, const char*  end )
{
    int   result = 0;
    int   len    = end - p;
    char  temp[16];

    if (len >= (int)sizeof(temp))
        return 0.;

    memcpy( temp, p, len );
    temp[len] = 0;
    return strtod( temp, NULL );
}

/*****************************************************************/
/*****************************************************************/
/*****                                                       *****/
/*****       N M E A   P A R S E R                           *****/
/*****                                                       *****/
/*****************************************************************/
/*****************************************************************/

static time_t
mkgmtime(struct tm *t)
{
    short month, year;
    time_t result;
    static int m_to_d[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    month = t->tm_mon;	
    year = t->tm_year + month / 12 + 1900;
    month %= 12;
    if (month < 0) {
	    year -= 1;
	    month += 12;
    }
    result = (year - 1970) * 365 + m_to_d[month];	
    if (month <= 1)
	    year -= 1;
    result += (year - 1968) / 4;
    result -= (year - 1900) / 100;
    result += (year - 1600) / 400;
    result += t->tm_mday;
    result -= 1;
    result *= 24;
    result += t->tm_hour;
    result *= 60;
    result += t->tm_min;
    result *= 60;
    result += t->tm_sec;
    return (result);
}

static void
nmea_reader_update_utc_diff( NmeaReader*  r )
{
    time_t         now = time(NULL);
    struct tm      tm_local;
    struct tm      tm_utc;
    long           time_local, time_utc;

    gmtime_r( &now, &tm_utc );
    localtime_r( &now, &tm_local );

    time_local = mktime(&tm_local);
    time_utc = mktime(&tm_utc);
    
    r->utc_diff = time_utc - time_local;
}


static void
nmea_reader_init( NmeaReader*  r )
{
    memset( r, 0, sizeof(*r) );

    r->pos      = 0;
    r->overflow = 0;
    r->utc_year = -1;
    r->utc_mon  = -1;
    r->utc_day  = -1;
    r->callback = NULL;
    r->fix.size = sizeof(r->fix);

    nmea_reader_update_utc_diff( r );
}

static int
nmea_reader_update_time( NmeaReader*  r, Token  tok )
{
    int        hour, minute, seconds, milliseconds;
    struct tm  tm;
    time_t     fix_time;

    if (tok.p + 6 > tok.end){
	ZE("nmea_reader_update_time ERROR\n");
        return -1;
    }

    if (r->utc_year < 0) {
        // no date, can't return valid timestamp (never ever make up a date, this could wreak havoc)
	ZE("nmea_reader_update_time ERROR\n");
        return -1;
    }
    else
    {
        tm.tm_year = r->utc_year - 1900;
        tm.tm_mon  = r->utc_mon - 1;
        tm.tm_mday = r->utc_day;
    }

    hour    = str2int(tok.p,   tok.p+2);
    minute  = str2int(tok.p+2, tok.p+4);
    seconds = str2int(tok.p+4, tok.p+6);

    // parse also milliseconds (if present) for better precision
    milliseconds = 0;
    if (tok.end - (tok.p+7) == 2) {
        milliseconds = str2int(tok.p+7, tok.end) * 10;
    }
    else if (tok.end - (tok.p+7) == 1) {
        milliseconds = str2int(tok.p+7, tok.end) * 100;
    }
    else if (tok.end - (tok.p+7) >= 3) {
        milliseconds = str2int(tok.p+7, tok.p+10);
    }

    // the following is only guaranteed to work if we have previously set a correct date, so be sure
    // to always do that before
    tm.tm_hour = hour;
    tm.tm_min  = minute;
    tm.tm_sec  = seconds;

    fix_time = mkgmtime( &tm );
    r->fix.timestamp = (long long)fix_time * 1000 + milliseconds;    

    return 0;
}

static int
nmea_reader_update_cdate( NmeaReader*  r, Token  tok_d, Token tok_m, Token tok_y )
{
    if ( (tok_d.p + 2 > tok_d.end) ||
         (tok_m.p + 2 > tok_m.end) ||
         (tok_y.p + 4 > tok_y.end) )
        return -1;
    
    r->utc_day = str2int(tok_d.p,   tok_d.p+2);
    r->utc_mon = str2int(tok_m.p, tok_m.p+2);
    r->utc_year = str2int(tok_y.p, tok_y.p+4);

    return 0;
}

static int
nmea_reader_update_date( NmeaReader*  r, Token  date, Token  time )
{
    Token  tok = date;
    int    day, mon, year;

    if (tok.p + 6 != tok.end) {
        D("nmea_reader_update_date Error\n");
	ZE("nmea_reader_update_date Error\n");
        return -1;
    }

    day  = str2int(tok.p, tok.p+2);
    mon  = str2int(tok.p+2, tok.p+4);
    year = str2int(tok.p+4, tok.p+6) + 2000;

    if ((day|mon|year) < 0) {
        D("date not properly formatted: '%.*s'", tok.end-tok.p, tok.p);
	ZE("date not properly formatted: '%.*s'\n", tok.end-tok.p, tok.p);
        return -1;
    }

    r->utc_year  = year;
    r->utc_mon   = mon;
    r->utc_day   = day;

    return nmea_reader_update_time( r, time );
}


static double
convert_from_hhmm( Token  tok )
{
    double  val     = str2float(tok.p, tok.end);
    int     degrees = (int)(floor(val) / 100);
    double  minutes = val - degrees*100.;
    double  dcoord  = degrees + minutes / 60.0;
    return dcoord;
}


static int
nmea_reader_update_latlong( NmeaReader*  r,
                            Token        latitude,
                            char         latitudeHemi,
                            Token        longitude,
                            char         longitudeHemi )
{
    double   lat, lon;
    Token    tok;

    tok = latitude;
    if (tok.p + 6 > tok.end) {
        D("latitude is too short: '%.*s'", tok.end-tok.p, tok.p);
	ZE("latitude is too short: '%.*s'", tok.end-tok.p, tok.p);
        return -1;
    }
    lat = convert_from_hhmm(tok);
    if (latitudeHemi == 'S')
        lat = -lat;

    tok = longitude;
    if (tok.p + 6 > tok.end) {
        D("longitude is too short: '%.*s'", tok.end-tok.p, tok.p);
	ZE("longitude is too short: '%.*s'", tok.end-tok.p, tok.p);
        return -1;
    }
    lon = convert_from_hhmm(tok);
    if (longitudeHemi == 'W')
        lon = -lon;

    r->fix.flags    |= GPS_LOCATION_HAS_LAT_LONG;
    r->fix.latitude  = lat;
    r->fix.longitude = lon;
    return 0;
}


static int
nmea_reader_update_altitude( NmeaReader*  r,
                             Token        altitude,
                             Token        units )
{
    double  alt;
    Token   tok = altitude;

    if (tok.p >= tok.end){
	ZE("nmea_reader_update_altitude ERROR\n");
        return -1;
    }

    r->fix.flags   |= GPS_LOCATION_HAS_ALTITUDE;
    r->fix.altitude = str2float(tok.p, tok.end);
    return 0;
}

static int
nmea_reader_update_accuracy( NmeaReader*  r,
                             Token        accuracy )
{
    double  acc;
    Token   tok = accuracy;

    if (tok.p >= tok.end){
      ZE("nmea_reader_update_accuracy ERROR\n");
        return -1;
    }

    r->fix.accuracy = str2float(tok.p, tok.end);

    if (r->fix.accuracy == 99.99){
      return 0;
    }

    r->fix.flags   |= GPS_LOCATION_HAS_ACCURACY;
    return 0;
}

static int
nmea_reader_update_bearing( NmeaReader*  r,
                            Token        bearing )
{
    double  alt;
    Token   tok = bearing;

    if (tok.p >= tok.end){
      ZE("nmea_reader_update_bearing ERROR\n");
        return -1;
    }

    r->fix.flags   |= GPS_LOCATION_HAS_BEARING;
    r->fix.bearing  = str2float(tok.p, tok.end);
    return 0;
}


static int
nmea_reader_update_speed( NmeaReader*  r,
                          Token        speed )
{
    double  alt;
    Token   tok = speed;

    if (tok.p >= tok.end){
      ZE("nmea_reader_update_speed ERROR\n");
        return -1;
    }

    r->fix.flags   |= GPS_LOCATION_HAS_SPEED;
    // convert knots into m/sec (1 knot equals 1.852 km/h, 1 km/h equals 3.6 m/s)
    // since 1.852 / 3.6 is an odd value (periodic), we're calculating the quotient on the fly
    // to obtain maximum precision (we don't want 1.9999 instead of 2)
    r->fix.speed    = str2float(tok.p, tok.end) * 1.852 / 3.6;
    return 0;
}


static void
nmea_reader_parse( NmeaReader*  r )
{
   /* we received a complete sentence, now parse it to generate
    * a new GPS fix...
    */
    NmeaTokenizer  tzer[1];
    Token          tok;
    int tmp_j;

    D("Received: '%.*s'", r->pos, r->in);
    if (r->pos < 9) {
        D("Too short. discarded.");
        return;
    }

    nmea_tokenizer_init(tzer, r->in, r->in + r->pos);
#if GPS_DEBUG
    {
        int  n;
        D("Found %d tokens", tzer->count);
        for (n = 0; n < tzer->count; n++) {
            Token  tok = nmea_tokenizer_get(tzer,n);
            D("%2d: '%.*s'", n, tok.end-tok.p, tok.p);
        }
    }
#endif

    tok = nmea_tokenizer_get(tzer, 0);
    if (tok.p + 5 > tok.end) {
        D("sentence id '%.*s' too short, ignored.", tok.end-tok.p, tok.p);
        return;
    }

    // ignore first two characters.
    tok.p += 2;
    if ( !memcmp(tok.p, "GGA", 3) ) {
	ZI("GGA\n");
        Token  tok_time          = nmea_tokenizer_get(tzer,1);
        Token  tok_latitude      = nmea_tokenizer_get(tzer,2);
        Token  tok_latitudeHemi  = nmea_tokenizer_get(tzer,3);
        Token  tok_longitude     = nmea_tokenizer_get(tzer,4);
        Token  tok_longitudeHemi = nmea_tokenizer_get(tzer,5);
        Token  tok_altitude      = nmea_tokenizer_get(tzer,9);
        Token  tok_altitudeUnits = nmea_tokenizer_get(tzer,10);

	// don't use this as we have no fractional seconds and no date; there are better ways to
        // get a good timestamp from GPS
        // nmea_reader_update_time(r, tok_time);
        nmea_reader_update_latlong(r, tok_latitude,
                                      tok_latitudeHemi.p[0],
                                      tok_longitude,
                                      tok_longitudeHemi.p[0]);
        nmea_reader_update_altitude(r, tok_altitude, tok_altitudeUnits);

    } 
    /*else if ( !memcmp(tok.p, "GLL", 3) ) {
	ZI("GLL\n");
	Token  tok_fixstaus      = nmea_tokenizer_get(tzer,6);
        if ((tok_fixstaus.p[0] == 'A') && (r->utc_year >= 0)) {
          // ignore this until we have a valid timestamp

	  Token  tok_latitude      = nmea_tokenizer_get(tzer,1);
          Token  tok_latitudeHemi  = nmea_tokenizer_get(tzer,2);
          Token  tok_longitude     = nmea_tokenizer_get(tzer,3);
          Token  tok_longitudeHemi = nmea_tokenizer_get(tzer,4);
          Token  tok_time          = nmea_tokenizer_get(tzer,5);

	  // don't use this as we have no fractional seconds and no date; there are better ways to
          // get a good timestamp from GPS
          //nmea_reader_update_time(r, tok_time);
          nmea_reader_update_latlong(r, tok_latitude,
                                        tok_latitudeHemi.p[0],
                                        tok_longitude,
                                        tok_longitudeHemi.p[0]);
        }
    }*/
    else if ( !memcmp(tok.p, "GSA", 3) ) {
	
	ZI("GSA\n");
        Token  tok_fixStatus   = nmea_tokenizer_get(tzer, 2);
        int i;
        
	if (tok_fixStatus.p[0] != '\0' && tok_fixStatus.p[0] != '1') {

//	  ZD("tzer->count = %d\n",tzer->count);
//	  ZD("Received: '%.*s'\n", r->pos, r->in);
//	  for(tmp_j = 0; tmp_j < tzer->count; tmp_j++){
//	    ZD("TOK: '%.*s'", tzer->tokens[tmp_j].end, tzer->tokens[tmp_j].p);
//	  }
	  
	  Token  tok_accuracy      = nmea_tokenizer_get(tzer, 15);

	  nmea_reader_update_accuracy(r, tok_accuracy);

	  r->sv_status.used_in_fix_mask = 0ul;

	  for (i = 3; i <= 14; ++i){

	    Token  tok_prn  = nmea_tokenizer_get(tzer, i);
            int prn = str2int(tok_prn.p, tok_prn.end);

	    if (prn > 0){
              r->sv_status.used_in_fix_mask |= (1ul << (32 - prn));
              r->sv_status_changed = 1;
              D("%s: fix mask is %d", __FUNCTION__, r->sv_status.used_in_fix_mask);
	      //ZI("%s: fix mask is %d", __FUNCTION__, r->sv_status.used_in_fix_mask);
            }

	  }

	}
    } else if ( !memcmp(tok.p, "GSV", 3) ) {
  
	Token  tok_noSatellites  = nmea_tokenizer_get(tzer, 3);
        int    noSatellites = str2int(tok_noSatellites.p, tok_noSatellites.end);
	ZI("GSV message with total satellites %d\n", noSatellites);   
	
	if (noSatellites > 0) {

	  Token  tok_noSentences   = nmea_tokenizer_get(tzer, 1);
          Token  tok_sentence      = nmea_tokenizer_get(tzer, 2);

	  int sentence = str2int(tok_sentence.p, tok_sentence.end);
          int totalSentences = str2int(tok_noSentences.p, tok_noSentences.end);
          int curr;
          int i;

	  if (sentence == 1) {
              r->sv_status_changed = 0;
              r->sv_status.num_svs = 0;
          }

	  curr = r->sv_status.num_svs;
          i = 0;

	  while (i < 4 && r->sv_status.num_svs < noSatellites){

	    Token  tok_prn = nmea_tokenizer_get(tzer, i * 4 + 4);
                 Token  tok_elevation = nmea_tokenizer_get(tzer, i * 4 + 5);
                 Token  tok_azimuth = nmea_tokenizer_get(tzer, i * 4 + 6);
                 Token  tok_snr = nmea_tokenizer_get(tzer, i * 4 + 7);
                 r->sv_status.sv_list[curr].prn = str2int(tok_prn.p, tok_prn.end);
                 r->sv_status.sv_list[curr].elevation = str2float(tok_elevation.p, tok_elevation.end);
                 r->sv_status.sv_list[curr].azimuth = str2float(tok_azimuth.p, tok_azimuth.end);
                 r->sv_status.sv_list[curr].snr = str2float(tok_snr.p, tok_snr.end);

		 r->sv_status.num_svs += 1;

		 curr += 1;

		 i += 1;
          }

	  if (sentence == totalSentences) {
              r->sv_status_changed = 1;
          }

	  D("%s: GSV message with total satellites %d", __FUNCTION__, noSatellites);

	}

    }else if ( !memcmp(tok.p, "RMC", 3) ) {
	
	//ZI("RMC\n");
	Token  tok_fixStatus     = nmea_tokenizer_get(tzer,2);
	ZI("RMC, fixStatus=%c\n", tok_fixStatus.p[0]);

	if (tok_fixStatus.p[0] == 'A')
        {
          Token  tok_time          = nmea_tokenizer_get(tzer,1);
          Token  tok_latitude      = nmea_tokenizer_get(tzer,3);
          Token  tok_latitudeHemi  = nmea_tokenizer_get(tzer,4);
          Token  tok_longitude     = nmea_tokenizer_get(tzer,5);
          Token  tok_longitudeHemi = nmea_tokenizer_get(tzer,6);
          Token  tok_speed         = nmea_tokenizer_get(tzer,7);
          Token  tok_bearing       = nmea_tokenizer_get(tzer,8);
          Token  tok_date          = nmea_tokenizer_get(tzer,9);

	  nmea_reader_update_date( r, tok_date, tok_time );

	  nmea_reader_update_latlong( r, tok_latitude,
                                           tok_latitudeHemi.p[0],
                                           tok_longitude,
                                           tok_longitudeHemi.p[0] );
            nmea_reader_update_bearing( r, tok_bearing );
            nmea_reader_update_speed  ( r, tok_speed );
        }
    } 
    /*else if ( !memcmp(tok.p, "VTG", 3) ) {
      ZI("VTG\n");
        Token  tok_fixStatus     = nmea_tokenizer_get(tzer,9);

	if (tok_fixStatus.p[0] != '\0' && tok_fixStatus.p[0] != 'N')
        {
            Token  tok_bearing       = nmea_tokenizer_get(tzer,1);
            Token  tok_speed         = nmea_tokenizer_get(tzer,5);

	    nmea_reader_update_bearing( r, tok_bearing );
            nmea_reader_update_speed  ( r, tok_speed );
        }
        
    }*/ 
    /*else if ( !memcmp(tok.p, "ZDA", 3) ) {
	ZI("ZDA\n");
	Token  tok_time;
        Token  tok_year  = nmea_tokenizer_get(tzer,4);
        tok_time  = nmea_tokenizer_get(tzer,1);
        if ((tok_year.p[0] != '\0') && (tok_time.p[0] != '\0')) {

	  // make sure to always set date and time together, lest bad things happen
          Token  tok_day   = nmea_tokenizer_get(tzer,2);
          Token  tok_mon   = nmea_tokenizer_get(tzer,3);

	  nmea_reader_update_cdate( r, tok_day, tok_mon, tok_year );
          nmea_reader_update_time(r, tok_time);
        }
    }*/ 
    else {
        tok.p -= 2;
        D("unknown sentence '%.*s", tok.end-tok.p, tok.p);
	ZE("ERROR unknown sentence '%.*s\n", tok.end-tok.p, tok.p);
    }

#if ZONE_DB
    if (r->fix.flags != 0) {
        char   temp[256];
	memset(temp,0,256);
        char*  p   = temp;
        char*  end = p + sizeof(temp);
        struct tm   utc;

        p += snprintf( p, end-p, "sending fix" );
        if (r->fix.flags & GPS_LOCATION_HAS_LAT_LONG) {
            p += snprintf(p, end-p, " lat=%g lon=%g", r->fix.latitude, r->fix.longitude);
        }
        if (r->fix.flags & GPS_LOCATION_HAS_ALTITUDE) {
            p += snprintf(p, end-p, " altitude=%g", r->fix.altitude);
        }
        if (r->fix.flags & GPS_LOCATION_HAS_SPEED) {
            p += snprintf(p, end-p, " speed=%g", r->fix.speed);
        }
        if (r->fix.flags & GPS_LOCATION_HAS_BEARING) {
            p += snprintf(p, end-p, " bearing=%g", r->fix.bearing);
        }
        if (r->fix.flags & GPS_LOCATION_HAS_ACCURACY) {
            p += snprintf(p,end-p, " accuracy=%g", r->fix.accuracy);
        }
        time_t tempT = (r->fix.timestamp/1000);
	gmtime_r(&tempT, &utc );
        p += snprintf(p, end-p, " time=%s", asctime( &utc ) );

	ZI("%s\n",temp);
    }
#endif

  if (!gps_state->first_fix &&
        gps_state->init == STATE_INIT &&
        r->fix.flags & GPS_LOCATION_HAS_LAT_LONG) {

	ZI("FIRST FIX\n");
	if (gps_state->callbacks.location_cb) {
            gps_state->callbacks.location_cb( &r->fix );
            r->fix.flags = 0;
        }else{
	  ZI("no gps_state->callbacks.location_cb\n");
	}
	gps_state->first_fix = 1;
  }

}


static void
nmea_reader_addc( NmeaReader*  r, int  c )
{
    if (r->overflow) {
        r->overflow = (c != '\n');
        return;
    }

    if (r->pos >= (int) sizeof(r->in)-1 ) {
        r->overflow = 1;
        r->pos      = 0;
        return;
    }

    r->in[r->pos] = (char)c;
    r->pos       += 1;

     if (c == '\n') {
        GPS_STATE_LOCK_FIX(gps_state);
        nmea_reader_parse( r );
        GPS_STATE_UNLOCK_FIX(gps_state);
        r->pos = 0;
    }
}


/*****************************************************************/
/*****************************************************************/
/*****                                                       *****/
/*****       C O N N E C T I O N   S T A T E                 *****/
/*****                                                       *****/
/*****************************************************************/
/*****************************************************************/

static void
gps_state_done( GpsState*  s )
{
    // tell the thread to quit, and wait for it
    char   cmd = CMD_QUIT;
    void*  dummy;

    int ret;
    do { ret=write( s->control[0], &cmd, 1 ); }
    while (ret < 0 && errno == EINTR);

    pthread_join(s->thread, &dummy);

    /* Timer thread depends on this state check */
    s->init = STATE_QUIT;
    s->fix_freq = 0;//-1;
    
    // close the control socket pair
    close( s->control[0] ); s->control[0] = -1;
    close( s->control[1] ); s->control[1] = -1;

    // close connection to the TCBIN GPS daemon
    close( s->fd ); s->fd = -1;
    
    // close connection to GPS power
    close( s->pwrfd ); s->pwrfd = -1;
    
    sem_destroy(&s->fix_sem);        
    memset(s, 0, sizeof(*s));
}


static void
gps_state_start( GpsState*  s )
{
    char  cmd = CMD_START;
    int   ret;

    do { ret=write( s->control[0], &cmd, 1 ); }
    while (ret < 0 && errno == EINTR);

    if (ret != 1)
        D("%s: could not send CMD_START command: ret=%d: %s",
          __FUNCTION__, ret, strerror(errno));
}


static void
gps_state_stop( GpsState*  s )
{
    char  cmd = CMD_STOP;
    int   ret;

    do { ret=write( s->control[0], &cmd, 1 ); }
    while (ret < 0 && errno == EINTR);

    if (ret != 1)
        D("%s: could not send CMD_STOP command: ret=%d: %s",
          __FUNCTION__, ret, strerror(errno));
}


static int
epoll_register( int  epoll_fd, int  fd )
{
    struct epoll_event  ev;
    int                 ret, flags;

    /* important: make the fd non-blocking */
    flags = fcntl(fd, F_GETFL);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);

    ev.events  = EPOLLIN;
    ev.data.fd = fd;
    do {
        ret = epoll_ctl( epoll_fd, EPOLL_CTL_ADD, fd, &ev );
    } while (ret < 0 && errno == EINTR);
    return ret;
}


static int
epoll_deregister( int  epoll_fd, int  fd )
{
    int  ret;
    do {
        ret = epoll_ctl( epoll_fd, EPOLL_CTL_DEL, fd, NULL );
    } while (ret < 0 && errno == EINTR);
    return ret;
}

int write_data (int fd, unsigned char write_buffer[], int write_len){
	int ret;
	
	ret = write(fd, write_buffer, write_len);
	
	return ret;
}

void read_all_data(int fd, unsigned char read_buffer[], int buf_len){
	int read_count=0;
	int ret=1;
	
	memset(read_buffer, 0, buf_len);
	
	while(ret > 0){
		ret = read(fd, &read_buffer[read_count], buf_len - read_count);
		ZD("Reading all data");
	}
}

int read_for_ack (int fd, unsigned char read_buffer[], int buf_len, int read_time, unsigned char short_str[], int len_short){
	int read_count=0,match_count=0;
	int ret=0;
	int i=0,j=0;
	
	memset(read_buffer, 0, buf_len);
	
	while(i<read_time){
	    ret = read(fd, &read_buffer[read_count], buf_len - read_count);
	    
	    if(ret > 0){
			for(j=0; j < ret; j++){
				if (read_buffer[read_count + j] == short_str[match_count]){
					match_count++;
				}else{
					match_count=0;
				}
				
				if(match_count == len_short){
					ZD("ACK received");
					return (read_count + ret);
				}
					
			}
		}
	    
	    i++;
	    
		sleep(1);
	    
	    if (ret < 0) {
		    continue;
	    }
	    
	    read_count += ret;
	}
	
	return read_count;
}

#ifdef ZONE_DB
int match_arr (unsigned char long_str[], int len_long, unsigned char short_str[], int len_short){
	int i=0,j=0;
	
	for(i=0; i<len_long; i++){
		if(long_str[i] == short_str[j]){
			j++;
		}else{
			j=0;
		}
		
		if(j >= len_short){
			return 1;
		}
	}
	
	return 0;
}
#endif

/* this is the main thread, it waits for commands from gps_state_start/stop and,
 * when started, messages from the TCBIN GPS daemon. these are simple NMEA sentences
 * that must be parsed to be converted into GPS fixes sent to the framework
 */
static void
gps_state_thread( void*  arg )
{
    GpsState*   state = (GpsState*) arg;
    NmeaReader  reader[1];
    int         epoll_fd   = epoll_create(2);
    int         started    = 0;
    int         gps_fd     = state->fd;
    int         control_fd = state->control[1];
    state->reader = reader;
    nmea_reader_init( reader );

    int offcount = 0;
    
    // register control file descriptors for polling
    epoll_register( epoll_fd, control_fd );
    epoll_register( epoll_fd, gps_fd );

    
    GPS_STATUS_CB(state->callbacks, GPS_STATUS_ENGINE_ON);

    D("gps thread running");
    ZI("gps thread running\n");

    // now loop
    for (;;) {
        struct epoll_event   events[2];
        int                  ne, nevents;

        nevents = epoll_wait( epoll_fd, events, 2, -1 );
        if (nevents < 0) {
            if (errno != EINTR)
                ALOGE("epoll_wait() unexpected error: %s", strerror(errno));
            continue;
        }
        
        D("gps thread received %d events", nevents);
        for (ne = 0; ne < nevents; ne++) {
            if ((events[ne].events & (EPOLLERR|EPOLLHUP)) != 0) {
                ALOGE("EPOLLERR or EPOLLHUP after epoll_wait() !?");
                goto Exit;
            }
            if ((events[ne].events & EPOLLIN) != 0) {
                int  fd = events[ne].data.fd;

                if (fd == control_fd)
                {
                    char  cmd = 255;
                    int   ret;
                    D("gps control fd event");
                    do {
                        ret = read( fd, &cmd, 1 );
                    } while (ret < 0 && errno == EINTR);

                    if (cmd == CMD_QUIT) {
                        D("gps thread quitting on demand");
                        goto Exit;
                    }
                    else if (cmd == CMD_START) {
                        if (!started) {
                            D("gps thread starting  location_cb=%p", state->callbacks.location_cb);
                            started = 1;
                            GPS_STATUS_CB(state->callbacks, GPS_STATUS_SESSION_BEGIN);
							state->init = STATE_START;
							//const char * test = "tcbinGPSTimer";
							state->tmr_thread = state->callbacks.create_thread_cb("tcbinGPSTimer",gps_timer_thread,state);
							if ( state->tmr_thread == 0 ) {
							  ALOGE("WARNING:state->callbacks->create_thread_cb returned zero: %s\n", strerror(errno));
							}
							gps_enable();
                        }
                    }
                    else if (cmd == CMD_STOP) {
                        if (started) {
                            D("gps thread stopping");
                            started = 0;
							//gps_power_toggle();
							gps_disable();
							state->init = STATE_INIT;
							void *dummy;
                            pthread_join(state->tmr_thread, &dummy);
                            GPS_STATUS_CB(state->callbacks, GPS_STATUS_SESSION_END);
                        }
                    }
                }
                else if (fd == gps_fd)
                {
                    char  buff[32];
                    D("gps fd event");

				for (;;) {
                        int  nn, ret;
						ret = read( fd, buff, sizeof(buff) );
						if(ret == 0){			 
						  break;
						}
			
                        if (ret < 0) {
                            if (errno == EINTR)
                                continue;
                            if (errno != EWOULDBLOCK)
                                ALOGE("error while reading from gps daemon socket: %s:", strerror(errno));
                            break;
                        }
                        
                        D("received %d bytes: %.*s", ret, ret, buff);
                        for (nn = 0; nn < ret; nn++)
                            nmea_reader_addc( reader, buff[nn] );
			
						//force disable GPS chip
						if(state->timer_thread_running == 0){			  
						  offcount++;
						  ZD("{RD} WRONG STATE = %d\n",offcount);
						  if(offcount == 4){
							//gps_power_toggle();
							gps_disable();
							offcount = 0;
						  }
						}else{
						 offcount = 0; 
						}
                    }
                    D("gps fd event end");
                }
                else
                {
                    ALOGE("epoll_wait() returned unkown fd %d ?", fd);
                }
            }
        }
    }

 Exit:
    GPS_STATUS_CB(state->callbacks, GPS_STATUS_ENGINE_OFF);
    //gps_power_toggle();
    gps_pwrOff();
    return;
}

static void
gps_timer_thread( void*  arg )
{

  GpsState *state = (GpsState *)arg;
  ZI("gps entered timer thread\n");
  state->timer_thread_running = 1;
  
  do {
    //ZI("gps timer exp; fix fq= %d\n",state->fix_freq);
    GPS_STATE_LOCK_FIX(state);
    
    if (state->reader->fix.flags != 0) {
      ZI("GPS FIX\n");
      
      if (state->callbacks.location_cb) {
	  state->callbacks.location_cb( &state->reader->fix );
          state->reader->fix.flags = 0;
          state->first_fix = 1;
	  
	  if(state->fix_freq == 0){
	    state->fix_freq = -1;
	    ZI("fix_frequency was 0, now -1\n");
	  }
      
      }else{
	  ZI("no location callback\n");
      }                                         
    }
    /*else{
      ZI("GPS NO FIX\n");
    }*/

    if (state->reader->sv_status_changed != 0) {

      ZI("SV STATUS CHANGED\n");
      if (state->callbacks.sv_status_cb) {
	  //ZI("SV STATUS CHANGED CP1\n");
          state->callbacks.sv_status_cb( &state->reader->sv_status );
          state->reader->sv_status_changed = 0;
      }else{
	  ZI("no SV CB\n");
      }
    }
    /*else{
      ZI("SV NOT CHANGED\n");
    }*/

    GPS_STATE_UNLOCK_FIX(state);
    sleep(state->fix_freq);

  } while(state->init == STATE_START);

  ZI("gps timer thread destroyed\n");
  state->timer_thread_running = 0;

  return;
}

static void
gps_state_init( GpsState*  state, GpsCallbacks* callbacks )
{   
	unsigned char to_binary_msg[] = {36,'P','S','R','F','1','0','0',44,'0',44,'9','6','0','0',44,'8',44,'1',44,'0',42,'0','C','\r','\n'};
    unsigned char gain_dis_msg[] = {0xA0, 0xA2, 0x00, 0x39, 0xB2, 0x02, 0x00, 0xF9, 0xC5, 0x68, 0x03, 0xFF, 0x00, 0x00, 0x0B, 0xB8, 0x00, 0x01, 0x77, 0xFA, 0x01, 0x01, 0x03, 0xFC, 0x03, 0xFC, 0x00, 0x04, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x80, 0x00, 0x00, 0x62, 0x00, 0x60, 0x01, 0x01, 0x01, 0xF4, 0x2A, 0x01, 0x0B, 0x58, 0xB0, 0xB3};
	unsigned char ack_pkt_gain_dis[] = {0xA0 ,0xA2 ,0x00 ,0x03 ,0x0B ,0xB2 ,0x00 ,0x00 ,0xBD ,0xB0 ,0xB3};
	unsigned char hot_reset_msg[] = {0xA0, 0xA2, 0x00, 0x19, 0x80, 0xFF, 0xD7, 0x00, 0xF9, 0xFF, 0xBE, 0x52, 0x66, 0x00, 0x3A, 0xC5, 0x7A, 0x00, 0x01, 0x24, 0xF8, 0x00, 0x83, 0xD6, 0x00, 0x03, 0x9C, 0x0C, 0x33, 0x0A, 0x91, 0xB0, 0xB3};
	unsigned char ack_pkt_reset[] = {0xA0, 0xA2, 0x00, 0x03, 0x0B, 0x80, 0x00, 0x00, 0x8B, 0xB0, 0xB3};
	unsigned char to_nema_msg[] = {0xA0, 0xA2, 0x00, 0x18, 0x81, 0x02, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x05, 0x01, 0x01, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x25, 0x80, 0x01, 0x3A, 0xB0, 0xB3};
	int sent;
	int count =0;
	unsigned char read_buffer[65536];
	
    ZI("gps_state_init\n");
    state->init       = STATE_INIT;
    state->control[0] = -1;
    state->control[1] = -1;
    state->fd         = -1;
    state->pwrfd      = -1;
    state->fix_freq   = 0;//-1;
    state->first_fix  = 0;
    state->timer_thread_running = 0;
  
	if (sem_init(&state->fix_sem, 0, 1) != 0) {
	  state->init = STATE_QUIT;
      ALOGE("gps semaphore initialization failed! errno = %d", errno);
      return;
    }
    
  	char pwr_name[128];
    strcpy(pwr_name, TCBIN_GPS_PWR_NODE);
	state->pwrfd = open(pwr_name, O_RDWR);
	
	if (state->pwrfd < 0) {
		state->init = STATE_QUIT;
		ALOGE("ERROR:: NO gps PWR node detected - %s\n",pwr_name);
        D("no gps PWR node detected");    
        return;
    }
      
    char dev_name[128];
    strcpy(dev_name, TCBIN_GPS_DATA_NODE);
    state->fd = open(dev_name, O_RDWR | O_NDELAY | O_NOCTTY);   
    
    if (state->fd < 0) {
		state->init = STATE_QUIT;
		ALOGE("ERROR:: NO gps data node detected - %s\n",dev_name);
        D("no gps device node detected");
        return;
    }
    
    struct termios tio;
    /* we are not concerned about preserving the old serial port configuration
    * CS8, 8 data bits
    * CREAD, receiver enabled
    * CLOCAL, don't change the port's owner 
    */
    tio.c_cflag = B9600 | CS8 | CREAD | CLOCAL;
    tio.c_cflag &= ~HUPCL;      /* clear the HUPCL bit, close doesn't change DTR */
    tio.c_lflag = 0;            /* set input flag non-canonical, no processing */
    tio.c_iflag = IGNPAR;       /* ignore parity errors */
    tio.c_oflag = 0;            /* set output flag non-canonical, no processing */
    tio.c_cc[VTIME] = 0;        /* no time delay */
    tio.c_cc[VMIN] = 0;         /* no char delay */

    tcflush(state->fd, TCIFLUSH);      /* flush the buffer */
    tcsetattr(state->fd, TCSANOW, &tio);       /* set the attributes */

    /* Set up for no delay, ie non-blocking reads will occur. 
    When we read, we'll get what's in the input buffer or nothing */
    fcntl(state->fd, F_SETFL, FNDELAY);

    //PWR ON GPS    
    gps_pwrOn();
    gps_disable();
    
    
    gps_enable();
    
    sleep(2);     	// Keep this - This interval will prepare to get data from GPS. Can be optimized
    ZD("From NEMA to OSP\n");
	sent = write_data(state->fd,to_binary_msg,sizeof(to_binary_msg));
	ZI("Out of %d bytes - %d bytes written\n", sizeof(to_binary_msg), sent);
	
	sleep(1);		// Added to give an interval to switch to OSP. Not optimized. Try removing
	
	ZD("To disable LNA\n");
	sent = write_data(state->fd,gain_dis_msg,sizeof(gain_dis_msg));
	ZI("Out of %d bytes - %d bytes written\n", sizeof(gain_dis_msg), sent);
	
	ZD("Waiting for ACK\n");
	count = read_for_ack(state->fd,read_buffer,sizeof(read_buffer),5, ack_pkt_gain_dis,sizeof(ack_pkt_gain_dis));
#ifdef ZONE_DB
	sent = match_arr(read_buffer,count,ack_pkt_gain_dis,sizeof(ack_pkt_gain_dis));
#endif
	ZI("Bytes read - %d, ACK Received -%d\n",count, sent);
	
	ZD("Hot start rest\n");
	sent = write_data(state->fd,hot_reset_msg,sizeof(hot_reset_msg));
	ZI("Out of %d bytes - %d bytes written\n", sizeof(hot_reset_msg), sent);
	
	ZD("Waiting for ACK\n");
	count = read_for_ack(state->fd,read_buffer,sizeof(read_buffer),5,ack_pkt_reset,sizeof(ack_pkt_reset));
#ifdef ZONE_DB
	sent = match_arr(read_buffer,count,ack_pkt_reset,sizeof(ack_pkt_reset));
#endif
	ZI("Bytes read - %d, ACK Received -%d\n",count, sent);
	
	ZD("To NEMA from OSP\n");
	sent = write_data(state->fd,to_nema_msg,sizeof(to_nema_msg));
	ZI("Out of %d bytes - %d bytes written\n", sizeof(to_nema_msg), sent);
	
	gps_disable();
	// Read all data and clear the read buffer
	read_all_data(state->fd,read_buffer,sizeof(read_buffer));
	
        
    if ( socketpair( AF_LOCAL, SOCK_STREAM, 0, state->control ) < 0 ) {
        ALOGE("could not create thread control socket pair: %s", strerror(errno));
        goto Fail;
    }

    state->thread = callbacks->create_thread_cb( "gps_state_thread", gps_state_thread, state );

    if ( !state->thread ) {
        ALOGE("could not create gps thread: %s", strerror(errno));
        goto Fail;
    }

    state->callbacks = *callbacks;

	ZI("gps state initialized\n");
    D("gps state initialized");
    return;

Fail:
    gps_state_done( state );
}


/*****************************************************************/
/*****************************************************************/
/*****                                                       *****/
/*****       I N T E R F A C E                               *****/
/*****                                                       *****/
/*****************************************************************/
/*****************************************************************/


static int
tcbin_gps_init(GpsCallbacks* callbacks)
{
    ALOGI("TCBIN GPS Init\n");
    GpsState*  s = _gps_state;

    if (!s->init)
        gps_state_init(s, callbacks);

    if (s->fd < 0){
	ALOGE("tcbin_gps_init FAILED\n");
        return -1;
    }

    ZI("tcbin_gps_init success\n");
    return 0;
}

static void
tcbin_gps_cleanup(void)
{
    ZI("tcbin_gps_cleanup\n");
    GpsState*  s = _gps_state;

    if (s->init)
        gps_state_done(s);
}


static int
tcbin_gps_start()
{
//    ZI("tcbin_gps_start\n");
    ALOGI("TCBIN GPS Start\n");
    GpsState*  s = _gps_state;

    if (!s->init) {
        D("%s: called with uninitialized state !!", __FUNCTION__);
        return -1;
    }

    D("%s: called", __FUNCTION__);
    gps_state_start(s);
    return 0;
}


static int
tcbin_gps_stop()
{
ALOGI("TCBIN GPS Stop\n");
    GpsState*  s = _gps_state;

    if (!s->init) {
        D("%s: called with uninitialized state !!", __FUNCTION__);
        return -1;
    }

    D("%s: called", __FUNCTION__);
    gps_state_stop(s);
    return 0;
}

static int
tcbin_gps_inject_time(GpsUtcTime time, int64_t timeReference, int uncertainty)
{
    ZI("tcbin_gps_inject_time: Not implemented\n");
    return 0;
}

static int
tcbin_gps_inject_location(double latitude, double longitude, float accuracy)
{
    ZI("tcbin_gps_inject_location: Not implemented\n");
    return 0;
}

static void
tcbin_gps_delete_aiding_data(GpsAidingData flags)
{
    ZI("tcbin_gps_delete_aiding_data: Not implemented\n");
}

static int tcbin_gps_set_position_mode(GpsPositionMode mode, int fix_frequency)
{
    ZI("tcbin_gps_set_position_mode\n");
    ZI("mode=%d\n",mode);
    ZI("fix_frequency=%d\n",fix_frequency);
    
    GpsState*  s = _gps_state;
//  only standalone supported gps_state_initfor now.
  if (mode != GPS_POSITION_MODE_STANDALONE)
      return -1;

    if (!s->init || fix_frequency < 0) {
        ALOGE("%s: called with uninitialized state !!", __FUNCTION__);
        return -1;
    }

    if(fix_frequency == 0){
      s->fix_freq = 2;
      ZI("fix_frequency was 0, now 2\n");
    }
    else if(fix_frequency == 1){
      s->fix_freq = 0;
      ZI("fix_frequency was 1, now 0\n");
    }else{
      s->fix_freq = fix_frequency;
    }
    D("gps fix frquency set to %d secs", fix_frequency);

    return 0;
}

static const void*
tcbin_gps_get_extension(const char* name)
{
    ZI("tcbin_gps_get_extension:%s - Not implemented\n",name);
    // no extensions supported
    return NULL;
}

static const GpsInterface  tcbinGpsInterface = {
    sizeof(GpsInterface),
    tcbin_gps_init,
    tcbin_gps_start,
    tcbin_gps_stop,
    tcbin_gps_cleanup,
    tcbin_gps_inject_time,
    tcbin_gps_inject_location,
    tcbin_gps_delete_aiding_data,
    tcbin_gps_set_position_mode,
    tcbin_gps_get_extension,
};

const GpsInterface* gps__get_gps_interface(struct gps_device_t* dev)
{
    return &tcbinGpsInterface;
}

static int open_gps(const struct hw_module_t* module, char const* name,
        struct hw_device_t** device)
{

    struct gps_device_t *dev = malloc(sizeof(struct gps_device_t));
    memset(dev, 0, sizeof(*dev));

    dev->common.tag = HARDWARE_DEVICE_TAG;
    dev->common.version = 0;
    dev->common.module = (struct hw_module_t*)module;
    dev->get_gps_interface = gps__get_gps_interface;

    *device = (struct hw_device_t*)dev;
    return 0;
}


static struct hw_module_methods_t gps_module_methods = {
    .open = open_gps
};

struct hw_module_t HAL_MODULE_INFO_SYM = {
    .tag = HARDWARE_MODULE_TAG,
    .version_major = 1,
    .version_minor = 0,
    .id = GPS_HARDWARE_MODULE_ID,
    .name = "TCBIN GPS Module",
    .author = "RuvindaD @ Zone24x7",
    .methods = &gps_module_methods,
};
