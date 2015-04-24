/**
 *
 * @ingroup swidm
 *
 * @file
 * SWI Dialogue Management Logging data and function types
 *
 * @author
 * Copyright: © 2011 Sierra Wireless, Inc.
 *            All rights reserved
 *
 */


#ifndef __SWIDM_H
#define __SWIDM_H

/* Linux definitions */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <stdbool.h>
#include "swims_ossdkuproto.h"

#ifdef ANDROID
#define  LOG_TAG  "swi_dm"
#include "swiril_log.h"
#else
#define LOGD printf
#endif

#define SWIDM_VERSION_STRING    "Sierra DM Log V1.0.1"
#define SWIDM_DEFAULT_LOG_FILE  "/data/swidmlog"

#define MAX_PATH 256
#define NETPORTDFLT 2500 /* default TCP port number for QPST*/

int open_dev (char *devp,
              struct termios *oldtiop,
              struct termios *newtiop);
int close_dev ( int devfd, struct termios *oldtiop );
              

#endif /*__SWIDM_H */

