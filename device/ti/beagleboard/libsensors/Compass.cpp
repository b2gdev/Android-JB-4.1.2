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

#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <poll.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/select.h>
#include <cutils/log.h>

#include "Compass.h"

#include "../../../../kernel/include/linux/i2c/hmc5883l_ioctl.h"

#define FETCH_FULL_EVENT_BEFORE_RETURN 1

// The offset required to componsate for the addition magnetic flux inside the enclosure in uT
static const float enclosure_offsets_y = 59.0f;
static const float enclosure_offsets_x = -15.0f;
static const float enclosure_offsets_z = 74.50f;

/*****************************************************************************/

Compass::Compass()
    : SensorBase(NULL, "compass"),
    mEnabled(0),
     mInputReader(4),
      mHasPendingEvent(false)
{
    mPendingEvent.version = sizeof(sensors_event_t);
    mPendingEvent.sensor = ID_M;
    mPendingEvent.type = SENSOR_TYPE_MAGNETIC_FIELD;
    memset(mPendingEvent.data, 0, sizeof(mPendingEvent.data));

   if (data_fd) {
        strcpy(input_sysfs_path, "/sys/bus/i2c/drivers/hmc5883l/2-001e/");
        input_sysfs_path_len = strlen(input_sysfs_path);
        enable(0, 1);
    }
}

Compass::~Compass() {
	if (mEnabled) {
        enable(0, 0);
    }    
}

int Compass::setInitialState() {
   /*if(dev_fd)
   {
	  ALOGI("CompassSensor: setInitialState");
      ioctl(dev_fd,HMC5883L_SET_TO_DEFAULTS,NULL);
   }*/
    return 0;
}

int Compass::enable(int32_t, int en) {
    //return 0;
    int flags = en ? 1 : 0;    
    if (flags != mEnabled) {
		ALOGI("compass: enable:%d",flags);
        int fd;
        strcpy(&input_sysfs_path[input_sysfs_path_len], "enable");
        fd = open(input_sysfs_path, O_RDWR);
        if (fd >= 0) {
            char buf[1];
            int err;
            if (flags) {
                buf[0] = '1';
            } else {
                buf[0] = '0';
            }
            err = write(fd, buf, sizeof(buf));
            close(fd);
            mEnabled = flags;
            setInitialState();
            return 0;
        }
        ALOGE("compass: enable failed to open: %s",input_sysfs_path);
        return -1;
    }
    return 0;
}

bool Compass::hasPendingEvents() const {
    return mHasPendingEvent;
}

int Compass::setDelay(int32_t handle, int64_t delay_ns)
{
    int fd;
    //struct hmc5883l_configs user_configs;
  
  /*  if(dev_fd)
   {
	  user_configs.flag = DATA_RATE;
	  ALOGI("CompassSensor: delay_ns=%d", delay_ns);
                    
	  if(delay_ns <= 14000 )
		user_configs.data_rate = DATA_RATE_75_00_HZ;
	else if(delay_ns <= 34000 )
		user_configs.data_rate = DATA_RATE_30_00_HZ;
	else if(delay_ns <= 67000 )
		user_configs.data_rate = DATA_RATE_15_00_HZ;
	else if(delay_ns <= 134000 )
		user_configs.data_rate = DATA_RATE_7_50_HZ;
	else if(delay_ns <= 334000 )
		user_configs.data_rate = DATA_RATE_3_00_HZ;
	else if(delay_ns <= 667000 )
		user_configs.data_rate = DATA_RATE_1_50_HZ;
	else
		user_configs.data_rate = DATA_RATE_0_75_HZ;
		
      ioctl(dev_fd,HMC5883L_SET_CONFIGS,&user_configs);
   }
   */
    //return 0;
    
    ALOGI("compass: delay_ns:%d",(int32_t)delay_ns);
        
    strcpy(&input_sysfs_path[input_sysfs_path_len], "pollrate_ms");
    fd = open(input_sysfs_path, O_RDWR);
    if (fd >= 0) {
        char buf[80];
        int32_t delay_ms;
        	 
        	  if(delay_ns <= 14000 )
		delay_ms = DATA_RATE_75_00_HZ;
	else if(delay_ns <= 34000 )
		delay_ms = DATA_RATE_30_00_HZ;
	else if(delay_ns <= 67000 )
		delay_ms = DATA_RATE_15_00_HZ;
	else if(delay_ns <= 134000 )
		delay_ms = DATA_RATE_7_50_HZ;
	else if(delay_ns <= 334000 )
		delay_ms = DATA_RATE_3_00_HZ;
	else if(delay_ns <= 667000 )
		delay_ms = DATA_RATE_1_50_HZ;
	else
		delay_ms = DATA_RATE_0_75_HZ;
		
        sprintf(buf, "%d", delay_ms);
        write(fd, buf, strlen(buf)+1);
        close(fd);
        return 0;
    }
    ALOGE("compass: setDelay failed to open: %s",input_sysfs_path);
    return -1;
}

// TWO's complement converter
float TwosComplmentToDecimal(unsigned short complemntValue)
{
   bool negativeFlag = false;
   float value = 0x00;
   unsigned short temValue = 0x00;

  
   negativeFlag = ((complemntValue & 0xF800)&& 0xF800);
  

   if(negativeFlag){
      temValue = (unsigned short)((~complemntValue)+1);
      value = (-temValue);
   }else{
      value = 0x7ff & complemntValue;
   }
return value;
}

int Compass::readEvents(sensors_event_t* data, int count)
{
    if (count < 1)
        return -EINVAL;

    if (mHasPendingEvent) {
        mHasPendingEvent = false;
        mPendingEvent.timestamp = getTimestamp();
        *data = mPendingEvent;
        return mEnabled ? 1 : 0;
    }

    ssize_t n = mInputReader.fill(data_fd);
    if (n < 0)
        return n;

    int numEventReceived = 0;
    input_event const* event;

    while (count && mInputReader.readEvent(&event)) {
        int type = event->type;
        if (type == EV_ABS) {
            float value = event->value;
            if (event->code == EVENT_TYPE_ACCEL_Y) {
                mPendingEvent.data[0] = ( ( TwosComplmentToDecimal(value) * CONVERT_M_Y ) + enclosure_offsets_x );
            } else if (event->code == EVENT_TYPE_ACCEL_X) {
                mPendingEvent.data[1] = ( ( TwosComplmentToDecimal(value) * CONVERT_M_X ) + enclosure_offsets_y );
            } else if (event->code == EVENT_TYPE_ACCEL_Z) {
                mPendingEvent.data[2] = ( ( TwosComplmentToDecimal(value) * CONVERT_M_Z ) + enclosure_offsets_z );
            }
        } else if (type == EV_SYN) {
            mPendingEvent.timestamp = timevalToNano(event->time);
            if (mEnabled) {
                *data++ = mPendingEvent;
                count--;
                numEventReceived++;
                //ALOGE("compass: Reading (y=%.2f(%0.2f), x=%.2f(%0.2f), z=%.2f(%0.2f))",
                //    mPendingEvent.data[1], enclosure_offsets_y, mPendingEvent.data[0], enclosure_offsets_x, 
                //    mPendingEvent.data[2], enclosure_offsets_z);		
			}
        } else {
            ALOGE("compass: unknown event (type=%d, code=%d)",
                    type, event->code);
        }
        mInputReader.next();
    }

    return numEventReceived;
}

