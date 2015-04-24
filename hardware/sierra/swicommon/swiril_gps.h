/**
 *
 * @ingroup swiril
 *
 * @file 
 * Vendor-specific GPS definitions exclusively for the swiril_gps side.
 *  
 * @author
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 * @note
 *     See also swigps_common.h for an explanation of naming conventions
 *     used by the vendor specific GPS code
 */

/* Includes */
#include "swigps_common.h"

/* Definitions */

/* External declarations */
extern void handle_swigps( int fd );
extern void RILGPS_Init( GPS_RILTYPE riltype );
extern void initializeRILType( GPS_RILTYPE riltype );
extern void rilgpsGetDef( void * );
extern void rilgpsSetDef( void * );
extern void rilgpsGetAgps( void * );
extern void rilgpsSetAgps( void * );
extern void rilgpsInjTime( void * );
extern void rilgpsStopSession( void * );
extern void rilgpsStartSession( void *paramp );
