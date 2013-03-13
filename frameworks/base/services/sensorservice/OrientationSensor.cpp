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

#include <stdint.h>
#include <math.h>
#include <sys/types.h>

#include <utils/Errors.h>

#include <hardware/sensors.h>

#include "OrientationSensor.h"

namespace android {
// ---------------------------------------------------------------------------

OrientationSensor::OrientationSensor(sensor_t const* list, size_t count)
    : mSensorDevice(SensorDevice::getInstance()),
      mALowPass(M_SQRT1_2, 1.5f),
      mAX(mALowPass), mAY(mALowPass), mAZ(mALowPass),
      mMLowPass(M_SQRT1_2, 1.5f),
      mMX(mMLowPass), mMY(mMLowPass), mMZ(mMLowPass)
{
    for (size_t i=0 ; i<count ; i++) {
        if (list[i].type == SENSOR_TYPE_ACCELEROMETER) {
            mAcc = Sensor(list + i);
        }
        if (list[i].type == SENSOR_TYPE_MAGNETIC_FIELD) {
            mMag = Sensor(list + i);
        }
    }
    memset(mMagData, 0, sizeof(mMagData));
}

bool OrientationSensor::process(sensors_event_t* outEvent,
        const sensors_event_t& event)
{
    const static double NS2S = 1.0 / 1000000000.0;
    const float rad2deg = 180 / M_PI;
    float values[3];
    
    if (event.type == SENSOR_TYPE_MAGNETIC_FIELD) {
        const double now = event.timestamp * NS2S;
        if (mMagTime == 0) {
            mMagData[0] = mMX.init(event.magnetic.x);
            mMagData[1] = mMY.init(event.magnetic.y);
            mMagData[2] = mMZ.init(event.magnetic.z);
        } else {
            double dT = now - mMagTime;
            mMLowPass.setSamplingPeriod(dT);
            mMagData[0] = mMX(event.magnetic.x);
            mMagData[1] = mMY(event.magnetic.y);
            mMagData[2] = mMZ(event.magnetic.z);
        }
        mMagTime = now;
    }
    if (event.type == SENSOR_TYPE_ACCELEROMETER) {
        const double now = event.timestamp * NS2S;
        float Ax, Ay, Az;
        if (mAccTime == 0) {
            Ax = mAX.init(event.acceleration.x);
            Ay = mAY.init(event.acceleration.y);
            Az = mAZ.init(event.acceleration.z);
        } else {
            double dT = now - mAccTime;
            mALowPass.setSamplingPeriod(dT);
            Ax = mAX(event.acceleration.x);
            Ay = mAY(event.acceleration.y);
            Az = mAZ(event.acceleration.z);
        }
        mAccTime = now;
        const float Ex = mMagData[0];
        const float Ey = mMagData[1];
        const float Ez = mMagData[2];
        float Hx = Ey*Az - Ez*Ay;
        float Hy = Ez*Ax - Ex*Az;
        float Hz = Ex*Ay - Ey*Ax;
        const float normH = sqrtf(Hx*Hx + Hy*Hy + Hz*Hz);
        if (normH < 0.1f) {
            // device is close to free fall (or in space?), or close to
            // magnetic north pole. Typical values are  > 100.
            return false;
        }
        const float invH = 1.0f / normH;
        const float invA = 1.0f / sqrtf(Ax*Ax + Ay*Ay + Az*Az);
        Hx *= invH;
        Hy *= invH;
        Hz *= invH;
        Ax *= invA;
        Ay *= invA;
        Az *= invA;
        const float Mx = Ay*Hz - Az*Hy;
        const float My = Az*Hx - Ax*Hz;
        const float Mz = Ax*Hy - Ay*Hx;

		values[0] = atan2f(Hy, My)* rad2deg;
		values[1] = asinf(-Ay)* rad2deg;
		values[2] = atan2f(-Ax, Az)* rad2deg * (-1);          			

		*outEvent = event;
		outEvent->orientation.azimuth = values[0];
		outEvent->orientation.pitch   = values[1];
		outEvent->orientation.roll    = values[2];
		outEvent->orientation.status  = SENSOR_STATUS_ACCURACY_HIGH;
		outEvent->sensor = '_ypr';
		outEvent->type = SENSOR_TYPE_ORIENTATION;
                    
        return true;
    }
    return false;
}

status_t OrientationSensor::activate(void* ident, bool enabled) {
    mSensorDevice.activate(this, mAcc.getHandle(), enabled);
    mSensorDevice.activate(this, mMag.getHandle(), enabled);
    if (enabled) {
        mMagTime = 0;
        mAccTime = 0;
    }
    return NO_ERROR;
}

status_t OrientationSensor::setDelay(void* ident, int handle, int64_t ns)
{
    mSensorDevice.setDelay(this, mAcc.getHandle(), ns);
    mSensorDevice.setDelay(this, mMag.getHandle(), ns);
    return NO_ERROR;
}

Sensor OrientationSensor::getSensor() const {
    sensor_t hwSensor;
    hwSensor.name       = "Orientation Sensor";
    hwSensor.vendor     = "Google Inc.";
    hwSensor.version    = 1;
    hwSensor.handle     = '_ypr';   
    hwSensor.type       = SENSOR_TYPE_ORIENTATION;
	hwSensor.maxRange   = 360.0f;
    hwSensor.resolution = 1.0f/256.0f;
    hwSensor.power      = mAcc.getPowerUsage() + mMag.getPowerUsage();
    hwSensor.minDelay   = mAcc.getMinDelay();
    Sensor sensor(&hwSensor);
    return sensor;
}

// ---------------------------------------------------------------------------
}; // namespace android

