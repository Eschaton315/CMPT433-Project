/*
 * Copyright 2018-21 CEVA, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License and 
 * any applicable agreements you may have with CEVA, Inc.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * I2C HAL for BNO08x
 */
#include <stdio.h>
#include "hal/i2c_hal.h"
#include "hal/sh2.h"
#include "hal/sh2_SensorValue.h"
#include "hal/sh2_err.h"

#define ADDR_SH2_0 (0x4A)
#define ADDR_SH2_1 (0x4B)
#define ADDR_BNODFU_0 (0x28)
#define ADDR_BNODFU_1 (0x29)

i2c_hal_t sh2_hal;
i2c_hal_t dfu_hal;
bool resetOccurred = false;

void printEvent(const sh2_SensorEvent_t * event);

void sensorHandler(sh2_SensorEvent_t *pEvent)
{
#ifdef DSF_OUTPUT
    printDsf(pEvent);
#else
    printEvent(pEvent);
#endif
}

void eventHandler(sh2_AsyncEvent_t *pEvent)
{
    // If we see a reset, set a flag so that sensors will be reconfigured.
    if (pEvent->eventId == SH2_RESET) {
        resetOccurred = true;
    }
    else if (pEvent->eventId == SH2_SHTP_EVENT) {
        printf("EventHandler  id:SHTP, %d\n", pEvent->shtpEvent);
    }
    else if (pEvent->eventId == SH2_GET_FEATURE_RESP) {
        // printf("EventHandler Sensor Config, %d\n", pEvent->sh2SensorConfigResp.sensorId);
    }
    else {
        printf("EventHandler, unknown event Id: %d\n", pEvent->eventId);
    }
}

void printEvent(const sh2_SensorEvent_t * event)
{
    int rc;
    sh2_SensorValue_t value;
    float scaleRadToDeg = 180.0 / 3.14159265358;
    float r, i, j, k, acc_deg, x, y, z;
    float t;
    static int skip = 0;

    rc = sh2_decodeSensorEvent(&value, event);
    if (rc != SH2_OK) {
        printf("Error decoding sensor event: %d\n", rc);
        return;
    }

    t = value.timestamp / 1000000.0;  // time in seconds.
    switch (value.sensorId) {
        case SH2_RAW_ACCELEROMETER:
            printf("%8.4f Raw acc: %d %d %d time_us:%d\n",
                   (double)t,
                   value.un.rawAccelerometer.x,
                   value.un.rawAccelerometer.y,
                   value.un.rawAccelerometer.z,
                   value.un.rawAccelerometer.timestamp);
            break;

        case SH2_ACCELEROMETER:
            printf("%8.4f Acc: %f %f %f\n",
                   (double)t,
                   (double)value.un.accelerometer.x,
                   (double)value.un.accelerometer.y,
                   (double)value.un.accelerometer.z);
            break;
            
        case SH2_RAW_GYROSCOPE:
            printf("%8.4f Raw gyro: x:%d y:%d z:%d temp:%d time_us:%d\n",
                   (double)t,
                   value.un.rawGyroscope.x,
                   value.un.rawGyroscope.y,
                   value.un.rawGyroscope.z,
                   value.un.rawGyroscope.temperature,
                   value.un.rawGyroscope.timestamp);
            break;
            
        case SH2_ROTATION_VECTOR:
            r = value.un.rotationVector.real;
            i = value.un.rotationVector.i;
            j = value.un.rotationVector.j;
            k = value.un.rotationVector.k;
            acc_deg = scaleRadToDeg * 
                value.un.rotationVector.accuracy;
            printf("%8.4f Rotation Vector: "
                   "r:%0.6f i:%0.6f j:%0.6f k:%0.6f (acc: %0.6f deg)\n",
                   (double)t,
                   (double)r, (double)i, (double)j, (double)k, (double)acc_deg);
            break;
        case SH2_GAME_ROTATION_VECTOR:
            r = value.un.gameRotationVector.real;
            i = value.un.gameRotationVector.i;
            j = value.un.gameRotationVector.j;
            k = value.un.gameRotationVector.k;
            printf("%8.4f GRV: "
                   "r:%0.6f i:%0.6f j:%0.6f k:%0.6f\n",
                   (double)t,
                   (double)r, (double)i, (double)j, (double)k);
            break;
        case SH2_GYROSCOPE_CALIBRATED:
            x = value.un.gyroscope.x;
            y = value.un.gyroscope.y;
            z = value.un.gyroscope.z;
            printf("%8.4f GYRO: "
                   "x:%0.6f y:%0.6f z:%0.6f\n",
                   (double)t,
                   (double)x, (double)y, (double)z);
            break;
        case SH2_GYROSCOPE_UNCALIBRATED:
            x = value.un.gyroscopeUncal.x;
            y = value.un.gyroscopeUncal.y;
            z = value.un.gyroscopeUncal.z;
            printf("%8.4f GYRO_UNCAL: "
                   "x:%0.6f y:%0.6f z:%0.6f\n",
                   (double)t,
                   (double)x, (double)y, (double)z);
            break;
        case SH2_GYRO_INTEGRATED_RV:
            // These come at 1kHz, too fast to print all of them.
            // So only print every 10th one
            skip++;
            if (skip == 10) {
                skip = 0;
                r = value.un.gyroIntegratedRV.real;
                i = value.un.gyroIntegratedRV.i;
                j = value.un.gyroIntegratedRV.j;
                k = value.un.gyroIntegratedRV.k;
                x = value.un.gyroIntegratedRV.angVelX;
                y = value.un.gyroIntegratedRV.angVelY;
                z = value.un.gyroIntegratedRV.angVelZ;
                printf("%8.4f Gyro Integrated RV: "
                       "r:%0.6f i:%0.6f j:%0.6f k:%0.6f x:%0.6f y:%0.6f z:%0.6f\n",
                       (double)t,
                       (double)r, (double)i, (double)j, (double)k,
                       (double)x, (double)y, (double)z);
            }
            break;
        case SH2_IZRO_MOTION_REQUEST:
            printf("IZRO Request: intent:%d, request:%d\n",
                   value.un.izroRequest.intent,
                   value.un.izroRequest.request);
            break;
            /*
        case SH2_SHAKE_DETECTOR:
            printf("Shake Axis: %c%c%c\n", 
                   (value.un.shakeDetector.shake & SHAKE_X) ? 'X' : '.',
                   (value.un.shakeDetector.shake & SHAKE_Y) ? 'Y' : '.',
                   (value.un.shakeDetector.shake & SHAKE_Z) ? 'Z' : '.');

            break;
            */
        case SH2_STABILITY_CLASSIFIER:
            printf("Stability Classification: %d\n",
                   value.un.stabilityClassifier.classification);
            break;
        case SH2_STABILITY_DETECTOR:
            printf("Stability Detector: %d\n",
                   value.un.stabilityDetector.stability);
            break;
        default:
            printf("Unknown sensor: %d\n", value.sensorId);
            break;
    }
}

sh2_Hal_t *sh2_hal_init(void)
{
    // Get instance of i2c hal suitable for SH2 on BNO08x
    return shtp_i2c_hal_init(&sh2_hal, false, ADDR_SH2_0);
}

sh2_Hal_t *dfu_hal_init(void)
{
    // Get instance of i2c hal suitable for SH2 on BNO08x
    return bno_dfu_i2c_hal_init(&sh2_hal, true, ADDR_BNODFU_0);
}
