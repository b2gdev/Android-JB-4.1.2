/*
 * Copyright (C) 2008 The Android Open Source Project
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
#include <hardware_legacy/vibrator.h>
#include "../../../../kernel/drivers/misc/twl3040_vib/twl3040_vib_ioctl.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/ioctl.h>

#define  LOG_TAG  "vibrator"
#include <cutils/log.h>
#include <cutils/sockets.h>

#define ZONE_DB 0
#if ZONE_DB
#  define  ZI(...)   ALOGI(__VA_ARGS__)
#else
#  define  ZI(...)   ((void)0)
#endif
#  define  ZE(...)   ALOGE(__VA_ARGS__)

#define THE_DEVICE "/dev/vibrator"

int vibrator_exists()
{
    int fd;
	ZI("vibrator_exists\n");	

    fd = open(THE_DEVICE, O_RDWR);
    if(fd < 0){
		ZE("Failed to open device /dev/vibrator \r\n");
        return 0;
	}
        	
    close(fd);
    return 1;
}

int vibrator_on(int timeout_ms)
{
    
    unsigned long ltimeout_ms = 0;				
	int ret, fd = 0;
	
    if(timeout_ms < 0){
		ZE("timeout_ms is negative!!\r\n");
        return -1;
	}    
            
    fd = open(THE_DEVICE, O_RDWR);
    if(fd < 0){
		ZE("Failed to open device /dev/vibrator \r\n");
        return -1;
	}
	
	ltimeout_ms = ((unsigned long)timeout_ms);
    ZI("vibrator_on: timeout=%d\n",ltimeout_ms);    
    
    ret = ioctl(fd, TWL3040_VIBRATOR_ON_WITH_TIMEOUT, ltimeout_ms);
	if ( ret < 0 ) {
		ZE("Ioctl failed. Error: %s\r\n", strerror(errno));
		ret = -1;
	}	
	
	close(fd);
								
    return ret;
}

int vibrator_off()
{
	int ret, fd = 0;
	
	ZI("vibrator_off\n");
	
	fd = open(THE_DEVICE, O_RDWR);
    if(fd < 0){
		ZE("Failed to open device /dev/vibrator \r\n");
        return -1;
	}
	
	ret = ioctl(fd, TWL3040_VIBRATORL_OFF, NULL) ;
	if ( ret < 0 ) {
		ZE("Ioctl failed. Error: %s\r\n", strerror(errno));
		ret = -1;
	}
	
	close(fd);
	
    return ret;
}
