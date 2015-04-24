/* 
 * This source code is "Not a Contribution" under Apache license
 *
 * Sierra Wireless GPS Handler, based on gps_freerunner.c
 *
 * Copyright 2006, The Android Open Source Project
 * Copyright 2009, Michael Trimarchi <michael@panicking.kicks-ass.org>
 * Copyright 2011, Sierra Wireless, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <errno.h>
#include <pthread.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <math.h>
#include <netdb.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#define  LOG_TAG  "gps_swi"

#include "swiril_log.h"
#include <cutils/sockets.h>
#include <cutils/properties.h>
#include "swigps_sgi.h"

#define  GPS_DEBUG  0

#define  DFR(...)   LOGD(__VA_ARGS__)

#if GPS_DEBUG
#  define  D(...)   LOGD(__VA_ARGS__)
#else
#  define  D(...)   ((void)0)
#endif

/* Constants used for converting Ascii strings to IP addresses */
#define MAXIPDOTS 3
#define MAXOCTETSIZE 3
#define IPOCTETS 4

/* NMEA parser requires locks */
#define GPS_STATE_LOCK_FIX(_s)         \
{                                      \
  int ret;                             \
  do {                                 \
    ret = sem_wait(&(_s)->fix_sem);    \
  } while (ret < 0 && errno == EINTR); \
}

#define GPS_STATE_UNLOCK_FIX(_s)       \
  sem_post(&(_s)->fix_sem)

static GpsState  _gps_state[1];
GpsState *gps_state = _gps_state;

/*****                              *****
 *****       NMEA   TOKENIZER       *****
 *****                              *****/

typedef struct {
    const char*  p;
    const char*  end;
} Token;

#define  MAX_NMEA_TOKENS  24

typedef struct {
    int     count;
    Token   tokens[ MAX_NMEA_TOKENS ];
} NmeaTokenizer;

/* Forward Declarations */
const void* swigps_get_extension(const char* name);

/** 
 * 
 * Return a pointer to the GPS State structure
 *
 * @param
 *    none
 *
 * @return
 *    Pointer to the swigps state structure
 *
 * @note
 *    None
 *
 */
GpsState *swigps_getstatep()
{
    return gps_state;
}

/** 
 * Convert an ASCII string into an IP Address, validating the contents
 * of the string in the process. Incoming strings must be of the form
 *                 aaa.bbb.ccc.ddd
 * although each octet may contain between 1 and 3 characters. An IP
 * address may not contain more than 3 dots and any octet must be less
 * than or equal to 255. 
 *
 * @param [in] endian
 *     SWIGPS_LE - Convert the incoming string into a Little Endian
 *                 binary IP address
 *     SWIGPS_BE - Convert the incoming string into a Big Endian
 * @param [in] stringp
 *     Pointer to the incoming Ascii string to be converted
 * @param [out] ipaddrp
 *     Pointer to storage into which the binary IP address will be written
 *     if the conversion is correct. Untouched if an error occurs
 *
 * Returns: -1 if not a valid IP address string
 *           0 if the conversion was successful
 *
 */
static int swigps_a2ip( int endianness, 
                        const char *stringp, 
                        unsigned int *ipaddrp )
{
    int index, i;
    int iplength;
    int ipvalid = 1;
    unsigned int ipaddrBE, ipaddrLE;
    int multiplier;
    char nextchar;
    int digit;
    const char *op;
    int dotcount;
    int octetsize[IPOCTETS];
    const char *octet[IPOCTETS];

    if( stringp == NULL ) {
        LOGE("URL String is NULL");
        return -1;
    }

    D("Scanning '%s'\n", stringp );

    /* Scan through making sure it's an IP string */
    iplength = strlen( stringp );

    /* If this is an IP address string, the first
     * (most significant) octet starts with the 
     * left-most character in the string
     */
    dotcount = 0;
    index = 0;
    octet[index] = stringp;
    octetsize[index] = 0;

    /* Scan through each character in the string 
     * checking that it's a valid IP address character
     */
    for( i=0; i<iplength; i++ ) {

        /* Read the next char to avoid a lot of dereferencing */
        nextchar = stringp[i];

        /* Check for acceptable characters in an ASCII IP address */
        if( ( nextchar >= '0' && nextchar <= '9' ) ||
            ( nextchar == '.' )
          )
        {
            /* End of the octet (or could be just a dot in 
             * the string) By the time we're done here we'll
             * know if the string is a valid IP or some fake
             */
            if( nextchar == '.' )
            {
                /* Can only have a certain # of dots in an IPv4 address */
                if( ++dotcount > MAXIPDOTS )
                {
                    ipvalid = 0;
                    break;
                }
                /* Note the size of this group of numbers */
                octetsize[index] = (i - octetsize[index]);

                /* Advance to the next group */
                index += 1;

                /* Remember the next octet's 
                 * starting offset
                 */
                octetsize[index] = i+1;

                /* And save a pointer to it 
                 * in the string as well
                 */
                octet[index] = stringp + (i+1);
            }
            continue;
        }
        else
        {
            /* Not a valid IP address string */
            ipvalid = 0;
            break;
        }
    }

    /* Finish off the last entry */
    octetsize[index] = i - octetsize[index];

    /* At this point we've been through the whole string.
     * Must contain all valid characters and the exact
     * correct number of dot separators. Each size must be
     * three or fewer characters per octet too.
     */
    if( ipvalid && dotcount == MAXIPDOTS ) {

        ipaddrBE = 0;
        ipaddrLE = 0;
        /* Run through each octet, converting it to binary */
        for( index=0; index <IPOCTETS; index++ ) {

            /* Can't be more than 3 chars per octet */
            if( octetsize[index] > MAXOCTETSIZE ) {
				D("Octet %d is %d digits\n", index+1, octetsize[index] );
                return -1;
            }
            multiplier = 1;
            digit = 0;
            op = octet[index];

            /* Go from least significant char to most */
            for( i=octetsize[index]-1; i >= 0; i-- ) {
                digit += (*(op+i) - 0x30) * multiplier;
                multiplier = multiplier * 10;
            }

            /* NOTE: We're reading the string most significant 
             * octet to least. So the first conversion becomes
             * the most significant byte and the last becomes
             * the least significant. 
             */
                 
            /* One final check to make sure the octets are
             * never more than 2**8 - 1 which would make 
             * an illegal IP address
             */
            if( digit > 255 ) {
                D("Digit at octet %d is %d. Too big for IP\n", 
                        index+1, digit );
                return -1;
            }

            /* Build the IP address in Big Endian format */
            ipaddrBE += digit << (8*index);

            /* ... and in Little Endian format */
            ipaddrLE += digit << (24-(8*index));
        }

        if( ipvalid ) {
            D("%s becomes:\n   0x%x LE\n   0x%x BE\n", 
                   stringp, ipaddrLE, ipaddrBE );

            /* Return what the caller asked for */
            if( endianness == SWIGPS_LE )
                *ipaddrp = ipaddrLE;
            else if( endianness == SWIGPS_BE )
                *ipaddrp = ipaddrBE;
            else 
            {
                LOGE("Invalid endianness specified: %d", endianness);
            }
            return 0;
	    }
    }

    LOGI("No IP Address to convert from URL: %s\n", stringp );

    return -1;
}

/**
 * 
 * Take an incoming NMEA string and break it into a series of tokens, 
 * using the embedded comma character as the delimiter
 *
 * @param [out] t 
 *    Token structure to be filled in by this function
 * @param [in] p
 *    Pointer to the string to be tokenized
 * @param [in] end 
 *    Pointer to the last character of the input string
 *    
 * @return
 *    The number of tokens detected. May contain information or be 
 *    NULL
 *
 * @par Abort:<br>
 *    none
 *
 * @note
 *    none
 */
static int
nmea_tokenizer_init( NmeaTokenizer* t, const char* p, const char* end )
{
    int    count = 0;
    char*  q;

    /* the initial '$' is optional */
    if (p < end && p[0] == '$')
        p += 1;

    /* remove trailing newline */
    if (end > p && end[-1] == '\n') {
        end -= 1;
        if (end > p && end[-1] == '\r')
            end -= 1;
    }

    /* delete end-of-sentence checksum */
    if (end >= p+3 && end[-3] == '*') {
        end -= 3;
    }

    while (p < end) {
        const char*  q = p;

        q = memchr(p, ',', end-p);
        if (q == NULL)
            q = end;

        if (count < MAX_NMEA_TOKENS) {
            t->tokens[count].p   = p;
            t->tokens[count].end = q;
            count += 1;
        }

        if (q < end)
            q += 1;

        p = q;
    }

    t->count = count;
    return count;
}

/**
 * 
 * Get the token from an array of tokens at the specified index
 *
 * @param [in] t
 *    Pointer to a token structure. Must have previously been
 *    initialized via a call to nmea_tokenizer_init()
 * @param index
 *    Index of the specified token to be returned to the caller
 *
 * @return
 *   Pointer to the token stored at the indexed location in t
 *
 * @par Abort:<br>
 *    none
 *
 * @note
 *    none
 */
static Token nmea_tokenizer_get( NmeaTokenizer*  t, int  index )
{
    Token  tok;
    static const char*  dummy = "";

    if (index < 0 || index >= t->count) {
        tok.p = tok.end = dummy;
    } else
        tok = t->tokens[index];

    return tok;
}

/** 
 * 
 * Convert a string to an integer
 *
 * @param [in] p
 *    Pointer to the string containing ASCII digits to convert to integer
 * @param [in] end
 *    Pointer to the last character in the input string
 *
 * @return
 *    The integer equivalent of the inputted string or -1 if there was a 
 *    problem with the conversion
 *
 * @par Abort:<br>
 *    none
 *
 * @note
 *    none
 */
static int str2int( const char*  p, const char*  end )
{
    int   result = 0;
    int   len    = end - p;

    if (len == 0) {
      return -1;
    }

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

/** 
 * 
 * Internal function to convert a string to a floating point number
 *
 * @param [in] p
 *    Pointer to the string containing ASCII digits to convert 
 * @param [in] end
 *    Pointer to the last character in the input string
 *
 * @return
 *    The floating point equivalent of the inputted string or 0. if
 *    there was a problem with the conversion
 *
 * @par Abort:<br>
 *    none
 *
 * @note
 *    none
 */
static double str2float( const char*  p, const char*  end )
{
    int   result = 0;
    int   len    = end - p;
    char  temp[16];

    if (len == 0) {
      return -1.0;
    }

    if (len >= (int)sizeof(temp))
        return 0.;

    memcpy( temp, p, len );
    temp[len] = 0;
    return strtod( temp, NULL );
}

/** @desc Convert struct tm to time_t (time zone neutral).
 *
 * The one missing function in libc: It works basically like mktime, 
 * with the main difference that it does no time zone-related processing 
 * but interprets the members of the struct tm as UTC. Unlike mktime, it 
 * will not modify any fields of the tm structure; if you need this behavior, call
 * mktime before this function.
 *
 * @param t Pointer to a struct tm containing date and time. Only the tm_year, 
 *          tm_mon, tm_mday, tm_hour, tm_min and tm_sec members will be evaluated, 
 *          all others will be ignored.
 *
 * @return The epoch time (seconds since 1970-01-01 00:00:00 UTC) which corresponds to t.
 *
 * @author Originally written by Philippe De Muyter <phdm@macqel.be> for Lynx.
 * http://lynx.isc.org/current/lynx2-8-8/src/mktime.c
 */
static time_t mkgmtime(struct tm *t)
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

/*****                                   ***** 
 *****       N M E A   P A R S E R       ***** 
 *****                                   *****/

/**
 * 
 * Obtain the local time and the UTC time from the o/s and compute
 * the difference, saving this into the specified NMEA Reader 
 * structure.
 *
 * @param [in] r
 *    NMEA reader structure to be updated
 *    
 * @return
 *    none
 *
 * @par Abort:<br>
 *    none
 *
 * @note
 *    none
 */
static void nmea_reader_update_utc_diff( NmeaReader*  r )
{
    time_t         now = time(NULL);
    struct tm      tm_local;
    struct tm      tm_utc;
    long           time_local, time_utc;

    gmtime_r( &now, &tm_utc );
    localtime_r( &now, &tm_local );

    time_local = mktime(&tm_local);
    time_utc = mktime(&tm_utc);

    r->utc_diff = time_local - time_utc;
}


/**
 *
 * Initialize an NMEA Reader structure
 *
 * @param [in] r
 *    Pointer to the NMEA reader structure to initialize
 *
 * @return
 *    none
 * 
 * @par Abort:<br>
 *    none
 *
 * @note
 *    none
 */
static void nmea_reader_init( NmeaReader*  r )
{
    memset( r, 0, sizeof(*r) );

    r->pos      = 0;
    r->overflow = 0;
    r->utc_year = -1;
    r->utc_mon  = -1;
    r->utc_day  = -1;

    /* not sure if we still need this (this module doesn't use utc_diff) */
    nmea_reader_update_utc_diff( r );
}

/**
 * 
 * Update the timestamp for the current fix
 *
 * @param [in] r
 *    NMEA reader structure to be updated
 * @param [in] tok
 *    Token containing the time to be converted into a timestamp
 *    
 * @return
 *    -1 if an error occurred
 *     0 if the operation was successful
 *
 * @par Abort:<br>
 *    none
 *
 * @note
 *    none
 */
static int nmea_reader_update_time( NmeaReader*  r, Token  tok )
{
    int        hour, minute, seconds, milliseconds;
    struct tm  tm;
    time_t     fix_time;

    if (tok.p + 6 > tok.end)
        return -1;

    if (r->utc_year < 0) {
        /* no date, can't return valid timestamp (never 
         * ever make up a date, this could wreak havoc)
         */
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

    /* parse also milliseconds (if present) for better precision */
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

    /* the following is only guaranteed to work if we have 
     * previously set a correct date, so be sure to always 
     * do that before
     */
    tm.tm_hour = hour;
    tm.tm_min  = minute;
    tm.tm_sec  = seconds;

    fix_time = mkgmtime( &tm );

    r->fix.timestamp = (long long)fix_time * 1000 + milliseconds;
    return 0;
}

/**
 * 
 * Update UTC time/date in the NMEA reader structure
 *
 * @param [in] r
 *    NMEA reader structure to be updated
 * @param [in] tok_d
 *    Token containing the day of the month, 1..31
 * @param [in] tok_m 
 *    Token containing the ordinal number of the month 1..12
 * @param [in] tok_y
 *    Token containing the year, 4 digits
 *    
 * @return
 *    -1 if an error occurred
 *     0 if the operation was successful
 *
 * @par Abort:<br>
 *    none
 *
 * @note
 *    none
 */
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

/**
 * 
 * Update the date fields in an NMEA Reader structure
 *
 * @param [in] r
 *    NMEA reader structure to be updated
 * @param [in] date
 *    Token containing the date to be written into the NMEA structure
 * @param [in] time 
 *    Token containing the time to be written into the NMEA structure
 *    
 * @return
 *    -1 if an error occurred
 *     0 if the operation was successful
 *
 * @par Abort:<br>
 *    none
 *
 * @note
 *    none
 */
static int nmea_reader_update_date( NmeaReader* r, Token date, Token time )
{
    Token  tok = date;
    int    day, mon, year;

    if (tok.p + 6 != tok.end) {
        D("date not properly formatted: '%.*s'", tok.end-tok.p, tok.p);
        return -1;
    }
    day  = str2int(tok.p, tok.p+2);
    mon  = str2int(tok.p+2, tok.p+4);
    year = str2int(tok.p+4, tok.p+6) + 2000;

    if ((day|mon|year) < 0) {
        D("date not properly formatted: '%.*s'", tok.end-tok.p, tok.p);
        return -1;
    }

    r->utc_year  = year;
    r->utc_mon   = mon;
    r->utc_day   = day;

    return nmea_reader_update_time( r, time );
}

/**
 * 
 * Convert the token into a double-precision coordinate
 *
 * @param token
 *    The token containing the value to be converted 
 *
 * @return
 *    The double precision equivalent value of the token
 * 
 * @par abort:<br>
 *    none
 * 
 * @note
 *    Assumes the caller has passed in a pointer to a token
 *    of the appropriate type of value for the conversion
 * 
 */
static double convert_from_hhmm( Token  tok )
{
    double  val     = str2float(tok.p, tok.end);
    int     degrees = (int)(floor(val) / 100);
    double  minutes = val - degrees*100.;
    double  dcoord  = degrees + minutes / 60.0;
    return dcoord;
}

/**
 * 
 * Update the NMEAreader structure with the specified latitude
 * and longitude values
 *
 * @param [in] r 
 *    Pointer to an NMEA reader structure whose lat/long values
 *    will be updated by this call
 *
 * @param latitude
 *    Token containing a latitude value
 * @param latitudeHemi
 *    Character indicating which hemisphere the latitude value
 *    applies to, N (north) or S (south)
 * @param longitude
 *    Token containing a longitude value
 * @param longitudeHemi
 *    Character indicating which hemisphere the longitude value
 *    applies to, W (west) or E (east)
 * 
 * @return
 *    -1 if there was a problem
 *     0 if the operation was a success
 *
 * @par abort:<br>
 *    none
 *  
 * @note
 *    none
 */
static int
nmea_reader_update_latlong( NmeaReader*  r,
                            Token        latitude,
                            char         latitudeHemi,
                            Token        longitude,
                            char         longitudeHemi )
{
    double   lat, lon;
    Token    tok;

    D("Update Latlong\n");

    tok = latitude;
    if (tok.p + 6 > tok.end) {
        D("latitude is too short: '%.*s'", tok.end-tok.p, tok.p);
        return -1;
    }
    lat = convert_from_hhmm(tok);
    if (latitudeHemi == 'S')
        lat = -lat;

    tok = longitude;
    if (tok.p + 6 > tok.end) {
        D("longitude is too short: '%.*s'", tok.end-tok.p, tok.p);
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


/**
 * 
 * Update the NMEAreader structure with the specified altitude
 *
 * @param [in] r 
 *    Pointer to an NMEA reader structure whose altitude value
 *    will be updated by this call
 * @param altitude
 *    Token containing an altitude value
 * @param units
 *    unused
 * 
 * @return
 *    -1 if there was a problem
 *     0 if the operation was a success
 *
 * @par abort:<br>
 *    none
 *  
 * @note
 *    none
 */
static int
nmea_reader_update_altitude( NmeaReader*  r,
                             Token        altitude,
                             Token        units )
{
    double  alt;
    Token   tok = altitude;

    if (tok.p >= tok.end)
        return -1;

    r->fix.flags   |= GPS_LOCATION_HAS_ALTITUDE;
    r->fix.altitude = str2float(tok.p, tok.end);
    return 0;
}

/**
 * 
 * Update the NMEAreader structure with the specified accuracy
 *
 * @param [in] r 
 *    Pointer to an NMEA reader structure whose PDOP accuracy
 *    value will be updated by this call
 * @param accuracy
 *    Token containing an accuracy value
 * @param units
 *    unused
 * 
 * @return
 *    -1 if there was a problem
 *     0 if the operation was a success
 *
 * @par abort:<br>
 *    none
 *  
 * @note
 *    none
 */
static int nmea_reader_update_accuracy( NmeaReader* r, Token accuracy )
{
    double  acc;
    Token   tok = accuracy;

    if (tok.p >= tok.end)
        return -1;

    r->fix.accuracy = str2float(tok.p, tok.end);

    /* No accuracy available yet, just return */
    if (r->fix.accuracy == 99.99){
      return 0;
    }

    r->fix.flags   |= GPS_LOCATION_HAS_ACCURACY;
    return 0;
}

/**
 * 
 * Update the NMEAreader structure with the specified bearing
 *
 * @param [in] r 
 *    Pointer to an NMEA reader structure whose bearing
 *    will be updated by this call
 * @param bearing
 *    Token containing a bearing
 * @param units
 *    unused
 * 
 * @return
 *    -1 if there was a problem
 *     0 if the operation was a success
 *
 * @par abort:<br>
 *    none
 *  
 * @note
 *    none
 */
static int nmea_reader_update_bearing( NmeaReader* r, Token bearing )
{
    double  alt;
    Token   tok = bearing;

    if (tok.p >= tok.end)
        return -1;

    r->fix.flags   |= GPS_LOCATION_HAS_BEARING;
    r->fix.bearing  = str2float(tok.p, tok.end);
    return 0;
}

/**
 * 
 * Update the NMEAreader structure with the specified speed
 *
 * @param [in] r 
 *    Pointer to an NMEA reader structure whose bearing
 *    will be updated by this call
 * @param speed
 *    Token containing a speed value
 * 
 * @return
 *    -1 if there was a problem
 *     0 if the operation was a success
 *
 * @par abort:<br>
 *    none
 *  
 * @note
 *    none
 */
static int nmea_reader_update_speed( NmeaReader* r, Token speed )
{
    double  alt;
    Token   tok = speed;

    if (tok.p >= tok.end)
        return -1;

    r->fix.flags   |= GPS_LOCATION_HAS_SPEED;
    /* convert knots into m/sec (1 knot equals 1.852 km/h, 
     * 1 km/h equals 3.6 m/s) since 1.852 / 3.6 is an odd 
     * value (periodic), we're calculating the quotient on the fly
     * to obtain maximum precision (we don't want 1.9999 instead of 2)
     */
    r->fix.speed    = str2float(tok.p, tok.end) * 1.852 / 3.6;
    return 0;
}

/**
 *
 * Parse an NMEA sentence
 *
 * @param [in] r
 *    the reader structure containing the raw sentence to be parsed
 *
 * @return
 *    none
 *
 * @par abort:<br>
 *    none
 * 
 * @note
 *    Fields within the NmeaReader structure are updated with data
 *    parsed from the packet, as applicable
 *
 */
static void nmea_reader_parse( NmeaReader*  r )
{
   /* received a complete sentence, now 
    * parse it and update the appropriate 
    * fields 
    */
    NmeaTokenizer  tzer[1];
    Token          tok;

    D("Received: '%.*s'", r->pos, r->in);

    if (r->pos < 9) {
        D("Too short. discarded.");
        return;
    }

    nmea_tokenizer_init(tzer, r->in, r->in + r->pos);
#if 0
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
#endif

    tok = nmea_tokenizer_get(tzer, 0);

    if (tok.p + 5 > tok.end) {
        D("sentence id '%.*s' too short, ignored.", tok.end-tok.p, tok.p);
        return;
    }

    /* ignore first two characters */
    tok.p += 2;

    /* GGA - GPS Fix Data message */
    if ( !memcmp(tok.p, "GGA", 3) ) {
        /* GPS fix */
        Token  tok_fixstaus      = nmea_tokenizer_get(tzer,6);

        if ((tok_fixstaus.p[0] > '0') && (r->utc_year >= 0)) {
          /* ignore this until we have a valid timestamp */

          Token  tok_time          = nmea_tokenizer_get(tzer,1);
          Token  tok_latitude      = nmea_tokenizer_get(tzer,2);
          Token  tok_latitudeHemi  = nmea_tokenizer_get(tzer,3);
          Token  tok_longitude     = nmea_tokenizer_get(tzer,4);
          Token  tok_longitudeHemi = nmea_tokenizer_get(tzer,5);
          Token  tok_altitude      = nmea_tokenizer_get(tzer,9);
          Token  tok_altitudeUnits = nmea_tokenizer_get(tzer,10);

          /* don't use this as we have no fractional seconds 
           * and no date; there are better ways to get a 
           * good timestamp from GPS
           */
          //nmea_reader_update_time(r, tok_time);
          nmea_reader_update_latlong(r, tok_latitude,
                                        tok_latitudeHemi.p[0],
                                        tok_longitude,
                                        tok_longitudeHemi.p[0]);
          nmea_reader_update_altitude(r, tok_altitude, tok_altitudeUnits);
        }

    /* GLL - Geographic Position - Lat/Long message */
    } else if ( !memcmp(tok.p, "GLL", 3) ) {

        Token  tok_fixstaus      = nmea_tokenizer_get(tzer,6);

        if ((tok_fixstaus.p[0] == 'A') && (r->utc_year >= 0)) {
          /* ignore this until we have a valid timestamp */

          Token  tok_latitude      = nmea_tokenizer_get(tzer,1);
          Token  tok_latitudeHemi  = nmea_tokenizer_get(tzer,2);
          Token  tok_longitude     = nmea_tokenizer_get(tzer,3);
          Token  tok_longitudeHemi = nmea_tokenizer_get(tzer,4);
          Token  tok_time          = nmea_tokenizer_get(tzer,5);

          /* don't use this as we have no fractional seconds and 
           * no date; there are better ways to get a good 
           * timestamp from GPS
           */
          //nmea_reader_update_time(r, tok_time);
          nmea_reader_update_latlong(r, tok_latitude,
                                        tok_latitudeHemi.p[0],
                                        tok_longitude,
                                        tok_longitudeHemi.p[0]);
        }

    /* GSA - GPS DOP and Active Satellites message */
    } else if ( !memcmp(tok.p, "GSA", 3) ) {

        Token  tok_fixStatus   = nmea_tokenizer_get(tzer, 2);
        int i;

        if (tok_fixStatus.p[0] != '\0' && tok_fixStatus.p[0] != '1') {

          /* Get PDOP as a check on overall accuracy. Three measures
           * are reported in the GSA sentence. According to Wikipedia,
           * Dilution of Precision can be interpreted as follows:
           * Value       Rating
           *    1         Ideal, i.e. as close to perfect as you can get
           *  1-2         Excellent
           *  2-5         Good
           * 5-10         Moderate
           * 10-20        Fair
           *   >20        Poor
           */
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
            }

          }

        }

    /* GSV - GPS Satellites in View message */
    } else if ( !memcmp(tok.p, "GSV", 3) ) {

        Token  tok_noSatellites  = nmea_tokenizer_get(tzer, 3);
        int    noSatellites = str2int(tok_noSatellites.p, tok_noSatellites.end);
       
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

    /* RMC - Recommended Minimum Specific GPS/Transit data message */
    } else if ( !memcmp(tok.p, "RMC", 3) ) {

        Token  tok_fixStatus     = nmea_tokenizer_get(tzer,2);

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

    /* VTG - Track Made Good and Ground Speed message */
    } else if ( !memcmp(tok.p, "VTG", 3) ) {

        Token  tok_fixStatus     = nmea_tokenizer_get(tzer,9);

        if (tok_fixStatus.p[0] != '\0' && tok_fixStatus.p[0] != 'N')
        {
            Token  tok_bearing       = nmea_tokenizer_get(tzer,1);
            Token  tok_speed         = nmea_tokenizer_get(tzer,5);

            nmea_reader_update_bearing( r, tok_bearing );
            nmea_reader_update_speed  ( r, tok_speed );
        }

    /* ZDA - Time and Date message */
    } else if ( !memcmp(tok.p, "ZDA", 3) ) {

        Token  tok_time;
        Token  tok_year  = nmea_tokenizer_get(tzer,4);
        tok_time  = nmea_tokenizer_get(tzer,1);

        if ((tok_year.p[0] != '\0') && (tok_time.p[0] != '\0')) {

          /* make sure to always set date and time together, lest bad things happen */
          Token  tok_day   = nmea_tokenizer_get(tzer,2);
          Token  tok_mon   = nmea_tokenizer_get(tzer,3);

          nmea_reader_update_cdate( r, tok_day, tok_mon, tok_year );
          nmea_reader_update_time(r, tok_time);
        }


      /* Remember the first two chars are already skipped 
       * so we look for XFI instead of PQXFI
       */
    } else if ( !memcmp(tok.p, "XFI", 3) ) {
        /* We don't really care about this packet, but 
         * we also don't like seeing all the unknown
         * sentence messages filling up the log 
         */
        D("Ignoring Qcom PQXFI sentence");
        tok.p -= 2; /* Dummy statement to prevent optimization */
    

    /* Remember the first two chars are already skipped 
     * so we look for GNS instead of GNGNS
     */
    } else if ( !memcmp(tok.p, "GNS", 3) ) {
          /* We don't really care about this packet, but 
           * we also don't like seeing all the unknown
           * sentence messages filling up the log 
           */
          D("Ignoring GNGNS sentence");
          tok.p -= 2; /* Dummy statement to prevent optimization */

    } else {
        tok.p -= 2;
        DFR("unknown sentence '%.*s'", tok.end-tok.p, tok.p);
    }

    /* This clause will report once only if conditions 
     * are right and there hasn't already been a first 
     * fix reported. Subsequent fixes are reported by 
     * the timer thread periodically
     */
    if (!gps_state->first_fix &&
        gps_state->init == STATE_INIT &&
        r->fix.flags & GPS_LOCATION_HAS_LAT_LONG) {

        if (gps_state->callbacks.location_cb) {
            gps_state->callbacks.location_cb( &r->fix );
            r->fix.flags = 0;
        }

        gps_state->first_fix = 1;
    }

#if 0
    if (r->fix.flags != 0) {
#if GPS_DEBUG
        char   temp[256];
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
        gmtime_r( (time_t*) &r->fix.timestamp, &utc );
        p += snprintf(p, end-p, " time=%s", asctime( &utc ) );
        D(temp);
#endif
        if (r->callback) {
            r->callback( &r->fix );
            r->fix.flags = 0;
        }
        else {
            D("no callback, keeping data until needed !");
        }
    }
#endif
}

/**
 * 
 * Add a character to the raw sentence buffer in the NMEA 
 * reader structure, sending that structure to be parsed 
 * if a NewLine sequence is detected
 *
 * @param [in] r 
 *    NMEA Reader structure the character will be added to
 *    and which will be parsed into when a newline is detected
 *
 * @param c
 *    next character to add into the raw buffer within the 
 *    NMEA reader structure
 * 
 * @par abort:<br>
 *    none
 * 
 * @note
 *    none
 */
static void nmea_reader_addc( NmeaReader*  r, int  c )
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

/*****                              *****
 *****       CONNECTION STATE       *****
 *****                              *****/

/* commands sent to the main gps state thread */
enum {
    CMD_QUIT       = 0,
    CMD_START      = 1,
    CMD_SETMODE    = 2,
    CMD_STOP       = 3,
    CMD_AGPSSRVR   = 4,
    CMD_INJECTTIME = 5
};

/**
 * 
 * Send a command to the main thread telling it to set the 
 * modem's operating mode
 *
 * @param [in] sp
 *    Pointer to a state structure - must have been previously
 *    initialized
 *    
 * @return
 *    none
 *
 * @par Abort:<br>
 *    none
 *
 * @note
 *    none
 */
void gps_state_setmode( GpsState*  s )
{
    char  cmd = CMD_SETMODE;
    int   ret;

    D("Entered %s\n", __FUNCTION__ );
    do { ret=write( s->control[0], &cmd, 1 ); }
    while (ret < 0 && errno == EINTR);

    if (ret != 1)
        D("%s: could not send CMD_SETMODE: ret=%d: %s",
          __FUNCTION__, ret, strerror(errno));
}

/**
 * 
 * Close down the gps engine. This function provides a convenient 
 * place to tie up any business necessary for an orderly shutdown
 * of the GPS package
 *
 * @param
 *   [in] s - pointer to a pre-initialized state variable 
 *
 * @note
 *   Called from a context outside of this package's thread
 */
static void gps_state_done( GpsState*  s )
{
    /* tell the thread to quit, and wait for it */
    char   cmd = CMD_QUIT;
    void*  dummy;
    int ret;

    DFR("%s started", __FUNCTION__);

    do { ret=write( s->control[0], &cmd, 1 ); }
    while (ret < 0 && errno == EINTR);

    DFR("gps waiting for threads to stop");

    pthread_join(s->swigps_thread, &dummy);

    D("state thread stopped");

    /* Timer thread depends on this state check */
    s->init = STATE_QUIT;
    s->fix_freq = -1;

    pthread_join(s->swigps_tmr_thread, &dummy);
    D("timer thread stopped");

    /* close the control socket pair */
    close( s->control[0] ); s->control[0] = -1;
    close( s->control[1] ); s->control[1] = -1;

    /* close the connection to the NMEA stream */
    if( s->fd != -1 ) {
        close( s->fd ); 
        s->fd = -1;
	}

    /* if the gps socket is open, close it */
    if(s->rilgpsfd >= 0)
    {
        close( s->rilgpsfd ); 
        s->rilgpsfd = -1;
    }

    sem_destroy(&s->fix_sem);

    memset(s, 0, sizeof(*s));

    DFR("gps deinit complete");
}


/**
 * 
 * Send a command to the main thread telling it to start up
 *
 * @param [in] sp
 *    Pointer to a state structure - must have been previously
 *    initialized
 *    
 * @return
 *    none
 *
 * @par Abort:<br>
 *    none
 *
 * @note
 *    none
 */
static void gps_state_start( GpsState*  s )
{
    char  cmd = CMD_START;
    int   ret;

    D("Entered %s\n", __FUNCTION__ );
    do { ret=write( s->control[0], &cmd, 1 ); }
    while (ret < 0 && errno == EINTR);

    if (ret != 1)
        D("%s: could not send CMD_START: ret=%d: %s",
          __FUNCTION__, ret, strerror(errno));
}


/**
 * 
 * Sends a command to the gps thread telling it to stop execution
 * 
 * @param
 *    sp [in] pointer to a gps state structure
 *
 * @note
 *    This function runs outside the context of the GPS thread
 *
 */
static void gps_state_stop( GpsState*  s )
{
    char  cmd = CMD_STOP;
    int   ret;

    do { ret=write( s->control[0], &cmd, 1 ); }
    while (ret < 0 && errno == EINTR);

    if (ret != 1)
        D("%s: could not send CMD_STOP command: ret=%d: %s",
          __FUNCTION__, ret, strerror(errno));
}

/**
 * 
 * Send a command to the main thread telling it to set the 
 * modem's AGPS server information
 *
 * @param [in] sp
 *    Pointer to a state structure - must have been previously
 *    initialized
 *    
 * @return
 *    none
 *
 * @par Abort:<br>
 *    none
 *
 * @note
 *    none
 */
static void gps_state_agpsserver( GpsState*  s )
{
    char  cmd = CMD_AGPSSRVR;
    int   ret;

    D("Entered %s\n", __FUNCTION__ );
    do { ret=write( s->control[0], &cmd, 1 ); }
    while (ret < 0 && errno == EINTR);

    if (ret != 1)
        D("%s: could not send CMD_AGPSSRVR: ret=%d: %s",
          __FUNCTION__, ret, strerror(errno));
}

/**
 * 
 * Send a command to the main thread to inject the current time
 * into the AGPS system in the modem
 *
 * @param [in] sp
 *    Pointer to a state structure - must have been previously
 *    initialized
 *    
 * @return
 *    none
 *
 * @par Abort:<br>
 *    none
 *
 * @note
 *    none
 */
static void gps_state_injecttime( GpsState*  s )
{
    char  cmd = CMD_INJECTTIME;
    int   ret;

    D("Entered %s\n", __FUNCTION__ );
    do { ret=write( s->control[0], &cmd, 1 ); }
    while (ret < 0 && errno == EINTR);

    if (ret != 1)
        D("%s: could not send CMD_SETMODE: ret=%d: %s",
          __FUNCTION__, ret, strerror(errno));
}

/**
 * 
 * Register the specified file descriptor against the specified
 * epoll descriptor. The epoll mechanism is similar to the select
 * mechanism but the original gps_qemu.c file used this instead
 *
 * @param [in] epoll_fd
 *    The epoll file descriptor to attach the incoming fd to
 * @param [in] 
 *    The file descriptor to attach
 *    
 * @return
 *    0 if the operation was a success, 
 *    negative value otherwise
 *
 * @par Abort:<br>
 *    none
 *
 * @note
 *    none
 */
static int epoll_register( int  epoll_fd, int  fd )
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

/**
 * 
 * Deregister the specified file descriptor, removing it from the
 * specified epoll descriptor. This operation is used when shutting
 * down the GPS engine
 *
 * @param [in] epoll_fd
 *    The epoll file descriptor containing the registered fd
 * @param [in] fd
 *    The file descriptor to detach from the epoll_fd
 *    
 * @return
 *    0 if the operation was a success, 
 *    negative value otherwise
 *
 * @par Abort:<br>
 *    none
 *
 * @note
 *    none
 */
static int epoll_deregister( int  epoll_fd, int  fd )
{
    int  ret;
    do {
        ret = epoll_ctl( epoll_fd, EPOLL_CTL_DEL, fd, NULL );
    } while (ret < 0 && errno == EINTR);
    return ret;
}

/**
 * 
 * Locate and open the NMEA serial port and initialize the TTY
 * settings for it
 *
 * @param [in] pstate
 *    Pointer to the GPS state structure
 *
 * @return
 *     0 Successfully opened
 *    -1 An error occurred, check the log for details
 *
 * @par Abort:<br>
 *    none
 *
 * @note
 *    none
 */
int open_nmea( GpsState *pstate )
{
    /* If no port name defined, best to bail out */
    if( !pstate->gpsnmeaport.initialized ) {
        LOGE("Gps NMEA Port not defined" );
        pstate->init = STATE_QUIT;
        return -1;
    }

    /* Log the action */
    LOGI("open_nmea: Opening %s for NMEA traffic", pstate->gpsnmeaport.name );

    /* Port name has been fetched */
    do {
        pstate->fd = open( pstate->gpsnmeaport.name, O_RDWR );
    } while (pstate->fd < 0 && errno == EINTR);

    if (pstate->fd < 0) {
        LOGE("open_nmea: can't open nmea port. Error %s", strerror(errno) );
        pstate->init = STATE_QUIT;
        return -1;
    }

    D("open_nmea: %s opened", pstate->gpsnmeaport.name);

    /* disable echo on serial lines */
    if ( isatty( pstate->fd ) ) {
        struct termios  ios;
        tcgetattr( pstate->fd, &ios );
        ios.c_lflag = 0;            /* disable ECHO, ICANON, etc... */
        ios.c_oflag &= (~ONLCR);    /* Stop \n -> \r\n translation on output */
        ios.c_iflag &= (~(ICRNL | INLCR)); /* Stop \r->\n & \n->\r Xlation */
        ios.c_iflag |= (IGNCR | IXOFF);    /* Ignore \r & XON/XOFF on input */
        tcsetattr( pstate->fd, TCSANOW, &ios );
    }

    /* Success */
    return 0;
}

/**
 * 
 * Auxiliary thread whose purpose is to run at the requested
 * fix_frequency rate. Some modems' NMEA outputs cannot be 
 * controlled and may spit out packets much more frequently 
 * than desired by the controlling application. This thread 
 * ensures a fix is sent to the application only as often as 
 * that application wants. 
 *
 * @param
 *    arg [in] - pointer to a state variable which this 
 *               thread will use to interact with other 
 *               parts of this package
 *
 * @note
 *   This thread is terminated whenever the interface is 
 *   stopped and re-opened on an interface (*start) call.
 * 
 *   It is terminated permanently when the (*close) function
 *   is called 
 */
void *gps_timer_thread( void*  arg )
{
  GpsState *state = (GpsState *)arg;

  DFR("timer thread started");

  /* This thread runs continuously unless the state variable
   * is changed to STATE_QUIT, which happens when the entire
   * interface is closed down via a call to swigps_cleanup()
   */
  for( ;; ) {
 
    /* The running state is selected when the interface's 
     * start routine is called 
     */
    if( state->init == STATE_RUNNING ) {

        GPS_STATE_LOCK_FIX(state);
 
        /* If the fix flags have been updated, send
         * the latest fix information to the installed
         * callback routine, if available 
         */
        if (state->reader.fix.flags != 0) {

          D("gps fix cb: 0x%x", state->reader.fix.flags);

          /* Take the recurrence flag into account. Only send
           * a fix if recurrence is "periodic" or recurrence is 
           * "single shot" and// there hasn't been a fix reported
           * yet. This latter condition is met if first_fix is
           * zero and recurrence is one. 
           */
          if( !(state->pdsDefs.recurrence) || 
               (state->pdsDefs.recurrence && !state->first_fix) ) {
              if (state->callbacks.location_cb) {
                  D("location_cb called with lat/long: %f/%f\n", 
                     state->reader.fix.latitude, state->reader.fix.longitude);
                  state->callbacks.location_cb( &state->reader.fix );
                  /* Clear the flags for next time */
                  state->reader.fix.flags = 0;
                  /* Flag first_fix has been achieved 
                   * (only counts the first time) 
                   */
                  state->first_fix = 1;
              }
          }
	    }
    
        if (state->reader.sv_status_changed != 0) {
    
          D("gps sv status callback");

          if (state->callbacks.sv_status_cb) {
              state->callbacks.sv_status_cb( &state->reader.sv_status );
              state->reader.sv_status_changed = 0;
          }
        }

        GPS_STATE_UNLOCK_FIX(state);
    }

    /* STATE_QUIT occurs when the entire interface shuts down.
     * When that happens, we want this thread to terminate
     */
    else if( state->init == STATE_QUIT ) {
        break;
    }

    /* Always sleep regardless of the state */
    sleep(state->fix_freq);
  }

  DFR("gps timer thread terminating");

  return NULL;
}

/**
 * 
 * Main GPS state thread - remains running until the Location application 
 * calls this interface's (*close) function. The thread awaits incoming 
 * commands to START/STOP or QUIT execution and also for NMEA sentences 
 * from the receiver which it parses
 *
 * @param [in] arg
 *    Pointer to an instance of this package's gps state structure, 
 *    previously initialized
 * 
 * @return
 *    NULL, always
 * 
 * @par abort:<br>
 *    none
 * 
 * @note
 *    The interface's Init function kicks off this thread. The thread runs 
 *    forever or until the interface's (*close) function is called
 *
 */
void *gps_state_thread( void*  arg )
{
    GpsState*   state = (GpsState*) arg;
    NmeaReader  *reader;
    struct hostent hostip;
    int         epoll_fd   = epoll_create(3);
    int         control_fd = state->control[1];
    int         gps_fd = -1;
    int         ret;
    static char rilbuf[512];

    /* Clear the interprocess message buffer */
    memset( rilbuf, 0, sizeof(rilbuf) );

    reader = &state->reader;

    nmea_reader_init( reader );

    /* register control file descriptors for polling */
    epoll_register( epoll_fd, control_fd );
    epoll_register( epoll_fd, state->rilgpsfd );

    DFR("gps state started. FDs: ctl=%d, rilgps=%d",control_fd,state->rilgpsfd);

    /* This function call is mandatory. This gps hardware interface
     * engine will not be fully started until the NMEA port is known
     * The ENGINE_ON status is sent once the response to the NMEA port
     * request is received
     */
    init_nmea();

    /* Kick off a call to get the PDS Defaults */
    swigps_init_state_thread( state );

    /* In case a modem's tracking session is enabled on
     * reboot, stop that session now until it's needed,
     * and in the meantime the modem will be able to 
     * enter a low-power state to reduce power drain
     */
    //stop_session(); UNCOMMENT THIS WHEN SL808X can accept it

    /* 
     * Send commands to the GPS device from this point 
     * if required as part of startup initialization 
     */

    /* loop forever */
    for (;;) {
        struct epoll_event   events[3];
        int                  ne, nevents;

        /* Await events from either the gps receiver or
         * a command generator such as gps_state_start(), 
         * etc.
         */
        nevents = epoll_wait( epoll_fd, events, 3, -1 );
        if (nevents < 0) {
            if (errno != EINTR)
                LOGE("epoll_wait() unexpected error: %s", strerror(errno));
            continue;
        }

        /* Got one or more events, process each one in sequence */
        D("state thread received %d events", nevents);
        for (ne = 0; ne < nevents; ne++) {
            if ((events[ne].events & (EPOLLERR|EPOLLHUP)) != 0) {
                LOGE("EPOLLERR or EPOLLHUP after epoll_wait() !?");
                goto Exit;
            }
            if ((events[ne].events & EPOLLIN) != 0) {
                int  fd = events[ne].data.fd;

                /* A GPS Interface function has originated a command */
                if (fd == control_fd) {
                    char  cmd = 255;
                    int   ret;
                    D("gps control fd event");
                    do {
                        ret = read( fd, &cmd, 1 );
                    } while (ret < 0 && errno == EINTR);

                    if (cmd == CMD_QUIT) {
                        D("gps state thread quitting by external request");
                        stop_session();
                        goto Exit;
                    }
                    else if (cmd == CMD_START) {
                        if (!state->tracking) {
                            D("gps state thread starting: location_cb=%p", 
                              state->callbacks.location_cb);
                            state->tracking = true;

                            /* Open the NMEA interface */
                            if( !open_nmea(state) ) {

                                /* Get the NMEA file descriptor */
                                gps_fd = state->fd;

                                D("NMEA Opened, FD=%d", gps_fd );
 
                                /* Add the new file descriptor to the poll */
                                ret = epoll_register( epoll_fd, gps_fd );

                                D("epoll_reg returns %d", ret );

                                /* Insert commands for the GPS 
                                 * device to start it here 
                                 */
                                start_session();
                            }
                            else {
                                LOGE("Failed to start GPS");
                            }
                        }
                    }
                    else if (cmd == CMD_STOP) {
                        if (state->tracking) {
                            void *dummy;
                            D("gps state thread stopping");
                            state->tracking = false;

                            /* Stopping but not terminating this thread
                             * so we need to deregister the NMEA FD
                             */
                            epoll_deregister( epoll_fd, gps_fd  );

                            /* Add commands here as needed to stop the GPS 
                             * device, e.g. send powerdown commands, etc. 
                             */
                            stop_session();

                            state->init = STATE_INIT;

                            /* close the connection to the NMEA stream */
                            if( state->fd != -1 ) {
                                close( state->fd ); 
                                state->fd = -1;
	                        }

                            swigps_report_state(&state->callbacks, 
                                                GPS_STATUS_SESSION_END);
                        }
                    }
                    else if (cmd == CMD_SETMODE) {
                        D("Initiating setmode request, new mode: %d",
                          state->pdsDefs.reqmode);
 
                        /* Call the modem-specific function */
                        set_defaults();
                    }
                    else if (cmd == CMD_AGPSSRVR) {
                        D("Initiating swigps_setagps, ip: 0x%x  url: %s", 
                          state->agpsInfo.reqipaddr, state->agpsInfo.requrl);

                        /* Call the modem-specific handler */
                        set_agps();
                    }
                    else if (cmd == CMD_INJECTTIME) {
                        D("Initiating inject_time(). time=%llu, uncertainty=%d",
                           state->injectTime.time, 
                           state->injectTime.uncertainty );

                        /* Format and send the request to the modem */
                        inject_time();
                    }
                }

                /* Incoming bytes from the NMEA channel available */
                else if (fd == gps_fd) {
                    char buf[512];
                    int  nn, ret;

                    do {
                        ret = read( fd, buf, sizeof(buf) );
                    } while (ret < 0 && errno == EINTR);

                    if (ret > 0)
                        for (nn = 0; nn < ret; nn++)
                            nmea_reader_addc( reader, buf[nn] );
                    D("gps fd event end");
                }
                /* Handle incoming packet from the ril GPS agent */
                else if (fd == state->rilgpsfd) {
                    handle_rilgps(state, fd);
                }
                else {
                    LOGE("epoll_wait() returned unknown fd %d ?", fd);
                }
            }
        }
	}
Exit:
    swigps_report_state(&state->callbacks, GPS_STATUS_ENGINE_OFF);
    /* From here, send any commands to 
     * the GPS needed to shut it down
     */

    DFR("gps state thread terminating");
    return NULL;
}


/**
 * 
 * Initialize the entire interface and engine. 
 *
 * @param 
 *   statep [in] - Pointer to a state structure which will be 
 *                 initialized by this call and which must
 *                 remain valid for this interface's lifetime
 * @return
 *    none
 * 
 * @par abort:<br>
 *    none
 * 
 * @note
 *    Initializes everything needed by the main state thread
 *    including state structure, GPS socket to the RIL daemon.
 *    When all initialization is complete the main state thread
 *    and timer thread are created
 *
 */
static void gps_state_init( GpsState*  pstate )
{
    struct sigevent tmr_event;

    pstate->init       = STATE_INIT;
    pstate->rilgpsfd    = -1;
    pstate->control[0] = -1;
    pstate->control[1] = -1;
    pstate->fd         = -1;
    pstate->fix_freq   = -1;
    pstate->first_fix  = 0;
    pstate->tracking = false;

    /* Initialize Default PDS settings */
    swigps_init_pdsDefs( pstate );

    /* Initialize the agps information */
    pstate->agpsInfo.type = -1;
    pstate->agpsInfo.ipaddr = -1;
    pstate->agpsInfo.port = -1;
    memset( pstate->agpsInfo.url, 0, MAXURL+1);

    /* Perform Android version-specific initialization */
    swigps_init_state( pstate );
    
    /* and the nmea port name string */
    pstate->gpsnmeaport.initialized = 0;
    memset( pstate->gpsnmeaport.name, 0, sizeof(pstate->gpsnmeaport.name) );

    if (sem_init(&pstate->fix_sem, 0, 1) != 0) {
      D("gps semaphore initialization failed! errno = %d", errno);
      return;
    }

    /* Create a socket pair to exchange control messages between 
     * the holder of this interface and the gps state thread
     */
    if ( socketpair( AF_LOCAL, SOCK_STREAM, 0, pstate->control ) < 0 ) {
        LOGE("could not create thread control socket pair: %s", strerror(errno));
        goto Fail;
    }

    /* This is to fix a race condition where the RIL daemon may not
     * have started before this code does. In that case, opening the
     * socket, below, will fail 
     */
    sleep(1);

    /* Open the socket to the GPS Agent running under the RIL daemon.
     * NOTE: The user running this process right here is "system" and 
     * the process name is "system_server". Important for setting up
     * permissions on this socket in init.rc (as of Android 2.2.1)
     */
    pstate->rilgpsfd = socket_local_client( SOCKET_NAME_SWIGPS, 
                              ANDROID_SOCKET_NAMESPACE_RESERVED, 
                              SOCK_STREAM );
 
    if( pstate->rilgpsfd < 0 )
    {
        LOGE("Can't open GPS socket %s\n", SOCKET_NAME_SWIGPS );
        goto Fail;
    }

    D("socket '%s' opened\n", SOCKET_NAME_SWIGPS );

    /* Create the gps state thread */
    if( swigps_create_state_thread( pstate ) != 0 ) {
        goto Fail;
    }

    /* Create the gps timer thread */
    if( swigps_create_timer_thread( pstate ) != 0 ) {
        goto Fail;
    }

    D("swigps initialized");

    return;

Fail:
    gps_state_done( pstate );
}


/*****      Interface Functions     *****/

/**
 * 
 * Opens the interface and assigns the caller's callback function
 * pointers through this interface. These are called during runtime
 * when fix information changes or when other status needs to be 
 * communicated to the host application
 *
 * @param [in]
 *    Pointer to structure of type "GpsCallbacks"
 *
 * @return
 *    -1 if failed to open serial interface to modem's NMEA channel
 *     0 if successfully started the interface
 *
 * @note
 *   Called outside of the context of this package
 *
 *   If this entry was already called once before then subsequent 
 *   calls quietly return "success" with no further action unless
 *   the cleanup interface function has been called
 *
 */
int swigps_init(GpsCallbacks* callbacks)
{
    GpsState*  s = _gps_state;
    int i;

    D("Entered %s\n",__FUNCTION__ );

    if (!s->init) {
        /* Callback structure validation */
        if( swigps_checkGpsCallbacks( callbacks ) ) {
            LOGE("%s, error in callback structure", __FUNCTION__);
            return -1;
        }

        /* Copy the callbacks before the rest of the init */
        s->callbacks = *callbacks;

        gps_state_init(s);
    }

    return 0;
}

/**
 * 
 * Starts the GPS receiver obtaining fixes
 *
 * @return 
 *    -1 if this interface's (*init) function hasn't been called
 *     0 if the receiver successfully started 
 *
 * @note
 *   Called outside of the context of this package
 *
 */
int swigps_start(void)
{
    GpsState*  s = _gps_state;

    D("Entered %s\n",__FUNCTION__ );
    
    /* Must have called this interface's initialization 
     * entry before trying to start the GPS engine 
     */
    if (!s->init) {
        DFR("%s: called with uninitialized state !!", __FUNCTION__);
        return -1;
    }

    /* Can't do much without knowing which port the NMEA
     * traffic is on so return an error for now
     */
    if( !s->gpsnmeaport.initialized ) {
        DFR("%s: called before NMEA port was defined", __FUNCTION__);
        return -1;
    }
    gps_state_start(s);
    return 0;
}

/**
 * 
 * Stop sending fixes to the higher-layer applications. Terminates the
 * gps interface. The main thread for this package remains running
 * and will start the interface again if the (*start) function is 
 * called
 *
 * @return
 *   -1 if this interface's (*init) function hasn't been called
 *    0 success
 *
 * @note
 *   Called outside of the context of this package
 *
 */
int swigps_stop(void)
{
    GpsState*  s = _gps_state;

    D("Entered %s\n",__FUNCTION__ );

    if (!s->init) {
        DFR("%s: called with uninitialized state !!", __FUNCTION__);
        return -1;
    }

    gps_state_stop(s);
    return 0;
}

/**
 * 
 * Closes all opened resources, terminates active threads and 
 * terminates the GPS session. When done, there are no traces
 * of the interface left in the system
 *
 * @note
 *   Called outside of the context of this package
 *
 */
void swigps_cleanup(void)
{
    GpsState*  s = _gps_state;

    D("Entered %s\n",__FUNCTION__ );

    if (s->init)
        gps_state_done(s);
}

/**
 * 
 * Method to inject time into the GPS engine which can be used 
 * to provide a shorter time to first fix. 
 *
 * @param [in] time
 *   Elapsed time since Jan 1, 1970 in milliseconds. This is a 64 bit value
 * @param [in] timeReference
 *   a 64-bit value, Ignored by swigps
 * @param [in] uncertainty
 *   one half the round trip delay time between the NTP server and this 
 *   android device
 *
 * @note
 *   Called outside of the context of this package
 *
 */
int swigps_inject_time(GpsUtcTime time, int64_t timeReference, int uncertainty )
{
    GpsState*  s = _gps_state;

    D("Entered %s\n",__FUNCTION__ );

    if(!s->init) {
        D("%s: called uninitialized. Init state: %x\n", __FUNCTION__, s->init );
        return -1;
    }

    /* Store the request variables */
    s->injectTime.time = time;
    s->injectTime.uncertainty = uncertainty;

    /* Update the modem's programming */
    gps_state_injecttime( s );

    return 0;
}

/**
 * 
 * Method to inject a rough location into the GPS engine to assist
 * with reducing the time to first fix
 *
 */
int swigps_inject_location(double latitude, double longitute, float accuracy )
{
    // - TODO add to this as needed when gps-to-RIL communication path is defined
    D("Entered %s\n",__FUNCTION__ );
    return 0;
}

/**
 * 
 * A method to erase all stored aids for the interface which may
 * have been injected during a recent running of the interface.
 * 
 * @param
 *   flags [in] - Flags are defined in gps.h
 *
 * @note
 *   see also gps.h for specific flags and their values
 *
 */
void swigps_delete_aiding_data(GpsAidingData flags)
{
    // - TODO add to this as needed when gps-to-RIL communication path is defined
    D("Entered %s\n",__FUNCTION__ );
}

/**
 * gps_get_swi_hardware_interface
 *
 * Provides the main entry point to this package, used by the
 * Android infrastructure to obtain a pointer to the 
 * GpsInterface structure, also defined in this file. 
 *
 * @return
 *   Pointer to swiGpsInterface
 *
 * @note
 *   Called outside of the context of this package
 */
const GpsInterface* gps_get_swi_hardware_interface(void)
{
    DFR("Entered %s", __FUNCTION__);
    return &swiGpsInterface;
}


/*****                             *****
 ***** AGPS INTERFACE & FUNCTIONS  *****
 *****                             *****/

/**
 * 
 * Method to initialize the AGPS subsystem within the vendor-specific
 * GPS engine
 *
 */
void swiagps_init( AGpsCallbacks* callbacks )
{
    // - TODO add to this as needed when gps-to-RIL communication path is defined
    D("Entered %s\n",__FUNCTION__ );
}
/**
 * 
 * Method to indicate to the Vendor-specific GPS engine that the 
 * requested data connection has been opened
 *
 */
int swiagps_open( const char *apn )
{
    // - TODO add to this as needed when gps-to-RIL communication path is defined
    D("Entered %s\n",__FUNCTION__ );
    return 0;
}
/**
 * 
 * Method to indicate to the Vendor-specific GPS engine that the
 * data connection has been closed
 *
 */
int swiagps_closed( void )
{
    // - TODO add to this as needed when gps-to-RIL communication path is defined
    D("Entered %s\n",__FUNCTION__ );
    return 0;
}
/**
 * 
 * Method to indicate to the AGPS interface that a requested data
 * connection attempt has failed
 *
 */
int swiagps_failed( void )
{
    // - TODO add to this as needed when gps-to-RIL communication path is defined
    D("Entered %s\n",__FUNCTION__ );
    return 0;
}
/**
 * 
 * Method to set the AGPS server information - gets sent to the 
 * modem. The hostname string pointer can be either a valid URL
 * or an ascii-coded IP address which will be converted to 
 * binary by this function. 
 *
 */
int swiagps_set_server( AGpsType type, const char* hostname, int port )
{
    unsigned int ipaddr;
    int urllength;
    GpsState *s = _gps_state;

    D("%s: server URL: %s. Type: %d, Port: %d\n",
      __FUNCTION__, 
      hostname, 
      type,
      port );

    /* Currently we only support SUPL type. Initialization must be complete */
    if(!s->init || type != AGPS_TYPE_SUPL) {
        D("%s: swiagps_set_server error: init: %d, type: %d",
          __FUNCTION__, s->init, type );
        return -1;
    }

    /* Validate the URL's length before we get too far */
    if( (urllength = strlen( hostname )) > MAXURL ) {
        LOGE("swiagps_set_server: URL too long %d", urllength );
        return -1;
    }

    /* Always copy the hostname into the requrl string location */
    strncpy( s->agpsInfo.requrl, hostname, MAXURL );

    /* Input is an IP Address/port combination */
    if( !swigps_a2ip( SWIGPS_BE, hostname, &ipaddr ) ) {
        /* IP Addresses cannot be 0 */
        if( !ipaddr ) {
            LOGE("IP Address from URL is 0.0.0.0. Invalid");
            return -1;
        }
        s->agpsInfo.reqipaddr = ipaddr;
    }
    /* Input is a URL/Port combination */
    else {
        s->agpsInfo.reqipaddr = 0;
    }

    s->agpsInfo.reqtype = type;
    s->agpsInfo.reqport = port;

    /* Update the modem's programming */
    gps_state_agpsserver( s );

    D("Agps server setting request complete");
    
    return 0;
}

/** 
 * 
 * Get a pointer to extension information
 *
 * @param
 *   pname - Pointer to a name of the extension to return
 *
 * @return - Void pointer to the requested extension 
 *           or NULL if not available
 *
 * @note
 *   Called outside of the context of this package
 *
 */
const void* swigps_get_extension(const char* name)
{
    D("Entered %s with name %s\n",__FUNCTION__, name );

    /* If the caller is asking for the AGPS interface */
    if( strcmp( name, AGPS_INTERFACE ) == 0 ) {
        return &swiAgpsInterface;
    }

    return NULL;
}
